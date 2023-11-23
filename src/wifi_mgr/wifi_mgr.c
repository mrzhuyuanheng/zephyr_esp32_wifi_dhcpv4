/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdio.h>
#include <zephyr/kernel.h>

#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wifi, LOG_LEVEL_DBG);

K_SEM_DEFINE(wifi_sem, 0, 1);
K_SEM_DEFINE(dhcp_sem, 0, 1);

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback dhcp_cb;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
    const struct wifi_status *status = (const struct wifi_status *)cb->info;

    if (status->status)
    {
        LOG_ERR("Connection request failed (%d)", status->status);
    }
    else
    {
        LOG_INF("Wifi Connected");
        k_sem_give(&wifi_sem);
    }
}

static void wifi_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    switch (mgmt_event)
    {

        case NET_EVENT_WIFI_CONNECT_RESULT:
			handle_wifi_connect_result(cb);
            break;

        case NET_EVENT_WIFI_DISCONNECT_RESULT:
            handle_wifi_connect_result(cb);
            break;

        default:
            break;
    }
}

static void wifi_connect(const char *ssid, const char *password)
{
	net_mgmt_init_event_callback(&wifi_cb, wifi_handler,
								NET_EVENT_WIFI_CONNECT_RESULT | 
                                NET_EVENT_WIFI_DISCONNECT_RESULT);
	net_mgmt_add_event_callback(&wifi_cb);

    /* it takes too long time to start esp32 wifi, make sure esp32 started before connect */
    k_msleep(5000); 

    struct net_if *iface = net_if_get_default();

    struct wifi_connect_req_params wifi_params = {0};

    wifi_params.ssid = ssid;
    wifi_params.psk = password;
    wifi_params.ssid_length = strlen(ssid);
    wifi_params.psk_length = strlen(password);
    wifi_params.channel = WIFI_CHANNEL_ANY;
    wifi_params.security = WIFI_SECURITY_TYPE_PSK;
    wifi_params.band = WIFI_FREQ_BAND_2_4_GHZ; 
    wifi_params.mfp = WIFI_MFP_OPTIONAL;

    LOG_INF("Connecting to SSID: %s", wifi_params.ssid);

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(struct wifi_connect_req_params)))
    {
        LOG_INF("WiFi Connection Request Failed");
    }

    k_sem_take(&wifi_sem, K_FOREVER);
}

static void dhcp_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
	static char buf[NET_IPV4_ADDR_LEN];

	if (mgmt_event != NET_EVENT_IPV4_ADDR_ADD) {
		return;
	}

    LOG_INF("Your address: %s",
        net_addr_ntop(AF_INET, &iface->config.dhcpv4.requested_ip, buf, sizeof(buf)));
    LOG_INF("    Subnet[%d]: %s", net_if_get_by_iface(iface),
        net_addr_ntop(AF_INET, &iface->config.ip.ipv4->netmask, buf, sizeof(buf)));
    LOG_INF("    Router[%d]: %s", net_if_get_by_iface(iface),
        net_addr_ntop(AF_INET, &iface->config.ip.ipv4->gw, buf, sizeof(buf)));
    LOG_INF("Lease time[%d]: %u seconds", net_if_get_by_iface(iface),iface->config.dhcpv4.lease_time);

    k_sem_give(&dhcp_sem);
}

static void dhcp_start(void)
{
	// net_mgmt_init_event_callback(&dhcp_cb, dhcp_handler, NET_EVENT_IPV4_DHCP_BOUND | NET_EVENT_IPV4_ADDR_ADD);
	// net_mgmt_add_event_callback(&dhcp_cb);

    // struct net_if *iface = net_if_get_default();
    // if(!iface){
    //     LOG_ERR("wifi interface not available");
    // }
    // net_dhcpv4_start(iface);

    k_sem_take(&dhcp_sem, K_FOREVER);
}

void wifi_init(const char *ssid, const char *password)
{
	net_mgmt_init_event_callback(&dhcp_cb, dhcp_handler, NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&dhcp_cb);

    wifi_connect(ssid, password);
    dhcp_start();
}
/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/posix/time.h>
#include <zephyr/net/sntp.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wifi_net, LOG_LEVEL_DBG);

#include "wifi_mgr.h"

#define SNTP_SERVER             "0.pool.ntp.org"
#define SNTP_RETRY_TIME         (5)

static uint64_t do_sntp(void)
{
	int rc;
	struct sntp_time sntp_time;
	char time_str[sizeof("1970-01-01T00:00:00")];

    int cnt = 0;
    do {
        LOG_INF("Sending NTP request for current time:");
        rc = sntp_simple(SNTP_SERVER, SYS_FOREVER_MS, &sntp_time);
        if (rc == 0) {
            time_t now = sntp_time.seconds;
            struct tm now_tm;

            gmtime_r(&now, &now_tm);
            strftime(time_str, sizeof(time_str), "%FT%T", &now_tm);
            LOG_INF("  Acquired time: %s", time_str);
            LOG_INF("  sec: %lld", sntp_time.seconds);
            break;
        } else {
            LOG_ERR("  Failed to acquire SNTP, code %d,  cnt:%d\n", rc, cnt);
        }
        cnt++;
    } while (cnt < SNTP_RETRY_TIME);

    if (rc == 0) {
        return sntp_time.seconds;
    } else {
        return -1;
    }
}

int main(void)
{
	LOG_INF("hello world, %s", CONFIG_BOARD);
	// wifi_init("TreeNewbear", "csk_wifi_connect");
	wifi_init(CONFIG_EXAMPLE_WIFI_SSID, CONFIG_EXAMPLE_WIFI_PASSWORD);

	do_sntp();

	while(1){
		k_msleep(10000);
		LOG_INF("Hello world");
	}

	return 0;
}

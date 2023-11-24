
Overview
********

This application starts a use esp32 wifi as station.

- first connect to wifi ap(Kconfig use CONFIG_EXAMPLE_WIFI_SSID and CONFIG_EXAMPLE_WIFI_PASSWORD)

- auto start DHCPv4

- then get ntp time

- finally print `Hello world`

Requirements
************

- esp32_devkitc_wroom board

Building and Running
********************

.. code-block:: shell
    west blobs fetch hal_espressif          ## only once
    west build -p -b esp32_devkitc_wroom    ## build
    west build -t menuconfig                ## can use it to config ssid and password
    west flash                              ## download
    west espressif monitor                  ## monitor log
    

Running DHCPv4 client in Linux Host
===================================

.. code-block:: console

    *** Booting Zephyr OS build zephyr-v3.5.0-2091-g0e11bcf5a0e7 ***
    [00:00:02.302,000] <inf> wifi_net: hello world, esp32_devkitc_wroom
    [00:00:07.302,000] <inf> wifi: Connecting to SSID: 707
    [00:00:09.597,000] <inf> net_dhcpv4: Received: 192.168.3.49
    [00:00:09.597,000] <inf> wifi: Wifi Connected
    [00:00:09.597,000] <inf> wifi_net: Sending NTP request for current time:
    [00:00:09.944,000] <inf> wifi_net:   Acquired time: 2023-11-24T16:24:39
    [00:00:09.944,000] <inf> wifi_net:   sec: 1700843079
    [00:00:19.944,000] <inf> wifi_net: Hello world
    [00:00:29.944,000] <inf> wifi_net: Hello world


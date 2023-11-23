# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/yhzhu/zephyrproject/modules/hal/espressif/components/bootloader/subproject"
  "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf/build/bootloader"
  "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf"
  "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf/tmp"
  "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf/src/EspIdfBootloader-stamp"
  "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf/src"
  "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf/src/EspIdfBootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf/src/EspIdfBootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/yhzhu/zephyrproject/dhcpv4_client/build/esp-idf/src/EspIdfBootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()

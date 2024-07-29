# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/nastyst16/.pico-sdk/sdk/1.5.1/tools/pioasm"
  "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pioasm"
  "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/nastyst16/Desktop/scoala_vara_tranca/HomeAutomation/HomeAutomation/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()

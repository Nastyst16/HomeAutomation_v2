cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HomeAutomation C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(dht)

add_executable(HomeAutomation HomeAutomation.c)

pico_enable_stdio_uart(HomeAutomation 1)
pico_enable_stdio_usb(HomeAutomation 1)

pico_add_extra_outputs(HomeAutomation)

target_compile_options(HomeAutomation PRIVATE -Wall)

target_link_libraries(HomeAutomation dht pico_stdlib)

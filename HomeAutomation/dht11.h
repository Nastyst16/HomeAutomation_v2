#ifndef DHT11_H
#define DHT11_H

#include "pico/stdlib.h"

// Error codes
#define DHTLIB_OK 0
#define DHTLIB_ERROR_CHECKSUM -1
#define DHTLIB_ERROR_TIMEOUT -2

typedef struct {
    int humidity;
    int temperature;
} dht11_data_t;

int dht11_read(uint gpio_pin, dht11_data_t *data);

#endif

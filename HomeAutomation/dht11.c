#include "dht11.h"
#include <stdio.h>

int dht11_read(uint gpio_pin, dht11_data_t *data) {
    uint8_t bits[5] = {0};
    uint8_t cnt = 7;
    uint8_t idx = 0;

    // Request sample
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_OUT);
    gpio_put(gpio_pin, 0);
    sleep_ms(18);
    gpio_put(gpio_pin, 1);
    sleep_us(30);
    gpio_set_dir(gpio_pin, GPIO_IN);

    // Acknowledge or timeout
    unsigned int loopCnt = 10000;
    while (gpio_get(gpio_pin) == 0) {
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
        sleep_us(100);
    }

    loopCnt = 10000;
    while (gpio_get(gpio_pin) == 1) {
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
        sleep_us(100);
    }

    // Read output - 40 bits => 5 bytes or timeout
    for (int i = 0; i < 40; i++) {
        loopCnt = 10000;
        while (gpio_get(gpio_pin) == 0) {
            if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
            sleep_us(100);
        }

        unsigned long t = time_us_32();

        loopCnt = 10000;
        while (gpio_get(gpio_pin) == 1) {
            if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
            sleep_us(100);
        }

        if ((time_us_32() - t) > 0) bits[idx] |= (1 << cnt);
        if (cnt == 0) {
            cnt = 7;
            idx++;
        } else {
            cnt--;
        }
    }

    data->humidity = bits[0];
    data->temperature = bits[2];

    uint8_t sum = bits[0] + bits[2];

    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
    return DHTLIB_OK;
}

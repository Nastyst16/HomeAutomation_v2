/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <dht.h>
#include <pico/stdlib.h>
#include <stdio.h>    

// change this to match your setup
static const dht_model_t DHT_MODEL = DHT11;
static const uint DATA_PIN = 14;

#define LED_PIN 16     
            
static float celsius_to_fahrenheit(float temperature) {
    return temperature * (9.0f / 5) + 32;
}
           
int main() {        
    stdio_init_all();
    // puts("\nDHT test");

    // Initialize the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // LED off initially


    printf("Begin Home Automation\n");

    dht_t dht;
    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, true /* pull_up */);
    do {

        // Check if there are characters available from USB serial
        char ch = getchar_timeout_us(0);  // Non-blocking read with 0 timeout

        if (ch != PICO_ERROR_TIMEOUT) {

            // Control the LED based on the command received
            if (ch == '1') {
                gpio_put(LED_PIN, 1); // Turn on the LED
                printf("Received: %c\n", ch);
            } else if (ch == '0') {
                gpio_put(LED_PIN, 0); // Turn off the LED
                printf("Received: %c\n", ch);
            }

        }


        dht_start_measurement(&dht);
        
        float humidity;
        float temperature_c;
        dht_result_t result = dht_finish_measurement_blocking(&dht, &humidity, &temperature_c);
        if (result == DHT_RESULT_OK) {
            printf("Temp:%.1f C (%.1f F), Hum:%.1f%% humidity\n", temperature_c, celsius_to_fahrenheit(temperature_c), humidity);
        } else if (result == DHT_RESULT_TIMEOUT) {
            puts("DHT sensor not responding. Please check your wiring.");
        } else {
            assert(result == DHT_RESULT_BAD_CHECKSUM);
            puts("Bad checksum");
        }

        sleep_ms(500);
    } while (true);
}

/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <dht.h>
#include <pico/stdlib.h>
#include <stdio.h>   
#include "hardware/gpio.h"
#include "hardware/timer.h" 

// change this to match your setup
static const dht_model_t DHT_MODEL = DHT11;
static const uint DATA_PIN = 12;

#define LED_PIN 16   
#define LED_PIN_HUM 19

#define BUZZER_PIN 8

static float celsius_to_fahrenheit(float temperature) {
    return temperature * (9.0f / 5) + 32;
}


#define TRIG_PIN 3       
#define ECHO_PIN 2                

// Funcție pentru măsurarea distanței
float measure_distance() {
    // Trimite un semnal de trigger
    gpio_put(TRIG_PIN, 0);
    sleep_us(2);
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // initializare buzzer pin
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    // Așteaptă semnalul de echo
    while (!gpio_get(ECHO_PIN)) {
        // Așteaptă semnalul de echo să înceapă
    }
    
    uint32_t start_time = time_us_32();

    // Așteaptă sfârșitul semnalului de echo
    while (gpio_get(ECHO_PIN)) {
        if (time_us_32() - start_time > 30000) { // Timeout de 30 ms
            return -1; // Eroare: timp de răspuns prea lung
        }
    }
    
    uint32_t end_time = time_us_32();
    
    // Calculăm durata semnalului de echo în microsecunde
    uint32_t pulse_duration = end_time - start_time;

    // Verificăm dacă timpul este valid
    if (pulse_duration == 0) {
        return -1; // Eroare: durata pulsului este 0
    }
    
    // Calculăm distanța în centimetri (viteză sunet aproximativ 34300 cm/s)
    // Formula: distanță (cm) = (durata_pulse / 2) * viteza_sunet / 10000
    float distance = (pulse_duration / 2.0) * (34300.0 / 1000000.0);
    
    return distance;
}
           
int main() {        
    stdio_init_all();
    // puts("\nDHT test");

    // Initialize the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // LED off initially

    gpio_init(LED_PIN_HUM);
    gpio_set_dir(LED_PIN_HUM, GPIO_OUT);
    gpio_put(LED_PIN_HUM, 0); // LED off initially

    // Initialize the trigger pin
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);



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
            if (humidity > 60) {
                gpio_put(LED_PIN_HUM, 1); // Turn on the LED
            } else {
                gpio_put(LED_PIN_HUM, 0); // Turn off the LED
            }
        } else if (result == DHT_RESULT_TIMEOUT) {
            puts("DHT sensor not responding. Please check your wiring.");
        } else {
            assert(result == DHT_RESULT_BAD_CHECKSUM);
            puts("Bad checksum");
        }


        // // Măsoară distanța și afișează rezultatul
        float distance = measure_distance();
        if (distance < 0) {
            printf("Eroare la măsurarea distanței\n");
        } else {
            printf("Distanța măsurată: %.2f cm\n", distance);
        }

        if (distance > 10 && distance < 30) {
            gpio_put(BUZZER_PIN, 1); // Pornește buzzer-ul
            sleep_ms(3000);           // Așteaptă 3 secunde
            gpio_put(BUZZER_PIN, 0); // Oprește buzzer-ul
        } else {
            gpio_put(BUZZER_PIN, 0); // Oprește buzzer-ul
        }

      

        sleep_ms(500);
    } while (true);
}

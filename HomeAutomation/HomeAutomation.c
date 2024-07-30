#include "pico/stdlib.h"
#include <stdio.h>
#include "dht11.h"

#define LED_PIN 15

int main() {
    
    // Initialize stdio
    stdio_init_all();
    // uart_init(uart0, 9600);
    
    // Initialize the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // LED off initially

    

    // print to serial
    printf("Begin Home Automation\n");
    while (true) {

        // Check if there are characters available from USB serial
        char ch = getchar_timeout_us(0);  // Non-blocking read with 0 timeout

        if (ch != PICO_ERROR_TIMEOUT) {
            printf("Received: %c\n", ch);
            // Control the LED based on the command received
            if (ch == '1') {
                gpio_put(LED_PIN, 1); // Turn on the LED
            } else if (ch == '0') {
                gpio_put(LED_PIN, 0); // Turn off the LED
            }
        }

        // Read the DHT11 sensor data
        dht11_data_t data;
        int ret = dht11_read(14, &data);
        if (ret == DHTLIB_OK) {
            printf("Temp: %d, Hum: %d\n", data.temperature, data.humidity);
        } else {
            printf("Error reading DHT11 sensor\n");
        }


        // Short delay to prevent high CPU usage
        sleep_ms(1000);
    }

    return 0;
}

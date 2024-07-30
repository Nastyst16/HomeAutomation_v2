#include "pico/stdlib.h"
#include <stdio.h>

#define LED_PIN 20

int main() {
    // Initialize the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // LED off initially

    // Initialize stdio
    stdio_init_all();

    // print to serial
    printf("Begin Home Automation\n");
    while (true) {

        // Check if there are characters available from USB serial
        int ch = getchar_timeout_us(0);  // Non-blocking read with 0 timeout

        if (ch != PICO_ERROR_TIMEOUT) {
            printf("Received: %c\n", ch);
            // Control the LED based on the command received
            if (ch == '1') {
                gpio_put(LED_PIN, 1); // Turn on the LED
            } else if (ch == '0') {
                gpio_put(LED_PIN, 0); // Turn off the LED
            }
        } else {
            // No characters received
            printf("No characters received\n");
        }

        // Short delay to prevent high CPU usage
        sleep_ms(1000);
    }

    return 0;
}

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>

// Definim pinul LED-ului
#define LED_PIN 20

// Inițializăm UART0
#define UART_ID uart0
#define UART_BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void setup_uart() {
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, 8, UART_PARITY_NONE, 1);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_fifo_enabled(UART_ID, true);
    printf("UART configurat.\n");
}

void setup_led() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // Asigură-te că LED-ul este oprit la început
    printf("LED configurat.\n");
}

void control_led(char command) {
    if (command == '1') {
        gpio_put(LED_PIN, 1); // Aprinde LED-ul
        printf("LED ON\n");
    } else if (command == '0') {
        gpio_put(LED_PIN, 0); // Stinge LED-ul
        printf("LED OFF\n");
    } else {
        printf("Comandă necunoscută: %c\n", command);
    }
}

int main() {
    stdio_init_all();
    setup_uart();
    setup_led();
    char c;

    while (true) {
    
        // printf("inaite de read blocking\n");
        // uart_read_blocking(UART_ID, &c, 1);
        // printf("dupa read blocking\n");

        // if (c == 1 || c == 0 || c == '1' || c == '0') {
        //     control_led(c);
        //     printf("Comandă primită: %c\n", c);
        // } else {
        //     printf("Comandă necunoscută: %c\n", c);
        // }

        if (uart_is_readable(UART_ID)) {
            char command = uart_getc(UART_ID);
            printf("Comandă primită: %c\n", command);
            control_led(command);
        } else {
            printf("Aștept comandă...\n"); 
        }

        // Pauză scurtă pentru a nu ocupa complet CPU-ul
        sleep_ms(1000);
    }

    return 0;
}

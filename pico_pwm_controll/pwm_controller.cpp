/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"


#define UART_ID uart0
#define BAUD_RATE 115200


#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define Front_R_pin 2
#define Front_L_pin 3
#define Back_R_pin 4
#define Back_L_pin 5


int main() {

    uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    gpio_set_function(Front_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Front_L_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_L_pin, GPIO_FUNC_PWM);

    uart_puts(UART_ID, "uart ready");

}

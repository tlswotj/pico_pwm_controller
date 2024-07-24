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
#define PWM_FREQ 300

struct levelcalc{
    uint16_t wrap;
    int mod = 1;
    void setWrap(uint16_t wrap){
        wrap = this->wrap;
    }
    uint16_t getLevel(int input){
        uint16_t res = wrap/2;
        res = res + (input*res)/100;        
        return res;
    }

};

void kill_all_pwms(){
    pwm_set_gpio_level(Front_L_pin, 0);
    pwm_set_gpio_level(Front_R_pin, 0);
    pwm_set_gpio_level(Back_L_pin, 0);
    pwm_set_gpio_level(Back_R_pin, 0);
}


int main() {
    
    gpio_set_function(Front_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Front_L_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_L_pin, GPIO_FUNC_PWM);

    uint sliceFR = pwm_gpio_to_slice_num(Front_R_pin);
    uint sliceFL = pwm_gpio_to_slice_num(Front_L_pin);
    uint sliceBR = pwm_gpio_to_slice_num(Back_R_pin);
    uint sliceBL = pwm_gpio_to_slice_num(Back_L_pin);

    uint chanFR = pwm_gpio_to_channel(Front_R_pin);
    
    uint16_t main_wrap = UINT16_MAX/PWM_FREQ;
    
    pwm_set_wrap(sliceFR, main_wrap);
    pwm_set_wrap(sliceFL, main_wrap);
    pwm_set_wrap(sliceBR, main_wrap);
    pwm_set_wrap(sliceBL, main_wrap);



    uart_puts(UART_ID, "uart ready");

    kill_all_pwms();

    bool kill = false;
    while(!kill){
        
    }

    kill_all_pwms();

}

/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

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


float calc_clkdv(uint32_t freq, uint16_t &wrap){
    float res;
    if(freq>1907){
        res=1;
        wrap = UINT16_MAX/freq;
    }
    else{
        wrap = UINT16_MAX-1;
        res = 1907.35/freq;
    }
    return res;
}


int main() {
    stdio_init_all();
    uint16_t main_wrap = UINT16_MAX/PWM_FREQ;
    float clkdv_main = calc_clkdv(PWM_FREQ, main_wrap);




    gpio_set_function(Front_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Front_L_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_L_pin, GPIO_FUNC_PWM);

    uint sliceFR = pwm_gpio_to_slice_num(Front_R_pin);
    uint sliceFL = pwm_gpio_to_slice_num(Front_L_pin);
    uint sliceBR = pwm_gpio_to_slice_num(Back_R_pin);
    uint sliceBL = pwm_gpio_to_slice_num(Back_L_pin);

    //uint chanFR = pwm_gpio_to_channel(Front_R_pin);
    
    
    
    pwm_set_wrap(sliceFR, main_wrap);
    pwm_set_wrap(sliceFL, main_wrap);
    pwm_set_wrap(sliceBR, main_wrap);
    pwm_set_wrap(sliceBL, main_wrap);


    kill_all_pwms();

    bool kill = false;
    while(!kill){
        std::cout<<"cout test"<<std::endl;
        
    }

    kill_all_pwms();

}

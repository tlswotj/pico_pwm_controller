/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1
#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define Front_R_pin 2
#define Front_L_pin 3
#define Back_R_pin 4
#define Back_L_pin 5
#define PWM_FREQ 300

struct levelcalc{
    uint16_t wrap =0;
    int mod = 1;
    void setWrap(uint16_t wrap){
        this->wrap = wrap;
    }
    uint16_t getLevel(int input){
        uint16_t res = wrap/2;
        res = res + (input*res)/100;        
        return res;
    }

};

uint16_t direct_levelcalc(int input, uint16_t wrap){
    uint16_t res = wrap/2;
    res = res + (input*res)/100;  
    return res;
}

void kill_all_pwms(){
    pwm_set_gpio_level(Front_L_pin, 0);
    pwm_set_gpio_level(Front_R_pin, 0);
    pwm_set_gpio_level(Back_L_pin, 0);
    pwm_set_gpio_level(Back_R_pin, 0);
    pwm_set_enabled(pwm_gpio_to_slice_num(Front_L_pin), false);
    pwm_set_enabled(pwm_gpio_to_slice_num(Front_R_pin), false);
    pwm_set_enabled(pwm_gpio_to_slice_num(Back_L_pin), false);
    pwm_set_enabled(pwm_gpio_to_slice_num(Back_R_pin), false);
}

float calc_clkdv(uint32_t freq, uint16_t &wrap){
    float res;
    if(freq>1907){
        res=1;
        wrap = 125000000/freq; // freq:1907 = wrap : uintmax, 1907 = wrap*freq*uintmax, wrap = 1907/freq*UINTMAX   freq = 125000000/wrap
    }
    else if(freq>953){
        res = 2;
        wrap = 62500000/freq;
    }
    else if(freq>636){
        res = 3;
        wrap = 41666667/freq;
    }
    else if(freq>477){
        res = 4;
        wrap = 31250000/freq;
    }
    else if(freq>381){
        res = 5;
        wrap = 25000000/freq;
    }
    else if(freq>238){
        res = 8;
        wrap = 15625000/freq;
    }
    else if(freq>190){
        res = 10;
        wrap = 12500000/freq;
    }
    else{
        wrap = UINT16_MAX;
        res = 1907.38/freq;
    }
    return res;
}

bool serial_input_detector(char *buffer){
    char temp = getchar_timeout_us(10);
    if(temp!=255){
        int counter=0;
        while(temp!=10&&counter<8){
            if(temp!=255){
                buffer[counter] = temp;
                counter++;
            }
            temp = getchar_timeout_us(0);
        }
        return true;
    }
    return false;
}

void init_pwm_value_enable(levelcalc level){
    pwm_set_gpio_level(Front_L_pin, level.getLevel(100));
    pwm_set_gpio_level(Front_R_pin, level.getLevel(100));
    pwm_set_gpio_level(Back_L_pin, level.getLevel(100));
    pwm_set_gpio_level(Back_R_pin, level.getLevel(100));
    pwm_set_enabled(pwm_gpio_to_slice_num(Front_L_pin), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(Front_R_pin), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(Back_L_pin), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(Back_R_pin), true);
}
void init_pwm_value_disable(levelcalc level){
    pwm_set_gpio_level(Front_L_pin, level.getLevel(100));
    pwm_set_gpio_level(Front_R_pin, level.getLevel(100));
    pwm_set_gpio_level(Back_L_pin, level.getLevel(100));
    pwm_set_gpio_level(Back_R_pin, level.getLevel(100));
    pwm_set_enabled(pwm_gpio_to_slice_num(Front_L_pin), false);
    pwm_set_enabled(pwm_gpio_to_slice_num(Front_R_pin), false);
    pwm_set_enabled(pwm_gpio_to_slice_num(Back_L_pin), false);
    pwm_set_enabled(pwm_gpio_to_slice_num(Back_R_pin), false);
}

int char_to_int(char *point, int startidx =0){
    int res=0;
    bool m = false;
    if(point[startidx]=='-'){
        startidx++;
        m = true;
    }
    for (int i=startidx; point[i]!='\n'; i++){
        res = res*10;
        res = res+ (point[i]-48);
    }
    if(m){
        res*(-1);
    }
    
    return res;
}

bool is_two_char_same(char* input, char a, char b){
    return input[0]== a && input[1] == b;
}

void set_lv_by_input(int pin_num, char *input, uint16_t wrap,int inputIdx = 2){
    pwm_set_gpio_level(pin_num, direct_levelcalc(char_to_int(input, inputIdx), wrap));
}


int main() {
    stdio_init_all();
    uint16_t main_wrap = UINT16_MAX/PWM_FREQ;
    float clkdv_main = calc_clkdv(PWM_FREQ, main_wrap);
    levelcalc level;
    level.setWrap(main_wrap);




    gpio_set_function(Front_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Front_L_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_R_pin, GPIO_FUNC_PWM);
    gpio_set_function(Back_L_pin, GPIO_FUNC_PWM);

    uint sliceFR = pwm_gpio_to_slice_num(Front_R_pin);
    uint sliceFL = pwm_gpio_to_slice_num(Front_L_pin);
    uint sliceBR = pwm_gpio_to_slice_num(Back_R_pin);
    uint sliceBL = pwm_gpio_to_slice_num(Back_L_pin);

        
    
    pwm_set_wrap(sliceFR, main_wrap);
    pwm_set_wrap(sliceFL, main_wrap);
    pwm_set_wrap(sliceBR, main_wrap);
    pwm_set_wrap(sliceBL, main_wrap);


    kill_all_pwms();
    bool kill = false;
    init_pwm_value_enable(level);

    while(!kill){

        char input_buffer[100];
        if(serial_input_detector(input_buffer)){

            if(is_two_char_same(input_buffer, 'f', 'r')){
                set_lv_by_input(Front_R_pin, input_buffer, main_wrap);
            }
            else if( is_two_char_same(input_buffer, 'f', 'l')){
                set_lv_by_input(Front_L_pin, input_buffer, main_wrap);
            }
            else if( is_two_char_same(input_buffer, 'b', 'r')){
                set_lv_by_input(Back_R_pin, input_buffer, main_wrap);
            }
            else if( is_two_char_same(input_buffer, 'b', 'l')){
                set_lv_by_input(Back_L_pin, input_buffer, main_wrap);
            }
            else{
                init_pwm_value_enable(level);
                kill = true;
            }
        }
    }

    kill_all_pwms();

}

 
// Output PWM signals on pins 0 and 1
#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"



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

int char_to_uint(char *point, int num, int startidx =0){
    int res=0;
    for ( int i=startidx; i<num; i++){
        res = res*10;
        res = res+ (point[i]-48);
    }
    
    return res;
}


int main() {
    stdio_init_all();
    // Tell GPIO 0 and 1 they are allocated to the PWM

    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);
    gpio_set_function(2, GPIO_FUNC_PWM);
 
    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)

    uint slice_num = pwm_gpio_to_slice_num(0);
    uint slice_num1 = pwm_gpio_to_slice_num(2);
    // Set period of 4 cycles (0 to 3 inclusive)
    uint16_t main_wrap;
    float clkdiv = calc_clkdv(30, main_wrap);
    printf("wrap : %u, clkdiv : %f\n",main_wrap);
    levelcalc level;
    level.setWrap(main_wrap);

    pwm_set_wrap(slice_num, main_wrap);
    pwm_set_wrap(slice_num1, main_wrap);

    pwm_set_clkdiv(slice_num, clkdiv);
    pwm_set_clkdiv(slice_num1,  clkdiv);
    // Set channel A output high for one cycle before dropping

    pwm_set_chan_level(slice_num, PWM_CHAN_A, level.getLevel(-80));
    // Set initial B output high for three cycles before dropping

    pwm_set_chan_level(slice_num, PWM_CHAN_B, level.getLevel(100));

    pwm_set_chan_level(slice_num1, PWM_CHAN_A, level.getLevel(0));
    // Set the PWM running

    pwm_set_enabled(slice_num, true);
    pwm_set_enabled(slice_num1, true);
 
    // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the

    // correct slice and channel for a given GPIO.
    char test[10];
    //printf("%c \n", test);
    int freq=30;
    int clevel = 100;
    while(1){
        

        
        char temp= getchar_timeout_us(10);
        if(temp!=255){
            //freq = 100;
            
            int counter=0;

            while(temp!=10&&counter<8){
                if(temp!=255){
                    test[counter] = temp;
                    counter++;
                }
                temp = getchar_timeout_us(0);
            }
            
            if(test[0]=='f'&&test[1]=='r'){
                freq = char_to_uint(test, counter, 2);
                
                clkdiv = calc_clkdv(freq, main_wrap);
                level.setWrap(main_wrap);

                pwm_set_wrap(slice_num, main_wrap);
                pwm_set_wrap(slice_num1, main_wrap);

                pwm_set_clkdiv(slice_num, clkdiv);
                pwm_set_clkdiv(slice_num1, clkdiv);
            }
            else if(test[0]=='l'&&test[1]=='v'){
                clevel = char_to_uint(test, counter, 2);
            }
            else{
                printf("error\n %c %c\n", test[0], test[1]);
            }            

            pwm_set_chan_level(slice_num, PWM_CHAN_A, level.getLevel(-80));
            pwm_set_chan_level(slice_num, PWM_CHAN_B, level.getLevel(clevel-100));
            pwm_set_chan_level(slice_num1, PWM_CHAN_A, level.getLevel(0));

            printf("freq : %d, level : %d\n",freq, clevel);
        }
        
    }

}

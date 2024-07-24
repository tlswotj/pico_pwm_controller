 
// Output PWM signals on pins 0 and 1
 
#include "pico/stdlib.h"
#include "hardware/pwm.h"
 

int main() {
 
    // Tell GPIO 0 and 1 they are allocated to the PWM

    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);
    gpio_set_function(2, GPIO_FUNC_PWM);
 
    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)

    uint slice_num = pwm_gpio_to_slice_num(0);
    uint slice_num1 = pwm_gpio_to_slice_num(2);
    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_clkdiv(slice_num, 5.0);
    pwm_set_clkdiv(slice_num1, 5.0);
    pwm_set_wrap(slice_num, UINT16_MAX);
    pwm_set_wrap(slice_num1, UINT16_MAX);
    // Set channel A output high for one cycle before dropping

    pwm_set_chan_level(slice_num, PWM_CHAN_A, 10000);
    // Set initial B output high for three cycles before dropping

    pwm_set_chan_level(slice_num, PWM_CHAN_B, UINT16_MAX);

    pwm_set_chan_level(slice_num1, PWM_CHAN_A, UINT16_MAX/2);
    // Set the PWM running

    pwm_set_enabled(slice_num, true);
    pwm_set_enabled(slice_num1, true);
 
    // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the

    // correct slice and channel for a given GPIO.

}

#include "rgb_example.h"

void rgb_example(void)
{
    red_led_on();
    HAL_Delay(500);
    red_led_off();

    blue_led_on();
    HAL_Delay(500);
    blue_led_off();

    green_led_on();
    HAL_Delay(500);
    green_led_off();
}

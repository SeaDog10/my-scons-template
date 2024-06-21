#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "config.h"

#ifdef USEING_REG_EXAMPLE
#include "rgb_example.h"
#endif

extern void SystemClock_Config(void);

int main(void)
{
    uint8_t str[20] = "Hello SCons";

    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART1_UART_Init();

    HAL_UART_Transmit(&huart1, (uint8_t *)&str[0], 20, 0xffff);

    while (1)
    {
#ifdef USEING_REG_EXAMPLE
        rgb_example();
#endif
    }
}

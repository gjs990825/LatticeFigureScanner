/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");
    // 基本外设初始化

    LED_Init();

    while (1)
    {
        PeriodicTask(500, LED_Blink(0));
        PeriodicTask(1000, LED_Blink(2));
    }
}

void assert_failed(uint8_t *file, uint32_t line)
{
    while (1)
    {
        printf("File: %s, line: %d\r\n", file, line);
        delay_ms(500);
    }
}
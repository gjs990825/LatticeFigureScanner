/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "iic.h"
#include "ads1115.h"
#include "oled.h"
#include "spi.h"
#include "gui.h"
#include "key.h"
#include "stepper.h"
#include "lightsensor.h"
#include "interface.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");

    HardwareSPI_Init();
    OLED_Init();
    GUI_ChangeDisplay(mainPage, mainPageNumber,
                      mainPage, mainPageNumber);
    TIM3_TimerInit(7200, 1000);

    LED_Init();
    KEY_Init();
    STEPPER_Init();
    SensorArray_Init();

    GUI_Control(ENABLE);

    uint8_t key;

    while (1)
    {
        PeriodicTask(500, LED_Blink(0));

        key = KEY_Scan(0);
        if (key)
        {
            while (key == KEY_Scan(1))
                ;
            printf("KEY%d\r\n", key);

            uint8_t opt = Key_Handle(key);
            printf("opt%d\r\n", opt);
            switch (opt)
            {
            case 0:
                Basic();
                break;
            case 1:
                Scan();
                break;
            case 2:
                Repay();
                break;
            case 3:
                LED_Repay();
                break;

            default:
                break;
            }
        }
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

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

void Key_Handle(uint8_t key)
{
   static int16_t currentComponent = 0;
   static int16_t lastComponent = 0;

   if (key == KEY_UP)
   {
       if ((--currentComponent) < 0)
           currentComponent = editableComponentsNumber - 1;
   }
   else if (key == KEY_DOWN)
   {
       if ((++currentComponent) >= editableComponentsNumber)
           currentComponent = 0;
   }

   if (currentComponent != lastComponent)
   {
       GUI_SetFlash(editableComponentsSet[lastComponent], false);
       GUI_SetFlash(editableComponentsSet[currentComponent], true);
       lastComponent = currentComponent;
   }
   
   

    // if (key == 5)
    // {
    //     // GUI_SetFlash(&TargetValue, true);
    //     // Key_GetSetValue(&setAngle1);
    //     // GUI_SetFlash(&TargetValue, false);
    //     // setAngle1 = constrain_float(setAngle1, 45, 145);

    //     // GUI_SetFlash(&Target2Value, true);
    //     // Key_GetSetValue(&setAngle2);
    //     // GUI_SetFlash(&Target2Value, false);
    //     // setAngle2 = constrain_float(setAngle2, 45, 145);

    //     // if (setAngle1 > setAngle2)
    //     // {
    //     //     setAngle1 = setAngle2;
    //     //     // setAngle2 = setAngle1 = (setAngle1 + setAngle2) / 2;
    //     // }

    //     key = GUI_ConfirmPage();
    //     // mission1Start = (key == 5) ? true : false;
    //     // printf("Start?:%d\r\n", mission1Start);
    // }
    // // 无重物模式
    // else if (key == 1)
    // {
    //     key = GUI_ConfirmPage();
    //     if (key == 5)
    //     {
    //         //PID_SetParam(25.0, 0.05, 165.0);
    //         // printf("Heavy Mode\r\n");
    //     }
    // }
    // // 重物模式
    // else if (key == 4)
    // {
    //     key = GUI_ConfirmPage();
    //     if (key == 5)
    //     {
    //         //PID_SetParam(25.0, 0.25, 365.0);
    //         // printf("Lite Mode\r\n");
    //     }
    // }
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");

    HardwareSPI_Init();
    OLED_Init();
    TIM3_TimerInit(7200, 1000);

    LED_Init();
    KEY_Init();

    uint8_t key;

    while (1)
    {
        PeriodicTask(500, LED_Blink(0));

        key = KEY_Scan(1);
        if (key)
        {
            while (key == KEY_Scan(1))
                ;
            printf("KEY%d\r\n", key);
            Key_Handle(key);
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

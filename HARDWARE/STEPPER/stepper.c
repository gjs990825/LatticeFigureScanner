#include "stepper.h"
#include "delay.h"
#include "math.h"
#include "stdio.h"
#include "usart.h"
#include "key.h"
#include "my_lib.h"

#define STEPPER_DELAY_X 1000
#define STEPPER_DELAY_Y 2000

const uint32_t MAX_STEP = 5000;

// Ã¿ÀåÃ×Âö³åÊý
const float stepsPerCM = 50.0;

int currentX;
int currentY;

int STEPPER_currentY = 0;
int STEPPER_currentX = 0;

void Polar_To_Cartesian(double r, double thet, double *x, double *y)
{
    *x = r * cos(thet);
    *y = r * sin(thet);
}

void STEPPER_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void STEPPER_Step_X(bool dir, uint32_t steps)
{
    dirPin1 = dir;

    for (uint32_t i = 0; i < steps; i++)
    {
        stepperPin1 = 1;
        delay_us(STEPPER_DELAY_X);
        stepperPin1 = 0;
        delay_us(STEPPER_DELAY_X);
    }
}

void STEPPER_Step_Y(bool dir, uint32_t steps)
{
    dirPin2 = dir;

    for (uint32_t i = 0; i < steps; i++)
    {
        stepperPin2 = 1;
        delay_us(STEPPER_DELAY_Y);
        stepperPin2 = 0;
        delay_us(STEPPER_DELAY_Y);
    }
}

void STEPPER_Move_To(uint32_t x, uint32_t y)
{
    if ((x > MAX_STEP) || (y > MAX_STEP))
    {
        return;
    }
    STEPPER_Step_X(!(x > currentX), (x > currentX) ? (x - currentX) : (currentX - x));
    STEPPER_Step_Y(!(y > currentY), (y > currentY) ? (y - currentY) : (currentY - y));
    currentX = x;
    currentY = y;
}

void STEPPER_Move_To_CM(float xcm, float ycm)
{
    if ((xcm > 30) || (ycm > 30))
    {
        return;
    }

    uint32_t x = stepsPerCM * xcm;
    uint32_t y = stepsPerCM * ycm;

    STEPPER_Step_X(!(x > currentX), (x > currentX) ? (x - currentX) : (currentX - x));
    STEPPER_Step_Y(!(y > currentY), (y > currentY) ? (y - currentY) : (currentY - y));

    currentX = x;
    currentY = y;
}

#include "lightsensor.h"
#include "stdio.h"

typedef struct sensorPin
{
    GPIO_TypeDef *GPIOx;
    uint16_t pin;
} sensorPin_t;

sensorPin_t sensorArray[4] = {
    {GPIOB, GPIO_Pin_3},
    {GPIOB, GPIO_Pin_4},
    {GPIOB, GPIO_Pin_5},
    {GPIOB, GPIO_Pin_6},
};

void SensorArray_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    for (uint8_t i = 0; i < 4; i++)
    {
        GPIO_InitStructure.GPIO_Pin = sensorArray[i].pin;
        GPIO_Init(sensorArray[i].GPIOx, &GPIO_InitStructure);
    }
}

uint8_t SensorArray_Read(void)
{
    uint8_t outData = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        outData |= GPIO_ReadInputDataBit(sensorArray[i].GPIOx, sensorArray[i].pin) ? (1 << i) : 0;
    }
    return outData;
}

bool SensorArray_GetMain(void)
{
    return GPIO_ReadInputDataBit(sensorArray[0].GPIOx, sensorArray[0].pin);
}

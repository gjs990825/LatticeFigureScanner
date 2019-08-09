#if !defined(_STEPPER_H_)
#define _STEPPER_H_

#include "sys.h"

#define LINE_SPEED 2500
#define PIXEL_SPEED 8000

#define ALL_BLACK 24

#define FAST 14000 //15000
#define SLOW 8000  //2500

#define MAX_STEP 49000

#define DEFAULT_STEPPER_SPEED FAST
#define scale 100

#define dirPin1 PAout(0)
#define dirPin2 PAout(1)
#define stepperPin1 PCout(14)
#define stepperPin2 PCout(15)
#define HIGH 1
#define LOW 0

#define ERR()                \
    while (1)                \
    {                        \
        printf("ERROR\r\n"); \
        delay_ms(100);       \
    }

void STEPPER_Init(void);
void STEPPER_Step(bool stepperN, bool dir, uint32_t steps);
void STEPPER_Move_To(uint32_t x, uint32_t y);
void STEPPER_Set_Speed(uint16_t speed);
void STEPPER_Draw_Circle(uint32_t x, uint32_t y, uint32_t r);
void STEPPER_Draw_Rectangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
void STEPPER_Draw_Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t speed);
// uint8_t Manual_Set(void);

#endif // _STEPPER_H_

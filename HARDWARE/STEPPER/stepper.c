#include "stepper.h"
#include "delay.h"
#include "math.h"
#include "stdio.h"
#include "usart.h"
#include "key.h"

int currentX;
int currentY;

int STEPPER_currentY = 0;
int STEPPER_currentX = 0;

uint16_t Stepper_Speed = DEFAULT_STEPPER_SPEED;

double map(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Polar_To_Cartesian(double r, double thet, double *x, double *y)
{
    *x = r * cos(thet);
    *y = r * sin(thet);
}

void STEPPER_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void STEPPER_Step(bool stepperN, bool dir, uint32_t steps)
{
    uint16_t Stepper_Delay = 1000000 / Stepper_Speed * 2;

    if (stepperN)
    {
        dirPin1 = dir;
        for (int i = 0; i < steps; i++)
        {
            stepperPin1 = HIGH;
            delay_us(Stepper_Delay);
            stepperPin1 = LOW;
            delay_us(Stepper_Delay);
        }
    }
    else
    {
        dirPin2 = dir;
        for (int i = 0; i < steps; i++)
        {
            stepperPin2 = HIGH;
            delay_us(Stepper_Delay);
            stepperPin2 = LOW;
            delay_us(Stepper_Delay);
        }
    }
}

void STEPPER_Move_To(uint32_t x, uint32_t y)
{
    if ((x > MAX_STEP) || (y > MAX_STEP))
    {
        ERR();
    }
    Step(1, !(x > currentX), (x > currentX) ? (x - currentX) : (currentX - x));
    Step(0, !(y > currentY), (y > currentY) ? (y - currentY) : (currentY - y));
    currentX = x;
    currentY = y;
}

void STEPPER_Set_Speed(uint16_t speed)
{
    Stepper_Speed = speed;
}

// uint8_t STEPPER_Manual_Set(void)
// {
//     int x = 0, y = 0;
//     uint16_t steplen = 10;
//     uint8_t key = 0;
//     key = KEY::Scan(1);
//     if (key)
//     {
//         switch (key)
//         {
//         case KEY_UP:
//             y += steplen;
//             break;
//         case KEY_DOWN:
//             y -= steplen;
//             break;
//         case KEY_LEFT:
//             x -= steplen;
//             break;
//         case KEY_RIGHT:
//             x += steplen;
//             break;
//         case KEY_OK:
//             lazerON();
//             delay_ms(500);
//             lazerOFF();
//             if (KEY_OK_STA == 0)
//             {
//                 printf("校准完成，开始运行！\r\n");
//                 return 0xFF;
//             }
//             break;
//         default:
//             break;
//         }
//         Set_Speed(FAST);
//         Step(1, (x > 0) ? 0 : 1, (x > 0) ? x : -x);
//         Step(0, (y > 0) ? 0 : 1, (y > 0) ? y : -y);
//     }
//     return 0;
// }

void STEPPER_Draw_Circle(uint32_t x, uint32_t y, uint32_t r)
{
    double i = 0.0, radius = 0.0, steplen = 0.0;
    if ((x < r) || (y < r))
    {
        ERR();
    }
    Set_Speed(FAST);
    Move_To(x - r, y); //起始点

    radius = (double)r / scale; //缩放
    steplen = 1.0 / scale;      //步长计算
    Set_Speed(LINE_SPEED);
    // lazerON();
    delay_ms(100);

    for (i = -(double)radius; i < radius; i += steplen)
    { //上半
        Move_To((uint32_t)(x + i * scale), (uint32_t)(y + sqrt(radius * radius - i * i) * scale));
    }
    for (i = radius; i > -(double)radius; i -= steplen)
    { //下半
        Move_To((uint32_t)(x + i * scale), (uint32_t)(y - sqrt(radius * radius - i * i) * scale));
    }

    delay_ms(100);
    // lazerOFF();
    Set_Speed(FAST);
}

void STEPPER_Draw_Rectangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
    Set_Speed(LINE_SPEED);
    Draw_Line(x1, y1, x2, y1, LINE_SPEED);
    Draw_Line(x1, y1, x1, y2, LINE_SPEED);
    Draw_Line(x1, y2, x2, y2, LINE_SPEED);
    Draw_Line(x2, y1, x2, y2, LINE_SPEED);
}

void STEPPER_Draw_Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t speed)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1; //设置单步方向
    else if (delta_x == 0)
        incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;
    Move_To(x1, y1);
    Set_Speed(speed);
    // lazerON();
    for (t = 0; t <= distance + 1; t++) //画线输出
    {
        Move_To(uRow, uCol); //画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
    // lazerOFF();
    Set_Speed(FAST);
}

#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY1 PAin(4)
#define KEY2 PAin(2)
#define KEY3 PAin(1)
#define KEY4 PAin(0)
#define KEY5 PAin(3)

#define KEY1_PRES 1
#define KEY2_PRES 2
#define KEY3_PRES 3
#define KEY4_PRES 4
#define KEY5_PRES 5

#define KEY_UP KEY1_PRES
#define KEY_LEFT KEY2_PRES
#define KEY_RIGHT KEY3_PRES
#define KEY_DOWN KEY4_PRES
#define KEY_OK KEY5_PRES

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t);
void Key_GetSetValue(float *val);

#endif

#if !defined(_STEPPER_H_)
#define _STEPPER_H_

#include "sys.h"

#define dirPin1 PAout(7)
#define dirPin2 PAout(6)
#define stepperPin1 PBout(1)
#define stepperPin2 PBout(0)


void STEPPER_Init(void);
void STEPPER_Move_To(uint32_t x, uint32_t y);
void STEPPER_Move_To_CM(float xcm, float ycm);

#endif // _STEPPER_H_

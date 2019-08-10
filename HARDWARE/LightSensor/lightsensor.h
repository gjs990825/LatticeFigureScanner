#if !defined(_LEDARRAY_H_)
#define _LEDARRAY_H_

#include "sys.h"

void SensorArray_Init(void);
uint8_t SensorArray_Read(void);
bool SensorArray_GetMain(void);

#endif // _LEDARRAY_H_

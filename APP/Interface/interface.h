#if !defined(_INTERFACE_H_)
#define _INTERFACE_H_

#include "sys.h"

uint16_t Display_ArrayToHex(bool pixArray[16]);
void Display_HexToArray(uint16_t hex, bool *pixArray);
uint8_t Key_Handle(uint8_t key);

void Basic(uint8_t n);
void Scan(void);
void Replay(void);
void LED_Replay(uint8_t n);

#endif // _INTERFACE_H_

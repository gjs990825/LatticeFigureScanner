#if !defined(_INTERFACE_H_)
#define _INTERFACE_H_

#include "sys.h"

uint16_t Display_ArrayToHex(bool pixArray[16]);
void Display_HexToArray(uint16_t hex, bool *pixArray);
uint8_t Key_Handle(uint8_t key);

void Basic(void);

void Scan_Once(bool pixel[][16]);
void Scan(void);
void Replay(void);
void LED_Replay(void);

void OLED_DrawImg16x16(uint16_t x, uint16_t y, uint16_t img[16]);

#endif // _INTERFACE_H_

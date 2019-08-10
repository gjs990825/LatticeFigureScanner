#include "interface.h"
#include "stdio.h"
#include "gui.h"
#include "key.h"
#include "stepper.h"
#include "delay.h"
#include "lightsensor.h"

// 从数组到十六进制
uint16_t Display_ArrayToHex(bool pixArray[16])
{
    uint16_t outData = 0;
    for (uint8_t i = 0; i < 16; i++)
    {
        outData |= (pixArray[i]) ? (1 << i) : 0;
    }
    return outData;
}

// 从十六进制到数组
void Display_HexToArray(uint16_t hex, bool *pixArray)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        pixArray[i] = (hex >> i) & 0x1;
    }
}

uint8_t Key_Handle(uint8_t key)
{
    if (editableSetPointer[0]->relatedVar != NULL)
    {
        if (key == KEY_OK)
        {
            for (uint8_t i = 0; i < editableNumberRec; i++)
            {
                GUI_SetFlash(editableSetPointer[i], true);
                Key_GetSetValue((uint8_t *)editableSetPointer[i]->relatedVar);
                GUI_SetFlash(editableSetPointer[i], false);
            }

            return GUI_ConfirmPage();
        }
        return 0;
    }
    else
    {
        return Get_Option();
    }
}

void Basic(void)
{
    GUI_ChangeDisplay(basicPageSet, basicPageNumber,
                      basicPageEditableSet, basicPageEditableNumber);
    uint8_t key, backKey;

    while (1)
    {
        key = KEY_Scan(1);
        if (key)
        {
            while (key == KEY_Scan(1))
                ;
            printf("KEY%d\r\n", key);
            backKey = Key_Handle(key);
            if (backKey)
                break;
        }
    }

    if (backKey == KEY_OK)
    {
        printf("CONTROL_Basic:[5][%d][%d]\r\n", *basicPageEditableSet[0]->relatedVar,
               *basicPageEditableSet[1]->relatedVar);
        delay(100);
        printf("CONTROL_Basic:[5][%d][%d]\r\n", *basicPageEditableSet[0]->relatedVar,
               *basicPageEditableSet[1]->relatedVar);
    }
    else
    {
        printf("Cancled, key%d\r\n", backKey);
    }

    GUI_ChangeDisplay(mainPage, mainPageNumber,
                      mainPage, mainPageNumber);
}

#define stepsPerGreed 25

void Scan_Once(bool pixel[][16])
{
    GUI_Control(DISABLE);

    bool sta = true;

    for (uint8_t i = 0; i < 16; i++)
    {
        if (sta)
        {
            for (uint8_t j = 0; j < 16; j++)
            {
                STEPPER_Move_To(i * stepsPerGreed, j * stepsPerGreed);
                pixel[15 - i][j] = SensorArray_GetMain();
            }
        }
        else
        {
            for (int8_t j = 15; j >= 0; j--)
            {
                STEPPER_Move_To(i * stepsPerGreed, j * stepsPerGreed);
                pixel[15 - i][j] = SensorArray_GetMain();
            }
        }
        sta = !sta;
    }
    STEPPER_Move_To(0, 0);

    GUI_Control(ENABLE);
}

bool scanBuffer[16][16];

// 暂存图形数据

uint16_t img1[16] = {
    0x8000,
    0x4000,
    0x2000,
    0x1000,
    0x0800,
    0x0400,
    0x0200,
    0x0100,
    0x0080,
    0x0040,
    0x0020,
    0x0010,
    0x0008,
    0x0004,
    0x0002,
    0x0001,
};
uint16_t img2[16] = {
    0x0080,
    0x0040,
    0x0020,
    0x0010,
    0x0008,
    0x0004,
    0x0002,
    0x0001,
    0x8000,
    0x4000,
    0x2000,
    0x1000,
    0x0800,
    0x0400,
    0x0200,
    0x0100,
};
uint16_t img3[16] = {
    0x8000,
    0x4000,
    0x2000,
    0x1000,
    0x0800,
    0x0400,
    0x0200,
    0x0100,
    0x0080,
    0x0040,
    0x0020,
    0x0010,
    0x0008,
    0x0004,
    0x0002,
    0x0001,
};

void Scan(void)
{
    GUI_ConfirmPage();
    Scan_Once(&scanBuffer[0]);
    for (uint8_t i = 0; i < 16; i++)
    {
        img1[i] = Display_ArrayToHex(scanBuffer[i]);
    }

    GUI_ConfirmPage();
    Scan_Once(scanBuffer);
    for (uint8_t i = 0; i < 16; i++)
    {
        img2[i] = Display_ArrayToHex(scanBuffer[i]);
    }
    GUI_ConfirmPage();

    Scan_Once(scanBuffer);
    for (uint8_t i = 0; i < 16; i++)
    {
        img3[i] = Display_ArrayToHex(scanBuffer[i]);
    }

    GUI_ChangeDisplay(mainPage, mainPageNumber,
                      mainPage, mainPageNumber);
}

// 画出16x16的数据，大小32x32(一个像素点用四个表示)
void OLED_DrawImg16x16(uint16_t x, uint16_t y, uint16_t img[16])
{
    uint32_t scaledImg[32];

    for (uint8_t i = 0; i < 16; i++)
    {
        scaledImg[i * 2] = img[i];
        scaledImg[i * 2 + 1] = img[i];
    }

    for (uint8_t i = 0; i < 32; i++)
    {
        uint32_t tmp = 0;
        for (uint8_t j = 0; j < 16; j++)
        {
            tmp |= (scaledImg[i] & 0x01) ? (1 << (j * 2)) : 0;
            tmp |= (scaledImg[i] & 0x01) ? (1 << (j * 2 + 1)) : 0;
            scaledImg[i] >>= 1;
        }
        scaledImg[i] = tmp;
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        OLED_SetPos(x, y + i);
        for (uint8_t j = 0; j < 32; j++)
        {
            OLED_WriteDat((scaledImg[j] >> (8 * i)) & 0xFF);
        }
    }
}

void Repay(void)
{
    GUI_Control(DISABLE);
    OLED_CLS();

    OLED_ShowStr((12 * 3 + 4) * 0, 0, "(1)", Font_12x24, false);
    OLED_ShowStr((12 * 3 + 4) * 1, 0, "(2)", Font_12x24, false);
    OLED_ShowStr((12 * 3 + 4) * 2, 0, "(3)", Font_12x24, false);
    OLED_DrawImg16x16((32 + 15) * 0, 4, img1);
    OLED_DrawImg16x16((32 + 15) * 1, 4, img2);
    OLED_DrawImg16x16((32 + 15) * 2, 4, img3);

    while (KEY_Scan(0) == 0)
        ;

    OLED_CLS();
    delay(100);
    GUI_Control(ENABLE);
}

void LED_Repay(void)
{
    GUI_Control(DISABLE);
    OLED_CLS();

    OLED_ShowStr(0, 2, "Sending...", Font_12x24, false);

    for (uint8_t i = 0; i < 3; i++)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            printf("IMG_DATA_1:[%d](%X)\r\n", i, img1[i]);
            delay(50);
            printf("IMG_DATA_2:[%d](%X)\r\n", i, img2[i]);
            delay(50);
            printf("IMG_DATA_3:[%d](%X)\r\n", i, img3[i]);
            delay(50);
        }
    }

    printf("IMG_DATA_OK\r\n");
    delay_ms(50);
    printf("IMG_DATA_OK\r\n");

    OLED_CLS();
    delay(100);
    GUI_Control(ENABLE);
}

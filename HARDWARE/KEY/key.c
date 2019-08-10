#include "sys.h"
#include "key.h"
#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "gui.h"

void KEY_Init(void) //IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

#define IfKey(n)     \
	if (KEY##n == 0) \
		return KEY##n##_PRES;

#define ElifKey(n)   \
	if (KEY##n == 0) \
		return KEY##n##_PRES;

// 0为不支持连按
uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up = 1; //按键按松开标志
	if (mode)
	{
		key_up = 1; //支持连按
	}

	if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0 || KEY5 == 0))
	{
		delay_ms(10); //去抖动
		key_up = 0;

		IfKey(1);
		ElifKey(2);
		ElifKey(3);
		ElifKey(4);
		ElifKey(5);
	}
	else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 && KEY5 == 1)
	{
		key_up = 1;
	}
	return 0; // 无按键按下
}

// 获取数值并取整
void Key_GetSetValue(uint8_t *val)
{
	if (val == NULL)
		return;

	uint8_t key;
	int8_t quitOption = 0;

	while (1)
	{
		key = KEY_Scan(1);

		switch (key)
		{
		case 2:
			*val -= 1;
			break;
		case 3:
			*val += 1;
			break;
		case 1:
			quitOption = -1;
			break;
		case 4:
			quitOption = -1;
			break;
		case 5:
			quitOption = 1;
			break;

		default:
			break;
		}

		if (quitOption)
			break;

		delay_ms(200);
	}

	while (key == KEY_Scan(1))
		;
}

uint8_t Get_Option(void)
{
	uint8_t key;
	int8_t currentCursor = 0;
	int8_t lastCursor = 0;

	GUI_SetFlash(editableSetPointer[0], true);
	while (1)
	{
		key = KEY_Scan(1);

		switch (key)
		{
		case KEY_UP:
			currentCursor--;
			break;
		case KEY_DOWN:
			currentCursor++;
			break;
		case KEY_OK:
			// 完成，取消闪烁
			GUI_SetFlash(editableSetPointer[currentCursor], false);
			return currentCursor;

		default:
			break;
		}

		if (currentCursor >= editableNumberRec)
			currentCursor = 0;
		else if (currentCursor < 0)
			currentCursor = editableNumberRec - 1;

		if (lastCursor != currentCursor)
		{
			GUI_SetFlash(editableSetPointer[lastCursor], false);
			GUI_SetFlash(editableSetPointer[currentCursor], true);

			lastCursor = currentCursor;
		}

		delay_ms(200);
	}
}

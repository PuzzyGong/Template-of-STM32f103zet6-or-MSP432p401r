#include "oled.h"

void oled_init(char* str1,  char* str2,  char* str3,  char* str4,
			   char* str5,  char* str6,  char* str7,  char* str8)

{
	OLED_Init();
	OLED_ShowString(0,  0, (uint8_t *)str1, 16);
	OLED_ShowString(64, 0, (uint8_t *)str2, 16);
	OLED_ShowString(0,  2, (uint8_t *)str3, 16);
	OLED_ShowString(64, 2, (uint8_t *)str4, 16);
	OLED_ShowString(0,  4, (uint8_t *)str5, 16);
	OLED_ShowString(64, 4, (uint8_t *)str6, 16);
	OLED_ShowString(0,  6, (uint8_t *)str7, 16);
	OLED_ShowString(64, 6, (uint8_t *)str8, 16);
}

void oled_show(uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4,
			   uint8_t i5, uint8_t i6, uint8_t i7, uint8_t i8)
{
	static uint16_t cnt = 0;
	
	cnt = (cnt == 3) ? 0 : cnt + 1;
	
	if(cnt == 0)
	{
		OLED_ShowNum(32, 0, i1, 3, 16);
		OLED_ShowNum(96, 0, i2, 3, 16);
	}
	else if(cnt == 1)
	{
		OLED_ShowNum(32, 2, i3, 3, 16);
		OLED_ShowNum(96, 2, i4, 3, 16);	
	}
	else if(cnt == 2)
	{
		OLED_ShowNum(32, 4, i5, 3, 16);
		OLED_ShowNum(96, 4, i6, 3, 16);
	}
	else if(cnt == 3)
	{
		OLED_ShowNum(32, 6, i7, 3, 16);
		OLED_ShowNum(96, 6, i8, 3, 16);
	}
}

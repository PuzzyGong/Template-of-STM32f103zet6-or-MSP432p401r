#ifndef __OLED_H
#define __OLED_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "olediic.h"

void oled_init(char* str1,  char* str2,  char* str3,  char* str4,
			   char* str5,  char* str6,  char* str7,  char* str8);

void oled_show(uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4,
			   uint8_t i5, uint8_t i6, uint8_t i7, uint8_t i8);
			   
#endif
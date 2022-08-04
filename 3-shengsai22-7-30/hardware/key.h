/****************************************************/
//MSP432P401R
//板载按键驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
/****************************************************/

#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define KEY1 (!BITBAND_PERI(P1IN, 1)) //读取按键1
#define KEY2 (!BITBAND_PERI(P1IN, 4)) //读取按键2

void KEY_Init(void);    //IO初始化


#endif

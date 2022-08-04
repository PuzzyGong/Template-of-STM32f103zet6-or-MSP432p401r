#ifndef __LED_H
#define __LED_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void LED_Init(void);

void LED_RED_On(void) ;
void LED_RED_Off(void);

void LED_Off(void)	;
void LED_R_On(void) ;
void LED_G_On(void) ;
void LED_Y_On(void) ;

void BUZZ_On(void);
void BUZZ_Off(void);

#endif

#include "led.h"

#define LED_RED BITBAND_PERI(P1OUT,0)
#define LED_R 	BITBAND_PERI(P2OUT,0)
#define LED_G 	BITBAND_PERI(P2OUT,1)
#define BUZZ 	BITBAND_PERI(P1OUT,7)

void LED_Init(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0 + GPIO_PIN7);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1);

    LED_RED_Off();
    LED_Off();
	BUZZ = 0;
}

void LED_RED_On(void) { LED_RED = 1; }
void LED_RED_Off(void) { LED_RED = 0; }

void LED_Off(void)	{ LED_R = 0; LED_G = 0;}

void LED_R_On(void) { LED_R = 1; LED_G = 0;}

void LED_G_On(void) { LED_R = 0; LED_G = 1;}

void LED_Y_On(void) { LED_R = 1; LED_G = 1;}

void BUZZ_On(void) {BUZZ = 1;}
void BUZZ_Off(void) {BUZZ = 0;}

 
#ifndef __RNA_TIMA_H
#define __RNA_TIMA_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void TimA0_PWM_Init(uint16_t ccr0, uint16_t psc);
void TimA1_PWM_Init(uint16_t ccr0, uint16_t psc);
void TimA2_PWM_Init(uint16_t ccr0, uint16_t psc);

#endif

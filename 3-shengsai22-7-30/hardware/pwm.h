#ifndef __PWM_H
#define __PWM_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void pwm_init(void);

void speed_set(int pwm1, int pwm2);

void speed_adjust(float dv1, float dv2);

void dirc_set(float angle);


#endif

#include "pwm.h"
#include "timA.h"

void pwm_init(void)
{
	TimA2_PWM_Init(19999, TIMER_A_CLOCKSOURCE_DIVIDER_48);
	MAP_Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 1499); 
	
	TimA1_PWM_Init(4800 - 1, TIMER_A_CLOCKSOURCE_DIVIDER_1);
}

void speed_set(int pwm1, int pwm2)
{
	if(pwm1 > 0)
	{
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, pwm1); 
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, 0);	
	}
	else
	{
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 0); 
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, -pwm1);	
	}
	if(pwm2 > 0)
	{
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, pwm2); 
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 0);	
	}
	else
	{
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, 0); 
		MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, -pwm2);	
	}
}

void speed_adjust(float dv1, float dv2)
{
	static float v1 = 0, v2 = 0;
	
	v1 -= dv1;
	if(v1 > 4700) v1 = 4700;
	else if(v1 < -4700) v1 = -4700;
	
	v2 -= dv2;
	if(v2 > 4700) v2 = 4700;
	else if(v2 < -4700) v2 = -4700;	
	
	speed_set(v1, v2);
}

void dirc_set(float angle)
{
	if(angle > 500) angle = 500;
	else if(angle < -500) angle = -500;
	
	MAP_Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, (uint32_t)(1499 + angle)); 
}
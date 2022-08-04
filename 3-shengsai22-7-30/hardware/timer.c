#include "timer.h"


void TimA3_Init(uint16_t ccr0, uint16_t psc)
{
    /*������ģʽ��ʼ��*/
    Timer_A_UpModeConfig upConfig;
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                                      //ʱ��Դ
    upConfig.clockSourceDivider = psc;                                                     //ʱ�ӷ�Ƶ ��Χ1-64
    upConfig.timerPeriod = ccr0;                                                           //�Զ���װ��ֵ��ARR��
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;                   //���� tim����ж�
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; //���� ccr0�����ж�
    upConfig.timerClear = TIMER_A_DO_CLEAR;                                                // Clear value

    /*��ʼ����ʱ��A*/
    MAP_Timer_A_configureUpMode(TIMER_A3_BASE, &upConfig);

    /*ѡ��ģʽ��ʼ����*/
    MAP_Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    /*����Ƚ��жϱ�־λ*/
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    /*�������ڶ˿��ж�*/
    MAP_Interrupt_enableInterrupt(INT_TA3_0);
}

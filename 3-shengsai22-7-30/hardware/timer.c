#include "timer.h"


void TimA3_Init(uint16_t ccr0, uint16_t psc)
{
    /*增计数模式初始化*/
    Timer_A_UpModeConfig upConfig;
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                                      //时钟源
    upConfig.clockSourceDivider = psc;                                                     //时钟分频 范围1-64
    upConfig.timerPeriod = ccr0;                                                           //自动重装载值（ARR）
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;                   //禁用 tim溢出中断
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; //启用 ccr0更新中断
    upConfig.timerClear = TIMER_A_DO_CLEAR;                                                // Clear value

    /*初始化定时器A*/
    MAP_Timer_A_configureUpMode(TIMER_A3_BASE, &upConfig);

    /*选择模式开始计数*/
    MAP_Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    /*清除比较中断标志位*/
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    /*开启串口端口中断*/
    MAP_Interrupt_enableInterrupt(INT_TA3_0);
}

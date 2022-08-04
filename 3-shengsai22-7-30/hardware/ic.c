#include "ic.h"
#include "delay.h"

static int v1 = 0;
static int v2 = 0;

int get_v1()
{
	int tmp_v1 = v1;
	v1 = 0;
	return -tmp_v1;
}

int get_v2()
{
	int tmp_v2 = v2;
	v2 = 0;
	return -tmp_v2;
}

void ic_init(void)
{
    /*开启外部中断*/
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN6);
    /*开启端口中断*/
    Interrupt_enableInterrupt(INT_PORT2);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT6);
    /*配置触发方式*/
    GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P6, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P6, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    /*由于编码器一般为开漏输出，所以配置为上拉输入引脚*/
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN6);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN4);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN6);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN6);
}


void PORT2_IRQHandler(void)
{
    uint16_t flag;

    /*获取中断状态*/
    flag = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    /*清除中断标志位*/
    GPIO_clearInterruptFlag(GPIO_PORT_P2, flag);
    /*左轮A相上升沿*/
    if (flag & GPIO_PIN4)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4) == 0)
            v1++;
        else if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4) == 1)
            v1--;
    }
    /*左轮A相下降沿*/
    if (flag & GPIO_PIN6)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4) == 0)
            v1--;
        else if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4) == 1)
            v1++;
    }
}

void PORT4_IRQHandler(void)
{
    uint16_t flag;

    /*获取中断状态*/
    flag = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    /*清除中断标志位*/
    GPIO_clearInterruptFlag(GPIO_PORT_P4, flag);

    /*左轮B相上升沿*/
    if (flag & GPIO_PIN4)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN6) == 0)
            v1--;
        else if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN6) == 1)
            v1++;
    }
    /*左轮B相下降沿*/
    if (flag & GPIO_PIN6)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN4) == 0)
            v1++;
        else if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN4) == 1)
            v1--;
    }
}

void PORT5_IRQHandler(void)
{
    uint16_t flag;

    /*获取中断状态*/
    flag = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    /*清除中断标志位*/
    GPIO_clearInterruptFlag(GPIO_PORT_P5, flag);
    /*右轮A相上升沿*/
    if (flag & GPIO_PIN4)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN6) == 0)
            v2--;
        else if (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN6) == 1)
            v2++;
    }
    /*右轮A相下降沿*/
    if (flag & GPIO_PIN6)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN4) == 0)
            v2++;
        else if (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN4) == 1)
            v2--;
    }
}

void PORT6_IRQHandler(void)
{
    uint16_t flag;

    /*获取中断状态*/
    flag = GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    /*清除中断标志位*/
    GPIO_clearInterruptFlag(GPIO_PORT_P6, flag);
    /*右轮B相上升沿*/
    if (flag & GPIO_PIN4)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6) == 0)
            v2++;
        else if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6) == 1)
            v2--;
    }
    /*右轮B相下降沿*/
    if (flag & GPIO_PIN6)
    {
        if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN4) == 0)
            v2--;
        else if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN4) == 1)
            v2++;
    }
}




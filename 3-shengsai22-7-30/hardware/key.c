#include "key.h"


void KEY_Init(void)
{
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 + GPIO_PIN4);
}

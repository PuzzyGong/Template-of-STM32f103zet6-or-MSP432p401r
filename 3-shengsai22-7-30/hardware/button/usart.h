#ifndef __USART_H
#define __USART_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h> //1.61328125kb
#include "string.h"

void uart_init(int i, uint32_t baudRate, void* T_ptr, uint32_t T_len, 
										 void* R_ptr, uint32_t R_len, int* R_flag);

void uart_singletrans(int i);


#endif

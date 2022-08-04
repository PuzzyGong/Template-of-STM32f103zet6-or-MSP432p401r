#include "usart.h"
#include "baudrate_calculate.h"
#include "stdarg.h"

//*************** USART0支持printf ******************//
#pragma import(__use_no_semihosting)

//标准库需要的支持函数
struct __FILE
{
	int handle;
};
FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
	UART_transmitData(EUSCI_A0_BASE, ch & 0xFF);
	return ch;
}

//***************************************************//
unsigned int	GPIO_PORT_Px	[4] = {GPIO_PORT_P2, 	GPIO_PORT_P3,	GPIO_PORT_P9,	GPIO_PORT_P1 	};
unsigned int 	T_GPIO_PINx		[4] = {GPIO_PIN3,		GPIO_PIN3,		GPIO_PIN7,		GPIO_PIN3		};
unsigned int 	R_GPIO_PINx		[4] = {GPIO_PIN2,		GPIO_PIN2,		GPIO_PIN6,		GPIO_PIN2		};
unsigned int 	EUSCI_Ax_BASE	[4] = {EUSCI_A1_BASE,	EUSCI_A2_BASE,	EUSCI_A3_BASE,	EUSCI_A0_BASE	};
unsigned int 	INT_EUSCIAx		[4] = {INT_EUSCIA1,		INT_EUSCIA2,	INT_EUSCIA3,	INT_EUSCIA0		};


void* 		T_ptrs 	[4] = {0,0,0,0};
uint32_t 	T_lens 	[4] = {0,0,0,0};
void* 		R_ptrs 	[4] = {0,0,0,0};
uint32_t 	R_lens 	[4] = {0,0,0,0};
int* 		R_flags [4] = {0,0,0,0};

//***************************************************//
void uart_init(int i, uint32_t baudRate, void* T_ptr, uint32_t T_len, 
										 void* R_ptr, uint32_t R_len, int* R_flag)
{
	const eUSCI_UART_ConfigV1 uartConfig =
		{
			EUSCI_A_UART_CLOCKSOURCE_SMCLK,				   // SMCLK Clock Source
			312,										   // BRDIV = 312
			8,											   // UCxBRF = 8
			1,											   // UCxBRS = 1
			EUSCI_A_UART_NO_PARITY,						   // No Parity
			EUSCI_A_UART_LSB_FIRST,						   // MSB First
			EUSCI_A_UART_ONE_STOP_BIT,					   // One stop bit
			EUSCI_A_UART_MODE,							   // UART mode
			EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
			EUSCI_A_UART_8_BIT_LEN						   // 8 bit data length
		};
	i--;  	
	 		
	eusci_calcBaudDividers((eUSCI_UART_ConfigV1 *)&uartConfig, baudRate); //配置波特率

	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_Px[i], R_GPIO_PINx[i], GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_Px[i], T_GPIO_PINx[i], GPIO_PRIMARY_MODULE_FUNCTION);
		
	MAP_UART_initModule(EUSCI_Ax_BASE[i], &uartConfig); // 3.初始化串口
	MAP_UART_enableModule(EUSCI_Ax_BASE[i]);			 // 4.开启串口模块

	MAP_UART_enableInterrupt(EUSCI_Ax_BASE[i], EUSCI_A_UART_RECEIVE_INTERRUPT); // 5.开启串口相关中断
	MAP_Interrupt_enableInterrupt(INT_EUSCIAx[i]);								 // 6.开启串口端口中断
		
	T_ptrs[i] = T_ptr;
	T_lens[i] = T_len;	
	R_ptrs[i] = R_ptr;
	R_lens[i] = R_len;
	R_flags[i] = R_flag;
}

void uart_singletrans(int i)
{
	uint16_t j;

	for (j = 0; j < T_lens[i - 1]; j++)
	{
		MAP_UART_transmitData(EUSCI_Ax_BASE[i - 1], ((char *)T_ptrs[i - 1])[j]);
	}
}


#define RX_BUFFER_MAX 400
uint8_t Rx_Buffer[4][RX_BUFFER_MAX] = {0};
static uint16_t Rx_Buffer_Index[4] = {0};
static uint8_t first_flag[4] = {0};

void EUSCIAx_IRQHandler(int i)
{
	uint16_t j;
	uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_Ax_BASE[i - 1]);
	uint8_t Rx_Buffer_Temp = 0;
	
	if(i == 3 || i == 4)
		first_flag[i - 1] = 1;
	
    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        Rx_Buffer_Temp = MAP_UART_receiveData(EUSCI_Ax_BASE[i - 1]);
		

		if(first_flag[i - 1] == 1)
			if (Rx_Buffer_Index[i - 1] < RX_BUFFER_MAX)
			{
				Rx_Buffer[i - 1][Rx_Buffer_Index[i - 1]] = Rx_Buffer_Temp;
				Rx_Buffer_Index[i - 1]++;
			}
    }
	if(Rx_Buffer_Index[i - 1] == R_lens[i - 1])
	{
		Rx_Buffer_Index[i - 1] = 0;
		*(R_flags[i - 1]) = R_lens[i - 1];
		for (j = 0; j < R_lens[i - 1]; j++)
		{
			((char*)R_ptrs[i - 1])[j] = Rx_Buffer[i - 1][j];
		}
	}
	
	if(i == 1 || i == 2)
	{
		if( Rx_Buffer_Temp == '\n')
		{
			first_flag[i - 1] = 1;
			Rx_Buffer_Index[i - 1] = 0;
		}	
	}
}

void EUSCIA1_IRQHandler(void)
{
	EUSCIAx_IRQHandler(1);
}

void EUSCIA2_IRQHandler(void)
{
	EUSCIAx_IRQHandler(2);
}

void EUSCIA3_IRQHandler(void)
{
	EUSCIAx_IRQHandler(3);
}

void EUSCIA0_IRQHandler(void)
{
	EUSCIAx_IRQHandler(4);
}
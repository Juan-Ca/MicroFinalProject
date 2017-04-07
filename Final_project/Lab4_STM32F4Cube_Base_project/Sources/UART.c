#include <stdlib.h>
#include "stm32f4xx_hal.h" 

UART_HandleTypeDef  UARTHandle;

extern uint32_t pData1[300];
extern uint32_t pData1[300];


void configureUART(void)
{
	UARTHandle.Instance 					= USART2;
	UARTHandle.Init.BaudRate 			= 115200;
	UARTHandle.Init.WordLength 		= UART_WORDLENGTH_8B;
	UARTHandle.Init.StopBits 			= UART_STOPBITS_1;
	UARTHandle.Init.Parity 				= UART_PARITY_NONE;
	UARTHandle.Init.Mode 					= UART_MODE_TX_RX;
	UARTHandle.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
	UARTHandle.Init.OverSampling 	= UART_OVERSAMPLING_16;
	
	HAL_UART_Init(&UARTHandle);
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);  	
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void UART_Transmit(uint8_t Data)
{
	HAL_UART_Transmit(&UARTHandle,&Data,1,100);
}

uint8_t UART_Receive(void)
{
	uint8_t Data;
	uint8_t buff[1200];
	memset(buff,0,1200);
	
	HAL_UART_Receive(&UARTHandle,(uint8_t*)pData1,1200,100);
	return buff[5];
}

void UART_Transmit_Float(float *values)
{
	HAL_UART_Transmit(&UARTHandle,(uint8_t *)values,1200,100);
}
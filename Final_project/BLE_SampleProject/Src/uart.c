/**
  ******************************************************************************
  * File Name          : uart.c
  * Description        : Initialization, configuration and utilization functions,
	* Author						 : Luis Gallet
  * Group              : 10	
	* Version            : 3.0.0
	* Date							 : April 23th, 2017
  ******************************************************************************
  */
  
#include "uart.h"
#include "supporting_functions.h"

extern UART_HandleTypeDef huart1;
extern uint8_t data_buffer[10];

static void GPIO_UART_Init(void);

float x_values[250], y_values[250], z_values[250];

BOOL READY = FALSE;
static int flag = 0;

/* Function: UART_Init()
* Input : UART_HandleTypeDef* huart
* Description: Initialize UART and its interrupt
*/
void UART_Init(UART_HandleTypeDef* huart){
	/*
		Initialize UART TX and RX GPIO pins
	*/
	GPIO_UART_Init();
	
  huart->Instance = USART1;
  huart->Init.BaudRate = 115200;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  
	if (HAL_UART_Init(huart) != HAL_OK)
  {
    printf("HAL_UART_Init error\n");
  }
	Init_NVIC_Interrupt(USART1_IRQn, 0, 1);	
}

/* Function: UART_Receive_Data()
* Input : uint8_t *buffer, uint32_t length
* Description: Enables interrupt to be prepared to receive data
*/
HAL_StatusTypeDef UART_Receive_Data(uint8_t *buffer, uint32_t length){
	HAL_StatusTypeDef status;
	
	status = HAL_UART_Receive_IT(&huart1, buffer, length);
	if(status != HAL_OK){
		printf("HAL_UART_Receive_IT error");
		return status;
	}
	return status;
}

/* Function: UART_Transmit_Data()
* Input : uint8_t *buffer, uint32_t length
* Description: Enables interrupt to send data
*/
HAL_StatusTypeDef UART_Transmit_Data(uint8_t *buffer, uint32_t length){
	HAL_StatusTypeDef status;
	
	status = HAL_UART_Transmit_IT(&huart1, buffer, length);
	if(status != HAL_OK){
		printf("HAL_UART_Transmit_IT error");
		return status;
	}
	return status;
}

/* Function: GPIO_UART_Init()
* Input : none
* Description: Initialize UART GPIOs
*/
void GPIO_UART_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct; 
	/* Peripheral clock enable */
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_GPIOH_CLK_ENABLE();
	/**USART1 GPIO Configuration    
	PA9    ------> USART1_TX
	PA10   ------> USART1_RX 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* Function: HAL_UART_RxCpltCallback()
* Input : UART_HandleTypeDef *huart1
* Description: Callback when data is received
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart1){
	if(flag == 0){
		UART_Receive_Data((uint8_t *)y_values, 1000);
		flag = 1;
	}else if(flag == 1){
		UART_Receive_Data((uint8_t *)z_values, 1000);
		flag = 2;
	}else if(flag == 2){
		printf("recevied\n");
		READY = TRUE;
		flag = 0;
		UART_Receive_Data((uint8_t *)x_values, 1000);
	}
}

/* Function: HAL_UART_TxCpltCallback()
* Input : UART_HandleTypeDef *huart1
* Description: Callback when data is sent
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart1){
	printf("sent\n");
}
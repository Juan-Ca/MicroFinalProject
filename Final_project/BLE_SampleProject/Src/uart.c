#include "uart.h"
#include "supporting_functions.h"

extern UART_HandleTypeDef huart1;
extern uint8_t data_buffer[10];

static void GPIO_UART_Init(void);

extern float x_values[250], y_values[250], z_values[250];

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
	
//	memset(x_values, 0.0, 300);
//	memset(y_values, 0.0, 300);
//	memset(z_values, 0.0, 300);
}

HAL_StatusTypeDef UART_Receive_Data(uint8_t *buffer, uint32_t length){
	HAL_StatusTypeDef status;
	
	status = HAL_UART_Receive_IT(&huart1, buffer, length);
	if(status != HAL_OK){
		printf("HAL_UART_Receive_IT error");
		return status;
	}
	return status;
}

HAL_StatusTypeDef UART_Transmit_Data(uint8_t *buffer, uint32_t length){
	HAL_StatusTypeDef status;
	
	status = HAL_UART_Transmit_IT(&huart1, buffer, length);
	if(status != HAL_OK){
		printf("HAL_UART_Transmit_IT error");
		return status;
	}
	return status;
}

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
char flag = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart1){
	if(huart1->Instance == USART1){
		
		}
		if(!flag){
			UART_Receive_Data((uint8_t *)y_values, 1200);
			flag = 1;
		}else{
			UART_Receive_Data((uint8_t *)z_values, 1200);
			printf("recevied\n");
			for(uint32_t i = 0; i < 300; i++){
				printf("%f,%f,%f\n", x_values[i], y_values[i], z_values[i]);
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart1){
	printf("sent\n");
}
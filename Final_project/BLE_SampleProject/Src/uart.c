#include "uart.h"

static void GPIO_UART_init(void);

void UART_Init(UART_HandleTypeDef* huart1){
	/*
		Initialize UART TX and RX GPIO pins
	*/
	GPIO_UART_init();
	
  huart1->Instance = USART1;
  huart1->Init.BaudRate = 115200;
  huart1->Init.WordLength = UART_WORDLENGTH_8B;
  huart1->Init.StopBits = UART_STOPBITS_1;
  huart1->Init.Parity = UART_PARITY_NONE;
  huart1->Init.Mode = UART_MODE_TX_RX;
  huart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1->Init.OverSampling = UART_OVERSAMPLING_16;
  
	if (HAL_UART_Init(huart1) != HAL_OK)
  {
    printf("HAL_UART_Init error code\n");
  }

}
void GPIO_UART_init(void){
	GPIO_InitTypeDef GPIO_InitStruct; 
	/* Peripheral clock enable */
	__HAL_RCC_USART1_CLK_ENABLE();

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
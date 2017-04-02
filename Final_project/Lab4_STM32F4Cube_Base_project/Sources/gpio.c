//All the GPIO stuff goes here

#include "gpio.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"

GPIO_InitTypeDef USARTGPIOHandle;

void configureUSARTGPIO(void)
{
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	USARTGPIOHandle.Pin = GPIO_PIN_5 | GPIO_PIN_6;
	USARTGPIOHandle.Mode = GPIO_MODE_AF_PP;
	USARTGPIOHandle.Pull = GPIO_NOPULL;
	USARTGPIOHandle.Speed = GPIO_SPEED_FREQ_MEDIUM;
	USARTGPIOHandle.Alternate = GPIO_AF7_USART2;
	
	HAL_GPIO_Init(GPIOD,&USARTGPIOHandle);
}

void configureDACGPIO(void)
{
	GPIO_InitTypeDef init_gpio;

	
	USARTGPIOHandle.Pin = GPIO_PIN_4;
	USARTGPIOHandle.Mode = GPIO_MODE_ANALOG;
	USARTGPIOHandle.Pull = GPIO_NOPULL;
	USARTGPIOHandle.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA,&init_gpio);
}

void configureButton(void)
{
	GPIO_InitTypeDef init_gpio;
	init_gpio.Pin = GPIO_PIN_0;
	init_gpio.Mode = GPIO_MODE_INPUT;
	init_gpio.Pull = GPIO_NOPULL;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &init_gpio);
}

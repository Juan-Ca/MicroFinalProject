
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "interrupts.h"
#include "accelerometer.h"
#include "gpio.h"
#include "main.h"
#include "threads.h"
#include <math.h>

void EXTI0_IRQHandler(void) //HAL_GPIO_EXTI_IRQHandler
{
	setAccelerometerValuesReady();
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	

}

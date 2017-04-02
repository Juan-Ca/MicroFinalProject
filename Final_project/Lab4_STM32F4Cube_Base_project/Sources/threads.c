/*******************************************************************************
  * @file    Thread_LED.c
  * @author  Ashraf Suyyagh
	* @version V1.0.0
  * @date    17-January-2016
  * @brief   This file initializes one LED as an output, implements the LED thread 
  *					 which toggles and LED, and function which creates and starts the thread	
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "accelerometer.h"
#include "UART.h"
extern UART_HandleTypeDef  UARTHandle;
extern uint32_t pData1[300];
extern float yArray[300];
extern float xArray[300];
extern float zArray[300];

int accelerometerValuesReady  = 0;

void USARTThread(void const *argument);
osThreadDef(USARTThread, osPriorityNormal, 1, 0);

void DACThread(void const *argument);
osThreadDef(DACThread, osPriorityNormal, 1, 0);

void accelThread(void const *argument);
osThreadDef(accelThread, osPriorityNormal, 1, 0);

void buttonThread(void const *argument);
osThreadDef(buttonThread, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 *    USART Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startUSARTThread(osThreadId USARTThreadID){
	USARTThreadID = osThreadCreate(osThread(USARTThread ), NULL);
	if(!USARTThreadID) return NULL;
	else return(USARTThreadID);
}
/*----------------------------------------------------------------------------
 *    USART thread content 
 *---------------------------------------------------------------------------*/
void USARTThread(void const *argument){
	uint8_t received;
	int i;
	float *xAnglesOut;
	float *yAnglesOut;
	float *zAnglesOut;
	static const uint8_t Data = 0x01;


	configureUSARTGPIO();
	configureUART();
	UART_Transmit(Data);
	HAL_UART_Receive_IT(&UARTHandle,(uint8_t*) pData1, 1200);
	
	while(1){
		//UART_Transmit(Data);
		//received = UART_Receive();
		
		//osDelay(10);
		if(areAnglesReady())
		{
			UART_Transmit_Float(xArray);

			osDelay(500);
			
			UART_Transmit_Float(yArray);

			osDelay(500);
			UART_Transmit_Float(zArray);
			anglesSent();
		}
	}
}
/*----------------------------------------------------------------------------
 *    USART Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startDACThread(osThreadId DACThreadID){
	DACThreadID = osThreadCreate(osThread(USARTThread ), NULL);
	if(!DACThreadID) return NULL;
	else return(DACThreadID);
}
/*----------------------------------------------------------------------------
 *    USART thread content 
 *---------------------------------------------------------------------------*/
void DACThread(void const *argument){

}
/*----------------------------------------------------------------------------
 *    Accelerometer Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startAccelThread(osThreadId accelThreadID){
	accelThreadID = osThreadCreate(osThread(accelThread ), NULL);
	if(!accelThreadID) return NULL;
	else return(accelThreadID);
}
/*----------------------------------------------------------------------------
 *    Accelerometer thread content 
 *---------------------------------------------------------------------------*/
void accelThread(void const *argument){
	initAccelerometer(getHandleSPI());	
	while(1){
		if(accelerometerValuesReady && isCollectingAngles())
		{
			collectXYZ();
			accelerometerValuesReady = 0;
		}
	}
}

/*----------------------------------------------------------------------------
 *    Accelerometer Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startButtonThread(osThreadId buttonThreadID){
	buttonThreadID = osThreadCreate(osThread(buttonThread ), NULL);
	if(!buttonThreadID) return NULL;
	else return(buttonThreadID);
}
/*----------------------------------------------------------------------------
 *    Accelerometer thread content 
 *---------------------------------------------------------------------------*/
void buttonThread(void const *argument)
{
	while(1)
	{
		osDelay(10);
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
		{
			startCollectingAngles();
		}
	}
}

void setAccelerometerValuesReady(void)
{
	accelerometerValuesReady = 1;
}

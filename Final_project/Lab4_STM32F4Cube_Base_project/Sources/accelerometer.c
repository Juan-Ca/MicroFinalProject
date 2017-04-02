//All the GPIO stuff goes here

#include "gpio.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "LIS3DSH.h"
#include "accelerometer.h"
#include "cmsis_os.h" 

static const uint32_t timerDelay = 10000;
osTimerDef(periodic, stopCollectingAngles); 
osTimerId periodic_id;


int collectingAngles 	= 0;
int anglesReady 			= 0;
int yElement 			= 0;
int xElement 			= 0;
int zElement 			= 0;

float yArray[300];
float xArray[300];
float zArray[300];



void initAccelerometer(SPI_HandleTypeDef * handleSPI){
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 7,7);
	
	
	HAL_SPI_MspInit(handleSPI); // sets up gpio stuff for the SPI config and accelerometer interrupt pin
	
	// configure accelerameter settings
	LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
	LIS3DSH_InitStruct.AA_Filter_BW								= LIS3DSH_AA_BW_50;
	LIS3DSH_InitStruct.Axes_Enable								= LIS3DSH_XYZ_ENABLE;
	LIS3DSH_InitStruct.Continous_Update						= LIS3DSH_ContinousUpdate_Enabled;
	LIS3DSH_InitStruct.Full_Scale									= LIS3DSH_FULLSCALE_2;
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate	= LIS3DSH_DATARATE_25;
	LIS3DSH_InitStruct.Self_Test									= LIS3DSH_SELFTEST_NORMAL;
	LIS3DSH_Init(&LIS3DSH_InitStruct); // must be called before LIS3DSH_DataReadyInterruptConfig, since this also sets up the SPI configuration
	
	// configure accelerometer interrupt
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_IntConfigStruct;
	LIS3DSH_IntConfigStruct.Dataready_Interrupt	= LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	LIS3DSH_IntConfigStruct.Interrupt_signal		= LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_IntConfigStruct.Interrupt_type			= LIS3DSH_INTERRUPT_REQUEST_PULSED;
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_IntConfigStruct);
}


void collectXYZ(void)
{
	float acceleration[3];
	LIS3DSH_ReadACC(acceleration);
	
	float calX;
	float calY;
	float calZ;
	
	float xResult;
	float yResult;
	float zResult;
	
	
	calX = calibrateX(acceleration[0],acceleration[1],acceleration[2]);
	calY = calibrateY(acceleration[0],acceleration[1],acceleration[2]);
	calZ = calibrateZ(acceleration[0],acceleration[1],acceleration[2]);
	
	xResult 		= acceleration[0];
	yResult   	= acceleration[1];
	zResult   	= acceleration[2];
	
	xArray[xElement] 		= xResult;
	yArray[yElement] 		= yResult;
	zArray[zElement] 		= zResult;
	
	xElement++;
	yElement++;
	zElement++;
}

float calibrateX(float rawX, float rawY, float rawZ){
	return (rawX*0.001011535773528 + rawY*-0.000021375722414 + rawZ*-0.000002617250425 -0.024434131248271); 
}

float calibrateY(float rawX, float rawY, float rawZ){
	return (rawX*-0.000022173875989 + rawY*0.000959285589914 + rawZ*-0.000004686872148 -0.010543316906594); 
}

float calibrateZ(float rawX, float rawY, float rawZ){
	return (rawX*-0.000003976693405 + rawY*-0.000008559170730 + rawZ*0.000991525335892 -0.020677895349495); 
}

float calculatePitch(float calX, float calY, float calZ){
	float y2 = pow(calY,2.0);
	float z2 = pow(calZ,2.0);
	float sqrtY2Z2 = sqrt(y2 + z2);
	
	return (atan(calX/sqrtY2Z2)*180.0/3.14159);
}

float calculateRoll(float calX, float calY, float calZ){
	float x2 = pow(calX,2.0);
	float z2 = pow(calZ,2.0);
	float sqrtX2Z2 = sqrt(x2 + z2);
	
	return (atan(calY/sqrtX2Z2)*180.0/3.14159);
}

void startCollectingAngles(void)
{
	
	osStatus status;
	if(periodic_id == 0 && collectingAngles == 0)
	{
		periodic_id = osTimerCreate(osTimer(periodic), osTimerPeriodic, (void *)0);
	}
	
	if (periodic_id && collectingAngles == 0)  {
    status = osTimerStart (periodic_id, timerDelay);                // start timer
    if (status != osOK)  {
      printf("timer not started");
    }
		else
		{
			collectingAngles = 1;
			//printf("sending angles");
		}
	}
}

void stopCollectingAngles(void)
{
	collectingAngles 	= 0;
	anglesReady 			= 1;
	zElement 					= 0;
	yElement 					= 0;
  xElement 					= 0;
	stopTimer();
}

void stopTimer(void)
{
	osStatus status;
	status = osTimerStop(periodic_id);                               // stop timer
  if (status != osOK)  {
    printf("timer not stopped");
  }
}

int isCollectingAngles(void)
{
	return collectingAngles;
}

int areAnglesReady(void)
{
	return anglesReady;
}

int anglesSent()
{
	anglesReady = 0;
}

void getAngles(float *xAnglesOut, float *yAnglesOut, float *zAnglesOut)
{
	anglesSent();
	xAnglesOut 		= xArray;
	yAnglesOut 		= yArray;
	zAnglesOut 		= zArray;
}

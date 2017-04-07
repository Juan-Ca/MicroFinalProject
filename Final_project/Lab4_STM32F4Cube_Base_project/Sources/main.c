/*******************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "gpio.h"
#include "adc.h"
#include "threads.h"

extern osThreadId tid_Thread_LED;
uint32_t readingADC[1];
	
//All handles
SPI_HandleTypeDef 			handleSPI;
DMA_HandleTypeDef  			hdma_dac1;
DMA_HandleTypeDef  			hdma_dac2;
DAC_HandleTypeDef 			hdac1;
DAC_HandleTypeDef 			hdac2;
DAC_ChannelConfTypeDef 	sConfig1;
DAC_ChannelConfTypeDef 	sConfig2;
TIM_HandleTypeDef 			tim_handle6; 
TIM_HandleTypeDef 			tim_handle7; 
//TIM_Base_InitTypeDef    tim_base_handle6;

//Global Pitch and Roll
float pitch = 0;
float roll 	= 0;

void ConfigureDAC(void);
void ConfigureTIM(void);
void ConfigureDMA(void);

uint32_t pData1[300] = { 2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897, 
                                      2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640, 
                                      3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002, 
                                      4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894, 
                                      3858, 3817, 3772, 3722, 3669, 3611, 3550, 3485, 3416, 3344, 
                                      3269, 3191, 3110, 3027, 2941, 2853, 2763, 2671, 2578, 2483, 
                                      2387, 2291, 2194, 2096, 1999, 1901, 1804, 1708, 1612, 1517, 
                                      1424, 1332, 1242, 1154, 1068, 985, 904, 826, 751, 679, 
                                      610, 545, 484, 426, 373, 323, 278, 237, 201, 169, 
                                      141, 118, 100, 87, 79, 75, 76, 82, 93, 109, 
                                      129, 154, 184, 218, 257, 300, 347, 399, 455, 514, 
                                      577, 644, 714, 788, 865, 944, 1026, 1111, 1198, 1287, 
                                      1378, 1471, 1565, 1660, 1756, 1853, 1950, 2047,2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897, 
                                      2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640, 
                                      3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002, 
                                      4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894, 
                                      3858, 3817, 3772, 3722, 3669, 3611, 3550, 3485, 3416, 3344, 
                                      3269, 3191, 3110, 3027, 2941, 2853, 2763, 2671, 2578, 2483, 
                                      2387, 2291, 2194, 2096, 1999, 1901, 1804, 1708, 1612, 1517, 
                                      1424, 1332, 1242, 1154, 1068, 985, 904, 826, 751, 679, 
                                      610, 545, 484, 426, 373, 323, 278, 237, 201, 169, 
                                      141, 118, 100, 87, 79, 75, 76, 82, 93, 109, 
                                      129, 154, 184, 218, 257, 300, 347, 399, 455, 514, 
                                      577, 644, 714, 788, 865, 944, 1026, 1111, 1198, 1287, 
                                      1378, 1471, 1565, 1660, 1756, 1853, 1950, 2047,2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897, 
                                      2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640, 
                                      3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002, 
                                      4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894, 
                                      3858, 3817, 3772, 3722};  
uint32_t pData2[300];

void setPitch(float newPitch)
{
	pitch = newPitch;
}

void setRoll(float newRoll)
{
	roll = newRoll;
}

float getPitch(void)
{
	return pitch;
}

float getRoll(void)
{
	return roll;
}


/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

//HANDLE ACCESSORS

SPI_HandleTypeDef * getHandleSPI(void){
	return &handleSPI;
}

int main (void){
	osThreadId accelThread;
	osThreadId USARTThread;
	osThreadId buttonThread;
	
  osKernelInitialize();                     /* initialize CMSIS-RTOS          */
  HAL_Init();                               /* Initialize the HAL Library     */
  SystemClock_Config();                     /* Configure the System Clock     */

	/* User codes goes here*/
	memset(pData1,0,300);
	memset(pData2,0,300);
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_DAC_CLK_ENABLE();
	__DMA1_CLK_ENABLE(); 
	__DMA2_CLK_ENABLE(); 
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	accelThread 	= startAccelThread(accelThread);
	USARTThread 	= startUSARTThread(USARTThread);
	buttonThread 	= startButtonThread(buttonThread);
	
	if((USARTThread || accelThread)==NULL){
		printf("Something went wrong");
	}
	
	configureDACGPIO();
	ConfigureTIM();
	ConfigureDMA();
	ConfigureDAC();
	InitAccGPIO();
	
	/* User codes ends here*/
  
	osKernelStart();                          /* start thread execution         */

	return 0;
}

void ConfigureDAC(void)
{
	hdac1.Instance = DAC;
	hdac1.DMA_Handle1 = &hdma_dac1;
	
	sConfig1.DAC_OutputBuffer 	= DAC_OUTPUTBUFFER_ENABLE;
	sConfig1.DAC_Trigger				= DAC_TRIGGER_T6_TRGO; 

	HAL_DAC_Init(&hdac1);
	HAL_DAC_ConfigChannel(&hdac1,&sConfig1,DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
	HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1,(uint32_t*)pData1,300,DAC_ALIGN_12B_R);
}

void ConfigureTIM(void)
{
	TIM_MasterConfigTypeDef sMasterConfig1;
	
	__TIM6_CLK_ENABLE();

	tim_handle6.Init.ClockDivision 	= TIM_CLOCKDIVISION_DIV1;
	tim_handle6.Init.CounterMode 		= TIM_COUNTERMODE_UP;
	tim_handle6.Init.Period 				= 13125;											// 42MHz/UpdateFreq = 42MHz/100Hz = period*prescaler
	tim_handle6.Init.Prescaler 			= 32;
	tim_handle6.Instance 						= TIM6;
	
	HAL_TIM_Base_Init(&tim_handle6);
	HAL_TIM_Base_Start(&tim_handle6);
	
	sMasterConfig1.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig1.MasterSlaveMode 		= TIM_MASTERSLAVEMODE_DISABLE;
	
	HAL_TIMEx_MasterConfigSynchronization(&tim_handle6,&sMasterConfig1); // this is needed!
}

void ConfigureDMA(void)
{
	hdma_dac1.Instance = DMA1_Stream5;

	HAL_DMA_DeInit(&hdma_dac1);

	hdma_dac1.Init.Channel 							= DMA_CHANNEL_7;
	hdma_dac1.Init.Direction 						= DMA_MEMORY_TO_PERIPH;
	hdma_dac1.Init.PeriphInc 						= DMA_PINC_DISABLE;
	hdma_dac1.Init.MemInc 							= DMA_MINC_ENABLE;
	hdma_dac1.Init.PeriphDataAlignment	= DMA_PDATAALIGN_WORD;
	hdma_dac1.Init.MemDataAlignment 		= DMA_MDATAALIGN_WORD;
	hdma_dac1.Init.Mode 								= DMA_CIRCULAR;
	hdma_dac1.Init.Priority 						= DMA_PRIORITY_HIGH;
	hdma_dac1.Init.FIFOMode 						= DMA_FIFOMODE_DISABLE;         
	hdma_dac1.Init.FIFOThreshold 				= DMA_FIFO_THRESHOLD_HALFFULL;
	hdma_dac1.Init.MemBurst 						= DMA_MBURST_SINGLE;
	hdma_dac1.Init.PeriphBurst 					= DMA_PBURST_SINGLE; 
	

	
	HAL_DMA_Init(&hdma_dac1);
    
	__HAL_DMA_ENABLE(&hdma_dac1);
 
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);  
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}



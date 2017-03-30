/**
  ******************************************************************************
  * File Name          : uart.h
  * Description        : uart.c configuration functions
	* Author						 : Luis Gallet
	* Version            : 1.0.0
	* Date							 : March 30rd, 2017
  ******************************************************************************
  */
#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"

void UART_Init(UART_HandleTypeDef* huart1);

#endif
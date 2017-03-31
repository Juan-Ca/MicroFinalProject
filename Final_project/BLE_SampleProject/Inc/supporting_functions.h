/**
  ******************************************************************************
  * File Name          : supporting_functions.h
  * Description        : User defined common functions that can be used across projects
	* Author						 : Luis Gallet
	* Version            : 1.0.0
	* Date							 : March 31th, 2016
  ******************************************************************************
  */
#ifndef SUPPORTING_FUNCTIONS_H
#define SUPPORTING_FUNCTIONS_H

#include "stm32f4xx_hal.h"

void Init_NVIC_Interrupt(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);

#endif

/**
  ******************************************************************************
  * File Name          : supporting_functions.c
  * Description        : User defined common functions that can be used across projects
	* Author						 : Luis Gallet
  * Group              : 10	
	* Version            : 3.0.0
	* Date							 : March 20th, 2017
  ******************************************************************************
  */
#include "supporting_functions.h"

/* Function: Init_NVIC_Interrupt()
* Input : IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority
* Description: General function that initialises the interrupt for a specific function
*/
void Init_NVIC_Interrupt(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority){
	HAL_NVIC_SetPriority(IRQn, PreemptPriority, SubPriority);
	HAL_NVIC_EnableIRQ(IRQn);
}
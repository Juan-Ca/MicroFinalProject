#include <stdlib.h>
#include "stm32f4xx_hal.h" 

void configureUART(void);
void UART_Transmit(uint8_t Data);
uint8_t UART_Receive(void);
void UART_Transmit_Float(float *values);

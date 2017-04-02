#ifndef __gpio_H
#define __gpio_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Prototypes */

//All GPIO initialization
void configureUSARTGPIO(void);
void configureDACGPIO(void);
void configureButton(void);





#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */


#include "cmsis_os.h"    

osThreadId startAccelThread(osThreadId accelThreadID);
void accelThread(void const *argument);
void setAccelerometerValuesReady(void);

osThreadId startDACThread(osThreadId DACThreadID);
void DACThread(void const *argument);

osThreadId startUSARTThread(osThreadId USARTThreadID);
void USARTThread(void const *argument);

osThreadId startButtonThread(osThreadId buttonThreadID);
void buttonThread(void const *argument);
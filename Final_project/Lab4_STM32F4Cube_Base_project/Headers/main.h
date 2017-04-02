
uint32_t getReadingADC(void);
ADC_HandleTypeDef * getHandleADC(void);
DMA_HandleTypeDef * getHandleDMA(void);
ADC_ChannelConfTypeDef * getHandleADCChannel(void);
uint32_t * getReadingADCLocation(void);
SPI_HandleTypeDef * getHandleSPI(void);
TIM_HandleTypeDef * getHandleTIM(void);
TIM_OC_InitTypeDef * getInitTIM(void);
GPIO_InitTypeDef * getKeypadGPIO(void);

void setPitch(float newPitch);
void setRoll(float newRoll);
float getPitch(void);
float getRoll(void);

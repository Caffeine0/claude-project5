#ifndef _ADC_H
#define _ADC_H

#include "stm32f10x.h"
#include "Delay.h"

#define AD_GPIO_PORT 		  GPIOA
#define AD_GPIO_CLK 			RCC_APB2Periph_GPIOA
#define AD_GPIO_PIN0 			GPIO_Pin_0//烟雾传感器mq-2

#define Flag_MQ2          80 //烟雾触发的阈值

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
void adc_value(void);

#endif

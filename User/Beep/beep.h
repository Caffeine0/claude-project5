#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f10x.h"
#include "Delay.h"

#define BEEP_GPIO_PORT 		  GPIOB
#define BEEP_GPIO_CLK 			RCC_APB2Periph_GPIOB
#define BEEP_GPIO_PIN 			GPIO_Pin_5

void BEEP_GPIO_Config(void);
void beep_on(void);
void beep_off(void);
void beep_control(void);
void beep_monitor(void);
#endif

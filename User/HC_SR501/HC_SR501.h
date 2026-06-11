#ifndef __HC_SR501_H
#define __HC_SR501_H


#define HC_SR501_GPIO_PORT 		 GPIOA
#define HC_SR501_GPIO_CLK 			RCC_APB2Periph_GPIOA
#define HC_SR501_GPIO_PIN 			GPIO_Pin_1



void HC_SR501_Init(void);
void HC_SR501_Show(void);
#endif


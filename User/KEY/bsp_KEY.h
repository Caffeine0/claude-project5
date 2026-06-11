#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#define KEY0_GPIO_PORT 	GPIOB
#define KEY0_GPIO_PIN 	GPIO_Pin_0
#define KEY0_GPIO_CLK 	RCC_APB2Periph_GPIOB

#define KEY1_GPIO_PORT 	GPIOB
#define KEY1_GPIO_PIN 	GPIO_Pin_1
#define KEY1_GPIO_CLK 	RCC_APB2Periph_GPIOB

#define KEY2_GPIO_PORT 	GPIOA
#define KEY2_GPIO_PIN 	GPIO_Pin_8
#define KEY2_GPIO_CLK 	RCC_APB2Periph_GPIOA

#define KEY_UP_GPIO_PORT GPIOA
#define KEY_UP_GPIO_PIN 	GPIO_Pin_0
#define KEY_UP_GPIO_CLK 	RCC_APB2Periph_GPIOA

#define KEY0_ON  GPIO_ReadInputDataBit(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == 1
#define KEY1_ON  GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 1
#define KEY2_ON  GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 1
#define KEY_UP_ON GPIO_ReadInputDataBit(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN) == 1


void KEY_GPIO_Config(void);
void KEY_Control(void);

#endif
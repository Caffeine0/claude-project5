#ifndef _BSP_KEY_H
#define _BSP_KEY_H

// 野火霸道板 按键定义
// K1 (KEY1) = PA0 - 具有唤醒功能
// K2 (KEY2) = PC13

#define KEY1_GPIO_PORT 	GPIOA
#define KEY1_GPIO_PIN 	GPIO_Pin_0
#define KEY1_GPIO_CLK 	RCC_APB2Periph_GPIOA

#define KEY2_GPIO_PORT 	GPIOC
#define KEY2_GPIO_PIN 	GPIO_Pin_13
#define KEY2_GPIO_CLK 	RCC_APB2Periph_GPIOC

// 按键按下为低电平（外部/内部上拉）
#define KEY1_ON  GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0
#define KEY2_ON  GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0


void KEY_GPIO_Config(void);
void KEY_Control(void);

#endif
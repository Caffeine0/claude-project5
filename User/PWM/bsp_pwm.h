#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#define PWM_LED_GPIO_PORT 	GPIOB
#define PWM_LED_GPIO_PIN 	GPIO_Pin_0
#define PWM_LED_GPIO_CLK 	RCC_APB2Periph_GPIOB

void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);

#endif
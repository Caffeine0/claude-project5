#include "stm32f10x.h"
#include "bsp_pwm.h"

void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(PWM_LED_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruc1;	
	GPIO_InitStruc1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruc1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruc1.GPIO_Pin = PWM_LED_GPIO_PIN;	
	GPIO_Init(PWM_LED_GPIO_PORT, &GPIO_InitStruc1);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_InternalClockConfig(TIM3);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure1;												
	TIM_TimeBaseInitStructure1.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure1.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure1.TIM_Period = 100 - 1;
	TIM_TimeBaseInitStructure1.TIM_Prescaler = 720 - 1;
	TIM_TimeBaseInitStructure1.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure1);

	TIM_OCInitTypeDef TIM_OCInitStructure1;
	TIM_OCStructInit(&TIM_OCInitStructure1);
	TIM_OCInitStructure1.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure1.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure1.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure1.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure1);
		
	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3, Compare);
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM3, Compare);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM3, Compare);
}
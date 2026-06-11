#include "stm32f10x.h"
#include "bsp_pwm.h"
#include "bsp_LED.h"
void PWM_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	

//-----------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStruc1;	
	//红色LED初始化	
	RCC_APB2PeriphClockCmd(RED_LED_GPIO_CLK, ENABLE);
	GPIO_InitStruc1.GPIO_Mode = GPIO_Mode_AF_PP; //GPIO_Mode_Out_PP;
	GPIO_InitStruc1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruc1.GPIO_Pin = GPIO_Pin_0;	
	GPIO_Init(RED_LED_GPIO_PORT, &GPIO_InitStruc1);
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                 // 1选择内部时钟
	TIM_InternalClockConfig(TIM3);                                             // 1初始化时基单元
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure1;												
	TIM_TimeBaseInitStructure1.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure1.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure1.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure1.TIM_Prescaler = 720 - 1;		//PSC
	TIM_TimeBaseInitStructure1.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure1);
	
	TIM_OCInitTypeDef TIM_OCInitStructure1;
	TIM_OCStructInit(&TIM_OCInitStructure1);																						//给结构体赋初始值
	TIM_OCInitStructure1.TIM_OCMode = TIM_OCMode_PWM1;																	//设置输出比较模式
	TIM_OCInitStructure1.TIM_OCPolarity = TIM_OCPolarity_High;		//有效电平为高					//设置输出比较极性
	TIM_OCInitStructure1.TIM_OutputState = TIM_OutputState_Enable;  //输出状态					//设置输出使能
	TIM_OCInitStructure1.TIM_Pulse = 0;		//CCR																				//设置CCR
	TIM_OC3Init(TIM3, &TIM_OCInitStructure1);
	
		
	TIM_Cmd(TIM3, ENABLE);									      //使能计数器
	TIM_CtrlPWMOutputs(TIM3, ENABLE);				              //使能输出通道
	

}


void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3, Compare);
}
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM3, Compare);
}

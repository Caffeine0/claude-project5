#include "stm32f10x.h"
#include "bsp_LED.h"




void RED_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initstruct;
	RCC_APB2PeriphClockCmd(RED_LED_GPIO_CLK, ENABLE);
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstruct.GPIO_Pin = RED_LED_GPIO_PIN;
	
	GPIO_Init(RED_LED_GPIO_PORT,&GPIO_Initstruct);
	
	GPIO_ResetBits(RED_LED_GPIO_PORT,RED_LED_GPIO_PIN);/* …Ë÷√µÕµÁ∆Ω */
}



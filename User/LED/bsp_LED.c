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
	
	GPIO_ResetBits(RED_LED_GPIO_PORT,RED_LED_GPIO_PIN);
}

void YELLOW_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initstruct;
	RCC_APB2PeriphClockCmd(YELLOW_LED_GPIO_CLK, ENABLE);
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstruct.GPIO_Pin = YELLOW_LED_GPIO_PIN;
	
	GPIO_Init(YELLOW_LED_GPIO_PORT,&GPIO_Initstruct);
	
	GPIO_ResetBits(YELLOW_LED_GPIO_PORT,YELLOW_LED_GPIO_PIN);
}

void GREEN_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initstruct;
	RCC_APB2PeriphClockCmd(GREEN_LED_GPIO_CLK, ENABLE);
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstruct.GPIO_Pin = GREEN_LED_GPIO_PIN;
	
	GPIO_Init(GREEN_LED_GPIO_PORT,&GPIO_Initstruct);
	
	GPIO_ResetBits(GREEN_LED_GPIO_PORT,GREEN_LED_GPIO_PIN);
}

void LED_GPIO_Config(void)
{
	RED_LED_GPIO_Config();
	YELLOW_LED_GPIO_Config();
	GREEN_LED_GPIO_Config();
}
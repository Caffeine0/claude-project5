#include "stm32f10x.h"
#include "bsp_KEY.h"
#include "OLED.h"
#include "Delay.h"
#include "usart.h"
#include "bsp_LED.h"
extern int led_flag;
extern int beep_flag;


void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(KEY0_GPIO_CLK | KEY1_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK | KEY_UP_GPIO_CLK, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = KEY0_GPIO_PIN | KEY1_GPIO_PIN;
	GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN | KEY_UP_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
		
}


void KEY_Control(void)
{		
		if(KEY1_ON)
		{
				Delay_ms(20);
			if (KEY1_ON == 1)
			{
				while(KEY1_ON == 1) ;
			
				led_flag=led_flag+1;
				UsartPrintf(USART_DEBUG, "led_flag+1");
				if(led_flag>3)
				{
					led_flag=1;
				}	
			}

		}
	
		if(KEY0_ON)
		{
			UsartPrintf(USART_DEBUG, "关闭报警");
			beep_flag=0;
		}	
}
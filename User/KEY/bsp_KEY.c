#include "stm32f10x.h"
#include "bsp_KEY.h"
#include "OLED.h"
#include "Delay.h"
#include "usart.h"
#include "bsp_LED.h"
extern int led_flag;
extern int beep_flag;

//GPIO_Pin_6 
//GPIO_Pin_7 
//GPIO_Pin_8;  换成15

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	

	GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
		
}


void KEY_Control(void)
{
		

		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
		{
				Delay_ms(20);
			/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 1)
			{
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 1) ; //如果不在按下则跳出循环
			
				led_flag=led_flag+1;
				UsartPrintf(USART_DEBUG, "led_flag+1");
				if(led_flag>3)
				{
					led_flag=1;
				}	
			}

		}
	
//		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
//		{
//			UsartPrintf(USART_DEBUG, "切换灯");
//			led_flag=led_flag+1;
//				UsartPrintf(USART_DEBUG, "led_flag+1");
//				if(led_flag>3)
//				{
//					led_flag=1;
//				}	
//		}
		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
		{
			UsartPrintf(USART_DEBUG, "关闭蜂鸣器");
			beep_flag=0;
		}
	
	

	


		
		
		
		
}



#include "stm32f10x.h"
#include "bsp_KEY_EXTI.h"
#include "Delay.h"
#include "OLED.h"
#include "usart.h"


//NVIC EXTI AFIP GPIO
void KEY_EXTI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	
	//配置红灯和黄灯引脚
	GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IN_FLOATING;  
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
		
	//配置AFIO(打开时钟后就一句话,一个函数就行)
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource9);
	
	//配置EXTI
	EXTI_InitStruct.EXTI_Line=EXTI_Line9;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	//配置NVIC
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组
	
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


//void EXTI9_5_IRQHandler(void)
//{

//	if (EXTI_GetITStatus(EXTI_Line9) == SET)		//判断是否是外部中断14号线触发的中断
//	{
//		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
//		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)
//		{
//			while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1) ; //如果不在按下则跳出循环
//			OLED_Clear();
//			currentMode=currentMode+1; // 切换到下一个模式

//			if(currentMode>4) 
//			{
//				currentMode=1;
//				flag_usart=49;
//			}
//			
//			
//		}
//		EXTI_ClearITPendingBit(EXTI_Line9);		//清除外部中断14号线的中断标志位
//													//中断标志位必须清除
//													//否则中断将连续不断地触发，导致主程序卡死
//	}

//}

#include "stm32f10x.h"
#include "HC_SR501.h"
#include "Delay.h"
#include "OLED.h"
#include "bsp_pwm.h"
#include "beep.h"

extern int led_flag;
void HC_SR501_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HC_SR501_GPIO_CLK, ENABLE);	 //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = HC_SR501_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //浮空输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(HC_SR501_GPIO_PORT, &GPIO_InitStructure);					 //根据设定参数初始化
	GPIO_ResetBits(HC_SR501_GPIO_PORT,HC_SR501_GPIO_PIN);						 // 输出高//GPIO_SetBits
}

void HC_SR501_Show(void)
{
	uint8_t i;
	if(GPIO_ReadInputDataBit(HC_SR501_GPIO_PORT,HC_SR501_GPIO_PIN)==1)
	{
		
		Delay_ms(500);
		
		if(GPIO_ReadInputDataBit(HC_SR501_GPIO_PORT,HC_SR501_GPIO_PIN)==1)
		{

			OLED_ShowChinese(3,7,45);//有
			OLED_ShowChinese(3,8,46);//人
			beep_on();//蜂鸣器
			for (i = 0; i <= 100; i++)
			{
				PWM_SetCompare2(i);
				Delay_ms(10);
			}	
			led_flag=2;
				
		}

		
	}
	else if(GPIO_ReadInputDataBit(HC_SR501_GPIO_PORT,HC_SR501_GPIO_PIN)==0)
	{

		OLED_ShowChinese(3,7,35);//没
		OLED_ShowChinese(3,8,46);//人
		beep_off();//蜂鸣器
	}

}

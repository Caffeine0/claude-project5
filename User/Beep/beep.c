#include "beep.h"
#include "Delay.h"

extern int beep_flag;
extern unsigned short timeCount;
void BEEP_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);	
	
	GPIO_InitStruct.GPIO_Pin  = BEEP_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStruct);
		
  beep_off();  // 初始化时关闭蜂鸣器（设置为高电平）
}

// 关闭蜂鸣器（高电平）
void beep_off(void)
{
	GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);  /* 高电平：不触发 */
}

// 打开蜂鸣器（低电平）
void beep_on(void)
{
	GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);  /* 低电平：触发发声 */
}
void beep_control(void)
{
		beep_on();timeCount=timeCount+10;
		Delay_ms(1000);
		beep_off();timeCount=timeCount+10;
		Delay_ms(1000);
}
void beep_monitor(void)
{
	if(beep_flag==1)
	{
		beep_control();
	}
	else if(beep_flag==0)
	{
		beep_off();
	}
}

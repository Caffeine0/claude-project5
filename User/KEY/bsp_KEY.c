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
    
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);

    // 配置KEY1 (PA0) 和 KEY2 (PC13) 为输入模式
    // 使用内部上拉，按键按下为低电平
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  // 内部上拉输入
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}


void KEY_Control(void)
{		
    // KEY1 控制蜂鸣器
    if(KEY1_ON)
    {
        UsartPrintf(USART_DEBUG, "关闭报警\r\n");
        beep_flag = 0;
    }

    // KEY2 控制LED
    if(KEY2_ON)
    {
        UsartPrintf(USART_DEBUG, "切换灯状态\r\n");
        led_flag = led_flag + 1;
        if(led_flag > 3)
        {
            led_flag = 1;
        }	
    }
}
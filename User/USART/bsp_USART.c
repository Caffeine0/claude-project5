#include "stm32f10x.h"
#include "bsp_USART.h"
#include <stdio.h>

void USART_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//配置USART1_TX端口
	GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	//配置USART1_RX端口
	GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	//配置USART1
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	
	USART_Cmd(USART1, ENABLE);
}


void USART_Sendbyte(USART_TypeDef* pUSARTx,uint8_t ch)
{
	USART_SendData(pUSARTx, ch);

	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
}

void USART_SendArray(USART_TypeDef* pUSARTx,uint8_t *Array,uint16_t Length)//发送数组
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		USART_Sendbyte(pUSARTx,Array[i]);
	}
}

void USART_SendString(USART_TypeDef* pUSARTx,char *String)
{
	uint8_t i;
	for(i=0;String[i] !='\0';i++)
	{
		USART_Sendbyte(USART1,String[i]);
	}
}






int fputc(int ch,FILE *f)//printf函数在打印时调用fputc,现在修改fputc直接输出到串口
{
	USART_Sendbyte(USART1,ch);
	return ch;
}



#ifndef _BSP_USART_H
#define _BSP_USART_H
 
#include <stdio.h>

void USART_GPIO_Config(void); //¶Ë¿ÚÅäÖÃº¯Êý
void USART_Sendbyte(USART_TypeDef* pUSARTx,uint8_t ch);


void USART_SendArray(USART_TypeDef* pUSARTx,uint8_t *Array,uint16_t Length);
void USART_SendString(USART_TypeDef* pUSARTx,char *String);

#endif

 
 
 
 
 
 
 

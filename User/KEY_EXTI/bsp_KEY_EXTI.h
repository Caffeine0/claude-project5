#ifndef _BSP_KEY_EXTI_H
#define _BSP_KEY_EXTI_H
 

#define KEY9_OFF GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0
#define KEY9_ON  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1

void KEY_EXTI_GPIO_Config(void); //¶Ë¿ÚÅäÖÃº¯Êý

 
#endif

 
 
 
 
 
 
 

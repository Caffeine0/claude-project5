#ifndef __BSP_LED_H
#define __BSP_LED_H

#define RED_LED_GPIO_PORT 		GPIOB
#define RED_LED_GPIO_CLK 			RCC_APB2Periph_GPIOB
#define RED_LED_GPIO_PIN 			GPIO_Pin_1
#define RED_LED_ON 						GPIO_SetBits(RED_LED_GPIO_PORT, RED_LED_GPIO_PIN)
#define RED_LED_OFF 					GPIO_ResetBits(RED_LED_GPIO_PORT, RED_LED_GPIO_PIN)




void RED_LED_GPIO_Config(void);


#endif

#include "stm32f10x.h"
#include <stdio.h>

// 全局变量（其他模块引用）
int beep_flag = 0;
int led_flag = 1;
unsigned short timeCount = 80;

// 简单延时函数
void SimpleDelay(uint32_t ms) {
    uint32_t i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 7200; j++);
    }
}

// 发送单个字符
void UART_SendChar(char c) {
    while(!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

// 发送字符串
void UART_SendString(const char *str) {
    while(*str) {
        UART_SendChar(*str++);
    }
}

// 发送数字
void UART_SendNumber(uint32_t num) {
    char buf[12];
    int i = 0;
    if(num == 0) {
        UART_SendChar('0');
        return;
    }
    while(num > 0) {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    while(i > 0) {
        UART_SendChar(buf[--i]);
    }
}

int main(void) {
    // ========== 时钟配置 ==========
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    // ========== 串口配置 (USART1 - PA9/PA10) ==========
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    // PA9: TX (复用推挽输出)
    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
    GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1;

    // PA10: RX (浮空输入)
    GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    USART1->BRR = 0x0045;  // 115200 @ 8MHz
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;

    // ========== LED配置 (PB5 - 野火霸道板绿灯) ==========
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
    GPIOB->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1;  // 推挽输出 50MHz
    GPIOB->BSRR = GPIO_BSRR_BS5;  // 绿灯灭（高电平）

    // ========== 按键配置 (PA0 - 野火霸道板KEY1) ==========
    // PA0配置为浮空输入，野火霸道板有外部上拉电阻
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOA->CRL |= GPIO_CRL_CNF0_0;  // 浮空输入

    SimpleDelay(500);

    // ========== 发送启动信息 ==========
    UART_SendString("\r\n");
    UART_SendString("========================================\r\n");
    UART_SendString("  STM32F103ZET6 测试程序\r\n");
    UART_SendString("========================================\r\n");
    UART_SendString("时钟: HSI 8MHz\r\n");
    UART_SendString("串口: USART1 @ 115200\r\n");
    UART_SendString("LED: PB5 (绿灯)\r\n");
    UART_SendString("KEY: PA0 (KEY1)\r\n");
    UART_SendString("========================================\r\n\r\n");

    // ========== LED测试 - 闪烁3次 ==========
    UART_SendString("[测试] LED闪烁3次...\r\n");
    for(int i = 0; i < 3; i++) {
        GPIOB->BSRR = GPIO_BSRR_BR5;  // 绿灯亮（低电平）
        SimpleDelay(200);
        GPIOB->BSRR = GPIO_BSRR_BS5;  // 绿灯灭（高电平）
        SimpleDelay(200);
    }

    UART_SendString("[测试] LED测试完成!\r\n\r\n");

    // ========== 主循环 ==========
    uint32_t keyCount = 0;
    uint8_t keyPrev = 1;  // 记录上次按键状态
    uint8_t ledMode = 0;  // LED模式: 0=普通开关, 1=呼吸灯, 2=闪烁
    uint8_t ledOn = 0;    // LED当前状态

    UART_SendString("[INFO] 模式说明:\r\n");
    UART_SendString("  短按: 切换LED开关\r\n");
    UART_SendString("  长按(>2s): 切换模式\r\n");
    UART_SendString("  模式0=普通 模式1=呼吸灯 模式2=快闪\r\n\r\n");

    while(1) {
        // 读取按键状态 (PA0 - 低电平有效)
        uint8_t keyNow = (GPIOA->IDR & GPIO_IDR_IDR0);

        // 下降沿检测：上次高电平，这次低电平 = 按键按下
        if(keyPrev && !keyNow) {
            SimpleDelay(20);  // 消抖
            if((GPIOA->IDR & GPIO_IDR_IDR0) == 0) {
                keyCount++;

                // 检测长按（超过2秒切换模式）
                uint32_t pressTime = 0;
                while((GPIOA->IDR & GPIO_IDR_IDR0) == 0 && pressTime < 200) {
                    SimpleDelay(10);
                    pressTime++;
                }

                if(pressTime >= 200) {
                    // 长按切换模式
                    ledMode = (ledMode + 1) % 3;
                    UART_SendString("[MODE] 切换到模式");
                    UART_SendNumber(ledMode);
                    if(ledMode == 0) UART_SendString(" (普通)\r\n");
                    else if(ledMode == 1) UART_SendString(" (呼吸灯)\r\n");
                    else UART_SendString(" (快闪)\r\n");

                    // 切换模式时关闭LED
                    GPIOB->BSRR = GPIO_BSRR_BS5;  // 绿灯灭
                    ledOn = 0;
                } else {
                    // 短按切换LED状态
                    if(ledMode == 0) {
                        // 普通模式
                        if(ledOn) {
                            GPIOB->BSRR = GPIO_BSRR_BS5;  // 绿灯灭
                            UART_SendString("[KEY] 绿灯灭 (");
                        } else {
                            GPIOB->BSRR = GPIO_BSRR_BR5;  // 绿灯亮
                            UART_SendString("[KEY] 绿灯亮 (");
                        }
                        ledOn = !ledOn;
                    } else if(ledMode == 1) {
                        // 呼吸灯模式
                        UART_SendString("[KEY] 切换到呼吸灯模式 (");
                        ledOn = !ledOn;
                    } else {
                        // 快闪模式
                        UART_SendString("[KEY] 切换到快闪模式 (");
                        ledOn = !ledOn;
                    }
                    UART_SendNumber(keyCount);
                    UART_SendString(")\r\n");
                }
            }
        }

        // 呼吸灯和快闪模式自动效果
        if(ledMode == 1 && ledOn) {
            // 呼吸灯：渐亮渐暗
            for(int i = 0; i < 100; i++) {
                GPIOB->BSRR = GPIO_BSRR_BR5;  // 亮
                SimpleDelay(i);
                GPIOB->BSRR = GPIO_BSRR_BS5;  // 灭
                SimpleDelay(100 - i);
                if((GPIOA->IDR & GPIO_IDR_IDR0) == 0) break;  // 按键中断
            }
            for(int i = 100; i > 0; i--) {
                GPIOB->BSRR = GPIO_BSRR_BR5;  // 亮
                SimpleDelay(i);
                GPIOB->BSRR = GPIO_BSRR_BS5;  // 灭
                SimpleDelay(100 - i);
                if((GPIOA->IDR & GPIO_IDR_IDR0) == 0) break;
            }
        } else if(ledMode == 2 && ledOn) {
            // 快闪模式：快速闪烁5次
            for(int i = 0; i < 5; i++) {
                GPIOB->BSRR = GPIO_BSRR_BR5;
                SimpleDelay(50);
                GPIOB->BSRR = GPIO_BSRR_BS5;
                SimpleDelay(50);
            }
        }

        keyPrev = keyNow;
        SimpleDelay(10);
    }
}
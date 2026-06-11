/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	esp8266.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		ESP8266的简单驱动
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络设备驱动
#include "esp8266.h"

//硬件驱动
#include "Delay.h"
#include "usart.h"
#include "bsp_LED.h" //用于调试,

//C库
#include <string.h>
#include <stdio.h>


#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Jwhandsome\",\"Jwhandsome\"\r\n"
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Xiaomi14\",\"Xiaomi14\"\r\n"
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"broker.emqx.io\",1883\r\n"


unsigned char esp8266_buf[128];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;


//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
// 新增宏定义：接收停滞时间（单位：ms，可根据ESP8266返回速度调整，推荐50-100ms）
#define RECV_STALL_TIME  50

_Bool ESP8266_WaitRecive(void)
{
    static unsigned short stall_cnt = 0; // 停滞计数器（静态变量，跨函数调用保留值）

    // 情况1：无任何数据接收，直接返回等待
    if (esp8266_cnt == 0)
    {
        stall_cnt = 0; // 重置停滞计数器
        return REV_WAIT;
    }

    // 情况2：数据有新增，重置停滞计数器
    if (esp8266_cnt != esp8266_cntPre)
    {
        esp8266_cntPre = esp8266_cnt;
        stall_cnt = 0;
        return REV_WAIT;
    }

    // 情况3：数据无新增，累计停滞时间
    if (esp8266_cnt == esp8266_cntPre)
    {
        stall_cnt++;
        Delay_ms(1); // 每次检测延时1ms，累计到STALL_TIME后判定完成
        if (stall_cnt >= RECV_STALL_TIME)
        {
            esp8266_cntPre = 0; // 重置比较基准
            stall_cnt = 0;      // 重置停滞计数器
            return REV_OK;      // 停滞时间足够，判定接收完成
        }
    }

    return REV_WAIT;
}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
    unsigned char timeOut = 200; // 总超时200*10=2000ms（2秒，足够ESP8266响应）

    // 关键：发送指令前先清空缓存，避免旧数据干扰
    ESP8266_Clear();
    // 发送指令（打印发送的指令，方便调试对比）
    UsartPrintf(USART_DEBUG, "Send cmd: %s", cmd);
    Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
    
    while(timeOut--)
    {
        if(ESP8266_WaitRecive() == REV_OK) // 接收完成
        {
           

            // 打印返回数据（区分“有数据”和“无数据”）
//						RED_LED_ON;
//            UsartPrintf(USART_DEBUG, "8266back: ");//无法判断8266状态时调试时才打开
            if (esp8266_cnt > 0)
            {
                // 补字符串结束符（因缓存预留了1字节，无需担心越界）
                esp8266_buf[esp8266_cnt] = '\0';
//                UsartPrintf(USART_DEBUG, "%s\n", esp8266_buf);
            }
            else
            {
                // 明确提示“无数据”，避免误以为是打印错误
//                UsartPrintf(USART_DEBUG, "(No data received)\n");
            }

            // 检索目标关键词
            if(strstr((const char *)esp8266_buf, res) != NULL)
            {
                ESP8266_Clear();
//                RED_LED_OFF; // 指令成功，LED熄灭（可选，方便硬件观察）
                return 0;
            }
        }
        
        Delay_ms(10);
    }

    // 超时处理：明确提示超时+未收到的关键词
//    UsartPrintf(USART_DEBUG, "8266back: Timeout! No '%s' received\n", res);
    ESP8266_Clear();
//    RED_LED_OFF;
    return 1;
}

//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
	{
		Usart_SendString(USART2, data, len);		//发送设备连接请求数据
	}

}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
//				UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		Delay_ms(5);													//延时等待
		timeOut--;
	} while(timeOut>0);
	
	return NULL;														//超时还未找到，返回空指针

}

//==========================================================
//	函数名称：	ESP8266_Init
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//ESP8266复位引脚和IO0引脚
	GPIO_InitTypeDef GPIO_Initure;	
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initure.GPIO_Pin = GPIO_Pin_14;					//GPIOC14-复位
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_Initure);
	
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;					//GPIOA5-IO0
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initure);
	
//    // 使能USART2的RXNE中断（核心！原代码若漏了这步，中断永不触发）
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    // 使能USART2外设
//    USART_Cmd(USART2, ENABLE);
//    // 使能USART2对应的NVIC中断（核心！中断优先级需合理设置）
//    NVIC_InitTypeDef NVIC_InitStruct;
//    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
//    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级（根据系统调整，不低于其他低优先级中断）
//    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStruct);
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
	
	
	
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
	Delay_ms(500);
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
	Delay_ms(1000);
	
	ESP8266_Clear();
	
	UsartPrintf(USART_DEBUG, "0. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))
		Delay_ms(500);

	UsartPrintf(USART_DEBUG, "1. RST\r\n");
	ESP8266_SendCmd("AT+RST\r\n", "");
		Delay_ms(500);
	Delay_ms(500);
	ESP8266_SendCmd("AT+CIPCLOSE\r\n", "");
		Delay_ms(500);
	
	
	UsartPrintf(USART_DEBUG, "2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		Delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "3. AT+CWDHCP\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		Delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "4. CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		Delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "5. CIPSTART\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		Delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "6. ESP8266 Init OK\r\n");

}

//==========================================================
//	函数名称：	USART2_IRQHandler
//
//	函数功能：	串口2收发中断
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void USART2_IRQHandler(void)
{
    uint8_t recv_data = 0; // 临时存储接收的1字节数据

    // 1. 处理接收溢出（ORE）：若溢出，先读取DR寄存器清除溢出标志
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
    {
        USART_ClearFlag(USART2, USART_FLAG_ORE); // 清除溢出标志
        recv_data = USART_ReceiveData(USART2);   // 读取DR寄存器（必须读，否则ORE无法清除）
    }

    // 2. 处理接收非空（RXNE）：正常接收数据
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        recv_data = USART_ReceiveData(USART2); // 读取接收数据（推荐用USART_ReceiveData，更规范）
        
        // 缓存未满时，存入数据（避免溢出）
        if (esp8266_cnt < (sizeof(esp8266_buf) - 1)) // 留1字节给'\0'，避免后续补结束符时越界
        {
            esp8266_buf[esp8266_cnt++] = recv_data;
        }
        else
        {
            esp8266_cnt = 0; // 缓存溢出时重置，避免后续数据完全失效
            memset(esp8266_buf, 0, sizeof(esp8266_buf));
        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除中断 pending 位（而非Flag，中断用PendingBit更准确）
    }
}
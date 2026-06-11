#include "stm32f10x.h"
#include "beep.h"
#include "Delay.h"
#include "ADC.h"
#include "bsp_USART.h"
#include "OLED.h"
#include <stdio.h>
#include "bsp_LED.h"
#include "bsp_pwm.h"
#include "bsp_KEY.h"
#include "bsp_KEY_EXTI.h"
#include "bsp_dht11.h"
#include "bsp_ds1302.h"
#include "HC_SR501.h"
#include "usart.h"

#include "esp8266.h"
#include "MqttKit.h"
#include "onenet.h"
#include <string.h>

const char devPubTopic[] = "/JW/pub";//发布的主题
const char *devSubTopic[] = {"/jw/sub"};	//kylinBoard//订阅的topic


// 全局变量
struct TIMEData
{
	u16 year;
	u8  month;
	u8  day;
	u8  hour;
	u8  minute;
	u8  second;
	u8  week;
};

extern struct TIMEData TimeData;
extern u8 read_time1[7];
int beep_flag = 0,led_flag=1;

extern uint8_t dat[5];//温湿度数组
extern uint16_t AD0;	//定义AD值变量

unsigned short timeCount = 80;	//发送间隔变量	
unsigned char *dataPtr = NULL;

void OLED_TempHumi(void);
void OLED_Date(void);
void OLED_Alert(void);
void change_lamp(void);
void uploadDataTask(void);




void hard_init()
{	

	BEEP_GPIO_Config();
	beep_off();
	AD_Init();				//AD初始化
	OLED_Init();	
	RED_LED_GPIO_Config();
	PWM_Init();			//PWM初始化

		
	
//	KEY_EXTI_GPIO_Config();  
	KEY_GPIO_Config();                                                                  


	ds1302_gpio_init();//端口初始化
//	ds1032_init();        //要修改时间数据取消注释函数,并进入这个函数修改数值
	ds1032_read_time();
	ds1302_read_jcq();
	HC_SR501_Init();

	Usart1_Init(115200);							//串口1，打印信息用	
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	UsartPrintf(USART_DEBUG, " Usart init OK\r\n");
										
	OLED_connectWifi();//连接wifi中,账号和密码
	ESP8266_Init();					//初始化ESP8266	
	UsartPrintf(USART_DEBUG, "Tips:	8266硬件初始化完成,接入ONEnet中\r\n");


	while(OneNet_DevLink())			//接入OneNET
	{		
		OLED_establishNetworkConnection();//OneNet网络连接中//8266硬件初始化完成,等待连接ONE NET
	}
	
	OneNet_Subscribe(devSubTopic, 1);	
}

int main(void)
{

	hard_init();
	OLED_Clear();


	while (1) 
	{	

		OLED_TempHumi();//显示温湿度
		OLED_Date();//显示时间
		
		adc_value();//检测烟雾传感器                    //有烟雾会报警
		OLED_ADC();//显示测烟雾传感器数值
		beep_monitor();//UsartPrintf(USART_DEBUG, "检测了BEEP");  //根据beep_flag的值来开关蜂鸣器
		OLED_Alert();
		KEY_Control();                                               //这个有阻塞
		
		HC_SR501_Show();//人体红外   有人的话会打开蜂鸣器(可以用按键关),再渐变亮 后常亮
		
		change_lamp();//按键改变灯状态
		
		
		//下面是上传数据的代码
		if(++timeCount >= 100)//上传时间的间隔在这里修改,100为15秒左右
		{
			uploadDataTask();//上传数据
		}
		dataPtr = ESP8266_GetIPD(3);//尝试从ESP8266模块获取接收到的数据。 //这个至少需要15ms
		if(dataPtr != NULL)//如果返回的dataPtr不为NULL，即存在接收到的数据，那么将调用OneNet_RevPro函数处理这些数据
			OneNet_RevPro(dataPtr);

	}

}

void uploadDataTask(void)
{
	char PUB_BUFF[256];
			OLED_uploadInProgress();//上传中
			UsartPrintf(USART_DEBUG, "OneNet_Publish马上上传\r\n");

//			OneNet_Publish(devPubTopic, "MQTT Publish 已上传数据\r\n");//调用OneNet_Publish函数，向OneNet平台的"pcTopic"主题发送消息"MQTT Publish Test"。
//			Delay_ms(10);
			sprintf(PUB_BUFF,"{\"MQ2\":%d,\"Temp\":%d,\"humi\":%d,\"Alert\":%d}",AD0,dat[2],dat[0],beep_flag);
			OneNet_Publish(devPubTopic, PUB_BUFF);
			
			timeCount = 0;
			ESP8266_Clear();
//			OLED_ShowString(1,1,"              " );
}


void OLED_Alert()
{
	if(	beep_flag==1)
	{
		OLED_ShowChinese(4,7,47);//警
		OLED_ShowChinese(4,8,48);//报
	}else
	{
		OLED_ShowString(4,13,"    ");
	}

}

void OLED_Date()
{

	Delay_ms(100);

	ds1032_read_realTime();

	OLED_ShowNum(1,1,TimeData.year,2);
	OLED_ShowString(1,3,"-");	
	OLED_ShowNum(1,4,TimeData.month,2);
	OLED_ShowString(1,6,"-");	
	OLED_ShowNum(1,7,TimeData.day,2);

	OLED_ShowNum(2,1,TimeData.hour,2);
	OLED_ShowString(2,3,":");	
	OLED_ShowNum(2,4,TimeData.minute,2);
	OLED_ShowString(2,6,":");	
	OLED_ShowNum(2.,7,TimeData.second,2);

	ds1032_read_time();

}


void OLED_TempHumi()
{
	if(DHT_Read())
	{
		OLED_ShowChinese(3,1,2);    //湿
		OLED_ShowChinese(3,2,3);		//度
		OLED_ShowChar(3,5,':');
		OLED_ShowNum(3,7,dat[0],2);
		OLED_ShowChar(3,9,'%');
		OLED_ShowChinese(4,1,0);		//温
		OLED_ShowChinese(4,2,1);		//度
		OLED_ShowChar(4,5,':');
		OLED_ShowNum(4,7,dat[2],2);
		OLED_ShowChinese(4,5,4);		//°
		OLED_ShowChar(4,10,'C');
	}
}



void change_lamp()
{
	uint8_t i;

	if(led_flag==3)//呼吸灯	
	{									
		UsartPrintf(USART_DEBUG, "led_flag=3呼吸灯\r\n");
		OLED_ShowChinese(2,6,49);//呼
		OLED_ShowChinese(2,7,50);//吸
		OLED_ShowChinese(2,8,44);//灯
		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare3(i);
			Delay_ms(10);
		}		

		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare3(100 - i);
			Delay_ms(10);		
		}
	}
	else if(led_flag==2)//开灯
	{									
		UsartPrintf(USART_DEBUG, "led_flag=2开灯\r\n");
		PWM_SetCompare3(100);

		OLED_ShowString(2,11,"  ");
		OLED_ShowChinese(2,7,20);//开
		OLED_ShowChinese(2,8,44);//灯
	}else if(led_flag==1)//关灯
	{					
	UsartPrintf(USART_DEBUG, "led_flag=1关灯\r\n");
		PWM_SetCompare3(0);	
		OLED_ShowString(2,11,"  ");
		OLED_ShowChinese(2,7,21);//关
		OLED_ShowChinese(2,8,44);//灯
	}		
}



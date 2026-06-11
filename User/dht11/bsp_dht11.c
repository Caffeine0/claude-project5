#include "stm32f10x.h"
#include "bsp_dht11.h"
#include "Delay.h"

uint8_t dat[5] = {0x00,0x00,0x00,0x00,0x00};   //存储读取的温湿度信息
uint32_t sum = 0;     //存放校验时的求和


//设置PB10端口为输出
void DHT11_PortOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

//设置PB10端口为输入
void DHT11_PortInput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}


//对DHT的数据中的一个字节读取函数
uint8_t DHT_Read_Byte(void)
{
	uint8_t temp;       //存放读取到的位数据
	uint8_t ReadDat = 0;
	uint8_t i;
	uint8_t retry = 0;
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0 && retry < 100)   //等待DHT11输出高电平
		{
			Delay_us(1);
			retry++;
		}
		retry = 0;
		Delay_us(30);       //延时30us，由于‘0’代码高电平时间26~28us，‘1’代码高电平时间70us，延时30us可判断
		temp = 0;           //先将寄存器清零
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==1)               //延时30us之后如果还是高电平，证明为1代码
			temp = 1;    //将1存储
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==1 && retry < 100)     //等待信号被拉低，跳出
		{
			Delay_us(1);
			retry++;
		}
		retry = 0;
		ReadDat<<=1;											//数据左移1位，存放新得到的数据
		ReadDat |= temp;									//新得到的数据放到最后1位
	
	}
	return ReadDat;


}

//读取DHT11的温湿度
uint8_t DHT_Read(void)
{
	uint8_t i;
	uint8_t retry = 0;
	DHT11_PortOutput();												//端口方向设置为输出
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);				//端口数据拉低
	Delay_ms(18);														//延时18ms，时序要求
	GPIO_SetBits(GPIOB,GPIO_Pin_10);        //端口数据拉高
	Delay_us(40);												//延时40us
	DHT11_PortInput();									//方向设置为输入
	Delay_us(20);                    //延时20us
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)											//如果读取到低电平，证明DHT11响应
	{
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0 && retry < 100)			//等待变高电平
		{
			Delay_us(1);
			retry++;
		}
		retry = 0;
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==1 && retry < 100)     //等待变低电平
		{
			Delay_us(1);
			retry++;
		}
		retry = 0;
		for(i=0;i<5;i++)
		{
			dat[i] = DHT_Read_Byte();
		}
		Delay_us(50);
	
	}
	sum=dat[0]+dat[1]+dat[2]+dat[3];
	if(dat[4]==(u8)(sum))
	{
		return 1;
	}
	else
		return 0;

}

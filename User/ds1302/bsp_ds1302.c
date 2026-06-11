#include "stm32f10x.h"
#include "bsp_ds1302.h"
#include "delay.h"
//#include "bsp_uart.h"

u8 read_time[7];
//TIMEData TimeData;

 /*
  * SCLK 和 CE初始化
  */
void ds1302_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = CE_PIN; //CE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(CE_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(CE_PORT,CE_PIN);

	GPIO_InitStructure.GPIO_Pin = SCLK_PIN; //SCLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(SCLK_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SCLK_PORT,SCLK_PIN); 
}
 
 /*
  * IO数据端口输出配置
  */
void ds1032_DATAOUT_init()//配置双向I/O端口为输出态
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 
	GPIO_InitStructure.GPIO_Pin = DATA_PIN; //DATA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(DATA_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(DATA_PORT,DATA_PIN);
}
 
 /*
  * IO数据端口输入配置
  */
void ds1032_DATAINPUT_init()//配置双向I/O端口为输入态
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DATA_PIN; //DATA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入 
	GPIO_Init(DATA_PORT, &GPIO_InitStructure);
}
 //---------------------------------------------------------------------------------------------------

/*
 * /向DS1302发送一字节数据
 */
void ds1302_write_onebyte(uint8_t data)//向DS1302发送一字节数据
{
	uint8_t count=0;
	ds1032_DATAOUT_init(); // I/O配置为输出 
	SCLK_L; // 拉低时钟
	for(count=0;count<8;count++)
	{	
		SCLK_L; // 拉低时钟
		//数据从低位开始传送
		if(data&0x01)
		{
			DATA_H;
		}
		else
		{
			DATA_L;
		}//先准备好数据再发送
		SCLK_H; //发送一位数据，clk上升沿
		data>>=1;
	}
}

/*
 * 向DS1302发送指定数据
 */
void ds1302_wirte_rig(uint8_t address,uint8_t data)//向指定寄存器地址发送数据
{
	uint8_t temp1 = address;
	uint8_t temp2 = data;
	
	
	CE_L; // 拉低CE
	SCLK_L; // 拉低SCLK,只有clk为低电平的时候，rst才能被置为高电平  
	CE_H; // 拉高CE,为高电平的时候才能进行数据传输  

	ds1302_write_onebyte(temp1); // 写命令
	ds1302_write_onebyte(temp2); // 写数据
	
	CE_L; // 拉低CE
	SCLK_L; //拉低clk，以备下一次数据发送
}

/*
 * 从DS1302读取数据
 */
uint8_t ds1302_read_rig(uint8_t address)//从指定地址读取一字节数据
{
	uint8_t temp3 = address;
	uint8_t count = 0;
	uint8_t return_data = 0x00;
	
	CE_L; // 拉低CE
	SCLK_L; // 拉低SCLK
	CE_H; // 拉高CE
	
	ds1302_write_onebyte(temp3); // 写地址
	
	ds1032_DATAINPUT_init();//配置I/O口为输入

	for(count = 0;count<8;count++)
	{
		return_data>>=1;
		SCLK_H;
		Delay_us(4);//使高电平持续一段时间
		SCLK_L;		//拉低时钟线，以便于数据的读入 
		Delay_us(14);//延时14us后再去读取电压，更加准确
		if(GPIO_ReadInputDataBit(DATA_PORT,DATA_PIN))//数据线此时为高电平 
		{
			return_data = return_data|0x80;
		}
	}
	
	CE_L; // 拉低CE
	DATA_L; // 拉低SCLK
	
	return return_data; // 返回数据
}

//-----------------------------------------

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

struct TIMEData TimeData;
u8 read_time1[7];

void ds1032_init() //要修改数据用这个函数
{
	ds1302_wirte_rig(0x8e,0x00);//关闭写保护
	ds1302_wirte_rig(0x80,0x00);//seconds37秒
	ds1302_wirte_rig(0x82,0x2);//minutes20分
	ds1302_wirte_rig(0x84,0x10);//hours21时
	ds1302_wirte_rig(0x86,0x7);//date28日
	ds1302_wirte_rig(0x88,0x09);//months8月
	ds1302_wirte_rig(0x8a,0x06);//days星期日
	ds1302_wirte_rig(0x8c,0x25);//year2025年
	ds1302_wirte_rig(0x8e,0x80);//关闭写保护
}

void ds1302_read_jcq()
{
	
	ds1302_wirte_rig(0x8e,0x00);//关闭写保护
	ds1302_wirte_rig(0x80,read_time1[0]);
	ds1302_wirte_rig(0x82,read_time1[1]);
	ds1302_wirte_rig(0x84,read_time1[2]);
	ds1302_wirte_rig(0x86,read_time1[3]);
	ds1302_wirte_rig(0x88,read_time1[4]);
	ds1302_wirte_rig(0x8a,read_time1[5]);
	ds1302_wirte_rig(0x8c,read_time1[6]);
	ds1302_wirte_rig(0x8e,0x80);//关闭写保护
	
	
}

 
void ds1032_read_time()
{
	read_time1[0]=ds1302_read_rig(0x81);//读秒
	read_time1[1]=ds1302_read_rig(0x83);//读分
	read_time1[2]=ds1302_read_rig(0x85);//读时
	read_time1[3]=ds1302_read_rig(0x87);//读日
	read_time1[4]=ds1302_read_rig(0x89);//读月
	read_time1[5]=ds1302_read_rig(0x8B);//读星期
	read_time1[6]=ds1302_read_rig(0x8D);//读年
}
 
void ds1032_read_realTime()
{
	ds1032_read_time();  //BCD码转换为10进制																			//读取数组read_time1数据
	TimeData.second=(read_time1[0]>>4)*10+(read_time1[0]&0x0f);
	TimeData.minute=((read_time1[1]>>4)&(0x07))*10+(read_time1[1]&0x0f);
	TimeData.hour=(read_time1[2]>>4)*10+(read_time1[2]&0x0f);
	TimeData.day=(read_time1[3]>>4)*10+(read_time1[3]&0x0f);
	TimeData.month=(read_time1[4]>>4)*10+(read_time1[4]&0x0f);
	TimeData.week=read_time1[5];
	TimeData.year=(read_time1[6]>>4)*10+(read_time1[6]&0x0f)+2000;
}




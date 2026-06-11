#ifndef __DSP_DS1302_H
#define __DSP_DS1302_H       



//#define WrEnDisCmd  0x8e  //写允许/禁止指令代码
//#define WrEnDat     0x00 //写允许数据
//#define WrDisDat    0x80 //写禁止数据
//#define OscEnDisCmd 0x80 //振荡器允许/禁止指令代码
//#define OscEnDat    0x00 //振荡器允许数据
//#define OscDisDat   0x80 //振荡器禁止数据
//#define WrMulti     0xbe //写入多个字节的指令代码
//#define WrSingle    0x84 //写入单个字节的指令代码
//#define RdMulti  		0xbf //读出多个字节的指令代码


//c15
#define SCLK_L  	GPIO_ResetBits(GPIOB,GPIO_Pin_12) // 拉低时钟线
#define SCLK_H  	GPIO_SetBits(GPIOB,GPIO_Pin_12) // 拉高时钟线
#define SCLK_PIN 	GPIO_Pin_12
#define SCLK_PORT GPIOB
//c14
#define DATA_L  	GPIO_ResetBits(GPIOB,GPIO_Pin_13) // 拉低数据线
#define DATA_H  	GPIO_SetBits(GPIOB,GPIO_Pin_13) // 拉高数据线
#define DATA_PIN 	GPIO_Pin_13
#define DATA_PORT GPIOB
//a0
#define CE_L    	GPIO_ResetBits(GPIOB,GPIO_Pin_14) // 拉低使能位
#define CE_H    	GPIO_SetBits(GPIOB,GPIO_Pin_14) // 拉高使能位
#define CE_PIN 		GPIO_Pin_14
#define CE_PORT 	GPIOB

void ds1302_wirte_rig(uint8_t address,uint8_t data);
uint8_t ds1302_read_rig(uint8_t address);
void ds1302_gpio_init(void);

void ds1032_init(void);

void ds1302_read_jcq(void);//写入电池

void ds1032_read_time(void);
void ds1032_read_realTime(void);


#endif 



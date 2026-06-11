#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H


void DHT11_PortOutput(void);
void DHT11_PortInput(void);
uint8_t DHT_Read_Byte(void);
uint8_t DHT_Read(void);

#endif


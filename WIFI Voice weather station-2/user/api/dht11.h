#ifndef _DHT11_H
#define _DHT11_H

#include "stm32f10x.h"
#define DHT11_L		(GPIO_ResetBits(GPIOG,GPIO_Pin_11))	//reset设置低电平
#define DHT11_H		(GPIO_SetBits(GPIOG,GPIO_Pin_11))		//设置为高电平
#define DHT11_DATA	 (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11))

typedef struct 
{
	u8 TMP;
	u8 HUM;
} DHT_Data;
extern DHT_Data dht11_data;


void DHT11_IOConfig(u8 mode);
u8 Get_dhtdata(void);


#endif



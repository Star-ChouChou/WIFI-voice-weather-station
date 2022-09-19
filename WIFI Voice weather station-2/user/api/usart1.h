#ifndef USART1_H
#define USART1_H

#include "stm32f10x.h"

#define  Load_BASEADDR  0x000000
#define MAX_SIZE 4096


typedef struct 
{
	u8 RX_buff[2][MAX_SIZE];//保存接收到的数据
	u16 RX_count;		 //记录接收数据的数量
	u8 index;			//记录二维数组下标值 0 1 
	vu8 USART_RevOverFlag; //作为接收完成的标志
	
}USART1_DataStruct;

extern USART1_DataStruct  USART1_Data;

void USART1_Config(void);
void USART1_SendData(u8 data);	
void USART1_SendStr(char * p);
void USART1_SendArray(u8 Arr[] ,u16 DataLenth);

void Fond_Load(void);

#endif

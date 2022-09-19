#include "usart1.h"

USART1_DataStruct  USART1_Data={0};


/*
USART1_TX	PA9		复用推挽	
USART1_RX	PA10	浮空输入 
*/
void USART1_Config(void)
{
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//配IO口模式
	GPIO_InitTypeDef GPIO_InitStruct;//定义结构体变量
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	//配USART的工作模式 
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate = 115200;//波特率
	USART1_InitStruct.USART_WordLength =USART_WordLength_8b;//数据位长度
	//硬件控制失能
	USART1_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//使能发送 和 接收 功能
	USART1_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//停止位1位
	USART1_InitStruct.USART_StopBits = USART_StopBits_1;
	//奇偶校验  不开
	USART1_InitStruct.USART_Parity = USART_Parity_No;
	USART_Init(USART1,&USART1_InitStruct);
	//使能串口1
	USART_Cmd(USART1,ENABLE);
	
	//接收中断配置
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,0);//00 01
	
}



void USART1_IRQHandler(void)
{	
	//10ms
	if(USART_GetITStatus(USART1,USART_IT_RXNE) ==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		u8 data = USART1->DR;
		//保存数据    收足一定字节之后  把收到的数据写入到FLASH
		USART1_Data.RX_buff[USART1_Data.index][ USART1_Data.RX_count ]=data;
		USART1_Data.RX_count++;
		
		if(USART1_Data.RX_count>=4096)
		{
			USART1_Data.index = !USART1_Data.index;//换个缓冲区 切换数组下标
			USART1_Data.RX_count=0;
			USART1_Data.USART_RevOverFlag = 1;
		}
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE) ==SET)			//空闲中断
	{
		u8 flag= USART1->SR ;
		flag= USART1->DR ;//清除中断
		USART1_Data.USART_RevOverFlag = 2;
	}	
	
}
#include "stdio.h"
#include "spi_flash.h"


u32 load_NO=0;

void Fond_Load(void)
{

		if(USART1_Data.USART_RevOverFlag == 1)
		{
			USART1_Data.USART_RevOverFlag=0;
			//扇区擦除
			printf("[%d]\r\n",load_NO);
			sFLASH_EraseSector(Load_BASEADDR+4096*load_NO);
			//数据写入
			sFLASH_WriteBuffer(USART1_Data.RX_buff[!USART1_Data.index],Load_BASEADDR+4096*load_NO ,4096);
			//等带最近一次操作完成
			sFLASH_WaitForWriteEnd();
			
			load_NO++;//下载的次数记录
			
		}else if(USART1_Data.USART_RevOverFlag == 2)
		{
			USART1_Data.USART_RevOverFlag=0;
			//扇区擦除
			printf("*");
			printf("[%d]\r\n",load_NO);
			sFLASH_EraseSector(Load_BASEADDR+4096*load_NO);
			//数据写入
			sFLASH_WriteBuffer(USART1_Data.RX_buff[USART1_Data.index],Load_BASEADDR+4096*load_NO ,4096);
			//等带最近一次操作完成
			sFLASH_WaitForWriteEnd();
//			load_NO++;//下载的次数记录
			
		}	
}



void USART1_SendStr(char * p)
{
	while(*p != '\0')
	{
		USART1_SendData(*p);
		p++;
	}
}
void USART1_SendArray(u8 Arr[] ,u16 DataLenth)
{
	u16 i=0;
	while(DataLenth --)
	{
		USART1_SendData(Arr[i++]);
	}
}

void USART1_SendData(u8 data)
{
	//等待上一帧数据发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET)
	{}
	USART_SendData(USART1,data);	
			
}

#include "stdio.h"
int fputc(int fc, FILE*fp)
{
	USART1_SendData(fc);
	return fc;
}









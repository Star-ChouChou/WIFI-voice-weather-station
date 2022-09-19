#include "usart1.h"

USART1_DataStruct  USART1_Data={0};


/*
USART1_TX	PA9		��������	
USART1_RX	PA10	�������� 
*/
void USART1_Config(void)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//��IO��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;//����ṹ�����
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	//��USART�Ĺ���ģʽ 
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate = 115200;//������
	USART1_InitStruct.USART_WordLength =USART_WordLength_8b;//����λ����
	//Ӳ������ʧ��
	USART1_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//ʹ�ܷ��� �� ���� ����
	USART1_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//ֹͣλ1λ
	USART1_InitStruct.USART_StopBits = USART_StopBits_1;
	//��żУ��  ����
	USART1_InitStruct.USART_Parity = USART_Parity_No;
	USART_Init(USART1,&USART1_InitStruct);
	//ʹ�ܴ���1
	USART_Cmd(USART1,ENABLE);
	
	//�����ж�����
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
		//��������    ����һ���ֽ�֮��  ���յ�������д�뵽FLASH
		USART1_Data.RX_buff[USART1_Data.index][ USART1_Data.RX_count ]=data;
		USART1_Data.RX_count++;
		
		if(USART1_Data.RX_count>=4096)
		{
			USART1_Data.index = !USART1_Data.index;//���������� �л������±�
			USART1_Data.RX_count=0;
			USART1_Data.USART_RevOverFlag = 1;
		}
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE) ==SET)			//�����ж�
	{
		u8 flag= USART1->SR ;
		flag= USART1->DR ;//����ж�
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
			//��������
			printf("[%d]\r\n",load_NO);
			sFLASH_EraseSector(Load_BASEADDR+4096*load_NO);
			//����д��
			sFLASH_WriteBuffer(USART1_Data.RX_buff[!USART1_Data.index],Load_BASEADDR+4096*load_NO ,4096);
			//�ȴ����һ�β������
			sFLASH_WaitForWriteEnd();
			
			load_NO++;//���صĴ�����¼
			
		}else if(USART1_Data.USART_RevOverFlag == 2)
		{
			USART1_Data.USART_RevOverFlag=0;
			//��������
			printf("*");
			printf("[%d]\r\n",load_NO);
			sFLASH_EraseSector(Load_BASEADDR+4096*load_NO);
			//����д��
			sFLASH_WriteBuffer(USART1_Data.RX_buff[USART1_Data.index],Load_BASEADDR+4096*load_NO ,4096);
			//�ȴ����һ�β������
			sFLASH_WaitForWriteEnd();
//			load_NO++;//���صĴ�����¼
			
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
	//�ȴ���һ֡���ݷ������
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









#include "key.h"
#include "delay.h"


//KEY1 PA0   ���� ��   û��  �� 	��������ģʽ	
//KEY2 PC4   ���� ��   û��  �� 	��������ģʽ
//KEY3 PC5   ���� ��   û��  �� 	��������ģʽ
//KEY4 PC6   ���� ��   û��  �� 	��������ģʽ

void KEY_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ȥ�ֲ�����   
	
	GPIO_InitTypeDef KEY_PinInitType ;
	KEY_PinInitType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	KEY_PinInitType.GPIO_Speed = GPIO_Speed_50MHz;
	KEY_PinInitType.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOA,&KEY_PinInitType);
	
	KEY_PinInitType.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6;
	GPIO_Init(GPIOC,&KEY_PinInitType);

}

u8 KEY_Check(void)
{
	u8 flag=0;
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0)  //�жϰ����Ƿ񱻰��� ����  ��ʱ
	{	
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == Bit_RESET) 
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0) 
			{}	
			flag=4;
		}	
	}	
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0)  //�жϰ���3�Ƿ񱻰��� ����  ��ʱ
	{
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0)
			{}
			flag=3;
		}	
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0)  //�жϰ���3�Ƿ񱻰��� ����  ��ʱ
	{
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0)
			{}
			flag=2;
		}	
	}	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  == 1)  //�жϰ���1 PA0�Ƿ񱻰���
	{	
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  == 1)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  == 1)
			{}	
			flag=1;
		}	
	}	
	
	return flag;
}

//KEY1 -- PA0 -- EXTI0
void Exti0_Config(void)
{
	//1.PA0����Ϊ��������
	KEY_Init();
	//2.EXTI0ӳ�䵽PA0
	//2.1 ��AFIO�Ĵ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//2.2 EXTI0ӳ�䵽PA0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	//3.����EXIT0
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;//�ⲿ�ж�0
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ��
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
	EXTI_Init(&EXTI_InitStruct);
	//4.NVIC:EXTI0�ж�ͨ��
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}
#include "beep.h"
//�жϷ�����
void EXTI0_IRQHandler(void)
{
	//1.�ж��Ƿ����
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		//2.���жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}



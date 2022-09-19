#include "beep.h"


void BEEP_Init(void)
{
	//������   PC0  �������ģʽ
	//��ʱ��   
	//RCC->APB2ENR |= 1<<4;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//��IOģʽ
	//	GPIOC->CRL &= ~0X0F;
	//	GPIOC->CRL |=  0X03;
	
	GPIO_InitTypeDef BEEP_TYPE;
	BEEP_TYPE.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	BEEP_TYPE.GPIO_Speed = GPIO_Speed_50MHz;
	BEEP_TYPE.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_Init(GPIOC,&BEEP_TYPE);
}

void BEEP_ON(void)
{
	
	GPIOC->BSRR  |=	0X1;	//��������
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_15);
}




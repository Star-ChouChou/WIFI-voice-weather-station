#include "beep.h"


void BEEP_Init(void)
{
	//蜂鸣器   PC0  推挽输出模式
	//开时钟   
	//RCC->APB2ENR |= 1<<4;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//配IO模式
	//	GPIOC->CRL &= ~0X0F;
	//	GPIOC->CRL |=  0X03;
	
	GPIO_InitTypeDef BEEP_TYPE;
	BEEP_TYPE.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	BEEP_TYPE.GPIO_Speed = GPIO_Speed_50MHz;
	BEEP_TYPE.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_Init(GPIOC,&BEEP_TYPE);
}

void BEEP_ON(void)
{
	
	GPIOC->BSRR  |=	0X1;	//开蜂鸣器
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_15);
}




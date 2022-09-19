#include "key.h"
#include "delay.h"


//KEY1 PA0   按下 高   没按  低 	浮空输入模式	
//KEY2 PC4   按下 低   没按  高 	浮空输入模式
//KEY3 PC5   按下 低   没按  高 	浮空输入模式
//KEY4 PC6   按下 低   没按  高 	浮空输入模式

void KEY_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//去手册里找   
	
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
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0)  //判断按键是否被按下 消抖  延时
	{	
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == Bit_RESET) 
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0) 
			{}	
			flag=4;
		}	
	}	
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0)  //判断按键3是否被按下 消抖  延时
	{
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0)
			{}
			flag=3;
		}	
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0)  //判断按键3是否被按下 消抖  延时
	{
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0)
			{}
			flag=2;
		}	
	}	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  == 1)  //判断按键1 PA0是否被按下
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
	//1.PA0配置为浮空输入
	KEY_Init();
	//2.EXTI0映射到PA0
	//2.1 打开AFIO寄存器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//2.2 EXTI0映射到PA0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	//3.配置EXIT0
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;//外部中断0
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
	EXTI_Init(&EXTI_InitStruct);
	//4.NVIC:EXTI0中断通道
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}
#include "beep.h"
//中断服务函数
void EXTI0_IRQHandler(void)
{
	//1.判断是否挂起
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		//2.清中断标志位
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}



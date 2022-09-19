#include "led.h"
#include "delay.h"

/*
 ***********************************
 *函数名：Led_Config
 *函数功能：初始化Led灯 
 *				LED1  	PE2
 *				LED2	PE3
 *				LED3  	PE4
 *				LED4	PE5
 *			IO输出低  亮
 *			IO输出高  灭
 ***********************************
 */
void Led_Config(void)
{


	
	LED1_CLKCMD(LED1_PORTCLK,ENABLE);
	LED2_CLKCMD(LED2_PORTCLK,ENABLE);
	LED3_CLKCMD(LED3_PORTCLK,ENABLE);
	LED4_CLKCMD(LED4_PORTCLK,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;//定义结构体变量
	
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = LED1Pin;
	GPIO_Init(LED1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LED2Pin;
	GPIO_Init(LED2_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LED3Pin;
	GPIO_Init(LED3_PORT,&GPIO_InitStruct);	
	
	LED_Off(LED1_PORT,LED1Pin);
	LED_Off(LED2_PORT,LED2Pin);
	LED_Off(LED3_PORT,LED3Pin);
	LED_Off(LED4_PORT,LED4Pin);
}

void Led_Switch(void)
{
	LED1_Toggle;
}


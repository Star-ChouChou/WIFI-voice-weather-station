#include "led.h"
#include "delay.h"

/*
 ***********************************
 *��������Led_Config
 *�������ܣ���ʼ��Led�� 
 *				LED1  	PE2
 *				LED2	PE3
 *				LED3  	PE4
 *				LED4	PE5
 *			IO�����  ��
 *			IO�����  ��
 ***********************************
 */
void Led_Config(void)
{


	
	LED1_CLKCMD(LED1_PORTCLK,ENABLE);
	LED2_CLKCMD(LED2_PORTCLK,ENABLE);
	LED3_CLKCMD(LED3_PORTCLK,ENABLE);
	LED4_CLKCMD(LED4_PORTCLK,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;//����ṹ�����
	
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


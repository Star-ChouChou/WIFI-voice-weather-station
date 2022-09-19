#include "useled.h"

/*
 ***********************************
 *��������led_Init(n);
 *�������ܣ���ʼ��Led�� 
 *				LED1  PE2
 *				LED2	PE3
 *				LED3  PE4
 *				LED4	PE5
 *			IO�����  ��
 *			IO�����  ��
 *			����Ĳ���Ϊ1~4��int���ͷ���ֱ�ӽ�������
 *			����������������ǶԵڼ����Ʋ���
 ***********************************
 */


//��ʼ����n��led��
void led_Init(int n)
{
	if(n>5||n<=0)
		return;
	RCC->APB2ENR |= (0x01<<6);	//��GPIOE�˿�ʱ��
	GPIOE->CRL &= ~(0xF<<((1+n)*4));
	GPIOE->CRL |=	0x03<<((1+n)*4);			//PE2����Ϊ�������ģʽ��ʱ��Ĭ�ϵ���led
	GPIOE->ODR |= 0x01<<(1+n);			//��ʼ�����֮���ȹر�led��
}

//�򿪵�n��ledָʾ��
void led_on(int n)
{
	if(n>5||n<=0)
		return;
	GPIOE->ODR &= ~(0x01<<(1+n));			//��led��
}


//�رյ�n��led��
void led_off(int n)
{
	if(n>5||n<=0)
		return;
	GPIOE->ODR |= 0x01<<(1+n);			//�ر�led��
}

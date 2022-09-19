#include "useled.h"

/*
 ***********************************
 *函数名：led_Init(n);
 *函数功能：初始化Led灯 
 *				LED1  PE2
 *				LED2	PE3
 *				LED3  PE4
 *				LED4	PE5
 *			IO输出低  亮
 *			IO输出高  灭
 *			传入的参数为1~4的int类型否则直接结束函数
 *			传入的是整数几就是对第几个灯操作
 ***********************************
 */


//初始化第n个led灯
void led_Init(int n)
{
	if(n>5||n<=0)
		return;
	RCC->APB2ENR |= (0x01<<6);	//开GPIOE端口时钟
	GPIOE->CRL &= ~(0xF<<((1+n)*4));
	GPIOE->CRL |=	0x03<<((1+n)*4);			//PE2设置为推挽输出模式此时会默认点亮led
	GPIOE->ODR |= 0x01<<(1+n);			//初始化完成之后先关闭led灯
}

//打开第n个led指示灯
void led_on(int n)
{
	if(n>5||n<=0)
		return;
	GPIOE->ODR &= ~(0x01<<(1+n));			//打开led灯
}


//关闭第n个led灯
void led_off(int n)
{
	if(n>5||n<=0)
		return;
	GPIOE->ODR |= 0x01<<(1+n);			//关闭led灯
}

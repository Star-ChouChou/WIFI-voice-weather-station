#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#if !defined  USE_STDPERIPH_DRIVER
#define LED1_Off (GPIOE->ODR |=(0x01<<2))
#define LED1_On  (GPIOE->ODR &=~(0x01<<2))

#define LED1_TTL(x) (x?(GPIOE->ODR &=~(0x01<<2)):(GPIOE->ODR |=(0x01<<2)))

#define LED_On(port,pin) (port->ODR &=~(0x01<<pin))
#define LED_Off(port,pin) (port->ODR |=(0x01<<pin))
#else

#define LED1_CLKCMD  	RCC_APB2PeriphClockCmd
#define LED1_PORTCLK    RCC_APB2Periph_GPIOE
#define LED2_CLKCMD  	RCC_APB2PeriphClockCmd
#define LED2_PORTCLK    RCC_APB2Periph_GPIOE
#define LED3_CLKCMD  	RCC_APB2PeriphClockCmd
#define LED3_PORTCLK    RCC_APB2Periph_GPIOE
#define LED4_CLKCMD  	RCC_APB2PeriphClockCmd
#define LED4_PORTCLK    RCC_APB2Periph_GPIOE



#define LED1_PORT   GPIOE
#define LED2_PORT   GPIOE
#define LED3_PORT   GPIOE
#define LED4_PORT   GPIOE


#define LED1Pin  GPIO_Pin_2
#define LED2Pin  GPIO_Pin_3
#define LED3Pin  GPIO_Pin_4
#define LED4Pin  GPIO_Pin_5

#define LED_On(port,pin)  GPIO_ResetBits(port,pin)
#define LED_Off(port,pin) GPIO_SetBits(port,pin)





#define LED1_Toggle (LED1_PORT->ODR ^=(0x01<<2))
#define LED2_Toggle (LED2_PORT->ODR ^=(0x01<<3))
#define LED3_Toggle (LED3_PORT->ODR ^=(0x01<<4))
#define LED4_Toggle (LED4_PORT->ODR ^=(0x01<<5))



#endif


void Led_Config(void);
void Led_Switch(void);

#endif

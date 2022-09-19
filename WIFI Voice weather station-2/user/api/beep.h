//头文件  、宏定义 结构体 函数声明 防止重复编译的结构
#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f10x.h"

#define  BEEP_OFF()  (GPIOC->BRR |= 0X1)


void BEEP_Init(void);
void BEEP_ON(void);

#endif

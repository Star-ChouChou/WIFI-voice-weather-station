//头文件  、宏定义 结构体 函数声明 防止重复编译的结构
#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"

void KEY_Init(void);
u8 KEY_Check(void);
void Exti0_Config(void);


#endif

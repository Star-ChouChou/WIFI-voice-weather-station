//ͷ�ļ�  ���궨�� �ṹ�� �������� ��ֹ�ظ�����Ľṹ
#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f10x.h"

#define  BEEP_OFF()  (GPIOC->BRR |= 0X1)


void BEEP_Init(void);
void BEEP_ON(void);

#endif

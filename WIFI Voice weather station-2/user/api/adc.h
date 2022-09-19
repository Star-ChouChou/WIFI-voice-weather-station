#ifndef _ADC_H
#define _ADC_H

#include "stm32f10x.h"

void light_resistance_adcConfig(void);
u16 get_adcvalue(void);
void MQ_adcConfig(void);
u16 get_MQvalue(void);
extern u16 ADCValue[2];
void ADC1_DMAConfig(void);

void Set_ADC_Init(void);
#endif



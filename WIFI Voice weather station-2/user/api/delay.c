#include "delay.h"
#include "lvgl.h"

void Delay_us(uint32_t time)
{
	while(time--)
		delay_1us();
}

void Delay_ms(uint32_t time)
{
	uint64_t ms = time*1000;
	while(ms--)
		
		delay_1us();
}


u32 task1time=0,task2time=0,task3time=0,task4time=0;
//�δ�ʱ���ж���Ӧ����
void SysTick_Handler(void)
{
	lv_tick_inc(1);//���������ϵͳ�ṩһ��1msһ�ε�����
	task1time++;
	task2time++;
	task3time++;
	task4time++;
}




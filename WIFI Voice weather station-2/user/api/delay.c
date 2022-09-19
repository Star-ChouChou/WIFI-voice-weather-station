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
//滴答定时器中断响应函数
void SysTick_Handler(void)
{
	lv_tick_inc(1);//给任务管理系统提供一个1ms一次的心跳
	task1time++;
	task2time++;
	task3time++;
	task4time++;
}




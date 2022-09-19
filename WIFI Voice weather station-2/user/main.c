#include "lcdshowtask.h"			//各种头文件放在这里面了

int main(void)
{
	SYSTEM_Init();				//*********系统初始化，初始化系统的各种配置，配置接口
	if(Get_info_from_Internet() == -1)
	{
		return -1;			//从网络获取数据失败
	}
	printf("\r\n准备删除初始化标签\r\n");
	Create_LV_Task();							//创建各种功能任务***************************
	Delay_ms(1000);
	playtime(time_stringdata.datetime,atoi(time_stringdata.week));						//欢迎并播报日期和时间

	
	while(1)
	{
			/*LVGL 提供任务管理系统  任务的调度需要这个函数放到循环中而且其他函数不能有较大的延时（阻塞）*/
			lv_task_handler();
	}
}





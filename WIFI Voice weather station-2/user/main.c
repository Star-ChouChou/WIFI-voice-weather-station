#include "lcdshowtask.h"			//����ͷ�ļ�������������

int main(void)
{
	SYSTEM_Init();				//*********ϵͳ��ʼ������ʼ��ϵͳ�ĸ������ã����ýӿ�
	if(Get_info_from_Internet() == -1)
	{
		return -1;			//�������ȡ����ʧ��
	}
	printf("\r\n׼��ɾ����ʼ����ǩ\r\n");
	Create_LV_Task();							//�������ֹ�������***************************
	Delay_ms(1000);
	playtime(time_stringdata.datetime,atoi(time_stringdata.week));						//��ӭ���������ں�ʱ��

	
	while(1)
	{
			/*LVGL �ṩ�������ϵͳ  ����ĵ�����Ҫ��������ŵ�ѭ���ж����������������нϴ����ʱ��������*/
			lv_task_handler();
	}
}





#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"

typedef struct{
	uint8_t hour;
	uint8_t min;
	vu8 flag;
}_alarm_obj;

extern _alarm_obj alarm;

//ʱ��ṹ��
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;	

extern _calendar_obj calendar;	//�����ṹ��

extern u8 const mon_table[12];	//�·��������ݱ�

u8 RTC_TIME_Init(void);
u8 RTC_Init(void);        //��ʼ��RTC
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��
void RTC_SerAlarmSec(uint32_t s);
u8 RTC_Timecorrect(long int seccount);

#endif



#include "rtc.h" 	
#include "delay.h"				//ʹ����ʱ����
#include "stdio.h"
#include "my1680.h"				//ʹ�ò�������
#include "ESP8266.h"			//ʹ�ý�����������ʱ������
#include "stdlib.h"
#include "string.h"

_calendar_obj calendar;//ʱ�ӽṹ�� 
_alarm_obj alarm = {0, 0, 0};
uint32_t alarm_time;


//������У׼ʱ��
u8 RTC_TIME_Init(void)
{
	Delay_ms(1000);
	char sendbuf[512]={0};										//����һ���������
	Connect_Server("api.k780.com",80);				//����ʱ���������ȡ����ʱ��
	strcpy(sendbuf,"GET http://api.k780.com:88/?app=life.time&appkey=10003\
&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json&HTTP/1.1\r\n");
	SendcmdtoServer(sendbuf);
	if(  Analyse_TimeJson() )		//����json�ַ���
	{
		printf("ʱ�����ݽ����ɹ�\r\n");
	}
	else
	{
		printf("ʱ�����ݽ���ʧ��\r\n");
		return 0;
	}
	if( Disconnect_Server() == 0)
	{
		return 0;		//û����ȷ���˳�������������ӣ������쳣����
	}
	long int timestamp = atol(time_stringdata.timestamp);
	timestamp += 8*3600+2;				//����������8��Сʱ��ʱ�䣬��λ���룬Ϊ��ʱ���׼������2s������ִ��
	RTC_Timecorrect(timestamp);						//ÿ�ο�������У׼ʱ�䣬����д��RTC�ĳ�ʼʱ��	
	printf("������У׼ʱ��ɹ���\r\n");
	return 1;
}






//RTCʱ���жϷ�����
//ÿ�봥��һ��  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{	
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)	//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	  	
		RTC_Get();								//����ʱ��
//		alarm_time = RTC_GetCounter()+24*60*60;
//		alarm_time = alarm_time - alarm_time%60;//����ʱ��ȷ���֣�����ȥ��
//		RTC_SerAlarmSec(alarm_time);//ÿ��һ�����ӣ�����ʱ�������24Сʱ
		alarm_time = RTC_GetCounter()+60*60;
		alarm_time = alarm_time;	
		RTC_SerAlarmSec(alarm_time);							//ÿ��һ�����ӣ�����ʱ�������1Сʱ
		alarm.flag = 1;
		if((calendar.hour<20)&&(calendar.hour>8))
		{
			playinttime(calendar.hour);								//���㲥��calendar.hour
			printf("����ʱ�䵽\r\n");
    }
	}
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
	{							
		RTC_Get();//����ʱ�� 
		RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//�����ж�
 	}
	RTC_WaitForLastTask();	  	    						 	   	 
}


static void RTC_NVIC_Config(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}



void BKP_test(void)
{
	char arr[]="���ǳ�";
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	   
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ󱸼Ĵ�������  
	BKP_WriteBackupRegister(BKP_DR1,(u16)arr[0]<<8 | (u16)arr[1]);		//8λ�ķ�ʽƴ�ӣ���ָ���ĺ󱸼Ĵ�����д���û���������
	BKP_WriteBackupRegister(BKP_DR2,(u16)arr[2]<<8 | (u16)arr[3]);
	BKP_WriteBackupRegister(BKP_DR3,(u16)arr[4]<<8 | (u16)arr[5]);
	//printf("%c%c%c%c%c%c\r\n",(u8)(BKP_ReadBackupRegister(BKP_DR1)>>8),(u8)BKP_ReadBackupRegister(BKP_DR1),(u8)(BKP_ReadBackupRegister(BKP_DR2)>>8),\
	(u8)BKP_ReadBackupRegister(BKP_DR2),(u8)(BKP_ReadBackupRegister(BKP_DR3)>>8),(u8)(BKP_ReadBackupRegister(BKP_DR3)));			//8λ�ķ�ʽ�𿪣�ǿתu8ȡu16�ĵͰ�λ
	
}


//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������

u8 RTC_Init(void)
{
	u8 temp=0;
	//ʹ��PWR��BKP����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	   
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ󱸼Ĵ�������  
	
	RCC_LSICmd(ENABLE);//ʹ���ڲ�ʱ��
	
	//���ָ����RCC��־λ�������,�ȴ����پ������
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp<250)	
	{
		temp++;
		Delay_ms(10);
	}
	if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5 )//��ָ���ĺ󱸼Ĵ����ж�������
	{	 			
		BKP_DeInit();	//��λ�������� 	    
	    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//����RTCʱ��(RTCCLK),ѡ��HSE/128��ΪRTCʱ��    
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);//����ʱ��LSIΪ RTC ��ʱ����Դ
		
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
		
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();	//�ȴ�RTC�Ĵ��� �� APB1ʱ�ӽӿ�ͬ���� 
		
	
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		//��������ģʽ���Ӷ�������RTC_CNT�� RTC_ALR��RTC_PRL�Ĵ���
		RTC_EnterConfigMode();	// ��������ģʽ����	
		
		RTC_SetPrescaler(62500-1); //����RTCԤ��Ƶ��ֵ 62500
		//RTC_SetPrescaler(40000-1); //����RTCԤ��Ƶ��ֵ 40000
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_Set(2022,7,15,10,44,00);  //����ʱ��
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_ExitConfigMode(); //�˳�����ģʽ 
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
		//printf("---------------this is if\r\n");
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//ʹ��RTC���ж�
	}
	else//ϵͳ������ʱ
	{
		BKP_test();
		//printf("---------------this is else\r\n");
		RTC_WaitForSynchro();	//�ȴ�RTC�Ĵ��� �� APB1ʱ�ӽӿ�ͬ���� 
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�  
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_ALR, ENABLE); //ʹ��RTC�����ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}
	RTC_NVIC_Config();//RCT�жϷ�������		    				     
	RTC_Get();//����ʱ��	
	return 0; //ok
}   


//RTCʱ��У׼�����������Ѿ�������RTC_Init�Ĺ���
u8 RTC_Timecorrect(long int seccount)
{
	u8 temp=0;
	//ʹ��PWR��BKP����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	   
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ󱸼Ĵ�������  
	
	RCC_LSICmd(ENABLE);//ʹ���ڲ�ʱ��
	
	//���ָ����RCC��־λ�������,�ȴ����پ������
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp<250)	
	{
		temp++;
		Delay_ms(10);
	}
	if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	
	
	BKP_DeInit();	//��λ�������� 	    
	    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//����RTCʱ��(RTCCLK),ѡ��HSE/128��ΪRTCʱ��    
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);//����ʱ��LSIΪ RTC ��ʱ����Դ
		
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
		
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();	//�ȴ�RTC�Ĵ��� �� APB1ʱ�ӽӿ�ͬ���� 
		
	
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		//��������ģʽ���Ӷ�������RTC_CNT�� RTC_ALR��RTC_PRL�Ĵ���
		RTC_EnterConfigMode();	// ��������ģʽ����	
		
		RTC_SetPrescaler(62500-1); //����RTCԤ��Ƶ��ֵ 62500
		//RTC_SetPrescaler(40000-1); //����RTCԤ��Ƶ��ֵ 40000
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		//RTC_Set(2022,7,15,10,44,00);  //����ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
		PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
		RTC_SetCounter(seccount);	//����RTC��������ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
		
		
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_ExitConfigMode(); //�˳�����ģʽ 
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
		//printf("---------------this is if\r\n");
		RTC_ITConfig(RTC_IT_SEC, ENABLE);			//ʹ��RTC���ж�
		RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_ALR, ENABLE); 		//ʹ��RTC�����ж�
		RTC_WaitForLastTask();
		RTC_NVIC_Config();										//RCT�жϷ�������		    				     
		RTC_Get();														//����ʱ��	
		return 0; 														//ok
}




//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
				return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else 
				return 0;   
		}
		else 
			return 1;   
	}
	else 
		return 0;	
}	 	


//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
  seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(seccount);	//����RTC��������ֵ
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;
}

//��ʼ������		  
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//syear,smon,sday,hour,min,sec�����ӵ�������ʱ����   
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ 			    
	//����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	//���������Ǳ����!
	
	RTC_SetAlarm(seccount);
 
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	
	return 0;	    
}






//�����µ�����
void RTC_SerAlarmSec(uint32_t s)
{
	//����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	playinttime(calendar.hour);								//���㲥��calendar.hour
	RTC_SetAlarm(s);
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  
}



//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;  
    timecount = RTC_GetCounter();//��ȡRTC�ļ�������ֵ	���� 
 	temp = timecount/86400;   //�õ�����(��������Ӧ��),ÿ��86400��
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)
					temp-=366;//���������
				else 
					{temp1++;break;}  
			}
			else 
				temp-=365;	  //ƽ�� 
			
			temp1++;  
		}   
		calendar.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])  temp-=mon_table[temp1];//
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//�õ��·�
		calendar.w_date=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������  һ��86400�� 
	calendar.hour=temp/3600;     	//Сʱ
	calendar.min=(temp%3600)/60; 	//����	
	calendar.sec=(temp%3600)%60; 	//����
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//��ȡ����   
	return 0;
}	 

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//�������������������
//����ֵ�����ڼ�																					 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  


















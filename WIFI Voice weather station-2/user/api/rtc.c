#include "rtc.h" 	
#include "delay.h"				//使用延时函数
#include "stdio.h"
#include "my1680.h"				//使用播报功能
#include "ESP8266.h"			//使用解析函数解析时间数据
#include "stdlib.h"
#include "string.h"

_calendar_obj calendar;//时钟结构体 
_alarm_obj alarm = {0, 0, 0};
uint32_t alarm_time;


//向网络校准时间
u8 RTC_TIME_Init(void)
{
	Delay_ms(1000);
	char sendbuf[512]={0};										//定义一个命令变量
	Connect_Server("api.k780.com",80);				//连接时间服务器获取网络时间
	strcpy(sendbuf,"GET http://api.k780.com:88/?app=life.time&appkey=10003\
&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json&HTTP/1.1\r\n");
	SendcmdtoServer(sendbuf);
	if(  Analyse_TimeJson() )		//解析json字符串
	{
		printf("时间数据解析成功\r\n");
	}
	else
	{
		printf("时间数据解析失败\r\n");
		return 0;
	}
	if( Disconnect_Server() == 0)
	{
		return 0;		//没能正确的退出与服务器的连接，程序异常结束
	}
	long int timestamp = atol(time_stringdata.timestamp);
	timestamp += 8*3600+2;				//东八区加上8个小时的时间，单位是秒，为了时间更准，加上2s给程序执行
	RTC_Timecorrect(timestamp);						//每次开机联网校准时间，重新写入RTC的初始时间	
	printf("向网络校准时间成功！\r\n");
	return 1;
}






//RTC时钟中断服务函数
//每秒触发一次  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{	
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)	//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	
		RTC_Get();								//更新时间
//		alarm_time = RTC_GetCounter()+24*60*60;
//		alarm_time = alarm_time - alarm_time%60;//设置时精确到分，将秒去除
//		RTC_SerAlarmSec(alarm_time);//每来一次闹钟，闹钟时间向后推24小时
		alarm_time = RTC_GetCounter()+60*60;
		alarm_time = alarm_time;	
		RTC_SerAlarmSec(alarm_time);							//每来一次闹钟，闹钟时间向后推1小时
		alarm.flag = 1;
		if((calendar.hour<20)&&(calendar.hour>8))
		{
			playinttime(calendar.hour);								//整点播报calendar.hour
			printf("闹钟时间到\r\n");
    }
	}
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{							
		RTC_Get();//更新时间 
		RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清秒中断
 	}
	RTC_WaitForLastTask();	  	    						 	   	 
}


static void RTC_NVIC_Config(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}



void BKP_test(void)
{
	char arr[]="周星晨";
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	   
	PWR_BackupAccessCmd(ENABLE);//使能后备寄存器访问  
	BKP_WriteBackupRegister(BKP_DR1,(u16)arr[0]<<8 | (u16)arr[1]);		//8位的方式拼接，向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(BKP_DR2,(u16)arr[2]<<8 | (u16)arr[3]);
	BKP_WriteBackupRegister(BKP_DR3,(u16)arr[4]<<8 | (u16)arr[5]);
	//printf("%c%c%c%c%c%c\r\n",(u8)(BKP_ReadBackupRegister(BKP_DR1)>>8),(u8)BKP_ReadBackupRegister(BKP_DR1),(u8)(BKP_ReadBackupRegister(BKP_DR2)>>8),\
	(u8)BKP_ReadBackupRegister(BKP_DR2),(u8)(BKP_ReadBackupRegister(BKP_DR3)>>8),(u8)(BKP_ReadBackupRegister(BKP_DR3)));			//8位的方式拆开，强转u8取u16的低八位
	
}


//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码

u8 RTC_Init(void)
{
	u8 temp=0;
	//使能PWR和BKP外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	   
	PWR_BackupAccessCmd(ENABLE);//使能后备寄存器访问  
	
	RCC_LSICmd(ENABLE);//使能内部时钟
	
	//检查指定的RCC标志位设置与否,等待低速晶振就绪
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp<250)	
	{
		temp++;
		Delay_ms(10);
	}
	if(temp>=250)return 1;//初始化时钟失败,晶振有问题	
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5 )//从指定的后备寄存器中读出数据
	{	 			
		BKP_DeInit();	//复位备份区域 	    
	    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//设置RTC时钟(RTCCLK),选择HSE/128作为RTC时钟    
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);//设置时钟LSI为 RTC 的时钟来源
		
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();	//等待RTC寄存器 和 APB1时钟接口同步。 
		
	
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		//进入配置模式，从而允许向RTC_CNT、 RTC_ALR或RTC_PRL寄存器
		RTC_EnterConfigMode();	// 进入配置模式配置	
		
		RTC_SetPrescaler(62500-1); //设置RTC预分频的值 62500
		//RTC_SetPrescaler(40000-1); //设置RTC预分频的值 40000
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		
		RTC_Set(2022,7,15,10,44,00);  //设置时间
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		
		RTC_ExitConfigMode(); //退出配置模式 
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
		//printf("---------------this is if\r\n");
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
	}
	else//系统继续计时
	{
		BKP_test();
		//printf("---------------this is else\r\n");
		RTC_WaitForSynchro();	//等待RTC寄存器 和 APB1时钟接口同步。 
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_ALR, ENABLE); //使能RTC闹钟中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	}
	RTC_NVIC_Config();//RCT中断分组设置		    				     
	RTC_Get();//更新时间	
	return 0; //ok
}   


//RTC时间校准函数，这里已经包含了RTC_Init的功能
u8 RTC_Timecorrect(long int seccount)
{
	u8 temp=0;
	//使能PWR和BKP外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	   
	PWR_BackupAccessCmd(ENABLE);//使能后备寄存器访问  
	
	RCC_LSICmd(ENABLE);//使能内部时钟
	
	//检查指定的RCC标志位设置与否,等待低速晶振就绪
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp<250)	
	{
		temp++;
		Delay_ms(10);
	}
	if(temp>=250)return 1;//初始化时钟失败,晶振有问题	
	
	BKP_DeInit();	//复位备份区域 	    
	    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//设置RTC时钟(RTCCLK),选择HSE/128作为RTC时钟    
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);//设置时钟LSI为 RTC 的时钟来源
		
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();	//等待RTC寄存器 和 APB1时钟接口同步。 
		
	
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		//进入配置模式，从而允许向RTC_CNT、 RTC_ALR或RTC_PRL寄存器
		RTC_EnterConfigMode();	// 进入配置模式配置	
		
		RTC_SetPrescaler(62500-1); //设置RTC预分频的值 62500
		//RTC_SetPrescaler(40000-1); //设置RTC预分频的值 40000
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		
		//RTC_Set(2022,7,15,10,44,00);  //设置时间
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
		PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
		RTC_SetCounter(seccount);	//设置RTC计数器的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
		
		
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		
		RTC_ExitConfigMode(); //退出配置模式 
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
		//printf("---------------this is if\r\n");
		RTC_ITConfig(RTC_IT_SEC, ENABLE);			//使能RTC秒中断
		RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_ALR, ENABLE); 		//使能RTC闹钟中断
		RTC_WaitForLastTask();
		RTC_NVIC_Config();										//RCT中断分组设置		    				     
		RTC_Get();														//更新时间	
		return 0; 														//ok
}




//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
				return 1;//如果以00结尾,还要能被400整除 	   
			else 
				return 0;   
		}
		else 
			return 1;   
	}
	else 
		return 0;	
}	 	


//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
  seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);	//设置RTC计数器的值
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	return 0;
}

//初始化闹钟		  
//以1970年1月1日为基准
//1970~2099年为合法年份
//syear,smon,sday,hour,min,sec：闹钟的年月日时分秒   
//返回值:0,成功;其他:错误代码.
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去 			    
	//设置时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	//上面三步是必须的!
	
	RTC_SetAlarm(seccount);
 
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	    
}






//设置新的闹钟
void RTC_SerAlarmSec(uint32_t s)
{
	//设置时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	playinttime(calendar.hour);								//整点播报calendar.hour
	RTC_SetAlarm(s);
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  
}



//得到当前的时间
//返回值:0,成功;其他:错误代码.
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;  
    timecount = RTC_GetCounter();//获取RTC的计数器的值	秒数 
 	temp = timecount/86400;   //得到天数(秒钟数对应的),每天86400秒
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)
					temp-=366;//闰年的天数
				else 
					{temp1++;break;}  
			}
			else 
				temp-=365;	  //平年 
			
			temp1++;  
		}   
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])  temp-=mon_table[temp1];//
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数  一天86400秒 
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟	
	calendar.sec=(temp%3600)%60; 	//秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期   
	return 0;
}	 

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日
//返回值：星期几																					 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  


















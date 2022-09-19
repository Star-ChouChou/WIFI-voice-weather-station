#ifndef _ESP8266_H
#define _ESP8266_H

#include "stm32f10x.h"

#define WIFI_SIZE 1200			//接收数据的数组长度为1024
#define WIFI_USART  				USART3
#define WIFI_USART_IRQn				USART3_IRQn
#define	WIFI_USARTPeriphClockCmd	RCC_APB1PeriphClockCmd
#define WIFI_USART_PeriphClock 		RCC_APB1Periph_USART3
#define	WIFI_IRQHandler   			USART3_IRQHandler

#define WIFI_ENABLE		GPIO_SetBits(GPIOE,GPIO_Pin_6)			//端口使能，打开ESP8266
#define WIFI_DISABLE	GPIO_ResetBits(GPIOE,GPIO_Pin_6)		//端口失能，关闭ESP8266，通过两者的搭配实现对ESP8266的复位


typedef struct 
{
	char RX_buff[WIFI_SIZE];	//保存接收到的数据
	u16 RX_count;		 					//记录接收数据的数量
	vu8 USART_RevOverFlag; 		//作为接收完成的标志
	
}WIFI_DataStruct;

extern WIFI_DataStruct  WIFI_Data;
extern char Now_temperature[3];
u8 ESP8266_Init(void);								//初始换WIFI串口，并连接指定局域网络
u8 GET_Weather(char *CityWeather);									//获取未来三天天气的函数封装
void WIFI_USART_Config(void);					//WIFI串口配置
void WIFI_SendData(u8 data);					//发送数据配置
void WIFI_SendStr(char * p);
void WIFI_SendArray(u8 Arr[] ,u16 DataLenth);

#define MYSSID "testwifi123"
#define MYPASWD "88889999"
#define mykey "SPH73WGf2rWcPlhjg"

u8 Connect_AP(char *ssid,char *paswd);
char *FindStr(char *dest, char *src, uint32_t timeout);
u8 WIFI_Sendcmd_Recack(char *cmd,char *ack,u32 timeout,u8 times);
u8  Connect_Server(char *IP ,u16 PORT);
u8 Analyse_TodayWeatherJson(void);
u8 Analyse_TimeJson(void);
void SendcmdtoServer(char *sendbuf);					//将命令发送给服务器，获取目的数据
u8 Disconnect_Server(void);
u8 Analyse_CityWeatherJson(char *cityname);
u8 GET_City5Weather(void);
u8 Get_Weather1(void);

//字符串类型的天气数据结构体
typedef struct
{
	char cityname[20];				//城市名
	char code[3];							//天气代码
	char temperature[3];			//当前气温
	char lowt[3];							//当天的最低温度
	char hight[3];							//当天的最高温度
	char	humidity[3];				//当天的相对湿度
	
}	weather_stringdataStruct;

typedef struct
{											
	weather_stringdataStruct acity_weather[3];			//结构体嵌套
} CITY_WEATHER;



extern weather_stringdataStruct weather_stringdata;
extern weather_stringdataStruct weather_day3stringdata[3];							//初始化郑州3天天气的结构体
extern CITY_WEATHER city5_weather[5];					//五个城市的天气数组

typedef struct
{
	char datetime[30];				//日期和时间
	char week[5];							//周几
	char timestamp[11];
}	time_stringdataStruct;
extern time_stringdataStruct time_stringdata;

////数据实际的数据类型的结构体
//typedef struct
//{
//	char cityname[30];				//城市名
//	int code;							//天气代码
//	int temperature;			//当前气温
//}	weather_realdataStruct;
//extern weather_realdataStruct weather_realdata;

#endif

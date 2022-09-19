#ifndef _ESP8266_H
#define _ESP8266_H

#include "stm32f10x.h"

#define WIFI_SIZE 1200			//�������ݵ����鳤��Ϊ1024
#define WIFI_USART  				USART3
#define WIFI_USART_IRQn				USART3_IRQn
#define	WIFI_USARTPeriphClockCmd	RCC_APB1PeriphClockCmd
#define WIFI_USART_PeriphClock 		RCC_APB1Periph_USART3
#define	WIFI_IRQHandler   			USART3_IRQHandler

#define WIFI_ENABLE		GPIO_SetBits(GPIOE,GPIO_Pin_6)			//�˿�ʹ�ܣ���ESP8266
#define WIFI_DISABLE	GPIO_ResetBits(GPIOE,GPIO_Pin_6)		//�˿�ʧ�ܣ��ر�ESP8266��ͨ�����ߵĴ���ʵ�ֶ�ESP8266�ĸ�λ


typedef struct 
{
	char RX_buff[WIFI_SIZE];	//������յ�������
	u16 RX_count;		 					//��¼�������ݵ�����
	vu8 USART_RevOverFlag; 		//��Ϊ������ɵı�־
	
}WIFI_DataStruct;

extern WIFI_DataStruct  WIFI_Data;
extern char Now_temperature[3];
u8 ESP8266_Init(void);								//��ʼ��WIFI���ڣ�������ָ����������
u8 GET_Weather(char *CityWeather);									//��ȡδ�����������ĺ�����װ
void WIFI_USART_Config(void);					//WIFI��������
void WIFI_SendData(u8 data);					//������������
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
void SendcmdtoServer(char *sendbuf);					//������͸�����������ȡĿ������
u8 Disconnect_Server(void);
u8 Analyse_CityWeatherJson(char *cityname);
u8 GET_City5Weather(void);
u8 Get_Weather1(void);

//�ַ������͵��������ݽṹ��
typedef struct
{
	char cityname[20];				//������
	char code[3];							//��������
	char temperature[3];			//��ǰ����
	char lowt[3];							//���������¶�
	char hight[3];							//���������¶�
	char	humidity[3];				//��������ʪ��
	
}	weather_stringdataStruct;

typedef struct
{											
	weather_stringdataStruct acity_weather[3];			//�ṹ��Ƕ��
} CITY_WEATHER;



extern weather_stringdataStruct weather_stringdata;
extern weather_stringdataStruct weather_day3stringdata[3];							//��ʼ��֣��3�������Ľṹ��
extern CITY_WEATHER city5_weather[5];					//������е���������

typedef struct
{
	char datetime[30];				//���ں�ʱ��
	char week[5];							//�ܼ�
	char timestamp[11];
}	time_stringdataStruct;
extern time_stringdataStruct time_stringdata;

////����ʵ�ʵ��������͵Ľṹ��
//typedef struct
//{
//	char cityname[30];				//������
//	int code;							//��������
//	int temperature;			//��ǰ����
//}	weather_realdataStruct;
//extern weather_realdataStruct weather_realdata;

#endif

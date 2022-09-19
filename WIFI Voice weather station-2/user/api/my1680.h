#ifndef _MY1680_H
#define	_MY1680_H

#include "stm32f10x.h"


void USART4_Config(void);
u8 USART4_RecvData(void);
void USART4_SendArray(u8 Arr[] ,u16 DataLenth);
void USART4_SendData(u8 data);
void USART4_SendStr(char * p);
void Play_MP3_root(u8 Name_H,u8 Name_L);
void Play_MP3_dir(u8 Dir_name,u8 Music_name);
void Play_Todayweather(char *city,int code,int temperature);
void playtime(char *datetime,int week);
void Play_Day3weather(void);				//播报获取到的三天的天气数据
void Play_Day3(char *city,char *code,char *lowt,char *hight,int day);
void playinttime(vu8 hour);


#endif


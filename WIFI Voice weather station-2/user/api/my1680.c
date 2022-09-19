#include "my1680.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "delay.h"
#include "ESP8266.h"		//Ϊ��ʹ��ESP8266�еĽṹ���������ݽ��в���

//����USART4
//��ԭ��ͼ��֪PC10ΪTX����Ϊ��������ģʽ
//PC11ΪRX����Ϊ�������ģʽ



//���Ÿ�Ŀ¼�µ����֣�������������������ֵĸ���λ�͵���λ
void Play_MP3_root(u8 Name_H,u8 Name_L)
{
	USART4_SendData(0x7E);
	USART4_SendData(0x05);
	USART4_SendData(0x41);
	USART4_SendData(Name_H);
	USART4_SendData(Name_L);
	u8 check = (0x05^0x41^Name_H^Name_L);
	USART4_SendData(check);
	USART4_SendData(0xEF);
}


//����һ������Ķ�λ����������һλ����
void play_twodigits(int data)
{				
	if(data>=10&&data<=99)		
	{
		if(data%10 == 0)				//ʮ������������λ����Ҫ��
		{
			u8 s=data/10;
			Play_MP3_dir(01,s);
			Play_MP3_dir(01,10);
		}
		else if(data>10&&data<20)		//����ʮ�������ֲ���1
		{
			u8 g=data%10;
			Play_MP3_dir(01,10);
			Play_MP3_dir(01,g);
		}
		else
		{
			u8 s=data/10;
			u8 g=data%10;
			Play_MP3_dir(01,s);
			Play_MP3_dir(01,10);
			Play_MP3_dir(01,g);
		}
	}
	else if(data>=0&&data<=9)
	{
		Play_MP3_dir(01,data);
	}
}



//����ָ��Ŀ¼�µ����ֺ���
void Play_MP3_dir(u8 Dir_name,u8 Music_name)
{
	USART4_SendData(0x7E);
	USART4_SendData(0x05);
	USART4_SendData(0x42);
	USART4_SendData(Dir_name);
	USART4_SendData(Music_name);
	u8 check = (0x05^0x42^Dir_name^Music_name);
	USART4_SendData(check);
	USART4_SendData(0xEF);
}

//������������Ĳ����ǣ����С�������롢�¶�
void Play_Todayweather(char *city,int code,int temperature)
{
	if(strcmp(city,"Zhengzhou") == 0)
	{
		Play_MP3_dir(01,14);
	}
	else if(strcmp(city,"Beijing") == 0)
	{
		Play_MP3_dir(01,15);
	}
	else			//û��¼��ó��е���������
	{
		Play_MP3_dir(01,17);
	}
	switch(code)
	{
		case 0:	;
		case 1:	;
		case 2:	;
		case 3:Play_MP3_dir(02,3); 				break;			//����
		case 4:Play_MP3_dir(02,4);				break;			//����
		case 5:	;
		case 6:Play_MP3_dir(02,6);				break;			//����
		case 7:	;
		case 8:Play_MP3_dir(02,8);				break;			//����
		case 9:Play_MP3_dir(02,9);				break;			//��
		case 10:Play_MP3_dir(02,10);			break;			//����
		case 11:Play_MP3_dir(02,11);			break;			//������
		case 13:Play_MP3_dir(02,13);			break;			//С��
		case 14:Play_MP3_dir(02,14);			break;			//����
		case 15:Play_MP3_dir(02,15);			break;			//����
		case 16:Play_MP3_dir(02,16);			break;			//����
	}
	
	play_twodigits(temperature);	//������λ����һ�����������Ƶ�λ�����磺21Ϊ��ʮһ��
	Play_MP3_dir(01,16);					//���϶�
	
}


	u8 tmp=0;
//������������Ĳ����ǣ����С�������롢����¶ȡ�����¶ȡ��ڼ���(day)0~2�Ĳ���
void Play_Day3(char *city,char *charcode,char *lowt,char *hight,int day)
{
	int code = atoi(charcode);

	if(strcmp(city,"Zhengzhou") == 0)
	{
		tmp=19;
//		switch(day)					//�жϲ�����һ�������
//		{
//			case 0:	Play_MP3_dir(02,19);	break;
//			case 1:	Play_MP3_dir(02,20);	break;
//			case 2:	Play_MP3_dir(02,21);	break;
//		}
	}
	else if(strcmp(city,"Beijing") == 0)
	{
		tmp=22;

	}
	else if(strcmp(city,"Shanghai") == 0)
	{
		tmp=25;

	}
	else if(strcmp(city,"Guangzhou") == 0)
	{
		tmp=28;
	}
		else if(strcmp(city,"Shenzhen") == 0)
	{
		tmp=31;
	}
	else			//û��¼��ó��е���������
	{
		Play_MP3_dir(01,17);
		return;
	}
	switch(day)					//�жϲ�����һ�������
	{
		case 0:	Play_MP3_dir(02,tmp);	break;
		case 1:	Play_MP3_dir(02,tmp+1);	break;
		case 2:	Play_MP3_dir(02,tmp+2);	break;
	}
		
	switch(code)
	{
		case 0:	;
		case 1:	;
		case 2:	;
		case 3:Play_MP3_dir(02,3); 				break;
		case 4:Play_MP3_dir(02,4);				break;
		case 5:	;
		case 6:Play_MP3_dir(02,6);				break;
		case 7:	;
		case 8:Play_MP3_dir(02,8);				break;
		case 9:Play_MP3_dir(02,9);				break;
		case 10:Play_MP3_dir(02,10);			break;
		case 11:Play_MP3_dir(02,11);			break;
		case 13:Play_MP3_dir(02,13);			break;
		case 14:Play_MP3_dir(02,14);			break;
		case 15:Play_MP3_dir(02,15);			break;
		case 16:Play_MP3_dir(02,16);			break;
	}
	int low = atoi(lowt);
	int high = atoi(hight);
	
	Play_MP3_dir(02,17);
	play_twodigits(low);					//������λ��
	Play_MP3_dir(01,16);					//���϶�
	
	Play_MP3_dir(02,18);
	play_twodigits(high);
	Play_MP3_dir(01,16);
	
	
}


void Play_Day3weather(void)
{
//	u8 i=0;
	for(u8 i=0;i<3;i++)
	{
		Play_Day3(weather_day3stringdata[i].cityname,weather_day3stringdata[i].code,weather_day3stringdata[i].lowt,weather_day3stringdata[i].hight,i);
		Delay_ms(8500);
	}
}


//�������ں�ʱ��ĺ���
/*��ȡ��ʱ�����ݸ�ʽ�ǣ�2022-07-11 11:56:13
���ݸ�ʽ�в�ͬ���ݵ�λ�����ַ�����ȡ��������
*/
void playtime(char *datetime,int week)
{
	Play_MP3_dir(01,11);			//��ӭ��ʾ������ʱ��׼������
	for(u8 i=0;i<4;i++)
	{
		if(datetime[i]-48 == 0)
		{
			Play_MP3_dir(01,30);
		}
		else
		{
			Play_MP3_dir(01,datetime[i]-48);
		}
	}
	Play_MP3_dir(01,24);		//������
	char month[2]={0};
	month[0]=datetime[5];
	month[1]=datetime[6];
	play_twodigits(atoi(month));
	Play_MP3_dir(01,23);		//������

	char day[2]={0};
	day[0]=datetime[8];
	day[1]=datetime[9];
	play_twodigits(atoi(day));
	Play_MP3_dir(01,22);		//������
	switch(week)
	{
		case 1:Play_MP3_dir(03,1);	break;
		case 2:Play_MP3_dir(03,2);	break;
		case 3:Play_MP3_dir(03,3);	break;
		case 4:Play_MP3_dir(03,4);	break;
		case 5:Play_MP3_dir(03,5);	break;
		case 6:Play_MP3_dir(03,6);	break;
		case 0:Play_MP3_dir(03,7);	break;
	}
	

	Delay_ms(10000);					//��������ģ����鲥��ʽһ��Ҫע�⣬�鲥���֧��20����������������
	char hour[2]={0};							//����ʱ
	hour[0]=datetime[11];
	hour[1]=datetime[12];
	Play_MP3_dir(01,27);			//�����������ǡ�
	play_twodigits(atoi(hour));
	Play_MP3_dir(01,25);
	

	char minute[2]={0};			//������
	minute[0]=datetime[14];
	minute[1]=datetime[15];
	play_twodigits(atoi(minute));
	Play_MP3_dir(01,26);
}

//���㲥��
void playinttime(vu8 hour)
{
	Play_MP3_dir(01,27);				//"������"
	play_twodigits(hour);				//����
	Play_MP3_dir(01,25);				//��
}

////�岥��Ŀ¼�µ�mp3
//void Play_MP3_insert_root()
//{
//	
//}

////�岥ָ���ļ����µ�mp3
//void Play_MP3_insert_dir()
//{
//	
//}

////����һ����λ����
//void Play_num(u8 num)
//{
//		int arr[]={0};
//}



////��������
//void Set_volum()
//{

//}


//���ô���4
/*
	����IOʱ�Ӻʹ���ʱ��
	����MCU�Ĵ��ڶ�Ӧ�����ŵĸ�������ģʽ�͸����������ģʽ
	
*/
void USART4_Config(void)
{
		//ʹ����ص�ʱ�ӣ�GPIOA�˿ں�USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);			//����ע�⴮��4дΪUART4
	
	//����PA10(RX)Ϊ�������룬PA9(TX)Ϊ�������
	GPIO_InitTypeDef Mygpio;
	Mygpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//PC11����Ϊ��������ģʽ
	Mygpio.GPIO_Pin = GPIO_Pin_11;							//��������
	Mygpio.GPIO_Speed = GPIO_Speed_50MHz;				//������������
	GPIO_Init(GPIOC,&Mygpio);										//��ʼ������
	
	Mygpio.GPIO_Mode = GPIO_Mode_AF_PP;		//PC10����Ϊ�����������ģʽ
	Mygpio.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC,&Mygpio);
	
	
	
	//����USART4������
	USART_InitTypeDef USART_InitStructure;				//�����ʼ��USART�Ľṹ�� 
	USART_InitStructure.USART_BaudRate = 9600; 				//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	
	//�������ݳ��ȣ�������8λ����9λ���ݳ��ȣ�һ��ʹ��8λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 				//����ֹͣλ,��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; 					//������żУ��λ�ر�
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;				//����Ӳ�������ƹر� 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 			//ʹ�ܽ��պͷ���
	USART_Init(UART4, &USART_InitStructure);
	
	USART_Cmd(UART4,ENABLE);			//�������֮��ʹ�ܴ���4
}



//��������u8����
void USART4_SendData(u8 data)
{
	//#ifdef USE_STDPERIPH_DRIVER
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC) != SET)		
		{}		//û�з����һֱ���ͣ�����֮��������ж�ΪSET
		USART_SendData(UART4,data);
	//#else
		//	while(((USART->SR)&(1<<6)) == 0)
			//{}				//û�з�����������Ĵ���ʵ��
			
}


////�����ַ�������
//void USART4_SendStr(char * p)
//{
//	while(*p != '\0')
//	{
//		USART4_SendData(*p);
//		p++;
//	}
//}


////����u8����
//void USART4_SendArray(u8 Arr[] ,u16 DataLenth)
//{
//	u16 i=0;
//	while(DataLenth --)
//	{
//		USART4_SendData(Arr[i++]);
//	}
//}


////��������u8����
//u8 USART4_RecvData(void)
//{
//	//led_Init(2);
//	while(USART_GetFlagStatus(UART4,USART_IT_RXNE) != SET)		//û���յ���һֱ�ȴ�
//	{
//	}
//	u8 RXData = USART_ReceiveData(UART4);
//	return RXData;
//}









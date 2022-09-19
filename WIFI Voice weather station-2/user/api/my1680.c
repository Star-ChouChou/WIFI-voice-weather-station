#include "my1680.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "delay.h"
#include "ESP8266.h"		//为了使用ESP8266中的结构体天气数据进行播报

//配置USART4
//由原理图可知PC10为TX设置为浮空输入模式
//PC11为RX设置为推挽输出模式



//播放根目录下的音乐，传入的两个参数是音乐的高两位和低两位
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


//播放一个任意的二位数变量或者一位变数
void play_twodigits(int data)
{				
	if(data>=10&&data<=99)		
	{
		if(data%10 == 0)				//十的整数倍数个位不需要念
		{
			u8 s=data/10;
			Play_MP3_dir(01,s);
			Play_MP3_dir(01,10);
		}
		else if(data>10&&data<20)		//惯例十几的数字不念1
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



//播放指定目录下的音乐函数
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

//播报天气传入的参数是：城市、气象代码、温度
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
	else			//没有录入该城市的语音播报
	{
		Play_MP3_dir(01,17);
	}
	switch(code)
	{
		case 0:	;
		case 1:	;
		case 2:	;
		case 3:Play_MP3_dir(02,3); 				break;			//晴天
		case 4:Play_MP3_dir(02,4);				break;			//多云
		case 5:	;
		case 6:Play_MP3_dir(02,6);				break;			//多云
		case 7:	;
		case 8:Play_MP3_dir(02,8);				break;			//多云
		case 9:Play_MP3_dir(02,9);				break;			//阴
		case 10:Play_MP3_dir(02,10);			break;			//阵雨
		case 11:Play_MP3_dir(02,11);			break;			//雷阵雨
		case 13:Play_MP3_dir(02,13);			break;			//小雨
		case 14:Play_MP3_dir(02,14);			break;			//中雨
		case 15:Play_MP3_dir(02,15);			break;			//大雨
		case 16:Play_MP3_dir(02,16);			break;			//暴雨
	}
	
	play_twodigits(temperature);	//播报两位数的一个数（带进制单位，例如：21为二十一）
	Play_MP3_dir(01,16);					//摄氏度
	
}


	u8 tmp=0;
//播报天气传入的参数是：城市、气象代码、最低温度、最高温度、第几天(day)0~2的播报
void Play_Day3(char *city,char *charcode,char *lowt,char *hight,int day)
{
	int code = atoi(charcode);

	if(strcmp(city,"Zhengzhou") == 0)
	{
		tmp=19;
//		switch(day)					//判断播放哪一天的数据
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
	else			//没有录入该城市的语音播报
	{
		Play_MP3_dir(01,17);
		return;
	}
	switch(day)					//判断播放哪一天的数据
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
	play_twodigits(low);					//播报两位数
	Play_MP3_dir(01,16);					//摄氏度
	
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


//播报日期和时间的函数
/*获取的时间数据格式是：2022-07-11 11:56:13
根据格式中不同数据的位数将字符串提取出来播报
*/
void playtime(char *datetime,int week)
{
	Play_MP3_dir(01,11);			//欢迎提示，今天时间准备播报
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
	Play_MP3_dir(01,24);		//播报年
	char month[2]={0};
	month[0]=datetime[5];
	month[1]=datetime[6];
	play_twodigits(atoi(month));
	Play_MP3_dir(01,23);		//播报月

	char day[2]={0};
	day[0]=datetime[8];
	day[1]=datetime[9];
	play_twodigits(atoi(day));
	Play_MP3_dir(01,22);		//播报日
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
	

	Delay_ms(10000);					//对于语音模块的组播方式一定要注意，组播最多支持20条语音的连续播报
	char hour[2]={0};							//播报时
	hour[0]=datetime[11];
	hour[1]=datetime[12];
	Play_MP3_dir(01,27);			//播报“现在是”
	play_twodigits(atoi(hour));
	Play_MP3_dir(01,25);
	

	char minute[2]={0};			//播报分
	minute[0]=datetime[14];
	minute[1]=datetime[15];
	play_twodigits(atoi(minute));
	Play_MP3_dir(01,26);
}

//整点播报
void playinttime(vu8 hour)
{
	Play_MP3_dir(01,27);				//"现在是"
	play_twodigits(hour);				//数字
	Play_MP3_dir(01,25);				//点
}

////插播根目录下的mp3
//void Play_MP3_insert_root()
//{
//	
//}

////插播指定文件夹下的mp3
//void Play_MP3_insert_dir()
//{
//	
//}

////播报一个百位数字
//void Play_num(u8 num)
//{
//		int arr[]={0};
//}



////设置音量
//void Set_volum()
//{

//}


//配置串口4
/*
	开启IO时钟和串口时钟
	设置MCU的串口对应的引脚的浮空输入模式和复用推挽输出模式
	
*/
void USART4_Config(void)
{
		//使能相关的时钟，GPIOA端口和USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);			//这里注意串口4写为UART4
	
	//设置PA10(RX)为浮空输入，PA9(TX)为推挽输出
	GPIO_InitTypeDef Mygpio;
	Mygpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//PC11设置为浮空输入模式
	Mygpio.GPIO_Pin = GPIO_Pin_11;							//设置引脚
	Mygpio.GPIO_Speed = GPIO_Speed_50MHz;				//设置引脚速率
	GPIO_Init(GPIOC,&Mygpio);										//初始化函数
	
	Mygpio.GPIO_Mode = GPIO_Mode_AF_PP;		//PC10设置为复用推挽输出模式
	Mygpio.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC,&Mygpio);
	
	
	
	//设置USART4的属性
	USART_InitTypeDef USART_InitStructure;				//定义初始化USART的结构体 
	USART_InitStructure.USART_BaudRate = 9600; 				//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	
	//设置数据长度，可设置8位或者9位数据长度，一般使用8位置
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 				//设置停止位,在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; 					//设置奇偶校验位关闭
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;				//设置硬件流控制关闭 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 			//使能接收和发送
	USART_Init(UART4, &USART_InitStructure);
	
	USART_Cmd(UART4,ENABLE);			//配置完成之后使能串口4
}



//发送数据u8类型
void USART4_SendData(u8 data)
{
	//#ifdef USE_STDPERIPH_DRIVER
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC) != SET)		
		{}		//没有发完就一直发送，发完之后发送完成中断为SET
		USART_SendData(UART4,data);
	//#else
		//	while(((USART->SR)&(1<<6)) == 0)
			//{}				//没有发完继续发，寄存器实现
			
}


////发送字符串数据
//void USART4_SendStr(char * p)
//{
//	while(*p != '\0')
//	{
//		USART4_SendData(*p);
//		p++;
//	}
//}


////发送u8数组
//void USART4_SendArray(u8 Arr[] ,u16 DataLenth)
//{
//	u16 i=0;
//	while(DataLenth --)
//	{
//		USART4_SendData(Arr[i++]);
//	}
//}


////接收数据u8类型
//u8 USART4_RecvData(void)
//{
//	//led_Init(2);
//	while(USART_GetFlagStatus(UART4,USART_IT_RXNE) != SET)		//没有收到就一直等待
//	{
//	}
//	u8 RXData = USART_ReceiveData(UART4);
//	return RXData;
//}









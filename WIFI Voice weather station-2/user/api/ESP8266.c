#include "ESP8266.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "cJSON.h"
#include "stdlib.h"
#include "my1680.h"





WIFI_DataStruct  WIFI_Data={0};
weather_stringdataStruct weather_stringdata={0};			//结构体初始化为0
CITY_WEATHER city5_weather[5];		//存储郑州北上广深五个城市的天气 
weather_stringdataStruct weather_day3stringdata[3];								//初始化郑州3天天气的结构体

/*
	实现对ESP8266模块发送命令，并且检测命令是否得到响应，
	得到响应之后在继续执行下一步，最后连接心知天气的服务器
	获取天气数据

***程序实现的结构和原理：首先由发送函数发送命令并检测是否收到
结果，WIFI_Sendcmd_Recack()函数的作用就是执行指定的命令，设置
延时时间，设置重发次数，并检测是否收到预期的结果。在该函数内部又调用
FindStr()函数用于检测在指定的延时时间内是否收到响应的字符串结果
而结果接收使用的是中断接受字符串，而后存储在WIFI_Data.RX_buff
中，这个结果是全局变量，所以直接在FindStr函数中对比是否存在即可。
这里需要注意，传入的延时时间应依据情况而定，相对而言连接wifi和服
务器的时间比较长。

*/



//初始化WIFI设置，并连接局域网络
u8 ESP8266_Init()
{
	WIFI_USART_Config();	//串口3配置和中断配置，以及WIFI的串口3设置
	Delay_ms(1000);
	if( Connect_AP(MYSSID,MYPASWD) == 1)	//连接热点
	{
		printf("wifi连接失败\r\n");
		return 0;
	}
	else
	{
		printf("wifi连接成功\r\n");
		Play_MP3_dir(01,12);						//wifi连接成功提示音
		return 1;
	}
}

u8 GET_City5Weather(void)
{
	
	if(!GET_Weather("zhengzhou"))			//获取并且解析郑州的三天天气
	{
		return 0;
	}
	
	if(!GET_Weather("beijing"))			//获取北京
	{
		return 0;
	}

	if(!GET_Weather("shanghai"))			//获取上海
	{
		return 0;
	}

	if(!GET_Weather("guangzhou"))			//获取广州
	{
		return 0;
	}
	if(!GET_Weather("shenzhen"))			//获取深圳
	{
		return 0;
	}
	return 1;
}




//获取指定城市的天气
u8 GET_Weather(char *CityWeather)
{
	char sendbuf[512] = {0};
	Connect_Server("api.seniverse.com",80);					//连接心知天气服务器获取天气数据
//		Connect_Server("api.seniverse.com",80);					//连接心知天气服务器获取天气数据
	//当被获取五天天气调用时只在获取五个城市的函数中连接一次服务器
	//获取当天的天气
	sprintf(sendbuf,"GET https://api.seniverse.com/v3/weather/now.json?\
key=%s&location=%s&language=en&unit=c\r\n",mykey,CityWeather);
	//api:https://api.seniverse.com/v3/weather/now.json?key=SNUfoDKrWHdMRHpuV&location=Zhengzhou&language=en&unit=c
	
	sprintf(sendbuf,"GET https://api.seniverse.com/v3/weather/daily.json?\
key=%s&location=%s&language=en&unit=c&start=0&days=5\r\n",mykey,CityWeather);
	SendcmdtoServer(sendbuf);
	
	if(  Analyse_CityWeatherJson(CityWeather) )		//解析三天的天气数据，参数是城市名
	{
		printf("天气数据解析成功\r\n");
		//Play_MP3_dir(01,11);							//欢迎提示音
	}
	else
	{
		printf("天气数据解析失败\r\n");
		return 0;
	}
	if( Disconnect_Server() == 0)		//断开与服务器的连接
	{
		return 0;		//没能正确的退出与服务器的连接，程序异常结束
	}
	return 1;
	
}

u8 Get_Weather1(void)
{
		char sendbuf[512] = {0};
	Connect_Server("api.seniverse.com",80);					//连接心知天气服务器获取天气数据
//		Connect_Server("api.seniverse.com",80);					//连接心知天气服务器获取天气数据
	//当被获取五天天气调用时只在获取五个城市的函数中连接一次服务器
	//获取当天的天气
	sprintf(sendbuf,"GET https://api.seniverse.com/v3/weather/now.json?\
key=%s&location=%s&language=en&unit=c\r\n",mykey,"Zhengzhou");
	//api:https://api.seniverse.com/v3/weather/now.json?key=SNUfoDKrWHdMRHpuV&location=Zhengzhou&language=en&unit=c
	
	SendcmdtoServer(sendbuf);
	
	if(  Analyse_TodayWeatherJson() )		//解析三天的天气数据
	{
		printf("天气数据解析成功\r\n");
		//Play_MP3_dir(01,11);							//欢迎提示音
	}
	else
	{
		printf("天气数据解析失败\r\n");
		return 0;
	}
	if( Disconnect_Server() == 0)		//断开与服务器的连接
	{
		return 0;		//没能正确的退出与服务器的连接，程序异常结束
	}
	return 1;
}

//退出透传模式
void EXIT_Scotch(void)
{
	WIFI_SendStr("+++");		//注意这里没有换行，否则无法退出透传模式
	Delay_ms(300);
}


//连接wifi
void ConnectWifi(char *ssid,char *paswd)
{
	char ord[128]={0};
	sprintf("AT+\"%s\",\"%s\"\r\n",ssid,paswd);
	WIFI_SendStr(ord);
	Delay_ms(2000);
}



//连接指定服务器
/*
	参数1是IP地址或者域名，参数2是端口号
	
*/


//连接热点，成功返回0，失败返回非0
u8 Connect_AP(char *ssid,char *paswd)
{
	char sendarr[256]={'\0'};
	EXIT_Scotch();//退出透传模式
	if( WIFI_Sendcmd_Recack("AT","OK",300,2) )
	{
		printf("AT 测试失败\r\n");
		return 1;
	}else
		printf("AT 测试成功\r\n");
	
	if( WIFI_Sendcmd_Recack("AT+CWMODE=1","OK",500,2) )
	{
		printf("AT+CWMODE 设置失败\r\n");
		return 2;
	}else
		printf("AT+CWMODE 设置成功\r\n");	
	
	sprintf(sendarr,"AT+CWJAP=\"%s\",\"%s\"",ssid,paswd);
	if(WIFI_Sendcmd_Recack(sendarr,"OK",10000,2))
	{
		printf("AT+CWJAP 设置失败\r\n");
		return 3;
	}else
	{
		printf("AT+CWJAP 设置成功\r\n");	
		printf("热点连接成功\r\n");	
	}		
	return 0;
	
}

/**
 * 功能：链接 TCP 服务器 进入透传
 * 参数：
 *      IP: 服务器 域名 或者 IP地址
*	  PORT: 服务器端口号
 * 返回值：成功返回 0  失败1/2/3/4
 *						 
 */
u8  Connect_Server(char *IP ,u16 PORT)
{
	char sendarr[128]={0};
	if(WIFI_Sendcmd_Recack("AT+CWJAP?","OK",5000,2) == 1)
	{
		printf("未检测到连接热点\r\n重连中....\r\n");
		if(Connect_AP(MYSSID,MYPASWD))
		{
			printf("未检测到热点,退出!!\r\n");
			return 1;
		}
	}
//4、	链接服务器：链接服务器需要 域名（或者IP）还有端口号 
//		AT+CIPSTART=“链接类型（TCP/UDP）”，” 域名或者IP”，端口号	
	sprintf(sendarr,"AT+CIPSTART=\"TCP\",\"%s\",%d",IP,PORT);
	if(WIFI_Sendcmd_Recack(sendarr,"OK",2000,2) == 1)
	{
		printf("连接服务器失败，退出\r\n");
		return  2;
	}	
//5、	开启透传模式  AT+CIPMODE=1;  
	if(WIFI_Sendcmd_Recack("AT+CIPMODE=1","OK",500,2) == 1)
	{
		printf("开启透传模式失败，退出\r\n");
		return 3;
	}
//6、	开始数据传输：AT+CIPSEND   发完指令收到‘>’代表成功进入发送模式	
	if(WIFI_Sendcmd_Recack("AT+CIPSEND",">",500,2) == 1)			
	//进入透传模式之后，ESP8266收到的所有的字符串都会当作命令发出给服务器
	{
		printf("进入透传模式失败，退出\r\n");
		return 4;
	}
	
	return 0;
}


//断开与当前服务器的连接，成功返回1，失败返回0
u8 Disconnect_Server()
{
	Delay_ms(500);		//退出透传不能太快否则指令无法生效，导致关闭服务器连接的无法关闭
	EXIT_Scotch();	//退出透传模式
	
	if(WIFI_Sendcmd_Recack("AT+CIPCLOSE","OK",1000,2) == 1)
	{
		printf("与服务器断开失败，退出\r\n");
		Delay_ms(50);
		return 0;
	}
	else
	{
		printf("成功断开与当前服务器的连接\r\n");
		Delay_ms(50);
		return 1;
	}
}


//为了方便连接不同服务器的时候获取不同的数据，将其分离出来写一个新的函数
void SendcmdtoServer(char *sendbuf)
{
	memset(&WIFI_Data,0,sizeof(WIFI_Data));
	WIFI_SendStr(sendbuf);
	while(WIFI_Data.USART_RevOverFlag != 1)
	{}
}




//限定时间在dest字符串中查找src字符，返回查找结果，找不到返回空指针，否则返回目的字符串的指针
char *FindStr(char *dest, char *src, uint32_t timeout)
{
	//状态检测机制，在限定时间内查找指定的字符串
	while(timeout-- && (strstr(dest,src) == NULL))
	{
		Delay_ms(1);
	}
	return strstr(dest,src);		//找到了返回值为非空指针，否则返回值为空指针
}


//发送指令并检测是否收到预期的效果
/*
	cmd是发送的命令，ack是要检测的返回结果，timeout是设置的超时时间，times是尝试的次数
	
*/
//u8 WIFI_Sendcmd_Recack(char *cmd,char *ack,u32 timeout,u8 times)
//{
//	//u32 len = strlen((char *)cmd);			//
//	WIFI_Data.RX_count = 0;
//	memset(WIFI_Data.RX_buff,0,sizeof(WIFI_Data.RX_buff));		//接收消息的结构体清零
//	while(times--)
//	{
//		strcat(cmd,"\r\n");			//为命令添加换行符
//		if(cmd != NULL)
//		{
//			WIFI_SendStr(cmd);
//		}
//		if(ack != NULL)
//		{
//			if(FindStr((char *)WIFI_Data.RX_buff,ack,timeout) != NULL)
//			{
//				WIFI_Data.RX_count=0;
//				return 0;
//			}
//		}
//		//times--;
//	}
//	
//	return 1;			//失败返回1
//}
u8  WIFI_Sendcmd_Recack(char *cmd, char *ack, uint32_t timeout, uint8_t check_cnt)
{
	uint32_t len = strlen((char *)cmd);
	WIFI_Data.RX_count = 0;
	memset(WIFI_Data.RX_buff, 0, sizeof(WIFI_Data.RX_buff));

	while(check_cnt--)
	{
		//发送命令
		if(cmd != NULL)
		{
			WIFI_SendArray((uint8_t *)cmd, len);
			WIFI_SendStr("\r\n");
		}
		//查询应答字符串
		if(ack != NULL)
		{
			if(FindStr((char *)WIFI_Data.RX_buff, ack, timeout) != NULL) //等待串口接收应答，并解析
			{
				WIFI_Data.RX_count = 0;
				return 0;//接收到正确应答
			}
		}
	}
	return 1;//失败
}


char Now_temperature[3];
//解析收到的当天天气json字符串，解析成功返回1，失败返回0
u8 Analyse_TodayWeatherJson(void)
{
	cJSON *json_root;		//创建一个json对象
	json_root = cJSON_Parse(WIFI_Data.RX_buff);	//生成一个存储要解析字符串的链表，注意内存消耗的问题
	if(json_root == NULL)
	{
		printf("要解析的字符串无效，解析失败\r\n");
		return 0;
	}
	cJSON *json_results = cJSON_GetObjectItem(json_root,"results");		//从对象中获取根节点对象中的数据
	if(json_results == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_array_0 = cJSON_GetArrayItem(json_results,0);		//打开数组下标0，获取对象
	if(json_array_0 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_location = cJSON_GetObjectItem(json_array_0,"location");
	if(json_location == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_name = cJSON_GetObjectItem(json_location,"name");
	if(json_name == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("\r\n城市:%s\r\n",json_name->valuestring);
	
	cJSON *json_now = cJSON_GetObjectItem(json_array_0,"now");
	if(json_now == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_temperature = cJSON_GetObjectItem(json_now,"temperature");
	if(json_temperature == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("现在温度：%s\r\n",json_temperature->valuestring);
	strcpy(Now_temperature,json_temperature->valuestring);
	cJSON *json_code = cJSON_GetObjectItem(json_now,"code");
	if(json_temperature == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("天气代码：%s\r\n",json_code->valuestring);
	
	cJSON *json_text = cJSON_GetObjectItem(json_now,"text");
	if(json_temperature == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("天气：%s\r\n",json_text->valuestring);
	
	
	strcpy(weather_stringdata.cityname,json_name->valuestring);
	strcpy(weather_stringdata.code,json_code->valuestring);
	strcpy(weather_stringdata.temperature,json_temperature->valuestring);
	
	
	cJSON_Delete(json_root);				//删除根节点，释放空间
	return 1;
}



//解析收到的来自心知天气的三天天气的json字符串
u8 Analyse_CityWeatherJson(char *cityname)
{
	u8 i;
	if(strcmp(cityname,"zhengzhou") == 0)
	{
		i=0;
	}
	else if(strcmp(cityname,"beijing") == 0)
	{
		i=1;
	}
	else if(strcmp(cityname,"shanghai") == 0)
	{
		i=2;
	}
	else if(strcmp(cityname,"guangzhou") == 0)		
	{
		i=3;
	}
	else if(strcmp(cityname,"shenzhen") == 0)		
	{
		i=4;
	}
	
	memset(weather_day3stringdata,0,sizeof(weather_day3stringdata));
	cJSON *json_root;		//创建一个json对象
	json_root = cJSON_Parse(WIFI_Data.RX_buff);	//生成一个存储要解析字符串的链表，注意内存消耗的问题
	if(json_root == NULL)
	{
		printf("要解析的字符串无效，解析失败\r\n");
		return 0;
	}
	cJSON *json_results = cJSON_GetObjectItem(json_root,"results");		//从对象中获取根节点对象中的数据
	if(json_results == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_array_0 = cJSON_GetArrayItem(json_results,0);		//打开数组下标0，获取对象
	if(json_array_0 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_location = cJSON_GetObjectItem(json_array_0,"location");
	if(json_location == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_name = cJSON_GetObjectItem(json_location,"name");
	if(json_name == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("\r\n城市:%s\r\n",json_name->valuestring);
	for(u8 j=0;j<3;j++)
	{
		strcpy(city5_weather[i].acity_weather[j].cityname,json_name->valuestring);
	}		//三天的城市都是一样的
	
	cJSON *json_daily = cJSON_GetObjectItem(json_array_0,"daily");
	if(json_daily == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_array_00 = cJSON_GetArrayItem(json_daily,0);		//打开数组下标0，获取对象			
	if(json_array_00 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	
	cJSON *json_array_01 = cJSON_GetArrayItem(json_daily,1);		//打开数组下标1，获取对象			
	if(json_array_01 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_array_02 = cJSON_GetArrayItem(json_daily,2);		//打开数组下标2，获取对象			
	if(json_array_02 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	//解析当天数据
	cJSON *json_low = cJSON_GetObjectItem(json_array_00,"low");
	if(json_low == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("今天最低温度：%s\r\n",json_low->valuestring);
	strcpy(city5_weather[i].acity_weather[0].lowt,json_low->valuestring);
	
	cJSON *json_high = cJSON_GetObjectItem(json_array_00,"high");
	if(json_high == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("今天最高温度：%s\r\n",json_high->valuestring);
	strcpy(city5_weather[i].acity_weather[0].hight,json_high->valuestring);
	
	cJSON *json_code_day = cJSON_GetObjectItem(json_array_00,"code_day");
	if(json_code_day == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("今天白天天气代码：%s\r\n",json_code_day->valuestring);
	strcpy(city5_weather[i].acity_weather[0].code,json_code_day->valuestring);
	
	//解析明天数据
	json_low = cJSON_GetObjectItem(json_array_01,"low");
	if(json_low == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("明天最低温度：%s\r\n",json_low->valuestring);
	strcpy(city5_weather[i].acity_weather[1].lowt,json_low->valuestring);
	
	json_high = cJSON_GetObjectItem(json_array_01,"high");
	if(json_high == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("明天最高温度：%s\r\n",json_high->valuestring);
	strcpy(city5_weather[i].acity_weather[1].hight,json_high->valuestring);
	
	json_code_day = cJSON_GetObjectItem(json_array_01,"code_day");
	if(json_code_day == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("明天白天天气代码：%s\r\n",json_code_day->valuestring);
	strcpy(city5_weather[i].acity_weather[1].code,json_code_day->valuestring);
	
	
	//解析后天数据
	json_low = cJSON_GetObjectItem(json_array_02,"low");
	if(json_low == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("后天最低温度：%s\r\n",json_low->valuestring);
	strcpy(city5_weather[i].acity_weather[2].lowt,json_low->valuestring);
	
	json_high = cJSON_GetObjectItem(json_array_02,"high");
	if(json_high == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("后天最高温度：%s\r\n",json_high->valuestring);
	strcpy(city5_weather[i].acity_weather[2].hight,json_high->valuestring);
	
	json_code_day = cJSON_GetObjectItem(json_array_02,"code_day");
	if(json_code_day == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("后天白天天气代码：%s\r\n",json_code_day->valuestring);
	strcpy(city5_weather[i].acity_weather[2].code,json_code_day->valuestring);
	
//	cJSON *json_code = cJSON_GetObjectItem(json_now,"code");
//	if(json_temperature == NULL)
//	{
//		cJSON_Delete(json_root);
//		return 0;
//	}
//	printf("天气代码：%s\r\n",json_code->valuestring);
//	
//	cJSON *json_text = cJSON_GetObjectItem(json_now,"text");
//	if(json_temperature == NULL)
//	{
//		cJSON_Delete(json_root);
//		return 0;
//	}
//	printf("天气：%s\r\n",json_text->valuestring);
//	
//	
//	strcpy(weather_stringdata.cityname,json_name->valuestring);
//	strcpy(weather_stringdata.code,json_code->valuestring);
//	strcpy(weather_stringdata.temperature,json_temperature->valuestring);
	
	
	cJSON_Delete(json_root);				//删除根节点，释放空间
	return 1;
}


//解析时间的json数据
time_stringdataStruct time_stringdata = {0};
u8 Analyse_TimeJson(void)
{
	cJSON *json_root = cJSON_Parse(WIFI_Data.RX_buff);
	if(json_root == NULL)
	{
		printf("要解析的字符串无效\r\n");
		return 0;
	}
	cJSON *json_success = cJSON_GetObjectItem(json_root,"success");
	if(atoi(json_success->valuestring) != 1)
	{
		cJSON_Delete(json_root);
		return 0;					//解析时间失败
	}
	cJSON *json_result = cJSON_GetObjectItem(json_root,"result");
	if(json_result == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_timestamp = cJSON_GetObjectItem(json_result,"timestamp");
	if(json_timestamp == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_datetime_1 = cJSON_GetObjectItem(json_result,"datetime_1");
	if(json_datetime_1 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_week_1 = cJSON_GetObjectItem(json_result,"week_1");
	if(json_week_1 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("\r\n当前时间：%s\r\n",json_datetime_1->valuestring);
	printf("今天是周%s\r\n",json_week_1->valuestring);
	
	strcpy(time_stringdata.timestamp,json_timestamp->valuestring);
	strcpy(time_stringdata.datetime,json_datetime_1->valuestring);		//存储datetime
	strcpy(time_stringdata.week,json_week_1->valuestring);						//存储week
	
	cJSON_Delete(json_root);			//释放空间
	return 1;											//解析时间成功
}


/*
USART3_TX	PB10		复用推挽	
USART3_RX	PB11	    浮空输入 
EN          PE6         推挽输出，复位开发板
*/
void WIFI_USART_Config(void)
{
	//开时钟
	WIFI_USARTPeriphClockCmd(WIFI_USART_PeriphClock,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	//配IO口模式
	GPIO_InitTypeDef GPIO_InitStruct;//定义结构体变量
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//配USART的工作模式 
	USART_InitTypeDef WIFI_USART_InitStruct;
	WIFI_USART_InitStruct.USART_BaudRate = 115200;//波特率
	WIFI_USART_InitStruct.USART_WordLength =USART_WordLength_8b;//数据位长度
	
	//硬件控制失能
	WIFI_USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//硬件流控关闭
	WIFI_USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;									//使能发送 和 接收 功能
	WIFI_USART_InitStruct.USART_StopBits = USART_StopBits_1;												//停止位1位
	WIFI_USART_InitStruct.USART_Parity = USART_Parity_No;														//奇偶校验  不开
	USART_Init(WIFI_USART,&WIFI_USART_InitStruct);
	USART_Cmd(WIFI_USART,ENABLE);																										//使能串口1
	
	//接收中断配置
	USART_ITConfig(WIFI_USART,USART_IT_RXNE,ENABLE);
	//空闲中断配置
	USART_ITConfig(WIFI_USART,USART_IT_IDLE,ENABLE);
	
	NVIC_EnableIRQ(WIFI_USART_IRQn);
	NVIC_SetPriority(WIFI_USART_IRQn,1);		//00 01
	
	WIFI_DISABLE;
	Delay_ms(50);
	WIFI_ENABLE;
	
}





#include "usart1.h"
//中断服务函数 
void WIFI_IRQHandler(void)
{	
	 
	if(USART_GetITStatus(WIFI_USART,USART_IT_RXNE) ==SET)
	{
		USART_ClearITPendingBit(WIFI_USART,USART_IT_RXNE);
		u8 data = WIFI_USART->DR ;					//串口3接收数据
//		USART1_SendData(data);
		USART1 ->DR=data;										//将串口3的数据给串口1
		WIFI_Data.RX_buff[WIFI_Data.RX_count]= data;	//将串口3的数据给wifi结构体
		WIFI_Data.RX_count++;
//		printf("%d\r\n",WIFI_Data.RX_count);
//		Delay_ms(50);
		if(WIFI_Data.RX_count >= WIFI_SIZE) 
			WIFI_Data.RX_count =0;
			
	}
	if(USART_GetITStatus(WIFI_USART,USART_IT_IDLE) ==SET)			//空闲总线中断发生，数据接收完成
	{
		u8 flag = WIFI_USART->SR ;
		flag = WIFI_USART->DR ;						//通过读DR和SR清除空闲中断，其实flag无用
		WIFI_Data.USART_RevOverFlag =1;		//接收完成标志
	}	
	
}

void WIFI_SendStr(char * p)
{
	while(*p != '\0')
	{
		WIFI_SendData(*p);
		p++;
	}
}

void WIFI_SendArray(u8 Arr[] ,u16 DataLenth)
{
	u16 i=0;
	while(DataLenth --)
	{
		WIFI_SendData(Arr[i++]);
	}
}

void WIFI_SendData(u8 data)
{
	//等待上一帧数据发送完成
	while(USART_GetFlagStatus(WIFI_USART,USART_FLAG_TC) == RESET)
	{}
	USART_SendData(WIFI_USART,data);				
}

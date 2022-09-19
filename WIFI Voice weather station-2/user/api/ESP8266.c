#include "ESP8266.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "cJSON.h"
#include "stdlib.h"
#include "my1680.h"





WIFI_DataStruct  WIFI_Data={0};
weather_stringdataStruct weather_stringdata={0};			//�ṹ���ʼ��Ϊ0
CITY_WEATHER city5_weather[5];		//�洢֣�ݱ��Ϲ���������е����� 
weather_stringdataStruct weather_day3stringdata[3];								//��ʼ��֣��3�������Ľṹ��

/*
	ʵ�ֶ�ESP8266ģ�鷢��������Ҽ�������Ƿ�õ���Ӧ��
	�õ���Ӧ֮���ڼ���ִ����һ�������������֪�����ķ�����
	��ȡ��������

***����ʵ�ֵĽṹ��ԭ�������ɷ��ͺ��������������Ƿ��յ�
�����WIFI_Sendcmd_Recack()���������þ���ִ��ָ�����������
��ʱʱ�䣬�����ط�������������Ƿ��յ�Ԥ�ڵĽ�����ڸú����ڲ��ֵ���
FindStr()�������ڼ����ָ������ʱʱ�����Ƿ��յ���Ӧ���ַ������
���������ʹ�õ����жϽ����ַ���������洢��WIFI_Data.RX_buff
�У���������ȫ�ֱ���������ֱ����FindStr�����жԱ��Ƿ���ڼ��ɡ�
������Ҫע�⣬�������ʱʱ��Ӧ���������������Զ�������wifi�ͷ�
������ʱ��Ƚϳ���

*/



//��ʼ��WIFI���ã������Ӿ�������
u8 ESP8266_Init()
{
	WIFI_USART_Config();	//����3���ú��ж����ã��Լ�WIFI�Ĵ���3����
	Delay_ms(1000);
	if( Connect_AP(MYSSID,MYPASWD) == 1)	//�����ȵ�
	{
		printf("wifi����ʧ��\r\n");
		return 0;
	}
	else
	{
		printf("wifi���ӳɹ�\r\n");
		Play_MP3_dir(01,12);						//wifi���ӳɹ���ʾ��
		return 1;
	}
}

u8 GET_City5Weather(void)
{
	
	if(!GET_Weather("zhengzhou"))			//��ȡ���ҽ���֣�ݵ���������
	{
		return 0;
	}
	
	if(!GET_Weather("beijing"))			//��ȡ����
	{
		return 0;
	}

	if(!GET_Weather("shanghai"))			//��ȡ�Ϻ�
	{
		return 0;
	}

	if(!GET_Weather("guangzhou"))			//��ȡ����
	{
		return 0;
	}
	if(!GET_Weather("shenzhen"))			//��ȡ����
	{
		return 0;
	}
	return 1;
}




//��ȡָ�����е�����
u8 GET_Weather(char *CityWeather)
{
	char sendbuf[512] = {0};
	Connect_Server("api.seniverse.com",80);					//������֪������������ȡ��������
//		Connect_Server("api.seniverse.com",80);					//������֪������������ȡ��������
	//������ȡ������������ʱֻ�ڻ�ȡ������еĺ���������һ�η�����
	//��ȡ���������
	sprintf(sendbuf,"GET https://api.seniverse.com/v3/weather/now.json?\
key=%s&location=%s&language=en&unit=c\r\n",mykey,CityWeather);
	//api:https://api.seniverse.com/v3/weather/now.json?key=SNUfoDKrWHdMRHpuV&location=Zhengzhou&language=en&unit=c
	
	sprintf(sendbuf,"GET https://api.seniverse.com/v3/weather/daily.json?\
key=%s&location=%s&language=en&unit=c&start=0&days=5\r\n",mykey,CityWeather);
	SendcmdtoServer(sendbuf);
	
	if(  Analyse_CityWeatherJson(CityWeather) )		//����������������ݣ������ǳ�����
	{
		printf("�������ݽ����ɹ�\r\n");
		//Play_MP3_dir(01,11);							//��ӭ��ʾ��
	}
	else
	{
		printf("�������ݽ���ʧ��\r\n");
		return 0;
	}
	if( Disconnect_Server() == 0)		//�Ͽ��������������
	{
		return 0;		//û����ȷ���˳�������������ӣ������쳣����
	}
	return 1;
	
}

u8 Get_Weather1(void)
{
		char sendbuf[512] = {0};
	Connect_Server("api.seniverse.com",80);					//������֪������������ȡ��������
//		Connect_Server("api.seniverse.com",80);					//������֪������������ȡ��������
	//������ȡ������������ʱֻ�ڻ�ȡ������еĺ���������һ�η�����
	//��ȡ���������
	sprintf(sendbuf,"GET https://api.seniverse.com/v3/weather/now.json?\
key=%s&location=%s&language=en&unit=c\r\n",mykey,"Zhengzhou");
	//api:https://api.seniverse.com/v3/weather/now.json?key=SNUfoDKrWHdMRHpuV&location=Zhengzhou&language=en&unit=c
	
	SendcmdtoServer(sendbuf);
	
	if(  Analyse_TodayWeatherJson() )		//�����������������
	{
		printf("�������ݽ����ɹ�\r\n");
		//Play_MP3_dir(01,11);							//��ӭ��ʾ��
	}
	else
	{
		printf("�������ݽ���ʧ��\r\n");
		return 0;
	}
	if( Disconnect_Server() == 0)		//�Ͽ��������������
	{
		return 0;		//û����ȷ���˳�������������ӣ������쳣����
	}
	return 1;
}

//�˳�͸��ģʽ
void EXIT_Scotch(void)
{
	WIFI_SendStr("+++");		//ע������û�л��У������޷��˳�͸��ģʽ
	Delay_ms(300);
}


//����wifi
void ConnectWifi(char *ssid,char *paswd)
{
	char ord[128]={0};
	sprintf("AT+\"%s\",\"%s\"\r\n",ssid,paswd);
	WIFI_SendStr(ord);
	Delay_ms(2000);
}



//����ָ��������
/*
	����1��IP��ַ��������������2�Ƕ˿ں�
	
*/


//�����ȵ㣬�ɹ�����0��ʧ�ܷ��ط�0
u8 Connect_AP(char *ssid,char *paswd)
{
	char sendarr[256]={'\0'};
	EXIT_Scotch();//�˳�͸��ģʽ
	if( WIFI_Sendcmd_Recack("AT","OK",300,2) )
	{
		printf("AT ����ʧ��\r\n");
		return 1;
	}else
		printf("AT ���Գɹ�\r\n");
	
	if( WIFI_Sendcmd_Recack("AT+CWMODE=1","OK",500,2) )
	{
		printf("AT+CWMODE ����ʧ��\r\n");
		return 2;
	}else
		printf("AT+CWMODE ���óɹ�\r\n");	
	
	sprintf(sendarr,"AT+CWJAP=\"%s\",\"%s\"",ssid,paswd);
	if(WIFI_Sendcmd_Recack(sendarr,"OK",10000,2))
	{
		printf("AT+CWJAP ����ʧ��\r\n");
		return 3;
	}else
	{
		printf("AT+CWJAP ���óɹ�\r\n");	
		printf("�ȵ����ӳɹ�\r\n");	
	}		
	return 0;
	
}

/**
 * ���ܣ����� TCP ������ ����͸��
 * ������
 *      IP: ������ ���� ���� IP��ַ
*	  PORT: �������˿ں�
 * ����ֵ���ɹ����� 0  ʧ��1/2/3/4
 *						 
 */
u8  Connect_Server(char *IP ,u16 PORT)
{
	char sendarr[128]={0};
	if(WIFI_Sendcmd_Recack("AT+CWJAP?","OK",5000,2) == 1)
	{
		printf("δ��⵽�����ȵ�\r\n������....\r\n");
		if(Connect_AP(MYSSID,MYPASWD))
		{
			printf("δ��⵽�ȵ�,�˳�!!\r\n");
			return 1;
		}
	}
//4��	���ӷ����������ӷ�������Ҫ ����������IP�����ж˿ں� 
//		AT+CIPSTART=���������ͣ�TCP/UDP�������� ��������IP�����˿ں�	
	sprintf(sendarr,"AT+CIPSTART=\"TCP\",\"%s\",%d",IP,PORT);
	if(WIFI_Sendcmd_Recack(sendarr,"OK",2000,2) == 1)
	{
		printf("���ӷ�����ʧ�ܣ��˳�\r\n");
		return  2;
	}	
//5��	����͸��ģʽ  AT+CIPMODE=1;  
	if(WIFI_Sendcmd_Recack("AT+CIPMODE=1","OK",500,2) == 1)
	{
		printf("����͸��ģʽʧ�ܣ��˳�\r\n");
		return 3;
	}
//6��	��ʼ���ݴ��䣺AT+CIPSEND   ����ָ���յ���>������ɹ����뷢��ģʽ	
	if(WIFI_Sendcmd_Recack("AT+CIPSEND",">",500,2) == 1)			
	//����͸��ģʽ֮��ESP8266�յ������е��ַ������ᵱ���������������
	{
		printf("����͸��ģʽʧ�ܣ��˳�\r\n");
		return 4;
	}
	
	return 0;
}


//�Ͽ��뵱ǰ�����������ӣ��ɹ�����1��ʧ�ܷ���0
u8 Disconnect_Server()
{
	Delay_ms(500);		//�˳�͸������̫�����ָ���޷���Ч�����¹رշ��������ӵ��޷��ر�
	EXIT_Scotch();	//�˳�͸��ģʽ
	
	if(WIFI_Sendcmd_Recack("AT+CIPCLOSE","OK",1000,2) == 1)
	{
		printf("��������Ͽ�ʧ�ܣ��˳�\r\n");
		Delay_ms(50);
		return 0;
	}
	else
	{
		printf("�ɹ��Ͽ��뵱ǰ������������\r\n");
		Delay_ms(50);
		return 1;
	}
}


//Ϊ�˷������Ӳ�ͬ��������ʱ���ȡ��ͬ�����ݣ�����������дһ���µĺ���
void SendcmdtoServer(char *sendbuf)
{
	memset(&WIFI_Data,0,sizeof(WIFI_Data));
	WIFI_SendStr(sendbuf);
	while(WIFI_Data.USART_RevOverFlag != 1)
	{}
}




//�޶�ʱ����dest�ַ����в���src�ַ������ز��ҽ�����Ҳ������ؿ�ָ�룬���򷵻�Ŀ���ַ�����ָ��
char *FindStr(char *dest, char *src, uint32_t timeout)
{
	//״̬�����ƣ����޶�ʱ���ڲ���ָ�����ַ���
	while(timeout-- && (strstr(dest,src) == NULL))
	{
		Delay_ms(1);
	}
	return strstr(dest,src);		//�ҵ��˷���ֵΪ�ǿ�ָ�룬���򷵻�ֵΪ��ָ��
}


//����ָ�����Ƿ��յ�Ԥ�ڵ�Ч��
/*
	cmd�Ƿ��͵����ack��Ҫ���ķ��ؽ����timeout�����õĳ�ʱʱ�䣬times�ǳ��ԵĴ���
	
*/
//u8 WIFI_Sendcmd_Recack(char *cmd,char *ack,u32 timeout,u8 times)
//{
//	//u32 len = strlen((char *)cmd);			//
//	WIFI_Data.RX_count = 0;
//	memset(WIFI_Data.RX_buff,0,sizeof(WIFI_Data.RX_buff));		//������Ϣ�Ľṹ������
//	while(times--)
//	{
//		strcat(cmd,"\r\n");			//Ϊ������ӻ��з�
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
//	return 1;			//ʧ�ܷ���1
//}
u8  WIFI_Sendcmd_Recack(char *cmd, char *ack, uint32_t timeout, uint8_t check_cnt)
{
	uint32_t len = strlen((char *)cmd);
	WIFI_Data.RX_count = 0;
	memset(WIFI_Data.RX_buff, 0, sizeof(WIFI_Data.RX_buff));

	while(check_cnt--)
	{
		//��������
		if(cmd != NULL)
		{
			WIFI_SendArray((uint8_t *)cmd, len);
			WIFI_SendStr("\r\n");
		}
		//��ѯӦ���ַ���
		if(ack != NULL)
		{
			if(FindStr((char *)WIFI_Data.RX_buff, ack, timeout) != NULL) //�ȴ����ڽ���Ӧ�𣬲�����
			{
				WIFI_Data.RX_count = 0;
				return 0;//���յ���ȷӦ��
			}
		}
	}
	return 1;//ʧ��
}


char Now_temperature[3];
//�����յ��ĵ�������json�ַ����������ɹ�����1��ʧ�ܷ���0
u8 Analyse_TodayWeatherJson(void)
{
	cJSON *json_root;		//����һ��json����
	json_root = cJSON_Parse(WIFI_Data.RX_buff);	//����һ���洢Ҫ�����ַ���������ע���ڴ����ĵ�����
	if(json_root == NULL)
	{
		printf("Ҫ�������ַ�����Ч������ʧ��\r\n");
		return 0;
	}
	cJSON *json_results = cJSON_GetObjectItem(json_root,"results");		//�Ӷ����л�ȡ���ڵ�����е�����
	if(json_results == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_array_0 = cJSON_GetArrayItem(json_results,0);		//�������±�0����ȡ����
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
	printf("\r\n����:%s\r\n",json_name->valuestring);
	
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
	printf("�����¶ȣ�%s\r\n",json_temperature->valuestring);
	strcpy(Now_temperature,json_temperature->valuestring);
	cJSON *json_code = cJSON_GetObjectItem(json_now,"code");
	if(json_temperature == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("�������룺%s\r\n",json_code->valuestring);
	
	cJSON *json_text = cJSON_GetObjectItem(json_now,"text");
	if(json_temperature == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("������%s\r\n",json_text->valuestring);
	
	
	strcpy(weather_stringdata.cityname,json_name->valuestring);
	strcpy(weather_stringdata.code,json_code->valuestring);
	strcpy(weather_stringdata.temperature,json_temperature->valuestring);
	
	
	cJSON_Delete(json_root);				//ɾ�����ڵ㣬�ͷſռ�
	return 1;
}



//�����յ���������֪����������������json�ַ���
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
	cJSON *json_root;		//����һ��json����
	json_root = cJSON_Parse(WIFI_Data.RX_buff);	//����һ���洢Ҫ�����ַ���������ע���ڴ����ĵ�����
	if(json_root == NULL)
	{
		printf("Ҫ�������ַ�����Ч������ʧ��\r\n");
		return 0;
	}
	cJSON *json_results = cJSON_GetObjectItem(json_root,"results");		//�Ӷ����л�ȡ���ڵ�����е�����
	if(json_results == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	cJSON *json_array_0 = cJSON_GetArrayItem(json_results,0);		//�������±�0����ȡ����
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
	printf("\r\n����:%s\r\n",json_name->valuestring);
	for(u8 j=0;j<3;j++)
	{
		strcpy(city5_weather[i].acity_weather[j].cityname,json_name->valuestring);
	}		//����ĳ��ж���һ����
	
	cJSON *json_daily = cJSON_GetObjectItem(json_array_0,"daily");
	if(json_daily == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_array_00 = cJSON_GetArrayItem(json_daily,0);		//�������±�0����ȡ����			
	if(json_array_00 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	
	cJSON *json_array_01 = cJSON_GetArrayItem(json_daily,1);		//�������±�1����ȡ����			
	if(json_array_01 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	cJSON *json_array_02 = cJSON_GetArrayItem(json_daily,2);		//�������±�2����ȡ����			
	if(json_array_02 == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	
	//������������
	cJSON *json_low = cJSON_GetObjectItem(json_array_00,"low");
	if(json_low == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������¶ȣ�%s\r\n",json_low->valuestring);
	strcpy(city5_weather[i].acity_weather[0].lowt,json_low->valuestring);
	
	cJSON *json_high = cJSON_GetObjectItem(json_array_00,"high");
	if(json_high == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������¶ȣ�%s\r\n",json_high->valuestring);
	strcpy(city5_weather[i].acity_weather[0].hight,json_high->valuestring);
	
	cJSON *json_code_day = cJSON_GetObjectItem(json_array_00,"code_day");
	if(json_code_day == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������������룺%s\r\n",json_code_day->valuestring);
	strcpy(city5_weather[i].acity_weather[0].code,json_code_day->valuestring);
	
	//������������
	json_low = cJSON_GetObjectItem(json_array_01,"low");
	if(json_low == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������¶ȣ�%s\r\n",json_low->valuestring);
	strcpy(city5_weather[i].acity_weather[1].lowt,json_low->valuestring);
	
	json_high = cJSON_GetObjectItem(json_array_01,"high");
	if(json_high == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������¶ȣ�%s\r\n",json_high->valuestring);
	strcpy(city5_weather[i].acity_weather[1].hight,json_high->valuestring);
	
	json_code_day = cJSON_GetObjectItem(json_array_01,"code_day");
	if(json_code_day == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������������룺%s\r\n",json_code_day->valuestring);
	strcpy(city5_weather[i].acity_weather[1].code,json_code_day->valuestring);
	
	
	//������������
	json_low = cJSON_GetObjectItem(json_array_02,"low");
	if(json_low == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������¶ȣ�%s\r\n",json_low->valuestring);
	strcpy(city5_weather[i].acity_weather[2].lowt,json_low->valuestring);
	
	json_high = cJSON_GetObjectItem(json_array_02,"high");
	if(json_high == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������¶ȣ�%s\r\n",json_high->valuestring);
	strcpy(city5_weather[i].acity_weather[2].hight,json_high->valuestring);
	
	json_code_day = cJSON_GetObjectItem(json_array_02,"code_day");
	if(json_code_day == NULL)
	{
		cJSON_Delete(json_root);
		return 0;
	}
	printf("��������������룺%s\r\n",json_code_day->valuestring);
	strcpy(city5_weather[i].acity_weather[2].code,json_code_day->valuestring);
	
//	cJSON *json_code = cJSON_GetObjectItem(json_now,"code");
//	if(json_temperature == NULL)
//	{
//		cJSON_Delete(json_root);
//		return 0;
//	}
//	printf("�������룺%s\r\n",json_code->valuestring);
//	
//	cJSON *json_text = cJSON_GetObjectItem(json_now,"text");
//	if(json_temperature == NULL)
//	{
//		cJSON_Delete(json_root);
//		return 0;
//	}
//	printf("������%s\r\n",json_text->valuestring);
//	
//	
//	strcpy(weather_stringdata.cityname,json_name->valuestring);
//	strcpy(weather_stringdata.code,json_code->valuestring);
//	strcpy(weather_stringdata.temperature,json_temperature->valuestring);
	
	
	cJSON_Delete(json_root);				//ɾ�����ڵ㣬�ͷſռ�
	return 1;
}


//����ʱ���json����
time_stringdataStruct time_stringdata = {0};
u8 Analyse_TimeJson(void)
{
	cJSON *json_root = cJSON_Parse(WIFI_Data.RX_buff);
	if(json_root == NULL)
	{
		printf("Ҫ�������ַ�����Ч\r\n");
		return 0;
	}
	cJSON *json_success = cJSON_GetObjectItem(json_root,"success");
	if(atoi(json_success->valuestring) != 1)
	{
		cJSON_Delete(json_root);
		return 0;					//����ʱ��ʧ��
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
	printf("\r\n��ǰʱ�䣺%s\r\n",json_datetime_1->valuestring);
	printf("��������%s\r\n",json_week_1->valuestring);
	
	strcpy(time_stringdata.timestamp,json_timestamp->valuestring);
	strcpy(time_stringdata.datetime,json_datetime_1->valuestring);		//�洢datetime
	strcpy(time_stringdata.week,json_week_1->valuestring);						//�洢week
	
	cJSON_Delete(json_root);			//�ͷſռ�
	return 1;											//����ʱ��ɹ�
}


/*
USART3_TX	PB10		��������	
USART3_RX	PB11	    �������� 
EN          PE6         �����������λ������
*/
void WIFI_USART_Config(void)
{
	//��ʱ��
	WIFI_USARTPeriphClockCmd(WIFI_USART_PeriphClock,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	//��IO��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;//����ṹ�����
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//��USART�Ĺ���ģʽ 
	USART_InitTypeDef WIFI_USART_InitStruct;
	WIFI_USART_InitStruct.USART_BaudRate = 115200;//������
	WIFI_USART_InitStruct.USART_WordLength =USART_WordLength_8b;//����λ����
	
	//Ӳ������ʧ��
	WIFI_USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//Ӳ�����عر�
	WIFI_USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;									//ʹ�ܷ��� �� ���� ����
	WIFI_USART_InitStruct.USART_StopBits = USART_StopBits_1;												//ֹͣλ1λ
	WIFI_USART_InitStruct.USART_Parity = USART_Parity_No;														//��żУ��  ����
	USART_Init(WIFI_USART,&WIFI_USART_InitStruct);
	USART_Cmd(WIFI_USART,ENABLE);																										//ʹ�ܴ���1
	
	//�����ж�����
	USART_ITConfig(WIFI_USART,USART_IT_RXNE,ENABLE);
	//�����ж�����
	USART_ITConfig(WIFI_USART,USART_IT_IDLE,ENABLE);
	
	NVIC_EnableIRQ(WIFI_USART_IRQn);
	NVIC_SetPriority(WIFI_USART_IRQn,1);		//00 01
	
	WIFI_DISABLE;
	Delay_ms(50);
	WIFI_ENABLE;
	
}





#include "usart1.h"
//�жϷ����� 
void WIFI_IRQHandler(void)
{	
	 
	if(USART_GetITStatus(WIFI_USART,USART_IT_RXNE) ==SET)
	{
		USART_ClearITPendingBit(WIFI_USART,USART_IT_RXNE);
		u8 data = WIFI_USART->DR ;					//����3��������
//		USART1_SendData(data);
		USART1 ->DR=data;										//������3�����ݸ�����1
		WIFI_Data.RX_buff[WIFI_Data.RX_count]= data;	//������3�����ݸ�wifi�ṹ��
		WIFI_Data.RX_count++;
//		printf("%d\r\n",WIFI_Data.RX_count);
//		Delay_ms(50);
		if(WIFI_Data.RX_count >= WIFI_SIZE) 
			WIFI_Data.RX_count =0;
			
	}
	if(USART_GetITStatus(WIFI_USART,USART_IT_IDLE) ==SET)			//���������жϷ��������ݽ������
	{
		u8 flag = WIFI_USART->SR ;
		flag = WIFI_USART->DR ;						//ͨ����DR��SR��������жϣ���ʵflag����
		WIFI_Data.USART_RevOverFlag =1;		//������ɱ�־
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
	//�ȴ���һ֡���ݷ������
	while(USART_GetFlagStatus(WIFI_USART,USART_FLAG_TC) == RESET)
	{}
	USART_SendData(WIFI_USART,data);				
}

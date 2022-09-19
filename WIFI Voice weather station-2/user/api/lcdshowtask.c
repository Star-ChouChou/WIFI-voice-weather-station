#include "lcdshowtask.h"

LV_FONT_DECLARE(font1);	

//�ڲ���������
void Show_Interface1(void);
void Show_Interface2(void);
void Create_LV_Task_interface1(void);
void Del_LV_Task_interface1(void);
void Show_Interface3(void);
void play_XCity_xDay_weather(char *city,int i);

LV_IMG_DECLARE(cloudy);				//����
LV_IMG_DECLARE(qing);					//��
//LV_IMG_DECLARE(baoyu);				//����
LV_IMG_DECLARE(zhongyu);			//����
LV_IMG_DECLARE(zhenyu);				//����
LV_IMG_DECLARE(yin);					//��
LV_IMG_DECLARE(xiaoyu);				//С��
LV_IMG_DECLARE(duoyun);				//����
LV_IMG_DECLARE(dayu);					//����
LV_IMG_DECLARE(leizhenyu);		//������


char CNWord[20];					//��ʾ�����ĺ����ı��ַ���


u8 nowui_flag = 1;			//��ǰ��ʾҳ��ı�־λ
u8 change_flag = 0;			//�л�ҳ���ֹ�ظ����Ƶı�־λ
u8 nowcity_flag = 1;				//��ǰ��ʾ�ĵڼ������е�����
u8 change_cityflag = 0;	//��ֹ�ػ��ͷͼ��ı�־λ




int Get_info_from_Internet(void)
{
	if(!ESP8266_Init())		//���ӳɹ�����1������ʧ�ܷ���0
	{
		printf("WIFI����ʧ��\r\n");
		return -1;
	}
	printf("\r\n��ʼ�����\r\n");
	if(!RTC_TIME_Init())		//��ʼ��RTC��У׼RTCʱ��
	{
		return -1;			
	}
	Get_Weather1();
	if(!GET_City5Weather())
	{
		printf("��ȡ5������ʧ��\r\n");
		return -1;
	}
}


//����������ת��ΪUTF-8�����ı�������Ĳ�������������
void Weathercode_to_CNWord(int code,int day)
{
	memset(CNWord,0,sizeof(CNWord));			
	switch(code)
	{
		case 0:	;
		case 1:	;
		case 2:	;
		case 3:	
				strcpy(CNWord,"晴 "); 
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&qing);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&qing);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&qing);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&qing);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
						lv_img_set_src(img3_weather_today,&qing);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
						lv_img_set_src(img3_weather_tommorrow,&qing);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
						lv_img_set_src(img3_weather_aftertomm,&qing);
				}
																						break;			//��
		case 4:	;																	
		case 5:	;
		case 6:	;																	
		case 7:	;
		case 8:	
				strcpy(CNWord,"多云 ");
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&duoyun);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&duoyun);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&duoyun);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&duoyun);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&duoyun);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&duoyun);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&duoyun);
				}																																
																					break;			//����
		case 9:	
				strcpy(CNWord,"阴 ");
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&yin);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&yin);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&yin);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&yin);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&yin);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&yin);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&yin);
				}																																
																				break;			//��
		case 10:
				strcpy(CNWord,"阵雨 ");
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&zhenyu);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&zhenyu);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&zhenyu);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&zhenyu);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&zhenyu);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&zhenyu);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&zhenyu);
				}																																
																				break;			//����
		case 11:
				strcpy(CNWord,"雷阵雨 ");	
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&leizhenyu);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&leizhenyu);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&leizhenyu);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&leizhenyu);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&leizhenyu);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&leizhenyu);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&leizhenyu);
				}																																
																				break;			//������
		case 13:
				strcpy(CNWord,"小雨 ");		
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&xiaoyu);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&xiaoyu);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&xiaoyu);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&xiaoyu);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&xiaoyu);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&xiaoyu);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&xiaoyu);
				}																																
																				break;			//С��
		case 14:
				strcpy(CNWord,"中雨 ");		
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&zhongyu);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&zhongyu);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&zhongyu);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&zhongyu);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&zhongyu);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&zhongyu);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&zhongyu);
				}																															
																				break;			//����
		case 15:
				strcpy(CNWord,"大雨 ");	
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&dayu);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&dayu);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&dayu);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&dayu);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&dayu);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&dayu);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&dayu);
				}																																
																					break;			//����
		case 16:
				strcpy(CNWord,"暴雨 ");	
				if(nowui_flag == 1)
				{					
					lv_img_set_src(img_weather,&dayu);																	
				}
				else if((nowui_flag ==2)&&(day == 1))
				{
					lv_img_set_src(img2_weather_today,&dayu);
				}
				else if((nowui_flag == 2)&&(day == 2))
				{
					lv_img_set_src(img2_weather_tommorrow,&dayu);
				}
				else if((nowui_flag == 2)&&(day == 3))
				{
					lv_img_set_src(img2_weather_aftertomm,&dayu);
				}
				else if((nowui_flag == 3)&&(day == 1))
				{
					lv_img_set_src(img3_weather_today,&dayu);
				}
				else if((nowui_flag == 3)&&(day == 2))
				{
					lv_img_set_src(img3_weather_tommorrow,&dayu);
				}
				else if((nowui_flag == 3)&&(day == 3))
				{
					lv_img_set_src(img3_weather_aftertomm,&dayu);
				}																														
																						break;			//����
		case 30:
				strcpy(CNWord,"雾 ");			
																						break;			//��
	}
}

/*
	������ȵ�˼·��
	1������ʹ��lvgl��������ȹ���ʵ��������ʱ��Ƭ�Ĺ��ܣ�
		�����ʱ��Ƭ��������������ȼ��ĸ������
	2��
	3��
*/




char tmpbuff[50];
//LED״̬����˸����1S��˸һ��
void Led1_TogleTask(lv_task_t * task)
{
	LED1_Toggle;
}

//�ɼ���ʪ������
void getdht_Task(lv_task_t *task)
{
		Get_dhtdata();
		
	//	printf("temp:%d,humi:%d\r\n",dht11_data.TMP,dht11_data.HUM);
}


//ҳ��1��ʾ��ʪ������
void showdht_Task(lv_task_t *task)
{
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%d℃ ",dht11_data.TMP);
	lv_label_set_text(label_TEM,tmpbuff);					//��ʾ�¶�

	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%d%%",dht11_data.HUM);
	lv_label_set_text(label_HUM,tmpbuff);					//��ʾʪ��
}


//ҳ��1��ʾʱ�������1sˢ��һ��
void showtime_Task(lv_task_t *task)
{
		memset(tmpbuff,0,sizeof(tmpbuff));
		sprintf(tmpbuff,"%02d:%02d:%02d",calendar.hour,calendar.min,calendar.sec);
		lv_label_set_text(label_time, tmpbuff);					//��ʾʱ�䣬LVGL��֧�����ģ���������Ŀ�
		memset(tmpbuff,0,sizeof(tmpbuff));
		sprintf(tmpbuff,"%04d-%02d-%02d",calendar.w_year,calendar.w_month,calendar.w_date);
		lv_label_set_text(label_date,tmpbuff);					//��ʾ����
	
		memset(tmpbuff,0,sizeof(tmpbuff));
		sprintf(tmpbuff,"星期%d ",calendar.week);
		lv_label_set_text(label_week,tmpbuff);					//��ʾ"����x"��x�����ָ�ʽ
}

//ҳ��1��ʾ��ǿ�Ϳ�������������2sˢ��һ��
void showLP_Task(lv_task_t *task)
{
//		printf("����ģ��ֵ��%d\r\n",ADCValue[0]);			//�ɼ���ǿ������Ҫ�ظ���ʼ������ʼ��֮��տ�ʼ�ɼ������������ܴ�
		switch(ADCValue[0]/1000)
		{
			case 0:	;
			case 1:
//				printf("����ǿ�ȣ�����\r\n");
				lv_label_set_text(label_light,"很亮 ");
			break;
			
			case 2:
//				printf("����ǿ�ȣ���\r\n");
				lv_label_set_text(label_light,"亮 ");			
			break;
			
			case 3:
//				printf("����ǿ�ȣ�����\r\n");		
				lv_label_set_text(label_light,"适中 ");
			break;
			
			case 4:
//				printf("����ǿ�ȣ���\r\n");
				lv_label_set_text(label_light,"暗 ");
			break;
		}
		
//		printf("����ģ��ֵ��%d\r\n",ADCValue[1]);
		switch(ADCValue[1]/100)
		{
			case 0:
//				printf("������������\r\n");
				lv_label_set_text(label_ppm,"优 ");
			break;
			
			case 1:
//				printf("������������\r\n");
				lv_label_set_text(label_ppm,"良 ");
			break;
			
			case 2:
//				printf("������������\r\n");
				lv_label_set_text(label_ppm,"中 ");
			break;
			
			case 3:
//				printf("������������\r\n");
				lv_label_set_text(label_ppm,"差 ");
			break;
			
			default:
//				printf("�����������ر��\r\n");
				lv_label_set_text(label_ppm,"特别差 ");
		}
}



//��ʾ�����ĺ����ı�
void show_weather_text(lv_task_t *task)
{
	
}



void key_Task(lv_task_t *task)
{
	switch(KEY_Check())							//������ĸ�����������
	{
		case 1:
//			printf("����1����\r\n");
			change_flag = 1;				
		//��־λ����л���������л�����ֹһֱ�ػ�Ҫ�л���ҳ�棬��ռ�ô���ϵͳʱ�� 
			nowui_flag++;
			if(nowui_flag == 4)
			{
				nowui_flag=1;
			}																		
																		break;
		
		case 2:	
//			printf("����2����\r\n");			
			if(nowui_flag == 3)		//��ҳ��3��ʱ���л��Ż���Ч
			{
				change_cityflag=1;
				nowcity_flag++;
			}
			else if(nowui_flag == 2)				//ҳ��2��ʱ����Ч
			{
//					Play_Day3("Zhengzhou",city5_weather[0].acity_weather[0].code,city5_weather[0].acity_weather[0].lowt,city5_weather[0].acity_weather[0].hight,0);
						play_XCity_xDay_weather("Zhengzhou",0);	
			}
			if(nowcity_flag == 5)
			{
				nowcity_flag = 1;
			}
			
																		break;

		case 3:	
//			printf("����3����\r\n");			
			if(nowui_flag == 2)
			{
//					Play_Day3("Zhengzhou",city5_weather[0].acity_weather[1].code,city5_weather[0].acity_weather[1].lowt,city5_weather[0].acity_weather[1].hight,1);
									play_XCity_xDay_weather("Zhengzhou",1);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 1))
			{
									play_XCity_xDay_weather("Beijing",0);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 2))
			{
									play_XCity_xDay_weather("Shanghai",0);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 3))
			{
									play_XCity_xDay_weather("Guangzhou",0);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 4))
			{
									play_XCity_xDay_weather("Shenzhen",0);	
			}
																		break;

		case 4:
//			printf("����4����\r\n");			
			if(nowui_flag == 2)
			{
//					Play_Day3("Zhengzhou",city5_weather[0].acity_weather[2].code,city5_weather[0].acity_weather[2].lowt,city5_weather[0].acity_weather[2].hight,2);
									play_XCity_xDay_weather("Zhengzhou",2);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 1))
			{
									play_XCity_xDay_weather("Beijing",1);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 2))
			{
									play_XCity_xDay_weather("Shanghai",1);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 3))
			{
									play_XCity_xDay_weather("Guangzhou",1);	
			}
			else if((nowui_flag == 3)&&(nowcity_flag == 4))
			{
									play_XCity_xDay_weather("Shenzhen",1);	
			}

																		break;
	}
}

void play_XCity_xDay_weather(char *city,int i)
{
	if(strcmp(city,"Zhengzhou") == 0)
	{
			Play_Day3("Zhengzhou",city5_weather[0].acity_weather[i].code,city5_weather[0].acity_weather[i].lowt,city5_weather[0].acity_weather[i].hight,i);
	}
	else if(strcmp(city,"Beijing") == 0)
	{
			Play_Day3("Beijing",city5_weather[1].acity_weather[i].code,city5_weather[1].acity_weather[i].lowt,city5_weather[1].acity_weather[i].hight,i);
	}
		else if(strcmp(city,"Shanghai") == 0)
	{
			Play_Day3("Shanghai",city5_weather[2].acity_weather[i].code,city5_weather[2].acity_weather[i].lowt,city5_weather[2].acity_weather[i].hight,i);
	}
	else if(strcmp(city,"Guangzhou") == 0)
	{
			Play_Day3("Guangzhou",city5_weather[3].acity_weather[i].code,city5_weather[3].acity_weather[i].lowt,city5_weather[3].acity_weather[i].hight,i);
	}
	else if(strcmp(city,"Shenzhen") == 0)
	{
		Play_Day3("Shenzhen",city5_weather[4].acity_weather[i].code,city5_weather[4].acity_weather[i].lowt,city5_weather[4].acity_weather[i].hight,i);
	}
	
}





/*
	�л�ҳ��ĺ�����10ms���һ�Σ��˺�����ʵ���Էŵ����������
	�л���־λ��⺯�������Ҫ�л���ʾ��ҳ�棬����Ƿ�����л�
	nowui_flag��������ʾ��ҳ�棬change_flag���Ƿ���Ч��ǰҳ��
*/
void interface_change(lv_task_t *task)
{
	if((nowui_flag == 2)&&(change_flag == 1))				//�л���ʾҳ��2
	{
		change_flag = 0;
//		printf("�л���ҳ��2\r\n");
		Del_LV_Task_interface1();				//ɾ��ҳ��1��ˢ���ı����񣬷�����
		lv_obj_del(controot1);					//ɾ��ҳ��1�ĸ�����
		Show_Interface2();							//����ҳ��2�ĸ���������ʾ��ǩ
	}
	else if((nowui_flag == 3)&&(change_flag == 1))	//�л���ʾҳ��3
	{
		change_flag = 0;
		nowcity_flag = 1;
		change_cityflag = 1; 
//		printf("�л���ҳ��1\r\n");
		lv_obj_del(controot2);
		Show_Interface3();
	}	
	else if((nowui_flag == 1)&&(change_flag == 1))		//�л���ʾҳ��1
	{
		change_flag = 0;
		lv_obj_del(controot3);
		Create_LV_Task_interface1();			//����ҳ��1���ı�ʱˢ������
		Show_Interface1();								//��������������ʾ��ǩ
	}
}

void change_arrow(lv_task_t *task)
{
//	if((nowcity_flag<5)&&(nowcity_flag>0))
//		if(nowui_flag == 3)
//	{
//		printf("���ڵ�nowcity=%d",nowcity_flag);
//	}
	if((nowui_flag == 3)&&(nowcity_flag ==2)&&(change_cityflag == 1))					//ָ���Ϻ�		
	{
			change_cityflag = 0; 
			printf("��ͷ��λ1\r\n");
			lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,80);			//�ı��ͷָ��λ��
			day1_weather_show(2);
			day2_weather_show(2);
			day3_weather_show(2);
	}
	else if((nowui_flag == 3)&&(nowcity_flag ==3)&&(change_cityflag == 1))		//ָ�����
	{
			change_cityflag = 0;
			printf("��ͷ��λ2\r\n");
			lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,120);
			day1_weather_show(3);
			day2_weather_show(3);
			day3_weather_show(3);
	}
	else if((nowui_flag == 3)&&(nowcity_flag ==4)&&(change_cityflag == 1))		//ָ������
	{
		change_cityflag = 0;
		printf("��ͷ��λ3\r\n");
		lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,160);
			day1_weather_show(4);
			day2_weather_show(4);
			day3_weather_show(4);
		
	}
	else if((nowui_flag == 3)&&(nowcity_flag ==1)&&(change_cityflag == 1))		//ָ��֣��
	{
		change_cityflag = 0;
		printf("��ͷ��λ4\r\n");
		lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,40);
			day1_weather_show(1);
			day2_weather_show(1);
			day3_weather_show(1);
	}
}





//ҳ��1��ʾ�¶ȵĺ���
void interface1_showWeather_Task(void)
{
//	//��ʾ��������
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%s℃ ",city5_weather[0].acity_weather[0].hight);
	lv_label_set_text(label_weather1_hight,tmpbuff);
	
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%s℃ ",city5_weather[0].acity_weather[0].lowt);
	lv_label_set_text(label_weather1_lowt,tmpbuff);
	
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[0].code),1);
	lv_label_set_text(label_weather_text,CNWord);		
}

lv_obj_t *label_init_text=NULL,*controot5=NULL;
lv_style_t init_style;
//չʾ��ʼ����ʾ����
void show_Initwords(void)
{
	//������ʽ
	lv_style_copy(&init_style,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	init_style.text.font = &font1;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	init_style.text.color = LV_COLOR_BLACK;
	
    
//	controot5 = lv_label_create(lv_scr_act(), NULL);
//	lv_obj_set_size(controot5,320,240);										//���������Ĵ�С
//	lv_obj_align(controot5,NULL,LV_ALIGN_CENTER,0,0);			//��Ļ���ж���
//	lv_cont_set_layout(controot5,LV_LAYOUT_OFF);					//�����޲��ַ�ʽ
	
	/*�ڵ�ǰ�����Ļ�ϴ���һ��Label*/
	label_init_text = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_style(label_init_text,LV_LABEL_STYLE_MAIN,&init_style);
	lv_obj_align(label_init_text, NULL, LV_ALIGN_CENTER, -32, 0);
	lv_label_set_text(label_init_text,"loading... ");
	lv_task_handler();
}






lv_task_t *Task1_Handle = NULL;
lv_task_t *Task2_Handle = NULL;
lv_task_t *Task3_Handle = NULL;
lv_task_t *Task4_Handle = NULL;
lv_task_t *Task5_Handle = NULL;
lv_task_t *Task6_Handle = NULL;
lv_task_t *Task7_Handle = NULL;
lv_task_t *Task8_Handle = NULL;
lv_task_t *Task9_Handle = NULL;
/*
	�����õ��ĺ���
	lv_task_ready(lv_task_t *task);	��������ص�����׼����ִ�У��˴β�������ִ��
*/
//ϵͳ��ʼ������
void SYSTEM_Init()
{
	SysTick_Config(SystemCoreClock/1000);
	BEEP_Init();
	Led_Config();
	KEY_Init();	
	RTC_Init();					//��ʼ��RTCs
	USART1_Config();		//����1��ʼ��
	USART4_Config();
	printf("ϵͳ��ʼ����\r\n");						//��������ٴ������ú�֮�󣬷���ᵼ������
	
	TFTLCD_Init();			//��Ļ�ײ�������ʼ��
	sFLASH_Init();
	lv_init();					//��ʼ��LVGL,�м����lvgl��ʽ�ĳ�ʼ��
	lv_port_disp_init();//��ʼ����ʾ�ӿ�
	light_resistance_adcConfig();		//��ʼ��ADC����ȡģ����ֵ
	Get_dhtdata();					//�Ȳ�һ����ʪ�ȣ�����ϵͳ�����ȴ��ſ�����ʾ��ʪ��
	show_Initwords();
}

void refresh_weather(lv_task_t *task)				//ˢ����������
{
	Get_Weather1();
	if(!GET_City5Weather())
	{
		printf("��ȡ5������ʧ��\r\n");
//		return -1;
	}
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%s�?",Now_temperature);
	lv_label_set_text(label_nowtemper, tmpbuff);								//��ǰ�¶�

}



//����lv����������������ʼ����һ��
void Create_LV_Task(void)
{	
	lv_obj_del(label_init_text);
	Create_LV_Task_interface1();		//ˢ��ҳ��1�������񣬴���֮�������Ҫʱ��ִ��,ע�����һ�����Ĵ���ʹ��
	Show_Interface1();							//����Ĭ����ʾҳ��1������ҳ��1����ʾ��ǩ

	Task1_Handle = lv_task_create(Led1_TogleTask,1000,LV_TASK_PRIO_MID,NULL);					//��������
	Task2_Handle = lv_task_create(getdht_Task,5000,LV_TASK_PRIO_HIGH,NULL);						//�ɼ���ʪ������
	Task5_Handle = lv_task_create(key_Task,10,LV_TASK_PRIO_HIGH,NULL);								//�����������
	Task7_Handle = lv_task_create(interface_change,10,LV_TASK_PRIO_HIGH,NULL);				//����Ƿ���Ҫ�л�ҳ�������
	Task8_Handle = lv_task_create(change_arrow,10,LV_TASK_PRIO_HIGH,NULL);
	Task9_Handle = lv_task_create(refresh_weather,36000000,LV_TASK_PRIO_LOWEST,NULL);	//ˢ����������
//	printf("\r\n���ڹ۲��ʱ����\r\n");
}

//����ҳ��1��Ҫˢ�µ�����
void Create_LV_Task_interface1(void)
{
	Task3_Handle = lv_task_create(showtime_Task,1000,LV_TASK_PRIO_HIGHEST,NULL);			//ˢ����ʾʱ������
	Task4_Handle = lv_task_create(showLP_Task,2000,LV_TASK_PRIO_LOWEST,NULL);					//ˢ����ʾ����͹�������
	Task6_Handle = lv_task_create(showdht_Task,5000,LV_TASK_PRIO_HIGH,NULL);					//ˢ����ʾ��ʪ������
}


//ɾ��ҳ��1��ˢ�������������ˢ���ı��Ῠ��
void Del_LV_Task_interface1(void)
{
	lv_task_del(Task3_Handle);
	lv_task_del(Task4_Handle);
	lv_task_del(Task6_Handle);
}

//void Create_LV_Task_interface3(void)
//{
//	Task8_Handle = lv_task_create(change_arrow,10,LV_TASK_PRIO_HIGH,NULL);
//}

//ҳ��1��ʾ��������������������ʾ��ǩ������
void Show_Interface1(void)
{
	lv_interface1_show_picture();				//��ʾ��ҳ��ͼƬ
	lv_interface1_show_time();					//��ʾʱ��
	lv_show_LP();												//��ʾ��ǿ������
	lv_show_dht11();										//��ʾ��ʪ��
	lv_interface1_show_weather();				//��ʾ����
	lv_interface1_show_text();					//��ʾѡ������
	interface1_showWeather_Task();			//��ʾ��������
}



//ҳ��2��ʾ����
void Show_Interface2(void)
{
	lv_interface2_show_picture();				//��ʾ������ҳ��
	lv_interface2_show_text();					//��ʾ������ ���� ���족�ı�
}


void Show_Interface3(void)
{
	lv_interface3_show_picture();
	lv_interface3_show_text();
}






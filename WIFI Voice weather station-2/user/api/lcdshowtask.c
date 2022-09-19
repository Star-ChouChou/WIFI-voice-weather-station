#include "lcdshowtask.h"

LV_FONT_DECLARE(font1);	

//ÄÚ²¿º¯ÊıÉùÃ÷
void Show_Interface1(void);
void Show_Interface2(void);
void Create_LV_Task_interface1(void);
void Del_LV_Task_interface1(void);
void Show_Interface3(void);
void play_XCity_xDay_weather(char *city,int i);

LV_IMG_DECLARE(cloudy);				//¶àÔÆ
LV_IMG_DECLARE(qing);					//Çç
//LV_IMG_DECLARE(baoyu);				//±©Óê
LV_IMG_DECLARE(zhongyu);			//ÖĞÓê
LV_IMG_DECLARE(zhenyu);				//ÕóÓê
LV_IMG_DECLARE(yin);					//Òõ
LV_IMG_DECLARE(xiaoyu);				//Ğ¡Óê
LV_IMG_DECLARE(duoyun);				//¶àÔÆ
LV_IMG_DECLARE(dayu);					//´óÓê
LV_IMG_DECLARE(leizhenyu);		//À×ÕóÓê


char CNWord[20];					//ÏÔÊ¾ÌìÆøµÄºº×ÖÎÄ±¾×Ö·û´®


u8 nowui_flag = 1;			//µ±Ç°ÏÔÊ¾Ò³ÃæµÄ±êÖ¾Î»
u8 change_flag = 0;			//ÇĞ»»Ò³Ãæ·ÀÖ¹ÖØ¸´»æÖÆµÄ±êÖ¾Î»
u8 nowcity_flag = 1;				//µ±Ç°ÏÔÊ¾µÄµÚ¼¸¸ö³ÇÊĞµÄÌìÆø
u8 change_cityflag = 0;	//·ÀÖ¹ÖØ»æ¼ıÍ·Í¼±êµÄ±êÖ¾Î»




int Get_info_from_Internet(void)
{
	if(!ESP8266_Init())		//Á¬½Ó³É¹¦·µ»Ø1£¬Á¬½ÓÊ§°Ü·µ»Ø0
	{
		printf("WIFIÁ¬½ÓÊ§°Ü\r\n");
		return -1;
	}
	printf("\r\n³õÊ¼»¯Íê³É\r\n");
	if(!RTC_TIME_Init())		//³õÊ¼»¯RTC£¬Ğ£×¼RTCÊ±¼ä
	{
		return -1;			
	}
	Get_Weather1();
	if(!GET_City5Weather())
	{
		printf("»ñÈ¡5ÌìÌìÆøÊ§°Ü\r\n");
		return -1;
	}
}


//½«ÌìÆø´úÂë×ª»»ÎªUTF-8ºº×ÖÎÄ±¾£¬´«ÈëµÄ²ÎÊıÊÇÌìÆø´úÂë
void Weathercode_to_CNWord(int code,int day)
{
	memset(CNWord,0,sizeof(CNWord));			
	switch(code)
	{
		case 0:	;
		case 1:	;
		case 2:	;
		case 3:	
				strcpy(CNWord,"æ™´ "); 
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
																						break;			//Çç
		case 4:	;																	
		case 5:	;
		case 6:	;																	
		case 7:	;
		case 8:	
				strcpy(CNWord,"å¤šäº‘ ");
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
																					break;			//¶àÔÆ
		case 9:	
				strcpy(CNWord,"é˜´ ");
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
																				break;			//Òõ
		case 10:
				strcpy(CNWord,"é˜µé›¨ ");
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
																				break;			//ÕóÓê
		case 11:
				strcpy(CNWord,"é›·é˜µé›¨ ");	
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
																				break;			//À×ÕóÓê
		case 13:
				strcpy(CNWord,"å°é›¨ ");		
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
																				break;			//Ğ¡Óê
		case 14:
				strcpy(CNWord,"ä¸­é›¨ ");		
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
																				break;			//ÖĞÓê
		case 15:
				strcpy(CNWord,"å¤§é›¨ ");	
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
																					break;			//´óÓê
		case 16:
				strcpy(CNWord,"æš´é›¨ ");	
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
																						break;			//±©Óê
		case 30:
				strcpy(CNWord,"é›¾ ");			
																						break;			//Îí
	}
}

/*
	ÈÎÎñµ÷¶ÈµÄË¼Â·£º
	1¡¢ÕâÀïÊ¹ÓÃlvglµÄÈÎÎñµ÷¶È¹¦ÄÜÊµÏÖÀàËÆÓÚÊ±¼äÆ¬µÄ¹¦ÄÜ£¬
		Ïà¶ÔÓÚÊ±¼äÆ¬£¬ÈÎÎñµ÷¶ÈÓĞÓÅÏÈ¼¶µÄ¸ÅÄî£¬ËùÒÔ
	2¡¢
	3¡¢
*/




char tmpbuff[50];
//LED×´Ì¬µÆÉÁË¸ÈÎÎñ£¬1SÉÁË¸Ò»´Î
void Led1_TogleTask(lv_task_t * task)
{
	LED1_Toggle;
}

//²É¼¯ÎÂÊª¶ÈÈÎÎñ
void getdht_Task(lv_task_t *task)
{
		Get_dhtdata();
		
	//	printf("temp:%d,humi:%d\r\n",dht11_data.TMP,dht11_data.HUM);
}


//Ò³Ãæ1ÏÔÊ¾ÎÂÊª¶ÈÈÎÎñ
void showdht_Task(lv_task_t *task)
{
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%dâ„ƒ ",dht11_data.TMP);
	lv_label_set_text(label_TEM,tmpbuff);					//ÏÔÊ¾ÎÂ¶È

	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%d%%",dht11_data.HUM);
	lv_label_set_text(label_HUM,tmpbuff);					//ÏÔÊ¾Êª¶È
}


//Ò³Ãæ1ÏÔÊ¾Ê±¼äµÄÈÎÎñ£¬1sË¢ĞÂÒ»´Î
void showtime_Task(lv_task_t *task)
{
		memset(tmpbuff,0,sizeof(tmpbuff));
		sprintf(tmpbuff,"%02d:%02d:%02d",calendar.hour,calendar.min,calendar.sec);
		lv_label_set_text(label_time, tmpbuff);					//ÏÔÊ¾Ê±¼ä£¬LVGL²»Ö§³ÖÖĞÎÄ£¬µÃÌí¼ÓÖĞÎÄ¿â
		memset(tmpbuff,0,sizeof(tmpbuff));
		sprintf(tmpbuff,"%04d-%02d-%02d",calendar.w_year,calendar.w_month,calendar.w_date);
		lv_label_set_text(label_date,tmpbuff);					//ÏÔÊ¾ÈÕÆÚ
	
		memset(tmpbuff,0,sizeof(tmpbuff));
		sprintf(tmpbuff,"æ˜ŸæœŸ%d ",calendar.week);
		lv_label_set_text(label_week,tmpbuff);					//ÏÔÊ¾"ĞÇÆÚx"£¬xÊÇÊı×Ö¸ñÊ½
}

//Ò³Ãæ1ÏÔÊ¾¹âÇ¿ºÍ¿ÕÆøÖÊÁ¿µÄÈÎÎñ£¬2sË¢ĞÂÒ»´Î
void showLP_Task(lv_task_t *task)
{
//		printf("¹âÕÕÄ£ÄâÖµ£º%d\r\n",ADCValue[0]);			//²É¼¯¹âÇ¿ºÍÑÌÎí²»ÒªÖØ¸´³õÊ¼»¯£¬³õÊ¼»¯Ö®ºó¸Õ¿ªÊ¼²É¼¯µ½µÄÊı¾İÎó²îºÜ´ó
		switch(ADCValue[0]/1000)
		{
			case 0:	;
			case 1:
//				printf("¹âÕÕÇ¿¶È£ººÜÁÁ\r\n");
				lv_label_set_text(label_light,"å¾ˆäº® ");
			break;
			
			case 2:
//				printf("¹âÕÕÇ¿¶È£ºÁÁ\r\n");
				lv_label_set_text(label_light,"äº® ");			
			break;
			
			case 3:
//				printf("¹âÕÕÇ¿¶È£ºÊÊÖĞ\r\n");		
				lv_label_set_text(label_light,"é€‚ä¸­ ");
			break;
			
			case 4:
//				printf("¹âÕÕÇ¿¶È£º°µ\r\n");
				lv_label_set_text(label_light,"æš— ");
			break;
		}
		
//		printf("ÑÌÎíÄ£ÄâÖµ£º%d\r\n",ADCValue[1]);
		switch(ADCValue[1]/100)
		{
			case 0:
//				printf("¿ÕÆøÖÊÁ¿£ºÓÅ\r\n");
				lv_label_set_text(label_ppm,"ä¼˜ ");
			break;
			
			case 1:
//				printf("¿ÕÆøÖÊÁ¿£ºÁ¼\r\n");
				lv_label_set_text(label_ppm,"è‰¯ ");
			break;
			
			case 2:
//				printf("¿ÕÆøÖÊÁ¿£ºÖĞ\r\n");
				lv_label_set_text(label_ppm,"ä¸­ ");
			break;
			
			case 3:
//				printf("¿ÕÆøÖÊÁ¿£º²î\r\n");
				lv_label_set_text(label_ppm,"å·® ");
			break;
			
			default:
//				printf("¿ÕÆøÖÊÁ¿£ºÌØ±ğ²î\r\n");
				lv_label_set_text(label_ppm,"ç‰¹åˆ«å·® ");
		}
}



//ÏÔÊ¾ÌìÆøµÄºº×ÖÎÄ±¾
void show_weather_text(lv_task_t *task)
{
	
}



void key_Task(lv_task_t *task)
{
	switch(KEY_Check())							//¼ì²âÊÇÄÄ¸ö°´¼ü°´ÏÂÁË
	{
		case 1:
//			printf("°´¼ü1°´ÏÂ\r\n");
			change_flag = 1;				
		//±êÖ¾Î»¼ì²âÇĞ»»ÈÎÎñ½øĞĞÇĞ»»£¬·ÀÖ¹Ò»Ö±ÖØ»æÒªÇĞ»»µÄÒ³Ãæ£¬»áÕ¼ÓÃ´óÁ¿ÏµÍ³Ê±¼ä 
			nowui_flag++;
			if(nowui_flag == 4)
			{
				nowui_flag=1;
			}																		
																		break;
		
		case 2:	
//			printf("°´¼ü2°´ÏÂ\r\n");			
			if(nowui_flag == 3)		//ÔÚÒ³Ãæ3µÄÊ±ºòÇĞ»»²Å»áÉúĞ§
			{
				change_cityflag=1;
				nowcity_flag++;
			}
			else if(nowui_flag == 2)				//Ò³Ãæ2µÄÊ±ºòÓĞĞ§
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
//			printf("°´¼ü3°´ÏÂ\r\n");			
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
//			printf("°´¼ü4°´ÏÂ\r\n");			
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
	ÇĞ»»Ò³ÃæµÄº¯Êı£¬10ms¼ì²âÒ»´Î£¬´Ëº¯ÊıÆäÊµ¿ÉÒÔ·Åµ½°´¼ü¼ì²âÖĞ
	ÇĞ»»±êÖ¾Î»¼ì²âº¯Êı£¬¼ì²âÒªÇĞ»»ÏÔÊ¾µÄÒ³Ãæ£¬¼ì²âÊÇ·ñ¿ÉÒÔÇĞ»»
	nowui_flag£ºÏÖÔÚÏÔÊ¾µÄÒ³Ãæ£¬change_flag£ºÊÇ·ñÉúĞ§µ±Ç°Ò³Ãæ
*/
void interface_change(lv_task_t *task)
{
	if((nowui_flag == 2)&&(change_flag == 1))				//ÇĞ»»ÏÔÊ¾Ò³Ãæ2
	{
		change_flag = 0;
//		printf("ÇĞ»»µ½Ò³Ãæ2\r\n");
		Del_LV_Task_interface1();				//É¾³ıÒ³Ãæ1µÄË¢ĞÂÎÄ±¾ÈÎÎñ£¬·ñÔò¿¨ËÀ
		lv_obj_del(controot1);					//É¾³ıÒ³Ãæ1µÄ¸ùÈİÆ÷
		Show_Interface2();							//´´½¨Ò³Ãæ2µÄ¸ùÈİÆ÷ºÍÏÔÊ¾±êÇ©
	}
	else if((nowui_flag == 3)&&(change_flag == 1))	//ÇĞ»»ÏÔÊ¾Ò³Ãæ3
	{
		change_flag = 0;
		nowcity_flag = 1;
		change_cityflag = 1; 
//		printf("ÇĞ»»µ½Ò³Ãæ1\r\n");
		lv_obj_del(controot2);
		Show_Interface3();
	}	
	else if((nowui_flag == 1)&&(change_flag == 1))		//ÇĞ»»ÏÔÊ¾Ò³Ãæ1
	{
		change_flag = 0;
		lv_obj_del(controot3);
		Create_LV_Task_interface1();			//´´½¨Ò³Ãæ1µÄÎÄ±¾Ê±Ë¢ĞÂÈÎÎñ
		Show_Interface1();								//´´½¨¸ùÈİÆ÷ºÍÏÔÊ¾±êÇ©
	}
}

void change_arrow(lv_task_t *task)
{
//	if((nowcity_flag<5)&&(nowcity_flag>0))
//		if(nowui_flag == 3)
//	{
//		printf("ÏÖÔÚµÄnowcity=%d",nowcity_flag);
//	}
	if((nowui_flag == 3)&&(nowcity_flag ==2)&&(change_cityflag == 1))					//Ö¸ÏòÉÏº£		
	{
			change_cityflag = 0; 
			printf("¼ıÍ·ÒÆÎ»1\r\n");
			lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,80);			//¸Ä±ä¼ıÍ·Ö¸ÏòÎ»ÖÃ
			day1_weather_show(2);
			day2_weather_show(2);
			day3_weather_show(2);
	}
	else if((nowui_flag == 3)&&(nowcity_flag ==3)&&(change_cityflag == 1))		//Ö¸Ïò¹ãÖİ
	{
			change_cityflag = 0;
			printf("¼ıÍ·ÒÆÎ»2\r\n");
			lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,120);
			day1_weather_show(3);
			day2_weather_show(3);
			day3_weather_show(3);
	}
	else if((nowui_flag == 3)&&(nowcity_flag ==4)&&(change_cityflag == 1))		//Ö¸ÏòÉîÛÚ
	{
		change_cityflag = 0;
		printf("¼ıÍ·ÒÆÎ»3\r\n");
		lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,160);
			day1_weather_show(4);
			day2_weather_show(4);
			day3_weather_show(4);
		
	}
	else if((nowui_flag == 3)&&(nowcity_flag ==1)&&(change_cityflag == 1))		//Ö¸ÏòÖ£Öİ
	{
		change_cityflag = 0;
		printf("¼ıÍ·ÒÆÎ»4\r\n");
		lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,40);
			day1_weather_show(1);
			day2_weather_show(1);
			day3_weather_show(1);
	}
}





//Ò³Ãæ1ÏÔÊ¾ÎÂ¶ÈµÄº¯Êı
void interface1_showWeather_Task(void)
{
//	//ÏÔÊ¾½ñÈÕÌìÆø
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%sâ„ƒ ",city5_weather[0].acity_weather[0].hight);
	lv_label_set_text(label_weather1_hight,tmpbuff);
	
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%sâ„ƒ ",city5_weather[0].acity_weather[0].lowt);
	lv_label_set_text(label_weather1_lowt,tmpbuff);
	
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[0].code),1);
	lv_label_set_text(label_weather_text,CNWord);		
}

lv_obj_t *label_init_text=NULL,*controot5=NULL;
lv_style_t init_style;
//Õ¹Ê¾³õÊ¼»¯ÌáÊ¾ÎÄ×Ö
void show_Initwords(void)
{
	//ÅäÖÃÑùÊ½
	lv_style_copy(&init_style,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	init_style.text.font = &font1;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	init_style.text.color = LV_COLOR_BLACK;
	
    
//	controot5 = lv_label_create(lv_scr_act(), NULL);
//	lv_obj_set_size(controot5,320,240);										//ÉèÖÃÈİÆ÷µÄ´óĞ¡
//	lv_obj_align(controot5,NULL,LV_ALIGN_CENTER,0,0);			//ÆÁÄ»¾ÓÖĞ¶ÔÆë
//	lv_cont_set_layout(controot5,LV_LAYOUT_OFF);					//ÉèÖÃÎŞ²¼¾Ö·½Ê½
	
	/*ÔÚµ±Ç°»î¶¯µÄÆÁÄ»ÉÏ´´½¨Ò»¸öLabel*/
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
	¿ÉÄÜÓÃµ½µÄº¯Êı
	lv_task_ready(lv_task_t *task);	Á¢¼´ÈÃÏà¹ØµÄÈÎÎñ×¼±¸ºÃÖ´ĞĞ£¬´Ë´Î²»°´ÖÜÆÚÖ´ĞĞ
*/
//ÏµÍ³³õÊ¼»¯º¯Êı
void SYSTEM_Init()
{
	SysTick_Config(SystemCoreClock/1000);
	BEEP_Init();
	Led_Config();
	KEY_Init();	
	RTC_Init();					//³õÊ¼»¯RTCs
	USART1_Config();		//´®¿Ú1³õÊ¼»¯
	USART4_Config();
	printf("ÏµÍ³¿ªÊ¼ÔËĞĞ\r\n");						//Êä³ö±ØĞëÔÙ´®¿ÚÅäÖÃºÃÖ®ºó£¬·ñÔò»áµ¼ÖÂ×èÈû
	
	TFTLCD_Init();			//ÆÁÄ»µ×²ãÇı¶¯³õÊ¼»¯
	sFLASH_Init();
	lv_init();					//³õÊ¼»¯LVGL,ÖĞ¼ä°üÀ¨lvglÑùÊ½µÄ³õÊ¼»¯
	lv_port_disp_init();//³õÊ¼»¯ÏÔÊ¾½Ó¿Ú
	light_resistance_adcConfig();		//³õÊ¼»¯ADC²¢»ñÈ¡Ä£ÄâÊıÖµ
	Get_dhtdata();					//ÏÈ²ÉÒ»´ÎÎÂÊª¶È£¬·ñÔòÏµÍ³Æô¶¯µÈ´ı²Å¿ÉÒÔÏÔÊ¾ÎÂÊª¶È
	show_Initwords();
}

void refresh_weather(lv_task_t *task)				//Ë¢ĞÂÌìÆøÊı¾İ
{
	Get_Weather1();
	if(!GET_City5Weather())
	{
		printf("»ñÈ¡5ÌìÌìÆøÊ§°Ü\r\n");
//		return -1;
	}
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%sâ„?",Now_temperature);
	lv_label_set_text(label_nowtemper, tmpbuff);								//µ±Ç°ÎÂ¶È

}



//´´½¨lvÈÎÎñº¯Êı£¬Ö÷º¯Êı¿ªÊ¼ÔËĞĞÒ»´Î
void Create_LV_Task(void)
{	
	lv_obj_del(label_init_text);
	Create_LV_Task_interface1();		//Ë¢ĞÂÒ³Ãæ1Êı¾İÈÎÎñ£¬´´½¨Ö®ºó³ÌĞòĞèÒªÊ±¼äÖ´ĞĞ,×¢ÒâºÍÏÂÒ»¸öÓï¾äµÄ´îÅäÊ¹ÓÃ
	Show_Interface1();							//¿ª»úÄ¬ÈÏÏÔÊ¾Ò³Ãæ1£¬ÅäÖÃÒ³Ãæ1µÄÏÔÊ¾±êÇ©

	Task1_Handle = lv_task_create(Led1_TogleTask,1000,LV_TASK_PRIO_MID,NULL);					//ÉÁµÆÈÎÎñ
	Task2_Handle = lv_task_create(getdht_Task,5000,LV_TASK_PRIO_HIGH,NULL);						//²É¼¯ÎÂÊª¶ÈÈÎÎñ
	Task5_Handle = lv_task_create(key_Task,10,LV_TASK_PRIO_HIGH,NULL);								//°´¼ü¼ì²âÈÎÎñ
	Task7_Handle = lv_task_create(interface_change,10,LV_TASK_PRIO_HIGH,NULL);				//¼ì²âÊÇ·ñĞèÒªÇĞ»»Ò³ÃæµÄÈÎÎñ
	Task8_Handle = lv_task_create(change_arrow,10,LV_TASK_PRIO_HIGH,NULL);
	Task9_Handle = lv_task_create(refresh_weather,36000000,LV_TASK_PRIO_LOWEST,NULL);	//Ë¢ĞÂÌìÆøÊı¾İ
//	printf("\r\nÓÃÓÚ¹Û²ìµÄÊ±¼ä½áµã\r\n");
}

//´´½¨Ò³Ãæ1ĞèÒªË¢ĞÂµÄÈÎÎñ
void Create_LV_Task_interface1(void)
{
	Task3_Handle = lv_task_create(showtime_Task,1000,LV_TASK_PRIO_HIGHEST,NULL);			//Ë¢ĞÂÏÔÊ¾Ê±¼äÈÎÎñ
	Task4_Handle = lv_task_create(showLP_Task,2000,LV_TASK_PRIO_LOWEST,NULL);					//Ë¢ĞÂÏÔÊ¾ÑÌÎíºÍ¹âÕÕÈÎÎñ
	Task6_Handle = lv_task_create(showdht_Task,5000,LV_TASK_PRIO_HIGH,NULL);					//Ë¢ĞÂÏÔÊ¾ÎÂÊª¶ÈÈÎÎñ
}


//É¾³ıÒ³Ãæ1µÄË¢ĞÂÈÎÎñ£¬Èç¹û¼ÌĞøË¢ĞÂÎÄ±¾»á¿¨ËÀ
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

//Ò³Ãæ1ÏÔÊ¾º¯Êı£¬´´½¨¸ùÈİÆ÷£¬ÏÔÊ¾±êÇ©µÄÉèÖÃ
void Show_Interface1(void)
{
	lv_interface1_show_picture();				//ÏÔÊ¾Ê×Ò³ÃæÍ¼Æ¬
	lv_interface1_show_time();					//ÏÔÊ¾Ê±¼ä
	lv_show_LP();												//ÏÔÊ¾¹âÇ¿ºÍÑÌÎí
	lv_show_dht11();										//ÏÔÊ¾ÎÂÊª¶È
	lv_interface1_show_weather();				//ÏÔÊ¾ÌìÆø
	lv_interface1_show_text();					//ÏÔÊ¾Ñ¡ÏîÎÄ×Ö
	interface1_showWeather_Task();			//ÏÔÊ¾ÌìÆøÊı¾İ
}



//Ò³Ãæ2ÏÔÊ¾º¯Êı
void Show_Interface2(void)
{
	lv_interface2_show_picture();				//ÏÔÊ¾Èı·ÖÆÁÒ³Ãæ
	lv_interface2_show_text();					//ÏÔÊ¾¡°½ñÌì Ã÷Ìì ºóÌì¡±ÎÄ±¾
}


void Show_Interface3(void)
{
	lv_interface3_show_picture();
	lv_interface3_show_text();
}






#ifndef STYLESETTING1_H_
#define	STYLESETTING1_H_

#include "lvgl.h"


extern lv_obj_t * label_time;
extern lv_obj_t *label_date,*label_week,*label_nowtemper;
extern lv_obj_t *controot1,*controot2,*controot3;
extern lv_obj_t *label_weather1_hight,*label_weather1_lowt,*label_weather2,*label_weather3;
extern lv_obj_t *label_TEM,*label_HUM,*label_light,*label_ppm,*label_weather_text;
extern lv_obj_t *img_weather,*img2_weather_today,*img2_weather_tommorrow,*img2_weather_aftertomm;
extern lv_obj_t *img3_weather_today,*img3_weather_tommorrow,*img3_weather_aftertomm;
extern lv_obj_t *interface3_img2;


void lv_interface1_show_picture(void);
void lv_interface1_show_time(void);
void lv_interface1_show_weather(void);
void lv_interface1_show_text(void);
void lv_show_dht11(void);
void lv_show_LP(void);

void lv_interface2_show_picture(void);
void lv_interface2_show_text(void);
void lv_interface2_show_weather(void);
void lv_interface3_show_picture(void);
void lv_interface3_show_text(void);
	
void day1_weather_show(int i);
void day2_weather_show(int i);
void day3_weather_show(int i);	
	
#endif



#ifndef _LCDSHOWTASK_H
#define _LCDSHOWTASK_H

#include "lvgl.h"
#include "stm32f10x.h"
#include "delay.h" //延时函数的头文件
#include "led.h"	//led灯控制文件
#include "beep.h"
#include "key.h"
#include "dht11.h "
#include "usart1.h"
#include "rtc.h" 
#include "adc.h"
#include "stdio.h" 
#include "bsp_lcd.h"
#include "ESP8266.h"
#include "my1680.h"
#include "string.h"
#include "cJSON.h"
#include "stdlib.h"
#include "spi_flash.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_tutorial_hello_world.h"
#include "stylesetting1.h"


extern lv_task_t *Task1_Handle;
extern lv_task_t *Task2_Handle;
extern lv_task_t *Task3_Handle;
extern lv_task_t *Task4_Handle;
extern lv_task_t *Task5_Handle;
extern lv_task_t *Task6_Handle;
extern char tmpbuff[50];
extern char CNWord[20];									//天气的中文表示
extern lv_obj_t *label_init_text;


void interface1_showWeather_Task(void);
void Create_LV_Task(void);
void SYSTEM_Init(void);
void Show_Interface1(void);
void lv_interface2_show_picture(void);
void Show_Interface2(void);
void Weathercode_to_CNWord(int code,int day);
int Get_info_from_Internet(void);



#endif



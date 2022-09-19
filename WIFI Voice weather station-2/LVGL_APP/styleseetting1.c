#include "stylesetting1.h"
#include "lv_tutorial_hello_world.h"
#include "stdio.h"
#include "lcdshowtask.h"
#include "ESP8266.h"


LV_IMG_DECLARE(qing);					//��

void day1_weather_show(int i);
void day2_weather_show(int i);
void day3_weather_show(int i);

/*
	ͼƬ��ע��
	cloudy�Ƕ���ͼ�ꣻwifi����ɫ��WIFIͼ�ꣻyunduan�Ǳ���ͼ��shangxiafanjiantou���¶ȷ�Χͼ�ꣻ
	location�Ƕ�λͼ�ꣻfont1�ǰ���ASCII������ú��ֵ��ֿ��ֿ���ʽ�ο����ǹȸ�ĳ������壻
*/
//��ʱ�ַ�������
char text_setting_tmpbuff[50];


//������ͼƬ
//����ͼƬ

LV_IMG_DECLARE(wifi);
LV_IMG_DECLARE(location);
LV_IMG_DECLARE(yunduan);
LV_IMG_DECLARE(shangxiafanjiantou);
LV_IMG_DECLARE(wenduji);
LV_IMG_DECLARE(shiduji);
LV_IMG_DECLARE(guangzhaoqiangdu);
LV_IMG_DECLARE(kongqizhiliang);
LV_IMG_DECLARE(dianchi);
LV_IMG_DECLARE(interface3img);
LV_IMG_DECLARE(zuojiantou);


//���ȶ���������ⲿ����
LV_FONT_DECLARE(font1);
LV_FONT_DECLARE(font3);				
lv_obj_t *img1=NULL,*img2=NULL,*img3=NULL,*img4=NULL,*img_weather=NULL;
lv_obj_t *controot1=NULL;
lv_style_t title0_style,contstyle1;



/*
	��ʾͼƬ����������������ʾͼƬ��ʱ��ע��
	ͼƬ�ĸ��ǹ����Ǻ���ֵĸ���֮ǰ���ֵģ�
	���Ա���ͼҪ�����ã�������Сͼ����ı�
	��bsp_lcd.cʹ��Display_Dir������Ļ����
	***************************************
	���������������picture������
	***************************************
	ע�����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	----------------------------------------------------------------------------
	1���ڵδ�ʱ����Ҫ��lv�������һ����������������Ҳ�ᵼ��ɾ��������ʧ�ܡ�����
		Ƶ����1msһ�Σ�����lv_task������ִ��Ƶ�ʵĵ�λ����1ms��������
	2��ɾ������֮��Ҫ�ٶԱ�ǩ�����ڵı�ǩ������ʾ����������Ҳ�ᵼ��ɾ������ʧ�ܡ�
	3��û�д�����ǩ�ͶԱ�ǩ���ݽ���д��ᵼ��ҳ����ʾ�쳣������������handler�޷�
		ִ�У��ᵼ������ҳ�涼�޷���ʾ��
	4������lv_obj_t *������ʹ�ã�����һ�������������˱���ָ�����࣬����֮��Ϊ�ñ���
		�������������ԡ��������ǵ�img�ؼ���label��ǩ����ʹ�øñ��������ġ���label��
		ʾ����Ϊ��������֮��text1����֮������������ʽ�������ı����뷽ʽ��ʹ��
		lv_label_set_text��ʾ�ı�����������ʾ��һ���ı���ʱ�����������ʹ����һ����
		����lv_obj_t *text1������������ʽ���ͻᵼ�µ�һ����ʾ���ı���ʧ������Ĳ���
		�����ǣ��ٴζԵ�һ�δ�����label������ʽ���ã��ٴ�ʹ��set_text��ʾ�ı�����ô
		��һ�������ͱ������ǡ����ˣ���ֻ�еڶ���������Ч�ˣ�����λ���ϵĸ��ǣ��������
		�����ǲ��ٶԸ��ı������޸ģ��ǾͿ����ٴ�ʹ��text1����һ���µ�create���������
		�����text1��һ���µı�����������֮ǰ���ı���ʾ������ˣ���һ���ı����ú���ʾ
		�Ѿ���ɣ���ʼ��һ���µ��ı����á�img�ؼ�������ʹ�������ͬ��ֱ��ʹ�ô�����img
		�����ᵼ��ֻ�к������õ�ͼƬ������Ч��
	5����lv_style_t��ʽ�������õ�ʱ��Ҳ�Ǻ����õĻḲ�������õġ�
	6��ʵ�ֹ�����ʱ�������ù���ģʽ�����ñ�ǩ��С��������ܻ�ʧЧ��
	7��lv_label_set_text���������ı���ʱ��������ַ�������ֱ�ӷŵ�ָ������λ�ü��ɣ�
		����ǡ������ֶ�������ı�����Ҫ����UTF-8�����ʽ���ı������Ե����������ı����ʽ
		ΪUTF-8�������룬����ʹ��ת�����ߵõ�UTF-8���͵ı���д�롣
	8�����һ��lv_obj_t *���͵Ŀؼ�û��create��ȥ������ʽ������Ч�ᵼ��������һ������
		�ᵼ��������ʾ�������⣬��������ʾҳ�治����ʾ���������е�handle�޷���ʱ����
	----------------------------------------------------------------------------
*/
//ҳ��1��ʾͼƬ������������1****************************************************************************************************************
void lv_interface1_show_picture(void)
{
	//����������ʽ
	lv_style_copy(&contstyle1,&lv_style_plain_color);
	contstyle1.body.main_color = LV_COLOR_WHITE;
	contstyle1.body.grad_color = LV_COLOR_WHITE;					//������������ɫ
	
	
	controot1 = lv_cont_create(lv_scr_act(),NULL);				//��������
	lv_obj_set_style(controot1,&contstyle1);							//����������ʽ
	lv_obj_set_size(controot1,320,240);										//���������Ĵ�С
	lv_obj_align(controot1,NULL,LV_ALIGN_CENTER,0,0);			//��Ļ���ж���
	lv_cont_set_layout(controot1,LV_LAYOUT_OFF);					//�����޲��ַ�ʽ
	
	img2 = lv_img_create(controot1,NULL);							
	lv_img_set_src(img2,&yunduan);
	lv_obj_align(img2,NULL,LV_ALIGN_CENTER,0,0);					//��ʾ����ͼ
	
	
	img2 = lv_img_create(controot1,NULL);										
	lv_img_set_src(img2,&wifi);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_RIGHT,0,0);		//��ʾWIFIͼ��
//	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&location);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_LEFT,0,16);		//��ʾ��λͼ��
	
	img_weather = lv_img_create(controot1,NULL);
	lv_obj_align(img_weather,NULL,LV_ALIGN_IN_TOP_RIGHT,-16,16);		//��ʾ����ͼ��
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&shangxiafanjiantou);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_MID,50,40);				//��ʾ˫���ͷͼ��
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&wenduji);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,-32);				//��ʾ�¶ȼ�ͼ��
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&shiduji);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,0);				//��ʾʪ�ȼ�ͼ��
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&guangzhaoqiangdu);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,32);				//��ʾ����ǿ��ͼ��
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&kongqizhiliang);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,64);				//��ʾ��������ͼ��
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&dianchi);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-32,0);				//��ʾ���ͼ��
	
}


lv_obj_t *label_time = NULL;
lv_obj_t *label_date = NULL,*label_week;
lv_style_t date_style,date_style2;
//lv_obj_t *label_test = NULL;
//��ʾʱ���ǩ����****************************************************************************************************************
void lv_interface1_show_time(void)
{
	//������ʽ
	lv_style_copy(&date_style,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	date_style.text.font = &font1;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	date_style.text.color = LV_COLOR_BLACK;
	
	
	lv_style_copy(&date_style2,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	date_style2.text.font = &font3;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	date_style2.text.color = LV_COLOR_RED;
    /*�ڵ�ǰ�����Ļ�ϴ���һ��Label*/
	//��ʾʱ����
	label_time = lv_label_create(controot1, NULL);
	lv_label_set_style(label_time,LV_LABEL_STYLE_MAIN,&date_style2);
	lv_obj_align(label_time, NULL, LV_ALIGN_IN_TOP_LEFT, 35, 20);
	
	//��ʾ������
	label_date = lv_label_create(controot1,NULL);
	lv_label_set_style(label_date,LV_LABEL_STYLE_MAIN,&date_style);
	lv_obj_align(label_date, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
		//��ʾ�ܼ�
	label_week = lv_label_create(controot1,NULL);
	lv_label_set_style(label_week,LV_LABEL_STYLE_MAIN,&date_style);
	lv_obj_align(label_week, label_time, LV_ALIGN_OUT_BOTTOM_MID, 40, 0 );
	
	lv_task_ready(Task3_Handle);
		//label_test = lv_label_create(controot1,NULL);
	/* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment
		 *	����ǩ���뵽����
		 * NULL��ζ�Ŷ��븸ĸ(�������ڵ���Ļ)
		 * 0,0��ĩβ��ʾ�ڶ�����x, yƫ����
	*/
		//lv_obj_align(label_test, NULL, LV_ALIGN_IN_TOP_MID, 128, 0);
		
/*
	Modify the Label's text���޸�Label���ı��������������пɼ���ʹ�øú���
	�޸��ı���������ı���ǰ���ֿ����ɵ��ı���ʾ��ʽ�ǲ�һ���ģ������ı���
	���е����ľ��εı�����������Ļ�ϣ�ֻ��Ҫ��ʾ�����ֳ��֡�
*/
}


lv_obj_t *label_weather1_hight = NULL,*label_weather1_lowt=NULL,*label_weather2 = NULL,*label_weather3 = NULL;
lv_obj_t *label_weather_text=NULL;		//�����ı���ʾ��ǩ
//ҳ��1��ʾ�����ı�****************************************************************************************************************
void lv_interface1_show_weather(void)
{
    label_weather1_hight =  lv_label_create(controot1, NULL);						//�������±�ǩ
		lv_label_set_style(label_weather1_hight,LV_LABEL_STYLE_MAIN,&date_style);
		lv_obj_align(label_weather1_hight, NULL, LV_ALIGN_IN_TOP_MID,50,26);						
	
	  label_weather1_lowt =  lv_label_create(controot1, NULL);						//�������±�ǩ
		lv_label_set_style(label_weather1_lowt,LV_LABEL_STYLE_MAIN,&date_style);
		lv_obj_align(label_weather1_lowt, NULL, LV_ALIGN_IN_TOP_MID,50,66);

		label_weather_text =  lv_label_create(controot1, NULL);						//�����ı���ʾ��ǩ
		lv_label_set_style(label_weather_text,LV_LABEL_STYLE_MAIN,&date_style);
		lv_obj_align(label_weather_text, img_weather, LV_ALIGN_OUT_BOTTOM_MID,0,10);			//��������ͼ��λ�������ı�
		
	
//		label_weather2 = lv_label_create(controot1,NULL);
//    lv_obj_align(label_weather2, NULL, LV_ALIGN_IN_LEFT_MID, 0, 16);
//	
//		label_weather3 = lv_label_create(controot1,NULL);
//    lv_obj_align(label_weather3, NULL, LV_ALIGN_IN_LEFT_MID, 0, 32);
}




lv_obj_t *label_text1=NULL,*label_text2=NULL,*label_nowtemper=NULL;
lv_style_t city_style,text_style1;

/*
	ҳ��1���ı���ʾ****************************************************************************************************************
*/
void lv_interface1_show_text(void)
{
	//������ʽ
	lv_style_copy(&city_style,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	city_style.text.font = &font1;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	city_style.text.color = LV_COLOR_RED; 
 
	
	//�����ı�
	label_text1 = lv_label_create(controot1, NULL);	
	lv_label_set_style(label_text1,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_text1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 45); 
	lv_label_set_text(label_text1, "郑州");							//��ʾ����֣��
	//lv_label_set_text(title0,"\xE8\xAF\xAD\xE9\x9F\xB3\xE6\xB0\x94\xE8\xB1\xA1\xE7\xAB\x99");
	
	//������ʽ
	lv_style_copy(&text_style1,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	text_style1.text.font = &font3;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	text_style1.text.color = LV_COLOR_RED;
	
	//�����ı�
//	label_text2 = lv_label_create(controot1, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&text_style1); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_CENTER, -48, -17); 
//	lv_label_set_text(label_text2, "1、主页面 ");			//����ҳ�桱����
//	
//	label_text2 = lv_label_create(controot1, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&text_style1); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_CENTER, -48,0); 
//	lv_label_set_text(label_text2, "2、三天天气 ");			//����������������
//	
//	label_text2 = lv_label_create(controot1, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&text_style1); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_CENTER, -48,17); 
//	lv_label_set_text(label_text2, "3、室内环境 ");			//�����ڻ���������
	
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%s℃ ",Now_temperature);
	
	label_nowtemper = lv_label_create(controot1, NULL); 
	lv_label_set_style(label_nowtemper,LV_LABEL_STYLE_MAIN,&text_style1); 
	lv_obj_align(label_nowtemper, NULL, LV_ALIGN_IN_TOP_MID,80,100); 		
	lv_label_set_text(label_nowtemper, tmpbuff);								//��ǰ�¶� 

}

lv_obj_t *label_HUM=NULL,*label_TEM=NULL;
//ҳ��1��ʾ��ʪ�Ⱥ���*************************************************************************************
void lv_show_dht11(void)
{
//	lv_style_copy(&city_style,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
//	city_style.text.font = &font1;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
//	city_style.text.color = LV_COLOR_BLACK; 
	 
	label_TEM = lv_label_create(controot1, NULL);
	lv_label_set_style(label_TEM,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_TEM, NULL, LV_ALIGN_IN_LEFT_MID, 37, -35);
	
	//�����ı�
	label_HUM = lv_label_create(controot1, NULL);
	lv_label_set_style(label_HUM,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_HUM, NULL, LV_ALIGN_IN_LEFT_MID, 37, -3); 
	
	
	//���ɱ�ǩ֮���������ֵһ�Σ���ֹ����
		lv_task_ready(Task6_Handle);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%d�?",dht11_data.TMP);
//	lv_label_set_text(label_TEM,text_setting_tmpbuff);					//��ʾ�¶�

//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%d%%",dht11_data.HUM);
//	lv_label_set_text(label_HUM,text_setting_tmpbuff);					//��ʾʪ��				
}


lv_obj_t *label_light=NULL,*label_ppm=NULL;
lv_style_t LP_style;
//��ʾ����ǿ�ȺͿ�������
void lv_show_LP(void)
{
		//������ʽ
	lv_style_copy(&LP_style,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	LP_style.text.font = &font1;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	LP_style.text.color = LV_COLOR_RED;
	
	label_light = lv_label_create(controot1, NULL);
	lv_label_set_style(label_light,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_light, NULL, LV_ALIGN_IN_LEFT_MID, 37,29);
	
	//�����ı�
	label_ppm = lv_label_create(controot1, NULL);
	lv_label_set_style(label_ppm,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_ppm, NULL, LV_ALIGN_IN_LEFT_MID, 37,61); 
	

	lv_task_ready(Task4_Handle);
}	




LV_IMG_DECLARE(xiaotu1);			//��������ͼƬ
LV_IMG_DECLARE(xiaotu2);
LV_IMG_DECLARE(xiaotu3);



lv_style_t contstyle2;				
lv_obj_t *controot2=NULL,*interface2_img1=NULL,*interface2_img2=NULL,*interface2_img3=NULL;
lv_obj_t *img2_weather_today=NULL,*img2_weather_tommorrow=NULL,*img2_weather_aftertomm=NULL;
/*
	��ҳ��2ʵ�ֵĹ����У�ҳ�汻�������ݣ�����ֱ���������Ŵ�Сһ�µ�ͼƬ
	ÿ��ͼƬ����һ������������������ͼƬ��obj���͵ı�������ע�⣺����ͼƬ�������Ϊһ��������
*/
//ҳ��2�ĸ�����������ͼƬ��ʾ*******************************************************************************************************************������������������������������������������������������������������������
void lv_interface2_show_picture(void)
{
	//����������ʽ
	lv_style_copy(&contstyle2,&lv_style_plain_color);
	contstyle2.body.main_color = LV_COLOR_WHITE;
	contstyle2.body.grad_color = LV_COLOR_WHITE;					//������������ɫ
	
	controot2 = lv_cont_create(lv_scr_act(),NULL);				//��������
	lv_obj_set_style(controot2,&contstyle2);							//����������ʽ	
	lv_obj_set_size(controot2,320,240);										//���������Ĵ�С
	lv_obj_align(controot2,NULL,LV_ALIGN_CENTER,0,0);			//��Ļ���ж���
	lv_cont_set_layout(controot2,LV_LAYOUT_OFF);					//�����޲��ַ�ʽ
	
	interface2_img1 = lv_img_create(controot2,NULL);							
	lv_img_set_src(interface2_img1,&xiaotu1);
	lv_obj_align(interface2_img1,NULL,LV_ALIGN_IN_BOTTOM_LEFT,0,0);					//��ʾ����ͼ
	
	interface2_img2 = lv_img_create(controot2,NULL);										
	lv_img_set_src(interface2_img2,&xiaotu2);
	lv_obj_align(interface2_img2,NULL,LV_ALIGN_IN_LEFT_MID,107,0);

	interface2_img3 = lv_img_create(controot2,NULL);
	lv_img_set_src(interface2_img3,&xiaotu3);	
	lv_obj_align(interface2_img3,NULL,LV_ALIGN_IN_LEFT_MID,212,0);
	
	img2_weather_today = lv_img_create(controot2,NULL);	
//	lv_img_set_src(img2_weather_today,&qing);
	lv_obj_align(img2_weather_today,NULL,LV_ALIGN_IN_TOP_LEFT,30,95);
	
	img2_weather_tommorrow = lv_img_create(interface2_img2,NULL);	
	lv_obj_align(img2_weather_tommorrow,NULL,LV_ALIGN_IN_TOP_MID,-10,95);
//		lv_img_set_src(img2_weather_tommorrow,&qing);

	
	img2_weather_aftertomm = lv_img_create(interface2_img3,NULL);	
	lv_obj_align(img2_weather_aftertomm,NULL,LV_ALIGN_IN_TOP_MID,-10,95);
//		lv_img_set_src(img2_weather_aftertomm,&qing);

	
	
}

//lv_obj_t *label_weather2_hight=NULL;
//void lv_interface2_show_weather(void)
//{
//		label_weather2_hight =  lv_label_create(controot1, NULL);						//�������±�ǩ
//		lv_label_set_style(label_weather2_hight,LV_LABEL_STYLE_MAIN,&date_style);
//		lv_obj_align(label_weather2_hight, NULL, LV_ALIGN_IN_TOP_MID,50,26);						
//	
////	  label_weather1_lowt =  lv_label_create(controot1, NULL);						//�������±�ǩ
////		lv_label_set_style(label_weather1_lowt,LV_LABEL_STYLE_MAIN,&date_style);
////		lv_obj_align(label_weather1_lowt, NULL, LV_ALIGN_IN_TOP_MID,50,66);
//}

lv_style_t three_day_Words;
//ҳ��2��ʾ�ı�����****************************************************************************************
void lv_interface2_show_text()
{
	//����������ʽ
	lv_style_copy(&three_day_Words,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	three_day_Words.text.font = &font1;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	three_day_Words.text.color = LV_COLOR_GREEN;
	
	//�����ı�
	label_text2 = lv_label_create(lv_scr_act(), NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 30); 
	lv_label_set_text(label_text2, "今天 ");			//�����족����
	
	//�����¶��ı�
	label_text2 = lv_label_create(lv_scr_act(), NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 50); 
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[0].acity_weather[0].lowt,city5_weather[0].acity_weather[0].hight);
	lv_label_set_text(label_text2, text_setting_tmpbuff);			//�����¶�
	
	//���������ı�
	label_text2 = lv_label_create(lv_scr_act(), NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 70); 
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[0].code),1);
	lv_label_set_text(label_text2, CNWord);			//��������
	
	//�����ı�
	label_text2 = lv_label_create(interface2_img2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID,0,30); 
	lv_label_set_text(label_text2, "明天 ");			//�����족����
	
	//�����¶��ı�
	label_text2 = lv_label_create(interface2_img2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 50); 
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[0].acity_weather[1].lowt,city5_weather[0].acity_weather[1].hight);
	lv_label_set_text(label_text2,text_setting_tmpbuff );			//�����¶�
	
	//���������ı�
	label_text2 = lv_label_create(interface2_img2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 70); 
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[1].code),2);
	lv_label_set_text(label_text2, CNWord);			//��������
	
	
	//�����ı�
	label_text2 = lv_label_create(interface2_img3, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID,0, 30); 
	lv_label_set_text(label_text2, "后天 ");			//�����족����
	
	//�����¶��ı�
	label_text2 = lv_label_create(interface2_img3, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 50); 
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[0].acity_weather[2].lowt,city5_weather[0].acity_weather[2].hight);
	lv_label_set_text(label_text2, text_setting_tmpbuff);			//�����¶�

	//���������ı�
	label_text2 = lv_label_create(interface2_img3, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 70); 
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[2].code),3);
	lv_label_set_text(label_text2, CNWord);			//��������
	
	//����ҳ��2����ʾ�ı�
	label_text2 = lv_label_create(controot2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, -64,5); 
	lv_label_set_text(label_text2, "最近三天天气展示 ");			//�������������չʾ���ı���ʾ
	
//		//�����ı���ʾ������
//	label_text2 = lv_label_create(interface2_img3, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID,0, 20); 
	
}


lv_obj_t *img3_weather_today=NULL,*img3_weather_tommorrow=NULL,*img3_weather_aftertomm=NULL;
lv_obj_t *controot3=NULL,*interface3_img1=NULL,*interface3_img2=NULL;
//ҳ��3������������ͼƬ��ʾ****************************************************************************************************������������������������������������������������������������������������������������������������������������
void lv_interface3_show_picture(void)
{
	controot3 = lv_cont_create(lv_scr_act(),NULL);				//��������

	lv_obj_set_size(controot3,320,240);										//���������Ĵ�С
	lv_obj_align(controot3,NULL,LV_ALIGN_CENTER,0,0);			//��Ļ���ж���
	lv_cont_set_layout(controot3,LV_LAYOUT_OFF);					//�����޲��ַ�ʽ
	
	interface3_img1 = lv_img_create(controot3,NULL);										
	lv_img_set_src(interface3_img1,&yunduan);
	lv_obj_align(interface3_img1,NULL,LV_ALIGN_IN_TOP_LEFT,0,0);
	
	interface3_img2 = lv_img_create(controot3,NULL);										
	lv_img_set_src(interface3_img2,&zuojiantou);
	lv_obj_align(interface3_img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-20,40);
	
	
	img3_weather_today = lv_img_create(controot3,NULL);	
//	lv_img_set_src(img3_weather_today,&qing);
	lv_obj_align(img3_weather_today,NULL,LV_ALIGN_IN_BOTTOM_LEFT,20,-30);
	
	img3_weather_tommorrow = lv_img_create(controot3,NULL);	
	lv_obj_align(img3_weather_tommorrow,img3_weather_today,LV_ALIGN_OUT_RIGHT_MID,25,0);
//		lv_img_set_src(img3_weather_tommorrow,&qing);

	
	img3_weather_aftertomm = lv_img_create(controot3,NULL);	
	lv_obj_align(img3_weather_aftertomm,img3_weather_tommorrow,LV_ALIGN_OUT_RIGHT_MID,25,0);
//		lv_img_set_src(img3_weather_aftertomm,&qing);
}



lv_style_t city4_style;
lv_obj_t *label3_text1=NULL,*label3_text2=NULL,*label3_text3=NULL;
lv_obj_t *label3_today=NULL,*label3_tommorrow=NULL,*label3_aftertomm=NULL;
lv_obj_t *label3_todayCN=NULL,*label3_tommorrowCN=NULL,*label3_aftertommCN=NULL;
void lv_interface3_show_text(void)
{
	//����������ʽ
	lv_style_copy(&city4_style,&lv_style_plain_color);			//�������ʽ����ֵ ��һ������д 
	city4_style.text.font = &font1;												//����ʽ��ʹ���������,font1�Ǻ����ֿ�
	city4_style.text.color = LV_COLOR_RED;
	
	//�����ı�
	label3_text1 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text1,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text1, NULL, LV_ALIGN_IN_TOP_RIGHT, -40, 40); 
	lv_label_set_text(label3_text1, "北京 ");			//������������
	
	//�����ı�
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, label3_text1, LV_ALIGN_IN_BOTTOM_MID, 0, 40); 
	lv_label_set_text(label3_text2, "上海 ");			//���Ϻ�������
	
	//�����ı�
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, label3_text1, LV_ALIGN_IN_BOTTOM_MID, 0, 80); 
	lv_label_set_text(label3_text2, "广州 ");			//�����ݡ�����
	
	//�����ı�
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, label3_text1, LV_ALIGN_IN_BOTTOM_MID, 0, 120); 
	lv_label_set_text(label3_text2, "深圳 ");			//�����ڡ�����
	
		//�����ı�
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, NULL, LV_ALIGN_IN_TOP_MID, -80, 0); 
	lv_label_set_text(label3_text2, "全国主要城市天气展示 ");			//��ȫ����Ҫ����չʾ������
	
	//�����ı�
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 50); 
	lv_label_set_text(label3_text2, "今天 ");														//�����족����
	
	//�����ı�
	label3_text3 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text3,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text3, label3_text2, LV_ALIGN_OUT_RIGHT_MID, 40, 0); 
	lv_label_set_text(label3_text3, "明天 ");			//�����족����
	
	//�����ı�
	label3_text3 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text3,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text3, label3_text2, LV_ALIGN_OUT_RIGHT_MID, 100, 0); 
	lv_label_set_text(label3_text3, "后天 ");			//�����족����

	//�����¶��ı�
	label3_today = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_today,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_today, label3_text2, LV_ALIGN_IN_BOTTOM_MID, 0, 30);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[1].acity_weather[0].lowt,city5_weather[1].acity_weather[0].hight);	
//	lv_label_set_text(label3_today, text_setting_tmpbuff);									//�����¶�
	
		//�����¶��ı�
	label3_tommorrow = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_tommorrow,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_tommorrow, label3_today, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[1].acity_weather[1].lowt,city5_weather[1].acity_weather[1].hight);	
//	lv_label_set_text(label3_tommorrow, text_setting_tmpbuff);									//�����¶�
	
		//�����¶��ı�
	label3_aftertomm = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_aftertomm,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_aftertomm, label3_today, LV_ALIGN_OUT_RIGHT_MID, 100, 0);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[1].acity_weather[2].lowt,city5_weather[1].acity_weather[2].hight);		
//	lv_label_set_text(label3_aftertomm, text_setting_tmpbuff);									//�����¶�
	
	//�������������ı�
	label3_todayCN = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_todayCN,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_todayCN, label3_today, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
//	Weathercode_to_CNWord(atoi(city5_weather[1].acity_weather[0].code));
//	lv_label_set_text(label3_todayCN, CNWord);																//���������ı�
	
		//�������������ı�
	label3_tommorrowCN = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_tommorrowCN,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_tommorrowCN, label3_todayCN, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
//	Weathercode_to_CNWord(atoi(city5_weather[1].acity_weather[1].code));
//	lv_label_set_text(label3_tommorrowCN, CNWord);														//���������ı�

	//�������������ı�
	label3_aftertommCN = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_aftertommCN,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_aftertommCN, label3_todayCN, LV_ALIGN_OUT_RIGHT_MID, 100, 0);
//	Weathercode_to_CNWord(atoi(city5_weather[1].acity_weather[2].code));
//	lv_label_set_text(label3_aftertommCN, CNWord);														//���������ı�
	
	//�л�����ҳ���ʱ��Ĭ����ʾ�����������������ֶ��л�
	day1_weather_show(1);
	day2_weather_show(1);
	day3_weather_show(1);


	
}

/*===���в������ձ�===
			iֵ			����
			1				����
			2				�Ϻ�
			3				����
			4				����
*/
//չʾ��i�����е�һ����¶Ⱥ����������ı�
void day1_weather_show(int i)
{
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[i].acity_weather[0].lowt,city5_weather[i].acity_weather[0].hight);	
	lv_label_set_text(label3_today, text_setting_tmpbuff);
	Weathercode_to_CNWord(atoi(city5_weather[i].acity_weather[0].code),1);
	lv_label_set_text(label3_todayCN, CNWord);								//��������
}


//չʾ��i�����еڶ�����¶Ⱥ����������ı�
void day2_weather_show(int i)
{
		memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[i].acity_weather[1].lowt,city5_weather[i].acity_weather[1].hight);	
	lv_label_set_text(label3_tommorrow, text_setting_tmpbuff);
	Weathercode_to_CNWord(atoi(city5_weather[i].acity_weather[1].code),2);
	lv_label_set_text(label3_tommorrowCN, CNWord);														//���������ı�
}


//չʾ��i�����е�������¶Ⱥ����������ı�
void day3_weather_show(int i)
{
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%s℃ ",city5_weather[i].acity_weather[2].lowt,city5_weather[i].acity_weather[2].hight);		
	lv_label_set_text(label3_aftertomm, text_setting_tmpbuff);
	Weathercode_to_CNWord(atoi(city5_weather[i].acity_weather[2].code),3);
	lv_label_set_text(label3_aftertommCN, CNWord);														//���������ı�	

}

void lv_interface4_show_picture(void)
{
	controot3 = lv_cont_create(lv_scr_act(),NULL);				//��������

	lv_obj_set_size(controot3,320,240);										//���������Ĵ�С
	lv_obj_align(controot3,NULL,LV_ALIGN_CENTER,0,0);			//��Ļ���ж���
	lv_cont_set_layout(controot3,LV_LAYOUT_OFF);					//�����޲��ַ�ʽ
	
	interface3_img1 = lv_img_create(controot3,NULL);										
	lv_img_set_src(interface3_img1,&yunduan);
	lv_obj_align(interface3_img1,NULL,LV_ALIGN_IN_TOP_LEFT,0,0);

}




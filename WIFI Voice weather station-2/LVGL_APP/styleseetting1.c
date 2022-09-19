#include "stylesetting1.h"
#include "lv_tutorial_hello_world.h"
#include "stdio.h"
#include "lcdshowtask.h"
#include "ESP8266.h"


LV_IMG_DECLARE(qing);					//Çç

void day1_weather_show(int i);
void day2_weather_show(int i);
void day3_weather_show(int i);

/*
	Í¼Æ¬±¸×¢£º
	cloudyÊÇ¶àÔÆÍ¼±ê£»wifiÊÇÀ¶É«µÄWIFIÍ¼±ê£»yunduanÊÇ±³¾°Í¼£»shangxiafanjiantouÊÇÎÂ¶È·¶Î§Í¼±ê£»
	locationÊÇ¶¨Î»Í¼±ê£»font1ÊÇ°üº¬ASCIIÂëºÍËùÓÃºº×ÖµÄ×Ö¿â×Ö¿âÑùÊ½²Î¿¼µÄÊÇ¹È¸èµÄ³£¹æ×ÖÌå£»
*/
//ÁÙÊ±×Ö·û´®±äÁ¿
char text_setting_tmpbuff[50];


//ÏÈÉêÃ÷Í¼Æ¬
//ÌìÆøÍ¼Æ¬

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


//Ê×ÏÈ¶Ô×ÖÌå½øĞĞÍâ²¿ÉùÃ÷
LV_FONT_DECLARE(font1);
LV_FONT_DECLARE(font3);				
lv_obj_t *img1=NULL,*img2=NULL,*img3=NULL,*img4=NULL,*img_weather=NULL;
lv_obj_t *controot1=NULL;
lv_style_t title0_style,contstyle1;



/*
	ÏÔÊ¾Í¼Æ¬º¯Êı£¬ÔÚÈİÆ÷ÖĞÏÔÊ¾Í¼Æ¬µÄÊ±ºò×¢Òâ
	Í¼Æ¬µÄ¸²¸Ç¹æÔòÊÇºó³öÏÖµÄ¸²¸ÇÖ®Ç°³öÏÖµÄ£¬
	ËùÒÔ±³¾°Í¼ÒªÏÈÉèÖÃ£¬ÔÚÉèÖÃĞ¡Í¼±êºÍÎÄ±¾
	ÔÙbsp_lcd.cÊ¹ÓÃDisplay_DirÉèÖÃÆÁÄ»·½Ïò
	***************************************
	ÕâÀïµÄÈİÆ÷´´½¨ÔÚpictureº¯ÊıÖĞ
	***************************************
	×¢ÒâÊÂÏî£º!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	----------------------------------------------------------------------------
	1¡¢ÔÚµÎ´ğ¶¨Ê±Æ÷ÖĞÒª¸ølvÈÎÎñ¹ÜÀíÒ»¸öĞÄÌø£¬²»¸øĞÄÌøÒ²»áµ¼ÖÂÉ¾³ıÈÎÈİÆ÷Ê§°Ü¡£ĞÄÌø
		ÆµÂÊÊÇ1msÒ»´Î£¬ËùÒÔlv_taskÖĞÈÎÎñÖ´ĞĞÆµÂÊµÄµ¥Î»¾ÍÊÇ1msµÄÕûÊı±¶
	2¡¢É¾³ıÈİÆ÷Ö®ºó²»ÒªÔÙ¶Ô±êÇ©ÈİÆ÷ÄÚµÄ±êÇ©½øĞĞÏÔÊ¾²Ù×÷£¬·ñÔòÒ²»áµ¼ÖÂÉ¾³ıÈİÆ÷Ê§°Ü¡£
	3¡¢Ã»ÓĞ´´½¨±êÇ©¾Í¶Ô±êÇ©ÄÚÈİ½øĞĞĞ´Èë»áµ¼ÖÂÒ³ÃæÏÔÊ¾Òì³££¬×èÈû³ÌĞòµ¼ÖÂhandlerÎŞ·¨
		Ö´ĞĞ£¬»áµ¼ÖÂÕû¸öÒ³Ãæ¶¼ÎŞ·¨ÏÔÊ¾¡£
	4¡¢¹ØÓÚlv_obj_t *±äÁ¿µÄÊ¹ÓÃ£¬¶¨ÒåÒ»¸ö±äÁ¿£¬´´½¨´Ë±äÁ¿Ö¸¶¨¸¸Àà£¬´´½¨Ö®ºóÎª¸Ã±äÁ¿
		ÉèÖÃÆäËûµÄÊôĞÔ¡£ÕâÀïÎÒÃÇµÄimg¿Ø¼şºÍlabel±êÇ©¶¼ÊÇÊ¹ÓÃ¸Ã±äÁ¿´´½¨µÄ¡£ÒÔlabelÏÔ
		Ê¾×ÖÌåÎªÀı£¬´´½¨Ö®ºótext1±äÁ¿Ö®ºó£¬ÉèÖÃ×ÖÌåÑùÊ½£¬ÉèÖÃÎÄ±¾¶ÔÆë·½Ê½£¬Ê¹ÓÃ
		lv_label_set_textÏÔÊ¾ÎÄ±¾¡£µ±ÎÒÃÇÏÔÊ¾ÏÂÒ»¸öÎÄ±¾µÄÊ±ºòÈç¹û½ö½ö»¹Ê¹ÓÃÉÏÒ»¸öÎÄ
		±¾µÄlv_obj_t *text1±äÁ¿ÔÙÉèÖÃÑùÊ½£¬¾Í»áµ¼ÖÂµÚÒ»¸öÏÔÊ¾µÄÎÄ±¾ÏûÊ§¡£ÕâÀïµÄ²Ù×÷
		±¾ÖÊÊÇ£¬ÔÙ´Î¶ÔµÚÒ»´Î´´½¨µÄlabel½øĞĞÑùÊ½ÉèÖÃ£¬ÔÙ´ÎÊ¹ÓÃset_textÏÔÊ¾ÎÄ±¾£¬ÄÇÃ´
		µÚÒ»¸ö²Ù×÷¾Í±»¡°¸²¸Ç¡±µôÁË£¬¼´Ö»ÓĞµÚ¶ş¸ö²Ù×÷ÉúĞ§ÁË£¨²»ÊÇÎ»ÖÃÉÏµÄ¸²¸Ç£©¡£Èç¹ûÒÔ
		ºóÎÒÃÇ²»ÔÙ¶Ô¸ÃÎÄ±¾½øĞĞĞŞ¸Ä£¬ÄÇ¾Í¿ÉÒÔÔÙ´ÎÊ¹ÓÃtext1½ÓÊÕÒ»¸öĞÂµÄcreate´´½¨½á¹û£¬
		ÕâÀïµÄtext1ÊÇÒ»¸öĞÂµÄ±äÁ¿£¬²»ÔÙÓëÖ®Ç°µÄÎÄ±¾ÏÔÊ¾Ïà¹ØÁªÁË£¬ÉÏÒ»¸öÎÄ±¾ÉèÖÃºÍÏÔÊ¾
		ÒÑ¾­Íê³É£¬¿ªÊ¼ÁËÒ»¸öĞÂµÄÎÄ±¾ÉèÖÃ¡£img¿Ø¼ş±äÁ¿µÄÊ¹ÓÃÓë´ËÏàÍ¬£¬Ö±½ÓÊ¹ÓÃ´´½¨µÄimg
		±äÁ¿»áµ¼ÖÂÖ»ÓĞºóÃæÉèÖÃµÄÍ¼Æ¬²Ù×÷ÓĞĞ§¡£
	5¡¢¶Ôlv_style_tÑùÊ½½øĞĞÉèÖÃµÄÊ±ºò£¬Ò²ÊÇºóÉèÖÃµÄ»á¸²¸ÇÏÈÉèÖÃµÄ¡£
	6¡¢ÊµÏÖ¹ö¶¯µÄÊ±ºò¡£ÏÈÉèÖÃ¹ö¶¯Ä£Ê½ÔÙÉèÖÃ±êÇ©´óĞ¡£¬·ñÔò¿ÉÄÜ»áÊ§Ğ§¡£
	7¡¢lv_label_set_textº¯ÊıÉèÖÃÎÄ±¾µÄÊ±ºò£¬Èç¹ûÊÇ×Ö·û´®±äÁ¿Ö±½Ó·Åµ½Ö¸¶¨²ÎÊıÎ»ÖÃ¼´¿É£¬
		Èç¹ûÊÇ¡°¡±ÄÚÊÖ¶¯ÊäÈëµÄÎÄ±¾×ÓĞèÒªÊäÈëUTF-8±àÂë¸ñÊ½µÄÎÄ±¾¡£¿ÉÒÔµ÷Õû±àÒëÆ÷µÄ±àÂë¸ñÊ½
		ÎªUTF-8½øĞĞÊäÈë£¬»òÕßÊ¹ÓÃ×ª»»¹¤¾ßµÃµ½UTF-8ÀàĞÍµÄ±àÂëĞ´Èë¡£
	8¡¢Èç¹ûÒ»¸ölv_obj_t *ÀàĞÍµÄ¿Ø¼şÃ»ÓĞcreate¾ÍÈ¥ÉèÖÃÑùÊ½²¢ÇÒÉúĞ§»áµ¼ÖÂ×èÈû£¬Ò»µ©×èÈû
		»áµ¼ÖÂÕû¸öÏÔÊ¾³öÏÖÎÊÌâ£¬¼´Õû¸öÏÔÊ¾Ò³Ãæ²»ÔÙÏÔÊ¾¡£Ö÷º¯ÊıÖĞµÄhandleÎŞ·¨¼°Ê±ÔËĞĞ
	----------------------------------------------------------------------------
*/
//Ò³Ãæ1ÏÔÊ¾Í¼Æ¬£¬´´½¨¸ùÈİÆ÷1****************************************************************************************************************
void lv_interface1_show_picture(void)
{
	//ÉèÖÃÈİÆ÷ÑùÊ½
	lv_style_copy(&contstyle1,&lv_style_plain_color);
	contstyle1.body.main_color = LV_COLOR_WHITE;
	contstyle1.body.grad_color = LV_COLOR_WHITE;					//ÉèÖÃÈİÆ÷µÄÑÕÉ«
	
	
	controot1 = lv_cont_create(lv_scr_act(),NULL);				//´´½¨ÈİÆ÷
	lv_obj_set_style(controot1,&contstyle1);							//ÉèÖÃÈİÆ÷ÑùÊ½
	lv_obj_set_size(controot1,320,240);										//ÉèÖÃÈİÆ÷µÄ´óĞ¡
	lv_obj_align(controot1,NULL,LV_ALIGN_CENTER,0,0);			//ÆÁÄ»¾ÓÖĞ¶ÔÆë
	lv_cont_set_layout(controot1,LV_LAYOUT_OFF);					//ÉèÖÃÎŞ²¼¾Ö·½Ê½
	
	img2 = lv_img_create(controot1,NULL);							
	lv_img_set_src(img2,&yunduan);
	lv_obj_align(img2,NULL,LV_ALIGN_CENTER,0,0);					//ÏÔÊ¾±³¾°Í¼
	
	
	img2 = lv_img_create(controot1,NULL);										
	lv_img_set_src(img2,&wifi);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_RIGHT,0,0);		//ÏÔÊ¾WIFIÍ¼±ê
//	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&location);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_LEFT,0,16);		//ÏÔÊ¾¶¨Î»Í¼±ê
	
	img_weather = lv_img_create(controot1,NULL);
	lv_obj_align(img_weather,NULL,LV_ALIGN_IN_TOP_RIGHT,-16,16);		//ÏÔÊ¾ÌìÆøÍ¼±ê
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&shangxiafanjiantou);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_MID,50,40);				//ÏÔÊ¾Ë«Ïò¼ıÍ·Í¼±ê
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&wenduji);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,-32);				//ÏÔÊ¾ÎÂ¶È¼ÆÍ¼±ê
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&shiduji);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,0);				//ÏÔÊ¾Êª¶È¼ÆÍ¼±ê
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&guangzhaoqiangdu);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,32);				//ÏÔÊ¾¹âÕÕÇ¿¶ÈÍ¼±ê
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&kongqizhiliang);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_LEFT_MID,5,64);				//ÏÔÊ¾¿ÕÆøÖÊÁ¿Í¼±ê
	
	img2 = lv_img_create(controot1,NULL);
	lv_img_set_src(img2,&dianchi);
	lv_obj_align(img2,NULL,LV_ALIGN_IN_TOP_RIGHT,-32,0);				//ÏÔÊ¾µç³ØÍ¼±ê
	
}


lv_obj_t *label_time = NULL;
lv_obj_t *label_date = NULL,*label_week;
lv_style_t date_style,date_style2;
//lv_obj_t *label_test = NULL;
//ÏÔÊ¾Ê±¼ä±êÇ©º¯Êı****************************************************************************************************************
void lv_interface1_show_time(void)
{
	//ÅäÖÃÑùÊ½
	lv_style_copy(&date_style,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	date_style.text.font = &font1;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	date_style.text.color = LV_COLOR_BLACK;
	
	
	lv_style_copy(&date_style2,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	date_style2.text.font = &font3;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	date_style2.text.color = LV_COLOR_RED;
    /*ÔÚµ±Ç°»î¶¯µÄÆÁÄ»ÉÏ´´½¨Ò»¸öLabel*/
	//ÏÔÊ¾Ê±·ÖÃë
	label_time = lv_label_create(controot1, NULL);
	lv_label_set_style(label_time,LV_LABEL_STYLE_MAIN,&date_style2);
	lv_obj_align(label_time, NULL, LV_ALIGN_IN_TOP_LEFT, 35, 20);
	
	//ÏÔÊ¾ÄêÔÂÈÕ
	label_date = lv_label_create(controot1,NULL);
	lv_label_set_style(label_date,LV_LABEL_STYLE_MAIN,&date_style);
	lv_obj_align(label_date, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
		//ÏÔÊ¾ÖÜ¼¸
	label_week = lv_label_create(controot1,NULL);
	lv_label_set_style(label_week,LV_LABEL_STYLE_MAIN,&date_style);
	lv_obj_align(label_week, label_time, LV_ALIGN_OUT_BOTTOM_MID, 40, 0 );
	
	lv_task_ready(Task3_Handle);
		//label_test = lv_label_create(controot1,NULL);
	/* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment
		 *	½«±êÇ©¶ÔÆëµ½ÖĞĞÄ
		 * NULLÒâÎ¶×Å¶ÔÆë¸¸Ä¸(ÕâÊÇÏÖÔÚµÄÆÁÄ»)
		 * 0,0ÔÚÄ©Î²±íÊ¾ÔÚ¶ÔÆëºóµÄx, yÆ«ÒÆÁ¿
	*/
		//lv_obj_align(label_test, NULL, LV_ALIGN_IN_TOP_MID, 128, 0);
		
/*
	Modify the Label's text£¨ĞŞ¸ÄLabelµÄÎÄ±¾£©£¬ÔÚÖ÷º¯ÊıÖĞ¿É¼ÌĞøÊ¹ÓÃ¸Ãº¯Êı
	ĞŞ¸ÄÎÄ±¾£¬ÕâÀïµÄÎÄ±¾ºÍÇ°Ãæ×Ö¿âÉú³ÉµÄÎÄ±¾ÏÔÊ¾·½Ê½ÊÇ²»Ò»ÑùµÄ£¬ÕâÀïÎÄ±¾²»
	»áÓĞµ¥¶ÀµÄ¾ØĞÎµÄ±³¾°³öÏÖÔÚÆÁÄ»ÉÏ£¬Ö»ÓĞÒªÏÔÊ¾µÄÎÄ×Ö³öÏÖ¡£
*/
}


lv_obj_t *label_weather1_hight = NULL,*label_weather1_lowt=NULL,*label_weather2 = NULL,*label_weather3 = NULL;
lv_obj_t *label_weather_text=NULL;		//ÌìÆøÎÄ±¾ÏÔÊ¾±êÇ©
//Ò³Ãæ1ÏÔÊ¾ÌìÆøÎÄ±¾****************************************************************************************************************
void lv_interface1_show_weather(void)
{
    label_weather1_hight =  lv_label_create(controot1, NULL);						//´´½¨¸ßÎÂ±êÇ©
		lv_label_set_style(label_weather1_hight,LV_LABEL_STYLE_MAIN,&date_style);
		lv_obj_align(label_weather1_hight, NULL, LV_ALIGN_IN_TOP_MID,50,26);						
	
	  label_weather1_lowt =  lv_label_create(controot1, NULL);						//´´½¨µÍÎÂ±êÇ©
		lv_label_set_style(label_weather1_lowt,LV_LABEL_STYLE_MAIN,&date_style);
		lv_obj_align(label_weather1_lowt, NULL, LV_ALIGN_IN_TOP_MID,50,66);

		label_weather_text =  lv_label_create(controot1, NULL);						//´´½¨ÎÄ±¾ÏÔÊ¾±êÇ©
		lv_label_set_style(label_weather_text,LV_LABEL_STYLE_MAIN,&date_style);
		lv_obj_align(label_weather_text, img_weather, LV_ALIGN_OUT_BOTTOM_MID,0,10);			//²ÎÕÕÌìÆøÍ¼±êÎ»ÖÃÉèÖÃÎÄ±¾
		
	
//		label_weather2 = lv_label_create(controot1,NULL);
//    lv_obj_align(label_weather2, NULL, LV_ALIGN_IN_LEFT_MID, 0, 16);
//	
//		label_weather3 = lv_label_create(controot1,NULL);
//    lv_obj_align(label_weather3, NULL, LV_ALIGN_IN_LEFT_MID, 0, 32);
}




lv_obj_t *label_text1=NULL,*label_text2=NULL,*label_nowtemper=NULL;
lv_style_t city_style,text_style1;

/*
	Ò³Ãæ1µÄÎÄ±¾ÏÔÊ¾****************************************************************************************************************
*/
void lv_interface1_show_text(void)
{
	//ÅäÖÃÑùÊ½
	lv_style_copy(&city_style,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	city_style.text.font = &font1;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	city_style.text.color = LV_COLOR_RED; 
 
	
	//ÉèÖÃÎÄ±¾
	label_text1 = lv_label_create(controot1, NULL);	
	lv_label_set_style(label_text1,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_text1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 45); 
	lv_label_set_text(label_text1, "éƒ‘å·");							//ÏÔÊ¾ºº×ÖÖ£Öİ
	//lv_label_set_text(title0,"\xE8\xAF\xAD\xE9\x9F\xB3\xE6\xB0\x94\xE8\xB1\xA1\xE7\xAB\x99");
	
	//ÅäÖÃÑùÊ½
	lv_style_copy(&text_style1,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	text_style1.text.font = &font3;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	text_style1.text.color = LV_COLOR_RED;
	
	//ÉèÖÃÎÄ±¾
//	label_text2 = lv_label_create(controot1, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&text_style1); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_CENTER, -48, -17); 
//	lv_label_set_text(label_text2, "1ã€ä¸»é¡µé¢ ");			//¡°Ö÷Ò³Ãæ¡±ÎÄ×Ö
//	
//	label_text2 = lv_label_create(controot1, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&text_style1); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_CENTER, -48,0); 
//	lv_label_set_text(label_text2, "2ã€ä¸‰å¤©å¤©æ°” ");			//¡°ÈıÌìÌìÆø¡±ÎÄ×Ö
//	
//	label_text2 = lv_label_create(controot1, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&text_style1); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_CENTER, -48,17); 
//	lv_label_set_text(label_text2, "3ã€å®¤å†…ç¯å¢ƒ ");			//¡°ÊÒÄÚ»·¾³¡±ÎÄ×Ö
	
	memset(tmpbuff,0,sizeof(tmpbuff));
	sprintf(tmpbuff,"%sâ„ƒ ",Now_temperature);
	
	label_nowtemper = lv_label_create(controot1, NULL); 
	lv_label_set_style(label_nowtemper,LV_LABEL_STYLE_MAIN,&text_style1); 
	lv_obj_align(label_nowtemper, NULL, LV_ALIGN_IN_TOP_MID,80,100); 		
	lv_label_set_text(label_nowtemper, tmpbuff);								//µ±Ç°ÎÂ¶È 

}

lv_obj_t *label_HUM=NULL,*label_TEM=NULL;
//Ò³Ãæ1ÏÔÊ¾ÎÂÊª¶Èº¯Êı*************************************************************************************
void lv_show_dht11(void)
{
//	lv_style_copy(&city_style,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
//	city_style.text.font = &font1;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
//	city_style.text.color = LV_COLOR_BLACK; 
	 
	label_TEM = lv_label_create(controot1, NULL);
	lv_label_set_style(label_TEM,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_TEM, NULL, LV_ALIGN_IN_LEFT_MID, 37, -35);
	
	//ÉèÖÃÎÄ±¾
	label_HUM = lv_label_create(controot1, NULL);
	lv_label_set_style(label_HUM,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_HUM, NULL, LV_ALIGN_IN_LEFT_MID, 37, -3); 
	
	
	//Éú³É±êÇ©Ö®ºó¾ÍÁ¢¼´¸³ÖµÒ»´Î£¬·ÀÖ¹¿¨¶Ù
		lv_task_ready(Task6_Handle);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%dâ„?",dht11_data.TMP);
//	lv_label_set_text(label_TEM,text_setting_tmpbuff);					//ÏÔÊ¾ÎÂ¶È

//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%d%%",dht11_data.HUM);
//	lv_label_set_text(label_HUM,text_setting_tmpbuff);					//ÏÔÊ¾Êª¶È				
}


lv_obj_t *label_light=NULL,*label_ppm=NULL;
lv_style_t LP_style;
//ÏÔÊ¾¹âÕÕÇ¿¶ÈºÍ¿ÕÆøÖÊÁ¿
void lv_show_LP(void)
{
		//ÅäÖÃÑùÊ½
	lv_style_copy(&LP_style,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	LP_style.text.font = &font1;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	LP_style.text.color = LV_COLOR_RED;
	
	label_light = lv_label_create(controot1, NULL);
	lv_label_set_style(label_light,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_light, NULL, LV_ALIGN_IN_LEFT_MID, 37,29);
	
	//ÉèÖÃÎÄ±¾
	label_ppm = lv_label_create(controot1, NULL);
	lv_label_set_style(label_ppm,LV_LABEL_STYLE_MAIN,&city_style); 
	lv_obj_align(label_ppm, NULL, LV_ALIGN_IN_LEFT_MID, 37,61); 
	

	lv_task_ready(Task4_Handle);
}	




LV_IMG_DECLARE(xiaotu1);			//ÏÈÉêÃ÷´ËÍ¼Æ¬
LV_IMG_DECLARE(xiaotu2);
LV_IMG_DECLARE(xiaotu3);



lv_style_t contstyle2;				
lv_obj_t *controot2=NULL,*interface2_img1=NULL,*interface2_img2=NULL,*interface2_img3=NULL;
lv_obj_t *img2_weather_today=NULL,*img2_weather_tommorrow=NULL,*img2_weather_aftertomm=NULL;
/*
	ÔÚÒ³Ãæ2ÊµÏÖµÄ¹ı³ÌÖĞ£¬Ò³Ãæ±»·ÖÁËÈı·İ£¬ÕâÀï·Ö±ğÌî³äÁËÈıÕÅ´óĞ¡Ò»ÖÂµÄÍ¼Æ¬
	Ã¿ÕÅÍ¼Æ¬¶¼ÊÇÒ»¸öÈİÆ÷£¬ÈİÆ÷Ãû¾ÍÊÇÍ¼Æ¬µÄobjÀàĞÍµÄ±äÁ¿¡££¨×¢Òâ£ºÕâÀïÍ¼Æ¬¿ÉÒÔÀí½âÎªÒ»¸öÈİÆ÷£©
*/
//Ò³Ãæ2µÄ¸ùÈİÆ÷´´½¨ºÍÍ¼Æ¬ÏÔÊ¾*******************************************************************************************************************¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª
void lv_interface2_show_picture(void)
{
	//ÉèÖÃÈİÆ÷ÑùÊ½
	lv_style_copy(&contstyle2,&lv_style_plain_color);
	contstyle2.body.main_color = LV_COLOR_WHITE;
	contstyle2.body.grad_color = LV_COLOR_WHITE;					//ÉèÖÃÈİÆ÷µÄÑÕÉ«
	
	controot2 = lv_cont_create(lv_scr_act(),NULL);				//´´½¨ÈİÆ÷
	lv_obj_set_style(controot2,&contstyle2);							//ÉèÖÃÈİÆ÷ÑùÊ½	
	lv_obj_set_size(controot2,320,240);										//ÉèÖÃÈİÆ÷µÄ´óĞ¡
	lv_obj_align(controot2,NULL,LV_ALIGN_CENTER,0,0);			//ÆÁÄ»¾ÓÖĞ¶ÔÆë
	lv_cont_set_layout(controot2,LV_LAYOUT_OFF);					//ÉèÖÃÎŞ²¼¾Ö·½Ê½
	
	interface2_img1 = lv_img_create(controot2,NULL);							
	lv_img_set_src(interface2_img1,&xiaotu1);
	lv_obj_align(interface2_img1,NULL,LV_ALIGN_IN_BOTTOM_LEFT,0,0);					//ÏÔÊ¾±³¾°Í¼
	
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
//		label_weather2_hight =  lv_label_create(controot1, NULL);						//´´½¨¸ßÎÂ±êÇ©
//		lv_label_set_style(label_weather2_hight,LV_LABEL_STYLE_MAIN,&date_style);
//		lv_obj_align(label_weather2_hight, NULL, LV_ALIGN_IN_TOP_MID,50,26);						
//	
////	  label_weather1_lowt =  lv_label_create(controot1, NULL);						//´´½¨µÍÎÂ±êÇ©
////		lv_label_set_style(label_weather1_lowt,LV_LABEL_STYLE_MAIN,&date_style);
////		lv_obj_align(label_weather1_lowt, NULL, LV_ALIGN_IN_TOP_MID,50,66);
//}

lv_style_t three_day_Words;
//Ò³Ãæ2ÏÔÊ¾ÎÄ±¾º¯Êı****************************************************************************************
void lv_interface2_show_text()
{
	//ÅäÖÃ×ÖÌåÑùÊ½
	lv_style_copy(&three_day_Words,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	three_day_Words.text.font = &font1;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	three_day_Words.text.color = LV_COLOR_GREEN;
	
	//ÉèÖÃÎÄ±¾
	label_text2 = lv_label_create(lv_scr_act(), NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 30); 
	lv_label_set_text(label_text2, "ä»Šå¤© ");			//¡°½ñÌì¡±ÎÄ×Ö
	
	//ÉèÖÃÎÂ¶ÈÎÄ±¾
	label_text2 = lv_label_create(lv_scr_act(), NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 50); 
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[0].acity_weather[0].lowt,city5_weather[0].acity_weather[0].hight);
	lv_label_set_text(label_text2, text_setting_tmpbuff);			//½ñÌìÎÂ¶È
	
	//ÉèÖÃÌìÆøÎÄ±¾
	label_text2 = lv_label_create(lv_scr_act(), NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 70); 
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[0].code),1);
	lv_label_set_text(label_text2, CNWord);			//½ñÌìÌìÆø
	
	//ÉèÖÃÎÄ±¾
	label_text2 = lv_label_create(interface2_img2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID,0,30); 
	lv_label_set_text(label_text2, "æ˜å¤© ");			//¡°Ã÷Ìì¡±ÎÄ×Ö
	
	//ÉèÖÃÎÂ¶ÈÎÄ±¾
	label_text2 = lv_label_create(interface2_img2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 50); 
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[0].acity_weather[1].lowt,city5_weather[0].acity_weather[1].hight);
	lv_label_set_text(label_text2,text_setting_tmpbuff );			//Ã÷ÌìÎÂ¶È
	
	//ÉèÖÃÌìÆøÎÄ±¾
	label_text2 = lv_label_create(interface2_img2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 70); 
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[1].code),2);
	lv_label_set_text(label_text2, CNWord);			//Ã÷ÌìÌìÆø
	
	
	//ÉèÖÃÎÄ±¾
	label_text2 = lv_label_create(interface2_img3, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID,0, 30); 
	lv_label_set_text(label_text2, "åå¤© ");			//¡°ºóÌì¡±ÎÄ×Ö
	
	//ÉèÖÃÎÂ¶ÈÎÄ±¾
	label_text2 = lv_label_create(interface2_img3, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 50); 
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[0].acity_weather[2].lowt,city5_weather[0].acity_weather[2].hight);
	lv_label_set_text(label_text2, text_setting_tmpbuff);			//ºóÌìÎÂ¶È

	//ÉèÖÃÌìÆøÎÄ±¾
	label_text2 = lv_label_create(interface2_img3, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, 0, 70); 
	Weathercode_to_CNWord(atoi(city5_weather[0].acity_weather[2].code),3);
	lv_label_set_text(label_text2, CNWord);			//ºóÌìÌìÆø
	
	//ÉèÖÃÒ³Ãæ2µÄÌáÊ¾ÎÄ±¾
	label_text2 = lv_label_create(controot2, NULL); 
	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID, -64,5); 
	lv_label_set_text(label_text2, "æœ€è¿‘ä¸‰å¤©å¤©æ°”å±•ç¤º ");			//¡°×î½üÈıÌìÌìÆøÕ¹Ê¾¡±ÎÄ±¾ÏÔÊ¾
	
//		//ÉèÖÃÎÄ±¾ÏÔÊ¾¼¸ÌìÄê
//	label_text2 = lv_label_create(interface2_img3, NULL); 
//	lv_label_set_style(label_text2,LV_LABEL_STYLE_MAIN,&three_day_Words); 
//	lv_obj_align(label_text2, NULL, LV_ALIGN_IN_TOP_MID,0, 20); 
	
}


lv_obj_t *img3_weather_today=NULL,*img3_weather_tommorrow=NULL,*img3_weather_aftertomm=NULL;
lv_obj_t *controot3=NULL,*interface3_img1=NULL,*interface3_img2=NULL;
//Ò³Ãæ3¸ùÈİÆ÷´´½¨ºÍÍ¼Æ¬ÏÔÊ¾****************************************************************************************************¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª
void lv_interface3_show_picture(void)
{
	controot3 = lv_cont_create(lv_scr_act(),NULL);				//´´½¨ÈİÆ÷

	lv_obj_set_size(controot3,320,240);										//ÉèÖÃÈİÆ÷µÄ´óĞ¡
	lv_obj_align(controot3,NULL,LV_ALIGN_CENTER,0,0);			//ÆÁÄ»¾ÓÖĞ¶ÔÆë
	lv_cont_set_layout(controot3,LV_LAYOUT_OFF);					//ÉèÖÃÎŞ²¼¾Ö·½Ê½
	
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
	//ÅäÖÃ×ÖÌåÑùÊ½
	lv_style_copy(&city4_style,&lv_style_plain_color);			//½«×ÖÌå¸ñÊ½¸³³õÖµ ÕâÒ»²½±ØĞëĞ´ 
	city4_style.text.font = &font1;												//ÔÚÑùÊ½ÖĞÊ¹ÓÃÕâ¸ö×ÖÌå,font1ÊÇºº×Ö×Ö¿â
	city4_style.text.color = LV_COLOR_RED;
	
	//ÉèÖÃÎÄ±¾
	label3_text1 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text1,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text1, NULL, LV_ALIGN_IN_TOP_RIGHT, -40, 40); 
	lv_label_set_text(label3_text1, "åŒ—äº¬ ");			//¡°±±¾©¡±ÎÄ×Ö
	
	//ÉèÖÃÎÄ±¾
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, label3_text1, LV_ALIGN_IN_BOTTOM_MID, 0, 40); 
	lv_label_set_text(label3_text2, "ä¸Šæµ· ");			//¡°ÉÏº£¡±ÎÄ×Ö
	
	//ÉèÖÃÎÄ±¾
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, label3_text1, LV_ALIGN_IN_BOTTOM_MID, 0, 80); 
	lv_label_set_text(label3_text2, "å¹¿å· ");			//¡°¹ãÖİ¡±ÎÄ×Ö
	
	//ÉèÖÃÎÄ±¾
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, label3_text1, LV_ALIGN_IN_BOTTOM_MID, 0, 120); 
	lv_label_set_text(label3_text2, "æ·±åœ³ ");			//¡°ÉîÛÚ¡±ÎÄ×Ö
	
		//ÉèÖÃÎÄ±¾
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, NULL, LV_ALIGN_IN_TOP_MID, -80, 0); 
	lv_label_set_text(label3_text2, "å…¨å›½ä¸»è¦åŸå¸‚å¤©æ°”å±•ç¤º ");			//¡°È«¹úÖ÷ÒªÌìÆøÕ¹Ê¾¡±ÎÄ×Ö
	
	//ÉèÖÃÎÄ±¾
	label3_text2 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text2,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text2, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 50); 
	lv_label_set_text(label3_text2, "ä»Šå¤© ");														//¡°½ñÌì¡±ÎÄ×Ö
	
	//ÉèÖÃÎÄ±¾
	label3_text3 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text3,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text3, label3_text2, LV_ALIGN_OUT_RIGHT_MID, 40, 0); 
	lv_label_set_text(label3_text3, "æ˜å¤© ");			//¡°Ã÷Ìì¡±ÎÄ×Ö
	
	//ÉèÖÃÎÄ±¾
	label3_text3 = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_text3,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_text3, label3_text2, LV_ALIGN_OUT_RIGHT_MID, 100, 0); 
	lv_label_set_text(label3_text3, "åå¤© ");			//¡°ºóÌì¡±ÎÄ×Ö

	//ÉèÖÃÎÂ¶ÈÎÄ±¾
	label3_today = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_today,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_today, label3_text2, LV_ALIGN_IN_BOTTOM_MID, 0, 30);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[1].acity_weather[0].lowt,city5_weather[1].acity_weather[0].hight);	
//	lv_label_set_text(label3_today, text_setting_tmpbuff);									//½ñÌìÎÂ¶È
	
		//ÉèÖÃÎÂ¶ÈÎÄ±¾
	label3_tommorrow = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_tommorrow,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_tommorrow, label3_today, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[1].acity_weather[1].lowt,city5_weather[1].acity_weather[1].hight);	
//	lv_label_set_text(label3_tommorrow, text_setting_tmpbuff);									//Ã÷ÌìÎÂ¶È
	
		//ÉèÖÃÎÂ¶ÈÎÄ±¾
	label3_aftertomm = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_aftertomm,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_aftertomm, label3_today, LV_ALIGN_OUT_RIGHT_MID, 100, 0);
//	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
//	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[1].acity_weather[2].lowt,city5_weather[1].acity_weather[2].hight);		
//	lv_label_set_text(label3_aftertomm, text_setting_tmpbuff);									//ºóÌìÎÂ¶È
	
	//ÉèÖÃÌìÆøÖĞÎÄÎÄ±¾
	label3_todayCN = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_todayCN,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_todayCN, label3_today, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
//	Weathercode_to_CNWord(atoi(city5_weather[1].acity_weather[0].code));
//	lv_label_set_text(label3_todayCN, CNWord);																//½ñÌìÌìÆøÎÄ±¾
	
		//ÉèÖÃÌìÆøÖĞÎÄÎÄ±¾
	label3_tommorrowCN = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_tommorrowCN,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_tommorrowCN, label3_todayCN, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
//	Weathercode_to_CNWord(atoi(city5_weather[1].acity_weather[1].code));
//	lv_label_set_text(label3_tommorrowCN, CNWord);														//Ã÷ÌìÌìÆøÎÄ±¾

	//ÉèÖÃÌìÆøÖĞÎÄÎÄ±¾
	label3_aftertommCN = lv_label_create(controot3, NULL); 
	lv_label_set_style(label3_aftertommCN,LV_LABEL_STYLE_MAIN,&city4_style); 
	lv_obj_align(label3_aftertommCN, label3_todayCN, LV_ALIGN_OUT_RIGHT_MID, 100, 0);
//	Weathercode_to_CNWord(atoi(city5_weather[1].acity_weather[2].code));
//	lv_label_set_text(label3_aftertommCN, CNWord);														//ºóÌìÌìÆøÎÄ±¾
	
	//ÇĞ»»µ½¸ÃÒ³ÃæµÄÊ±ºòÄ¬ÈÏÏÔÊ¾±±¾©ÌìÆø£¬ºóĞø¿ÉÊÖ¶¯ÇĞ»»
	day1_weather_show(1);
	day2_weather_show(1);
	day3_weather_show(1);


	
}

/*===³ÇÊĞ²ÎÊı¶ÔÕÕ±í===
			iÖµ			³ÇÊĞ
			1				±±¾©
			2				ÉÏº£
			3				¹ãÖİ
			4				ÉîÛÚ
*/
//Õ¹Ê¾µÚi¸ö³ÇÊĞµÚÒ»ÌìµÄÎÂ¶ÈºÍÖĞÎÄÌìÆøÎÄ±¾
void day1_weather_show(int i)
{
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[i].acity_weather[0].lowt,city5_weather[i].acity_weather[0].hight);	
	lv_label_set_text(label3_today, text_setting_tmpbuff);
	Weathercode_to_CNWord(atoi(city5_weather[i].acity_weather[0].code),1);
	lv_label_set_text(label3_todayCN, CNWord);								//½ñÌìÌìÆø
}


//Õ¹Ê¾µÚi¸ö³ÇÊĞµÚ¶şÌìµÄÎÂ¶ÈºÍÖĞÎÄÌìÆøÎÄ±¾
void day2_weather_show(int i)
{
		memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[i].acity_weather[1].lowt,city5_weather[i].acity_weather[1].hight);	
	lv_label_set_text(label3_tommorrow, text_setting_tmpbuff);
	Weathercode_to_CNWord(atoi(city5_weather[i].acity_weather[1].code),2);
	lv_label_set_text(label3_tommorrowCN, CNWord);														//Ã÷ÌìÌìÆøÎÄ±¾
}


//Õ¹Ê¾µÚi¸ö³ÇÊĞµÚÈıÌìµÄÎÂ¶ÈºÍÖĞÎÄÌìÆøÎÄ±¾
void day3_weather_show(int i)
{
	memset(text_setting_tmpbuff,0,sizeof(text_setting_tmpbuff));
	sprintf(text_setting_tmpbuff,"%s/%sâ„ƒ ",city5_weather[i].acity_weather[2].lowt,city5_weather[i].acity_weather[2].hight);		
	lv_label_set_text(label3_aftertomm, text_setting_tmpbuff);
	Weathercode_to_CNWord(atoi(city5_weather[i].acity_weather[2].code),3);
	lv_label_set_text(label3_aftertommCN, CNWord);														//ºóÌìÌìÆøÎÄ±¾	

}

void lv_interface4_show_picture(void)
{
	controot3 = lv_cont_create(lv_scr_act(),NULL);				//´´½¨ÈİÆ÷

	lv_obj_set_size(controot3,320,240);										//ÉèÖÃÈİÆ÷µÄ´óĞ¡
	lv_obj_align(controot3,NULL,LV_ALIGN_CENTER,0,0);			//ÆÁÄ»¾ÓÖĞ¶ÔÆë
	lv_cont_set_layout(controot3,LV_LAYOUT_OFF);					//ÉèÖÃÎŞ²¼¾Ö·½Ê½
	
	interface3_img1 = lv_img_create(controot3,NULL);										
	lv_img_set_src(interface3_img1,&yunduan);
	lv_obj_align(interface3_img1,NULL,LV_ALIGN_IN_TOP_LEFT,0,0);

}




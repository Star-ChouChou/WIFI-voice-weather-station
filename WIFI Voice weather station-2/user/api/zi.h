#ifndef _____zi_____h______
#define _____zi_____h______
 extern unsigned char zf[];
 extern unsigned char zf_index[];

 extern unsigned char hz[];
 extern unsigned char hz_index[];


 /*****************************************************************************
*****函数名：  GB16_NUM()
*****参数：    无
*****功能：    返回GB16字符个数
*****作者：    
*****创建时间：
*****备注：创建这个函数是解决KEIL中sizeof()必须和结构变量同一个文件才准确识别的BUGE
*****************************************************************************/  
extern unsigned int GB16_NUM(void) ;
 #endif

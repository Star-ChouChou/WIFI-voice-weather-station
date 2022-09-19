#include "dht11.h"
#include "stdio.h"
#include "delay.h"

//设置枚举表示
enum mode{OUTPUT=0,INPUT};
DHT_Data dht11_data;		//存储温湿度的结构体


//初始化DHT11相关引脚
void DHT11_IOConfig(u8 mode)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	
	//配IO口模式
	GPIO_InitTypeDef GPIO_InitStruct;//定义结构体变量
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	
	if(mode == OUTPUT)
	{
		GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_OD;		//开漏输出
	}
	else
	{
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//浮空输入
	}
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}


//获取温湿度数据
u8 Get_dhtdata(void)
{
	u8 count=0;
	DHT11_IOConfig(OUTPUT);		//设置引脚为输出模式
	DHT11_L;									//先由单片机拉低电平18~30ms，通知传感器准备数据
	Delay_ms(20);
	DHT11_H;									//再由单片机拉高电平，等待传感器的响应
	DHT11_IOConfig(INPUT);		//单片机释放总线，传感器得到总线，设置为输入模式
//	while(DHT11_DATA == 0)		
//	{
//		Delay_us(1);
//		count++;
//		if(count>100)
//		{
//			return 1;
//		}
//	}
	
	count =0;
	while(DHT11_DATA == 1)		//等待传感器拉低电平回复响应信号，如果是高就一直等待
	{
		Delay_us(1);
		count++;
		if(count>100)
		{
			return 2;
		}
	}
	
	count=0;
	while(DHT11_DATA == 0)		//等待传感器拉高电平
	{
		Delay_us(1);
		count++;
		if(count>100)
		{
			return 3;
		}
	}
	//printf("正在执行\r\n");
	u8 data[5]={0};
	for(u8 i=0;i<40;i++)
	{
		count=0;
		while(DHT11_DATA == 1)		//等待传感器拉低电平
		{
			Delay_us(1);
			count++;
			if(count>100)
			{	
				return 4;
			}
		}
		count=0;
		while(DHT11_DATA == 0)		//等待传感器拉高电平
		{
			Delay_us(1);
			count++;
			if(count>100)
			{
				return 5;
			}
		}
		Delay_us(30);
		if(DHT11_DATA)				//为高电平收到的就是1
		{
			data[i/8]|=(1<<(7-i%8));
		}
		else			//为低电平收到的就是0
		{
			data[i/8]&=~(1<<(7-i%8));
		}
	}
//	printf("正在执行\r\n");
	u8 checknum=data[0]+data[1]+data[2]+data[3];
	if(checknum == data[4])
	{
//		printf("temperatuer:%d\r\n",data[2]);
		dht11_data.TMP = data[2];
//		printf("humidity:%d\r\n",data[0]);
		dht11_data.HUM = data[0];
		return 0;					//校验成功成功获取数据
	}
	else							//校验不正确返回错误
	{
		return 6;
	}
}

//u8 Get_dhtdata(void)
//{	
//	u8 data[5]={0};
//	u8 count=0;
//	//发送起始信号
//	DHT11_IOConfig(OUTPUT);
//	DHT11_L;
//	Delay_ms(20);
//	DHT11_H;
//	//切换为输入 准备检测应答信号  和数据 
//	DHT11_IOConfig(INPUT);
//	
//	//等待数据线被拉高
//	while(DHT11_DATA == 0) //等待高电平到来
//	{
//		count ++;
//		Delay_us(1);
//		if(count >=100) return 1;
//	}
//	//等待数据线被拉低  响应信号  83us左右低电平
//	count=0;
//	while(DHT11_DATA == 1) 
//	{
//		count ++;
//		Delay_us(1);
//		if(count >=100) return 2;
//	}	
//	//等待  响应信号结束，  87us左右高电平到来
//	count=0;
//	while(DHT11_DATA == 0) 
//	{
//		count ++;
//		Delay_us(1);
//		if(count >=100) return 3;
//	}
//	for(u8 i=0;i<40;i++)
//	{
//		//等待高电平结束  数据的低电平起始到来
//		count=0;
//		while(DHT11_DATA == 1) 
//		{
//			count ++;
//			Delay_us(1);
//			if(count >=100) return 4;
//		}
//		//等待低电平结束  数据的高电平起始到来
//		count=0;
//		while(DHT11_DATA == 0) 
//		{
//			count ++;
//			Delay_us(1);
//			if(count >=100) return 5;
//		}		
//		Delay_us(30);
//		if(DHT11_DATA == 0)//数据是0  
//		{
//			data[i/8] &= ~(1<<(7- i%8 ));
//		}else		//数据是1 
//		{
//			data[i/8] |= (1<<(7- i%8 ));
//		}
//	}
//	
//	u8 cheak = data[0]+data[1]+data[2]+data[3];
//	if( cheak == data[4] )
//	{
//		printf("TEM = %d\r\n",data[2]);
//		printf("HUM = %d\r\n",data[0]);
//	}else
//	printf("检验错误\r\n");
//	
//	return 0;
//}




#include "dht11.h"
#include "stdio.h"
#include "delay.h"

//����ö�ٱ�ʾ
enum mode{OUTPUT=0,INPUT};
DHT_Data dht11_data;		//�洢��ʪ�ȵĽṹ��


//��ʼ��DHT11�������
void DHT11_IOConfig(u8 mode)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	
	//��IO��ģʽ
	GPIO_InitTypeDef GPIO_InitStruct;//����ṹ�����
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	
	if(mode == OUTPUT)
	{
		GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_OD;		//��©���
	}
	else
	{
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//��������
	}
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}


//��ȡ��ʪ������
u8 Get_dhtdata(void)
{
	u8 count=0;
	DHT11_IOConfig(OUTPUT);		//��������Ϊ���ģʽ
	DHT11_L;									//���ɵ�Ƭ�����͵�ƽ18~30ms��֪ͨ������׼������
	Delay_ms(20);
	DHT11_H;									//���ɵ�Ƭ�����ߵ�ƽ���ȴ�����������Ӧ
	DHT11_IOConfig(INPUT);		//��Ƭ���ͷ����ߣ��������õ����ߣ�����Ϊ����ģʽ
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
	while(DHT11_DATA == 1)		//�ȴ����������͵�ƽ�ظ���Ӧ�źţ�����Ǹ߾�һֱ�ȴ�
	{
		Delay_us(1);
		count++;
		if(count>100)
		{
			return 2;
		}
	}
	
	count=0;
	while(DHT11_DATA == 0)		//�ȴ����������ߵ�ƽ
	{
		Delay_us(1);
		count++;
		if(count>100)
		{
			return 3;
		}
	}
	//printf("����ִ��\r\n");
	u8 data[5]={0};
	for(u8 i=0;i<40;i++)
	{
		count=0;
		while(DHT11_DATA == 1)		//�ȴ����������͵�ƽ
		{
			Delay_us(1);
			count++;
			if(count>100)
			{	
				return 4;
			}
		}
		count=0;
		while(DHT11_DATA == 0)		//�ȴ����������ߵ�ƽ
		{
			Delay_us(1);
			count++;
			if(count>100)
			{
				return 5;
			}
		}
		Delay_us(30);
		if(DHT11_DATA)				//Ϊ�ߵ�ƽ�յ��ľ���1
		{
			data[i/8]|=(1<<(7-i%8));
		}
		else			//Ϊ�͵�ƽ�յ��ľ���0
		{
			data[i/8]&=~(1<<(7-i%8));
		}
	}
//	printf("����ִ��\r\n");
	u8 checknum=data[0]+data[1]+data[2]+data[3];
	if(checknum == data[4])
	{
//		printf("temperatuer:%d\r\n",data[2]);
		dht11_data.TMP = data[2];
//		printf("humidity:%d\r\n",data[0]);
		dht11_data.HUM = data[0];
		return 0;					//У��ɹ��ɹ���ȡ����
	}
	else							//У�鲻��ȷ���ش���
	{
		return 6;
	}
}

//u8 Get_dhtdata(void)
//{	
//	u8 data[5]={0};
//	u8 count=0;
//	//������ʼ�ź�
//	DHT11_IOConfig(OUTPUT);
//	DHT11_L;
//	Delay_ms(20);
//	DHT11_H;
//	//�л�Ϊ���� ׼�����Ӧ���ź�  ������ 
//	DHT11_IOConfig(INPUT);
//	
//	//�ȴ������߱�����
//	while(DHT11_DATA == 0) //�ȴ��ߵ�ƽ����
//	{
//		count ++;
//		Delay_us(1);
//		if(count >=100) return 1;
//	}
//	//�ȴ������߱�����  ��Ӧ�ź�  83us���ҵ͵�ƽ
//	count=0;
//	while(DHT11_DATA == 1) 
//	{
//		count ++;
//		Delay_us(1);
//		if(count >=100) return 2;
//	}	
//	//�ȴ�  ��Ӧ�źŽ�����  87us���Ҹߵ�ƽ����
//	count=0;
//	while(DHT11_DATA == 0) 
//	{
//		count ++;
//		Delay_us(1);
//		if(count >=100) return 3;
//	}
//	for(u8 i=0;i<40;i++)
//	{
//		//�ȴ��ߵ�ƽ����  ���ݵĵ͵�ƽ��ʼ����
//		count=0;
//		while(DHT11_DATA == 1) 
//		{
//			count ++;
//			Delay_us(1);
//			if(count >=100) return 4;
//		}
//		//�ȴ��͵�ƽ����  ���ݵĸߵ�ƽ��ʼ����
//		count=0;
//		while(DHT11_DATA == 0) 
//		{
//			count ++;
//			Delay_us(1);
//			if(count >=100) return 5;
//		}		
//		Delay_us(30);
//		if(DHT11_DATA == 0)//������0  
//		{
//			data[i/8] &= ~(1<<(7- i%8 ));
//		}else		//������1 
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
//	printf("�������\r\n");
//	
//	return 0;
//}




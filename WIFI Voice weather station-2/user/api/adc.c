#include "adc.h"
#include "delay.h"

/*��ADC1��ʱ�Ӻ�����ʱ�ӣ��������ŵ�ģ�����룻
	���ýṹ������ADCʹ��ADC_Init()������
*/
void light_resistance_adcConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);					//����ADCʱ��6��Ƶ
	
	GPIO_InitTypeDef PA5;
	PA5.GPIO_Mode = GPIO_Mode_AIN;
	PA5.GPIO_Pin = GPIO_Pin_5;
	PA5.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&PA5);
	
	GPIO_InitTypeDef PC1;
	PC1.GPIO_Mode = GPIO_Mode_AIN;
	PC1.GPIO_Pin = GPIO_Pin_5;
	PC1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&PC1);
	
	ADC1_DMAConfig();					//����DMA
	
	//�ṹ��������ADC1������
	ADC_InitTypeDef ADC_InitStructure; 												//����ADC��ʼ���ṹ��
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 				//����Ϊ��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 						//���ö�ͨ�����ǵ�ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 			//�Ƿ�����ת��
	//�ⲿ������ʽ���ã���������Ϊ���������ת��
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 		//ADC�������Ҷ������ã�Ĭ���Ҷ���		
	ADC_InitStructure.ADC_NbrOfChannel = 2;					//˳����й���ת����ADCͨ����Ŀ����Χ1~16(��ת��ͨ������Ŀ) 					
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	/*����ͨ���������顢ת��˳��ת��ʱ��*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADC1,ENABLE);					//ʹ��ADC1��DMA����
	ADC_Cmd(ADC1,ENABLE);							//ʹ��ADC1
	ADC_ResetCalibration(ADC1);				//����У׼�Ĵ���
	ADC_StartCalibration(ADC1);				//����У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}




u16 ADCValue[2]={0};
//ADC1��DMA���ú���
void ADC1_DMAConfig(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	//��DMA��AHBʱ��
	
	DMA_DeInit(DMA1_Channel1);					//��ADC1�����мĴ�������ΪĬ��ֵ
	DMA_InitTypeDef DMA_InitStructure; 
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR;	//ָ�������豸��ַ 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCValue; 		//ָ���ڴ���ʼ��ַ�����ｫ���ݴ洢���������У�ָ���ĵ�ַ������ĵ�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 							//ָ��������������Դ��������ȥ��
	//ѭ��ģʽ������������������֮�󣬻����»ص�ָ���ڴ����ʼ��ַ���¼���д������
	DMA_InitStructure.DMA_BufferSize = 2; 															
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 		//�����ַ��������һֱ��һ��DR�Ĵ����϶�ȡ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 						//�ڴ�洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 		//�������ݴ�С����16λ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;								//�ڴ����ݴ�С
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 		//������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;			//����������ȼ�Ϊ�����ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 						//��ʹ���ڴ浽�ڴ��ģʽ
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);				//ADC1��DMA1��ͨ��1
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}



//ʹ���޷��˲���ȡ����ģ��ֵ
#define A 300			
//��ȡadcģ��ֵ����
u16 get_adcvalue(void)
{
	static u16 light_val=0,newlight_val=0;
	static int light_flag=0;
	u16 temp;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);			//�յ�������־EOC
	newlight_val=ADC_GetConversionValue(ADC1);
	if(light_flag == 0)		//�ж��ǲ��ǵ�һ�ε��õı�־λ
	{
		light_flag=1;
		light_val=newlight_val;
	}
	
	//�������޷�������Чֵ�ͷ�����һ�ε���ֵ
	if((light_val-newlight_val>A)||(newlight_val-light_val>A))	
	{
		return light_val;
	}
	light_val=newlight_val;
	return newlight_val;	//�������޷��ͷ�������ֵ��ͬʱ���ϴ���Чֵ����
}


void MQ_adcConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOC,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);					//����ADCʱ��
	
	GPIO_InitTypeDef PC1;
	PC1.GPIO_Mode = GPIO_Mode_AIN;
	PC1.GPIO_Pin = GPIO_Pin_1;
	PC1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&PC1);
	
	ADC_InitTypeDef ADC_InitStructure; 												//����ADC��ʼ���ṹ��
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 				//����Ϊ��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 						//���ö�ͨ�����ǵ�ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 			//�Ƿ�����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;	
	//�ⲿ������ʽ���ã���������Ϊ���������ת��
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 		//ADC�������Ҷ������ã�Ĭ���Ҷ���		
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//˳����й���ת����ADCͨ����Ŀ����Χ1~16(��ת��ͨ������Ŀ) 					
	ADC_Init(ADC2, &ADC_InitStructure);
	
	
	/*����ͨ���������顢ת��˳��ת��ʱ��*/
	ADC_RegularChannelConfig(ADC2,ADC_Channel_11,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC2,ENABLE);				
	ADC_ResetCalibration(ADC2);				//����У׼�Ĵ���
	ADC_StartCalibration(ADC2);				//����У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
}



#define N 8			//����ƽ���˲�����ƽ������
//��ȡMQģ��ֵ����
u16 get_MQvalue(void)
{
	u16 sum=0;
	for(u8 i=1;i<=N;i++)
	{
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)==RESET);			//�յ�������־EOC
		sum+=ADC_GetConversionValue(ADC2);
		//Delay_ms(50);
	}
	return (u16)(sum/N);				//����ƽ���˲�
}

/**************
��ͨ��ģʽ�£���ȡ����͹�������
void Set_ADC_Init(void)
{
	//����ADCʱ�ӣ�������14MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//����ʱ��   PA5--����--ADC12_IN5   PC1--����--ADC123_IN11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//�˿�PA5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//�˿�PC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC1
	
	//���ö˿ڳ�ʼ��Ϊģ������
	GPIO_InitTypeDef Set_GPIO_init;
	Set_GPIO_init.GPIO_Pin = GPIO_Pin_5;
	Set_GPIO_init.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA,&Set_GPIO_init);  //���մ�����������
	Set_GPIO_init.GPIO_Pin = GPIO_Pin_1	;
	GPIO_Init(GPIOC,&Set_GPIO_init);//��������������
	
	//ADC��ʼ������
	ADC_InitTypeDef Set_ADC_Init_Struct;
	//����ת��ģʽ���ǵ���ת��ģʽ
	Set_ADC_Init_Struct.ADC_ContinuousConvMode = DISABLE;//����ת��	
	//ָ��Ϊ����뻹���Ҷ���
	Set_ADC_Init_Struct.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	//����������������ͨ����ADC�ⲿ������
	Set_ADC_Init_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//����ADC�����ڶ�������˫ADCģʽ
	Set_ADC_Init_Struct.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	//ת��ͨ����Ŀ
	Set_ADC_Init_Struct.ADC_NbrOfChannel = 2;
	//��ͨ��ģʽ����ɨ��ģʽ
	Set_ADC_Init_Struct.ADC_ScanConvMode = DISABLE;//��ͨ��ģʽ
	ADC_Init(ADC1,&Set_ADC_Init_Struct);
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
	//ʹ��ADC1����У׼
	ADC_Cmd(ADC1,ENABLE);
	//ʹ�ܻ���ʧ��ָ����ADC������ͨ���ļ��ģʽ
	ADC_DiscModeCmd(ADC1,ENABLE);
	//����У׼�Ĵ�������ȡ��״̬
	ADC_ResetCalibration(ADC1);
	//�ȴ�У׼�Ĵ���  RESETΪУ׼�Ĵ�������ɳ�ʼ��
	while(ADC_GetResetCalibrationStatus(ADC1))
	{};
	//��ʼָ��ADC��У׼״̬
	ADC_StartCalibration(ADC1);
	//�ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC1))
	{};
}
u16 ADC_data[2]={0};
void Get_ADC_Data(void)
{
	u32 light_data = 0;
	u8 count = 0;
	u16 smog_data = 0;
	u16 new_smog_data = 0;
	for(u8 i=0;i<20;i++){
		if(i%2   == 0){
			//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
			ADC_SoftwareStartConvCmd(ADC1,ENABLE);
			//��ȡEOC״̬λ��ת�����Ϊ1
			while(ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET)
			{};
			light_data += ADC_GetConversionValue(ADC1);
		}
		else if(i%2 == 1)
		{
			//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
			ADC_SoftwareStartConvCmd(ADC1,ENABLE);
			//��ȡת��״̬��ת�����Ϊ1
			while(ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET)
			{};
			count ++;
			
			if(count == 1 )
				smog_data = ADC_GetConversionValue(ADC1);
			else if(count >= 2)
			{
				new_smog_data = ADC_GetConversionValue(ADC1);
				if((new_smog_data-smog_data>A)||(smog_data-new_smog_data>A))
					smog_data = smog_data; 
				else
					smog_data = new_smog_data;
			}
		}
	}
	ADC_data[0] =(light_data/10); 
	ADC_data[1] = smog_data;
	count = 0;

}

*************/
//u16 ADCValue[2]={0};
//void Set_ADC_Init(void)
//{
//	//����ADCʱ�ӣ�������14MHZ
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//	//����ʱ��   PA5--����--ADC12_IN5   PC1--����--ADC123_IN11
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//�˿�PA5
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//�˿�PC1
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC1
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//ʹ��DMA1
//	//���ö˿ڳ�ʼ��Ϊģ������
//	GPIO_InitTypeDef Set_GPIO_init;
//	Set_GPIO_init.GPIO_Pin = GPIO_Pin_5;
//	Set_GPIO_init.GPIO_Mode = GPIO_Mode_AIN;//ģ������
//	GPIO_Init(GPIOA,&Set_GPIO_init);  //���մ�����������
//	Set_GPIO_init.GPIO_Pin = GPIO_Pin_1	;
//	GPIO_Init(GPIOC,&Set_GPIO_init);//��������������
//	
//	//ADC��ʼ������
//	ADC_InitTypeDef Set_ADC_Init_Struct;
//	//����ת��ģʽ���ǵ���ת��ģʽ
//	Set_ADC_Init_Struct.ADC_ContinuousConvMode = ENABLE;//����ת��	
//	//ָ��Ϊ����뻹���Ҷ���
//	Set_ADC_Init_Struct.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
//	//����������������ͨ����ADC�ⲿ������
//	Set_ADC_Init_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//	//����ADC�����ڶ�������˫ADCģʽ
//	Set_ADC_Init_Struct.ADC_Mode = ADC_Mode_Independent;//����ģʽ
//	//ת��ͨ����Ŀ
//	Set_ADC_Init_Struct.ADC_NbrOfChannel = 2;
//	//��ͨ��ģʽ����ɨ��ģʽ
//	Set_ADC_Init_Struct.ADC_ScanConvMode = ENABLE;//��ͨ��ģʽ
//	ADC_Init(ADC1,&Set_ADC_Init_Struct);
//	
//	//DMA1��ʼ��
//	DMA_InitTypeDef Set_DMA_Init_Struct;
//	//DMA�����С��������
//	Set_DMA_Init_Struct.DMA_BufferSize = 2;
//	//��������Ϊ���ݴ����Ŀ�ĵػ�����Դ
//	Set_DMA_Init_Struct.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ������Դ
//	//ʹ�� DMA ͨ�����ڴ浽�ڴ洫��
//	Set_DMA_Init_Struct.DMA_M2M = DMA_M2M_Disable;//��ʹ��
//	//DMA�ڴ����ַ
//	Set_DMA_Init_Struct.DMA_MemoryBaseAddr = (uint32_t)ADCValue;
//	//DMA�ڴ����ݿ��
//	Set_DMA_Init_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	//�ڴ�Ĵ����������
//	Set_DMA_Init_Struct.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ�Ĵ�������
//	//DMA����ģʽ
//	Set_DMA_Init_Struct.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
//	//DMA�������ַ
//	Set_DMA_Init_Struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
//	//DMA�������ݿ��
//	Set_DMA_Init_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	//�����ַ�Ĵ����������
//	Set_DMA_Init_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	//DMAͨ�����ȼ�
//	Set_DMA_Init_Struct.DMA_Priority = DMA_Priority_High;
//	
//	DMA_Init(DMA1_Channel1,&Set_DMA_Init_Struct);
//	
//	//ʹ��DMA
//	DMA_Cmd(DMA1_Channel1,ENABLE);
//	//ʹ��ADCͨ����DMA1
//	ADC_DMACmd(ADC1,ENABLE);
//	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
//	//ʹ��ADC1����У׼
//	ADC_Cmd(ADC1,ENABLE);

//	//����У׼�Ĵ�������ȡ��״̬
//	ADC_ResetCalibration(ADC1);
//	//�ȴ�У׼�Ĵ���  RESETΪУ׼�Ĵ�������ɳ�ʼ��
//	while(ADC_GetResetCalibrationStatus(ADC1))
//	{};
//	//��ʼָ��ADC��У׼״̬
//	ADC_StartCalibration(ADC1);
//	//�ȴ�У׼���
//	while(ADC_GetCalibrationStatus(ADC1))
//	{};
//}




//void Get_ADC_Data(void)
//{
//	for(u8 i=0;i<2;i++){
//		//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
//		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
//		//��ȡEOC״̬λ��ת�����Ϊ1
//		while(ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET)
//		{};
//		ADC_data[i] = ADC_GetConversionValue(ADC1);
//		}
//}





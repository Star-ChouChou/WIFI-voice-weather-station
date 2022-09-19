#include "adc.h"
#include "delay.h"

/*开ADC1的时钟和引脚时钟；配置引脚的模拟输入；
	利用结构体配置ADC使用ADC_Init()函数；
*/
void light_resistance_adcConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);					//设置ADC时钟6分频
	
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
	
	ADC1_DMAConfig();					//配置DMA
	
	//结构体中配置ADC1的属性
	ADC_InitTypeDef ADC_InitStructure; 												//定义ADC初始化结构体
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 				//设置为独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 						//设置多通道还是单通道
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 			//是否连续转换
	//外部触发方式设置，这里设置为由软件触发转换
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 		//ADC数据左右对齐设置，默认右对齐		
	ADC_InitStructure.ADC_NbrOfChannel = 2;					//顺序进行规则转换的ADC通道数目，范围1~16(待转换通道的数目) 					
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	/*设置通道、规则组、转换顺序、转换时间*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADC1,ENABLE);					//使能ADC1的DMA请求
	ADC_Cmd(ADC1,ENABLE);							//使能ADC1
	ADC_ResetCalibration(ADC1);				//重置校准寄存器
	ADC_StartCalibration(ADC1);				//开启校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}




u16 ADCValue[2]={0};
//ADC1的DMA配置函数
void ADC1_DMAConfig(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	//开DMA的AHB时钟
	
	DMA_DeInit(DMA1_Channel1);					//将ADC1的所有寄存器设置为默认值
	DMA_InitTypeDef DMA_InitStructure; 
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR;	//指定外设设备地址 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCValue; 		//指定内存起始地址，这里将数据存储到了数组中，指定的地址是数组的地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 							//指定外设是数据来源还是数据去向
	//循环模式，当缓冲区计数慢满之后，会重新回到指定内存的起始地址重新继续写入数据
	DMA_InitStructure.DMA_BufferSize = 2; 															
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 		//外设地址不递增，一直从一个DR寄存器上读取数据
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 						//内存存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 		//外设数据大小半字16位长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;								//内存数据大小
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 		//工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;			//设置软件优先级为高优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 						//不使用内存到内存的模式
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);				//ADC1在DMA1的通道1
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}



//使用限幅滤波获取烟雾模拟值
#define A 300			
//获取adc模拟值函数
u16 get_adcvalue(void)
{
	static u16 light_val=0,newlight_val=0;
	static int light_flag=0;
	u16 temp;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);			//收到结束标志EOC
	newlight_val=ADC_GetConversionValue(ADC1);
	if(light_flag == 0)		//判断是不是第一次调用的标志位
	{
		light_flag=1;
		light_val=newlight_val;
	}
	
	//超过了限幅属于无效值就返回上一次的数值
	if((light_val-newlight_val>A)||(newlight_val-light_val>A))	
	{
		return light_val;
	}
	light_val=newlight_val;
	return newlight_val;	//不超过限幅就返回新数值，同时将上次有效值更新
}


void MQ_adcConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOC,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);					//设置ADC时钟
	
	GPIO_InitTypeDef PC1;
	PC1.GPIO_Mode = GPIO_Mode_AIN;
	PC1.GPIO_Pin = GPIO_Pin_1;
	PC1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&PC1);
	
	ADC_InitTypeDef ADC_InitStructure; 												//定义ADC初始化结构体
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 				//设置为独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 						//设置多通道还是单通道
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 			//是否连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;	
	//外部触发方式设置，这里设置为由软件触发转换
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 		//ADC数据左右对齐设置，默认右对齐		
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//顺序进行规则转换的ADC通道数目，范围1~16(待转换通道的数目) 					
	ADC_Init(ADC2, &ADC_InitStructure);
	
	
	/*设置通道、规则组、转换顺序、转换时间*/
	ADC_RegularChannelConfig(ADC2,ADC_Channel_11,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC2,ENABLE);				
	ADC_ResetCalibration(ADC2);				//重置校准寄存器
	ADC_StartCalibration(ADC2);				//开启校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
}



#define N 8			//算数平均滤波的求平均次数
//获取MQ模拟值函数
u16 get_MQvalue(void)
{
	u16 sum=0;
	for(u8 i=1;i<=N;i++)
	{
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)==RESET);			//收到结束标志EOC
		sum+=ADC_GetConversionValue(ADC2);
		//Delay_ms(50);
	}
	return (u16)(sum/N);				//算数平均滤波
}

/**************
单通道模式下，获取烟雾和光照数据
void Set_ADC_Init(void)
{
	//设置ADC时钟，不超过14MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置时钟   PA5--光照--ADC12_IN5   PC1--烟雾--ADC123_IN11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//端口PA5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//端口PC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC1
	
	//配置端口初始化为模拟输入
	GPIO_InitTypeDef Set_GPIO_init;
	Set_GPIO_init.GPIO_Pin = GPIO_Pin_5;
	Set_GPIO_init.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA,&Set_GPIO_init);  //光照传感器的配置
	Set_GPIO_init.GPIO_Pin = GPIO_Pin_1	;
	GPIO_Init(GPIOC,&Set_GPIO_init);//烟雾传感器的配置
	
	//ADC初始化配置
	ADC_InitTypeDef Set_ADC_Init_Struct;
	//连续转换模式还是单次转换模式
	Set_ADC_Init_Struct.ADC_ContinuousConvMode = DISABLE;//单次转换	
	//指定为左对齐还是右对齐
	Set_ADC_Init_Struct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	//定义用于启动常规通道的ADC外部触发器
	Set_ADC_Init_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//设置ADC工作在独立或者双ADC模式
	Set_ADC_Init_Struct.ADC_Mode = ADC_Mode_Independent;//独立模式
	//转换通道数目
	Set_ADC_Init_Struct.ADC_NbrOfChannel = 2;
	//单通道模式或者扫描模式
	Set_ADC_Init_Struct.ADC_ScanConvMode = DISABLE;//单通道模式
	ADC_Init(ADC1,&Set_ADC_Init_Struct);
	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
	//使能ADC1后开启校准
	ADC_Cmd(ADC1,ENABLE);
	//使能或者失能指定的ADC规则组通道的间断模式
	ADC_DiscModeCmd(ADC1,ENABLE);
	//重置校准寄存器并获取其状态
	ADC_ResetCalibration(ADC1);
	//等待校准寄存器  RESET为校准寄存器已完成初始化
	while(ADC_GetResetCalibrationStatus(ADC1))
	{};
	//开始指定ADC的校准状态
	ADC_StartCalibration(ADC1);
	//等待校准完成
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
			//使能或者失能指定的ADC的软件转换启动功能
			ADC_SoftwareStartConvCmd(ADC1,ENABLE);
			//读取EOC状态位，转换完成为1
			while(ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET)
			{};
			light_data += ADC_GetConversionValue(ADC1);
		}
		else if(i%2 == 1)
		{
			//使能或者失能指定的ADC的软件转换启动功能
			ADC_SoftwareStartConvCmd(ADC1,ENABLE);
			//读取转换状态，转换完成为1
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
//	//设置ADC时钟，不超过14MHZ
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//	//配置时钟   PA5--光照--ADC12_IN5   PC1--烟雾--ADC123_IN11
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//端口PA5
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//端口PC1
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC1
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//使能DMA1
//	//配置端口初始化为模拟输入
//	GPIO_InitTypeDef Set_GPIO_init;
//	Set_GPIO_init.GPIO_Pin = GPIO_Pin_5;
//	Set_GPIO_init.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
//	GPIO_Init(GPIOA,&Set_GPIO_init);  //光照传感器的配置
//	Set_GPIO_init.GPIO_Pin = GPIO_Pin_1	;
//	GPIO_Init(GPIOC,&Set_GPIO_init);//烟雾传感器的配置
//	
//	//ADC初始化配置
//	ADC_InitTypeDef Set_ADC_Init_Struct;
//	//连续转换模式还是单次转换模式
//	Set_ADC_Init_Struct.ADC_ContinuousConvMode = ENABLE;//单次转换	
//	//指定为左对齐还是右对齐
//	Set_ADC_Init_Struct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
//	//定义用于启动常规通道的ADC外部触发器
//	Set_ADC_Init_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//	//设置ADC工作在独立或者双ADC模式
//	Set_ADC_Init_Struct.ADC_Mode = ADC_Mode_Independent;//独立模式
//	//转换通道数目
//	Set_ADC_Init_Struct.ADC_NbrOfChannel = 2;
//	//单通道模式或者扫描模式
//	Set_ADC_Init_Struct.ADC_ScanConvMode = ENABLE;//单通道模式
//	ADC_Init(ADC1,&Set_ADC_Init_Struct);
//	
//	//DMA1初始化
//	DMA_InitTypeDef Set_DMA_Init_Struct;
//	//DMA缓存大小，即次数
//	Set_DMA_Init_Struct.DMA_BufferSize = 2;
//	//外设是作为数据传输的目的地还是来源
//	Set_DMA_Init_Struct.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据来源
//	//使能 DMA 通道的内存到内存传输
//	Set_DMA_Init_Struct.DMA_M2M = DMA_M2M_Disable;//不使能
//	//DMA内存基地址
//	Set_DMA_Init_Struct.DMA_MemoryBaseAddr = (uint32_t)ADCValue;
//	//DMA内存数据宽度
//	Set_DMA_Init_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	//内存寄存器递增与否
//	Set_DMA_Init_Struct.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存寄存器递增
//	//DMA工作模式
//	Set_DMA_Init_Struct.DMA_Mode = DMA_Mode_Circular;//循环模式
//	//DMA外设基地址
//	Set_DMA_Init_Struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
//	//DMA外设数据宽度
//	Set_DMA_Init_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	//外设地址寄存器递增与否
//	Set_DMA_Init_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	//DMA通道优先级
//	Set_DMA_Init_Struct.DMA_Priority = DMA_Priority_High;
//	
//	DMA_Init(DMA1_Channel1,&Set_DMA_Init_Struct);
//	
//	//使能DMA
//	DMA_Cmd(DMA1_Channel1,ENABLE);
//	//使能ADC通道的DMA1
//	ADC_DMACmd(ADC1,ENABLE);
//	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_55Cycles5);
//	//使能ADC1后开启校准
//	ADC_Cmd(ADC1,ENABLE);

//	//重置校准寄存器并获取其状态
//	ADC_ResetCalibration(ADC1);
//	//等待校准寄存器  RESET为校准寄存器已完成初始化
//	while(ADC_GetResetCalibrationStatus(ADC1))
//	{};
//	//开始指定ADC的校准状态
//	ADC_StartCalibration(ADC1);
//	//等待校准完成
//	while(ADC_GetCalibrationStatus(ADC1))
//	{};
//}




//void Get_ADC_Data(void)
//{
//	for(u8 i=0;i<2;i++){
//		//使能或者失能指定的ADC的软件转换启动功能
//		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
//		//读取EOC状态位，转换完成为1
//		while(ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET)
//		{};
//		ADC_data[i] = ADC_GetConversionValue(ADC1);
//		}
//}





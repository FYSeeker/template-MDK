#include "adc.h"
// PA.0 DMA-ADC

__IO uint16_t ADC_ConvertedValue[4];

/*
 * 函数名：ADC_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PA.00
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC12_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable ADC1  and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_AHBPeriph_GPIOA, ENABLE);
	
	/* Configure PA.06  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  //不上拉也不下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PA6,输入时不用设置速率

	
}


/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式   ADC1 ch0
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC12_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1);//ADC恢复默认设置	

	
	DMA_DeInit(DMA1_Channel1);	                                                    /* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)ADC1_DR_Address;

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue[0];             //内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                              //外设作为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = 4;                                           //定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里是ADC_ConvertedValue的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        									//内存地址递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;     //数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;             //数据宽度为16位
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;			//半字
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                 //循环传输
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                             //DMA_Priority设定DMA通道x的软件优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                    //DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); /* Enable ADC_DMA */	
  ADC_DMACmd(ADC1, ENABLE);  
	
	ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //规定模式装换工作在连续模式
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC的扫描方向
  ADC_Init(ADC1, &ADC_InitStructure); 


  ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_55_5Cycles);

	ADC_GetCalibrationFactor(ADC1); /* ADC 校准 */  
  ADC_Cmd(ADC1, ENABLE);  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
  ADC_StartOfConversion(ADC1); /* 软件启动ADC */ 
	
}

/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void ADC1_Init(void)
{
	ADC12_GPIO_Config();
	ADC12_Mode_Config();
}

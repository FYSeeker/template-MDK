#include "adc.h"
// PA.0 DMA-ADC

__IO uint16_t ADC_ConvertedValue[4];

/*
 * ��������ADC_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PA.00
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC12_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable ADC1  and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_AHBPeriph_GPIOA, ENABLE);
	
	/* Configure PA.06  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  //������Ҳ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PA6,����ʱ������������

	
}


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ   ADC1 ch0
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC12_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1);//ADC�ָ�Ĭ������	

	
	DMA_DeInit(DMA1_Channel1);	                                                    /* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)ADC1_DR_Address;

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue[0];             //�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                              //������Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = 4;                                           //����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ��������ADC_ConvertedValue�Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        									//�ڴ��ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;     //���ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;             //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;			//����
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                 //ѭ������
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                             //DMA_Priority�趨DMAͨ��x��������ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                    //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); /* Enable ADC_DMA */	
  ADC_DMACmd(ADC1, ENABLE);  
	
	ADC_StructInit(&ADC_InitStructure);//��ʼ��ADC�ṹ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λ����
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //�涨ģʽװ������������ģʽ
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ���Ϊ�Ҷ���
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC��ɨ�跽��
  ADC_Init(ADC1, &ADC_InitStructure); 


  ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_55_5Cycles);

	ADC_GetCalibrationFactor(ADC1); /* ADC У׼ */  
  ADC_Cmd(ADC1, ENABLE);  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
  ADC_StartOfConversion(ADC1); /* �������ADC */ 
	
}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void ADC1_Init(void)
{
	ADC12_GPIO_Config();
	ADC12_Mode_Config();
}

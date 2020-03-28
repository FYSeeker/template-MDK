
#include "stm32f0xx.h"
#include "color.h"
#include "system/delay/delay.h"

u16 Rgena=0X00;    //��ƽ��R��������
u16 Ggena=0X00;    //��ƽ��G��������
u16 Bgena=0X00;    //��ƽ��B��������
u16 Ramount=0X00;
u16 Gamount=0x00;
u16 Bamount=0x00;
u16 amount;        //������
int sample_time=10;    //����ʱ��
void tcs3200_init()
	
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);//ʹ�������� PA ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);//ʹ�������� PB ʱ��
	//S2���ų�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //�������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //����
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//S3���ų�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //�������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);

}

void KEY_EXTI_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStruct; 
     EXTI_InitTypeDef EXTI_InitStruct;
     NVIC_InitTypeDef NVIC_InitStruct;

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
              
              
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; 
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
     GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
     GPIO_Init(GPIOA, &GPIO_InitStruct);                  

     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);

              
     EXTI_ClearITPendingBit(EXTI_Line6);

              
     EXTI_InitStruct.EXTI_Line = EXTI_Line6;
     EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 
     EXTI_InitStruct.EXTI_LineCmd = ENABLE;
     EXTI_Init(&EXTI_InitStruct); 

     NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
     NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;
     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStruct);
}

void whitebalance(void)
{

  GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	amount=0;			 //��ʼ����
	delay_ms(sample_time);
	Rgena = amount;   //�����ɫ����      
	amount=0;
//----------------------------------
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Ggena = amount;	 //�����ɫ����
	amount=0;
//----------------------------------
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Bgena = amount;	 //�����ɫ����
	amount=0;

}  

//����ɫ
u16 tcs3200_RED(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Ramount=(u32) amount*255/Rgena;	     //ȡRֵ
	if(Ramount>=255) Ramount = 254;
	return Ramount;

}

//����ɫ
u16 tcs3200_GREEN(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Gamount=(u32) amount*255/Ggena;	    //ȡGֵ
	if(Gamount>=255) Gamount = 254;
	return Gamount;

	

}

//����ɫ
u16 tcs3200_BLUE(void)
{

	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Bamount=(u32) amount*255/Bgena;    //ȡBֵ
	GPIO_SetBits(GPIOA,GPIO_Pin_7);    
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //�ر�ͨ��
	if(Bamount>=255) Bamount = 254;
	return Bamount;

} 

void EXTI4_15_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line6)!= RESET ) 
	{		
	
		EXTI_ClearITPendingBit(EXTI_Line6);      //�½����жϼ�������
		amount++;
	}		
	
	
}	

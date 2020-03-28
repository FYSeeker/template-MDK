
#include "stm32f0xx.h"
#include "color.h"
#include "system/delay/delay.h"

u16 Rgena=0X00;    //白平衡R比例因子
u16 Ggena=0X00;    //白平衡G比例因子
u16 Bgena=0X00;    //白平衡B比例因子
u16 Ramount=0X00;
u16 Gamount=0x00;
u16 Bamount=0x00;
u16 amount;        //计数器
int sample_time=10;    //采样时间
void tcs3200_init()
	
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);//使能晚外设 PA 时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);//使能晚外设 PB 时钟
	//S2引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//S3引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉
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
	amount=0;			 //开始计数
	delay_ms(sample_time);
	Rgena = amount;   //求出红色因子      
	amount=0;
//----------------------------------
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Ggena = amount;	 //求出绿色因子
	amount=0;
//----------------------------------
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Bgena = amount;	 //求出蓝色因子
	amount=0;

}  

//读红色
u16 tcs3200_RED(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Ramount=(u32) amount*255/Rgena;	     //取R值
	if(Ramount>=255) Ramount = 254;
	return Ramount;

}

//读绿色
u16 tcs3200_GREEN(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Gamount=(u32) amount*255/Ggena;	    //取G值
	if(Gamount>=255) Gamount = 254;
	return Gamount;

	

}

//读蓝色
u16 tcs3200_BLUE(void)
{

	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
	amount=0;
	delay_ms(sample_time);
	Bamount=(u32) amount*255/Bgena;    //取B值
	GPIO_SetBits(GPIOA,GPIO_Pin_7);    
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //关闭通道
	if(Bamount>=255) Bamount = 254;
	return Bamount;

} 

void EXTI4_15_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line6)!= RESET ) 
	{		
	
		EXTI_ClearITPendingBit(EXTI_Line6);      //下降沿中断计算脉冲
		amount++;
	}		
	
	
}	

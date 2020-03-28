#include "tim/TIM.h"
#include "stm32f0xx.h"
#include <stdlib.h>
#include "color/color.h"
#include "protocol/UsarDataDIS.h"

extern u16 R;   
extern u16 G;  
extern u16 B; 

/*
 *函数名：TIM1初始化
 *功能：只用计数器，循环累加，用作接受超时判断
 * */
void TIM1Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = 65534; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =47; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_Cmd(TIM1, DISABLE);  //使能TIMx，开启时钟

}
//初始化超时定时器
void OTstart(void)
{
	TIM_Cmd(TIM1,DISABLE);
	TIM_SetCounter(TIM1,0x0);
	TIM_Cmd(TIM1,ENABLE);
	
}
void OTdisen(void)
{
	TIM_Cmd(TIM1,DISABLE);
	TIM_SetCounter(TIM1,0x0);
}


//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M  
//arr：自动重装值。
//psc：时钟预分频数
//**TIM4
void TIM14_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE ); //使能指定的TIM14中断,允许更新中断

	NVIC_InitStruct.NVIC_IRQChannel = TIM14_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
  TIM_Cmd(TIM14, ENABLE);  //使能TIMx，开启时钟				
}

//定时器3中断服务程序  
void TIM14_IRQHandler(void)   //TIM3中断
{
	
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
	  
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update  );  //清除TIMx更新中断标志 
			
		if((R>100)&&(R<140)&&(G>120)&&(G<200)&&(B>150)&&(B<200))
		MotoFlashEEPROM[color_read_vaule]=0x01;//1色
		else if((R>100)&&(R<200)&&(G>60)&&(G<120)&&(B>90)&&(B<130))
		MotoFlashEEPROM[color_read_vaule]=0x02;//2色
		else if((R>200)&&(R<255)&&(G>150)&&(G<200)&&(B>60)&&(B<120))
		MotoFlashEEPROM[color_read_vaule]=0x03;//3色
		else if((R>230)&&(R<255)&&(G>230)&&(G<255)&&(B>230)&&(B<255))
		MotoFlashEEPROM[color_read_vaule]=0x04;//4色
		else if((R>150)&&(R<230)&&(G>30)&&(G<90)&&(B>30)&&(B<90))
		MotoFlashEEPROM[color_read_vaule]=0x05;//5色
		else if((R>85)&&(R<130)&&(G>90)&&(G<150)&&(B>30)&&(B<90))
		MotoFlashEEPROM[color_read_vaule]=0x06;//6色
		else if((R>30)&&(R<85)&&(G>30)&&(G<90)&&(B>60)&&(B<120))
		MotoFlashEEPROM[color_read_vaule]=0x07;//7色
		else if((R>0)&&(R<60)&&(G>00)&&(G<60)&&(B>00)&&(B<60))
		MotoFlashEEPROM[color_read_vaule]=0x08;//8色
		else MotoFlashEEPROM[color_read_vaule]=0x00;   //无对应颜色返回00
		}
}

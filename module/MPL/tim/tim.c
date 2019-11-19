#include "tim/tim.h"

//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void Tim2IntInit(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器
}


static void (*Tim2Irq)(void) = NULL;

void Tim2Irq_Set(void (*func)(void))
{
  Tim2Irq = func;
}

//定时器中断服务函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
    
    if(Tim2Irq != NULL)
      Tim2Irq();
	}
}


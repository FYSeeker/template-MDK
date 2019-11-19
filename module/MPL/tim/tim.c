#include "tim/tim.h"

//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void Tim2IntInit(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIMʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ�������ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��
}


static void (*Tim2Irq)(void) = NULL;

void Tim2Irq_Set(void (*func)(void))
{
  Tim2Irq = func;
}

//��ʱ���жϷ�����
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
    
    if(Tim2Irq != NULL)
      Tim2Irq();
	}
}


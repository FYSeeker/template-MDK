#include "tim/TIM.h"
#include "stm32f0xx.h"
#include <stdlib.h>
#include "crash/crash.h"
#include "protocol/UsarDataDIS.h"


/*
 *��������TIM1��ʼ��
 *���ܣ�ֻ�ü�������ѭ���ۼӣ��������ܳ�ʱ�ж�
 * */


void TIM1Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 65534; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =47; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_Cmd(TIM1, DISABLE);  //ʹ��TIMx������ʱ��

}
//��ʼ����ʱ��ʱ��
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


//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M  
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//**TIM4
void TIM14_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM14�ж�,��������ж�

	NVIC_InitStruct.NVIC_IRQChannel = TIM14_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
  TIM_Cmd(TIM14, ENABLE);  //ʹ��TIMx������ʱ��				
}

//��ʱ��3�жϷ������  
void TIM14_IRQHandler(void)   //TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM14, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		  MotoFlashEEPROM[crash_vaule]=CrashStatusGet(); 
		}
}

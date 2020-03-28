#include "TIM.h"
#include "stm32f0xx.h"
#include <stdlib.h>
#include "fan/fan.h"


extern u8 MotoFlashEEPROM[0XFF];
extern u8 MotoFlashREG[0xff];






/*
 *��������TIM1��ʼ��
 *�����ˣ�����
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






TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;



/**
  * @brief  Configure the TIM1 Pins.
  * @param  ����pwm����
  * @retval None
  */
void TIM_PIN_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Clocks enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);//ʹ��IOʱ��Դ
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);//�������Ź���ΪPWM����   
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);//�������Ź���ΪPWM����  

}



void TIM3_PWM_Config(void)
{
  TIM_PIN_Config();

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);//���ö�ʱ��14ʱ��Դ
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 2400;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Channel 1, 2, 3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OCInitStructure.TIM_Pulse = 2400/2;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
  TIM_OCInitStructure.TIM_Pulse = 2400/2;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM3, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_SetCompare1(TIM3,0); 
  TIM_SetCompare2(TIM3,0);  

}


//��ʱ��3�жϷ������  
void TIM14_IRQHandler(void)   //TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM14, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	    Run_Moto(MotoFlashEEPROM[fan_vaule]);
		}
}

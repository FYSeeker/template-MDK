#include "infrared/infrared.h"
#include "stm32f0xx.h"



void InfraredInit(void)
{
	
  GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(Sensor_RCC  , ENABLE);//ʹ�������� PA ʱ��
  
  GPIO_InitStructure.GPIO_Pin = Sensor_PIN ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  //������Ҳ������
	GPIO_Init(Sensor_GPIO, &GPIO_InitStructure);	        //��ȡ��ƽ
}


u8 InfraredStatusGet(void)
{
  	if(GPIO_ReadInputDataBit(Sensor_GPIO, Sensor_PIN)==SET)          
       return 1;
     else return 0;
}


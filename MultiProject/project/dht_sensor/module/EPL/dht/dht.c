#include "stm32f0xx.h"
#include "dht.h"
#include "system/delay/delay.h"

  
long long val=0;
int timeout;
	

void Dht_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	
  RCC_AHBPeriphClockCmd(Dht_RCC , ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = Dht_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;   //推挽输出
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; //无上下拉
	GPIO_Init(Dht_GPIO, &GPIO_InitStructure);

  GPIO_SetBits(Dht_GPIO, Dht_PIN);
}


void mode_input(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = Dht_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP; //上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void mode_output(void )
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = Dht_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;   //推挽输出
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; //无上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned int Dht11_read(void)
{
  
  u8 i=0;
  Dht_L();
  delay_us(18000);  //pulldown  for 18ms
  Dht_H();
  delay_us(100);	//pullup for 100us
  mode_input();

  //等待拉高80us
  timeout = 5000;
  while( (! GPIO_ReadInputDataBit  (Dht_GPIO, Dht_PIN)) && (timeout > 0) ) timeout--;	 //wait HIGH

  //等待拉低80us
  timeout = 5000;
  while( GPIO_ReadInputDataBit (Dht_GPIO, Dht_PIN) && (timeout > 0) ) timeout-- ;	 //wait LOW
	
  for(i=0;i<40;i++)
  {
	 timeout = 5000;
	 while( (! GPIO_ReadInputDataBit  (Dht_GPIO, Dht_PIN)) && (timeout > 0) ) timeout--;	 //wait HIGH
	 delay_us(CHECK_TIME);
	 if ( GPIO_ReadInputDataBit (Dht_GPIO, Dht_PIN) )   val=(val<<1)+1;
    else val<<=1;
	 timeout = 5000;
	 while( GPIO_ReadInputDataBit (Dht_GPIO, Dht_PIN) && (timeout > 0) ) timeout-- ;	 //wait LOW
  }

  mode_output();
  Dht_H(); 

  if (((val>>32)+(val>>24)+(val>>16)+(val>>8) -val ) & 0xff  ) return 0; //
    else return val>>8; 

}

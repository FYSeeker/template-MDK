
#include "stm32f0xx.h"
#include "system/delay/delay.h"
#include "string.h"
#include "color/color.h"
#include "tim/TIM.h"
#include "uart/usart.h"
#include "stmflash/stmflash.h"
#include "protocol/communication.h"
#include "protocol/UsarDataDIS.h"


u16 R=0x00,G=0x00,B=0x00;    //RGBֵ

int main()
{
  delay_init(); //��ʱ��ʼ��
  
  USART1_Init(1000000);
  KEY_EXTI_Init();  //�����ⲿ�ж�
	tcs3200_init();   //���������ų�ʼ��
	whitebalance();   //�����Զ�����һ�ΰ�ƽ��	 
  cbInit(&rxbuf,rx_buf_len,rxbuf_st);
  STMFLASH_Read(MotoFlashHead,(u16 *)MotoFlashEEPROM,15);   
  if((MotoId == 0xff) || (MotoId == 0x00))
    MotoFlashEEPROMinit(); // ��ʼ��MotoFlashEEPROMinit
  memcpy(MotoFlashREG,MotoFlashEEPROM,200);
  TIM14_Int_Init(999,47);        	        
  TIM1Init();
  
  while(1)
  {
    DataDis();
		 R = tcs3200_RED();
	   G = tcs3200_GREEN();	
     B = tcs3200_BLUE();	
  }
}


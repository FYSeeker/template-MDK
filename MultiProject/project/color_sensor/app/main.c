
#include "stm32f0xx.h"
#include "system/delay/delay.h"
#include "string.h"
#include "color/color.h"
#include "tim/TIM.h"
#include "uart/usart.h"
#include "stmflash/stmflash.h"
#include "protocol/communication.h"
#include "protocol/UsarDataDIS.h"


u16 R=0x00,G=0x00,B=0x00;    //RGB值

int main()
{
  delay_init(); //延时初始化
  
  USART1_Init(1000000);
  KEY_EXTI_Init();  //启用外部中断
	tcs3200_init();   //传感器引脚初始化
	whitebalance();   //开机自动进行一次白平衡	 
  cbInit(&rxbuf,rx_buf_len,rxbuf_st);
  STMFLASH_Read(MotoFlashHead,(u16 *)MotoFlashEEPROM,15);   
  if((MotoId == 0xff) || (MotoId == 0x00))
    MotoFlashEEPROMinit(); // 初始化MotoFlashEEPROMinit
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


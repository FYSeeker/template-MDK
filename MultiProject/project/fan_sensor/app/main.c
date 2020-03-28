
#include "stm32f0xx.h"
#include "system/delay/delay.h"
#include "string.h"
#include "fan/fan.h"
#include "tim/TIM.h"
#include "uart/usart.h"
#include "stmflash/stmflash.h"
#include "protocol/communication.h"
#include "protocol/UsarDataDIS.h"



int main()
{
  delay_init(); //延时初始化
  
  USART1_Init(1000000);
  cbInit(&rxbuf,rx_buf_len,rxbuf_st);
  STMFLASH_Read(MotoFlashHead,(u16 *)MotoFlashEEPROM,15);      
	TIM3_PWM_Config();	 
  if((MotoId == 0xff) || (MotoId == 0x00))
    MotoFlashEEPROMinit(); // 初始化MotoFlashEEPROMinit
  memcpy(MotoFlashREG,MotoFlashEEPROM,200);
  
  TIM14_Int_Init(999,47);        	        
  TIM1Init();
  
  while(1)
  {
    DataDis();
  }
}


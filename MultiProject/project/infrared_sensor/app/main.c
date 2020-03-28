
#include "stm32f0xx.h"
#include "system/delay/delay.h"
#include "string.h"
#include "infrared/infrared.h"
#include "tim/TIM.h"
#include "uart/usart.h"
#include "stmflash/stmflash.h"
#include "protocol/communication.h"
#include "protocol/UsarDataDIS.h"



int main()
{
  delay_init(); //��ʱ��ʼ��
  
  USART1_Init(1000000);
  InfraredInit();
  
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
  }
}


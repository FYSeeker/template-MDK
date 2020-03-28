
#include "stm32f0xx.h"
#include "system/delay/delay.h"
#include "string.h"
#include "dht/dht.h"
#include "tim/TIM.h"
#include "uart/usart.h"
#include "stmflash/stmflash.h"
#include "protocol/communication.h"
#include "protocol/UsarDataDIS.h"

extern u32 dht_value;

int main()
{
 delay_init();
	 USART1_Init(1000000);
	 cbInit(&rxbuf,rx_buf_len,rxbuf_st);
	 STMFLASH_Read(MotoFlashHead,(u16 *)MotoFlashEEPROM,15);      
   TIM14_Int_Init(14999,4799);        //1.5s 	        
	 TIM1Init();
   Dht_Init();
	 if ((MotoId == 0xff) || (MotoId == 0x00))
	    MotoFlashEEPROMinit();               //初始化MotoFlashEEPROMinit
	 memcpy(MotoFlashREG,MotoFlashEEPROM,200);
   dht_value=Dht11_read();
	 
	 MotoFlashEEPROM[dht_vaule]=(u8) ((dht_value&(0x0000FF00))>>8);        //温度整数部分
	 MotoFlashEEPROM[dht_vaule+1]=(u8) (dht_value&(0x000000FF));           //温度小数部分
	 MotoFlashEEPROM[dht_vaule+2]=(u8) ((dht_value&(0xFF000000))>>24);     //湿度整数部分
	 MotoFlashEEPROM[dht_vaule+3]=(u8) ((dht_value&(0x00FF0000))>>16);     //温度小数部分
   while(1)
  	{
				
		
	  	DataDis();//串口数据处理

	  }
}


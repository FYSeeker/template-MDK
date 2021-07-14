/*************************************************************************
    > File Name: UsarDataDIS.h
    > Author: WS
		> Mail:  
    > Created Time: 2018年05月04日 星期五 10时22分58秒
 ************************************************************************/

#ifndef _UsarDataDIS_H
#define _UsarDataDIS_H
#include "stm32f0xx.h"

#define BulkReadTIMLongOut  TIM1->CNT>50000

extern u8 MotoFlashEEPROM[0X66];
extern u8 MotoFlashREG[0X66];

void MotoFlashEEPROMinit(void);
void usart_send(u8 *usart1send_my_buff,u8 len_usart1_send);

void Read(u8 ReadFlashAddrs,u8 ReadFlashLen);
void DataDis(void);
void SyncWrite(u8 datalen);
void BulkRead(u8 *BulkReadData,u8 datalen);
void Ping(void);
u8 DataSumCheck(u8 dataID,u8 datalen,u8 datacmd,u8 *databuffwill);
#endif


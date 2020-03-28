/*************************************************************************
    > File Name: UsarDataDIS.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2018年05月04日 星期五 10时21分17秒
 ************************************************************************/

#include "UsarDataDIS.h"
#include "string.h"
#include "stm32f0xx_usart.h"	
#include "tim/TIM.h"
#include "stmflash/stmflash.h"
#include "communication.h"

u8 MotoDisCtp=0;
extern u8 MotoRxBuff[0xff];
extern u8 MotoRXCtp;
u8 Radioflag = 0;
u8 MotoDataBuff[0xff]={
	0x00,  //0 
	0x00,  //1
	0x00,  //2
	0x00,  //3  软件版本H
	0x00,  //4  软件版本L
	0x00,  //5  ID
	0x00,  //6  波特率
	0x00,  //7  返回延迟时间 	
	0x00,  //8  应答状态级别
	0x00,  //9	最小角度限制（H）
	0x00,  //10 最小角度限制（L）
	0x00,  //11 最大角度限制（H）
	0x00,  //12 最大角度限制（L）
	0x00,  //13 温度上限
	0x00,  //14 电压上限 
	0x00,  //15 电压下线
	0x00,  //16 最大扭矩（H）
	0x00,  //17 最大扭矩（L）
	0x00,  //18 速度调整
	0x00,  //19	卸载条件	
	0x00,  //20 中位调整(H)
	0x00,  //21 中位调整(L)
	0x00,  //22 电流阀值(H)
	0x00,  //23 电流阀值(L)
	0x00,  //24 最小PWM（H）
	0x00,  //25 最小PWM（L）
	0x00,  //26 顺时针不灵敏区
	0x00,  //27 逆时针不灵敏区	
	0x00,  //28 LED警报
	0x00,  //29	撤销扭矩警报	
	0x00,  //30 
	0x00,  //31
	0x00,  //32
	0x00,  //33 
	0x00,  //34 
	0x00,  //35 
	0x00,  //36 
	0x00,  //37 
	0x00,  //38 
	0x00,  //39		
	0x00,  //40 扭矩开关 
	0x00,  //41 LED
	0x00,  //42 目标位置（H）
	0x00,  //43 目标位置（L）
	0x00,  //44 运行时间（H）
	0x00,  //45 运行时间（L）
	0x00,  //46 当前电流(H)
	0x00,  //47 当前电流(L)
	0x00,  //48 锁标志 
	0x00,  //49	
	0x00,  //50 
	0x00,  //51
	0x00,  //52
	0x00,  //53
	0x00,  //54 
	0x00,  //55 
	0x00,  //56 当前位置（H）
	0x00,  //57 当前位置（L）
	0x00,  //58 运行速度（H） 
	0x00,  //59 运行速度（L）
	0x00,  //60 当前负载 (H)
	0x00,  //61 当前负载(L)
	0x00,  //62 当前电压 
	0x00,  //63 当前温度
	0x00,  //64 REG WRITE标志
	
};
//程序没有对EEPROM Area和RAM Area作区分，而是共用一个表
//MotoFlashREG作为暂存表，MotoFlashEEPROM作为FLASH映射表
__align(2) u8 MotoFlashEEPROM[0x66]={0};
u8 MotoFlashREG[0x66] = {0};
u8 RegWIRflag=0;
/*
 * 函数名：读取环形缓冲区值
 * 返回：缓冲区的值
 * */

//u8 BuffRead(void)
//{
//	u8 test;
//	test = MotoRxBuff[MotoDisCtp];
//	++MotoDisCtp;
//	return test;
//}

void MotoFlashEEPROMinit(void)
{
	
	 MotoFlashEEPROM[0] = 0x0c;//0x1d moto   0x0c sensor
	 MotoFlashEEPROM[1] = 0x00;
	 MotoFlashEEPROM[2] = 0x26;
   MotoFlashEEPROM[3] = sensor_id;//ID
   
	 
		MotoFlashEEPROM[4] = 0x01;
		MotoFlashEEPROM[5] = 0x00;
		MotoFlashEEPROM[6] = 0x00;
	 
		MotoFlashEEPROM[7] = 0x00;
		MotoFlashEEPROM[8] = 0xff;
		MotoFlashEEPROM[9] = 0x0f;
		MotoFlashEEPROM[10] = 0x00;
		
		MotoFlashEEPROM[11] = 0x50;
		MotoFlashEEPROM[12] = 0x3c;
		MotoFlashEEPROM[13] = 0xa0;
		MotoFlashEEPROM[14] = 0xff;
		
		MotoFlashEEPROM[15] = 0x03;
		MotoFlashEEPROM[16] = 0x01;
		MotoFlashEEPROM[17] = 0x24;
		MotoFlashEEPROM[18] = 0x24;
		
		MotoFlashEEPROM[19] = 0x00;
		MotoFlashEEPROM[20] = 0x00;
		MotoFlashEEPROM[21] = 0x00;
		MotoFlashEEPROM[22] = 0x00;
		
		MotoFlashEEPROM[23] = 0x00;
		MotoFlashEEPROM[24] = 0x01;
		MotoFlashEEPROM[25] = 0x00;
		MotoFlashEEPROM[26] = 0x00;
		
		MotoFlashEEPROM[27] = 0x00;
		MotoFlashEEPROM[28] = 0x00;
		MotoFlashEEPROM[29] = 0x00;
		MotoFlashEEPROM[30] = 0x00;
		
		MotoFlashEEPROM[31] = 0x00;
		MotoFlashEEPROM[32] = 0x00;
		MotoFlashEEPROM[33] = 0x00;
		MotoFlashEEPROM[34] = 0xff;
		
		MotoFlashEEPROM[35] = 0x03;
		MotoFlashEEPROM[36] = 0xe3;
		MotoFlashEEPROM[37] = 0x0f;
		MotoFlashEEPROM[38] = 0x00;
		
		MotoFlashEEPROM[39] = 0x00;
		MotoFlashEEPROM[40] = 0x00;
		MotoFlashEEPROM[41] = 0x00;
		MotoFlashEEPROM[42] = 0x4f;
		
		MotoFlashEEPROM[43] = 0x20;
		MotoFlashEEPROM[44] = 0x00;
		MotoFlashEEPROM[45] = 0x00;
		MotoFlashEEPROM[46] = 0x00;
		
		MotoFlashEEPROM[47] = 0x00;
		MotoFlashEEPROM[48] = 0x00;
		MotoFlashEEPROM[49] = 0x00;
		MotoFlashEEPROM[50] = 0x00;
		
		MotoFlashEEPROM[51] = 0x00;
		MotoFlashEEPROM[52] = 0xe2;
		MotoFlashEEPROM[53] = 0x0f;
		MotoFlashEEPROM[54] = 0xff;
		
		MotoFlashEEPROM[55] = 0x03;
		MotoFlashEEPROM[56] = 0x00;
		MotoFlashEEPROM[57] = 0x00;
		MotoFlashEEPROM[58] = 0x00;
		
		MotoFlashEEPROM[59] = 0x00;
		MotoFlashEEPROM[60] = 0x00;
		MotoFlashEEPROM[61] = 0x00;
		MotoFlashEEPROM[62] = 0x00;
		MotoFlashEEPROM[63] = 0x00;
		MotoFlashEEPROM[64] = 0x00;
		MotoFlashEEPROM[65] = 0x00;
		MotoFlashEEPROM[66] = 0x00;
		MotoFlashEEPROM[67] = 0x00;
		MotoFlashEEPROM[68] = 0x00;
		MotoFlashEEPROM[69] = 0x00;
		MotoFlashEEPROM[70] = 0x00;
		MotoFlashEEPROM[71] = 0x00;
		MotoFlashEEPROM[72] = 0x00;
		MotoFlashEEPROM[73] = 0x00;
		
}

/*
 * 函数名：ping操作
 * */
void Ping()
{
	u8 PingSendBuff[]={0xff,0xff,0xff,0x02,0x00,0x00};
	PingSendBuff[2] = MotoId;
	PingSendBuff[5] = MotoId+0x02+0x00;
	PingSendBuff[5] =~PingSendBuff[5];
	usart_send(PingSendBuff,6);
}

/*
 *函数名：写操作
 * */
void Write(u8 WriteFlashAddrs,u8 *WriteBuff,u8 WriteFlashLEN)
{
	u8 ctp;

	for(ctp=0;ctp<WriteFlashLEN-1;ctp++)
	{
		MotoFlashEEPROM[WriteFlashAddrs+ctp]=WriteBuff[ctp];
	}
	if(WriteFlashAddrs<23)	//EEPROM Area
	{
		STMFLASH_Write(MotoFlashHead,(u16 *)MotoFlashEEPROM,11);
	}


	memcpy(MotoFlashREG,MotoFlashEEPROM,0xff);
	MotoDisCtp = MotoRXCtp;
}
/*
 *函数名：读操作
 * */
void Read(u8 ReadFlashAddrs,u8 ReadFlashLen)
{
	u8 ReadSendBuff[100]={0xff,0xff,0x00,0x04,0x00,0x00,0x00,0x00};
	u8 i,SUM=0,SendBuffCount = 2;
	
//	if(ReadFlashAddrs<0x20)
//	{
//	STMFLASH_Read(MotoFlashHead,(u16 *)MotoFlashEEPROM,15);
//	}
	ReadSendBuff[SendBuffCount++] = MotoId;							//ReadSendBuff[2]
	ReadSendBuff[SendBuffCount++] = ReadFlashLen +2;
	ReadSendBuff[SendBuffCount++] = 0x00;								//ERR
	for (i=0;i<ReadFlashLen;i++)
	ReadSendBuff[SendBuffCount++] = MotoFlashEEPROM[ReadFlashAddrs+i];
	for (i=2;i<SendBuffCount;i++)
	SUM += ReadSendBuff[i];
	ReadSendBuff[SendBuffCount++] =~SUM;
	usart_send(ReadSendBuff,SendBuffCount);
	MotoDisCtp = MotoRXCtp;
	
}

/*
 *函数名：异步写
 * */
void REGwri(u8 RegWriFlashAddrs,u8 RegWrilenFLASH)
{
	u8 i=0;
	u8 PingSendBuff[]={0xff,0xff,0X00,0x02,0x00,0x00};
	
	PingSendBuff[2]=MotoId;
	 
	PingSendBuff[5]= ~(PingSendBuff[2]+PingSendBuff[3]+PingSendBuff[4]);
	
	for(i = 0;i<RegWrilenFLASH;i++)
	{
		MotoFlashREG[RegWriFlashAddrs+i]=MotoDataBuff[i+1];
	}

	RegWIRflag = 1;
	MotoFlashREG[Registered]=1;
	
	usart_send(PingSendBuff,6);
	
}
/*
 *函数名：执行异步写
 * */
void action()
{
	RegWIRflag = 0;
	MotoFlashREG[Registered]=0;
	memcpy(MotoFlashEEPROM,MotoFlashREG,0xff);
	MotoDisCtp = MotoRXCtp;
}
/*
 *函数名：多个ID控制
  只有当要写入的控制表的地址和长度相同时，才能使用SYNC WRITE命令。另外，ID应该以广播ID的形式发送。
  Length (L+1) X N + 4   (L: Data Length per RX-64, N: the number of RX-64s)
  注意数据包要结合缓冲区长度，不能过长
 * */
void SyncWrite(u8 datalen)
{
	u8 statraddr,writelen,i,j,id_num,startid;
	
	
	statraddr=MotoDataBuff[0];
	writelen=MotoDataBuff[1];
	startid=MotoDataBuff[2];
	id_num=(datalen-2)/(writelen+1);
	
	for(i=0;i<id_num;i++)
	{
		if(MotoDataBuff[2+i*(writelen+1)]==MotoId)
		{
				for(j = 0;j<writelen;j++)
				{
					MotoFlashREG[statraddr+j]=MotoDataBuff[2+1+j+i*(writelen+1)];
				}
        break;				
		}
	}
}


/*
 * 函数名：BulkRead数据监听
 * */

u8 BulkReadListening(u8 id)
{
	u8 BulkReadDataTest;
	
  if(id==0)
		return 1;
	
	OTstart();
  while(1)
	{
		 if(cbCount(&rxbuf)>3)
		 {
			 cbRead(&rxbuf,&BulkReadDataTest);
			 if(BulkReadDataTest!=0xFF)
			  continue;
			 
			 cbRead(&rxbuf,&BulkReadDataTest);
			 if(BulkReadDataTest!=0xFF)
			  continue;
			 
			 cbRead(&rxbuf,&BulkReadDataTest);
			 if(BulkReadDataTest==id)
				 return 1;
			 
			 OTstart();
		 }
		else if(BulkReadTIMLongOut) // timeout
    {
			OTdisen();
      break;
    }		 
	}
	return 0;
}

/*
 * 函数名：BulkRead
 * */
void BulkRead(u8 *BulkReadData,u8 datalen)
{
  u8 param1[3] = {0};
  u8 param2[3] = {0};
  u8 BulkReadStatus = 0;
	u8 BulkReadSendBuff[48]={0};
	u8 checksum=0;
	u8 i=0;
	
	BulkReadSendBuff[0]=0xFF;
  BulkReadSendBuff[1]=0xFF;
	BulkReadSendBuff[2]=MotoId;
	
  for(i=0;i<datalen;i+=3)
  {
    if(BulkReadData[i+1] == MotoId)
    {
      param2[0] = BulkReadData[i];
      param2[1] = BulkReadData[i+1];
      param2[2] = BulkReadData[i+2];
      BulkReadStatus = 1;
      break;
    }
    param1[0] = BulkReadData[i];
    param1[1] = BulkReadData[i+1];
    param1[2] = BulkReadData[i+2];
  }
	
	BulkReadSendBuff[3]=param2[0]+2;
	if(BulkReadStatus == 1)
  {
		if(BulkReadListening(param1[1]) == 1)
    {
			  for(i = 0;i<param2[0];i++)
				{
					BulkReadSendBuff[5+i]=MotoFlashEEPROM[param2[2]+i];		
				}
				
				for(i = 2;i<param2[0]+5;i++)
				{
					checksum+=BulkReadSendBuff[i];
				}
				
			BulkReadSendBuff[5+param2[0]]=~checksum;
      usart_send(BulkReadSendBuff,6+param2[0]);
    }
	}
	
}




/*
 * 函数名：自算校验和
 * */
u8 DataSumCheck(u8 dataID,u8 datalen,u8 datacmd,u8 *databuffwill)
{
	u8 temp,ctp;
	temp = dataID+datalen+2+datacmd;
	for(ctp = 0;ctp<datalen;ctp++)
	{
		temp+=databuffwill[ctp];
	}
	temp=~temp;
	return temp;
}

/*
MX-28
http://emanual.robotis.com/docs/en/dxl/protocol1/#protocol
*/

void DataDis(void)
{
	u8 MotoIDtest;
	u8 MotoLenTest;
	u8 MotoLenCtpTest;
	u8 MotoDataTest;
	u8 MotoDataCmdTest;
	u8 MotoDataSumCh;
  MotoFlashEEPROM[73] = 0x00;
	if(cbCount(&rxbuf)>4)
	{
		cbRead(&rxbuf,&MotoDataTest);
		if(MotoDataTest!=0xff)
		{
			return;
		}
		cbRead(&rxbuf,&MotoDataTest);
		if(MotoDataTest!=0xff)
		{
			return;
		}
		cbRead(&rxbuf,&MotoIDtest);
		if(MotoIDtest!=MotoId)
		{
			if(MotoIDtest!=0xfe)
			{
				cbInit(&rxbuf,rx_buf_len,rxbuf_st);
				return;
			}
		}
		cbRead(&rxbuf,&MotoLenTest);//Length LEN
		MotoLenTest-=2;							//LEN-1(INST)-1(CKSM)->参数个数
		cbRead(&rxbuf,&MotoDataCmdTest);//CMD INST
		OTstart();
		while(cbCount(&rxbuf)<MotoLenTest+1)
		{
			if(TIMLongOut)
			{
				cbInit(&rxbuf,rx_buf_len,rxbuf_st);
				OTdisen();
				return;
			}
		}
		
		for(MotoLenCtpTest=0;MotoLenCtpTest<MotoLenTest;++MotoLenCtpTest)//Parameter
		{
			cbRead(&rxbuf,&MotoDataTest);
			MotoDataBuff[MotoLenCtpTest]=MotoDataTest;//数据内容存储到数组MotoDataBuff[]   0,1,2,3...n  -> P1,P2,P3...Pn
		}
		cbRead(&rxbuf,&MotoDataSumCh);
		if(MotoDataSumCh==DataSumCheck(MotoIDtest,MotoLenTest,MotoDataCmdTest,MotoDataBuff))//Checksum
		{
			switch(MotoDataCmdTest)
			{
				case PingCmd:
					Ping();
					break;
				case WriteCmd:
					Write(MotoDataBuff[0],MotoDataBuff+1,MotoLenTest);
					break;
				case ReadCmd:
					Read(MotoDataBuff[0],MotoDataBuff[1]);
					break;
				case RegWRICmd:
					REGwri(MotoDataBuff[0],MotoLenTest-1);
					break;
				case Action:
					action();
				  break;
				case SyncWRCMD:
					SyncWrite(MotoLenTest);
					break;
				case BulkReadCMD:
					BulkRead(MotoDataBuff+1,MotoLenTest-1);
					break;
			}
		}
	}
}


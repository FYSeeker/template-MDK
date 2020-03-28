//=============================================================================
//文件名称：usart1.c
//功能概要：串口1驱动文件
//更新时间：2014-01-04
//=============================================================================
#include "system/sys/sys.h"
#include "uart/usart.h"	
#include "protocol/communication.h"
#include "string.h"


u8 MotoRxBuff[0xff]={0};
u8 MotoRXCtp=0;
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	 
uint32_t baud_init;       //波特率
/* USART初始化 */

extern u8 rxbuf_st[rx_buf_len];//串口缓冲区实际位置
#define USART_BUR_MAX 128

u8 USART1_Rev_Branch_Buffer[USART_BUR_MAX];

void USART1_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	//RX1 DMA
	DMA_DeInit(DMA1_Channel3); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->RDR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART1_Rev_Branch_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USART_BUR_MAX;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority =DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel3,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TE,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Channel3,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel2_3_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}  



  
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = bound;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx| USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
  
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	USART1_DMA_Config();    	
}

void DMA1_Channel3_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC3);
	DMA_ClearITPendingBit(DMA1_IT_TE3);
	DMA_Cmd(DMA1_Channel3, DISABLE);
}

void TXenInit()
{
	
  GPIO_InitTypeDef GPIO_InitStructure;

	USART1->CR1 &= 0xffdb ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
	
	
}
void TXdisInit()
{
	
	
  GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART1->CR1 |= 0x0024 ;                   //使能USART1

}
u8 i=0;
void USART1_IRQHandler(void)
{ 
	
	u16 recv_data_len;
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!= RESET)
	{
		DMA_Cmd(DMA1_Channel3,DISABLE);
		recv_data_len = USART_BUR_MAX - DMA_GetCurrDataCounter(DMA1_Channel3);//读取数据的长度
		if(recv_data_len>0)
		{
			while(DMA_GetITStatus(DMA1_IT_TC3)){};//等待DMA传输完成
				for(i=0;i<recv_data_len;i++)
					cbWrite(&rxbuf, &USART1_Rev_Branch_Buffer[i]);//将数据写入环形缓冲区
		}
		DMA_ClearFlag(DMA1_FLAG_GL3 | DMA1_FLAG_TC3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3); 
		DMA1_Channel3->CNDTR = USART_BUR_MAX;
		DMA_Cmd(DMA1_Channel3,ENABLE);
	}
	if(USART_GetITStatus(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//出错
		USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	DMA_Cmd(DMA1_Channel3,ENABLE);
}


void usart_send(u8 *usart1send_my_buff,u8 len_usart1_send)
{
	u8 usart1send_ctp;
	
	TIM_Cmd(TIM14, DISABLE);
	TIM_ITConfig(TIM14,TIM_IT_Update,DISABLE );
	
	TXenInit();      //使能TX
	delay_us(1);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	for(usart1send_ctp=0;usart1send_ctp<len_usart1_send;usart1send_ctp++)
	{
		USART_SendData(USART1,usart1send_my_buff[usart1send_ctp]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	}
	TXdisInit();     //关闭TX
	cbInit(&rxbuf,rx_buf_len,rxbuf_st);
	TIM_Cmd(TIM14, ENABLE);
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE );
}



/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
#endif


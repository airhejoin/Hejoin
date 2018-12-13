#ifndef __TASKCOM_H
#define __TASKCON_H

//#include"includes.h"



#define MAXMSGSIZE 5
#define MAXMSGBUFFERLEN  80
#define RECMSGTIMEOUT    10

typedef struct
{
	u8 Msg[MAXMSGSIZE][MAXMSGBUFFERLEN];			//!数据缓冲区
	u8 SoleId[6];								//唯一的ID
	u8 MsgBegin;									//!处理一个数据包
	u8 MsgEnd;									//!写入一个数据包
	u8 RecTimeOut;							//!接收数据包超时封包
	u8 RecCount;							//!顺序写入缓冲
}S_COM;

extern S_COM EncryptRec,RS485Rec,RS485Send,GsmSend,GsmRec;

extern INT8U SendAskBuf[MAXMSGBUFFERLEN];
extern INT8U SendNoAskBuf[MAXMSGBUFFERLEN];

//!串口相关信号量
extern xSemaphoreHandle xRS485RecSemaphore,xGsmRecSemaphore;
extern xSemaphoreHandle xRS485SendSemaphore,xGsmSendSemaphore,xTftCtlSemaphore;
extern xQueueHandle     xRS485AskQueue,xGsmAskQueue; 


extern xSemaphoreHandle xEncryptRecSemaphore;

#endif


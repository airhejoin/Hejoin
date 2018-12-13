#ifndef __TASKCOM_H
#define __TASKCON_H

//#include"includes.h"



#define MAXMSGSIZE 5
#define MAXMSGBUFFERLEN  80
#define RECMSGTIMEOUT    10

typedef struct
{
	u8 Msg[MAXMSGSIZE][MAXMSGBUFFERLEN];			//!���ݻ�����
	u8 SoleId[6];								//Ψһ��ID
	u8 MsgBegin;									//!����һ�����ݰ�
	u8 MsgEnd;									//!д��һ�����ݰ�
	u8 RecTimeOut;							//!�������ݰ���ʱ���
	u8 RecCount;							//!˳��д�뻺��
}S_COM;

extern S_COM EncryptRec,RS485Rec,RS485Send,GsmSend,GsmRec;

extern INT8U SendAskBuf[MAXMSGBUFFERLEN];
extern INT8U SendNoAskBuf[MAXMSGBUFFERLEN];

//!��������ź���
extern xSemaphoreHandle xRS485RecSemaphore,xGsmRecSemaphore;
extern xSemaphoreHandle xRS485SendSemaphore,xGsmSendSemaphore,xTftCtlSemaphore;
extern xQueueHandle     xRS485AskQueue,xGsmAskQueue; 


extern xSemaphoreHandle xEncryptRecSemaphore;

#endif


/*
*********************************************************************************
*                                 STM32F103C8T6
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver1.1
*                                   �ʼҿƼ�
*
*ģ  �飺LPB100-SMT
*����ߣ�Andy
*ʱ  �䣺2015-01-07
*********************************************************************************
*/
#ifndef __LPB100SET_H
#define __LPB100SET_H
#include <stdarg.h>
#if 0

#define LPB100RESETIO			GPIO_Pin_4
#define LPB100LINKIO			GPIO_Pin_0
#define LPB100READYIO			GPIO_Pin_5

#define LPB100RESETL			GPIO_ResetBits(GPIOC, LPB100RESETIO)
#define LPB100RESETH			GPIO_SetBits(GPIOC, LPB100RESETIO)

#endif

#define LPB100RESETIO			GPIO_Pin_15
#define LPB100LINKIO			GPIO_Pin_13
#define LPB100READYIO			GPIO_Pin_14

#define LPB100RESETL			GPIO_ResetBits(GPIOC, LPB100RESETIO)
#define LPB100RESETH			GPIO_SetBits(GPIOC, LPB100RESETIO)

#define LPB100_LINK_DATA       GPIO_ReadOutputDataBit(GPIOC,LPB100LINKIO)
#define LPB100_READY_DATA      GPIO_ReadOutputDataBit(GPIOC,LPB100READYIO)


#define RxBufferSize2  256
extern INT8U RxBuffer2[RxBufferSize2];//����2������ʵ��
extern INT8U Rx2Beg,Rx2End;

#define Next_RxBf2(xxx) ++xxx;xxx&=(RxBufferSize2-1); //ָ����һ����ֵ

extern INT8U LenStr(INT8U *ip);
extern INT8U Wait_Answer(INT8U *str);
extern INT16U CompareStr(INT8U *pt1,INT8U size,INT8U *pt2);

extern void A11SMTSengBuffer(INT8U *data);
extern void PrintfA11SMT(char *fmt, ...);

#if 0
#define DoWait(Do,Wait) 	do{\
printf(Do);\
vTaskDelay(100);\
}while(!Wait_Answer(Wait))
#endif



#if 1

#define DoWait(Do,Wait,...) 	do{\
PrintfA11SMT(Do);\
vTaskDelay(200);\
}while(!Wait_Answer(Wait))

#define DoWaitSlow(Do,Wait,...) 	do{\
taskENTER_CRITICAL();\
PrintfA11SMT(Do);\
taskEXIT_CRITICAL();\
vTaskDelay(10000);\
}while(!Wait_Answer(Wait))

#define DoWaitQuicket(Do,Wait) 	do{\
taskENTER_CRITICAL();\
PrintfA11SMT(Do);\
taskEXIT_CRITICAL();\
vTaskDelay(1000);\
}while(!Wait_Answer(Wait))

#endif


typedef struct
{
	INT8U Station;					//!LPB100��״̬
	INT16U WrongCnt;					//!���ϼ�����������λLPB100
	INT8U SendQueueLock;			//!�б仯�ŷ���
	INT8U AllHardReset;				//!ϵͳȫ����λ
}S_LPB100;
extern S_LPB100 LPB100Sta;

enum
{
	LPB100_NORMAL=0,				//!ϵͳ����
	LPB100_HARDWRONG,				//!Ӳ������
	LPB100_APWRONG,					//!APû������
 LPB100_SERVERWRONG,//����������
};

//!��Ҫ���浽��������ò���
typedef struct
{
	INT8U TcpAddr;				/*!����IP��ַ���һλ*/
	INT32U ServerSN;			/*!���������ر��*/
	INT8U SingalPower;			/*!AP���ź�ǿ��*/
}S_PARA;
extern S_PARA SystemPara;

//!���ڽ���
typedef struct
{
	INT8U RxLock;				/*!���յ�ָ���İ�ͷ����ʼ������ݶ�*/
	INT8U RxCnt;				/*!���������ֽڼ���*/
	INT8U RxSetCnt;				/*!��Ҫ���յ����ݰ�����*/
	INT8U HeaderBuffer[3];		/*!��ͷ���ջ��壬����ƥ���ͷ*/
	INT8U HeaderRxCnt;			/*!��ͷ���ռ���*/
 INT8U RxOk;
}S_COMREC;
extern S_COMREC SerDataRec;

#define UART_RxSize     4
#define UART_LENG       64
extern INT8U Uart2MsgBeg;			
extern INT8U Uart2MsgEnd;			
extern INT8U Uart2Msg[UART_RxSize][UART_LENG];

#define LPB100_SEND_SIZE  2
#define LPB100_SEND_LEN   100

extern INT8U Lpb100_SendBuff[LPB100_SEND_SIZE][LPB100_SEND_LEN];
extern INT8U Lpb100_SendStart;
extern INT8U Lpb100_SendEnd;

extern INT8U SeverBuff[LPB100_SEND_SIZE][LPB100_SEND_LEN];
extern INT8U SeverStart;
extern INT8U SeverEnd;

enum
{
	Connect_nomal=0,
	Connect_off,	
};

//!TCP����
typedef struct
{
	INT8U RecMode;
}S_TCPFLAG;
extern S_TCPFLAG TcpFlag;

extern void LPB100Init(void);
extern void LPB100Set(void);
extern INT8S LPB100GetWSSID(void);
extern void LPB100Scan(void);
extern void LPB100Reset(INT32U Reset);
extern xSemaphoreHandle xUart3RXSemaphore;  /*����2�����ж�*/

extern void vLPB100ProcessTask( void *pvParameters );
extern void vLPB100SendSeverTask( void *pvParameters );
extern void Wait_LPB100Ask(INT8U cmd);

extern void SendUsartCom(void);

#endif





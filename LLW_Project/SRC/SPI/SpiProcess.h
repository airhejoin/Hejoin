/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                             ���Ѿ���ӹ�����
*
*ģ  �飺SPI����165��595
*����ߣ�����������
*ʱ  �䣺2014-09-17 21:28:48
*********************************************************************************
*/
#ifndef __SPIPROCESS_H
#define __SPIPROCESS_H

//!595����
#define MAXNUM595     4

//!COM������
#define MAXCOM		  4

//!�ܹ����������
#define LEDTOTAL      8

//!����165����
#define MAX165        4

#define MAXKEYNUMBER  32

#define KEY_DELAY_PRESS			2
#define KEY_DELAY_RELEASE		2

//!SPIƬѡ
#define SPI2SSL			GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define SPI2SSH			GPIO_SetBits(GPIOB,GPIO_Pin_12)

//!8������ܸ��»�����
extern INT8U DisBuff[MAXCOM][MAXNUM595];

typedef struct
{
	INT8U OutPutBuffer[MAXNUM595];
	INT8U InPutBufferISR[MAX165];
	//INT8U InPutBufferOld[MAX165];
	//INT8U InPutBufferNew[MAX165];
}S_SPIDATA;
extern S_SPIDATA GameSpiData;
extern INT8U LedTab[38];

extern void DigitalSPIInit(void);
//!����595���ݣ��ɼ�165����
extern void SendBuffAndGetKeyBuff( INT8U *SendBuff, INT8U* KeyBuff);

//!��ָ��λ����ʾ�����
extern void DisNumberCoor(INT8U zero, INT8U StartLed, INT8U Width, INT32S Num);

//!�ر�ָ��λ���������ʾ
extern void CloseLedCoor(INT8U StartLed, INT8U Width);

//!��ʾ���
extern void DisCleanCoin(void);
extern void UpdateKeyAndDis(void);
#endif


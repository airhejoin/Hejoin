/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                             大脚丫电子工作室
*
*模  块：SPI操作165和595
*设计者：南下三五年
*时  间：2014-09-17 21:28:48
*********************************************************************************
*/
#ifndef __SPIPROCESS_H
#define __SPIPROCESS_H

//!595数量
#define MAXNUM595     4

//!COM段数量
#define MAXCOM		  4

//!总共数码管数量
#define LEDTOTAL      8

//!定义165数量
#define MAX165        4

#define MAXKEYNUMBER  32

#define KEY_DELAY_PRESS			2
#define KEY_DELAY_RELEASE		2

//!SPI片选
#define SPI2SSL			GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define SPI2SSH			GPIO_SetBits(GPIOB,GPIO_Pin_12)

//!8个数码管更新缓冲区
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
//!发送595数据，采集165数据
extern void SendBuffAndGetKeyBuff( INT8U *SendBuff, INT8U* KeyBuff);

//!在指定位置显示数码管
extern void DisNumberCoor(INT8U zero, INT8U StartLed, INT8U Width, INT32S Num);

//!关闭指定位置数码管显示
extern void CloseLedCoor(INT8U StartLed, INT8U Width);

//!显示清币
extern void DisCleanCoin(void);
extern void UpdateKeyAndDis(void);
#endif


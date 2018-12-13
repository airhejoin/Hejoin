/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                             大脚丫电子工作室
*
*模  块：USB通信
*设计者：南下三五年
*时  间：2013-08-29 10:28:48
*********************************************************************************
*/

#ifndef __TASKPCSERVER_H
#define __TASKPCSERVER_H

//!USB使能口线控制
#define USB_DISCONNECT                      GPIOA  
#define USB_DISCONNECT_PIN                  GPIO_Pin_8
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA


//!接收信号量和发送信号量
extern xSemaphoreHandle xUSBRecSemaphore;
extern xSemaphoreHandle xUSBSendSemaphore;

extern xSemaphoreHandle xPCASKSemaphore;

extern xTaskHandle xHandleGameDemo;
extern xQueueHandle  xQueueServerAsk;


//!接收和发送缓冲区
#define USBMsgSize    5
extern INT8U USBTxMsgBeg;			//处理一个包，
extern INT8U USBTxMsgEnd;			//写入一个包，
extern INT8U USBTxMsg[USBMsgSize][65];//数据缓冲区,数组0表示长度

#define RXUSBMsgSize  5

extern INT8U USBRxMsgBeg;			//处理一个包，
extern INT8U USBRxMsgEnd;			//写入一个包，
extern INT8U USBRxMsg[RXUSBMsgSize][65];//数据缓冲区，数组0表示长度


//!命令字
enum
{   
    USB_COINSELLTICK=0x70,			//!售币机发送心跳包
    USB_COINSELLSETCOINOUT,			//!设置售币机的退币数量  现金4字节  数量4字节
    USB_COINSELLSENDSTA,			//!售币机发送状态信息 剩余退币数量4字节 故障1字节
    USB_COINSELLREVERSERUN,			//!售币机反转
    USB_COINSELLSTOP,				//!售币机停止
    USB_COINSELLCONTINUE,			//!售币机续转
    USB_COINCLEAN,					//!清币
    USB_COINSELLOUTREADY,			//!出币完成
};

//!操作刷卡系统
//!------------------------------------------------------------
//!发送M1卡的内部序列号和编号
extern void SendM1CardSNAndNum(void);
extern void SendSetPlayerMoney(void);
extern void SendSetPlayerLottery(void);

//!------------------------------------------------------------


extern void USB_Init(void);
extern void InitUSBIO(void);
extern void vConnectPCTask( void *pvParameters );
extern void vRecMsgPCTask( void *pvParameters );
extern void SendCurrentAllBill(void);
extern void SendServerSysKernel(void);
extern void SendServerRestOK(void);
extern void SendBaoZhangSta(INT8U sta);
extern void SendJieMaSta(INT8U sta);
extern void SendTestBet(void);
extern void SendSystemPara(INT8U sel);
extern void SendPCDateMsg(void);
extern void SendGameSpend(void);

extern void SendTest1(void);
extern void SendTest2(void);

extern void SendNextCodeDate(void);
extern void WaitServerReadyUSB(void);
extern void AskServerImOK(void);


extern void TestSendUSB(void);

extern void SendServerCardSN(void);
extern void SendServerCardMacAddr(void);
extern void SendServerDelMacAddr(void);


extern void SendServerTick(void);
extern void SendServerOutCoinReady(void);










#endif




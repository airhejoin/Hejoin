/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                             ���Ѿ���ӹ�����
*
*ģ  �飺USBͨ��
*����ߣ�����������
*ʱ  �䣺2013-08-29 10:28:48
*********************************************************************************
*/

#ifndef __TASKPCSERVER_H
#define __TASKPCSERVER_H

//!USBʹ�ܿ��߿���
#define USB_DISCONNECT                      GPIOA  
#define USB_DISCONNECT_PIN                  GPIO_Pin_8
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA


//!�����ź����ͷ����ź���
extern xSemaphoreHandle xUSBRecSemaphore;
extern xSemaphoreHandle xUSBSendSemaphore;

extern xSemaphoreHandle xPCASKSemaphore;

extern xTaskHandle xHandleGameDemo;
extern xQueueHandle  xQueueServerAsk;


//!���պͷ��ͻ�����
#define USBMsgSize    5
extern INT8U USBTxMsgBeg;			//����һ������
extern INT8U USBTxMsgEnd;			//д��һ������
extern INT8U USBTxMsg[USBMsgSize][65];//���ݻ�����,����0��ʾ����

#define RXUSBMsgSize  5

extern INT8U USBRxMsgBeg;			//����һ������
extern INT8U USBRxMsgEnd;			//д��һ������
extern INT8U USBRxMsg[RXUSBMsgSize][65];//���ݻ�����������0��ʾ����


//!������
enum
{   
    USB_COINSELLTICK=0x70,			//!�۱һ�����������
    USB_COINSELLSETCOINOUT,			//!�����۱һ����˱�����  �ֽ�4�ֽ�  ����4�ֽ�
    USB_COINSELLSENDSTA,			//!�۱һ�����״̬��Ϣ ʣ���˱�����4�ֽ� ����1�ֽ�
    USB_COINSELLREVERSERUN,			//!�۱һ���ת
    USB_COINSELLSTOP,				//!�۱һ�ֹͣ
    USB_COINSELLCONTINUE,			//!�۱һ���ת
    USB_COINCLEAN,					//!���
    USB_COINSELLOUTREADY,			//!�������
};

//!����ˢ��ϵͳ
//!------------------------------------------------------------
//!����M1�����ڲ����кźͱ��
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




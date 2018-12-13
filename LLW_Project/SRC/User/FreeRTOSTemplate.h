#ifndef _FREERTOSTEMPLATE_H_
#define _FREERTOSTEMPLATE_H_

#include "stm32f10x.h"

//ϵͳ����

#define MAIN_FIRST 181005
#define APP_VERSOIN 105


//���е�
#define RunLedOn()  GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define RunLedOff() GPIO_SetBits(GPIOD,GPIO_Pin_12)

//״̬��
#define StaLedOn()  GPIO_ResetBits(GPIOD, GPIO_Pin_11)
#define StaLedOff() GPIO_SetBits(GPIOD,GPIO_Pin_11)

typedef struct{
  u8  ParaStatus;       //����״̬ 0 Ϊδ��ʼ����1ΪĬ�����ã�2Ϊ���ú�
  u16 HighValue;        //�ߵ�ƽ��ѹmv
  u8  WorkMode;         //����ģʽ 0��ͨ 1��һ�ο� �ڶ��ι�
  u8  TriggerLevel;     //������ƽ
  u16 TriggerHoldTimes; //��ƽ����ʱ�� ms ���60S
  u32 NextTriggerDelay; //���δ���ʱ����������������1�꣬��λ��
  
  u8 OutFixStatus;      //����İ�״̬ 
}S_CHKIN;

typedef struct{
  u8  FixInId;          //�󶨵�����
  u8  OutLevel;         //�����ƽ
  u32 OutHoldTimes;     //��Ч���ʱ�䣬��λ0.1��
}S_CTLOUT;


typedef struct
{
  u8 InUseSta[8];       //8�������ʹ��״̬
  u8 OutUseSta[8];      //8�������ʹ��״̬
  u32 SetOutDelay[8];   //�����������ʱ����
}S_CTL;

typedef struct
{
  S_CHKIN Insert[8];    //8������
  S_CTLOUT CtlOut[8];   //8��������
  u32 RunFirst;				  //4byte��һ������
  u32 RamdomSeed;			  //4byte
}S_SYSTEM;              //
extern S_SYSTEM System;


typedef struct
{
	u8 ServerOn;				//��ģ���ϵ��־
	u8 ServerStatus;		//��ģ��״̬
	u8 ClientStatus;		//��̨״̬
	u8 RamdomXorData;
	
	u8 Code;
  u16 Error;
	u8 TxSync;
	u16 Random;
	u8 SaveBill;
	u8 McuReset;
	
	u16 Delay1sCnt;
	u8 Blink50Ms;     //50ms
	u8 Blink125Ms;    //250ms
  u8 Blink250Ms;    //250ms
  u8 Blink500Ms;    //
  u8 Blink1S;       //
  
	u8 Out573Buf[2];
	u16 Out2803Value;
  
	u8 TftErrorDisplay;
	u8 TftDisplayNum;
	//TFT
	u8 CoinOld;
	u8 CoinPlayOld;
	u8 MainPageLoad;
	u8 SucceedPageLoad;
	u8 FailPageLoad;
	
	u8 ArrowsChange;
	u8 PageIdex;
  u8 Step;
}S_FLAG;
extern S_FLAG Flag; 

enum
{
	C_TASKPRIO_vTftCtlTask=1,
	C_TASKPRIO_vRS485SendTask,
	C_TASKPRIO_vRS485ProcessTask,
  C_TASKPRIO_vDisplayTask,
  C_TASKPRIO_vKeyTask,
	
	C_TASKPRIO_vGsmTask,
	C_TASKPRIO_vGsmRecTask,
	C_TASKPRIO_vGsmSendTask,
  C_TASKPRIO_vGameTask,
};
extern u32 DevID[3];
#endif

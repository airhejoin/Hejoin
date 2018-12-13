#ifndef _FREERTOSTEMPLATE_H_
#define _FREERTOSTEMPLATE_H_

#include "stm32f10x.h"

//系统设置

#define MAIN_FIRST 181005
#define APP_VERSOIN 105


//运行灯
#define RunLedOn()  GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define RunLedOff() GPIO_SetBits(GPIOD,GPIO_Pin_12)

//状态灯
#define StaLedOn()  GPIO_ResetBits(GPIOD, GPIO_Pin_11)
#define StaLedOff() GPIO_SetBits(GPIOD,GPIO_Pin_11)

typedef struct{
  u8  ParaStatus;       //参数状态 0 为未初始化，1为默认设置，2为设置后
  u16 HighValue;        //高电平电压mv
  u8  WorkMode;         //工作模式 0普通 1第一次开 第二次关
  u8  TriggerLevel;     //触发电平
  u16 TriggerHoldTimes; //电平保持时间 ms 最大60S
  u32 NextTriggerDelay; //两次触发时间设置最大可以设置1年，单位秒
  
  u8 OutFixStatus;      //输出的绑定状态 
}S_CHKIN;

typedef struct{
  u8  FixInId;          //绑定的输入
  u8  OutLevel;         //输出电平
  u32 OutHoldTimes;     //有效输出时间，单位0.1秒
}S_CTLOUT;


typedef struct
{
  u8 InUseSta[8];       //8组输入的使用状态
  u8 OutUseSta[8];      //8组输出的使用状态
  u32 SetOutDelay[8];   //触发后输出延时设置
}S_CTL;

typedef struct
{
  S_CHKIN Insert[8];    //8个输入
  S_CTLOUT CtlOut[8];   //8组控制输出
  u32 RunFirst;				  //4byte第一次运行
  u32 RamdomSeed;			  //4byte
}S_SYSTEM;              //
extern S_SYSTEM System;


typedef struct
{
	u8 ServerOn;				//云模块上电标志
	u8 ServerStatus;		//云模块状态
	u8 ClientStatus;		//机台状态
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

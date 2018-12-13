#ifndef _GAME_H_
#define _GAME_H_
#include ".\Lib\includes.h"

#define Start1LedOn()  Flag.Out2803Value|=0x40;
#define Start1LedOff() Flag.Out2803Value&=~0x40;

#define Start2LedOn()  Flag.Out2803Value|=0x80;
#define Start2LedOff() Flag.Out2803Value&=~0x80;


void ParaInit(void);
void GameInit(void);//参数初始化
void GameVoice(void);
void LedCtlTask(void);
void ClockLedInit(void);
void ClockLedRunProcess(void);
u8 RevenueCalculate(void);//营收计算
u8 ResultCheat(void);			//结果作弊
void IdleLedRun(void);
void OutCtlProcess(void);
void DataSaveProcess(void);
#endif

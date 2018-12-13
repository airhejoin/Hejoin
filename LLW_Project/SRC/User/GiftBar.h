#ifndef _GIFTBAR_H_
#define _GIFTBAR_H_

#include "stm32f10x.h"

#define GIFTBAR_MAX 5	//最大支持8个，可扩展，需修改定义

enum{
	ERR_BAR1_BLOCK=0,
	ERR_BAR2_BLOCK,
	ERR_BAR3_BLOCK,
	ERR_BAR4_BLOCK,
	ERR_BAR5_BLOCK,
	ERR_BAR_EMPTY,
};

#define GiftBarSetRun(id) do{GiftPort[id]->BSRR=GiftPin[id];GiftBar.Enable|=1<<id;}while(0)			//打开礼品杆
#define GiftBarSetStop(id) do{GiftPort[id]->BRR=GiftPin[id];GiftBar.Enable&=~(1<<id);}while(0);	//关闭礼品杆

typedef struct{
	u16 Error;				//故障
	u8 	Enable;				//使能
	u8  InStatus;			//微动输入状态
	u8 	Status;				//默认状态
	u16 PressCount[GIFTBAR_MAX];	//有效状态计数
	u16 FreeCount[GIFTBAR_MAX];	//缺礼品状态计数
}S_GIFTBAR;
extern S_GIFTBAR GiftBar;
extern GPIO_TypeDef *GiftPort[8];
extern u16 GiftPin[8];
void GiftBarParaInit(void);
void GiftBarInt(void);
void GiftBarSetAllStop(void);
void GiftBarLedProcess(void);
#endif



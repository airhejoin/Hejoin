#ifndef _GIFTBAR_H_
#define _GIFTBAR_H_

#include "stm32f10x.h"

#define GIFTBAR_MAX 5	//���֧��8��������չ�����޸Ķ���

enum{
	ERR_BAR1_BLOCK=0,
	ERR_BAR2_BLOCK,
	ERR_BAR3_BLOCK,
	ERR_BAR4_BLOCK,
	ERR_BAR5_BLOCK,
	ERR_BAR_EMPTY,
};

#define GiftBarSetRun(id) do{GiftPort[id]->BSRR=GiftPin[id];GiftBar.Enable|=1<<id;}while(0)			//����Ʒ��
#define GiftBarSetStop(id) do{GiftPort[id]->BRR=GiftPin[id];GiftBar.Enable&=~(1<<id);}while(0);	//�ر���Ʒ��

typedef struct{
	u16 Error;				//����
	u8 	Enable;				//ʹ��
	u8  InStatus;			//΢������״̬
	u8 	Status;				//Ĭ��״̬
	u16 PressCount[GIFTBAR_MAX];	//��Ч״̬����
	u16 FreeCount[GIFTBAR_MAX];	//ȱ��Ʒ״̬����
}S_GIFTBAR;
extern S_GIFTBAR GiftBar;
extern GPIO_TypeDef *GiftPort[8];
extern u16 GiftPin[8];
void GiftBarParaInit(void);
void GiftBarInt(void);
void GiftBarSetAllStop(void);
void GiftBarLedProcess(void);
#endif



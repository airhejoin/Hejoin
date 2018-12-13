#ifndef _SEGDISPLAY_H_
#define _SEGDISPLAY_H_
#include".\Lib\includes.h"

//��Ϸ�÷�=3λ ��Ϸʱ��=3λ  ���Ƚ�����=2λ  ���Ƚ�����=2λ  һ�Ƚ�����=2λ 
#define SEGMAX 10	//����ܵ��������
#define SEGCODE_MAX 23
#define SEG_OFF 16
#define SEG_E 14
#define SEG_R 19
#define SEG_V 20
#define SEG_N 21
#define SEG_GANG 22
//////SEGB����ܺ궨��
#define SHCP_H GPIO_SetBits(GPIOB, GPIO_Pin_0)//
#define SHCP_L GPIO_ResetBits(GPIOB, GPIO_Pin_0)//

#define STCP_H GPIO_SetBits(GPIOC, GPIO_Pin_4)//
#define STCP_L GPIO_ResetBits(GPIOC, GPIO_Pin_4)//

#define DS_H GPIO_SetBits(GPIOC, GPIO_Pin_5)//
#define DS_L GPIO_ResetBits(GPIOC, GPIO_Pin_5)//
//////

extern u8 DisplayBuf[SEGMAX];
extern u8 SegBuf[SEGMAX];
extern const u8 SEGCODE[SEGCODE_MAX];
void SegDisplayInit(void);
void SegSelfCheck(void);
void NumToStr(u32 Num,u8 *dat,u8 size,u8 mode);
void SegDisplay(void);
void SetDisplay(void);
void SegDisplayOff(void);
void HC595SendData(u8 *buf);
#endif

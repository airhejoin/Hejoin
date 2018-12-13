#ifndef _WS281X_H_
#define _WS281X_H_

#include "stm32f10x.h"

#define GROUPSUM_MAX  10	//���������
#define GROUPSIZE_MAX  1	//ÿ��ư������ٸ����ص�
#define PIXELSUM_MAX  (GROUPSUM_MAX*GROUPSIZE_MAX) //�ܵ����ص�
#define PIXELBUFFER_MAX PIXELSUM_MAX*24
//#define 
enum{
	COLOR_BLUE=0x0000ff,	//
	COLOR_RED=0x00ff00,		//
	COLOR_GREEN=0xff0000,	//
	//COLOR_GREEN=0xffff00,	//
	COLOR_PINK=0x00ffff,		//
	COLOR_SKYBLUE=0xff00ff,		//
	//COLOR_YELLOW=0xffff00,	//
	COLOR_YELLOW=0xA7FF00,
	COLOR_WHITE=0xffffff,	//
	COLOR_DARK=0,	//
};
enum{
	LAMP_IDLE_NORMAL=0,
	LAMP_MODE_GAME,
};
typedef struct{
	u8  RunMode;		//����ģʽ
	u8 	Refresh;
	u16 PixelPointer;//����ָ��
	u32 GroupColor[GROUPSUM_MAX];	//��ɫ
	u8  PixelBuffer[PIXELBUFFER_MAX];//���ص�
} S_LEDINFO;
extern S_LEDINFO LedLamp;



void WS281xInit(void);
void LedLampOff(void);
void WS281xSendPixel(u32 color);
void WS281xDMAUpdate(u32 *buffer, u32 length);

void LedLampProcess(void);
#endif

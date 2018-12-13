#ifndef _WS281X_H_
#define _WS281X_H_

#include "stm32f10x.h"

#define GROUPSUM_MAX  10	//灯组的数量
#define GROUPSIZE_MAX  1	//每组灯包含多少个像素点
#define PIXELSUM_MAX  (GROUPSUM_MAX*GROUPSIZE_MAX) //总的像素点
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
	u8  RunMode;		//运行模式
	u8 	Refresh;
	u16 PixelPointer;//像素指针
	u32 GroupColor[GROUPSUM_MAX];	//颜色
	u8  PixelBuffer[PIXELBUFFER_MAX];//像素点
} S_LEDINFO;
extern S_LEDINFO LedLamp;



void WS281xInit(void);
void LedLampOff(void);
void WS281xSendPixel(u32 color);
void WS281xDMAUpdate(u32 *buffer, u32 length);

void LedLampProcess(void);
#endif

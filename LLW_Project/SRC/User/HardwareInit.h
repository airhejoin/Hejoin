#ifndef __HAL_H
#define __HAL_H
#include "stm32f10x.h"

#include "NVIC.h"
#include "TIM.h"
#include "RCC.h"
#include "SPI.h"
#include "USART.h"
#include "W25Q128.h"
//#define VERSION          100
enum{
	SETDEFAULT_ALL,
	SETDEFAULT_BILL,
	SETDEFAULT_PARA,
	SETDEFAULT_BILL_PARA,
};
void  ChipHalInit(void);
void  ChipOutHalInit(void);
void SystemParaDefault(u8 type);
void DelayXUs(u32 time);
void DelayXMs(u32 nCount);
#endif


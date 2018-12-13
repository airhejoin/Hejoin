#ifndef _WTV020_H_
#define _WTV020_H_

#include "stm32f10x.h"

#define ASDA_H GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define ASDA_L GPIO_ResetBits(GPIOC, GPIO_Pin_1)

#define ASCL_H GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define ASCL_L GPIO_ResetBits(GPIOC, GPIO_Pin_0)

#define BSDA_H GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define BSDA_L GPIO_ResetBits(GPIOC, GPIO_Pin_3)

#define BSCL_H GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define BSCL_L GPIO_ResetBits(GPIOC, GPIO_Pin_2)


void WTV020Init(void);
void WTV020ASend(u8 addr);
void WTV020BSend(u8 addr);

#endif

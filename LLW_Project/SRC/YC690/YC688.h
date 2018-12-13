#ifndef _YC688_H_
#define _YC688_H_

#include "stm32f10x.h"

void YC688_Game_Stop(void);
void YC688_Back_Stop(void);
void YC688_Game_Play(u8 num );
void YC688_Back_Play(u8 num );
void YC688_Game_SetVol(u8 vol);
void YC688_Back_SetVol(u8 vol);

#endif





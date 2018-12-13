#ifndef _YC690_H_
#define _YC690_H_

#include "stm32f10x.h"

#define YC690_BUSY_STA 1//播放为高电平
#define YC690_GAME 0xd0
#define YC690_BACK 0xe0
#define CMD_INTERVAL_MAX 100
//extern u16 I2C_TimeOut;
void YC690Init(void);
u8 YC690_SendCmd(u8 chip_addr,u8 *pbuff,u8 len);
u8 YC690_Play(u8 chip_addr,u8 num );
void YC690_SetVol(u8 chip_addr,u8 vol);
void YC690_Stop(u8 chip_addr );
#endif


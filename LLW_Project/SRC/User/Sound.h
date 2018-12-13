#ifndef _SOUND_H_
#define _SOUND_H_
#include "..\STM32Lib\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h"


#define VOICE_BUSY_STA 0 
#define VoiceGameBusySta (GPIOD->IDR&GPIO_Pin_0)
#define VoiceBackBusySta (GPIOD->IDR&GPIO_Pin_4)

//声音地址枚举
enum{
	VOICE_DIDIDI=1,
	VOICE_TRYPLAY,//2
	VOICE_BACK1,//3
	VOICE_SUCCEED,//4
	VOICE_FAILURE,//5
	VOICE_GAMESTART,//6
	VOICE_FEIDAO,//7
	VOICE_GONGJIAN,//8
	VOICE_SOU,//9
	VOICE_SHOTFLY,//10
	VOICE_BACK2,//11
	VOICE_NEXTOK,//12
	VOICE_INCOINDONG,//13
	VOICE_INCOINDANG,//14
	VOICE_GIFTSELACT,//15
	VOICE_TOSELGIFT,//16
	VOICE_WAITGIFT,//17
	VOICE_PLEASECOIN,//18
	VOICE_PLEASESTART,
	VOICE_MAX,//
};

#define VOICE_DEEP 5
typedef struct{
	u8 BegIndex;        //
	u8 EndIndex;        //
	u8 Buf[VOICE_DEEP];	//
	u8 NowAddr;		      //当前播放的语音的地址
}S_SOUND;             //

extern S_SOUND SoundBack,SoundGame;           

typedef struct{
	u8 Background1;
	u8 Background2;
	u8 Background3;
	u8 PrizeFirstWord;
	u8 PrizeSecondWord;
	u8 PrizeThirdWord;
	u8 PrizeFourthWord;
	u8 PrizeMusic;
	u8 CongratsPrizeWord;
	u8 GameStartWord;
	u8 GameOverWord;
	u8 TimeOverWord;
	u8 TicketFaultWord;
	u8 ConnectServerWord;
	u8 YouAreGreatWord;
	u8 YouAreBestWord;
	u8 InBall;
	u8 Cheer;
	u8 TryPlayWord;
	u8 FreeWord[5];
}S_SOUNDADDR;
extern S_SOUNDADDR SoundAddr;


extern u16 const VoiceDelLen[VOICE_MAX]; 

void SoundInit(void);           
void VoiceGameSendBreak(u8 addr);  
void VoiceGameSendOrder(u8 addr);

void VoiceBackSendBreak(u8 addr);
void VoiceBackSendOrder(u8 addr);

void VoiceGameSetSound(u8 vol);
void VoiceBackSetSound(u8 vol);

void VoiceBackStop(void);
void VoiceGameStop(void);
void VoicePlayProcess(void);
void VoiceTime(void);
#endif

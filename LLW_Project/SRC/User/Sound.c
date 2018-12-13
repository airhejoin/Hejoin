#include"..\User\Sound.h"
#include"..\YC690\YC688.h"
#include"..\User\FreeRTOSTemplate.h"
#include ".\Lib\includes.h"
S_SOUND SoundGame,SoundBack;
S_SOUNDADDR SoundAddr;
void SoundInit(void)
{
	u8 i=0;
	SoundGame.NowAddr=0;	
	SoundGame.BegIndex=0;
	SoundGame.EndIndex=0;
	for(i=0;i<VOICE_DEEP;i++)
		SoundGame.Buf[i]=0;
	
	SoundBack.NowAddr=0;	
	SoundBack.BegIndex=0;
	SoundBack.EndIndex=0;
	for(i=0;i<VOICE_DEEP;i++)
		SoundBack.Buf[i]=0;
}

//语音芯片A打断当前播放
void VoiceGameSendBreak(u8 addr)
{
	SoundGame.NowAddr=addr;
	SoundGame.BegIndex=0;
	SoundGame.EndIndex=0;
	SoundGame.Buf[0]=addr;
	Flag.YC690CmdDelay[0]=0;
	YC688_Game_Play(addr);
}
//语音芯片A顺序播放
void VoiceGameSendOrder(u8 addr)
{
	//SoundA.NowAddr=addr;
	SoundGame.Buf[SoundGame.EndIndex]=addr;//将声音放入队列
	if(++SoundGame.EndIndex>=VOICE_DEEP)
		SoundGame.EndIndex=0;
}

//语音芯片B打断当前播放
void VoiceBackSendBreak(u8 addr)
{
	SoundBack.NowAddr=addr;
	SoundBack.BegIndex=0;
	SoundBack.EndIndex=0;
	SoundBack.Buf[0]=addr;
	Flag.YC690CmdDelay[1]=0;
	YC688_Back_Play(addr);
}
//语音芯片B顺序播放
void VoiceBackSendOrder(u8 addr)
{
	SoundBack.Buf[SoundBack.EndIndex]=addr;//将声音放入队列
	if(++SoundBack.EndIndex>=VOICE_DEEP)
		SoundBack.EndIndex=0;
}
//语音芯片A声音调节
void VoiceGameSetSound(u8 vol)
{
  if(vol>32)
    vol=32;
	YC688_Game_SetVol(vol);
}

//语音芯片B声音调节
void VoiceBackSetSound(u8 vol)
{
  if(vol>32)
    vol=32;
	YC688_Back_SetVol(vol);
}

//语音芯片game停止播放
void VoiceGameStop(void)
{
	SoundGame.NowAddr=0;
	SoundGame.BegIndex=0;
	SoundGame.EndIndex=0;
	SoundGame.Buf[0]=0;
	Flag.YC690CmdDelay[0]=0;
	YC688_Game_Stop();
}

//语音芯片back停止播放
void VoiceBackStop(void)
{
	SoundBack.NowAddr=0;
	SoundBack.BegIndex=0;
	SoundBack.EndIndex=0;
	SoundBack.Buf[0]=0;
	Flag.YC690CmdDelay[1]=0;
	YC688_Back_Stop();
}

void VoicePlayProcess(void)
{
	u8 t1=3,t2=3;
	//语音A
	//if((GPIOD->IDR&GPIO_Pin_0)&&(Flag.YC690CmdDelay[0]==CMD_INTERVAL_MAX))
	//if(VoiceGameBusySta!=VOICE_BUSY_STA)
	if((VoiceGameBusySta!=VOICE_BUSY_STA)&&(Flag.YC690CmdDelay[0]==CMD_INTERVAL_MAX))
	{//上一个声音已经播放完
		if(SoundGame.BegIndex!=SoundGame.EndIndex)
		{//有声音需要处理
			Flag.YC690CmdDelay[0]=0;
			SoundGame.NowAddr=SoundGame.Buf[SoundGame.BegIndex];
			YC688_Game_Play(SoundGame.NowAddr);
			if(++SoundGame.BegIndex>=VOICE_DEEP)
				SoundGame.BegIndex=0;
		}
	}
	//语音B
	if((VoiceBackBusySta!=VOICE_BUSY_STA)&&(Flag.YC690CmdDelay[1]==CMD_INTERVAL_MAX))
	{//上一个声音已经播放完
		if(SoundBack.BegIndex!=SoundBack.EndIndex)
		{//有声音需要处理
			Flag.YC690CmdDelay[1]=0;
			SoundBack.NowAddr=SoundBack.Buf[SoundBack.BegIndex];
			YC688_Back_Play(SoundBack.NowAddr);
			if(++SoundBack.BegIndex>=VOICE_DEEP)
				SoundBack.BegIndex=0;
		}
		else 
		{
			if(Flag.Step==STEP_IDLE)
			{
				if(SoundBack.NowAddr==VOICE_BACK1)
				{
					VoiceBackSendBreak(VOICE_BACK2);//背景音2					
					VoiceBackSendOrder(VOICE_TRYPLAY);//快来玩吧
				}
				else
					VoiceBackSendBreak(VOICE_BACK1);
			}
		}
	}	
	if(CoinA.InsertFlag||CoinB.InsertFlag)
	{
		if(Flag.Step==STEP_IDLE)
		{
			VoiceGameSendBreak(VOICE_INCOINDONG);
			if(Flag.StartKeySound==0)
			{
				VoiceGameSendOrder(VOICE_PLEASESTART);//播放按开始键声音
				Flag.StartKeySound=1;
			}         
		}
		CoinA.InsertFlag=0;
		CoinB.InsertFlag=0;
	}
}

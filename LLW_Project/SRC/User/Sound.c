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

//����оƬA��ϵ�ǰ����
void VoiceGameSendBreak(u8 addr)
{
	SoundGame.NowAddr=addr;
	SoundGame.BegIndex=0;
	SoundGame.EndIndex=0;
	SoundGame.Buf[0]=addr;
	Flag.YC690CmdDelay[0]=0;
	YC688_Game_Play(addr);
}
//����оƬA˳�򲥷�
void VoiceGameSendOrder(u8 addr)
{
	//SoundA.NowAddr=addr;
	SoundGame.Buf[SoundGame.EndIndex]=addr;//�������������
	if(++SoundGame.EndIndex>=VOICE_DEEP)
		SoundGame.EndIndex=0;
}

//����оƬB��ϵ�ǰ����
void VoiceBackSendBreak(u8 addr)
{
	SoundBack.NowAddr=addr;
	SoundBack.BegIndex=0;
	SoundBack.EndIndex=0;
	SoundBack.Buf[0]=addr;
	Flag.YC690CmdDelay[1]=0;
	YC688_Back_Play(addr);
}
//����оƬB˳�򲥷�
void VoiceBackSendOrder(u8 addr)
{
	SoundBack.Buf[SoundBack.EndIndex]=addr;//�������������
	if(++SoundBack.EndIndex>=VOICE_DEEP)
		SoundBack.EndIndex=0;
}
//����оƬA��������
void VoiceGameSetSound(u8 vol)
{
  if(vol>32)
    vol=32;
	YC688_Game_SetVol(vol);
}

//����оƬB��������
void VoiceBackSetSound(u8 vol)
{
  if(vol>32)
    vol=32;
	YC688_Back_SetVol(vol);
}

//����оƬgameֹͣ����
void VoiceGameStop(void)
{
	SoundGame.NowAddr=0;
	SoundGame.BegIndex=0;
	SoundGame.EndIndex=0;
	SoundGame.Buf[0]=0;
	Flag.YC690CmdDelay[0]=0;
	YC688_Game_Stop();
}

//����оƬbackֹͣ����
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
	//����A
	//if((GPIOD->IDR&GPIO_Pin_0)&&(Flag.YC690CmdDelay[0]==CMD_INTERVAL_MAX))
	//if(VoiceGameBusySta!=VOICE_BUSY_STA)
	if((VoiceGameBusySta!=VOICE_BUSY_STA)&&(Flag.YC690CmdDelay[0]==CMD_INTERVAL_MAX))
	{//��һ�������Ѿ�������
		if(SoundGame.BegIndex!=SoundGame.EndIndex)
		{//��������Ҫ����
			Flag.YC690CmdDelay[0]=0;
			SoundGame.NowAddr=SoundGame.Buf[SoundGame.BegIndex];
			YC688_Game_Play(SoundGame.NowAddr);
			if(++SoundGame.BegIndex>=VOICE_DEEP)
				SoundGame.BegIndex=0;
		}
	}
	//����B
	if((VoiceBackBusySta!=VOICE_BUSY_STA)&&(Flag.YC690CmdDelay[1]==CMD_INTERVAL_MAX))
	{//��һ�������Ѿ�������
		if(SoundBack.BegIndex!=SoundBack.EndIndex)
		{//��������Ҫ����
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
					VoiceBackSendBreak(VOICE_BACK2);//������2					
					VoiceBackSendOrder(VOICE_TRYPLAY);//�������
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
				VoiceGameSendOrder(VOICE_PLEASESTART);//���Ű���ʼ������
				Flag.StartKeySound=1;
			}         
		}
		CoinA.InsertFlag=0;
		CoinB.InsertFlag=0;
	}
}

#include ".\Lib\includes.h"
void ParaInit(void)
{
	Flag.Out573Buf[0]=0;
	Flag.Out573Buf[1]=0;
	Flag.Out2803Value=0;
}

void DataSaveProcess(void)
{
  if(Flag.SaveBill)
  {
		if(FM24WriteBuf(USER_ADDR,(u8 *)&System,sizeof(S_SYSTEM)))//sizeof(System.Bill)+sizeof(System.Play)+sizeof(System.ProbBill)+sizeof(System.Backup));
			Flag.SaveBill=0;
	}
	if(Flag.McuReset==1)
	{
		if(Flag.SaveBill)
		{
			return ;
		}
		Flag.McuReset=0;
		NVIC_SystemReset();
		//÷ÿ∆Ù
	}  
}



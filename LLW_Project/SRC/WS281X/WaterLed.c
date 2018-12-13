#include"WaterLed.h"

void LedGradual(void)
{
	u8 i=0,change=0;
	u8 buf[3]={0};
	static u8 dir=0,grade=0,color=0;
	if(dir)
	{
		if(grade>=255)
		{//½¥ÁÁ
			dir=0;//½¥Ãð
		}
		else
		{
			grade++;
			change=1;
		}
	}
	else
	{
		if(grade==0)
		{
			dir=1;//½¥ÁÁ
			if(++color>6)//LEDÑÕÉ«
			{
				color=0;
			}
		}
		else
		{
			grade--;
			change=1;
		}
	}
	switch(color)
	{
		case 0:
			buf[0]=grade;
			buf[1]=0;
			buf[2]=0;
			break;
		case 1:
			buf[0]=0;
			buf[1]=grade;
			buf[2]=0;
			break;
		case 2:
			buf[0]=0;
			buf[1]=0;
			buf[2]=grade;
			break;
		case 3:
			buf[0]=grade;
			buf[1]=grade;
			buf[2]=0;
			break;
		case 4:
			buf[0]=grade;
			buf[1]=0;
			buf[2]=grade;
			break;
		case 5:
			buf[0]=0;
			buf[1]=grade;
			buf[2]=grade;
			break;
		case 6:
			buf[0]=grade;
			buf[1]=grade;
			buf[2]=grade;
			break;
		default:
			break;
	}
	if(change)
	{
		for(i=0;i<GROUPSUM_MAX;i++)
		{
			LedLamp.GroupColor[i]=buf[0];
			LedLamp.GroupColor[i]|=(buf[1]<<8);
			LedLamp.GroupColor[i]|=(buf[2]<<16);
		}	
		WS281xDMAUpdate(LedLamp.GroupColor,GROUPSUM_MAX);
	}
}

void LedIdleProcess(void)
{
	switch(LedLamp.RunMode)
	{
		case LAMP_MODE_NORMAL:
			break;
		case LAMP_MODE_GAME:
			break;
		default:
			break;
	}
}



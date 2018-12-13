#include "Key.h"
#define INSERT_CONVERT 1
S_INSERTSIGN SignIn;
void GetIn245Status(void)
{
	u8 i=0;
	u8 temp=0;
	u8 In245Buf[4];

	//245A
	GPIOB->BRR=GPIO_Pin_8;
	__NOP();
	__NOP();
  In245Buf[0]=(GPIOE->IDR&0x007c)>>2;
  In245Buf[0]|=((GPIOC->IDR&0xe000)>>8);
	GPIOB->BSRR=GPIO_Pin_8;
	
	//245B
	GPIOB->BRR=GPIO_Pin_9;
	__NOP();
	__NOP();
  In245Buf[1]=(GPIOE->IDR&0x007c)>>2;
  In245Buf[1]|=((GPIOC->IDR&0xe000)>>8);
	GPIOB->BSRR=GPIO_Pin_9;
	
	//245C
	GPIOE->BRR=GPIO_Pin_0;
	__NOP();
	__NOP();
	In245Buf[2]=(GPIOE->IDR&0x007c)>>2;
  In245Buf[2]|=((GPIOC->IDR&0xe000)>>8);
	GPIOE->BSRR=GPIO_Pin_0;
	
	GPIOE->BRR=GPIO_Pin_1;	
	__NOP();
	__NOP();
	In245Buf[3]=(GPIOE->IDR&0x007c)>>2;
  In245Buf[3]|=((GPIOC->IDR&0xe000)>>8);
	GPIOE->BSRR=GPIO_Pin_1;
	
	SignIn.In245Status=In245Buf[0]+(In245Buf[1]<<8)+(In245Buf[2]<<16)+(In245Buf[3]<<24);
}
void InsertSignScan(void)//输入信号检测
{
  u8 i=0;
  u16 value=0;
  u8 temp=0;
	value =  SignIn.In245Status&0x0000001f;//取出礼品杆状态
	value|=((SignIn.In245Status&0x3f000000)>>19);
	value&=0x07ff;
  for(i=0;i<KEYID_MAX;i++)
  {
		if(i<5)//in1~in4
		{
			if(value&(1<<i))
			{//弹起
				SignIn.PressCount[i]=0;
				if(++SignIn.FreeCount[i]>30)
				{
					SignIn.FreeCount[i]=30;
					 if(SignIn.Status&(1<<i))
					{
						SignIn.Status&=~(1<<i);//清除标志
						SignIn.Error&=~(1<<(i+1));
					}         
				}
			}
			else
			{//按下
				SignIn.FreeCount[i]=0;
				SignIn.PressCount[i]++;
				if(SignIn.PressCount[i]>400)
				{//超过2S报错
					SignIn.PressCount[i]=400;
					SignIn.Error|=1<<(i+1);
				}
				else if(SignIn.PressCount[i]>3)
				{//短按
					if((SignIn.Status&(1<<i))==0)
					{
						SignIn.Status|=1<<i;
						SignIn.Value|=1<<i;
					} 
				}
			}      			
		}
		else//KEYSET~START2
		{
			if(value&(1<<i))
			{//弹起
				SignIn.PressCount[i]=0;
				if(++SignIn.FreeCount[i]>1)
				{
					SignIn.FreeCount[i]=30;
					 if(SignIn.Status&(1<<i))
					{
						SignIn.Status&=~(1<<i);//清除标志
						SignIn.Error&=~(1<<(i+1));
					}         
				}
			}
			else
			{//按下
				SignIn.FreeCount[i]=0;
				SignIn.PressCount[i]++;
				if(SignIn.PressCount[i]>400)
				{//超过2S报错
					SignIn.PressCount[i]=400;
					SignIn.Error|=1<<(i+1);
				}
				//else if(SignIn.PressCount[i]>3)
				{//短按
					if((SignIn.Status&(1<<i))==0)
					{
						SignIn.Status|=1<<i;
						SignIn.Value|=1<<i;
					} 
				}
			}      
		}
  }
}
u8 ReadKey(u16 num)
{
  u8 val=0;
  if(SignIn.Value&(1L<<num))
  {
    val=1;
  }
  SignIn.Value&=~(1L<<num);
  return val;
}
u8 ReadStatus(u16 num)
{
  u8 val=0;
  if(SignIn.Status&(1L<<num))
  {
    val=1;
  }
  return val;
}





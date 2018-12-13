#include ".\Lib\includes.h"
#include "HardwareInit.h"
#include"..\WS281X\WS281X.h"
//拨码开关 \\1PE1 \\PE0 升级stc程序 \\3PB9 \\4PB8
S_SYSTEM System;
S_FLAG Flag;
//S_GAME Game;
extern void GPIOInit(void);
extern void I2CInit(void);
u32 DevID[3]={0};

void DelayXUs(u32 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //
      while(i--) ;    
   }
}
/******************************************************************************
 * 函数名:DelayXMs
 * 说明: 
 *    延时XXMS
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void DelayXMs(u32 nCount)
{
	u32 i=0;

  	for(; nCount != 0; nCount--)
  	{
  		i=12000;
			while(--i);
  	}
}

/*************    函数名：ChipHalInit              ***************************
【功能】：系统外设初始化
【参数】：
【返回】：
【说明】：
*********************************************************************************/
void  ChipHalInit(void)
{
	u8 tp=0;
	DelayXMs(10);/*延时100MS ，等待外围器件就绪*/
	RccInit();
	NvicInit();
  GPIOInit();
  I2CInit();
	TimInit();
	DelayXMs(100);	
  UsartInit();
}
/*************    函数名：SystemDefault           ***************************
【功能】：系统参数初始化
【参数】：
【返回】：
【说明】：0清除所有数据 1 清账 2 恢复出厂并清账
**************   设计：DAVID 2017-03-01  11:32:35***********************/
void SystemParaDefault(u8 type)
{
  u8 i=0;
  
  for(i=0;i<8;i++)
  {
    System.Insert[0].HighValue=1;
    System.Insert[0].NextTriggerDelay=1000;//1秒    
    
  }

}
//20181213
/*************    函数名：ChipOutHalInit           ***************************
【功能】：其他资源初始化
【参数】：
【返回】：
【说明】：
*********************************************************************************/
void  ChipOutHalInit(void)
{
  u8 i,j;
	/*获取STM32的唯一ID，进行加密判断*/
	DevID[0] = *(vu32*)(0x1FFFF7E8);
  DevID[1] = *(vu32*)(0x1FFFF7EC);
  DevID[2] = *(vu32*)(0x1FFFF7F0);
	Flag.Random=((DevID[2]>>8)^(DevID[1]<<8)^(DevID[0]<<16))*1664525+1013904223l;//计算随机数
	FM24ReadBuf(USER_ADDR,(u8 *)&System,sizeof(S_SYSTEM));//读取主机信息
	if(System.RunFirst!=MAIN_FIRST)
	{//未初始化过，初始化参数
    SystemParaDefault(SETDEFAULT_ALL);
    System.RunFirst=MAIN_FIRST;
		System.RamdomSeed=Flag.Random;
		FM24WriteBuf(USER_ADDR,(u8 *)&System,sizeof(S_SYSTEM));//写入数据
  }
	RandomInit(System.RamdomSeed);//写入随机数种子
  System.RamdomSeed=IRandomX(1,999999);//重新获取随机数
  Flag.SaveBill=1;//保存获取的随机数
}  


   

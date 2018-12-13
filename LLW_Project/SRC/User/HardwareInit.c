#include ".\Lib\includes.h"
#include "HardwareInit.h"
#include"..\WS281X\WS281X.h"
//���뿪�� \\1PE1 \\PE0 ����stc���� \\3PB9 \\4PB8
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
 * ������:DelayXMs
 * ˵��: 
 *    ��ʱXXMS
 * ����: 
 * ���: 
 * ����: 
 * ���:
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

/*************    ��������ChipHalInit              ***************************
�����ܡ���ϵͳ�����ʼ��
����������
�����ء���
��˵������
*********************************************************************************/
void  ChipHalInit(void)
{
	u8 tp=0;
	DelayXMs(10);/*��ʱ100MS ���ȴ���Χ��������*/
	RccInit();
	NvicInit();
  GPIOInit();
  I2CInit();
	TimInit();
	DelayXMs(100);	
  UsartInit();
}
/*************    ��������SystemDefault           ***************************
�����ܡ���ϵͳ������ʼ��
����������
�����ء���
��˵������0����������� 1 ���� 2 �ָ�����������
**************   ��ƣ�DAVID 2017-03-01  11:32:35***********************/
void SystemParaDefault(u8 type)
{
  u8 i=0;
  
  for(i=0;i<8;i++)
  {
    System.Insert[0].HighValue=1;
    System.Insert[0].NextTriggerDelay=1000;//1��    
    
  }

}
//20181213
/*************    ��������ChipOutHalInit           ***************************
�����ܡ���������Դ��ʼ��
����������
�����ء���
��˵������
*********************************************************************************/
void  ChipOutHalInit(void)
{
  u8 i,j;
	/*��ȡSTM32��ΨһID�����м����ж�*/
	DevID[0] = *(vu32*)(0x1FFFF7E8);
  DevID[1] = *(vu32*)(0x1FFFF7EC);
  DevID[2] = *(vu32*)(0x1FFFF7F0);
	Flag.Random=((DevID[2]>>8)^(DevID[1]<<8)^(DevID[0]<<16))*1664525+1013904223l;//���������
	FM24ReadBuf(USER_ADDR,(u8 *)&System,sizeof(S_SYSTEM));//��ȡ������Ϣ
	if(System.RunFirst!=MAIN_FIRST)
	{//δ��ʼ��������ʼ������
    SystemParaDefault(SETDEFAULT_ALL);
    System.RunFirst=MAIN_FIRST;
		System.RamdomSeed=Flag.Random;
		FM24WriteBuf(USER_ADDR,(u8 *)&System,sizeof(S_SYSTEM));//д������
  }
	RandomInit(System.RamdomSeed);//д�����������
  System.RamdomSeed=IRandomX(1,999999);//���»�ȡ�����
  Flag.SaveBill=1;//�����ȡ�������
}  


   

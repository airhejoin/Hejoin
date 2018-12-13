#include".\Lib\includes.h"
#include"string.h"
#define SPI2NSS_H() GPIO_SetBits(GPIOB, GPIO_Pin_12);//
#define SPI2NSS_L() GPIO_ResetBits(GPIOB, GPIO_Pin_12);//
const u8 SEGCODE[SEGCODE_MAX]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,	
	0x77,0x7c,0x39,0x5e,0x79,0x71,
	0x00,//Disable 16
	0x1e,//J 17
	0x73,//P 18
  0x50,//r 19
  0x1c,//v 20
	0x37,//n
	0x40,//中间横杠
};//共阴数码管

//一等奖2 二等奖2 三等奖2 游戏时间3 游戏得分3
u8 DisplayBuf[SEGMAX]={0};
u8 SegBuf[SEGMAX]={0};

void SegDisplayInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  /* SCK, MIS and MOSI  PB13=CLK,PB14=MISO,PB15=MOSI*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*  PB12 作片选*/
  GPIO_SetBits(GPIOB, GPIO_Pin_12);//预置为高
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* SPI2 configuration  */
  SPI_Cmd(SPI2, DISABLE); 												//必须先禁能,才能改变MODE
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=0 时钟悬空低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=0 数据捕获第1个
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//4分频	,不能太快
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7
  
  SPI_Init(SPI2, &SPI_InitStructure);
  SPI_Cmd(SPI2, ENABLE); 
  SPI2NSS_L();
	
	SegDisplayOff();
}

/******************************************************************************
 * 函数名:SPI2_ReadWriteByte
 * 说明: 
 *    SPI2读写数据 
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
INT8U SPI2_ReadWriteByte(u8  TxData)
{
  	/* Loop while DR register in not emplty */
  	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  	/* Send byte through the SPI2 peripheral */
  	SPI_I2S_SendData(SPI2, TxData);

  	/* Wait to receive a byte */
  	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  	/* Return the byte read from the SPI bus */
  	return SPI_I2S_ReceiveData(SPI2);
}

void HC595SendData(u8 *buf)
{
	u8 i;
  for(i=0;i<SEGMAX;i++)
	{
		SPI2_ReadWriteByte(buf[SEGMAX-i-1]);
	}
	//!更新到595显示缓冲区
	SPI2NSS_H();
	__NOP();
	__NOP();
	SPI2NSS_L();
}

//获取数字中某一位的大小
u8 GetNumBit(u32 Num,u8 index)
{
	u8 temp[8]={0};
	while(Num>=100000000) Num-=100000000;
	while(Num>=10000000)	{Num-=10000000;temp[7]++;};
	while(Num>=1000000) 	{Num-=1000000;temp[6]++;};
	while(Num>=100000) 		{Num-=100000;temp[5]++;};
	while(Num>=10000) 		{Num-=10000;temp[4]++;};
	while(Num>=1000) 			{Num-=1000;temp[3]++;};
	while(Num>=100) 			{Num-=100;temp[2]++;};
	while(Num>=10) 				{Num-=10;temp[1]++;};
												temp[0]=Num;
	return temp[index];
}
/******************************************************************************
 * 函数名:Num3BitToStr
 * 说明: 
 *     显示3位数，高位为0 不显示
 *	   num需要转化的数字，buf指针，size 显示长度 
 * 输入: 
 * 输出: 
 * 返回: 
*  2017年5月12日, David
 *****************************************************************************/
#define DECNUM_MAX 10
void NumToStr(u32 Num,u8 *dat,u8 size,u8 mode)
{
	u8 i=0,buf[DECNUM_MAX]={0},temp[DECNUM_MAX]={0};
	u8 flag=0;
	if((size<2)||(size>DECNUM_MAX))
		return;//最大位数不能超过8位,最小不能小于2位数
	while(Num>=100000000) Num-=100000000;
	while(Num>=1000000000)	{Num-=1000000000;temp[9]++;};
	while(Num>=100000000) 	{Num-=100000000;temp[8]++;};
	while(Num>=10000000)		{Num-=10000000;temp[7]++;};
	while(Num>=1000000) 		{Num-=1000000;temp[6]++;};
	while(Num>=100000) 			{Num-=100000;temp[5]++;};
	while(Num>=10000) 			{Num-=10000;temp[4]++;};
	while(Num>=1000) 				{Num-=1000;temp[3]++;};
	while(Num>=100) 				{Num-=100;temp[2]++;};
	while(Num>=10) 					{Num-=10;temp[1]++;};
																temp[0]=Num;
	flag=0;
	i=DECNUM_MAX;
	do
	{
		i--;
		if(flag==0)
		{
			if(temp[i]==0)
			{
				if(mode==0)
					buf[i]=SEGCODE[SEG_OFF];
				else if(mode==1)
					buf[i]=0x40;
				else if(mode==2)
					buf[i]=0x00;
			}
			else
			{
				if(mode==2)
					buf[i]=temp[i];
				else
					buf[i]=SEGCODE[temp[i]];
				flag=1;//temp出现大于0的数
			}
		}
		else
		{
			if(mode==2)
					buf[i]=temp[i];
			else
				buf[i]=SEGCODE[temp[i]];
		}
	}while(i);
	if(flag==0)
	{
		if(mode==2)//带0
				buf[0]=0;
		else
			buf[0]=SEGCODE[0];	
	}
	if(mode<2)
	{//高位前低位后，向前缩进
		for(i=0;i<size;i++)
			dat[i]=buf[size-i-1];
	}
	else
	{
		for(i=0;i<size;i++)
			dat[i]=buf[i];	
	}
}
void SegDisplay(void)
{
	u8 buf[4]={0};
	u8 i=0;
	u32 Num=0;
	u8 temp=0;
	if(Flag.Step!=STEP_SELGIFT)
	{
		Num=Flag.Time10SCnt;
		if(Num<1000)
		{
			DisplayBuf[0]=SEGCODE[SEG_OFF];
			DisplayBuf[1]=SEGCODE[0];
		}
		else if(Flag.Time10SCnt<10000)
		{
			DisplayBuf[0]=SEGCODE[SEG_OFF];
			DisplayBuf[1]=SEGCODE[Num/1000];
		}
		else
		{
			DisplayBuf[0]=SEGCODE[Num/10000];
			DisplayBuf[1]=SEGCODE[Num%10000/1000];
		}
		Num=Num%1000;
		DisplayBuf[2]=SEGCODE[Num/100];
		DisplayBuf[3]=SEGCODE[Num%100/10];
		DisplayBuf[4]=SEGCODE[Num%10];
		DisplayBuf[1]|=0x80;
		//if(Flag.ResultCheat==0)
		{//未作弊同步显示，作弊显示作弊的时候
			for(i=0;i<5;i++)
				DisplayBuf[5+i]=DisplayBuf[i];		
			
		}
		#if 1
		if(Flag.PrizeBlinkSta==1)
		{
			if(Flag.Blink125Ms)
			{
				for(i=0;i<SEGMAX;i++)
				{
					DisplayBuf[i]=SEGCODE[SEG_OFF];
				}
			}
		}
		#else
		if(Flag.PrizeBlinkSta==1)
		{
			if(Flag.Blink125Ms)
			{
				for(i=0;i<5;i++)
				{
					DisplayBuf[i]=SEGCODE[SEG_OFF];
				}
			}
		}
		if(Flag.ResultPrize==1)
			DisplayBuf[0]|=0x80;
		if(Flag.ResultFail==1)
			DisplayBuf[4]|=0x80;
		if(Flag.ResultCheat==1)
			DisplayBuf[3]|=0x80;		
		#endif
		if(Flag.Step==STEP_SET)
		{
			DisplayBuf[0]=SEGCODE[0x0c];
			DisplayBuf[1]=SEGCODE[0];
			DisplayBuf[2]=SEGCODE[SEG_N];
			DisplayBuf[3]=SEGCODE[0x0F];
			for(i=4;i<SEGMAX;i++)
				DisplayBuf[i]=SEGCODE[SEG_GANG];
			
		}		
	}
	else
	{
		for(i=0;i<SEGMAX;i++)
			DisplayBuf[i]=SEGCODE[Flag.GiftSelTime];
	}
	
	HC595SendData(DisplayBuf);
}

void SegDisplayOff(void)
{
  u8 i;
  for(i=0;i<SEGMAX;i++)
  {
    DisplayBuf[i]=SEGCODE[SEG_OFF];
  }
  HC595SendData(DisplayBuf);
}
void SegSelfCheck(void)
{
  u8 i=0;
	for(i=0;i<8;i++)
	{
    memset(DisplayBuf,1<<i,sizeof(DisplayBuf));
		HC595SendData(DisplayBuf);
		///ClockLedInit();
		vTaskDelay(100);
		//ClockLedInit();
		vTaskDelay(100);
		//ClockLedInit();
		//vTaskDelay(100);
	}
	//ClockLedInit();
	vTaskDelay(200);
  //显示版本号
  memset(DisplayBuf,SEGCODE[SEG_OFF],sizeof(DisplayBuf));
  DisplayBuf[1]=SEGCODE[SEG_V];
  NumToStr(System.AppVersion,&DisplayBuf[2],3,0);
  HC595SendData(DisplayBuf);
  vTaskDelay(1000);
}

/*-------------------------------------------------------------------------
    
-------------------------------------------------------------------------*/
/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                             大脚丫电子工作室
*
*模  块：SPI操作165和595
*设计者：南下三五年
*时  间：2014-09-17 21:28:48
*********************************************************************************
*/
#include"includes.h"

INT8U Get165BuffISR[MAXNUM595]={0};
INT8U Get165BuffNew[MAXNUM595]={0};
INT8U Get165BuffOld[MAXNUM595]={0};

//!数码管编码
INT8U LedTab[38]={
	LedChar_0,LedChar_1,LedChar_2,LedChar_3,LedChar_4,LedChar_5,LedChar_6,
	LedChar_7,LedChar_8,LedChar_9,LedChar_OFF,
	LedChar_A,
	LedChar_B,
	LedChar_C,
	LedChar_D,
	LedChar_E,
	LedChar_F,
	LedChar_G,
	LedChar_H,
	LedChar_I,
	LedChar_J,
	LedChar_K,
	LedChar_L,
	LedChar_M,
	LedChar_N,
	LedChar_O,
	LedChar_P,
	LedChar_Q,
	LedChar_R,
	LedChar_S,
	LedChar_T,
	LedChar_U,
	LedChar_V,
	LedChar_W,
	LedChar_X,
	LedChar_Y,
	LedChar_Z,
	LedChar__,
};

//!根据数码管编号，转换到对应的二维数组中
#define SETMAP(A) ((A)/16*MAXNUM595+(A)%16)
INT8U mapbcd[8]=
{
	SETMAP(0x00),SETMAP(0x10),SETMAP(0x20),SETMAP(0x30),//!现金
	SETMAP(0x02),SETMAP(0x12),SETMAP(0x22),SETMAP(0x32),//!应出币
};

//!8个数码管更新缓冲区
INT8U DisBuff[MAXCOM][MAXNUM595]={0};

//!最大显示8个数字
INT8U BCD[8]={0};

//!需要更新的COM端
INT8U gSelCom=0;

/******************************************************************************
 * 函数名:DisMapBCD
 * 说明: 
 *    更新显示缓冲区的二维数组 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 23:34:13, 2014年9月17日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void DisMapBCD(INT8U led_id, INT8U num)			//显示子程序
{
	led_id=mapbcd[led_id];
	DisBuff[0][led_id]=LedTab[num];
}

/******************************************************************************
 * 函数名:DisNumberCoor
 * 说明: 
 *    在指定位置显示数字 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 23:0:17, 2014年9月17日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void DisNumberCoor(INT8U zero, INT8U StartLed, INT8U Width, INT32S Num)//zero=0,前面带0，zero=1，前面清零
{
	INT32U i=0;
	INT32U j=0;
	
	if(StartLed>LEDTOTAL)
		return;

	BCD[7]=BCD[6]=BCD[5]=BCD[4]=BCD[3]=BCD[2]=BCD[1]=0;

	while(Num>=10000000)	{Num-=100000000;	        	}
	while(Num>=10000000)	{Num-=10000000;		BCD[7]++; }
	while(Num>=1000000)		{Num-=1000000;		BCD[6]++;	}
  while(Num>=100000)		{Num-=100000;			BCD[5]++;	}
  while(Num>=10000)			{Num-=10000;			BCD[4]++;	}
  while(Num>=1000)			{Num-=1000;				BCD[3]++;	}
  while(Num>=100)				{Num-=100;				BCD[2]++;	}
  while(Num>=10)				{Num-=10;					BCD[1]++;	}
  BCD[0]=Num;

	//!清楚前导0
	if(zero)
	{
		for(i=0;i<7;i++)
		{
			if(BCD[7-i])
				break;
		}

		for(j=0;j<i;j++)
			BCD[7-j]=10;
	}
	
	i=0;
	do
	{
		Width--;
		DisMapBCD(StartLed+Width, BCD[i]);
		if(i==sizeof(BCD)-1)	break;
		i++;
	}while(Width);
}

/******************************************************************************
 * 函数名:DigitalSPIInit
 * 说明: 
 *    数码管显示SPI初始化 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 11:41:50, 2016年7月12日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void DigitalSPIInit(void)
{
   SPI_InitTypeDef SPI_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO ,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

   /* SCK, MIS and MOSI  A5=CLK,A6=MISO,A7=MOSI*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*  PA.4 作片选*/
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//预置为高
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI1 configuration  */
	SPI_Cmd(SPI2, DISABLE); 												//必须先禁能,才能改变MODE
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=0 时钟悬空低
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=0 数据捕获第1个
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//4分频	,不能太快
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7
    
	SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE); 
}

/******************************************************************************
 * 函数名:SPI2_ReadWriteByte
 * 说明: 
 *    SPI2读写数据 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 11:43:25, 2016年7月12日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
INT8U SPI2_ReadWriteByte(INT8U  TxData)
{
  	/* Loop while DR register in not emplty */
  	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  	/* Send byte through the SPI1 peripheral */
  	SPI_I2S_SendData(SPI2, TxData);

  	/* Wait to receive a byte */
  	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  	/* Return the byte read from the SPI bus */
  	return SPI_I2S_ReceiveData(SPI2);
}

/******************************************************************************
 * 函数名:SendBuffAndGetKeyBuff
 * 说明: 
 *    SPI送出显示缓冲区并且获取按键 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 21:50:47, 2014年9月17日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void SendBuffAndGetKeyBuff( INT8U *SendBuff, INT8U *KeyBuff)
{
	INT32U i=0;
	INT8U data=0;

	//!更新165键值
	#if 0
	SPI2SSL;
	__NOP();
	__NOP();
	SPI2SSH;
	__NOP();
	__NOP();
	#endif

	for(i=0;i<MAXNUM595;i++)
	{
		data=SPI2_ReadWriteByte(*SendBuff++);
		*KeyBuff++=(~data);
	}

	//!更新到595显示缓冲区
	SPI2SSL;
	__NOP();
	__NOP();
	SPI2SSH;
}

/******************************************************************************
 * 函数名:UpdatekeyAndDis
 * 说明: 
 *    刷新显示缓冲区和更新键值 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 16:16:37, 2014年9月22日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void UpdateKeyAndDis(void)
{
	if(++gSelCom>=4)
	{
		gSelCom=0;
	}
	
	DisBuff[gSelCom][1]=1<<gSelCom;
	DisBuff[gSelCom][3]=1<<gSelCom;

	SendBuffAndGetKeyBuff(&DisBuff[gSelCom][0],Get165BuffISR);
}

/******************************************************************************
 * 函数名:CloseLedCoor
 * 说明: 
 *    关闭指定位置数码管显示 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 12:8:48, 2016年7月13日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void CloseLedCoor(INT8U StartLed, INT8U Width)
{
	do
	{
		Width--;
		DisMapBCD(StartLed+Width, 10);
	}while(Width);
}

/******************************************************************************
 * 函数名:DisHardWrong
 * 说明: 
 *    显示故障信息 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 12:15:36, 2016年7月13日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void DisHardWrong(INT8U WrongList)
{
	INT32U i=0;

	for(i=0;i<8;i++)
	{
		if(WrongList&1<<i)
			break;
	}

	if(i<8)
	{
		DisMapBCD(0, 15);
		DisMapBCD(1, 37);
		DisMapBCD(2, 37);
		DisNumberCoor(0,3,1,i);
	}
}

/******************************************************************************
 * 函数名:DisCleanCoin
 * 说明: 
 *    显示清币 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 11:22:46, 2016年7月14日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void DisCleanCoin(void)
{
	DisMapBCD(0, 13);
	DisMapBCD(1, 22);
	DisMapBCD(2, 15);
	DisMapBCD(3, 28);
}


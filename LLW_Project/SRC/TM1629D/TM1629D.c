/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：TM1629D初始化配置
*设计者：南下三五年
*时  间：2012-05-24  01:03:27
*********************************************************************************
*/
#include"..\Lib\includes.h"

/*************    函数名：InitTM1629DIO             ****************************
【功能】：初始化单片机的TM1629控制口线
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-25  22:59:48***********************/
void InitTM1629DIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = TM1629DIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TM1629CLK|TM1629DOUT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, TM1629CLK);
	GPIO_SetBits(GPIOC, TM1629DOUT);
	GPIO_SetBits(GPIOE, TM1629DIN);
}
/*************    函数名：SendByteTM1629D         ****************************
【功能】：向TM1629发送一个字节
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-25  23:12:30***********************/
void SendByteTM1629D(INT8U data)
{
	INT32U i=0;

	for(i=0;i<8;i++)
	{
		TM1629DCLKL;
		__NOP();
		__NOP();

		if(data&0x01)
			TM1629DOUTH;
		else
			TM1629DOUTL;

		data>>=1;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		TM1629DCLKH;
		__NOP();
		__NOP();
	}
}
/*************    函数名：ReadDataTM1629D           **************************
【功能】：从TM1629缓冲区读取一个字节
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-26  20:49:46***********************/
INT8U ReadDataTM1629D(void)
{
	INT32U i=0;
	INT8U data=0;

	for(i=0;i<8;i++)
	{
		TM1629DCLKL;
		data>>=1;
		
		__NOP();
		__NOP();

		TM1629DCLKH;
		
		__NOP();
		__NOP();
		
		if(GPIO_ReadInputDataBit(GPIOE, TM1629DIN))
			data|=0x80;
		else
			data&=0x7f;
		
		__NOP();
	}

	return data;
}

/*************    函数名：InitTM1629D             ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-25  23:37:06***********************/
void InitTM1629D(void)
{
	InitTM1629DIO();
	SendByteTM1629D(WRITEBUFFER);
	SendByteTM1629D(SETPLUSE14);
	SendByteTM1629D(DISON );
}

/*************    函数名：ScanKey                 ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-13  13:29:14***********************/
INT32U ScanKey(void)
{
 	INT8U i=0;
	INT32U key=0;
	INT16U sel=0;
	
	SET_DATASEL(sel);
	sel=0x07;
	SET_DATASEL(sel);
	SendByteTM1629D(READYKEY);
		
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	for(i=0;i<4;i++)
	{
		key<<=8;
		key+=ReadDataTM1629D();
		
	}

	sel=0;
	SET_DATASEL(sel);
	return key;
}
































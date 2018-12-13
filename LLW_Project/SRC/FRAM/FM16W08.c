/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                              大脚丫电子工作室
*
*模  块：FM16W08
*说  明:
*        为了项目的管理，项目的全局变量声明放在这里，方便项目的后期维护，和多人
*        协作
*设计者：南下三五年
*时  间：2012-11-18  10:23:49
*********************************************************************************
*/
#include".\Lib\includes.h"
/*************    函数名：InitFM16W08Hard         ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-18  10:46:31***********************/
void InitFM16W08Hard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//输出口线设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//!数据输入口线
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //下拉输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//!口线预置
	FM16W08CEH;
	FM16W08OEH;
	FM16W08WEH;
}
/*************    函数名：ReadDataFM16W08          ***************************
【功能】：
【参数】：
【返回】：
【说明】：72M,一条指令为13NS，
**************   设计：南下三五年 2012-11-18  10:53:14***********************/
INT8U ReadDataFM16W08(INT16U addr,INT8U check)
{
	INT16U sendaddr=addr;
	INT8U data=0;

	OUTADDRFM16W08(sendaddr);//!送地址
	__NOP();
	FM16W08CEL;  //!片选
	__NOP();
	__NOP();
	FM16W08OEL; //!开始读取数据
	__NOP();
	__NOP();
	data=GPIO_ReadInputData(GPIOE)&0xff;
	FM16W08CEH;
	__NOP();
	FM16W08OEH;
	__NOP();
	__NOP();
	
	return (data^check);
}
/*************    函数名：BuffReadFM16W08          ***************************
【功能】：
【参数】：
【返回】：
【说明】：
        如果有零，则需要整体进行偏移,程序只有启动的时候读一次，其他时候都是
        写
**************   设计：南下三五年 2012-11-18  10:55:14***********************/
void BuffReadFM16W08(INT16U addr, INT16U len, INT8U *ch,INT8U check)
{
	INT16U address=addr;
	
	taskENTER_CRITICAL(); /*调度器上锁*/
	while(len)
	{
		*ch = ReadDataFM16W08(address,check);
		ch++;
		len--;
		address++;
	}
	taskEXIT_CRITICAL(); /*调度器开锁*/
}
/*************    函数名：WriteDataFM16W08          **************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-18  23:01:30***********************/
void WriteDataFM16W08(INT16U addr, INT8U data,INT8U check)
{
	INT16U senddata=(data^check);
	INT16U sendaddr=addr;
	GPIO_InitTypeDef GPIO_InitStructure;

	//!输出口线设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	FM16W08WEL; //!开始写数据
	__NOP();
	FM16W08CEL;  //!片选
	__NOP();
	__NOP();
	OUTADDRFM16W08(sendaddr);//!送地址
	__NOP();
	OUTDATAFM16W08(senddata);
	__NOP();
	__NOP();
	FM16W08CEH;
	__NOP();
	FM16W08WEH;
	__NOP();
	__NOP();

	

	//!输入口线设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //下拉输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//__NOP();
}

/*************    函数名：BuffWriteFM16W08          **************************
【功能】：
【参数】：
【返回】：
【说明】：
		如果有零，则需要整体进行偏移,程序只有启动的时候读一次，其他时候都是
        写,写的时候，需要随时根据长度和起始地址进行偏移
**************   设计：南下三五年 2012-11-18  23:01:30***********************/
void BuffWriteFM16W08(INT8U *pbuff, INT32U addr, INT32U len, INT8U check)
{
	INT16U senddata=0;
	INT16U sendaddr=addr;
	GPIO_InitTypeDef GPIO_InitStructure;

	//!输出口线设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	while(len)
	{	
		OUTADDRFM16W08(sendaddr);//!送地址
		FM16W08WEL; //!开始写数据
		__NOP();
		FM16W08CEL;  //!片选
		__NOP();
		__NOP();
		senddata=*pbuff;
		senddata^=check;
		OUTDATAFM16W08(senddata);
		__NOP();
		__NOP();
		FM16W08CEH;
		__NOP();
		FM16W08WEH;
		__NOP();
		__NOP();
		
		pbuff++;
		len--;
		sendaddr++;
	}

	//!输入口线设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //下拉输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}








































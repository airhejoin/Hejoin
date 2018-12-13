/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                             大脚丫电子工作室
*
*模  块：USB通信
*设计者：南下三五年
*时  间：2013-08-29 10:28:48
*********************************************************************************
*/
#include".\Lib\includes.h"

#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_pwr.h"

xSemaphoreHandle xUSBRecSemaphore;
xSemaphoreHandle xUSBSendSemaphore;
xSemaphoreHandle xPCASKSemaphore;
xQueueHandle  xQueueServerAsk;


INT8U USBTxMsgBeg;			//处理一个包，
INT8U USBTxMsgEnd;			//写入一个包，
INT8U USBTxMsg[USBMsgSize][65];//数据缓冲区,数组0表示长度

INT8U USBRxMsgBeg;			//处理一个包，
INT8U USBRxMsgEnd;			//写入一个包，
INT8U USBRxMsg[RXUSBMsgSize][65];//数据缓冲区，数组0表示长度

/*************    函数名：InitUSBIO                ****************************
【功能】：初始化USB硬件接口电路
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-17  16:34:53***********************/
void InitUSBIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);  

	/* Select USBCLK source */
  	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  	/* Enable the USB clock */
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*!通过数组查表，提高计算速度*/
INT16U const crctab[16]=
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};

/*************    函数名：Crc16Check              ****************************
【功能】：CRC16校验
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-06-17  22:15:52***********************/
INT16U Crc16Check(INT8U *pdata, INT8U len)
{
	INT16U crc16=0;
	INT8U data=0;

	while(len--!=0)
	{
		data=crc16>>12;
		crc16<<=4;
		crc16^=crctab[data^(*pdata/16)];

		data=crc16>>12;
		crc16<<=4;
		crc16^=crctab[data^(*pdata&0x0f)];
		pdata++;
	}

	return (crc16);
}


/*************    函数名: WaitServerReady  **********************************
【功能】:
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-09-19 15:20:3 **********************/
void WaitServerReadyUSB(void)
{

}

/*************    函数名: AskServerImOK  ***********************************
【功能】:
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-09-19 20:38:46 **********************/
void AskServerImOK(void)
{

}

/*************    函数名: SendUSBNoSend  ***********************************
【功能】:
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-10-23 12:42:4 **********************/
void SendUSBNoSend(void)
{

}
/*************    函数名: SendIOStation  **********************************
【功能】:发送IO板状态
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-10-05 8:0:9 **********************/
void SendIOStation(INT8U sta)
{

}

/*************	  函数名：CheckPCMsg			   **************************
【功能】：
【参数】：
【返回】：
【说明】：
**************	 设计：南下三五年 2012-11-30  10:13:14***********************/
INT8U CheckPCMsg( INT8U *ptr )
{
	INT8U len=*(ptr+5);
	INT16U crc16r=0;
	INT16U crc16t=0;

	crc16r=*(ptr+6+len)+(*(ptr+7+len)*256);
	crc16t=Crc16Check((ptr), (len+6));

	if(crc16r==crc16t)
		return 1;
	else
		return 0;
}

/*************    函数名: SendPlayerBill  ***********************************
【功能】:回送指定玩家的账目信息
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-09-19 13:35:47 **********************/
void SendPlayerBill(INT8U player)
{

}

/*************    函数名: SendSystemPara  ***********************************
【功能】:发送系统参数
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-10-03 16:29:59 **********************/
void SendSystemPara(INT8U sel)
{

}

/*************    函数名: SendServerTick  **********************************
【功能】:
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-11-30 10:25:57 **********************/
void SendServerTick(void)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	INT32U SendCoin=0;

	if(SetCoinOut.SetOutValue>=SetCoinOut.OutValue)
		SendCoin=SetCoinOut.SetOutValue-SetCoinOut.OutValue;
	
	/*!打包数据*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID数据长度
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLTICK;		//!命令
	USBTxMsg[USBTxMsgEnd][8]=0x06;					//!数据包长度

	//!当前出币数量
	USBTxMsg[USBTxMsgEnd][9]=SendCoin;
	USBTxMsg[USBTxMsgEnd][10]=SendCoin>>8;
	USBTxMsg[USBTxMsgEnd][11]=SendCoin>>16;
	USBTxMsg[USBTxMsgEnd][12]=SendCoin>>24;
	
	//!故障信息
	USBTxMsg[USBTxMsgEnd][13]=SetCoinOut.Devstation;

	//!出币完成
	USBTxMsg[USBTxMsgEnd][14]=SetCoinOut.OutCoinReady;

	//!校验
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][15]=crccheck;
	USBTxMsg[USBTxMsgEnd][16]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);		 //!干扰码尾巴
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][20]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][21]='K';
	USBTxMsg[USBTxMsgEnd][22]='J';
	
	//!转换为密文
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * 函数名:AskSetOutCoin
 * 说明: 
 *    应答出币数量设置 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 9:36:16, 2016年7月14日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void AskSetOutCoin(INT8U result,INT32U SetValue)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!打包数据*/ 
	USBTxMsg[USBTxMsgEnd][0]= 64;					//!HID数据长度
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	
	USBTxMsg[USBTxMsgEnd][3]=0;		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=0;
	USBTxMsg[USBTxMsgEnd][5]=0;			
	USBTxMsg[USBTxMsgEnd][6]=0;	
	
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLSETCOINOUT;//!命令
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!数据包长度

	//!执行状态
	USBTxMsg[USBTxMsgEnd][9]=result;
	
	//!当前出币数量
	USBTxMsg[USBTxMsgEnd][10]=SetValue;
	USBTxMsg[USBTxMsgEnd][11]=SetValue>>8;
	USBTxMsg[USBTxMsgEnd][12]=SetValue>>16;
	USBTxMsg[USBTxMsgEnd][13]=SetValue>>24;
	
	//!校验
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!干扰码尾巴
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!转换为密文
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

//	for(i=0;i<length;i++)
//		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * 函数名:AskSetMacReverseRun
 * 说明: 
 *    设置马达反转 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 9:44:33, 2016年7月14日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void AskSetMacReverseRun(INT8U Result)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!打包数据*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID数据长度
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLREVERSERUN;//!命令
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!数据包长度

	//!执行状态
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!当前出币数量
	USBTxMsg[USBTxMsgEnd][10]=0;
	USBTxMsg[USBTxMsgEnd][11]=0;
	USBTxMsg[USBTxMsgEnd][12]=0;
	USBTxMsg[USBTxMsgEnd][13]=0;
	
	//!校验
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!干扰码尾巴
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!转换为密文
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * 函数名:AskForceStop
 * 说明: 
 *    应答强制停止 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 9:48:20, 2016年7月14日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void AskForceStop(INT8U Result,INT32U OutValue)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!打包数据*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID数据长度
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLSTOP;//!命令
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!数据包长度

	//!执行状态
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!当前出币数量
	USBTxMsg[USBTxMsgEnd][10]=OutValue;
	USBTxMsg[USBTxMsgEnd][11]=OutValue>>8;
	USBTxMsg[USBTxMsgEnd][12]=OutValue>>16;
	USBTxMsg[USBTxMsgEnd][13]=OutValue>>24;
	
	//!校验
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!干扰码尾巴
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!转换为密文
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * 函数名:AskContinueRun
 * 说明: 
 *    续转 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 9:56:16, 2016年7月14日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void AskContinueRun(INT8U Result,INT32U OutValue)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!打包数据*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID数据长度
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLCONTINUE;//!命令
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!数据包长度

	//!执行状态
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!当前出币数量
	USBTxMsg[USBTxMsgEnd][10]=OutValue;
	USBTxMsg[USBTxMsgEnd][11]=OutValue>>8;
	USBTxMsg[USBTxMsgEnd][12]=OutValue>>16;
	USBTxMsg[USBTxMsgEnd][13]=OutValue>>24;
	
	//!校验
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!干扰码尾巴
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!转换为密文
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * 函数名:AskCleanRun
 * 说明: 
 *    清币 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 10:59:15, 2016年7月14日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void AskCleanRun(INT8U Result)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!打包数据*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID数据长度
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINCLEAN;//!命令
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!数据包长度

	//!执行状态
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!当前出币数量
	USBTxMsg[USBTxMsgEnd][10]=0;
	USBTxMsg[USBTxMsgEnd][11]=0;
	USBTxMsg[USBTxMsgEnd][12]=0;
	USBTxMsg[USBTxMsgEnd][13]=0;
	
	//!校验
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!干扰码尾巴
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!转换为密文
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * 函数名:SendServerOutCoinReady
 * 说明: 
 *    应答服务器出币完成 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 15:28:35, 2016年7月16日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void SendServerOutCoinReady(void)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	INT32U SendCoin=0;
	
	/*!打包数据*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID数据长度
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!干扰码头
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLOUTREADY;		//!命令
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!数据包长度

	//!当前出币数量
	USBTxMsg[USBTxMsgEnd][9]=SendCoin;
	USBTxMsg[USBTxMsgEnd][10]=SendCoin>>8;
	USBTxMsg[USBTxMsgEnd][11]=SendCoin>>16;
	USBTxMsg[USBTxMsgEnd][12]=SendCoin>>24;
	
	//!故障信息
	USBTxMsg[USBTxMsgEnd][13]=SetCoinOut.Devstation;

	//!校验
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!干扰码尾巴
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!转换为密文
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}
/*************    函数名: ResolvePCMsg  ************************************
【功能】:
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-09-09 22:6:23 **********************/
void ResolvePCMsg(void)
{
	INT8U i=0;
	INT8U check=0;
	INT8U cmd=0;
	INT8U *ptr=NULL;
	INT8U length=0;
	INT8U random = 0;

	while(USBRxMsgBeg!=USBRxMsgEnd)
	{
		//!这里将数据部分的密文转换为明文
		length = (USBRxMsg[USBRxMsgBeg][8]^USBRxMsg[USBRxMsgBeg][3])+4;
		random = USBRxMsg[USBRxMsgBeg][3];

		for(i=0;i<length;i++)
		{
			USBRxMsg[USBRxMsgBeg][7+i]^=random;
		}
		
		//判断数据包校验是否完整
		check=CheckPCMsg(&USBRxMsg[USBRxMsgBeg][3]);

		if(check)
		{
			cmd=(USBRxMsg[USBRxMsgBeg][7]&0x7f);
			
			switch(cmd)
			{
				//!设置退币数量
				case USB_COINSELLSETCOINOUT:
					if((SetCoinOut.OutValue==0)&&(0==SetCoinOut.RunSta))
					{
						ptr=(INT8U *)&SetCoinOut.SetOutValue;//!设置出币数量
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][9+i];

						ptr=(INT8U *)&SetCoinOut.OutValue;//实际出币数量
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][13+i];

						//!启动退币
						SetCoinOut.EnableOut=1;
						SetCoinOut.OutTimeOut=0;
						SetCoinOut.ForwardOrReverse=Motor_Forward;//正转
						SetCoinOut.ChangeSetCnt=0;
						SetCoinOut.ChangeCnt=0;
						SetCoinOut.Devstation=0;
						SetCoinOut.StartOutCoin=2;//？？
						SetCoinOut.OutCoinReady=0;

						//!应答命令
						AskSetOutCoin(0,SetCoinOut.SetOutValue);
					}
					else//!应答命令
					{
						AskSetOutCoin(1,0);
					}
					break;

				//!退币马达反转
				case USB_COINSELLREVERSERUN:
					if(0==SetCoinOut.RunSta)
					{
						SetCoinOut.SetReverseRun=1;
						SetCoinOut.OutTimeOut=0;
						SetCoinOut.Devstation=0;

						AskSetMacReverseRun(0);
					}
					else
						AskSetMacReverseRun(1);
					break;

				//!退币强制停止 
				case USB_COINSELLSTOP:
					if(1==SetCoinOut.RunSta)
					{
						SetCoinOut.EnableOut=0;
						MotoTurnReverseStop();
						MotoTurnForwardStop();

						if(SetCoinOut.Clean==0)
						{
							//!应答当前出币数量
							AskForceStop(0,SetCoinOut.OutValue);
						}

						SetCoinOut.SetOutValue=0;
						SetCoinOut.OutValue=0;
						SetCoinOut.Clean=0;
					}
					else//!应答当前出币数量
					{
						AskForceStop(1,0);
					}
					break;

				//!退币马达续转,为了防止非法断电，有吧台更新续币数量
				case USB_COINSELLCONTINUE:
					if(0==SetCoinOut.RunSta)
					{//马达在停止状态
						ptr=(INT8U *)&SetCoinOut.SetOutValue;
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][9+i];

						ptr=(INT8U *)&SetCoinOut.OutValue;
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][13+i];//

						
						SetCoinOut.EnableOut=1;//!启动退币
						SetCoinOut.OutTimeOut=0;
						SetCoinOut.ForwardOrReverse=Motor_Forward;//正转
						SetCoinOut.ChangeSetCnt=0;
						SetCoinOut.ChangeCnt=0;
						SetCoinOut.Devstation=0;

						AskContinueRun(0,SetCoinOut.SetOutValue);
					}
					else
						AskContinueRun(1,0);
					break;

				//!退币马达清币
				case USB_COINCLEAN:
					if(0==SetCoinOut.RunSta)
					{//马达在停止状态
						SetCoinOut.SetOutValue=60000;//设置出币数量
						SetCoinOut.OutValue=60000;//
						//!启动退币
						SetCoinOut.EnableOut=1;//退币使能
						SetCoinOut.OutTimeOut=0;//
						SetCoinOut.ForwardOrReverse=Motor_Forward;//马达正转
						SetCoinOut.ChangeSetCnt=0;
						SetCoinOut.ChangeCnt=0;
						SetCoinOut.Devstation=0;
						SetCoinOut.Clean=1;

						AskCleanRun(0);
					}
					else
						AskCleanRun(1);
					break;
					
				default:
					break;
			}
		}

		//!清空缓冲区
		for(i=0;i<65;i++)
			USBRxMsg[USBRxMsgBeg][i]=0;
		
		if( ++USBRxMsgBeg >= RXUSBMsgSize)	USBRxMsgBeg=0;
	}
}

/*************    函数名：vConnectPCTask            *************************
【功能】：USB发送任务
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-17  16:53:55***********************/
xTaskHandle xHandleGameDemo;
void vConnectPCTask( void *pvParameters )
{
	INT8U usbsendok=0;
	INT8U i=0;
	INT32U sendtick=0;

	taskENTER_CRITICAL(); /*调度器上锁*/
	USB_Init();
	taskEXIT_CRITICAL(); /*调度器开锁*/
	
	vTaskDelay(5000);

	xSemaphoreTake( xUSBSendSemaphore, ( portTickType ) 0);

	for(;;)
	{
		xSemaphoreTake( xUSBSendSemaphore, ( portTickType ) 10);

		//!发送心跳
		if(++sendtick>=5)
		{
			sendtick=0;
			SendServerTick();
		}
		
		while(USBTxMsgBeg!=USBTxMsgEnd)
		{//有数据需要发送
			do
			{
				vTaskDelay(10);
				usbsendok=GetEPTxStatus(ENDP1);//!查询是否发送完成
			}while(EP_TX_NAK!=usbsendok);
			
			taskENTER_CRITICAL(); /*调度器上锁*/
			UserToPMABufferCopy(&USBTxMsg[USBTxMsgBeg][1], GetEPTxAddr(ENDP1), USBTxMsg[USBTxMsgBeg][0]);
			SetEPTxCount(ENDP1, USBTxMsg[USBTxMsgBeg][0]);                    
			SetEPTxValid(ENDP1);
			taskEXIT_CRITICAL(); /*调度器开锁*/
			
			/*!清除缓冲数据*/
			for(i=0;i<65;i++)
				USBTxMsg[USBTxMsgBeg][i]=0x00;
			
			if( ++USBTxMsgBeg >= USBMsgSize)	USBTxMsgBeg=0;
		}
	}
}

/*************    函数名：vRecMsgPCTask            **************************
【功能】：USB接收任务
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-30  09:44:56***********************/
void vRecMsgPCTask( void *pvParameters )
{
	INT8U i=0;
	
	for(;;)
	{
		//!等待接收信号量
		xSemaphoreTake( xUSBRecSemaphore, ( portTickType ) portMAX_DELAY); 
		taskENTER_CRITICAL(); /*调度器上锁*/
		ResolvePCMsg();
		taskEXIT_CRITICAL(); /*调度器开锁*/
	}
}


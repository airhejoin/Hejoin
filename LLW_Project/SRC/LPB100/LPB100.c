#include"includes.h"

INT8U RxBuffer2[RxBufferSize2];//串口2缓冲区实体
INT8U Rx2Beg=0,Rx2End=0;

S_LPB100 LPB100Sta;
S_PARA SystemPara;
S_COMREC SerDataRec;
INT8U Uart2MsgBeg;			
INT8U Uart2MsgEnd;			
INT8U Uart2Msg[UART_RxSize][UART_LENG];

INT8U Lpb100_SendBuff[LPB100_SEND_SIZE][LPB100_SEND_LEN];
INT8U Lpb100_SendStart;
INT8U Lpb100_SendEnd;

INT8U SeverBuff[LPB100_SEND_SIZE][LPB100_SEND_LEN];
INT8U SeverStart;
INT8U SeverEnd;


S_TCPFLAG TcpFlag;

xSemaphoreHandle xUart3RXSemaphore;  /*串口2接收中断*/

// //!CRC16校验
// INT16U const crctab[16]=
// {
// 	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
// 	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
// };

// /******************************************************************************
//  * 函数名:Crc16
//  * 说明: 
//  *    CRC16校验 
//  * 输入: 
//  * 输出: 
//  * 返回: 
//  * 设计:
//  *****************************************************************************/
// INT16U Crc16(INT8U *pdata, INT8U len)
// {
// 	INT16U crc16=0;
// 	INT8U data;

// 	while(len--!=0)
// 	{
// 		data=crc16>>12;
// 		crc16<<=4;
// 		crc16^=crctab[data^(*pdata/16)];

// 		data=crc16>>12;
// 		crc16<<=4;
// 		crc16^=crctab[data^(*pdata&0x0f)];
// 		pdata++;
// 	}
// 	return (crc16);
// }

/*************    函数名：USART_SendBuff     ***********************
【功能】：串口发送函数、发送缓冲区、数据长度
【参数】：
【返回】：
【说明】：
*************************************/
void USART_SendBuff(USART_TypeDef* USARTx, INT8U *Data,INT16U Len)
{
	INT16U i;
    INT16U j=0;
    
    USART_ClearFlag(USARTx,USART_FLAG_TC);
    
	for(i=0;i<Len;i++,j++)
	{
		USART_SendData(USARTx,Data[j]);
		while( USART_GetFlagStatus(USARTx,USART_FLAG_TC)!= SET);
	}
}
/*---- L E N   S T R ----
【功能】：测量串的长度
【参数】：
****
【返回】：****
【说明】：****
-------------------------------*/
INT8U LenStr(INT8U *ip)
{
	unsigned int len=0;
	while(*ip++)	++len;
	return len;
}

/*********************************************
*函数功能:在pt1中查找pt2,并返回位置，若没有，则返回0xffff
*说明：
	1、调用函数：INT8U LenStr(INT8U *ip) 计算字符串长度
	2、变量说明：		
	3、常量说明：
		
*入口：*pt1 搜索的起始地址，len 搜索的长度，  *pt2对照的样本
*出口：返回相对位置或错误(0xffff)	 
*********************************************/
INT16U CompareStr(INT8U *pt1,INT8U size,INT8U *pt2)
{
	INT16U len=0,place=0,i;
	INT8U Flag=0;

	place=0;
	len=LenStr(pt2);
	while(size--)
	{
		if(*pt1 == *pt2)
		{
			Flag=0;
			for(i=0;i<(len);i++)
			{
				if(*(pt1+i)  != *(pt2+i) )
					{
						Flag=0xff;
						break;
					}
			}
			if(Flag != 0xff)
			{ 
				return place; 
			}
		}
		pt1++;		//越界了怎么办?
		place++;
	}
	return 0xffff;
}


void PrintfA11SMT(char *fmt, ...)
{
	char temp[128]={0};
	va_list argptr;

	va_start(argptr, fmt);
	vsprintf(temp, fmt, argptr);
	va_end(argptr);
	
	A11SMTSengBuffer((INT8U *)temp);
}


void A11SMTSengBuffer(INT8U *data)
{
	while(*data != 0)
	{
		USART2->DR = (u8) *data; 

		/* Loop until the end of transmission */ 
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;


//		 while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
#if 0
		UART1->DR = (u8) *data; 

		/* Loop until the end of transmission */ 
		while(USART_GetFlagStatus(UART1, USART_FLAG_TC) == RESET) 
		{ 

		} 		

		
#endif	
	data++;
	}
}





/*************    函数名：Wait_Answer              ****************************
【功能】：
【参数】：
【返回】：
【说明】：
*************************************/
INT8U Wait_Answer(INT8U *str)
{
	if(Rx2Beg ==Rx2End )	return 0;		//没有包
	if(0xffff != CompareStr(&RxBuffer2[Rx2Beg],Rx2End-Rx2Beg,str))	
	{
		Rx2Beg=Rx2End;
		return 1;
	}
	return 0;
}

/*********************************************
*函数功能:从*str字符串找到数字字符，拷贝 
*说明：
	1、调用函数：
	2、变量说明：		
	3、常量说明：
*入口：*str 源  *Data 放数据的地方，
			Len要拷贝的数据长度(指定0时拷贝所有连续的数据)
*出口：操作成功" TRUE"	 
*********************************************/
INT8U CopyNum(INT8U *str,INT8U *Data,INT8U Len)
{
	INT8U ErrCnt=0;
	
	do{
	if( (*str>='0') && (*str<='9') )//找到起始数据
		{
		if(0==Len)
			{
			while(1)
				{
				*(Data++)=*(str++);
				if((*str<'0') || (*str>'9')) return 1;
				}
			}
		else {
			while(Len--)
				{
				*(Data++)=*(str++);
				if((*str<'0') || (*str>'9')) break;
				}
			if(0==Len) return 1;
			else return 0;
			}
		}
	if(++ErrCnt>200) return 0;	//防错
	}while(*(str++));
	return 0;
}

/*********************************************
*函数功能: 擦除接收短信的串口缓冲区
*说明：
	1、调用函数：
	2、变量说明：
	3、常量说明：
*入口：
*出口：	
*********************************************/
void InitRx2Buffer(void)
{
	INT16U i;
	Rx2Beg=0;
	Rx2End=0;
	for(i=0;i<RxBufferSize2;i++)
		{RxBuffer2[i]=0;}		
}

/******************************************************************************
 * 函数名:LPB100Init
 * 说明: 
 *    LPB100初始化 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void LPB100Init(void)
{
	INT32U i=0;
	
	//!硬件复位
	LPB100RESETL;
	vTaskDelay(50);
	LPB100RESETH;

	//!等待模块正常启动
	vTaskDelay(3000);

	//!这里做超时判断,超时表示模块坏掉
	LPB100Sta.Station=LPB100_NORMAL;
	do
	{
		if(++i>=20*30)
		{
			i=0;
			LPB100Sta.Station=LPB100_HARDWRONG;
			break;
		}
		vTaskDelay(50);
	}while(1==GPIO_ReadInputDataBit(GPIOC, LPB100READYIO));//(1==GPIO_ReadInputDataBit(GPIOC, LPB100READYIO));

#if 0
	if((MyPacket.AdvanceSet.Address!=MyPacket.AdvanceSet.AddressBack)||
	(MyPacket.AdvanceSet.GameroomSNBack!=MyPacket.Children.GameroomSN))//不同了，表示了改变了ip地址，要设置初始化
	{
		return;
	}
	if(FLAG.CanOrWifi==COM_WIFISEL)
	{
		vTaskDelay(2000);
		do
		{
			if(++i>=20*60)
			{
				i=0;
				LPB100Sta.Station=LPB100_APWRONG;
				break;
			}
			vTaskDelay(50);
		}while(1==GPIO_ReadInputDataBit(GPIOC, LPB100LINKIO));

	}
	//!等待模块连接到AP,超时表示AP没有打开，或者服务器没有打开，或者
	//!模块本身的设置有问题

#endif	
}

/******************************************************************************
 * 函数名:LPB100Set
 * 说明: 
 *    LPB100配置 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void LPB100Set(void)
{
	TcpFlag.RecMode=0;
		//启动前要给他发个数据包，否则下面发 "+++"会出现不正常现象
		USART_SendData(USART2,0);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		vTaskDelay(5000);
//		PrintfA11SMT("AT+NETP=TCP,CLIENT,%d,",1234);//服务器地址
//		DoWait("192.168.1.114\xd","+ok");
		vTaskDelay(1000);
	 
		taskENTER_CRITICAL(); 
		BuffWriteFM24C04((u8*)&SYSTEMSET,SystemInitAddr,sizeof(SYSTEMSET));
		taskEXIT_CRITICAL(); 
}

/******************************************************************************
 * 函数名:LPB100GetWSSID
 * 说明: 
 *    获取信号强度 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
INT8S LPB100GetWSSID(void)
{
	INT8U temp[12]={0};
	//!切换到AT命令行模式
	DoWait("+++","a");	

	//!等待进入命令行模式
	DoWait("a","+ok");

	//!关闭回显
	DoWait("AT+E\xd","+ok");

	//!获取信号强度
	DoWait("AT+WSLQ\xd","+ok=");

	//!得到实际的数值
	if(CopyNum(RxBuffer2,temp,0))
	{
		if(temp[2]=='0')
		{
			DoWait("AT+ENTM\xd","+ok");
			return 100;
		}
		else
		{
			DoWait("AT+ENTM\xd","+ok");
			return (temp[0]-0x30)*10+(temp[1]-0x30);
		}
	}
	else
	{
		DoWait("AT+ENTM\xd","+ok");
		return 0;
	}
}

/******************************************************************************
 * 函数名:LPB100Scan
 * 说明: 
 *    LPB100运行状态监测 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void LPB100Scan(void)
{
	INT8U cmd=0;
	
}

/******************************************************************************
 * 函数名:LPB100Reset
 * 说明: 
 *    LPB100复位 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void LPB100Reset(INT32U Reset)
{
	if(Reset)
		return;

	//!硬件复位
//	LPB100RESETL;
//	vTaskDelay(50);
//	LPB100RESETH;
}

/*************    函数名: SendUsartCom  ********************************
【功能】:跟主机通信，发送数据函数
【参数】:
【返回】:
【说明】:
************************************/
void SendUsartCom(void)
{  
	INT8U i=0;
	INT8U len=0;
	while(Lpb100_SendStart!=Lpb100_SendEnd)
	{
		len=Lpb100_SendBuff[Lpb100_SendEnd][2]+10;
		
		taskENTER_CRITICAL(); 
		USART_SendBuff(USART2,(INT8U*)&Lpb100_SendBuff[Lpb100_SendEnd][0],len);
		//USART_SendBuff(UART4,(INT8U*)&Lpb100_SendBuff[Lpb100_SendEnd][0],len);
		taskEXIT_CRITICAL();


		for(i=0;i<len;i++)
		{
			Lpb100_SendBuff[Lpb100_SendEnd][i]=0;
		}
		
		if(++Lpb100_SendEnd>=LPB100_SEND_SIZE)
			Lpb100_SendEnd=0;			
	}
}








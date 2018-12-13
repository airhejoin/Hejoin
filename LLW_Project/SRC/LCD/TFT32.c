#include"includes.h"
#define MSG_HEADER 0xfd

xSemaphoreHandle xRS485RecSemaphore,xGsmRecSemaphore;
xSemaphoreHandle xRS485SendSemaphore,xTftCtlSemaphore,xGsmSendSemaphore;
xQueueHandle     xRS485AskQueue,xGsmAskQueue; 

S_COM RS485Rec,RS485Send,GsmSend,GsmRec;

INT8U TFT_SendBuf[100]={0};

//XOR检验
u8 XorCalculate(u8 *dat,u8 len)
{
	u8 temp=0,i=0;
	for(;i<len;i++)
	{
		temp^=dat[i];
	}
	return temp;
}
/******************************************************************************
 * 函数名:CheckPacket
 * 说明: 
 *    验证包的完整性 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 0:43:17, 2016年7月18日, DAVID
 * -----------------------------------
 *****************************************************************************/
u8 CheckPacket(INT8U *data , INT8U len)
{
	u8 code=0;
	if(data[0]==MSG_HEADER)//帧头检测
	{
		code=XorCalculate(&data[0],data[1]-1);//异或校验
		if(code==data[data[1]-1])
			return 1;
		else
			return 0;		
	}
	else
		return 0;
}
/**********	********************************************************************
 * 函数名:ComSendPacket
 * 说明: 串口发送 
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void ComSendPacket(u8 cmd,u8 len, u8 *buf)
{
	u8 i=0;
	/*!打包数据*/ 
	RS485Send.Msg[RS485Send.MsgEnd][0]=MSG_HEADER;			
	RS485Send.Msg[RS485Send.MsgEnd][1]=4+len;		//数据长度////////////////////
	RS485Send.Msg[RS485Send.MsgEnd][2]=cmd;		//!命令
	for(i=0;i<len;i++)
	{
		RS485Send.Msg[RS485Send.MsgEnd][3+i]=buf[i];
	}
	RS485Send.Msg[RS485Send.MsgEnd][3+len]=XorCalculate(RS485Send.Msg[RS485Send.MsgEnd],3+len);
	if( ++RS485Send.MsgEnd >= MAXMSGSIZE)	RS485Send.MsgEnd=0;
	xSemaphoreGive( xRS485SendSemaphore );
}

/******************************************************************************
 * 函数名:ComSendComNoAsk
 * 说明: 
 *    发送数据给安卓板不需要应答 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 * 11:58:3, 2016年9月02日, DAVID
 * -----------------------------------
 *****************************************************************************/
void ComSendNoAsk(INT8U cmd, INT8U len, INT8U *data)
{
	ComSendPacket(cmd,len,data);
}
/******************************************************************************
 * 函数名:ComSendWaitAsk
 * 说明: 发送数据给安卓板，需要应答 
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void ComSendWaitAsk(INT8U Cmd, INT8U Len, INT8U *Data, INT8U AskCmd, INT8U TimeOut)
{
	INT8U RecCmd=0;
	INT8U SendCnt=0;
//!清空当前队列，等待数据
	xQueueReceive( xRS485AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xRS485AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xRS485AskQueue, &( RecCmd ), 0 );
	RecCmd=0;
	do
	{
		if(RecCmd==AskCmd)
		{
			break;
		}
		else
		{
			if(TimeOut)
			{
				if(++SendCnt>TimeOut)
				{
					break;
				}
			}
		}
		ComSendPacket(Cmd,Len,Data);
	}while((pdTRUE!=xQueueReceive( xRS485AskQueue, &( RecCmd ), 100 )));
	//!清空当前队列，等待数据
	xQueueReceive( xRS485AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xRS485AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xRS485AskQueue, &( RecCmd ), 0 );
}
/**********	********************************************************************
 * 函数名:TFT32_SetResetvoid
 * 说明: 液晶屏复位指令
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_SetReset(void)
{
	ComSendWaitAsk(CMD_RESET,0,&TFT_SendBuf[0],CMD_RESET,3);
}

/**********	********************************************************************
 * 函数名:TFT32_SetResetvoid
 * 说明: 读取液晶屏软件版本指令
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_ReadVersion(void)
{
	ComSendWaitAsk(CMD_VERSION,0,&TFT_SendBuf[0],CMD_VERSION,3);
}

/**********	********************************************************************
 * 函数名:TFT32_DisplayVersion
 * 说明: 显示液晶版本指令
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DisplayVersion(u16 x,u16 y,u8 font,u16 color)
{
	TFT_SendBuf[0]=x>>8;
	TFT_SendBuf[1]=x&0xff;
	TFT_SendBuf[2]=y>>8;
	TFT_SendBuf[3]=y&0xff;
	TFT_SendBuf[4]=font;
	TFT_SendBuf[5]=color>>8;
	TFT_SendBuf[6]=color;
	ComSendWaitAsk(CMD_DISPVERSION,7,&TFT_SendBuf[0],CMD_DISPVERSION,3);
}
/**********	********************************************************************
 * 函数名:TFT32_DisplayPic
 * 说明: 显示图片指令
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DisplayPic(u16 x,u16 y,u8 picid)
{
	TFT_SendBuf[0]=x>>8;
	TFT_SendBuf[1]=x&0xff;
	TFT_SendBuf[2]=y>>8;
	TFT_SendBuf[3]=y&0xff;
	TFT_SendBuf[4]=picid;
	ComSendWaitAsk(CMD_DISPPIC,5,&TFT_SendBuf[0],CMD_DISPPIC,3);
}
/**********	********************************************************************
 * 函数名:TFT32_ScreenClear
 * 说明: 清屏指令
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_ScreenClear(u8 color)
{
	TFT_SendBuf[0]=color>>8;
	TFT_SendBuf[1]=color;
	ComSendWaitAsk(CMD_SCREENCLEAR,2,&TFT_SendBuf[0],CMD_SCREENCLEAR,3);
}

/**********	********************************************************************
 * 函数名:TFT32_RefreshBlock
* 说明: 块更新: x1y1x2y2 x3 y3 picid:图片编号
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_RefreshBlock(u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u8 picid)
{
	TFT_SendBuf[0]=x1>>8;
	TFT_SendBuf[1]=x1&0xff;
	TFT_SendBuf[2]=y1>>8;
	TFT_SendBuf[3]=y1&0xff;
	
	TFT_SendBuf[4]=x2>>8;
	TFT_SendBuf[5]=x2&0xff;
	TFT_SendBuf[6]=y2>>8;
	TFT_SendBuf[7]=y2&0xff;
	
	TFT_SendBuf[8]=x3>>8;
	TFT_SendBuf[9]=x3&0xff;
	TFT_SendBuf[10]=y3>>8;
	TFT_SendBuf[11]=y3&0xff;

	TFT_SendBuf[12]=picid;
	ComSendWaitAsk(CMD_AREAFREASH,13,&TFT_SendBuf[0],CMD_AREAFREASH,3);
}

/**********	********************************************************************
 * 函数名:TFT32_DispString
* 说明: 显示字符串函数
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DispString(u16 x,u16 y,u8 font,u16 color,u8 mode,u8 len,u8 *string)
{
	u8 i=0;
	TFT_SendBuf[0]=x>>8;
	TFT_SendBuf[1]=x&0xff;
	TFT_SendBuf[2]=y>>8;
	TFT_SendBuf[3]=y&0xff;
	
	TFT_SendBuf[4]=font;
	TFT_SendBuf[5]=color>>8;
	TFT_SendBuf[6]=color&0xff;
	TFT_SendBuf[7]=mode;
	for(i=0;i<len;i++)
	{
		TFT_SendBuf[8+i]=string[i];
	}
	ComSendWaitAsk(CMD_SHOWSTRING,8+len,&TFT_SendBuf[0],CMD_SHOWSTRING,3);
}
/**********	********************************************************************
 * 函数名:TFT32_DispNum
* 说明: 显示数字函数
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DispNum(u16 x,u16 y,u8 font,u16 color,u8 mode,u32 num,u8 size)
{
	u8 i=0;
	TFT_SendBuf[0]=x>>8;
	TFT_SendBuf[1]=x&0xff;
	TFT_SendBuf[2]=y>>8;
	TFT_SendBuf[3]=y&0xff;
	
	TFT_SendBuf[4]=font;
	TFT_SendBuf[5]=color>>8;
	TFT_SendBuf[6]=color&0xff;
	TFT_SendBuf[7]=mode;
	
	TFT_SendBuf[8]=num>>24;
	TFT_SendBuf[9]=num>>16;
	TFT_SendBuf[10]=num>>8;
	TFT_SendBuf[11]=num;
	TFT_SendBuf[12]=size;
	ComSendWaitAsk(CMD_SHOWNUM,13,&TFT_SendBuf[0],CMD_SHOWSTRING,3);
}
/**********	********************************************************************
 * 函数名:TFT32_DrawPoint
* 说明: 画点函数
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DrawPoint(u16 x,u16 y,u16 color)
{
	TFT_SendBuf[0]=x>>8;
	TFT_SendBuf[1]=x&0xff;
	TFT_SendBuf[2]=y>>8;
	TFT_SendBuf[3]=y&0xff;
	TFT_SendBuf[4]=color>>8;
	TFT_SendBuf[5]=color&0xff;
	ComSendWaitAsk(CMD_DRAWPOINT,6,&TFT_SendBuf[0],CMD_DRAWPOINT,3);
}

/**********	********************************************************************
 * 函数名:TFT32_DrawLine
* 说明: 画线函数
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	TFT_SendBuf[0]=x1>>8;
	TFT_SendBuf[1]=x1&0xff;
	TFT_SendBuf[2]=y1>>8;
	TFT_SendBuf[3]=y1&0xff;
	
	TFT_SendBuf[4]=x2>>8;
	TFT_SendBuf[5]=x2&0xff;
	TFT_SendBuf[6]=y2>>8;
	TFT_SendBuf[7]=y2&0xff;
	
	TFT_SendBuf[8]=color>>8;
	TFT_SendBuf[9]=color&0xff;
	ComSendWaitAsk(CMD_DRAWLINE,10,&TFT_SendBuf[0],CMD_DRAWLINE,3);
}

/**********	********************************************************************
 * 函数名:TFT32_DrawCircle
* 说明: 画圆函数
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DrawCircle(u16 x,u16 y,u16 r,u16 color)
{
	TFT_SendBuf[0]=x>>8;
	TFT_SendBuf[1]=x&0xff;
	TFT_SendBuf[2]=y>>8;
	TFT_SendBuf[3]=y&0xff;
	
	TFT_SendBuf[4]=r>>8;
	TFT_SendBuf[5]=r&0xff;
	TFT_SendBuf[6]=color>>8;
	TFT_SendBuf[7]=color&0xff;
	ComSendWaitAsk(CMD_DRAWCIRCLE,8,&TFT_SendBuf[0],CMD_DRAWCIRCLE,3);
}

/**********	********************************************************************
 * 函数名:TFT32_DrawRectangle
* 说明: 画框函数
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_DrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	TFT_SendBuf[0]=x1>>8;
	TFT_SendBuf[1]=x1&0xff;
	TFT_SendBuf[2]=y1>>8;
	TFT_SendBuf[3]=y1&0xff;
	TFT_SendBuf[4]=x2>>8;
	TFT_SendBuf[5]=x2&0xff;
	TFT_SendBuf[6]=y2>>8;
	TFT_SendBuf[7]=y2&0xff;
	TFT_SendBuf[8]=color>>8;
	TFT_SendBuf[9]=color&0xff;
	ComSendWaitAsk(CMD_DRAWRECTANGLE,10,&TFT_SendBuf[0],CMD_DRAWRECTANGLE,3);
}

/**********	********************************************************************
 * 函数名:TFT32_FillBlock
* 说明: 块填充
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_FillBlock(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	TFT_SendBuf[0]=x1>>8;
	TFT_SendBuf[1]=x1&0xff;
	TFT_SendBuf[2]=y1>>8;
	TFT_SendBuf[3]=y1&0xff;
	TFT_SendBuf[4]=x2>>8;
	TFT_SendBuf[5]=x2&0xff;
	TFT_SendBuf[6]=y2>>8;
	TFT_SendBuf[7]=y2&0xff;
	TFT_SendBuf[8]=color>>8;
	TFT_SendBuf[9]=color&0xff;//15552468330
	ComSendWaitAsk(CMD_BLOCKFILL,10,&TFT_SendBuf[0],CMD_BLOCKFILL,3);
}

/**********	********************************************************************
 * 函数名:TFT32_FillBlock
* 说明: 块填充
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void TFT32_ReadColor(u16 x,u16 y)
{
	TFT_SendBuf[0]=x>>8;
	TFT_SendBuf[1]=x&0xff;
	TFT_SendBuf[2]=y>>8;
	TFT_SendBuf[3]=y&0xff;
	ComSendWaitAsk(CMD_READCOLOR,4,&TFT_SendBuf[0],CMD_READCOLOR,3);
}

/******************************************************************************
 * 函数名:ProcessComMsg
 * 说明: 
 *    分机通信处理 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 12:0:30, 2016年7月15日, DAVID
 * -----------------------------------
 *****************************************************************************/
void ProcessComMsg(void)
{
	u8 check=0;
	u8 cmd=0;
	u8 i=0,len=0,FontSize=0,mode=0;
	u16 x1,y1,x2,y2,x3,y3,r;
	while(RS485Rec.MsgBegin!=RS485Rec.MsgEnd)
	{
		//!检测包的完整性
		check=CheckPacket(&RS485Rec.Msg[RS485Rec.MsgBegin][0],RS485Rec.Msg[RS485Rec.MsgBegin][1]);
		if(check)
		{
			cmd=RS485Rec.Msg[RS485Rec.MsgBegin][2];
			switch(cmd)
			{		
				case CMD_RESET://复位指令
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_VERSION://版本固件读取
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_DISPVERSION:
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_DISPPIC://显示图片
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_SCREENCLEAR://清屏
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_AREAFREASH://区域刷新
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_SHOWSTRING://显示字符
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_SHOWNUM://显示数字
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_MIXDISP://混合显示
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_DRAWPOINT://画点
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_DRAWLINE://画线
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_DRAWCIRCLE://画圆
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_DRAWRECTANGLE://画框
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_BLOCKFILL:
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_ONOFF://显示开关
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_BACKLIGHT://背光调节
					while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				case CMD_READCOLOR:
				while(pdPASS != xQueueSend( xRS485AskQueue, &cmd, 10 ));//向队列填入命令
					break;
				default:
					break;
			}
			
		}
		//!清空数据包
		for(i=0;i<MAXMSGBUFFERLEN;i++)
		{
			RS485Rec.Msg[RS485Rec.MsgBegin][i]=0;
		}
		//!循环处理数据包
		if( ++RS485Rec.MsgBegin >= MAXMSGSIZE)	RS485Rec.MsgBegin=0;
	}
}
/******************************************************************************
 * 函数名:vComProcessTask
 * 说明: 
 *    分机通信任务 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 11:56:57, 2016年7月15日, DAVID
 * -----------------------------------
 *****************************************************************************/
void vRS485ProcessTask( void *pvParameters )
{
	for(;;)
	{
		xSemaphoreTake( xRS485RecSemaphore, ( portTickType )100);
		ProcessComMsg();
	}
}

/******************************************************************************
 * 函数名:vComSendTask
 * 说明: 
 *    串口发送任务
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 23:57:17, 2016年7月17日, DAVID
 * -----------------------------------
 *****************************************************************************/
void vRS485SendTask( void *pvParameters )
{
	INT32U i=0;
	INT32U len=0;
	
	for(;;)
	{
		xSemaphoreTake( xRS485SendSemaphore, ( portTickType ) 100);
		while(RS485Send.MsgBegin!=RS485Send.MsgEnd)
		{
			//RS485TxEn();
			taskENTER_CRITICAL(); 
			len=RS485Send.Msg[RS485Send.MsgBegin][1];
			for(i=0;i<len;i++)
			{
				USART3->DR = RS485Send.Msg[RS485Send.MsgBegin][i]; 
				while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
			}
			//!清空数据
			memset(&RS485Send.Msg[RS485Send.MsgBegin][0],0,MAXMSGBUFFERLEN);
			taskEXIT_CRITICAL();
			//!循环处理数据包
			if( ++RS485Send.MsgBegin >= MAXMSGSIZE)	RS485Send.MsgBegin=0;	
			///RS485RxEn();
			vTaskDelay(10);//
		}
		//
	}
}
/******************************************************************************
 * 函数名:vComSendTask
 * 说明: 
 *    串口发送任务
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 23:57:17, 2016年7月17日, DAVID
 * -----------------------------------
 *****************************************************************************/
typedef struct{
	u8 Id;
	u8 CointVal;
}S_MAINPAGE;
void vTftCtlTask( void *pvParameters )
{
	u8 buf[5]={0};
	for(;;)
	{
		if(Flag.Step==0)
		{//只在空闲模式下显示
			MainDispProcess();
		}
		else if(Flag.Step==1)
		{
			vTaskDelay(150);
			//SetProcess();
		}
		vTaskDelay(20);
	}
}

#include"includes.h"
#define HEADER1 0x24
#define HEADER2 0x40
typedef struct{
  u8 SysType[2];          //系统设备类型
  u8 FactoryId[4];      //厂家编号
  u8 MachineType[4];    //机台类型
  u8 HardwareVer[4];    //硬件版本
  u8 BoardUniqueId[12];     //板子唯一ID
}S_DEVICEINFO;
S_DEVICEINFO DeviceInfo={""};

u8 GsmNoAskBuf[MAXMSGBUFFERLEN]={0};
u8 GsmAskBuf[MAXMSGBUFFERLEN]={0};

u8 GetXorCal(u8 *data,u8 len)
{
	u8 i=0;
	u8 xdat=0;
	for(i=0;i<len;i++)
	{
		xdat^=data[i];
	}
	return xdat;
}
/******************************************************************************
 * 函数名:ComSendPacket
 * 说明: 
 *    串口发送 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void GsmSendPacket(u16 cmd, u8 *dat,u8 len,u8 sync)
{
	u16 crccheck=0;
	u8  random=0;
	u8 i=0;
	/*!打包数据*/ 
	GsmSend.Msg[GsmSend.MsgEnd][0]=HEADER1;			
	GsmSend.Msg[GsmSend.MsgEnd][1]=HEADER2;	
	GsmSend.Msg[GsmSend.MsgEnd][2]=cmd>>8;
	GsmSend.Msg[GsmSend.MsgEnd][3]=0xff&cmd;
	GsmSend.Msg[GsmSend.MsgEnd][4]=6+len;	//数据长度////////////////////
	for(i=0;i<len;i++)
	{
		GsmSend.Msg[GsmSend.MsgEnd][5+i]=dat[i];
	}
	GsmSend.Msg[GsmSend.MsgEnd][5+i]=sync+1;//命令唯一码
	GsmSend.Msg[GsmSend.MsgEnd][6+i]=sync+2;//
	GsmSend.Msg[GsmSend.MsgEnd][7+i]=sync+3;//
	GsmSend.Msg[GsmSend.MsgEnd][8+i]=sync+4;//
	GsmSend.Msg[GsmSend.MsgEnd][9+i]=sync+5;//
	GsmSend.Msg[GsmSend.MsgEnd][10+i]=sync+6;//
	
	GsmSend.Msg[GsmSend.MsgEnd][11+i]=GetXorCal(&GsmSend.Msg[GsmSend.MsgEnd][2],len+9);
	GsmSend.Msg[GsmSend.MsgEnd][12+i]=0x0d;//
	GsmSend.Msg[GsmSend.MsgEnd][13+i]=0x0a;//
	//!校验
	//crccheck=Crc16(&GsmSend.Msg[GsmSend.MsgEnd][0], len+10);
//	GsmSend.Msg[GsmSend.MsgEnd][10+len]=crccheck;
//	GsmSend.Msg[GsmSend.MsgEnd][11+len]=crccheck>>8;

	//!转换为密文
//	random=GsmSend.Msg[GsmSend.MsgEnd][3];
//	#if MSG_ENCRYPT
//	for(i=0;i<len+5;i++)
//		GsmSend.Msg[GsmSend.MsgEnd][7+i]^=random;
//	#endif	
	GsmSend.Msg[GsmSend.MsgEnd][MAXMSGBUFFERLEN-1]=len+14;
	if( ++GsmSend.MsgEnd >= MAXMSGSIZE)	GsmSend.MsgEnd=0;
	xSemaphoreGive( xGsmSendSemaphore );
//	if(Flag.TxSync<255)
//		Flag.TxSync++;
//	else
//		Flag.TxSync=0;
}
/******************************************************************************
 * 函数名:GsmSendWaitAsk
 * 说明: 
 *    发送数据给云模块，需要应答 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void GsmSendWaitAsk(u16 Cmd,u8 *Data,u8 len, u16 AskCmd, u8 TimeOut)
{
	u8 RecCmd=0;
	u8 SendCnt=0;
//!清空当前队列，等待数据
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
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
				if(++SendCnt>=TimeOut)
				{
					break;
				}
			}
		}
		GsmSendPacket(Cmd,Data,len,Flag.TxSync);
	}while((pdTRUE!=xQueueReceive( xGsmAskQueue, &( RecCmd ), 3000 )));//3秒重发一次
	if(Flag.TxSync<255)
	{
		Flag.TxSync++;
	}
	else
	{
		Flag.TxSync=0;
	}
	//!清空当前队列，等待数据
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
}
/******************************************************************************
 * 函数名:GsmSendWaitAsk
 * 说明: 
 *    机台发送上电指令给云模块，需要应答 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ClientSendOn(void)
{
	u8 i=0;
	
	GsmAskBuf[0]=0x00;//系统设备类型 (投币器0001)(礼品机0002)(露天模拟机0003)(售币机0004)(促销机0005)(剪刀机0006)(弹珠机0007)(礼品机发送二维码4098)()
	GsmAskBuf[1]=0x02;//设备类型 不可修改
	GsmAskBuf[2]='C';//厂家编号//荣翔对应CG52 不可修改
	GsmAskBuf[3]='G';//厂家编号
	GsmAskBuf[4]='5';//厂家编号
	GsmAskBuf[5]='2';//厂家编号
	#if 1
	GsmAskBuf[6]='R';//机器类型代码 厂家自定义 01彩票机
	GsmAskBuf[7]='X';//机器类型代码 
	GsmAskBuf[8]='0';//机器类型代码
	GsmAskBuf[9]='1';//机器类型代码
	
	GsmAskBuf[10]='R';//硬件程序版本
	GsmAskBuf[11]='X';//硬件程序版本
	GsmAskBuf[12]='0';//硬件程序版本
	GsmAskBuf[13]='1';//硬件程序版本
	#else
  GsmAskBuf[6]=0;//机器类型代码 厂家自定义 01彩票机
	GsmAskBuf[7]=0;//机器类型代码 
	GsmAskBuf[8]=0;//机器类型代码
	GsmAskBuf[9]=0;//机器类型代码
	
	GsmAskBuf[10]=0;//硬件程序版本
	GsmAskBuf[11]=0;//硬件程序版本
	GsmAskBuf[12]=0;//硬件程序版本
	GsmAskBuf[13]=0;//硬件程序版本	
  #endif
	//12byte字节唯一ID
	for(i=0;i<12;i++)
		GsmAskBuf[14+i]=0;
	GsmSendWaitAsk(CMD_CLIENTONSYNC,GsmAskBuf,26,CMD_CLIENTONSYNC,0);
}
/******************************************************************************
 * 函数名:GsmSendWaitAsk
 * 说明: 
 *    机台发送上电指令给云模块，需要应答 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ClientSendUserPara(void)
{
	GsmAskBuf[0]=System.Para.CoinPlay;
  //GsmAskBuf[1]=System.Para.RoundToPrize;
	GsmAskBuf[2]=System.Para.GameVol;
	GsmAskBuf[3]=System.Para.BackVol;
	GsmSendWaitAsk(CMD_CLIENTSETUSER,GsmAskBuf,4,CMD_CLIENTSETUSER,0);
}
/******************************************************************************
 * 函数名:ClientReadUserPara
 * 说明: 
 *    机台发送读自定义参数
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ClientReadUserPara(void)
{
	u8 i=0;
	GsmAskBuf[0]=rand()%255;//取随机数
	GsmAskBuf[1]=rand()%255;
	GsmAskBuf[2]=rand()%255;
	GsmAskBuf[3]=rand()%255;
	GsmAskBuf[4]=rand()%255;
	GsmAskBuf[5]=rand()%255;
	GsmAskBuf[6]=rand()%255;
	GsmAskBuf[7]=rand()%255;
	Flag.RamdomXorData=0;	
	for(i=0;i<8;i++)
	{
		Flag.RamdomXorData^=GsmAskBuf[i];
	}
	GsmSendWaitAsk(CMD_CLIENTREADUSER,GsmAskBuf,8,CMD_CLIENTREADUSER,0);
}
/******************************************************************************
 * 函数名:ClientReadMachineID
 * 说明: 
 *    读取云模块的唯一ID
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ClientReadServerID(void)
{
	u8 i=0;
	GsmSendWaitAsk(CMD_CLIENTREADID,GsmAskBuf,0,CMD_CLIENTREADID,0);
}
/******************************************************************************
 * 函数名:ClientReadUserPara
 * 说明: 
 *    机台发送读自定义参数
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ClientReadServerSta(void)
{
	u8 i=0;
	GsmSendWaitAsk(CMD_CLIENTGETSERVERSTA,GsmAskBuf,8,CMD_CLIENTGETSERVERSTA,2);
}

enum{
	BASEPARA=1,
	PROBPARA,//2
	CURBILLPARA,//3
	TOTALBILLPARA,//4
	RUNCTL,//5
	USERPARA,//6
	PARASET_7,
	PARASET_8,
	PARASET_9,
	PARASET_A,
	PARASET_FF,
};
//云模块读取参数应答
void ReadParaAck(u8 *dat)
{
	u8 i=0;
	/*!打包数据*/ 
	GsmSend.Msg[GsmSend.MsgEnd][0]=HEADER1;			
	GsmSend.Msg[GsmSend.MsgEnd][1]=HEADER2;	
	GsmSend.Msg[GsmSend.MsgEnd][2]='d';
	GsmSend.Msg[GsmSend.MsgEnd][3]='d';
	GsmSend.Msg[GsmSend.MsgEnd][4]=26;	//数据长度////////////////////
	GsmSend.Msg[GsmSend.MsgEnd][5]=dat[0];
	switch(dat[0])
	{
		case BASEPARA://1
    break;
		case PROBPARA://2
			break;
		case CURBILLPARA://3
			
			break;
		case TOTALBILLPARA://4
			
			break;
		case RUNCTL://停机模式控制 5
			break;
		case USERPARA:
			break;
		case PARASET_7:
			//GsmSend.Msg[GsmSend.MsgEnd][6]=Para.UserSet7;
			break;
		case PARASET_8:
			//GsmSend.Msg[GsmSend.MsgEnd][6]=Para.UserSet8;
			break;
		case PARASET_9:
			//GsmSend.Msg[GsmSend.MsgEnd][6]=Para.UserSet9;
			break;
		case PARASET_A:
			//GsmSend.Msg[GsmSend.MsgEnd][6]=Para.UserSetA;
			break;
		case PARASET_FF:
			//GsmSend.Msg[GsmSend.MsgEnd][6]=Para.UserSetFF;
			break;
		default:
			break;
	}
	//唯一码
	for(i=1;i<7;i++)
	{
		GsmSend.Msg[GsmSend.MsgEnd][24+i]=dat[i];
	}
	GsmSend.Msg[GsmSend.MsgEnd][24+i]=GetXorCal(&GsmSend.Msg[GsmSend.MsgEnd][2],29);
	GsmSend.Msg[GsmSend.MsgEnd][25+i]=0x0d;//
	GsmSend.Msg[GsmSend.MsgEnd][26+i]=0x0a;//
	GsmSend.Msg[GsmSend.MsgEnd][MAXMSGBUFFERLEN-1]=34;
	if( ++GsmSend.MsgEnd >= MAXMSGSIZE)	GsmSend.MsgEnd=0;
	xSemaphoreGive( xGsmSendSemaphore );		
}

void ParaSet(u8 *dat)
{
  u32 temp=0;
  if(Flag.Step==1)
    return;
	switch(dat[0])
	{
		case BASEPARA:
			break;
		case PROBPARA:
			break;
		case CURBILLPARA:
			break;
		case TOTALBILLPARA:
			break;
		case RUNCTL:
			break;
		case USERPARA:
			break;
		case PARASET_7:
			//Para.UserSet7=dat[1];
			break;
		case PARASET_8:
			//Para.UserSet8=dat[1];
			break;
		case PARASET_9:
			//Para.UserSet9=dat[1];
			break;
		case PARASET_A:
			//Para.UserSetA=dat[1];
			break;
		case PARASET_FF:
			//Para.UserSetFF=dat[1];
			break;
		default:
			break;
	}
}

void GsmSendAckPacket(u8 *dat)
{
	u8 i=0;
	for(i=0;i<(dat[4]+8);i++)
		GsmSend.Msg[GsmSend.MsgEnd][i]=dat[i];
	GsmSend.Msg[GsmSend.MsgEnd][MAXMSGBUFFERLEN-1]=dat[4]+8;
	if( ++GsmSend.MsgEnd >= MAXMSGSIZE)	GsmSend.MsgEnd=0;
	xSemaphoreGive( xGsmSendSemaphore );
}
/******************************************************************************
 * 函数名:XorCheck
 * 说明: 
 *    验证包的完整性 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
u8 XorCal(u8 *data,u8 len,u8 result)
{
	u8 i=0;
	u8 xdat=0;
	for(i=0;i<len;i++)
	{
		xdat^=data[i];
	}
	if(xdat==result)
		return 1;
	return 0;
}
/******************************************************************************
 * 函数名:CheckPacket
 * 说明: 
 *    验证包的完整性 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
u8 GsmCheckPacket(u8 *data , u8 len)
{
	u8 code=0;
	u8 i=0;
	u16 crc1=0,crc=0;
	if((data[0]==HEADER1)&&(data[1]==HEADER2))//帧头检测
	{
		#if MSG_ENCRYPT
		code=data[3];//密匙
		for(i=0;i<data[2]-7;i++)
		{
			data[7+i]^=code;//解析数据
		}
		#endif
		if(XorCal((u8 *) &data[2],3+data[4],data[5+data[4]]))
			return 1;
		else 
			return 0;
	}
	else
		return 0;
}
u8 SoleIdCheck(u8 *dat)
{
	u8 i=0;
	for(i=0;i<6;i++)
	{
		if(dat[i]!=GsmRec.SoleId[i])
		{
			break;
		}
	}
	if(i==6)
	{//ID相同
		return 0;
	}
	for(i=0;i<6;i++)//保存新的ID
	{
		GsmRec.SoleId[i]=dat[i];
	}
	return 1;
}
/******************************************************************************
 * 函数名:ProcessComMsg
 * 说明: 
 *    分机通信处理 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void GsmProcessComMsg(void)
{
	u8 check=0;
	u16 cmd=0;
	u32 i=0;
	//u8 *ptr=NULL;
	while(GsmRec.MsgBegin!=GsmRec.MsgEnd)
	{
		//!检测包的完整性
		check=GsmCheckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0],GsmRec.Msg[GsmRec.MsgBegin][MAXMSGBUFFERLEN-1]);
		if(check)
		{
			check=SoleIdCheck(&GsmRec.Msg[GsmRec.MsgBegin][GsmRec.Msg[GsmRec.MsgBegin][4]-1]);
			cmd=(GsmRec.Msg[GsmRec.MsgBegin][2]<<8)|GsmRec.Msg[GsmRec.MsgBegin][3];
			if(check)
			{//新唯一码
				switch(cmd)
				{
					case CMD_SERVECOIN://云投币指令 aa
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//原指令应答
						printf("云投币%d",GsmRec.Msg[GsmRec.MsgBegin][5]);
						break;
					case CMD_SERVEONSYNC://云模块上电同步指令 kk 原数据返回
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);
						Flag.ServerOn=1;//云模块已上电
            printf("云上电同步");
						break;
					
					case CMD_SERVESTATUS://云模块状态指令 jj
						Flag.ServerStatus=GsmRec.Msg[GsmRec.MsgBegin][5];//云模块状态
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//应答指令
						break;
					
					case CMD_SERVEREADPARA://云模块查询主板参数ee 返回dd
						ReadParaAck(&GsmRec.Msg[GsmRec.MsgBegin][5]);
						printf("查询主板参数");
						break;
					case CMD_SERVESETDEFAULT://云模块设置主板恢复出厂ff
						SystemParaDefault(SETDEFAULT_BILL_PARA);
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//应答指令
						printf("恢复出厂");
						break;
					case CMD_SERVESETPARA://云模块设置主板参数dd
						ParaSet(&GsmRec.Msg[GsmRec.MsgBegin][5]);//设置参数
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//应答指令
						printf("设置主板参数");
						break;	
					case CMD_SERVERESET://云模块设置主板关机重启清除数据命令mm
//						for(i=0;i<6;i++)
//						{
//							if(System.PassWord[i]!=GsmRec.Msg[GsmRec.MsgBegin][6+i])
//								break;
//						}
						i=6;
            GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//应答指令
						if(i==6) 
						{
							if(GsmRec.Msg[GsmRec.MsgBegin][5]==1)
							{//关机
								printf("云控制关机");
							}
							else if(GsmRec.Msg[GsmRec.MsgBegin][5]==2)
							{//重启
								printf("云控制重启");
                Flag.McuReset=1;
							}
							else if(GsmRec.Msg[GsmRec.MsgBegin][5]==3)
							{//清除数据
								printf("云控制清数据");
							}
						}
						else  
						{//密码错误
							GsmRec.Msg[GsmRec.MsgBegin][5]=0;
						}							
						break;				
					
					//应答处理
						case CMD_CLIENTGETSERVERSTA://机台查询云模块应答
						Flag.ServerStatus=GsmRec.Msg[GsmRec.MsgBegin][5];//云模块状态
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));		
						break;
						
					case CMD_CLIENTREADID://礼品机读取模块唯一码命令am
            printf("读唯一码应答");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));	
						break;
					case CMD_CLIENTREADUSER://读取自定义数据命令应答 dm
						printf("读自定义参数");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;							
					case CMD_CLIENTONSYNC://机台上电指令应答gg
						printf("机台上电应答");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;
	
					case CMD_CLIENTSTATUS://机台上传状态指令bb
						printf("机台上传状态");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;			
					case CMD_CLIENTCOINCNT://机台上传投币数指令cc
						printf("机台上传币数");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;		
					case CMD_CLIENTGIFTOUT://机台上传礼品数指令ii
						printf("机台上传礼品数");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;						
					case CMD_CLIENTERROR://机台上传故障代码指令hh
						printf("机台上传故障");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;	
						
					case CMD_CLIENTSETUSER://礼品机写入厂家自定义参数命令jm
						printf("机台写入自定义");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;							
					default:
						break;
				}
			}
			else
			{//唯一码相同
				GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//应答指令
			}
		}
		//!清空数据包
		for(i=0;i<MAXMSGBUFFERLEN;i++)
		{
			GsmRec.Msg[GsmRec.MsgBegin][i]=0;
		}
		//!循环处理数据包
		if( ++GsmRec.MsgBegin >= MAXMSGSIZE)	GsmRec.MsgBegin=0;
	}
}

/******************************************************************************
 * 函数名:vU2ProcessTask
 * 说明: 
 *    分机通信任务 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void vGsmProcessTask( void *pvParameters )
{
	for(;;)
	{
		xSemaphoreTake( xGsmRecSemaphore, ( portTickType )100);
		GsmProcessComMsg();
	}
}

/******************************************************************************
 * 函数名:vU2SendTask
 * 说明: 
 *    串口2发送任务
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void vGsmSendTask( void *pvParameters )
{
	INT32U i=0;
	INT32U len=0;
	for(;;)
	{
		xSemaphoreTake( xGsmSendSemaphore, ( portTickType ) 1000);
		while(GsmSend.MsgBegin!=GsmSend.MsgEnd)
		{
			taskENTER_CRITICAL(); 
			len=GsmSend.Msg[GsmSend.MsgBegin][MAXMSGBUFFERLEN-1];//0xfc
			for(i=0;i<len;i++)
			{
				UART4->DR = GsmSend.Msg[GsmSend.MsgBegin][i]; 
				while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
			}
			//!清空数据
			memset(&GsmSend.Msg[GsmSend.MsgBegin][0],0,MAXMSGBUFFERLEN);
			taskEXIT_CRITICAL();
			//!循环处理数据包
			if( ++GsmSend.MsgBegin >= MAXMSGSIZE)	GsmSend.MsgBegin=0;
			vTaskDelay(10);
		}
		vTaskDelay(50);
	}
}

void vGsmTask( void *pvParameters )
{
	u8 times=0;
  u8 i=0;
	u16 err_old=0;
  YUN_RSTL();
	while(Flag.ServerOn==0)
	{
		vTaskDelay(100);
		if(++times<200)
			break;
	}
	times=0;
	Flag.ServerOn=1;
	ClientSendOn();//发送上电指令
	for(;;)
	{
		if(++times>30)
		{
			times=0;
			ClientReadServerSta();//
		}
		vTaskDelay(100);
	}
}


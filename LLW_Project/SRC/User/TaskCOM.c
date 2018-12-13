#include"includes.h"
#include<string.h>
S_COM U2Rec,U3Rec,U4Rec;
S_COM U2Tx,U3Tx,U4Tx;


u8 SendNoAskBuf[MAXMSGBUFFERLEN]={0};
u8 SendAskBuf[MAXMSGBUFFERLEN]={0};

//!串口接收信号量
xSemaphoreHandle xU2RecSemaphore,xU3RecSemaphore,xU4RecSemaphore;
//!串口发送信号量
xSemaphoreHandle xU2TxSemaphore,xU2TxSemaphore,xU4TxSemaphore;
//!串口应答队列
xQueueHandle     xU2AskQueue,xU3AskQueue,xU4AskQueue; 

#define MSG_ENCRYPT 0//进行异或加密
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
void ComSendPacket(u16 cmd, u8 *dat,u8 len,u8 sync)
{
	u16 crccheck=0;
	u8  random=0;
	u8 i=0;
	/*!打包数据*/ 
	U2Tx.Msg[U2Tx.MsgEnd][0]=HEADER1;			
	U2Tx.Msg[U2Tx.MsgEnd][1]=HEADER2;	
	U2Tx.Msg[U2Tx.MsgEnd][2]=cmd>>8;
	U2Tx.Msg[U2Tx.MsgEnd][3]=0xff&cmd;
	U2Tx.Msg[U2Tx.MsgEnd][4]=6+len;	//数据长度////////////////////
	for(i=0;i<len;i++)
	{
		U2Tx.Msg[U2Tx.MsgEnd][5+i]=dat[i];
	}
	U2Tx.Msg[U2Tx.MsgEnd][5+i]=sync+1;//命令唯一码
	U2Tx.Msg[U2Tx.MsgEnd][6+i]=sync+2;//
	U2Tx.Msg[U2Tx.MsgEnd][7+i]=sync+3;//
	U2Tx.Msg[U2Tx.MsgEnd][8+i]=sync+4;//
	U2Tx.Msg[U2Tx.MsgEnd][9+i]=sync+5;//
	U2Tx.Msg[U2Tx.MsgEnd][10+i]=sync+6;//
	
	U2Tx.Msg[U2Tx.MsgEnd][11+i]=GetXorCal(&U2Tx.Msg[U2Tx.MsgEnd][2],len+9);
	U2Tx.Msg[U2Tx.MsgEnd][12+i]=0x0d;//
	U2Tx.Msg[U2Tx.MsgEnd][13+i]=0x0a;//
	//!校验
	//crccheck=Crc16(&U2Tx.Msg[U2Tx.MsgEnd][0], len+10);
//	U2Tx.Msg[U2Tx.MsgEnd][10+len]=crccheck;
//	U2Tx.Msg[U2Tx.MsgEnd][11+len]=crccheck>>8;

	//!转换为密文
//	random=U2Tx.Msg[U2Tx.MsgEnd][3];
//	#if MSG_ENCRYPT
//	for(i=0;i<len+5;i++)
//		U2Tx.Msg[U2Tx.MsgEnd][7+i]^=random;
//	#endif	
	U2Tx.Msg[U2Tx.MsgEnd][MAXMSGBUFFERLEN-1]=len+14;
	if( ++U2Tx.MsgEnd >= MAXMSGSIZE)	U2Tx.MsgEnd=0;
	xSemaphoreGive( xU2TxSemaphore );
//	if(Flag.TxSync<255)
//		Flag.TxSync++;
//	else
//		Flag.TxSync=0;
}
/******************************************************************************
 * 函数名:ComSendWaitAsk
 * 说明: 
 *    发送数据给云模块，需要应答 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ComSendWaitAsk(u16 Cmd,u8 *Data,u8 len, u16 AskCmd, u8 TimeOut)
{
	u8 RecCmd=0;
	u8 SendCnt=0;
//!清空当前队列，等待数据
	xQueueReceive( xU2AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xU2AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xU2AskQueue, &( RecCmd ), 0 );
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
		ComSendPacket(Cmd,Data,len,Flag.TxSync);
	}while((pdTRUE!=xQueueReceive( xU2AskQueue, &( RecCmd ), 3000 )));//3秒重发一次
	if(Flag.TxSync<255)
	{
		Flag.TxSync++;
	}
	else
	{
		Flag.TxSync=0;
	}
	//!清空当前队列，等待数据
	xQueueReceive( xU2AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xU2AskQueue, &( RecCmd ), 0 );
	xQueueReceive( xU2AskQueue, &( RecCmd ), 0 );
}

/******************************************************************************
 * 函数名:ComSendComNoAsk
 * 说明: 
 *    发送数据给安卓板不需要应答 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ComSendNoAsk(u16 cmd,u8 *data,u8 len)
{
	ComSendPacket(cmd,data,len,Flag.TxSync);
	if(Flag.TxSync<255)
	{
		Flag.TxSync++;
	}
	else
	{
		Flag.TxSync=0;
	}
}

void ComSendAckPacket(u8 *dat)
{
	u8 i=0;
	for(i=0;i<(dat[4]+8);i++)
		U2Tx.Msg[U2Tx.MsgEnd][i]=dat[i];
	U2Tx.Msg[U2Tx.MsgEnd][MAXMSGBUFFERLEN-1]=dat[4]+8;
	if( ++U2Tx.MsgEnd >= MAXMSGSIZE)	U2Tx.MsgEnd=0;
	xSemaphoreGive( xU2TxSemaphore );
}

void ComSendParaAck(void)
{
	
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
u8 CheckPacket(u8 *data , u8 len)
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
//		crc=(u16)(data[data[2]-2]|((u16)data[data[2]-1]<<8));//!判断CRC校验
//		crc1=Crc16((u8 *) &data[0],data[2]-2);
//		if(crc==crc1)
//			return 1;
//		else
//			return 0;		
	}
	else
		return 0;
}
u8 SoleIdCheck(u8 *dat)
{
	u8 i=0;
	for(i=0;i<6;i++)
	{
		if(dat[i]!=U2Rec.SoleId[i])
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
		U2Rec.SoleId[i]=dat[i];
	}
	return 1;
}
enum{
	SYSPARA=1,
	BILLPARA,
	TICKETPARA,
	BILLAWARD,
	STOPRUN,
	PARASET_6,
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
	U2Tx.Msg[U2Tx.MsgEnd][0]=HEADER1;			
	U2Tx.Msg[U2Tx.MsgEnd][1]=HEADER2;	
	U2Tx.Msg[U2Tx.MsgEnd][2]='d';
	U2Tx.Msg[U2Tx.MsgEnd][3]='d';
	U2Tx.Msg[U2Tx.MsgEnd][4]=26;	//数据长度////////////////////
	U2Tx.Msg[U2Tx.MsgEnd][5]=dat[0];
	switch(dat[0])
	{
		case SYSPARA://1
//			U2Tx.Msg[U2Tx.MsgEnd][6]=System.Para.CoinPlay;
//			U2Tx.Msg[U2Tx.MsgEnd][7]=1;
//      U2Tx.Msg[U2Tx.MsgEnd][8]=System.Para.PlayTime>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][9]=System.Para.PlayTime&0xff;
//      U2Tx.Msg[U2Tx.MsgEnd][10]=System.Para.GameSoundVol;   //游戏音量
//      U2Tx.Msg[U2Tx.MsgEnd][11]=System.Para.BackSoundVol;   //背景音量
//			U2Tx.Msg[U2Tx.MsgEnd][12]=ENABLE;
    break;
		case BILLPARA://2
//			U2Tx.Msg[U2Tx.MsgEnd][6]=System.Bill.TotalCoin>>24;
//			U2Tx.Msg[U2Tx.MsgEnd][7]=System.Bill.TotalCoin>>16;
//			U2Tx.Msg[U2Tx.MsgEnd][8]=System.Bill.TotalCoin>>8;
//			U2Tx.Msg[U2Tx.MsgEnd][9]=System.Bill.TotalCoin&0xff;
//			U2Tx.Msg[U2Tx.MsgEnd][10]=0;
//			U2Tx.Msg[U2Tx.MsgEnd][11]=0;
//			U2Tx.Msg[U2Tx.MsgEnd][12]=0;
//			U2Tx.Msg[U2Tx.MsgEnd][13]=0;
//			U2Tx.Msg[U2Tx.MsgEnd][14]=System.Backup.TotalCoin>>24;
//			U2Tx.Msg[U2Tx.MsgEnd][15]=System.Backup.TotalCoin>>16;
//			U2Tx.Msg[U2Tx.MsgEnd][16]=System.Backup.TotalCoin>>8;
//			U2Tx.Msg[U2Tx.MsgEnd][17]=System.Backup.TotalCoin&0xff;
//			U2Tx.Msg[U2Tx.MsgEnd][18]=0;
//			U2Tx.Msg[U2Tx.MsgEnd][19]=0;
//			U2Tx.Msg[U2Tx.MsgEnd][20]=0;
//			U2Tx.Msg[U2Tx.MsgEnd][21]=0;
    
//			U2Tx.Msg[U2Tx.MsgEnd][18]=System.Bill.TotalSmallPrize>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][19]=System.Bill.TotalSmallPrize&0xff;
//      U2Tx.Msg[U2Tx.MsgEnd][20]=System.Bill.TotalBigPrize>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][21]=System.Bill.TotalBigPrize&0xff;
			break;
		case TICKETPARA://3
//      U2Tx.Msg[U2Tx.MsgEnd][6]=System.Para.TicketEn;
//      U2Tx.Msg[U2Tx.MsgEnd][7]=System.Para.HoleTicket[0]>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][8]=System.Para.HoleTicket[0];
//      U2Tx.Msg[U2Tx.MsgEnd][9]=System.Para.HoleTicket[1]>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][10]=System.Para.HoleTicket[1];
//      U2Tx.Msg[U2Tx.MsgEnd][11]=System.Para.HoleTicket[2]>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][12]=System.Para.HoleTicket[2];
//      U2Tx.Msg[U2Tx.MsgEnd][13]=System.Para.HoleTicket[3]>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][14]=System.Para.HoleTicket[3];
//      U2Tx.Msg[U2Tx.MsgEnd][15]=System.Para.HoleTicket[4]>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][16]=System.Para.HoleTicket[4];
//      U2Tx.Msg[U2Tx.MsgEnd][17]=System.Para.HoleTicket[5]>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][18]=System.Para.HoleTicket[5];
//      U2Tx.Msg[U2Tx.MsgEnd][19]=System.Para.HoleTicket[6]>>8;
//      U2Tx.Msg[U2Tx.MsgEnd][20]=System.Para.HoleTicket[6];  
			break;
		case BILLAWARD://4
//			U2Tx.Msg[U2Tx.MsgEnd][6]=System.Bill.TotalPast>>24;
//			U2Tx.Msg[U2Tx.MsgEnd][7]=System.Bill.TotalPast>>16;
//			U2Tx.Msg[U2Tx.MsgEnd][8]=System.Bill.TotalPast>>8;
//			U2Tx.Msg[U2Tx.MsgEnd][9]=System.Bill.TotalPast&0xff;
//    
//    	U2Tx.Msg[U2Tx.MsgEnd][10]=System.Bill.TotalAward>>24;
//			U2Tx.Msg[U2Tx.MsgEnd][11]=System.Bill.TotalAward>>16;
//			U2Tx.Msg[U2Tx.MsgEnd][12]=System.Bill.TotalAward>>8;
//			U2Tx.Msg[U2Tx.MsgEnd][13]=System.Bill.TotalAward&0xff;
//    
//			U2Tx.Msg[U2Tx.MsgEnd][14]=System.Backup.TotalPast>>24;
//			U2Tx.Msg[U2Tx.MsgEnd][15]=System.Backup.TotalPast>>16;
//			U2Tx.Msg[U2Tx.MsgEnd][16]=System.Backup.TotalPast>>8;
//			U2Tx.Msg[U2Tx.MsgEnd][17]=System.Backup.TotalPast&0xff;
//    
//			U2Tx.Msg[U2Tx.MsgEnd][18]=System.Backup.TotalAward>>24;
//			U2Tx.Msg[U2Tx.MsgEnd][19]=System.Backup.TotalAward>>16;
//			U2Tx.Msg[U2Tx.MsgEnd][20]=System.Backup.TotalAward>>8;
//			U2Tx.Msg[U2Tx.MsgEnd][21]=System.Backup.TotalAward&0xff;

			break;
		case STOPRUN://停机模式控制 5
      U2Tx.Msg[U2Tx.MsgEnd][6]=System.Para.RunStop;//0正常运行，1停机
//			U2Tx.Msg[U2Tx.MsgEnd][6]=Para.MusicCtl;
//			U2Tx.Msg[U2Tx.MsgEnd][7]=Para.MusicMode;
//			U2Tx.Msg[U2Tx.MsgEnd][8]=Para.MusicVol;
//			U2Tx.Msg[U2Tx.MsgEnd][9]=Para.LightCtl;
			break;
		case PARASET_6:
//			U2Tx.Msg[U2Tx.MsgEnd][6]=Para.ValueGameSumClr;
//			U2Tx.Msg[U2Tx.MsgEnd][7]=Para.CoinClr;
//			U2Tx.Msg[U2Tx.MsgEnd][8]=Para.GetGiftClr;
//			U2Tx.Msg[U2Tx.MsgEnd][9]=Para.PayCntClr;
//			U2Tx.Msg[U2Tx.MsgEnd][10]=Para.DonateGameClr;
//			U2Tx.Msg[U2Tx.MsgEnd][11]=Para.CurCoinCLr;
			break;
		case PARASET_7:
			//U2Tx.Msg[U2Tx.MsgEnd][6]=Para.UserSet7;
			break;
		case PARASET_8:
			//U2Tx.Msg[U2Tx.MsgEnd][6]=Para.UserSet8;
			break;
		case PARASET_9:
			//U2Tx.Msg[U2Tx.MsgEnd][6]=Para.UserSet9;
			break;
		case PARASET_A:
			//U2Tx.Msg[U2Tx.MsgEnd][6]=Para.UserSetA;
			break;
		case PARASET_FF:
			//U2Tx.Msg[U2Tx.MsgEnd][6]=Para.UserSetFF;
			break;
		default:
			break;
	}
	//唯一码
	for(i=1;i<7;i++)
	{
		U2Tx.Msg[U2Tx.MsgEnd][24+i]=dat[i];
	}
	U2Tx.Msg[U2Tx.MsgEnd][24+i]=GetXorCal(&U2Tx.Msg[U2Tx.MsgEnd][2],29);
	U2Tx.Msg[U2Tx.MsgEnd][25+i]=0x0d;//
	U2Tx.Msg[U2Tx.MsgEnd][26+i]=0x0a;//
	U2Tx.Msg[U2Tx.MsgEnd][MAXMSGBUFFERLEN-1]=34;
	if( ++U2Tx.MsgEnd >= MAXMSGSIZE)	U2Tx.MsgEnd=0;
	xSemaphoreGive( xU2TxSemaphore );		
}

void ParaSet(u8 *dat)
{
  if(Flag.Set==1)
    return;
	switch(dat[0])
	{
		case SYSPARA:
//			System.Para.CoinPlay=dat[1];
//      System.Para.PlayTime=dat[2]<<8;
//      System.Para.PlayTime|=dat[3];
//      System.Para.GameSoundVol=dat[4];
//      System.Para.BackSoundVol=dat[5];
      Flag.YunSet=1;
      Flag.SaveBill=1;
			break;
		case BILLPARA:
//			Para.CoinPlay=dat[1];
//			Para.PlayCoinB=dat[2];
//			Para.PlayTime=dat[3];
//			Para.GiftValue=(u16)dat[4]<<8;
//			Para.GiftValue|=dat[5];
//			Para.GiftSensitivity=dat[6];
//			Para.SelectTimeHold=dat[7];
//			Para.DisSumMoneyTime=dat[8];
//			Para.SkyGetGift=dat[9];
//			Para.ResetClearCoinEn=dat[10];
//			Para.DefendAgainCoin=dat[11];
//			Para.PromotionSta=dat[12];
//			Para.PromotionOut=dat[13];
			break;
		case TICKETPARA:
     
			break;
		case BILLAWARD:
//			Para.PowerForce=dat[1];
//			Para.WeakForce=dat[2];
//			Para.GetHeight=dat[3];
//			Para.DownDelay=dat[4];
//			Para.UpDelay=dat[5];
			break;
		case STOPRUN:
      //System.Para.RunStop=dat[1];//0正常运行，1停机
      if(dat[1]==1)
      {//停机指令
        if(System.Para.RunStop==0)
        {//当前在运行状态
          System.Para.RunStop=1;
          //Flag.RunStop=1;
          Flag.SaveBill=1;
          Flag.McuReset=1;
        }
      }
      else
      {//启动指令
        if(System.Para.RunStop==1)
        {//当前在运行状态
          System.Para.RunStop=0;
          //Flag.RunStop=0;
          Flag.SaveBill=1;
          Flag.McuReset=1;
        }        
      }
			break;
		case PARASET_6:
//			Para.ValueGameSumClr=dat[1];
//			Para.CoinClr=dat[2];
//			Para.GetGiftClr=dat[3];
//			Para.PayCntClr=dat[4];
//			Para.DonateGameClr=dat[5];
//			Para.CurCoinCLr=dat[6];
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
/******************************************************************************
 * 函数名:ProcessComMsg
 * 说明: 
 *    分机通信处理 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
void ProcessComMsg(void)
{
	u8 check=0;
	u16 cmd=0;
	u32 i=0;
	//u8 *ptr=NULL;
	while(U2Rec.MsgBegin!=U2Rec.MsgEnd)
	{
		//!检测包的完整性
		check=CheckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0],U2Rec.Msg[U2Rec.MsgBegin][MAXMSGBUFFERLEN-1]);
		if(check)
		{
			check=SoleIdCheck(&U2Rec.Msg[U2Rec.MsgBegin][U2Rec.Msg[U2Rec.MsgBegin][4]-1]);
			cmd=(U2Rec.Msg[U2Rec.MsgBegin][2]<<8)|U2Rec.Msg[U2Rec.MsgBegin][3];
			if(check)
			{//新唯一码
				switch(cmd)
				{
					case CMD_SERVECOIN://云投币指令 aa
						//加入云投币的账目中
//						if(System.Bill.TotalYunCoin<(999999-U2Rec.Msg[U2Rec.MsgBegin][5]))
//							System.Bill.TotalYunCoin+=U2Rec.Msg[U2Rec.MsgBegin][5];
//						else
//							System.Bill.TotalYunCoin=999999;
						//增加当前币数
            Flag.SaveBill=1;
						ComSendAckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0]);//原指令应答
						SendNoAskBuf[0]=U2Rec.Msg[U2Rec.MsgBegin][5]+0x30;
						SendNoAskBuf[1]=0;
						printf("云投币%d",U2Rec.Msg[U2Rec.MsgBegin][5]);
						break;
					case CMD_SERVEONSYNC://云模块上电同步指令 kk 原数据返回
						ComSendAckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0]);
						Game.ServerOn=1;//云模块已上电
            printf("云上电同步");
						break;
					
					case CMD_SERVESTATUS://云模块状态指令 jj
						Game.ServerStatus=U2Rec.Msg[U2Rec.MsgBegin][5];//云模块状态
						if(Game.ServerStatus!=SERSTA_NORMAL)
							Flag.MobilePaySta=0;
						else
						{
							if(Game.ServerReady==0)
							{
								Flag.MobilePaySta=1;
								Game.ServerReady=1;			
                printf("已连接服务器");
							}
						}
						if(Game.ServerReady==0)
						{
							printf("云状态%d",Game.ServerStatus);							
						}
						ComSendAckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0]);//应答指令
						break;
					
					case CMD_SERVEREADPARA://云模块查询主板参数ee 返回dd
						ReadParaAck(&U2Rec.Msg[U2Rec.MsgBegin][5]);
						printf("查询主板参数");
						break;
					case CMD_SERVESETDEFAULT://云模块设置主板恢复出厂ff
						SystemParaDefault(2);
						ComSendAckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0]);//应答指令
						printf("恢复出厂");
						break;
					case CMD_SERVESETPARA://云模块设置主板参数dd
						ParaSet(&U2Rec.Msg[U2Rec.MsgBegin][5]);//设置参数
						ComSendAckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0]);//应答指令
						printf("设置主板参数");
						break;	
					case CMD_SERVERESET://云模块设置主板关机重启清除数据命令mm
						for(i=0;i<6;i++)
						{
							if(System.PassWord[i]!=U2Rec.Msg[U2Rec.MsgBegin][6+i])
								break;
						}
            ComSendAckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0]);//应答指令
						if(i==6) 
						{
							//U2Rec.Msg[U2Rec.MsgBegin][5]=1;
							if(U2Rec.Msg[U2Rec.MsgBegin][5]==1)
							{//关机
								printf("云控制关机");
							}
							else if(U2Rec.Msg[U2Rec.MsgBegin][5]==2)
							{//重启
								printf("云控制重启");
//                NVIC_SETFAULTMASK();
//                GenerateSystemReset();
                Flag.McuReset=1;
							}
							else if(U2Rec.Msg[U2Rec.MsgBegin][5]==3)
							{//清除数据
								printf("云控制清数据");
							}
						}
						else  
						{//密码错误
							U2Rec.Msg[U2Rec.MsgBegin][5]=0;
						}							
						
						
						break;				
					
					//应答处理
						case CMD_CLIENTGETSERVERSTA://机台查询云模块应答
						Game.ServerStatus=U2Rec.Msg[U2Rec.MsgBegin][5];//云模块状态
						if(Game.ServerStatus!=SERSTA_NORMAL)
							Flag.MobilePaySta=0;
						else
						{
							if(Game.ServerReady==0)
							{
								Flag.MobilePaySta=1;
								Game.ServerReady=1;
                printf("已连接服务器！！");                
							}
						}
						if(Game.ServerReady==0)
						{
							printf("云状态%d",Game.ServerStatus);							
						}
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}				
						break;
						
					case CMD_CLIENTREADID://礼品机读取模块唯一码命令am
            printf("读唯一码应答");
//						Para.UserPara1=U2Rec.Msg[U2Rec.MsgBegin][5]^0x30;
//						Para.UserPara2=U2Rec.Msg[U2Rec.MsgBegin][6]^0x30;
//						Para.UserPara3=U2Rec.Msg[U2Rec.MsgBegin][7]^0x30;
//						Para.UserPara4=U2Rec.Msg[U2Rec.MsgBegin][8]^0x30;
//						Para.UserPara5=U2Rec.Msg[U2Rec.MsgBegin][9]^0x30;
//						Para.UserPara6=U2Rec.Msg[U2Rec.MsgBegin][10]^0x30;
//						Para.UserPara7=U2Rec.Msg[U2Rec.MsgBegin][11]^0x30;
//						Para.UserPara8=U2Rec.Msg[U2Rec.MsgBegin][12]^0x30;
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}						
						break;
					case CMD_CLIENTREADUSER://读取自定义数据命令应答 dm
						printf("读自定义参数");
//						Para.UserPara1=U2Rec.Msg[U2Rec.MsgBegin][5]^Flag.RamdomXorData;
//						Para.UserPara2=U2Rec.Msg[U2Rec.MsgBegin][6]^Flag.RamdomXorData;
//						Para.UserPara3=U2Rec.Msg[U2Rec.MsgBegin][7]^Flag.RamdomXorData;
//						Para.UserPara4=U2Rec.Msg[U2Rec.MsgBegin][8]^Flag.RamdomXorData;
//						Para.UserPara5=U2Rec.Msg[U2Rec.MsgBegin][9]^Flag.RamdomXorData;
//						Para.UserPara6=U2Rec.Msg[U2Rec.MsgBegin][10]^Flag.RamdomXorData;
//						Para.UserPara7=U2Rec.Msg[U2Rec.MsgBegin][11]^Flag.RamdomXorData;
//						Para.UserPara8=U2Rec.Msg[U2Rec.MsgBegin][12]^Flag.RamdomXorData;
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}
						break;							
					case CMD_CLIENTONSYNC://机台上电指令应答gg
						printf("机台上电应答");
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}
						break;
	
					case CMD_CLIENTSTATUS://机台上传状态指令bb
						printf("机台上传状态");
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}
						break;			
					case CMD_CLIENTCOINCNT://机台上传投币数指令cc
						printf("机台上传币数");
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}
						break;		
					case CMD_CLIENTGIFTOUT://机台上传礼品数指令ii
						printf("机台上传礼品数");
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}
						break;						
					case CMD_CLIENTERROR://机台上传故障代码指令hh
						printf("机台上传故障");
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}
						break;	
						
					case CMD_CLIENTSETUSER://礼品机写入厂家自定义参数命令jm
						printf("机台写入自定义");
						while(pdPASS != xQueueSend( xU2AskQueue, &cmd, 10 ))
						{
							;
						}
						break;							
					default:
						break;
				}
			}
			else
			{//唯一码相同
				ComSendAckPacket(&U2Rec.Msg[U2Rec.MsgBegin][0]);//应答指令
			}
		}
		//!清空数据包
		for(i=0;i<MAXMSGBUFFERLEN;i++)
		{
			U2Rec.Msg[U2Rec.MsgBegin][i]=0;
		}
		//!循环处理数据包
		if( ++U2Rec.MsgBegin >= MAXMSGSIZE)	U2Rec.MsgBegin=0;
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
void vU2ProcessTask( void *pvParameters )
{
	for(;;)
	{
		xSemaphoreTake( xU2RecSemaphore, ( portTickType )100);
		ProcessComMsg();
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
void vU2SendTask( void *pvParameters )
{
	INT32U i=0;
	INT32U len=0;
	for(;;)
	{
		xSemaphoreTake( xU2TxSemaphore, ( portTickType ) 1000);
		while(U2Tx.MsgBegin!=U2Tx.MsgEnd)
		{
			taskENTER_CRITICAL(); 
			len=U2Tx.Msg[U2Tx.MsgBegin][MAXMSGBUFFERLEN-1];//0xfc
			for(i=0;i<len;i++)
			{
				USART2->DR = U2Tx.Msg[U2Tx.MsgBegin][i]; 
				while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );
			}
			//!清空数据
			memset(&U2Tx.Msg[U2Tx.MsgBegin][0],0,MAXMSGBUFFERLEN);
			taskEXIT_CRITICAL();
			//!循环处理数据包
			if( ++U2Tx.MsgBegin >= MAXMSGSIZE)	U2Tx.MsgBegin=0;
			vTaskDelay(10);
		}
		vTaskDelay(50);
	}
}


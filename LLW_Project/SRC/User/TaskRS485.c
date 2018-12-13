/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：通信部分
*设计者：南下三五年
*时  间：2012-05-24  00:53:19
*********************************************************************************
*/
#include".\Lib\includes.h"
S_GAME mYGame;

INT8U S485MsgBeg=0;			//处理一个包，
INT8U S485MsgEnd=0;			//写入一个包，

INT8U S485Msg[S485MsgSize][S485MsgBufferLen]={0};//数据缓冲区
S_S485 MyS485Msg;
S_CTRGAME CtrGame;
S_TRANSPC PC;
S_SENDSLAVEPARA MySendSlavePara;

//!铁电存储
S_SLAVEBILLGROUP MySlaveBill;
S_SLAVEYAFEN MySlaveYafen;
S_RELIST MyRelist;
S_KERNEL MyKernel;

INT8U gSendPackBuff[80]={0};
INT8U gSendPacket[80]={0};
INT32U gSendTestcnt=0;

xSemaphoreHandle xS485RecSemaphore;

//!游戏赔率选择
const INT8U gamepeilv[6][8]={
{  6,  6, 12, 12, 12, 12,  6,  6, },
{  6,  8,  8, 12, 12,  8,  8,  6, },
{  4,  8, 12, 24, 24, 12,  8,  4, },
{  3, 12, 24, 24, 24, 24, 12,  3, },
{  6,  6,  8, 24, 24,  8,  6,  6, },
{  4,  6, 24, 24, 24, 24,  6,  4, },
};
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
/*************    函数名：PacketMsg               ****************************
【功能】：broad = 1 ,表示该数据帧为广播，否则表示点对点通信
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-21  13:46:13***********************/
void PacketMsg(INT8U *ptr, INT8U cmd, INT8U broad, INT8U addr,INT8U *pto,INT8U len)
{
	INT32U random=0;
	INT32U i=0;
	INT16U crc16=0;
	INT8U *check=pto;
	INT8U backlen=len;

	/*!生成4字节的随机干扰码*/
	random=IRandomX(1,0xffffff);

	/*!4字节干扰码*/
	for(i=0;i<4;i++)
	{
		*pto++=random>>(8*i);
	}
	*pto++=0x80;  //!本机地址
	if(broad)
		*pto++=0x00;//!广播
	else
		*pto++=0x01;//!目标机器地址

	*pto++=backlen+1;  //!长度
	*pto++=cmd|0x80;  //!命令
	
	while(len--)
	{
		*pto++=*ptr++; //!打包数据
	}

	if(broad)
	{
		for(i=0;i<16;i++)
		{
			*pto++=i+1;
			*pto++=MySlaveBill.slavebill[i].slaveidcheck&0xff;
			*pto++=(MySlaveBill.slavebill[i].slaveidcheck>>8)&0xff;
		}

		/*!数据CRC16 */
		crc16=Crc16Check(check,(backlen+8+48));
	}
	else
	{
		*pto++=addr;
		*pto++=MySlaveBill.slavebill[addr-1].slaveidcheck&0xff;
		*pto++=(MySlaveBill.slavebill[addr-1].slaveidcheck>>8)&0xff;
		
			
		/*!数据CRC16 */
		crc16=Crc16Check(check,(backlen+8+3));
	}

	*pto++=crc16;
	*pto++=crc16>>8;
}
/*************    函数名：SendSlaveS485             **************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-20  19:30:07***********************/
void SendSlaveS485(INT8U *data, INT8U len)
{
	INT8U i=0;

	SEND485;//!使能发送
	vTaskDelay(1);
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1, (INT8U )(*data++));
		/*!需要注意，这里判断发送完毕用的是TC，不是TXE，否则导致数据在
		缓冲区，就切换了485状态，引起最后一个字节不能发送出去*/
    	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
	vTaskDelay(1);
	RECEIVE485;//!使能接收
}

/*************    函数名: CheckPacket  **************************************
【功能】:
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-20 3:34:13 **********************/
INT8U CheckPacket(INT8U *data , INT8U len)
{
	INT16U crc16=0;
	INT16U crc16rec=0;

	/*!CRC16判断*/
	crc16=Crc16Check(data,(len-2));
	crc16rec=*(data+len-2)+(*(data+len-1)*256);

	if(crc16==crc16rec)
		return 1;
	else
		return 0;
}
/*************    函数名: CreateAllSaveId  *********************************
【功能】:绑定所有分机
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-26 16:57:33 **********************/
void CreateAllSaveId(void)
{
	int i=0;

	for(i=0;i<MAXSLAVE;i++)
	{
		/*!收取账单*/
		taskENTER_CRITICAL(); /*调度器上锁*/
		MySlaveBill.slavebill[i].slaveidcheck=0;

		if(i<4)
		{
			SAVEFM16W08SLAVEBILL1(MySlaveBill.slavebill[i].slaveidcheck);
		}
		else if((i>=4)&&(i<8))
		{
			SAVEFM16W08SLAVEBILL2(MySlaveBill.slavebill[i].slaveidcheck);
		}
		else if((i>=8)&&(i<12))
		{
			SAVEFM16W08SLAVEBILL3(MySlaveBill.slavebill[i].slaveidcheck);
		}
		else
		{
			SAVEFM16W08SLAVEBILL4(MySlaveBill.slavebill[i].slaveidcheck);
		}
		
		PacketMsg(gSendPackBuff,SHARK_BINDSLAVE,0,i+1,gSendPacket,0);
		SendSlaveS485(gSendPacket,13);
		taskEXIT_CRITICAL(); /*调度器开锁*/
		
		vTaskDelay(100);
	}
	
	vTaskDelay(100);
}
/*************    函数名：ProcessR485Msg          ****************************
【功能】：
【参数】：
【返回】：
【说明】：
		在这里解析命令
**************   设计：南下三五年 2012-11-17  00:04:22***********************/
INT8U gSendPack=0;
void ProcessR485Msg(void)
{
	INT8U err=0;
	INT8U i=0; 
	INT8U *temp=NULL;

	while(S485MsgBeg!=S485MsgEnd)
	{
		if((S485Msg[S485MsgBeg][7]==MOTHERADDR))
		{
			if((S485Msg[S485MsgBeg][5]==(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck&0xff))&&
				(S485Msg[S485MsgBeg][6]==((MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck>>8)&0xff)))
			{
				err=1;
			}
			else
				err=0;

			if((S485Msg[S485MsgBeg][8]&0x7f)==SHARK_BINDSLAVE)
				err=1;
		}
		else
		{
			err=0;
		}
		//!只处理本机数据
		if(err)
		{
			err=CheckPacket(&S485Msg[S485MsgBeg][0],S485Msg[S485MsgBeg][S485MsgBufferLen-1]);

			if(err)
			{
				switch(S485Msg[S485MsgBeg][8]&0x7f)
				{
					/*!获取分机账单*/
					//!指针是否会溢出，需要注意 玩家14 彩票 16711680
					case SHARK_GETBILL:
						temp=(INT8U *)(&MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].totalgain);
						for(i=0;i<4;i++)
						{
							*(temp+i)=S485Msg[S485MsgBeg][9+i];
						}
						temp=(INT8U *)(&MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].totalgaincurrent);
						for(i=0;i<4;i++)
						{
							*(temp+i)=S485Msg[S485MsgBeg][13+i];
						}
						temp=(INT8U *)(&MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].totalkanfen);
						for(i=0;i<32;i++)
						{
							*(temp+i)=S485Msg[S485MsgBeg][17+i];
						}

						if((S485Msg[S485MsgBeg][4]-1)<4)
						{
							SAVEFM16W08SLAVEBILL1ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1]);
						}
						else if(((S485Msg[S485MsgBeg][4]-1)>=4)&&((S485Msg[S485MsgBeg][4]-1)<8))
						{
							SAVEFM16W08SLAVEBILL2ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1]);
						}
						else if (((S485Msg[S485MsgBeg][4]-1)>=8)&&((S485Msg[S485MsgBeg][4]-1)<12))
						{
							SAVEFM16W08SLAVEBILL3ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1]);
						}
						else
						{
							SAVEFM16W08SLAVEBILL4ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1]);
						}
						//SAVEFM16W08ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1]);
						break;

					/*!获取分机押分信息*/
					case SHARK_GETBET:
						//!暂时没有使用到
						temp=(INT8U *)(&MySlaveYafen.yafen[S485Msg[S485MsgBeg][4]-1][0]);
						for(i=0;i<26;i++)
						{
							*(temp+i)=S485Msg[S485MsgBeg][9+i];
						}
						break;

					/*!绑定分机信息*/
					case SHARK_BINDSLAVE:
						MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck=
							S485Msg[S485MsgBeg][9]+((INT16U)S485Msg[S485MsgBeg][10]<<8);

						if((S485Msg[S485MsgBeg][4]-1)<4)
						{
							SAVEFM16W08SLAVEBILL1ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck);
						}
						else if(((S485Msg[S485MsgBeg][4]-1)>=4)&&((S485Msg[S485MsgBeg][4]-1)<8))
						{
							SAVEFM16W08SLAVEBILL2ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck);
						}
						else if (((S485Msg[S485MsgBeg][4]-1)>=8)&&((S485Msg[S485MsgBeg][4]-1)<12))
						{
							SAVEFM16W08SLAVEBILL3ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck);
						}
						else
						{
							SAVEFM16W08SLAVEBILL4ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck);
						}
						//SAVEFM16W08ISR(MySlaveBill.slavebill[S485Msg[S485MsgBeg][4]-1].slaveidcheck);
						break;

					default:
						break;
				}
			}
		}
		if( ++S485MsgBeg >= S485MsgSize)	S485MsgBeg=0;
	}
}

/*************    函数名：SendGameListPC          ****************************
【功能】：发送历史记录给PC
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-30  13:32:24***********************/
void SendGameListPC(void)
{
	INT16U crcsend=0;
	INT8U i=0;

#ifndef NOSENDPC
	do
	{
		if(1==FLAG.resetgame)
		{
			break;
		}
		
		USBTxMsg[USBTxMsgEnd][0]=64;//发送长度
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x16;  //长度
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255); //干扰码
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_RELIST;  //命令
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //同步字
		for(i=0;i<20;i++)
			USBTxMsg[USBTxMsgEnd][10+i]=MyRelist.gamelist[i];
		crcsend=Crc16Check(&USBTxMsg[USBTxMsgEnd][8], USBTxMsg[USBTxMsgEnd][3]);
		USBTxMsg[USBTxMsgEnd][30]=crcsend;
		USBTxMsg[USBTxMsgEnd][31]=crcsend>>8;
		USBTxMsg[USBTxMsgEnd][32]='K';
		USBTxMsg[USBTxMsgEnd][33]='J';
		if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
		xSemaphoreGive( xUSBSendSemaphore );	
	}
	while(1!=xSemaphoreTake( xPCASKSemaphore, ( portTickType ) configTICK_RATE_HZ));
#endif
}

/*************    函数名：SendGameStart              *************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-30  13:35:53***********************/
void SendGameStart(void)
{
	INT16U crcsend=0;
#ifndef NOSENDPC
	do
	{
		if(1==FLAG.resetgame)
		{
			break;
		}
		
		USBTxMsg[USBTxMsgEnd][0]=64;//发送长度
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x03;  //长度
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //干扰码
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_START;  //命令
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //同步字
		USBTxMsg[USBTxMsgEnd][10]=MyKernel.setpara.bettime;
		crcsend=Crc16Check(&USBTxMsg[USBTxMsgEnd][8], USBTxMsg[USBTxMsgEnd][3]);
		USBTxMsg[USBTxMsgEnd][11]=crcsend;
		USBTxMsg[USBTxMsgEnd][12]=crcsend>>8;
		USBTxMsg[USBTxMsgEnd][13]='K';
		USBTxMsg[USBTxMsgEnd][14]='J';
		if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
		xSemaphoreGive( xUSBSendSemaphore );
	}
	while(1!=xSemaphoreTake( xPCASKSemaphore, ( portTickType ) configTICK_RATE_HZ));
#endif
}

/*************    函数名：WaitPCAskOpen            ***************************
【功能】：
【参数】：
【返回】：
【说明】：
		2013-08-17
         测试过程中发现，等待过程中电脑发送数据，丢失，导致死等，所以这里加入
         查询
**************   设计：南下三五年 2012-11-30  13:39:53***********************/
void WaitPCAskOpen(void)
{
	INT16U crcsend=0;
	
#ifndef NOSENDPC
	//!等待PC请求结果
	do
	{
		if(1==FLAG.resetgame)
		{
			break;
		}

		//2013-08-17
		//------------------------------------------------------
		#if 0
		USBTxMsg[USBTxMsgEnd][0]=64;//发送长度
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x02;  //长度
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //干扰码
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_WAITPCASKOPEN;  //命令
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //同步字

		crcsend=Crc16Check(&USBTxMsg[USBTxMsgEnd][8], USBTxMsg[USBTxMsgEnd][3]);
		USBTxMsg[USBTxMsgEnd][10]=crcsend;
		USBTxMsg[USBTxMsgEnd][11]=crcsend>>8;
		USBTxMsg[USBTxMsgEnd][12]='K';
		USBTxMsg[USBTxMsgEnd][13]='J';
		if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;

		xSemaphoreGive( xUSBSendSemaphore );
		#endif
		//-----------------------------------------------------------
	}
	while(1!=xSemaphoreTake( xPCASKSemaphore, ( portTickType ) configTICK_RATE_HZ));
	
	do
	{
		if(1==FLAG.resetgame)
		{
			break;
		}
		
		USBTxMsg[USBTxMsgEnd][0]=64;//发送长度
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x0a;  //长度
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //干扰码
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_GETOPEN|0x80;  //命令
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //同步字
		USBTxMsg[USBTxMsgEnd][10]=PC.openshark; //鲨鱼
		USBTxMsg[USBTxMsgEnd][11]=PC.sharkcoor;//鲨鱼坐标
		USBTxMsg[USBTxMsgEnd][12]=PC.sharkpeilv;//鲨鱼赔率
		USBTxMsg[USBTxMsgEnd][13]=PC.secondopen;//二次开奖的动物
		USBTxMsg[USBTxMsgEnd][14]=PC.secondpeilv;//二次开奖的动物赔率
		USBTxMsg[USBTxMsgEnd][15]=PC.thirdopen;//三次开奖的动物
		USBTxMsg[USBTxMsgEnd][16]=PC.thirdpeilv;//三次开奖的动物赔率
		USBTxMsg[USBTxMsgEnd][17]=PC.boundpelv;//彩金赔率
		crcsend=Crc16Check(&USBTxMsg[USBTxMsgEnd][8], USBTxMsg[USBTxMsgEnd][3]);
		USBTxMsg[USBTxMsgEnd][18]=crcsend;
		USBTxMsg[USBTxMsgEnd][19]=crcsend>>8;
		USBTxMsg[USBTxMsgEnd][20]='K';
		USBTxMsg[USBTxMsgEnd][21]='J';
		if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
		xSemaphoreGive( xUSBSendSemaphore );
	}
	while(1!=xSemaphoreTake( xPCASKSemaphore, ( portTickType ) configTICK_RATE_HZ));
#endif
}

/*************    函数名: WaitPCRunLedOver  *********************************
【功能】:等待PC跑灯结束
【参数】:
【返回】:
【说明】:
         2013-08-17
         测试过程中发现，等待过程中电脑发送数据，丢失，导致死等，所以这里加入
         查询
**************   设计: 南下三五年 2013-03-19 15:14:18 **********************/
void WaitPCRunLedOver(void)
{
	do
	{
		if(1==FLAG.resetgame)
			break;

		//2013-08-17
		//------------------------------------------------------
		#if 0
		USBTxMsg[USBTxMsgEnd][0]=64;//发送长度
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x02;  //长度
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //干扰码
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_WAITPCRUNOVER;  //命令
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //同步字

		crcsend=Crc16Check(&USBTxMsg[USBTxMsgEnd][8], USBTxMsg[USBTxMsgEnd][3]);
		USBTxMsg[USBTxMsgEnd][10]=crcsend;
		USBTxMsg[USBTxMsgEnd][11]=crcsend>>8;
		USBTxMsg[USBTxMsgEnd][12]='K';
		USBTxMsg[USBTxMsgEnd][13]='J';
		if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;

		xSemaphoreGive( xUSBSendSemaphore );
		#endif
		//-----------------------------------------------------------
	}
	while(1!=xSemaphoreTake( xPCASKSemaphore, ( portTickType ) configTICK_RATE_HZ));
}

/*************    函数名: EnableSlaveBet  **********************************
【功能】:允许分机押分
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-19 15:25:5 **********************/
void EnableSlaveBet(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	gSendPackBuff[0]=mYGame.peilv;//!押分赔率
	gSendPackBuff[1]=MyKernel.setpara.bettime;//!押分赔率
	PacketMsg(gSendPackBuff,SHARK_START,1,7,gSendPacket,2);
	SendSlaveS485(gSendPacket,2+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: EnableOpenShark  **********************************
【功能】:是否出鲨鱼
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-19 16:40:50 **********************/
void EnableOpenShark(void)
{
	portTickType xLastWakeTime;
	
	taskENTER_CRITICAL(); /*调度器上锁*/
	gSendPackBuff[0]=PC.openshark;//!是否出鲨鱼
	PacketMsg(gSendPackBuff,SHARK_STOPBET,1,7,gSendPacket,1);
	SendSlaveS485(gSendPacket,1+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/

	if(PC.openshark)
	{
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil( &xLastWakeTime, configTICK_RATE_HZ*12);

		taskENTER_CRITICAL(); /*调度器上锁*/
		PacketMsg(gSendPackBuff,SHARK_STOPSHARKSHOW,1,1,gSendPacket,0);
		SendSlaveS485(gSendPacket,58);
		taskEXIT_CRITICAL(); /*调度器开锁*/
	}
}

/*************    函数名: SendSlavePara  ************************************
【功能】:更新分机参数
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-27 20:34:47 **********************/
INT32U gBaojiWin[3]={500000l,500000l,500000l,};
void SendSlavePara(void)
{
	INT8U *temp=NULL;
	INT8U i=0;

	taskENTER_CRITICAL(); /*调度器上锁*/
	
	/*!将分机参数转换到发送区*/
	MySendSlavePara.unitcoin=MyKernel.setuncode.unitcoin;
	MySendSlavePara.baojifen=gBaojiWin[MyKernel.setuncode.spcaemode];
	MySendSlavePara.maxbetzhuangxian=MyKernel.setpara.bpmaxbet;
	MySendSlavePara.maxbetshark=MyKernel.setpara.hemaxbet;
	MySendSlavePara.maxbetanimal=MyKernel.setpara.othermaxbet;
	MySendSlavePara.minbetunit=MyKernel.setpara.betunitmin;
	MySendSlavePara.unitlottery=MyKernel.setpara.unitlottery;

	temp=(INT8U *)&MySendSlavePara.unitcoin;
	for(i=0;i<18;i++)
		gSendPackBuff[i]=*temp++;

	PacketMsg(gSendPackBuff,SHARK_PARAMETERS,1,7,gSendPacket,18);
	SendSlaveS485(gSendPacket,18+58);
	
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: SendTestMsg  **************************************
【功能】:发送通信测试
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-28 1:51:31 **********************/
void SendTestMsg(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_TESTCOMMUNICATION,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: SendRestSlave  ***********************************
【功能】:复位分机
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-28 1:55:39 **********************/
void SendRestSlave(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_RESETSLAVE,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: SlaveBetOver  **************************************
【功能】:分机开奖效果
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-19 16:44:23 **********************/
void SlaveBetOver(void)
{
	INT32U bounds=0;

	bounds=IRandomX(100,100000);
		
	if(PC.sharkcoor)
	{
		/*!给出鲨鱼*/
		taskENTER_CRITICAL(); /*调度器上锁*/
		gSendPackBuff[0]=mYGame.open;//!游戏结果
		gSendPackBuff[1]=mYGame.sharkpeilv;//!游戏结果
		//!这里加入彩金同步
		//2013-08-20
		//---------------------------------------------------
		gSendPackBuff[2]=bounds;
		gSendPackBuff[3]=bounds>>8;
		gSendPackBuff[4]=bounds>>16;
		gSendPackBuff[5]=bounds>>24;
		//---------------------------------------------------
		PacketMsg(gSendPackBuff,SHARK_OPEN,1,7,gSendPacket,6);//2
		SendSlaveS485(gSendPacket,6+58);//2
		taskEXIT_CRITICAL(); /*调度器开锁*/

		//!等待PC指定结果就绪，发送给分机
		WaitPCRunLedOver();

		//!计算得失
		if(8==mYGame.open)
		{
			MyKernel.realbill.totalwin+=MyRelist.allyafen[11]*mYGame.sharkpeilv;
		}
		else if(9==mYGame.open)
		{
			MyKernel.realbill.totalwin+=MyRelist.allyafen[10]*mYGame.sharkpeilv;
		}
		else if(10==mYGame.open)
		{
			MyKernel.realbill.totalwin+=MyRelist.allyafen[9]*mYGame.sharkpeilv;
		}	
		
		SendCurrentAllBill();
		
		/*!二次送灯*/
		if((mYGame.second==8)||(mYGame.second==9))
		{
			taskENTER_CRITICAL(); /*调度器上锁*/
			gSendPackBuff[0]=mYGame.second;//!游戏结果
			gSendPackBuff[1]=mYGame.secondpeilv;
			PacketMsg(gSendPackBuff,SHARK_HAPPY,1,7,gSendPacket,2);
			SendSlaveS485(gSendPacket,2+58);
			taskEXIT_CRITICAL(); /*调度器开锁*/

			//!等待PC指定结果就绪，发送给分机
			WaitPCRunLedOver();

			//!计算得失
			if(8==mYGame.second)
			{
				MyKernel.realbill.totalwin+=MyRelist.allyafen[11]*mYGame.secondpeilv;
			}
			else if(9==mYGame.second)
			{
				MyKernel.realbill.totalwin+=MyRelist.allyafen[10]*mYGame.secondpeilv;
			}
			
			SendCurrentAllBill();
			
			taskENTER_CRITICAL(); /*调度器上锁*/
			gSendPackBuff[0]=mYGame.third;//!游戏结果
			PacketMsg(gSendPackBuff,SHARK_HAPPYTHREE,1,7,gSendPacket,1);
			SendSlaveS485(gSendPacket,1+58);
			taskEXIT_CRITICAL(); /*调度器开锁*/

			//!计算得失
			MyKernel.realbill.totalwin+=MyRelist.allyafen[mYGame.third]*
					gamepeilv[mYGame.peilv][mYGame.third];

			/*!计算庄闲得分*/
			if((mYGame.open>3)&&(mYGame.open<8))
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[12]*2);
			else
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[8]*2);

			SendCurrentAllBill();

			if(mYGame.open==10)
			{
				//!等待PC指定结果就绪，发送给分机
				WaitPCRunLedOver();

				//!计算得失
				MyKernel.realbill.totalwin+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;
				MyKernel.realbill.totalbounds+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;
				SendCurrentAllBill();
				
				/*!给出鲨鱼*/
				taskENTER_CRITICAL(); /*调度器上锁*/
				gSendPackBuff[0]=mYGame.boundspeilv;//!游戏彩金
				PacketMsg(gSendPackBuff,SHARK_BOUNDS,1,7,gSendPacket,1);
				SendSlaveS485(gSendPacket,1+58);
				taskEXIT_CRITICAL(); /*调度器开锁*/
			}
		}
		else
		{
			taskENTER_CRITICAL(); /*调度器上锁*/
			gSendPackBuff[0]=mYGame.second;//!游戏结果
			PacketMsg(gSendPackBuff,SHARK_HAPPY,1,7,gSendPacket,1);
			SendSlaveS485(gSendPacket,1+58);
			taskEXIT_CRITICAL(); /*调度器开锁*/

			//!计算得失
			MyKernel.realbill.totalwin+=MyRelist.allyafen[mYGame.second]*
					gamepeilv[mYGame.peilv][mYGame.second];

			/*!计算庄闲得分*/
			if((mYGame.open>3)&&(mYGame.open<8))
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[12]*2);
			else
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[8]*2);
			SendCurrentAllBill();
			
			if(mYGame.open==10)
			{
				//!等待PC指定结果就绪，发送给分机
				WaitPCRunLedOver();

				//!计算得失
				MyKernel.realbill.totalwin+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;

				MyKernel.realbill.totalbounds+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;
				SendCurrentAllBill();
				
				/*!给出鲨鱼*/
				taskENTER_CRITICAL(); /*调度器上锁*/
				gSendPackBuff[0]=mYGame.boundspeilv;//!游戏彩金
				PacketMsg(gSendPackBuff,SHARK_BOUNDS,1,7,gSendPacket,1);
				SendSlaveS485(gSendPacket,1+58);
				taskEXIT_CRITICAL(); /*调度器开锁*/
			}
		}
	}
	else
	{
		taskENTER_CRITICAL(); /*调度器上锁*/
		gSendPackBuff[0]=mYGame.open;//!游戏结果
		gSendPackBuff[1]=0;
		//!这里加入彩金同步
		//2013-08-20
		//---------------------------------------------------
		gSendPackBuff[2]=bounds;
		gSendPackBuff[3]=bounds>>8;
		gSendPackBuff[4]=bounds>>16;
		gSendPackBuff[5]=bounds>>24;
		//---------------------------------------------------
		
		PacketMsg(gSendPackBuff,SHARK_OPEN,1,7,gSendPacket,6);//2
		SendSlaveS485(gSendPacket,6+58);//2
		taskEXIT_CRITICAL(); /*调度器开锁*/

		//!计算得失
		MyKernel.realbill.totalwin+=(MyRelist.allyafen[mYGame.open]*
					gamepeilv[mYGame.peilv][mYGame.open]);
		/*!计算庄闲得分*/
		if((mYGame.open>3)&&(mYGame.open<8))
			MyKernel.realbill.totalwin+=(MyRelist.allyafen[12]*2);
		else
			MyKernel.realbill.totalwin+=(MyRelist.allyafen[8]*2);
		
		SendCurrentAllBill();
	}
}

/*************    函数名: GetAllSlaveBill  *********************************
【功能】:收取所有分机账单
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-19 17:5:14 **********************/
void GetAllSlaveBill(void)
{
	int i=0;

	for(i=0;i<MAXSLAVE;i++)
	{
		/*!收取账单*/
		taskENTER_CRITICAL(); /*调度器上锁*/
		PacketMsg(gSendPackBuff,SHARK_GETBILL,0,i+1,gSendPacket,0);
		SendSlaveS485(gSendPacket,13);
		taskEXIT_CRITICAL(); /*调度器开锁*/
		vTaskDelay(100);
	}
}

/*************    函数名: SendSlavePCUnReady  *******************************
【功能】:通知所有分机，电脑不在线
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-28 21:25:26 **********************/
void SendSlavePCUnReady(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_SERVERDROP,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: SendSlavePCGameWrong  *****************************
【功能】:PC游戏不匹配
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-28 21:27:22 **********************/
void SendSlavePCGameWrong(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_PCGAMEWRONG,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: SendSlavePCComWrong  ******************************
【功能】:PC通信错误
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-28 21:28:21 **********************/
void SendSlavePCComWrong(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_PCMSGWRONG,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: SendSlaveRunTimeOver  ****************************
【功能】:系统运行时间到，请延时
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-28 21:29:31 **********************/
void SendSlaveRunTimeOver(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_RUNTIMEOVER,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: SendSlaveBillOver  *******************************
【功能】:通知分机，当期账目满，请报账
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-28 21:30:17 **********************/
void SendSlaveBillOver(void)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_GAINOVER,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: UpDateHistoryList  *******************************
【功能】:更新历史记录
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-19 17:8:30 **********************/
void UpDateHistoryList(void)
{
	INT8U i=0;

	taskENTER_CRITICAL(); /*调度器上锁*/
	for(i=20;i>0;i--)
			MyRelist.gamelist[i]=MyRelist.gamelist[i-1];

	/*!处理历史记录，鲨鱼优先*/
	if(PC.sharkcoor)
		MyRelist.gamelist[0]=PC.sharkcoor;
	else
		MyRelist.gamelist[0]=PC.thirdopen;

	SAVEFM16W08RELISTISR(MyRelist.gamelist);
	//SAVEFM16W08ISR(MyRelist.gamelist);

	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: WaitGameOver  ************************************
【功能】:等待游戏结束
【参数】:
【返回】:
【说明】:
		 2013-08-17
         测试过程中发现，等待过程中电脑发送数据，丢失，导致死等，所以这里加入
         查询
**************   设计: 南下三五年 2013-03-20 1:55:29 **********************/
void WaitGameOver(void)
{
	do
	{
		if(1==FLAG.resetgame)
			break;

		//2013-08-17
		//------------------------------------------------------
		#if 0
		USBTxMsg[USBTxMsgEnd][0]=64;//发送长度
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x02;  //长度
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //干扰码
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_WAITPCGAMEOVER;  //命令
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //同步字

		crcsend=Crc16Check(&USBTxMsg[USBTxMsgEnd][8], USBTxMsg[USBTxMsgEnd][3]);
		USBTxMsg[USBTxMsgEnd][10]=crcsend;
		USBTxMsg[USBTxMsgEnd][11]=crcsend>>8;
		USBTxMsg[USBTxMsgEnd][12]='K';
		USBTxMsg[USBTxMsgEnd][13]='J';
		if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;

		xSemaphoreGive( xUSBSendSemaphore );
		#endif
		//-----------------------------------------------------------
	}
	while(1!=xSemaphoreTake( xPCASKSemaphore, ( portTickType ) configTICK_RATE_HZ));
}

/*************    函数名: SaveAllBill  **************************************
【功能】:保存账目
【参数】:
【返回】:
【说明】:
        由于清帐，不能清除分机的账目，所以针对本次实际利润的计算重新计算，
        本次实际利润==全部总盈利-上次总盈利
**************   设计: 南下三五年 2013-03-20 18:3:19 **********************/
void SaveAllBill(void)
{
	int i=0;
	taskENTER_CRITICAL(); /*调度器上锁*/
	
	MyKernel.realbill.totalgain=0;
	MyKernel.realbill.totalgaincurrent=0;
	MyKernel.realbill.totalkaifen=0;
	MyKernel.realbill.totalxifen=0;
	MyKernel.realbill.totalcoin=0;
	MyKernel.realbill.totalcout=0;
	MyKernel.realbill.totallottery=0;
	MyKernel.realbill.totalbet=0;
	MyKernel.realbill.totalwin=0;
	MyKernel.realbill.totalbounds=0;
	MyKernel.realbill.totalbetin=0;    //!系统运行的核心参数，通过该值判断营收
	MyKernel.realbill.totalbetout=0;   //!系统运行的核心参数
	
	for(i=0;i<MAXSLAVE;i++)
	{
		MyKernel.realbill.totalgain+=MySlaveBill.slavebill[i].totalgain;
		//MyShark.realbill.totalgaincurrent+=MyShark.slavebill[i].totalgaincurrent;
		MyKernel.realbill.totalkaifen+=MySlaveBill.slavebill[i].totalkanfen;
		MyKernel.realbill.totalxifen+=MySlaveBill.slavebill[i].totalxifen;
		MyKernel.realbill.totalcoin+=MySlaveBill.slavebill[i].totalcoin;
		MyKernel.realbill.totalcout+=MySlaveBill.slavebill[i].totalcout;
		MyKernel.realbill.totallottery+=MySlaveBill.slavebill[i].totallottery;
		//!2013-08-13
		//!防止总进分量翻转,导致放水
		if(MyKernel.realbill.totalbetin<0xefffffff)
			MyKernel.realbill.totalbetin+=MySlaveBill.slavebill[i].totalbetin;
		
		MyKernel.realbill.totalbetout+=MySlaveBill.slavebill[i].totalbetout;
	}
	MyKernel.realbill.totalgaincurrent=MyKernel.realbill.totalgain-
		              MyKernel.realbill.totalgainpre;

	//SAVEFM16W08ISR(MyKernel.realbill);
	SAVEFM16W08KERNELISR(MyKernel.realbill);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: GetAllBetMsg  *************************************
【功能】:获取所有分机的押分信息
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-21 11:45:29 **********************/
void GetAllBetMsg(void)
{
	int i=0,j=0;

	for(i=0;i<MAXSLAVE;i++)
	{
		for(j=0;j<13;j++)
		{
			MySlaveYafen.yafen[i][j]=0;
		}
	}

	for(i=0;i<MAXSLAVE;i++)
	{
		/*!收取账单*/
		taskENTER_CRITICAL(); /*调度器上锁*/
		PacketMsg(gSendPackBuff,SHARK_GETBET,0,i+1,gSendPacket,0);
		SendSlaveS485(gSendPacket,13);
		taskEXIT_CRITICAL(); /*调度器开锁*/
		vTaskDelay(100);
	}
}

/*************    函数名: SaveAllBet  **************************************
【功能】:处理所有分机押分
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-21 12:50:36 **********************/
void SaveAllBet(void)
{
	INT8U i=0,j=0;

	taskENTER_CRITICAL(); /*调度器上锁*/

	for(i=0;i<13;i++)
		MyRelist.allyafen[i]=0;
	
	for(i=0;i<MAXSLAVE;i++)
	{
		for(j=0;j<13;j++)
		{
			CtrGame.MyPlayer.Player[i].bet[j]=MySlaveYafen.yafen[i][j];
			MyRelist.allyafen[j]+=MySlaveYafen.yafen[i][j];
		}
	}

	MyKernel.realbill.totalbet=0;
	MyKernel.realbill.totalwin=0;
	MyKernel.realbill.totalbounds=0;
	for(i=0;i<13;i++)
	{
		MyKernel.realbill.totalbet+=MyRelist.allyafen[i];
	}
	taskEXIT_CRITICAL(); /*调度器开锁*/
}
/*************    函数名: ClearSlaveBill  ***********************************
【功能】:清除账单
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-21 11:53:58 **********************/
void ClearSlaveBill(void)
{
	/*!收取账单*/
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_CLEARBILL,1,1,gSendPacket,0);
	SendSlaveS485(gSendPacket,58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: ClearSlaveBillCurrent  ***************************
【功能】:清除分机的本次实际利润
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-06-18 12:0:21 **********************/
void ClearSlaveBillCurrent(void)
{
	/*!收取账单*/
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_CLEARBILLCURRENT,1,1,gSendPacket,0);
	SendSlaveS485(gSendPacket,58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: StopSlaveBeting  **********************************
【功能】:停止所有分机押分
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-21 15:37:55 **********************/
void StopSlaveBeting(void)
{
	/*!收取账单*/
	taskENTER_CRITICAL(); /*调度器上锁*/
	PacketMsg(gSendPackBuff,SHARK_NOTIMEBET,1,1,gSendPacket,0);
	SendSlaveS485(gSendPacket,58);
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: vRS485RecTask  **********************************
【功能】:485分机命令解析
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-03-20 3:17:3 **********************/
void vRS485RecTask( void *pvParameters )
{
	for(;;)
	{
		xSemaphoreTake( xS485RecSemaphore, ( portTickType ) portMAX_DELAY);

		taskENTER_CRITICAL(); /*调度器上锁*/
		ProcessR485Msg();
		taskEXIT_CRITICAL(); /*调度器开锁*/
	}
}

/*************    函数名：vGamePlayerTask          ***************************
【功能】：主游戏流程
【参数】：
【返回】：
【说明】：
		游戏流程:
		1、分机自检
		2、等待与主机建立连接，监听总线信息
		3、主机发送启动命令，游戏开始，允许押分
		4、押分结束，等待游戏结果
		5、主机发送游戏结果，进行相应操作
		6、主机收集账目，开始下一轮游戏

		2013-07-03 修改:
		1、通信部分，核心通信必须通过应答的方式，否侧容易引起丢包，在长期运行
		中，发现丢包现象
		2、调整了鲨鱼的概率，在难度等级为死难的情况下，鲨鱼太少，故加大了鲨鱼
		的概率
		
**************   设计：南下三五年 2012-11-16  17:53:23***********************/
void vGamePlayerTask( void *pvParameters )
{
	portTickType xLastWakeTime;

	mYGame.step=GAME_WAITMOTHER;

	/*!使用当前时间 初始化xLastWakeTime 变量*/
    xLastWakeTime = xTaskGetTickCount();
	vTaskDelay(3000);
	xSemaphoreTake( xPCASKSemaphore, ( portTickType ) 0);
	FLAG.resetgame=1;

	for(;;)
	{	
		//!发送核心参数给PC
		if(FLAG.resetgame)
		{
			SendServerRestOK();
			vTaskDelay(50);
			SendServerSysKernel();
			vTaskDelay(50);
			SendSystemPara();
			vTaskDelay(50);
			SendNextCodeDate();
			vTaskDelay(50);
			FLAG.resetgame=0;
		}
		
		//!更新账单  ok
		SendCurrentAllBill();

		//!PC更新历史记录  ok
		SendGameListPC();

		//!挂起任务
		vTaskDelay(50);
		if(FLAG.lockgametask)
		{
			SendGameSpend();
			vTaskDelay(50);
			vTaskSuspend( NULL );
		}
		
		/*!游戏开始，读秒，允许押分*/
		//!产生随机赔率
		if(0==MyKernel.setpara.oddsselect)
		{
			mYGame.peilv=IRandomX(0,5);
		}
		else
			mYGame.peilv=MyKernel.setpara.oddsselect-1;
		
		//发命令给服务器  ok
		SendGameStart();

		//发命令给分机,游戏开始，更新赔率,允许押分
		if(0==FLAG.resetgame)
			EnableSlaveBet();

		/*!等待读秒结束*/
		if(0==FLAG.resetgame)
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil( &xLastWakeTime, configTICK_RATE_HZ*MyKernel.setpara.bettime);

			vTaskDelay(500);
			//!收取押分，进行结果判断
			StopSlaveBeting();
			vTaskDelay(100);
			GetAllBetMsg();
			SaveAllBet();
		}

		if(0==FLAG.resetgame)
		{
			//!更新押分信息  ok
			SendCurrentAllBill();
			
			//!产生游戏结果
			HappyGameSet();
			
			//!将游戏结果转换成PC指示跑灯位置
			GetGameOpenPC();
		}
		
		//!等待PC过来取结果
		WaitPCAskOpen();

		/*!是否出鲨鱼*/
		if(0==FLAG.resetgame)
			EnableOpenShark();
		
		//!等待PC指定结果就绪，发送给分机
		WaitPCRunLedOver();
		
		/*!游戏结果*/
		if(0==FLAG.resetgame)
			SlaveBetOver();

		/*!等待游戏结束*/
		WaitGameOver();

		if(0==FLAG.resetgame)
		{
			/*!更新历史记录*/
			UpDateHistoryList();

			/*!收取所有分机账单*/
			GetAllSlaveBill();

			/*!保存实时账目*/
			SaveAllBill();
		}
	}
}




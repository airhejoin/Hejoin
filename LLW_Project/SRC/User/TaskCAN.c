/*
*********************************************************************************
*                                 STM32F103C8T6
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver1.1
*                                   皇家科技
*
*模  块：CAN协议解析
*设计者：Andy
*时  间：2010-10-19
*********************************************************************************
*/
#include".\Lib\includes.h"

xSemaphoreHandle xCANSendSemaphore;  
xSemaphoreHandle xCANTxSemaphore, xCANRxSemaphore;  /*CAN接收信号量*/
xSemaphoreHandle xCANDataReadySemaphore;  

S_SERVERMSG PCMSG;
xQueueHandle     xPCServerQueue;            /**!创建服务器应答队列*/
extern xSemaphoreHandle xAskServerSemaphore;      /*!服务器应答的信号量*/
/*************    函数名：PackCanMsg              ****************************
【功能】：对CAN进行，进行封包,写入缓冲区
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-26  01:19:55***********************/
void PackCanMsg(CanTxMsg SendMsg, INT8U cmd ,INT8U id, INT8U msgsyn)
{
	INT32U  gexid=0;
	INT32U  i=0;
	INT32U  cansyn=msgsyn;

	for(i=0;i<8;i++)
		CanTXMsg[CanTxMsgBeg].Data[i]=0;
	/*打包扩展帧*/
	gexid=(((INT32U)cansyn)<<24)|(((INT32U)cmd)<<16)|(((INT32U)FLAG.id)<<8)|id; 

	CanTXMsg[CanTxMsgBeg].ExtId=gexid;   /*打包命令和数据*/
	CanTXMsg[CanTxMsgBeg].IDE=CAN_ID_EXT;
	CanTXMsg[CanTxMsgBeg].RTR=CAN_RTR_DATA;
	CanTXMsg[CanTxMsgBeg].DLC=SendMsg.DLC;

	for(i=0;i<SendMsg.DLC;i++)
		CanTXMsg[CanTxMsgBeg].Data[i]=SendMsg.Data[i];

	/*数据压栈*/
	if( ++CanTxMsgBeg>= CAN_RxSize)	CanTxMsgBeg=0;

	xSemaphoreGive( xCANDataReadySemaphore );  //注意发消息的地方
}

/*************    函数名：ProcPage                ****************************
【功能】：解析CAN命令,服务器格式
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-30  15:08:18***********************/
INT32U gExid=0;
void ProcPage(void)
{
	CanTxMsg TxMsg;

	INT8U cmd=0,i=0;
#ifdef TWOLCDMODE
	INT32U sendid=0;
#endif

	
	while(CanMsgBeg!=CanMsgEnd)
	{
		gExid=(CanMsg[CanMsgBeg].StdId<<18)|(CanMsg[CanMsgBeg].ExtId);
		cmd=(gExid>>16)&0x7f;
		FLAG.cmd=cmd;

		switch(cmd)
		{
			/*!刷卡主机和自动兑币机功能协议解析*/
			/*终端ID配置*/
			case SETSLAVEID:
					#ifdef SAVECOIN
						FLAG.setcanid=1;
						FLAG.setcanlcd=1;
					#endif
				break;

			/*ID配置结束*/
			case EXECUTEID:
					#ifdef SAVECOIN
						ST.id=FLAG.id;
						SAVE3130(ST.id);
						CAN_Configuration();
						/*使能CAN*/
						CAN_ITConfig(CAN1,CAN_IT_TME|CAN_IT_FMP0, ENABLE);
						FLAG.setcanid=0;
						gFreshTwo=1;
						FLAG.setcanlcd=0;
					#endif
				break;

			/*参数设置*/
			case SETTINGSYS:
				CARDMSG.unit=CanMsg[CanMsgBeg].Data[0];
				CARDMSG.maxcoin=((INT32U)CanMsg[CanMsgBeg].Data[2]<<8)+CanMsg[CanMsgBeg].Data[1];
				xSemaphoreGive( xAskServerSemaphore );

				/*!发消息给24064模块*/
				#ifdef TWOLCDMODE
					#ifdef MOTHERCARD
					TxMsg.DLC=1;
					TxMsg.Data[0]=CARDMSG.unit;
					PackCanMsg(TxMsg, PARAMETERINIT24064, (FLAG.id+242),0);
					vTaskDelay(50);
					PackCanMsg(TxMsg, PARAMETERINIT24064, (FLAG.id+242),0);//补发
					#endif
				#endif
				
				break;

			/*锁住终端*/
			case DISSYS:
				FLAG.lock=0;
				xSemaphoreGive( xAskServerSemaphore );
				TxMsg.DLC=0;
				PackCanMsg(TxMsg, DISSYS|0x80, MasterID,FLAG.syn);
				if(++FLAG.syn==31)  FLAG.syn=1;
				break;

			/*使能终端*/
			case ENABLESYS:
				FLAG.lock=1;
				xSemaphoreGive( xAskServerSemaphore );
				TxMsg.DLC=0;
				PackCanMsg(TxMsg, ENABLESYS|0x80, MasterID,FLAG.syn);
				if(++FLAG.syn==31)  FLAG.syn=1;
				//FLAG.msg=DISFIANDCARD;
				//xSemaphoreGive( xLCDRefreshSemaphore );

				break;

			/*上分*/
			case COINUP:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				FLAG.setsysbill=1;
				
				CARDMSG.backcoin=CARDMSG.coin;
				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*下分*/
			case COINDOWN:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				FLAG.setsysbill=1;
				
				CARDMSG.backcoin=CARDMSG.coin;
				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*请求总分*/
			case REQUESTCOIN:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				if(CARDMSG.coin>900000L)
					CARDMSG.coin=0;

				xSemaphoreGive( xAskServerSemaphore );
				
				break;

			/*终端就绪*/
			case READYSYS:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*请求解锁*/
			case JIESUO:
				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*请求配置*/
			case PEIZHI:
				//if(CanMsg[CanMsgBeg].Data[0]&0xf0)
					//CARDMSG.unit=(CanMsg[CanMsgBeg].Data[0]>>4)|0x80;
				//else
				CARDMSG.unit=CanMsg[CanMsgBeg].Data[0];
				CARDMSG.maxcoin=((INT32U)CanMsg[CanMsgBeg].Data[2]<<8)+CanMsg[CanMsgBeg].Data[1];
				CARDMSG.yajin=((INT16U)CanMsg[CanMsgBeg].Data[4]<<8)+CanMsg[CanMsgBeg].Data[3];
				
				xSemaphoreGive( xAskServerSemaphore );


				break;

			/*机台投币*/
			case TOUBIOK:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*机台退币*/
			case TUIBIOK:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*机台退币*/
			case SENDCARDID:
					CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
						((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];
					/*if(CARDMSG.unit&0x80)
						CARDMSG.coin=CARDMSG.coin/(CARDMSG.unit&0x7f);
					else
						CARDMSG.coin=CARDMSG.coin*CARDMSG.unit;*/

				#ifdef PRINTFMSG
					printf("coin:%ld\r\n",CARDMSG.coin);
				#endif
				if(CARDMSG.coin>900000L) CARDMSG.coin=0;
				xSemaphoreGive( xAskServerSemaphore );

				break;

			case REFRESHCOIN:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			case REFRESHLOTTERY:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			case DECLOTTERY:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			case REQLOTTERY:
				CARDMSG.lottery=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
						((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];
				xSemaphoreGive( xAskServerSemaphore );
				break;
#ifdef MOTHERCARD 
			case ADDMONEY:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];
				
				CARDMSG.coin=CARDMSG.coin/CARDMSG.unit;
				CARDMSG.backcoin=CARDMSG.coin;

				FLAG.setsysbill=1;
				xSemaphoreGive( xAskServerSemaphore );
				break;

			case DECMONEY:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				CARDMSG.coin=CARDMSG.coin/CARDMSG.unit;

				FLAG.setsysbill=1;
				xSemaphoreGive( xAskServerSemaphore );
				break;
#endif				
				/*!校正系统时间，联系设备通过服务器广播就可以了，不连线设备，通过手持设备*/
			case CHECKDATE:

				break;		

			case CASHADD:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				CARDMSG.backcoin=CARDMSG.coin;

				FLAG.setsysbill=1;
				xSemaphoreGive( xAskServerSemaphore );
				break;

			case CASHDEC:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				CARDMSG.backcoin=CARDMSG.coin;

				FLAG.setsysbill=1;
				xSemaphoreGive( xAskServerSemaphore );
				break;

			case GETCOINMONEY:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[1]<<8)+CanMsg[CanMsgBeg].Data[0];
				xSemaphoreGive( xAskServerSemaphore );
				break;

			case SELLCOINSEMIMAC:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[1]<<8)+CanMsg[CanMsgBeg].Data[0];
				xSemaphoreGive( xAskServerSemaphore );
				break;

			case OPENMONEYBOX:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
						((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				#ifdef PRINTFMSG
					printf("coin:%ld\r\n",CARDMSG.coin);
				#endif
				OpenMoneyBox();
				if(CARDMSG.coin>900000L) CARDMSG.coin=0;
				xSemaphoreGive( xLEDSendSemaphore );
				CARDMSG.step=WRITCARD;
				CARDMSG.sta=0;
				break;

			case ReaderParaMode:
				FLAG.WriteCardStop=CanMsg[CanMsgBeg].Data[0];
				break;
				
			default:
				break;
		}
		//缓冲区清零
		for(i=0;i<8;i++)
			CanMsg[CanMsgBeg].Data[i]=0;
		CanMsg[CanMsgBeg].DLC=0;
		CanMsg[CanMsgBeg].ExtId=0;
		CanMsg[CanMsgBeg].StdId=0;
		
		if( ++CanMsgBeg >= CAN_RxSize)	CanMsgBeg=0;
	}
}
/*************    函数名：WaitServerReady          ***************************
【功能】：系统启动，需要向服务器上报状态，同时请求跟新自己的参数，同时进入就绪
          态，等待服务员的后续操作
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-08-12  22:45:21***********************/
void WaitServerReady(void)
{
#ifdef REQUESTSERVER
	CanTxMsg TxMsg;
//	INT8U number=0;

	/*!主机执行动作后，需要服务器服务器应答后，才离开*/
	/*!就绪*/
	TxMsg.DLC=0;
	PackCanMsg(TxMsg, READYSYS, MasterID,FLAG.syn);
	vTaskDelay(100);

	#ifdef PRINTFMSG
		printf("regedit\r\n");
	#endif
			
	while(1)
	{
		ProcPage();
		if(xSemaphoreTake( xAskServerSemaphore, ( portTickType ) 200 ) == pdTRUE)
		{
			if(FLAG.cmd==READYSYS)
			{
				FLAG.cmd=0;
				break;
			}
		}
		else
		{
			/*!就绪*/
			TxMsg.DLC=0;
			PackCanMsg(TxMsg, READYSYS, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;

	/*!主机执行动作后，需要服务器服务器应答后，才离开*/
	FLAG.noasktime=0;
	/*!配置*/
	TxMsg.DLC=0;
	PackCanMsg(TxMsg, PEIZHI, MasterID,FLAG.syn);
	vTaskDelay(100);
	
	while(1)
	{
		ProcPage();
		if(xSemaphoreTake( xAskServerSemaphore, ( portTickType ) 200 ) == pdTRUE)
		{
			if(FLAG.cmd==PEIZHI)
			{
				FLAG.cmd=0;
				break;
			}
		}
		else
		{
			/*!配置*/
			TxMsg.DLC=0;
			PackCanMsg(TxMsg, PEIZHI, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;

	/*!主机执行动作后，需要服务器服务器应答后，才离开*/
	FLAG.noasktime=0;
	/*!解锁*/
	TxMsg.DLC=0;
	PackCanMsg(TxMsg, JIESUO, MasterID,FLAG.syn);
	vTaskDelay(100);
	
	while(1)
	{
		ProcPage();
		if(xSemaphoreTake( xAskServerSemaphore, ( portTickType ) 200 ) == pdTRUE)
		{
			if(FLAG.cmd==ENABLESYS)
			{
				FLAG.cmd=0;
				break;
			}
		}
		else
		{
			/*!解锁*/
			TxMsg.DLC=0;
			PackCanMsg(TxMsg, JIESUO, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;

	FLAG.line=LINEREADY;
#endif
}
void TestMoniAuto(void)
{
	CanTxMsg TxMsg;
	/*!全自动*/
	TxMsg.DLC=2;
	TxMsg.Data[0]=0x64;
	TxMsg.Data[1]=0x00;
	PackCanMsg(TxMsg, GETCOINMONEY, MasterID,FLAG.syn);
	vTaskDelay(100);
			
	while(1)
	{
		ProcPage();
		if(xSemaphoreTake( xAskServerSemaphore, ( portTickType ) 200 ) == pdTRUE)
		{
			if(FLAG.cmd==GETCOINMONEY)
			{
				FLAG.cmd=0;
				break;
			}
		}
		else
		{
			/*!就绪*/
			TxMsg.DLC=2;
			TxMsg.Data[0]=0x64;
			TxMsg.Data[1]=0x00;
			PackCanMsg(TxMsg, GETCOINMONEY, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;

	vTaskDelay(10000);
	/*!半自动*/
	TxMsg.DLC=2;
	TxMsg.Data[0]=0xe8;
	TxMsg.Data[1]=0x03;
	PackCanMsg(TxMsg, SELLCOINSEMIMAC, MasterID,FLAG.syn);
	vTaskDelay(100);
			
	while(1)
	{
		ProcPage();
		if(xSemaphoreTake( xAskServerSemaphore, ( portTickType ) 200 ) == pdTRUE)
		{
			if(FLAG.cmd==SELLCOINSEMIMAC)
			{
				FLAG.cmd=0;
				break;
			}
		}
		else
		{
			/*!就绪*/
			TxMsg.DLC=2;
			TxMsg.Data[0]=0xe8;
			TxMsg.Data[1]=0x03;
			PackCanMsg(TxMsg, SELLCOINSEMIMAC, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;
	
}
/*************    函数名：vCanprocTask             ***************************
【功能】：CAN处理任务，解析CAN命令，做出相应处理
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-26  02:02:40***********************/
void vCanprocTask( void *pvParameters )
{	
	/*使能CAN*/
	CAN_ITConfig(CAN1,CAN_IT_TME|CAN_IT_FMP0, ENABLE);
	DisCardMoney(12345678);
	WaitServerReady();
	DisCardMoney(0);

	#ifdef PRINTFMSG
	printf("Server is ready \r\n");
	#endif
	//!系统就绪后，方可创建任务
	xTaskCreate( vFM1702MangerTask, ( signed char * ) "FM1702", 512, NULL, 3, NULL );
	for(;;)
	{
		xSemaphoreTake( xCANRxSemaphore, portMAX_DELAY);
		
		ProcPage();
	}
}
/*************    函数名：vCanSendTask            ****************************
【功能】：CAN中断发送函数
【参数】：
【返回】：
【说明】：
         第一次在这里犯了傻，写了个死循环，导致这个任务一打开，其他任务就无法
         运行，起初以为是信号量的问题，后来才发现原来是这样，傻傻傻
**************   设计：南下三五年 2011-08-05  12:45:55***********************/
void vCanSendTask( void *pvParameters )
{
	
	for(;;)
	{
		/*有CAN消息需要发送*/
		xSemaphoreTake( xCANDataReadySemaphore, portMAX_DELAY);
		/*缓冲区中有数据*/
		while(CanTxMsgEnd!=CanTxMsgBeg)
		{
			/*发送邮箱就绪*/
			if(pdTRUE==xSemaphoreTake( xCANSendSemaphore, ( portTickType ) 5 ))
			{
				CAN_Transmit(CAN1,&CanTXMsg[CanTxMsgEnd]);				
				/*数据压栈*/
				if( ++CanTxMsgEnd>= CAN_RxSize)	CanTxMsgEnd=0;
			}
		}
	}
}



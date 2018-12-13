/*
*********************************************************************************
*                                 STM32F103C8T6
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver1.1
*                                   �ʼҿƼ�
*
*ģ  �飺CANЭ�����
*����ߣ�Andy
*ʱ  �䣺2010-10-19
*********************************************************************************
*/
#include".\Lib\includes.h"

xSemaphoreHandle xCANSendSemaphore;  
xSemaphoreHandle xCANTxSemaphore, xCANRxSemaphore;  /*CAN�����ź���*/
xSemaphoreHandle xCANDataReadySemaphore;  

S_SERVERMSG PCMSG;
xQueueHandle     xPCServerQueue;            /**!����������Ӧ�����*/
extern xSemaphoreHandle xAskServerSemaphore;      /*!������Ӧ����ź���*/
/*************    ��������PackCanMsg              ****************************
�����ܡ�����CAN���У����з��,д�뻺����
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-26  01:19:55***********************/
void PackCanMsg(CanTxMsg SendMsg, INT8U cmd ,INT8U id, INT8U msgsyn)
{
	INT32U  gexid=0;
	INT32U  i=0;
	INT32U  cansyn=msgsyn;

	for(i=0;i<8;i++)
		CanTXMsg[CanTxMsgBeg].Data[i]=0;
	/*�����չ֡*/
	gexid=(((INT32U)cansyn)<<24)|(((INT32U)cmd)<<16)|(((INT32U)FLAG.id)<<8)|id; 

	CanTXMsg[CanTxMsgBeg].ExtId=gexid;   /*������������*/
	CanTXMsg[CanTxMsgBeg].IDE=CAN_ID_EXT;
	CanTXMsg[CanTxMsgBeg].RTR=CAN_RTR_DATA;
	CanTXMsg[CanTxMsgBeg].DLC=SendMsg.DLC;

	for(i=0;i<SendMsg.DLC;i++)
		CanTXMsg[CanTxMsgBeg].Data[i]=SendMsg.Data[i];

	/*����ѹջ*/
	if( ++CanTxMsgBeg>= CAN_RxSize)	CanTxMsgBeg=0;

	xSemaphoreGive( xCANDataReadySemaphore );  //ע�ⷢ��Ϣ�ĵط�
}

/*************    ��������ProcPage                ****************************
�����ܡ�������CAN����,��������ʽ
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-30  15:08:18***********************/
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
			/*!ˢ���������Զ��ұһ�����Э�����*/
			/*�ն�ID����*/
			case SETSLAVEID:
					#ifdef SAVECOIN
						FLAG.setcanid=1;
						FLAG.setcanlcd=1;
					#endif
				break;

			/*ID���ý���*/
			case EXECUTEID:
					#ifdef SAVECOIN
						ST.id=FLAG.id;
						SAVE3130(ST.id);
						CAN_Configuration();
						/*ʹ��CAN*/
						CAN_ITConfig(CAN1,CAN_IT_TME|CAN_IT_FMP0, ENABLE);
						FLAG.setcanid=0;
						gFreshTwo=1;
						FLAG.setcanlcd=0;
					#endif
				break;

			/*��������*/
			case SETTINGSYS:
				CARDMSG.unit=CanMsg[CanMsgBeg].Data[0];
				CARDMSG.maxcoin=((INT32U)CanMsg[CanMsgBeg].Data[2]<<8)+CanMsg[CanMsgBeg].Data[1];
				xSemaphoreGive( xAskServerSemaphore );

				/*!����Ϣ��24064ģ��*/
				#ifdef TWOLCDMODE
					#ifdef MOTHERCARD
					TxMsg.DLC=1;
					TxMsg.Data[0]=CARDMSG.unit;
					PackCanMsg(TxMsg, PARAMETERINIT24064, (FLAG.id+242),0);
					vTaskDelay(50);
					PackCanMsg(TxMsg, PARAMETERINIT24064, (FLAG.id+242),0);//����
					#endif
				#endif
				
				break;

			/*��ס�ն�*/
			case DISSYS:
				FLAG.lock=0;
				xSemaphoreGive( xAskServerSemaphore );
				TxMsg.DLC=0;
				PackCanMsg(TxMsg, DISSYS|0x80, MasterID,FLAG.syn);
				if(++FLAG.syn==31)  FLAG.syn=1;
				break;

			/*ʹ���ն�*/
			case ENABLESYS:
				FLAG.lock=1;
				xSemaphoreGive( xAskServerSemaphore );
				TxMsg.DLC=0;
				PackCanMsg(TxMsg, ENABLESYS|0x80, MasterID,FLAG.syn);
				if(++FLAG.syn==31)  FLAG.syn=1;
				//FLAG.msg=DISFIANDCARD;
				//xSemaphoreGive( xLCDRefreshSemaphore );

				break;

			/*�Ϸ�*/
			case COINUP:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				FLAG.setsysbill=1;
				
				CARDMSG.backcoin=CARDMSG.coin;
				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*�·�*/
			case COINDOWN:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				FLAG.setsysbill=1;
				
				CARDMSG.backcoin=CARDMSG.coin;
				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*�����ܷ�*/
			case REQUESTCOIN:
				CARDMSG.coin=((INT32U)CanMsg[CanMsgBeg].Data[7]<<24)+((INT32U)CanMsg[CanMsgBeg].Data[6]<<16)+
					((INT32U)CanMsg[CanMsgBeg].Data[5]<<8)+CanMsg[CanMsgBeg].Data[4];

				if(CARDMSG.coin>900000L)
					CARDMSG.coin=0;

				xSemaphoreGive( xAskServerSemaphore );
				
				break;

			/*�ն˾���*/
			case READYSYS:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*�������*/
			case JIESUO:
				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*��������*/
			case PEIZHI:
				//if(CanMsg[CanMsgBeg].Data[0]&0xf0)
					//CARDMSG.unit=(CanMsg[CanMsgBeg].Data[0]>>4)|0x80;
				//else
				CARDMSG.unit=CanMsg[CanMsgBeg].Data[0];
				CARDMSG.maxcoin=((INT32U)CanMsg[CanMsgBeg].Data[2]<<8)+CanMsg[CanMsgBeg].Data[1];
				CARDMSG.yajin=((INT16U)CanMsg[CanMsgBeg].Data[4]<<8)+CanMsg[CanMsgBeg].Data[3];
				
				xSemaphoreGive( xAskServerSemaphore );


				break;

			/*��̨Ͷ��*/
			case TOUBIOK:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*��̨�˱�*/
			case TUIBIOK:

				xSemaphoreGive( xAskServerSemaphore );
				break;

			/*��̨�˱�*/
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
				/*!У��ϵͳʱ�䣬��ϵ�豸ͨ���������㲥�Ϳ����ˣ��������豸��ͨ���ֳ��豸*/
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
		//����������
		for(i=0;i<8;i++)
			CanMsg[CanMsgBeg].Data[i]=0;
		CanMsg[CanMsgBeg].DLC=0;
		CanMsg[CanMsgBeg].ExtId=0;
		CanMsg[CanMsgBeg].StdId=0;
		
		if( ++CanMsgBeg >= CAN_RxSize)	CanMsgBeg=0;
	}
}
/*************    ��������WaitServerReady          ***************************
�����ܡ���ϵͳ��������Ҫ��������ϱ�״̬��ͬʱ��������Լ��Ĳ�����ͬʱ�������
          ̬���ȴ�����Ա�ĺ�������
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-08-12  22:45:21***********************/
void WaitServerReady(void)
{
#ifdef REQUESTSERVER
	CanTxMsg TxMsg;
//	INT8U number=0;

	/*!����ִ�ж�������Ҫ������������Ӧ��󣬲��뿪*/
	/*!����*/
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
			/*!����*/
			TxMsg.DLC=0;
			PackCanMsg(TxMsg, READYSYS, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;

	/*!����ִ�ж�������Ҫ������������Ӧ��󣬲��뿪*/
	FLAG.noasktime=0;
	/*!����*/
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
			/*!����*/
			TxMsg.DLC=0;
			PackCanMsg(TxMsg, PEIZHI, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;

	/*!����ִ�ж�������Ҫ������������Ӧ��󣬲��뿪*/
	FLAG.noasktime=0;
	/*!����*/
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
			/*!����*/
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
	/*!ȫ�Զ�*/
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
			/*!����*/
			TxMsg.DLC=2;
			TxMsg.Data[0]=0x64;
			TxMsg.Data[1]=0x00;
			PackCanMsg(TxMsg, GETCOINMONEY, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;

	vTaskDelay(10000);
	/*!���Զ�*/
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
			/*!����*/
			TxMsg.DLC=2;
			TxMsg.Data[0]=0xe8;
			TxMsg.Data[1]=0x03;
			PackCanMsg(TxMsg, SELLCOINSEMIMAC, MasterID,FLAG.syn);
		}
	}
	if(++FLAG.syn==31)  FLAG.syn=1;
	
}
/*************    ��������vCanprocTask             ***************************
�����ܡ���CAN�������񣬽���CAN���������Ӧ����
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-26  02:02:40***********************/
void vCanprocTask( void *pvParameters )
{	
	/*ʹ��CAN*/
	CAN_ITConfig(CAN1,CAN_IT_TME|CAN_IT_FMP0, ENABLE);
	DisCardMoney(12345678);
	WaitServerReady();
	DisCardMoney(0);

	#ifdef PRINTFMSG
	printf("Server is ready \r\n");
	#endif
	//!ϵͳ�����󣬷��ɴ�������
	xTaskCreate( vFM1702MangerTask, ( signed char * ) "FM1702", 512, NULL, 3, NULL );
	for(;;)
	{
		xSemaphoreTake( xCANRxSemaphore, portMAX_DELAY);
		
		ProcPage();
	}
}
/*************    ��������vCanSendTask            ****************************
�����ܡ���CAN�жϷ��ͺ���
����������
�����ء���
��˵������
         ��һ�������ﷸ��ɵ��д�˸���ѭ���������������һ�򿪣�����������޷�
         ���У������Ϊ���ź��������⣬�����ŷ���ԭ����������ɵɵɵ
**************   ��ƣ����������� 2011-08-05  12:45:55***********************/
void vCanSendTask( void *pvParameters )
{
	
	for(;;)
	{
		/*��CAN��Ϣ��Ҫ����*/
		xSemaphoreTake( xCANDataReadySemaphore, portMAX_DELAY);
		/*��������������*/
		while(CanTxMsgEnd!=CanTxMsgBeg)
		{
			/*�����������*/
			if(pdTRUE==xSemaphoreTake( xCANSendSemaphore, ( portTickType ) 5 ))
			{
				CAN_Transmit(CAN1,&CanTXMsg[CanTxMsgEnd]);				
				/*����ѹջ*/
				if( ++CanTxMsgEnd>= CAN_RxSize)	CanTxMsgEnd=0;
			}
		}
	}
}



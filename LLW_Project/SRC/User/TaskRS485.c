/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺ͨ�Ų���
*����ߣ�����������
*ʱ  �䣺2012-05-24  00:53:19
*********************************************************************************
*/
#include".\Lib\includes.h"
S_GAME mYGame;

INT8U S485MsgBeg=0;			//����һ������
INT8U S485MsgEnd=0;			//д��һ������

INT8U S485Msg[S485MsgSize][S485MsgBufferLen]={0};//���ݻ�����
S_S485 MyS485Msg;
S_CTRGAME CtrGame;
S_TRANSPC PC;
S_SENDSLAVEPARA MySendSlavePara;

//!����洢
S_SLAVEBILLGROUP MySlaveBill;
S_SLAVEYAFEN MySlaveYafen;
S_RELIST MyRelist;
S_KERNEL MyKernel;

INT8U gSendPackBuff[80]={0};
INT8U gSendPacket[80]={0};
INT32U gSendTestcnt=0;

xSemaphoreHandle xS485RecSemaphore;

//!��Ϸ����ѡ��
const INT8U gamepeilv[6][8]={
{  6,  6, 12, 12, 12, 12,  6,  6, },
{  6,  8,  8, 12, 12,  8,  8,  6, },
{  4,  8, 12, 24, 24, 12,  8,  4, },
{  3, 12, 24, 24, 24, 24, 12,  3, },
{  6,  6,  8, 24, 24,  8,  6,  6, },
{  4,  6, 24, 24, 24, 24,  6,  4, },
};
/*!ͨ����������߼����ٶ�*/
INT16U const crctab[16]=
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};

/*************    ��������Crc16Check              ****************************
�����ܡ���CRC16У��
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-06-17  22:15:52***********************/
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
/*************    ��������PacketMsg               ****************************
�����ܡ���broad = 1 ,��ʾ������֡Ϊ�㲥�������ʾ��Ե�ͨ��
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-21  13:46:13***********************/
void PacketMsg(INT8U *ptr, INT8U cmd, INT8U broad, INT8U addr,INT8U *pto,INT8U len)
{
	INT32U random=0;
	INT32U i=0;
	INT16U crc16=0;
	INT8U *check=pto;
	INT8U backlen=len;

	/*!����4�ֽڵ����������*/
	random=IRandomX(1,0xffffff);

	/*!4�ֽڸ�����*/
	for(i=0;i<4;i++)
	{
		*pto++=random>>(8*i);
	}
	*pto++=0x80;  //!������ַ
	if(broad)
		*pto++=0x00;//!�㲥
	else
		*pto++=0x01;//!Ŀ�������ַ

	*pto++=backlen+1;  //!����
	*pto++=cmd|0x80;  //!����
	
	while(len--)
	{
		*pto++=*ptr++; //!�������
	}

	if(broad)
	{
		for(i=0;i<16;i++)
		{
			*pto++=i+1;
			*pto++=MySlaveBill.slavebill[i].slaveidcheck&0xff;
			*pto++=(MySlaveBill.slavebill[i].slaveidcheck>>8)&0xff;
		}

		/*!����CRC16 */
		crc16=Crc16Check(check,(backlen+8+48));
	}
	else
	{
		*pto++=addr;
		*pto++=MySlaveBill.slavebill[addr-1].slaveidcheck&0xff;
		*pto++=(MySlaveBill.slavebill[addr-1].slaveidcheck>>8)&0xff;
		
			
		/*!����CRC16 */
		crc16=Crc16Check(check,(backlen+8+3));
	}

	*pto++=crc16;
	*pto++=crc16>>8;
}
/*************    ��������SendSlaveS485             **************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-20  19:30:07***********************/
void SendSlaveS485(INT8U *data, INT8U len)
{
	INT8U i=0;

	SEND485;//!ʹ�ܷ���
	vTaskDelay(1);
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1, (INT8U )(*data++));
		/*!��Ҫע�⣬�����жϷ�������õ���TC������TXE��������������
		�����������л���485״̬���������һ���ֽڲ��ܷ��ͳ�ȥ*/
    	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
	vTaskDelay(1);
	RECEIVE485;//!ʹ�ܽ���
}

/*************    ������: CheckPacket  **************************************
�����ܡ�:
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-20 3:34:13 **********************/
INT8U CheckPacket(INT8U *data , INT8U len)
{
	INT16U crc16=0;
	INT16U crc16rec=0;

	/*!CRC16�ж�*/
	crc16=Crc16Check(data,(len-2));
	crc16rec=*(data+len-2)+(*(data+len-1)*256);

	if(crc16==crc16rec)
		return 1;
	else
		return 0;
}
/*************    ������: CreateAllSaveId  *********************************
�����ܡ�:�����зֻ�
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-26 16:57:33 **********************/
void CreateAllSaveId(void)
{
	int i=0;

	for(i=0;i<MAXSLAVE;i++)
	{
		/*!��ȡ�˵�*/
		taskENTER_CRITICAL(); /*����������*/
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
		taskEXIT_CRITICAL(); /*����������*/
		
		vTaskDelay(100);
	}
	
	vTaskDelay(100);
}
/*************    ��������ProcessR485Msg          ****************************
�����ܡ���
����������
�����ء���
��˵������
		�������������
**************   ��ƣ����������� 2012-11-17  00:04:22***********************/
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
		//!ֻ����������
		if(err)
		{
			err=CheckPacket(&S485Msg[S485MsgBeg][0],S485Msg[S485MsgBeg][S485MsgBufferLen-1]);

			if(err)
			{
				switch(S485Msg[S485MsgBeg][8]&0x7f)
				{
					/*!��ȡ�ֻ��˵�*/
					//!ָ���Ƿ���������Ҫע�� ���14 ��Ʊ 16711680
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

					/*!��ȡ�ֻ�Ѻ����Ϣ*/
					case SHARK_GETBET:
						//!��ʱû��ʹ�õ�
						temp=(INT8U *)(&MySlaveYafen.yafen[S485Msg[S485MsgBeg][4]-1][0]);
						for(i=0;i<26;i++)
						{
							*(temp+i)=S485Msg[S485MsgBeg][9+i];
						}
						break;

					/*!�󶨷ֻ���Ϣ*/
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

/*************    ��������SendGameListPC          ****************************
�����ܡ���������ʷ��¼��PC
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-30  13:32:24***********************/
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
		
		USBTxMsg[USBTxMsgEnd][0]=64;//���ͳ���
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x16;  //����
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255); //������
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_RELIST;  //����
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //ͬ����
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

/*************    ��������SendGameStart              *************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-30  13:35:53***********************/
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
		
		USBTxMsg[USBTxMsgEnd][0]=64;//���ͳ���
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x03;  //����
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //������
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_START;  //����
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //ͬ����
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

/*************    ��������WaitPCAskOpen            ***************************
�����ܡ���
����������
�����ء���
��˵������
		2013-08-17
         ���Թ����з��֣��ȴ������е��Է������ݣ���ʧ���������ȣ������������
         ��ѯ
**************   ��ƣ����������� 2012-11-30  13:39:53***********************/
void WaitPCAskOpen(void)
{
	INT16U crcsend=0;
	
#ifndef NOSENDPC
	//!�ȴ�PC������
	do
	{
		if(1==FLAG.resetgame)
		{
			break;
		}

		//2013-08-17
		//------------------------------------------------------
		#if 0
		USBTxMsg[USBTxMsgEnd][0]=64;//���ͳ���
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x02;  //����
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //������
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_WAITPCASKOPEN;  //����
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //ͬ����

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
		
		USBTxMsg[USBTxMsgEnd][0]=64;//���ͳ���
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x0a;  //����
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //������
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_GETOPEN|0x80;  //����
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //ͬ����
		USBTxMsg[USBTxMsgEnd][10]=PC.openshark; //����
		USBTxMsg[USBTxMsgEnd][11]=PC.sharkcoor;//��������
		USBTxMsg[USBTxMsgEnd][12]=PC.sharkpeilv;//��������
		USBTxMsg[USBTxMsgEnd][13]=PC.secondopen;//���ο����Ķ���
		USBTxMsg[USBTxMsgEnd][14]=PC.secondpeilv;//���ο����Ķ�������
		USBTxMsg[USBTxMsgEnd][15]=PC.thirdopen;//���ο����Ķ���
		USBTxMsg[USBTxMsgEnd][16]=PC.thirdpeilv;//���ο����Ķ�������
		USBTxMsg[USBTxMsgEnd][17]=PC.boundpelv;//�ʽ�����
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

/*************    ������: WaitPCRunLedOver  *********************************
�����ܡ�:�ȴ�PC�ܵƽ���
��������:
�����ء�:
��˵����:
         2013-08-17
         ���Թ����з��֣��ȴ������е��Է������ݣ���ʧ���������ȣ������������
         ��ѯ
**************   ���: ���������� 2013-03-19 15:14:18 **********************/
void WaitPCRunLedOver(void)
{
	do
	{
		if(1==FLAG.resetgame)
			break;

		//2013-08-17
		//------------------------------------------------------
		#if 0
		USBTxMsg[USBTxMsgEnd][0]=64;//���ͳ���
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x02;  //����
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //������
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_WAITPCRUNOVER;  //����
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //ͬ����

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

/*************    ������: EnableSlaveBet  **********************************
�����ܡ�:����ֻ�Ѻ��
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-19 15:25:5 **********************/
void EnableSlaveBet(void)
{
	taskENTER_CRITICAL(); /*����������*/
	gSendPackBuff[0]=mYGame.peilv;//!Ѻ������
	gSendPackBuff[1]=MyKernel.setpara.bettime;//!Ѻ������
	PacketMsg(gSendPackBuff,SHARK_START,1,7,gSendPacket,2);
	SendSlaveS485(gSendPacket,2+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: EnableOpenShark  **********************************
�����ܡ�:�Ƿ������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-19 16:40:50 **********************/
void EnableOpenShark(void)
{
	portTickType xLastWakeTime;
	
	taskENTER_CRITICAL(); /*����������*/
	gSendPackBuff[0]=PC.openshark;//!�Ƿ������
	PacketMsg(gSendPackBuff,SHARK_STOPBET,1,7,gSendPacket,1);
	SendSlaveS485(gSendPacket,1+58);
	taskEXIT_CRITICAL(); /*����������*/

	if(PC.openshark)
	{
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil( &xLastWakeTime, configTICK_RATE_HZ*12);

		taskENTER_CRITICAL(); /*����������*/
		PacketMsg(gSendPackBuff,SHARK_STOPSHARKSHOW,1,1,gSendPacket,0);
		SendSlaveS485(gSendPacket,58);
		taskEXIT_CRITICAL(); /*����������*/
	}
}

/*************    ������: SendSlavePara  ************************************
�����ܡ�:���·ֻ�����
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-27 20:34:47 **********************/
INT32U gBaojiWin[3]={500000l,500000l,500000l,};
void SendSlavePara(void)
{
	INT8U *temp=NULL;
	INT8U i=0;

	taskENTER_CRITICAL(); /*����������*/
	
	/*!���ֻ�����ת����������*/
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
	
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: SendTestMsg  **************************************
�����ܡ�:����ͨ�Ų���
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-28 1:51:31 **********************/
void SendTestMsg(void)
{
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_TESTCOMMUNICATION,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: SendRestSlave  ***********************************
�����ܡ�:��λ�ֻ�
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-28 1:55:39 **********************/
void SendRestSlave(void)
{
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_RESETSLAVE,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: SlaveBetOver  **************************************
�����ܡ�:�ֻ�����Ч��
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-19 16:44:23 **********************/
void SlaveBetOver(void)
{
	INT32U bounds=0;

	bounds=IRandomX(100,100000);
		
	if(PC.sharkcoor)
	{
		/*!��������*/
		taskENTER_CRITICAL(); /*����������*/
		gSendPackBuff[0]=mYGame.open;//!��Ϸ���
		gSendPackBuff[1]=mYGame.sharkpeilv;//!��Ϸ���
		//!�������ʽ�ͬ��
		//2013-08-20
		//---------------------------------------------------
		gSendPackBuff[2]=bounds;
		gSendPackBuff[3]=bounds>>8;
		gSendPackBuff[4]=bounds>>16;
		gSendPackBuff[5]=bounds>>24;
		//---------------------------------------------------
		PacketMsg(gSendPackBuff,SHARK_OPEN,1,7,gSendPacket,6);//2
		SendSlaveS485(gSendPacket,6+58);//2
		taskEXIT_CRITICAL(); /*����������*/

		//!�ȴ�PCָ��������������͸��ֻ�
		WaitPCRunLedOver();

		//!�����ʧ
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
		
		/*!�����͵�*/
		if((mYGame.second==8)||(mYGame.second==9))
		{
			taskENTER_CRITICAL(); /*����������*/
			gSendPackBuff[0]=mYGame.second;//!��Ϸ���
			gSendPackBuff[1]=mYGame.secondpeilv;
			PacketMsg(gSendPackBuff,SHARK_HAPPY,1,7,gSendPacket,2);
			SendSlaveS485(gSendPacket,2+58);
			taskEXIT_CRITICAL(); /*����������*/

			//!�ȴ�PCָ��������������͸��ֻ�
			WaitPCRunLedOver();

			//!�����ʧ
			if(8==mYGame.second)
			{
				MyKernel.realbill.totalwin+=MyRelist.allyafen[11]*mYGame.secondpeilv;
			}
			else if(9==mYGame.second)
			{
				MyKernel.realbill.totalwin+=MyRelist.allyafen[10]*mYGame.secondpeilv;
			}
			
			SendCurrentAllBill();
			
			taskENTER_CRITICAL(); /*����������*/
			gSendPackBuff[0]=mYGame.third;//!��Ϸ���
			PacketMsg(gSendPackBuff,SHARK_HAPPYTHREE,1,7,gSendPacket,1);
			SendSlaveS485(gSendPacket,1+58);
			taskEXIT_CRITICAL(); /*����������*/

			//!�����ʧ
			MyKernel.realbill.totalwin+=MyRelist.allyafen[mYGame.third]*
					gamepeilv[mYGame.peilv][mYGame.third];

			/*!����ׯ�е÷�*/
			if((mYGame.open>3)&&(mYGame.open<8))
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[12]*2);
			else
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[8]*2);

			SendCurrentAllBill();

			if(mYGame.open==10)
			{
				//!�ȴ�PCָ��������������͸��ֻ�
				WaitPCRunLedOver();

				//!�����ʧ
				MyKernel.realbill.totalwin+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;
				MyKernel.realbill.totalbounds+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;
				SendCurrentAllBill();
				
				/*!��������*/
				taskENTER_CRITICAL(); /*����������*/
				gSendPackBuff[0]=mYGame.boundspeilv;//!��Ϸ�ʽ�
				PacketMsg(gSendPackBuff,SHARK_BOUNDS,1,7,gSendPacket,1);
				SendSlaveS485(gSendPacket,1+58);
				taskEXIT_CRITICAL(); /*����������*/
			}
		}
		else
		{
			taskENTER_CRITICAL(); /*����������*/
			gSendPackBuff[0]=mYGame.second;//!��Ϸ���
			PacketMsg(gSendPackBuff,SHARK_HAPPY,1,7,gSendPacket,1);
			SendSlaveS485(gSendPacket,1+58);
			taskEXIT_CRITICAL(); /*����������*/

			//!�����ʧ
			MyKernel.realbill.totalwin+=MyRelist.allyafen[mYGame.second]*
					gamepeilv[mYGame.peilv][mYGame.second];

			/*!����ׯ�е÷�*/
			if((mYGame.open>3)&&(mYGame.open<8))
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[12]*2);
			else
				MyKernel.realbill.totalwin+=(MyRelist.allyafen[8]*2);
			SendCurrentAllBill();
			
			if(mYGame.open==10)
			{
				//!�ȴ�PCָ��������������͸��ֻ�
				WaitPCRunLedOver();

				//!�����ʧ
				MyKernel.realbill.totalwin+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;

				MyKernel.realbill.totalbounds+=MyRelist.allyafen[10]*
					mYGame.boundspeilv;
				SendCurrentAllBill();
				
				/*!��������*/
				taskENTER_CRITICAL(); /*����������*/
				gSendPackBuff[0]=mYGame.boundspeilv;//!��Ϸ�ʽ�
				PacketMsg(gSendPackBuff,SHARK_BOUNDS,1,7,gSendPacket,1);
				SendSlaveS485(gSendPacket,1+58);
				taskEXIT_CRITICAL(); /*����������*/
			}
		}
	}
	else
	{
		taskENTER_CRITICAL(); /*����������*/
		gSendPackBuff[0]=mYGame.open;//!��Ϸ���
		gSendPackBuff[1]=0;
		//!�������ʽ�ͬ��
		//2013-08-20
		//---------------------------------------------------
		gSendPackBuff[2]=bounds;
		gSendPackBuff[3]=bounds>>8;
		gSendPackBuff[4]=bounds>>16;
		gSendPackBuff[5]=bounds>>24;
		//---------------------------------------------------
		
		PacketMsg(gSendPackBuff,SHARK_OPEN,1,7,gSendPacket,6);//2
		SendSlaveS485(gSendPacket,6+58);//2
		taskEXIT_CRITICAL(); /*����������*/

		//!�����ʧ
		MyKernel.realbill.totalwin+=(MyRelist.allyafen[mYGame.open]*
					gamepeilv[mYGame.peilv][mYGame.open]);
		/*!����ׯ�е÷�*/
		if((mYGame.open>3)&&(mYGame.open<8))
			MyKernel.realbill.totalwin+=(MyRelist.allyafen[12]*2);
		else
			MyKernel.realbill.totalwin+=(MyRelist.allyafen[8]*2);
		
		SendCurrentAllBill();
	}
}

/*************    ������: GetAllSlaveBill  *********************************
�����ܡ�:��ȡ���зֻ��˵�
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-19 17:5:14 **********************/
void GetAllSlaveBill(void)
{
	int i=0;

	for(i=0;i<MAXSLAVE;i++)
	{
		/*!��ȡ�˵�*/
		taskENTER_CRITICAL(); /*����������*/
		PacketMsg(gSendPackBuff,SHARK_GETBILL,0,i+1,gSendPacket,0);
		SendSlaveS485(gSendPacket,13);
		taskEXIT_CRITICAL(); /*����������*/
		vTaskDelay(100);
	}
}

/*************    ������: SendSlavePCUnReady  *******************************
�����ܡ�:֪ͨ���зֻ������Բ�����
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-28 21:25:26 **********************/
void SendSlavePCUnReady(void)
{
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_SERVERDROP,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: SendSlavePCGameWrong  *****************************
�����ܡ�:PC��Ϸ��ƥ��
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-28 21:27:22 **********************/
void SendSlavePCGameWrong(void)
{
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_PCGAMEWRONG,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: SendSlavePCComWrong  ******************************
�����ܡ�:PCͨ�Ŵ���
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-28 21:28:21 **********************/
void SendSlavePCComWrong(void)
{
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_PCMSGWRONG,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: SendSlaveRunTimeOver  ****************************
�����ܡ�:ϵͳ����ʱ�䵽������ʱ
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-28 21:29:31 **********************/
void SendSlaveRunTimeOver(void)
{
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_RUNTIMEOVER,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: SendSlaveBillOver  *******************************
�����ܡ�:֪ͨ�ֻ���������Ŀ�����뱨��
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-28 21:30:17 **********************/
void SendSlaveBillOver(void)
{
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_GAINOVER,1,7,gSendPacket,0);
	SendSlaveS485(gSendPacket,0+58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: UpDateHistoryList  *******************************
�����ܡ�:������ʷ��¼
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-19 17:8:30 **********************/
void UpDateHistoryList(void)
{
	INT8U i=0;

	taskENTER_CRITICAL(); /*����������*/
	for(i=20;i>0;i--)
			MyRelist.gamelist[i]=MyRelist.gamelist[i-1];

	/*!������ʷ��¼����������*/
	if(PC.sharkcoor)
		MyRelist.gamelist[0]=PC.sharkcoor;
	else
		MyRelist.gamelist[0]=PC.thirdopen;

	SAVEFM16W08RELISTISR(MyRelist.gamelist);
	//SAVEFM16W08ISR(MyRelist.gamelist);

	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: WaitGameOver  ************************************
�����ܡ�:�ȴ���Ϸ����
��������:
�����ء�:
��˵����:
		 2013-08-17
         ���Թ����з��֣��ȴ������е��Է������ݣ���ʧ���������ȣ������������
         ��ѯ
**************   ���: ���������� 2013-03-20 1:55:29 **********************/
void WaitGameOver(void)
{
	do
	{
		if(1==FLAG.resetgame)
			break;

		//2013-08-17
		//------------------------------------------------------
		#if 0
		USBTxMsg[USBTxMsgEnd][0]=64;//���ͳ���
		USBTxMsg[USBTxMsgEnd][1]='H';
		USBTxMsg[USBTxMsgEnd][2]='J';
		USBTxMsg[USBTxMsgEnd][3]=0x02;  //����
		USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);  //������
		USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][7]=IRandomX(0,255);
		USBTxMsg[USBTxMsgEnd][8]=PC_SHARK_WAITPCGAMEOVER;  //����
		USBTxMsg[USBTxMsgEnd][9]=0x00;  //ͬ����

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

/*************    ������: SaveAllBill  **************************************
�����ܡ�:������Ŀ
��������:
�����ء�:
��˵����:
        �������ʣ���������ֻ�����Ŀ��������Ա���ʵ������ļ������¼��㣬
        ����ʵ������==ȫ����ӯ��-�ϴ���ӯ��
**************   ���: ���������� 2013-03-20 18:3:19 **********************/
void SaveAllBill(void)
{
	int i=0;
	taskENTER_CRITICAL(); /*����������*/
	
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
	MyKernel.realbill.totalbetin=0;    //!ϵͳ���еĺ��Ĳ�����ͨ����ֵ�ж�Ӫ��
	MyKernel.realbill.totalbetout=0;   //!ϵͳ���еĺ��Ĳ���
	
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
		//!��ֹ�ܽ�������ת,���·�ˮ
		if(MyKernel.realbill.totalbetin<0xefffffff)
			MyKernel.realbill.totalbetin+=MySlaveBill.slavebill[i].totalbetin;
		
		MyKernel.realbill.totalbetout+=MySlaveBill.slavebill[i].totalbetout;
	}
	MyKernel.realbill.totalgaincurrent=MyKernel.realbill.totalgain-
		              MyKernel.realbill.totalgainpre;

	//SAVEFM16W08ISR(MyKernel.realbill);
	SAVEFM16W08KERNELISR(MyKernel.realbill);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: GetAllBetMsg  *************************************
�����ܡ�:��ȡ���зֻ���Ѻ����Ϣ
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-21 11:45:29 **********************/
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
		/*!��ȡ�˵�*/
		taskENTER_CRITICAL(); /*����������*/
		PacketMsg(gSendPackBuff,SHARK_GETBET,0,i+1,gSendPacket,0);
		SendSlaveS485(gSendPacket,13);
		taskEXIT_CRITICAL(); /*����������*/
		vTaskDelay(100);
	}
}

/*************    ������: SaveAllBet  **************************************
�����ܡ�:�������зֻ�Ѻ��
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-21 12:50:36 **********************/
void SaveAllBet(void)
{
	INT8U i=0,j=0;

	taskENTER_CRITICAL(); /*����������*/

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
	taskEXIT_CRITICAL(); /*����������*/
}
/*************    ������: ClearSlaveBill  ***********************************
�����ܡ�:����˵�
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-21 11:53:58 **********************/
void ClearSlaveBill(void)
{
	/*!��ȡ�˵�*/
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_CLEARBILL,1,1,gSendPacket,0);
	SendSlaveS485(gSendPacket,58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: ClearSlaveBillCurrent  ***************************
�����ܡ�:����ֻ��ı���ʵ������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-06-18 12:0:21 **********************/
void ClearSlaveBillCurrent(void)
{
	/*!��ȡ�˵�*/
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_CLEARBILLCURRENT,1,1,gSendPacket,0);
	SendSlaveS485(gSendPacket,58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: StopSlaveBeting  **********************************
�����ܡ�:ֹͣ���зֻ�Ѻ��
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-21 15:37:55 **********************/
void StopSlaveBeting(void)
{
	/*!��ȡ�˵�*/
	taskENTER_CRITICAL(); /*����������*/
	PacketMsg(gSendPackBuff,SHARK_NOTIMEBET,1,1,gSendPacket,0);
	SendSlaveS485(gSendPacket,58);
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: vRS485RecTask  **********************************
�����ܡ�:485�ֻ��������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-03-20 3:17:3 **********************/
void vRS485RecTask( void *pvParameters )
{
	for(;;)
	{
		xSemaphoreTake( xS485RecSemaphore, ( portTickType ) portMAX_DELAY);

		taskENTER_CRITICAL(); /*����������*/
		ProcessR485Msg();
		taskEXIT_CRITICAL(); /*����������*/
	}
}

/*************    ��������vGamePlayerTask          ***************************
�����ܡ�������Ϸ����
����������
�����ء���
��˵������
		��Ϸ����:
		1���ֻ��Լ�
		2���ȴ��������������ӣ�����������Ϣ
		3�������������������Ϸ��ʼ������Ѻ��
		4��Ѻ�ֽ������ȴ���Ϸ���
		5������������Ϸ�����������Ӧ����
		6�������ռ���Ŀ����ʼ��һ����Ϸ

		2013-07-03 �޸�:
		1��ͨ�Ų��֣�����ͨ�ű���ͨ��Ӧ��ķ�ʽ������������𶪰����ڳ�������
		�У����ֶ�������
		2������������ĸ��ʣ����Ѷȵȼ�Ϊ���ѵ�����£�����̫�٣��ʼӴ�������
		�ĸ���
		
**************   ��ƣ����������� 2012-11-16  17:53:23***********************/
void vGamePlayerTask( void *pvParameters )
{
	portTickType xLastWakeTime;

	mYGame.step=GAME_WAITMOTHER;

	/*!ʹ�õ�ǰʱ�� ��ʼ��xLastWakeTime ����*/
    xLastWakeTime = xTaskGetTickCount();
	vTaskDelay(3000);
	xSemaphoreTake( xPCASKSemaphore, ( portTickType ) 0);
	FLAG.resetgame=1;

	for(;;)
	{	
		//!���ͺ��Ĳ�����PC
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
		
		//!�����˵�  ok
		SendCurrentAllBill();

		//!PC������ʷ��¼  ok
		SendGameListPC();

		//!��������
		vTaskDelay(50);
		if(FLAG.lockgametask)
		{
			SendGameSpend();
			vTaskDelay(50);
			vTaskSuspend( NULL );
		}
		
		/*!��Ϸ��ʼ�����룬����Ѻ��*/
		//!�����������
		if(0==MyKernel.setpara.oddsselect)
		{
			mYGame.peilv=IRandomX(0,5);
		}
		else
			mYGame.peilv=MyKernel.setpara.oddsselect-1;
		
		//�������������  ok
		SendGameStart();

		//��������ֻ�,��Ϸ��ʼ����������,����Ѻ��
		if(0==FLAG.resetgame)
			EnableSlaveBet();

		/*!�ȴ��������*/
		if(0==FLAG.resetgame)
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil( &xLastWakeTime, configTICK_RATE_HZ*MyKernel.setpara.bettime);

			vTaskDelay(500);
			//!��ȡѺ�֣����н���ж�
			StopSlaveBeting();
			vTaskDelay(100);
			GetAllBetMsg();
			SaveAllBet();
		}

		if(0==FLAG.resetgame)
		{
			//!����Ѻ����Ϣ  ok
			SendCurrentAllBill();
			
			//!������Ϸ���
			HappyGameSet();
			
			//!����Ϸ���ת����PCָʾ�ܵ�λ��
			GetGameOpenPC();
		}
		
		//!�ȴ�PC����ȡ���
		WaitPCAskOpen();

		/*!�Ƿ������*/
		if(0==FLAG.resetgame)
			EnableOpenShark();
		
		//!�ȴ�PCָ��������������͸��ֻ�
		WaitPCRunLedOver();
		
		/*!��Ϸ���*/
		if(0==FLAG.resetgame)
			SlaveBetOver();

		/*!�ȴ���Ϸ����*/
		WaitGameOver();

		if(0==FLAG.resetgame)
		{
			/*!������ʷ��¼*/
			UpDateHistoryList();

			/*!��ȡ���зֻ��˵�*/
			GetAllSlaveBill();

			/*!����ʵʱ��Ŀ*/
			SaveAllBill();
		}
	}
}




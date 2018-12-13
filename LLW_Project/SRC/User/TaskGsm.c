#include"includes.h"
#define HEADER1 0x24
#define HEADER2 0x40
typedef struct{
  u8 SysType[2];          //ϵͳ�豸����
  u8 FactoryId[4];      //���ұ��
  u8 MachineType[4];    //��̨����
  u8 HardwareVer[4];    //Ӳ���汾
  u8 BoardUniqueId[12];     //����ΨһID
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
 * ������:ComSendPacket
 * ˵��: 
 *    ���ڷ��� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void GsmSendPacket(u16 cmd, u8 *dat,u8 len,u8 sync)
{
	u16 crccheck=0;
	u8  random=0;
	u8 i=0;
	/*!�������*/ 
	GsmSend.Msg[GsmSend.MsgEnd][0]=HEADER1;			
	GsmSend.Msg[GsmSend.MsgEnd][1]=HEADER2;	
	GsmSend.Msg[GsmSend.MsgEnd][2]=cmd>>8;
	GsmSend.Msg[GsmSend.MsgEnd][3]=0xff&cmd;
	GsmSend.Msg[GsmSend.MsgEnd][4]=6+len;	//���ݳ���////////////////////
	for(i=0;i<len;i++)
	{
		GsmSend.Msg[GsmSend.MsgEnd][5+i]=dat[i];
	}
	GsmSend.Msg[GsmSend.MsgEnd][5+i]=sync+1;//����Ψһ��
	GsmSend.Msg[GsmSend.MsgEnd][6+i]=sync+2;//
	GsmSend.Msg[GsmSend.MsgEnd][7+i]=sync+3;//
	GsmSend.Msg[GsmSend.MsgEnd][8+i]=sync+4;//
	GsmSend.Msg[GsmSend.MsgEnd][9+i]=sync+5;//
	GsmSend.Msg[GsmSend.MsgEnd][10+i]=sync+6;//
	
	GsmSend.Msg[GsmSend.MsgEnd][11+i]=GetXorCal(&GsmSend.Msg[GsmSend.MsgEnd][2],len+9);
	GsmSend.Msg[GsmSend.MsgEnd][12+i]=0x0d;//
	GsmSend.Msg[GsmSend.MsgEnd][13+i]=0x0a;//
	//!У��
	//crccheck=Crc16(&GsmSend.Msg[GsmSend.MsgEnd][0], len+10);
//	GsmSend.Msg[GsmSend.MsgEnd][10+len]=crccheck;
//	GsmSend.Msg[GsmSend.MsgEnd][11+len]=crccheck>>8;

	//!ת��Ϊ����
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
 * ������:GsmSendWaitAsk
 * ˵��: 
 *    �������ݸ���ģ�飬��ҪӦ�� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void GsmSendWaitAsk(u16 Cmd,u8 *Data,u8 len, u16 AskCmd, u8 TimeOut)
{
	u8 RecCmd=0;
	u8 SendCnt=0;
//!��յ�ǰ���У��ȴ�����
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
	}while((pdTRUE!=xQueueReceive( xGsmAskQueue, &( RecCmd ), 3000 )));//3���ط�һ��
	if(Flag.TxSync<255)
	{
		Flag.TxSync++;
	}
	else
	{
		Flag.TxSync=0;
	}
	//!��յ�ǰ���У��ȴ�����
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
	xQueueReceive( xGsmAskQueue, &( RecCmd ), 0 );
}
/******************************************************************************
 * ������:GsmSendWaitAsk
 * ˵��: 
 *    ��̨�����ϵ�ָ�����ģ�飬��ҪӦ�� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void ClientSendOn(void)
{
	u8 i=0;
	
	GsmAskBuf[0]=0x00;//ϵͳ�豸���� (Ͷ����0001)(��Ʒ��0002)(¶��ģ���0003)(�۱һ�0004)(������0005)(������0006)(�����0007)(��Ʒ�����Ͷ�ά��4098)()
	GsmAskBuf[1]=0x02;//�豸���� �����޸�
	GsmAskBuf[2]='C';//���ұ��//�����ӦCG52 �����޸�
	GsmAskBuf[3]='G';//���ұ��
	GsmAskBuf[4]='5';//���ұ��
	GsmAskBuf[5]='2';//���ұ��
	#if 1
	GsmAskBuf[6]='R';//�������ʹ��� �����Զ��� 01��Ʊ��
	GsmAskBuf[7]='X';//�������ʹ��� 
	GsmAskBuf[8]='0';//�������ʹ���
	GsmAskBuf[9]='1';//�������ʹ���
	
	GsmAskBuf[10]='R';//Ӳ������汾
	GsmAskBuf[11]='X';//Ӳ������汾
	GsmAskBuf[12]='0';//Ӳ������汾
	GsmAskBuf[13]='1';//Ӳ������汾
	#else
  GsmAskBuf[6]=0;//�������ʹ��� �����Զ��� 01��Ʊ��
	GsmAskBuf[7]=0;//�������ʹ��� 
	GsmAskBuf[8]=0;//�������ʹ���
	GsmAskBuf[9]=0;//�������ʹ���
	
	GsmAskBuf[10]=0;//Ӳ������汾
	GsmAskBuf[11]=0;//Ӳ������汾
	GsmAskBuf[12]=0;//Ӳ������汾
	GsmAskBuf[13]=0;//Ӳ������汾	
  #endif
	//12byte�ֽ�ΨһID
	for(i=0;i<12;i++)
		GsmAskBuf[14+i]=0;
	GsmSendWaitAsk(CMD_CLIENTONSYNC,GsmAskBuf,26,CMD_CLIENTONSYNC,0);
}
/******************************************************************************
 * ������:GsmSendWaitAsk
 * ˵��: 
 *    ��̨�����ϵ�ָ�����ģ�飬��ҪӦ�� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
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
 * ������:ClientReadUserPara
 * ˵��: 
 *    ��̨���Ͷ��Զ������
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void ClientReadUserPara(void)
{
	u8 i=0;
	GsmAskBuf[0]=rand()%255;//ȡ�����
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
 * ������:ClientReadMachineID
 * ˵��: 
 *    ��ȡ��ģ���ΨһID
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void ClientReadServerID(void)
{
	u8 i=0;
	GsmSendWaitAsk(CMD_CLIENTREADID,GsmAskBuf,0,CMD_CLIENTREADID,0);
}
/******************************************************************************
 * ������:ClientReadUserPara
 * ˵��: 
 *    ��̨���Ͷ��Զ������
 * ����: 
 * ���: 
 * ����: 
 * ���:
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
//��ģ���ȡ����Ӧ��
void ReadParaAck(u8 *dat)
{
	u8 i=0;
	/*!�������*/ 
	GsmSend.Msg[GsmSend.MsgEnd][0]=HEADER1;			
	GsmSend.Msg[GsmSend.MsgEnd][1]=HEADER2;	
	GsmSend.Msg[GsmSend.MsgEnd][2]='d';
	GsmSend.Msg[GsmSend.MsgEnd][3]='d';
	GsmSend.Msg[GsmSend.MsgEnd][4]=26;	//���ݳ���////////////////////
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
		case RUNCTL://ͣ��ģʽ���� 5
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
	//Ψһ��
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
 * ������:XorCheck
 * ˵��: 
 *    ��֤���������� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
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
 * ������:CheckPacket
 * ˵��: 
 *    ��֤���������� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
u8 GsmCheckPacket(u8 *data , u8 len)
{
	u8 code=0;
	u8 i=0;
	u16 crc1=0,crc=0;
	if((data[0]==HEADER1)&&(data[1]==HEADER2))//֡ͷ���
	{
		#if MSG_ENCRYPT
		code=data[3];//�ܳ�
		for(i=0;i<data[2]-7;i++)
		{
			data[7+i]^=code;//��������
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
	{//ID��ͬ
		return 0;
	}
	for(i=0;i<6;i++)//�����µ�ID
	{
		GsmRec.SoleId[i]=dat[i];
	}
	return 1;
}
/******************************************************************************
 * ������:ProcessComMsg
 * ˵��: 
 *    �ֻ�ͨ�Ŵ��� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void GsmProcessComMsg(void)
{
	u8 check=0;
	u16 cmd=0;
	u32 i=0;
	//u8 *ptr=NULL;
	while(GsmRec.MsgBegin!=GsmRec.MsgEnd)
	{
		//!������������
		check=GsmCheckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0],GsmRec.Msg[GsmRec.MsgBegin][MAXMSGBUFFERLEN-1]);
		if(check)
		{
			check=SoleIdCheck(&GsmRec.Msg[GsmRec.MsgBegin][GsmRec.Msg[GsmRec.MsgBegin][4]-1]);
			cmd=(GsmRec.Msg[GsmRec.MsgBegin][2]<<8)|GsmRec.Msg[GsmRec.MsgBegin][3];
			if(check)
			{//��Ψһ��
				switch(cmd)
				{
					case CMD_SERVECOIN://��Ͷ��ָ�� aa
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//ԭָ��Ӧ��
						printf("��Ͷ��%d",GsmRec.Msg[GsmRec.MsgBegin][5]);
						break;
					case CMD_SERVEONSYNC://��ģ���ϵ�ͬ��ָ�� kk ԭ���ݷ���
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);
						Flag.ServerOn=1;//��ģ�����ϵ�
            printf("���ϵ�ͬ��");
						break;
					
					case CMD_SERVESTATUS://��ģ��״ָ̬�� jj
						Flag.ServerStatus=GsmRec.Msg[GsmRec.MsgBegin][5];//��ģ��״̬
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//Ӧ��ָ��
						break;
					
					case CMD_SERVEREADPARA://��ģ���ѯ�������ee ����dd
						ReadParaAck(&GsmRec.Msg[GsmRec.MsgBegin][5]);
						printf("��ѯ�������");
						break;
					case CMD_SERVESETDEFAULT://��ģ����������ָ�����ff
						SystemParaDefault(SETDEFAULT_BILL_PARA);
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//Ӧ��ָ��
						printf("�ָ�����");
						break;
					case CMD_SERVESETPARA://��ģ�������������dd
						ParaSet(&GsmRec.Msg[GsmRec.MsgBegin][5]);//���ò���
						GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//Ӧ��ָ��
						printf("�����������");
						break;	
					case CMD_SERVERESET://��ģ����������ػ����������������mm
//						for(i=0;i<6;i++)
//						{
//							if(System.PassWord[i]!=GsmRec.Msg[GsmRec.MsgBegin][6+i])
//								break;
//						}
						i=6;
            GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//Ӧ��ָ��
						if(i==6) 
						{
							if(GsmRec.Msg[GsmRec.MsgBegin][5]==1)
							{//�ػ�
								printf("�ƿ��ƹػ�");
							}
							else if(GsmRec.Msg[GsmRec.MsgBegin][5]==2)
							{//����
								printf("�ƿ�������");
                Flag.McuReset=1;
							}
							else if(GsmRec.Msg[GsmRec.MsgBegin][5]==3)
							{//�������
								printf("�ƿ���������");
							}
						}
						else  
						{//�������
							GsmRec.Msg[GsmRec.MsgBegin][5]=0;
						}							
						break;				
					
					//Ӧ����
						case CMD_CLIENTGETSERVERSTA://��̨��ѯ��ģ��Ӧ��
						Flag.ServerStatus=GsmRec.Msg[GsmRec.MsgBegin][5];//��ģ��״̬
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));		
						break;
						
					case CMD_CLIENTREADID://��Ʒ����ȡģ��Ψһ������am
            printf("��Ψһ��Ӧ��");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));	
						break;
					case CMD_CLIENTREADUSER://��ȡ�Զ�����������Ӧ�� dm
						printf("���Զ������");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;							
					case CMD_CLIENTONSYNC://��̨�ϵ�ָ��Ӧ��gg
						printf("��̨�ϵ�Ӧ��");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;
	
					case CMD_CLIENTSTATUS://��̨�ϴ�״ָ̬��bb
						printf("��̨�ϴ�״̬");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;			
					case CMD_CLIENTCOINCNT://��̨�ϴ�Ͷ����ָ��cc
						printf("��̨�ϴ�����");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;		
					case CMD_CLIENTGIFTOUT://��̨�ϴ���Ʒ��ָ��ii
						printf("��̨�ϴ���Ʒ��");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;						
					case CMD_CLIENTERROR://��̨�ϴ����ϴ���ָ��hh
						printf("��̨�ϴ�����");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;	
						
					case CMD_CLIENTSETUSER://��Ʒ��д�볧���Զ����������jm
						printf("��̨д���Զ���");
						while(pdPASS != xQueueSend( xGsmAskQueue, &cmd, 10 ));
						break;							
					default:
						break;
				}
			}
			else
			{//Ψһ����ͬ
				GsmSendAckPacket(&GsmRec.Msg[GsmRec.MsgBegin][0]);//Ӧ��ָ��
			}
		}
		//!������ݰ�
		for(i=0;i<MAXMSGBUFFERLEN;i++)
		{
			GsmRec.Msg[GsmRec.MsgBegin][i]=0;
		}
		//!ѭ���������ݰ�
		if( ++GsmRec.MsgBegin >= MAXMSGSIZE)	GsmRec.MsgBegin=0;
	}
}

/******************************************************************************
 * ������:vU2ProcessTask
 * ˵��: 
 *    �ֻ�ͨ������ 
 * ����: 
 * ���: 
 * ����: 
 * ���:
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
 * ������:vU2SendTask
 * ˵��: 
 *    ����2��������
 * ����: 
 * ���: 
 * ����: 
 * ���:
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
			//!�������
			memset(&GsmSend.Msg[GsmSend.MsgBegin][0],0,MAXMSGBUFFERLEN);
			taskEXIT_CRITICAL();
			//!ѭ���������ݰ�
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
	ClientSendOn();//�����ϵ�ָ��
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


#include"includes.h"

INT8U RxBuffer2[RxBufferSize2];//����2������ʵ��
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

xSemaphoreHandle xUart3RXSemaphore;  /*����2�����ж�*/

// //!CRC16У��
// INT16U const crctab[16]=
// {
// 	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
// 	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
// };

// /******************************************************************************
//  * ������:Crc16
//  * ˵��: 
//  *    CRC16У�� 
//  * ����: 
//  * ���: 
//  * ����: 
//  * ���:
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

/*************    ��������USART_SendBuff     ***********************
�����ܡ������ڷ��ͺ��������ͻ����������ݳ���
����������
�����ء���
��˵������
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
�����ܡ����������ĳ���
����������
****
�����ء���****
��˵������****
-------------------------------*/
INT8U LenStr(INT8U *ip)
{
	unsigned int len=0;
	while(*ip++)	++len;
	return len;
}

/*********************************************
*��������:��pt1�в���pt2,������λ�ã���û�У��򷵻�0xffff
*˵����
	1�����ú�����INT8U LenStr(INT8U *ip) �����ַ�������
	2������˵����		
	3������˵����
		
*��ڣ�*pt1 ��������ʼ��ַ��len �����ĳ��ȣ�  *pt2���յ�����
*���ڣ��������λ�û����(0xffff)	 
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
		pt1++;		//Խ������ô��?
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





/*************    ��������Wait_Answer              ****************************
�����ܡ���
����������
�����ء���
��˵������
*************************************/
INT8U Wait_Answer(INT8U *str)
{
	if(Rx2Beg ==Rx2End )	return 0;		//û�а�
	if(0xffff != CompareStr(&RxBuffer2[Rx2Beg],Rx2End-Rx2Beg,str))	
	{
		Rx2Beg=Rx2End;
		return 1;
	}
	return 0;
}

/*********************************************
*��������:��*str�ַ����ҵ������ַ������� 
*˵����
	1�����ú�����
	2������˵����		
	3������˵����
*��ڣ�*str Դ  *Data �����ݵĵط���
			LenҪ���������ݳ���(ָ��0ʱ������������������)
*���ڣ������ɹ�" TRUE"	 
*********************************************/
INT8U CopyNum(INT8U *str,INT8U *Data,INT8U Len)
{
	INT8U ErrCnt=0;
	
	do{
	if( (*str>='0') && (*str<='9') )//�ҵ���ʼ����
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
	if(++ErrCnt>200) return 0;	//����
	}while(*(str++));
	return 0;
}

/*********************************************
*��������: �������ն��ŵĴ��ڻ�����
*˵����
	1�����ú�����
	2������˵����
	3������˵����
*��ڣ�
*���ڣ�	
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
 * ������:LPB100Init
 * ˵��: 
 *    LPB100��ʼ�� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void LPB100Init(void)
{
	INT32U i=0;
	
	//!Ӳ����λ
	LPB100RESETL;
	vTaskDelay(50);
	LPB100RESETH;

	//!�ȴ�ģ����������
	vTaskDelay(3000);

	//!��������ʱ�ж�,��ʱ��ʾģ�黵��
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
	(MyPacket.AdvanceSet.GameroomSNBack!=MyPacket.Children.GameroomSN))//��ͬ�ˣ���ʾ�˸ı���ip��ַ��Ҫ���ó�ʼ��
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
	//!�ȴ�ģ�����ӵ�AP,��ʱ��ʾAPû�д򿪣����߷�����û�д򿪣�����
	//!ģ�鱾�������������

#endif	
}

/******************************************************************************
 * ������:LPB100Set
 * ˵��: 
 *    LPB100���� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void LPB100Set(void)
{
	TcpFlag.RecMode=0;
		//����ǰҪ�����������ݰ����������淢 "+++"����ֲ���������
		USART_SendData(USART2,0);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		vTaskDelay(5000);
//		PrintfA11SMT("AT+NETP=TCP,CLIENT,%d,",1234);//��������ַ
//		DoWait("192.168.1.114\xd","+ok");
		vTaskDelay(1000);
	 
		taskENTER_CRITICAL(); 
		BuffWriteFM24C04((u8*)&SYSTEMSET,SystemInitAddr,sizeof(SYSTEMSET));
		taskEXIT_CRITICAL(); 
}

/******************************************************************************
 * ������:LPB100GetWSSID
 * ˵��: 
 *    ��ȡ�ź�ǿ�� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
INT8S LPB100GetWSSID(void)
{
	INT8U temp[12]={0};
	//!�л���AT������ģʽ
	DoWait("+++","a");	

	//!�ȴ�����������ģʽ
	DoWait("a","+ok");

	//!�رջ���
	DoWait("AT+E\xd","+ok");

	//!��ȡ�ź�ǿ��
	DoWait("AT+WSLQ\xd","+ok=");

	//!�õ�ʵ�ʵ���ֵ
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
 * ������:LPB100Scan
 * ˵��: 
 *    LPB100����״̬��� 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void LPB100Scan(void)
{
	INT8U cmd=0;
	
}

/******************************************************************************
 * ������:LPB100Reset
 * ˵��: 
 *    LPB100��λ 
 * ����: 
 * ���: 
 * ����: 
 * ���:
 *****************************************************************************/
void LPB100Reset(INT32U Reset)
{
	if(Reset)
		return;

	//!Ӳ����λ
//	LPB100RESETL;
//	vTaskDelay(50);
//	LPB100RESETH;
}

/*************    ������: SendUsartCom  ********************************
�����ܡ�:������ͨ�ţ��������ݺ���
��������:
�����ء�:
��˵����:
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








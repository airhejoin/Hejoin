/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                             ���Ѿ���ӹ�����
*
*ģ  �飺USBͨ��
*����ߣ�����������
*ʱ  �䣺2013-08-29 10:28:48
*********************************************************************************
*/
#include".\Lib\includes.h"

#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_pwr.h"

xSemaphoreHandle xUSBRecSemaphore;
xSemaphoreHandle xUSBSendSemaphore;
xSemaphoreHandle xPCASKSemaphore;
xQueueHandle  xQueueServerAsk;


INT8U USBTxMsgBeg;			//����һ������
INT8U USBTxMsgEnd;			//д��һ������
INT8U USBTxMsg[USBMsgSize][65];//���ݻ�����,����0��ʾ����

INT8U USBRxMsgBeg;			//����һ������
INT8U USBRxMsgEnd;			//д��һ������
INT8U USBRxMsg[RXUSBMsgSize][65];//���ݻ�����������0��ʾ����

/*************    ��������InitUSBIO                ****************************
�����ܡ�����ʼ��USBӲ���ӿڵ�·
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-17  16:34:53***********************/
void InitUSBIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);  

	/* Select USBCLK source */
  	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  	/* Enable the USB clock */
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

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


/*************    ������: WaitServerReady  **********************************
�����ܡ�:
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-09-19 15:20:3 **********************/
void WaitServerReadyUSB(void)
{

}

/*************    ������: AskServerImOK  ***********************************
�����ܡ�:
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-09-19 20:38:46 **********************/
void AskServerImOK(void)
{

}

/*************    ������: SendUSBNoSend  ***********************************
�����ܡ�:
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-10-23 12:42:4 **********************/
void SendUSBNoSend(void)
{

}
/*************    ������: SendIOStation  **********************************
�����ܡ�:����IO��״̬
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-10-05 8:0:9 **********************/
void SendIOStation(INT8U sta)
{

}

/*************	  ��������CheckPCMsg			   **************************
�����ܡ���
����������
�����ء���
��˵������
**************	 ��ƣ����������� 2012-11-30  10:13:14***********************/
INT8U CheckPCMsg( INT8U *ptr )
{
	INT8U len=*(ptr+5);
	INT16U crc16r=0;
	INT16U crc16t=0;

	crc16r=*(ptr+6+len)+(*(ptr+7+len)*256);
	crc16t=Crc16Check((ptr), (len+6));

	if(crc16r==crc16t)
		return 1;
	else
		return 0;
}

/*************    ������: SendPlayerBill  ***********************************
�����ܡ�:����ָ����ҵ���Ŀ��Ϣ
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-09-19 13:35:47 **********************/
void SendPlayerBill(INT8U player)
{

}

/*************    ������: SendSystemPara  ***********************************
�����ܡ�:����ϵͳ����
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-10-03 16:29:59 **********************/
void SendSystemPara(INT8U sel)
{

}

/*************    ������: SendServerTick  **********************************
�����ܡ�:
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-11-30 10:25:57 **********************/
void SendServerTick(void)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	INT32U SendCoin=0;

	if(SetCoinOut.SetOutValue>=SetCoinOut.OutValue)
		SendCoin=SetCoinOut.SetOutValue-SetCoinOut.OutValue;
	
	/*!�������*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID���ݳ���
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLTICK;		//!����
	USBTxMsg[USBTxMsgEnd][8]=0x06;					//!���ݰ�����

	//!��ǰ��������
	USBTxMsg[USBTxMsgEnd][9]=SendCoin;
	USBTxMsg[USBTxMsgEnd][10]=SendCoin>>8;
	USBTxMsg[USBTxMsgEnd][11]=SendCoin>>16;
	USBTxMsg[USBTxMsgEnd][12]=SendCoin>>24;
	
	//!������Ϣ
	USBTxMsg[USBTxMsgEnd][13]=SetCoinOut.Devstation;

	//!�������
	USBTxMsg[USBTxMsgEnd][14]=SetCoinOut.OutCoinReady;

	//!У��
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][15]=crccheck;
	USBTxMsg[USBTxMsgEnd][16]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);		 //!������β��
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][20]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][21]='K';
	USBTxMsg[USBTxMsgEnd][22]='J';
	
	//!ת��Ϊ����
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * ������:AskSetOutCoin
 * ˵��: 
 *    Ӧ������������� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 9:36:16, 2016��7��14��, ����������
 * -----------------------------------
 *****************************************************************************/
void AskSetOutCoin(INT8U result,INT32U SetValue)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!�������*/ 
	USBTxMsg[USBTxMsgEnd][0]= 64;					//!HID���ݳ���
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	
	USBTxMsg[USBTxMsgEnd][3]=0;		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=0;
	USBTxMsg[USBTxMsgEnd][5]=0;			
	USBTxMsg[USBTxMsgEnd][6]=0;	
	
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLSETCOINOUT;//!����
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!���ݰ�����

	//!ִ��״̬
	USBTxMsg[USBTxMsgEnd][9]=result;
	
	//!��ǰ��������
	USBTxMsg[USBTxMsgEnd][10]=SetValue;
	USBTxMsg[USBTxMsgEnd][11]=SetValue>>8;
	USBTxMsg[USBTxMsgEnd][12]=SetValue>>16;
	USBTxMsg[USBTxMsgEnd][13]=SetValue>>24;
	
	//!У��
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!������β��
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!ת��Ϊ����
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

//	for(i=0;i<length;i++)
//		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * ������:AskSetMacReverseRun
 * ˵��: 
 *    ������ﷴת 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 9:44:33, 2016��7��14��, ����������
 * -----------------------------------
 *****************************************************************************/
void AskSetMacReverseRun(INT8U Result)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!�������*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID���ݳ���
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLREVERSERUN;//!����
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!���ݰ�����

	//!ִ��״̬
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!��ǰ��������
	USBTxMsg[USBTxMsgEnd][10]=0;
	USBTxMsg[USBTxMsgEnd][11]=0;
	USBTxMsg[USBTxMsgEnd][12]=0;
	USBTxMsg[USBTxMsgEnd][13]=0;
	
	//!У��
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!������β��
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!ת��Ϊ����
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * ������:AskForceStop
 * ˵��: 
 *    Ӧ��ǿ��ֹͣ 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 9:48:20, 2016��7��14��, ����������
 * -----------------------------------
 *****************************************************************************/
void AskForceStop(INT8U Result,INT32U OutValue)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!�������*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID���ݳ���
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLSTOP;//!����
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!���ݰ�����

	//!ִ��״̬
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!��ǰ��������
	USBTxMsg[USBTxMsgEnd][10]=OutValue;
	USBTxMsg[USBTxMsgEnd][11]=OutValue>>8;
	USBTxMsg[USBTxMsgEnd][12]=OutValue>>16;
	USBTxMsg[USBTxMsgEnd][13]=OutValue>>24;
	
	//!У��
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!������β��
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!ת��Ϊ����
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * ������:AskContinueRun
 * ˵��: 
 *    ��ת 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 9:56:16, 2016��7��14��, ����������
 * -----------------------------------
 *****************************************************************************/
void AskContinueRun(INT8U Result,INT32U OutValue)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!�������*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID���ݳ���
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLCONTINUE;//!����
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!���ݰ�����

	//!ִ��״̬
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!��ǰ��������
	USBTxMsg[USBTxMsgEnd][10]=OutValue;
	USBTxMsg[USBTxMsgEnd][11]=OutValue>>8;
	USBTxMsg[USBTxMsgEnd][12]=OutValue>>16;
	USBTxMsg[USBTxMsgEnd][13]=OutValue>>24;
	
	//!У��
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!������β��
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!ת��Ϊ����
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * ������:AskCleanRun
 * ˵��: 
 *    ��� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 10:59:15, 2016��7��14��, ����������
 * -----------------------------------
 *****************************************************************************/
void AskCleanRun(INT8U Result)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	
	/*!�������*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID���ݳ���
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINCLEAN;//!����
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!���ݰ�����

	//!ִ��״̬
	USBTxMsg[USBTxMsgEnd][9]=Result;
	
	//!��ǰ��������
	USBTxMsg[USBTxMsgEnd][10]=0;
	USBTxMsg[USBTxMsgEnd][11]=0;
	USBTxMsg[USBTxMsgEnd][12]=0;
	USBTxMsg[USBTxMsgEnd][13]=0;
	
	//!У��
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!������β��
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!ת��Ϊ����
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}

/******************************************************************************
 * ������:SendServerOutCoinReady
 * ˵��: 
 *    Ӧ�������������� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 15:28:35, 2016��7��16��, ����������
 * -----------------------------------
 *****************************************************************************/
void SendServerOutCoinReady(void)
{
	INT16U crccheck=0;
	INT8U  random=0;
	INT8U  length=0;
	INT8U  i=0;
	INT32U SendCoin=0;
	
	/*!�������*/ 
	USBTxMsg[USBTxMsgEnd][0]=64;					//!HID���ݳ���
	USBTxMsg[USBTxMsgEnd][1]='H';
	USBTxMsg[USBTxMsgEnd][2]='J';
	USBTxMsg[USBTxMsgEnd][3]=IRandomX(0,255);		//!������ͷ
	USBTxMsg[USBTxMsgEnd][4]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][5]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][6]=IRandomX(0,255);		
	USBTxMsg[USBTxMsgEnd][7]=USB_COINSELLOUTREADY;		//!����
	USBTxMsg[USBTxMsgEnd][8]=0x05;					//!���ݰ�����

	//!��ǰ��������
	USBTxMsg[USBTxMsgEnd][9]=SendCoin;
	USBTxMsg[USBTxMsgEnd][10]=SendCoin>>8;
	USBTxMsg[USBTxMsgEnd][11]=SendCoin>>16;
	USBTxMsg[USBTxMsgEnd][12]=SendCoin>>24;
	
	//!������Ϣ
	USBTxMsg[USBTxMsgEnd][13]=SetCoinOut.Devstation;

	//!У��
	crccheck=Crc16Check(&USBTxMsg[USBTxMsgEnd][3], (USBTxMsg[USBTxMsgEnd][8]+6));
	USBTxMsg[USBTxMsgEnd][14]=crccheck;
	USBTxMsg[USBTxMsgEnd][15]=crccheck>>8;

	USBTxMsg[USBTxMsgEnd][16]=IRandomX(0,255);		 //!������β��
	USBTxMsg[USBTxMsgEnd][17]=IRandomX(0,255);
	USBTxMsg[USBTxMsgEnd][18]=IRandomX(0,255);			
	USBTxMsg[USBTxMsgEnd][19]=IRandomX(0,255);	
	USBTxMsg[USBTxMsgEnd][20]='K';
	USBTxMsg[USBTxMsgEnd][21]='J';
	
	//!ת��Ϊ����
	length=USBTxMsg[USBTxMsgEnd][8]+4;
	random=USBTxMsg[USBTxMsgEnd][3];

	for(i=0;i<length;i++)
		USBTxMsg[USBTxMsgEnd][7+i]^=random;
		
	if( ++USBTxMsgEnd >= USBMsgSize)	USBTxMsgEnd=0;
	xSemaphoreGive( xUSBSendSemaphore );
}
/*************    ������: ResolvePCMsg  ************************************
�����ܡ�:
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-09-09 22:6:23 **********************/
void ResolvePCMsg(void)
{
	INT8U i=0;
	INT8U check=0;
	INT8U cmd=0;
	INT8U *ptr=NULL;
	INT8U length=0;
	INT8U random = 0;

	while(USBRxMsgBeg!=USBRxMsgEnd)
	{
		//!���ｫ���ݲ��ֵ�����ת��Ϊ����
		length = (USBRxMsg[USBRxMsgBeg][8]^USBRxMsg[USBRxMsgBeg][3])+4;
		random = USBRxMsg[USBRxMsgBeg][3];

		for(i=0;i<length;i++)
		{
			USBRxMsg[USBRxMsgBeg][7+i]^=random;
		}
		
		//�ж����ݰ�У���Ƿ�����
		check=CheckPCMsg(&USBRxMsg[USBRxMsgBeg][3]);

		if(check)
		{
			cmd=(USBRxMsg[USBRxMsgBeg][7]&0x7f);
			
			switch(cmd)
			{
				//!�����˱�����
				case USB_COINSELLSETCOINOUT:
					if((SetCoinOut.OutValue==0)&&(0==SetCoinOut.RunSta))
					{
						ptr=(INT8U *)&SetCoinOut.SetOutValue;//!���ó�������
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][9+i];

						ptr=(INT8U *)&SetCoinOut.OutValue;//ʵ�ʳ�������
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][13+i];

						//!�����˱�
						SetCoinOut.EnableOut=1;
						SetCoinOut.OutTimeOut=0;
						SetCoinOut.ForwardOrReverse=Motor_Forward;//��ת
						SetCoinOut.ChangeSetCnt=0;
						SetCoinOut.ChangeCnt=0;
						SetCoinOut.Devstation=0;
						SetCoinOut.StartOutCoin=2;//����
						SetCoinOut.OutCoinReady=0;

						//!Ӧ������
						AskSetOutCoin(0,SetCoinOut.SetOutValue);
					}
					else//!Ӧ������
					{
						AskSetOutCoin(1,0);
					}
					break;

				//!�˱���ﷴת
				case USB_COINSELLREVERSERUN:
					if(0==SetCoinOut.RunSta)
					{
						SetCoinOut.SetReverseRun=1;
						SetCoinOut.OutTimeOut=0;
						SetCoinOut.Devstation=0;

						AskSetMacReverseRun(0);
					}
					else
						AskSetMacReverseRun(1);
					break;

				//!�˱�ǿ��ֹͣ 
				case USB_COINSELLSTOP:
					if(1==SetCoinOut.RunSta)
					{
						SetCoinOut.EnableOut=0;
						MotoTurnReverseStop();
						MotoTurnForwardStop();

						if(SetCoinOut.Clean==0)
						{
							//!Ӧ��ǰ��������
							AskForceStop(0,SetCoinOut.OutValue);
						}

						SetCoinOut.SetOutValue=0;
						SetCoinOut.OutValue=0;
						SetCoinOut.Clean=0;
					}
					else//!Ӧ��ǰ��������
					{
						AskForceStop(1,0);
					}
					break;

				//!�˱������ת,Ϊ�˷�ֹ�Ƿ��ϵ磬�а�̨������������
				case USB_COINSELLCONTINUE:
					if(0==SetCoinOut.RunSta)
					{//�����ֹͣ״̬
						ptr=(INT8U *)&SetCoinOut.SetOutValue;
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][9+i];

						ptr=(INT8U *)&SetCoinOut.OutValue;
						for(i=0;i<4;i++)
							*(ptr+i)=USBRxMsg[USBRxMsgBeg][13+i];//

						
						SetCoinOut.EnableOut=1;//!�����˱�
						SetCoinOut.OutTimeOut=0;
						SetCoinOut.ForwardOrReverse=Motor_Forward;//��ת
						SetCoinOut.ChangeSetCnt=0;
						SetCoinOut.ChangeCnt=0;
						SetCoinOut.Devstation=0;

						AskContinueRun(0,SetCoinOut.SetOutValue);
					}
					else
						AskContinueRun(1,0);
					break;

				//!�˱�������
				case USB_COINCLEAN:
					if(0==SetCoinOut.RunSta)
					{//�����ֹͣ״̬
						SetCoinOut.SetOutValue=60000;//���ó�������
						SetCoinOut.OutValue=60000;//
						//!�����˱�
						SetCoinOut.EnableOut=1;//�˱�ʹ��
						SetCoinOut.OutTimeOut=0;//
						SetCoinOut.ForwardOrReverse=Motor_Forward;//�����ת
						SetCoinOut.ChangeSetCnt=0;
						SetCoinOut.ChangeCnt=0;
						SetCoinOut.Devstation=0;
						SetCoinOut.Clean=1;

						AskCleanRun(0);
					}
					else
						AskCleanRun(1);
					break;
					
				default:
					break;
			}
		}

		//!��ջ�����
		for(i=0;i<65;i++)
			USBRxMsg[USBRxMsgBeg][i]=0;
		
		if( ++USBRxMsgBeg >= RXUSBMsgSize)	USBRxMsgBeg=0;
	}
}

/*************    ��������vConnectPCTask            *************************
�����ܡ���USB��������
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-17  16:53:55***********************/
xTaskHandle xHandleGameDemo;
void vConnectPCTask( void *pvParameters )
{
	INT8U usbsendok=0;
	INT8U i=0;
	INT32U sendtick=0;

	taskENTER_CRITICAL(); /*����������*/
	USB_Init();
	taskEXIT_CRITICAL(); /*����������*/
	
	vTaskDelay(5000);

	xSemaphoreTake( xUSBSendSemaphore, ( portTickType ) 0);

	for(;;)
	{
		xSemaphoreTake( xUSBSendSemaphore, ( portTickType ) 10);

		//!��������
		if(++sendtick>=5)
		{
			sendtick=0;
			SendServerTick();
		}
		
		while(USBTxMsgBeg!=USBTxMsgEnd)
		{//��������Ҫ����
			do
			{
				vTaskDelay(10);
				usbsendok=GetEPTxStatus(ENDP1);//!��ѯ�Ƿ������
			}while(EP_TX_NAK!=usbsendok);
			
			taskENTER_CRITICAL(); /*����������*/
			UserToPMABufferCopy(&USBTxMsg[USBTxMsgBeg][1], GetEPTxAddr(ENDP1), USBTxMsg[USBTxMsgBeg][0]);
			SetEPTxCount(ENDP1, USBTxMsg[USBTxMsgBeg][0]);                    
			SetEPTxValid(ENDP1);
			taskEXIT_CRITICAL(); /*����������*/
			
			/*!�����������*/
			for(i=0;i<65;i++)
				USBTxMsg[USBTxMsgBeg][i]=0x00;
			
			if( ++USBTxMsgBeg >= USBMsgSize)	USBTxMsgBeg=0;
		}
	}
}

/*************    ��������vRecMsgPCTask            **************************
�����ܡ���USB��������
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-30  09:44:56***********************/
void vRecMsgPCTask( void *pvParameters )
{
	INT8U i=0;
	
	for(;;)
	{
		//!�ȴ������ź���
		xSemaphoreTake( xUSBRecSemaphore, ( portTickType ) portMAX_DELAY); 
		taskENTER_CRITICAL(); /*����������*/
		ResolvePCMsg();
		taskEXIT_CRITICAL(); /*����������*/
	}
}


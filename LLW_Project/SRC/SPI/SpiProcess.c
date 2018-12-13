/*-------------------------------------------------------------------------
    
-------------------------------------------------------------------------*/
/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                             ���Ѿ���ӹ�����
*
*ģ  �飺SPI����165��595
*����ߣ�����������
*ʱ  �䣺2014-09-17 21:28:48
*********************************************************************************
*/
#include"includes.h"

INT8U Get165BuffISR[MAXNUM595]={0};
INT8U Get165BuffNew[MAXNUM595]={0};
INT8U Get165BuffOld[MAXNUM595]={0};

//!����ܱ���
INT8U LedTab[38]={
	LedChar_0,LedChar_1,LedChar_2,LedChar_3,LedChar_4,LedChar_5,LedChar_6,
	LedChar_7,LedChar_8,LedChar_9,LedChar_OFF,
	LedChar_A,
	LedChar_B,
	LedChar_C,
	LedChar_D,
	LedChar_E,
	LedChar_F,
	LedChar_G,
	LedChar_H,
	LedChar_I,
	LedChar_J,
	LedChar_K,
	LedChar_L,
	LedChar_M,
	LedChar_N,
	LedChar_O,
	LedChar_P,
	LedChar_Q,
	LedChar_R,
	LedChar_S,
	LedChar_T,
	LedChar_U,
	LedChar_V,
	LedChar_W,
	LedChar_X,
	LedChar_Y,
	LedChar_Z,
	LedChar__,
};

//!��������ܱ�ţ�ת������Ӧ�Ķ�ά������
#define SETMAP(A) ((A)/16*MAXNUM595+(A)%16)
INT8U mapbcd[8]=
{
	SETMAP(0x00),SETMAP(0x10),SETMAP(0x20),SETMAP(0x30),//!�ֽ�
	SETMAP(0x02),SETMAP(0x12),SETMAP(0x22),SETMAP(0x32),//!Ӧ����
};

//!8������ܸ��»�����
INT8U DisBuff[MAXCOM][MAXNUM595]={0};

//!�����ʾ8������
INT8U BCD[8]={0};

//!��Ҫ���µ�COM��
INT8U gSelCom=0;

/******************************************************************************
 * ������:DisMapBCD
 * ˵��: 
 *    ������ʾ�������Ķ�ά���� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 23:34:13, 2014��9��17��, ����������
 * -----------------------------------
 *****************************************************************************/
void DisMapBCD(INT8U led_id, INT8U num)			//��ʾ�ӳ���
{
	led_id=mapbcd[led_id];
	DisBuff[0][led_id]=LedTab[num];
}

/******************************************************************************
 * ������:DisNumberCoor
 * ˵��: 
 *    ��ָ��λ����ʾ���� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 23:0:17, 2014��9��17��, ����������
 * -----------------------------------
 *****************************************************************************/
void DisNumberCoor(INT8U zero, INT8U StartLed, INT8U Width, INT32S Num)//zero=0,ǰ���0��zero=1��ǰ������
{
	INT32U i=0;
	INT32U j=0;
	
	if(StartLed>LEDTOTAL)
		return;

	BCD[7]=BCD[6]=BCD[5]=BCD[4]=BCD[3]=BCD[2]=BCD[1]=0;

	while(Num>=10000000)	{Num-=100000000;	        	}
	while(Num>=10000000)	{Num-=10000000;		BCD[7]++; }
	while(Num>=1000000)		{Num-=1000000;		BCD[6]++;	}
  while(Num>=100000)		{Num-=100000;			BCD[5]++;	}
  while(Num>=10000)			{Num-=10000;			BCD[4]++;	}
  while(Num>=1000)			{Num-=1000;				BCD[3]++;	}
  while(Num>=100)				{Num-=100;				BCD[2]++;	}
  while(Num>=10)				{Num-=10;					BCD[1]++;	}
  BCD[0]=Num;

	//!���ǰ��0
	if(zero)
	{
		for(i=0;i<7;i++)
		{
			if(BCD[7-i])
				break;
		}

		for(j=0;j<i;j++)
			BCD[7-j]=10;
	}
	
	i=0;
	do
	{
		Width--;
		DisMapBCD(StartLed+Width, BCD[i]);
		if(i==sizeof(BCD)-1)	break;
		i++;
	}while(Width);
}

/******************************************************************************
 * ������:DigitalSPIInit
 * ˵��: 
 *    �������ʾSPI��ʼ�� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 11:41:50, 2016��7��12��, ����������
 * -----------------------------------
 *****************************************************************************/
void DigitalSPIInit(void)
{
   SPI_InitTypeDef SPI_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO ,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

   /* SCK, MIS and MOSI  A5=CLK,A6=MISO,A7=MOSI*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*  PA.4 ��Ƭѡ*/
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//Ԥ��Ϊ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI1 configuration  */
	SPI_Cmd(SPI2, DISABLE); 												//�����Ƚ���,���ܸı�MODE
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//��
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=0 ʱ�����յ�
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=0 ���ݲ����1��
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//���NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//4��Ƶ	,����̫��
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//��λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7
    
	SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE); 
}

/******************************************************************************
 * ������:SPI2_ReadWriteByte
 * ˵��: 
 *    SPI2��д���� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 11:43:25, 2016��7��12��, ����������
 * -----------------------------------
 *****************************************************************************/
INT8U SPI2_ReadWriteByte(INT8U  TxData)
{
  	/* Loop while DR register in not emplty */
  	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  	/* Send byte through the SPI1 peripheral */
  	SPI_I2S_SendData(SPI2, TxData);

  	/* Wait to receive a byte */
  	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  	/* Return the byte read from the SPI bus */
  	return SPI_I2S_ReceiveData(SPI2);
}

/******************************************************************************
 * ������:SendBuffAndGetKeyBuff
 * ˵��: 
 *    SPI�ͳ���ʾ���������һ�ȡ���� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 21:50:47, 2014��9��17��, ����������
 * -----------------------------------
 *****************************************************************************/
void SendBuffAndGetKeyBuff( INT8U *SendBuff, INT8U *KeyBuff)
{
	INT32U i=0;
	INT8U data=0;

	//!����165��ֵ
	#if 0
	SPI2SSL;
	__NOP();
	__NOP();
	SPI2SSH;
	__NOP();
	__NOP();
	#endif

	for(i=0;i<MAXNUM595;i++)
	{
		data=SPI2_ReadWriteByte(*SendBuff++);
		*KeyBuff++=(~data);
	}

	//!���µ�595��ʾ������
	SPI2SSL;
	__NOP();
	__NOP();
	SPI2SSH;
}

/******************************************************************************
 * ������:UpdatekeyAndDis
 * ˵��: 
 *    ˢ����ʾ�������͸��¼�ֵ 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 16:16:37, 2014��9��22��, ����������
 * -----------------------------------
 *****************************************************************************/
void UpdateKeyAndDis(void)
{
	if(++gSelCom>=4)
	{
		gSelCom=0;
	}
	
	DisBuff[gSelCom][1]=1<<gSelCom;
	DisBuff[gSelCom][3]=1<<gSelCom;

	SendBuffAndGetKeyBuff(&DisBuff[gSelCom][0],Get165BuffISR);
}

/******************************************************************************
 * ������:CloseLedCoor
 * ˵��: 
 *    �ر�ָ��λ���������ʾ 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 12:8:48, 2016��7��13��, ����������
 * -----------------------------------
 *****************************************************************************/
void CloseLedCoor(INT8U StartLed, INT8U Width)
{
	do
	{
		Width--;
		DisMapBCD(StartLed+Width, 10);
	}while(Width);
}

/******************************************************************************
 * ������:DisHardWrong
 * ˵��: 
 *    ��ʾ������Ϣ 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 12:15:36, 2016��7��13��, ����������
 * -----------------------------------
 *****************************************************************************/
void DisHardWrong(INT8U WrongList)
{
	INT32U i=0;

	for(i=0;i<8;i++)
	{
		if(WrongList&1<<i)
			break;
	}

	if(i<8)
	{
		DisMapBCD(0, 15);
		DisMapBCD(1, 37);
		DisMapBCD(2, 37);
		DisNumberCoor(0,3,1,i);
	}
}

/******************************************************************************
 * ������:DisCleanCoin
 * ˵��: 
 *    ��ʾ��� 
 * ����: 
 * ���: 
 * ����: 
 * -----------------------------------
 * ���:
 * 11:22:46, 2016��7��14��, ����������
 * -----------------------------------
 *****************************************************************************/
void DisCleanCoin(void)
{
	DisMapBCD(0, 13);
	DisMapBCD(1, 22);
	DisMapBCD(2, 15);
	DisMapBCD(3, 28);
}


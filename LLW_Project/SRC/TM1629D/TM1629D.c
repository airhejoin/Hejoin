/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺TM1629D��ʼ������
*����ߣ�����������
*ʱ  �䣺2012-05-24  01:03:27
*********************************************************************************
*/
#include"..\Lib\includes.h"

/*************    ��������InitTM1629DIO             ****************************
�����ܡ�����ʼ����Ƭ����TM1629���ƿ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-25  22:59:48***********************/
void InitTM1629DIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = TM1629DIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TM1629CLK|TM1629DOUT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, TM1629CLK);
	GPIO_SetBits(GPIOC, TM1629DOUT);
	GPIO_SetBits(GPIOE, TM1629DIN);
}
/*************    ��������SendByteTM1629D         ****************************
�����ܡ�����TM1629����һ���ֽ�
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-25  23:12:30***********************/
void SendByteTM1629D(INT8U data)
{
	INT32U i=0;

	for(i=0;i<8;i++)
	{
		TM1629DCLKL;
		__NOP();
		__NOP();

		if(data&0x01)
			TM1629DOUTH;
		else
			TM1629DOUTL;

		data>>=1;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		TM1629DCLKH;
		__NOP();
		__NOP();
	}
}
/*************    ��������ReadDataTM1629D           **************************
�����ܡ�����TM1629��������ȡһ���ֽ�
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-26  20:49:46***********************/
INT8U ReadDataTM1629D(void)
{
	INT32U i=0;
	INT8U data=0;

	for(i=0;i<8;i++)
	{
		TM1629DCLKL;
		data>>=1;
		
		__NOP();
		__NOP();

		TM1629DCLKH;
		
		__NOP();
		__NOP();
		
		if(GPIO_ReadInputDataBit(GPIOE, TM1629DIN))
			data|=0x80;
		else
			data&=0x7f;
		
		__NOP();
	}

	return data;
}

/*************    ��������InitTM1629D             ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-25  23:37:06***********************/
void InitTM1629D(void)
{
	InitTM1629DIO();
	SendByteTM1629D(WRITEBUFFER);
	SendByteTM1629D(SETPLUSE14);
	SendByteTM1629D(DISON );
}

/*************    ��������ScanKey                 ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-13  13:29:14***********************/
INT32U ScanKey(void)
{
 	INT8U i=0;
	INT32U key=0;
	INT16U sel=0;
	
	SET_DATASEL(sel);
	sel=0x07;
	SET_DATASEL(sel);
	SendByteTM1629D(READYKEY);
		
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	for(i=0;i<4;i++)
	{
		key<<=8;
		key+=ReadDataTM1629D();
		
	}

	sel=0;
	SET_DATASEL(sel);
	return key;
}
































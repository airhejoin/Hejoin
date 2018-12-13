/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                              ���Ѿ���ӹ�����
*
*ģ  �飺FM16W08
*˵  ��:
*        Ϊ����Ŀ�Ĺ�����Ŀ��ȫ�ֱ��������������������Ŀ�ĺ���ά�����Ͷ���
*        Э��
*����ߣ�����������
*ʱ  �䣺2012-11-18  10:23:49
*********************************************************************************
*/
#include".\Lib\includes.h"
/*************    ��������InitFM16W08Hard         ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-18  10:46:31***********************/
void InitFM16W08Hard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//!�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //�������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//!����Ԥ��
	FM16W08CEH;
	FM16W08OEH;
	FM16W08WEH;
}
/*************    ��������ReadDataFM16W08          ***************************
�����ܡ���
����������
�����ء���
��˵������72M,һ��ָ��Ϊ13NS��
**************   ��ƣ����������� 2012-11-18  10:53:14***********************/
INT8U ReadDataFM16W08(INT16U addr,INT8U check)
{
	INT16U sendaddr=addr;
	INT8U data=0;

	OUTADDRFM16W08(sendaddr);//!�͵�ַ
	__NOP();
	FM16W08CEL;  //!Ƭѡ
	__NOP();
	__NOP();
	FM16W08OEL; //!��ʼ��ȡ����
	__NOP();
	__NOP();
	data=GPIO_ReadInputData(GPIOE)&0xff;
	FM16W08CEH;
	__NOP();
	FM16W08OEH;
	__NOP();
	__NOP();
	
	return (data^check);
}
/*************    ��������BuffReadFM16W08          ***************************
�����ܡ���
����������
�����ء���
��˵������
        ������㣬����Ҫ�������ƫ��,����ֻ��������ʱ���һ�Σ�����ʱ����
        д
**************   ��ƣ����������� 2012-11-18  10:55:14***********************/
void BuffReadFM16W08(INT16U addr, INT16U len, INT8U *ch,INT8U check)
{
	INT16U address=addr;
	
	taskENTER_CRITICAL(); /*����������*/
	while(len)
	{
		*ch = ReadDataFM16W08(address,check);
		ch++;
		len--;
		address++;
	}
	taskEXIT_CRITICAL(); /*����������*/
}
/*************    ��������WriteDataFM16W08          **************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-18  23:01:30***********************/
void WriteDataFM16W08(INT16U addr, INT8U data,INT8U check)
{
	INT16U senddata=(data^check);
	INT16U sendaddr=addr;
	GPIO_InitTypeDef GPIO_InitStructure;

	//!�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	FM16W08WEL; //!��ʼд����
	__NOP();
	FM16W08CEL;  //!Ƭѡ
	__NOP();
	__NOP();
	OUTADDRFM16W08(sendaddr);//!�͵�ַ
	__NOP();
	OUTDATAFM16W08(senddata);
	__NOP();
	__NOP();
	FM16W08CEH;
	__NOP();
	FM16W08WEH;
	__NOP();
	__NOP();

	

	//!�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //�������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//__NOP();
}

/*************    ��������BuffWriteFM16W08          **************************
�����ܡ���
����������
�����ء���
��˵������
		������㣬����Ҫ�������ƫ��,����ֻ��������ʱ���һ�Σ�����ʱ����
        д,д��ʱ����Ҫ��ʱ���ݳ��Ⱥ���ʼ��ַ����ƫ��
**************   ��ƣ����������� 2012-11-18  23:01:30***********************/
void BuffWriteFM16W08(INT8U *pbuff, INT32U addr, INT32U len, INT8U check)
{
	INT16U senddata=0;
	INT16U sendaddr=addr;
	GPIO_InitTypeDef GPIO_InitStructure;

	//!�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	while(len)
	{	
		OUTADDRFM16W08(sendaddr);//!�͵�ַ
		FM16W08WEL; //!��ʼд����
		__NOP();
		FM16W08CEL;  //!Ƭѡ
		__NOP();
		__NOP();
		senddata=*pbuff;
		senddata^=check;
		OUTDATAFM16W08(senddata);
		__NOP();
		__NOP();
		FM16W08CEH;
		__NOP();
		FM16W08WEH;
		__NOP();
		__NOP();
		
		pbuff++;
		len--;
		sendaddr++;
	}

	//!�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //�������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}








































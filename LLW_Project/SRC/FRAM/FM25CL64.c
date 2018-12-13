/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺FM25CL64��ʼ������
*����ߣ�����������
*ʱ  �䣺2012-11-15  10:41:05
*********************************************************************************
*/
#include".\Lib\includes.h"
/*************    ��������InitFM25CL64Hard        ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-15  10:42:24***********************/
void InitFM25CL64Hard(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
						   RCC_APB2Periph_AFIO ,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	/* SCK, MIS and MOSI  A5=CLK,A6=MISO,A7=MOSI*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*  PA.4 ��Ƭѡ*/
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//Ԥ��Ϊ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI1 configuration  */
	SPI_Cmd(SPI1, DISABLE); 												//�����Ƚ���,���ܸı�MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=0 ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=0 ���ݲ����1��
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//���NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	    //4��Ƶ	,����̫��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE); 
}

/*************    ��������SPI1_ReadWriteByte       ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-15  10:55:18***********************/
INT8U SPI1_ReadWriteByte(INT8U  TxData)
{
  	/* Loop while DR register in not emplty */
  	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  	/* Send byte through the SPI1 peripheral */
  	SPI_I2S_SendData(SPI1, TxData);

  	/* Wait to receive a byte */
  	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  	/* Return the byte read from the SPI bus */
  	return SPI_I2S_ReceiveData(SPI1);
}
/*************    ��������ReadDataFM25CL64          *************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-15  11:50:33***********************/
INT8U ReadDataFM25CL64(INT16U addr)
{
	INT8U data=0;

	if(addr>0x1fff)
		return 0;
	
	FM25CL64_CS_L;

	//!������
	SPI1_ReadWriteByte(READ_FM25CL);

	//!��ַ
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!��ȡ����
	data=SPI1_ReadWriteByte(0xff);
	
	FM25CL64_CL_H;
	
	return data;
}

/*************    ��������WriteDataFM25CL64       ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-15  11:53:23***********************/
void WriteDataFM25CL64(INT16U addr ,INT8U data)
{
	if(addr>0x1fff)
		return ;
	
	FM25CL64_CS_L;
	//!ʹ��д
	SPI1_ReadWriteByte(WREN_FM25CL);

	FM25CL64_CL_H;

	FM25CL64_CS_L;
	//!д����
	SPI1_ReadWriteByte(WRITE_FM25CL);

	//!��ַ
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!����
	SPI1_ReadWriteByte(data);

	FM25CL64_CL_H;
}

/*************    ��������BuffWriteFM25CL64       ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-15  11:12:55***********************/
void BuffWriteFM25CL64(INT8U *pbuff, INT16U addr, INT16U len)
{
	if(addr>0x1fff)
		return ;
	
	FM25CL64_CS_L;
	//!ʹ��д
	SPI1_ReadWriteByte(WREN_FM25CL);

	FM25CL64_CL_H;

	FM25CL64_CS_L;
	//!д����
	SPI1_ReadWriteByte(WRITE_FM25CL);

	//!��ַ
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!����
	while(len)
	{
		SPI1_ReadWriteByte(*pbuff);
		pbuff++;
		len--;
	}

	FM25CL64_CL_H;
}

/*************    ��������BuffReadFM25CL64        ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-15  13:23:48***********************/
void BuffReadFM25CL64(INT16U addr, INT8U *pbuff,  INT16U len)
{
	if(addr>0x1fff)
		return ;
	
	FM25CL64_CS_L;

	//!������
	SPI1_ReadWriteByte(READ_FM25CL);

	//!��ַ
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!��ȡ����
	while(len)
	{
		*pbuff=SPI1_ReadWriteByte(0xff);
		len--;
		pbuff++;
	}
	
	FM25CL64_CL_H;
}

/*************    ��������ClearFM25CL64            ***************************
�����ܡ����������
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-15  15:27:22***********************/
void ClearFM25CL64(void)
{
	INT16U i=0;
	
	FM25CL64_CS_L;
	//!ʹ��д
	SPI1_ReadWriteByte(WREN_FM25CL);

	FM25CL64_CL_H;

	FM25CL64_CS_L;
	//!д����
	SPI1_ReadWriteByte(WRITE_FM25CL);

	//!��ַ
	SPI1_ReadWriteByte(0);
	SPI1_ReadWriteByte(0);

	//!����
	for(i=0;i<8192;i++)
	{
		SPI1_ReadWriteByte(0);
	}

	FM25CL64_CL_H;
}




































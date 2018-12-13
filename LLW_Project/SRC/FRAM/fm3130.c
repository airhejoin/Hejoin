/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺IIC��ʼ������
*����ߣ�����������
*ʱ  �䣺2012-05-24  01:03:27
*********************************************************************************
*/
#include "..\STM32Lib\stm32f10x.h"
#include "..\hal.h"
#include <stdio.h>

#define EEPROM_ADDR		0xA0
#define CLOCK_ADDR		0xD0
#define KEY_ADDR        0X1c20    //7K�ռ����������  7200
/*********************��������:WriteFM3130      **************************
�����ܡ���дFRAM 1���ֽ�
����������
�����ء���
��˵������
**********************����: ľ��   2010��01��13��15:23:52 ***************/
void WriteFM3130(INT16U addr,INT8U data)
{
	u8 address=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x2000)
		return ;
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
	/*���͸�λ��ַ*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	/*���͵�λ��ַ*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	/* дһ���ֽ�*/
  	I2C_SendData(I2C2, data); 
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* ֹͣλ*/
  	I2C_GenerateSTOP(I2C2, ENABLE);
}
/*************    ��������BuffWriteFM3130          ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-12  10:39:54***********************/
void BuffWriteFM3130(INT8U *pbuff, INT32U addr, INT32U len)
{
	u8 address=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x2000)
		return ;
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
	/*���͸�λ��ַ*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	/*���͵�λ��ַ*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	while(len)
	{
		/* дһ���ֽ�*/
	  	/* дһ���ֽ�*/
  		I2C_SendData(I2C2, *pbuff); 
  		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		pbuff++;
		len--;
	}

	/* ֹͣλ*/
	I2C_GenerateSTOP(I2C2, ENABLE);
}
/*************    ��������ReadFM3130              ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-12  10:34:26***********************/
INT8U ReadFM3130(INT16U addr)
{
	u8 address=0;
	INT8U data=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x2000)
		return 0;

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͸�λ��ַ*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	/*���͵�λ��ַ*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C2, EEPROM_ADDR+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C2, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    I2C_GenerateSTOP(I2C2, ENABLE);			//����ֹͣλ

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C2);

   //�ٴ�����Ӧ��ģʽ
	I2C_AcknowledgeConfig(I2C2, ENABLE);  

	return data;
}
/*********************��������:BuffReadFM3130    **************************
�����ܡ�����FRAM ����ֽ�
����������
�����ء���
��˵������
**********************����: ľ��   2010��01��13��15:24:37 ***************/
void BuffReadFM3130(INT16U addr, INT8U len, INT8U *ch)
{
   	u8 address=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x2000)
		return ;
taskENTER_CRITICAL(); /*����������*/
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͸�λ��ַ*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	/*���͵�λ��ַ*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C2, EEPROM_ADDR+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(len)
	{
		if(1==len)
		{
			I2C_AcknowledgeConfig(I2C2, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    		I2C_GenerateSTOP(I2C2, ENABLE);			//����ֹͣλ
		}

		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
		*ch = I2C_ReceiveData(I2C2);
		ch++;
		len--;
	}
   //�ٴ�����Ӧ��ģʽ
	I2C_AcknowledgeConfig(I2C2, ENABLE); 
  taskEXIT_CRITICAL(); /*����������*/
}

/*********************��������:iic_clear_fram    ************************
�����ܡ���˳��дFRAM
����������
�����ء���
��˵������
**********************����: ľ��   2010��01��13��15:25:32 ***************/
void ClearFM3130(u16 addr, u16 leng)
{
    u16 i=0;
    
    for(i=0;i<leng;i++)
    {
        WriteFM3130(addr+i, 0);
    }
}
/*************    ��������WriteFMRTC              ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-12  11:22:57***********************/
void WriteFMRTC(INT8U addr,INT8U data)
{
	u8 address=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x0E)
		return ;
taskENTER_CRITICAL(); /*����������*/
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  CLOCK_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	/* дһ���ֽ�*/
  	I2C_SendData(I2C2, data); 
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* ֹͣλ*/
  	I2C_GenerateSTOP(I2C2, ENABLE);
taskEXIT_CRITICAL(); /*����������*/
}
/*************    ��������ReadFMRTC               ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-12  11:24:59***********************/
INT8U ReadFMRTC(INT8U addr)
{
	u8 address=0;
	INT8U data=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x0E)
		return 0;

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  CLOCK_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C2, CLOCK_ADDR+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C2, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    I2C_GenerateSTOP(I2C2, ENABLE);			//����ֹͣλ

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C2);

   //�ٴ�����Ӧ��ģʽ
	I2C_AcknowledgeConfig(I2C2, ENABLE);  

	return data;
}
/*************    ��������BuffReadFMRTC           ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2011-07-27  23:45:37***********************/
void BuffReadFMRTC(INT8U addr,INT8U len, INT8U *ch)
{
	u8 address=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x0E)
		return ;
taskENTER_CRITICAL(); /*����������*/
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  CLOCK_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C2, CLOCK_ADDR+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(len)
	{
		if(1==len)
		{
			I2C_AcknowledgeConfig(I2C2, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    		I2C_GenerateSTOP(I2C2, ENABLE);			//����ֹͣλ
		}

		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
		*ch = I2C_ReceiveData(I2C2);
		ch++;
		len--;
	}
   //�ٴ�����Ӧ��ģʽ
	I2C_AcknowledgeConfig(I2C2, ENABLE);  
taskEXIT_CRITICAL(); /*����������*/
}




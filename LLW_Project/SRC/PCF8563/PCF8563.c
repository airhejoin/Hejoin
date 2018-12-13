/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺IICʱ��PCF8563
*����ߣ�����������
*ʱ  �䣺2012-05-29  13:10:43
*********************************************************************************
*/
#include".\Lib\includes.h"
/*************    ��������WritePCF8563             ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-29  13:19:18***********************/
INT8U WritePCF8563(INT8U addr,INT8U data)
{
	INT8U address=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x0E)
		return 0;
	taskENTER_CRITICAL(); /*����������*/
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));	

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C1,  WRITEPCF8563, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C1, address);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	/* дһ���ֽ�*/
  	I2C_SendData(I2C1, data); 
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* ֹͣλ*/
  	I2C_GenerateSTOP(I2C1, ENABLE);
	taskEXIT_CRITICAL(); /*����������*/
	
	return 0;
}
/*************    ��������ReadPCF8563             ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-29  13:35:04***********************/
INT8U ReadPCF8563(INT8U addr)
{
	INT8U address=0;
	INT8U data=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x0E)
		return 0;

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C1,  WRITEPCF8563, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C1, address);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C1, READPCF8563, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C1);

   //�ٴ�����Ӧ��ģʽ
	I2C_AcknowledgeConfig(I2C1, ENABLE);  

	return data;
}
/*************    ��������BuffReadPCF8563           **************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-29  13:36:12***********************/
void BuffReadPCF8563(INT8U addr,INT8U len, INT8U *ch)
{
	INT8U address=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x0E)
		return ;
	taskENTER_CRITICAL(); /*����������*/
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C1,  WRITEPCF8563, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C1, address);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C1, READPCF8563, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(len)
	{
		if(1==len)
		{
			I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    		I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
		}

		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
		*ch = I2C_ReceiveData(I2C1);
		ch++;
		len--;
	}
   //�ٴ�����Ӧ��ģʽ
	I2C_AcknowledgeConfig(I2C1, ENABLE);  
	taskEXIT_CRITICAL(); /*����������*/
}
/*************    ��������GetSyeTime               ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-29  13:37:55***********************/
void GetSyeTime(void)
{
	INT8U readdata[7]={0};
//    WritePCF8563(0x00,0x01);  //����ʱ��
//	WritePCF8563(0x00,0x00);

	BuffReadPCF8563(0x02, 7, readdata);

	CLOCK.year=(readdata[6]>>4)*10+(readdata[6]&0x0f);
	CLOCK.month=(readdata[5]>>4)*10+(readdata[5]&0x0f);
	CLOCK.day=(readdata[4]>>4)*10+(readdata[4]&0x0f);
	CLOCK.week=(readdata[3]>>4)*10+(readdata[3]&0x0f);
	CLOCK.hour=(readdata[2]>>4)*10+(readdata[2]&0x0f);
	CLOCK.min=(readdata[1]>>4)*10+(readdata[1]&0x0f);
	CLOCK.sec=(readdata[0]>>4)*10+(readdata[0]&0x0f);
	
}






























#include"..\Lib\includes.h"
extern void I2CInit(void);
void Delay10Us(void)
{    
   u16 i=0;  
   i=100;  //
   while(i--) ;    
}
/*************    ��������Fm24WriteByte***************************
�����ܡ���
����������
�����ء���
��˵������
*******************************/
void FM24WriteByte(u16 addr,u8 data)
{
	u8 address=0;
	u8 slaveaddr=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x200)
		return ;
	/*��ҳ����*/
	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;
	
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
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
}
/*************    ��������HJ                ****************************
�����ܡ���
����������
�����ء���
��˵������
*******************************/
u8 FM24ReadByte(u16 addr)
{
	u8 address=0,data=0;
	u8 slaveaddr=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x200)
		return 0;

	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C2, slaveaddr+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2C2, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
	I2C_GenerateSTOP(I2C2, ENABLE);			//����ֹͣλ

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C2);

   //�ٴ�����Ӧ��ģʽ
	I2C_AcknowledgeConfig(I2C2, ENABLE);  
   	return data;
}
#if 1
/*************��������BuffReadFM24C04*********
�����ܡ���
����������
�����ء���
��˵������
*******************************/
void FM24ReadBuf(u16 addr, u8 *ch, u8 len)
{
	u8 address=0;
	u8 slaveaddr=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x200)
		return ;

	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C2, slaveaddr+1, I2C_Direction_Receiver);
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
}

/*************    ��������BuffFm24WriteByte           ********************
�����ܡ���
����������
�����ء���
��˵������
********************************/
u8  FM24WriteBuf(INT32U addr, u8 *pbuff, INT32U len)
{
	u8 address=0;
	u8 slaveaddr=0;
	u16 TimeOut=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x200)
		return 1 ;
	portENTER_CRITICAL();
	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;
	
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
	I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
	
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
//		Delay10Us();
//		if(++TimeOut>200)
//		{
//			TimeOut=0;
//			I2C_DeInit(I2C2);
//			I2CInit();
//			return 0;
//		}
	}
	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	while(len)
	{
		/* дһ���ֽ�*/
	  	I2C_SendData(I2C2, *pbuff); 
	  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		pbuff++;
		len--;
	}

	/* ֹͣλ*/
	I2C_GenerateSTOP(I2C2, ENABLE);
	portEXIT_CRITICAL();
	return 1;
}
#else
/*************��������BuffReadFM24C04*********
�����ܡ���
����������
�����ء���
��˵������
*******************************/
void FM24ReadBuf(u16 addr, u8 *ch, u8 len)
{
	u8 address=0;
	u8 slaveaddr=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x200)
		return ;

	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*����������ַ(��)24C01*/
	I2C_Send7bitAddress(I2C2, slaveaddr+1, I2C_Direction_Receiver);
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
}

/*************    ��������BuffFm24WriteByte           ********************
�����ܡ���
����������
�����ء���
��˵������
********************************/
u8  FM24WriteBuf(INT32U addr, u8 *pbuff, INT32U len)
{
	u8 address=0;
	u8 slaveaddr=0;
	u16 TimeOut=0;
	/*�жϵ�ַ��Ч*/
	if(addr>0x200)
		return 1 ;
	portENTER_CRITICAL();
	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;
	
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* ������ʼλ */
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
	I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
	
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
//		Delay10Us();
//		if(++TimeOut>200)
//		{
//			TimeOut=0;
//			I2C_DeInit(I2C2);
//			I2CInit();
//			return 0;
//		}
	}
	/*���͵�λ��ַ*/
	address=addr;
	I2C_SendData(I2C2, address);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	while(len)
	{
		/* дһ���ֽ�*/
	  	I2C_SendData(I2C2, *pbuff); 
	  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		pbuff++;
		len--;
	}

	/* ֹͣλ*/
	I2C_GenerateSTOP(I2C2, ENABLE);
	portEXIT_CRITICAL();
	return 1;
}
#endif



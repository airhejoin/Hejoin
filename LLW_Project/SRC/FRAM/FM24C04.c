#include"..\Lib\includes.h"
extern void I2CInit(void);
void Delay10Us(void)
{    
   u16 i=0;  
   i=100;  //
   while(i--) ;    
}
/*************    函数名：Fm24WriteByte***************************
【功能】：
【参数】：
【返回】：
【说明】：
*******************************/
void FM24WriteByte(u16 addr,u8 data)
{
	u8 address=0;
	u8 slaveaddr=0;
	/*判断地址有效*/
	if(addr>0x200)
		return ;
	/*分页处理*/
	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;
	
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	/* 写一个字节*/
  	I2C_SendData(I2C2, data); 
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* 停止位*/
	I2C_GenerateSTOP(I2C2, ENABLE);
}
/*************    函数名：HJ                ****************************
【功能】：
【参数】：
【返回】：
【说明】：
*******************************/
u8 FM24ReadByte(u16 addr)
{
	u8 address=0,data=0;
	u8 slaveaddr=0;
	/*判断地址有效*/
	if(addr>0x200)
		return 0;

	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;

	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C2, slaveaddr+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2C2, DISABLE);	//最后一位后要关闭应答的
	I2C_GenerateSTOP(I2C2, ENABLE);			//发送停止位

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C2);

   //再次允许应答模式
	I2C_AcknowledgeConfig(I2C2, ENABLE);  
   	return data;
}
#if 1
/*************函数名：BuffReadFM24C04*********
【功能】：
【参数】：
【返回】：
【说明】：
*******************************/
void FM24ReadBuf(u16 addr, u8 *ch, u8 len)
{
	u8 address=0;
	u8 slaveaddr=0;
	/*判断地址有效*/
	if(addr>0x200)
		return ;

	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;

	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C2, slaveaddr+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(len)
	{
		if(1==len)
		{
			I2C_AcknowledgeConfig(I2C2, DISABLE);	//最后一位后要关闭应答的
    		I2C_GenerateSTOP(I2C2, ENABLE);			//发送停止位
		}

		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
		*ch = I2C_ReceiveData(I2C2);
		ch++;
		len--;
	}
   //再次允许应答模式
	I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/*************    函数名：BuffFm24WriteByte           ********************
【功能】：
【参数】：
【返回】：
【说明】：
********************************/
u8  FM24WriteBuf(INT32U addr, u8 *pbuff, INT32U len)
{
	u8 address=0;
	u8 slaveaddr=0;
	u16 TimeOut=0;
	/*判断地址有效*/
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
	
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
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
	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	while(len)
	{
		/* 写一个字节*/
	  	I2C_SendData(I2C2, *pbuff); 
	  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		pbuff++;
		len--;
	}

	/* 停止位*/
	I2C_GenerateSTOP(I2C2, ENABLE);
	portEXIT_CRITICAL();
	return 1;
}
#else
/*************函数名：BuffReadFM24C04*********
【功能】：
【参数】：
【返回】：
【说明】：
*******************************/
void FM24ReadBuf(u16 addr, u8 *ch, u8 len)
{
	u8 address=0;
	u8 slaveaddr=0;
	/*判断地址有效*/
	if(addr>0x200)
		return ;

	if(addr>255)
	{
		addr-=255;
		slaveaddr=EEPROM_ADDR+2;
	}
	else
		slaveaddr=EEPROM_ADDR;

	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  slaveaddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C2, slaveaddr+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(len)
	{
		if(1==len)
		{
			I2C_AcknowledgeConfig(I2C2, DISABLE);	//最后一位后要关闭应答的
    		I2C_GenerateSTOP(I2C2, ENABLE);			//发送停止位
		}

		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
		*ch = I2C_ReceiveData(I2C2);
		ch++;
		len--;
	}
   //再次允许应答模式
	I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/*************    函数名：BuffFm24WriteByte           ********************
【功能】：
【参数】：
【返回】：
【说明】：
********************************/
u8  FM24WriteBuf(INT32U addr, u8 *pbuff, INT32U len)
{
	u8 address=0;
	u8 slaveaddr=0;
	u16 TimeOut=0;
	/*判断地址有效*/
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
	
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
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
	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	while(len)
	{
		/* 写一个字节*/
	  	I2C_SendData(I2C2, *pbuff); 
	  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		pbuff++;
		len--;
	}

	/* 停止位*/
	I2C_GenerateSTOP(I2C2, ENABLE);
	portEXIT_CRITICAL();
	return 1;
}
#endif



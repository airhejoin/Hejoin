/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：IIC初始化配置
*设计者：南下三五年
*时  间：2012-05-24  01:03:27
*********************************************************************************
*/
#include "..\STM32Lib\stm32f10x.h"
#include "..\hal.h"
#include <stdio.h>

#define EEPROM_ADDR		0xA0
#define CLOCK_ADDR		0xD0
#define KEY_ADDR        0X1c20    //7K空间给主程序用  7200
/*********************函数名称:WriteFM3130      **************************
【功能】：写FRAM 1个字节
【参数】：
【返回】：
【说明】：
**********************作者: 木瓜   2010年01月13日15:23:52 ***************/
void WriteFM3130(INT16U addr,INT8U data)
{
	u8 address=0;
	/*判断地址有效*/
	if(addr>0x2000)
		return ;
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
	/*发送高位地址*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	/*发送低位地址*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	/* 写一个字节*/
  	I2C_SendData(I2C2, data); 
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* 停止位*/
  	I2C_GenerateSTOP(I2C2, ENABLE);
}
/*************    函数名：BuffWriteFM3130          ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-12  10:39:54***********************/
void BuffWriteFM3130(INT8U *pbuff, INT32U addr, INT32U len)
{
	u8 address=0;
	/*判断地址有效*/
	if(addr>0x2000)
		return ;
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
	/*发送高位地址*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	/*发送低位地址*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	while(len)
	{
		/* 写一个字节*/
	  	/* 写一个字节*/
  		I2C_SendData(I2C2, *pbuff); 
  		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		pbuff++;
		len--;
	}

	/* 停止位*/
	I2C_GenerateSTOP(I2C2, ENABLE);
}
/*************    函数名：ReadFM3130              ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-12  10:34:26***********************/
INT8U ReadFM3130(INT16U addr)
{
	u8 address=0;
	INT8U data=0;
	/*判断地址有效*/
	if(addr>0x2000)
		return 0;

	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送高位地址*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	/*发送低位地址*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C2, EEPROM_ADDR+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C2, DISABLE);	//最后一位后要关闭应答的
    I2C_GenerateSTOP(I2C2, ENABLE);			//发送停止位

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C2);

   //再次允许应答模式
	I2C_AcknowledgeConfig(I2C2, ENABLE);  

	return data;
}
/*********************函数名称:BuffReadFM3130    **************************
【功能】：读FRAM 多个字节
【参数】：
【返回】：
【说明】：
**********************作者: 木瓜   2010年01月13日15:24:37 ***************/
void BuffReadFM3130(INT16U addr, INT8U len, INT8U *ch)
{
   	u8 address=0;
	/*判断地址有效*/
	if(addr>0x2000)
		return ;
taskENTER_CRITICAL(); /*调度器上锁*/
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送高位地址*/
	address=addr>>8;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	/*发送低位地址*/
	address=addr&0xff;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C2, EEPROM_ADDR+1, I2C_Direction_Receiver);
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
  taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*********************函数名称:iic_clear_fram    ************************
【功能】：顺序写FRAM
【参数】：
【返回】：
【说明】：
**********************作者: 木瓜   2010年01月13日15:25:32 ***************/
void ClearFM3130(u16 addr, u16 leng)
{
    u16 i=0;
    
    for(i=0;i<leng;i++)
    {
        WriteFM3130(addr+i, 0);
    }
}
/*************    函数名：WriteFMRTC              ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-12  11:22:57***********************/
void WriteFMRTC(INT8U addr,INT8U data)
{
	u8 address=0;
	/*判断地址有效*/
	if(addr>0x0E)
		return ;
taskENTER_CRITICAL(); /*调度器上锁*/
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));	

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  CLOCK_ADDR, I2C_Direction_Transmitter);
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
taskEXIT_CRITICAL(); /*调度器开锁*/
}
/*************    函数名：ReadFMRTC               ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-12  11:24:59***********************/
INT8U ReadFMRTC(INT8U addr)
{
	u8 address=0;
	INT8U data=0;
	/*判断地址有效*/
	if(addr>0x0E)
		return 0;

	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  CLOCK_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C2, CLOCK_ADDR+1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C2, DISABLE);	//最后一位后要关闭应答的
    I2C_GenerateSTOP(I2C2, ENABLE);			//发送停止位

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C2);

   //再次允许应答模式
	I2C_AcknowledgeConfig(I2C2, ENABLE);  

	return data;
}
/*************    函数名：BuffReadFMRTC           ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2011-07-27  23:45:37***********************/
void BuffReadFMRTC(INT8U addr,INT8U len, INT8U *ch)
{
	u8 address=0;
	/*判断地址有效*/
	if(addr>0x0E)
		return ;
taskENTER_CRITICAL(); /*调度器上锁*/
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C2,  CLOCK_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C2, address);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C2, CLOCK_ADDR+1, I2C_Direction_Receiver);
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
taskEXIT_CRITICAL(); /*调度器开锁*/
}




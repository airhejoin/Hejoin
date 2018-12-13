/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：IIC时钟PCF8563
*设计者：南下三五年
*时  间：2012-05-29  13:10:43
*********************************************************************************
*/
#include".\Lib\includes.h"
/*************    函数名：WritePCF8563             ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-29  13:19:18***********************/
INT8U WritePCF8563(INT8U addr,INT8U data)
{
	INT8U address=0;
	/*判断地址有效*/
	if(addr>0x0E)
		return 0;
	taskENTER_CRITICAL(); /*调度器上锁*/
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));	

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C1,  WRITEPCF8563, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C1, address);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/

	/* 写一个字节*/
  	I2C_SendData(I2C1, data); 
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* 停止位*/
  	I2C_GenerateSTOP(I2C1, ENABLE);
	taskEXIT_CRITICAL(); /*调度器开锁*/
	
	return 0;
}
/*************    函数名：ReadPCF8563             ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-29  13:35:04***********************/
INT8U ReadPCF8563(INT8U addr)
{
	INT8U address=0;
	INT8U data=0;
	/*判断地址有效*/
	if(addr>0x0E)
		return 0;

	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C1,  WRITEPCF8563, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C1, address);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C1, READPCF8563, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C1, DISABLE);	//最后一位后要关闭应答的
    I2C_GenerateSTOP(I2C1, ENABLE);			//发送停止位

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	data = I2C_ReceiveData(I2C1);

   //再次允许应答模式
	I2C_AcknowledgeConfig(I2C1, ENABLE);  

	return data;
}
/*************    函数名：BuffReadPCF8563           **************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-29  13:36:12***********************/
void BuffReadPCF8563(INT8U addr,INT8U len, INT8U *ch)
{
	INT8U address=0;
	/*判断地址有效*/
	if(addr>0x0E)
		return ;
	taskENTER_CRITICAL(); /*调度器上锁*/
	/*等待IIC总线空闲*/
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* 发送起始位 */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	/*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C1,  WRITEPCF8563, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送低位地址*/
	address=addr;
	I2C_SendData(I2C1, address);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
    
	/*起始位*/
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/*发送器件地址(读)24C01*/
	I2C_Send7bitAddress(I2C1, READPCF8563, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(len)
	{
		if(1==len)
		{
			I2C_AcknowledgeConfig(I2C1, DISABLE);	//最后一位后要关闭应答的
    		I2C_GenerateSTOP(I2C1, ENABLE);			//发送停止位
		}

		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
		*ch = I2C_ReceiveData(I2C1);
		ch++;
		len--;
	}
   //再次允许应答模式
	I2C_AcknowledgeConfig(I2C1, ENABLE);  
	taskEXIT_CRITICAL(); /*调度器开锁*/
}
/*************    函数名：GetSyeTime               ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-29  13:37:55***********************/
void GetSyeTime(void)
{
	INT8U readdata[7]={0};
//    WritePCF8563(0x00,0x01);  //更新时钟
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






























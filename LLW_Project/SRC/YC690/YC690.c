#include"YC690.h"
#include".\Lib\includes.h"

#define SOFT_I2C_SEL 1

#define false	0
#define true    1
#define NULL 0
enum {
    CMD_START_FLAG_INDEX = 0,
    CMD_LENGTH_INDEX,   //1
    CMD_CMD_INDEX,      //2
    CMD_PARA_INDEX      //3
};
#define CMD_END_FLAG_LEN        1
#define CRC16_LENGTH            2

/************************************************************
**compute_crc_16 计算CRC
**check_crc16 检查CRC结果是否正确
************************************************************/
u16 compute_crc_16(const u8 *data, u32 length);
signed char check_crc16(const u8 *data, u32 length);


static const u16 crc16_table[256]={ //生成多项式为0x1021 ,正序表
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
  };


//计算CRC16函数。输出数据为小端格式
u16 compute_crc_16(const u8 *data, u32 length)
{
	u16 crc = 0xFFFF;
	u32 i=0;
	for(i = 0;i < length;i++) 
		crc = (crc << 8) ^ crc16_table[((crc >> 8) ^ data[i]) & 0xFF];

	return crc ^ 0xFFFF;
}

//校验接收命令CRC
signed char check_crc16(const u8 *data, u32 length)
{
	u8 crc_tmp[CRC16_LENGTH];
	u8 crc_start_pos;
	if(data == NULL)
		return false;

	crc_start_pos = data[CMD_LENGTH_INDEX] + CMD_CMD_INDEX;

	crc_tmp[0] = data[crc_start_pos + 1];
	crc_tmp[1] = data[crc_start_pos];

	if(*((u16 *)crc_tmp) == compute_crc_16(data, length - CRC16_LENGTH - CMD_END_FLAG_LEN))
		return true;
	else
		return false;
}


void YC690Init(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	#if SOFT_I2C_SEL
	
		/* PB6,7 SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);
	#else
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

	/* PB6,7 SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  I2C_DeInit(I2C1);
	
	I2C1->CR1 |= 0x8000;
	I2C1->CR1 &= ~0x8000;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x30;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 10000;//400K速度
    
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	#endif
}
#define I2C_TIMOUT_MAX 100
#if SOFT_I2C_SEL

#define SDA_H GPIOB->BSRR |= GPIO_Pin_7
#define SDA_L GPIOB->BRR |= GPIO_Pin_7

#define SCL_H GPIOB->BSRR |= GPIO_Pin_6
#define SCL_L GPIOB->BRR |= GPIO_Pin_6

#define SCL_IN GPIOB-IDR & GPIO_Pin_6
#define SDA_IN GPIOB->IDR & GPIO_Pin_7
void SDA_SETIN(void)
{
	GPIOB->CRL &=0x0FFFFFFF; GPIOB->CRL |= 4<<28;
}
void SDA_SETOUT(void)
{
	GPIOB->CRL &=0x0FFFFFFF; GPIOB->CRL |= 3<<28;
}
void I2C_Delay(void)
{    
   u16 i = 0;  
		u16 time=120;
   while(time--)
   {
      i = 5;   //随便定义的一个值, 无意义, 纯粹是消耗CPU时间而已, 可能会编译器被优化  
      while(i--);    
   }
}

u8 I2C_START(void)
{
	SDA_SETOUT();
	SDA_H;
	SCL_H;
	I2C_Delay();
	SDA_L;
	I2C_Delay();
	SCL_L;
	I2C_Delay();
	return 1;
}

void I2C_STOP(void)
{
	SDA_SETOUT();
	SCL_L;
	SDA_L;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	SDA_H;
	I2C_Delay();
}

void I2C_Ack(void)
{
	SCL_L;	
	SDA_SETOUT();
	SDA_L;
	I2C_Delay();
	SCL_H;	
	I2C_Delay();
	SCL_L;	
}

u8 I2C_WaitAck(void)  //返回为：-1有ACK， =0 无ACK
{
	u8 timeout=0;
	//SDA_H;
	SDA_SETIN();
	SCL_H;
	I2C_Delay();
	while(SDA_IN)
	{
		timeout++;
		if(timeout>250)
		{
			//I2C_STOP();
			SCL_L;
			return false;
		}
		//DelayXUs(1);
	}
	SCL_L;
	return true;
}
void I2C_SendByte(u8 SendByte) //数据从高位到低位
{
	u8 i=0;
	SDA_SETOUT();
	SCL_L;
	for(i=0;i<8;i++)
	{
		if(SendByte&0x80)
			SDA_H;
		else
			SDA_L;
		SendByte <<= 1;
		I2C_Delay();
		SCL_H;
		I2C_Delay();
		SCL_L;
		I2C_Delay();
	}
	
}
u8 YC690_SendCmd(u8 chip_addr,u8 *pbuff,u8 len)
{	
	taskENTER_CRITICAL();
	I2C_START();
	I2C_SendByte(chip_addr); //设置高起始地址 + 器件地址
	I2C_WaitAck();
	while(len--)
	{
		I2C_SendByte(*pbuff);
		I2C_WaitAck();
		pbuff++; 
	}
	I2C_STOP();
	taskEXIT_CRITICAL();
	return true; 
}

#else
u8 YC690_SendCmd(u8 chip_addr,u8 *pbuff,u8 len)
{
	u16 I2C_TimeOut=0;
	u8 address=0;
	
	/*判断地址有效*/
	if((chip_addr!=YC690_GAME)&&(chip_addr!=YC690_BACK))
		return false;
	
	/*等待IIC总线空闲*/
	I2C_TimeOut=0;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
		DelayXUs(10);
		if(++I2C_TimeOut>I2C_TIMOUT_MAX)
		{
			I2C_GenerateSTOP(I2C1, ENABLE);
			YC690Init();
			return false;
		}
	}
	taskENTER_CRITICAL();
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* 发送起始位 */
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
	/*发送器件地址(写)*/
	I2C_Send7bitAddress(I2C1,  chip_addr, I2C_Direction_Transmitter);
	I2C_TimeOut=0;
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		DelayXUs(10);
		if(++I2C_TimeOut>I2C_TIMOUT_MAX)
		{
			//I2C_DeInit(I2C1);
			I2C_GenerateSTOP(I2C1, ENABLE);
			YC690Init();
			taskEXIT_CRITICAL();
			return false;
		}
	}
	while(len)
	{
		/* 写一个字节*/
		I2C_SendData(I2C1, *pbuff); 
		I2C_TimeOut=0;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			DelayXUs(10);
			if(++I2C_TimeOut>I2C_TIMOUT_MAX)
			{
				I2C_GenerateSTOP(I2C1, ENABLE);
				YC690Init();
				taskEXIT_CRITICAL();
				return false;
			}
		}
		pbuff++;
		len--;
	}
	/* 停止位*/
	I2C_GenerateSTOP(I2C1, ENABLE);
	taskEXIT_CRITICAL();
	return true;
}
#endif
u8 YC690_Play(u8 chip_addr,u8 num )
{
	u16 crc16=0;
	u8 buf[9]={0x5A, 0x04, 0x0C, 0x00, 0x03, 0x02, 0xB4, 0x5C, 0xA5};	
	if(num>=VOICE_MAX)
		return 1;
	buf[3]=0;
	buf[4]=num;
	crc16=compute_crc_16(buf,6);
	buf[6]=crc16>>8;
	buf[7]=crc16&0xff;
	return YC690_SendCmd(chip_addr,buf,9);
}

void YC690_SetVol(u8 chip_addr,u8 vol)
{
	u16 crc16=0;
	u8 buf[7]={0x5a,0x02,0x14,0x00,0x00,0x00,0xa5};
	buf[3]=vol;
	crc16=compute_crc_16(buf,4);
	buf[4]=crc16>>8;
	buf[5]=crc16&0xff;
	YC690_SendCmd(chip_addr,buf,7);
}


void YC690_Stop(u8 chip_addr )
{
	u8 buf[6]={0x5a,0x01,0x02,0xb9,0x1f,0xa5};
	YC690_SendCmd(chip_addr,buf,6);
}





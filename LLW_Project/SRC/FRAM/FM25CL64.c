/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：FM25CL64初始化配置
*设计者：南下三五年
*时  间：2012-11-15  10:41:05
*********************************************************************************
*/
#include".\Lib\includes.h"
/*************    函数名：InitFM25CL64Hard        ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-15  10:42:24***********************/
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

	/*  PA.4 作片选*/
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//预置为高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI1 configuration  */
	SPI_Cmd(SPI1, DISABLE); 												//必须先禁能,才能改变MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=0 时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=0 数据捕获第1个
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	    //4分频	,不能太快
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE); 
}

/*************    函数名：SPI1_ReadWriteByte       ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-15  10:55:18***********************/
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
/*************    函数名：ReadDataFM25CL64          *************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-15  11:50:33***********************/
INT8U ReadDataFM25CL64(INT16U addr)
{
	INT8U data=0;

	if(addr>0x1fff)
		return 0;
	
	FM25CL64_CS_L;

	//!读命令
	SPI1_ReadWriteByte(READ_FM25CL);

	//!地址
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!获取数据
	data=SPI1_ReadWriteByte(0xff);
	
	FM25CL64_CL_H;
	
	return data;
}

/*************    函数名：WriteDataFM25CL64       ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-15  11:53:23***********************/
void WriteDataFM25CL64(INT16U addr ,INT8U data)
{
	if(addr>0x1fff)
		return ;
	
	FM25CL64_CS_L;
	//!使能写
	SPI1_ReadWriteByte(WREN_FM25CL);

	FM25CL64_CL_H;

	FM25CL64_CS_L;
	//!写命令
	SPI1_ReadWriteByte(WRITE_FM25CL);

	//!地址
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!数据
	SPI1_ReadWriteByte(data);

	FM25CL64_CL_H;
}

/*************    函数名：BuffWriteFM25CL64       ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-15  11:12:55***********************/
void BuffWriteFM25CL64(INT8U *pbuff, INT16U addr, INT16U len)
{
	if(addr>0x1fff)
		return ;
	
	FM25CL64_CS_L;
	//!使能写
	SPI1_ReadWriteByte(WREN_FM25CL);

	FM25CL64_CL_H;

	FM25CL64_CS_L;
	//!写命令
	SPI1_ReadWriteByte(WRITE_FM25CL);

	//!地址
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!数据
	while(len)
	{
		SPI1_ReadWriteByte(*pbuff);
		pbuff++;
		len--;
	}

	FM25CL64_CL_H;
}

/*************    函数名：BuffReadFM25CL64        ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-15  13:23:48***********************/
void BuffReadFM25CL64(INT16U addr, INT8U *pbuff,  INT16U len)
{
	if(addr>0x1fff)
		return ;
	
	FM25CL64_CS_L;

	//!读命令
	SPI1_ReadWriteByte(READ_FM25CL);

	//!地址
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(addr>>8);

	//!获取数据
	while(len)
	{
		*pbuff=SPI1_ReadWriteByte(0xff);
		len--;
		pbuff++;
	}
	
	FM25CL64_CL_H;
}

/*************    函数名：ClearFM25CL64            ***************************
【功能】：清除铁电
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-15  15:27:22***********************/
void ClearFM25CL64(void)
{
	INT16U i=0;
	
	FM25CL64_CS_L;
	//!使能写
	SPI1_ReadWriteByte(WREN_FM25CL);

	FM25CL64_CL_H;

	FM25CL64_CS_L;
	//!写命令
	SPI1_ReadWriteByte(WRITE_FM25CL);

	//!地址
	SPI1_ReadWriteByte(0);
	SPI1_ReadWriteByte(0);

	//!数据
	for(i=0;i<8192;i++)
	{
		SPI1_ReadWriteByte(0);
	}

	FM25CL64_CL_H;
}




































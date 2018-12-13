#ifndef _W25Q128_H_
#define _W25Q128_H_
#include"stm32f10x.h"

#define CHIP_ADDR_MAX 0x1000000//最大地址



/*W25Q128
//1 Sector=4096 bytes
//1 Block =16 Sector
//128M bit=16M byte=256 block=4096 Sector

*共65536 Page ，每个Page有256 Byte，每次最多可以写256个字节，可一次擦除16页，128页，256页
*共4096 Sector，共256可擦除的块
*64字节独立串行数据，包括制造商和芯片ID
*/

//W25X系列/Q系列芯片列表	   
//#define W25Q80 	0XEF13 	
//#define W25Q16 	0XEF14
//#define W25Q32 	0XEF15
//#define W25Q64 	0XEF16
#define W25Q128 0xEF17

#define W25x_ReadId				0x90
#define W25x_ReadData 		0x03
#define W25x_WriteEnable 	0x06
#define W25x_WriteDisable 0x04
#define W25x_PageProgram 	0x02
#define W25x_ChipErase 		0xC7

#define W25x_ReadReg1 		0x05
#define W25x_ReadReg2 		0x35
#define W25x_WriteStatusReg  		0x01

#define W25x_SectorErase 	0x20
#define W25x_PowerDown 		0xB9
#define W25x_PowerUp 			0xAB
/****************W24QXX SPI端口定义*******************
	连接STM32的SPI2
	CS	->PB12
	SCLK->PB13
	MISO->PB14
	MOSI->PB15
***********************************************/



#define USER_START_ADDR 2097152//用户区地址
//#define FONTINFO_START_ADDR 2097152//字体储存信息地址

#define FONTDATA1_START_ADDR 0//字库数据开始地址
#define FONTDATA2_START_ADDR 1048576//字库数据开始地址


#define PICINFO_START_ADDR 3145728//图片存储信息地址
#define PICDATA_START_ADDR 3149824//图片数据开始

extern u16 SPI_FLASH_TYPE; //W25Q128
extern u8 W25Qxx_BUFFER[4096];
void W25QxxInit(void);
void W25QxxErase_Sector(u32 Dst_Addr); 
void W25QxxErase_Chip(void);   
void W25QxxWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite); 
void W25QxxWrite_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QxxWrite_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QxxRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);  
void W25QxxWrite_Enable(void); 
void W25QxxWrite_Disable(void);  
void W25QxxWrite_SR(u8 sr);
void W25QxxWait_Busy(void);  
u8 W25QxxReadSR(void);  
u16 W25QxxReadID(void);
void ErasePictureSector(void);  
#endif

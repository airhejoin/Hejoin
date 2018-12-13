#include "W25Q128.h"
#include "SPI.h"
u16 SPI_FLASH_TYPE=W25Q64; //W25Q128
void W25QxxDelay(u16 t)
{    
   u8 i=0;  
   while(t--)
   {
      i=10; 
      while(i--) ;    
   }
}
//W25Qxx
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:0,,WP
//TB,BP2,BP1,BP0:FLASH
//WEL:
//BUSY:(1,;0,)
//:0x00
u8 W25QxxReadSR(void)   
{  
	u8 byte=0;   
	SPI2_CS_CLR;                            //   
	SPI2_ReadWriteByte(W25x_ReadReg1);    //    
	byte=SPI2_ReadWriteByte(0Xff);             //  
	SPI2_CS_SET;                            //     
	return byte;   
}
//W25Qxx
//SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)!!!
void W25QxxWrite_SR(u8 sr)   
{   
	SPI2_CS_CLR;                            //   
	SPI2_ReadWriteByte(W25x_WriteStatusReg);   //    
	SPI2_ReadWriteByte(sr);               //  
	SPI2_CS_SET;                            //         
}   
//W25Qxx 
//WEL   
void W25QxxWrite_Enable(void)   
{
	SPI2_CS_CLR;                            //   
	SPI2_ReadWriteByte(W25x_WriteEnable);      //  
	SPI2_CS_SET;                            //         
} 
//W25Qxx 
//WEL  
void W25QxxWrite_Disable(void)   
{  
	SPI2_CS_CLR;                            //   
	SPI2_ReadWriteByte(W25x_WriteDisable);     //    
	SPI2_CS_SET;                            //         
} 
//ID
//:   
//0XEF13,W25Q80  
//0XEF14,W25Q16    
//0XEF15,W25Q32  
//0XEF16,W25Q64 
//0XEF17,W25Q128  
//u16 W25QxxReadID(void)
//{
//	u16 Temp = 0;   
//	SPI2_CS_CLR;
//	
//	SPI2_ReadWriteByte(W25x_ReadId);//ID   
//	SPI2_ReadWriteByte(0x00);//传入24bit地址 
//	SPI2_ReadWriteByte(0x00); 
//	SPI2_ReadWriteByte(0x00); 
//		
//	Temp|=SPI2_ReadWriteByte(0xFF)<<8;//先传出制造商ID，再传出芯片ID  
//	Temp|=SPI2_ReadWriteByte(0xFF);
//	
//	SPI2_CS_SET;     
//	return Temp;
//}      
//SPI FLASH  
//
//pBuffer:
//ReadAddr:(24bit)
//NumByteToRead:(65535)
void W25QxxReadBuf(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead)   
{ 
	u16 i;      
	SPI2_CS_CLR;                            //   
	SPI2_ReadWriteByte(W25x_ReadData);         //   
	SPI2_ReadWriteByte((u8)((ReadAddr)>>16));  //24bit    
	SPI2_ReadWriteByte((u8)((ReadAddr)>>8));   
	SPI2_ReadWriteByte((u8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=SPI2_ReadWriteByte(0XFF);   //  
	}
	SPI2_CS_SET;
}

void W25QxxWriteBuf(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)
{
	u16 i;  
	W25QxxWrite_Enable();                  //SET WEL 
	SPI2_CS_CLR;                            //   
	SPI2_ReadWriteByte(W25x_PageProgram);      //   
	SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); //24bit    
	SPI2_ReadWriteByte((u8)((WriteAddr)>>8));   
	SPI2_ReadWriteByte((u8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)
		SPI2_ReadWriteByte(pBuffer[i]);//  
	SPI2_CS_SET;                            // 
	W25QxxWait_Busy();  //
} 


//SPI FLASH 
//0XFF,0XFF!
// 
//,!
//pBuffer:
//WriteAddr:(24bit)
//NumByteToWrite:(65535)
//CHECK OK
void W25QxxWriteBufNoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{  
	u16 pageremain;  
	pageremain=256-WriteAddr%256; //   
	if(NumByteToWrite<=pageremain)
		pageremain=NumByteToWrite;//256
	while(1)
	{   
		W25QxxWriteBuf(WriteAddr,pBuffer,pageremain);
		if(NumByteToWrite==pageremain)
			break;//
		else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain; //
			if(NumByteToWrite>256)
				pageremain=256; //256
			else 
				pageremain=NumByteToWrite; //256
		}
	}   
} 



//SPI FLASH  
//
//!
//pBuffer:
//WriteAddr:(24bit) 
//NumByteToWrite:(65535)   
u8 W25Qxx_BUFFER[4096];  
void W25QxxWrite(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 
	u32 secpos;//扇区地址
	u16 secoff;//扇区偏移地址
	u16 secremain;//扇区内剩余地址  
	u16 i;    
	u8 * W25Qxx_BUF; 
	W25Qxx_BUF=W25Qxx_BUFFER; 
	secpos=WriteAddr/4096;//扇区地址
	secoff=WriteAddr%4096;//在扇区内的偏移地址
	secremain=4096-secoff;//扇区内剩余地址   
	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//带写入数据量不大于扇区内剩余的大小
	while(1) 
	{ 
		W25QxxReadBuf(secpos*4096,W25Qxx_BUF,4096);//读出secpos扇区的数据，4096
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25Qxx_BUF[secoff+i]!=0XFF)
				break;//需要擦除   
		}
		if(i<secremain)//
		{//里面有数据，需要擦除
			W25QxxEraseSector(secpos);//擦除扇区
			for(i=0;i<secremain;i++)  //填充数据
			{
				W25Qxx_BUF[i+secoff]=pBuffer[i]; 
			}
			W25QxxWriteBufNoCheck(secpos*4096,W25Qxx_BUF,4096);//写入数据  
		}
		else//里面没有数据
			W25QxxWriteBufNoCheck(WriteAddr,pBuffer,secremain);//直接写入数据
		if(NumByteToWrite==secremain)
			break;//写入结束
		else
		{//未完成
			secpos++;//扇区地址加1
			secoff=0;//地址偏移为0 
			pBuffer+=secremain;//数据指针偏移
			WriteAddr+=secremain;//写地址偏移
			NumByteToWrite-=secremain;//重新计算待写入数量
			if(NumByteToWrite>4096)
				secremain=4096;//下一扇区剩余数
			else 
				secremain=NumByteToWrite;//
		}  
	} 
}





////芯片擦除
//void W25QxxErase_Chip(void)   
//{                                   
//	W25QxxWrite_Enable();                  //SET WEL 
//	W25QxxWait_Busy();   
//	SPI2_CS_CLR;                            //   
//	SPI2_ReadWriteByte(W25x_ChipErase);        //  
//	SPI2_CS_SET;                            //         
//	W25QxxWait_Busy();   
//}   
////扇区擦除
////:150ms
void W25QxxEraseSector(u32 Dst_Addr)   
{  
//falsh,   
	Dst_Addr*=4096;
	W25QxxWrite_Enable();                  //SET WEL 
	W25QxxWait_Busy();   
	SPI2_CS_CLR;                            //   
	SPI2_ReadWriteByte(W25x_SectorErase);      // 
	SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));  //24bit    
	SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
	SPI2_ReadWriteByte((u8)Dst_Addr);  
	SPI2_CS_SET;                            //         
	W25QxxWait_Busy();     //
}  
////
void W25QxxWait_Busy(void)   
{   
	while((W25QxxReadSR()&0x01)==0x01);   // BUSY
}  
//

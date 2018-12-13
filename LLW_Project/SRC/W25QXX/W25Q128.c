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
//	SPI2_ReadWriteByte(0x00);//����24bit��ַ 
//	SPI2_ReadWriteByte(0x00); 
//	SPI2_ReadWriteByte(0x00); 
//		
//	Temp|=SPI2_ReadWriteByte(0xFF)<<8;//�ȴ���������ID���ٴ���оƬID  
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
	u32 secpos;//������ַ
	u16 secoff;//����ƫ�Ƶ�ַ
	u16 secremain;//������ʣ���ַ  
	u16 i;    
	u8 * W25Qxx_BUF; 
	W25Qxx_BUF=W25Qxx_BUFFER; 
	secpos=WriteAddr/4096;//������ַ
	secoff=WriteAddr%4096;//�������ڵ�ƫ�Ƶ�ַ
	secremain=4096-secoff;//������ʣ���ַ   
	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//��д��������������������ʣ��Ĵ�С
	while(1) 
	{ 
		W25QxxReadBuf(secpos*4096,W25Qxx_BUF,4096);//����secpos���������ݣ�4096
		for(i=0;i<secremain;i++)//У������
		{
			if(W25Qxx_BUF[secoff+i]!=0XFF)
				break;//��Ҫ����   
		}
		if(i<secremain)//
		{//���������ݣ���Ҫ����
			W25QxxEraseSector(secpos);//��������
			for(i=0;i<secremain;i++)  //�������
			{
				W25Qxx_BUF[i+secoff]=pBuffer[i]; 
			}
			W25QxxWriteBufNoCheck(secpos*4096,W25Qxx_BUF,4096);//д������  
		}
		else//����û������
			W25QxxWriteBufNoCheck(WriteAddr,pBuffer,secremain);//ֱ��д������
		if(NumByteToWrite==secremain)
			break;//д�����
		else
		{//δ���
			secpos++;//������ַ��1
			secoff=0;//��ַƫ��Ϊ0 
			pBuffer+=secremain;//����ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��
			NumByteToWrite-=secremain;//���¼����д������
			if(NumByteToWrite>4096)
				secremain=4096;//��һ����ʣ����
			else 
				secremain=NumByteToWrite;//
		}  
	} 
}





////оƬ����
//void W25QxxErase_Chip(void)   
//{                                   
//	W25QxxWrite_Enable();                  //SET WEL 
//	W25QxxWait_Busy();   
//	SPI2_CS_CLR;                            //   
//	SPI2_ReadWriteByte(W25x_ChipErase);        //  
//	SPI2_CS_SET;                            //         
//	W25QxxWait_Busy();   
//}   
////��������
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

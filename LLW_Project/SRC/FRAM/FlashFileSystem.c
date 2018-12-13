#include "FlashFileSystem.h"
#include "W25Q128.h"
#include "SPI.h"
//flash 16M in byte
//0~1M用户区
//1M~2M文件表格 byte0 图片数量 byte1~Byte4可用空间  byte5~byte8可用空间的开始地址 byte9 byte10,图片1的长宽。。。。。。
//
//2M~14M图片区
//14M~16M字库区
#define PIC_BLOCK_SIZE 12//12M
//
#define FLASH_TYPE_ERROR 0

#define FLASHCHIP_INIT_FLAG1 0xF3
#define FLASHCHIP_INIT_FLAG2 0x3F

#define USER_ADDR_START 0x00000

#define PIC_NUM_ADDR	1024 //0~255张图片

#define PIC_USERFULSIZE_ADDR 1025//4byte 0


#define PIC_RECORD_ADDR  
PIC_SAVE PicSave;
PICTURE Picture;
u8 buf[10]={0};
u8 FlashFileSysInit(void)
{
	u8 status;
	
	u32 temp=0;
	
	W25QxxInit();
	if(W25Q128!=W25QxxReadID())//读取ID,判断是否为128
	{
		printf("Cannot read flash chip id!\r\n");
		return FLASH_TYPE_ERROR;//Flash型号错误
	}
		
	W25QxxRead(buf,USER_ADDR_START,2);
	if((buf[0]==FLASHCHIP_INIT_FLAG1)&&(buf[1]==FLASHCHIP_INIT_FLAG2))
	{//芯片已经被初始化
		W25QxxRead(&PicSave.Num,PIC_NUM_ADDR,1);//获取图片数量
		W25QxxRead(buf,PIC_USERFULSIZE_ADDR,8);//获取可用空间大小，及可用空间的开始地址
		PicSave.UsableSize   =((u32)buf[0]<<24)+((u32)buf[1]<<16)+((u32)buf[2]<<8)+(u32)buf[3];//图片区剩余的可用空间
		PicSave.UsableStrAddr=((u32)buf[4]<<24)+((u32)buf[5]<<16)+((u32)buf[6]<<8)+(u32)buf[7];
		printf("Flash has been initial!\r\n");
	}
	else
	{
		W25QxxErase_Chip();//擦除芯片
		W25QxxWrite_Enable();
		buf[0]=FLASHCHIP_INIT_FLAG1;
		buf[1]=FLASHCHIP_INIT_FLAG2;
		W25QxxWrite(buf,USER_ADDR_START,2);//写入初始化标志
		temp=1024*1024*PIC_BLOCK_SIZE/4096;//图片区可用空间总扇区大小
		buf[0]=0;//图片数量
		buf[1]=(u8)(temp>>24);
		buf[2]=(u8)(temp>>16);
		buf[3]=(u8)(temp>>8);
		buf[4]=(u8)temp;
		PicSave.UsableSize=temp;
		temp=2048*1024;//2M开始为图片区
		buf[5]=(u8)(temp>>24);
		buf[6]=(u8)(temp>>16);
		buf[7]=(u8)(temp>>8);
		buf[8]=(u8)temp;
		W25QxxWrite(buf,PIC_NUM_ADDR,9);//写入数据
		PicSave.Num=0;
		PicSave.UsableStrAddr=temp;
		W25QxxWrite_Disable();
		printf("Flash first initial ok!\r\n");
	}
	PicSave.Idex=0;
}
void WritePicSaveData(void)
{
	
	buf[0]=PicSave.Num;//图片数量
	buf[1]=(u8)(PicSave.UsableSize>>24);//剩余空间
	buf[2]=(u8)(PicSave.UsableSize>>16);
	buf[3]=(u8)(PicSave.UsableSize>>8);
	buf[4]=(u8)PicSave.UsableSize;

	buf[5]=(u8)(PicSave.UsableStrAddr>>24);//空闲空间的起始地址
	buf[6]=(u8)(PicSave.UsableStrAddr>>16);
	buf[7]=(u8)(PicSave.UsableStrAddr>>8);
	buf[8]=(u8)PicSave.UsableStrAddr;
	W25QxxWrite(buf,PIC_NUM_ADDR,9);//写入数据
	
}
//u8 W25Qxx_BUFFER[4096];  
void SpaceErase(u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;//扇区地址
	u16 secoff;//扇区偏移地址
	u16 secremain;//扇区内剩余地址  
	u16 i;     
	secpos=WriteAddr/4096;//扇区地址
	secoff=WriteAddr%4096;//在扇区内的偏移地址
	secremain=4096-secoff;//扇区内剩余地址   
	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//带写入数据量不大于扇区内剩余的大小
	while(1) 
	{ 
		W25QxxRead(W25Qxx_BUFFER,secpos*4096,4096);//读出secpos扇区的数据，4096
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25Qxx_BUFFER[secoff+i]!=0XFF)
				break;//需要擦除   
		}
		if(i<secremain)//
		{//里面有数据，需要擦除
			W25QxxErase_Sector(secpos);//擦除扇区
			for(i=0;i<secremain;i++)  //填充数据
			{
				W25Qxx_BUFFER[i+secoff]=0xff; 
			}
			W25QxxWrite_NoCheck(W25Qxx_BUFFER,secpos*4096,4096);//写入数据  
		}
		//else//里面没有数据
			//W25QxxWrite_NoCheck(pBuffer,WriteAddr,secremain);//直接写入数据
		if(NumByteToWrite==secremain)
			break;//写入结束
		else
		{//未完成
			secpos++;//扇区地址加1
			secoff=0;//地址偏移为0 
			//pBuffer+=secremain;//数据指针偏移
			WriteAddr+=secremain;//写地址偏移
			NumByteToWrite-=secremain;//重新计算待写入数量
			if(NumByteToWrite>4096)
				secremain=4096;//下一扇区剩余数
			else 
				secremain=NumByteToWrite;//
		}  
	} 
}
void W25QxxUsartWrite(u32 WriteAddr,u32 NumByteToWrite)   
{  
	u8 error=0;
	u32 pageremain=0,i=0;  
	u32 addr_tmp=0,pageremain_tmp=0;
	u8 temp=0;
	Picture.Times=0;
	W25QxxWrite_Enable(); 
	pageremain=256-WriteAddr%256; //   
	if(NumByteToWrite<=pageremain)
		pageremain=NumByteToWrite;//256
	while(1)
	{   
		addr_tmp=WriteAddr;
		pageremain_tmp=pageremain;
		SPI2_CS_CLR;//
		SPI2_ReadWriteByte(W25x_PageProgram); //   
		SPI2_ReadWriteByte((u8)((addr_tmp)>>16)); //24bit    
		SPI2_ReadWriteByte((u8)((addr_tmp)>>8));   
		SPI2_ReadWriteByte((u8)addr_tmp);   
		for(i=0;i<pageremain;i++)
		{
			while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET)
			{//接收超时就自动跳出
				if(Picture.Times>0)
				{
					Picture.Times--;
					if(Picture.Times==0)
					{
						error=1;//超时,图片接收不完整
						break;
					}
				}
			}
			if(Picture.Times>0)
			{
				temp=USART_ReceiveData(USART1);
				SPI2_ReadWriteByte(temp);// 	
				Picture.Times=20000000;				
			}
			else
			{
				break;
			}
		}
		SPI2_CS_SET;                            // 
		W25QxxWait_Busy();  //
		if(error)
		{
			printf("Receive pic data error\r\n");
			break;
		}
		if(NumByteToWrite==pageremain)
		{
			PicSave.UsableSize=PicSave.UsableSize-NumByteToWrite;
			PicSave.UsableStrAddr+=WriteAddr;
			PicSave.Num+=1;
			
			break;//
		}
		else //NumByteToWrite>pageremain
		{
			WriteAddr+=pageremain;
			NumByteToWrite-=pageremain; //
			if(NumByteToWrite>256)
				pageremain=256; //256
			else 
				pageremain=NumByteToWrite; //256
		}
	}   
} 
//void AddPicToFlash(u32 addr,u32 size,u8 *dat)
//{
//	u32 tmp1=0,tmp2=0,i=0,j=0;
//	u32 remain=0,noremain=0,sector,st_read_addr;
//	if(addr>=CHIP_ADDR_MAX)
//	{
//		return;//超出范围
//	}
//	else
//	{//获得整除部分和余数部分
//		noremain=addr/4096;//能够被4096整除的
//		remain=addr%4096;////不足4096剩余的
//		if(remain>0)//有余
//		{
//			st_read_addr=noremain*4096;
//			//需读取的数据的起始地址
//			W25QxxRead(FlashBuf,st_read_addr,4096);//读出原来的数据
//			W25QxxErase_Sector(noremain);//擦除该扇区
//			
//			//写入数据到这个4096的数据缓冲中
//			i=addr-st_read_addr;//需要填充数据的起始地址
//			for(;i<(4096-(addr-remain));i++)
//			{
//				FlashBuf[i]=dat[j++];
//			}
//			W25QxxWrite_Enable();
//			W25QxxWrite(FlashBuf,st_read_addr,4096);//写入原来flash中的内容
//			noremain+=1;//下一个需要擦除的扇区号
//		}
//		else
//		{//刚好整除
//			i=noremain;//
//			noremain=(addr+size)/4096+1;
//			for(;i<noremain;i++)
//			{
//				W25QxxErase_Sector(noremain);//擦除扇区
//			}
//		}
//	}
//}

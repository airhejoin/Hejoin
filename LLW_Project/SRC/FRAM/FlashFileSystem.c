#include "FlashFileSystem.h"
#include "W25Q128.h"
#include "SPI.h"
//flash 16M in byte
//0~1M�û���
//1M~2M�ļ���� byte0 ͼƬ���� byte1~Byte4���ÿռ�  byte5~byte8���ÿռ�Ŀ�ʼ��ַ byte9 byte10,ͼƬ1�ĳ�������������
//
//2M~14MͼƬ��
//14M~16M�ֿ���
#define PIC_BLOCK_SIZE 12//12M
//
#define FLASH_TYPE_ERROR 0

#define FLASHCHIP_INIT_FLAG1 0xF3
#define FLASHCHIP_INIT_FLAG2 0x3F

#define USER_ADDR_START 0x00000

#define PIC_NUM_ADDR	1024 //0~255��ͼƬ

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
	if(W25Q128!=W25QxxReadID())//��ȡID,�ж��Ƿ�Ϊ128
	{
		printf("Cannot read flash chip id!\r\n");
		return FLASH_TYPE_ERROR;//Flash�ͺŴ���
	}
		
	W25QxxRead(buf,USER_ADDR_START,2);
	if((buf[0]==FLASHCHIP_INIT_FLAG1)&&(buf[1]==FLASHCHIP_INIT_FLAG2))
	{//оƬ�Ѿ�����ʼ��
		W25QxxRead(&PicSave.Num,PIC_NUM_ADDR,1);//��ȡͼƬ����
		W25QxxRead(buf,PIC_USERFULSIZE_ADDR,8);//��ȡ���ÿռ��С�������ÿռ�Ŀ�ʼ��ַ
		PicSave.UsableSize   =((u32)buf[0]<<24)+((u32)buf[1]<<16)+((u32)buf[2]<<8)+(u32)buf[3];//ͼƬ��ʣ��Ŀ��ÿռ�
		PicSave.UsableStrAddr=((u32)buf[4]<<24)+((u32)buf[5]<<16)+((u32)buf[6]<<8)+(u32)buf[7];
		printf("Flash has been initial!\r\n");
	}
	else
	{
		W25QxxErase_Chip();//����оƬ
		W25QxxWrite_Enable();
		buf[0]=FLASHCHIP_INIT_FLAG1;
		buf[1]=FLASHCHIP_INIT_FLAG2;
		W25QxxWrite(buf,USER_ADDR_START,2);//д���ʼ����־
		temp=1024*1024*PIC_BLOCK_SIZE/4096;//ͼƬ�����ÿռ���������С
		buf[0]=0;//ͼƬ����
		buf[1]=(u8)(temp>>24);
		buf[2]=(u8)(temp>>16);
		buf[3]=(u8)(temp>>8);
		buf[4]=(u8)temp;
		PicSave.UsableSize=temp;
		temp=2048*1024;//2M��ʼΪͼƬ��
		buf[5]=(u8)(temp>>24);
		buf[6]=(u8)(temp>>16);
		buf[7]=(u8)(temp>>8);
		buf[8]=(u8)temp;
		W25QxxWrite(buf,PIC_NUM_ADDR,9);//д������
		PicSave.Num=0;
		PicSave.UsableStrAddr=temp;
		W25QxxWrite_Disable();
		printf("Flash first initial ok!\r\n");
	}
	PicSave.Idex=0;
}
void WritePicSaveData(void)
{
	
	buf[0]=PicSave.Num;//ͼƬ����
	buf[1]=(u8)(PicSave.UsableSize>>24);//ʣ��ռ�
	buf[2]=(u8)(PicSave.UsableSize>>16);
	buf[3]=(u8)(PicSave.UsableSize>>8);
	buf[4]=(u8)PicSave.UsableSize;

	buf[5]=(u8)(PicSave.UsableStrAddr>>24);//���пռ����ʼ��ַ
	buf[6]=(u8)(PicSave.UsableStrAddr>>16);
	buf[7]=(u8)(PicSave.UsableStrAddr>>8);
	buf[8]=(u8)PicSave.UsableStrAddr;
	W25QxxWrite(buf,PIC_NUM_ADDR,9);//д������
	
}
//u8 W25Qxx_BUFFER[4096];  
void SpaceErase(u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;//������ַ
	u16 secoff;//����ƫ�Ƶ�ַ
	u16 secremain;//������ʣ���ַ  
	u16 i;     
	secpos=WriteAddr/4096;//������ַ
	secoff=WriteAddr%4096;//�������ڵ�ƫ�Ƶ�ַ
	secremain=4096-secoff;//������ʣ���ַ   
	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//��д��������������������ʣ��Ĵ�С
	while(1) 
	{ 
		W25QxxRead(W25Qxx_BUFFER,secpos*4096,4096);//����secpos���������ݣ�4096
		for(i=0;i<secremain;i++)//У������
		{
			if(W25Qxx_BUFFER[secoff+i]!=0XFF)
				break;//��Ҫ����   
		}
		if(i<secremain)//
		{//���������ݣ���Ҫ����
			W25QxxErase_Sector(secpos);//��������
			for(i=0;i<secremain;i++)  //�������
			{
				W25Qxx_BUFFER[i+secoff]=0xff; 
			}
			W25QxxWrite_NoCheck(W25Qxx_BUFFER,secpos*4096,4096);//д������  
		}
		//else//����û������
			//W25QxxWrite_NoCheck(pBuffer,WriteAddr,secremain);//ֱ��д������
		if(NumByteToWrite==secremain)
			break;//д�����
		else
		{//δ���
			secpos++;//������ַ��1
			secoff=0;//��ַƫ��Ϊ0 
			//pBuffer+=secremain;//����ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��
			NumByteToWrite-=secremain;//���¼����д������
			if(NumByteToWrite>4096)
				secremain=4096;//��һ����ʣ����
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
			{//���ճ�ʱ���Զ�����
				if(Picture.Times>0)
				{
					Picture.Times--;
					if(Picture.Times==0)
					{
						error=1;//��ʱ,ͼƬ���ղ�����
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
//		return;//������Χ
//	}
//	else
//	{//����������ֺ���������
//		noremain=addr/4096;//�ܹ���4096������
//		remain=addr%4096;////����4096ʣ���
//		if(remain>0)//����
//		{
//			st_read_addr=noremain*4096;
//			//���ȡ�����ݵ���ʼ��ַ
//			W25QxxRead(FlashBuf,st_read_addr,4096);//����ԭ��������
//			W25QxxErase_Sector(noremain);//����������
//			
//			//д�����ݵ����4096�����ݻ�����
//			i=addr-st_read_addr;//��Ҫ������ݵ���ʼ��ַ
//			for(;i<(4096-(addr-remain));i++)
//			{
//				FlashBuf[i]=dat[j++];
//			}
//			W25QxxWrite_Enable();
//			W25QxxWrite(FlashBuf,st_read_addr,4096);//д��ԭ��flash�е�����
//			noremain+=1;//��һ����Ҫ������������
//		}
//		else
//		{//�պ�����
//			i=noremain;//
//			noremain=(addr+size)/4096+1;
//			for(;i<noremain;i++)
//			{
//				W25QxxErase_Sector(noremain);//��������
//			}
//		}
//	}
//}

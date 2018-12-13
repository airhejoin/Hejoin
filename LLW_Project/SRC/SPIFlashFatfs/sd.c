/*-----------------------------------------------------------------------*/
/* MMC/SDSC/SDHC (in SPI mode) control module  (C)                       */
/*-----------------------------------------------------------------------*/
/* filename: sdc_diskio.c                                                */
//	FFT文件系统的移植部分，需要设置好SD卡的几个函数
//
/*-----------------------------------------------------------------------*/
#include".\Lib\includes.h"
#include "diskio.h"
#include "tff.h"
#include <stdarg.h>

/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD9	(0x40+9)	/* SEND_CSD */
#define CMD10	(0x40+10)	/* SEND_CID */
#define CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */

#define MMC_SELECT()     GPIO_ResetBits(GPIOA, GPIO_Pin_4)       /* MMC CS = L */ 
#define MMC_DESELECT()   GPIO_SetBits(GPIOA, GPIO_Pin_4)         /* MMC CS = H */

extern void Program_IAP(void);

/*--------------------------------------------------------------------------
  Module Private Functions
  ---------------------------------------------------------------------------*/

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */

static BYTE CardType;			    /* b0:MMC(1), b1:SDv1(2), b2:SDv2(4), b3:Block addressing(8) */

extern volatile u32 Timer1, Timer2;    

FATFS fs;
FIL fil;
FRESULT res;		//文件系统返回信息

/***************************************
**函数名:MMC_SPI_Config
**功能:初始化SPI的接口IO，使其支持SPI接口
**注意事项:这个实际是在disk_initialize上调用所以可以考虑更改在MAIN调用
***************************************/
void SPI_Release(void);
void MMC_SPI_Config(void)
{
}
/*************    函数名：TurnToSD            ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：木瓜 2010-03-11  09:21:40***********************/
//每次要用SD的时候都调用此函数。
void TurnToSD(void)
{
     SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1,ENABLE);

    /*A5=CLK,A6=MISO,A7=MOSI*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	/*A5=CLK,A6=MISO,A7=MOSI*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
    /* Configure PA.4 as CS*/
	//置高
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//置为高
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI1 configuration */
	SPI_Cmd(SPI1, DISABLE); //必须要有才能改变MODE
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;								//CPOL=1 时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 数据捕获第二个
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//256分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7
    
	SPI_Init(SPI1, &SPI_InitStructure);
	//SPI_SSOutputCmd(SPI1, ENABLE); //使能NSS脚可用	
    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE); 
	SPI_Release();		//复位一下总线，这个比较重要
}


#define NULL 0
/******************************************************************************
 * 函数名:ToFile
 * 说明: 
 *    转换数据 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 21:21:12, 2015年8月29日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void ToFile(INT8S *file,char *fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(file, fmt, argptr);
	va_end(argptr);
}

/******************************************************************************
 * 函数名:UpdateProgramFromSD
 * 说明: 
 *    通过SD卡更新程序 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 21:24:36, 2015年8月29日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
//#define DATA_XOR
extern INT8U gFlashData[1024];
void UpdateProgramFromSD(void)
{
	INT32U len=0;
	INT16U i=0;
	MMC_SELECT();
	f_mount(0,&fs);//注册一个文件系统
	res=f_open(&fil,"/Game/Game.img",FA_OPEN_EXISTING|FA_READ);
  
	if(res!=FR_OK) 
	{
    IapFlag.FindFile=1L<<File_NoFindProgram;
		return;
	}
	IapFlag.FindFile=1L<<File_FindProgram;
	Start_IAP();
	for(;;)
	{
		vTaskDelay(5);

		res = f_read(&fil, gFlashData, 1024, &len);

		if (res || len == 0)
		{
			break;   
		} 

//解密数据
		#ifdef DATA_XOR
 		for(i=0;i<1024;i++)
 			gFlashData[i]^=i;
		#endif
		Program_IAP();
	}
	f_close(&fil);
	f_mount(0, NULL);
	
	SYS_LED_ON();//打开系统运行指示灯
	IapFlag.LoadFlash=1;//系统开始升级
  LOAD_RIGHT_LED();
	vTaskDelay(3000);
}

/******************************************************************************
 * 函数名:UpdateSoundProgramFromSD
 * 说明: 
 *    升级功放板 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 18:3:18, 2016年9月29日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
void UpdateSoundProgramFromSD(void)
{
	INT32U len=0;
	INT32U i=0,j=0;
	CanTxMsg SendMsg;

	MMC_SELECT();
	f_mount(0,&fs);//注册一个文件系统
	res=f_open(&fil,"/Game/Lamp.img",FA_OPEN_EXISTING|FA_READ);
	IapFlag.LoadFlash=1;
	if(res!=FR_OK)
	{
		IapFlag.FindFile=File_NoFindSound;
		IapFlag.LoadFlash=0;
		return;
	}
	LOAD_RIGHT_LED();
	IapFlag.FindFile=File_FindSound;
	

	//!发送开始升级命令
	SendSoundIapStart();

	vTaskDelay(5000);

	for(;;)
	{
		vTaskDelay(5);

		res = f_read(&fil, gFlashData, 1024, &len);

		if (res || len == 0)
		{
			break;   
		} 

		//!解密数据
		#ifdef DATA_XOR
		for(i=0;i<1024;i++)
			gFlashData[i]^=i;
		#endif
		//!通过CAN发送数据
		for(i=0;i<128;i++)
		{
			SendMsg.DLC=8;

			for(j=0;j<8;j++)
				SendMsg.Data[j]=gFlashData[i*8+j];
			SendServerNoWaitAsk(SendMsg,CanCMD_IapData,SoundCanAddr,IapFlag.CanSync);
			vTaskDelay(5);
		}
		SendMsg.DLC=0;
		SendServerNoWaitAsk(SendMsg,CanCMD_IapDataReady,SoundCanAddr,IapFlag.CanSync);
		
		vTaskDelay(100);
		//!清空数据
		for(i=0;i<1024;i++)
			gFlashData[i]=0xff;
	}
	f_close(&fil);
	f_mount(0, NULL);
}





INT8U StrSearchStr(INT8U *source ,INT8U *str,INT16U len)
{
    INT16U i,j=0;
    for(i=0;i<len;i++)
    {
        if(*source==*str)
        {
            j++;
        }
        source++;
        str++;
    }
    if(j==len)
    {
        return 1;
    }
    return 0;
}
/******************************************************************************
 * 函数名:void ResetBillCheck(void)
 * 说明: 
 *    通过SD卡更新程序 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 21:24:36, 2015年8月29日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
enum{
    CMD_SN=0,
    CMD_RSTBILL,
};
void ConfigSet(INT8U cmd,INT8U *str)
{
    INT8U i=0;
    INT32U val=0;
    switch(cmd)
    {
        case CMD_SN://修改场地编号
            for(i=0;i<6;i++)//场地编号编号范围0~999999
            {
                if((*str<0x30)||(*str>0x39))
                {
                    break;//跳出for循环
                }
                else
                {
                    val=val*10+*str-0x30;
                    str++;
                }
            }
            if(i>0)
            {
                //存储场地编号
                IapFram.SiteSn=val;
                SAVE24C04(IapFram.SiteSn,IapFlag.IdKey);
            }
            break;
//         case CMD_ENCRYPT://加密设置
//             if((*str>0x2f)&&(*str<0x3a))
//             {
//                 val=*str-0x30;
//                 if(val>0)
//                 {    
//                     if(!IapFram.IdEncrypt)
//                     {//在解密状态下，加密所有数据
//                         IapFram.IdEncrypt=1;//加密
//                         DevID[0] = *(vu32*)(0x1FFFF7E8);
//                         DevID[1] = *(vu32*)(0x1FFFF7EC);
//                         DevID[2] = *(vu32*)(0x1FFFF7F0);
//                         IapFlag.IdCode=((DevID[2]>>8)^(DevID[1]<<8)^(DevID[0]<<16))*1664525+1013904223l;
//                         IapFlag.IdKey=IapFram.Code&0xff;//计算ID加密的Key
//                         BuffWriteFM24C04(&IapFram, IAP_FRAM_ADDR,sizeof(IapFram),IapFlag.IdKey); 
//                     }
//                 }
//             }
//             break;
        case CMD_RSTBILL://重置总账
            if((*str>0x2f)&&(*str<0x3a))
            {
               val=*str-0x30;
               if(val>0)
               {
                   if(!IapFram.ClearAllBill)//清账标志置位，实际清账由APP实现
                   {
                       IapFram.ClearAllBill=1;//加密
                       SAVE24C04(IapFram.ClearAllBill,IapFlag.IdKey);
                   }
                }
            }
            break;
        default:
            break;
    }
}
#define CONFIG_LINE_NUM 5
#define CONFIG_NAME_SIZE 10
INT8U ConfigName[CONFIG_LINE_NUM][CONFIG_NAME_SIZE]={
"SN=",
"RstBill=",
"A=",
"B=",
"C="
};
INT8U ConfigNameLen[CONFIG_LINE_NUM]={3,8,2,2,2};
void ResetBillCheck(void)
{
	INT32U len=0;
	INT8U i=0,j=0,k;
    INT8U LineStart[CONFIG_LINE_NUM]={0};
	MMC_SELECT();
	f_mount(0,&fs);//注册一个文件系统
	res=f_open(&fil,"/Game/Config.txt",FA_OPEN_EXISTING|FA_READ);
	if(res!=FR_OK) 
	{
		return;
	}
    
    vTaskDelay(5);
    res = f_read(&fil, gFlashData, 100, &len);
    if (res || len == 0)
    {
        return;   
    }
    for(i=0;i<100;i++)
    {
        if(i>0)
        {
            if((gFlashData[i-1]==0x0d)&&(gFlashData[i]==0x0a))
            {//检查文件内有多少个回车符
                LineStart[++j]=i+1;//记录下一行的开始位置
            }
        }
    }
    
    for(k=0;k<j;k++)
    {   
        for(i=0;i<CONFIG_LINE_NUM;i++)
        {
            if(gFlashData[LineStart[k]]==ConfigName[i][0])
            {//对比参数的头一个字母
                if(StrSearchStr(&gFlashData[LineStart[k]],&ConfigName[i][0],ConfigNameLen[i]))
                {//名字一致
                    ConfigSet(i,&gFlashData[LineStart[k]+ConfigNameLen[i]]);
                    break;
                }
            }
        }
        
    }
	f_close(&fil);
	f_mount(0, NULL);
}

/******************************************************************************
 * 函数名:UpdateLKT4200FromSD
 * 说明: 
 *    更新LKT4200 
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 14:45:0, 2015年12月02日, 南下三五年
 * -----------------------------------
 *****************************************************************************/
INT8U DesKey[16]={
	0x19+1,0x83+2,0x84+3,0x68+4,0x11+5,0x74+6,0x20+7,0x92+8,
	0x30+9,0x80+10,0x42+11,0x00+12,0x31+13,0x99+14,0x75+15,0x19+16,
	};
INT8U TrueKey[16]={0};
void UpdateLKT4200FromSD(void)
{
	INT32U len=0;
	INT32U i=0;
	INT32U lkt4200ready=0;
	INT8U  Codekey=0,Codekey1=0;
	INT8U  First=0;
	//INT8U err=0;

	MMC_SELECT();
	f_mount(0,&fs);//注册一个文件系统
	res=f_open(&fil,"/Game/LKT4200.img",FA_OPEN_EXISTING|FA_READ);
    IapFlag.LoadFlash=1;
	if(res!=FR_OK) 
	{
		IapFlag.FindFile=1L<<File_NoFindLKT4200;
		return;
	}
    LOAD_RIGHT_LED();
	for(i=0;i<16;i++)
	{
		DesKey[i]=DesKey[i]-(i+1);
		DesKey[i]^=i;
		DesKey[i]^=i;
	}

	//IapFlag.FindProgram=File_LKT4200;
	IapFlag.LoadFlash=1;
	xSemaphoreTake( xLKT4200RecSemaphore, 0);
	for(;;)
	{
		vTaskDelay(5);
		
		if(lkt4200ready==0)
		{
			//!首先获取长度
			res = f_read(&fil, gFlashData, 10, &len);
			if (res || len == 0)
			{
				break;   
			} 
			//!获取实际数据
			res = f_read(&fil, (INT8U *)&gFlashData[10], ((gFlashData[8]-'0')*16+(gFlashData[9]-'0'))*2+11, &len);
			if (res || len == 0)
			{
				break;   
			} 
			//!将ASCII码转化为16进制
			for(i=0;i<(gFlashData[8]-'0')*16+(gFlashData[9]-'0')+5;i++)
			{
				if((gFlashData[2*i]>='0')&&(gFlashData[2*i]<='9'))
				{
					LKT4200.SendBuffer[i]=((gFlashData[2*i]-'0')<<4);
				}
				else
				{
					LKT4200.SendBuffer[i]=((gFlashData[2*i]-'7')<<4);
				}

				if((gFlashData[2*i+1]>='0')&&(gFlashData[2*i+1]<='9'))
				{
					LKT4200.SendBuffer[i]+=(gFlashData[2*i+1]-'0');
				}
				else
				{
					LKT4200.SendBuffer[i]+=(gFlashData[2*i+1]-'7');
				}
			}
			lkt4200ready=1;
		}
		//!发送数据给LKT4200
		if(First==0)
		{
			//!修改秘钥为新秘钥
			if(Codekey==0)
			{
				//!解密旧秘钥
				for(i=0;i<8;i++)
					TrueKey[i]=LKT4200.SendBuffer[5+i];
				TripDes(TrueKey,DesKey,DesKey+8,0);

				for(i=0;i<8;i++)
					LKT4200.SendBuffer[5+i]=TrueKey[i];

				//!解密新秘钥
				for(i=0;i<8;i++)
					TrueKey[i]=LKT4200.SendBuffer[13+i];
				TripDes(TrueKey,DesKey,DesKey+8,0);
				for(i=0;i<8;i++)
					LKT4200.SendBuffer[5+i+8]=TrueKey[i];
			}
			else
			{
				//!需要重启，否则不能正常升级
				ResetLKT4200();
				LKT4200.SendBuffer[4]=0x08;
				gFlashData[8]='0';
				gFlashData[9]='8';
				for(i=0;i<8;i++)
					LKT4200.SendBuffer[5+i]=LKT4200.SendBuffer[13+i];
			}

			LKT4200.SendDataToLKT=1;
			LKT4200.RecCnt=0;
			for(i=0;i<128;i++)
				LKT4200.RecBuffer[i]=0;			
			xSemaphoreTake( xLKT4200RecSemaphore, 0);
			portENTER_CRITICAL();
			for(i=0;i<5;i++)
			{
				UART4->DR = LKT4200.SendBuffer[i]; 
				while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
			}
			LKT4200.SendDataToLKT=0;
			portEXIT_CRITICAL();
		}
		else
		{
			if(LKT4200.SendBuffer[0]==0xf0)
			{
				LKT4200.SendDataToLKT=1;
				LKT4200.RecCnt=0;
				for(i=0;i<128;i++)
					LKT4200.RecBuffer[i]=0;
		
				xSemaphoreTake( xLKT4200RecSemaphore, 0);
				portENTER_CRITICAL();
				for(i=0;i<5;i++)
				{
					UART4->DR = LKT4200.SendBuffer[i]; 
					while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
				}
				LKT4200.SendDataToLKT=0;
				portEXIT_CRITICAL();
			}
			else
			{
				for(i=0;i<8;i++)
					TrueKey[i]=LKT4200.SendBuffer[9+i];
				
				TripDes(TrueKey,DesKey,DesKey+8,0);

				for(i=0;i<8;i++)
					LKT4200.SendBuffer[9+i]=TrueKey[i];

				for(i=0;i<8;i++)
					TrueKey[i]=LKT4200.SendBuffer[9+i+8];
				
				TripDes(TrueKey,DesKey,DesKey+8,0);

				for(i=0;i<8;i++)
					LKT4200.SendBuffer[9+i+8]=TrueKey[i];

				LKT4200.SendDataToLKT=1;//失能接收
				LKT4200.RecCnt=0;//计数清零
				for(i=0;i<128;i++)//清零接收器
					LKT4200.RecBuffer[i]=0;
		
				xSemaphoreTake( xLKT4200RecSemaphore, 0);
				portENTER_CRITICAL();
				for(i=0;i<5;i++)
				{
					UART4->DR = LKT4200.SendBuffer[i]; 
					while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
				}
				LKT4200.SendDataToLKT=0;
				portEXIT_CRITICAL();
			}
		}

		xSemaphoreTake( xLKT4200RecSemaphore, 1000);

		if(LKT4200.SendBuffer[0]==0xf0)
		{
			if((LKT4200.RecBuffer[0]==0xf6)||(LKT4200.RecBuffer[0]==0xf4))
			{
				//!算法校验码解码
				if((LKT4200.SendBuffer[2]==0x00)&&(LKT4200.SendBuffer[3]==0x02))
				{
					if(Codekey1==0)
					{
						for(i=0;i<8;i++)
							TrueKey[i]=LKT4200.SendBuffer[5+i];
						TripDes(TrueKey,DesKey,DesKey+8,0);

						for(i=0;i<8;i++)
							LKT4200.SendBuffer[5+i]=TrueKey[i];
					}
					Codekey1=1;
				}

				LKT4200.SendDataToLKT=1;
				LKT4200.RecCnt=0;
				for(i=0;i<128;i++)
					LKT4200.RecBuffer[i]=0;
				xSemaphoreTake( xLKT4200RecSemaphore, 0);
				portENTER_CRITICAL();
				for(i=0;i<(gFlashData[8]-'0')*16+(gFlashData[9]-'0');i++)
				{
					UART4->DR = LKT4200.SendBuffer[5+i]; 
					while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
				}
				LKT4200.SendDataToLKT=0;
				portEXIT_CRITICAL();

				xSemaphoreTake( xLKT4200RecSemaphore, 1000);

				if((LKT4200.RecBuffer[0]==0x90)&&(LKT4200.RecBuffer[1]==0x00))
				{
					lkt4200ready=0;
					First=1;

					//!情况发送缓冲区
					for(i=0;i<5+(gFlashData[8]-'0')*16+(gFlashData[9]-'0');i++)
						LKT4200.SendBuffer[i]=0;
				}
				else
				{
					Codekey=1;
				}
			}
		}
		else
		{
			if(LKT4200.RecBuffer[0]==0x08)
			{
				LKT4200.SendDataToLKT=1;
				LKT4200.RecCnt=0;
				for(i=0;i<128;i++)
					LKT4200.RecBuffer[i]=0;
				xSemaphoreTake( xLKT4200RecSemaphore, 0);
				portENTER_CRITICAL();
				for(i=0;i<(gFlashData[8]-'0')*16+(gFlashData[9]-'0');i++)
				{
					UART4->DR = LKT4200.SendBuffer[5+i]; 
					while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
				}
				LKT4200.SendDataToLKT=0;
				portEXIT_CRITICAL();

				xSemaphoreTake( xLKT4200RecSemaphore, 1000);

				if((LKT4200.RecBuffer[0]==0x61)&&(LKT4200.RecBuffer[1]==0x00))
				{
					lkt4200ready=0;
				}
			}
		}
	}
	
	f_close(&fil);
	f_mount(0, NULL);
}

//*_*这个函数有待优化
static
u8 SPI_ReadWrite_Byte(unsigned char byte)
{
    /* Loop while DR register in not emplty */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, byte);

    /* Wait to receive a byte */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);  
}

static
void SPI_LowSpeed(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1, &SPI_InitStructure);

    /* SPI1 enable */
    SPI_Cmd(SPI1, ENABLE);
}

static
void SPI_HighSpeed(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1, &SPI_InitStructure);

    /* SPI1 enable */
    SPI_Cmd(SPI1, ENABLE);
}
/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
BYTE Wait_Ready (void)
{
    BYTE res;

    Timer2 = 500;	/* Wait for ready in timeout of 500ms */
    SPI_ReadWrite_Byte(0xff);;
    do{
        res = SPI_ReadWrite_Byte(0xff);
    }while ((res != 0xFF) && Timer2);

    return res;
}

/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void SPI_Release(void)
{
    MMC_DESELECT();
    SPI_ReadWrite_Byte(0xff);;
}



/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */
/*-----------------------------------------------------------------------*/
//暂时取消电源管理
#define MMC_POWERON()    /*GPIO_ResetBits(GPIOD, GPIO_Pin_10)      */
#define MMC_POWEROFF()   /*GPIO_SetBits(GPIOD, GPIO_Pin_10)			*/

int chk_power(void)		/* Socket power state: 0=off, 1=on */
{
    return 1;
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
BOOL Receive_DataBlock(
        BYTE *buff,			/* Data buffer to store received data */
        UINT btr			/* Byte count (must be multiple of 4) */
        )
{
    BYTE token;
    
    Timer1 = 100;
    do {							/* Wait for data packet in timeout of 100ms */
        token = SPI_ReadWrite_Byte(0xff);
    } while ((token == 0xFF) && Timer1);
    if(token != 0xFE) return FALSE;	/* If not valid data token, retutn with error */

    do {							/* Receive the data block into buffer */
        *buff++ = SPI_ReadWrite_Byte(0xff);
    } while (btr--);
    SPI_ReadWrite_Byte(0xff);		/* Discard CRC */
    SPI_ReadWrite_Byte(0xff);

    return TRUE;					/* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
static
BOOL Transmit_DataBlock (
        const BYTE *buff,	/* 512 byte data block to be transmitted */
        BYTE token			/* Data/Stop token */
        )
{
    BYTE resp;
    UINT wc;


    if (Wait_Ready() != 0xFF) return FALSE;

    SPI_ReadWrite_Byte(token);			/* Transmit data token */
    if (token != 0xFD) {	            /* Is data token, 0xFD for stop token */
        wc = 512;
        do {							/* Transmit the 512 byte data block to MMC */
            SPI_ReadWrite_Byte(*buff++);
        } while (--wc);
        SPI_ReadWrite_Byte(0xFF);		/* CRC (Dummy) */
        SPI_ReadWrite_Byte(0xFF);
        resp = SPI_ReadWrite_Byte(0xff);/* Reveive data response */
        if ((resp & 0x1F) != 0x05)		/* If not accepted, return with error */
            return FALSE;
    }

    return TRUE;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/* Output: R1, R2, R3 response                                           */
/*         In idle mode R1 Response is 0x01                              */
/*         When the card is initialized successfuly, R1 response is 0x00 */
/*-----------------------------------------------------------------------*/

static
BYTE Send_Command(
        BYTE cmd,		/* Command byte */
        DWORD arg		/* Argument */
        )
{
    BYTE n, res;

    /* Is a ACMD<n>? */
    if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
        cmd &= 0x7F;
        res = Send_Command(CMD55, 0);
        if (res > 1) return res;
    }

    /* Select the card and wait for ready */
    MMC_DESELECT();
    MMC_SELECT();
    if (Wait_Ready() != 0xFF) return 0xFF;

    /* Send command packet */
    SPI_ReadWrite_Byte(cmd);
    SPI_ReadWrite_Byte((BYTE)(arg >> 24));		/* Argument[31..24] */
    SPI_ReadWrite_Byte((BYTE)(arg >> 16));		/* Argument[23..16] */
    SPI_ReadWrite_Byte((BYTE)(arg >> 8));		/* Argument[15..8] */
    SPI_ReadWrite_Byte((BYTE)arg);				/* Argument[7..0] */
    n = 0x01;							        /* Dummy CRC + Stop */
    if (cmd == CMD0) n = 0x95;			        /* Valid CRC for CMD0(0) */
    if (cmd == CMD8) n = 0x87;			        /* Valid CRC for CMD8(0x1AA) */
    SPI_ReadWrite_Byte(n);

    /* Receive command response */
    if (cmd == CMD12) { 
        /* The received byte immediataly following 
         * CMD12 is a stuff byte, it should be discarded
         * before receive the response of the CMD12. */
        SPI_ReadWrite_Byte(0xff);		    
    }

    /* Wait for a valid response in timeout of 10 attempts */
    n = 10;								
    do{
        res = SPI_ReadWrite_Byte(0xff);
    }while ((res & 0x80) && --n);

    return res;			/* Return with the response value */
}

/*--------------------------------------------------------------------------

  Public Functions

  ---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/
//在auto_mount()内调用
DSTATUS disk_initialize (
        BYTE drv		/* Physical drive nmuber (0) */
        )
{
    BYTE n, cmd, ty, ocr[4];


    if (drv) return STA_NOINIT;			    /* Supports only single drive */
    if (Stat & STA_NODISK) return Stat;	            /* No card in the socket */

    MMC_SPI_Config(); //初始化IO
   
    MMC_POWERON();				            /* 开电 */
    for(Timer1=500; Timer1; );              

    SPI_LowSpeed();    
    MMC_DESELECT();							//不选SD

	/* Wait for enter Idle state in timeout of 5000 msec */
    Timer1 = 5000;
    do{

        for (n = 10; n; n--) SPI_ReadWrite_Byte(0xff);	    /* 80 dummy clocks */
    }
    while((Send_Command(CMD0,0) != 1) && Timer1);

    ty = 0;
    Timer1 = 2000;			        /* Initialization timeout of 2000 msec */
    if (Send_Command(CMD8, 0x1AA) == 1) {	    /* 检查是否支持SDC Ver2 */
        for (n = 0; n < 4; n++) 
            ocr[n] = SPI_ReadWrite_Byte(0xff);	/* Get trailing return value of R7 resp */
        if (ocr[2] == 0x01 && ocr[3] == 0xAA) {			
            /* The card can work at vdd range of 2.7-3.6V */
            /* Wait for leaving idle state (ACMD41 with HCS bit) */
            while (Timer1 && Send_Command(ACMD41, 1UL << 30));  				
            if (Timer1 && Send_Command(CMD58, 0) == 0) {		
                /* Check CCS bit in the OCR */
                for (n = 0; n < 4; n++)
                    ocr[n] = SPI_ReadWrite_Byte(0xff);
                /* When CCS bit is set  R/W in block address insted of byte address */
                ty = (ocr[0] & 0x40) ? 12 : 4;
            }
        }
    } else {							
        /* SDSC or MMC */
        if (Send_Command(ACMD41, 0) <= 1) 	{           /* initialize successful will response 0x00 */
            ty = 2; cmd = ACMD41;	/* SDv1 */
        } else {
            ty = 1; cmd = CMD1;		/* MMC */
        }
        while (Timer1 && Send_Command(cmd, 0));			/* Wait for leaving idle state */
        if (!Timer1 || Send_Command(CMD16, 512) != 0)	/* Set R/W block length to 512 */
            ty = 0;
    }

    CardType = ty;	
    SPI_HighSpeed();
    SPI_Release();

    if (ty) {
        /* Initialization succeded */
        Stat &= ~STA_NOINIT;	
    //    USART1_Puts("Initialization succeded.\n");
    } else {			            
        /* Initialization failed */
        MMC_POWEROFF(); 
    //    USART1_Puts("Initialization failed.\n");
    }

    return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
        BYTE drv		/* Physical drive nmuber (0) */
        )
{
    if (drv) return STA_NOINIT;		/* Supports only single drive */
    return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
        BYTE drv,			/* Physical drive nmuber (0) */
        BYTE *buff,			/* Pointer to the data buffer to store read data */
        DWORD sector,		        /* Start sector number (LBA) */
        BYTE count			/* Sector count (1..255) */
        )
{
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    if (!(CardType & 8)) sector *= 512;	/* Convert to byte address if needed */

    if (count == 1) {	/* Single block read */
        if ((Send_Command(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
                && Receive_DataBlock(buff, 512))
            count = 0;
    }
    else {				/* Multiple block read */
        if (Send_Command(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
            do {
                if (!Receive_DataBlock(buff, 512)) break;
                buff += 512;
            } while (--count);
            Send_Command(CMD12, 0);				/* STOP_TRANSMISSION */
        }
    }
    SPI_Release();

    return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
        BYTE drv,			/* Physical drive nmuber (0) */
        const BYTE *buff,	/* Pointer to the data to be written */
        DWORD sector,		/* Start sector number (LBA) */
        BYTE count			/* Sector count (1..255) */
        )
{
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    if (!(CardType & 8)) sector *= 512;	/* Convert to byte address if needed */

    if (count == 1) {	/* Single block write */
        if ((Send_Command(CMD24, sector) == 0)	/* WRITE_BLOCK */
                && Transmit_DataBlock(buff, 0xFE))
            count = 0;
    }
    else {				/* Multiple block write */
        if (CardType & 6) Send_Command(ACMD23, count);
        if (Send_Command(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
            do {
                if (!Transmit_DataBlock(buff, 0xFC)) break;
                buff += 512;
            } while (--count);
            if (!Transmit_DataBlock(0, 0xFD))	/* STOP_TRAN token */
                count = 1;                      /* faild */
        }
    }
    SPI_Release();

    return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT disk_ioctl (
        BYTE drv,		/* Physical drive nmuber (0) */
        BYTE ctrl,		/* Control code */
        void *buff		/* Buffer to send/receive control data */
        )
{
    DRESULT res;
    BYTE n, csd[16], *ptr = buff;
    WORD csize;


    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (ctrl == CTRL_POWER) {
        switch (*ptr) {
            case 0:		/* Sub control code == 0 (POWER_OFF) */
                if (chk_power())
                    MMC_POWEROFF(); 		/* Power off */
                res = RES_OK;
                break;
            case 1:		/* Sub control code == 1 (POWER_ON) */
                MMC_POWERON();				/* Power on */
                res = RES_OK;
                break;
            case 2:		/* Sub control code == 2 (POWER_GET) */
                *(ptr+1) = (BYTE)chk_power();
                res = RES_OK;
                break;
            default :
                res = RES_PARERR;
        }
    }
    else {
        if (Stat & STA_NOINIT) return RES_NOTRDY;

        switch (ctrl) {
            case CTRL_SYNC :		/* Make sure that no pending write process */
                MMC_SELECT();
                if (Wait_Ready() == 0xFF)
                    res = RES_OK;
                break;

            case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
                if ((Send_Command(CMD9, 0) == 0) && Receive_DataBlock(csd, 16)) {
                    if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
                        csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                        *(DWORD*)buff = (DWORD)csize << 10;
                    } else {					/* SDC ver 1.XX or MMC*/
                        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                        csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                        *(DWORD*)buff = (DWORD)csize << (n - 9);
                    }
                    res = RES_OK;
                }
                break;

            case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
                *(WORD*)buff = 512;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
                if (CardType & 4) {			/* SDC ver 2.00 */
                    if (Send_Command(ACMD13, 0) == 0) {		/* Read SD status */
                        SPI_ReadWrite_Byte(0xff);
                        if (Receive_DataBlock(csd, 16)) {				/* Read partial block */
                            for (n = 64 - 16; n; n--) SPI_ReadWrite_Byte(0xff);	/* Purge trailing data */
                            *(DWORD*)buff = 16UL << (csd[10] >> 4);
                            res = RES_OK;
                        }
                    }
                } else {					/* SDC ver 1.XX or MMC */
                    if ((Send_Command(CMD9, 0) == 0) && Receive_DataBlock(csd, 16)) {	/* Read CSD */
                        if (CardType & 2) {			/* SDC ver 1.XX */
                            *(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
                        } else {					/* MMC */
                            *(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
                        }
                        res = RES_OK;
                    }
                }
                break;

            case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
                *ptr = CardType;
                res = RES_OK;
                break;

            case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
                if (Send_Command(CMD9, 0) == 0		/* READ_CSD */
                        && Receive_DataBlock(ptr, 16))
                    res = RES_OK;
                break;

            case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
                if (Send_Command(CMD10, 0) == 0		/* READ_CID */
                        && Receive_DataBlock(ptr, 16))
                    res = RES_OK;
                break;

            case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
                if (Send_Command(CMD58, 0) == 0) {	/* READ_OCR */
                    for (n = 4; n; n--) *ptr++ = SPI_ReadWrite_Byte(0xff);
                    res = RES_OK;
                }
                break;

            case MMC_GET_SDSTAT :	/* Receive SD statsu as a data block (64 bytes) */
                if (Send_Command(ACMD13, 0) == 0) {	/* SD_STATUS */
                    SPI_ReadWrite_Byte(0xff);
                    if (Receive_DataBlock(ptr, 64))
                        res = RES_OK;
                }
                break;

            default:
                res = RES_PARERR;
        }

    }
    
    SPI_Release();
    return res;
}
#endif /* _USE_IOCTL != 0 */

DWORD get_fattime (void)
{
    return 0;  
}
/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */

/*
   void disk_timerproc (void)
   {
   static BYTE pv;
   BYTE n, s;


   n = Timer1;						    // 100Hz decrement timer
   if (n) Timer1 = --n;
   n = Timer2;
   if (n) Timer2 = --n;

   n = pv;
   pv = SOCKPORT & (SOCKWP | SOCKINS);	// Sample socket switch 

   if (n == pv) {					    // Have contacts stabled? 
   s = Stat;

   if (pv & SOCKWP)			// WP is H (write protected) 
   s |= STA_PROTECT;
   else						// WP is L (write enabled) 
   s &= ~STA_PROTECT;

   if (pv & SOCKINS)			// INS = H (Socket empty) 
   s |= (STA_NODISK | STA_NOINIT);
   else						// INS = L (Card inserted) 
   s &= ~STA_NODISK;

   Stat = s;
   }
   }
   */

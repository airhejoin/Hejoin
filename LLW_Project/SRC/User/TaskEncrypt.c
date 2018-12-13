#include"includes.h"
#define HEAD1 0xcf
#define ENCRYPT 0x9d
#define SLAVERSIZE 8

S_COM EncryptRec;
xSemaphoreHandle xEncryptRecSemaphore;
u8 EncryptIndex=0;
u8 Sendbuf[12]={0};
//CRC16校验 CRC-16CCITT
//const u16 crctab[16]=
//{
//	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
//	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
//};

/******************************************************************************
 * 函数名:Crc16
 * 说明: 
 *    CRC16校验 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
//u16 Crc16Check(u8 *dat, u8 len)
//{
//	u16 crc=0;
//	u8 temp;
//	while(len--!=0)
//	{
//		temp=crc>>12;
//		crc<<=4;
//		crc^=crctab[temp^(*dat/16)];

//		temp=crc>>12;
//		crc<<=4;
//		crc^=crctab[temp^(*dat&0x0f)];
//		dat++;
//	}
//  return crc;
//}

/*************   函数名：Calculate   ************************
【功能】：对比加密芯片的计算结果
【参数】：加密芯片的计算结果
【返回】：对比结果
【说明】：
**************设计：David 2017-11-16 ***********************/
u8 Calculate(u8 *dat)
{
  u32 tmp1=0,tmp2=0,tmp3=0,tmp4=0,tmp5=0,tmp6=0;
  tmp1=((DevID[2]^0xc47cd8ba)&0xca2a1f83)&DevID[2];
  tmp3=(DevID[0]>>8)+(0xadf77692^DevID[0])&0x9fe34ad7;
  tmp2=((DevID[1]+0x3e78fc47)+0x9c37dc0b)^(DevID[1]<<8);
  
  tmp4=dat[0];
  tmp4|=dat[1]<<8;
  tmp4|=dat[2]<<16;
  tmp4|=dat[3]<<24;
  
  tmp5=dat[4];
  tmp5|=dat[5]<<8;
  tmp5|=dat[6]<<16;
  tmp5|=dat[7]<<24;
  
  tmp6=dat[8];
  tmp6|=dat[9]<<8;
  tmp6|=dat[10]<<16;
  tmp6|=dat[11]<<24;
  
  if((tmp1==tmp4)&&(tmp2==tmp5)&&(tmp3==tmp6))
  {
    return 1;
  }
  return 0;
}
/*************   函数名：DecryptData   ************************
【功能】：解密通信帧
【参数】：通信帧
【返回】：解密结果
【说明】：
**************设计：David 2017-11-16 ***********************/
u8 DecryptData(u8 *dat)
{
  u8 code=0,i=0;
  u8 len=0;
  u8 cmd=0;
  u16 crc1=0,crc2=0;
  if(dat[0]!=HEAD1)
  {
    return 0;
  }
  code=(dat[2]&0x03)|((dat[3]&0x03)<<2)|((dat[4]&0x03)<<4)|((dat[5]&0x03)<<6);//加密字
  //解密密文
    for(i=0;i<13;i++)
    {
      dat[6+i]^=code;
    }
    cmd=dat[6];
    crc1=Crc16(&dat[0],19);
    crc2=(dat[19]<<8)+dat[20];
    if(crc1==crc2)
    {
      if(Calculate(&dat[7]))
      {
          return 1;
      }
    }
  return 0;
}
/*************   函数名：EncryptSendPacket ************************
【功能】：加密数据发送函数
【参数】：
【返回】：无
【说明】：
**************设计：David 2017-11-16 ***********************/
void EncryptSendPacket(u8 *dat,u8 len)//0为广播，从机1~7
{
	u8 buf[20]={0};
	u8 i=0;
  u8 code=0;
	u16 crc=0;
  u32 ramdom=0;
	buf[0]=HEAD1;	        //帧头1
	buf[1]=rand()%255;				        //帧长度
  ramdom=rand();
  buf[2]=ramdom;
  buf[3]=ramdom>>8;
  buf[4]=ramdom>>16;
  buf[5]=ramdom>>24;
  buf[6]=ENCRYPT;
  code=(buf[2]&0x03)|((buf[3]&0x03)<<2)|((buf[4]&0x03)<<4)|((buf[5]&0x03)<<6);//加密字
  for(i=0;i<len;i++)
  {
    buf[7+i]=dat[i];
  }
  crc=Crc16(&buf[0],len+7);
  buf[7+i]=crc>>8;
  buf[8+i]=crc&0xff;
  //产生密文
	for(i=0;i<(len+1);i++)
	{
		buf[6+i]^=code;//cf 27 18 96 a7 1c 9b 59 db 7c为加密
	}
  //加密 cf 27 18 96 a7 1c a3 61 db 7c
  //发送数据
  taskENTER_CRITICAL(); 
  for(i=0;i<len+9;i++)
  {
    USART1->DR = buf[i]; 
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
  }
  taskEXIT_CRITICAL();
}
const u8 rDataTab[2][5]={
  {0xcf,0x7d,0x33,0xdb,0x1e},
  {0x59,0x82,0xc6,0x74,0x0d},
};
/*************   函数名：EncryptDelete ************************
【功能】：删除加密函数，创建云模块通信任务
【参数】：
【返回】：无
【说明】：
**************设计：David 2017-11-16 ***********************/
void EncryptDelete(void)
{
  EncryptIndex=2;
  Flag.EncryptPass=1;//验证成功
  USART_Cmd(USART1, DISABLE);            
  vTaskDelete(NULL);//删除任务  
}
/*************   函数名：EncryptTask ************************
【功能】：加密任务
【参数】：
【返回】：无
【说明】：
**************设计：David 2017-11-16 ***********************/
void EncryptTask( void *pvParameters )
{
  u8 sbuf[10]={0};
  u8 i=0;
  u8 times=0;
  for(;;)
  {
    if(EncryptIndex==0)
    {//
      xSemaphoreTake( xEncryptRecSemaphore, 0);
      Sendbuf[0]=DevID[0];
      Sendbuf[1]=DevID[0]>>8;
      Sendbuf[2]=DevID[0]>>16;
      Sendbuf[3]=DevID[0]>>24;
      Sendbuf[4]=DevID[1];
      Sendbuf[5]=DevID[1]>>8;
      Sendbuf[6]=DevID[1]>>16;
      Sendbuf[7]=DevID[1]>>24;      
      Sendbuf[8]=DevID[2];
      Sendbuf[9]=DevID[2]>>8;
      Sendbuf[10]=DevID[2]>>16;
      Sendbuf[11]=DevID[2]>>24;
      EncryptSendPacket((u8 *)&Sendbuf,12);
      if(pdTRUE==xSemaphoreTake( xEncryptRecSemaphore, 1000))
      {//信号量获取成功
        if(DecryptData(&EncryptRec.Msg[EncryptRec.MsgBegin][0]))
        {//数据验证通过
					if(System.EncryptErrCnt)
					{
						System.EncryptErrCnt=0;
						Flag.SaveBill=1;
					}
          EncryptIndex=1;
        }
        for(i=0;i<MAXMSGBUFFERLEN;i++)
          EncryptRec.Msg[EncryptRec.MsgBegin][i]=0;
        if( ++EncryptRec.MsgBegin >= MAXMSGSIZE)	
          EncryptRec.MsgBegin=0;		
      }
      if(++times>5)
      {//加密错误
        EncryptIndex=1;
        if(System.EncryptErrCnt==0)
        {
          System.EncryptErrCnt=1;
          Flag.SaveBill=1;
        }
      }
    }
    else
    {//验证完成
      EncryptDelete();
    }
  }
}
void EncryptInit(void)
{
  if(System.EncryptErrCnt)
  {
    //DrawFlagPoint();
    
		++System.EncryptErrCnt;   
    if(System.EncryptErrCnt>=100)
    {
      System.EncryptErrCnt=0;
      SystemParaDefault(SETDEFAULT_ALL);
      FM24WriteBuf(USER_ADDR,(u8 *)&System,sizeof(S_SYSTEM));//写入数据
      FLASH_Unlock();
      FLASH_EraseAllPages();
    }
    Flag.SaveBill=1; 
  }
}

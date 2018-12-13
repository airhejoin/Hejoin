#include ".\Lib\includes.h"
u8 LcdDisplayBuf[20]={0};
const u8 TFT_ErrChar[12][18]={
	"    ��Ͷ��������  ",
	"    ��Ͷ��������  ",
	" 1����Ʒ��΢������",
	" 2����Ʒ��΢������",
	" 3����Ʒ��΢������",
	" 4����Ʒ��΢������",
	" 5����Ʒ��΢������",
	"  1����Ʒ��ȱ��Ʒ ",
	"  2����Ʒ��ȱ��Ʒ ",
	"  3����Ʒ��ȱ��Ʒ ",
	"  4����Ʒ��ȱ��Ʒ ",
	"  5����Ʒ��ȱ��Ʒ ",
};

void MainDispProcess(void)
{
	u16 x=0,y=0,len=0;
//	if((System.Para.CoinPlay)||(Flag.MainPageLoad))
//	{
//		//ҳ��
//		if(Flag.MainPageLoad)
//			Flag.MainPageLoad=0;
//		if(Flag.PageIdex!=PAGE_MAIN)
//		{
//			Flag.MobilePayStaOld=0xff;
//			TFT32_DisplayPic(0,0,PAGE_MAIN);
//			Flag.PageIdex=PAGE_MAIN;
//		}
//		//����
//		if(Flag.CoinOld!=System.Play.CurCoins)
//		{
//			Flag.CoinOld=System.Play.CurCoins;
//			if(System.Play.CurCoins>9)//��ǰ����
//			{
//				LcdDisplayBuf[0]=System.Play.CurCoins/10+0x30;
//				LcdDisplayBuf[1]=System.Play.CurCoins%10+0x30;
//				x=265;y=85;len=2;
//			}
//			else
//			{
//				LcdDisplayBuf[0]=System.Play.CurCoins%10+0x30;
//				x=265;y=85;len=1;
//			}
//			TFT32_RefreshBlock(260,85,260,85,48,48,PAGE_MAIN);
//			TFT32_DispString(x,y,FONT32,0xffff,FONT_MODE_OVERLAY,len,LcdDisplayBuf);vTaskDelay(200);
//		}
//		
//		//����һ�洦��
//		if(Flag.CoinPlayOld!=System.Para.CoinPlay)
//		{
//			Flag.CoinPlayOld=System.Para.CoinPlay;
//			if(System.Para.CoinPlay>9)//����һ��
//			{
//				LcdDisplayBuf[0]=System.Para.CoinPlay/10+0x30;
//				LcdDisplayBuf[1]=System.Para.CoinPlay%10+0x30;
//				x=24;y=85;len=2;
//			}
//			else
//			{
//				LcdDisplayBuf[0]=System.Para.CoinPlay%10+0x30;
//				x=30;y=85;len=1;
//			}
//			TFT32_RefreshBlock(15,85,15,85,32,70,1);
//			TFT32_DispString(x,y,FONT32,0xffff,FONT_MODE_OVERLAY,len,LcdDisplayBuf);vTaskDelay(200);
//		}

//	}
//	else
//	{
//		Flag.CoinOld=0xff;
//		Flag.CoinPlayOld=0xff;
//		if(Flag.PageIdex!=PAGE_FREEMODE)
//		{
//			Flag.MobilePayStaOld=0;
//			TFT32_DisplayPic(0,0,PAGE_FREEMODE);
//			Flag.PageIdex=PAGE_FREEMODE;
//		}
//	}
//	//�ƶ�֧��ͼ��
//	if(Flag.MobilePayStaOld!=Flag.MobilePaySta)
//	{
//		Flag.MobilePayStaOld=Flag.MobilePaySta;
//		TFT32_DisplayPic(295,224,PAGE_GSMOFF+Flag.MobilePayStaOld);
//	}
}
void vDisplayTask( void *pvParameters )
{
	vTaskDelay(2000);
	TFT32_DisplayPic(0,0,PAGE_START);
  xTaskCreate( vGameTask,    ( signed char * ) "Display", 256,0, C_TASKPRIO_vDisplayTask,    NULL );
	xTaskCreate( vTftCtlTask,    ( signed char * ) "TftTask", 256,0, C_TASKPRIO_vTftCtlTask,    NULL );	
  for(;;)
  {
    vTaskDelay(5);
  }
}


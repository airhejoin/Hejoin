#include".\Lib\includes.h"

//�Ӷ���ʱģʽ
void vGameTask( void *pvParameters )
{
	u16 i=0;
  for(;;)
  {
		//�Ѷȵȼ� H M L
		switch(Flag.Step)
		{
			case STEP_IDLE:
				IdleLedRun();
				if(System.Play.CurCoins>=System.Para.CoinPlay)
				{
					Flag.StartLedBlink=1;
				}
				if(ReadKey(KEYID_SET))
				{
					Flag.DisplayType=DISPLAY_TYPE_SET;
					Flag.Step=STEP_SET;
					break;
				}
				start=0;
				if(System.Play.CurCoins>=System.Para.CoinPlay)
				{
					if((ReadKey(KEYID_START1)||ReadKey(KEYID_START2)))
					{
						KeyValueClear();
						Flag.AutoModeSta=0;//���ģʽ.insdex
						Flag.AutoModeDelay=0;
						System.Play.CurCoins-=System.Para.CoinPlay;//Coinlay
						Flag.SaveBill=1;
						start=1;				
					}
				}
				else
				{
					if(Flag.AutoModeDelay>=60000)
					{
						KeyValueClear();
						Flag.AutoModeDelay=0;
						Flag.AutoModeSta=1;//���ģʽ.insdex
						start=1;
					}
					else if(ReadKey(KEYID_START1)||ReadKey(KEYID_START2))
					{//��Ͷ�Ұ���Ϸ�Ϳ�������
						VoiceGameSendBreak(VOICE_PLEASECOIN);
						KeyValueClear();			
					}
				}
				if(start==1)
				{
					start=0;
					Flag.ClockSecSta=1;
					Flag.StartLedBlink=0;
					Flag.ResultCheat=0;
					Flag.ResultPrize=0;
					Flag.ResultFail=0;
					Start1LedOn();
					Start2LedOn();
					if(Flag.AutoModeSta==0)
					{//ѡ����Ʒ
						Flag.Time10SCnt=0;
						Flag.ClockPointer=0;
						Flag.ClockLedSta=0;
						
						Flag.Step=STEP_SELGIFT;
						Flag.GiftId=0;
						Flag.GiftLedSta=0x01;
						SoundGame.BegIndex=0;
						SoundGame.EndIndex=0;
						VoiceGameStop();
						VoiceBackSendBreak(VOICE_TOSELGIFT);//��ѡ����Ʒ��							
						TFT32_DisplayPic(0,0,PAGE_SELGIFT);//ѡ����Ʒ
						TFT32_DispString(40,10,FONT32,0xffff,FONT_MODE_OVERLAY,12,"��ǰ��Ʒ��: ");
						TFT32_DispNum(224,10,FONT32,0xffff,ALIGN_LEFT|FONT_MODE_OVERLAY,Flag.GiftId+1,1);							
						vTaskDelay(1000);
						Flag.Delay1sCnt=0;
						Flag.GiftSelTime=9;			
						KeyValueClear();
					}
					else
					{//��ʾģʽ
						Flag.Step=STEP_READY;
					}
          Flag.TftErrorDisplay=0;
				}
				break;
				case STEP_SELGIFT:
					if(ReadKey(KEYID_START1))
					{
						VoiceBackSendBreak(VOICE_FEIDAO);
						if(++Flag.GiftId>4)
							Flag.GiftId=0;
						TFT32_RefreshBlock(224,10,224,10,32,32,2);//�����ʾ
						TFT32_DispNum(224,10,FONT32,0xffff,ALIGN_LEFT|FONT_MODE_OVERLAY,Flag.GiftId+1,1);//��ʾ��Ʒ��
						Flag.GiftLedSta=1<<Flag.GiftId;//������ƷID
					}
					else if(ReadKey(KEYID_START2)||(Flag.GiftSelTime==0))
					{//ȷ����Ʒ
						TFT32_RefreshBlock(40,10,40,10,224,32,2);
						TFT32_DispString(20,10,FONT32,0xffff,FONT_MODE_OVERLAY,18,"��Ʒȷ�ϣ�׼����ʼ");
						Flag.GiftSelEn=DISABLE;
						Flag.Step=STEP_READY;
					}
					//GiftBarLedProcess();
					break;

			case STEP_GAME:///�������ȣ���ʾ����Ʒ��������ʾ��Ʒ��
				break;
			case STEP_OUTGIFT:
				if(((GiftBar.Enable&(1<<Flag.GiftId))==0)||(Flag.Error&(1L<<(Flag.GiftId+1))))//0xffc))
				{//����Ʒ���
					if((Flag.Error&0xffc)==0)
					{//������Ʒ����
						TFT32_RefreshBlock(20,10,20,10,300,32,2);
						TFT32_DispString(20,10,FONT32,0xffff,FONT_MODE_OVERLAY,18,"����Ʒ�ɹ�����ȡ��Ʒ");
					}
					
					Flag.SetDisplayIdle=1;
					Flag.PrizeBlinkSta=0;
					Flag.ClockLedSta=0;
					Flag.GiftLedSta=0xff;
					//GiftBarLedProcess();
					vTaskDelay(2000);
					VoiceBackSendOrder(VOICE_BACK2);	
					if(System.Para.CoinPlay)
						TFT32_DisplayPic(0,0,PAGE_MAIN);
					else
						TFT32_DisplayPic(0,0,PAGE_FREEMODE);
					Flag.Step=STEP_IDLE;
					vTaskDelay(100);
					Flag.CoinPlayOld=0xff;
					Flag.CoinOld=0xff;
					Flag.MobilePayStaOld=0xff;
					Flag.ClockRunEn=DISABLE;
					vTaskDelay(500);
					KeyValueClear();
				}
				break;
			case STEP_OVER:
				Start1LedOff();
				Start2LedOff();
				if(Flag.GameOverDelay)
				{
					if(Flag.GameOverDelay<500)
					{
						if(ReadKey(KEYID_START1)||ReadKey(KEYID_START2))
						{
							if(System.Para.CoinPlay)
								TFT32_DisplayPic(0,0,PAGE_MAIN);
							else
								TFT32_DisplayPic(0,0,PAGE_FREEMODE);
							Flag.CoinPlayOld=0xff;
							Flag.CoinOld=0xff;
							Flag.MobilePayStaOld=0xff;
							Flag.SetDisplayIdle=1;
							Flag.Time10SCnt=0;
							Flag.GameOverDelay=0;
							Flag.Step=STEP_IDLE;
							Flag.ClockRunEn=DISABLE;
							Flag.PrizeBlinkSta=0;
							Flag.GiftLedSta=0xff;
							//GiftBarLedProcess();
							vTaskDelay(1000);
							KeyValueClear();
						}
					}
				}
				else
				{
					if(System.Para.CoinPlay)
						TFT32_DisplayPic(0,0,PAGE_MAIN);
					else
						TFT32_DisplayPic(0,0,PAGE_FREEMODE);
					Flag.CoinPlayOld=0xff;
					Flag.CoinOld=0xff;
					Flag.MobilePayStaOld=0xff;
					Flag.SetDisplayIdle=1;
					Flag.PrizeBlinkSta=0;
					Flag.Time10SCnt=0;
					Flag.ClockRunEn=DISABLE;
					Flag.Step=STEP_IDLE;
					Flag.GiftLedSta=0xff;
					//GiftBarLedProcess();
					vTaskDelay(500);
					KeyValueClear();
				}
				//GiftBarLedProcess();
				break;
			case STEP_SET:
				//GiftBarLedProcess();
				break;
		}
		VoicePlayProcess();
		vTaskDelay(10);
  }
}



























#include ".\User\Set.h"
#include<string.h> 
void BackMainSet(void);
#define BILLITEM_SUM (SET_CLEARBILL-SET_CLEARLED-1)

u8 tet[1]={16};
const u8 Arrow=16;
S_SET Set;

typedef struct {
	u8 Index;//索引
  u16 ValMin;
  u16 ValMax;
}S_KEYTAB;
const u16 SetValueLimit[6][2]={{0,99},{1,250},{0,4},{0,31},{0,31},{1,5}};

typedef struct {
	u16 DispX;
	u16 DispY;
  u16 ValMin;
  u16 ValMax;
	u8 	PicIdex;
	u8  NumSize;	//数字长度
	u8  AlignType;//对齐方式，0居中，1靠左，2靠右
	//u32 Value;
}S_SETNUM;
S_SETNUM SetNum;
u8 SetNumProcess(u32 *value)//数字设置
{
	u8 change=0;
	if(ReadKey(KEYID_DEC))
	{//减
		if(*value>SetNum.ValMin)
		{
			(*value)--;
			change=1;
		} 
	}
	else if(ReadKey(KEYID_ADD))
	{//上加
		if(*value<SetNum.ValMax)
		{
			(*value)++;
			change=1;
		}
	}
	else if(SignIn.PressCount[KEYID_DEC]>300)
	{//减长按
		if(*value>(SetNum.ValMin+10))
		{
			(*value)-=10;
			change=1;
		}
		else
		{
			*value=SetNum.ValMin;  
			change=1;
		}
	}
	else if((SignIn.PressCount[KEYID_ADD]>300))
	{//加长按
		if((*value+10)<SetNum.ValMax)
		{//长按加
			(*value)+=10;
			change=1;
		}
		else
		{
			*value=SetNum.ValMax;
			change=1;				
		}
	}
	if(change)
	{
		TFT32_RefreshBlock(SetNum.DispX,SetNum.DispY,SetNum.DispX,SetNum.DispY,16*SetNum.NumSize,32,SetNum.PicIdex);
		TFT32_DispNum(SetNum.DispX,SetNum.DispY,FONT32,RED,SetNum.AlignType|FONT_MODE_OVERLAY,*value,SetNum.NumSize);

	}
	return change;
}

typedef struct {
	u16 DispX;
	u16 DispY;
	u16 SpaceY;		//竖坐标
	u8 PagePic[5];//图片ID
	u8 Idex;			//当前索引号
  u8 IdexMax;		//最大索引号
	u8 PageIdex;	//当前图片索引
	u8 PageIdexNum;//一页有多少个索引
}S_SETARROWS;
S_SETARROWS SetArrows;
u8 SetArrowsProcess(u8 *arrows_idex)//数字设置
{
	u8 change=0;
	if(ReadKey(KEYID_ADD))
	{//上
		SetArrows.PageIdex=*arrows_idex/SetArrows.PageIdexNum;//当前界面
		if(SetArrows.PageIdex)
		{//不为第一页
			if((*arrows_idex%SetArrows.PageIdexNum)==0)
			{//在当前页的第一项，需要翻页
				(*arrows_idex)--;
				SetArrows.PageIdex=*arrows_idex/SetArrows.PageIdexNum;//计算界面
				TFT32_DisplayPic(0,0,SetArrows.PagePic[SetArrows.PageIdex]);//重新加载界面
				change=1;
			}
			else
			{//不在当前页第一项
				TFT32_RefreshBlock(SetArrows.DispX,SetArrows.DispY-((*arrows_idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),SetArrows.DispX,\
					SetArrows.DispY-((*arrows_idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),22,20,SetArrows.PagePic[SetArrows.PageIdex]);
				(*arrows_idex)--;
				change=1;
			}
		}
		else
		{//在第一页
			if(*arrows_idex)
			{//不在第一项
				TFT32_RefreshBlock(SetArrows.DispX,SetArrows.DispY-((*arrows_idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),SetArrows.DispX,\
					SetArrows.DispY-((*arrows_idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),22,20,SetArrows.PagePic[SetArrows.PageIdex]);
				(*arrows_idex)--;
				change=1;
			}
			else
			{
				*arrows_idex=SetArrows.IdexMax-1;
				SetArrows.PageIdex=*arrows_idex/SetArrows.PageIdexNum;//计算界面
				TFT32_DisplayPic(0,0,SetArrows.PagePic[SetArrows.PageIdex]);//重新加载界面
				change=1;
			}
		}
	}    
	else if(ReadKey(KEYID_DEC))
	{//减下
		if(*arrows_idex<(SetArrows.IdexMax-1))
		{
			if((*arrows_idex%SetArrows.PageIdexNum)==(SetArrows.PageIdexNum-1))
			{//需要翻页
				(*arrows_idex)++;
				SetArrows.PageIdex=*arrows_idex/SetArrows.PageIdexNum;//计算界面
				TFT32_DisplayPic(0,0,SetArrows.PagePic[SetArrows.PageIdex]);//重新加载界面
				change=1;
			}
			else
			{
				TFT32_RefreshBlock(SetArrows.DispX,SetArrows.DispY-((*arrows_idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),SetArrows.DispX,\
				SetArrows.DispY-((*arrows_idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),22,20,SetArrows.PagePic[SetArrows.PageIdex]);
				(*arrows_idex)++;
				change=1;
			}
		}
		else
		{
			*arrows_idex=0;
			SetArrows.PageIdex=*arrows_idex/SetArrows.PageIdexNum;//计算界面
			TFT32_DisplayPic(0,0,SetArrows.PagePic[SetArrows.PageIdex]);//重新加载界面
			change=1;
		}
	}  
	if(change)
	{
		TFT32_DisplayPic(SetArrows.DispX,SetArrows.DispY-((*arrows_idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),PAGE_ARROWS);
	}
	return change;
}
#if 0
void SetProcess(void)
{
  u8 change=0;
	u8 buf[20]={0};
	u8 i=0;
  u32 *ptr=(u32 *)&SetPara;
  u32 *sptr=(u32 *)&System.Para;
	//游戏中禁止进入使用时在此加限制
	if(Set.Enable==0)
	{
    Set.Enable=1;//进入设置模式
    SignIn.Value=0;
    Set.OldIndex=0;
    memcpy((u8 *)&SetPara,(u8 *)&System.Para,sizeof(S_BACKPARA));//将数据复制给SetPara
		SetPara.GiftBarChk=1;
    BackMainSet();

		SetArrows.DispX=45;
		SetArrows.DispY=174;
		SetArrows.PagePic[0]=PAGE_SET1;
		SetArrows.PagePic[1]=PAGE_SET2;
		SetArrows.Idex=0;
		SetArrows.IdexMax=SET_MAX;
		SetArrows.PageIdex=0;
		SetArrows.PageIdexNum=6;
		SetArrows.SpaceY=30;
		
    return;
	}
  //一级菜单
  if(Set.Level==0)
  {
    if(ReadKey(KEYID_SET))
    {//设置键被按下
			Flag.ArrowsChange=0;
			if(Set.CurIndex<=SET_GIFTPROB)
			{
				TFT32_DisplayPic(0,0,PAGE_COINPLAY+Set.CurIndex);
			}  
			else if(Set.CurIndex!=SET_EXIT)
			{
				TFT32_DisplayPic(0,0,PAGE_COINPLAY+Set.CurIndex+1);
			}
      Set.Item=Set.CurIndex;
      Set.OldIndex=Set.CurIndex;
      Set.CurIndex=0;
      if((Set.Item!=SET_BILLBROWSE)&&(Set.Item!=SET_EXIT))
      {//在查账之前
        memcpy((u8 *)&SetPara,(u8 *)&System.Para,sizeof(S_BACKPARA));
        SetPara.ClearBill=0;
        SetPara.Default=0;
        Set.Level=1;//进入二级菜单
        if(Set.Item==SET_VERSION)
				{//版本信息
					TFT32_DispString(40,160,FONT24,0xffff,FONT_MODE_OVERLAY,18,"游戏名称：挑战一刻");
					TFT32_DispString(40,128,FONT24,0xffff,FONT_MODE_OVERLAY,11,"软件版本：V");
					TFT32_DispNum(172,128,FONT24,0xffff,ALIGN_LEFT|FONT_MODE_OVERLAY,System.AppVersion,3);
					TFT32_DispString(40,96,FONT24,0xffff,FONT_MODE_OVERLAY,18,"  制造商：大智动漫");	
				}
				else if(Set.Item==SET_GIFTBARCHK)
				{//礼品杆测试
					Set.DispType=SETTYPE_ARROWS;
					SetArrows.DispX=67;
					SetArrows.DispY=155;
					SetArrows.PagePic[0]=PAGE_GIFTBAR;
					SetArrows.Idex=0;
					SetArrows.IdexMax=5;
					SetArrows.PageIdex=0;
					SetArrows.PageIdexNum=5;
					SetArrows.SpaceY=30;
					TFT32_DisplayPic(SetArrows.DispX,SetArrows.DispY,PAGE_ARROWS);
					GiftBarSetRun(SetArrows.Idex);
					Flag.GiftLedSta=1<<(SetArrows.Idex);
				}
				else if(Set.Item==SET_GIFTPROB)
				{//礼品杆概率
					Set.Level=4;//进入二级菜单
					Set.DispType=SETTYPE_ARROWS;
					SetArrows.DispX=67;
					SetArrows.DispY=165;
					SetArrows.PagePic[0]=PAGE_GIFTPROBSEL;
					SetArrows.Idex=0;
					SetArrows.IdexMax=6;
					SetArrows.PageIdex=0;
					SetArrows.PageIdexNum=6;
					SetArrows.SpaceY=30;
					TFT32_DisplayPic(SetArrows.DispX,SetArrows.DispY,PAGE_ARROWS);
				}
        else if(Set.Item==SET_HARDLEVEL)
        {
          Set.DispType=SETTYPE_ARROWS;
					SetArrows.DispX=87;
					SetArrows.DispY=129;
					SetArrows.PagePic[0]=PAGE_HARDLEVEL;
					SetArrows.Idex=SetPara.HardLevel;
					SetArrows.IdexMax=3;
					SetArrows.PageIdex=0;
					SetArrows.PageIdexNum=3;
					SetArrows.SpaceY=30;
					TFT32_DisplayPic(SetArrows.DispX,SetArrows.DispY-SetPara.HardLevel*30,PAGE_ARROWS);
        }
        else if(Set.Item==SET_TIMERANGE)
        {
          Set.DispType=SETTYPE_ARROWS;
					SetArrows.DispX=23;
					SetArrows.DispY=142;
					SetArrows.PagePic[0]=PAGE_TIMERANGE;
					SetArrows.Idex=SetPara.PrizeTimesRange;
					SetArrows.IdexMax=4;
					SetArrows.PageIdex=0;
					SetArrows.PageIdexNum=4;
					SetArrows.SpaceY=29;
					TFT32_DisplayPic(SetArrows.DispX,SetArrows.DispY-SetPara.PrizeTimesRange*29,PAGE_ARROWS);
        }
				if(Set.Item>=SET_CLEARBILL)
        {//清账或者恢复出厂
					if(Set.Item!=SET_VERSION)
					{
						Set.DispType=SETTYPE_ARROWS;
						Set.Level=1;//进入二级菜单
						SetArrows.DispX=120;
						SetArrows.DispY=97;
						SetArrows.PagePic[0]=PAGE_COINPLAY+Set.Item+1;
						SetArrows.Idex=1;
						SetArrows.IdexMax=2;
						SetArrows.PageIdex=0;
						SetArrows.PageIdexNum=2;
						SetArrows.SpaceY=35;
						TFT32_DisplayPic(SetArrows.DispX,SetArrows.DispY-35,PAGE_ARROWS);
						
					}
        }
				else if((Set.Item!=SET_GIFTBARCHK)&&(Set.Item!=SET_GIFTPROB)&&(Set.Item!=SET_HARDLEVEL)&&(Set.Item!=SET_TIMERANGE))
				{
					if((Set.Item==SET_GAMEVOL)||(Set.Item==SET_BACKVOL))
					{	
						SetNum.DispX=205;
						SetNum.DispY=100;						
					}
					else
					{
						SetNum.DispX=110;
						SetNum.DispY=100;					
					}
					
					if(Set.Item<SET_GIFTPROB)
					{
						SetNum.PicIdex=PAGE_COINPLAY+Set.Item;//当前界面
					}  
					else 
					{
						SetNum.PicIdex=PAGE_COINPLAY+Set.Item+1;//当前界面
					}
					
					Set.DispType=SETTYPE_NUM;
					SetNum.AlignType=ALIGN_CENTER;//居中显示
					SetNum.NumSize=2;//最大两位小数
					SetNum.ValMin=SetValueLimit[Set.Item][0];
					SetNum.ValMax=SetValueLimit[Set.Item][1];
          //SetNum.ValMin=KeyTab[Set.Item].ValMin;
					//SetNum.ValMax=KeyTab[Set.Item].ValMax;
					if((Set.Item==SET_GAMEVOL)||(Set.Item==SET_BACKVOL)||(Set.Item==SET_HARDLEVEL)||(Set.Item==SET_TIMERANGE))
					{	
						TFT32_DispNum(SetNum.DispX,SetNum.DispY,FONT32,RED,SetNum.AlignType|FONT_MODE_OVERLAY,*(ptr+Set.Item+4),2);					
					}
					else
					{
						TFT32_DispNum(SetNum.DispX,SetNum.DispY,FONT32,RED,SetNum.AlignType|FONT_MODE_OVERLAY,*(ptr+Set.Item),2);				
					}	
				}
      }
      else if(Set.Item==SET_BILLBROWSE)
      {//查账模式
        Set.Level=1;//查账菜单      
        ptr=(u32 *)&System.Bill.TotalCoins;
				SetNum.PicIdex=0;
				TFT32_DispNum(195,163,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalCoins,6);
				TFT32_DispNum(195,133,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[0],6);	
				TFT32_DispNum(195,103 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[1],6);	
				TFT32_DispNum(195,73 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[2],6);
				TFT32_DispNum(195,43 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[3],6);	
				TFT32_DispNum(195,13 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[4],6);
      }
      else 
      {//直接退出
        Set.Enable=0;
				if(System.Para.CoinPlay)
				{
					TFT32_DisplayPic(0,0,PAGE_MAIN);
					Flag.DisplayType=DISPLAY_TYPE_NORMAL;
					vTaskDelay(50);
					Flag.CoinOld=0xff;
					Flag.CoinPlayOld=0xff;
					Flag.CoinPlayOld=0xff;
					Flag.MobilePayStaOld=0xff;
					Flag.MainPageLoad=1;
					MainDispProcess();
					//vTaskDelay(10);					
				}
				else
				{
					TFT32_DisplayPic(0,0,PAGE_FREEMODE);
					Flag.DisplayType=DISPLAY_TYPE_NORMAL;
					vTaskDelay(200);
				}
				Flag.Step=STEP_IDLE;
      }
      KeyValueClear(); 			
    }
		else
		{
			change=SetArrowsProcess(&Set.CurIndex);
		}
    return ;
  }  
  
  //二级菜单
  if(Set.Level==1)
  {
		if(Set.Item<SET_CLEARBILL) 
    {

			if(Set.Item!=SET_GIFTPROB)
			{
				if(Set.Item>SET_GIFTPROB)
					ptr+=(Set.Item+4);
				else
					ptr+=Set.Item;
			}
			else 
			{
				ptr+=Set.Item+SetArrows.Idex;
			}
    }
    else if(Set.Item!=SET_VERSION)
    {
      ptr+=Set.Item-1;
    }
		if(ReadKey(KEYID_SET))
		{
      if(Set.Item>=SET_GIFTBARCHK)
      {//清账或者恢复出厂
        if((Set.Item==SET_CLEARBILL)&&(SetArrows.Idex==0))
        {//清账
          SetPara.ClearBill=0;
          SystemParaDefault(SETDEFAULT_BILL);        
        }
        else if((Set.Item==SET_DEFAULT)&&(SetArrows.Idex==0))
        {//恢复出厂
          SetPara.Default=0;
          SystemParaDefault(SETDEFAULT_BILL_PARA);
        }
				else if(Set.Item==SET_GIFTBARCHK)
				{
					GiftBarSetAllStop();
					Flag.GiftLedSta=0x1f;
				}
        memcpy((u8 *)&SetPara,(u8 *)&System.Para,sizeof(S_BACKPARA));//将数据复制给SetPara
        Flag.SaveBill=1;//保存数据
        BackMainSet();
      }    
			else //if(Set.Item!=SET_GIFTPROB)
			{
				Set.Level=2;//进入三级菜单   
				Set.CurIndex=0;
				TFT32_DisplayPic(50,50,PAGE_EXIT);			
				TFT32_DisplayPic(70,117,PAGE_ARROWS);
			}
      KeyValueClear();
		}
		else
		{
			if(Set.Item!=SET_BILLBROWSE)
			{
				if(Set.DispType==SETTYPE_NUM)
				{//设置数值模式
					change=SetNumProcess(ptr);
				}
				else
				{
					change=SetArrowsProcess(&SetArrows.Idex);
				}
			}
			else
			{
				if(ReadKey(KEYID_ADD))
				{//上
					if(SetNum.PicIdex==1)
					{
						SetNum.PicIdex=0;
						TFT32_DisplayPic(0,0,PAGE_CURBILL);	
						TFT32_DispNum(195,163,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalCoins,6);
						TFT32_DispNum(195,133,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[0],6);	
						TFT32_DispNum(195,103 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[1],6);	
						TFT32_DispNum(195,73 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[2],6);
						TFT32_DispNum(195,43 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[3],6);	
						TFT32_DispNum(195,13 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Bill.TotalGift[4],6);
					}
				}    
				else if(ReadKey(KEYID_DEC))
				{//减下
					if(SetNum.PicIdex==0)
					{
						SetNum.PicIdex=1;
						TFT32_DisplayPic(0,0,PAGE_TOTALBILL);	
						TFT32_DispNum(195,163,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Backup.TotalCoins,6);
						TFT32_DispNum(195,133,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Backup.TotalGift[0],6);	
						TFT32_DispNum(195,103 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Backup.TotalGift[1],6);	
						TFT32_DispNum(195,73 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Backup.TotalGift[2],6);
						TFT32_DispNum(195,43 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Backup.TotalGift[3],6);	
						TFT32_DispNum(195,13 ,FONT32,WHITE,ALIGN_LEFT|FONT_MODE_OVERLAY,System.Backup.TotalGift[4],6);
					}
				}
			}
		}
		
    if(change==1)
    {
      if((Set.Item==SET_GAMEVOL)||(Set.Item==SET_BACKVOL))
      {
        VoiceBackSetSound(SetPara.BackVol);
				VoiceGameSetSound(SetPara.GameVol);
      }   
			else if(Set.Item==SET_GIFTBARCHK)
			{
				GiftBarSetAllStop();
				GiftBarSetRun(SetArrows.Idex);
				Flag.GiftLedSta=1<<(SetArrows.Idex);
				//TFT32_DisplayPic(Set.NumX,Set.NumY-(*ptr)*30+30,PAGE_ARROWS);
			}
//      else if((Set.Item==SET_HARDLEVEL)||(Set.Item==SET_TIMERANGE))
//      {
//        *ptr=SetArrows.Idex;
//      }
    } 
    return;
  }
  
  //三级菜单
  if(Set.Level==2)
  {
    if(ReadKey(KEYID_SET))
    {//设置键被按下
      if(Set.CurIndex==1)
      {//保存数据 
        if((Set.Item==SET_HARDLEVEL)||(Set.Item==SET_TIMERANGE))
        {
          if(Set.Item==SET_HARDLEVEL)
            SetPara.HardLevel=SetArrows.Idex;
          else
            SetPara.PrizeTimesRange=SetArrows.Idex;
        }
				memcpy((u8 *)&System.Para,(u8 *)&SetPara,sizeof(S_BACKPARA));//将数据复制给SetPara
        Flag.SaveBill=1;//保存数据
      }
      else
      {
        memcpy((u8 *)&SetPara,(u8 *)&System.Para,sizeof(S_BACKPARA));//将数据复制给SetPara
        Flag.SaveBill=1;//保存数据
      }
      if((Set.Item==SET_GAMEVOL)||(Set.Item==SET_BACKVOL))
      {
        VoiceBackSetSound(System.Para.BackVol);
				VoiceGameSetSound(System.Para.GameVol);
      }
			if(Set.Item!=SET_GIFTPROB)
				BackMainSet();
			else
			{
				Set.Level=4;
				TFT32_DisplayPic(0,0,PAGE_GIFTPROBSEL);
				TFT32_DisplayPic(SetArrows.DispX,SetArrows.DispY-((SetArrows.Idex%SetArrows.PageIdexNum)*SetArrows.SpaceY),PAGE_ARROWS);
			}	
    }
		else if(ReadKey(KEYID_DEC))
    {//减下
      Set.CurIndex=1;
			TFT32_RefreshBlock(70,117,20,67,22,20,PAGE_EXIT);
			TFT32_DisplayPic(70,117-35,PAGE_ARROWS);
			
    }
    else if(ReadKey(KEYID_ADD))
    {//上加
      Set.CurIndex=0;
			TFT32_RefreshBlock(70,117-35,20,67,22,20,PAGE_EXIT);
			TFT32_DisplayPic(70,117,PAGE_ARROWS);
    
    return;    
  }

  //三级菜单 查账
  if(Set.Level==3)
  {
		ptr=(u32 *)&System.Bill.TotalCoins;
    if(ReadKey(KEYID_SET))
    {//设置键被按下
      BackMainSet();
    }
    else if(ReadKey(KEYID_ADD))
    {
      if(Set.CurIndex>0)
        Set.CurIndex--;
    }
    else if(ReadKey(KEYID_DEC))
    {
      if(Set.CurIndex<(SetValueLimit[Set.Item][1]))
        Set.CurIndex++;
    }
    return;    
  }
	
	if(Set.Level==4)
	{//设置中奖概率
		if(ReadKey(KEYID_SET))
		{
			if(SetArrows.Idex<5)
			{
				Set.Level=1; 
				TFT32_DisplayPic(0,0,PAGE_SETGIFTPROB);
				vTaskDelay(50);
				TFT32_DisplayPic(51,203,PAGE_SETPROB1+SetArrows.Idex);
				Set.DispType=SETTYPE_NUM;
				SetNum.DispX=103;
				SetNum.DispY=100;
				SetNum.NumSize=3;
				SetNum.PicIdex=PAGE_SETGIFTPROB;//当前界面
				SetNum.ValMin=SetValueLimit[1][0];
				SetNum.ValMax=SetValueLimit[1][1];
				TFT32_DispNum(SetNum.DispX,SetNum.DispY,FONT32,RED,ALIGN_CENTER|FONT_MODE_OVERLAY,SetPara.RoundToPrize[SetArrows.Idex],SetNum.NumSize);
			}
			else
			{
				BackMainSet();
			}
      KeyValueClear();
		}
		else
		{
			change=SetArrowsProcess(&SetArrows.Idex);
		}
    return;
	}
}
void BackMainSet(void)
{
  Set.Level=0;//返回主菜单
  Set.Item=0;//返回主菜单  
  Set.CurIndex=Set.OldIndex;
	if(Set.CurIndex>5)
	{
		TFT32_DisplayPic(0,0,PAGE_SET2);
	}
	else
	{
		TFT32_DisplayPic(0,0,PAGE_SET1);
	}
	TFT32_DisplayPic(45,174-((Set.CurIndex%6)*30),PAGE_ARROWS);
	
	SetArrows.DispX=45;
	SetArrows.DispY=174;
	SetArrows.PagePic[0]=PAGE_SET1;
	SetArrows.PagePic[1]=PAGE_SET2;
	SetArrows.Idex=Set.CurIndex;
	SetArrows.IdexMax=SET_MAX;
	SetArrows.PageIdex=Set.CurIndex/6;
	SetArrows.PageIdexNum=6;
	SetArrows.SpaceY=30;
	
	KeyValueClear();
}
#endif

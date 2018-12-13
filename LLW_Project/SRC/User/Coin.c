#include ".\Lib\includes.h"

S_COIN CoinA,CoinB;

S_TICKET TicketA,TicketB;
void CoinInit(void)
{
#if COIN_SELECT
	CoinA.NormSta=COININ_NOR;
	CoinA.OldSta=COININ_NOR;	
	CoinB.NormSta=COININ_NOR;
	CoinB.OldSta=COININ_NOR;	  
#endif

#if TICKET_SELECT 
	TicketA.NormSta=TICKETIN_NOR;
	TicketA.OldSta=TICKETIN_NOR;	
	TicketB.NormSta=TICKETIN_NOR;
	TicketB.OldSta=TICKETIN_NOR;	
#endif
}

void Coin1ChkIn(void)
{
	#if COIN_SELECT
	u8 NewSta=0;
	if(SignIn.In245Status&(1<<IN245ID_COIN1))
		NewSta=1;
	//卡币故障处理
	if(NewSta!=CoinA.NormSta)
	{
		if(++CoinA.BlockCnt>=200)
		{	
			CoinA.BlockCnt=0;
			Flag.Error|=1L<<ERROR_COIN1_BLOCK; //投币器故障
		}
	}
	else
	{
		CoinA.BlockCnt=0;
		Flag.Error&=~(1L<<ERROR_COIN1_BLOCK); //清除投币器故障
	}
	//投币器故障，返回
	if(Flag.Error&(1L<<ERROR_COIN1_BLOCK))
		return;
	//投币信号处理
	if(NewSta==CoinA.NormSta)
	{//常态电平
		if(CoinA.OldSta==NewSta)//上一次也为常态电平
		{
			CoinA.Delay=0;
			return;
		}
	}
	//检测到投币信号
	if(++CoinA.Delay>=5)	//10ms延时
	{
		CoinA.OldSta=NewSta;
		if(CoinA.OldSta==CoinA.NormSta)
		{//投币信号恢复正常，投币信号有效
				if(System.Play.CurCoins<99)
				System.Play.CurCoins++;
			
			if(System.Bill.TotalCoins<9999999)//当前帐
				System.Bill.TotalCoins++;
			else
				System.Bill.TotalCoins=0;
			
			if(System.Backup.TotalCoins<9999999)//总账
				System.Backup.TotalCoins++;
			else
			{
				System.Backup.TotalCoins=0;
			}
			if(System.SendCoin<9999)
			{
				++System.SendCoin;
			}
      Flag.SaveBill=1;
			CoinA.InsertFlag=1;
			Flag.StartKeySound=0;
		}
		CoinA.Delay=0;
	}
	#endif
}
void Coin2ChkIn(void)
{
	#if COIN_SELECT
	u8 NewSta=0;
	if(SignIn.In245Status&(1<<IN245ID_COIN2))
		NewSta=1;
	//卡币故障处理
	if(NewSta!=CoinB.NormSta)
	{
		if(++CoinB.BlockCnt>=200)
		{	
			CoinB.BlockCnt=0;
			Flag.Error|=1L<<ERROR_COIN2_BLOCK; //投币器故障
		}
	}
	else
	{
		CoinB.BlockCnt=0;
		Flag.Error&=~(1L<<ERROR_COIN2_BLOCK); //清除投币器故障
	}
	//投币器故障，返回
	if(Flag.Error&(1L<<ERROR_COIN2_BLOCK))
		return;
	//投币信号处理
	if(NewSta==CoinB.NormSta)
	{//常态电平
		if(CoinB.OldSta==NewSta)//上一次也为常态电平
		{
			CoinB.Delay=0;
			return;
		}
	}
	//检测到投币信号
	if(++CoinB.Delay>=5)	//10ms延时
	{
		CoinB.OldSta=NewSta;
		if(CoinB.OldSta==CoinB.NormSta)
		{//投币信号恢复正常，投币信号有效
			
			if(System.Play.CurCoins<99)
				System.Play.CurCoins++;
			
			if(System.Bill.TotalCoins<9999999)//当前帐
				System.Bill.TotalCoins++;
			else
				System.Bill.TotalCoins=0;
			
			if(System.Backup.TotalCoins<9999999)//总账
				System.Backup.TotalCoins++;
			else
			{
				System.Backup.TotalCoins=0;
			}

			if(System.SendCoin<9999)
			{
				++System.SendCoin;
			}
      Flag.SaveBill=1;
			CoinB.InsertFlag=1;
			Flag.StartKeySound=0;
		}
		CoinB.Delay=0;
	}
	#endif
}
void TicketAChkIn(void)
{
	#if TICKET_SELECT 
	u8 DelayTuiPiao=0;
	u8 NewSta=0;
	NewSta=TicketAIn;
	if(NewSta!=TicketA.NormSta)
	{
		if(++TicketA.BlockCnt>=1000)
		{//错误计时时间到
			TicketA.BlockCnt=1000;
			TicketA.Enable=0;
			TicketAStop();//关闭彩票机
			TicketA.Error|=(1<<ERROR_TICKETA_BLOCK);//彩票错误标志置位
		}
	}
	else
	{//状态已经恢复
		TicketA.BlockCnt=0;
		TicketA.Error&=~(1<<ERROR_TICKETA_BLOCK);//彩票错误标志清除
	}

	/*!缺彩票故障,定时器3S判断*/
	if(1==TicketA.Enable)
	{//出彩票在使能状态
		if(++TicketA.EmptyCnt>=600)//3s=5ms*600
		{//彩票空计数到
			TicketA.Enable=0;
			TicketA.EmptyCnt=0;//清空检测计数
			TicketA.Error|=(1L<<ERROR_TICKETA_EMPTY); //!缺彩票故障
      TicketAStop();
    }
	}
	if(TicketA.Error&(1L<<ERROR_TICKETA_BLOCK))
	{//处于彩票故障
		return;
	}	
	if(0==TicketA.Enable)
	{//彩票在失能状态
		TicketAStop();//关闭彩票机
	}
	else
	{//彩票在使能状态
		if(System.Play.CurTickets)//彩票数不为0
		{
			TicketAStart();//打开彩票机
			if(TicketA.OldSta==NewSta)
			{//状态与上一次相同
				if(NewSta==TicketA.NormSta)//
				{//都为常态电平
					TicketA.Delay=0;//检测到出一张票的电平，延时清零
				}
				return;
			}
			//TicketA.OldSta=NewSta;
			/*!彩票信号10MS有效，否则丢弃*/
			if(++TicketA.Delay>=1)
			{//彩票检测延时时间到
				TicketA.OldSta=NewSta;
				if(NewSta==TicketA.NormSta)
				{//出一张彩票完成
					if(System.Play.CurTickets)
						System.Play.CurTickets--;//彩票数减1
					
					if(System.Bill.TotalTickets<999999)
						System.Bill.TotalTickets++;
					else
						System.Bill.TotalTickets=0;
					
					if(System.Backup.TotalTickets<999999)
						System.Backup.TotalTickets++;
					else
						System.Backup.TotalTickets=0;
					
					Flag.SaveBill=1;
					TicketA.EmptyCnt=0;//彩票空清零
					TicketA.Error&=~(1<<ERROR_TICKETA_EMPTY);
				}
				TicketA.Delay=0;//检测延时时间清零
			}			
		}
		else
		{//彩票已经全部出完
			//if(++DelayTuiPiao>=2)
			{
				Flag.SaveBill=1;
				DelayTuiPiao=0;
				TicketA.EmptyCnt=0;
				TicketA.Enable=0;
				TicketA.Error&=~(1<<ERROR_TICKETA_EMPTY);
				TicketAStop();
				TicketAStop();
			}
		}
	}
	#endif
}


void TicketBChkIn(void)
{
	#if TICKET_SELECT 
	u8 DelayTuiPiao=0;
	u8 NewSta=0;
	NewSta=TicketBIn;
	if(NewSta!=TicketB.NormSta)
	{
		if(++TicketB.BlockCnt>=1000)
		{//错误计时时间到
			TicketB.BlockCnt=1000;
			TicketB.Enable=0;
			TicketBStop();//关闭彩票机
			TicketB.Error|=(1<<ERROR_TICKETB_BLOCK);//彩票错误标志置位
		}
	}
	else
	{//状态已经恢复
		TicketB.BlockCnt=0;
		TicketB.Error&=~(1<<ERROR_TICKETB_BLOCK);//彩票错误标志清除
	}

	/*!缺彩票故障,定时器3S判断*/
	if(1==TicketB.Enable)
	{//出彩票在使能状态
		if(++TicketB.EmptyCnt>=600)//3s=5ms*600
		{//彩票空计数到
			TicketB.Enable=0;
			TicketB.EmptyCnt=0;//清空检测计数
			TicketB.Error|=(1L<<ERROR_TICKETB_EMPTY); //!缺彩票故障
      TicketBStop();
    }
	}
	if(TicketB.Error&(1L<<ERROR_TICKETB_BLOCK))
	{//处于彩票故障
		return;
	}	
	if(0==TicketB.Enable)
	{//彩票在失能状态
		TicketBStop();//关闭彩票机
	}
	else
	{//彩票在使能状态
		if(System.Play.CurTickets)//彩票数不为0
		{
			TicketBStart();//打开彩票机
			if(TicketB.OldSta==NewSta)
			{//状态与上一次相同
				if(NewSta==TicketB.NormSta)//
				{//都为常态电平
					TicketB.Delay=0;//检测到出一张票的电平，延时清零
				}
				return;
			}
			//TicketB.OldSta=NewSta;
			/*!彩票信号10MS有效，否则丢弃*/
			if(++TicketB.Delay>=1)
			{//彩票检测延时时间到
				TicketB.OldSta=NewSta;
				if(NewSta==TicketB.NormSta)
				{//出一张彩票完成
					
					if(System.Play.CurTickets)
						System.Play.CurTickets--;//彩票数减1   
					
					if(System.Bill.TotalTickets<999999)
						System.Bill.TotalTickets++;
					else
						System.Bill.TotalTickets=0;
					
					if(System.Backup.TotalTickets<999999)
						System.Backup.TotalTickets++;
					else
						System.Backup.TotalTickets=0;
					Flag.SaveBill=1;
					TicketB.EmptyCnt=0;//彩票空清零
					TicketB.Error&=~(1<<ERROR_TICKETB_EMPTY);//清除缺彩票故障
				}
				TicketB.Delay=0;//检测延时时间清零
			}			
		}
		else
		{//彩票已经全部出完
			//if(++DelayTuiPiao>=2)
			{
				Flag.SaveBill=1;
				DelayTuiPiao=0;
				TicketB.EmptyCnt=0;
				TicketB.Enable=0;
				TicketB.Error&=~(1<<ERROR_TICKETB_EMPTY);
				TicketBStop();
				TicketBStop();
			}
		}
	}
	#endif
}



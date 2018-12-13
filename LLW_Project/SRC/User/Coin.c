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
	//���ҹ��ϴ���
	if(NewSta!=CoinA.NormSta)
	{
		if(++CoinA.BlockCnt>=200)
		{	
			CoinA.BlockCnt=0;
			Flag.Error|=1L<<ERROR_COIN1_BLOCK; //Ͷ��������
		}
	}
	else
	{
		CoinA.BlockCnt=0;
		Flag.Error&=~(1L<<ERROR_COIN1_BLOCK); //���Ͷ��������
	}
	//Ͷ�������ϣ�����
	if(Flag.Error&(1L<<ERROR_COIN1_BLOCK))
		return;
	//Ͷ���źŴ���
	if(NewSta==CoinA.NormSta)
	{//��̬��ƽ
		if(CoinA.OldSta==NewSta)//��һ��ҲΪ��̬��ƽ
		{
			CoinA.Delay=0;
			return;
		}
	}
	//��⵽Ͷ���ź�
	if(++CoinA.Delay>=5)	//10ms��ʱ
	{
		CoinA.OldSta=NewSta;
		if(CoinA.OldSta==CoinA.NormSta)
		{//Ͷ���źŻָ�������Ͷ���ź���Ч
				if(System.Play.CurCoins<99)
				System.Play.CurCoins++;
			
			if(System.Bill.TotalCoins<9999999)//��ǰ��
				System.Bill.TotalCoins++;
			else
				System.Bill.TotalCoins=0;
			
			if(System.Backup.TotalCoins<9999999)//����
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
	//���ҹ��ϴ���
	if(NewSta!=CoinB.NormSta)
	{
		if(++CoinB.BlockCnt>=200)
		{	
			CoinB.BlockCnt=0;
			Flag.Error|=1L<<ERROR_COIN2_BLOCK; //Ͷ��������
		}
	}
	else
	{
		CoinB.BlockCnt=0;
		Flag.Error&=~(1L<<ERROR_COIN2_BLOCK); //���Ͷ��������
	}
	//Ͷ�������ϣ�����
	if(Flag.Error&(1L<<ERROR_COIN2_BLOCK))
		return;
	//Ͷ���źŴ���
	if(NewSta==CoinB.NormSta)
	{//��̬��ƽ
		if(CoinB.OldSta==NewSta)//��һ��ҲΪ��̬��ƽ
		{
			CoinB.Delay=0;
			return;
		}
	}
	//��⵽Ͷ���ź�
	if(++CoinB.Delay>=5)	//10ms��ʱ
	{
		CoinB.OldSta=NewSta;
		if(CoinB.OldSta==CoinB.NormSta)
		{//Ͷ���źŻָ�������Ͷ���ź���Ч
			
			if(System.Play.CurCoins<99)
				System.Play.CurCoins++;
			
			if(System.Bill.TotalCoins<9999999)//��ǰ��
				System.Bill.TotalCoins++;
			else
				System.Bill.TotalCoins=0;
			
			if(System.Backup.TotalCoins<9999999)//����
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
		{//�����ʱʱ�䵽
			TicketA.BlockCnt=1000;
			TicketA.Enable=0;
			TicketAStop();//�رղ�Ʊ��
			TicketA.Error|=(1<<ERROR_TICKETA_BLOCK);//��Ʊ�����־��λ
		}
	}
	else
	{//״̬�Ѿ��ָ�
		TicketA.BlockCnt=0;
		TicketA.Error&=~(1<<ERROR_TICKETA_BLOCK);//��Ʊ�����־���
	}

	/*!ȱ��Ʊ����,��ʱ��3S�ж�*/
	if(1==TicketA.Enable)
	{//����Ʊ��ʹ��״̬
		if(++TicketA.EmptyCnt>=600)//3s=5ms*600
		{//��Ʊ�ռ�����
			TicketA.Enable=0;
			TicketA.EmptyCnt=0;//��ռ�����
			TicketA.Error|=(1L<<ERROR_TICKETA_EMPTY); //!ȱ��Ʊ����
      TicketAStop();
    }
	}
	if(TicketA.Error&(1L<<ERROR_TICKETA_BLOCK))
	{//���ڲ�Ʊ����
		return;
	}	
	if(0==TicketA.Enable)
	{//��Ʊ��ʧ��״̬
		TicketAStop();//�رղ�Ʊ��
	}
	else
	{//��Ʊ��ʹ��״̬
		if(System.Play.CurTickets)//��Ʊ����Ϊ0
		{
			TicketAStart();//�򿪲�Ʊ��
			if(TicketA.OldSta==NewSta)
			{//״̬����һ����ͬ
				if(NewSta==TicketA.NormSta)//
				{//��Ϊ��̬��ƽ
					TicketA.Delay=0;//��⵽��һ��Ʊ�ĵ�ƽ����ʱ����
				}
				return;
			}
			//TicketA.OldSta=NewSta;
			/*!��Ʊ�ź�10MS��Ч��������*/
			if(++TicketA.Delay>=1)
			{//��Ʊ�����ʱʱ�䵽
				TicketA.OldSta=NewSta;
				if(NewSta==TicketA.NormSta)
				{//��һ�Ų�Ʊ���
					if(System.Play.CurTickets)
						System.Play.CurTickets--;//��Ʊ����1
					
					if(System.Bill.TotalTickets<999999)
						System.Bill.TotalTickets++;
					else
						System.Bill.TotalTickets=0;
					
					if(System.Backup.TotalTickets<999999)
						System.Backup.TotalTickets++;
					else
						System.Backup.TotalTickets=0;
					
					Flag.SaveBill=1;
					TicketA.EmptyCnt=0;//��Ʊ������
					TicketA.Error&=~(1<<ERROR_TICKETA_EMPTY);
				}
				TicketA.Delay=0;//�����ʱʱ������
			}			
		}
		else
		{//��Ʊ�Ѿ�ȫ������
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
		{//�����ʱʱ�䵽
			TicketB.BlockCnt=1000;
			TicketB.Enable=0;
			TicketBStop();//�رղ�Ʊ��
			TicketB.Error|=(1<<ERROR_TICKETB_BLOCK);//��Ʊ�����־��λ
		}
	}
	else
	{//״̬�Ѿ��ָ�
		TicketB.BlockCnt=0;
		TicketB.Error&=~(1<<ERROR_TICKETB_BLOCK);//��Ʊ�����־���
	}

	/*!ȱ��Ʊ����,��ʱ��3S�ж�*/
	if(1==TicketB.Enable)
	{//����Ʊ��ʹ��״̬
		if(++TicketB.EmptyCnt>=600)//3s=5ms*600
		{//��Ʊ�ռ�����
			TicketB.Enable=0;
			TicketB.EmptyCnt=0;//��ռ�����
			TicketB.Error|=(1L<<ERROR_TICKETB_EMPTY); //!ȱ��Ʊ����
      TicketBStop();
    }
	}
	if(TicketB.Error&(1L<<ERROR_TICKETB_BLOCK))
	{//���ڲ�Ʊ����
		return;
	}	
	if(0==TicketB.Enable)
	{//��Ʊ��ʧ��״̬
		TicketBStop();//�رղ�Ʊ��
	}
	else
	{//��Ʊ��ʹ��״̬
		if(System.Play.CurTickets)//��Ʊ����Ϊ0
		{
			TicketBStart();//�򿪲�Ʊ��
			if(TicketB.OldSta==NewSta)
			{//״̬����һ����ͬ
				if(NewSta==TicketB.NormSta)//
				{//��Ϊ��̬��ƽ
					TicketB.Delay=0;//��⵽��һ��Ʊ�ĵ�ƽ����ʱ����
				}
				return;
			}
			//TicketB.OldSta=NewSta;
			/*!��Ʊ�ź�10MS��Ч��������*/
			if(++TicketB.Delay>=1)
			{//��Ʊ�����ʱʱ�䵽
				TicketB.OldSta=NewSta;
				if(NewSta==TicketB.NormSta)
				{//��һ�Ų�Ʊ���
					
					if(System.Play.CurTickets)
						System.Play.CurTickets--;//��Ʊ����1   
					
					if(System.Bill.TotalTickets<999999)
						System.Bill.TotalTickets++;
					else
						System.Bill.TotalTickets=0;
					
					if(System.Backup.TotalTickets<999999)
						System.Backup.TotalTickets++;
					else
						System.Backup.TotalTickets=0;
					Flag.SaveBill=1;
					TicketB.EmptyCnt=0;//��Ʊ������
					TicketB.Error&=~(1<<ERROR_TICKETB_EMPTY);//���ȱ��Ʊ����
				}
				TicketB.Delay=0;//�����ʱʱ������
			}			
		}
		else
		{//��Ʊ�Ѿ�ȫ������
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



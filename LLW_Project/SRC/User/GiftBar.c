#include "GiftBar.h"
#include ".\Lib\includes.h"
S_GIFTBAR GiftBar;
//��Ʒ������ӿ�ʹ��˵��
//������Ʒ�ź� GiftBar.InStatus 
//��Ʒ������ӿ�����
GPIO_TypeDef *GiftPort[8]={
GPIOC,//0
GPIOC,//1
GPIOB,//2
GPIOB,//3
GPIOB,//4
GPIOE,//5
GPIOE,//6
GPIOE //7
};
u16 GiftPin[8]={
GPIO_Pin_4,//0
GPIO_Pin_5,//1
GPIO_Pin_0,//2
GPIO_Pin_1,//3
GPIO_Pin_2,//4
GPIO_Pin_7,//5
GPIO_Pin_8,//6
GPIO_Pin_9 //7
};
void GiftBarParaInit(void)
{
  GiftBar.Error=0;				//����
	GiftBar.Enable=0;				//ʹ��
	GiftBar.InStatus=0;			//΢������״̬
	GiftBar.Status=0;				//Ĭ��״̬
	GiftBar.PressCount[0]=0;//��Ч״̬����
  GiftBar.PressCount[1]=0;//��Ч״̬����
  GiftBar.PressCount[2]=0;//��Ч״̬����
  GiftBar.PressCount[3]=0;//��Ч״̬����
  GiftBar.PressCount[4]=0;//��Ч״̬����
	GiftBar.FreeCount[0]=0;	//ȱ��Ʒ״̬����
  GiftBar.FreeCount[1]=0;	//ȱ��Ʒ״̬����
  GiftBar.FreeCount[2]=0;	//ȱ��Ʒ״̬����
  GiftBar.FreeCount[3]=0;	//ȱ��Ʒ״̬����
  GiftBar.FreeCount[4]=0;	//ȱ��Ʒ״̬����
}
void GiftBarSetAllStop(void)
{
	GiftPort[0]->BRR=GiftPin[0];
	GiftPort[1]->BRR=GiftPin[1];
	GiftPort[2]->BRR=GiftPin[2];
	GiftPort[3]->BRR=GiftPin[3];
	GiftPort[4]->BRR=GiftPin[4];
	GiftBar.Enable=0;
} 
void GiftBarAutoOut(void)
{
	u8 i=0;
	for(;i<5;i++)
	{
		if(Flag.GiftExistSta&(1<<i))
		{
			Flag.GiftLedSta=1<<i;
			Flag.GiftId=i;
			GiftBarSetRun(Flag.GiftId);
			break;
		}
	}
}
void GiftBarInt(void)
{
	u8 i=0;
  u8 temp=0;
	if(Flag.GiftSelEn==ENABLE)
		return;
	GiftBar.InStatus=SignIn.In245Status&0x001f;//����Ʒ�ź�
  for(i=0;i<GIFTBAR_MAX;i++)
  {
		if(GiftBar.InStatus&(1<<i))
		{//����
			GiftBar.PressCount[i]=0;
			if(GiftBar.Enable&(1<<i))
			{//��Ʒ���Ѿ�ʹ��
				if(GiftBar.Status&(1<<i))
				{//����Ʒ���
					GiftBarSetStop(i);									//�ر���Ʒ��
				}
				else
				{
					if(++GiftBar.FreeCount[i]>12000)		//
					{//40Sʱ�䵽������Ʒ
						GiftBar.FreeCount[i]=12000;								//
						Flag.GiftExistSta &=~(1<<i);							//�����Ʒ���ڱ�־
						GiftBarSetStop(i);												//�ر���Ʒ��
						Flag.Error|=1<<(ERROR_GIFTBAR1_EMPTY+i);	//��Ʒ�˿չ���				
					}
				}
			}
			else
			{//��Ʒ��ʧ��
				GiftBar.FreeCount[i]=0;
			}
			GiftBar.Status&=~(1<<i);
			Flag.Error&=~(1<<(ERROR_GIFTBAR1_BLOCK+i));
		}
		else
		{//����
			GiftBar.FreeCount[i]=0;
			GiftBar.PressCount[i]++;
			if(GiftBar.PressCount[i]>400)
			{//����2S����
				GiftBar.PressCount[i]=400;
				Flag.Error|=1<<(i+ERROR_GIFTBAR1_BLOCK);
				GiftBarSetStop(i);//�ر���Ʒ��
			}
			else if(GiftBar.PressCount[i]>3)
			{//�̰�
				GiftBar.Status|=1<<i;
			}
		}      			
	}
}
// GiftLed1->A573B4 GiftLed2->A573B4 GiftLed3->MO6 GiftLed4->MO7 GiftLed5->MO8
void GiftBarLedProcess(void)
{
	u8 i=0;
	for(i=0;i<GIFTBAR_MAX;i++)
	{
		if(i<2)
		{
			if(Flag.GiftLedSta&(1<<i))
			{
				Flag.Out2803Value|=1<<(4+i);
			}
			else
			{
				Flag.Out2803Value&=~(1<<(4+i));
			}
			
		}
		else
		{
			if(Flag.GiftLedSta&(1<<i))
			{
				GiftPort[i+3]->BSRR=GiftPin[i+3];
			}
			else
			{
				GiftPort[i+3]->BRR=GiftPin[i+3];
			}
		}
	}
}



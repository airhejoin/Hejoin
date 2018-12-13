#ifndef _COIN_H_
#define _COIN_H_

//Ť�� ��Ʊ Ͷ�ҹ���ѡ��
#define EGG_SELECT 0
#define TICKET_SELECT 0
#define COIN_SELECT 1

//Ͷ����ض���
#define COININ_NOR 1 //Ͷ�ҳ�̬��ƽ
#define CoinAIn   //GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define CoinBIn   //GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)

typedef struct{
	u8 NormSta;
	u8 OldSta;
	u16 BlockCnt;//
	u16 EmptyCnt;
	u8 InsertFlag;//��Ͷ�ұ�־
	u8 Delay;
}S_COIN;
extern S_COIN CoinA,CoinB;

//Ť����ض���
#define EGGIN_NOR 1 //Ť����̬��ƽ

//#define EggAStop()   do{GPIO_ResetBits(GPIOE,GPIO_Pin_13);}while(0)
//#define EggAStart()  do{GPIO_SetBits(GPIOE,GPIO_Pin_13);}while(0)
//#define EggBStop()   do{GPIO_ResetBits(GPIOE,GPIO_Pin_15);}while(0)
//#define EggBStart()  do{GPIO_SetBits(GPIOE,GPIO_Pin_15);}while(0)

#define EggAIn    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)
#define EggBIn    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)

//��Ʊ��ض���

#define TICKETIN_NOR 0 //��Ʊ������źų�̬��ƽ

#define TicketAStop()   do{Flag.Out573Buf[0]&=~0x10;}while(0)
#define TicketAStart()  do{Flag.Out573Buf[0]|=0x10;}while(0)
#define TicketBStop()   do{Flag.Out573Buf[0]&=~0x20;}while(0)
#define TicketBStart()  do{Flag.Out573Buf[0]|=0x20;}while(0)

#define TicketAIn GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)
#define TicketBIn GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

typedef struct{
	u8 Error;
	u8 NormSta;
	u8 OldSta;
	u8 Enable;
	u16 BlockCnt;//
	u16 EmptyCnt;
	u8 Delay;
}S_TICKET;
extern S_TICKET TicketA,TicketB;

enum{
	ERROR_TICKETA_BLOCK=0,			//��Ʊ������11
	ERROR_TICKETA_EMPTY,			//��Ʊ��ȱƱ12	
	ERROR_TICKETB_BLOCK,			//��Ʊ������13
	ERROR_TICKETB_EMPTY,			//��Ʊ��ȱƱ14	
};

void CoinInit(void);

void Coin1ChkIn(void);
void Coin2ChkIn(void);
void TicketAChkIn(void);
void TicketBChkIn(void);
void TicketTask(void);

#endif

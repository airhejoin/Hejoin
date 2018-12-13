#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"
enum{
	IN245ID_IN1,				//0
	IN245ID_IN2,				//1
	IN245ID_IN3,				//2
	IN245ID_IN4,				//3
	IN245ID_IN5,				//4
	IN245ID_IN6,				//5
	IN245ID_IN7,				//6
	IN245ID_IN8,				//7
	IN245ID_IN9,				//8
	IN245ID_IN10,				//9
	IN245ID_IN11,				//10
	IN245ID_IN12,				//11
	IN245ID_IN13,				//12
	IN245ID_IN14,				//13
	IN245ID_IN15,				//14
	IN245ID_IN16,				//15
	IN245ID_IN17,				//16
	IN245ID_IN18,				//17
	IN245ID_IN19,				//18
	IN245ID_IN20,				//19
	IN245ID_COIN1,			//20
	IN245ID_COIN2,   		//21
	IN245ID_TICKET1,   	//22
	IN245ID_TICKET2,   	//23	
	IN245ID_SET,				//24
	IN245ID_ADD,				//25
	IN245ID_DEC,				//26
	IN245ID_NULL,				//27
	IN245ID_START1,   	//28
	IN245ID_START2,   	//29	
	IN245ID_EGG1,				//30
	IN245ID_EGG2,				//31
	IN245ID_MAX,				//32
};


enum {
	KEYID_IN1,
	KEYID_IN2,
	KEYID_IN3,
	KEYID_IN4,
	
	KEYID_IN5,
  KEYID_SET,//设置
  KEYID_ADD,//加
  KEYID_DEC,//减
	
	KEYID_NULL,//预留
  KEYID_START1,//开始1
  KEYID_START2,//开始2
  KEYID_MAX,//
};

typedef struct{
	u32 In245Status;
	u16 Error;
  u16 Status;
  u16 Value;
  u16 PressCount[KEYID_MAX];
  u16 FreeCount[KEYID_MAX];
}S_INSERTSIGN;

extern S_INSERTSIGN SignIn;
void GetIn245Status(void);
void InsertSignScan(void);//输入信号检测
u8 ReadKey(u16 num);//读后自动清楚  
u8 ReadStatus(u16 num);
#define KeyValueClear() do{SignIn.Value&=0x001f;}while(0)
#endif

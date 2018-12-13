/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：鲨皇算法
*设计者：南下三五年
*时  间：2012-11-30  14:08:19
*********************************************************************************
*/
#ifndef __SHARKRANDOM_H
#define __SHARKRANDOM_H


//!算法版本1.0
#define SHARKGAMEVER              20

//!游戏完整描述
/*
*    鲨皇，13门押分，其押分项分布如下:
*
*          飞禽(庄 X2)              射灯(和)                走兽(闲X2)
*    燕子、鸽子、孔雀、老鹰    银鲨、金鲨、大王鲨    兔子、猴子、熊猫、狮子
*
*    赔率说明:
*            飞禽和走兽对称赔率，6种组合:
*             6、 6、 12、 12
*             6、 8、  8、 12
*             4、 8、 12、 24
*             3、12、 24、 24
*             6、 6、  8、 24
*             4、 6、 24、 24
*             飞禽和走兽的赔率固定:
*             2
*             银鲨:
*             24、32,同时送灯
*             金鲨:
*             32、40，同时送灯
*             大王鲨:
*             40-126 ，同时送灯，也送彩金
*    抽水难度:
*            98%  97%  96%  95%  94% 
*    数学模型:
*            该玩法，可以抽象为百乐玩法，同时将庄闲洗分为8门，有8门来完成百乐的庄赢
*            5%的抽水，后台可以依据百乐的模型，然后用和来平衡玩家的抽水，使盈利和总
*            进分量的币值收敛到设定的难度。
*            算法设计上，需要收集几个条件，在8个玩家中，选择押分较多的3个玩家，保证
*            在三个玩家是盈利的，2个玩家小额输赢，3个玩家总体趋势输钱，通过押分多的
*            玩家赢钱，估计玩家多押分，同时由于只是3个玩家在输钱，转移了输钱玩家的
*            注意力，让其认为自己运气不佳。
*            在输钱的玩家，如果其在输钱后，如果重新上分，就有很大的可能可以翻本，给
*            玩家一种输钱只是暂时的假象。
*            机台在清帐打码后，将上次盈利的40%在前期放水给玩家，来吸引玩家。
*            机台的营收，每天最多10000块，多出的抽水，以鲨鱼或者大赔率送出，在场地和
*            玩家之间动态平衡。
*            出鲨鱼系列时，尽量多次连出，以诱惑玩家押分，如果在玩家在余分为零的情况
*            下，押了鲨鱼，则尽可能送出鲨鱼，让其有翻本机会。
*            引入贪心算法，在玩家输钱的时候，如果上分，有机会翻本，连续赢钱的情况下，
*            设计几个时间段，让其冲击高分，然后下滑，在其冲击高分的过程中，慢慢的抽
*           掉玩家余分，玩家的输钱多少转换为玩家自行控制。
*            算法的输入:
*            1、机台的当期总进分
*            2、玩家的押分
*           3、玩家的余额
*            4、玩家当前的输分
*
*/

#define MAXSLAVE    16  //!目前游戏最多3个玩家

enum
{
	YINSHAKR=8,
	GOLDSHARK=9,
	KINTSHARK=10,
};

enum
{
	TYPECLEARALLBILL=1,   //!继续请总账
	TYPEADDSLAVE=2,       //!添加分机
};

enum
{
	ADJ_UNITCOIN=1,     //!调整币值
	ADJ_CHANGDI,		//!调整场地类型
	ADJ_CODEDAY,		//!调整打码时间
	ADJ_DATE,			//!调整系统时间
	ADJ_DIS,			//!打码信息是否显示
};

typedef struct
{
	INT8U betrank[4];         //!押分最多的3组
	INT32U sharkbet[4];        //!鲨鱼的押分
	INT32U betfowls;           //!飞禽单项押分
	INT32U betbeasts;          //!走兽单项押分
	INT32U betfowls4;          //!飞禽4门总押分
	INT32U betbeasts4;         //!走兽4门总押分
	INT32U totalbet;           //!全部押分
}S_GAMEBET;

typedef struct
{
	S_GAMEBET betplayer[MAXSLAVE];     //!玩家押分信息
	INT8U betbigmac[4];                //!台面押分3甲
	INT8U betbigalone[4];              //!单项押分3甲
	INT32U totalbetfowls;              //!台面飞禽押分
	INT32U totalbetbeasts;             //!台面走兽押分
	INT32U totalbetfowls4;             //!台面飞禽4门总押分
	INT32U totalbetbeasts4;            //!台面走兽4分总押分
	INT32U shark[4];                   //!台面鲨鱼总押分
	INT32U totalbetalone[8];           //!台面8门独立总押分
	INT32U totalbetlose[8];            //!台面8门独立总赔分
	INT32U totalyinshalose;            //!台面银鲨总赔分
	INT32U totaljinshalose;            //!台面金鲨总赔分
	INT32U totalfowlslose;             //!台面飞禽总赔分
	INT32U totalbeastslose;            //!台面走兽总赔分
	INT32U totalkingsharklose;         //!台面大王鲨总赔分
	INT32U totalbet;                   //!台面总押分
	INT8U  gainrange;				   //!吃分范围
	INT8U  sendrange;				   //!送分范围
	INT8U  sharkrange;                 //!鲨鱼范围
	INT8U  randsel;                    //!随机选择
	INT32U totalbetloseback[8];
}S_BET;
extern S_BET MsgBet;

enum
{
	FREEDOMDOT=12,			//!算法自由运行的点数  37.52%自由结果,骰子的概率
};

extern void GetGameOpenPC(void);
extern void HappyGameSet(void);
extern void GetBetCnt(INT8U *ptr ,INT8U sla);

extern void GetUncodePara(void);
extern INT8U CheckCodeParaReady(INT32U code);
extern void GetSetParaUncode(void);
extern INT8U CheckSetParacode(INT32U codeinput1, INT32U codeinput2);
extern void AudgeNextCodeDate(void);


#endif

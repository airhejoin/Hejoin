/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：数码管显示
*设计者：南下三五年
*时  间：2012-05-24  00:53:19
*********************************************************************************
*/
#ifndef __TASKLED_H
#define __TASKLED_H

//显示数据包,为了方便处理，采用数据块的形式存储
typedef struct
{
	INT8U id;                	//!分机号
	INT16U unitcoin;            //!币值
	INT16U unitlottery;         //!彩票
	INT32U baojifen;            //!爆机分值
	INT8U selpeilv;				//!赔率选择
	INT16U yafen[13];			//!押分项
	INT16U peilv[8];			//!赔率
	INT32U credit;				//!玩家总分
	INT32U bouns;				//!随机彩金
	INT32U win;					//!玩家赢分
	INT32S gain;                //!全部总盈利
	INT32S gainbefore;          //!上次总盈利
	INT32S gaincurrent;         //!本次实际盈利
	INT32U totalupfen;          //!上分
	INT32U totaldownfen;        //!下分
	INT32U totalcoin;           //!投币
	INT32U totalcout;           //!退币
	INT32U totallottery;        //!彩票
}S_FRAM;

typedef struct
{
	INT8U time;
	INT16U unit;
	S_FRAM ST;
}S_DIS;


typedef struct
{
	S_DIS 	myDis;   
}S_DISLED;

typedef union
{
	S_DISLED DISLED;
	INT8U    buffer[sizeof(S_DISLED)];
}DISBUFF;

extern DISBUFF MyDisbuff;



extern void vLedDisplayTask( void *pvParameters );
























#endif



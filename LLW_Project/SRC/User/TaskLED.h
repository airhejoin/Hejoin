/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺�������ʾ
*����ߣ�����������
*ʱ  �䣺2012-05-24  00:53:19
*********************************************************************************
*/
#ifndef __TASKLED_H
#define __TASKLED_H

//��ʾ���ݰ�,Ϊ�˷��㴦���������ݿ����ʽ�洢
typedef struct
{
	INT8U id;                	//!�ֻ���
	INT16U unitcoin;            //!��ֵ
	INT16U unitlottery;         //!��Ʊ
	INT32U baojifen;            //!������ֵ
	INT8U selpeilv;				//!����ѡ��
	INT16U yafen[13];			//!Ѻ����
	INT16U peilv[8];			//!����
	INT32U credit;				//!����ܷ�
	INT32U bouns;				//!����ʽ�
	INT32U win;					//!���Ӯ��
	INT32S gain;                //!ȫ����ӯ��
	INT32S gainbefore;          //!�ϴ���ӯ��
	INT32S gaincurrent;         //!����ʵ��ӯ��
	INT32U totalupfen;          //!�Ϸ�
	INT32U totaldownfen;        //!�·�
	INT32U totalcoin;           //!Ͷ��
	INT32U totalcout;           //!�˱�
	INT32U totallottery;        //!��Ʊ
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



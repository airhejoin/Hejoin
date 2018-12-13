#ifndef _TASKVOICE_H_
#define _TASKVOICE_H_

#include "stm32f10x.h"

#include"..\WTV020\WTV020.h"

#define VOICE_MAX 143
//������ַö��
enum{
	VOICE_INSERTCOIN=0,		//Ͷ����0
	VOICE_PLEASECOIN,			//��Ͷ��1
	VOICE_PLEASE2COIN,		//��Ͷ�����ҿ�ʼ��Ϸ2
	VOICE_PLEASE3COIN,		//��Ͷ�����ҿ�ʼ��Ϸ3
	VOICE_PLEASE4COIN,		//��Ͷ�ĸ��ҿ�ʼ��Ϸ4
	VOICE_PRESSSTARTKEY,	//�밴��ʼ����ʼ��Ϸ��5
	VOICE_GETBALL,				//�������6
	VOICE_STARTGAME,				//��ʼ��Ϸ7
	VOICE_GAMESTART,				//��Ϸ��ʼ8
	
	VOICE_TICKETERROR=0x0a,			//��Ʊ������
	VOICE_CONNECTSERVER,				//�������Ա��ϵ	
	
	
	VOICE_GETSECONDPRIZE=0x11,	//���������// ���Ƚ�
	VOICE_WISHYOU,							//��ϲ��//��������
	VOICE_GETFIRSTPRIZE,				//���ó�����//һ�Ƚ�
	VOICE_BALLOVER,							//��Ϸ����//��������
	
	VOICE_TIMEOVER=0x17,				//��Ϸ�Ѿ���ʱ
	VOICE_GETTHIRDPRIZE,				//�����//���Ƚ�
	
	VOICE_TRYTOPLAY=0x1a,				//�����������������ɣ������������
	VOICE_CONNECTSERVETOGETGIFT,
  VOICE_INBALL=0x1f,					//���������
	VOICE_MUSIC,								//��ʱ����
  VOICE_THANKUSE=0x73,//��л��ʹ��
	VOICE_RONGXIANGMACHINE,//���趯�����豸
	VOICE_MACHINESTOPUSE,//�豸������ֹͣʹ��
	VOICE_TOTALCOIN=0x76,
	VOICE_TOTALGIFT,//77
	VOICE_TOTALSMALLPRIZE,//78
	VOICE_TOTALBIGPRIZE,//79
	VOICE_TOTALSUPERPRIZE,//7a
	VOICE_EGG,//7b
	VOICE_TOTALEGG,//7c
	VOICE_TICKET,//7d
	VOICE_TOTALTICKET,//7e
	VOICE_GE,//7f
	VOICE_ZHANG,//80
	
	VOICE_0=0x81,
	VOICE_1,
	VOICE_2,
	VOICE_3,
	VOICE_4,
	VOICE_5,
	VOICE_6,
	VOICE_7,
	VOICE_8,
	VOICE_9,
	VOICE_10,
	VOICE_100,
	VOICE_1000,
	VOICE_10000,
	VOICE_STOP=0xfe,						//��ͣ����
};
void vVoiceTask( void *pvParameters );

#endif

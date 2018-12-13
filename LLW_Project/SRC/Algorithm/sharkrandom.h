/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺����㷨
*����ߣ�����������
*ʱ  �䣺2012-11-30  14:08:19
*********************************************************************************
*/
#ifndef __SHARKRANDOM_H
#define __SHARKRANDOM_H


//!�㷨�汾1.0
#define SHARKGAMEVER              20

//!��Ϸ��������
/*
*    ��ʣ�13��Ѻ�֣���Ѻ����ֲ�����:
*
*          ����(ׯ X2)              ���(��)                ����(��X2)
*    ���ӡ����ӡ���ȸ����ӥ    ���衢���衢������    ���ӡ����ӡ���è��ʨ��
*
*    ����˵��:
*            ���ݺ����޶Գ����ʣ�6�����:
*             6�� 6�� 12�� 12
*             6�� 8��  8�� 12
*             4�� 8�� 12�� 24
*             3��12�� 24�� 24
*             6�� 6��  8�� 24
*             4�� 6�� 24�� 24
*             ���ݺ����޵����ʹ̶�:
*             2
*             ����:
*             24��32,ͬʱ�͵�
*             ����:
*             32��40��ͬʱ�͵�
*             ������:
*             40-126 ��ͬʱ�͵ƣ�Ҳ�Ͳʽ�
*    ��ˮ�Ѷ�:
*            98%  97%  96%  95%  94% 
*    ��ѧģ��:
*            ���淨�����Գ���Ϊ�����淨��ͬʱ��ׯ��ϴ��Ϊ8�ţ���8������ɰ��ֵ�ׯӮ
*            5%�ĳ�ˮ����̨�������ݰ��ֵ�ģ�ͣ�Ȼ���ú���ƽ����ҵĳ�ˮ��ʹӯ������
*            �������ı�ֵ�������趨���Ѷȡ�
*            �㷨����ϣ���Ҫ�ռ�������������8������У�ѡ��Ѻ�ֽ϶��3����ң���֤
*            �����������ӯ���ģ�2�����С����Ӯ��3���������������Ǯ��ͨ��Ѻ�ֶ��
*            ���ӮǮ��������Ҷ�Ѻ�֣�ͬʱ����ֻ��3���������Ǯ��ת������Ǯ��ҵ�
*            ע������������Ϊ�Լ��������ѡ�
*            ����Ǯ����ң����������Ǯ����������Ϸ֣����кܴ�Ŀ��ܿ��Է�������
*            ���һ����Ǯֻ����ʱ�ļ���
*            ��̨�����ʴ���󣬽��ϴ�ӯ����40%��ǰ�ڷ�ˮ����ң���������ҡ�
*            ��̨��Ӫ�գ�ÿ�����10000�飬����ĳ�ˮ����������ߴ������ͳ����ڳ��غ�
*            ���֮�䶯̬ƽ�⡣
*            ������ϵ��ʱ������������������ջ����Ѻ�֣��������������Ϊ������
*            �£�Ѻ�����㣬�򾡿����ͳ����㣬�����з������ᡣ
*            ����̰���㷨���������Ǯ��ʱ������Ϸ֣��л��ᷭ��������ӮǮ������£�
*            ��Ƽ���ʱ��Σ��������߷֣�Ȼ���»����������߷ֵĹ����У������ĳ�
*           �������֣���ҵ���Ǯ����ת��Ϊ������п��ơ�
*            �㷨������:
*            1����̨�ĵ����ܽ���
*            2����ҵ�Ѻ��
*           3����ҵ����
*            4����ҵ�ǰ�����
*
*/

#define MAXSLAVE    16  //!Ŀǰ��Ϸ���3�����

enum
{
	YINSHAKR=8,
	GOLDSHARK=9,
	KINTSHARK=10,
};

enum
{
	TYPECLEARALLBILL=1,   //!����������
	TYPEADDSLAVE=2,       //!��ӷֻ�
};

enum
{
	ADJ_UNITCOIN=1,     //!������ֵ
	ADJ_CHANGDI,		//!������������
	ADJ_CODEDAY,		//!��������ʱ��
	ADJ_DATE,			//!����ϵͳʱ��
	ADJ_DIS,			//!������Ϣ�Ƿ���ʾ
};

typedef struct
{
	INT8U betrank[4];         //!Ѻ������3��
	INT32U sharkbet[4];        //!�����Ѻ��
	INT32U betfowls;           //!���ݵ���Ѻ��
	INT32U betbeasts;          //!���޵���Ѻ��
	INT32U betfowls4;          //!����4����Ѻ��
	INT32U betbeasts4;         //!����4����Ѻ��
	INT32U totalbet;           //!ȫ��Ѻ��
}S_GAMEBET;

typedef struct
{
	S_GAMEBET betplayer[MAXSLAVE];     //!���Ѻ����Ϣ
	INT8U betbigmac[4];                //!̨��Ѻ��3��
	INT8U betbigalone[4];              //!����Ѻ��3��
	INT32U totalbetfowls;              //!̨�����Ѻ��
	INT32U totalbetbeasts;             //!̨������Ѻ��
	INT32U totalbetfowls4;             //!̨�����4����Ѻ��
	INT32U totalbetbeasts4;            //!̨������4����Ѻ��
	INT32U shark[4];                   //!̨��������Ѻ��
	INT32U totalbetalone[8];           //!̨��8�Ŷ�����Ѻ��
	INT32U totalbetlose[8];            //!̨��8�Ŷ��������
	INT32U totalyinshalose;            //!̨�����������
	INT32U totaljinshalose;            //!̨����������
	INT32U totalfowlslose;             //!̨����������
	INT32U totalbeastslose;            //!̨�����������
	INT32U totalkingsharklose;         //!̨������������
	INT32U totalbet;                   //!̨����Ѻ��
	INT8U  gainrange;				   //!�Էַ�Χ
	INT8U  sendrange;				   //!�ͷַ�Χ
	INT8U  sharkrange;                 //!���㷶Χ
	INT8U  randsel;                    //!���ѡ��
	INT32U totalbetloseback[8];
}S_BET;
extern S_BET MsgBet;

enum
{
	FREEDOMDOT=12,			//!�㷨�������еĵ���  37.52%���ɽ��,���ӵĸ���
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

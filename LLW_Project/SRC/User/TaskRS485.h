/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺ͨ�Ų���
*����ߣ�����������
*ʱ  �䣺2012-05-24  00:53:19
*********************************************************************************
*/
#ifndef __TASKRS485_H
#define __TASKRS485_H

#define MOTHERADDR      0x80


#define SEND485			GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define RECEIVE485  	GPIO_SetBits(GPIOB, GPIO_Pin_5)


//!��Ϸ����
extern const INT8U gamepeilv[6][8];

//!��Ϸ���̿���
typedef struct
{
	INT8U  step;			//!��Ϸ����
	INT32U systemsta;       //!ϵͳ״̬��0��ʾϵͳ������������Ϸ�����������Ϸ
	INT8U  open;            //!��Ϸ���
	INT8U  sharkpeilv;      //!��������
	INT8U  peilv;			//!��Ϸ����
	INT8U  second;          //!�����н�
	INT8U  secondpeilv;     //!�����н�����
	INT8U  third;           //!�����н�
	INT8U  boundspeilv;     //!�ʽ�����
}S_GAME;
extern S_GAME mYGame;

//!����
enum
{
	GAME_WAITMOTHER=0,      //!�ȴ�����,����3S�ղ�����ʼ��Ϸ�źţ�������
	GAME_START=1,        	//!��Ϸ��ʼ
	GAME_TIME,              //!��Ϸʱ��
	GAME_STOPYAFEN,         //!ֹͣѺ��
	GAME_RESULT,            //!��Ϸ���
	GAME_SENDBILL,          //!����
	GAME_HAPPYOPEN,         //!�����͵�
	GAME_HAPPYTHREE,        //!�����͵�
};

//!������
enum
{
	CMD_START=1,            //!��Ϸ��ʼ
	CMD_TIME,               //!��Ϸʱ��
	CMD_RESULT,             //!��Ϸ���
};
//!������
enum
{
	SHARK_START=0x51,		//!��Ϸ��ʼ
    SHARK_STOPBET,			//!ֹͣѺ��,�Ƿ������
    SHARK_OPEN,   			//!��Ϸ���
    SHARK_HAPPY,            //!�����͵�
    SHARK_HAPPYTHREE,       //!�����͵�
    SHARK_GETBILL,			//!��ȡ�ֻ��˵�
    SHARK_TESTCOMMUNICATION,//!����ͨ��
    SHARK_PARAMETERS,		//!��������
    SHARK_BINDSLAVE,		//!�󶨷ֻ�
    SHARK_SERVERDROP,		//!��������
    SHARK_BILLERR,			//!��Ŀ�쳣
    SHARK_GETBET,           //!��ȡѺ����Ϣ
    SHARK_CLEARBILL,        //!�����Ŀ
    SHARK_NOTIMEBET,        //!ֹͣѺ��
    SHARK_HAPPYSHARK,       //!�����Ƿ������
    SHARK_BOUNDS,           //!�ʽ�
    SHARK_RESETSLAVE,       //!�����ֻ�
    SHARK_GAINOVER,         //!����������Ŀ������Ҫ����
    SHARK_RUNTIMEOVER,      //!����ʱ�䵽������ʱ
    SHARK_PCMSGWRONG,       //!����ͨ�Ź���
    SHARK_PCGAMEWRONG,      //!������Ϸ��ƥ��
    SHARK_STOPSHARKSHOW,    //!ֹͣ������Ч
    SHARK_CLEARBILLCURRENT, //!���������Ŀ
};


//!����
enum
{
	ERR_WAITCONNECT=9000,   //!�ȴ�����������
	ERR_COUTING,            //!�����˱�
	ERR_LOTTERYING,         //!������Ʊ
	ERR_SENDBILL,           //!���ڱ���
	ERR_SENDBILLOK,         //!���˳ɹ�
	ERR_SENDBILLWRONG,      //!����ʧ��
	ERR_MSG,                //!ͨ�Ź���
	ERR_BILLWRONG,          //!�ֻ���Ҫ������ͳһ����
	ERR_MSGDATA,            //!ͨ�����ݴ���
	ERR_CHECKMSG,           //!ͨ�Ų�����
	ERR_PCSERVER,           //!�ȴ����������
	ERR_GAMEWRONG,          //!������Ϸ��ƥ��
	ERR_PCMSG,              //!����ͨ�Ź���
	ERR_SYSTEMCLEAR,        //!ϵͳ����,�����
	ERR_RUNTIME,            //!����ʱ�䵽������ʱ
	ERR_BILLOVER,           //!������Ŀ�����뱨��
	ERR_SYSYTEMWRONG,       //!ϵͳ���ϣ������
};

#define S485MsgSize    16   //16
#define S485MsgBufferLen  80
#define S485RecTimeOut  3           //!10MSǿ�Ʒ��
extern INT8U S485MsgBeg;			//����һ������
extern INT8U S485MsgEnd;			//д��һ������
extern INT8U S485Msg[S485MsgSize][S485MsgBufferLen];//���ݻ�����

typedef struct
{
	INT8U RevBufCount;     //!���ռ���
	INT8U RevTimeOut;      //!���ճ�ʱ
}S_S485;
extern S_S485 MyS485Msg;

extern xSemaphoreHandle xS485RecSemaphore;

extern void vGamePlayerTask( void *pvParameters );

extern INT16U Crc16Check(INT8U *pdata, INT8U len);


//===========================================================
//�������ݱ������磬ͬʱ�ϴ���PC

//ͨ�Ű���Ϸ��������
typedef struct
{
	INT32S totalgain;				//ȫ����ӯ��
	INT32S totalgaincurrent;		//����ʵ������
	INT32U totalkaifen;				//�Ϸ�
	INT32U totalxifen;				//�·�
	INT32U totalcoin;				//Ͷ��
	INT32U totalcout;				//�˱�
	INT32U totallottery;			//��Ʊ
	INT32U totalbet;				//��ǰ��Ѻ��
	INT32U totalwin;				//��ǰ�ܵ÷�
	INT32U totalbounds;				//�ܲʽ�
	INT32U totalruntime;			//ʣ������ʱ��
	INT32U totalgainpre;            //!�ϴ���ӯ��
	INT32U totalbetin;              //!ϵͳ��Ѻ��
	INT32U totalbetout;             //!ϵͳ�ܵ÷�
	INT32S totalreleasemoney;       //!ϵͳ���ˮ
}S_REALTIMEBILL;

//��������
typedef struct
{
	INT32U grade;				//!ϵͳ�Ѷ� ������0������1����2������3������4
	INT32U bettime;     		//!Ѻעʱ��   10-99
	INT32U bpmaxbet;			//!ׯ�����Ѻ��  100-9900
	INT32U hemaxbet;			//!�����Ѻ��    100-5000
	INT32U othermaxbet;    		//!�������Ѻ��  100-5000
	INT32U betunitmin;      	//!��СѺ���л�  1��10��100��500��1000
	INT32U oddsselect;      	//!����ѡ�� 
	INT32U unitlottery;       	//!��Ʊ����  n �� m Ʊ  ��16λ��ʾ�ң���16λ��ʾ��Ʊ
	INT32U macplace;        	//!��̨�ڷ�
}S_PARASET;

//�������
typedef struct
{
	INT32U unitcoin;			//!Ͷ�ҵ�λ   1�� n ��
	INT32U spcaemode;           //!��������
	INT32U uncodedate;          //!��������
	INT32U uncodemsg;           //!������ʾ��Ϣ
}S_UNCODESET;

//ϵͳ����
typedef struct
{
	INT32S 	totalgain;     		//!ȫ����ӯ��
	INT32S 	totalgainpre; 		//!�ϴ���ӯ��
	INT32S  totalgaincurrent;	//!����ʵ������
	INT32U  totalkaifen;        //!���Ϸ�
	INT32U  totalxifen;			//!��ϴ��
	INT32U 	totalcoin;			//!��Ͷ��
	INT32U 	totalcout;			//!���˱�
	INT32U 	totallottery;		//!����Ʊ
	INT32U  systemlineid;  		//!ϵͳ�ߺ�
	INT32U  macid;				//!��̨����
	INT32U 	totalruntime;		//!ʣ������ʱ��
	INT32U 	uncodeday;			//!��������
}S_ALLBILL;

/*!�ֻ���Ŀ*/
typedef struct
{
	INT32U slaveidcheck;		//!�ֻ����к�
	INT32S totalgain;           //!��ӯ��
	INT32U totalkanfen;     	//!���Ϸ�
	INT32U totalxifen;			//!��ϴ��
	INT32U totalcoin;           //!��Ͷ��
	INT32U totalcout;           //!���˱�
	INT32U totallottery;        //!���˲�Ʊ
	INT32U totalbetin;          //!������
	INT32U totalbetout;         //!�����
	INT32U totalbonds;          //!ȫ���ʽ�
	INT32S totalgaincurrent;    //!����ʵ������
}S_SLAVEBILL;

typedef struct
{
	INT8U year;
	INT8U month;
	INT8U day;
	INT8U hour;
	INT8U min;
	INT8U sec;
	INT8U msec;
	INT8U mmsec;
}S_DATE;

/*!��Щ���ݱ���������*/
/*!ע��ṹ���������*/
/*!Ϊ��ת���ķ��㣬�����ݽ��зֿ�洢,�����ݽ����˲��*/
typedef struct
{
	S_REALTIMEBILL	realbill;          //!60�ֽ�
	S_PARASET		setpara;           //!36�ֽ�
	S_UNCODESET     setuncode;         //!16�ֽ�
	S_ALLBILL       systembill;        //!48�ֽ�
	INT32U          timedama;          //!ͨ�������ݶ��ж�ϵͳ�Ƿ��Ǹմ���
	INT32U          hidebounds;        //!���زʽ��
	INT32U          gameseed;          //!��Ϸ�õ������������
	INT32U          codelock;          //!ϵͳ����
	INT32U          vender;             //!�ߺ�   1
	INT32U          serialno;           //!��̨���к�  2
	INT32U          version;            //!����汾 ��16λ ��16λ
	INT32U          password;           //!ϵͳ����   3
	INT32U          codekernel;         //!������㹫ʽ 4
	INT32U          codeready;          //!���˴���
	INT32U          codecnt;            //!�������
	INT32U          codecheck;          //!����У����
	INT32U          adjpara;            //!�����Ѷȣ���Ҫ����
	INT32U          soundvol;           //!������С  ����
}S_KERNEL;//216�ֽ�
extern S_KERNEL MyKernel;

typedef struct
{
	INT32U          allyafen[13];       //!������Ѻ��  52�ֽ�
	S_DATE          codedate;           //!����ʱ��  8�ֽ�
	S_DATE          codedatecurrent;    //!��ǰʱ��  8�ֽ�
	INT8U           gamelist[20];       //!          20�ֽ�
	INT8U           compile_time[20];  	//��ȡϵͳ����ʱ��,�ж��Ƿ��ʼ����Ҫ����  20�ֽ�
}S_RELIST; //108�ֽ�
extern S_RELIST MyRelist;

typedef struct
{
	INT16U          yafen[MAXSLAVE][14];//!�ֻ�Ѻ����Ϣ  448�ֽ�
	INT32U          test;
}S_SLAVEYAFEN; //448
extern S_SLAVEYAFEN MySlaveYafen;


typedef struct
{
	S_SLAVEBILL     slavebill[MAXSLAVE];
	INT32U          test;
}S_SLAVEBILLGROUP; //704
extern S_SLAVEBILLGROUP MySlaveBill;


/*!��Ϸ����,�������ݸ�PC*/
typedef struct
{
	INT8U openshark;		//!�Ƿ������
	INT8U sharkcoor;        //!���������
	INT8U sharkpeilv;       //!���������
	INT8U secondopen;		//!���ο�������
	INT8U secondpeilv;		//!���ο�����������
	INT8U thirdopen;		//!���ο�������
	INT8U thirdpeilv;		//!���ο�����������
	INT8U boundpelv;		//!�ʽ�����
	INT8U sharkcoorback;    //!�������꣬��֤�����ظ�
	INT8U secondopenback;   //!���ο������ﱸ�ݣ���֤�����ظ�
	INT8U thirdopenback;    //!���ο������ﱸ�ݣ���֤�����ظ�
}S_TRANSPC;
extern S_TRANSPC PC;

/*!�㷨��������Ҫ�ķֻ�����*/
typedef struct
{
	INT32U credits;
	INT16U bet[13];
}S_PLAYER;

typedef union
{
	S_PLAYER Player[MAXSLAVE];
	INT8U    ch[sizeof(S_PLAYER)*MAXSLAVE];
}U_PLAYER;

#define MAXTONGJILIST    5
typedef struct
{
	INT16U bet[MAXTONGJILIST];		//!���Ѻ��
	INT8U  open[MAXTONGJILIST];     //!���Ѻ�ֵ���
	INT8U  current;                 //!��ǰͳ�Ƽ�¼
}S_PLAYERBET;

typedef struct
{
	U_PLAYER MyPlayer;                  //���Ѻ�ֺ�������
	S_PLAYERBET MyPlayerBet[MAXSLAVE];  //Ѻ��ͳ��
}S_CTRGAME;

extern S_CTRGAME CtrGame;

typedef struct
{
	INT16U unitcoin;            //!��ֵ
	INT32U baojifen;            //!������ֵ
	INT16U  maxbetzhuangxian;   //!ׯ�����Ѻ��
	INT16U  maxbetshark;        //!�������Ѻ��
	INT16U  maxbetanimal;       //!�������Ѻ��
	INT16U  minbetunit;         //!��СѺ�ֵ�λ
	INT32U unitlottery;         //!��Ʊ
}__attribute__((packed))S_SENDSLAVEPARA;
extern S_SENDSLAVEPARA MySendSlavePara;


extern void SendGameStart(void);

extern void vRS485RecTask( void *pvParameters );
extern void ClearSlaveBill(void);
extern void CreateAllSaveId(void);
extern void EnableSlaveBet(void);
extern void SendSlavePara(void);
extern void SendTestMsg(void);
extern void SendRestSlave(void);
extern void SendSlavePCUnReady(void);
extern void SendSlavePCGameWrong(void);
extern void SendSlavePCComWrong(void);
extern void SendSlaveRunTimeOver(void);
extern void SendSlaveBillOver(void);
extern void ClearSlaveBillCurrent(void);
#endif



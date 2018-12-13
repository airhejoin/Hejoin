/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：通信部分
*设计者：南下三五年
*时  间：2012-05-24  00:53:19
*********************************************************************************
*/
#ifndef __TASKRS485_H
#define __TASKRS485_H

#define MOTHERADDR      0x80


#define SEND485			GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define RECEIVE485  	GPIO_SetBits(GPIOB, GPIO_Pin_5)


//!游戏赔率
extern const INT8U gamepeilv[6][8];

//!游戏流程控制
typedef struct
{
	INT8U  step;			//!游戏流程
	INT32U systemsta;       //!系统状态，0表示系统正常，可以游戏，否则挂起游戏
	INT8U  open;            //!游戏结果
	INT8U  sharkpeilv;      //!鲨鱼赔率
	INT8U  peilv;			//!游戏赔率
	INT8U  second;          //!二次中奖
	INT8U  secondpeilv;     //!二次中奖赔率
	INT8U  third;           //!三次中奖
	INT8U  boundspeilv;     //!彩金赔率
}S_GAME;
extern S_GAME mYGame;

//!流程
enum
{
	GAME_WAITMOTHER=0,      //!等待连接,启动3S收不到开始游戏信号，报故障
	GAME_START=1,        	//!游戏开始
	GAME_TIME,              //!游戏时间
	GAME_STOPYAFEN,         //!停止押分
	GAME_RESULT,            //!游戏结果
	GAME_SENDBILL,          //!报账
	GAME_HAPPYOPEN,         //!二次送灯
	GAME_HAPPYTHREE,        //!三次送灯
};

//!命令字
enum
{
	CMD_START=1,            //!游戏开始
	CMD_TIME,               //!游戏时间
	CMD_RESULT,             //!游戏结果
};
//!命令字
enum
{
	SHARK_START=0x51,		//!游戏开始
    SHARK_STOPBET,			//!停止押分,是否出鲨鱼
    SHARK_OPEN,   			//!游戏结果
    SHARK_HAPPY,            //!幸运送灯
    SHARK_HAPPYTHREE,       //!三次送灯
    SHARK_GETBILL,			//!收取分机账单
    SHARK_TESTCOMMUNICATION,//!测试通信
    SHARK_PARAMETERS,		//!调整参数
    SHARK_BINDSLAVE,		//!绑定分机
    SHARK_SERVERDROP,		//!主机掉线
    SHARK_BILLERR,			//!账目异常
    SHARK_GETBET,           //!获取押分信息
    SHARK_CLEARBILL,        //!清除账目
    SHARK_NOTIMEBET,        //!停止押分
    SHARK_HAPPYSHARK,       //!本局是否出鲨鱼
    SHARK_BOUNDS,           //!彩金
    SHARK_RESETSLAVE,       //!重启分机
    SHARK_GAINOVER,         //!主机当期账目满，需要报账
    SHARK_RUNTIMEOVER,      //!运行时间到，请延时
    SHARK_PCMSGWRONG,       //!电脑通信故障
    SHARK_PCGAMEWRONG,      //!电脑游戏不匹配
    SHARK_STOPSHARKSHOW,    //!停止鲨鱼特效
    SHARK_CLEARBILLCURRENT, //!清除当期账目
};


//!故障
enum
{
	ERR_WAITCONNECT=9000,   //!等待与主机连接
	ERR_COUTING,            //!正在退币
	ERR_LOTTERYING,         //!正在退票
	ERR_SENDBILL,           //!正在报账
	ERR_SENDBILLOK,         //!报账成功
	ERR_SENDBILLWRONG,      //!报账失败
	ERR_MSG,                //!通信故障
	ERR_BILLWRONG,          //!分机需要与主机统一归零
	ERR_MSGDATA,            //!通信数据错误
	ERR_CHECKMSG,           //!通信测试中
	ERR_PCSERVER,           //!等待与电脑连接
	ERR_GAMEWRONG,          //!电脑游戏不匹配
	ERR_PCMSG,              //!电脑通信故障
	ERR_SYSTEMCLEAR,        //!系统故障,请归零
	ERR_RUNTIME,            //!运行时间到，请延时
	ERR_BILLOVER,           //!当期账目满，请报账
	ERR_SYSYTEMWRONG,       //!系统故障，请检修
};

#define S485MsgSize    16   //16
#define S485MsgBufferLen  80
#define S485RecTimeOut  3           //!10MS强制封包
extern INT8U S485MsgBeg;			//处理一个包，
extern INT8U S485MsgEnd;			//写入一个包，
extern INT8U S485Msg[S485MsgSize][S485MsgBufferLen];//数据缓冲区

typedef struct
{
	INT8U RevBufCount;     //!接收计数
	INT8U RevTimeOut;      //!接收超时
}S_S485;
extern S_S485 MyS485Msg;

extern xSemaphoreHandle xS485RecSemaphore;

extern void vGamePlayerTask( void *pvParameters );

extern INT16U Crc16Check(INT8U *pdata, INT8U len);


//===========================================================
//以下数据保存铁电，同时上传给PC

//通信板游戏结束后报账
typedef struct
{
	INT32S totalgain;				//全部总盈利
	INT32S totalgaincurrent;		//本次实际利润
	INT32U totalkaifen;				//上分
	INT32U totalxifen;				//下分
	INT32U totalcoin;				//投币
	INT32U totalcout;				//退币
	INT32U totallottery;			//彩票
	INT32U totalbet;				//当前总押分
	INT32U totalwin;				//当前总得分
	INT32U totalbounds;				//总彩金
	INT32U totalruntime;			//剩余运行时间
	INT32U totalgainpre;            //!上次总盈利
	INT32U totalbetin;              //!系统总押分
	INT32U totalbetout;             //!系统总得分
	INT32S totalreleasemoney;       //!系统抽放水
}S_REALTIMEBILL;

//参数调整
typedef struct
{
	INT32U grade;				//!系统难度 最容易0、容易1、难2、最难3、死难4
	INT32U bettime;     		//!押注时间   10-99
	INT32U bpmaxbet;			//!庄闲最大押分  100-9900
	INT32U hemaxbet;			//!和最大押分    100-5000
	INT32U othermaxbet;    		//!其他最大押分  100-5000
	INT32U betunitmin;      	//!最小押分切换  1、10、100、500、1000
	INT32U oddsselect;      	//!倍率选择 
	INT32U unitlottery;       	//!彩票比例  n 币 m 票  高16位表示币，低16位表示彩票
	INT32U macplace;        	//!机台摆放
}S_PARASET;

//解码调整
typedef struct
{
	INT32U unitcoin;			//!投币单位   1币 n 分
	INT32U spcaemode;           //!场地类型
	INT32U uncodedate;          //!打码天数
	INT32U uncodemsg;           //!打码提示信息
}S_UNCODESET;

//系统总账
typedef struct
{
	INT32S 	totalgain;     		//!全部总盈利
	INT32S 	totalgainpre; 		//!上次总盈利
	INT32S  totalgaincurrent;	//!本次实际利润
	INT32U  totalkaifen;        //!总上分
	INT32U  totalxifen;			//!总洗分
	INT32U 	totalcoin;			//!总投币
	INT32U 	totalcout;			//!总退币
	INT32U 	totallottery;		//!总退票
	INT32U  systemlineid;  		//!系统线号
	INT32U  macid;				//!机台号码
	INT32U 	totalruntime;		//!剩余运行时间
	INT32U 	uncodeday;			//!报码天数
}S_ALLBILL;

/*!分机账目*/
typedef struct
{
	INT32U slaveidcheck;		//!分机序列号
	INT32S totalgain;           //!总盈利
	INT32U totalkanfen;     	//!总上分
	INT32U totalxifen;			//!总洗分
	INT32U totalcoin;           //!总投币
	INT32U totalcout;           //!总退币
	INT32U totallottery;        //!总退彩票
	INT32U totalbetin;          //!总玩入
	INT32U totalbetout;         //!总玩出
	INT32U totalbonds;          //!全部彩金
	INT32S totalgaincurrent;    //!本次实际利润
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

/*!这些数据保存铁电中*/
/*!注意结构体对齐问题*/
/*!为了转换的方便，则数据进行分块存储,故数据进行了拆分*/
typedef struct
{
	S_REALTIMEBILL	realbill;          //!60字节
	S_PARASET		setpara;           //!36字节
	S_UNCODESET     setuncode;         //!16字节
	S_ALLBILL       systembill;        //!48字节
	INT32U          timedama;          //!通过该数据段判断系统是否是刚打码
	INT32U          hidebounds;        //!隐藏彩金池
	INT32U          gameseed;          //!游戏用到的随机数种子
	INT32U          codelock;          //!系统加密
	INT32U          vender;             //!线号   1
	INT32U          serialno;           //!机台序列号  2
	INT32U          version;            //!软件版本 高16位 低16位
	INT32U          password;           //!系统密码   3
	INT32U          codekernel;         //!解码计算公式 4
	INT32U          codeready;          //!报账打码
	INT32U          codecnt;            //!报码次数
	INT32U          codecheck;          //!报码校验码
	INT32U          adjpara;            //!调整难度，需要归零
	INT32U          soundvol;           //!声音大小  对齐
}S_KERNEL;//216字节
extern S_KERNEL MyKernel;

typedef struct
{
	INT32U          allyafen[13];       //!测试用押分  52字节
	S_DATE          codedate;           //!到期时间  8字节
	S_DATE          codedatecurrent;    //!当前时间  8字节
	INT8U           gamelist[20];       //!          20字节
	INT8U           compile_time[20];  	//获取系统编译时间,判断是否初始化重要参数  20字节
}S_RELIST; //108字节
extern S_RELIST MyRelist;

typedef struct
{
	INT16U          yafen[MAXSLAVE][14];//!分机押分信息  448字节
	INT32U          test;
}S_SLAVEYAFEN; //448
extern S_SLAVEYAFEN MySlaveYafen;


typedef struct
{
	S_SLAVEBILL     slavebill[MAXSLAVE];
	INT32U          test;
}S_SLAVEBILLGROUP; //704
extern S_SLAVEBILLGROUP MySlaveBill;


/*!游戏控制,发送数据给PC*/
typedef struct
{
	INT8U openshark;		//!是否出鲨鱼
	INT8U sharkcoor;        //!鲨鱼的坐标
	INT8U sharkpeilv;       //!鲨鱼的赔率
	INT8U secondopen;		//!二次开奖动物
	INT8U secondpeilv;		//!二次开奖动物赔率
	INT8U thirdopen;		//!三次开奖动物
	INT8U thirdpeilv;		//!三次开奖动物赔率
	INT8U boundpelv;		//!彩金赔率
	INT8U sharkcoorback;    //!鲨鱼坐标，保证不能重复
	INT8U secondopenback;   //!二次开奖动物备份，保证不能重复
	INT8U thirdopenback;    //!三次开奖动物备份，保证不能重复
}S_TRANSPC;
extern S_TRANSPC PC;

/*!算法控制中需要的分机数据*/
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
	INT16U bet[MAXTONGJILIST];		//!最大押分
	INT8U  open[MAXTONGJILIST];     //!最大押分的项
	INT8U  current;                 //!当前统计记录
}S_PLAYERBET;

typedef struct
{
	U_PLAYER MyPlayer;                  //玩家押分和余分情况
	S_PLAYERBET MyPlayerBet[MAXSLAVE];  //押分统计
}S_CTRGAME;

extern S_CTRGAME CtrGame;

typedef struct
{
	INT16U unitcoin;            //!币值
	INT32U baojifen;            //!爆机分值
	INT16U  maxbetzhuangxian;   //!庄闲最大押分
	INT16U  maxbetshark;        //!鲨鱼最大押分
	INT16U  maxbetanimal;       //!动物最大押分
	INT16U  minbetunit;         //!最小押分单位
	INT32U unitlottery;         //!彩票
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



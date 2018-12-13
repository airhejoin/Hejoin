/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：CPU卡或者M1卡操作
*设计者：南下三五年
*时  间：2012-05-24  00:53:19
*********************************************************************************
*/
#ifndef __TASKFM1702_H
#define __TASKFM1702_H


#define MAXPLAYERCARD    100


#define BUZZERON		    GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define BUZZEROFF			GPIO_ResetBits(GPIOA, GPIO_Pin_9)

#define MONEYBOXON    	 GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define MONEYBOXOFF		 GPIO_SetBits(GPIOC, GPIO_Pin_15)

/*********************************************************************************
	将该变量定义为16，会导致数组访问溢出，
	以前都是定义为16，由于某些未知原因，没有暴露出来，这次在修改程序的时候
	调整了数组在结构体中的位置，刚好影响到了后面的变量，而后面的变量则是至
	关重要的，故发现了此问题
	{

	数组之所以会越界，是因为FM1702操作数组的时候，16个字节的数据，后面还需
	要跟上命令字，故导致结构中的错误
	
	}

	南下三五年  2010-11-19  01:10:37	
*********************************************************************************/
#define FM1702BUFF      40      //注意该缓冲区的大小
/*
	IC卡用到的数据,合理分配数据结构，可以节省FLSAH空间
	CHAR     1字节对齐
	SHORT    2字节对齐
	INT      4字节对齐
*/
typedef struct
{
	INT32U id;              //卡号
	INT32U coin;            //币值
	INT32U oldpassword;     //旧密码
	INT32U newpassword;     //新密码
	INT32U money;           //币值转换为对应的钱
	INT32U word;            //修改密码
	INT32U limitup;         //上分限制
	INT8U  readbuff[FM1702BUFF];    //读卡缓冲区
	INT8U  writebuff[FM1702BUFF];   //写卡缓冲区
	INT8U  err;             //错误标记
	INT8U  mode;            //卡的类型
	INT8U  search;          //寻找卡
	INT8U  step;            //设置步骤
	INT8U  flash;           //选定闪烁
	INT16U unit;            //投币单位
	INT16U yajin;
}S_IC;
extern S_IC CtrIC;

/*
	系统标志
*/
typedef struct
{
	INT8U cardready;       //认证的卡是否就绪
	INT8U readorwrite;     //读或者写操作卡 0读 1写
	INT8U list;            //查询充值记录
	INT8U lock;            //系统锁
	INT8U cardok;          //卡正常
	INT8U addordec;        //充值或者减值 1表示充值 0表示减值
	INT8U bill;            //充值标志
	INT8U initpassword;    //初始化密码
	INT8U checkok;         //密码认证通过
	INT8U wordstep;        //设置密码步骤
	INT8U setting;         //设置进程
	INT8U waitcard;        //授权的卡在位
	INT8U setcoin;         //允许解锁卡设置币值
	INT8U desinbuf[16];     //DES输入数组
	INT8U desoutbuf[16];     //DES输出数组
	INT8U descheckbuf[16];   //DES校验数组
	INT8U desreadbuf[32];   //DES读M1卡缓冲区
	INT8U deswritebuf[32];  //DES写M1卡缓冲区
	INT8U coinerr;          //允许账目卡归零非法的M1卡
	INT8U desid[4];         //M1卡的ID参与DES加密，防止非法一张卡克隆多张

	INT8U id;

	INT8U msg;              /*更新需要显示的信息*/
	INT8U update;
	INT8U line;
	INT8U keymsg;
	INT32U code;
	INT8U syn;             //!同步字段，范围为1-31
	INT8U test;
	INT8U cmd;

	INT32U ps2word;
	INT16U ps2unit;
	INT32U ps2setword;

	/*!CPU卡用到的数据*/
	INT8U deskey[16];
	INT8U desout[32];

	INT8U clearlcd;
	INT8U backset;      //!后台参数调整中
	INT8U diserr;       //!错误提示一段时间后关闭
	
	/*!键盘用到标志*/
	INT8U step;         //!按键选择步骤
	INT8U billsetbit;   //!账目设置
	INT8U locksetbit;
	INT8U nolock;       //!是否更新系统锁
	INT16U noasktime;    //!服务器超时时间

	INT8U errlong;      //!文件长度错误
	INT8U lotteryorcoin;//!选择操作彩票或者币
	INT8U lotteryorcoinback;

	INT32U rec;
	INT32U send;
	INT8U serverok;     //!服务器正常
	INT8U tuibilock;
	INT8U tuibilock2;
	INT8U sendcoin;     //!赠币 1表示赠币，0表示正常
	INT8U sendcoinback;
	INT8U tuibiready;

	INT8U  serverask;
	INT8U  setsysbill;
	INT16U billcoin;   //!现金兑币值
	INT8U  enforcelogo;//!强制显示LOGO
	INT8U  settime;    //!调整时间
	INT8U  setdatestep;//!调整时钟步骤

	INT8U  macsyn[50];   //!机台注册后，生成同步字
	INT8U  mangercard;   //!管理会员卡状态
	INT8U  WriteCardStop; //!禁止写卡
	INT32U usbrec;
	INT8U  server_ready;		//!服务器就绪

	INT8U  ReaderMode;     //!读卡器模式 0 正常 1同步余额和彩票
}S_FLAG;
extern S_FLAG FLAG;

/*
	读卡或者写卡，0读，1写
*/
enum 
{
	READCARD=0,
	WRITCARD,
};
enum
{
	ECOIN = 0x50,
	ECOINREG = 0xa0,
	ECOINDEL = 0xb0,
};

/*
	卡操作过程中，状态指示
*/
enum
{
	QUEOK=0,
	ERR_NOCARD,
	ERR_COL,  		//卡冲突
	ERR_SELE,  		//选卡错误
	ERR_AUTHERR,  	//认证错误
	ERR_READ, 		//读卡错误
	ERR_WRI,	 	//写卡错误
	ERR_CRC,		//校验错误
	ERR_ADF, 		//余额超过限制
	ERR_SUE, 		//余额不足
	OK_REA,			//读卡成功
	OK_WRI,			//写卡成功
	ERR_PASS,		//密码错误
	OK_PASS,		//密码正确
	ERR_LEN,		//数据长度错误
};

/***********************M1卡操作故障*********************/
enum
{
	DISNOCARD=1,		/*无卡*/
	DISFIANDCARD,		/*有卡*/
	DISUPCOIN,			/*正在上分*/
	DISUPREADY,			/*上分成功*/
	DISDOWNCOIN,		/*正在下分*/
	DISDOWNREADY,		/*下分成功*/
	DISCOINLESS,		/*余额不足*/
	DISCOINOVERFLOW,	/*分值越限*/
	DISERRCHECK,		/*校验错误*/
	DISERRPASS,			/*密码错误*/
	DISRESUME,			/*恢复中*/
	DISERRCARDID,		/*卡号错误*/
	DISERRDES,			/*币值非法*/
	DISERRLINE,			/*掉线错误*/
	DISSETTING,			/*设置卡*/
	DISVIEWBILL,		/*查看账目*/
	DISVIEWSET,         /*!显示解锁设置画面*/
	DISERRDEFINE,		/*未定义卡*/
	DISSYSLOCK,         /*系统锁定*/
	DISSERNO,           /*显示系统串号*/

	DISMANGER,          /*!显示管理画面*/
	DISSLAVEBILL,       /*!显示卡头统计账目*/
};

enum
{
	DECCARD=0,				/*上分*/
	ADDCARD,				/*下分*/
};

/*****************************模拟投退币相关***************************/
typedef struct
{
	INT8U mode;             /*投退币模式*/
	INT8U stacoin;          /*投币信号常态电平*/
	INT8U stacout;			/*退币信号常态电平*/
	INT8U stassr;           /*退币使能信号常态电平*/
	INT8U speed;			/*投退币速度*/
	INT8U pulsecoin;		/*投币脉宽*/
	INT8U pulsecout;		/*退币脉宽*/
	INT8U gapcoin;			/*投币间隔*/
	INT8U gapcout;			/*退币间隔*/
	INT16U delaycoin;       /*投币脉冲保持时间*/
	INT16U delaycout;       /*退币脉冲保持时间*/
	INT8U bell;             /*蜂鸣器*/
	INT8U inorout;          /*上下分选择*/
	INT16U ssrok;           /*SSR稳定*/
	INT16U gapcoinkey;      /*投币间隔*/
	INT16U gapcoutkey;      /*退币间隔*/
}S_COIN;
extern S_COIN CTRCOIN;

/*
	玩家取币记录
*/
typedef struct
{
	INT32U id;               //!玩家卡号
	INT32U coin;             //!当前币值
	INT32U num;              //!玩家取币值
}S_PLAYERRELIST;

/***********************************主机实现服务器功能******************************/
//!一条会员卡的操作信息
typedef struct
{
	INT8U macid;           //!机台编号
	INT8U sta;             //!上下分
	INT16U credit;         //!会员操作记录
}S_LISTPLAYER;

//!会员的完整信息
typedef struct
{
	INT32U uid;             //!会员卡的内部序列号
	INT32U no;              //!会员卡的编号
	INT32U coin;            //!会员卡的余额
	INT32U index;           //!操作记录的索引
	S_LISTPLAYER  list[5];  //!会员卡的5次操作记录
}S_PLAYERMESSAGE;

//!会员在数据库里面的结构
typedef struct
{
	INT32U index;            //!会员信息索引,当索引为0，表示该索引无效，可以覆盖
	S_PLAYERMESSAGE message;//!会员信息
}S_SERVER;

enum
{
	LISTOPEN=1,              //!开分
	LISTCLOSE,               //!洗分
	LISTUPCOIN,              //!刷卡上分
	LISTDOWNCOIN,            //!刷卡下分
};

/*
	需要存储在铁电中的数据映射，如果增加变量，请不要改变以前的顺序
*/
typedef struct
{
	INT8U  compile_time[20];  	//获取系统编译时间,判断是否初始化重要参数
	INT32U codelock;          	//系统加密	
	INT8U  id;                  //本机ID
	INT32U password;			//密码
	INT32U seed;				//!随机数种子
}S_STEP;
extern S_STEP ST;

/*
	定时器2时间任务
*/
typedef struct
{
	INT8U buzzer;               //蜂鸣器 50MS
	INT32U opentime;            //!开箱时间
}S_TIME;
extern S_TIME TM;

/*
	解锁卡设置步骤
*/
enum
{
	STEPPASS=2,
	STEPUNIT,
	STEPLIMIT,
	STEPINIT,
	STEPMONEY,
	STEPYAJIN,
};

/*
	系统时钟参数
*/
typedef struct
{
	INT8U sec;
	INT8U min;
	INT8U hour;
	INT8U week;
	INT8U day;
	INT8U month;
	INT8U year;
} S_CLOCK;

extern S_CLOCK CLOCK;
extern S_CLOCK CLOCK1;


/*!人民币校验模块*/
typedef struct
{
	INT8U money;
	INT8U backmoney;
}S_ACCEPT;

extern S_ACCEPT ACCEPT;


#define CAN_RxSize 8		    	//Can接受缓冲区大小
extern INT8U CanMsgBeg;				//处理一个包，CanMsgBeg++
extern INT8U CanMsgEnd;				//写入一个包，CanMsgEnd++
extern CanRxMsg CanMsg[CAN_RxSize]; //CAN数据包接收缓冲区
extern void vFM1702MangerTask( void *pvParameters );
extern void InitCardPassword(void);
extern void StartBuzzer(void);
extern void OpenMoneyBox(void);

extern INT8U CanTxMsgBeg;			//处理一个包，CanMsgBeg++
extern INT8U CanTxMsgEnd;			//写入一个包，CanMsgEnd++
extern CanTxMsg CanTXMsg[CAN_RxSize];//CAN数据包接收缓冲区

extern INT8U CheckCardList(INT32U cardid);

extern xSemaphoreHandle xAskServerSemaphore;

#endif



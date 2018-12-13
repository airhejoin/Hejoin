/*
*********************************************************************************
*                                 STM32F103C8T6
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver1.1
*                                   皇家科技
*
*模  块：CAN协议解析
*设计者：Andy
*时  间：2010-10-19
*********************************************************************************
*/
#ifndef __TASK_CAN_H
#define __TASK_CNA_H

/*通信解析命令字*/
enum
{
	SETSLAVEID=1,    	//终端ID配置
	EXECUTEID,        	//ID配置结束
	SETTINGSYS,       	//参数设置
	DISSYS,        		//锁住终端
	ENABLESYS,      	//使能终端
	COINUP,         	//上分
	COINDOWN,        	//下分
	REQUESTCOIN,       	//请求总分	
	READYSYS,        	//终端就绪
	JIESUO,     		//请求解锁
	PEIZHI,     		//请求更新配置
	TOUBIOK,			//直接投币
	TUIBIOK,			//直接退币
	SENDCARDID,         //!发送卡的序列号和ID
	SYSTICK,            //!发送系统心跳包
	REFRESHCOIN,        /*!请求服务器同步玩家币值*/
	REFRESHLOTTERY,     /*!请求服务器同步玩家彩票*/
	ADDLOTTERY,        /*!增加彩票*/
	DECLOTTERY,        /*!减少彩票*/
	REQLOTTERY,        /*!请求彩票*/
#ifdef MOTHERCARD 
	ADDMONEY,          /*!主机给卡充值*/
	DECMONEY,          /*!主机给卡结账*/
#endif
#ifdef MONIGATE
	ADDMONEY,          /*!主机给卡充值*/
	DECMONEY,          /*!主机给卡结账*/
#endif

	/*!半自动售币机命令字*/
#ifdef SEMIAUTOMATIC
	PS2GETSET=0x16,     /*!请求币值*/
	PS2TUIBI,           /*!设置退币数量*/
	PS2STOP,            /*!强制停止出币*/
	PS2CLEAR,           /*!将当前已退币值归零*/
	PS2ERR,             /*!故障信息*/
	PS2TOTALBILL,       /*!查询主机的总退币数量*/
	PS2TUIBIOVER,       /*!退币正常结束*/
	PS2RESETTUIBI,      /*!补币完成，恢复退币*/

	PS2GETPASS,         /*!请求密码*/
	PS2SETPASS,         /*!设置新密码*/
	PS2SETUNIT,         /*!设置新的币值*/
	PS2SETMOOER,        /*!预留，马达反转*/
	PS2PASSOK,          /*!密码认证通过*/
	PS2PASSERR,         /*!密码认证失败*/
	PS2CLEARBILL,
	PS2KEYPASS,
	PS2KEYPRINT,        /*!启动打印机打印交接班账目*/
#endif
	 //!新增协议
    //!2012-12-05
	SENDPASSWORD=0x27,		//!密码认证，自动取币机
	RESETPASSWORD=0x28, 	//!重新定设定密码
	GETCOINCARD=0x29, 		//!用会员卡在自助售币机上取币
	GETCOINMONEY=0x2a, 		//!用现金在自动售币机上取币
	STORAGECOINCARD=0x2b, 	//!用会员卡存币
	SELLCOINSEMIMAC=0x2c, 	//!半自动售币机售币
	CASHADD=0x2d,			//!吧台主机充值 参数现金
    CASHDEC=0x2e,           //!吧台主机取现 参数现金
    OPENMONEYBOX=0x2f,      //!开钱箱


	CHECKDATE=0x30,     /*!校正时间*/

	/*!彩票打印机模块使用*/
	REQUESTBILL=0x40,    //!获取账单
	CLAERBILL,           //!清除当前账单
	REQUESTTOTALBILL,    //!获取记录账单
	GETBILL,             //!获得当前账单

	/*!人民币校验模块*/
	ACCEPT_MONEY=0x50,      //!正常接收人民币
	MOTHER_READY=0x51,      //!主机就绪
	REQUEST_MOTHER=0x52,    //!请求主机状态
	MOTHER_DIS=0x53,        //!主机忙
	TUIBIREADY=0x54,        //!可以接收新的纸币
	TUIBIBUSY=0x55,         //!正在退币，不接收新的纸币
	CHECKMONEY_ENABLE=0x56, //!使能纸币识别
	CHECKMONEY_DISABLE=0x57,//!除能纸币识别
	RMBREADY=0x58,          //!人民币就绪

    /*!打印专用命令*/
    PRINTBILL=0x60,         //!请求打印信息          
    PRINTCOUNT,             //!打印次数
    PRINTLAST,              //!上次账目
    PRINTCURRENT,           //!本次账目
    PRINTDATA,              //!时间

	//!2013-10-07 增加命令
	UpGuZhang = 0x70, 		 //上分故障
	DownGuZhang = 0x71,		 //下分故障
	AddCaiPiaoGuZhang = 0X72,  //增加彩票故障
	DecCaiPiaoGuZhang = 0x73,//减少彩票故障
	CashGetCoinGuZhang = 0x74,//现金取币故障
    CashChongZhiGuZhang = 0x75, //现金充值故障
    ReaderWrongList = 0x76,//!读卡器故障 后面跟1个故障代码 0 连续多次下分 1异常断电 2作弊报警
    ReaderParaMode = 0x77, //!读卡器参数更新方式
    ReaderGetPara = 0x78,  //!读卡器获取核心参数
	ReaderEnableSetXishu = 0x79, //!手动调整参数模式下，通过该命令才能调整核心参数
	ReaderChangeUnit = 0x7a,   //!调整卡头的切换单位
    ReaderCoreSet = 0x7b, //!卡头绑定卡、卡头报警[新增加命令]

};

/**!CAN和FM1702通信的数据包*/
typedef struct
{
	INT8U ucCMD;

}S_SERVERMSG;
extern S_SERVERMSG PCMSG;

extern void PackCanMsg(CanTxMsg SendMsg, INT8U cmd ,INT8U id, INT8U msgsyn);
extern void vCanprocTask( void *pvParameters );
extern void vCanSendTask( void *pvParameters );

#endif


























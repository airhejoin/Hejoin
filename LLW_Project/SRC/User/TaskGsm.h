#ifndef _TASKGSM_H_
#define _TASKGSM_H_
#define YUN_RSTL() GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define YUN_RSTH() GPIO_SetBits(GPIOA, GPIO_Pin_1)

//通信相关参数定义 重发时间为3S
enum
{
	CMD_SERVECOIN=0x6161,					//云模块投币指令 aa
	CMD_CLIENTSTATUS=0x6262,			//机台上传状态指令 0x62 bb
	CMD_CLIENTCOINCNT=0x6363,			//机台上传硬币计数命令 0x63 cc
	CMD_SERVESETPARA=0x6464,			//云模块设置机台参数0x64 dd
	CMD_SERVEREADPARA=0x6565,			//云模块读取机台参数0x65 ee 应答用dd
	CMD_SERVESETDEFAULT=0x6666,		//云模块设置机台恢复出厂 ff
	CMD_CLIENTONSYNC=0x6767,			//机台发送上电命令	gg
	CMD_CLIENTERROR=0x6868,				//机台发送故障命令hh
	CMD_CLIENTGIFTOUT=0x6969,			//机台上传已出礼品指令 ii
	CMD_SERVESTATUS=0x6a6a,				//云模块状态指令 jj
	CMD_SERVEONSYNC=0x6b6b,				//云模块上电同步指令 kk
	CMD_SERVERESET=0x6d6d,				//云模块控制礼品机1关机 2重启3清除数据命令 mm
	CMD_CLIENTREADID=0x616d,			//礼品机读取模块唯一码命令am
	CMD_CLIENTREADUSER=0x646d,		//礼品机读取厂家自定义参数命令dm
	CMD_CLIENTSETUSER=0x6a6d,			//礼品机写入厂家自定义参数命令jm
	CMD_CLIENTGETSERVERSTA=0x7070,//礼品机获取云模块状态pp
};

//云模块状态
enum{
	SERSTA_INIT=0,//模块初始化
	SERSTA_TESTUNCONN,	//测试模式,未连接到服务器1
	SERSTA_UNCONECTBS,//未连接到基站2
	SERSTA_APREGISTER,//AP注册模式，未设置wifi密码3
	SERSTA_UNCONNECTSER,//未连接到服务器4
	SERSTA_NORMAL,//已连接上服务器可正常使用5
	SERSTA_UNSENDDATA,//有数据未上传服务器6
	SERSTA_ERROR,//故障，此状态不支持移动支付消费7
	SERSTA_TESTCONN,//测试模式下，已经连接到测试服务器8
};
void GsmInit(void);
void vGsmTask( void *pvParameters );
void vGsmProcessTask( void *pvParameters );
void vGsmSendTask( void *pvParameters );
#endif




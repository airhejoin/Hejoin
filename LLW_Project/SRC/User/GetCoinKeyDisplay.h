#ifndef _GETCOINKEYDISPLAY_H_
#define _GETCOINKEYDISPLAY_H_

#define HARDFAULT_RETRUN_CHECK 
typedef struct
{
	INT32U First;       		        //!用来判读是否是第一次运行,用来初始化参数
	INT32U IapVersion;					//!IAP版本号
  INT32U SiteSn;                      //场地编号
	
	INT32 PassWord;
  INT8U PerCoinVal;//一元几币
	
	INT32U SurplusCoinBill;//余币数
	INT32U GiftCoinBill;//赠币账单
	INT32U OutCoinBill;//不计算赠币的出币总数
	INT32U TotalOutCoinBill;//总出币数
	INT32U MoneyTotalBill;//总金额
	
}S_SAVE;
extern S_SAVE FramSave;
//!系统变量
typedef struct
{
	INT32U Random;						//!通信加密
	INT8U OutMode;						//!退票或者退扭蛋
	INT8U SendErrClear;					//!故障清除发送数据给安卓
	INT8U SavePara;						//!保存全部参数
	INT8U SavePlayerCur;				//!保存玩家参数
	INT8U NeedAsk;						//!是否需要应答
	INT8U Mode;//工作的当前模式
}S_Flag;

extern S_Flag Flag;
enum
{
	C_TASKPRIO_vKeyProcessTask=1,
	C_TASKPRIO_vComProcessTask,
	C_TASKPRIO_vComSendTask,
	C_TASKPRIO_vTaskDisplay,
};
#endif

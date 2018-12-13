#ifndef _SET_H_
#define _SET_H_
#include ".\Lib\includes.h"
enum{
	SET_MAIN=0,
  SET_COINPLAY=0,  	//过关分数0
  SET_GIFTPROB,     //礼品杆概率设置1
  SET_HARDLEVEL,		//难度设置2
	SET_TIMERANGE,		//中奖设置3
  SET_GAMEVOL,      //背景音4
	SET_BACKVOL,      //背景音5
	SET_GIFTBARCHK,		//礼品杆测试6
	SET_BILLBROWSE,   //账目查询7
  SET_CLEARBILL,    //清账8
  SET_DEFAULT,      //恢复出厂9
	SET_VERSION,			//恢复出厂10
	SET_EXIT,					//退出11
  SET_MAX,					//12
};
enum{
	SETTYPE_ARROWS,
	SETTYPE_NUM,
};
typedef struct {
	u8 Item;
	u8 OldIndex;
	u8 Enable;
	u8 CurIndex;
	u8 Level;
	u8 OldLevel;
  u8 OldWinIndex;
  u8 WinIndex;//在屏幕中的有效的移动位置 0 1 2
	u8 DispType;
	u16 NumX;
	u16 NumY;
  u32 Value;
	u8 *ItemName[4];
}S_SET;
extern S_SET Set;
void SetProcess(void);
#endif


#ifndef _SET_H_
#define _SET_H_
#include ".\Lib\includes.h"
enum{
	SET_MAIN=0,
  SET_COINPLAY=0,  	//���ط���0
  SET_GIFTPROB,     //��Ʒ�˸�������1
  SET_HARDLEVEL,		//�Ѷ�����2
	SET_TIMERANGE,		//�н�����3
  SET_GAMEVOL,      //������4
	SET_BACKVOL,      //������5
	SET_GIFTBARCHK,		//��Ʒ�˲���6
	SET_BILLBROWSE,   //��Ŀ��ѯ7
  SET_CLEARBILL,    //����8
  SET_DEFAULT,      //�ָ�����9
	SET_VERSION,			//�ָ�����10
	SET_EXIT,					//�˳�11
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
  u8 WinIndex;//����Ļ�е���Ч���ƶ�λ�� 0 1 2
	u8 DispType;
	u16 NumX;
	u16 NumY;
  u32 Value;
	u8 *ItemName[4];
}S_SET;
extern S_SET Set;
void SetProcess(void);
#endif


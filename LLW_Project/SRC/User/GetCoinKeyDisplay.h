#ifndef _GETCOINKEYDISPLAY_H_
#define _GETCOINKEYDISPLAY_H_

#define HARDFAULT_RETRUN_CHECK 
typedef struct
{
	INT32U First;       		        //!�����ж��Ƿ��ǵ�һ������,������ʼ������
	INT32U IapVersion;					//!IAP�汾��
  INT32U SiteSn;                      //���ر��
	
	INT32 PassWord;
  INT8U PerCoinVal;//һԪ����
	
	INT32U SurplusCoinBill;//�����
	INT32U GiftCoinBill;//�����˵�
	INT32U OutCoinBill;//���������ҵĳ�������
	INT32U TotalOutCoinBill;//�ܳ�����
	INT32U MoneyTotalBill;//�ܽ��
	
}S_SAVE;
extern S_SAVE FramSave;
//!ϵͳ����
typedef struct
{
	INT32U Random;						//!ͨ�ż���
	INT8U OutMode;						//!��Ʊ������Ť��
	INT8U SendErrClear;					//!��������������ݸ���׿
	INT8U SavePara;						//!����ȫ������
	INT8U SavePlayerCur;				//!������Ҳ���
	INT8U NeedAsk;						//!�Ƿ���ҪӦ��
	INT8U Mode;//�����ĵ�ǰģʽ
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

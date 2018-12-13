#ifndef __FUNCTION_H
#define __FUNCTION_H

//ϵͳ�������ɵ�ʱ����ѵ��������
#define  FUNTIME   5
//���һ�������Ԫ�ظ���
#define GetItemSum( XX )	( sizeof( XX )/sizeof( XX[0] ) )
/*���ݽṹ����*/
typedef struct timer_func{	//����һ������
	void (*func)();			//ָ��ʱִ�еĳ���(��Ϊ����ĳ�������ж���,���Բ�׼ͣ���ĳ���)
	INT32U ticks;			//����ʱʱ��
	INT32U cnt;				//����ʱ������
	struct timer_func *Next;			
	struct timer_func *Prev;
}TIMER_FUNC;


/*��������*/
extern TIMER_FUNC TimerMission[FUNTIME];	
extern TIMER_FUNC *TimerFucFreeList; //������
extern TIMER_FUNC *TimerFucList;     //��������
extern void TimerFucInit(void);  
extern INT8U TimerFucCreat(void (*func)(TIMER_FUNC*),INT32U Ticks);
extern INT8U TimerFucDel(void (*func)());
extern void TimerFucPoll(void);

extern void MemoryCopy(INT8U *source, INT8U *target, INT8U length);
extern void Delay1ms(INT8U u);	

#endif



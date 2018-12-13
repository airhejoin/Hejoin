#ifndef __FUNCTION_H
#define __FUNCTION_H

//系统可以容纳的时间轮训任务数量
#define  FUNTIME   5
//获得一个数组的元素个数
#define GetItemSum( XX )	( sizeof( XX )/sizeof( XX[0] ) )
/*数据结构声明*/
typedef struct timer_func{	//建立一个链表
	void (*func)();			//指向定时执行的程序(因为里面的程序放在中断里,所以不准停留的程序)
	INT32U ticks;			//倒计时时间
	INT32U cnt;				//倒计时计数器
	struct timer_func *Next;			
	struct timer_func *Prev;
}TIMER_FUNC;


/*函数声明*/
extern TIMER_FUNC TimerMission[FUNTIME];	
extern TIMER_FUNC *TimerFucFreeList; //空链表
extern TIMER_FUNC *TimerFucList;     //任务链表
extern void TimerFucInit(void);  
extern INT8U TimerFucCreat(void (*func)(TIMER_FUNC*),INT32U Ticks);
extern INT8U TimerFucDel(void (*func)());
extern void TimerFucPoll(void);

extern void MemoryCopy(INT8U *source, INT8U *target, INT8U length);
extern void Delay1ms(INT8U u);	

#endif



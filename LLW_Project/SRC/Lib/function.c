#include"includes.h"

TIMER_FUNC TimerMission[FUNTIME];		//可以连接5个定时器的任务
TIMER_FUNC *TimerFucFreeList;
TIMER_FUNC *TimerFucList;
/*************    函数名：TimerFucInit        **************************
【功能】：初始化计时任务链表
【参数】：
【返回】：
【说明】：
**********************************************************************/
void TimerFucInit(void)						
{
	INT8U i;
	for(i=0;i<GetItemSum(TimerMission)-1;i++)	
	{
		TimerMission[i].Next=&TimerMission[i+1];
	}
	TimerMission[GetItemSum(TimerMission)-1].Next=(TIMER_FUNC*)0;
	TimerFucFreeList=&TimerMission[0];
	TimerFucList=(TIMER_FUNC*)0;
}
/*************    函数名：TimerFucCreat     ****************************
【功能】：添加定时器函数
【参数】：func 被定时执行的函数 ,Ticks 定时的时间
【返回】：
【说明】：
**********************************************************************/
INT8U TimerFucCreat(void (*func)(TIMER_FUNC*),INT32U Ticks)
{
	TIMER_FUNC* pTimerFuc;
	//IEC0bits.T1IE = 0;/* enable interrupts */
	if(TimerFucFreeList!=(TIMER_FUNC*)0)
	{
		pTimerFuc=TimerFucFreeList;
		if(TimerFucList!=(TIMER_FUNC*)0)	TimerFucList->Next=pTimerFuc;
		pTimerFuc->Prev=TimerFucList;
		TimerFucList=pTimerFuc;
		TimerFucFreeList=pTimerFuc->Next;
//		IEC0bits.T1IE = 1;/* enable interrupts */
		pTimerFuc->func=func;				//指向要运行的函数
		pTimerFuc->ticks=Ticks;				//设定等待的时间
		pTimerFuc->cnt=0;					//cnt;	//初始化计数器
		return 1;
	}
	//IEC0bits.T1IE = 1;/* enable interrupts */
	return 0;
}
/*************    函数名：TimerFucDel       ****************************
【功能】：删除定时函数
【参数】：func 函数地址,
		寻找列表中与函数地址相对应的列表项,删除
【返回】：
【说明】：
**********************************************************************/
INT8U TimerFucDel(void (*func)())
{
	//通过链表搜索,然后删除
	TIMER_FUNC* pTimerFuc;
	pTimerFuc=TimerFucList;
	while(pTimerFuc!=(TIMER_FUNC*)0)
	{
		if(pTimerFuc->func==func)	break;
		pTimerFuc=pTimerFuc->Prev;
	}
	if(pTimerFuc==(TIMER_FUNC*)0)	return 0;	//没有发现该程序
//	IEC0bits.T1IE = 0;/* enable interrupts */
	//del pTimerFuc in use link
	if(pTimerFuc->Prev!=(TIMER_FUNC*)0)
		pTimerFuc->Prev->Next=pTimerFuc->Next;
	if(pTimerFuc->Next!=(TIMER_FUNC*)0)	
		pTimerFuc->Next->Prev=pTimerFuc->Prev;
	if(pTimerFuc==TimerFucList)	TimerFucList=pTimerFuc->Prev;
	//add pTimerFuc in free link
	pTimerFuc->Next=TimerFucFreeList;
	TimerFucFreeList=pTimerFuc;
//	IEC0bits.T1IE = 1;/* enable interrupts */
	return 1;
}
/*************    函数名：TimerFucPoll      ****************************
【功能】：轮询即时任务
【参数】：
【返回】：
【说明】：
**********************************************************************/
void TimerFucPoll(void)		
{
	TIMER_FUNC* pTimerFuc;
	pTimerFuc=TimerFucList;
	while(pTimerFuc!=(TIMER_FUNC*)0)
	{
		if(++pTimerFuc->cnt>=pTimerFuc->ticks)
		{
			pTimerFuc->cnt=0;			//重新定时
			pTimerFuc->func(pTimerFuc);
		}
		pTimerFuc=pTimerFuc->Prev;
	}
}
/*************    函数名：Delay1ms          ****************************
【功能】：延时ms
【参数】：
【返回】：
【说明】：在外部晶振8M，4倍PLL，即系统时钟为8M的情况下，延时基准为1ms
**********************************************************************/
void Delay1ms(INT8U u)
{
	INT8U i=0;
	INT16U j=0;
	for(i=0;i<u;i++)
	{
		for(j=0;j<1280;j++)	  ;
		//	ClrWdt();
	}
}
/*************    函数名：MemoryCopy        ****************************
【功能】：内存复制函数
【参数】：
【返回】：
【说明】：
*********************************************************************************/
void MemoryCopy(INT8U *source, INT8U *target, INT8U length)
{
	do{
		*target++=*source++;
	}while(--length);
}


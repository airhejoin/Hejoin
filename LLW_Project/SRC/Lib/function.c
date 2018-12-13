#include"includes.h"

TIMER_FUNC TimerMission[FUNTIME];		//��������5����ʱ��������
TIMER_FUNC *TimerFucFreeList;
TIMER_FUNC *TimerFucList;
/*************    ��������TimerFucInit        **************************
�����ܡ�����ʼ����ʱ��������
����������
�����ء���
��˵������
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
/*************    ��������TimerFucCreat     ****************************
�����ܡ�����Ӷ�ʱ������
����������func ����ʱִ�еĺ��� ,Ticks ��ʱ��ʱ��
�����ء���
��˵������
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
		pTimerFuc->func=func;				//ָ��Ҫ���еĺ���
		pTimerFuc->ticks=Ticks;				//�趨�ȴ���ʱ��
		pTimerFuc->cnt=0;					//cnt;	//��ʼ��������
		return 1;
	}
	//IEC0bits.T1IE = 1;/* enable interrupts */
	return 0;
}
/*************    ��������TimerFucDel       ****************************
�����ܡ���ɾ����ʱ����
����������func ������ַ,
		Ѱ���б����뺯����ַ���Ӧ���б���,ɾ��
�����ء���
��˵������
**********************************************************************/
INT8U TimerFucDel(void (*func)())
{
	//ͨ����������,Ȼ��ɾ��
	TIMER_FUNC* pTimerFuc;
	pTimerFuc=TimerFucList;
	while(pTimerFuc!=(TIMER_FUNC*)0)
	{
		if(pTimerFuc->func==func)	break;
		pTimerFuc=pTimerFuc->Prev;
	}
	if(pTimerFuc==(TIMER_FUNC*)0)	return 0;	//û�з��ָó���
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
/*************    ��������TimerFucPoll      ****************************
�����ܡ�����ѯ��ʱ����
����������
�����ء���
��˵������
**********************************************************************/
void TimerFucPoll(void)		
{
	TIMER_FUNC* pTimerFuc;
	pTimerFuc=TimerFucList;
	while(pTimerFuc!=(TIMER_FUNC*)0)
	{
		if(++pTimerFuc->cnt>=pTimerFuc->ticks)
		{
			pTimerFuc->cnt=0;			//���¶�ʱ
			pTimerFuc->func(pTimerFuc);
		}
		pTimerFuc=pTimerFuc->Prev;
	}
}
/*************    ��������Delay1ms          ****************************
�����ܡ�����ʱms
����������
�����ء���
��˵���������ⲿ����8M��4��PLL����ϵͳʱ��Ϊ8M������£���ʱ��׼Ϊ1ms
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
/*************    ��������MemoryCopy        ****************************
�����ܡ����ڴ渴�ƺ���
����������
�����ء���
��˵������
*********************************************************************************/
void MemoryCopy(INT8U *source, INT8U *target, INT8U length)
{
	do{
		*target++=*source++;
	}while(--length);
}


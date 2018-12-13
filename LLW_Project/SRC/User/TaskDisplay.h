#ifndef _TASKDISPLAY_H_
#define _TASKDISPLAY_H_

#include "stm32f10x.h"

enum{
	LOW_DISP_PRIZE,	//��ʾ�н���	
	LOW_DISP_ERR,		//��ʾ����
	LOW_DISP_NOTE,	//��ʾ��ʾ
};
enum{
	DISPLAY_TYPE_NORMAL=0,
	DISPLAY_TYPE_SUCCEED,
	DISPLAY_TYPE_FAIL,
	DISPLAY_TYPE_SET,
	DISPLAY_TYPE_OTHER,
};
void vDisplayTask( void *pvParameters );
void MainDispProcess(void);
void MainPlayDis(void);
#endif

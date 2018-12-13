#ifndef _TASKDISPLAY_H_
#define _TASKDISPLAY_H_

#include "stm32f10x.h"

enum{
	LOW_DISP_PRIZE,	//显示中奖数	
	LOW_DISP_ERR,		//显示错误
	LOW_DISP_NOTE,	//显示提示
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

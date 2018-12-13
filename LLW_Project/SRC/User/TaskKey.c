#include".\Lib\includes.h"

/*************    函数名：vKeyPrecessTask      *************************
【功能】：按键采集处理任务
【参数】：
【返回】：
【说明】：
**************   设计：DAVID 2017-02-08  14:04:36***********************/
S_SETKEY Key;
u8 temp=0;
void vKeyProcessTask( void *pvParameters )//10ms采集一次
{
  u8 value=0;
  u8 i=0;
	for(;;)
	{
    DataSaveProcess();
		vTaskDelay(40);	
	}
}




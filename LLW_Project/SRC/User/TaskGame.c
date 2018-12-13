#include".\Lib\includes.h"

//加多闲时模式
void vGameTask( void *pvParameters )
{
	u16 i=0;
  for(;;)
  {
		switch(Flag.Step)
		{
			case STEP_IDLE:
				break;
			case STEP_SET:
				break;
		}
		vTaskDelay(10);
  }
}



























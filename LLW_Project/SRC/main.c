#include ".\Lib\includes.h"
#include "HardwareInit.h"
static void prvSetupHardware( void )
{
	ChipHalInit();
	ChipOutHalInit();
}
int main(void)
{
	prvSetupHardware();
	//串口接收信号量
	vSemaphoreCreateBinary( xRS485RecSemaphore );
	vSemaphoreCreateBinary( xRS485SendSemaphore );
	vSemaphoreCreateBinary( xGsmRecSemaphore );
	vSemaphoreCreateBinary( xGsmSendSemaphore );
	vSemaphoreCreateBinary( xTftCtlSemaphore );
	xRS485AskQueue=xQueueCreate(3,sizeof(INT8U));//深度为3，大小为8bit
	xGsmAskQueue=xQueueCreate(4,sizeof(INT8U));//深度为3，大小为8bit
	//串口处理任务
	xTaskCreate( vDisplayTask,    ( signed char * ) "Display", 256,0, C_TASKPRIO_vDisplayTask,    NULL );
  xTaskCreate( vKeyProcessTask,    ( signed char * ) "Key", 256,0, C_TASKPRIO_vKeyTask,    NULL );
	//TFT液晶屏任务
	xTaskCreate( vRS485ProcessTask,    ( signed char * ) "RS485Rx", 256,0, C_TASKPRIO_vRS485ProcessTask,    NULL );
  xTaskCreate( vRS485SendTask,    ( signed char * ) "RS485Tx", 256,0, C_TASKPRIO_vRS485SendTask,    NULL );	
	//支付模块任务
	xTaskCreate( vGsmTask,    ( signed char * ) "GsmTask", 256,0, C_TASKPRIO_vGsmTask,    NULL );	
	xTaskCreate( vGsmProcessTask,    ( signed char * ) "GsmRec", 256,0, C_TASKPRIO_vGsmRecTask,    NULL );	
	xTaskCreate( vGsmSendTask,    ( signed char * ) "GsmSend", 256,0, C_TASKPRIO_vGsmSendTask,    NULL );	
  vTaskStartScheduler();
	return 0;  
}

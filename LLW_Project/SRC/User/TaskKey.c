#include".\Lib\includes.h"

/*************    ��������vKeyPrecessTask      *************************
�����ܡ��������ɼ���������
����������
�����ء���
��˵������
**************   ��ƣ�DAVID 2017-02-08  14:04:36***********************/
S_SETKEY Key;
u8 temp=0;
void vKeyProcessTask( void *pvParameters )//10ms�ɼ�һ��
{
  u8 value=0;
  u8 i=0;
	for(;;)
	{
    DataSaveProcess();
		vTaskDelay(40);	
	}
}




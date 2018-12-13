#include"..\Lib\includes.h"
/*
HC245 I0~I7->A7~A0
LEFT->KEY1->245D.5 A2 ×ó
LEFT->KEY2->245D.6 A1 ÓÒ
LEFT->KEY3->245D.7 A0	°´¼ü
*/

INT8U HC245ReadBit(INT8U port,INT16U GPIO_Pin)
{
	INT8U sta=0;
	HC245ABCD_DIS();
	GPIO_ResetBits(GPIOC,0x0020>>port);
	if(GPIOA->IDR&GPIO_Pin)
		sta=1;
	else
		sta=0;
	return sta;
}
INT8U HC245ReadData(INT8U port)
{
	INT8U temp=0;
	HC245ABCD_DIS();
	GPIO_ResetBits(GPIOC,0x0020>>port);
	temp=GPIOA->IDR&0x00ff;
	return temp;
}




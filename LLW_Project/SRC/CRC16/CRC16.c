#include"..\Lib\includes.h"

//!CRC16校验
INT16U const crctab[16]=
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};

/******************************************************************************
 * 函数名:Crc16
 * 说明: 
 *    CRC16校验 
 * 输入: 
 * 输出: 
 * 返回: 
 * 设计:
 *****************************************************************************/
INT16U Crc16(INT8U *pdata, INT8U len)
{
	INT16U crc16=0;
	INT8U data;

	while(len--!=0)
	{
		data=crc16>>12;
		crc16<<=4;
		crc16^=crctab[data^(*pdata/16)];

		data=crc16>>12;
		crc16<<=4;
		crc16^=crctab[data^(*pdata&0x0f)];
		pdata++;
	}
	return (crc16);
}




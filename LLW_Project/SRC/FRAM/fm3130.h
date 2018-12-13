/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：IIC初始化配置
*设计者：南下三五年
*时  间：2012-05-24  01:03:27
*********************************************************************************
*/
#ifndef __FM3130_H
#define __FM3130_H

#define EEPROM_ADDR		0xA0
#define CLOCK_ADDR		0xD0
#define KEY_ADDR        0X1c20    //7K空间给主程序用  7200

#define ST_ADDR        0x10
#define ST_FIRST       0x05

extern void WriteFM3130(INT16U addr,INT8U data);
extern INT8U ReadFM3130(INT16U addr);
extern void BuffReadFM3130(INT16U addr, INT8U len, INT8U *ch);
extern void BuffWriteFM3130(INT8U *pbuff, INT32U addr, INT32U len);
extern void BuffReadFMRTC(INT8U addr,INT8U len, INT8U *ch);

extern void WriteFMRTC(INT8U addr,INT8U data);
extern INT8U ReadFMRTC(INT8U addr);


#define	SAVE3130(ST_NAME)	do{\
	portENTER_CRITICAL();\
	BuffWriteFM3130((INT8U*)&(ST_NAME), \
		ST_ADDR+((INT32U)(&(ST_NAME)))- (INT32U)&ST,\
	sizeof(ST_NAME));	\
	portEXIT_CRITICAL();\
}while(0)

#endif




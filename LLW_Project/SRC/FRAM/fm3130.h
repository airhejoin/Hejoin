/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺IIC��ʼ������
*����ߣ�����������
*ʱ  �䣺2012-05-24  01:03:27
*********************************************************************************
*/
#ifndef __FM3130_H
#define __FM3130_H

#define EEPROM_ADDR		0xA0
#define CLOCK_ADDR		0xD0
#define KEY_ADDR        0X1c20    //7K�ռ����������  7200

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




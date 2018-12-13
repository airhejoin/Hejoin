/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺IICʱ��PCF8563
*����ߣ�����������
*ʱ  �䣺2012-05-29  13:10:43
*********************************************************************************
*/
#ifndef __PCF8563_H
#define __PCF8563_H

#define READPCF8563   0xa3
#define WRITEPCF8563  0xa2

extern INT8U WritePCF8563(INT8U addr,INT8U data);
extern void GetSyeTime(void);

























#endif



/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺IICʱ��ISL1208
*����ߣ�����������
*ʱ  �䣺2012-05-29  13:10:43
*********************************************************************************
*/
#ifndef __ISL1208_H
#define __ISL1208_H

#define READISL1208   0xDF
#define WRITEISL1208  0xDE

extern INT8U WriteISL1208(INT8U addr,INT8U data);
extern void GetSyeTime(void);

























#endif



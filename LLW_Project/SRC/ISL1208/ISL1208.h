/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：IIC时钟ISL1208
*设计者：南下三五年
*时  间：2012-05-29  13:10:43
*********************************************************************************
*/
#ifndef __ISL1208_H
#define __ISL1208_H

#define READISL1208   0xDF
#define WRITEISL1208  0xDE

extern INT8U WriteISL1208(INT8U addr,INT8U data);
extern void GetSyeTime(void);

























#endif



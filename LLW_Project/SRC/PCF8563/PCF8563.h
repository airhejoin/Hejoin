/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：IIC时钟PCF8563
*设计者：南下三五年
*时  间：2012-05-29  13:10:43
*********************************************************************************
*/
#ifndef __PCF8563_H
#define __PCF8563_H

#define READPCF8563   0xa3
#define WRITEPCF8563  0xa2

extern INT8U WritePCF8563(INT8U addr,INT8U data);
extern void GetSyeTime(void);

























#endif



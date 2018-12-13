/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：CPU卡密钥头文件
*设计者：南下三五年
*时  间：2012-05-30  23:19:15
*说  明:
*        为了整个系统的安全，故该部门给出的是库文件
*********************************************************************************
*/

#ifndef  __PASSWORD_H
#define  __PASSWORD_H

//密钥文件
extern const unsigned char  KEY1[16];
extern const unsigned char  KEY2[16];
extern unsigned char  InterAute[8];

#endif


/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                             大脚丫电子工作室
*
*模  块：项目头文件
*设计者：David
*时  间：22:41:33, 2016年12月03日, David
*********************************************************************************
*/
#ifndef __FRUITFOISON_H
#define __FRUITFOISON_H
extern INT8U DesKey[16];
void TurnToSD(void);
void UpdateProgramFromSD(void);
void UpdateLKT4200FromSD(void);
#endif


/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：U6295初始化配置
*设计者：南下三五年
*时  间：2012-11-14  11:03:27
*********************************************************************************
*/
#ifndef __U6295_H
#define __U6295_H

#define U6295RD     GPIO_Pin_9
#define U6295WR     GPIO_Pin_8
#define U6295CS     GPIO_Pin_10
#define U6295RST    GPIO_Pin_12
#define U6295TIMER  GPIO_Pin_9

#define  SENDDATU6295(dat)   GPIOD->BSRR = (dat & 0xff) | (~dat & 0xff)<<16 

extern void InitU6295IO(void);
extern void StopMusic(INT8U channel);
extern void SendMusic(INT8U music, INT8U channel, INT8U reduction);
extern void GetStationU6295(void);



























#endif




/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：TM1629D头文件
*设计者：南下三五年
*时  间：2012-05-24  01:03:27
*********************************************************************************
*/
#ifndef __TM1629D_H
#define __TM1629D_H

//!使用GPIOC的低8位并行送数据
#define  SET_DATASEL(dat)   GPIOC->BSRR = (dat & 0x0f) | (~dat & 0x0f)<<16 

//口线重新定义
#define   TM1629DOUT	   GPIO_Pin_7
#define   TM1629CLK        GPIO_Pin_6
#define   TM1629DIN        GPIO_Pin_14

//口线输出
#define   TM1629DOUTH      GPIO_SetBits(GPIOC, TM1629DOUT)
#define   TM1629DOUTL      GPIO_ResetBits(GPIOC, TM1629DOUT)

#define   TM1629DCLKH      GPIO_SetBits(GPIOC, TM1629CLK)
#define   TM1629DCLKL      GPIO_ResetBits(GPIOC, TM1629CLK)


//命令字定义
#define    WRITEBUFFER    0x40
#define    READYKEY       0x42
#define    AUTOADDADDR    0x40
#define    LOCKADDR       0x44
#define    NORMALMODE     0x40
#define    TESTMODE       0x40

#define    SETADDR        0xc0

#define    SETPLUSE1      0x80
#define    SETPLUSE2      0x81
#define    SETPLUSE4      0x82
#define    SETPLUSE10     0x83
#define    SETPLUSE11     0x84
#define    SETPLUSE12     0x85
#define    SETPLUSE13     0x86
#define    SETPLUSE14     0x87

#define    DISOFF         0x80
#define    DISON          0x88

extern void InitTM1629DIO(void);
extern void SendByteTM1629D(INT8U data);
extern INT8U ReadDataTM1629D(void);
extern void InitTM1629D(void);
extern INT32U ScanKey(void);


























#endif




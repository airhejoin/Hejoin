/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺TM1629Dͷ�ļ�
*����ߣ�����������
*ʱ  �䣺2012-05-24  01:03:27
*********************************************************************************
*/
#ifndef __TM1629D_H
#define __TM1629D_H

//!ʹ��GPIOC�ĵ�8λ����������
#define  SET_DATASEL(dat)   GPIOC->BSRR = (dat & 0x0f) | (~dat & 0x0f)<<16 

//�������¶���
#define   TM1629DOUT	   GPIO_Pin_7
#define   TM1629CLK        GPIO_Pin_6
#define   TM1629DIN        GPIO_Pin_14

//�������
#define   TM1629DOUTH      GPIO_SetBits(GPIOC, TM1629DOUT)
#define   TM1629DOUTL      GPIO_ResetBits(GPIOC, TM1629DOUT)

#define   TM1629DCLKH      GPIO_SetBits(GPIOC, TM1629CLK)
#define   TM1629DCLKL      GPIO_ResetBits(GPIOC, TM1629CLK)


//�����ֶ���
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




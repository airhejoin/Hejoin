/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                               大脚丫电子工作室
*
*模  块：FM16W08
*说  明:
*        为了项目的管理，项目的全局变量声明放在这里，方便项目的后期维护，和多人
*        协作
*设计者：南下三五年
*时  间：2012-11-18  10:23:49
*修  改: 2013-06-25 
*        测试过程中，发现有些数据莫名其妙的错误，检查了主程序流程，没有发现异常，
*        屏蔽一些任务，问题依旧，后来调整了驱动部分，参数正常了
*        201d-06-25
*        测试中发现，仍有错误的部分，出现在频繁的断电上电，问题，应该出现在这里，
*        写过程尽量短，没有改变不写铁电，这样才可以尽量避免数据错误，如果数据错
*        从分机更新数据，或者清除铁电
*        修改:
*            频繁断电，发现总是在固定的位置出现错误，奇怪的问题，程序中使用了指针
*        检查了指针，没有发现错误，重新根据数据手册调整驱动部分
*        2013-06-28  问题确认
*        问题确认,经过测试，发现FM16W08的32个块，每一个块的首地址容易丢失数据,
*        所以驱动的设计上，要避免使用每一块的首地址
*********************************************************************************
*/
//!为了最大延长铁电的使用寿命，数据按照4字节对齐
#ifndef __FM16W08_H
#define __FM16W08_H

//!片选信号，低有效
#define FM16W08CEL		GPIO_SetBits(GPIOE, GPIO_Pin_8)
#define FM16W08CEH		GPIO_ResetBits(GPIOE, GPIO_Pin_8)

//!数据输出使能，低有效
#define FM16W08OEL		GPIO_ResetBits(GPIOE, GPIO_Pin_9)
#define FM16W08OEH		GPIO_SetBits(GPIOE, GPIO_Pin_9)

//!读写控制
#define FM16W08WEL		GPIO_ResetBits(GPIOE, GPIO_Pin_10)
#define FM16W08WEH		GPIO_SetBits(GPIOE, GPIO_Pin_10)

//!地址输出控制,13位地址
#define  OUTADDRFM16W08(addr)   GPIOD->BSRR = (addr & 0x1fff) | (~addr & 0x1fff)<<16

//!输出并口数据
#define  OUTDATAFM16W08(data)   GPIOE->BSRR = (data & 0xff) | (~data & 0xff)<<16 

#define  FISH_ADDR  				8
#define  GROUP1_ADDR  				256+8
#define  GROUP2_ADDR  			    512+8
#define  MYCODE_ADDR  			    768+8
#define  SLAVEBILL1_ADDR  			1024+8
#define  SLAVEBILL2_ADDR  			1280+8
#define  SLAVEBILL3_ADDR  			1536+8
#define  SLAVEBILL4_ADDR  			1792+8



extern void InitFM16W08Hard(void);
extern INT8U ReadDataFM16W08(INT16U addr,INT8U check);
extern void WriteDataFM16W08(INT16U addr, INT8U data, INT8U check);

extern void BuffReadFM16W08(INT16U addr, INT16U len, INT8U *ch,INT8U check);
extern void BuffWriteFM16W08(INT8U *pbuff, INT32U addr, INT32U len,INT8U check);


#define	SAVEFM16W08Fish(ST_NAME,CHECK)	do{\
	portENTER_CRITICAL();\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		FISH_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyFish.MyCode,\
	sizeof(ST_NAME),CHECK);	\
	portEXIT_CRITICAL();\
}while(0)


#define	SAVEFM16W08FishISR(ST_NAME,CHECK)	do{\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		FISH_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyFish.MyCode,\
	sizeof(ST_NAME),CHECK);	\
}while(0)

#define	SAVEFM16W08Group1(ST_NAME,CHECK)	do{\
	portENTER_CRITICAL();\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		GROUP1_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyFish.MyPlayerGroup1,\
	sizeof(ST_NAME),CHECK);	\
	portEXIT_CRITICAL();\
}while(0)


#define	SAVEFM16W08Group1ISR(ST_NAME,CHECK)	do{\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		GROUP1_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyFish.MyPlayerGroup1,\
	sizeof(ST_NAME),CHECK);	\
}while(0)

#define	SAVEFM16W08Group2(ST_NAME,CHECK)	do{\
	portENTER_CRITICAL();\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		GROUP2_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyFish.MyPlayerGroup2,\
	sizeof(ST_NAME),CHECK);	\
	portEXIT_CRITICAL();\
}while(0)


#define	SAVEFM16W08Group2ISR(ST_NAME,CHECK)	do{\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		GROUP2_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyFish.MyPlayerGroup2,\
	sizeof(ST_NAME),CHECK);	\
}while(0)

#ifdef TESTFRAM
#define	SAVEFM16W08MyCode(ST_NAME,CHECK)	do{\
	portENTER_CRITICAL();\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		MYCODE_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyCtrTime,\
	sizeof(ST_NAME),CHECK);	\
	portEXIT_CRITICAL();\
}while(0)


#define	SAVEFM16W08MyCodeISR(ST_NAME,CHECK)	do{\
	BuffWriteFM16W08((INT8U*)&(ST_NAME), \
		MYCODE_ADDR+((INT8U*)(&(ST_NAME)))- (INT8U*)&MyCtrTime,\
	sizeof(ST_NAME),CHECK);	\
}while(0)

#endif

































#endif



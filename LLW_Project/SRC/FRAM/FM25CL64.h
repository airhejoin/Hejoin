/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：FM25CL64初始化配置
*设计者：南下三五年
*时  间：2012-11-15  10:41:05
*********************************************************************************
*/
#ifndef __FM25CL64_H
#define __FM25CL64_H

//命令字宏定义
#define WREN_FM25CL        0X06    //!设置写使能寄存器
#define WRDI_FM25CL        0X04    //!写禁止
#define RDSR_FM25CL        0X05    //!读状态寄存器
#define WRSR_FM25CL        0X01    //!写状态寄存器
#define READ_FM25CL        0X03    //!读存储器
#define WRITE_FM25CL       0X02    //!写存储器

#define FM25CL64_CS_L      GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define FM25CL64_CL_H      GPIO_SetBits(GPIOA, GPIO_Pin_4)


#define ST_ADDR        0x100
#define ST_FIRST       0x00


extern void InitFM25CL64Hard(void);
extern INT8U ReadDataFM25CL64(INT16U addr);
extern void WriteDataFM25CL64(INT16U addr ,INT8U data);

extern void BuffWriteFM25CL64(INT8U *pbuff, INT16U addr, INT16U len);
extern void BuffReadFM25CL64(INT16U addr, INT8U *pbuff,  INT16U len);
extern void ClearFM25CL64(void);

//!应用程序调用
#define	SAVEFM25CL64(ST_NAME)	do{\
	portENTER_CRITICAL();\
	BuffWriteFM25CL64((INT8U*)&(ST_NAME), \
		ST_ADDR+((INT32U)(&(ST_NAME)))- (INT32U)&MyDisbuff.DISLED.myDis.ST,\
	sizeof(ST_NAME));	\
	portEXIT_CRITICAL();\
}while(0)

//!中断中调用
#define	SAVEFM25CL64ISR(ST_NAME)	do{\
	BuffWriteFM25CL64((INT8U*)&(ST_NAME), \
		ST_ADDR+((INT32U)(&(ST_NAME)))- (INT32U)&MyDisbuff.DISLED.myDis.ST,\
	sizeof(ST_NAME));	\
}while(0)























#endif

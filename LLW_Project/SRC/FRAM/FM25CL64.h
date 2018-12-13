/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺FM25CL64��ʼ������
*����ߣ�����������
*ʱ  �䣺2012-11-15  10:41:05
*********************************************************************************
*/
#ifndef __FM25CL64_H
#define __FM25CL64_H

//�����ֺ궨��
#define WREN_FM25CL        0X06    //!����дʹ�ܼĴ���
#define WRDI_FM25CL        0X04    //!д��ֹ
#define RDSR_FM25CL        0X05    //!��״̬�Ĵ���
#define WRSR_FM25CL        0X01    //!д״̬�Ĵ���
#define READ_FM25CL        0X03    //!���洢��
#define WRITE_FM25CL       0X02    //!д�洢��

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

//!Ӧ�ó������
#define	SAVEFM25CL64(ST_NAME)	do{\
	portENTER_CRITICAL();\
	BuffWriteFM25CL64((INT8U*)&(ST_NAME), \
		ST_ADDR+((INT32U)(&(ST_NAME)))- (INT32U)&MyDisbuff.DISLED.myDis.ST,\
	sizeof(ST_NAME));	\
	portEXIT_CRITICAL();\
}while(0)

//!�ж��е���
#define	SAVEFM25CL64ISR(ST_NAME)	do{\
	BuffWriteFM25CL64((INT8U*)&(ST_NAME), \
		ST_ADDR+((INT32U)(&(ST_NAME)))- (INT32U)&MyDisbuff.DISLED.myDis.ST,\
	sizeof(ST_NAME));	\
}while(0)























#endif

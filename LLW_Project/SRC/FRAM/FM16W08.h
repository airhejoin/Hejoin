/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                               ���Ѿ���ӹ�����
*
*ģ  �飺FM16W08
*˵  ��:
*        Ϊ����Ŀ�Ĺ�����Ŀ��ȫ�ֱ��������������������Ŀ�ĺ���ά�����Ͷ���
*        Э��
*����ߣ�����������
*ʱ  �䣺2012-11-18  10:23:49
*��  ��: 2013-06-25 
*        ���Թ����У�������Щ����Ī������Ĵ��󣬼�������������̣�û�з����쳣��
*        ����һЩ�����������ɣ������������������֣�����������
*        201d-06-25
*        �����з��֣����д���Ĳ��֣�������Ƶ���Ķϵ��ϵ磬���⣬Ӧ�ó��������
*        д���̾����̣�û�иı䲻д���磬�����ſ��Ծ����������ݴ���������ݴ�
*        �ӷֻ��������ݣ������������
*        �޸�:
*            Ƶ���ϵ磬���������ڹ̶���λ�ó��ִ�����ֵ����⣬������ʹ����ָ��
*        �����ָ�룬û�з��ִ������¸��������ֲ������������
*        2013-06-28  ����ȷ��
*        ����ȷ��,�������ԣ�����FM16W08��32���飬ÿһ������׵�ַ���׶�ʧ����,
*        ��������������ϣ�Ҫ����ʹ��ÿһ����׵�ַ
*********************************************************************************
*/
//!Ϊ������ӳ������ʹ�����������ݰ���4�ֽڶ���
#ifndef __FM16W08_H
#define __FM16W08_H

//!Ƭѡ�źţ�����Ч
#define FM16W08CEL		GPIO_SetBits(GPIOE, GPIO_Pin_8)
#define FM16W08CEH		GPIO_ResetBits(GPIOE, GPIO_Pin_8)

//!�������ʹ�ܣ�����Ч
#define FM16W08OEL		GPIO_ResetBits(GPIOE, GPIO_Pin_9)
#define FM16W08OEH		GPIO_SetBits(GPIOE, GPIO_Pin_9)

//!��д����
#define FM16W08WEL		GPIO_ResetBits(GPIOE, GPIO_Pin_10)
#define FM16W08WEH		GPIO_SetBits(GPIOE, GPIO_Pin_10)

//!��ַ�������,13λ��ַ
#define  OUTADDRFM16W08(addr)   GPIOD->BSRR = (addr & 0x1fff) | (~addr & 0x1fff)<<16

//!�����������
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



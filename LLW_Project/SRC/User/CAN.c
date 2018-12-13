/*
*********************************************************************************
*                                 STM32F103C8T6
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver1.1
*                                   �ʼҿƼ�
*
*ģ  �飺CAN
*����ߣ�Andy
*ʱ  �䣺2010-07-21
*********************************************************************************
*/
#include".\Lib\includes.h"

void CAN_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	
	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 


	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	/*125K������*/
	CAN_InitStructure.CAN_TTCM=DISABLE;	//ʱ�䴥��ģʽ
	CAN_InitStructure.CAN_ABOM=ENABLE;	//ʹ�����߹��� 
	CAN_InitStructure.CAN_AWUM=DISABLE;	//�Զ�����
	CAN_InitStructure.CAN_NART=DISABLE;	//���Զ��ش�
	CAN_InitStructure.CAN_RFLM=DISABLE;	//FIFO����
	CAN_InitStructure.CAN_TXFP=DISABLE;	//FIFO���ȼ�  
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal  ;		//��������ģʽ CAN_Mode_LoopBack 
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;			//1-4
	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq;			//1-16
	CAN_InitStructure.CAN_BS2=CAN_BS2_1tq;			//1-8
	CAN_InitStructure.CAN_Prescaler=32;				//������Ϊ 16/(32*(1+7+1))=125k
	CAN_Init(CAN1,&CAN_InitStructure);
	
	/* CAN ���������� */
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0;//ֻ���ձ�������
	CAN_FilterInitStructure.CAN_FilterIdLow=((u32)FLAG.id)<<3|CAN_ID_EXT; //
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//���˵�ַ
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x07fc;//
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	//���չ㲥����
	CAN_FilterInitStructure.CAN_FilterNumber=1;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=((u32)FLAG.id)<<3|CAN_ID_EXT; //
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x00;//���չ㲥��Ϣ
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x07fc;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* ����FMP0�ж�*/ 
	CAN_ITConfig(CAN1,CAN_IT_TME|CAN_IT_FMP0, DISABLE);

}


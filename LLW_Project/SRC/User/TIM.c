#include"Tim.h"
/*************    ��������TIM_Configuration       *********************
�����ܡ���
����������
�����ء���
��˵������
***************************************************************************/
void TimInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* TIM2 ����*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* ��������*/
	TIM_TimeBaseStructure.TIM_Period = 50;			    //����ֵ,5MS 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
	/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	/* 4��ͨ��������������ж�*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
	/* ����TIM2��ʼ���� */
	TIM_Cmd(TIM2, ENABLE);
	
	/* ����TIM3*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* ��������*/
	TIM_TimeBaseStructure.TIM_Period = 20;			    //����ֵ,2MS 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
	/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
	/* 4��ͨ��������������ж�*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	
	/* ����TIM2��ʼ���� */
	TIM_Cmd(TIM3, ENABLE);
	
	/* ����TIM4*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/* ��������*/
	TIM_TimeBaseStructure.TIM_Period = 10;			    //����ֵ,1MS 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		
	/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
	/* 4��ͨ��������������ж�*/
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	
	/* ����TIM2��ʼ���� */
	TIM_Cmd(TIM4, ENABLE);
}


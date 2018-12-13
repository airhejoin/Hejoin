/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺U6295��ʼ������
*����ߣ�����������
*ʱ  �䣺2012-11-14  11:03:27
*********************************************************************************
*/
#include"..\Lib\includes.h"

/*************    ��������InitU6295IO             ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-13  17:01:41***********************/
void InitU6295IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = U6295CS|U6295RD|U6295RST|U6295WR
		|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
		|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	GPIO_SetBits(GPIOD, U6295CS);
	GPIO_SetBits(GPIOD, U6295RD);
	GPIO_SetBits(GPIOD, U6295RST);
	GPIO_SetBits(GPIOD, U6295WR);
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
	
	GPIO_ResetBits(GPIOD, U6295RST);
	vTaskDelay(100);
	GPIO_SetBits(GPIOD, U6295RST);

	SENDDATU6295(0xff);
}

/*************    ��������Delay50us               ****************************
�����ܡ���Ӳ��ʱ��Ҫ������500NS
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-14  15:25:35***********************/
void Delay50us(void)
{
	INT16U i=0;

	for(i=0;i<36;i++)
		__NOP();
}
/*************    ��������SendMusic                ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-13  17:35:32***********************/
void SendMusic(INT8U music, INT8U channel, INT8U reduction)
{
	INT16U data=0;
	
	data=0x80|music;
	
	GPIO_ResetBits(GPIOD, U6295CS);  //����CS��Ƭѡ
	GPIO_ResetBits(GPIOD, U6295WR);  //���ݾ���
	SENDDATU6295(data);              //����׼������
	Delay50us();
	GPIO_SetBits(GPIOD, U6295WR);    //����ȡ��
	GPIO_SetBits(GPIOD, U6295CS);
	
	GPIO_ResetBits(GPIOD, U6295CS);
	GPIO_ResetBits(GPIOD, U6295WR);
	data=(channel<<4) + reduction;
	SENDDATU6295(data);
	Delay50us();
	GPIO_SetBits(GPIOD, U6295WR);
	GPIO_SetBits(GPIOD, U6295CS);
	data=0xff;
	SENDDATU6295(data);
}

/*************    ��������StopMusic                ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-13  17:42:05***********************/
void StopMusic(INT8U channel)
{
	INT16U data=0;
	GPIO_ResetBits(GPIOD, U6295CS);
	GPIO_ResetBits(GPIOD, U6295WR);
	data=1<<(2+channel);
	SENDDATU6295(data);
	Delay50us();
	GPIO_SetBits(GPIOD, U6295WR);  
	GPIO_SetBits(GPIOD, U6295CS);
	data=0xff;
	SENDDATU6295(data);
}

/*************    ��������GetStationU6295         ****************************
�����ܡ����ж������Ƿ񲥷����
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-16  10:03:13***********************/
void GetStationU6295(void)
{
	INT16U i=0;
	
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
		                          GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	for(i=0;i<0xffff;i++)
	{
		GPIO_ResetBits(GPIOD, U6295CS);  //����CS��Ƭѡ
		GPIO_ResetBits(GPIOD, U6295RD);  //��ʼ��
		Delay50us();
		
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0))
		{
			break;
		}
		
		GPIO_SetBits(GPIOD, U6295RD);    //������
		GPIO_SetBits(GPIOD, U6295CS);

		vTaskDelay(10);//Delay50us();
	}
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
		                          GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	vTaskDelay(50);
}





































#include"RCC.h"

/*************    ��������RCC_Configuration          *************************
�����ܡ���ϵͳʱ������
����������
�����ء���
��˵������
         ʹ���ڲ�8M��RC������9����Ƶ��ϵͳʱ��Ϊ36M
*********************************************************************************/
void RccInit(void)
{
#if 1
	RCC_ClocksTypeDef RCC_ClockFreq;
	SystemInit();//Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC������.�����뿴2_RCC

	/**************************************************
	��ȡRCC����Ϣ,������
	��ο�RCC_ClocksTypeDef�ṹ�������,��ʱ��������ɺ�,
	���������ֵ��ֱ�ӷ�ӳ�������������ֵ�����Ƶ��
	***************************************************/
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/* ������ÿ�ʹ�ⲿ����ͣ���ʱ��,����һ��NMI�ж�,����Ҫ�õĿ����ε�*/
//	RCC_ClockSecuritySystemCmd(ENABLE);
#else 
  	//������ RCC�Ĵ�������Ϊȱʡֵ
  RCC_DeInit();
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	{
	}
  if(1)
	{
 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
	 
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
	 
		RCC_PCLK2Config(RCC_HCLK_Div1);
	
		RCC_PCLK1Config(RCC_HCLK_Div1);
	 
		//���� PLL ʱ��Դ����Ƶϵ�� �ڲ�RC��󲻳���64M
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);
	
							 
		//ʹ�ܻ���ʧ�� PLL,�����������ȡ��ENABLE����DISABLE
		RCC_PLLCmd(ENABLE);//���PLL������ϵͳʱ��,��ô�����ܱ�ʧ��
		//�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		//����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
		//�ȴ�PLL�ɹ�������ϵͳʱ�ӵ�ʱ��Դ
		//  0x00��HSI ��Ϊϵͳʱ��
		//  0x04��HSE��Ϊϵͳʱ��
		//  0x08��PLL��Ϊϵͳʱ��  
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
#endif
}

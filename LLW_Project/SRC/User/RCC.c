#include"RCC.h"

/*************    函数名：RCC_Configuration          *************************
【功能】：系统时钟配置
【参数】：
【返回】：
【说明】：
         使用内部8M的RC振荡器，9倍倍频，系统时钟为36M
*********************************************************************************/
void RccInit(void)
{
#if 1
	RCC_ClocksTypeDef RCC_ClockFreq;
	SystemInit();//源自system_stm32f10x.c文件,只需要调用此函数,则可完成RCC的配置.具体请看2_RCC

	/**************************************************
	获取RCC的信息,调试用
	请参考RCC_ClocksTypeDef结构体的内容,当时钟配置完成后,
	里面变量的值就直接反映了器件各个部分的运行频率
	***************************************************/
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/* 这个配置可使外部晶振停振的时候,产生一个NMI中断,不需要用的可屏蔽掉*/
//	RCC_ClockSecuritySystemCmd(ENABLE);
#else 
  	//将外设 RCC寄存器重设为缺省值
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
	 
		//设置 PLL 时钟源及倍频系数 内部RC最大不超过64M
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);
	
							 
		//使能或者失能 PLL,这个参数可以取：ENABLE或者DISABLE
		RCC_PLLCmd(ENABLE);//如果PLL被用于系统时钟,那么它不能被失能
		//等待指定的 RCC 标志位设置成功 等待PLL初始化成功
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		//设置系统时钟（SYSCLK） 设置PLL为系统时钟源
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
		//等待PLL成功用作于系统时钟的时钟源
		//  0x00：HSI 作为系统时钟
		//  0x04：HSE作为系统时钟
		//  0x08：PLL作为系统时钟  
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
#endif
}

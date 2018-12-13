#include"Tim.h"
/*************    函数名：TIM_Configuration       *********************
【功能】：
【参数】：
【返回】：
【说明】：
***************************************************************************/
void TimInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* TIM2 配置*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 50;			    //计数值,5MS 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	/* 4个通道和溢出都配置中断*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM2, ENABLE);
	
	/* 配置TIM3*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 20;			    //计数值,2MS 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
	/* 4个通道和溢出都配置中断*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM3, ENABLE);
	
	/* 配置TIM4*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 10;			    //计数值,1MS 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		
	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
	/* 4个通道和溢出都配置中断*/
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM4, ENABLE);
}


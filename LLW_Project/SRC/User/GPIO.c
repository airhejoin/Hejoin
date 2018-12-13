#include"includes.h"
/******************************************************************************
 * 函数名:GPIO_Configuration
 * 说明: 
 *    IO口初始化 
 * 输入: 
 * 输出: 
 * 返回: 
 *****************************************************************************/
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*允许总线CLOCK,在使用GPIO之前必须允许相应端的时钟.
	从STM32的设计角度上说,没被允许的端将不接入时钟,也就不会耗能,
	这是STM32节能的一种技巧,*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁止jtag,启用swd
	//IO口输出配置
  
  PWR->CR|=1<<8;          //取消备份区写保护
  RCC->BDCR&=0xFFFFFFFE;  //外部低速振荡器关闭，PC14,PC15作为普通IO口 
  BKP->CR&=0xFFFFFFFE;    //侵入检测TAMPERPC13作为普通IO口 
  PWR->CR&=0xFFFFFEFF;    //备份区写保护
  
	//GPIOA输出 DO4->PA0 DO3->PA1 DO2->PA2 DO1->PA3 USBEN->PA15 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
	//GPIOB输出 MO3->PB0 MO4->PB1 MO5->PB2 245OE2->PB8 245OE3->PB9  
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	
	//GPIOC输出配置   DO8->PC0 DO7->PC1 DO6->PC2 DO5->PC3 MO1->PC4 MO2->PC5 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  //GPIOD输出配置 573LE1->PD1 573LE2->PD3 485EN->PD10  sta led->PD11 run led->PD12  
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	RS485RxEn();

	//GPIOE输出配置 245OE3->PE0 245OE4->PE1 MO6->PE7 MO7->PE8 MO8->PE9 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  
//IO口输入配置
  //GPIOA输入配置
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//下拉输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//GPIOB输入配置PB3->Key4 PB4->Key5 PB5->Key6 PB6->Key7 PB7->Key8 
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//下拉输入
//	GPIO_Init(GPIOB, &GPIO_InitStructure);


  //GPIOC输入配置 DI6->PC13  DI6->PC14 DI6->PC15 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//下拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
	
  //GPIOD输入配置  YC_BUSY2->PD0 YC_BUSY1->PD4 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//下拉输入
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	//GPIOE输入配置 DI1->PE2 DI2->PE3 DI3->PE4 DI4->PE5 DI5->PE6
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//下拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);
	GPIO_ResetBits(GPIOE,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
  RunLedOff();
  StaLedOff();
}





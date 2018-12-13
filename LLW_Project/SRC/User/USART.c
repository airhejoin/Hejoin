#include ".\Lib\includes.h"
#include "Stdio.h"


void UsartInit(void)
{
  u32 APB2temp=0,APB1temp=0;
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
  #if USART1_USE
  APB2temp=RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA;
  #endif
  
  #if USART2_USE
  APB1temp=RCC_APB1Periph_USART2;
  APB2temp|=RCC_APB2Periph_GPIOD;
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE); 
  #endif  
  
  #if USART3_USE
  APB1temp|=RCC_APB1Periph_USART3;
  APB2temp|=RCC_APB2Periph_GPIOD;
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE); 
  #endif
  
  #if UART4_USE
  APB1temp|=RCC_APB1Periph_UART4;
  APB2temp|=RCC_APB2Periph_GPIOC;
  #endif  
  
	#if UART5_USE
  APB1temp|=RCC_APB1Periph_UART5;
  APB2temp|=RCC_APB2Periph_GPIOC;
	APB2temp|=RCC_APB2Periph_GPIOD;
  #endif  
	
	RCC_APB2PeriphClockCmd(APB2temp|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(APB1temp, ENABLE);
  
  
#if USART1_USE //串口1
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	//USART1_TX   PA.9  remapPB6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	//USART1_RX	  PA.10 remap PB7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
  
  //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1
#endif
  
  
  #if USART2_USE //串口2
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
  //USART2_TX   PA2 rempa PD5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//USART2_RX	  PA3 remap PD6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(USART2, ENABLE);                    //使能串口1
#endif 
  
#if USART3_USE //串口3
  //USART3_TX   PD8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//USART3_RX	  PD9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
  
  //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(USART3, ENABLE);                    //使能串口1
#endif
  
#if UART4_USE //串口4
  //UART4_TX   PC10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//UART4_RX	  PC11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
  //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART4, &USART_InitStructure);
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(UART4, ENABLE);                    //使能串口1
#endif

#if UART5_USE //串口5
  //UART5_TX   PC12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//UART5_RX	  PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART5, &USART_InitStructure);
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(UART5, ENABLE);                    //使能串口1
#endif
}

/*定义 fputc 此函数为printf所用*/
int fputc(int ch, FILE *f) 
{ 
    //USART1->DR = (u8) ch; 
    //while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);		
    return ch; 
}
void Usart1SendByte(u8 dat)
{
	USART_SendData(USART1,dat);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);	
}
void Usart1SendString(u8 *str,u16 len)
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1,str[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);		
	}
}
void Usart2SendString(u8 *str,u16 len)
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART2,str[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)!=SET);		
	}
}
void Usart3SendString(u8 *str,u16 len)
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART3,str[i]);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)!=SET);		
	}
}
void Uart4SendString(u8 *str,u16 len)
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		USART_SendData(UART4,str[i]);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)!=SET);		
	}
}
void Uart5SendString(u8 *str,u16 len)
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		USART_SendData(UART5,str[i]);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC)!=SET);		
	}
}



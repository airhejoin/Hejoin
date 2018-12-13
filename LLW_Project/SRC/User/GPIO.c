#include"includes.h"
/******************************************************************************
 * ������:GPIO_Configuration
 * ˵��: 
 *    IO�ڳ�ʼ�� 
 * ����: 
 * ���: 
 * ����: 
 *****************************************************************************/
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*��������CLOCK,��ʹ��GPIO֮ǰ����������Ӧ�˵�ʱ��.
	��STM32����ƽǶ���˵,û������Ķ˽�������ʱ��,Ҳ�Ͳ������,
	����STM32���ܵ�һ�ּ���,*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//��ֹjtag,����swd
	//IO���������
  
  PWR->CR|=1<<8;          //ȡ��������д����
  RCC->BDCR&=0xFFFFFFFE;  //�ⲿ���������رգ�PC14,PC15��Ϊ��ͨIO�� 
  BKP->CR&=0xFFFFFFFE;    //������TAMPERPC13��Ϊ��ͨIO�� 
  PWR->CR&=0xFFFFFEFF;    //������д����
  
	//GPIOA��� DO4->PA0 DO3->PA1 DO2->PA2 DO1->PA3 USBEN->PA15 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
	//GPIOB��� MO3->PB0 MO4->PB1 MO5->PB2 245OE2->PB8 245OE3->PB9  
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	
	//GPIOC�������   DO8->PC0 DO7->PC1 DO6->PC2 DO5->PC3 MO1->PC4 MO2->PC5 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  //GPIOD������� 573LE1->PD1 573LE2->PD3 485EN->PD10  sta led->PD11 run led->PD12  
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	RS485RxEn();

	//GPIOE������� 245OE3->PE0 245OE4->PE1 MO6->PE7 MO7->PE8 MO8->PE9 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  
//IO����������
  //GPIOA��������
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//GPIOB��������PB3->Key4 PB4->Key5 PB5->Key6 PB6->Key7 PB7->Key8 
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
//	GPIO_Init(GPIOB, &GPIO_InitStructure);


  //GPIOC�������� DI6->PC13  DI6->PC14 DI6->PC15 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
	
  //GPIOD��������  YC_BUSY2->PD0 YC_BUSY1->PD4 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	//GPIOE�������� DI1->PE2 DI2->PE3 DI3->PE4 DI4->PE5 DI5->PE6
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);
	GPIO_ResetBits(GPIOE,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
  RunLedOff();
  StaLedOff();
}





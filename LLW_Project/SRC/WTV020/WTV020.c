#include"WTV020.h"

//ASDA->C1;
//ASCL->C0;

//BSDA->C3;
//BSCL->C2;
void Delay200us()
{
	unsigned char i, j;
	i = 5;
	j = 74;
	do
	{
		while (--j);
	} while (--i);
}
void WTV020Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	ASDA_H;
	ASCL_H;
	BSDA_H;
	BSCL_H;
}
void WTV020ASend(u8 addr)
{
	u8 i=0;
	__disable_irq(); 
	for(i=0;i<8;i++)
	{
		ASCL_L;
		Delay200us();
		if(addr&0x80)
			ASDA_H;
		else
			ASDA_L;
		addr<<=1;
		Delay200us();
		ASCL_H;
		Delay200us();
	}
	ASDA_H;
	__enable_irq();
}
void WTV020BSend(u8 addr)
{
	u8 i=0;
	__disable_irq();
	for(i=0;i<8;i++)
	{
		BSCL_L;
		Delay200us();
		if(addr&0x80)
			BSDA_H;
		else
			BSDA_L;
		addr<<=1;
		Delay200us();
		BSCL_H;
		Delay200us();
	}
	BSDA_H;
	__enable_irq();
}









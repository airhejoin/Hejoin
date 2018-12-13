#include"WS281X.h"

S_LEDINFO LedLamp={LAMP_IDLE_NORMAL,0,{COLOR_DARK},{0xAA}};
extern void LedGameMode(void);
void WS281xInit(void)
{
	u32 i=0;
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE  , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 , ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_7);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	

	DMA_DeInit(DMA2_Channel2);
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI3->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)LedLamp.PixelBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_Init(DMA2_Channel2, &DMA_InitStructure); /* DMA1 CH3 = MEM -> DR */

	SPI_Cmd(SPI3, DISABLE);
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=0 数据捕获第1个
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; /* 72MHz / 8 = 9MHz */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStructure);

	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);

	SPI_Cmd(SPI3, ENABLE);

	LedLampOff();
}
void LedLampOff(void)
{
	u16 i=0;
	for(i=0;i<GROUPSUM_MAX;i++)
	{
		LedLamp.GroupColor[i]=COLOR_DARK;
	}
	WS281xDMAUpdate(LedLamp.GroupColor,GROUPSUM_MAX);
}
//发送一个像素点
void WS281xSendPixel(u32 color)
{
	u8 i;
	for(i=0;i<24;i++)
	{
		if(color&0x800000)
		{
			LedLamp.PixelBuffer[LedLamp.PixelPointer]=0x7c;//绿
		}
		else
		{
			LedLamp.PixelBuffer[LedLamp.PixelPointer]=0x60;
		}
		color<<=1;
		LedLamp.PixelPointer++;//DMA指针加1
	}
}
//spi dma输出
void WS281xDMAUpdate(u32 *buffer, u32 length)
{
	u16 i = 0,j=0;
	if(DMA_GetCurrDataCounter(DMA2_Channel2) == 0)
	{
		for (i = 0; i <length;i++) 
		{
			for(j=0;j<GROUPSIZE_MAX;j++)
				WS281xSendPixel(buffer[i]);
		}
		LedLamp.PixelPointer = 0;
		DMA_Cmd(DMA2_Channel2, DISABLE);
		DMA_ClearFlag(DMA2_FLAG_TC2);
		DMA_SetCurrDataCounter(DMA2_Channel2, PIXELBUFFER_MAX);
		DMA_Cmd(DMA2_Channel2, ENABLE);
	}
}


void LedGradual(void)
{
	u8 i=0,change=0;
	u8 buf[3]={0};
	static u8 dir=0,grade=0,color=0;
	if(dir)
	{
		if(grade>=255)
		{//渐亮
			dir=0;//渐灭
		}
		else
		{
			grade++;
			change=1;
		}
	}
	else
	{
		if(grade==0)
		{
			dir=1;//渐亮
			if(++color>6)//LED颜色
			{
				color=0;
			}
		}
		else
		{
			grade--;
			change=1;
		}
	}
	switch(6)
	{
		case 0:
			buf[0]=grade;buf[1]=0;buf[2]=0;
			break;
		case 1:
			buf[0]=0;buf[1]=grade;buf[2]=0;
			break;
		case 2:
			buf[0]=0;buf[1]=0;buf[2]=grade;
			break;
		case 3:
			buf[0]=grade;buf[1]=grade;buf[2]=0;
			break;
		case 4:
			buf[0]=grade;buf[1]=0;buf[2]=grade;
			break;
		case 5:
			buf[0]=0;buf[1]=grade;buf[2]=grade;
			break;
		case 6:
			buf[0]=grade;buf[1]=grade;buf[2]=grade;
			break;
		default:
			break;
	}
	if(change)
	{
		for(i=0;i<GROUPSUM_MAX;i++)
		{
			LedLamp.GroupColor[i]=buf[0];
			LedLamp.GroupColor[i]|=(buf[1]<<8);
			LedLamp.GroupColor[i]|=(buf[2]<<16);
		}	
		
	}
}


void LedLampProcess(void)
{
	u8 i=0;
	static u8 count=0;
	//LedGradual();
	LedGameMode();
	WS281xDMAUpdate(LedLamp.GroupColor,GROUPSUM_MAX);
//	switch(LedLamp.RunMode)
//	{
//		case LAMP_IDLE_NORMAL://空闲模式
//			LedGradual();
//			break;
//		case LAMP_MODE_GAME:
//			
//			break;
//		default:
//			break;
//	}
}

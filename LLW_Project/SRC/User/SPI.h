#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f10x.h"

#define	SPI2_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_12);//GPIOB->BSRR=1<<12    //片选端口1
#define	SPI2_CS_CLR  GPIO_ResetBits(GPIOB,GPIO_Pin_12);//GPIOB->BRR=1<<12    //片选端口0 

void SPIInit(void);
u8 SPI2_ReadWriteByte(u8 TxData);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
#endif

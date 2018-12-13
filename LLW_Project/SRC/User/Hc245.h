#ifndef _HC245_H_
#define _HC245_H_
#include"..\Lib\includes.h"
//245ABC OUT 245.0->.7==A7->A0
#define HC245ABCD_DIS() do{GPIO_SetBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);}while(0)

//’Î∂‘HC245C
#define PinKeyA GPIO_Pin_7
#define PinKeyB GPIO_Pin_6
#define PinKeyC GPIO_Pin_5
#define PinKeyD GPIO_Pin_4
#define PinKeyE GPIO_Pin_3
#define PinKeyF GPIO_Pin_2
#define PinKeyG GPIO_Pin_1
#define PinKeyH GPIO_Pin_0
//’Î∂‘HC245D
#define PinGunIn 		GPIO_Pin_7
#define PinCoinA 		GPIO_Pin_6
#define PinCoinB 		GPIO_Pin_5
#define PinEggIn 		GPIO_Pin_4
#define PinTicketIn GPIO_Pin_3

#define PinKeyLeft 	GPIO_Pin_2
#define PinKeyRight GPIO_Pin_1
#define PinKeyStart GPIO_Pin_0


#define KeyLeft 	HC245ReadBit(HC245D,PinKeyLeft)
#define KeyRight 	HC245ReadBit(HC245D,PinKeyRight)
#define KeyStart 	HC245ReadBit(HC245D,PinKeyStart)

INT8U HC245ReadBit(INT8U port,INT16U GPIO_Pin);
INT8U HC245ReadData(INT8U port);
#endif













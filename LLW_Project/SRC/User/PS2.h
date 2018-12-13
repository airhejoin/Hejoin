#ifndef _PS2_H_
#define _PS2_H_
#include"stm32f10x.h"
#define RCC_APB2Periph_PS2 RCC_APB2Periph_GPIOA
#define PS2_DATA_PORT GPIOA
#define PS2_CLK_PORT  GPIOA

#define GPIO_PortSourcePS2CLK GPIO_PortSourceGPIOA
#define GPIO_PinSourcePS2CLK GPIO_PinSource11
#define EXTI_LinePS2CLK EXTI_Line11////EXTI_LinePS2CLK
#define EXTI_PS2CLK_IRQn EXTI15_10_IRQn

#define PS2_DATA_PIN GPIO_Pin_12	// 数据管脚PA12
#define PS2_CLK_PIN GPIO_Pin_11	// 时钟管脚PA11

extern volatile u8 PS2RxFlag;
extern volatile u16 PS2Data;
enum {
	KEY_0=0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_EXIT,
	KEY_PGUP,
	KEY_PGDN,
	KEY_ENTER,
	KEY_CLEAR,
	KEY_POINT,
	KEY_BREAK,
};
void PS2Init(void);
u8 PS2Decode(u16 dat);//解码刷出
#endif

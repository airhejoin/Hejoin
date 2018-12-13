#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"
void UsartInit(void);
void Usart1SendByte(u8 dat);
void Usart1SendString(u8 *str,u16 len);
void Usart2SendString(u8 *str,u16 len);
void Usart3SendString(u8 *str,u16 len);
void Uart4SendString(u8 *str,u16 len);
void Uart5SendString(u8 *str,u16 len);
#endif

#ifndef _TEXT_H_
#define _TEXT_H_
 
#include "..\STM32Lib\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h"     
		 
#define FONTINFOADDR	0	//字体信息保存地址,占25个字节,第24个字节用于标记字库是否存在.
//按钮结构体定义
__packed typedef struct 
{
	const u32 ugbkaddr; 	//unigbk的地址
	u32 f24addr;			//gbk24地址
	u32 f16addr;			//gbk12地址	
	u32 ugbksize;			//unigbk的大小	 
}_font_info;

extern _font_info ftinfo;	//字库信息结构体

void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);//得到汉字的点阵码
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u16 color);//在指定位置显示一个汉字
void ShowChinese(u16 x,u16 y,u8*str,u8 size,u16 color);//在指定位置显示一个字符串 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
//void Show_PTStr(u8 x,u8 y,u8*str,u8 size,u8 mode,u8 len,u16 offx);//显示部分字符
//void Str_Scroll(u8 x,u8 y,u8*str,u8 size,u8 mode,u8 len,u8 start);//滚动字符串
//void my_stradd(u8*str1,u8*str2);//将str2与str1相加,结果保存在str1

#endif

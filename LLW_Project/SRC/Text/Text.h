#ifndef _TEXT_H_
#define _TEXT_H_
 
#include "..\STM32Lib\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h"     
		 
#define FONTINFOADDR	0	//������Ϣ�����ַ,ռ25���ֽ�,��24���ֽ����ڱ���ֿ��Ƿ����.
//��ť�ṹ�嶨��
__packed typedef struct 
{
	const u32 ugbkaddr; 	//unigbk�ĵ�ַ
	u32 f24addr;			//gbk24��ַ
	u32 f16addr;			//gbk12��ַ	
	u32 ugbksize;			//unigbk�Ĵ�С	 
}_font_info;

extern _font_info ftinfo;	//�ֿ���Ϣ�ṹ��

void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);//�õ����ֵĵ�����
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u16 color);//��ָ��λ����ʾһ������
void ShowChinese(u16 x,u16 y,u8*str,u8 size,u16 color);//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
//void Show_PTStr(u8 x,u8 y,u8*str,u8 size,u8 mode,u8 len,u16 offx);//��ʾ�����ַ�
//void Str_Scroll(u8 x,u8 y,u8*str,u8 size,u8 mode,u8 len,u8 start);//�����ַ���
//void my_stradd(u8*str1,u8*str2);//��str2��str1���,���������str1

#endif

#ifndef _BMP_H_
#define _BMP_H_
#include "stm32f10x.h"
//BMPͷ�ļ�
typedef __packed struct
{
    u16  bType ;     //�ļ���־.ֻ��'BM',����ʶ��BMPλͼ���� 0 1
    u32  bSize ;	  //�ļ���С,ռ�ĸ��ֽ� 2 3 4 5
    u16  bReserved1 ;//����6 7
    u16  bReserved2 ;//����7 8
    u32  bOffBits ;  //���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ���� 9 10 11 12
}BITMAPFILEHEADER ;

//BMP��Ϣͷ
typedef __packed struct
{
    u32 biSize ;		   		//˵��BITMAPINFOHEADER�ṹ����Ҫ��������
    long  biWidth ;		   	//˵��ͼ��Ŀ�ȣ�������Ϊ��λ 
    long  biHeight ;	   	//˵��ͼ��ĸ߶ȣ�������Ϊ��λ 
    u16  biPlanes ;	   		//ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1 
    u16  biBitCount ;	   	//˵��������/���أ���ֵΪ1��4��8��16��24����32
    u32 biCompression ;  	//˵��ͼ������ѹ�������͡���ֵ����������ֵ֮һ��
													//BI_RGB��û��ѹ����
													//BI_RLE8��ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��  
													//BI_RLE4��ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
													//BI_BITFIELDS��ÿ�����صı�����ָ�������������
    u32 biSizeImage ;			//˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0  
    long  biXPelsPerMeter ;	//˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
    long  biYPelsPerMeter ;	//˵����ֱ�ֱ��ʣ�������/�ױ�ʾ
    u32 biClrUsed ;	  	 		//˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    u32 biClrImportant ; 		//˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ�� 
}BITMAPINFOHEADER ;

//��ɫ�� 
typedef __packed struct 
{
    u8 rgbBlue ;    //ָ����ɫǿ��
    u8 rgbGreen ;	//ָ����ɫǿ�� 
    u8 rgbRed ;	  	//ָ����ɫǿ�� 
    u8 rgbReserved ;//����������Ϊ0 
}RGBQUAD ;
//λͼ��Ϣͷ
typedef __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	u32 RGB_MASK[3];			//��ɫ�����ڴ��RGB����.
	//RGBQUAD bmiColors[256];  
}BITMAPINFO; 
#define PIC_MAX 30

#define USER_START_ADDR 0//�û�����ַ
#define FONT_START_ADDR 1048576//��������ַ
#define PICINFO_START_ADDR 3145728//ͼƬ�洢��Ϣ��ַ
#define PICDATA_START_ADDR 3149824//ͼƬ���ݿ�ʼ

typedef struct{
	u8 Sum;
	u32 BeginAddr[PIC_MAX];
	u32 Size[PIC_MAX];
}PIC_SAVE_STRUCT;
extern PIC_SAVE_STRUCT PicSaveInfo;

#endif

#ifndef _BMP_H_
#define _BMP_H_
#include "stm32f10x.h"
//BMP头文件
typedef __packed struct
{
    u16  bType ;     //文件标志.只对'BM',用来识别BMP位图类型 0 1
    u32  bSize ;	  //文件大小,占四个字节 2 3 4 5
    u16  bReserved1 ;//保留6 7
    u16  bReserved2 ;//保留7 8
    u32  bOffBits ;  //从文件开始到位图数据(bitmap data)开始之间的的偏移量 9 10 11 12
}BITMAPFILEHEADER ;

//BMP信息头
typedef __packed struct
{
    u32 biSize ;		   		//说明BITMAPINFOHEADER结构所需要的字数。
    long  biWidth ;		   	//说明图象的宽度，以象素为单位 
    long  biHeight ;	   	//说明图象的高度，以象素为单位 
    u16  biPlanes ;	   		//为目标设备说明位面数，其值将总是被设为1 
    u16  biBitCount ;	   	//说明比特数/象素，其值为1、4、8、16、24、或32
    u32 biCompression ;  	//说明图象数据压缩的类型。其值可以是下述值之一：
													//BI_RGB：没有压缩；
													//BI_RLE8：每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；  
													//BI_RLE4：每个象素4比特的RLE压缩编码，压缩格式由2字节组成
													//BI_BITFIELDS：每个象素的比特由指定的掩码决定。
    u32 biSizeImage ;			//说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0  
    long  biXPelsPerMeter ;	//说明水平分辨率，用象素/米表示
    long  biYPelsPerMeter ;	//说明垂直分辨率，用象素/米表示
    u32 biClrUsed ;	  	 		//说明位图实际使用的彩色表中的颜色索引数
    u32 biClrImportant ; 		//说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。 
}BITMAPINFOHEADER ;

//彩色表 
typedef __packed struct 
{
    u8 rgbBlue ;    //指定蓝色强度
    u8 rgbGreen ;	//指定绿色强度 
    u8 rgbRed ;	  	//指定红色强度 
    u8 rgbReserved ;//保留，设置为0 
}RGBQUAD ;
//位图信息头
typedef __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	u32 RGB_MASK[3];			//调色板用于存放RGB掩码.
	//RGBQUAD bmiColors[256];  
}BITMAPINFO; 
#define PIC_MAX 30

#define USER_START_ADDR 0//用户区地址
#define FONT_START_ADDR 1048576//字体区地址
#define PICINFO_START_ADDR 3145728//图片存储信息地址
#define PICDATA_START_ADDR 3149824//图片数据开始

typedef struct{
	u8 Sum;
	u32 BeginAddr[PIC_MAX];
	u32 Size[PIC_MAX];
}PIC_SAVE_STRUCT;
extern PIC_SAVE_STRUCT PicSaveInfo;

#endif

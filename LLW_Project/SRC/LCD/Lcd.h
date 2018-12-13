#ifndef __LCD_H
#define __LCD_H	

#include "stm32f10x.h"	
#include "stdlib.h"

#define	WriteGram      0x22	
//9320/9325 LCD寄存器  
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5		

//颜色对应的RGB值
#define WHITE      	0xFFFF
#define BLACK      	0x0000	  
#define BLUE      	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED        	0xF800
#define MAGENTA    	0xF81F
#define GREEN      	0x07E0
#define CYAN       	0x7FFF
#define YELLOW     	0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色
#define DARKBLUE   	0X01CF	//深蓝色
#define LIGHTBLUE   0X7D7C	//浅蓝色  
#define GRAYBLUE    0X5458 //灰蓝色
#define LIGHTGREEN	0X841F //浅绿色
#define LGRAY 			0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE  	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE     	0X2B12 //浅棕蓝色(选择条目的反色)


/////////////////////////////////////用户配置区///////////////////////////////////	 
//以下2个宏定义，定义屏幕的显示方式及IO速度
#define USE_HORIZONTAL  1	//定义是否使用横屏 		0,不使用.1,使用.
#define LCD_FAST_IO     1 	//定义是否使用快速IO	0,不实用.1,使用

//定义LCD的尺寸
#if USE_HORIZONTAL==1	//使用横屏
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif
////扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  1//R2L_D2U  //默认的扫描方向

/****************LCD端口定义*******************
	TFT_PORT->PC
	TFT_RS->PA0
	TFT_CS->PA1
	TFT_RD->PA2
	TFT_WR->PA3
	TFT_RST->PB2
	TFT_BACKLIGHT->PB10
***********************************************/

#define TFT_DATA_PORT 			GPIOC				//数据端口	
#define TFT_BACKLIGHT_PORT 	GPIOD				//背光端口

#define TFT_RS_PORT					GPIOA				//数据/命令端口
#define TFT_CS_PORT					GPIOA				//片选端口
#define TFT_RD_PORT					GPIOA				//读数据端口
#define TFT_WR_PORT					GPIOA				//写数据端口
#define TFT_RST_PORT				GPIOB				//复位端口

#define TFT_RS_PIN 					((uint16_t)0x0001)	// 数据命令管脚
#define TFT_CS_PIN  				((uint16_t)0x0002) 	//片选管脚
#define TFT_RD_PIN  				((uint16_t)0x0004) 	//读数据管脚
#define TFT_WR_PIN  				((uint16_t)0x0008)  //写数据管脚	
#define TFT_RST_PIN  				((uint16_t)0x0004) 	//复位管脚
#define TFT_BACKLIGHT_PIN  	1<<2							 //背光管脚	PD2

#define LCD_BACKLIGHT_ON() 	TFT_BACKLIGHT_PORT->BRR=TFT_BACKLIGHT_PIN;
#define LCD_BACKLIGHT_OFF() TFT_BACKLIGHT_PORT->BSRR=TFT_BACKLIGHT_PIN;

#define	LCD_RS_SET	TFT_RS_PORT->BSRR=TFT_RS_PIN    //数据/命令1 
#define	LCD_CS_SET  TFT_CS_PORT->BSRR=TFT_CS_PIN    //片选端口1  
#define	LCD_RD_SET	TFT_RD_PORT->BSRR=TFT_RD_PIN    //读数据	1		  
#define	LCD_WR_SET	TFT_WR_PORT->BSRR=TFT_WR_PIN    //写数据1		

#define	LCD_RST_SET	TFT_RST_PORT->BSRR=TFT_RST_PIN  //复位端口

#define	LCD_RS_CLR	TFT_RS_PORT->BRR=TFT_RS_PIN    //数据/命令0  	  								    
#define	LCD_CS_CLR  TFT_CS_PORT->BRR=TFT_CS_PIN     //片选端口0  
#define	LCD_RD_CLR	TFT_RD_PORT->BRR=TFT_RD_PIN     //读数据0		
#define	LCD_WR_CLR	TFT_WR_PORT->BRR=TFT_WR_PIN     //写数据0			

#define	LCD_RST_CLR	TFT_RST_PORT->BRR=TFT_RST_PIN   //复位端口0

#define DATAOUT(x) 	TFT_DATA_PORT->ODR=x; //数据输出
#define DATAIN     	TFT_DATA_PORT->IDR;   //数据输入


//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  Cycle_COLOR ; //背景颜色.默认为白色

//用宏定义,提高速度.
#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 		
typedef struct {
	u16 BackColor;
	u16 FrontColor;
}LCD;
extern LCD LcdStruct;

void LcdInit(void);
void LcdDisplayOn(void);
void LcdDisplayOff(void);
void LcdClear(u16 Color);	 
void LcdSetCursor(u16 Xpos, u16 Ypos);
void LcdSetScanDir(u8 dir);
void LcdDrawPoint(u16 x,u16 y,u16 color);
u16  LcdReadPoint(u16 x,u16 y); //读点
void DrawCircle(u16 x0,u16 y0,u8 r);
void LcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LcdDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);	   
void LcdFill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LcdShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);//显示一个字符
void LcdShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  //显示一个数字
void LcdShow2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode);//显示2个数字
void LcdShowString(u8 x,u8 y,u8*str,u8 size,u8 mode);

u16  LcdReadData(void);//读取LCD数据									    
void LcdWriteReg(u8 reg, u16 value);
u16  LcdReadReg(u8 reg);
void LcdWriteRAM_Prepare(void);
void LcdWriteRAM(u16 RGB_Code);
u16  LcdReadRAM(void);		   
u16  LcdBGR2RGB(u16 c);
void LcdWriteRamPrepare(void);
void LcdDisplayOneNum(u16 x,u16 y,u8 num,u8 size);
void DisplayNumToNum(u16 x,u16 y,u32 num,u8 size,u8 space);
//void FillPic(u16 x,u16 y,u16 len,u16 height,u8 *str,u16 color);
void FillPic(u16 x,u16 y,u16 len,u16 height,const u8 *str,u16 color);
void LcdHanZi(u16 x,u16 y,u8 idex,u16 color);

void LCD_SetDomain(u16 Xsta, u16 Ysta,u16 Xend, u16 Yend);
void LcdFill111(u16 x,u16 y,u16 len,u16 height,const u8 *str,u16 color);

void DrawPoint8_8(u16 x,u16 y,u16 color);
void DrawTimeChar(u16 x,u16 y,u16 color);//显示时分的:
void DrawDateChar(u16 x,u16 y,u16 color);
void DisplayNumToString(u16 x,u16 y,u32 num,u8 len,u8 size,u8 space);
#endif  
	 
	 




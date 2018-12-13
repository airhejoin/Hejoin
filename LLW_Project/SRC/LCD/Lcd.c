#include "Lcd.h"
#include "stdlib.h"
#include "font.h" 	 
#include ".\Lib\includes.h"		 
//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,Cycle_COLOR = BLUE;  

LCD LcdStruct;

void LcdInit(void);//LCD初始化
void LcdPortInit(void);//LCD接口初始化
void LcdConfig(void);//LCD配置
void LcdReset(void);//LCD复位

//******************************************************************
//函数名：  LcdReset
//作者：    
//日期：    
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LcdPortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = TFT_RS_PIN|TFT_CS_PIN|TFT_RD_PIN|TFT_WR_PIN;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA	
	GPIO_SetBits(GPIOA,TFT_RS_PIN|TFT_CS_PIN|TFT_RD_PIN|TFT_WR_PIN);

	GPIO_InitStructure.GPIO_Pin = TFT_RST_PIN|GPIO_Pin_9;	   //PB9 led
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
	
	GPIO_InitStructure.GPIO_Pin = TFT_BACKLIGHT_PIN;	   //背光电源
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); //GPIOD
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(TFT_DATA_PORT, &GPIO_InitStructure); //GPIOC
	GPIO_SetBits(TFT_DATA_PORT,GPIO_Pin_All);	 
}
void LcdConfig(void)
{
	//ILI9325配置开始
	LcdWriteReg(0x00E5,0x78F0); 
	LcdWriteReg(0x0001,0x0100); 
	LcdWriteReg(0x0002,0x0700); 
	LcdWriteReg(0x0003,0x1030);//0b0001 0000 0011 0000 
	LcdWriteReg(0x0004,0x0000); 
	LcdWriteReg(0x0008,0x0202);  
	LcdWriteReg(0x0009,0x0000);
	LcdWriteReg(0x000A,0x0000); 
	LcdWriteReg(0x000C,0x0000); 
	LcdWriteReg(0x000D,0x0000);
	LcdWriteReg(0x000F,0x0000);
	//power on sequence VGHVGL
	LcdWriteReg(0x0010,0x0000);   
	LcdWriteReg(0x0011,0x0007);  
	LcdWriteReg(0x0012,0x0000);  
	LcdWriteReg(0x0013,0x0000); 
	LcdWriteReg(0x0007,0x0000); 
	//vgh 
	LcdWriteReg(0x0010,0x1690);   
	LcdWriteReg(0x0011,0x0227);
	//delayms(100);
	//vregiout 
	LcdWriteReg(0x0012,0x009D); //0x001b
	//delayms(100); 
	//vom amplitude
	LcdWriteReg(0x0013,0x1900);
	//delayms(100); 
	//vom H
	LcdWriteReg(0x0029,0x0025); 
	LcdWriteReg(0x002B,0x000D); 
	//gamma
	LcdWriteReg(0x0030,0x0007);
	LcdWriteReg(0x0031,0x0303);
	LcdWriteReg(0x0032,0x0003);// 0006
	LcdWriteReg(0x0035,0x0206);
	LcdWriteReg(0x0036,0x0008);
	LcdWriteReg(0x0037,0x0406); 
	LcdWriteReg(0x0038,0x0304);//0200
	LcdWriteReg(0x0039,0x0007); 
	LcdWriteReg(0x003C,0x0602);// 0504
	LcdWriteReg(0x003D,0x0008); 
	//ram
	LcdWriteReg(0x0050,0x0000); 
	LcdWriteReg(0x0051,0x00EF);
	LcdWriteReg(0x0052,0x0000); 
	LcdWriteReg(0x0053,0x013F);  
	LcdWriteReg(0x0060,0xA700); 
	LcdWriteReg(0x0061,0x0001); 
	LcdWriteReg(0x006A,0x0000); 
	//
	LcdWriteReg(0x0080,0x0000); 
	LcdWriteReg(0x0081,0x0000); 
	LcdWriteReg(0x0082,0x0000); 
	LcdWriteReg(0x0083,0x0000); 
	LcdWriteReg(0x0084,0x0000); 
	LcdWriteReg(0x0085,0x0000); 
	//
	LcdWriteReg(0x0090,0x0010); 
	LcdWriteReg(0x0092,0x0600); 
	
	LcdWriteReg(0x0007,0x0133);
	LcdWriteReg(0x00,0x0022);//
	
//ILI9325配置结束		
}
void LcdReset(void)
{
	LCD_RST_CLR;
	//delay_ms(50);
	LCD_RST_SET;
	//delay_ms(50);
}

			
void LcdInit(void)
{  			
	//u32 DeviceCode;	
	LcdPortInit();
	LcdReset();
	LcdConfig();
	LcdSetScanDir(DFT_SCAN_DIR);		 
	LCD_BACKLIGHT_ON();//点亮背光	
	LcdClear(BLACK);
	//DeviceCode = LcdReadReg(0x0000);
	//DeviceCode = 0;
} 

//写寄存器函数
void LcdWriteData(u8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
//读LCD数据
//返回值:读到的值
u16 LcdReadData(void)
{										   
	u16 t;
 	TFT_DATA_PORT->CRL=0X88888888; //PB0-7  上拉输入
	TFT_DATA_PORT->CRH=0X88888888; //PB8-15 上拉输入
	TFT_DATA_PORT->ODR=0X0000;     //全部输出0
	
	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;
	
	
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET; 
	
	TFT_DATA_PORT->CRL=0X33333333; //PB0-7  上拉输出
	TFT_DATA_PORT->CRH=0X33333333; //PB8-15 上拉输出
	TFT_DATA_PORT->ODR=0XFFFF;    //全部输出高
	return t;  
}	

//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LcdWriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LcdWriteData(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
//读寄存器
//LCD_Reg:寄存器编号
//返回值:读到的值
u16 LcdReadReg(u8 LCD_Reg)
{										   
 	LcdWriteData(LCD_Reg);  //写入要读的寄存器号  
	
	return LcdReadData(); 
}  	 
//开始写GRAM
void LcdWriteRamPrepare(void)
{
		LcdWriteData(R34);
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}

//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341这个奇葩),所以,
//一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//0~7：代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341等IC已经实际测试	   
void LcdSetScanDir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;
#if USE_HORIZONTAL//使用横屏
	switch(dir)//方向转换
	{
		case 0:dir=6;break;
		case 1:dir=7;break;
		case 2:dir=4;break;
		case 3:dir=5;break;
		case 4:dir=1;break;
		case 5:dir=0;break;
		case 6:dir=3;break;
		case 7:dir=2;break;	     
	}
#endif
	switch(dir)
	{
		case L2R_U2D://从左到右,从上到下
			regval|=(1<<5)|(1<<4)|(0<<3); 
			break;
		case L2R_D2U://从左到右,从下到上
			regval|=(0<<5)|(1<<4)|(0<<3); 
			break;
		case R2L_U2D://从右到左,从上到下
			regval|=(1<<5)|(0<<4)|(0<<3);
			break;
		case R2L_D2U://从右到左,从下到上
			regval|=(0<<5)|(0<<4)|(0<<3); 
			break;	 
		case U2D_L2R://从上到下,从左到右
			regval|=(1<<5)|(1<<4)|(1<<3); 
			break;
		case U2D_R2L://从上到下,从右到左
			regval|=(1<<5)|(0<<4)|(1<<3); 
			break;
		case D2U_L2R://从下到上,从左到右
			regval|=(0<<5)|(1<<4)|(1<<3); 
			break;
		case D2U_R2L://从下到上,从右到左
			regval|=(0<<5)|(0<<4)|(1<<3); 
			break;	 
	}
	dirreg=0X03;
	regval|=1<<12;  
	LcdWriteReg(dirreg,regval);
} 
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 BGRToRGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}

//LCD开启显示
void LcdDisplayOn(void)
{	
	LcdWriteReg(R7,0x0173); 			//开启显示
}	 
//LCD关闭显示
void LcdDisplayOff(void)
{	   
	LcdWriteReg(R7, 0x0);//关闭显示 
}   
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LcdSetCursor(u16 Xpos, u16 Ypos)
{
#if USE_HORIZONTAL==1 
	Xpos=319-Xpos;
	LcdWriteReg(R32,Ypos);
	LcdWriteReg(R33,Xpos);
	
#else
	LcdWriteReg(R32, Xpos);
	LcdWriteReg(R33, Ypos);					    
#endif
}
//读取个某点的颜色值	 
//x:0~239
//y:0~319
//返回值:此点的颜色
u16 LcdReadPoint(u16 x,u16 y)
{
 	u16 temp;
	if(x>=LCD_W||y>=LCD_H)
		return 0;			//超过了范围,直接返回		   
	LcdSetCursor(x,y);
	LcdWriteData(R34);      		 		//其他IC发送读GRAM指令
	TFT_DATA_PORT->CRL=0X88888888; //PB0-7  上拉输入
	TFT_DATA_PORT->CRH=0X88888888; //PB8-15 上拉输入
	TFT_DATA_PORT->ODR=0XFFFF;     //全部输出高

	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读GRAM时,第一次为假读)	
	LCD_RD_CLR;	
  	LCD_RD_SET;
 	//dummy READ
	LCD_RD_CLR;					   			   
	LCD_RD_SET;
 	temp=DATAIN;  
	
	LCD_CS_SET;
	TFT_DATA_PORT->CRL=0X33333333; //PB0-7  上拉输出
	TFT_DATA_PORT->CRH=0X33333333; //PB8-15 上拉输出
	TFT_DATA_PORT->ODR=0XFFFF;    //全部输出高  
	
	return BGRToRGB(temp);																	//其他
}
void DrawPoint8_8(u16 x,u16 y,u16 color)
{
	FillPic(x,y,12,8,Point88,color);
}
void DrawDateChar(u16 x,u16 y,u16 color)//显示年月日的-
{
	FillPic(x,y,8,12,DateChar[0],color);
}
void DrawTimeChar(u16 x,u16 y,u16 color)//显示时分的:
{
	FillPic(x,y,8,12,DateChar[1],color);
}

//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void LcdDrawPoint(u16 x,u16 y,u16 color)
{
	LcdSetCursor(x,y);//设置光标位置 
	LcdWriteRamPrepare();	//开始写入GRAM
	LCD_WR_DATA(color); 
} 	
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)
		result=result*m;    
	return result;
}	

#define NUMFONT_MAX 7
const u8 NumFont[NUMFONT_MAX][2]={
	{12,1},
	{34,2},
	{36,2},
	{38,2},
	{46,2},
	{152,4},
	{152,4},
};
void LcdDisplayOneNum(u16 x,u16 y,u8 num,u8 size)
{
	u16 i=0,j=0;
	u16 len=0;//数组字总节数
	u8 x_len=0;//宽度占的字节数
	u8 temp=0;
	u16 x0=x;
	len=NumFont[size][0];
	x_len=NumFont[size][1];
	for(i=0;i<len;i++)
	{
		switch(size)
		{
			case NUM18_12:
				temp=Num1812[num][i];
				break;		
			case NUM19_12:
				temp=Num1912[num][i];
				break;
			case NUM38_26:
				temp=Num3826[num][i];
				break;
			case NUM38_28:
				temp=Num3828[num][i];
				break;
			case NUM17_12:
				temp=Num1712[num][i];
				break;
			case NUM23_16:
				temp=Num2316[num][i];
				break;
			case NUM12_08:
				temp=Num1208[num][i];
				break;
		}
		if((i>0)&&((i%x_len)==0))
		{
			x=x0;
			y++;			
		}
		for(j=0;j<8;j++)
		{
			if(temp&0x80)
			{
				LcdDrawPoint(x,y,POINT_COLOR);	
			}
			temp<<=1; 
			x++;
		}
	}	  
}   
/******************************************************************************
 * 函数名:DisplayNumToNum
 * 说明: 
 *    输出数字，输入num为0的时候显示0
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 16:5:14, 2017年3月3日, David
 * -----------------------------------
 *****************************************************************************/
void DisplayNumToNum(u16 x,u16 y,u32 num,u8 size,u8 space)
{         	
	u8 t,temp;
	u8 i=0;
	u8 enshow=0;		
	if(num==0)
	{//显示0
		LcdDisplayOneNum(x,y,num,size); 
	}		
	else
	{
		for(t=0;t<8;t++)
		{
			temp=(num/mypow(10,8-t-1))%10;
			if((enshow==0)&&(temp==0))
			{//未开始显示，并且这一位为0
				
			}
			else
			{//enshow==1或者temp!=0
				enshow=1;//第一位不为0，开始显示
				if(size==NUM24_24)
				{
					//ShowChinese(x+24*i,y,temp+0x30,size); 
				}
				else
				{
					if(size==NUM16_16)
					{
						//ShowChinese(x+16*i,y,temp+0x30,size); 
					}
					else
					{
						LcdDisplayOneNum(x+FontNumSize[size]*i,y,temp,size); 
					}
				}
				
				x+=space;
				i++;
			}
		}		
	}
	
}  
/******************************************************************************
 * 函数名:DisplayNumToString
 * 说明: 
 *    按照所给的长度将数字转换成字符串，数字长度小于给定的长度，在数字字符串前面补0
 *		例如123，长度5的话，输出00123
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 16:5:14, 2017年3月3日, David
 * -----------------------------------
 *****************************************************************************/
void DisplayNumToString(u16 x,u16 y,u32 num,u8 len,u8 size,u8 space)
{         	
	u8 t,temp;				   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LcdDisplayOneNum(x+FontNumSize[size]*t,y,temp,size);
		x+=space;
	}
}   

//清屏函数
//Color:要清屏的填充色
void LcdClear(u16 Color)
{
	u32 index=0;      
	LcdSetCursor(0x00,0x0000);//设置光标位置 
	LcdWriteRamPrepare();     //开始写入GRAM	 	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);    
	}
	LcdStruct.BackColor=Color;
}  


//x1,y1:起点坐标
//x2,y2:终点坐标  
void LcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LcdDrawPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

/******************************************************************************
 * 函数名:FillPic
 * 说明: 
 *    输出数字，输入num为0的时候显示0
 * 输入: 
 * 输出: 
 * 返回: 
 * -----------------------------------
 * 设计:
 * 16:5:14, 2017年3月3日, David
 * -----------------------------------
 *****************************************************************************/
void FillPic(u16 x,u16 y,u16 len,u16 height,const u8 *str,u16 color)
{
	u16 i,j,k;
	u16 x0=x;
	u8 temp=len;
	if(len%8)
	{
		len=(temp>>3)+1;
	}
	else
	{
		len=temp>>3;
	}
	height=y+height;
	for(i=y;i<height;i++)
	{
		for(j=0;j<len;j++)
		{
			temp=*str++;
			for(k=0;k<8;k++)
			{
				if(temp&0x80)
				{
					LcdDrawPoint(x0,i,color);
				}
				temp<<=1;
				x0++;
			}
		}
		x0=x;
	}
}
//设定窗口区域写数据
//Xsta , Ysta:起点横坐标和纵坐标
//Xend , Yend:终点横坐标和纵坐标
void LCD_SetDomain(u16 Xsta, u16 Ysta,u16 Xend, u16 Yend)
{			
	#if	USE_HORIZONTAL == 0
		LcdWriteReg(0x50, Xsta); //水平GRAM起始位置 
		LcdWriteReg(0x51, Xend); //水平GRAM终点位置 
		LcdWriteReg(0x52, Ysta); //水平GRAM起始位置
		LcdWriteReg(0x53, Yend); //垂直GRAM终点位置
//50h和52h是起点，51h和53h是终点
	#else
		LcdWriteReg(0x50, Ysta);  //水平GRAM起始位置 
		LcdWriteReg(0x51, Yend);  //水平GRAM终点位置
		LcdWriteReg(0x52, 319-Xend); //垂直GRAM起始位置
		LcdWriteReg(0x53, 319-Xsta); //垂直GRAM终点位置
	#endif
		LcdWriteReg(0x20, 0x0000);
		LcdWriteReg(0x21, 0x0000);
	LcdWriteData(WriteGram);			//准备写入ram
}  

//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
//void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
//{          
//	u16 i,j;
//	LCD_SetDomain(xsta,ysta,xend,yend);//设置区域   
//	for(i=ysta;i<=yend;i++)
//	{	  
//		for(j=xsta;j<=xend;j++) 
//		LCD_WR_DATA(color);	    
//	}
//	LCD_SetDomain(0,0,LCD_W,LCD_H);//恢复Gram全屏设置						  	    
//}  

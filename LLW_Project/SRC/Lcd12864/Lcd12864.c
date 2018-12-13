/********************************************************************
* 文件名  ： Lcd12864驱动
* 描述    :  
* 创建人  ： David
* 时间    ： 17.5.6 15:54
***********************************************************************/
#include"includes.h"

#include <stdio.h>
//#include "intrins.h"
//#include"USART.h"

#define X1 0x80
#define X2 0x88
#define Y  0x80

/*可显示4行汉字，每行8个*/
/*GDRAM
绘图显示RAM，不使用内部字库，用户可以自定义字体或者画图，对应整个屏幕
扩充指令先设置垂直地址再设置水平地址(连续写入)再写入两个8位数据到RAM
|上半屏：
Y 0~31	X=80H,81H,82H,83H,84H,85H,86H,87H,		 
|下半屏：
Y 0~31 	X=88H,89H,8AH,8BH,8CH,8DH,8EH,8FH,
X自动加1，Y手动设置，X地址对应两个水平字节，高字节在前，低字节在后
*/

/*CGRAM 
4个16*16的缓存区，可以定义字库没有的字或者字符，
也可以拼成一个32*32的字符或者图像
地址 0000H 0002H 0004H 0006H
*/

/*DDRAM
文本显示RAM,其提供10个*2行的汉字空间，对应地址
80H,81H,82H,83H,84H,85H,86H,87H,
90H,91H,92H,93H,94H,95H,96H,97H,
88H,89H,8AH,8BH,8CH,8DH,8EH,8FH,
98H,99H,9AH,9BH,9CH,9DH,9EH,9FH,
*/

void delay(void)
{
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();
  
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();  
  __nop();__nop();__nop();__nop();__nop();
  
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();  
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();  
  __nop();__nop();__nop();__nop();__nop();
  
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();  
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();  
	
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();
}

/********************************************************************
* 名称 : sendbyte()
* 功能 : 按照液晶的串口通信协议，发送数据
* 输入 : zdata
* 输出 : 无
***********************************************************************/
#if USE_LPT
#else
void sendbyte(unsigned char zdata)
{
	unsigned int i;
	for(i=0; i<8; i++)
	{
		if((zdata << i) & 0x80)
		{
			LCD_SIDH;
		}
		else 
		{
			LCD_SIDL;
		}
		LCD_SCKL;
		delay();
		LCD_SCKH;
	}
}
u8 ReadByte(void)
{
	u8 i,temp1,temp2,value;
	temp1=0;
	temp2=0;
	LCD_SIDH;
	
	for(i=0;i<8;i++)
	{
		temp1=temp1<<1;
		LCD_SCKL;
		DelayXUs(1);
		LCD_SCKH;
		DelayXUs(1);
		LCD_SCKL;
		if(LCD_SID==1)
		{
			temp1|=0x01;
		}
	}
	for(i=0;i<8;i++)
	{
		temp2=temp2<<1;
		LCD_SCKL;
		DelayXUs(1);
		LCD_SCKH;
		DelayXUs(1);
		LCD_SCKL;
		if(LCD_SID==1)
		{
			temp2|=0x01;
		}
	}
	temp1=0xf0&temp1;
	temp2=0x0f&temp2;
	value=temp1+temp2;
	LCD_SIDL;
	return value ;
	
	#if 0
	u8 i,j;
	u8 dat=0;
	LCD_SIDH;
	for(j=0; j<2; i++)
	{
		for(i=0; i<4; i++)
		{
			SCLK = 1;
			DelayXUs(1);			
			dat<<=1;
			if(LCD_SID==1)
			{
				dat|=0x01;
			}
			SCLK = 0;		
			DelayXUs(1);	
		}
		for(i=0; i<4; i++)
		{	
			SCLK = 1;
			DelayXUs(1);
			SCLK = 0;			
		}		
	}
	LCD_SIDL;
	
	return dat;
	#endif
}
#endif
void LcdWriteCmd(u8 cmd)  
{
#if USE_LPT
	RS=0;
	WRD=0;
	DelayXUs(1);
	E=1;
	LcdPort=cmd;
	DelayXUs(2);
	E=0;
#else
	LCD_CSH;
	sendbyte(0xf8);
	sendbyte(cmd & 0xf0);
	sendbyte((cmd << 4) & 0xf0);
	delay();
#endif
}
/********************************************************************
* 名称 : LcdWriteData()
* 功能 : 传送数据
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdWriteData(u8 dat)  
{
#if USE_LPT
	WRD=0;
	RS=1;
	DelayXUs(1);
	LcdPort=dat;
	E=1;
	DelayXUs(2);
	E=0;
#else	
	LCD_CSH;
	sendbyte(0xfa);
	sendbyte(dat & 0xf0);
	sendbyte((dat << 4) & 0xf0);
	delay();	
#endif
}
/********************************************************************
* 名称 : LcdReadByte()
* 功能 : 从lcd中读取数据
* 输入 : 无
* 输出 : 无
***********************************************************************/
u8 LcdReadByte( void )
{
	u8 dat ;	
#if USE_LPT
	LcdPort = 0xff ;
	RS = 1 ;
	DelayXUs(4);
	WRD = 1;
	E = 0 ;
	DelayXUs(4);
	E = 1 ;
	DelayXUs(4);
	dat = LcdPort ;
	E = 0 ;
#else		
	LCD_CSH;
	sendbyte(0xfe);
	dat=ReadByte();
	DelayXUs(2);
	LCD_CSL;
#endif	
	return dat ;
}

/********************************************************************
* 名称 : LcdInit()
* 功能 : LCD12864初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdInit(void)    
{
#if USE_LPT
	RES=0;                 //复位
	DelayXMs(10);              //延时
	RES=1;                 //复位置高
	DelayXMs(10);
	LcdWriteCmd(0x30);	//基本指令集动作  
	DelayXMs(10);	
	LcdWriteCmd(0x0c);  //显示状态设置，整体显示，光标OFF,光标位置OFF
	DelayXMs(10);						//
	LcdWriteCmd(0x01);  //清除显示
	DelayXMs(10);
	LcdWriteCmd(0x06);  //光标右移，DDRAM加1
	DelayXMs(10);
	LcdWriteCmd(0x06);  //光标右移，DDRAM加1
	DelayXMs(10);
#else
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	LCD_SCKL;
	LCD_SIDL;
	
	DelayXMs(100);
	LcdWriteCmd(0x30);
	DelayXMs(10);
	LcdWriteCmd(0x0c);
	DelayXMs(10);
	
#endif
	LcdGDRAMClear();
	LcdScreenClear();	
  LcdLoadPic((u8 *)&CompanyLogo);//将图片数据加载到内存中
  //LcdMainPageRefresh();
}
/********************************************************************
* 名称 : LcdSetPos()
* 功能 : 定位
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdSetPos(u8 row, u8 col)  
{  
    u8 pos;  
    if( row == 0)  row = 0x80;  
    else if(row == 1) row = 0x90;  
    else if(row == 2) row = 0x88;  
    else if(row == 3) row = 0x98;     
    pos = row + col;
		
    LcdWriteCmd(pos);
    DelayXUs(10);
}
/********************************************************************
* 名称 : LcdScreenClear()
* 功能 : 清屏
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdScreenClear(void)
{
	LcdWriteCmd(0x30); //基本指令
	LcdWriteCmd(0x01); //清屏处理
	DelayXMs(5); //要做至少几百US的延时，不然清屏指令不起作用！540kHz 1.6ms
}
/********************************************************************
* 名称 : LcdRAMShowStr()
* 功能 : 在指定位置显示指定长度的字符串
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdRAMShowStr(u8 row, u8 col,u8 *p,u8 len)
{
	LcdSetPos( row, col );
	while(len--)
	{
		LcdWriteData( *p);
		p++;
	}
}

/********************************************************************
* 名称 : LcdShowChinse()
* 功能 : 汉字字符串混合输出
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdShowString(u8 row,u8 col,u8 *str)
{
  	u8 i = 0;
  	LcdSetPos( row,col);
  	while (str[i] != '\0')
  	{
			LcdWriteData( str[i++]);
    	if((2*col+i)%(16)==0)
    	{            
       		LcdSetPos(row,0);
					row++;
    	}   
  	} 
}
/********************************************************************
* 名称 : LcdShowMix()
* 功能 : 指定位置显示字符
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdShowChar(u8 row,u8 col,u8 dat)
{    
  LcdSetPos(row, col);
  LcdWriteData(dat);
}

/********************************************************************
* 名称 : LcdUpDisChn()
* 功能 : 上半屏显示汉字或者字符
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdUpDisChn(u8 *str)
{
	u8 i;
	LcdWriteCmd(0x30);								//基本指令操作
	DelayXUs(100);
	LcdWriteCmd(0x80);  							//设置DDRAM地址
	DelayXUs(100);
	for(i=0;i<16;i++) LcdWriteData( str[i]);
	LcdWriteCmd(0x90);								//切换到第2行
	for(i=0;i<16;i++) LcdWriteData( str[16+i]);
}

/********************************************************************
* 名称 : LcdDisPic()
* 功能 : 图片显示函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
#define LINE1 0x80
#define LINE2 0x88
void LcdDisPic(const u8 *dat)
{
	u8 i,j;
	LcdWriteCmd(0x34);			//扩充指令操作，绘图开
	for(j=0;j<32;j++)
	{
		LcdWriteCmd(0x80+j);
		LcdWriteCmd(0x80);
		for(i=0;i<16;i++)
		{
			LcdWriteData(*dat);
			dat++;
		}
	}
	for(j=0;j<32;j++)
	{
		LcdWriteCmd(0x80+j);
		LcdWriteCmd(0x88);
		for(i=0;i<16;i++)
		{
			LcdWriteData(*dat);
			dat++;
		}
	}
	LcdWriteCmd(0x36);			//扩充指令操作，绘图开
	LcdWriteCmd(0x30);  		//恢复基本指令动作
}
/********************************************************************
* 名称 : LcdDisPic()
* 功能 : 显示16*32的图像
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdShow1632Pic(u8 x,u8 y,u8 const *bmp)
{
	u8 i,j,basex,basey;
	switch(y)	//由y纵坐标定是上半屏还是下半屏
	{
		case 1: basex=0x80; 
			break; //上半屏
		case 2: basex=0x80; 
			break; //先上半屏，下面再下半屏。
		case 3: basex=0x88; 
			break; //下半屏
		default: return; //别的则返回
	}
	basey=basex+x-1;
	LcdWriteCmd(0x36); //绘图模式
	if(y==1||y==3)	//如为第一第三行，则直接是在同一半屏，直接绘完32行数据。
	{
		for(i=0;i<32;i++)	//写入32行点阵
		{
			LcdWriteCmd(0x80+i);	//先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80
			LcdWriteCmd(basey); //再写入水平位址(选上下半屏)
			for(j=0;j<2;j++) //2个8位元的数据,即16BIT宽度
				LcdWriteData(*bmp++);
		}
	}
	if(y==2) //从第二行开始则画图将上下半屏都有，所以先画完上半屏16行，再画下半屏16行。
	{
		for(i=0;i<16;i++)	//写入上半屏16行点阵
		{
			LcdWriteCmd(0x90+i); //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80，第二行为0X90。
			LcdWriteCmd(basey); //(选上半屏)再写入水平位址
			for(j=0;j<2;j++) //2个8位元的数据,即16BIT宽度
				LcdWriteData(*bmp++);
		}
		for(i=0;i<16;i++)	//写入下半屏16行点阵
		{
			LcdWriteCmd(0x80+i); //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80
			LcdWriteCmd(basey+8); //(选下半屏)入水平位址
			for(j=0;j<2;j++) //2个8位元的数据,即16BIT宽度
				LcdWriteData(*bmp++);
		}
	}
	LcdWriteCmd(0x30);
}

void LcdShow1616Pic(u8 x,u8 y,u8 const *bmp)
{
	u8 i,j,basex,basey;
	switch(y)	//由y纵坐标定是上半屏还是下半屏
	{
		case 1: basex=0x80; 
			break; //上半屏
		case 2: basex=0x80; 
			break; //先上半屏，下面再下半屏。
		case 3: basex=0x88; 
			break; //下半屏
		default: return; //别的则返回
	}
	basey=basex+x-1;
	LcdWriteCmd(0x36); //绘图模式
	if(y==1||y==3)	//如为第一第三行，则直接是在同一半屏，直接绘完32行数据。
	{
		for(i=0;i<16;i++)	//写入32行点阵
		{
			LcdWriteCmd(0x80+i);	//先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80
			LcdWriteCmd(basey); //再写入水平位址(选上下半屏)
			for(j=0;j<2;j++) //2个8位元的数据,即16BIT宽度
				LcdWriteData(*bmp++);
		}
	}
	if(y==2) //从第二行开始则画图将上下半屏都有，所以先画完上半屏16行，再画下半屏16行。
	{
		for(i=0;i<16;i++)	//写入上半屏16行点阵
		{
			LcdWriteCmd(0x90+i); //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80，第二行为0X90。
			LcdWriteCmd(basey); //(选上半屏)再写入水平位址
			for(j=0;j<2;j++) //2个8位元的数据,即16BIT宽度
				LcdWriteData(*bmp++);
		}
		for(i=0;i<16;i++)	//写入下半屏16行点阵
		{
			LcdWriteCmd(0x80+i); //先写入垂直位址,选上下32行的哪一行,不管上下半屏，首行都为0X80
			LcdWriteCmd(basey+8); //(选下半屏)入水平位址
			for(j=0;j<2;j++) //2个8位元的数据,即16BIT宽度
				LcdWriteData(*bmp++);
		}
	}
	LcdWriteCmd(0x30);
}
/********************************************************************
* 名称 : LcdGDRAMClear()
* 功能 : 清除GDRAM中的内容，在绘图前需先调用此函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdGDRAMClear(void)
{
	u8 x,y;
	LcdWriteCmd(0x34);//扩展指令集
	for (y=0;y<32;y++)
	{
		LcdWriteCmd(0x80+y); //设置y=1000+00xx，y+1则往下一行
		LcdWriteCmd(0x80);	//设置x=1000 0000
		for (x=0;x<16;x++)
		{
			LcdWriteData(0x00); //
		}
	}
	for (y=0;y<32;y++)
	{
		LcdWriteCmd(0x80+y); //设置y=1000+00xx，y+1则往下一行
		LcdWriteCmd(0x88);	//设置x=1000 0000
		for (x=0;x<16;x++)
		{
			LcdWriteData(0x00); //
		}
	}
	LcdWriteCmd(0x36);	//扩展指令，绘图开
	//DelayXMs(50);
}
/********************************************************************
* 名称 : LcdShow1616Anti()
* 功能 : 使用绘图的方法让一个1616的字符反白
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdShow1616Anti(u8 x,u16 y,u8 sign,u8 *bmp)
{
	u8 i,j,basex;
	LcdWriteCmd(0x36);
	if(y==1||y==2)	//第一第二行
	{
		basex=0x80;	//上半屏
		y=(y-1)*16;	//垂直位址从0X80开始.
	}
	if(y==3||y==4)	//第三第四行
	{
		basex=0x88;	//下半屏
		y=(y-3)*16;	//垂直位址都是从0X80开始的，不管上下半屏。
	}
	for(i=0;i<16;i++)
	{
		LcdWriteCmd(0x80+y+i); //写入垂直位址。
		LcdWriteCmd(basex+x-1); //再写入水平位址(上半屏第一字为0X80，……第七字为0X87)
		//下半屏第一字为0X88，……第七字为0X8F；
		for(j=0;j<2;j++) //再写入两个8位元的数据,AC会自动增一，接着写数据
		{
			if(sign==1)
				LcdWriteData(~(*bmp++));
			else
				LcdWriteData(*bmp++);
		}
	}
	LcdWriteCmd(0x30);
}
void lcd_row_on(u8 temp)
{
    u8 i, j, x, y;
    y = temp - 1;
    if(y >= 2)
    {
        x = 8;
        y = (y-2)*16;
     }
     else
     {
        x = 0;
        y = y*16;
      }

        LcdWriteCmd(0x34);
        LcdWriteCmd(0x36);
        for(i=0; i<16; i++)
        {    
        
        LcdWriteCmd(0x80+y);
        LcdWriteCmd(0x80+x);
        y++;

                for(j=0; j<16; j++)
                {
                        LcdWriteData(0xff);    
         }
        }
    LcdWriteCmd(0x30);
    //delay(2);
}
/********************************************************************
* 名称 : LcdShowAnti()
* 功能 : 指定位置长度反白
* 输入 : row行号 col列号 len长度 bkcor 1反白或0者消白
* 输出 : 无
***********************************************************************/
void LcdShowAnti(u8 row,u8 col,u8 len,u8 bkcor)
{
	u8 i,j,basex,basey,color;
	LcdWriteCmd(0x36); //扩展指令
	if(bkcor==1)	color=0xff;	//全写入0XFF，反白。
	if(bkcor==0)	color=0x00;	//全写入0X00，消白。
	if(row==0||row==1)	//第一第二行
	{
		basex=0x80;	//上半屏
	}
	if(row==2||row==3)	//第三第四行
	{
		basex=0x88;	//下半屏
		row=row-2;	//垂直位址都是从0X80开始的，不管上下半屏。
	}
	basey=0x80+row*16;	//从哪一行的首行点阵开始
	for(i=0;i<16;i++)	//一行有16行点阵
	{
		LcdWriteCmd(basey+i); //写入垂直位址。
		LcdWriteCmd(basex+col); //水平位址(上半屏第一字为0X80)//下半屏第一字为0X88
		for(j=0;j<len;j++) //再写入两个8位元的数据,AC会自动增一，接着写数据
		{
			LcdWriteData(color);
			LcdWriteData(color);
		}
	}
	LcdWriteCmd(0x30);
}
/********************************************************************
* 名称 : LcdShowRowAnti()
* 功能 : 使用绘图的方法让任意个汉字进行反白
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdShowChineseAnti(u8 x,u8 y,u8 end_x,u8 bkcor)
{
	u8 i, j, white_x, white_y,white_end_x,clr_x,clr_y;
	LcdWriteCmd(0x36); //扩展指令，写图片时，关图片显示
	white_end_x = (end_x-x+1);
	white_end_x <<= 1;
	if(y==1)
	{
		white_x = (0x80+x-1);
		white_y = 0x80;
		clr_x = 0x80;
		clr_y = 0x80;
	}
	else if(y==2)
	{
		white_x = (0x80+x-1);
		white_y = 0x90;
		clr_x = 0x80;
		clr_y = 0x90;
	}
	else if(y==3)
	{
		white_x = (0x88+x-1);
		white_y = 0x80;
		clr_x = 0x88;
		clr_y = 0x80;
	}
	else if(y==4)
	{
		white_x = (0x88+x-1);
		white_y = 0x90;
		clr_x = 0x88;
		clr_y = 0x90;
	}
	for(i=0;i<16;i++ )	//16行，因为是16*16汉字
	{
		LcdWriteCmd(white_y++);	//设置绘图区的Y地址坐标0
		LcdWriteCmd(white_x);	//设置绘图区的X地址坐标0
		for(j=0;j<white_end_x;j++)
		{
			if(bkcor==1)
			{
				LcdWriteData(0xff); //取消这一行的8个点的反白，液晶地址自动加1
				//(此处行指一个一个液晶点所组成的行）
			}
			else
			{
				LcdWriteData(0x00); //反白这一行的8个点，液晶地址自动加1
				//（此处行指一个一个液晶点所组成的行）
			}
		}
	}
	LcdWriteCmd(0x30);
}

/********************************************************************
* 名称 : LcdDrawPoint()
* 功能 : 打点函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LcdDrawPoint(u8 x,u8 y,u8 Mode) 
{     
	u8 x_byte,x_bit;     
	u8 y_byte,y_bit;     
	u8 LCD_H,LCD_L;
	
	x&=0x7f;//防止x坐标超过127     
	y&=0x3f;//防止y坐标超过63  
	x_byte=x/16;//在哪个列字节
	x_bit=x%16;//计算在自己中的哪一位    
	y_byte=y/32;//计算在上半屏还是下半屏     
	y_bit=y%32;//在半屏0~31中的哪一行
  
  
	LcdWriteCmd(0x36);		//扩展     
	LcdWriteCmd(0x80+y_bit);//垂直     
	LcdWriteCmd(0x80+x_byte+8*y_byte);//水平     
	LcdReadByte();     
	LCD_H=LcdReadByte();     
	LCD_L=LcdReadByte();     
	LcdWriteCmd(0x80+y_bit);//垂直     
	LcdWriteCmd(0x80+x_byte+8*y_byte);//水平     
	if(x_bit<8)
	{         
		if(Mode)         
		{             
			LcdWriteData(LCD_H|(0x01<<(7-x_bit)));             
			LcdWriteData(LCD_L);         
		}         
		else                  
		{             
			LcdWriteData(LCD_H&(~(0x01<<(7-x_bit))));             
			LcdWriteData(LCD_L);         
		}     
	}     
	else     
	{
		if(Mode)         
		{             
			LcdWriteData(LCD_H);                     
			LcdWriteData(LCD_L|(0x01<<(15-x_bit)));         
		}         
		else         
		{             
			LcdWriteData(LCD_H);             
			LcdWriteData(LCD_L&(~(0x01<<(15-x_bit))));         
		}     
	}     
	LcdWriteCmd(0x30);//恢复基本指令 
}


//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)
		result=result*m;    
	return result;
}	
u8 GetNumLen(u32 num)
{
  u8 temp=0;
  do{
    if(num)
    {
      temp++;
    }
    num=num/10;
  }while(num);
  return temp;
}
void LcdShowNum(u8 row,u8 col ,u32 num,u8 size,u8 mode)
{
	u8 NumStr[20]="                    ";
  u32 temp=num;
  u8 tmp1,tmp2;
  u8 i=0,bsize=0;//bsize 数字有效位数
  if((size>9)||(size==0))
    return;
  LcdRAMShowStr(row,col,NumStr,size);
  for(i=0;i<size;i++)
  {
    temp=temp/10;
    bsize++;
    if(temp==0)
    {
      break;
    }
  }
  if(bsize>size)
    return;
  tmp1=(size-bsize)/2;
  tmp2=(size-bsize)%2;
  sprintf((u8 *)&NumStr[tmp1+tmp2],"%ld",num);
  LcdRAMShowStr(row,col,NumStr,tmp1+tmp2+bsize);
}


void ShowMainPage(void)
{
	LcdScreenClear();										
	LcdDisPic(MainPage);
  LcdLoadPic((u8 *)&MainPage);
	Flag.NormalDisplay=0;
}

void DrawFlagPoint(void)
{
  LcdWriteCmd(0x36); //绘图模式
  if(System.EncryptErrCnt)
  {
    LcdWriteCmd(0x8f);
    LcdWriteCmd(0x86);
    LcdWriteData(0xff);
    LcdWriteData(0x18);//0xF7,0x18->0xFF,0x18    
  }
  LcdWriteCmd(0x30);  		//恢复基本指令动作
}  
u8 LcdRam[1024]={0};
void LcdLoadPic(u8 *dat)
{
  u16 i=0;
  LcdScreenClear();
  for(i=0;i<1024;i++)
    LcdRam[i]=dat[i];
  LcdMainPageRefresh();
}
void DrawPoint(u8 y,u8 x,u8 mode)
{
  u8 x_byte, x_bit;      
  u8 y_byte, y_bit;
  u8 tmph, tmpl;   
  
  LcdWriteCmd(0x36); //绘图模式
  x &= 0x7F;
  y &= 0x3F;
  x_byte=x>>3;//确定在哪个字节
  x_bit=x&0x07;//确定在哪个位
  y_byte=y>>5;//0上半屏 1下半屏
  y_bit=y&0x1f;//在哪一行
  if(y_byte==0)
  {//上半屏
    if(mode)
      LcdRam[(y_bit<<4)+x_byte]|=0x80>>x_bit;
    else
      LcdRam[(y_bit<<4)+x_byte]&=~(0x80>>x_bit);
    if(x_byte%2)
    {
      LcdWriteCmd(0x80+y_bit);
      LcdWriteCmd(0x80+(x_byte>>1));
      LcdWriteData(LcdRam[(y_bit<<4)+x_byte-1]);
      LcdWriteData(LcdRam[(y_bit<<4)+x_byte]);
    }
    else
    {
      LcdWriteCmd(0x80+y_bit);
      LcdWriteCmd(0x80+(x_byte>>1));  
      LcdWriteData(LcdRam[(y_bit<<4)+x_byte]);
      LcdWriteData(LcdRam[(y_bit<<4)+x_byte+1]);        
    }
  }
  else
  {//下半屏
    if(mode)
      LcdRam[512+(y_bit<<4)+x_byte]|=0x80>>x_bit;
    else
      LcdRam[512+(y_bit<<4)+x_byte]&=~(0x80>>x_bit);
    if(x_byte%2)
    {
      LcdWriteCmd(0x80+y_bit);
      LcdWriteCmd(0x80+(x_byte>>1)+8);
      LcdWriteData(LcdRam[512+(y_bit<<4)+x_byte-1]);
      LcdWriteData(LcdRam[512+(y_bit<<4)+x_byte]);       
    }
    else
    {
      LcdWriteCmd(0x80+y_bit);
      LcdWriteCmd(0x80+(x_byte>>1)+8);
      LcdWriteData(LcdRam[512+(y_bit<<4)+x_byte]);
      LcdWriteData(LcdRam[512+(y_bit<<4)+x_byte+1]);        
    }    
  }
	LcdWriteCmd(0x30);  		//恢复基本指令动作
}  
void DrawPointToArray(u8 y,u8 x,u8 mode)
{
  u8 x_byte, x_bit;      
  u8 y_byte, y_bit;
  x &= 0x7F;
  y &= 0x3F;
  x_byte=x>>3;//确定在哪个字节
  x_bit=x&0x07;//确定在哪个位
  y_byte=y>>5;//0上半屏 1下半屏
  y_bit=y&0x1f;//在哪一行
  if(y_byte==0)
  {//上半屏
    if(mode)
      LcdRam[y_bit*16+x_byte]|=0x80>>x_bit;
    else
      LcdRam[y_bit*16+x_byte]&=~(0x80>>x_bit);
  }
  else
  {//下半屏
    if(mode)
      LcdRam[512+y_bit*16+x_byte]|=0x80>>x_bit;
    else
      LcdRam[512+y_bit*16+x_byte]&=~(0x80>>x_bit);
  }
}  

void RamShow6_8Num(u8 y,u8 x,u32 num)
{
  u8 x_byte, x_bit;      
  u8 y_byte, y_bit;
  u8 tmph, tmpl;   
  
  u8 i=0,j=0;
  
  if((y>55)||(x>121))
    return;
  for(j=0;j<16;j++)
    for(i=0;i<8;i++)
    {
      if(num<(Font6_8Max-1))
      {
        if(Font6_8Tab[num][j]&(0x80>>i))
          DrawPointToArray(y+j,x+i,1);
        else
          DrawPointToArray(y+j,x+i,0);        
      }
      else
      {
        DrawPointToArray(y+j,x+i,0); 
      }
    }

}
void ShowMainData(void)
{
  u8 i=0,j=0;
  LcdWriteCmd(0x36); //绘图模式
	for(i=CPLAYNUM_Y;i<CPLAYNUM_Y+14;i++)
	{
		LcdWriteCmd(0x80+i);//y=7 x=2
		LcdWriteCmd(0x80);
		#if LANGUAGE_CN
		for(j=0;j<4;j++)
		{
			LcdWriteData(LcdRam[(i<<4)+j]);
		}
		#else
		for(j=0;j<12;j++)
		{
			LcdWriteData(LcdRam[(i<<4)+j]);
		}      
		#endif
	}    
	//币数 2位
	for(i=15;i<29;i++)
	{
		LcdWriteCmd(0x80+i);//y=7 x=2
		LcdWriteCmd(0x80+(10>>1));
		for(j=10;j<14;j++)
		{
			LcdWriteData(LcdRam[(i<<4)+j]);
		}
	}
  LcdWriteCmd(0x30);  		//恢复基本指令动作
}
void LcdMainPageRefresh(void)
{
  LcdDisPic((u8 *)&LcdRam);
}
//
void LcdTicketRefresh(u8 x,u8 y,u8 len,u8 high)
{
  u8 i=0,j=0;
  u8 *ptr=0;
	u8 y_max=0,x_max=0;
  LcdWriteCmd(0x36);			//扩充指令操作，绘图开
	if((y+high)>31)
		y_max=32;
	for(j=y;j<y_max;j++)
	{
    ptr=&LcdRam[(j<<4)+4];
		LcdWriteCmd(0x80+j);
		LcdWriteCmd(0x80+0);
		for(i=x;i<(x+len);i++)
		{
			LcdWriteData(*ptr++);
		}
	}
	
	if((y+high)>32)
		y_max=y+high-32;
	for(j=0;j<y_max;j++)
	{
		ptr=&LcdRam[512+(j<<4)];
		LcdWriteCmd(0x80+j);
		LcdWriteCmd(0x88+0);	
		for(i=x;i<(x+len);i++)
		{
			LcdWriteData(*ptr++);
		}
	}
	LcdWriteCmd(0x30);  		//恢复基本指令动作  
}
void LcdShowVersion(u32 ver)
{
  u8 buf[2];
  LcdRAMShowStr(3,2,"Ver:",4);//
  buf[0]=ver%1000/100+0x30;
  buf[1]='.';
  LcdRAMShowStr(3,4,buf,2);
  
  buf[0]=ver%100/10+0x30;
  buf[1]='.';
  LcdRAMShowStr(3,5,buf,2);
  
  buf[0]=ver%10+0x30;
  buf[1]=' ';
  LcdRAMShowStr(3,6,buf,2);
}

void LcdClearRam(u8 x,u8 y,u8 len,u8 high)
{
	u8 i=0,j=0;
	u8 y_max=y+high,x_max=0;
	for(i=y;i<y_max;i++)
	{
		for(j=x;j<(x+len);j++)
		{
			LcdRam[(i<<4)+j]=0;
		}
	}
}


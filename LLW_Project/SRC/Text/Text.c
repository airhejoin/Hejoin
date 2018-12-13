#include "text.h"													   
#include"..\W25QXX\W25Q128.h"
#include"..\LCD\Lcd.h"
/*************    函数名：Get_HzMat              ***************************
【功能】:中文GB2312寻址，获取汉字在eeprom中的点阵数据
【参数】：
【返回】：
【说明】：
**************   设计：DAVID 2017-03-27  09:32:22***********************/
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	u8 qh,ql;			  
	u32 foffset;
	u32 startaddr=0;
	u16 len=0;
	qh=*code;
	ql=*(code+1);
	qh-=0xa1;
	ql-=0xa1;
	if(size==24)
	{
		startaddr=FONTDATA1_START_ADDR;
		len=72;
	}
	else//16
	{
		startaddr=FONTDATA2_START_ADDR;
		len=32;
	}
	foffset=(unsigned long)len*(qh*94+ql);//得到字库中的字节偏移量	
	W25QxxRead(mat,foffset+startaddr,len);   												    
}  
/*************    函数名：Show_Font              ***************************
【功能】:显示一个指定大小的汉字
【参数】：
【返回】：
【说明】：size:字体大小
					mode:1,正常显示,0,叠加显示	 
**************   设计：DAVID 2017-03-27  09:32:22***********************/
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u16 color)
{
	u8 tmp1,tmp2,t,t1;
	u16 x0=x;
	u8 dzk[72];
	u8 tempchar;
	if((size!=24)&&(size!=16))
		return;//不支持的size
	tmp1=size/8;
	tmp2=tmp1*size;//总字节数
	Get_HzMat(font,dzk,size);//得到相应大小的点阵数据
	for(t=0;t<tmp2;t++)
	{
		tempchar=dzk[t];
		if((t>0)&&((t%tmp1)==0))
		{
			y++;
			x=x0;				
		}
		for(t1=0;t1<8;t1++)
		{
			if(tempchar&0x80)
				LcdDrawPoint(x,y,color);
			tempchar<<=1;
			x++;
		}
	}
}
/*************    函数名：Show_Str              ***************************
【功能】:显示一串指定大小的汉字
【参数】：
【返回】：
【说明】：size:字体大小
					mode:0,正常显示,1,叠加显示	 
**************   设计：DAVID 2017-03-27  09:32:22***********************/
void ShowChinese(u16 x,u16 y,u8*str,u8 size,u16 color)
{												  	  	    				    				  	  
	while(*str!=0)//数据未结束
	{  
		if(x>(LCD_W-size))//换行
		{	    
			y+=size;
			x=0;		  
		}
		if(y>(LCD_H-size))
			break;//越界返回  						     
		Show_Font(x,y,str,size,color); //显示这个汉字,空心显示 
		str+=2; 
		x+=size;//下一个汉字偏移	    
	}						  
}  			 		  


////在指定宽度的中间显示字符串
////如果字符长度超过了len,则用Show_Str显示
////len:指定要显示的宽度
////针对12*12字体!!!
//void Show_Str_Mid(u8 x,u16 y,u8*str,u8 size,u8 len)
//{
//	u16 strlenth=0;
//   	strlenth=my_strlen(str);
//	strlenth*=size/2;
//	if(strlenth>len)Show_Str(x,y,str,size,1);
//	else
//	{
//		strlenth=(len-strlenth)/2;
//	  Show_Str(strlenth+x,y,str,size,1);
//	}
//}   






















		  







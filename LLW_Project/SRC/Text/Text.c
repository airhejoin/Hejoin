#include "text.h"													   
#include"..\W25QXX\W25Q128.h"
#include"..\LCD\Lcd.h"
/*************    ��������Get_HzMat              ***************************
�����ܡ�:����GB2312Ѱַ����ȡ������eeprom�еĵ�������
����������
�����ء���
��˵������
**************   ��ƣ�DAVID 2017-03-27  09:32:22***********************/
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
	foffset=(unsigned long)len*(qh*94+ql);//�õ��ֿ��е��ֽ�ƫ����	
	W25QxxRead(mat,foffset+startaddr,len);   												    
}  
/*************    ��������Show_Font              ***************************
�����ܡ�:��ʾһ��ָ����С�ĺ���
����������
�����ء���
��˵������size:�����С
					mode:1,������ʾ,0,������ʾ	 
**************   ��ƣ�DAVID 2017-03-27  09:32:22***********************/
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u16 color)
{
	u8 tmp1,tmp2,t,t1;
	u16 x0=x;
	u8 dzk[72];
	u8 tempchar;
	if((size!=24)&&(size!=16))
		return;//��֧�ֵ�size
	tmp1=size/8;
	tmp2=tmp1*size;//���ֽ���
	Get_HzMat(font,dzk,size);//�õ���Ӧ��С�ĵ�������
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
/*************    ��������Show_Str              ***************************
�����ܡ�:��ʾһ��ָ����С�ĺ���
����������
�����ء���
��˵������size:�����С
					mode:0,������ʾ,1,������ʾ	 
**************   ��ƣ�DAVID 2017-03-27  09:32:22***********************/
void ShowChinese(u16 x,u16 y,u8*str,u8 size,u16 color)
{												  	  	    				    				  	  
	while(*str!=0)//����δ����
	{  
		if(x>(LCD_W-size))//����
		{	    
			y+=size;
			x=0;		  
		}
		if(y>(LCD_H-size))
			break;//Խ�緵��  						     
		Show_Font(x,y,str,size,color); //��ʾ�������,������ʾ 
		str+=2; 
		x+=size;//��һ������ƫ��	    
	}						  
}  			 		  


////��ָ����ȵ��м���ʾ�ַ���
////����ַ����ȳ�����len,����Show_Str��ʾ
////len:ָ��Ҫ��ʾ�Ŀ��
////���12*12����!!!
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






















		  







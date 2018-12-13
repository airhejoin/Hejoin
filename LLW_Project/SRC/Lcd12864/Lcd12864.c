/********************************************************************
* �ļ���  �� Lcd12864����
* ����    :  
* ������  �� David
* ʱ��    �� 17.5.6 15:54
***********************************************************************/
#include"includes.h"

#include <stdio.h>
//#include "intrins.h"
//#include"USART.h"

#define X1 0x80
#define X2 0x88
#define Y  0x80

/*����ʾ4�к��֣�ÿ��8��*/
/*GDRAM
��ͼ��ʾRAM����ʹ���ڲ��ֿ⣬�û������Զ���������߻�ͼ����Ӧ������Ļ
����ָ�������ô�ֱ��ַ������ˮƽ��ַ(����д��)��д������8λ���ݵ�RAM
|�ϰ�����
Y 0~31	X=80H,81H,82H,83H,84H,85H,86H,87H,		 
|�°�����
Y 0~31 	X=88H,89H,8AH,8BH,8CH,8DH,8EH,8FH,
X�Զ���1��Y�ֶ����ã�X��ַ��Ӧ����ˮƽ�ֽڣ����ֽ���ǰ�����ֽ��ں�
*/

/*CGRAM 
4��16*16�Ļ����������Զ����ֿ�û�е��ֻ����ַ���
Ҳ����ƴ��һ��32*32���ַ�����ͼ��
��ַ 0000H 0002H 0004H 0006H
*/

/*DDRAM
�ı���ʾRAM,���ṩ10��*2�еĺ��ֿռ䣬��Ӧ��ַ
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
* ���� : sendbyte()
* ���� : ����Һ���Ĵ���ͨ��Э�飬��������
* ���� : zdata
* ��� : ��
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
* ���� : LcdWriteData()
* ���� : ��������
* ���� : ��
* ��� : ��
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
* ���� : LcdReadByte()
* ���� : ��lcd�ж�ȡ����
* ���� : ��
* ��� : ��
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
* ���� : LcdInit()
* ���� : LCD12864��ʼ��
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdInit(void)    
{
#if USE_LPT
	RES=0;                 //��λ
	DelayXMs(10);              //��ʱ
	RES=1;                 //��λ�ø�
	DelayXMs(10);
	LcdWriteCmd(0x30);	//����ָ�����  
	DelayXMs(10);	
	LcdWriteCmd(0x0c);  //��ʾ״̬���ã�������ʾ�����OFF,���λ��OFF
	DelayXMs(10);						//
	LcdWriteCmd(0x01);  //�����ʾ
	DelayXMs(10);
	LcdWriteCmd(0x06);  //������ƣ�DDRAM��1
	DelayXMs(10);
	LcdWriteCmd(0x06);  //������ƣ�DDRAM��1
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
  LcdLoadPic((u8 *)&CompanyLogo);//��ͼƬ���ݼ��ص��ڴ���
  //LcdMainPageRefresh();
}
/********************************************************************
* ���� : LcdSetPos()
* ���� : ��λ
* ���� : ��
* ��� : ��
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
* ���� : LcdScreenClear()
* ���� : ����
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdScreenClear(void)
{
	LcdWriteCmd(0x30); //����ָ��
	LcdWriteCmd(0x01); //��������
	DelayXMs(5); //Ҫ�����ټ���US����ʱ����Ȼ����ָ������ã�540kHz 1.6ms
}
/********************************************************************
* ���� : LcdRAMShowStr()
* ���� : ��ָ��λ����ʾָ�����ȵ��ַ���
* ���� : ��
* ��� : ��
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
* ���� : LcdShowChinse()
* ���� : �����ַ���������
* ���� : ��
* ��� : ��
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
* ���� : LcdShowMix()
* ���� : ָ��λ����ʾ�ַ�
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdShowChar(u8 row,u8 col,u8 dat)
{    
  LcdSetPos(row, col);
  LcdWriteData(dat);
}

/********************************************************************
* ���� : LcdUpDisChn()
* ���� : �ϰ�����ʾ���ֻ����ַ�
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdUpDisChn(u8 *str)
{
	u8 i;
	LcdWriteCmd(0x30);								//����ָ�����
	DelayXUs(100);
	LcdWriteCmd(0x80);  							//����DDRAM��ַ
	DelayXUs(100);
	for(i=0;i<16;i++) LcdWriteData( str[i]);
	LcdWriteCmd(0x90);								//�л�����2��
	for(i=0;i<16;i++) LcdWriteData( str[16+i]);
}

/********************************************************************
* ���� : LcdDisPic()
* ���� : ͼƬ��ʾ����
* ���� : ��
* ��� : ��
***********************************************************************/
#define LINE1 0x80
#define LINE2 0x88
void LcdDisPic(const u8 *dat)
{
	u8 i,j;
	LcdWriteCmd(0x34);			//����ָ���������ͼ��
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
	LcdWriteCmd(0x36);			//����ָ���������ͼ��
	LcdWriteCmd(0x30);  		//�ָ�����ָ���
}
/********************************************************************
* ���� : LcdDisPic()
* ���� : ��ʾ16*32��ͼ��
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdShow1632Pic(u8 x,u8 y,u8 const *bmp)
{
	u8 i,j,basex,basey;
	switch(y)	//��y�����궨���ϰ��������°���
	{
		case 1: basex=0x80; 
			break; //�ϰ���
		case 2: basex=0x80; 
			break; //���ϰ������������°�����
		case 3: basex=0x88; 
			break; //�°���
		default: return; //����򷵻�
	}
	basey=basex+x-1;
	LcdWriteCmd(0x36); //��ͼģʽ
	if(y==1||y==3)	//��Ϊ��һ�����У���ֱ������ͬһ������ֱ�ӻ���32�����ݡ�
	{
		for(i=0;i<32;i++)	//д��32�е���
		{
			LcdWriteCmd(0x80+i);	//��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80
			LcdWriteCmd(basey); //��д��ˮƽλַ(ѡ���°���)
			for(j=0;j<2;j++) //2��8λԪ������,��16BIT���
				LcdWriteData(*bmp++);
		}
	}
	if(y==2) //�ӵڶ��п�ʼ��ͼ�����°������У������Ȼ����ϰ���16�У��ٻ��°���16�С�
	{
		for(i=0;i<16;i++)	//д���ϰ���16�е���
		{
			LcdWriteCmd(0x90+i); //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80���ڶ���Ϊ0X90��
			LcdWriteCmd(basey); //(ѡ�ϰ���)��д��ˮƽλַ
			for(j=0;j<2;j++) //2��8λԪ������,��16BIT���
				LcdWriteData(*bmp++);
		}
		for(i=0;i<16;i++)	//д���°���16�е���
		{
			LcdWriteCmd(0x80+i); //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80
			LcdWriteCmd(basey+8); //(ѡ�°���)��ˮƽλַ
			for(j=0;j<2;j++) //2��8λԪ������,��16BIT���
				LcdWriteData(*bmp++);
		}
	}
	LcdWriteCmd(0x30);
}

void LcdShow1616Pic(u8 x,u8 y,u8 const *bmp)
{
	u8 i,j,basex,basey;
	switch(y)	//��y�����궨���ϰ��������°���
	{
		case 1: basex=0x80; 
			break; //�ϰ���
		case 2: basex=0x80; 
			break; //���ϰ������������°�����
		case 3: basex=0x88; 
			break; //�°���
		default: return; //����򷵻�
	}
	basey=basex+x-1;
	LcdWriteCmd(0x36); //��ͼģʽ
	if(y==1||y==3)	//��Ϊ��һ�����У���ֱ������ͬһ������ֱ�ӻ���32�����ݡ�
	{
		for(i=0;i<16;i++)	//д��32�е���
		{
			LcdWriteCmd(0x80+i);	//��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80
			LcdWriteCmd(basey); //��д��ˮƽλַ(ѡ���°���)
			for(j=0;j<2;j++) //2��8λԪ������,��16BIT���
				LcdWriteData(*bmp++);
		}
	}
	if(y==2) //�ӵڶ��п�ʼ��ͼ�����°������У������Ȼ����ϰ���16�У��ٻ��°���16�С�
	{
		for(i=0;i<16;i++)	//д���ϰ���16�е���
		{
			LcdWriteCmd(0x90+i); //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80���ڶ���Ϊ0X90��
			LcdWriteCmd(basey); //(ѡ�ϰ���)��д��ˮƽλַ
			for(j=0;j<2;j++) //2��8λԪ������,��16BIT���
				LcdWriteData(*bmp++);
		}
		for(i=0;i<16;i++)	//д���°���16�е���
		{
			LcdWriteCmd(0x80+i); //��д�봹ֱλַ,ѡ����32�е���һ��,�������°��������ж�Ϊ0X80
			LcdWriteCmd(basey+8); //(ѡ�°���)��ˮƽλַ
			for(j=0;j<2;j++) //2��8λԪ������,��16BIT���
				LcdWriteData(*bmp++);
		}
	}
	LcdWriteCmd(0x30);
}
/********************************************************************
* ���� : LcdGDRAMClear()
* ���� : ���GDRAM�е����ݣ��ڻ�ͼǰ���ȵ��ô˺���
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdGDRAMClear(void)
{
	u8 x,y;
	LcdWriteCmd(0x34);//��չָ�
	for (y=0;y<32;y++)
	{
		LcdWriteCmd(0x80+y); //����y=1000+00xx��y+1������һ��
		LcdWriteCmd(0x80);	//����x=1000 0000
		for (x=0;x<16;x++)
		{
			LcdWriteData(0x00); //
		}
	}
	for (y=0;y<32;y++)
	{
		LcdWriteCmd(0x80+y); //����y=1000+00xx��y+1������һ��
		LcdWriteCmd(0x88);	//����x=1000 0000
		for (x=0;x<16;x++)
		{
			LcdWriteData(0x00); //
		}
	}
	LcdWriteCmd(0x36);	//��չָ���ͼ��
	//DelayXMs(50);
}
/********************************************************************
* ���� : LcdShow1616Anti()
* ���� : ʹ�û�ͼ�ķ�����һ��1616���ַ�����
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdShow1616Anti(u8 x,u16 y,u8 sign,u8 *bmp)
{
	u8 i,j,basex;
	LcdWriteCmd(0x36);
	if(y==1||y==2)	//��һ�ڶ���
	{
		basex=0x80;	//�ϰ���
		y=(y-1)*16;	//��ֱλַ��0X80��ʼ.
	}
	if(y==3||y==4)	//����������
	{
		basex=0x88;	//�°���
		y=(y-3)*16;	//��ֱλַ���Ǵ�0X80��ʼ�ģ��������°�����
	}
	for(i=0;i<16;i++)
	{
		LcdWriteCmd(0x80+y+i); //д�봹ֱλַ��
		LcdWriteCmd(basex+x-1); //��д��ˮƽλַ(�ϰ�����һ��Ϊ0X80������������Ϊ0X87)
		//�°�����һ��Ϊ0X88������������Ϊ0X8F��
		for(j=0;j<2;j++) //��д������8λԪ������,AC���Զ���һ������д����
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
* ���� : LcdShowAnti()
* ���� : ָ��λ�ó��ȷ���
* ���� : row�к� col�к� len���� bkcor 1���׻�0������
* ��� : ��
***********************************************************************/
void LcdShowAnti(u8 row,u8 col,u8 len,u8 bkcor)
{
	u8 i,j,basex,basey,color;
	LcdWriteCmd(0x36); //��չָ��
	if(bkcor==1)	color=0xff;	//ȫд��0XFF�����ס�
	if(bkcor==0)	color=0x00;	//ȫд��0X00�����ס�
	if(row==0||row==1)	//��һ�ڶ���
	{
		basex=0x80;	//�ϰ���
	}
	if(row==2||row==3)	//����������
	{
		basex=0x88;	//�°���
		row=row-2;	//��ֱλַ���Ǵ�0X80��ʼ�ģ��������°�����
	}
	basey=0x80+row*16;	//����һ�е����е���ʼ
	for(i=0;i<16;i++)	//һ����16�е���
	{
		LcdWriteCmd(basey+i); //д�봹ֱλַ��
		LcdWriteCmd(basex+col); //ˮƽλַ(�ϰ�����һ��Ϊ0X80)//�°�����һ��Ϊ0X88
		for(j=0;j<len;j++) //��д������8λԪ������,AC���Զ���һ������д����
		{
			LcdWriteData(color);
			LcdWriteData(color);
		}
	}
	LcdWriteCmd(0x30);
}
/********************************************************************
* ���� : LcdShowRowAnti()
* ���� : ʹ�û�ͼ�ķ�������������ֽ��з���
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdShowChineseAnti(u8 x,u8 y,u8 end_x,u8 bkcor)
{
	u8 i, j, white_x, white_y,white_end_x,clr_x,clr_y;
	LcdWriteCmd(0x36); //��չָ�дͼƬʱ����ͼƬ��ʾ
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
	for(i=0;i<16;i++ )	//16�У���Ϊ��16*16����
	{
		LcdWriteCmd(white_y++);	//���û�ͼ����Y��ַ����0
		LcdWriteCmd(white_x);	//���û�ͼ����X��ַ����0
		for(j=0;j<white_end_x;j++)
		{
			if(bkcor==1)
			{
				LcdWriteData(0xff); //ȡ����һ�е�8����ķ��ף�Һ����ַ�Զ���1
				//(�˴���ָһ��һ��Һ��������ɵ��У�
			}
			else
			{
				LcdWriteData(0x00); //������һ�е�8���㣬Һ����ַ�Զ���1
				//���˴���ָһ��һ��Һ��������ɵ��У�
			}
		}
	}
	LcdWriteCmd(0x30);
}

/********************************************************************
* ���� : LcdDrawPoint()
* ���� : ��㺯��
* ���� : ��
* ��� : ��
***********************************************************************/
void LcdDrawPoint(u8 x,u8 y,u8 Mode) 
{     
	u8 x_byte,x_bit;     
	u8 y_byte,y_bit;     
	u8 LCD_H,LCD_L;
	
	x&=0x7f;//��ֹx���곬��127     
	y&=0x3f;//��ֹy���곬��63  
	x_byte=x/16;//���ĸ����ֽ�
	x_bit=x%16;//�������Լ��е���һλ    
	y_byte=y/32;//�������ϰ��������°���     
	y_bit=y%32;//�ڰ���0~31�е���һ��
  
  
	LcdWriteCmd(0x36);		//��չ     
	LcdWriteCmd(0x80+y_bit);//��ֱ     
	LcdWriteCmd(0x80+x_byte+8*y_byte);//ˮƽ     
	LcdReadByte();     
	LCD_H=LcdReadByte();     
	LCD_L=LcdReadByte();     
	LcdWriteCmd(0x80+y_bit);//��ֱ     
	LcdWriteCmd(0x80+x_byte+8*y_byte);//ˮƽ     
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
	LcdWriteCmd(0x30);//�ָ�����ָ�� 
}


//m^n����
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
  u8 i=0,bsize=0;//bsize ������Чλ��
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
  LcdWriteCmd(0x36); //��ͼģʽ
  if(System.EncryptErrCnt)
  {
    LcdWriteCmd(0x8f);
    LcdWriteCmd(0x86);
    LcdWriteData(0xff);
    LcdWriteData(0x18);//0xF7,0x18->0xFF,0x18    
  }
  LcdWriteCmd(0x30);  		//�ָ�����ָ���
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
  
  LcdWriteCmd(0x36); //��ͼģʽ
  x &= 0x7F;
  y &= 0x3F;
  x_byte=x>>3;//ȷ�����ĸ��ֽ�
  x_bit=x&0x07;//ȷ�����ĸ�λ
  y_byte=y>>5;//0�ϰ��� 1�°���
  y_bit=y&0x1f;//����һ��
  if(y_byte==0)
  {//�ϰ���
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
  {//�°���
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
	LcdWriteCmd(0x30);  		//�ָ�����ָ���
}  
void DrawPointToArray(u8 y,u8 x,u8 mode)
{
  u8 x_byte, x_bit;      
  u8 y_byte, y_bit;
  x &= 0x7F;
  y &= 0x3F;
  x_byte=x>>3;//ȷ�����ĸ��ֽ�
  x_bit=x&0x07;//ȷ�����ĸ�λ
  y_byte=y>>5;//0�ϰ��� 1�°���
  y_bit=y&0x1f;//����һ��
  if(y_byte==0)
  {//�ϰ���
    if(mode)
      LcdRam[y_bit*16+x_byte]|=0x80>>x_bit;
    else
      LcdRam[y_bit*16+x_byte]&=~(0x80>>x_bit);
  }
  else
  {//�°���
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
  LcdWriteCmd(0x36); //��ͼģʽ
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
	//���� 2λ
	for(i=15;i<29;i++)
	{
		LcdWriteCmd(0x80+i);//y=7 x=2
		LcdWriteCmd(0x80+(10>>1));
		for(j=10;j<14;j++)
		{
			LcdWriteData(LcdRam[(i<<4)+j]);
		}
	}
  LcdWriteCmd(0x30);  		//�ָ�����ָ���
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
  LcdWriteCmd(0x36);			//����ָ���������ͼ��
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
	LcdWriteCmd(0x30);  		//�ָ�����ָ���  
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


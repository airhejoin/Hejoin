#ifndef _TOUCH_H_
#define _TOUCH_H_
#include "stm32f10x.h"

//����״̬	 
#define TOUCH_DOWN 0x01
#define TOUCH_UP   0x00 
//�ʸ˽ṹ��
typedef struct 
{
	u16 X0;//ԭʼ����
	u16 Y0;
	u16 X; //����/�ݴ�����
	u16 Y;						   	    
	u8  Key_Sta;//�ʵ�״̬			  
//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//touchtype=0��ʱ��,�ʺ�����ΪX����,����ΪY�����TP.
//touchtype=1��ʱ��,�ʺ�����ΪY����,����ΪX�����TP.
	u8 touchtype;
}Pen_Holder;	   
extern Pen_Holder Pen_Point;
/****************�������ӿڶ���*******************
	IRQ	->PB0
	busy->PB1
	SPI ->SPI1
***********************************************/ 
#define TOUCH_SPI_PORT	GPIOA
#define TOUCH_IRQ_PORT	GPIOB
#define TOUCH_BUSY_PORT GPIOB

#define TOUCH_CS_PIN		1<<4//
#define TOUCH_CLK_PIN		1<<5//
#define TOUCH_DOUT_PIN	1<<6//
#define TOUCH_DIN_PIN		1<<7//

#define TOUCH_IRQ_PIN		1<<0//PB0
#define TOUCH_BUSY_PIN	1<<1//PB1

//#define PEN  PCin(1)   //PC1  INT
//#define DOUT PCin(2)   //PC2  MISO
//#define TDIN PCout(3)  //PC3  MOSI
//#define TCLK PCout(0)  //PC0  SCLK
//#define TCS  PCout(13) //PC13 CS    
//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ�
//#define CMD_RDX   0X90  //0B10010000���ò�ַ�ʽ��X����
//#define CMD_RDY	0XD0  //0B11010000���ò�ַ�ʽ��Y����
extern u8 CMD_RDX;
extern u8 CMD_RDY;
   											 
#define TEMP_RD	0XF0  //0B11110000���ò�ַ�ʽ��Y����    
//ʹ�ñ���
#define ADJ_SAVE_ENABLE	    
			  
void Touch_Init(void);		 //��ʼ��
u8 Read_ADS(u16 *x,u16 *y);	 //��������˫�����ȡ
u8 Read_ADS2(u16 *x,u16 *y); //����ǿ�˲���˫���������ȡ
u16 ADS_Read_XY(u8 xy);		 //���˲��������ȡ(������)
u16 ADS_Read_AD(u8 CMD);	 //��ȡADת��ֵ
void ADS_Write_Byte(u8 num); //�����оƬд��һ������
void Drow_Touch_Point(u8 x,u16 y);//��һ�������׼��
void Draw_Big_Point(u8 x,u16 y);  //��һ�����
void Touch_Adjust(void);          //������У׼
void Save_Adjdata(void);		  //����У׼����
u8 Get_Adjdata(void); 			  //��ȡУ׼����
void Pen_Int_Set(u8 en); 		  //PEN�ж�ʹ��/�ر�
void Convert_Pos(void);           //���ת������	 

#endif
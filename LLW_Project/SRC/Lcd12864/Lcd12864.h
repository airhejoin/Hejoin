#ifndef _LCD12864_H_
#define _lCD12864_H_

#include"Pic.h"
/***************����ָ��ģʽ��*************/
//RS\RW =0
//
#define CLEAR 0x01			//������DDRAM����
#define DDRAM_RESET 0x02	//DDRAM��ַ��Ϊ0����ʾDDRAM�ޱ仯
//��ʾ״̬
#define ALL_ON 0x0c
#define CURSOR_ON 0x0a
#define CURSORSITE_ON 0x09
//�α����ʾ��λ����
#define	CURSORDUN_LEFT 0x10		//�α������ƶ�
#define CURSORDUN_RIGHT 0x14	//�α������ƶ�
//�����趨
#define COMMAND_BASIC	0x30	//����ָ�����
#define COMMAND_EXTRA	0x34	//����ָ�����
//�趨CGRAM��ַ
#define CGRAMADDR_SET	0x40	//AC0~AC5
//�趨DDRAM��ַ
#define DDRAMADDR_SET	0x80	//AC0~AC6

//RS=0\RW =1��״̬
//RS=1\RW =0д����
//RS=1\RW =1��RAM

/***************����ָ��ģʽ��*************/
//����ģʽ
#define STANDBY 0x01
//����ַ��ַѡ��
#define SCROLLADDR_SEL 0x03
//IRAM��ַѡ��
#define IRAMADDR_SEL 0x02
//����ѡ�� 
#define ANTIWHITE	0x04
//˯��ģʽ
#define MOD_SLEEP	0x08
#define MOD_WAKE	0x0c
//���书���趨
#define PICDRAW_ON 0x32
#define PICDRAW_OFF 0x30

//sbit CS=P5^4;	    //Ƭѡ�ź� RS ST
//sbit SID=P1^7;		//�����ź� RW DS
//sbit SCLK=P1^6;		//ʱ���ź�  E SH

#define LCD_CSH GPIO_SetBits(GPIOA, GPIO_Pin_10)//ST RS RES
#define LCD_CSL GPIO_ResetBits(GPIOA,GPIO_Pin_10)

#define LCD_SIDH GPIO_SetBits(GPIOA, GPIO_Pin_9)//DATA RW  SDA
#define LCD_SIDL GPIO_ResetBits(GPIOA, GPIO_Pin_9)

#define LCD_SCKH GPIO_SetBits(GPIOD, GPIO_Pin_7)//CLK E SCL
#define LCD_SCKL GPIO_ResetBits(GPIOD, GPIO_Pin_7)

#define LCD_SID GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)

enum{
	WRITE_CMD=0,
	WRITE_DATA,
};
extern u8 LcdRam[1024];
void LcdInit(void);
void LcdWriteData(u8 dat);
u8 LcdReadByte( void );
void LcdWriteCmd(u8 cmd);
void LcdSetPos(u8 row, u8 col);
void LcdScreenClear(void);
void LcdRAMShowStr(u8 row, u8 col,u8 *p,u8 len);
//void LcdShowChinse(u8 row,u8 col,u8 *str);
void LcdShowChar(u8 row,u8 col,u8 dat);
void LcdShowString(u8 row,u8 col,u8 *s);
void LcdShowNum(u8 row,u8 col ,u32 num,u8 size,u8 mode);
void LcdDisPic(const u8 *dat);
//void LcdShow1632Pic(u8 x,u8 y,u8 const *bmp);
void LcdGDRAMClear(void);
void LcdShow1616Anti(u8 x,u16 y,u8 sign,u8 *bmp);
void LcdShowAnti(u8 row,u8 sx,u8 len,u8 bkcor);
void LcdShowChineseAnti(u8 x,u8 y,u8 end_x,u8 clear);
void LcdDrawPoint(u8 x,u8 y,u8 Mode);

void LcdLoadPic(u8 *dat);
void lcd_row_on(u8 temp);
void LcdGDRAMClear(void);
void ShowMainPage(void);
void Display(u8 *str,u8 len);
void DrawPoint(u8 y,u8 x,u8 mode);
void DrawFlagPoint(void);
void RamShowErrClear(void);
void RamShow6_8Num(u8 y,u8 x,u32 num);
void LcdMainPageRefresh(void);
void ShowMainData(void);
void LcdTicketRefresh(u8 x,u8 y,u8 len,u8 high);
void LcdShowVersion(u32 ver);
void LcdClearRam(u8 x,u8 y,u8 len,u8 high);
#endif

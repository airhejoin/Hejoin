#ifndef _TFT32_H_
#define _TFT32_H_

#include"includes.h"

#define RS485RxEn()		GPIO_ResetBits(GPIOD, GPIO_Pin_10)
#define RS485TxEn()  	GPIO_SetBits(GPIOD, GPIO_Pin_10)

//��ɫ��Ӧ��RGBֵ
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
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ
#define DARKBLUE   	0X01CF	//����ɫ
#define LIGHTBLUE   0X7D7C	//ǳ��ɫ  
#define GRAYBLUE    0X5458 //����ɫ
#define LIGHTGREEN	0X841F //ǳ��ɫ
#define LGRAY 			0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE  	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE     	0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
//!������
enum
{   
	CMD_RESET=1,				//��λָ��1
	CMD_VERSION,			//�汾�̼���ȡ2
	CMD_DISPVERSION,	//��ʾ�汾�̼�3
	CMD_DISPPIC,			//��ʾͼƬ4
	CMD_SCREENCLEAR,	//����5
	CMD_AREAFREASH,		//����ˢ��6
	CMD_SHOWSTRING,		//��ʾ�ַ�7
	CMD_SHOWSTRINGMIX,//��ʾ�മ�ַ�8
	CMD_SHOWNUM,			//��ʾ����9
	CMD_SHOWNUMMIX,		//��ʾ�മ����10
	CMD_MIXDISP,			//�����ʾ11
	CMD_DRAWPOINT,		//����12
	CMD_DRAWLINE,			//����13
	CMD_DRAWCIRCLE,		//��Բ14
	CMD_DRAWRECTANGLE,//������15
	CMD_BLOCKFILL,		//�����ɫ16
	CMD_ONOFF,				//��ʾ����17
	CMD_BACKLIGHT,		//�������18
	CMD_READCOLOR,		//��ȡ����ɫ19
};
enum{
	PAGE_START=0,		//0
	PAGE_MAIN,			//1
	PAGE_SUCCEED,		//2	
	PAGE_FAIL,			//3	
	PAGE_SET1,			//4
	PAGE_SET2,			//5
	PAGE_COINPLAY,	//6
	PAGE_GIFTPROBSEL,//7ѡ����Ʒ��
	PAGE_SETGIFTPROB,//8������Ʒ�˸���
	PAGE_HARDLEVEL,	//9�Ѷ�����
  PAGE_TIMERANGE,	//10�Ѷ�����
	PAGE_GAMEVOL,		//11������Ϸ����
	PAGE_BACKVOL,		//12���ñ�������
	PAGE_GIFTBAR,		//13��Ʒ�˲���
	PAGE_CURBILL,		//14��ѯ��ǰ��Ŀ
	PAGE_CLEARBILL,//15�����Ŀ
	PAGE_SETDEFALT,	//16�ָ�����
	PAGE_SYSTEMINFO,//17ϵͳ��Ϣ
	PAGE_ARROWS,		//18��ͷ
	PAGE_EXIT,			//19�Ƿ񱣴��˳�
	PAGE_GSMOFF,		//20GSM�ر�
	PAGE_GSMON,			//21GSM��
	PAGE_FREEMODE,	//22���ģʽ
	PAGE_GAME,			//23��Ϸ����
	PAGE_SELGIFT,		//24��Ʒѡ��
	PAGE_TOTALBILL,	//25��ѯ����Ŀ
	PAGE_SETPROB1,	//26��Ʒ������1
	PAGE_SETPROB2,	//27
	PAGE_SETPROB3,	//28
	PAGE_SETPROB4,	//29
	PAGE_SETPROB5,	//30
	PAGE_MAX,				//
};
enum{
	FONT12=0,
	FONT16,
	FONT24,
	FONT32,
	FONT48,
	FONT56,
};
enum{
	FONT_MODE_OVERLAY=0,//������ʾ
	FONT_MODE_FILL,//�����ʾ
};
enum{
	ALIGN_CENTER=0x00,			//������ʾ
	ALIGN_LEFT=0x10,				//������ʾ
	ALIGN_RIGHT=0x20,				//������ʾ
};
//extern xSemaphoreHandle xRS485RecSemaphore;
//extern xSemaphoreHandle xRS485SendSemaphore;
//extern xQueueHandle     xRS485AskQueue; 
//extern S_COM RS485Rec,RS485Send;
void vRS485ProcessTask( void *pvParameters );
void vRS485SendTask( void *pvParameters );
void vTftCtlTask( void *pvParameters );
void TFT32_SetReset(void);
void TFT32_ReadVersion(void);
void TFT32_DisplayVersion(u16 x,u16 y,u8 font,u16 color);
void TFT32_DisplayPic(u16 x,u16 y,u8 picid);
void TFT32_ScreenClear(u8 color);
void TFT32_RefreshBlock(u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u8 picid);
void TFT32_DispString(u16 x,u16 y,u8 font,u16 color,u8 mode,u8 len,u8 *string);
void TFT32_DispNum(u16 x,u16 y,u8 font,u16 color,u8 mode,u32 num,u8 size);
void TFT32_DrawPoint(u16 x,u16 y,u16 color);
void TFT32_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void TFT32_DrawCircle(u16 x,u16 y,u16 r,u16 color);
void TFT32_DrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void TFT32_FillBlock(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void TFT32_ReadColor(u16 x,u16 y);
#endif


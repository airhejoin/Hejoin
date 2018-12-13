#ifndef _TFT32_H_
#define _TFT32_H_

#include"includes.h"

#define RS485RxEn()		GPIO_ResetBits(GPIOD, GPIO_Pin_10)
#define RS485TxEn()  	GPIO_SetBits(GPIOD, GPIO_Pin_10)

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
//!命令字
enum
{   
	CMD_RESET=1,				//复位指令1
	CMD_VERSION,			//版本固件读取2
	CMD_DISPVERSION,	//显示版本固件3
	CMD_DISPPIC,			//显示图片4
	CMD_SCREENCLEAR,	//清屏5
	CMD_AREAFREASH,		//区域刷新6
	CMD_SHOWSTRING,		//显示字符7
	CMD_SHOWSTRINGMIX,//显示多串字符8
	CMD_SHOWNUM,			//显示数字9
	CMD_SHOWNUMMIX,		//显示多串数字10
	CMD_MIXDISP,			//混合显示11
	CMD_DRAWPOINT,		//画点12
	CMD_DRAWLINE,			//画线13
	CMD_DRAWCIRCLE,		//画圆14
	CMD_DRAWRECTANGLE,//画方框15
	CMD_BLOCKFILL,		//填充颜色16
	CMD_ONOFF,				//显示开关17
	CMD_BACKLIGHT,		//背光调节18
	CMD_READCOLOR,		//读取点颜色19
};
enum{
	PAGE_START=0,		//0
	PAGE_MAIN,			//1
	PAGE_SUCCEED,		//2	
	PAGE_FAIL,			//3	
	PAGE_SET1,			//4
	PAGE_SET2,			//5
	PAGE_COINPLAY,	//6
	PAGE_GIFTPROBSEL,//7选择礼品杆
	PAGE_SETGIFTPROB,//8设置礼品杆概率
	PAGE_HARDLEVEL,	//9难度设置
  PAGE_TIMERANGE,	//10难度设置
	PAGE_GAMEVOL,		//11设置游戏音量
	PAGE_BACKVOL,		//12设置北京音量
	PAGE_GIFTBAR,		//13礼品杆测试
	PAGE_CURBILL,		//14查询当前账目
	PAGE_CLEARBILL,//15清除账目
	PAGE_SETDEFALT,	//16恢复出厂
	PAGE_SYSTEMINFO,//17系统信息
	PAGE_ARROWS,		//18箭头
	PAGE_EXIT,			//19是否保存退出
	PAGE_GSMOFF,		//20GSM关闭
	PAGE_GSMON,			//21GSM打开
	PAGE_FREEMODE,	//22免费模式
	PAGE_GAME,			//23游戏界面
	PAGE_SELGIFT,		//24礼品选择
	PAGE_TOTALBILL,	//25查询总帐目
	PAGE_SETPROB1,	//26礼品杆数字1
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
	FONT_MODE_OVERLAY=0,//叠加显示
	FONT_MODE_FILL,//填充显示
};
enum{
	ALIGN_CENTER=0x00,			//居中显示
	ALIGN_LEFT=0x10,				//靠左显示
	ALIGN_RIGHT=0x20,				//靠右显示
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


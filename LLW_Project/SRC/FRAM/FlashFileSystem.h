#ifndef _FLASHFILESYSTEM_H_
#define _FLASHFILESYSTEM_H_

#include"stm32f10x.h"

typedef struct
{
	u8 Num;//图片数量
	u32 UsableSize;//剩余空间
	u32 UsableStrAddr;
	u8 Idex;//当前推片索引
}PIC_SAVE;
//extern PIC_SAVE PicSave;
typedef struct{
	u16 X;
	u16 Y;
	u8 Idex;
	u32 Size;
	u32 Count;
	u32 Addr;
	u32 Times;
}PICTURE;
extern PICTURE Picture;
u8 FlashFileSysInit(void);
void SpaceErase(u32 WriteAddr,u16 NumByteToWrite);
void W25QxxUsartWrite(u32 WriteAddr,u32 NumByteToWrite);
#endif

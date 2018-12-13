#ifndef __FM24C04_H
#define __FM24C04_H

#define EEPROM_ADDR		0xA0

#define USER_ADDR        0x00


void FM24WriteByte(u16 addr,u8 data);
u8 FM24ReadByte(u16 addr);
void FM24ReadBuf(u16 addr , u8 *ch,u8 len);
u8 FM24WriteBuf(u32 addr, u8 *pbuff, u32 len);


#define	SAVE24C04(ST_NAME)	do{\
	portENTER_CRITICAL();\
	FM24WriteBuf( APP_FRAM_ADDR+((INT32U)(&(ST_NAME)))- (INT32U)&System,\
	(INT8U*)&(ST_NAME),sizeof(ST_NAME));\
	portEXIT_CRITICAL();\
}while(0)

#endif


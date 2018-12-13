#ifndef __TASKKEY_H
#define __TASKKEY_H


typedef struct{
  u8  Status;
  u8  ShortPress;
  u8  LongPress;
  u16 PressCount[5];
  u16 FreeCount[5];
}S_SETKEY;
extern S_SETKEY Key;

void vKeyProcessTask( void *pvParameters );//10ms采集一次
#endif

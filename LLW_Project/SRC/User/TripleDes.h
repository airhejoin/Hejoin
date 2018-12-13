#ifndef __TRIPLEDES_H
#define __TRIPLEDES_H

static void doxor(INT8U* sourceaddr,INT8U* targetaddr,INT8U length);
static void setbit(INT8U* dataddr,INT8U pos,INT8U b0);
static INT8U getbit(INT8U* dataddr,INT8U pos);
static void selectbits(INT8U* source,const INT8U* table,INT8U* target,INT8U count);
static void shlc(INT8U* data_p);
static void shrc(INT8U* data_p);
static void strans(INT8U* index,INT8U* target);

extern void Des(INT8U *data_p,INT8U* key_p,INT8U type);
extern void TripDes(INT8U *data_p, INT8U* key_x, INT8U *key_y, INT8U type);


#endif
























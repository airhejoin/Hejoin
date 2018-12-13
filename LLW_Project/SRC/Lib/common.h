#ifndef __COMMON_H
#define __COMMON_H

typedef    unsigned char    BOOLEAN;
typedef    unsigned char    INT8U;
typedef    signed char      INT8S;
typedef    unsigned short int     INT16U;
typedef    signed short int       INT16S;
typedef    unsigned int    INT32U;
typedef    signed int      INT32S;
typedef    float            FP32;
typedef    long double      FP64;

union INT_CHAR
{
    	INT16U i;
    	INT8U  j[2];
};

union LONG_CHAR
{
    INT32U i;
   	INT8U  j[4];
};

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;

#endif



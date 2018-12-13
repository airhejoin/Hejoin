#ifndef _PIC_H_
#define _PIC_H_



#if LANGUAGE_CN //中文版本

#define CPLAYNUM_Y 8
#define CPLAYNUM_X1 1
#define CPLAYNUM_X2 10

#define CPLAYNUM_X3 35

#else //英文版

#define CPLAYNUM_Y 13
#define CPLAYNUM_X 29

#endif

#define COINNUM_Y 15
#define COINNUM_X1 90
#define COINNUM_X2 98
#define COINNUM_X3 92

#if 1
#define TICKETNUM_Y 40
#define TICKETNUM_X1 79
#define TICKETNUM_X2 87
#define TICKETNUM_X3 95
#else
#define TICKETNUM_Y 39
#define TICKETNUM_X1 68
#define TICKETNUM_X2 76
#define TICKETNUM_X3 84
#endif

#define BALL_Y  15
#define BALL_X1 60

#define TIMENUM_Y 47
#define TIMENUM_X1 51

#define Font6_8Max 37
extern unsigned char const CompanyLogo[];
extern unsigned char const MainPage[];
extern unsigned char const ArrowTab[];
extern unsigned char const FailPage[];
extern unsigned char const SucceedPage[];

extern unsigned char const Font6_8Tab[Font6_8Max][16];
#endif



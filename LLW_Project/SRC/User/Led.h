#ifndef __LED_H
#define __LED_H

#define Led_CA    0xFF    // 共阳极，Seg  低电平亮
#define Led_CC    0x00    // 共阴极，Seg  高电平亮


// 数码管极性设置
#define LedPin    Led_CC    


// 数码管段位设置
#define BIT0    0
#define BIT1    1
#define BIT2    2
#define BIT3    3
#define BIT4    4
#define BIT5    5
#define BIT6    6
#define BIT7    7

#define LedSeg_a  ( 1<<BIT0 )
#define LedSeg_b  ( 1<<BIT1 )
#define LedSeg_c  ( 1<<BIT2 )
#define LedSeg_d  ( 1<<BIT3 )
#define LedSeg_e  ( 1<<BIT4 )
#define LedSeg_f  ( 1<<BIT5 )
#define LedSeg_g  ( 1<<BIT6 )
#define LedSeg_h  ( 1<<BIT7 )    


// 编码计算
#define LedChar_0 LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c |LedSeg_d |LedSeg_e |LedSeg_f )
#define LedChar_1 LedPin ^ ( LedSeg_b |LedSeg_c )
#define LedChar_2 LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_d |LedSeg_e |LedSeg_g )
#define LedChar_3 LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c |LedSeg_d |LedSeg_g )
#define LedChar_4 LedPin ^ ( LedSeg_b |LedSeg_c |LedSeg_f |LedSeg_g )
#define LedChar_5 LedPin ^ ( LedSeg_a |LedSeg_c |LedSeg_d | LedSeg_f |LedSeg_g )
#define LedChar_6 LedPin ^ ( LedSeg_a |LedSeg_c |LedSeg_d | LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_7 LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c )
#define LedChar_8 LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_9 LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_f  |LedSeg_g )
#define LedChar_A LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_e |LedSeg_f  |LedSeg_g )
#define LedChar_B LedPin ^ ( LedSeg_c |LedSeg_d |LedSeg_e | LedSeg_f |LedSeg_g )
#define LedChar_C LedPin ^ ( LedSeg_a |LedSeg_d |LedSeg_e | LedSeg_f )
#define LedChar_D LedPin ^ ( LedSeg_b |LedSeg_c |LedSeg_d | LedSeg_e |LedSeg_g )
#define LedChar_E LedPin ^ ( LedSeg_a |LedSeg_d |LedSeg_e | LedSeg_f |LedSeg_g )
#define LedChar_F LedPin ^ ( LedSeg_a |LedSeg_e |LedSeg_f | LedSeg_g )
#define LedChar_G LedPin ^ ( LedSeg_a |LedSeg_c |LedSeg_d | LedSeg_e |LedSeg_f )
#define LedChar_H LedPin ^ ( LedSeg_b |LedSeg_c |LedSeg_e | LedSeg_f |LedSeg_g )
#define LedChar_I LedPin ^ ( LedSeg_e |LedSeg_f  )

#define LedChar_J LedPin ^ ( LedSeg_b |LedSeg_c | LedSeg_d| LedSeg_e )
#define LedChar_K LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_L LedPin ^ ( LedSeg_d |LedSeg_e |LedSeg_f )
#define LedChar_M LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_N LedPin ^ ( LedSeg_c |LedSeg_e |LedSeg_g )
#define LedChar_O LedPin ^ ( LedSeg_c |LedSeg_d |LedSeg_e | LedSeg_g )
#define LedChar_P LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_e | LedSeg_f |LedSeg_g )
#define LedChar_Q LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_R LedPin ^ ( LedSeg_e |LedSeg_g )
#define LedChar_S LedPin ^ ( LedSeg_a |LedSeg_c | LedSeg_d| LedSeg_f  |LedSeg_g )
#define LedChar_T LedPin ^ ( LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_U LedPin ^ ( LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f )
#define LedChar_V LedPin ^ ( LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f )
#define LedChar_W LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_X LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )
#define LedChar_Y LedPin ^ ( LedSeg_b |LedSeg_c | LedSeg_d|LedSeg_f |LedSeg_g )
#define LedChar_Z LedPin ^ ( LedSeg_a |LedSeg_b |LedSeg_c | LedSeg_d |LedSeg_e |LedSeg_f |LedSeg_g )

#define LedChar__ LedPin ^ ( LedSeg_g )

#define LedChar_  LedPin ^  LedSeg_g


#define LedChar_ON   LedPin ^ 0xFF
#define LedChar_OFF  LedPin ^ 0x00




#endif




/*
*********************************************************************************
*                                 dsPIC30F4011
*                     基于MPLAB集成开发环境,MCC30编译器应用库
*									 Ver2.0
*                                   皇家科技
*
*模  块：FM1208
*设计者：Andy
*时  间：2011-03-21
*********************************************************************************
*/

#ifndef __FM1208_H
#define __FM1208_H

//#define DEBUGFM1208   //定义该宏，表示测试FM1208,打印调试信息


//复旦微COS操作命令
/*************************FMCOS2.0命令表***************************/
#define		FMCOS_VERIFY			                0x20   //验证口令
#define     FMCOS_EXTERNAL_AUTHENTICATE             0x82   //外部认证
#define     FMCOS_GET_CHALLENGE                     0x84   //取随机数
#define     FMCOS_INTTERNAL_AUTHENTICATE            0x88   //内部认证
#define     FMCOS_SELECT                            0xa4   //选择文件
#define     FMCOS_READ_BINARY                       0xb0   //读二进制文件
#define     FMCOS_READ_RECORD                       0xb2   //读记录文件
#define     FMCOS_GET_RESPONSE                      0xc0   //取响应数据
#define     FMCOS_UPDATE_BINARY                     0xd0   //写二进制文件
#define     FMCOS_UPDATE_RECORD                     0xd2   //写记录文件
#define     FMCOS_CARD_BLOCK                        0x16   //卡片锁定
#define     FMCOS_APPLICATION_UNBLOCK               0x18   //应用解锁
#define     FMCOS_APPLICATION_BLOCK                 0x1e   //应用锁定
#define     FMCOS_PIN_UNBLOCK                       0x24   //个人密码解锁
#define     FMCOS_UNBLOCK                           0x2c   //解锁被锁住的口令
#define     FMCOS_INITLALIZE                        0x50   //初始化交易           建设部
#define     FMCOS_CREDIT_FORLOAD                    0x52   //圈存
#define     FMCOS_DEBIT_FORPURCHASE                 0x54   //消费、取现、圈提
#define     FMCOS_UPDATE_OVERDRAW_LIMIT             0x58   //修改透支限额
#define     FMCOS_GET_TRANS_CATION_PROVE            0x5a   //取交易认证           建设部
#define     FMCOS_GET_BALANCE                       0x5c   //读余额
#define     FMCOS_RELOAD_CHANGE_PIN                 0x5e   //重装、修改个人密码
#define     FMCOS_ERASE_DF                          0x0e   //擦除DF               专有
#define     FMCOS_PULL                              0x30   //专用消费             建设部
#define     FMCOS_CHARGE                            0x32   //专用充值				建设部
#define     FMCOS_WRITE_KEY                         0xd4   //增加或者修改密钥  	专有
#define     FMCOS_CREATE                            0xe0   //建立文件          	专有

#define     FMCOS_WRITE_EEPROM                      0x00   //写数据EEPROM      	生产测试
#define     FMCOS_READ_EEPROM                       0x04   //读数据EEPROM      	生产测试
#define     FMCOS_INITLAL_EEPROM                    0x02   //初始化EEPROM      	生产测试
#define     FMCOS_READ_ROM                          0x0c   //读程序ROM         	生产测试
#define     FMCOS_CALCULATE_ROM_CRC                 0x0a   //计算程序ROM CRC   	生产测试


/*****************************应用函数定义**********************************/
/*发送请求应答信息*/
extern INT8U Rats(INT8U *ATS);

/*发送COS命令*/
extern INT8U Coscommand(INT8U *send,INT8U s_len,INT8U *rece,INT8U *r_len,INT8U mode);

/*COS请求延时*/
extern void WaitFM1208Ready(void);
#endif




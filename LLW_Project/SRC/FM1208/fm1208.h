/*
*********************************************************************************
*                                 dsPIC30F4011
*                     ����MPLAB���ɿ�������,MCC30������Ӧ�ÿ�
*									 Ver2.0
*                                   �ʼҿƼ�
*
*ģ  �飺FM1208
*����ߣ�Andy
*ʱ  �䣺2011-03-21
*********************************************************************************
*/

#ifndef __FM1208_H
#define __FM1208_H

//#define DEBUGFM1208   //����ú꣬��ʾ����FM1208,��ӡ������Ϣ


//����΢COS��������
/*************************FMCOS2.0�����***************************/
#define		FMCOS_VERIFY			                0x20   //��֤����
#define     FMCOS_EXTERNAL_AUTHENTICATE             0x82   //�ⲿ��֤
#define     FMCOS_GET_CHALLENGE                     0x84   //ȡ�����
#define     FMCOS_INTTERNAL_AUTHENTICATE            0x88   //�ڲ���֤
#define     FMCOS_SELECT                            0xa4   //ѡ���ļ�
#define     FMCOS_READ_BINARY                       0xb0   //���������ļ�
#define     FMCOS_READ_RECORD                       0xb2   //����¼�ļ�
#define     FMCOS_GET_RESPONSE                      0xc0   //ȡ��Ӧ����
#define     FMCOS_UPDATE_BINARY                     0xd0   //д�������ļ�
#define     FMCOS_UPDATE_RECORD                     0xd2   //д��¼�ļ�
#define     FMCOS_CARD_BLOCK                        0x16   //��Ƭ����
#define     FMCOS_APPLICATION_UNBLOCK               0x18   //Ӧ�ý���
#define     FMCOS_APPLICATION_BLOCK                 0x1e   //Ӧ������
#define     FMCOS_PIN_UNBLOCK                       0x24   //�����������
#define     FMCOS_UNBLOCK                           0x2c   //��������ס�Ŀ���
#define     FMCOS_INITLALIZE                        0x50   //��ʼ������           ���貿
#define     FMCOS_CREDIT_FORLOAD                    0x52   //Ȧ��
#define     FMCOS_DEBIT_FORPURCHASE                 0x54   //���ѡ�ȡ�֡�Ȧ��
#define     FMCOS_UPDATE_OVERDRAW_LIMIT             0x58   //�޸�͸֧�޶�
#define     FMCOS_GET_TRANS_CATION_PROVE            0x5a   //ȡ������֤           ���貿
#define     FMCOS_GET_BALANCE                       0x5c   //�����
#define     FMCOS_RELOAD_CHANGE_PIN                 0x5e   //��װ���޸ĸ�������
#define     FMCOS_ERASE_DF                          0x0e   //����DF               ר��
#define     FMCOS_PULL                              0x30   //ר������             ���貿
#define     FMCOS_CHARGE                            0x32   //ר�ó�ֵ				���貿
#define     FMCOS_WRITE_KEY                         0xd4   //���ӻ����޸���Կ  	ר��
#define     FMCOS_CREATE                            0xe0   //�����ļ�          	ר��

#define     FMCOS_WRITE_EEPROM                      0x00   //д����EEPROM      	��������
#define     FMCOS_READ_EEPROM                       0x04   //������EEPROM      	��������
#define     FMCOS_INITLAL_EEPROM                    0x02   //��ʼ��EEPROM      	��������
#define     FMCOS_READ_ROM                          0x0c   //������ROM         	��������
#define     FMCOS_CALCULATE_ROM_CRC                 0x0a   //�������ROM CRC   	��������


/*****************************Ӧ�ú�������**********************************/
/*��������Ӧ����Ϣ*/
extern INT8U Rats(INT8U *ATS);

/*����COS����*/
extern INT8U Coscommand(INT8U *send,INT8U s_len,INT8U *rece,INT8U *r_len,INT8U mode);

/*COS������ʱ*/
extern void WaitFM1208Ready(void);
#endif




/*
*********************************************************************************
*                                 dsPIC30F4011
*                     ����MPLAB���ɿ�������,MCC30������Ӧ�ÿ�
*									 Ver1.1
*                                   �ʼҿƼ�
*
*ģ  �飺FM1702
*����ߣ�Andy
*ʱ  �䣺2010-06-10
*********************************************************************************
*/
#ifndef __FM1702_H
#define __FM1702_H

/* FM1702������ */
#define Transceive	0x1E			/* ���ͽ������� */
#define Transmit	0x1a			/* �������� */
#define ReadE2		0x03			/* ��FM1702 EEPROM���� */
#define WriteE2		0x01			/* дFM1702 EEPROM���� */
#define Authent1	0x0c			/* ��֤������֤���̵�1�� */
#define Authent2	0x14			/* ��֤������֤���̵�2�� */
#define LoadKeyE2	0x0b			/* ����Կ��EEPROM���Ƶ�KEY���� */
#define LoadKey		0x19			/* ����Կ��FIFO���渴�Ƶ�KEY���� */
#define RF_TimeOut	0x3ff			/* ����������ʱʱ�� ysz ԭ��Ϊfff����С���ڿ�״̬�����仯ʱ������Ѹ�ٷ�Ӧ */
#define Req			0x01
#define Sel			0x02

/* ��Ƭ���Ͷ��嶨�� */
#define TYPEA_MODE	0			/* TypeAģʽ */
//#define SHANGHAI_MODE	2			/* �Ϻ�ģʽ */
#define TM0_HIGH	0xf0			/* ��ʱ��0��λ,4MS��ʱ */
#define TM0_LOW		0x60			/* ��ʱ��0��λ */
#define TIMEOUT		100			/* ��ʱ������4MS��100=0.4�� */

/* ��Ƶ��ͨ�������붨�� */
#define RF_CMD_REQUEST_STD	0x26
#define RF_CMD_REQUEST_ALL	0x52
#define RF_CMD_ANTICOL		0x93
#define RF_CMD_SELECT		0x93
#define RF_CMD_AUTH_LA		0x60
#define RF_CMD_AUTH_LB		0x61
#define RF_CMD_READ		    0x30
#define RF_CMD_WRITE		0xa0
#define RF_CMD_INC		    0xc1
#define RF_CMD_DEC		    0xc0
#define RF_CMD_RESTORE		0xc2
#define RF_CMD_TRANSFER		0xb0
#define RF_CMD_HALT		    0x50



/* ����������붨�� ERR CODE  */
#define FM1702_OK			0		/* ��ȷ */
#define FM1702_NOTAGETR		1		/* �޿� */
#define FM1702_CRCERR		2		/* ��ƬCRCУ����� */
#define FM1702_EMPTY		3		/* ��ֵ������� */
#define FM1702_AUTHERR		4		/* ��֤���ɹ� */
#define FM1702_PARITYERR	5		/* ��Ƭ��żУ����� */
#define FM1702_CODEERR		6		/* ͨѶ����(BCCУ���) */
#define FM1702_SERNRERR		8		/* ��Ƭ���кŴ���(anti-collision ����) */
#define FM1702_SELECTERR	9		/* ��Ƭ���ݳ����ֽڴ���(SELECT����) */
#define FM1702_NOTAUTHERR	10		/* ��Ƭû��ͨ����֤ */
#define FM1702_BITCOUNTERR	11		/* �ӿ�Ƭ���յ���λ������ */
#define FM1702_BYTECOUNTERR	12		/* �ӿ�Ƭ���յ����ֽ����������������Ч */
#define FM1702_RESTERR		13		/* ����restore�������� */
#define FM1702_TRANSERR		14		/* ����transfer�������� */
#define FM1702_WRITEERR		15		/* ����write�������� */
#define FM1702_INCRERR		16		/* ����increment�������� */
#define FM1702_DECRERR		17		/* ����decrement�������� */
#define FM1702_READERR		18		/* ����read�������� */
#define FM1702_LOADKEYERR	19		/* ����LOADKEY�������� */
#define FM1702_FRAMINGERR	20		/* FM1702֡���� */
#define FM1702_REQERR		21		/* ����req�������� */
#define FM1702_SELERR		22		/* ����sel�������� */
#define FM1702_ANTICOLLERR	23		/* ����anticoll�������� */
#define FM1702_INTIVALERR	24		/* ���ó�ʼ���������� */
#define FM1702_READVALERR	25		/* ���ø߼�����ֵ�������� */
#define FM1702_DESELECTERR	26
#define FM1702_CMD_ERR		42		/* ������� */

/* FM1702�Ĵ������� */
#define Page_Sel		0x00	/* ҳд�Ĵ��� */
#define Command			0x01	/* ����Ĵ��� */
#define FIFO			0x02	/* 64�ֽ�FIFO�������������Ĵ��� */
#define PrimaryStatus	0x03	/* ��������������FIFO��״̬�Ĵ���1 */
#define FIFO_Length		0x04	/* ��ǰFIFO���ֽ����Ĵ��� */
#define SecondaryStatus	0x05	/* ����״̬�Ĵ���2 */
#define InterruptEn		0x06	/* �ж�ʹ��/��ֹ�Ĵ��� */
#define	RegInterruptEn	0x06

#define Int_Req			0x07	/* �ж������ʶ�Ĵ��� */
#define RegInterruptRq  0x07    // IRQ bits Register

#define Control			0x09	/* ���ƼĴ��� */
#define ErrorFlag		0x0A	/* ����״̬�Ĵ��� */
#define CollPos			0x0B	/* ��ͻ���Ĵ��� */
#define TimerValue		0x0c	/* ��ʱ����ǰֵ */
#define Bit_Frame		0x0F	/* λ֡�����Ĵ��� */
#define TxControl		0x11	/* ���Ϳ��ƼĴ��� */
#define RegTxControl    0x11

#define CWConductance	0x12	/* ѡ�����TX1��TX2�������ߵ��迹 */
#define ModConductance	0x13	/* ������������迹 */
#define CoderControl	0x14	/* �������ģʽ��ʱ��Ƶ�� */
#define RxControl1		0x19	/*�����������*/
#define DecoderControl	0x1a	/* ������ƼĴ��� */
#define RegBitPhase     0x1B
#define RegRxThreshold  0x1C

#define RxControl2		0x1e	/* ������Ƽ�ѡ�����Դ */
#define RegClockQControl    0x1F
#define RxWait				0x21	/* ѡ����ͽ���֮���ʱ���� */
#define ChannelRedundancy	0x22	/* RFͨ������ģʽ���üĴ��� */
#define CRCPresetLSB		0x23
#define CRCPresetMSB		0x24
#define MFOUTSelect		0x26	/* mf OUT ѡ�����üĴ��� */
#define RegFIFOLevel    0x29
#define TimerClock		0x2a	/* ��ʱ���������üĴ��� */
#define RegTimerClock   0x2A

#define TimerControl	0x2b	/* ��ʱ�����ƼĴ��� */
#define TimerReload		0x2c	/* ��ʱ����ֵ�Ĵ��� */
#define RegTimerReload  0x2C

#define RegIRqPinConfig 0x2D

#define TypeSH			0x31	/* �Ϻ���׼ѡ��Ĵ��� */
#define TestDigiSelect	0x3d	/* ���Թܽ����üĴ��� */

/* Status Values */
#define ALL				0x01
#define KEYB			0x04
#define KEYA			0x00
#define _AB				0x40
#define CRC_A			1
#define CRC_B			2
#define CRC_OK			0
#define CRC_ERR 		1
#define BCC_OK			0
#define BCC_ERR 		1

/* �����Ͷ��� */
#define MIFARE_8K		0			/* MIFAREϵ��8KB��Ƭ */
#define MIFARE_TOKEN	1			/* MIFAREϵ��1KB TOKEN��Ƭ */
#define SHANGHAI_8K		2			/* �Ϻ���׼ϵ��8KB��Ƭ */
#define SHANGHAI_TOKEN	3			/* �Ϻ���׼ϵ��1KB TOKEN��Ƭ */

#define mifare1			1
#define mifarepro		2
#define mifarelight		3
#define unknowncard     4

enum
{
	COM_HALT_CARD=1,
	COM_REQUEST_CARD,
	COM_READ_ID,
	COM_SEL_CARD,
};

typedef struct
{
	INT8U tagtype[2];
	INT8U uid[5];
	INT8U uidback[5];
	INT8U revbuffer[40];
}S_CARD;
extern S_CARD S50;

//extern INT8U UID[5];
//extern INT8U RevBuffer[40];
//extern INT8U tagtype[2];

extern INT8U SPIRead(INT8U SpiAddress);
extern void SPIWrite(INT8U SpiAddress,INT8U SpiData);
extern void Init_FM1702(INT8U mode);
extern INT8U Command_Send(INT8U count, INT8U  *buff, INT8U Comm_Set);
extern INT8U MIF_Halt(void);
extern INT8U Load_keyE2_CPY(INT8U Secnr, INT8U Mode);
extern INT8U Request(INT8U mode);
extern INT8U AntiColl(void);
extern INT8U Select_Card(void);
extern INT8U Authentication(INT8U  *UID, INT8U SecNR, INT8U mode);
extern INT8U MIF_READ(INT8U  *buff, INT8U Block_Adr);
extern INT8U MIF_Write(INT8U  *buff, INT8U Block_Adr);
extern INT8U MIF_Increment(INT8U  *buff, INT8U Block_Adr);
extern INT8U MIF_Decrement(INT8U  *buff, INT8U Block_Adr);
extern INT8U MIF_Restore(INT8U Block_Adr);
extern INT8U MIF_Transfer(INT8U Block_Adr);
extern INT8U HL_Active(INT8U Block_Adr, INT8U Mode);
extern INT8U MIF_Initival(INT8U *buff, INT8U Block_Adr);
extern INT8U HL_Read(INT8U *buff, INT8U Block_Adr, INT8U Mode);
extern INT8U HL_Write(INT8U *buff, INT8U Block_Adr, INT8U Mode);


extern INT8U Read_FIFO(INT8U  *buff);

#endif


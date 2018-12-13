/*
*********************************************************************************
*                                 STM32F103C8T6
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver1.1
*                                   �ʼҿƼ�
*
*ģ  �飺CANЭ�����
*����ߣ�Andy
*ʱ  �䣺2010-10-19
*********************************************************************************
*/
#ifndef __TASK_CAN_H
#define __TASK_CNA_H

/*ͨ�Ž���������*/
enum
{
	SETSLAVEID=1,    	//�ն�ID����
	EXECUTEID,        	//ID���ý���
	SETTINGSYS,       	//��������
	DISSYS,        		//��ס�ն�
	ENABLESYS,      	//ʹ���ն�
	COINUP,         	//�Ϸ�
	COINDOWN,        	//�·�
	REQUESTCOIN,       	//�����ܷ�	
	READYSYS,        	//�ն˾���
	JIESUO,     		//�������
	PEIZHI,     		//�����������
	TOUBIOK,			//ֱ��Ͷ��
	TUIBIOK,			//ֱ���˱�
	SENDCARDID,         //!���Ϳ������кź�ID
	SYSTICK,            //!����ϵͳ������
	REFRESHCOIN,        /*!���������ͬ����ұ�ֵ*/
	REFRESHLOTTERY,     /*!���������ͬ����Ҳ�Ʊ*/
	ADDLOTTERY,        /*!���Ӳ�Ʊ*/
	DECLOTTERY,        /*!���ٲ�Ʊ*/
	REQLOTTERY,        /*!�����Ʊ*/
#ifdef MOTHERCARD 
	ADDMONEY,          /*!����������ֵ*/
	DECMONEY,          /*!������������*/
#endif
#ifdef MONIGATE
	ADDMONEY,          /*!����������ֵ*/
	DECMONEY,          /*!������������*/
#endif

	/*!���Զ��۱һ�������*/
#ifdef SEMIAUTOMATIC
	PS2GETSET=0x16,     /*!�����ֵ*/
	PS2TUIBI,           /*!�����˱�����*/
	PS2STOP,            /*!ǿ��ֹͣ����*/
	PS2CLEAR,           /*!����ǰ���˱�ֵ����*/
	PS2ERR,             /*!������Ϣ*/
	PS2TOTALBILL,       /*!��ѯ���������˱�����*/
	PS2TUIBIOVER,       /*!�˱���������*/
	PS2RESETTUIBI,      /*!������ɣ��ָ��˱�*/

	PS2GETPASS,         /*!��������*/
	PS2SETPASS,         /*!����������*/
	PS2SETUNIT,         /*!�����µı�ֵ*/
	PS2SETMOOER,        /*!Ԥ������ﷴת*/
	PS2PASSOK,          /*!������֤ͨ��*/
	PS2PASSERR,         /*!������֤ʧ��*/
	PS2CLEARBILL,
	PS2KEYPASS,
	PS2KEYPRINT,        /*!������ӡ����ӡ���Ӱ���Ŀ*/
#endif
	 //!����Э��
    //!2012-12-05
	SENDPASSWORD=0x27,		//!������֤���Զ�ȡ�һ�
	RESETPASSWORD=0x28, 	//!���¶��趨����
	GETCOINCARD=0x29, 		//!�û�Ա���������۱һ���ȡ��
	GETCOINMONEY=0x2a, 		//!���ֽ����Զ��۱һ���ȡ��
	STORAGECOINCARD=0x2b, 	//!�û�Ա�����
	SELLCOINSEMIMAC=0x2c, 	//!���Զ��۱һ��۱�
	CASHADD=0x2d,			//!��̨������ֵ �����ֽ�
    CASHDEC=0x2e,           //!��̨����ȡ�� �����ֽ�
    OPENMONEYBOX=0x2f,      //!��Ǯ��


	CHECKDATE=0x30,     /*!У��ʱ��*/

	/*!��Ʊ��ӡ��ģ��ʹ��*/
	REQUESTBILL=0x40,    //!��ȡ�˵�
	CLAERBILL,           //!�����ǰ�˵�
	REQUESTTOTALBILL,    //!��ȡ��¼�˵�
	GETBILL,             //!��õ�ǰ�˵�

	/*!�����У��ģ��*/
	ACCEPT_MONEY=0x50,      //!�������������
	MOTHER_READY=0x51,      //!��������
	REQUEST_MOTHER=0x52,    //!��������״̬
	MOTHER_DIS=0x53,        //!����æ
	TUIBIREADY=0x54,        //!���Խ����µ�ֽ��
	TUIBIBUSY=0x55,         //!�����˱ң��������µ�ֽ��
	CHECKMONEY_ENABLE=0x56, //!ʹ��ֽ��ʶ��
	CHECKMONEY_DISABLE=0x57,//!����ֽ��ʶ��
	RMBREADY=0x58,          //!����Ҿ���

    /*!��ӡר������*/
    PRINTBILL=0x60,         //!�����ӡ��Ϣ          
    PRINTCOUNT,             //!��ӡ����
    PRINTLAST,              //!�ϴ���Ŀ
    PRINTCURRENT,           //!������Ŀ
    PRINTDATA,              //!ʱ��

	//!2013-10-07 ��������
	UpGuZhang = 0x70, 		 //�Ϸֹ���
	DownGuZhang = 0x71,		 //�·ֹ���
	AddCaiPiaoGuZhang = 0X72,  //���Ӳ�Ʊ����
	DecCaiPiaoGuZhang = 0x73,//���ٲ�Ʊ����
	CashGetCoinGuZhang = 0x74,//�ֽ�ȡ�ҹ���
    CashChongZhiGuZhang = 0x75, //�ֽ��ֵ����
    ReaderWrongList = 0x76,//!���������� �����1�����ϴ��� 0 ��������·� 1�쳣�ϵ� 2���ױ���
    ReaderParaMode = 0x77, //!�������������·�ʽ
    ReaderGetPara = 0x78,  //!��������ȡ���Ĳ���
	ReaderEnableSetXishu = 0x79, //!�ֶ���������ģʽ�£�ͨ����������ܵ������Ĳ���
	ReaderChangeUnit = 0x7a,   //!������ͷ���л���λ
    ReaderCoreSet = 0x7b, //!��ͷ�󶨿�����ͷ����[����������]

};

/**!CAN��FM1702ͨ�ŵ����ݰ�*/
typedef struct
{
	INT8U ucCMD;

}S_SERVERMSG;
extern S_SERVERMSG PCMSG;

extern void PackCanMsg(CanTxMsg SendMsg, INT8U cmd ,INT8U id, INT8U msgsyn);
extern void vCanprocTask( void *pvParameters );
extern void vCanSendTask( void *pvParameters );

#endif


























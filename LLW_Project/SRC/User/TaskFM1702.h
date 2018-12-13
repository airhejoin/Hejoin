/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺CPU������M1������
*����ߣ�����������
*ʱ  �䣺2012-05-24  00:53:19
*********************************************************************************
*/
#ifndef __TASKFM1702_H
#define __TASKFM1702_H


#define MAXPLAYERCARD    100


#define BUZZERON		    GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define BUZZEROFF			GPIO_ResetBits(GPIOA, GPIO_Pin_9)

#define MONEYBOXON    	 GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define MONEYBOXOFF		 GPIO_SetBits(GPIOC, GPIO_Pin_15)

/*********************************************************************************
	���ñ�������Ϊ16���ᵼ��������������
	��ǰ���Ƕ���Ϊ16������ĳЩδ֪ԭ��û�б�¶������������޸ĳ����ʱ��
	�����������ڽṹ���е�λ�ã��պ�Ӱ�쵽�˺���ı�����������ı���������
	����Ҫ�ģ��ʷ����˴�����
	{

	����֮���Ի�Խ�磬����ΪFM1702���������ʱ��16���ֽڵ����ݣ����滹��
	Ҫ���������֣��ʵ��½ṹ�еĴ���
	
	}

	����������  2010-11-19  01:10:37	
*********************************************************************************/
#define FM1702BUFF      40      //ע��û������Ĵ�С
/*
	IC���õ�������,����������ݽṹ�����Խ�ʡFLSAH�ռ�
	CHAR     1�ֽڶ���
	SHORT    2�ֽڶ���
	INT      4�ֽڶ���
*/
typedef struct
{
	INT32U id;              //����
	INT32U coin;            //��ֵ
	INT32U oldpassword;     //������
	INT32U newpassword;     //������
	INT32U money;           //��ֵת��Ϊ��Ӧ��Ǯ
	INT32U word;            //�޸�����
	INT32U limitup;         //�Ϸ�����
	INT8U  readbuff[FM1702BUFF];    //����������
	INT8U  writebuff[FM1702BUFF];   //д��������
	INT8U  err;             //������
	INT8U  mode;            //��������
	INT8U  search;          //Ѱ�ҿ�
	INT8U  step;            //���ò���
	INT8U  flash;           //ѡ����˸
	INT16U unit;            //Ͷ�ҵ�λ
	INT16U yajin;
}S_IC;
extern S_IC CtrIC;

/*
	ϵͳ��־
*/
typedef struct
{
	INT8U cardready;       //��֤�Ŀ��Ƿ����
	INT8U readorwrite;     //������д������ 0�� 1д
	INT8U list;            //��ѯ��ֵ��¼
	INT8U lock;            //ϵͳ��
	INT8U cardok;          //������
	INT8U addordec;        //��ֵ���߼�ֵ 1��ʾ��ֵ 0��ʾ��ֵ
	INT8U bill;            //��ֵ��־
	INT8U initpassword;    //��ʼ������
	INT8U checkok;         //������֤ͨ��
	INT8U wordstep;        //�������벽��
	INT8U setting;         //���ý���
	INT8U waitcard;        //��Ȩ�Ŀ���λ
	INT8U setcoin;         //������������ñ�ֵ
	INT8U desinbuf[16];     //DES��������
	INT8U desoutbuf[16];     //DES�������
	INT8U descheckbuf[16];   //DESУ������
	INT8U desreadbuf[32];   //DES��M1��������
	INT8U deswritebuf[32];  //DESдM1��������
	INT8U coinerr;          //������Ŀ������Ƿ���M1��
	INT8U desid[4];         //M1����ID����DES���ܣ���ֹ�Ƿ�һ�ſ���¡����

	INT8U id;

	INT8U msg;              /*������Ҫ��ʾ����Ϣ*/
	INT8U update;
	INT8U line;
	INT8U keymsg;
	INT32U code;
	INT8U syn;             //!ͬ���ֶΣ���ΧΪ1-31
	INT8U test;
	INT8U cmd;

	INT32U ps2word;
	INT16U ps2unit;
	INT32U ps2setword;

	/*!CPU���õ�������*/
	INT8U deskey[16];
	INT8U desout[32];

	INT8U clearlcd;
	INT8U backset;      //!��̨����������
	INT8U diserr;       //!������ʾһ��ʱ���ر�
	
	/*!�����õ���־*/
	INT8U step;         //!����ѡ����
	INT8U billsetbit;   //!��Ŀ����
	INT8U locksetbit;
	INT8U nolock;       //!�Ƿ����ϵͳ��
	INT16U noasktime;    //!��������ʱʱ��

	INT8U errlong;      //!�ļ����ȴ���
	INT8U lotteryorcoin;//!ѡ�������Ʊ���߱�
	INT8U lotteryorcoinback;

	INT32U rec;
	INT32U send;
	INT8U serverok;     //!����������
	INT8U tuibilock;
	INT8U tuibilock2;
	INT8U sendcoin;     //!���� 1��ʾ���ң�0��ʾ����
	INT8U sendcoinback;
	INT8U tuibiready;

	INT8U  serverask;
	INT8U  setsysbill;
	INT16U billcoin;   //!�ֽ�ұ�ֵ
	INT8U  enforcelogo;//!ǿ����ʾLOGO
	INT8U  settime;    //!����ʱ��
	INT8U  setdatestep;//!����ʱ�Ӳ���

	INT8U  macsyn[50];   //!��̨ע�������ͬ����
	INT8U  mangercard;   //!�����Ա��״̬
	INT8U  WriteCardStop; //!��ֹд��
	INT32U usbrec;
	INT8U  server_ready;		//!����������

	INT8U  ReaderMode;     //!������ģʽ 0 ���� 1ͬ�����Ͳ�Ʊ
}S_FLAG;
extern S_FLAG FLAG;

/*
	��������д����0����1д
*/
enum 
{
	READCARD=0,
	WRITCARD,
};
enum
{
	ECOIN = 0x50,
	ECOINREG = 0xa0,
	ECOINDEL = 0xb0,
};

/*
	�����������У�״ָ̬ʾ
*/
enum
{
	QUEOK=0,
	ERR_NOCARD,
	ERR_COL,  		//����ͻ
	ERR_SELE,  		//ѡ������
	ERR_AUTHERR,  	//��֤����
	ERR_READ, 		//��������
	ERR_WRI,	 	//д������
	ERR_CRC,		//У�����
	ERR_ADF, 		//��������
	ERR_SUE, 		//����
	OK_REA,			//�����ɹ�
	OK_WRI,			//д���ɹ�
	ERR_PASS,		//�������
	OK_PASS,		//������ȷ
	ERR_LEN,		//���ݳ��ȴ���
};

/***********************M1����������*********************/
enum
{
	DISNOCARD=1,		/*�޿�*/
	DISFIANDCARD,		/*�п�*/
	DISUPCOIN,			/*�����Ϸ�*/
	DISUPREADY,			/*�Ϸֳɹ�*/
	DISDOWNCOIN,		/*�����·�*/
	DISDOWNREADY,		/*�·ֳɹ�*/
	DISCOINLESS,		/*����*/
	DISCOINOVERFLOW,	/*��ֵԽ��*/
	DISERRCHECK,		/*У�����*/
	DISERRPASS,			/*�������*/
	DISRESUME,			/*�ָ���*/
	DISERRCARDID,		/*���Ŵ���*/
	DISERRDES,			/*��ֵ�Ƿ�*/
	DISERRLINE,			/*���ߴ���*/
	DISSETTING,			/*���ÿ�*/
	DISVIEWBILL,		/*�鿴��Ŀ*/
	DISVIEWSET,         /*!��ʾ�������û���*/
	DISERRDEFINE,		/*δ���忨*/
	DISSYSLOCK,         /*ϵͳ����*/
	DISSERNO,           /*��ʾϵͳ����*/

	DISMANGER,          /*!��ʾ������*/
	DISSLAVEBILL,       /*!��ʾ��ͷͳ����Ŀ*/
};

enum
{
	DECCARD=0,				/*�Ϸ�*/
	ADDCARD,				/*�·�*/
};

/*****************************ģ��Ͷ�˱����***************************/
typedef struct
{
	INT8U mode;             /*Ͷ�˱�ģʽ*/
	INT8U stacoin;          /*Ͷ���źų�̬��ƽ*/
	INT8U stacout;			/*�˱��źų�̬��ƽ*/
	INT8U stassr;           /*�˱�ʹ���źų�̬��ƽ*/
	INT8U speed;			/*Ͷ�˱��ٶ�*/
	INT8U pulsecoin;		/*Ͷ������*/
	INT8U pulsecout;		/*�˱�����*/
	INT8U gapcoin;			/*Ͷ�Ҽ��*/
	INT8U gapcout;			/*�˱Ҽ��*/
	INT16U delaycoin;       /*Ͷ�����屣��ʱ��*/
	INT16U delaycout;       /*�˱����屣��ʱ��*/
	INT8U bell;             /*������*/
	INT8U inorout;          /*���·�ѡ��*/
	INT16U ssrok;           /*SSR�ȶ�*/
	INT16U gapcoinkey;      /*Ͷ�Ҽ��*/
	INT16U gapcoutkey;      /*�˱Ҽ��*/
}S_COIN;
extern S_COIN CTRCOIN;

/*
	���ȡ�Ҽ�¼
*/
typedef struct
{
	INT32U id;               //!��ҿ���
	INT32U coin;             //!��ǰ��ֵ
	INT32U num;              //!���ȡ��ֵ
}S_PLAYERRELIST;

/***********************************����ʵ�ַ���������******************************/
//!һ����Ա���Ĳ�����Ϣ
typedef struct
{
	INT8U macid;           //!��̨���
	INT8U sta;             //!���·�
	INT16U credit;         //!��Ա������¼
}S_LISTPLAYER;

//!��Ա��������Ϣ
typedef struct
{
	INT32U uid;             //!��Ա�����ڲ����к�
	INT32U no;              //!��Ա���ı��
	INT32U coin;            //!��Ա�������
	INT32U index;           //!������¼������
	S_LISTPLAYER  list[5];  //!��Ա����5�β�����¼
}S_PLAYERMESSAGE;

//!��Ա�����ݿ�����Ľṹ
typedef struct
{
	INT32U index;            //!��Ա��Ϣ����,������Ϊ0����ʾ��������Ч�����Ը���
	S_PLAYERMESSAGE message;//!��Ա��Ϣ
}S_SERVER;

enum
{
	LISTOPEN=1,              //!����
	LISTCLOSE,               //!ϴ��
	LISTUPCOIN,              //!ˢ���Ϸ�
	LISTDOWNCOIN,            //!ˢ���·�
};

/*
	��Ҫ�洢�������е�����ӳ�䣬������ӱ������벻Ҫ�ı���ǰ��˳��
*/
typedef struct
{
	INT8U  compile_time[20];  	//��ȡϵͳ����ʱ��,�ж��Ƿ��ʼ����Ҫ����
	INT32U codelock;          	//ϵͳ����	
	INT8U  id;                  //����ID
	INT32U password;			//����
	INT32U seed;				//!���������
}S_STEP;
extern S_STEP ST;

/*
	��ʱ��2ʱ������
*/
typedef struct
{
	INT8U buzzer;               //������ 50MS
	INT32U opentime;            //!����ʱ��
}S_TIME;
extern S_TIME TM;

/*
	���������ò���
*/
enum
{
	STEPPASS=2,
	STEPUNIT,
	STEPLIMIT,
	STEPINIT,
	STEPMONEY,
	STEPYAJIN,
};

/*
	ϵͳʱ�Ӳ���
*/
typedef struct
{
	INT8U sec;
	INT8U min;
	INT8U hour;
	INT8U week;
	INT8U day;
	INT8U month;
	INT8U year;
} S_CLOCK;

extern S_CLOCK CLOCK;
extern S_CLOCK CLOCK1;


/*!�����У��ģ��*/
typedef struct
{
	INT8U money;
	INT8U backmoney;
}S_ACCEPT;

extern S_ACCEPT ACCEPT;


#define CAN_RxSize 8		    	//Can���ܻ�������С
extern INT8U CanMsgBeg;				//����һ������CanMsgBeg++
extern INT8U CanMsgEnd;				//д��һ������CanMsgEnd++
extern CanRxMsg CanMsg[CAN_RxSize]; //CAN���ݰ����ջ�����
extern void vFM1702MangerTask( void *pvParameters );
extern void InitCardPassword(void);
extern void StartBuzzer(void);
extern void OpenMoneyBox(void);

extern INT8U CanTxMsgBeg;			//����һ������CanMsgBeg++
extern INT8U CanTxMsgEnd;			//д��һ������CanMsgEnd++
extern CanTxMsg CanTXMsg[CAN_RxSize];//CAN���ݰ����ջ�����

extern INT8U CheckCardList(INT32U cardid);

extern xSemaphoreHandle xAskServerSemaphore;

#endif



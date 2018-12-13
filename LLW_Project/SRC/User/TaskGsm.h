#ifndef _TASKGSM_H_
#define _TASKGSM_H_
#define YUN_RSTL() GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define YUN_RSTH() GPIO_SetBits(GPIOA, GPIO_Pin_1)

//ͨ����ز������� �ط�ʱ��Ϊ3S
enum
{
	CMD_SERVECOIN=0x6161,					//��ģ��Ͷ��ָ�� aa
	CMD_CLIENTSTATUS=0x6262,			//��̨�ϴ�״ָ̬�� 0x62 bb
	CMD_CLIENTCOINCNT=0x6363,			//��̨�ϴ�Ӳ�Ҽ������� 0x63 cc
	CMD_SERVESETPARA=0x6464,			//��ģ�����û�̨����0x64 dd
	CMD_SERVEREADPARA=0x6565,			//��ģ���ȡ��̨����0x65 ee Ӧ����dd
	CMD_SERVESETDEFAULT=0x6666,		//��ģ�����û�̨�ָ����� ff
	CMD_CLIENTONSYNC=0x6767,			//��̨�����ϵ�����	gg
	CMD_CLIENTERROR=0x6868,				//��̨���͹�������hh
	CMD_CLIENTGIFTOUT=0x6969,			//��̨�ϴ��ѳ���Ʒָ�� ii
	CMD_SERVESTATUS=0x6a6a,				//��ģ��״ָ̬�� jj
	CMD_SERVEONSYNC=0x6b6b,				//��ģ���ϵ�ͬ��ָ�� kk
	CMD_SERVERESET=0x6d6d,				//��ģ�������Ʒ��1�ػ� 2����3����������� mm
	CMD_CLIENTREADID=0x616d,			//��Ʒ����ȡģ��Ψһ������am
	CMD_CLIENTREADUSER=0x646d,		//��Ʒ����ȡ�����Զ����������dm
	CMD_CLIENTSETUSER=0x6a6d,			//��Ʒ��д�볧���Զ����������jm
	CMD_CLIENTGETSERVERSTA=0x7070,//��Ʒ����ȡ��ģ��״̬pp
};

//��ģ��״̬
enum{
	SERSTA_INIT=0,//ģ���ʼ��
	SERSTA_TESTUNCONN,	//����ģʽ,δ���ӵ�������1
	SERSTA_UNCONECTBS,//δ���ӵ���վ2
	SERSTA_APREGISTER,//APע��ģʽ��δ����wifi����3
	SERSTA_UNCONNECTSER,//δ���ӵ�������4
	SERSTA_NORMAL,//�������Ϸ�����������ʹ��5
	SERSTA_UNSENDDATA,//������δ�ϴ�������6
	SERSTA_ERROR,//���ϣ���״̬��֧���ƶ�֧������7
	SERSTA_TESTCONN,//����ģʽ�£��Ѿ����ӵ����Է�����8
};
void GsmInit(void);
void vGsmTask( void *pvParameters );
void vGsmProcessTask( void *pvParameters );
void vGsmSendTask( void *pvParameters );
#endif




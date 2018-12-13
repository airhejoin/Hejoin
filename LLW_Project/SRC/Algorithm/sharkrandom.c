/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺����㷨
*����ߣ�����������
*ʱ  �䣺2012-11-30  14:08:19
*********************************************************************************
*/
#include".\Lib\includes.h"
/*
�㷨��������:
1��ϵͳ�������жϴ���ʱ���Ƿ񵽣�ʱ�䵽����ʾ��������
2��ϵͳ���������غ�̨��������ȡϵͳ�ĵ�ǰӯ������ʼ�������
3������PC��ʷ��¼�����¶���
4������������ռ��������Ѻ�֣�
5��������ҵ�Ѻ��������ж���Ϸ���
6��Ӯ�ֻ���

��Ϸ���:
	1���������밵��ʽ�صĸ��ֻҪѺ�֣��ʽ�ؾͳ�ˮ��ͨ���ʽ�����������㣬�ݶ�
	   �ʽ�صĳ�ˮΪ��Ѻ�ֵ�ǧ��֮һ
	2�����н�������ж����������ڷ�ֵ�ϴ󣬲��û�����ı�ֵ���м��㣬�ڴ�˵��
	   A����ǰ����ӯ��
	   B���Ƿ��Ǹմ���2��
	   C����̨���ܽ�����
	   D������ʽ��
	   E����̨13����Ѻ�ֻ��ܡ���̨����Ѻ�ֻ��ܣ�
	   F����̨��Ѻ��ǰ���ף�16�����
	   G��������������Ϸ�(���Ϊ0�����������������ж�)
	   H��ÿ����Ҽ�¼10����¼����̬����Ѻ�ֹ���
	3���ж�����
	   ���Ӫ��Ϊ��������5%������Ĳ��ַų�ȥ���������������ҵ�Ѻ����Ϣ���㷨
	   �����ڶԻ�̨���׶����һ�������̨��Ѻ��������ѡ��ͬʱ�˼������ȡ��2
	   ����ҵ�Ӫ��С���Ȳ���������3����ң�����д���Ȳ�����������̨���Ϊ��ʱ��
	   ϵͳ����ϴ��

*/
S_BET MsgBet;
S_ANT mYAnt;

/*************    ��������CheckBetSmall             **************************
�����ܡ�����̨8��Ѻ�����ð������
����������
�����ء���
��˵������
          ��������
**************   ��ƣ����������� 2012-12-02  10:51:46***********************/
void CheckBetSmall(void)
{
	INT8U i=0,j=0;
	INT32U bettmep=0;

	for(i=0;i<8;i++)
		MsgBet.totalbetloseback[i]=MsgBet.totalbetlose[i];

	/*!ð������*/
	for(i=0;i<8;i++)
	{
		for(j=i+1;j<8;j++)
		{
			if(MsgBet.totalbetloseback[i]<MsgBet.totalbetloseback[j])
			{
				bettmep=MsgBet.totalbetloseback[i];
				MsgBet.totalbetloseback[i]=MsgBet.totalbetloseback[j];
				MsgBet.totalbetloseback[j]=bettmep;
			}
		}
	}
}
/*************    ��������CheckBetBig             **************************
�����ܡ���̨��Ѻ��ð������
����������
�����ء���
��˵������
          ��������
          ����:
              �����ֵ����ȵ�ѡ��ʱ������ֻ������һ��������Ӷ����жϴ���ʧ��
              ���ڸ�������Ľ������֤ÿ�����ֻ����һ�Σ�Ϊ�˸��õ��жϣ��㷨
              �����ͽ��������Ѻ�ֵ�������
**************   ��ƣ����������� 2012-12-02  10:51:46***********************/
void CheckBetBig(INT8U *ptr )
{
	INT8U i=0,j=0,m=0,n=0;
	INT32U bettmep=0;
	INT32U temp[MAXSLAVE]={0};

	for(i=0;i<MAXSLAVE;i++)
		temp[i]=MsgBet.betplayer[i].totalbet;

	/*!ð������*/
	for(i=0;i<MAXSLAVE;i++)
	{
		for(j=i+1;j<MAXSLAVE;j++)
		{
			if(temp[i]<temp[j])
			{
				bettmep=temp[i];
				temp[i]=temp[j];
				temp[j]=bettmep;
			}
		}
	}

	/*!��һ*/
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			break;
	*ptr++=i;

	/*!�ڶ�*/
	m=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			if(i!=m)
				break;
	*ptr++=i;

	/*!����*/
	n=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			if((i!=n)&&(i!=m))
				break;
	*ptr++=i;
#if 0
	/*!��״ԪѺ����ͬ������*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!�Ͱ���Ѻ����ͬ������*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!��̽��Ѻ����ͬ������*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;
#endif
}

/*************    ��������CheckBetAloneBig           *************************
�����ܡ��������Ѻ������
����������
�����ء���
��˵������
          ��������
**************   ��ƣ����������� 2012-12-02  10:51:46***********************/
void CheckBetAloneBig(INT8U *ptr )
{
	INT8U i=0,j=0,m=0,n=0;
	INT32U bettmep=0;
	INT32U temp[MAXSLAVE]={0};

	for(i=0;i<MAXSLAVE;i++)
		temp[i]=MsgBet.betplayer[i].totalbet;

	/*!ð������*/
	for(i=0;i<MAXSLAVE;i++)
	{
		for(j=i+1;j<MAXSLAVE;j++)
		{
			if(temp[i]<temp[j])
			{
				bettmep=temp[i];
				temp[i]=temp[j];
				temp[j]=bettmep;
			}
		}
	}

	/*!��һ*/
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			break;
	*ptr++=i;

	/*!�ڶ�*/
	m=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			if(i!=m)
				break;
	*ptr++=i;

	/*!����*/
	n=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			if((i!=n)&&(i!=m))
				break;
	*ptr++=i;
#if 0
	/*!��״ԪѺ����ͬ������*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!�Ͱ���Ѻ����ͬ������*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!��̽��Ѻ����ͬ������*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;
#endif
}

/***********************������:HappyGame   ***********************************
�����ܡ�����Ϸ�����㷨
����������
�����ء���
��˵������
		  ��ǰ��Ŀ���Ա���Ѻ�ֵĽ�����и�Ԥ��ֻ��Ӱ�쵽�����ķ���
		  1�����������ͨ������������ת��ʵ�֣�����1-100������
		  2��1-20֮��Է�  21-80֮�� ���   81-100֮���ͷ�
		  3��ͨ�������Էֺ��ͷֵķ�Χ���Ӷ�����ϵͳ���Ѷ�
		  
          2013-04-25 �㷨����
          1��Ϊ���㷨����ѧ����������ǰ���̵���ʽ�������������Ϊ�����ӵ�ģ�ͣ���
          �жϽ��������ϣ���3�����ӣ��������ӵĸ�����������Ϸ�Ľ����
          2���ڿ���Ϸ�����ʱ����Ҫ�ɿ����ռ����зֻ�����Ϣ����Ч������̨��
          ��Ӫ��״��
          3������󣬻�̨��ˮ���Ϊ�ϴ�Ӫ�յ�50%����������£���֤��̨������һ��
          ʱ���ڴ��ڷ�ˮ״̬
          4����̨��ķ�Χ�������ô�����ע�����ӯ������ȡ����С����ң�ʵ��
          ��ˮ��Ŀ�ģ�ͬʱ������Ҷ�Ѻ��
          5��ÿ�ֶ��������������
          6�������⵽ĳ���������׷Ѻһ�ţ�����Ѻ�еĸ��ʻ�����
          7�������ϱ�֤�����1�����ӯ����3����ҹ�����4�������Ǯ���������
          ����Ϊ�ƽ�ָ��
          8������и���������Ϸֺ󣬻������ȡ�����˱���
          
          �޸�:
          2013��5-18�źͼ����������죬������۲�ʢ���ֶԳ�������ƵĻ�̨������
          �����⣬�Գƻ�̨������ҿ��Խ���һ��״̬�����۽������һ���������Ӯ
          Ϊ0�������Ļ���������ͽ�����̨�ض����䣬���仰˵��������úͲ�ʢ
          һ�������ʣ���ô��������ĳ��ң��Ͳ����ܲ���ʢ������������бȲ�ʢ
          �Ļ�̨�������������ڴ˽���˵��.
          ͬʱ�и�����˵�������ϰ����˲�ʢ��2�����Թ���ʦ�������ϰ�����㷨����
          ���Թ���ʦ���в��ԣ�����ҵ���һ�����ӽ���ʢ���㷨��
          �Ա��㷨�������ǣ�ͨ����ͬ�İ汾�����в��ԣ�����ѡ��һ�����ʵİ汾������
          2013��5��20��
          ͨ�����ʱ���Ѻ�ֻ�����˼�����㷨��������ֿ��ܸ��٣��������Ҫ����
          ��Ԥ�����������֣���ǰ���õ���ģ�����̵ķ�ʽ����������Բ���̫�ã�����
          ���������ӵķ�ʽ���������ӣ�ͨ���ж��������ͣ�������Ϸ�����������������
          �����Ч���϶����á�
          
**************   ��ƣ����������� 2012-12-12  14:51:46***********************/
/*************    ������: DicePlayer  **************************************
�����ܡ�:������
��������:
�����ء�:
��˵����:
        3-18��ķ�Χ���ڴ�ֻ���ģ�������ͬʱ��ע3�����ӵ���һ���������
        111  3��
        222  6��
        333  9��
        444  12��
        555  15��
        666  18��
        ��3�����ӵ���һ��������󽱣�
        ���ӵ���Ȼ����
        ��������Ȼ����

        �����Ƿ�Ӧ������2��������ȵ�����������ж���Ϊ��Ԥ
**************   ���: ���������� 2013-05-20 9:24:18 **********************/
void DicePlayer(void)
{
	//!�������״̬
	FLAG.dice[0]=FLAG.dice[1]=FLAG.dice[2]=0;
	FLAG.samedice=0;
	FLAG.dicedot=0;

	//!��ʼ������
	FLAG.dice[0]=IRandomX(1,6);
	FLAG.dice[1]=IRandomX(1,6);
	FLAG.dice[2]=IRandomX(1,6);

	//!�жϱ��ӵļ���
	if((FLAG.dice[0]==FLAG.dice[1])&&(FLAG.dice[0]==FLAG.dice[2]))
		FLAG.samedice=1;

	//!�����ܵ���
	FLAG.dicedot=FLAG.dice[0]+FLAG.dice[1]+FLAG.dice[2];
}

/*************    ������: GetPlayerBetMsg  *********************************
�����ܡ�:���Ѻ����Ϣ����
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-20 10:3:20 **********************/
void GetPlayerBetMsg(void)
{
	INT32U i=0,j=0;

	//!����ϴμ�¼
	//!ǫ̈̄����
	MsgBet.shark[0]=0;
	MsgBet.shark[1]=0;
	MsgBet.shark[2]=0;
	//!ǫ̈̄����
	MsgBet.totalbetfowls=0;
	//!ǫ̈̄����
	MsgBet.totalbetbeasts=0;
	//!ǫ̈̄��������
	MsgBet.totalbetfowls4=0;
	//!ǫ̈̄��������
	MsgBet.totalbetbeasts4=0;

	for(j=0;j<8;j++)
		MsgBet.totalbetalone[j]=0;

	/*!���Ѻ����Ϣ���ܣ���������*/
	for(i=0;i<MAXSLAVE;i++)
	{
		//!����
		MsgBet.betplayer[i].sharkbet[0]=CtrGame.MyPlayer.Player[i].bet[11];
		//!����
		MsgBet.betplayer[i].sharkbet[1]=CtrGame.MyPlayer.Player[i].bet[10];
		//!������
		MsgBet.betplayer[i].sharkbet[2]=CtrGame.MyPlayer.Player[i].bet[9];
		//!����
		MsgBet.betplayer[i].betfowls=CtrGame.MyPlayer.Player[i].bet[12];
        //!����
        MsgBet.betplayer[i].betbeasts=CtrGame.MyPlayer.Player[i].bet[8];
        //!���ݵ���
        MsgBet.betplayer[i].betfowls4=CtrGame.MyPlayer.Player[i].bet[4]
                                     +CtrGame.MyPlayer.Player[i].bet[5]
                                     +CtrGame.MyPlayer.Player[i].bet[6]
                                     +CtrGame.MyPlayer.Player[i].bet[7];
        //!���޵���
        MsgBet.betplayer[i].betbeasts4=CtrGame.MyPlayer.Player[i].bet[0]
                                     +CtrGame.MyPlayer.Player[i].bet[1]
                                     +CtrGame.MyPlayer.Player[i].bet[2]
                                     +CtrGame.MyPlayer.Player[i].bet[3];

		//!ȫ��Ѻ��
		MsgBet.betplayer[i].totalbet=MsgBet.betplayer[i].sharkbet[0]
		                            +MsgBet.betplayer[i].sharkbet[1]
		                            +MsgBet.betplayer[i].sharkbet[2]
		                            +MsgBet.betplayer[i].betfowls
		                            +MsgBet.betplayer[i].betbeasts
		                            +MsgBet.betplayer[i].betfowls4
		                            +MsgBet.betplayer[i].betbeasts4;
		
		//!�������Ѻ����������
		GetBetCnt(&MsgBet.betplayer[i].betrank[0],i);

		//!ǫ̈̄����
		MsgBet.shark[0]+=MsgBet.betplayer[i].sharkbet[0];
		MsgBet.shark[1]+=MsgBet.betplayer[i].sharkbet[1];
		MsgBet.shark[2]+=MsgBet.betplayer[i].sharkbet[2];
		//!ǫ̈̄����
		MsgBet.totalbetfowls+=MsgBet.betplayer[i].betfowls;
		//!ǫ̈̄����
		MsgBet.totalbetbeasts+=MsgBet.betplayer[i].betbeasts;
		//!ǫ̈̄��������
		MsgBet.totalbetfowls4+=MsgBet.betplayer[i].betfowls4;
		//!ǫ̈̄��������
		MsgBet.totalbetbeasts4+=MsgBet.betplayer[i].betbeasts4;

		for(j=0;j<8;j++)
			MsgBet.totalbetalone[j]+=CtrGame.MyPlayer.Player[i].bet[j];

	}

	MsgBet.totalbet=MsgBet.totalbetfowls+MsgBet.totalbetbeasts
		           +MsgBet.totalbetfowls4+MsgBet.totalbetbeasts4
		           +MsgBet.shark[0]+MsgBet.shark[1]+MsgBet.shark[2];

	/*!̨��Ѻ������*/
	CheckBetBig(MsgBet.betbigmac);
	/*!����Ѻ������*/
	
	/*!����Ѻ������*/
	for(i=0;i<MAXSLAVE;i++)
		CheckBetAloneBig(MsgBet.betbigalone);
	
	/*!��������,����ǫ̈̄,ת��Ϊ��ֵ��������unitcoin*/
	//!����8��Ѻ��
	for(i=0;i<8;i++)
		MsgBet.totalbetlose[i]=MsgBet.totalbetalone[i]*gamepeilv[mYGame.peilv][i];
	
	MsgBet.totalfowlslose=MsgBet.totalbetfowls<<1;
	MsgBet.totalbeastslose=MsgBet.totalbetbeasts<<1;
	
	//!��������
	MsgBet.totalyinshalose=MsgBet.shark[0]<<5;
	//!��������
	MsgBet.totaljinshalose=MsgBet.shark[1]*40;
	//!����������
	MsgBet.totalkingsharklose=MsgBet.shark[2]*126;

	/*!�������ׯ�е�����*/
	for(i=0;i<8;i++)
	{
		if(i<4)//!����
			MsgBet.totalbetlose[i]+=MsgBet.totalbeastslose;
		else//!����
			MsgBet.totalbetlose[i]+=MsgBet.totalfowlslose;
	}

	//!��������͸�PC��ʾ�����������ж��㷨�Ƿ����
	#if 0
	SendTest1();
	vTaskDelay(100);
	SendTest2();
	vTaskDelay(100);
	#endif
}

/*************    ������: gGetSmallBet  **************************************
�����ܡ�:��ʧ���ٵ�3�����
��������:
�����ء�:
��˵����:
        2013-05-29
		������Ҫ���� ���ݺ����޵�Ѻ�����ʣ�����ʱѺƫ��ʱ�������һ
**************   ���: ���������� 2013-05-28 11:47:57 **********************/
void gGetSmallBet(INT8U *temp)
{
	INT8U i=0,m=0,n=0;
	CheckBetSmall();

	/*!��ʧ����*/
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[7]==(MsgBet.totalbetlose[i]))
		{
			break;
		}
	}
	*temp++=i;

	/*!��ʧ���*/
	m=i;
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[6]==(MsgBet.totalbetlose[i]))
		{
			if(i!=m)
				break;
		}
	}
	*temp++=i;

	/*!��ʧ�ٴ�*/
	n=i;
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[5]==(MsgBet.totalbetlose[i]))
		{
			if((i!=m)&&(i!=n))
				break;
		}
	}
	*temp++=i;
}

/*************    ������: gGetBigBet  **************************************
�����ܡ�:��ʧ�����������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-29 11:10:32 **********************/
void gGetBigBet(INT8U *temp)
{
	INT8U i=0,m=0,n=0;
	CheckBetSmall();

	/*!��ʧ����*/
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[0]==(MsgBet.totalbetlose[i]))
		{
			break;
		}
	}
	*temp++=i;

	/*!��ʧ���*/
	m=i;
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[1]==(MsgBet.totalbetlose[i]))
		{
			if(i!=m)
				break;
		}
	}
	*temp++=i;

	/*!��ʧ�ٴ�*/
	n=i;
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[2]==(MsgBet.totalbetlose[i]))
		{
			if((i!=m)&&(i!=n))
				break;
		}
	}
	*temp++=i;
}

/*************    ������: NoBetGetShark  ************************************
�����ܡ�:û��Ѻ�ֵ�����£�����ĳ���
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-08-10 14:58:55 **********************/
void NoBetGetShark(void)
{
	INT8U bettemp[8]={0};
	
	switch(IRandomX(0,2))
	{
		case 0:
			PC.openshark=3;
			mYGame.open=KINTSHARK;
			mYGame.sharkpeilv=IRandomX(40,126);
			mYGame.boundspeilv=IRandomX(6,36);
			mYGame.second=IRandomX(0,7);	//!�������
			mYGame.third=0;

			/*!����������С��*/
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
			break;

		case 1:
			PC.openshark=1;
			mYGame.open=YINSHAKR;
			mYGame.sharkpeilv=24+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			/*!����������С��*/
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
			break;

		case 2:
			PC.openshark=2;
			mYGame.open=GOLDSHARK;
			mYGame.sharkpeilv=32+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			/*!����������С��*/
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
			break;
	}
}
/*************    ������: gGetHappyPlayer  **********************************
�����ܡ�:������Ѻ�������ѡ���������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-29 11:24:32 **********************/
INT8U gGetHappyPlayer(void)
{
	INT8U i=0,j=0,m=0,n=0;

	//!�ж��м������Ѻ��
	for(i=0;i<MAXSLAVE;i++)
	{
		if(MsgBet.betplayer[i].totalbet>0)
			m++;
	}

	if(m>2)//!��������ڼ�����ʱ�Ų����������
	{
		//!�����������
		do
		{
			i=IRandomX(0,MAXSLAVE-1);
		}while(MsgBet.betplayer[i].totalbet==0);
		n=i;
		
		//!�������˽��
		for(i=0;i<8;i++)
			if(MySlaveYafen.yafen[n][i]>0)
				j++;
		if(j>2)
		{
			do
			{
				i=IRandomX(0,7);
			}while(MySlaveYafen.yafen[n][i]==0);

			return i;
		}
		return 0xff;
	}

	return 0xff;
}

/*************    ������: SharkAlgorithmOne  ********************************
�����ܡ�:ӯ�������ˮ�������Ƿ������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-29 10:59:41 **********************/
void SharkAlgorithmOne(void)
{
	INT8U bettemp[8]={0};
	//!���ȳ�����  ��������
	if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
	{
		PC.openshark=3;
		mYGame.open=KINTSHARK;
		mYGame.sharkpeilv=IRandomX(40,126);
		mYGame.boundspeilv=IRandomX(6,36);
		mYGame.second=IRandomX(0,7);	//!�������
		mYGame.third=0;

		/*!����������С��*/
		if(IRandomX(1,100)>90)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}

		//!�ͳ��������н���
		if(IRandomX(1,100)>80)
		{
			if(IRandomX(1,100)>70)
			{
				mYGame.third=mYGame.second;
				mYGame.second=GOLDSHARK;
				mYGame.secondpeilv=32+8*IRandomX(0,1);
			}
			else
			{
				mYGame.third=mYGame.second;
				mYGame.second=YINSHAKR;
				mYGame.secondpeilv=24+8*IRandomX(0,1);
			}
		}

	}//!����
	else if(MsgBet.totalbet>(MsgBet.totalyinshalose))
	{
		PC.openshark=1;
		mYGame.open=YINSHAKR;
		mYGame.sharkpeilv=24+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!����������С��*/
		if(IRandomX(1,100)>90)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}

		//!�ͳ��������н���
		if(IRandomX(1,100)>80)
		{
			if(IRandomX(1,100)>70)
			{
				mYGame.third=mYGame.second;
				mYGame.second=GOLDSHARK;
				mYGame.secondpeilv=32+8*IRandomX(0,1);
			}
			else
			{
				mYGame.third=mYGame.second;
				mYGame.second=YINSHAKR;
				mYGame.secondpeilv=24+8*IRandomX(0,1);
			}
		}
	}//!����
	else if(MsgBet.totalbet>(MsgBet.totaljinshalose))
	{
		PC.openshark=2;
		mYGame.open=GOLDSHARK;
		mYGame.sharkpeilv=32+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!����������С��*/
		if(IRandomX(1,100)>90)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}

		//!�ͳ��������н���
		if(IRandomX(1,100)>80)
		{
			if(IRandomX(1,100)>70)
			{
				mYGame.third=mYGame.second;
				mYGame.second=GOLDSHARK;
				mYGame.secondpeilv=32+8*IRandomX(0,1);
			}
			else
			{
				mYGame.third=mYGame.second;
				mYGame.second=YINSHAKR;
				mYGame.secondpeilv=24+8*IRandomX(0,1);
			}
		}
	}
	else
	{
		//!��������û�����Ѻ��ҲҪ������
		if(0==MyKernel.realbill.totalbet)
		{
			NoBetGetShark();
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;

			/*!��Ѻ��������ѡ����*/
			if(IRandomX(1,100)>70)
			{
				gGetBigBet(bettemp);
				
				mYGame.open=bettemp[IRandomX(0,2)];
				
				mYGame.second=0;
				mYGame.third=0;
			}
			else//!��������
			{
				if(IRandomX(1,100)>90)//!��һ�����ʳԴ���С
				{
					PC.openshark=0;
					mYGame.sharkpeilv=0;
					mYGame.second=0;
					mYGame.third=0;

					gGetSmallBet(bettemp);
					/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
					if(IRandomX(1,100)>50)
					{
						mYGame.open=bettemp[2-IRandomX(0,2)];
					}
					else
					{
						mYGame.open=bettemp[IRandomX(0,2)];
					}
				}
				else
				{
					PC.openshark=0;
					mYGame.sharkpeilv=0;
					mYGame.second=0;
					mYGame.third=0;
					mYGame.open=IRandomX(0,7);
				}
			}

			//!��������С��͵����
			if(IRandomX(1,100)>=95)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,3);
				}
			}

			//!��������С��͵����
			if(IRandomX(1,100)>97)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,4);
				}
			}

			//!��������С��͵����
			if(IRandomX(1,100)>=99)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,5);
				}
			}
		}
	}
}

/*************    ������: SharkAlgorithmTwo  ********************************
�����ܡ�:ӯ���������ˮ������������ֵΪ�����Ƿ������
��������:
�����ء�:
��˵����:
        ��Ҫ�����ݶ�����
**************   ���: ���������� 2013-05-29 11:14:31 **********************/
void SharkAlgorithmTwo(void)
{
	INT8U bettemp[8]={0};
	
	//!���ȳ�����
	if(MsgBet.totalbet>(MsgBet.totalkingsharklose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!������
		PC.openshark=3;
		mYGame.open=KINTSHARK;
		mYGame.sharkpeilv=IRandomX(40,126);
		mYGame.boundspeilv=IRandomX(6,36);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!����������С��*/
		if(IRandomX(1,100)>80)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
	}
	else if(MsgBet.totalbet>(MsgBet.totalyinshalose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!����
		PC.openshark=1;
		mYGame.open=YINSHAKR;
		mYGame.sharkpeilv=24+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!����������С��*/
		if(IRandomX(1,100)>80)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
	}
	else if(MsgBet.totalbet>(MsgBet.totaljinshalose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!����
		PC.openshark=2;
		mYGame.open=GOLDSHARK;
		mYGame.sharkpeilv=32+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!����������С��*/
		if(IRandomX(1,100)>80)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
	}
	else
	{
		if(0==MyKernel.realbill.totalbet)
		{
			NoBetGetShark();
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			
			//!��������������
			if(IRandomX(1,100)>70)
			{
				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}
			else
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
			}

			//!��������С��͵����
			if(IRandomX(1,100)>=90)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,3);
				}
			}

			//!��������С��͵����
			if(IRandomX(1,100)>=95)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,4);
				}
			}
		}
	}
}

/*************    ������: SharkAlgorithmThr  ********************************
�����ܡ�:ӯ������0���������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-29 11:18:34 **********************/
void SharkAlgorithmThr(void)
{
	INT8U happy=0;
	INT8U bettemp[8]={0};
	
	//!���,�������������� �ƽ�ָ����������
	PC.openshark=0;
	mYGame.sharkpeilv=0;
	mYGame.second=0;
	mYGame.third=0;
	mYGame.open=IRandomX(0,7);

	//!�������ѡ��һ���������
	if(IRandomX(1,100)>80)
	{
		PC.openshark=0;
		mYGame.sharkpeilv=0;
		mYGame.second=0;
		mYGame.third=0;
		
		//!��Ѻ�ֵ���������ѡ��������ң������û��Ѻ�������
		happy=gGetHappyPlayer();
		if(0xff!=happy)
			mYGame.open=happy;
			
	}
	//!�����в����շֿ���,�̼����
	if(IRandomX(1,100)>90)
	{
		PC.openshark=0;
		mYGame.sharkpeilv=0;
		mYGame.second=0;
		mYGame.third=0;

		gGetSmallBet(bettemp);
		/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
		if(IRandomX(1,100)>50)
		{
			mYGame.open=bettemp[2-IRandomX(0,2)];
		}
		else
		{
			mYGame.open=bettemp[IRandomX(0,2)];
		}
	
		mYGame.second=0;
		mYGame.third=0;
	}

	//!��������С��͵����
	if(IRandomX(1,100)>=95)
	{
		if(0==FLAG.eatcnt)
		{
			FLAG.eatcnt=IRandomX(1,3);
		}
	}

	//!��������С��͵����
	if(IRandomX(1,100)>=99)
	{
		if(0==FLAG.eatcnt)
		{
			FLAG.eatcnt=IRandomX(1,4);
		}
	}
}

/*************    ������: SharkAlgorithmFou  *********************************
�����ܡ�:ӯ�������ˮ�㣬�㷨����
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-29 11:37:13 **********************/
void SharkAlgorithmFou(void)
{
	INT8U bettemp[8]={0};
	INT8U temp=0;
	
	//!һ��ĸ����ô��Ѻ�������в���,�˴��������
	if(IRandomX(1,100)>60)
	{
		//!���ȳ�����  ������
		if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
		{
			PC.openshark=3;
			mYGame.open=KINTSHARK;
			mYGame.sharkpeilv=IRandomX(40,126);
			mYGame.boundspeilv=IRandomX(6,36);
			mYGame.second=IRandomX(0,7);	
			mYGame.third=0;

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}//!������
		else if(MsgBet.totalbet>(MsgBet.totalyinshalose))
		{
			PC.openshark=1;
			mYGame.open=YINSHAKR;
			mYGame.sharkpeilv=24+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}//!�����
		else if(MsgBet.totalbet>(MsgBet.totaljinshalose))
		{
			PC.openshark=2;
			mYGame.open=GOLDSHARK;
			mYGame.sharkpeilv=32+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}
		else//!��������������
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;

			/*!��Ѻ��������ѡ����*/
			if(IRandomX(1,100)>70)
			{
				gGetBigBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}
			else
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
			}
		}
	}
	else
	{
		if(IRandomX(1,100)>=60)
		{
			/*!��Ѻ������3������У�ѡ��һ����ҵĵ����3��Ѻ��*/
			MsgBet.randsel=IRandomX(0,2);
			temp=MsgBet.betbigalone[MsgBet.randsel];
			mYGame.open=MsgBet.betplayer[temp].betrank[IRandomX(0,2)];
			
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;
			mYGame.open=IRandomX(0,7);
		}

		//!��������С��͵����
		if(IRandomX(1,100)>=95)
		{
			if(0==FLAG.eatcnt)
			{
				FLAG.eatcnt=IRandomX(1,3);
			}
		}
	}
}

/*************    ������: SharkAlgorithmFiv  ********************************
�����ܡ�:����ӯ������0��С�ڳ�ˮ��
��������:
�����ء�:
��˵����:
		��Ҫ�����ݶ�����
**************   ���: ���������� 2013-05-29 11:39:8 **********************/
void SharkAlgorithmFiv(void)
{
	INT8U bettemp[8]={0};
	INT8U happy=0;
	INT8U temp=0;
	
	//!һ��ĸ����ô��Ѻ�������в���,�˴��ٳ�����
	if(IRandomX(0,100)>=80)
	{
		//!���ȳ�����
		if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
		{
			PC.openshark=3;
			mYGame.open=KINTSHARK;
			mYGame.sharkpeilv=IRandomX(40,126);
			mYGame.boundspeilv=IRandomX(6,36);
			mYGame.second=IRandomX(0,7);	
			mYGame.third=0;

			//!��С����
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}
		else if(MsgBet.totalbet>(MsgBet.totalyinshalose))
		{
			PC.openshark=1;
			mYGame.open=YINSHAKR;
			mYGame.sharkpeilv=24+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!��С����
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}
		else if(MsgBet.totalbet>(MsgBet.totaljinshalose))
		{
			PC.openshark=2;
			mYGame.open=GOLDSHARK;
			mYGame.sharkpeilv=32+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!��С����
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>80)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;

			/*!��Ѻ��������ѡ����*/
			if(IRandomX(1,100)>70)
			{
				gGetBigBet(bettemp);
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}
			else
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
			}
		}
	}
	else
	{
		if(IRandomX(1,100)>80)
		{
			/*!��Ѻ������3������У�ѡ��һ����ҵĵ����3��Ѻ��*/
			MsgBet.randsel=IRandomX(0,2);
			temp=MsgBet.betbigalone[MsgBet.randsel];
			mYGame.open=MsgBet.betplayer[temp].betrank[IRandomX(0,2)];
			
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;

			//!�����в����շֿ���,�̼����
			if(IRandomX(0,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				
				/*!Ϊ�˳������壬�����ʧһ���Ļ��������ѡ����*/
				gGetSmallBet(bettemp);
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;
			mYGame.open=IRandomX(0,7);

			//!�������ѡ��һ���������
			if(IRandomX(1,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
				
				//!��Ѻ�ֵ���������ѡ��������ң������û��Ѻ�������
				happy=gGetHappyPlayer();
				if(0xff!=happy)
					mYGame.open=happy;
			}
			//!�����в����շֿ���,�̼����
			if(IRandomX(1,100)>90)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;

				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}

			//!��������С��͵����
			if(IRandomX(1,100)>=80)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,3);
				}
			}
		}
	}
}

/*************    ������: SharkAlgorithmSix  *********************************
�����ܡ�:ӯ��Ϊ��������£�������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-29 13:23:30 **********************/
void SharkAlgorithmSix(void)
{
	INT8U bettemp[8]={0};
	
	//!���ȳ�����
	if(MsgBet.totalbet>(MsgBet.totalkingsharklose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!������
		PC.openshark=3;
		mYGame.open=KINTSHARK;
		mYGame.sharkpeilv=IRandomX(40,126);
		mYGame.boundspeilv=IRandomX(6,36);
		mYGame.second=IRandomX(0,7);

		if(IRandomX(1,100)>60)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
		mYGame.third=0;
	}
	else if(MsgBet.totalbet>(MsgBet.totalyinshalose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!����
		PC.openshark=1;
		mYGame.open=YINSHAKR;
		mYGame.sharkpeilv=24+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		if(IRandomX(1,100)>50)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
	}
	else if(MsgBet.totalbet>(MsgBet.totaljinshalose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!����
		PC.openshark=2;
		mYGame.open=GOLDSHARK;
		mYGame.sharkpeilv=32+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		if(IRandomX(1,100)>60)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
	}
	else
	{
		if(0==MyKernel.realbill.totalbet)
		{
			NoBetGetShark();
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			
			//!��������������
			if(IRandomX(1,100)>70)
			{
				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}
			else
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
			}

			//!��������С��͵����
			if(IRandomX(1,100)>=80)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,3);
				}
			}
		}
	}
}

/*************    ������: SharkAlgorithmSev  ********************************
�����ܡ�:ӯ��Ϊ��ʱ��������������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-29 13:24:14 **********************/
void SharkAlgorithmSev(void)
{
	INT8U bettemp[8]={0};
	INT8U happy=0;
	INT8U temp=0;
	
	//!һ��ĸ����ô��Ѻ�������в���,�˴��ٳ�����
	if(IRandomX(0,100)>=80)
	{
		//!���ȳ�����
		if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
		{
			PC.openshark=3;
			mYGame.open=KINTSHARK;
			mYGame.sharkpeilv=IRandomX(40,126);
			mYGame.boundspeilv=IRandomX(6,36);
			mYGame.second=IRandomX(0,7);	
			mYGame.third=0;

			//!��С����
			if(IRandomX(1,100)>80)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>90)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}
		else if(MsgBet.totalbet>(MsgBet.totalyinshalose))
		{
			PC.openshark=1;
			mYGame.open=YINSHAKR;
			mYGame.sharkpeilv=24+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!��С����
			if(IRandomX(1,100)>80)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>90)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}
		else if(MsgBet.totalbet>(MsgBet.totaljinshalose))
		{
			PC.openshark=2;
			mYGame.open=GOLDSHARK;
			mYGame.sharkpeilv=32+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!��С����
			if(IRandomX(1,100)>80)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!�ͳ��������н���
			if(IRandomX(1,100)>90)
			{
				if(IRandomX(1,100)>70)
				{
					mYGame.third=mYGame.second;
					mYGame.second=GOLDSHARK;
					mYGame.secondpeilv=32+8*IRandomX(0,1);
				}
				else
				{
					mYGame.third=mYGame.second;
					mYGame.second=YINSHAKR;
					mYGame.secondpeilv=24+8*IRandomX(0,1);
				}
			}
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;

			/*!��Ѻ��������ѡ����*/
			if(IRandomX(1,100)>80)
			{
				gGetBigBet(bettemp);
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}
			else
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
			}
		}
	}
	else
	{
		if(IRandomX(1,100)>90)
		{
			/*!��Ѻ������3������У�ѡ��һ����ҵĵ����3��Ѻ��*/
			MsgBet.randsel=IRandomX(0,2);
			temp=MsgBet.betbigalone[MsgBet.randsel];
			mYGame.open=MsgBet.betplayer[temp].betrank[IRandomX(0,2)];
			
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;

			//!�����в����շֿ���,�̼����
			if(IRandomX(0,100)>70)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				
				/*!Ϊ�˳������壬�����ʧһ���Ļ��������ѡ����*/
				gGetSmallBet(bettemp);
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
		
				mYGame.second=0;
				mYGame.third=0;
			}
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;
			mYGame.open=IRandomX(0,7);

			//!�������ѡ��һ���������
			if(IRandomX(0,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
				
				//!��Ѻ�ֵ���������ѡ��������ң������û��Ѻ�������
				happy=gGetHappyPlayer();
				if(0xff!=happy)
					mYGame.open=happy;
			}
			
			//!�����в����շֿ���,�̼����
			if(IRandomX(0,100)>70)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;

				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}

			//!��������С��͵����
			if(IRandomX(1,100)>80)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,5);
				}
			}
		}
	}
}

/*************    ������: gEatPlayerCoin  **********************************
�����ܡ�:ǿ�ƳԷ�
��������:
�����ء�:
��˵����:
        ��������İѷ����Խ�����ͬʱ��һ����Ұ�Ǯ����ȥ���Ǹ�����
**************   ���: ���������� 2013-05-29 12:46:33 **********************/
void gEatPlayerCoin(void)
{
	INT8U bettemp[8]={0};
	
	//!�մ���ʱ
	if(0)
	{

	}
	else
	{
		//!�ڻ�̨��Ǯ��Խ����ʱ����ֽ׶ν��е���,��װ1W�ֵ���
		//!1W 2W 3W 4W 
		if((FLAG.currentgain>-10000L)&&(FLAG.currentgain<0))//-9999 --- -1
		{
			if(IRandomX(1,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
	
				mYGame.second=0;
				mYGame.third=0;
			}

			//!��������С��͵����
			if(IRandomX(1,100)>85)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,3);
				}
			}
		}

		if((FLAG.currentgain>-20000L)&&(FLAG.currentgain<-10000l))//-19999 --- -10001
		{
			if(IRandomX(1,100)>70)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}

			if(IRandomX(1,100)>80)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,4);
				}
			}
		}

		if((FLAG.currentgain>-30000L)&&(FLAG.currentgain<-20000l))//-29999 --- -20001
		{
			if(IRandomX(1,100)>60)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}

			if(IRandomX(1,100)>70)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,5);
				}
			}
		}
		
		if(FLAG.currentgain<-40000l)
		{
			if(IRandomX(1,100)>50)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				gGetSmallBet(bettemp);
				
				/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
				if(IRandomX(1,100)>50)
				{
					mYGame.open=bettemp[2-IRandomX(0,2)];
				}
				else
				{
					mYGame.open=bettemp[IRandomX(0,2)];
				}
				
				mYGame.second=0;
				mYGame.third=0;
			}

			if(IRandomX(1,100)>60)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,5);
				}
			}
		}

		/*!����û����*/
		if(FLAG.eatcnt)
		{
			FLAG.eatcnt--;

			PC.openshark=0;
			mYGame.sharkpeilv=0;
			gGetSmallBet(bettemp);
			
			/*!������Ҫ�Խ�����и�Ԥ�£���ֹ���ƫ��*/
			if(IRandomX(1,100)>50)
			{
				mYGame.open=bettemp[2-IRandomX(0,2)];
			}
			else
			{
				mYGame.open=bettemp[IRandomX(0,2)];
			}
	
			mYGame.second=0;
			mYGame.third=0;
		}

		/*!�����Ƿ����һ����ù��ң������Է�*/
	}
}

/*************    ������: KingAnt  **************************************
�����ܡ�:�Ϻ���߹���
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-31 14:51:51 **********************/
void KingAnt(void)
{
	if(FLAG.currentgain>0)//!�ֿ�������
	{
		if(FLAG.currentwater>FLAG.currentgain)//!�������
		{
			if(FLAG.samedice)//!��������
			{

			}
			else
			{

			}
		}
		else//!��治��
		{
			if(FLAG.samedice)//!��������
			{

			}
			else
			{

			}
		}
	}
	else//!��������
	{
		if(FLAG.samedice)//!��������
		{

		}
		else
		{

		}
	}
}

/*************    ������: WorkingAnt  **************************************
�����ܡ�:���Ͼ��߹���
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-05-31 15:0:23 **********************/
void WorkingAnt(void)
{

}
/*************    ������: HappyGameSet  ************************************
�����ܡ�:
��������:
�����ء�:
��˵����:
		��Ҫ���ǲ���:
		1����������
		2�����ˮ
		3��ϵͳ���ʽ�   ��Ѻ�ֵ�1%��ȡ,�����Ĭ��Ϊ10000,�ð��ʽ𣬾�����
		                �ͷֵ������Ҳ�������������Ѻ�У��������Ѻ���ˣ���
		                �۳����Ѻ�в��ֵİ��ʽ�
�汾�޸�˵��:
2013-05-27
1����ĿΪ����ʱ�������

2013-05-28
1����ˮ��������
2�����������������20��¼��3������
3����������Ӯ
4��������ҵ�ӮǮ��Χ����1���������Ӯ�������������
5���ڻ�̨�ǿ����ʱ�򣬽�����ٳԷ֣��������ĳ��Ѻ�ֱȽ϶࣬�������Էּ���

6���Ѷ�����
   0 ���� 
   1 ��
   2 ����
   3 ����
   4 ������

2013-05-29
�޸ļ�¼:
1������ͨ���жϵ�����
2��ϵͳ���������Ӫ��Ϊ20000����Ϸ��

2013-05-30
�޸ļ�¼:
29�ŵ��㷨�汾��������һ���ϣ�Ч�����ã����Ѻ�ָо�����������û�ܽ��кܺ�
��Ӫ�գ����쿴�飬żȻ����Ѱ��·�����⣬����żȻ������Ⱥ�㷨����Ϸ���㷨����
����������Ⱥ�㷨������ͨ����Ⱥ�㷨����ģ����ԣ�������ԣ������Ժ����Ϸ��̨
�㷨��ƴ����ô���Ҳ��ʹ�Լ�����Ϸ��������վס�š�
��Ⱥ�㷨����һ��������ͼ��Ѱ���Ż�·���Ļ������㷨��
ǰ����Ⱥ�㷨ʱ�����ģ��������ܱ�֤Ӫ�ն�̬�ĺ�Ѻ�����ӽ��ڳ�ˮ�ʡ�

��֪����:
1��ÿ����ҵ�Ѻ����Ϣ���ܷ���Ϣ
2����ǰ��ʵʱӪ��
3��ϵͳ�趨�ĳ�ˮ��
ʵ��Ŀ��:
��̨Ӫ�պ�Ѻ������̬�����ڳ�ˮ�㣬ͬʱҪ����Ϸ����.

�㷨ģ��:
��̨�൱���Ϻ�16������൱��16ֻ���ϣ�ÿ�����϶�ҪѰ��ʳ���֤�Ϻ󲻱�
������ͬʱҲҪ��֤�Լ���������Ϻ󲻱������������ǣ�ӯ����Ѻ�������ˮ����
���ڲ������ˮ����ʱ���Ϻ��о��Ե�Ȩ������Ϸ��ӣ�������Ҫ�������ϣ�����
������ӡ�
ͬʱ�Ϻ�Ϊ�˹������еĹ���Ŭ���Ĺ�������Ҫ���뼤�����ƣ���֤16ֻ�����У�
��6ֻ�������㹻��ʳ������ϻ�չ���Ϊ�˷�ֹ���ϵ������Ϻ��ͷ�6ֻ���ϡ�
16ֻ����֮�����ɽ��о�����ʹ�Լ���Ϊ��������6ֻ���ϣ�����ɹ����ͷ���6ֻ���ϡ�
�������Ĺ��ϣ������Ϻ��ȡ�������ʳ�����ʳ����������
ͬʱΪ�˷�ֹ���Ծ��������еĹ��϶������������Ӷ���֤���еĹ��ϻ�����ȡ�
ÿһ����Ϸ��ʼ������Ϊ���Ϻ��´�ʳ����������ϣ�������ִ�еĹ����У�16ֻͳ��
���ߣ�����ÿ��ʵ�ʻ�ȡ��ʳ����Ϻ��´�����񲻷��ϣ������٣�ʵ�ֲ���������
�����ʳ����������������

�Ϻ��������ķ���:
�Ϻ��ȡʳ��(Ѻ����)�󣬸��������Ӫ�գ���ÿֻ�����´�ʳ������ͬʱ���ѡ��
3ֻ���ϱ����Ͻ�ȫ��ʳ��
����:
Ϊ�˱�֤���ţ�������Լ��������,ͬʱ������������Ϸ���

���ʵ��:


**************   ���: ���������� 2013-05-20 9:44:22 **********************/
void HappyGameSet(void)
{
	INT32S gnextwater=0;

	taskENTER_CRITICAL(); /*����������*/
	
	//!ͨ����Ӯ�ı�־����������ǿ������
	FLAG.currentgain=MyKernel.realbill.totalbetin-MyKernel.realbill.totalbetout;

	//!�����ӣ�ȷ������
	DicePlayer();

	//!̨��Ѻ������
	GetPlayerBetMsg();

	//!��̨���۳�ˮ
	FLAG.currentwater=(MyKernel.realbill.totalbetin*(FLAG.currentgrade))/100;

	/*!Ϊ�������ˮ�������������ǿ��ת��*/
	gnextwater=(MyKernel.realbill.totalbetin*(FLAG.currentgrade-1))/100;
	if(FLAG.currentgain) //!�㷨̫��������������£���֤��ˮ����һ����ˮ��
	{
		FLAG.currentgain-=gnextwater;
	}
	else
	{
		FLAG.currentgain-=FLAG.currentwater;
	}
	
	/*!ϵͳ�մ���*/
	if(0)
	{

	}
	else
	{
		//!�ͷ�,��������
		if(FLAG.samedice)
		{
			if(FLAG.currentgain>0)//!ʵ��ӯ��Ϊ��
			{
				if(FLAG.currentwater>FLAG.currentgain)//!��ˮ���ڵ�ǰ�趨�Ѷ�
				{
					SharkAlgorithmOne();
				}
				else//!��ˮС�ڵ�ǰ�趨ֵ,��һ�����ʳ�����
				{
					SharkAlgorithmTwo();
				}
			}
			else//!��̨����ʵ��ӯ��Ϊ��ֵ
			{
				SharkAlgorithmSix();
			}
		}
		else//!��� ����
		{
			if(FLAG.currentgain>0)
			{
				if(FLAG.currentwater>FLAG.currentgain)
				{
					if(FLAG.dicedot>=FREEDOMDOT)
					{
						SharkAlgorithmThr();
					}
					else
					{
						SharkAlgorithmFou();
					}	
				}
				else
				{
					if(FLAG.dicedot>=FREEDOMDOT)
					{
						SharkAlgorithmThr();
					}
					else
					{
						SharkAlgorithmFiv();
					}
				}
			}
			else
			{
				SharkAlgorithmSev();
			}
		}
	}

	//!�����и�ǿ�������ģ���ֹ��̨����̫��
	gEatPlayerCoin();

	//!���Խ��
	//SendTestBet();

	//!����ǿ�Ʋ�����Ϸ���
	#if 0
	PC.openshark=1;
	mYGame.open=10;
	mYGame.sharkpeilv=60;
	mYGame.boundspeilv=15;
	mYGame.second=9;
	mYGame.secondpeilv=32;
	mYGame.third=0;
	#endif

	#if 0
		FLAG.gameCnt=8;
		if(FLAG.gameCnt<8)
		{
			PC.openshark=0;
			mYGame.open=FLAG.gameCnt;
			mYGame.sharkpeilv=0;
			mYGame.boundspeilv=0;
			mYGame.second=0;
			mYGame.secondpeilv=0;
			mYGame.third=0;
		}
		else if(FLAG.gameCnt==8)
		{
			PC.openshark=1;
			mYGame.open=FLAG.gameCnt;
			mYGame.sharkpeilv=32;
			mYGame.boundspeilv=0;
			mYGame.second=IRandomX(0,7);
			mYGame.secondpeilv=0;
			mYGame.third=0;
		}
		else if(FLAG.gameCnt==9)
		{
			PC.openshark=1;
			mYGame.open=FLAG.gameCnt;
			mYGame.sharkpeilv=40;
			mYGame.boundspeilv=0;
			mYGame.second=IRandomX(0,7);
			mYGame.secondpeilv=0;
			mYGame.third=0;
		}
		else if(FLAG.gameCnt==10)
		{
			PC.openshark=1;
			mYGame.open=FLAG.gameCnt;
			mYGame.sharkpeilv=88;
			mYGame.boundspeilv=12;
			mYGame.second=IRandomX(0,7);
			mYGame.secondpeilv=0;
			mYGame.third=0;
		}

		FLAG.gameCnt++;

		if(FLAG.gameCnt>=11)
			FLAG.gameCnt=0;
	#endif
	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ��������GetBetCnt                ***************************
�����ܡ�����ȡѺ�ֵ�ǰ��λ
����������
�����ء���
��˵������
          ���㲻��������
**************   ��ƣ����������� 2012-12-02  10:51:46***********************/
void GetBetCnt(INT8U *ptr ,INT8U sla)
{
	INT8U i=0,j=0,m=0,n=0;
	INT32U bettmep=0;
	INT32U temp[8]={0};

	for(i=0;i<8;i++)
		temp[i]=CtrGame.MyPlayer.Player[sla].bet[i];

	/*!ð������*/
	for(i=0;i<8;i++)
	{
		for(j=i+1;j<8;j++)
		{
			if(temp[i]<temp[j])
			{
				bettmep=temp[i];
				temp[i]=temp[j];
				temp[j]=bettmep;
			}
		}
	}

	/*!��һ*/
	for(i=0;i<8;i++)
		if(temp[0]==CtrGame.MyPlayer.Player[sla].bet[i])
			break;
	*ptr++=i;

	/*!�ڶ�*/
	m=i;
	for(i=0;i<8;i++)
		if(temp[1]==CtrGame.MyPlayer.Player[sla].bet[i])
			if(i!=m)
				break;
	*ptr++=i;

	/*!����*/
	n=i;
	for(i=0;i<8;i++)
		if(temp[2]==CtrGame.MyPlayer.Player[sla].bet[i])
			if((i!=m)&&(i!=n))
				break;
	*ptr++=i;
}

/*************    ��������GetPlayerBetSta           **************************
�����ܡ���ͳ����ҵ�Ѻ�������ÿ�����ͳ��10��
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-12-02  12:40:30***********************/
void GetPlayerBetSta(void)
{
	INT8U i=0,j=0;

	for(i=0;i<MAXSLAVE;i++)
	{
		for(j=0;j<13;j++)
		{
			if(CtrGame.MyPlayerBet[i].bet[CtrGame.MyPlayerBet[i].current]
				<=CtrGame.MyPlayer.Player[i].bet[j])	
			{
				CtrGame.MyPlayerBet[i].bet[CtrGame.MyPlayerBet[i].current]
				=CtrGame.MyPlayer.Player[i].bet[j];
			}	
		}

		if(++CtrGame.MyPlayerBet[i].current>=MAXTONGJILIST)
			CtrGame.MyPlayerBet[i].current=0;
	}
}

/*************    ������: GetCoorThirdAnimal  *****************************
�����ܡ�:���н�����ת������Ӧ��������
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-16 11:1:17 **********************/
void GetCoorThirdAnimal(INT8U temp)
{
	switch(temp)
	{
		case 0: //!����
			PC.thirdopen=IRandomX(25,26);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 1: //����
			PC.thirdopen=IRandomX(21,22);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 2: //��è
			PC.thirdopen=IRandomX(18,19);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 3: //ʨ��
			PC.thirdopen=IRandomX(14,15);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 4: //��ӥ
			PC.thirdopen=IRandomX(11,12);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 5: //��ȸ
			PC.thirdopen=IRandomX(7,8);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 6://����
			PC.thirdopen=IRandomX(4,5);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 7: //����
			PC.thirdopen=IRandomX(0,1);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;
	}
}

/*************    ������: GetCoorSecondAnimal  ******************************
�����ܡ�:
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-04-16 11:17:37 **********************/
void GetCoorSecondAnimal(INT8U temp)
{
	switch(temp)
	{
		case 0: //!����
			PC.secondopen=IRandomX(25,26);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 1: //����
			PC.secondopen=IRandomX(21,22);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 2: //��è
			PC.secondopen=IRandomX(18,19);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 3: //ʨ��
			PC.secondopen=IRandomX(14,15);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 4: //��ӥ
			PC.secondopen=IRandomX(11,12);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 5: //��ȸ
			PC.secondopen=IRandomX(7,8);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 6://����
			PC.secondopen=IRandomX(4,5);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 7: //����
			PC.secondopen=IRandomX(0,1);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;
	}
}
/*************    ��������GetGameOpen              ***************************
�����ܡ�����Ϸ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-12-02  13:02:51***********************/
void GetGameOpenPC(void)
{	
	//!������������
	PC.sharkcoor=0;
	PC.sharkpeilv=0;
	//!���ο������������
	PC.secondopen=0;
	PC.secondpeilv=0;
	//!���ο������������
	PC.thirdopen=0;
	PC.thirdpeilv=0;
	//!�ʽ�����
	PC.boundpelv=0;

	PC.sharkcoorback=0;
	PC.secondopenback=0;
	PC.thirdopenback=0;
	
	if(PC.openshark)
	{
		switch(mYGame.open)
		{
			case 8: //����  10   (11   17 ) 18	
			    PC.sharkpeilv=mYGame.sharkpeilv;
				if(PC.sharkpeilv==32)
				{
					PC.sharkcoor=2+8*IRandomX(0,1);
				}
				else
				{
					PC.sharkcoor=3+6*IRandomX(0,1);
				}

				PC.sharkcoorback=PC.sharkcoor;
				
				//!�˴��ж��Ƿ��������
				if(mYGame.second==8)
				{
					if(mYGame.secondpeilv==32)
					{
						do
						{
							PC.secondopen=2+8*IRandomX(0,1);
						}
						while(PC.sharkcoorback==PC.secondopen);
					}
					else
					{
						do
						{
							PC.secondopen=3+6*IRandomX(0,1);
						}
						while(PC.sharkcoorback==PC.secondopen);
					}
					
					PC.secondpeilv=mYGame.secondpeilv;
					GetCoorThirdAnimal(mYGame.third);
				}
				else if(mYGame.second==9)
				{
					if(mYGame.secondpeilv==40)
					{
						PC.secondopen=16+8*IRandomX(0,1);
					}
					else
					{
						PC.secondopen=17+6*IRandomX(0,1);
					}
					PC.secondpeilv=mYGame.secondpeilv;
					GetCoorThirdAnimal(mYGame.third);
				}
				else
				{
					GetCoorSecondAnimal(mYGame.second);
				}
				
				break;

			case 9: //����  3 (4 24 )25 
			    PC.sharkpeilv=mYGame.sharkpeilv;
				if(PC.sharkpeilv==40)
				{
					PC.sharkcoor=16+8*IRandomX(0,1);
				}
				else
				{
					PC.sharkcoor=17+6*IRandomX(0,1);
				}

				PC.sharkcoorback=PC.sharkcoor;

				//!�˴��ж��Ƿ��������
				if(mYGame.second==8)
				{
					if(mYGame.secondpeilv==32)
					{
						PC.secondopen=2+8*IRandomX(0,1);
					}
					else
					{
						PC.secondopen=3+6*IRandomX(0,1);
					}
					PC.secondpeilv=mYGame.secondpeilv;
					GetCoorThirdAnimal(mYGame.third);
				}
				else if(mYGame.second==9)
				{
					if(mYGame.secondpeilv==40)
					{
						do
						{
							PC.secondopen=16+8*IRandomX(0,1);
						}
						while(PC.sharkcoorback==PC.secondopen);
					}
					else
					{
						do
						{
							PC.secondopen=17+6*IRandomX(0,1);
						}
						while(PC.sharkcoorback==PC.secondopen);
					}
					PC.secondpeilv=mYGame.secondpeilv;
					GetCoorThirdAnimal(mYGame.third);
				}
				else
				{
					GetCoorSecondAnimal(mYGame.second);
				}
				break;

			case 10://������ 0 7  14  21 
				PC.sharkpeilv=mYGame.sharkpeilv;
				PC.sharkcoor=6+7*IRandomX(0,3); 
				PC.boundpelv=mYGame.boundspeilv;

				PC.sharkcoorback=PC.sharkcoor;
				
				//!�˴��ж��Ƿ��������
				if(mYGame.second==8)
				{
					if(mYGame.secondpeilv==32)
					{
						PC.secondopen=2+8*IRandomX(0,1);
					}
					else
					{
						PC.secondopen=3+6*IRandomX(0,1);
					}
					PC.secondpeilv=mYGame.secondpeilv;
					GetCoorThirdAnimal(mYGame.third);
				}
				else if(mYGame.second==9)
				{
					if(mYGame.secondpeilv==40)
					{
						PC.secondopen=16+8*IRandomX(0,1);
					}
					else
					{
						PC.secondopen=17+6*IRandomX(0,1);
					}
					PC.secondpeilv=mYGame.secondpeilv;
					GetCoorThirdAnimal(mYGame.third);
				}
				else
				{
					GetCoorSecondAnimal(mYGame.second);
				}
			    break;
		}
	}
	else
	{
		switch(mYGame.open)
		{
			case 0: //!����
				PC.thirdopen=IRandomX(25,26);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 1: //����
				PC.thirdopen=IRandomX(21,22);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 2: //��è
				PC.thirdopen=IRandomX(18,19);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 3: //ʨ��
				PC.thirdopen=IRandomX(14,15);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 4: //��ӥ
				PC.thirdopen=IRandomX(11,12);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 5: //��ȸ
				PC.thirdopen=IRandomX(7,8);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 6://����
				PC.thirdopen=IRandomX(4,5);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 7: //����
				PC.thirdopen=IRandomX(0,1);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;
		}
	}
}

/*************    ������: GetUncodePara  *********************************
�����ܡ�:���˲�������
��������:
�����ء�:
��˵����:
		��Ҫ�ж�У�����Ψһ�Ͳ��ɼ��ƽ�
		��ȫ����ӯ��������ʵ�����󡢻�̨���롢�������������CRC16���㣬
		Ϊ�˰�ȫ���ߺ�Ҳ�������,������ͬ�Ĵ����̲��ɿ���ͨ��
		�ó���CRCУ��ֵ�ͽ�����㹫ʽ����������ͬ�࣬Ȼ���10000ȡģ
		�ò����ܵ����㹫ʽ��Ӱ��
**************   ���: ���������� 2013-05-13 9:0:2 **********************/
INT8U macdata[25]={0};
void GetUncodePara(void)
{
	INT16U crcsend=0;
	INT16U i=0;
	INT8U  *temp=NULL;

	taskENTER_CRITICAL(); /*����������*/
	for(i=0;i<20;i++)
		macdata[i]=0;

	//!ȫ����ӯ��
	temp=(INT8U *)&MyKernel.realbill.totalgain;
	for(i=0;i<4;i++)
		macdata[i]=*(temp+i);

	//!����ʵ������
	temp=(INT8U *)&MyKernel.realbill.totalgaincurrent;
	for(i=0;i<4;i++)
		macdata[4+i]=*(temp+i);

	//!��̨����
	temp=(INT8U *)&MyKernel.serialno;
	for(i=0;i<4;i++)
		macdata[8+i]=*(temp+i);

	//!�������
	temp=(INT8U *)&MyKernel.codecnt;
	for(i=0;i<4;i++)
		macdata[12+i]=*(temp+i);
	
	crcsend=Crc16Check(macdata, 20);

	MyKernel.codecheck=(crcsend*1664525l+MyKernel.codekernel*1664525l+1013904223l+
		MyKernel.vender*1664525l)%10000l;

	/*!���뱨�ˣ�������룬�����ܽ�����Ϸ*/
	if(MyKernel.realbill.totalgain||MyKernel.realbill.totalgaincurrent)
	{
		MyKernel.codeready=1;
		SAVEFM16W08KERNELISR(MyKernel.codeready);
	}

	taskEXIT_CRITICAL(); /*����������*/
}

/*************    ������: AudgeNextCodeDate  *******************************
�����ܡ�:�����´δ���ʱ��
��������:
�����ء�:
��˵����:
**************   ���: ���������� 2013-06-21 0:41:36 **********************/
void AudgeNextCodeDate(void)
{
	//!ͨ���¿�ʼ�жϣ���Ϊ����Ľ��������
	switch(MyRelist.codedate.month)
	{
		//!31��
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if((MyRelist.codedate.day+MyKernel.setuncode.uncodedate)>31)
			{
				MyRelist.codedate.day=(MyRelist.codedate.day+MyKernel.setuncode.uncodedate)-31;
				MyRelist.codedate.month++;
				if(MyRelist.codedate.month>12)
				{
					MyRelist.codedate.month=1;
					MyRelist.codedate.year++;
				}
			}
			else
			{
				MyRelist.codedate.day+=MyKernel.setuncode.uncodedate;
			}
			
			break;

		//!28�����29��
		case 2:
				if(0==(((MyRelist.codedate.year+2000)/4)&&
					(0!=(MyRelist.codedate.year+2000)/100))||
					(0==(MyRelist.codedate.year+2000)/400))//!����
				{
					if((MyRelist.codedate.day+MyKernel.setuncode.uncodedate)>29)
					{
						MyRelist.codedate.day=(MyRelist.codedate.day+MyKernel.setuncode.uncodedate)-29;
						MyRelist.codedate.month++;
						if(MyRelist.codedate.month>12)
						{
							MyRelist.codedate.month=1;
							MyRelist.codedate.year++;
						}
					}
					else
					{
						MyRelist.codedate.day+=MyKernel.setuncode.uncodedate;
					}
				}
				else
				{
					if((MyRelist.codedate.day+MyKernel.setuncode.uncodedate)>28)
					{
						MyRelist.codedate.day=(MyRelist.codedate.day+MyKernel.setuncode.uncodedate)-28;
						MyRelist.codedate.month++;
						if(MyRelist.codedate.month>12)
						{
							MyRelist.codedate.month=1;
							MyRelist.codedate.year++;
						}
					}
					else
					{
						MyRelist.codedate.day+=MyKernel.setuncode.uncodedate;
					}
				}
			break;

		//!30��
		case 4:
		case 6:
		case 9:
		case 11:
			if((MyRelist.codedate.day+MyKernel.setuncode.uncodedate)>30)
			{
				MyRelist.codedate.day=(MyRelist.codedate.day+MyKernel.setuncode.uncodedate)-30;
				MyRelist.codedate.month++;
				if(MyRelist.codedate.month>12)
				{
					MyRelist.codedate.month=1;
					MyRelist.codedate.year++;
				}
			}
			else
			{
				MyRelist.codedate.day+=MyKernel.setuncode.uncodedate;
			}
			
			break;
	}
}

/*************    ������: CheckCodeParaReady  ******************************
�����ܡ�:�жϱ��������Ƿ���ȷ
��������:
�����ء�:
��˵����:
        ���룬��Ҫ������ʱ�䵽������ʱ�����ã�ͬʱ�����ڱ����У��ʵ��ĵ���
        ��̨���ˮ
        ϵͳĬ�ϰ���һԪһ�ҽ����㷨����
        �����ȡһ���ֽڵĲ���
**************   ���: ���������� 2013-05-15 9:10:39 **********************/
INT8U CheckCodeParaReady(INT32U code)
{
	INT16U crcsend=0;
	INT16U i=0;
	INT8U  *temp=NULL;
	INT32U codecnt=0;
	INT32U temp2=0,temp1=0;

	for(i=0;i<20;i++)
		macdata[i]=0;

	//!ȫ����ӯ��
	temp=(INT8U *)&MyKernel.realbill.totalgain;
	for(i=0;i<4;i++)
		macdata[i]=*(temp+i);

	//!����ʵ������
	temp=(INT8U *)&MyKernel.realbill.totalgaincurrent;
	for(i=0;i<4;i++)
		macdata[4+i]=*(temp+i);

	//!��̨����
	temp=(INT8U *)&MyKernel.serialno;
	for(i=0;i<4;i++)
		macdata[8+i]=*(temp+i);

	//!�������
	temp=(INT8U *)&MyKernel.codecnt;
	for(i=0;i<4;i++)
		macdata[12+i]=*(temp+i);
	
	crcsend=Crc16Check(macdata, 20);

	//!ͨ�����������ִ���ͽ���
	temp2=((MyKernel.codekernel&0xffff)<<16)|(MyKernel.codekernel>>16);
	temp1=MyKernel.codekernel*MyKernel.vender+1013904223l;
	codecnt=(crcsend*1664525l+temp2*1664525+1013904223l+
		temp1*1664525l)%100000000l;

	if(codecnt<10000000l)
		codecnt+=10000000l;

	if((codecnt/100)==(code/100))
	{
		MyKernel.realbill.totalreleasemoney=code%100;//!���³��ˮ


		/*!������ȷ��������1*/
		MyKernel.codecnt++;

		//!�������ʵ������Ϊ0�������ȫ����ӯ��
		if(0==MyKernel.realbill.totalgaincurrent)
		{
			MyKernel.realbill.totalgain=0;
			FLAG.typeaccount=TYPEADDSLAVE;
			if(MyKernel.adjpara)
				MyKernel.adjpara=0;
		}
		else
		{
			FLAG.typeaccount=TYPECLEARALLBILL;
			
			if(MyKernel.adjpara)
				MyKernel.adjpara=1;
		}
		
		//!����ʵ����������
		MyKernel.realbill.totalgainpre+=MyKernel.realbill.totalgaincurrent;
		MyKernel.realbill.totalgaincurrent=0;
		//!�����зֻ��ı���ʵ����������
		for(i=0;i<MAXSLAVE;i++)
		{
			MySlaveBill.slavebill[i].totalgaincurrent=0;
		}

		//!�������
		MyKernel.codeready=0;

		//!����ֻ��ı���ʵ������
		#if 0
		ClearSlaveBillCurrent();
		#endif

		//!��ʱ
		MyKernel.realbill.totalruntime=1440*MyKernel.setuncode.uncodedate;

		//!�����´ε�������
		AudgeNextCodeDate();
		
		SAVEFM16W08KERNELISR(MyKernel.realbill);
		SAVEFM16W08KERNELISR(MyKernel.codeready);
		SAVEFM16W08KERNELISR(MyKernel.adjpara);
		SAVEFM16W08KERNELISR(MyKernel.codecnt);
		SAVEFM16W08RELISTISR(MyRelist.codedate);

		for(i=0;i<4;i++)
		{
			SAVEFM16W08SLAVEBILL1ISR(MySlaveBill.slavebill[i]);
			SAVEFM16W08SLAVEBILL2ISR(MySlaveBill.slavebill[4+i]);
			SAVEFM16W08SLAVEBILL3ISR(MySlaveBill.slavebill[8+i]);
			SAVEFM16W08SLAVEBILL4ISR(MySlaveBill.slavebill[12+i]);
		}
		
		return FLAG.typeaccount;
	}
	else
		return 0;
}

/*************    ������: GetSetParaUncode  *******************************
�����ܡ�:�����������
��������:
�����ء�:
��˵����:
		��������û��������ز�����ͨ���������������ͬʱ��Ҫ���ߺţ���̨��
		��أ���ֹ������ͨ��,
		�޸�:
		������Ҫ���������໥��֤����Ҫʹ���ߺš���̨�š�ռ��4Ϊ�������ռ��
		4λ��Ȼ������ߵ�λ
**************   ���: ���������� 2013-05-13 9:1:48 **********************/
void GetSetParaUncode(void)
{
	INT32U crcsend=0;
	INT32U rand=0;
	INT32U rand2=0;

	//!ȫ����ӯ��
	rand=IRandomX(0,0xff);
	rand2=IRandomX(1,4);

	crcsend=(MyKernel.codekernel*1664525l+MyKernel.serialno*1664525l+MyKernel.vender*1664525l
		   +1013904223l)&0xffff;
	
	//!��Ҫ����������,ͬʱ���ܴ���99999999L��������������¼������
	MyKernel.codecheck=((crcsend<<8)|((rand2)<<24)|((rand)&0xff));
	MyKernel.codecheck=MyKernel.codecheck%100000000l;

}

/*************    ������: CheckSetParacode  *********************************
�����ܡ�:�����������
��������:
�����ء�:
��˵����:
		1��Ͷ�ҵ�λ             //Ͷ�ҵ�λ     2���ֽ�
		2����������             //���ز�����С 1���ֽ�
		3����������             //���˵�����   1���ֽ�
		4��ϵͳʱ��             //����ϵͳʱ�� 
		5������ɹ���Ϣ         //������Ϣ     1���ֽ�
		˵��:
		    ������Ϣ����12�����֣�5����Ϣ����ʼ��2�����֣���һλ�������ͣ��ڶ�
		    λΪ�������ݵ�CRCУ������һ��λ
**************   ���: ���������� 2013-05-15 10:25:46 **********************/
INT8U CheckSetParacode(INT32U codeinput1, INT32U codeinput2)
{
	INT32U crc=0;
	INT32U crc1=0;
	INT32U selpara=0;
	INT32U ready=0;
	INT32U  sel=0;

	//!��֤������������ͨ��
	crc=(((MyKernel.codekernel+MyKernel.serialno+MyKernel.vender)*1664525l)
		   +1013904223l);
	crc1=(crc&0x26)<<8;
	if(crc1<0x400)
		crc1+=0x400;

	selpara=codeinput1>>24;
	sel=codeinput2&0xff;

	if(4==selpara)//!����ʱ��
	{
		ready=1;
	}
	else
	{
		if((codeinput1&0xfffff)==(crc&0xfffff))
		{
			if((codeinput2&0x2600)==crc1)
				ready=1;//!��֤ͨ��
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	
	FLAG.uncodetype=0;

	//!�������
	if(ready)
	{
		switch(selpara)
		{
			//!Ͷ�ҵ�λ
			case 1:
				if(sel<10)
					sel=sel+1;
				else if(sel<20)
					sel=(sel-10+1)*10;
				else if(sel<30)
					sel=(sel-20+1)*100;
				else if(sel<40)
					sel=(sel-30+1)*1000;
				
				MyKernel.setuncode.unitcoin=sel;
				FLAG.uncodetype=ADJ_UNITCOIN;
				break;

			//!��������
			case 2:
				MyKernel.setuncode.spcaemode=sel;
				FLAG.uncodetype=ADJ_CHANGDI;
				break;

			//!��������
			case 3:
				MyKernel.setuncode.uncodedate=sel+1;
				FLAG.uncodetype=ADJ_CODEDAY;
				break;

			//!����ʱ��
			case 4:
				MyRelist.codedatecurrent.year=(crc&0xff)^((codeinput1>>16)&0xff);
				MyRelist.codedatecurrent.month=((crc>>8)&0xff)^((codeinput1>>8)&0xff);
				MyRelist.codedatecurrent.day=((crc>>16)&0xff)^((codeinput1)&0xff);

				MyRelist.codedatecurrent.year=
					(MyRelist.codedatecurrent.year>>4)*10+
					(MyRelist.codedatecurrent.year&0x0f);

				MyRelist.codedatecurrent.month=
					(MyRelist.codedatecurrent.month>>4)*10+
					(MyRelist.codedatecurrent.month&0x0f);

				MyRelist.codedatecurrent.day=
					(MyRelist.codedatecurrent.day>>4)*10+
					(MyRelist.codedatecurrent.day&0x0f);

				
				MyRelist.codedatecurrent.hour=(codeinput2-6811)/100;
				MyRelist.codedatecurrent.min=(codeinput2-6811)%100;

				SendPCDateMsg();
				vTaskDelay(50);
				FLAG.uncodetype=ADJ_DATE;
				break;

			//!������Ϣ�Ƿ���ʾ
			case 5:
				MyKernel.setuncode.uncodemsg=sel^1;
				FLAG.uncodetype=ADJ_DIS;
				break;
		}
		SAVEFM16W08RELIST(MyRelist.codedatecurrent);
		SAVEFM16W08KERNEL(MyKernel.setuncode);
	}
	return 1;
}






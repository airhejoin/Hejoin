/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：鲨皇算法
*设计者：南下三五年
*时  间：2012-11-30  14:08:19
*********************************************************************************
*/
#include".\Lib\includes.h"
/*
算法控制流程:
1、系统启动，判断打码时间是否到，时间到，提示挂起任务
2、系统正常，加载后台参数，获取系统的当前盈利，初始化随机数
3、更新PC历史记录，更新读秒
4、读秒结束，收集所有玩家押分，
5、根据玩家的押分情况，判断游戏结果
6、赢分画面

游戏结果:
	1、这里引入暗箱彩金池的概念，只要押分，彩金池就抽水，通过彩金池来控制鲨鱼，暂定
	   彩金池的抽水为总押分的千分之一
	2、出中奖结果的判断条件，由于分值较大，采用换算过的币值进行计算，在此说明
	   A、当前的总盈利
	   B、是否是刚打码2天
	   C、机台的总进分量
	   D、暗箱彩金池
	   E、机台13门总押分汇总、机台单项押分汇总，
	   F、机台总押分前三甲，16个玩家
	   G、玩家有无重新上分(余分为0，其他条件不进行判断)
	   H、每个玩家记录10场记录，动态分析押分规律
	3、判断条件
	   最大营收为进分量的5%，多余的部分放出去，读秒结束汇总玩家的押分信息，算法
	   倾向于对机台贡献多的玩家获利，在台面押分三甲中选择，同时顾忌随机抽取的2
	   个玩家的营收小幅度波动，另外3个玩家，则进行大幅度波动，除非其台面分为零时，
	   系统重新洗牌

*/
S_BET MsgBet;
S_ANT mYAnt;

/*************    函数名：CheckBetSmall             **************************
【功能】：机台8门押分赔分冒泡排序
【参数】：
【返回】：
【说明】：
          降序排列
**************   设计：南下三五年 2012-12-02  10:51:46***********************/
void CheckBetSmall(void)
{
	INT8U i=0,j=0;
	INT32U bettmep=0;

	for(i=0;i<8;i++)
		MsgBet.totalbetloseback[i]=MsgBet.totalbetlose[i];

	/*!冒泡排序*/
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
/*************    函数名：CheckBetBig             **************************
【功能】：台面押分冒泡排序
【参数】：
【返回】：
【说明】：
          降序排列
          修正:
              当最大值有相等的选项时，计算只给出了一个结果，从而给判断带来失误
              现在给出合理的结果，保证每个结果只出现一次，为了更好的判断，算法
              给出和结果在三甲押分的人数，
**************   设计：南下三五年 2012-12-02  10:51:46***********************/
void CheckBetBig(INT8U *ptr )
{
	INT8U i=0,j=0,m=0,n=0;
	INT32U bettmep=0;
	INT32U temp[MAXSLAVE]={0};

	for(i=0;i<MAXSLAVE;i++)
		temp[i]=MsgBet.betplayer[i].totalbet;

	/*!冒泡排序*/
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

	/*!第一*/
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			break;
	*ptr++=i;

	/*!第二*/
	m=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			if(i!=m)
				break;
	*ptr++=i;

	/*!第三*/
	n=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			if((i!=n)&&(i!=m))
				break;
	*ptr++=i;
#if 0
	/*!和状元押分相同的数量*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!和榜眼押分相同的数量*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!和探花押分相同的数量*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;
#endif
}

/*************    函数名：CheckBetAloneBig           *************************
【功能】：玩家总押分三甲
【参数】：
【返回】：
【说明】：
          降序排列
**************   设计：南下三五年 2012-12-02  10:51:46***********************/
void CheckBetAloneBig(INT8U *ptr )
{
	INT8U i=0,j=0,m=0,n=0;
	INT32U bettmep=0;
	INT32U temp[MAXSLAVE]={0};

	for(i=0;i<MAXSLAVE;i++)
		temp[i]=MsgBet.betplayer[i].totalbet;

	/*!冒泡排序*/
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

	/*!第一*/
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			break;
	*ptr++=i;

	/*!第二*/
	m=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			if(i!=m)
				break;
	*ptr++=i;

	/*!第三*/
	n=i;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			if((i!=n)&&(i!=m))
				break;
	*ptr++=i;
#if 0
	/*!和状元押分相同的数量*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[0]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!和榜眼押分相同的数量*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[1]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;

	/*!和探花押分相同的数量*/
	m=0;
	for(i=0;i<MAXSLAVE;i++)
		if(temp[2]==MsgBet.betplayer[i].totalbet)
			m++;
	*ptr++=m;
#endif
}

/***********************函数名:HappyGame   ***********************************
【功能】：游戏核心算法
【参数】：
【返回】：
【说明】：
		  当前账目不对本次押分的结果进行干预，只是影响到波动的幅度
		  1、随机的轮盘通过马特赛特旋转来实现，产生1-100的数据
		  2、1-20之间吃分  21-80之间 随机   81-100之间送分
		  3、通过调整吃分和送分的范围，从而调整系统的难度
		  
          2013-04-25 算法调整
          1、为了算法更科学，调整了以前轮盘的形式来出结果，调整为掷骰子的模型，在
          判断结果的随机上，出3个骰子，根据骰子的概率来调整游戏的结果。
          2、在开游戏结果的时候，需要可靠的收集所有分机的信息，有效评估机台整
          体营收状况
          3、打码后，机台放水最大为上次营收的50%，正常情况下，保证机台在运行一天
          时间内处在放水状态
          4、机台大的范围趋向于让大额度下注的玩家盈利，收取其他小额玩家，实现
          抽水的目的，同时鼓励玩家多押分
          5、每局都会评估幸运玩家
          6、如果检测到某个玩家连续追押一门，则其押中的概率会增加
          7、概率上保证玩家中1个玩家盈利，3个玩家够本，4个玩家输钱，这个后期
          调整为黄金分割比
          8、如果有个玩家重新上分后，会随机抽取其幸运比重
          
          修改:
          2013年5-18号和几个朋友聊天，大家讨论昌盛这种对称赔率设计的机台，存在
          的问题，对称机台导致玩家可以进入一种状态，无论结果出哪一个，玩家输赢
          为0，这样的话，如果多送奖项，则机台必定会输，换句话说，如果采用和昌盛
          一样的赔率，那么后来跟风的厂家，就不可能不昌盛多其他奖项，进行比昌盛
          的机台更有吸引力。在此进行说明.
          同时有个朋友说，他们老板请了昌盛的2个测试工程师，他们老板调整算法，让
          测试工程师进行测试，最后找到了一个更接近昌盛的算法。
          对本算法的启发是，通过不同的版本，进行测试，最终选择一个合适的版本出来。
          2013年5月20日
          通过这段时间对押分机器的思考，算法的随机部分可能更少，更多的需要进行
          干预，针对随机部分，以前采用的是模拟轮盘的方式，导致随机性不是太好，这里
          引入掷骰子的方式，三个骰子，通过判断其结果类型，决定游戏结果，这样由于三次
          随机，效果肯定更好。
          
**************   设计：南下三五年 2012-12-12  14:51:46***********************/
/*************    函数名: DicePlayer  **************************************
【功能】:掷骰子
【参数】:
【返回】:
【说明】:
        3-18点的范围，在此只关心，点数，同时关注3个骰子点数一样的情况，
        111  3点
        222  6点
        333  9点
        444  12点
        555  15点
        666  18点
        当3个骰子点数一样，则出大奖，
        豹子的自然概率
        点数的自然概率

        这里是否应该引入2个骰子相等的情况，用来判断人为干预
**************   设计: 南下三五年 2013-05-20 9:24:18 **********************/
void DicePlayer(void)
{
	//!清空骰子状态
	FLAG.dice[0]=FLAG.dice[1]=FLAG.dice[2]=0;
	FLAG.samedice=0;
	FLAG.dicedot=0;

	//!开始掷骰子
	FLAG.dice[0]=IRandomX(1,6);
	FLAG.dice[1]=IRandomX(1,6);
	FLAG.dice[2]=IRandomX(1,6);

	//!判断豹子的几率
	if((FLAG.dice[0]==FLAG.dice[1])&&(FLAG.dice[0]==FLAG.dice[2]))
		FLAG.samedice=1;

	//!骰子总点数
	FLAG.dicedot=FLAG.dice[0]+FLAG.dice[1]+FLAG.dice[2];
}

/*************    函数名: GetPlayerBetMsg  *********************************
【功能】:玩家押分信息汇总
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-20 10:3:20 **********************/
void GetPlayerBetMsg(void)
{
	INT32U i=0,j=0;

	//!清除上次记录
	//!全台鲨鱼
	MsgBet.shark[0]=0;
	MsgBet.shark[1]=0;
	MsgBet.shark[2]=0;
	//!全台飞禽
	MsgBet.totalbetfowls=0;
	//!全台走兽
	MsgBet.totalbetbeasts=0;
	//!全台各个飞禽
	MsgBet.totalbetfowls4=0;
	//!全台各个走兽
	MsgBet.totalbetbeasts4=0;

	for(j=0;j<8;j++)
		MsgBet.totalbetalone[j]=0;

	/*!玩家押分信息汇总，风险评估*/
	for(i=0;i<MAXSLAVE;i++)
	{
		//!银鲨
		MsgBet.betplayer[i].sharkbet[0]=CtrGame.MyPlayer.Player[i].bet[11];
		//!金鲨
		MsgBet.betplayer[i].sharkbet[1]=CtrGame.MyPlayer.Player[i].bet[10];
		//!大王鲨
		MsgBet.betplayer[i].sharkbet[2]=CtrGame.MyPlayer.Player[i].bet[9];
		//!飞禽
		MsgBet.betplayer[i].betfowls=CtrGame.MyPlayer.Player[i].bet[12];
        //!走兽
        MsgBet.betplayer[i].betbeasts=CtrGame.MyPlayer.Player[i].bet[8];
        //!飞禽单项
        MsgBet.betplayer[i].betfowls4=CtrGame.MyPlayer.Player[i].bet[4]
                                     +CtrGame.MyPlayer.Player[i].bet[5]
                                     +CtrGame.MyPlayer.Player[i].bet[6]
                                     +CtrGame.MyPlayer.Player[i].bet[7];
        //!走兽单项
        MsgBet.betplayer[i].betbeasts4=CtrGame.MyPlayer.Player[i].bet[0]
                                     +CtrGame.MyPlayer.Player[i].bet[1]
                                     +CtrGame.MyPlayer.Player[i].bet[2]
                                     +CtrGame.MyPlayer.Player[i].bet[3];

		//!全部押分
		MsgBet.betplayer[i].totalbet=MsgBet.betplayer[i].sharkbet[0]
		                            +MsgBet.betplayer[i].sharkbet[1]
		                            +MsgBet.betplayer[i].sharkbet[2]
		                            +MsgBet.betplayer[i].betfowls
		                            +MsgBet.betplayer[i].betbeasts
		                            +MsgBet.betplayer[i].betfowls4
		                            +MsgBet.betplayer[i].betbeasts4;
		
		//!单个玩家押分最多的三组
		GetBetCnt(&MsgBet.betplayer[i].betrank[0],i);

		//!全台鲨鱼
		MsgBet.shark[0]+=MsgBet.betplayer[i].sharkbet[0];
		MsgBet.shark[1]+=MsgBet.betplayer[i].sharkbet[1];
		MsgBet.shark[2]+=MsgBet.betplayer[i].sharkbet[2];
		//!全台飞禽
		MsgBet.totalbetfowls+=MsgBet.betplayer[i].betfowls;
		//!全台走兽
		MsgBet.totalbetbeasts+=MsgBet.betplayer[i].betbeasts;
		//!全台各个飞禽
		MsgBet.totalbetfowls4+=MsgBet.betplayer[i].betfowls4;
		//!全台各个走兽
		MsgBet.totalbetbeasts4+=MsgBet.betplayer[i].betbeasts4;

		for(j=0;j<8;j++)
			MsgBet.totalbetalone[j]+=CtrGame.MyPlayer.Player[i].bet[j];

	}

	MsgBet.totalbet=MsgBet.totalbetfowls+MsgBet.totalbetbeasts
		           +MsgBet.totalbetfowls4+MsgBet.totalbetbeasts4
		           +MsgBet.shark[0]+MsgBet.shark[1]+MsgBet.shark[2];

	/*!台面押分三甲*/
	CheckBetBig(MsgBet.betbigmac);
	/*!单项押分三甲*/
	
	/*!个人押分三甲*/
	for(i=0;i<MAXSLAVE;i++)
		CheckBetAloneBig(MsgBet.betbigalone);
	
	/*!风险评估,首先全台,转换为币值进行评估unitcoin*/
	//!评估8门押分
	for(i=0;i<8;i++)
		MsgBet.totalbetlose[i]=MsgBet.totalbetalone[i]*gamepeilv[mYGame.peilv][i];
	
	MsgBet.totalfowlslose=MsgBet.totalbetfowls<<1;
	MsgBet.totalbeastslose=MsgBet.totalbetbeasts<<1;
	
	//!评估银鲨
	MsgBet.totalyinshalose=MsgBet.shark[0]<<5;
	//!评估金鲨
	MsgBet.totaljinshalose=MsgBet.shark[1]*40;
	//!评估大王鲨
	MsgBet.totalkingsharklose=MsgBet.shark[2]*126;

	/*!这里加入庄闲的赔率*/
	for(i=0;i<8;i++)
	{
		if(i<4)//!走兽
			MsgBet.totalbetlose[i]+=MsgBet.totalbeastslose;
		else//!飞禽
			MsgBet.totalbetlose[i]+=MsgBet.totalfowlslose;
	}

	//!将结果发送给PC显示出来，方便判断算法是否合理
	#if 0
	SendTest1();
	vTaskDelay(100);
	SendTest2();
	vTaskDelay(100);
	#endif
}

/*************    函数名: gGetSmallBet  **************************************
【功能】:损失最少的3个玩家
【参数】:
【返回】:
【说明】:
        2013-05-29
		这里需要考虑 飞禽和走兽的押分赔率，否则到时押偏门时，结果单一
**************   设计: 南下三五年 2013-05-28 11:47:57 **********************/
void gGetSmallBet(INT8U *temp)
{
	INT8U i=0,m=0,n=0;
	CheckBetSmall();

	/*!损失最少*/
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[7]==(MsgBet.totalbetlose[i]))
		{
			break;
		}
	}
	*temp++=i;

	/*!损失其次*/
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

	/*!损失再次*/
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

/*************    函数名: gGetBigBet  **************************************
【功能】:损失最多的三个玩家
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-29 11:10:32 **********************/
void gGetBigBet(INT8U *temp)
{
	INT8U i=0,m=0,n=0;
	CheckBetSmall();

	/*!损失最少*/
	for(i=0;i<8;i++)
	{
		if(MsgBet.totalbetloseback[0]==(MsgBet.totalbetlose[i]))
		{
			break;
		}
	}
	*temp++=i;

	/*!损失其次*/
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

	/*!损失再次*/
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

/*************    函数名: NoBetGetShark  ************************************
【功能】:没有押分的情况下，鲨鱼的出法
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-08-10 14:58:55 **********************/
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
			mYGame.second=IRandomX(0,7);	//!这里随机
			mYGame.third=0;

			/*!在这里出结果小的*/
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			/*!在这里出结果小的*/
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			/*!在这里出结果小的*/
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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
/*************    函数名: gGetHappyPlayer  **********************************
【功能】:在所有押分玩家中选择幸运玩家
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-29 11:24:32 **********************/
INT8U gGetHappyPlayer(void)
{
	INT8U i=0,j=0,m=0,n=0;

	//!判断有几个玩家押分
	for(i=0;i<MAXSLAVE;i++)
	{
		if(MsgBet.betplayer[i].totalbet>0)
			m++;
	}

	if(m>2)//!这个给出在几个人时才产生幸运玩家
	{
		//!产生幸运玩家
		do
		{
			i=IRandomX(0,MAXSLAVE-1);
		}while(MsgBet.betplayer[i].totalbet==0);
		n=i;
		
		//!产生幸运结果
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

/*************    函数名: SharkAlgorithmOne  ********************************
【功能】:盈利满足抽水条件，是否出鲨鱼
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-29 10:59:41 **********************/
void SharkAlgorithmOne(void)
{
	INT8U bettemp[8]={0};
	//!优先出鲨鱼  大王鲨鱼
	if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
	{
		PC.openshark=3;
		mYGame.open=KINTSHARK;
		mYGame.sharkpeilv=IRandomX(40,126);
		mYGame.boundspeilv=IRandomX(6,36);
		mYGame.second=IRandomX(0,7);	//!这里随机
		mYGame.third=0;

		/*!在这里出结果小的*/
		if(IRandomX(1,100)>90)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}

		//!送出第三个中奖项
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

	}//!银鲨
	else if(MsgBet.totalbet>(MsgBet.totalyinshalose))
	{
		PC.openshark=1;
		mYGame.open=YINSHAKR;
		mYGame.sharkpeilv=24+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!在这里出结果小的*/
		if(IRandomX(1,100)>90)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}

		//!送出第三个中奖项
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
	}//!金鲨
	else if(MsgBet.totalbet>(MsgBet.totaljinshalose))
	{
		PC.openshark=2;
		mYGame.open=GOLDSHARK;
		mYGame.sharkpeilv=32+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!在这里出结果小的*/
		if(IRandomX(1,100)>90)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}

		//!送出第三个中奖项
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
		//!这里加入对没有玩家押分也要出鲨鱼
		if(0==MyKernel.realbill.totalbet)
		{
			NoBetGetShark();
		}
		else
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;

			/*!在押分三甲总选择结果*/
			if(IRandomX(1,100)>70)
			{
				gGetBigBet(bettemp);
				
				mYGame.open=bettemp[IRandomX(0,2)];
				
				mYGame.second=0;
				mYGame.third=0;
			}
			else//!随机出结果
			{
				if(IRandomX(1,100)>90)//!有一定概率吃大赔小
				{
					PC.openshark=0;
					mYGame.sharkpeilv=0;
					mYGame.second=0;
					mYGame.third=0;

					gGetSmallBet(bettemp);
					/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里引入小鸡偷吃米
			if(IRandomX(1,100)>=95)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,3);
				}
			}

			//!这里引入小鸡偷吃米
			if(IRandomX(1,100)>97)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,4);
				}
			}

			//!这里引入小鸡偷吃米
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

/*************    函数名: SharkAlgorithmTwo  ********************************
【功能】:盈利不满足抽水条件，但是其值为正，是否出鲨鱼
【参数】:
【返回】:
【说明】:
        需要设置梯度条件
**************   设计: 南下三五年 2013-05-29 11:14:31 **********************/
void SharkAlgorithmTwo(void)
{
	INT8U bettemp[8]={0};
	
	//!优先出鲨鱼
	if(MsgBet.totalbet>(MsgBet.totalkingsharklose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!大王鲨
		PC.openshark=3;
		mYGame.open=KINTSHARK;
		mYGame.sharkpeilv=IRandomX(40,126);
		mYGame.boundspeilv=IRandomX(6,36);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!在这里出结果小的*/
		if(IRandomX(1,100)>80)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
	}
	else if(MsgBet.totalbet>(MsgBet.totalyinshalose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!银鲨
		PC.openshark=1;
		mYGame.open=YINSHAKR;
		mYGame.sharkpeilv=24+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!在这里出结果小的*/
		if(IRandomX(1,100)>80)
		{
			gGetSmallBet(bettemp);
			mYGame.second=bettemp[IRandomX(0,2)];
		}
	}
	else if(MsgBet.totalbet>(MsgBet.totaljinshalose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!金鲨
		PC.openshark=2;
		mYGame.open=GOLDSHARK;
		mYGame.sharkpeilv=32+8*IRandomX(0,1);
		mYGame.second=IRandomX(0,7);
		mYGame.third=0;

		/*!在这里出结果小的*/
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
			
			//!不满足鲨鱼条件
			if(IRandomX(1,100)>70)
			{
				gGetSmallBet(bettemp);
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里引入小鸡偷吃米
			if(IRandomX(1,100)>=90)
			{
				if(0==FLAG.eatcnt)
				{
					FLAG.eatcnt=IRandomX(1,3);
				}
			}

			//!这里引入小鸡偷吃米
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

/*************    函数名: SharkAlgorithmThr  ********************************
【功能】:盈利大于0，随机控制
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-29 11:18:34 **********************/
void SharkAlgorithmThr(void)
{
	INT8U happy=0;
	INT8U bettemp[8]={0};
	
	//!随机,不考虑其他参数 黄金分割比自由运行
	PC.openshark=0;
	mYGame.sharkpeilv=0;
	mYGame.second=0;
	mYGame.third=0;
	mYGame.open=IRandomX(0,7);

	//!这里随机选择一个幸运玩家
	if(IRandomX(1,100)>80)
	{
		PC.openshark=0;
		mYGame.sharkpeilv=0;
		mYGame.second=0;
		mYGame.third=0;
		
		//!从押分的玩家中随机选择幸运玩家，如果都没有押分则随机
		happy=gGetHappyPlayer();
		if(0xff!=happy)
			mYGame.open=happy;
			
	}
	//!这里有部分收分控制,刺激玩家
	if(IRandomX(1,100)>90)
	{
		PC.openshark=0;
		mYGame.sharkpeilv=0;
		mYGame.second=0;
		mYGame.third=0;

		gGetSmallBet(bettemp);
		/*!这里需要对结果进行干预下，防止结果偏向*/
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

	//!这里引入小鸡偷吃米
	if(IRandomX(1,100)>=95)
	{
		if(0==FLAG.eatcnt)
		{
			FLAG.eatcnt=IRandomX(1,3);
		}
	}

	//!这里引入小鸡偷吃米
	if(IRandomX(1,100)>=99)
	{
		if(0==FLAG.eatcnt)
		{
			FLAG.eatcnt=IRandomX(1,4);
		}
	}
}

/*************    函数名: SharkAlgorithmFou  *********************************
【功能】:盈利满足抽水点，算法控制
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-29 11:37:13 **********************/
void SharkAlgorithmFou(void)
{
	INT8U bettemp[8]={0};
	INT8U temp=0;
	
	//!一般的概率让大的押分三甲中产生,此处多出鲨鱼
	if(IRandomX(1,100)>60)
	{
		//!优先出鲨鱼  大王鲨
		if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
		{
			PC.openshark=3;
			mYGame.open=KINTSHARK;
			mYGame.sharkpeilv=IRandomX(40,126);
			mYGame.boundspeilv=IRandomX(6,36);
			mYGame.second=IRandomX(0,7);	
			mYGame.third=0;

			//!送出第三个中奖项
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
		}//!大银鲨
		else if(MsgBet.totalbet>(MsgBet.totalyinshalose))
		{
			PC.openshark=1;
			mYGame.open=YINSHAKR;
			mYGame.sharkpeilv=24+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!送出第三个中奖项
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
		}//!大金鲨
		else if(MsgBet.totalbet>(MsgBet.totaljinshalose))
		{
			PC.openshark=2;
			mYGame.open=GOLDSHARK;
			mYGame.sharkpeilv=32+8*IRandomX(0,1);
			mYGame.second=IRandomX(0,7);
			mYGame.third=0;

			//!送出第三个中奖项
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
		else//!不满足鲨鱼条件
		{
			PC.openshark=0;
			mYGame.sharkpeilv=0;

			/*!在押分三甲总选择结果*/
			if(IRandomX(1,100)>70)
			{
				gGetBigBet(bettemp);
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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
			/*!从押分最多的3个玩家中，选择一个玩家的单项的3门押分*/
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

		//!这里引入小鸡偷吃米
		if(IRandomX(1,100)>=95)
		{
			if(0==FLAG.eatcnt)
			{
				FLAG.eatcnt=IRandomX(1,3);
			}
		}
	}
}

/*************    函数名: SharkAlgorithmFiv  ********************************
【功能】:控制盈利大于0，小于抽水点
【参数】:
【返回】:
【说明】:
		需要设置梯度条件
**************   设计: 南下三五年 2013-05-29 11:39:8 **********************/
void SharkAlgorithmFiv(void)
{
	INT8U bettemp[8]={0};
	INT8U happy=0;
	INT8U temp=0;
	
	//!一般的概率让大的押分三甲中产生,此处少出鲨鱼
	if(IRandomX(0,100)>=80)
	{
		//!优先出鲨鱼
		if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
		{
			PC.openshark=3;
			mYGame.open=KINTSHARK;
			mYGame.sharkpeilv=IRandomX(40,126);
			mYGame.boundspeilv=IRandomX(6,36);
			mYGame.second=IRandomX(0,7);	
			mYGame.third=0;

			//!出小赔率
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			//!出小赔率
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			//!出小赔率
			if(IRandomX(1,100)>90)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			/*!在押分三甲总选择结果*/
			if(IRandomX(1,100)>70)
			{
				gGetBigBet(bettemp);
				/*!这里需要对结果进行干预下，防止结果偏向*/
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
			/*!从押分最多的3个玩家中，选择一个玩家的单项的3门押分*/
			MsgBet.randsel=IRandomX(0,2);
			temp=MsgBet.betbigalone[MsgBet.randsel];
			mYGame.open=MsgBet.betplayer[temp].betrank[IRandomX(0,2)];
			
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;

			//!这里有部分收分控制,刺激玩家
			if(IRandomX(0,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				
				/*!为了程序不死板，如果损失一样的话，就随机选择结果*/
				gGetSmallBet(bettemp);
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里随机选择一个幸运玩家
			if(IRandomX(1,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
				
				//!从押分的玩家中随机选择幸运玩家，如果都没有押分则随机
				happy=gGetHappyPlayer();
				if(0xff!=happy)
					mYGame.open=happy;
			}
			//!这里有部分收分控制,刺激玩家
			if(IRandomX(1,100)>90)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;

				gGetSmallBet(bettemp);
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里引入小鸡偷吃米
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

/*************    函数名: SharkAlgorithmSix  *********************************
【功能】:盈利为负的情况下，出鲨鱼
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-29 13:23:30 **********************/
void SharkAlgorithmSix(void)
{
	INT8U bettemp[8]={0};
	
	//!优先出鲨鱼
	if(MsgBet.totalbet>(MsgBet.totalkingsharklose+
		MyKernel.setpara.othermaxbet*gamepeilv[mYGame.peilv][3]))
	{//!大王鲨
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
	{//!银鲨
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
	{//!金鲨
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
			
			//!不满足鲨鱼条件
			if(IRandomX(1,100)>70)
			{
				gGetSmallBet(bettemp);
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里引入小鸡偷吃米
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

/*************    函数名: SharkAlgorithmSev  ********************************
【功能】:盈利为负时，不出鲨鱼的情况
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-29 13:24:14 **********************/
void SharkAlgorithmSev(void)
{
	INT8U bettemp[8]={0};
	INT8U happy=0;
	INT8U temp=0;
	
	//!一般的概率让大的押分三甲中产生,此处少出鲨鱼
	if(IRandomX(0,100)>=80)
	{
		//!优先出鲨鱼
		if(MsgBet.totalbet>(MsgBet.totalkingsharklose))
		{
			PC.openshark=3;
			mYGame.open=KINTSHARK;
			mYGame.sharkpeilv=IRandomX(40,126);
			mYGame.boundspeilv=IRandomX(6,36);
			mYGame.second=IRandomX(0,7);	
			mYGame.third=0;

			//!出小赔率
			if(IRandomX(1,100)>80)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			//!出小赔率
			if(IRandomX(1,100)>80)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			//!出小赔率
			if(IRandomX(1,100)>80)
			{
				gGetSmallBet(bettemp);
				mYGame.second=bettemp[IRandomX(0,2)];
			}

			//!送出第三个中奖项
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

			/*!在押分三甲总选择结果*/
			if(IRandomX(1,100)>80)
			{
				gGetBigBet(bettemp);
				/*!这里需要对结果进行干预下，防止结果偏向*/
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
			/*!从押分最多的3个玩家中，选择一个玩家的单项的3门押分*/
			MsgBet.randsel=IRandomX(0,2);
			temp=MsgBet.betbigalone[MsgBet.randsel];
			mYGame.open=MsgBet.betplayer[temp].betrank[IRandomX(0,2)];
			
			PC.openshark=0;
			mYGame.sharkpeilv=0;
			mYGame.second=0;
			mYGame.third=0;

			//!这里有部分收分控制,刺激玩家
			if(IRandomX(0,100)>70)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				
				/*!为了程序不死板，如果损失一样的话，就随机选择结果*/
				gGetSmallBet(bettemp);
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里随机选择一个幸运玩家
			if(IRandomX(0,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;
				mYGame.open=IRandomX(0,7);
				
				//!从押分的玩家中随机选择幸运玩家，如果都没有押分则随机
				happy=gGetHappyPlayer();
				if(0xff!=happy)
					mYGame.open=happy;
			}
			
			//!这里有部分收分控制,刺激玩家
			if(IRandomX(0,100)>70)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				mYGame.second=0;
				mYGame.third=0;

				gGetSmallBet(bettemp);
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里引入小鸡偷吃米
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

/*************    函数名: gEatPlayerCoin  **********************************
【功能】:强制吃分
【参数】:
【返回】:
【说明】:
        如何慢慢的把分数吃进来，同时让一个玩家把钱输下去。是个难题
**************   设计: 南下三五年 2013-05-29 12:46:33 **********************/
void gEatPlayerCoin(void)
{
	INT8U bettemp[8]={0};
	
	//!刚打码时
	if(0)
	{

	}
	else
	{
		//!在机台输钱跨越红线时，则分阶段进行调整,安装1W分档，
		//!1W 2W 3W 4W 
		if((FLAG.currentgain>-10000L)&&(FLAG.currentgain<0))//-9999 --- -1
		{
			if(IRandomX(1,100)>80)
			{
				PC.openshark=0;
				mYGame.sharkpeilv=0;
				gGetSmallBet(bettemp);
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

			//!这里引入小鸡偷吃米
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
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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
				
				/*!这里需要对结果进行干预下，防止结果偏向*/
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

		/*!吃你没商量*/
		if(FLAG.eatcnt)
		{
			FLAG.eatcnt--;

			PC.openshark=0;
			mYGame.sharkpeilv=0;
			gGetSmallBet(bettemp);
			
			/*!这里需要对结果进行干预下，防止结果偏向*/
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

		/*!这里是否产生一个倒霉玩家，连续吃分*/
	}
}

/*************    函数名: KingAnt  **************************************
【功能】:蚁后决策过程
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-31 14:51:51 **********************/
void KingAnt(void)
{
	if(FLAG.currentgain>0)//!仓库有余粮
	{
		if(FLAG.currentwater>FLAG.currentgain)//!库存正常
		{
			if(FLAG.samedice)//!激励工蚁
			{

			}
			else
			{

			}
		}
		else//!库存不足
		{
			if(FLAG.samedice)//!激励工蚁
			{

			}
			else
			{

			}
		}
	}
	else//!余粮亏空
	{
		if(FLAG.samedice)//!激励工蚁
		{

		}
		else
		{

		}
	}
}

/*************    函数名: WorkingAnt  **************************************
【功能】:工蚁决策过程
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-05-31 15:0:23 **********************/
void WorkingAnt(void)
{

}
/*************    函数名: HappyGameSet  ************************************
【功能】:
【参数】:
【返回】:
【说明】:
		需要考虑参数:
		1、场地类型
		2、抽放水
		3、系统暗彩金   总押分的1%抽取,打码后，默认为10000,该暗彩金，决定了
		                送分的情况，也就是主动让玩家押中，若果玩家押中了，则
		                扣除玩家押中部分的暗彩金
版本修改说明:
2013-05-27
1、账目为负数时计算错误

2013-05-28
1、放水调整缓和
2、增加鲨鱼的数量，20记录在3个左右
3、随机让玩家赢
4、控制玩家的赢钱范围，让1个玩家连续赢，限制其他玩家
5、在机台是亏损的时候，进入快速吃分，如果发现某次押分比较多，在连续吃分几次

6、难度设置
   0 容易 
   1 难
   2 最难
   3 死难
   4 最容易

2013-05-29
修改记录:
1、调整通盘判断的能力
2、系统启动后，最大营收为20000个游戏币

2013-05-30
修改记录:
29号的算法版本，测试了一晚上，效果不好，玩家押分感觉还不错，不过没能进行很好
的营收，昨天看书，偶然看到寻找路径问题，今天偶然想起蚁群算法，游戏的算法或许
可以引入蚁群算法，打算通过蚁群算法进行模拟测试，如果可以，将对以后的游戏后台
算法设计带来好处，也能使自己的游戏在市面上站住脚。
蚁群算法，是一种用来在图中寻找优化路径的机率型算法。
前提蚁群算法时收敛的，这样才能保证营收动态的和押分量接近于抽水率。

已知条件:
1、每个玩家的押分信息和总分信息
2、当前的实时营收
3、系统设定的抽水率
实现目标:
机台营收和押分量动态保持在抽水点，同时要让游戏好玩.

算法模型:
机台相当于蚁后，16个玩家相当于16只工蚁，每个工蚁都要寻找食物，保证蚁后不被
饿死，同时也要保证自己可以生活，蚁后不被饿死的条件是，盈利和押分满足抽水条件
，在不满足抽水条件时，蚁后有绝对的权利，令工蚁服从，甚至需要牺牲工蚁，工蚁
必须服从。
同时蚁后为了鼓励所有的工蚁努力的工作，需要引入激励机制，保证16只工蚁中，
有6只工蚁有足够的食物，否则工蚁会罢工。为了防止工蚁怠工，蚁后会惩罚6只工蚁。
16只工蚁之间自由进行竞争，使自己成为被激励的6只工蚁，避免成功被惩罚的6只工蚁。
被饿死的工蚁，会在蚁后获取到充足的食物后，用食物获得重生。
同时为了防止恶性竞争，所有的工蚁都会主动犯错，从而保证所有的工蚁机会均等。
每一局游戏开始，抽象为，蚁后下达食物任务给工蚁，工蚁在执行的过程中，16只统筹
决策，导致每次实际获取的食物和蚁后下达的任务不符合，或多或少，实现波动，不过
整体的食物量满足任务需求。

蚁后分派任务的方法:
蚁后获取食物(押分量)后，根据自身的营收，给每只工蚁下达食物任务，同时随机选择
3只工蚁必须上交全部食物
工蚁:
为了保证活着，必须给自己留有余地,同时集体表决给出游戏结果

软件实现:


**************   设计: 南下三五年 2013-05-20 9:44:22 **********************/
void HappyGameSet(void)
{
	INT32S gnextwater=0;

	taskENTER_CRITICAL(); /*调度器上锁*/
	
	//!通盘输赢的标志，用来控制强制收敛
	FLAG.currentgain=MyKernel.realbill.totalbetin-MyKernel.realbill.totalbetout;

	//!掷骰子，确定概率
	DicePlayer();

	//!台面押分评估
	GetPlayerBetMsg();

	//!机台理论抽水
	FLAG.currentwater=(MyKernel.realbill.totalbetin*(FLAG.currentgrade))/100;

	/*!为了满足抽水条件，这里进行强制转换*/
	gnextwater=(MyKernel.realbill.totalbetin*(FLAG.currentgrade-1))/100;
	if(FLAG.currentgain) //!算法太随机，在最坏的情况下，保证抽水在下一个抽水点
	{
		FLAG.currentgain-=gnextwater;
	}
	else
	{
		FLAG.currentgain-=FLAG.currentwater;
	}
	
	/*!系统刚打码*/
	if(0)
	{

	}
	else
	{
		//!送分,鲨鱼优先
		if(FLAG.samedice)
		{
			if(FLAG.currentgain>0)//!实际盈利为正
			{
				if(FLAG.currentwater>FLAG.currentgain)//!抽水大于当前设定难度
				{
					SharkAlgorithmOne();
				}
				else//!抽水小于当前设定值,有一定概率出鲨鱼
				{
					SharkAlgorithmTwo();
				}
			}
			else//!机台亏损，实际盈利为负值
			{
				SharkAlgorithmSix();
			}
		}
		else//!随机 控制
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

	//!这里有个强制收敛的，防止机台亏损太多
	gEatPlayerCoin();

	//!测试结果
	//SendTestBet();

	//!这里强制测试游戏结果
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
	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名：GetBetCnt                ***************************
【功能】：获取押分的前三位
【参数】：
【返回】：
【说明】：
          鲨鱼不计入评比
**************   设计：南下三五年 2012-12-02  10:51:46***********************/
void GetBetCnt(INT8U *ptr ,INT8U sla)
{
	INT8U i=0,j=0,m=0,n=0;
	INT32U bettmep=0;
	INT32U temp[8]={0};

	for(i=0;i<8;i++)
		temp[i]=CtrGame.MyPlayer.Player[sla].bet[i];

	/*!冒泡排序*/
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

	/*!第一*/
	for(i=0;i<8;i++)
		if(temp[0]==CtrGame.MyPlayer.Player[sla].bet[i])
			break;
	*ptr++=i;

	/*!第二*/
	m=i;
	for(i=0;i<8;i++)
		if(temp[1]==CtrGame.MyPlayer.Player[sla].bet[i])
			if(i!=m)
				break;
	*ptr++=i;

	/*!第三*/
	n=i;
	for(i=0;i<8;i++)
		if(temp[2]==CtrGame.MyPlayer.Player[sla].bet[i])
			if((i!=m)&&(i!=n))
				break;
	*ptr++=i;
}

/*************    函数名：GetPlayerBetSta           **************************
【功能】：统计玩家的押分情况，每个玩家统计10场
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-12-02  12:40:30***********************/
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

/*************    函数名: GetCoorThirdAnimal  *****************************
【功能】:将中奖动物转换到对应的坐标上
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-16 11:1:17 **********************/
void GetCoorThirdAnimal(INT8U temp)
{
	switch(temp)
	{
		case 0: //!兔子
			PC.thirdopen=IRandomX(25,26);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 1: //猴子
			PC.thirdopen=IRandomX(21,22);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 2: //熊猫
			PC.thirdopen=IRandomX(18,19);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 3: //狮子
			PC.thirdopen=IRandomX(14,15);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 4: //老鹰
			PC.thirdopen=IRandomX(11,12);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 5: //孔雀
			PC.thirdopen=IRandomX(7,8);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 6://鸽子
			PC.thirdopen=IRandomX(4,5);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 7: //燕子
			PC.thirdopen=IRandomX(0,1);
			PC.thirdpeilv=gamepeilv[mYGame.peilv][temp];
			break;
	}
}

/*************    函数名: GetCoorSecondAnimal  ******************************
【功能】:
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-04-16 11:17:37 **********************/
void GetCoorSecondAnimal(INT8U temp)
{
	switch(temp)
	{
		case 0: //!兔子
			PC.secondopen=IRandomX(25,26);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 1: //猴子
			PC.secondopen=IRandomX(21,22);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 2: //熊猫
			PC.secondopen=IRandomX(18,19);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 3: //狮子
			PC.secondopen=IRandomX(14,15);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 4: //老鹰
			PC.secondopen=IRandomX(11,12);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 5: //孔雀
			PC.secondopen=IRandomX(7,8);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 6://鸽子
			PC.secondopen=IRandomX(4,5);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;

		case 7: //燕子
			PC.secondopen=IRandomX(0,1);
			PC.secondpeilv=gamepeilv[mYGame.peilv][temp];
			break;
	}
}
/*************    函数名：GetGameOpen              ***************************
【功能】：游戏结果
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-12-02  13:02:51***********************/
void GetGameOpenPC(void)
{	
	//!鲨鱼坐标赔率
	PC.sharkcoor=0;
	PC.sharkpeilv=0;
	//!二次开奖坐标和赔率
	PC.secondopen=0;
	PC.secondpeilv=0;
	//!三次开奖坐标和赔率
	PC.thirdopen=0;
	PC.thirdpeilv=0;
	//!彩金赔率
	PC.boundpelv=0;

	PC.sharkcoorback=0;
	PC.secondopenback=0;
	PC.thirdopenback=0;
	
	if(PC.openshark)
	{
		switch(mYGame.open)
		{
			case 8: //银鲨  10   (11   17 ) 18	
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
				
				//!此处判断是否开三个结果
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

			case 9: //金鲨  3 (4 24 )25 
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

				//!此处判断是否开三个结果
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

			case 10://大王鲨 0 7  14  21 
				PC.sharkpeilv=mYGame.sharkpeilv;
				PC.sharkcoor=6+7*IRandomX(0,3); 
				PC.boundpelv=mYGame.boundspeilv;

				PC.sharkcoorback=PC.sharkcoor;
				
				//!此处判断是否开三个结果
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
			case 0: //!兔子
				PC.thirdopen=IRandomX(25,26);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 1: //猴子
				PC.thirdopen=IRandomX(21,22);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 2: //熊猫
				PC.thirdopen=IRandomX(18,19);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 3: //狮子
				PC.thirdopen=IRandomX(14,15);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 4: //老鹰
				PC.thirdopen=IRandomX(11,12);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 5: //孔雀
				PC.thirdopen=IRandomX(7,8);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 6://鸽子
				PC.thirdopen=IRandomX(4,5);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;

			case 7: //燕子
				PC.thirdopen=IRandomX(0,1);
				PC.thirdpeilv=gamepeilv[mYGame.peilv][mYGame.open];
				break;
		}
	}
}

/*************    函数名: GetUncodePara  *********************************
【功能】:报账产生条码
【参数】:
【返回】:
【说明】:
		主要判断校验码的唯一和不可简单破解
		将全部总盈利、本次实际利润、机台号码、报码次数、进行CRC16计算，
		为了安全，线号也加入打码,这样不同的代理商不可可以通打
		得出的CRC校验值和解码计算公式，进行线性同余，然后对10000取模
		该参数受到计算公式的影响
**************   设计: 南下三五年 2013-05-13 9:0:2 **********************/
INT8U macdata[25]={0};
void GetUncodePara(void)
{
	INT16U crcsend=0;
	INT16U i=0;
	INT8U  *temp=NULL;

	taskENTER_CRITICAL(); /*调度器上锁*/
	for(i=0;i<20;i++)
		macdata[i]=0;

	//!全部总盈利
	temp=(INT8U *)&MyKernel.realbill.totalgain;
	for(i=0;i<4;i++)
		macdata[i]=*(temp+i);

	//!本次实际利润
	temp=(INT8U *)&MyKernel.realbill.totalgaincurrent;
	for(i=0;i<4;i++)
		macdata[4+i]=*(temp+i);

	//!机台号码
	temp=(INT8U *)&MyKernel.serialno;
	for(i=0;i<4;i++)
		macdata[8+i]=*(temp+i);

	//!报码次数
	temp=(INT8U *)&MyKernel.codecnt;
	for(i=0;i<4;i++)
		macdata[12+i]=*(temp+i);
	
	crcsend=Crc16Check(macdata, 20);

	MyKernel.codecheck=(crcsend*1664525l+MyKernel.codekernel*1664525l+1013904223l+
		MyKernel.vender*1664525l)%10000l;

	/*!进入报账，必须打码，否则不能进行游戏*/
	if(MyKernel.realbill.totalgain||MyKernel.realbill.totalgaincurrent)
	{
		MyKernel.codeready=1;
		SAVEFM16W08KERNELISR(MyKernel.codeready);
	}

	taskEXIT_CRITICAL(); /*调度器开锁*/
}

/*************    函数名: AudgeNextCodeDate  *******************************
【功能】:计算下次打码时间
【参数】:
【返回】:
【说明】:
**************   设计: 南下三五年 2013-06-21 0:41:36 **********************/
void AudgeNextCodeDate(void)
{
	//!通过月开始判断，因为打码的结果是天数
	switch(MyRelist.codedate.month)
	{
		//!31天
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

		//!28天或者29天
		case 2:
				if(0==(((MyRelist.codedate.year+2000)/4)&&
					(0!=(MyRelist.codedate.year+2000)/100))||
					(0==(MyRelist.codedate.year+2000)/400))//!闰年
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

		//!30天
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

/*************    函数名: CheckCodeParaReady  ******************************
【功能】:判断报码输入是否正确
【参数】:
【返回】:
【说明】:
        报码，主要是运行时间到，起到延时的作用，同时可以在报码中，适当的调整
        机台抽放水
        系统默认按照一元一币进行算法调整
        解码获取一个字节的参数
**************   设计: 南下三五年 2013-05-15 9:10:39 **********************/
INT8U CheckCodeParaReady(INT32U code)
{
	INT16U crcsend=0;
	INT16U i=0;
	INT8U  *temp=NULL;
	INT32U codecnt=0;
	INT32U temp2=0,temp1=0;

	for(i=0;i<20;i++)
		macdata[i]=0;

	//!全部总盈利
	temp=(INT8U *)&MyKernel.realbill.totalgain;
	for(i=0;i<4;i++)
		macdata[i]=*(temp+i);

	//!本次实际利润
	temp=(INT8U *)&MyKernel.realbill.totalgaincurrent;
	for(i=0;i<4;i++)
		macdata[4+i]=*(temp+i);

	//!机台号码
	temp=(INT8U *)&MyKernel.serialno;
	for(i=0;i<4;i++)
		macdata[8+i]=*(temp+i);

	//!报码次数
	temp=(INT8U *)&MyKernel.codecnt;
	for(i=0;i<4;i++)
		macdata[12+i]=*(temp+i);
	
	crcsend=Crc16Check(macdata, 20);

	//!通过这里来区分打码和解码
	temp2=((MyKernel.codekernel&0xffff)<<16)|(MyKernel.codekernel>>16);
	temp1=MyKernel.codekernel*MyKernel.vender+1013904223l;
	codecnt=(crcsend*1664525l+temp2*1664525+1013904223l+
		temp1*1664525l)%100000000l;

	if(codecnt<10000000l)
		codecnt+=10000000l;

	if((codecnt/100)==(code/100))
	{
		MyKernel.realbill.totalreleasemoney=code%100;//!更新抽放水


		/*!报账正确，次数加1*/
		MyKernel.codecnt++;

		//!如果本次实际林润为0，则清除全部总盈利
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
		
		//!本次实际利润清零
		MyKernel.realbill.totalgainpre+=MyKernel.realbill.totalgaincurrent;
		MyKernel.realbill.totalgaincurrent=0;
		//!将所有分机的本次实际利润清零
		for(i=0;i<MAXSLAVE;i++)
		{
			MySlaveBill.slavebill[i].totalgaincurrent=0;
		}

		//!清除报账
		MyKernel.codeready=0;

		//!清除分机的本次实际利润
		#if 0
		ClearSlaveBillCurrent();
		#endif

		//!延时
		MyKernel.realbill.totalruntime=1440*MyKernel.setuncode.uncodedate;

		//!计算下次到期日期
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

/*************    函数名: GetSetParaUncode  *******************************
【功能】:解码调整参数
【参数】:
【返回】:
【说明】:
		解码由于没有其他相关参数，通过随机数来产生，同时需要和线号，机台号
		相关，防止打码器通打,
		修改:
		由于需要打码器，相互验证，需要使用线号、机台号、占用4为，随机数占用
		4位，然后调整高低位
**************   设计: 南下三五年 2013-05-13 9:1:48 **********************/
void GetSetParaUncode(void)
{
	INT32U crcsend=0;
	INT32U rand=0;
	INT32U rand2=0;

	//!全部总盈利
	rand=IRandomX(0,0xff);
	rand2=IRandomX(1,4);

	crcsend=(MyKernel.codekernel*1664525l+MyKernel.serialno*1664525l+MyKernel.vender*1664525l
		   +1013904223l)&0xffff;
	
	//!需要看不出规律,同时不能大于99999999L，否则溢出，导致计算错误
	MyKernel.codecheck=((crcsend<<8)|((rand2)<<24)|((rand)&0xff));
	MyKernel.codecheck=MyKernel.codecheck%100000000l;

}

/*************    函数名: CheckSetParacode  *********************************
【功能】:解码参数调整
【参数】:
【返回】:
【说明】:
		1、投币单位             //投币单位     2个字节
		2、场地类型             //场地波动大小 1个字节
		3、打码天数             //报账的天数   1个字节
		4、系统时间             //调整系统时间 
		5、打码成功信息         //打码信息     1个字节
		说明:
		    解码信息，共12个数字，5种信息，开始的2个数字，第一位调整类型，第二
		    位为后面数据的CRC校验的最后一个位
**************   设计: 南下三五年 2013-05-15 10:25:46 **********************/
INT8U CheckSetParacode(INT32U codeinput1, INT32U codeinput2)
{
	INT32U crc=0;
	INT32U crc1=0;
	INT32U selpara=0;
	INT32U ready=0;
	INT32U  sel=0;

	//!保证打码器不可以通打
	crc=(((MyKernel.codekernel+MyKernel.serialno+MyKernel.vender)*1664525l)
		   +1013904223l);
	crc1=(crc&0x26)<<8;
	if(crc1<0x400)
		crc1+=0x400;

	selpara=codeinput1>>24;
	sel=codeinput2&0xff;

	if(4==selpara)//!调整时间
	{
		ready=1;
	}
	else
	{
		if((codeinput1&0xfffff)==(crc&0xfffff))
		{
			if((codeinput2&0x2600)==crc1)
				ready=1;//!认证通过
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	
	FLAG.uncodetype=0;

	//!两次异或
	if(ready)
	{
		switch(selpara)
		{
			//!投币单位
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

			//!场地类型
			case 2:
				MyKernel.setuncode.spcaemode=sel;
				FLAG.uncodetype=ADJ_CHANGDI;
				break;

			//!打码天数
			case 3:
				MyKernel.setuncode.uncodedate=sel+1;
				FLAG.uncodetype=ADJ_CODEDAY;
				break;

			//!调整时间
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

			//!打码信息是否显示
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






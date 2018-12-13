/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：数码管显示
*设计者：南下三五年
*时  间：2012-05-24  00:53:19
*********************************************************************************
*/
#include".\Lib\includes.h"
#if 0
DISBUFF MyDisbuff;
S_FLAG FLAG;
/*************    函数名：InitAllTM1629           ****************************
【功能】：初始化所有TM1629
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-12  13:07:16***********************/
void InitAllTM1629(void)
{
	INT32U i=0;

	INT16U off=0;

	InitTM1629DIO();
	
	SET_DATASEL(off);
	for(i=1;i<8;i++)
	{
		SET_DATASEL(i);
		SendByteTM1629D(WRITEBUFFER);
		SET_DATASEL(off);
		SET_DATASEL(i);
		SendByteTM1629D(SETPLUSE4);
		SET_DATASEL(off);
		SET_DATASEL(i);
		SendByteTM1629D(DISON );
		SET_DATASEL(off);
	}
}

/*************    函数名：LockAddrDisTM1629D       ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-26  00:59:18***********************/
void LockAddrDisTM1629D(INT8U sel,INT8U addr, INT8U data)
{
	INT16U selcs=sel;
	INT16U off=0;
	TM1629DCLKH;
	TM1629DOUTH;
	SET_DATASEL(selcs);
	SendByteTM1629D(WRITEBUFFER);
	SET_DATASEL(off);
	SET_DATASEL(selcs);
	SendByteTM1629D(SETADDR|addr);
	SendByteTM1629D(data);
	SET_DATASEL(off);
}
/*************    函数名：MuxAddrDisTM1629D        ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-05-26  20:45:30***********************/
void MuxAddrDisTM1629D(INT8U sel,INT8U addr, INT8U length, INT8U *data)
{
	INT16U selcs=sel;
	INT16U off=0;
	
	TM1629DOUTH;
	TM1629DCLKH;
	
	SET_DATASEL(selcs);
	SendByteTM1629D(AUTOADDADDR);//设置地址自动加1模式
	SET_DATASEL(off);
	
	SET_DATASEL(selcs);
	SendByteTM1629D(SETADDR|addr);//发送起始地址

	if(length>16) length=16; //最多发送14个数据

	while(length)
	{
		length--;
		SendByteTM1629D(*data++);
	}
	SET_DATASEL(off);
	SET_DATASEL(selcs);
	SendByteTM1629D(SETPLUSE14|DISON);
	SET_DATASEL(off);
}
/*************    函数名：TestCoorSeg              ***************************
【功能】：检测数码管
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-12  13:25:11***********************/
INT8U gCheckSeg[16]={0x00};
INT8U gSegNum[12]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x40};
void TestCoorSeg(void)
{
	INT32U i=0;

	for(i=1;i<8;i++)
		MuxAddrDisTM1629D(i,0, 16, gCheckSeg);
}

/*************    函数名：TrandBCD                ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-13  11:20:02***********************/
void TrandBCD(INT32U data, INT8U *ptr, INT8U cnt)
{
	INT8U i=0;
	INT8U num=cnt;

	/*!对需要指针操作的部分清零*/
	//if(FLAG.systemready)
	{
		while(num)
		{
			(*(ptr+num-1))=0;
			num--;
		}
	}
	
	//首先转换为BCD码
	while(data>=1000000)	{data-=1000000;		        				    }
    while(data>=100000)		{data-=100000;		{if(cnt>5) (*(ptr+5))++;}	}
    while(data>=10000)		{data-=10000;		{if(cnt>4) (*(ptr+4))++;}	}
    while(data>=1000)		{data-=1000;		{if(cnt>3) (*(ptr+3))++;}	}
    while(data>=100)		{data-=100;			{if(cnt>2) (*(ptr+2))++;}	}
    while(data>=10)			{data-=10;			           (*(ptr+1))++; 	}
    *ptr=data;

	//清除前导零
	
	for(i=cnt-1;i>0;i--)
	{
		if(*(ptr+i))
			break;
		*(ptr+i)=10;
	}
}


/*************    函数名：DisLedCoor              ***************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-12  17:42:09***********************/
void DisLedCooryafen(INT8U sel, DISBUFF mybuff, INT8U led)
{
	INT8U disnum[16]={0};
	INT8U i=0;

	if(sel<4)
	{
		TrandBCD(mybuff.DISLED.myDis.ST.yafen[sel*2],&disnum[0],4);
		TrandBCD(mybuff.DISLED.myDis.ST.yafen[sel*2+1],&disnum[4],4);

		TrandBCD(mybuff.DISLED.myDis.ST.peilv[sel*2],&disnum[8],3);
		disnum[11]=disnum[9];
		disnum[9]=0;
		TrandBCD(mybuff.DISLED.myDis.ST.peilv[sel*2+1],&disnum[12],3);
		disnum[15]=disnum[13];
		disnum[13]=0;	
	}
	else if(sel==4)
	{
		TrandBCD(mybuff.DISLED.myDis.ST.yafen[sel*2],&disnum[0],4);
		TrandBCD(mybuff.DISLED.myDis.ST.yafen[sel*2+1],&disnum[4],4);

		TrandBCD(mybuff.DISLED.myDis.time,&disnum[8],2);
		TrandBCD(mybuff.DISLED.myDis.ST.credit,&disnum[10],6);
	}
	else if(sel==5) //3个押分和一个切换
	{
		TrandBCD(mybuff.DISLED.myDis.ST.yafen[10],&disnum[0],4);
		TrandBCD(mybuff.DISLED.myDis.ST.yafen[11],&disnum[4],4);

		TrandBCD(mybuff.DISLED.myDis.ST.yafen[12],&disnum[8],4);

		if(0==mYGame.systemsta)
			TrandBCD(mybuff.DISLED.myDis.unit,&disnum[12],4);
		else
		{
			for(i=0;i<32;i++)
			{
				/*!找到最优先的系统故障*/
				if(mYGame.systemsta&(1l<<i))
					break;
			}
				
			TrandBCD((9000+i),&disnum[12],4);
		}
	}
	else//赢分和彩金
	{
		TrandBCD(mybuff.DISLED.myDis.ST.bouns,&disnum[0],6);
		TrandBCD(mybuff.DISLED.myDis.ST.win,&disnum[6],6);
	}

	/*!中奖灯闪烁控制*/
	for(i=0;i<16;i++)
	{
		if(0==(i%2)) //灯0
		{
			if(0)
			{
				disnum[i]=gSegNum[disnum[i]]|0x80;
			}
			else
			{
				if(i<2)
					disnum[i]=gSegNum[disnum[i]]|0x80;
				else
					disnum[i]=gSegNum[disnum[i]];
			}
		}
		else//灯1
		{
			if(0)
			{
				disnum[i]=gSegNum[disnum[i]]|0x80;
			}
			else
			{
				if(i<3)
					disnum[i]=gSegNum[disnum[i]]|0x80;
				else
					disnum[i]=gSegNum[disnum[i]];
			}
		}
	}
	
	MuxAddrDisTM1629D(sel+1,0, 16, disnum);
}

/*************    函数名：InitStructDis           ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-13  12:38:21***********************/
void InitStructDis(void)
{
	INT8U i=0;

	for(i=0;i<sizeof(DISBUFF);i++)
	{
		MyDisbuff.buffer[i]=0;
	}
}
/*************    函数名：SysTemCheck              ***************************
【功能】：系统自检
【参数】：
【返回】：
【说明】：
        1、KEY0 查账
        2、KEY2 语音测试
        3、KEY3 分机号设置
        4、KEY2+KEY3 控台自动测试
        5、KEY0+KEY3 系统账目归零
**************   设计：南下三五年 2012-11-15  15:21:54***********************/
INT8U gMoveSeg[7][16]={0};
void SysTemCheck(void)
{
	INT8U disnum[16]={0};
	INT8U i=0,j=0,n=0;
	INT32S *ptr=&MyDisbuff.DISLED.myDis.ST.gain;
	
	FLAG.systemready=1;

	//显示开机画面  101    地址

	//关闭显示
	for(i=0;i<16;i++)
		disnum[i]=10;

	//写入数据
	TrandBCD(101,&disnum[13],3);

	//更新码表
	for(i=0;i<16;i++)
		disnum[i]=gSegNum[disnum[i]];
	
	//显示
	MuxAddrDisTM1629D(6,0, 16, disnum);
	
	//!账目
	//关闭显示
	for(i=0;i<16;i++)
		disnum[i]=10;

	//写入数据
	TrandBCD(MyDisbuff.DISLED.myDis.ST.id,&disnum[8],2);

	//更新码表
	for(i=0;i<16;i++)
		disnum[i]=gSegNum[disnum[i]];

	//显示
	MuxAddrDisTM1629D(5,0, 16, disnum);

	vTaskDelay(1000);
	
	//!账目查询
	if(0==GPIO_ReadInputDataBit(GPIOE, KEY0)
		&&GPIO_ReadInputDataBit(GPIOE, KEY1)
		&&GPIO_ReadInputDataBit(GPIOE, KEY2)
		&&GPIO_ReadInputDataBit(GPIOE, KEY3))
	{
		FLAG.billstep=50;
		FLAG.systemstep=READBILL;
		
		StopMusic(1);
		SendMusic(41,1,1);
		//GetStationU6295();
		vTaskDelay(2000);
		StopMusic(1);
		SendMusic(FLAG.billstep-22,1,1);
		for(;;)
		{
			taskENTER_CRITICAL(); /*调度器上锁*/
			//!指示
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			TrandBCD(4,&disnum[15],1);
			TrandBCD(FLAG.billstep,&disnum[12],2);

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//显示
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!账目
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			TrandBCD(*(ptr+(FLAG.billstep-50)),&disnum[10],6);

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//显示
			MuxAddrDisTM1629D(5,0, 16, disnum);
			
			taskEXIT_CRITICAL(); /*调度器开锁*/
			vTaskDelay(100);
		}
	}

	//语音测试
	if(0==GPIO_ReadInputDataBit(GPIOE, KEY2)
		&&GPIO_ReadInputDataBit(GPIOE, KEY0)
		&&GPIO_ReadInputDataBit(GPIOE, KEY1)
		&&GPIO_ReadInputDataBit(GPIOE, KEY3))
	{
		FLAG.systemstep=TESTSOUND;
		for(;;)
		{
			//声音播放
			if(++FLAG.sound>=42)
				FLAG.sound=1;
			
			StopMusic(1);
			SendMusic(FLAG.sound,1,1);
			GetStationU6295();
			
			taskENTER_CRITICAL(); /*调度器上锁*/
			//!指示
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			disnum[12]=11;
			disnum[13]=11;
			disnum[14]=11;
			disnum[15]=11;

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//显示
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!账目
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			if(FLAG.sound==41)
				TrandBCD(1,&disnum[10],6);
			else
				TrandBCD(FLAG.sound+1,&disnum[10],6);

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//显示
			MuxAddrDisTM1629D(5,0, 16, disnum);
			taskEXIT_CRITICAL(); /*调度器开锁*/
			vTaskDelay(2000);
		}
	}

	//分机ID设置
	if(0==GPIO_ReadInputDataBit(GPIOE, KEY3)
		&&GPIO_ReadInputDataBit(GPIOE, KEY0)
		&&GPIO_ReadInputDataBit(GPIOE, KEY1)
		&&GPIO_ReadInputDataBit(GPIOE, KEY2))
	{
		FLAG.systemstep=SETSLAVEID;
		FLAG.billstep=48;
		StopMusic(1);
		SendMusic(41,1,1);
		//GetStationU6295();
		vTaskDelay(2000);
		StopMusic(1);
		SendMusic(FLAG.billstep-22,1,1);
		
		for(;;)
		{
			taskENTER_CRITICAL(); /*调度器上锁*/
			//!指示
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			TrandBCD(3,&disnum[15],1);
			TrandBCD(50,&disnum[12],2);

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//显示
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!账目
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			TrandBCD(MyDisbuff.DISLED.myDis.ST.id,&disnum[8],2);

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//显示
			MuxAddrDisTM1629D(5,0, 16, disnum);
			
			taskEXIT_CRITICAL(); /*调度器开锁*/
			vTaskDelay(100);
		}
	}

	//自动测试
	if((0==GPIO_ReadInputDataBit(GPIOE, KEY2))
		&&(0==GPIO_ReadInputDataBit(GPIOE, KEY3))
		&&GPIO_ReadInputDataBit(GPIOE, KEY0)
		&&GPIO_ReadInputDataBit(GPIOE, KEY1))
	{
		FLAG.systemstep=AUTOCHECK;
		StopMusic(1);
		SendMusic(40,1,1);
		for(;;)
		{
			//8闪烁3次
			for(j=0;j<3;j++)
			{
				for(i=0;i<16;i++)
					gCheckSeg[i]=0xff;
				taskENTER_CRITICAL(); /*调度器上锁*/
				TestCoorSeg();
				taskEXIT_CRITICAL(); /*调度器开锁*/
				GPIO_SetBits(GPIOC, GPIO_Pin_4);
				vTaskDelay(500);

				for(i=0;i<16;i++)
					gCheckSeg[i]=0;
				taskENTER_CRITICAL(); /*调度器上锁*/
				TestCoorSeg();
				taskEXIT_CRITICAL(); /*调度器开锁*/
				GPIO_ResetBits(GPIOC, GPIO_Pin_4);
				vTaskDelay(500);
			}
			
			//!画线显示一次
			for(j=0;j<8;j++)
			{
				for(i=0;i<16;i++)
					gCheckSeg[i]=(1<<j)|0x80;
				taskENTER_CRITICAL(); /*调度器上锁*/
				TestCoorSeg();
				taskEXIT_CRITICAL(); /*调度器开锁*/
				vTaskDelay(500);
			}

			//!闪烁一次
			for(i=0;i<16;i++)
				gCheckSeg[i]=0xff;
			taskENTER_CRITICAL(); /*调度器上锁*/
			TestCoorSeg();
			taskEXIT_CRITICAL(); /*调度器开锁*/
			vTaskDelay(500);

			for(i=0;i<16;i++)
				gCheckSeg[i]=0;
			taskENTER_CRITICAL(); /*调度器上锁*/
			TestCoorSeg();
			taskEXIT_CRITICAL(); /*调度器开锁*/
			vTaskDelay(500);

			//8移动一次,共140个数码管
			for(j=0;j<100;j++)
			{
				//数据变换
				for(i=0;i<7;i++)
				{
					for(n=0;n<16;n++)
						gMoveSeg[i][n]=0; //关闭显示
				}
				
				if(j<32)
				{
					gMoveSeg[j/8][j%8]=0x7f;
				}
				else if(j<56)
				{
					if(((j-32)%6)==0)
						gMoveSeg[(j-32)/6][8]=0x7f;
					else if(((j-32)%6)==1)
						gMoveSeg[(j-32)/6][11]=0x7f;
					else if(((j-32)%6)==2)
						gMoveSeg[(j-32)/6][10]=0x7f;
					else if(((j-32)%6)==3)
						gMoveSeg[(j-32)/6][12]=0x7f;
					else if(((j-32)%6)==4)
						gMoveSeg[(j-32)/6][15]=0x7f;
					else if(((j-32)%6)==5)
						gMoveSeg[(j-32)/6][14]=0x7f;
				}
				else if(j<76)
				{
					if(j<64)
						gMoveSeg[4][j-56]=0x7f;
					else
						gMoveSeg[5][j-64]=0x7f;
				}
				else if(j<84)
				{
					gMoveSeg[4][j-76+8]=0x7f;
				}
				else if(j<96)
				{
					gMoveSeg[6][j-84]=0x7f;
				}
				else
					gMoveSeg[5][j-96+12]=0x7f;
				
				for(i=1;i<8;i++)
				{
					taskENTER_CRITICAL(); /*调度器上锁*/
					MuxAddrDisTM1629D(i,0, 16, gMoveSeg[i-1]);
					taskEXIT_CRITICAL(); /*调度器开锁*/
					vTaskDelay(20);
				}
			}
			vTaskDelay(5000);
		}
	}

	//!清帐
	if((0==GPIO_ReadInputDataBit(GPIOE, KEY0))
		&&(0==GPIO_ReadInputDataBit(GPIOE, KEY3))
		&&GPIO_ReadInputDataBit(GPIOE, KEY1)
		&&GPIO_ReadInputDataBit(GPIOE, KEY2))
	{
		FLAG.systemstep=CLEARBILL;

		StopMusic(1);
		SendMusic(27,1,1);

		for(i=0;i<8;i++)
			*ptr++=0;
		MyDisbuff.DISLED.myDis.ST.bouns=0;
		MyDisbuff.DISLED.myDis.ST.credit=0;
		MyDisbuff.DISLED.myDis.ST.selpeilv=0;
		MyDisbuff.DISLED.myDis.ST.win=0;
		for(i=0;i<13;i++)
			MyDisbuff.DISLED.myDis.ST.yafen[i]=0;
		SAVEFM25CL64(MyDisbuff.DISLED.myDis.ST);

		n=10;
		for(;;)
		{
			taskENTER_CRITICAL(); /*调度器上锁*/
			//!指示
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			TrandBCD(5,&disnum[15],1);
			TrandBCD(88,&disnum[12],2);

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//显示
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!账目
			//关闭显示
			for(i=0;i<16;i++)
				disnum[i]=10;

			//写入数据
			n--;
			if(n==0)
				break;
			TrandBCD(66,&disnum[10],2);
			TrandBCD(n,&disnum[15],1);
			disnum[12]=11;
			disnum[13]=11;
			disnum[14]=11;

			//更新码表
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//显示
			MuxAddrDisTM1629D(5,0, 16, disnum);
			
			taskEXIT_CRITICAL(); /*调度器开锁*/
			vTaskDelay(1000);
		}
	}
	
	FLAG.systemready=0;
}

/*************    函数名：CheckPara                ***************************
【功能】：参数合理化检查
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-16  22:29:52***********************/
void CheckPara(void)
{
	INT8U j=0;
	/*!爆机判断*/

	/*!押分返还*/

	/*!赔率选择*/
	if(MyDisbuff.DISLED.myDis.ST.selpeilv>6||(0==MyDisbuff.DISLED.myDis.ST.selpeilv))
		MyDisbuff.DISLED.myDis.ST.selpeilv=6;
	for(j=0;j<8;j++)
		MyDisbuff.DISLED.myDis.ST.peilv[j]=gamepeilv[MyDisbuff.DISLED.myDis.ST.selpeilv-1][j];
}

/*************    函数名：vLedDisplayTask           ***************************
【功能】：LED显示任务
【参数】：
【返回】：
【说明】：
		通过其他任务发送过来的信号量，来刷新显示
		显示部分测试正常
**************   设计：南下三五年 2012-05-27  21:38:40***********************/
INT32U gKeyPress=0;
void vLedDisplayTask( void *pvParameters )
{
	//!初始化
	vTaskDelay(100);
	InitAllTM1629();
	TestCoorSeg();
	//!获取铁电数据
	BuffReadFM25CL64(ST_ADDR, (INT8U *)&MyDisbuff.DISLED.myDis.ST,  sizeof(S_FRAM));

	//!系统自检判断
	SysTemCheck();

	CheckPara();

	//!自检结束，创建游戏任务
	xTaskCreate( vGamePlayerTask, ( signed char * ) "Game "  ,256,0, 3, NULL );

	//!任务中更新参数，中断刷新
	for(;;)
	{
		SAVEFM25CL64(MyDisbuff.DISLED.myDis.ST);
	
		vTaskDelay(100);
	}
}
#endif
   




























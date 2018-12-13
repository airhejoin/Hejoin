/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺�������ʾ
*����ߣ�����������
*ʱ  �䣺2012-05-24  00:53:19
*********************************************************************************
*/
#include".\Lib\includes.h"
#if 0
DISBUFF MyDisbuff;
S_FLAG FLAG;
/*************    ��������InitAllTM1629           ****************************
�����ܡ�����ʼ������TM1629
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-12  13:07:16***********************/
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

/*************    ��������LockAddrDisTM1629D       ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-26  00:59:18***********************/
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
/*************    ��������MuxAddrDisTM1629D        ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-05-26  20:45:30***********************/
void MuxAddrDisTM1629D(INT8U sel,INT8U addr, INT8U length, INT8U *data)
{
	INT16U selcs=sel;
	INT16U off=0;
	
	TM1629DOUTH;
	TM1629DCLKH;
	
	SET_DATASEL(selcs);
	SendByteTM1629D(AUTOADDADDR);//���õ�ַ�Զ���1ģʽ
	SET_DATASEL(off);
	
	SET_DATASEL(selcs);
	SendByteTM1629D(SETADDR|addr);//������ʼ��ַ

	if(length>16) length=16; //��෢��14������

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
/*************    ��������TestCoorSeg              ***************************
�����ܡ�����������
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-12  13:25:11***********************/
INT8U gCheckSeg[16]={0x00};
INT8U gSegNum[12]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x40};
void TestCoorSeg(void)
{
	INT32U i=0;

	for(i=1;i<8;i++)
		MuxAddrDisTM1629D(i,0, 16, gCheckSeg);
}

/*************    ��������TrandBCD                ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-13  11:20:02***********************/
void TrandBCD(INT32U data, INT8U *ptr, INT8U cnt)
{
	INT8U i=0;
	INT8U num=cnt;

	/*!����Ҫָ������Ĳ�������*/
	//if(FLAG.systemready)
	{
		while(num)
		{
			(*(ptr+num-1))=0;
			num--;
		}
	}
	
	//����ת��ΪBCD��
	while(data>=1000000)	{data-=1000000;		        				    }
    while(data>=100000)		{data-=100000;		{if(cnt>5) (*(ptr+5))++;}	}
    while(data>=10000)		{data-=10000;		{if(cnt>4) (*(ptr+4))++;}	}
    while(data>=1000)		{data-=1000;		{if(cnt>3) (*(ptr+3))++;}	}
    while(data>=100)		{data-=100;			{if(cnt>2) (*(ptr+2))++;}	}
    while(data>=10)			{data-=10;			           (*(ptr+1))++; 	}
    *ptr=data;

	//���ǰ����
	
	for(i=cnt-1;i>0;i--)
	{
		if(*(ptr+i))
			break;
		*(ptr+i)=10;
	}
}


/*************    ��������DisLedCoor              ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-12  17:42:09***********************/
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
	else if(sel==5) //3��Ѻ�ֺ�һ���л�
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
				/*!�ҵ������ȵ�ϵͳ����*/
				if(mYGame.systemsta&(1l<<i))
					break;
			}
				
			TrandBCD((9000+i),&disnum[12],4);
		}
	}
	else//Ӯ�ֺͲʽ�
	{
		TrandBCD(mybuff.DISLED.myDis.ST.bouns,&disnum[0],6);
		TrandBCD(mybuff.DISLED.myDis.ST.win,&disnum[6],6);
	}

	/*!�н�����˸����*/
	for(i=0;i<16;i++)
	{
		if(0==(i%2)) //��0
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
		else//��1
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

/*************    ��������InitStructDis           ****************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-13  12:38:21***********************/
void InitStructDis(void)
{
	INT8U i=0;

	for(i=0;i<sizeof(DISBUFF);i++)
	{
		MyDisbuff.buffer[i]=0;
	}
}
/*************    ��������SysTemCheck              ***************************
�����ܡ���ϵͳ�Լ�
����������
�����ء���
��˵������
        1��KEY0 ����
        2��KEY2 ��������
        3��KEY3 �ֻ�������
        4��KEY2+KEY3 ��̨�Զ�����
        5��KEY0+KEY3 ϵͳ��Ŀ����
**************   ��ƣ����������� 2012-11-15  15:21:54***********************/
INT8U gMoveSeg[7][16]={0};
void SysTemCheck(void)
{
	INT8U disnum[16]={0};
	INT8U i=0,j=0,n=0;
	INT32S *ptr=&MyDisbuff.DISLED.myDis.ST.gain;
	
	FLAG.systemready=1;

	//��ʾ��������  101    ��ַ

	//�ر���ʾ
	for(i=0;i<16;i++)
		disnum[i]=10;

	//д������
	TrandBCD(101,&disnum[13],3);

	//�������
	for(i=0;i<16;i++)
		disnum[i]=gSegNum[disnum[i]];
	
	//��ʾ
	MuxAddrDisTM1629D(6,0, 16, disnum);
	
	//!��Ŀ
	//�ر���ʾ
	for(i=0;i<16;i++)
		disnum[i]=10;

	//д������
	TrandBCD(MyDisbuff.DISLED.myDis.ST.id,&disnum[8],2);

	//�������
	for(i=0;i<16;i++)
		disnum[i]=gSegNum[disnum[i]];

	//��ʾ
	MuxAddrDisTM1629D(5,0, 16, disnum);

	vTaskDelay(1000);
	
	//!��Ŀ��ѯ
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
			taskENTER_CRITICAL(); /*����������*/
			//!ָʾ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			TrandBCD(4,&disnum[15],1);
			TrandBCD(FLAG.billstep,&disnum[12],2);

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//��ʾ
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!��Ŀ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			TrandBCD(*(ptr+(FLAG.billstep-50)),&disnum[10],6);

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//��ʾ
			MuxAddrDisTM1629D(5,0, 16, disnum);
			
			taskEXIT_CRITICAL(); /*����������*/
			vTaskDelay(100);
		}
	}

	//��������
	if(0==GPIO_ReadInputDataBit(GPIOE, KEY2)
		&&GPIO_ReadInputDataBit(GPIOE, KEY0)
		&&GPIO_ReadInputDataBit(GPIOE, KEY1)
		&&GPIO_ReadInputDataBit(GPIOE, KEY3))
	{
		FLAG.systemstep=TESTSOUND;
		for(;;)
		{
			//��������
			if(++FLAG.sound>=42)
				FLAG.sound=1;
			
			StopMusic(1);
			SendMusic(FLAG.sound,1,1);
			GetStationU6295();
			
			taskENTER_CRITICAL(); /*����������*/
			//!ָʾ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			disnum[12]=11;
			disnum[13]=11;
			disnum[14]=11;
			disnum[15]=11;

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//��ʾ
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!��Ŀ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			if(FLAG.sound==41)
				TrandBCD(1,&disnum[10],6);
			else
				TrandBCD(FLAG.sound+1,&disnum[10],6);

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//��ʾ
			MuxAddrDisTM1629D(5,0, 16, disnum);
			taskEXIT_CRITICAL(); /*����������*/
			vTaskDelay(2000);
		}
	}

	//�ֻ�ID����
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
			taskENTER_CRITICAL(); /*����������*/
			//!ָʾ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			TrandBCD(3,&disnum[15],1);
			TrandBCD(50,&disnum[12],2);

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//��ʾ
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!��Ŀ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			TrandBCD(MyDisbuff.DISLED.myDis.ST.id,&disnum[8],2);

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//��ʾ
			MuxAddrDisTM1629D(5,0, 16, disnum);
			
			taskEXIT_CRITICAL(); /*����������*/
			vTaskDelay(100);
		}
	}

	//�Զ�����
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
			//8��˸3��
			for(j=0;j<3;j++)
			{
				for(i=0;i<16;i++)
					gCheckSeg[i]=0xff;
				taskENTER_CRITICAL(); /*����������*/
				TestCoorSeg();
				taskEXIT_CRITICAL(); /*����������*/
				GPIO_SetBits(GPIOC, GPIO_Pin_4);
				vTaskDelay(500);

				for(i=0;i<16;i++)
					gCheckSeg[i]=0;
				taskENTER_CRITICAL(); /*����������*/
				TestCoorSeg();
				taskEXIT_CRITICAL(); /*����������*/
				GPIO_ResetBits(GPIOC, GPIO_Pin_4);
				vTaskDelay(500);
			}
			
			//!������ʾһ��
			for(j=0;j<8;j++)
			{
				for(i=0;i<16;i++)
					gCheckSeg[i]=(1<<j)|0x80;
				taskENTER_CRITICAL(); /*����������*/
				TestCoorSeg();
				taskEXIT_CRITICAL(); /*����������*/
				vTaskDelay(500);
			}

			//!��˸һ��
			for(i=0;i<16;i++)
				gCheckSeg[i]=0xff;
			taskENTER_CRITICAL(); /*����������*/
			TestCoorSeg();
			taskEXIT_CRITICAL(); /*����������*/
			vTaskDelay(500);

			for(i=0;i<16;i++)
				gCheckSeg[i]=0;
			taskENTER_CRITICAL(); /*����������*/
			TestCoorSeg();
			taskEXIT_CRITICAL(); /*����������*/
			vTaskDelay(500);

			//8�ƶ�һ��,��140�������
			for(j=0;j<100;j++)
			{
				//���ݱ任
				for(i=0;i<7;i++)
				{
					for(n=0;n<16;n++)
						gMoveSeg[i][n]=0; //�ر���ʾ
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
					taskENTER_CRITICAL(); /*����������*/
					MuxAddrDisTM1629D(i,0, 16, gMoveSeg[i-1]);
					taskEXIT_CRITICAL(); /*����������*/
					vTaskDelay(20);
				}
			}
			vTaskDelay(5000);
		}
	}

	//!����
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
			taskENTER_CRITICAL(); /*����������*/
			//!ָʾ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			TrandBCD(5,&disnum[15],1);
			TrandBCD(88,&disnum[12],2);

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];
			
			//��ʾ
			MuxAddrDisTM1629D(6,0, 16, disnum);
			
			//!��Ŀ
			//�ر���ʾ
			for(i=0;i<16;i++)
				disnum[i]=10;

			//д������
			n--;
			if(n==0)
				break;
			TrandBCD(66,&disnum[10],2);
			TrandBCD(n,&disnum[15],1);
			disnum[12]=11;
			disnum[13]=11;
			disnum[14]=11;

			//�������
			for(i=0;i<16;i++)
				disnum[i]=gSegNum[disnum[i]];

			//��ʾ
			MuxAddrDisTM1629D(5,0, 16, disnum);
			
			taskEXIT_CRITICAL(); /*����������*/
			vTaskDelay(1000);
		}
	}
	
	FLAG.systemready=0;
}

/*************    ��������CheckPara                ***************************
�����ܡ��������������
����������
�����ء���
��˵������
**************   ��ƣ����������� 2012-11-16  22:29:52***********************/
void CheckPara(void)
{
	INT8U j=0;
	/*!�����ж�*/

	/*!Ѻ�ַ���*/

	/*!����ѡ��*/
	if(MyDisbuff.DISLED.myDis.ST.selpeilv>6||(0==MyDisbuff.DISLED.myDis.ST.selpeilv))
		MyDisbuff.DISLED.myDis.ST.selpeilv=6;
	for(j=0;j<8;j++)
		MyDisbuff.DISLED.myDis.ST.peilv[j]=gamepeilv[MyDisbuff.DISLED.myDis.ST.selpeilv-1][j];
}

/*************    ��������vLedDisplayTask           ***************************
�����ܡ���LED��ʾ����
����������
�����ء���
��˵������
		ͨ�����������͹������ź�������ˢ����ʾ
		��ʾ���ֲ�������
**************   ��ƣ����������� 2012-05-27  21:38:40***********************/
INT32U gKeyPress=0;
void vLedDisplayTask( void *pvParameters )
{
	//!��ʼ��
	vTaskDelay(100);
	InitAllTM1629();
	TestCoorSeg();
	//!��ȡ��������
	BuffReadFM25CL64(ST_ADDR, (INT8U *)&MyDisbuff.DISLED.myDis.ST,  sizeof(S_FRAM));

	//!ϵͳ�Լ��ж�
	SysTemCheck();

	CheckPara();

	//!�Լ������������Ϸ����
	xTaskCreate( vGamePlayerTask, ( signed char * ) "Game "  ,256,0, 3, NULL );

	//!�����и��²������ж�ˢ��
	for(;;)
	{
		SAVEFM25CL64(MyDisbuff.DISLED.myDis.ST);
	
		vTaskDelay(100);
	}
}
#endif
   




























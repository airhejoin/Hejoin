/*
*********************************************************************************
*                                 STM32F103
*                     ����RVMDK���ɿ�������,FreeRTOSӦ�ÿ�
*									 Ver2.0
*                            �ùܼҵ��ӿƼ����޹�˾
*
*ģ  �飺IIC��ʼ������
*����ߣ�����������
*ʱ  �䣺2012-05-24  01:03:27
*********************************************************************************
*/
#include"includes.h"

/*��ȡ����ʱ��*/
INT8U const compile_time[20]=__DATE__ __TIME__;

/*************    ��������ResetDate          ***************************
�����ܡ���
����������
�����ء���
��˵������
**************   ��ƣ�ľ�� 2010-06-13  16:55:57***********************/
void ResetDate(void)
{
	INT8U month=0;
	INT8U mon_str[12][4]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	for(month=0;month<12;month++)
	{
		if(compile_time[0]==mon_str[month][0] &&
				compile_time[1]==mon_str[month][1]&&
				compile_time[2]==mon_str[month][2])
		{
				month++;
				break;
		}
	}

	CLOCK.month=month;

	if(compile_time[4]==' ')
		CLOCK.day=compile_time[5]-'0';
	else
		CLOCK.day=(compile_time[4]-'0')*10+(compile_time[5]-'0');
	
	CLOCK.year=(compile_time[9]-'0')*10+(compile_time[10]-'0');
	CLOCK.hour=(compile_time[11]-'0')*10+(compile_time[12]-'0');
	CLOCK.min=(compile_time[14]-'0')*10+(compile_time[15]-'0')+1;
	CLOCK.sec=(compile_time[17]-'0')*10+(compile_time[18]-'0');

	//ת��ΪBCD���ʽ��������ʼ��ʱ�ӼĴ���
	CLOCK.sec=(CLOCK.sec/10)*16+(CLOCK.sec%10);
	CLOCK.min=(CLOCK.min/10)*16+(CLOCK.min%10);
	CLOCK.hour=(CLOCK.hour/10)*16+(CLOCK.hour%10);
	CLOCK.week=(CLOCK.week/10)*16+(CLOCK.week%10);
	CLOCK.day=(CLOCK.day/10)*16+(CLOCK.day%10);
	CLOCK.month=(CLOCK.month/10)*16+(CLOCK.month%10);
	CLOCK.year=(CLOCK.year/10)*16+(CLOCK.year%10);

	CLOCK.week=4;  

	WriteISL1208(0x07,0x90);//��ʼ��ʱ��
	
	WriteISL1208( 0x00, CLOCK.sec);
	WriteISL1208( 0x01, CLOCK.min);
	WriteISL1208( 0x02, CLOCK.hour|0x80);
	WriteISL1208( 0x03, CLOCK.day);
	WriteISL1208( 0x04, CLOCK.month);
	WriteISL1208( 0x05, CLOCK.year);  
	WriteISL1208( 0x06, CLOCK.week);

 
	WriteISL1208(0x08,0x40);
	WriteISL1208(0x00,0x00);//��ʼ��ʱ��   
}











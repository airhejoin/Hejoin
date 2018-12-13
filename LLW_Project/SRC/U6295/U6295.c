/*
*********************************************************************************
*                                 STM32F103
*                     基于RVMDK集成开发环境,FreeRTOS应用库
*									 Ver2.0
*                            好管家电子科技有限公司
*
*模  块：U6295初始化配置
*设计者：南下三五年
*时  间：2012-11-14  11:03:27
*********************************************************************************
*/
#include"..\Lib\includes.h"

/*************    函数名：InitU6295IO             ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-13  17:01:41***********************/
void InitU6295IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = U6295CS|U6295RD|U6295RST|U6295WR
		|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
		|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	GPIO_SetBits(GPIOD, U6295CS);
	GPIO_SetBits(GPIOD, U6295RD);
	GPIO_SetBits(GPIOD, U6295RST);
	GPIO_SetBits(GPIOD, U6295WR);
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
	
	GPIO_ResetBits(GPIOD, U6295RST);
	vTaskDelay(100);
	GPIO_SetBits(GPIOD, U6295RST);

	SENDDATU6295(0xff);
}

/*************    函数名：Delay50us               ****************************
【功能】：硬件时序要求，至少500NS
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-14  15:25:35***********************/
void Delay50us(void)
{
	INT16U i=0;

	for(i=0;i<36;i++)
		__NOP();
}
/*************    函数名：SendMusic                ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-13  17:35:32***********************/
void SendMusic(INT8U music, INT8U channel, INT8U reduction)
{
	INT16U data=0;
	
	data=0x80|music;
	
	GPIO_ResetBits(GPIOD, U6295CS);  //拉低CS，片选
	GPIO_ResetBits(GPIOD, U6295WR);  //数据就绪
	SENDDATU6295(data);              //口线准备数据
	Delay50us();
	GPIO_SetBits(GPIOD, U6295WR);    //数据取走
	GPIO_SetBits(GPIOD, U6295CS);
	
	GPIO_ResetBits(GPIOD, U6295CS);
	GPIO_ResetBits(GPIOD, U6295WR);
	data=(channel<<4) + reduction;
	SENDDATU6295(data);
	Delay50us();
	GPIO_SetBits(GPIOD, U6295WR);
	GPIO_SetBits(GPIOD, U6295CS);
	data=0xff;
	SENDDATU6295(data);
}

/*************    函数名：StopMusic                ****************************
【功能】：
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-13  17:42:05***********************/
void StopMusic(INT8U channel)
{
	INT16U data=0;
	GPIO_ResetBits(GPIOD, U6295CS);
	GPIO_ResetBits(GPIOD, U6295WR);
	data=1<<(2+channel);
	SENDDATU6295(data);
	Delay50us();
	GPIO_SetBits(GPIOD, U6295WR);  
	GPIO_SetBits(GPIOD, U6295CS);
	data=0xff;
	SENDDATU6295(data);
}

/*************    函数名：GetStationU6295         ****************************
【功能】：判断声音是否播放完成
【参数】：
【返回】：
【说明】：
**************   设计：南下三五年 2012-11-16  10:03:13***********************/
void GetStationU6295(void)
{
	INT16U i=0;
	
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
		                          GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	    //下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	for(i=0;i<0xffff;i++)
	{
		GPIO_ResetBits(GPIOD, U6295CS);  //拉低CS，片选
		GPIO_ResetBits(GPIOD, U6295RD);  //开始读
		Delay50us();
		
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0))
		{
			break;
		}
		
		GPIO_SetBits(GPIOD, U6295RD);    //读结束
		GPIO_SetBits(GPIOD, U6295CS);

		vTaskDelay(10);//Delay50us();
	}
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
		                          GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	vTaskDelay(50);
}





































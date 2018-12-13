#include"PS2.h"
#include"USART.h"
#define PS2_KEYCODE_SIZE 17
volatile u16 PS2Data=0;
const u8 PS2_KEYCODE[PS2_KEYCODE_SIZE][2]=           
{     
  0x45,'0',	
  0x16,'1',   
  0x1e,'2',  
  0x26,'3', 
	0x25,'4',  
  0x2e,'5',  
  0x36,'6',   
  0x3d,'7',  
  0x3e,'8',  
  0x46,'9', 
	0x76,'B',//back
	0x7d,'U',//上翻up
	0x7A,'D',//下翻down
	0x5A,'E',//回车enter
	0x66,'C',//清除clear
	0x49,'P',//点pointer
	0xf0,0xf0
};  
u8 PS2_KeyStatus[PS2_KEYCODE_SIZE]={0};

//PS2产生的时钟频率在10~20Khz(最大33K)
//高/低电平的持续时间为25~50us之间.	  
			 
//PS2_Status当前状态标志
//[5:4]:当前工作的模式;[7]:接收到一次数据
//[6]:校验错误;[3:0]:收到的数据长度;
void PS2Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;	//外部中断线
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);   
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;	//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//EXTI_Mode_Event ;//		//设外外部中断模式:EXTI线路为中断请求  设置 EXTI线路为事件请求 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //外部中断触发沿选择:设置输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//使能外部中断新状态
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器		  

}
u8 PS2Decode(u16 dat)
{
	u8 i=0;
	if(dat!=PS2_KEYCODE[PS2_KEYCODE_SIZE-1][0])
	{
		for(;i<PS2_KEYCODE_SIZE;i++)
		{
			if(dat==PS2_KEYCODE[i][0])
			{
				if(PS2_KeyStatus[i]==0)
				{
					PS2_KeyStatus[i]=1;//按下的标志置位
					//Usart1SendByte(i);
					return i;
				}
				else
				{
					PS2_KeyStatus[i]=0;//按下的标志复位
				}
			}
		}		
	}
	return 0xff;//0xf0
}

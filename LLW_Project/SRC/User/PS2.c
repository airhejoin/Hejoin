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
	0x7d,'U',//�Ϸ�up
	0x7A,'D',//�·�down
	0x5A,'E',//�س�enter
	0x66,'C',//���clear
	0x49,'P',//��pointer
	0xf0,0xf0
};  
u8 PS2_KeyStatus[PS2_KEYCODE_SIZE]={0};

//PS2������ʱ��Ƶ����10~20Khz(���33K)
//��/�͵�ƽ�ĳ���ʱ��Ϊ25~50us֮��.	  
			 
//PS2_Status��ǰ״̬��־
//[5:4]:��ǰ������ģʽ;[7]:���յ�һ������
//[6]:У�����;[3:0]:�յ������ݳ���;
void PS2Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;	//�ⲿ�ж���
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);   
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;	//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//EXTI_Mode_Event ;//		//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����  ���� EXTI��·Ϊ�¼����� 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�ⲿ�жϴ�����ѡ��:����������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//ʹ���ⲿ�ж���״̬
	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���		  

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
					PS2_KeyStatus[i]=1;//���µı�־��λ
					//Usart1SendByte(i);
					return i;
				}
				else
				{
					PS2_KeyStatus[i]=0;//���µı�־��λ
				}
			}
		}		
	}
	return 0xff;//0xf0
}

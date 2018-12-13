#include "SPI.h"

void SPIInit(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI1 	,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PE7 SDƬѡ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_7); //PE7����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PB12 FlashƬѡ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); //PB12/PB13/14/15����
	/* SPI1 configuration  */
	SPI_Cmd(SPI1, DISABLE); 												//�����Ƚ���,���ܸı�MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;								//CPOL=0 ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=0 ���ݲ����1��
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//���NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//16��Ƶ	,����̫��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE); 
	SPI1_ReadWriteByte(0xff);//��������
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);//����Ϊ18Mʱ��,����ģʽ
}

//SPI1��дһ�ֽ�����
u8 SPI1_ReadWriteByte(u8  TxData)
{

  	/* Loop while DR register in not emplty */
  	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  	/* Send byte through the SPI1 peripheral */
  	SPI_I2S_SendData(SPI1, TxData);

  	/* Wait to receive a byte */
  	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  	/* Return the byte read from the SPI bus */
  	return SPI_I2S_ReceiveData(SPI1);
}

void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 

} 

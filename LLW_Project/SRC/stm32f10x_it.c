/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include".\Lib\includes.h"
#define HARDFAULT_RETRUN_CHECK
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : 
* Description    : 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{//CLK�ж�
	}
	EXTI_ClearITPendingBit(EXTI_Line11);  //���EXTI13��·����λ
	/*�ж��Ƿ���Ҫ�����л�*/
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); 
}

/*************    ��������TIM2_IRQHandler         ****************************
�����ܡ���
����������
�����ء���
��˵������
*******************************************************************************/
#define TIME_50MS_MAX 5
#define TIME_125MS_MAX 25
#define TIME_250MS_MAX 50
#define TIME_500MS_MAX 100
#define TIME_1S_MAX 200
#define BLINK_DELAY_MAX 50
u16 InitDelay;
u16 Delay1sCnt=0;
u16 Delay50MsCnt=0;
u16 Delay125MsCnt=0;
u16 Delay250MsCnt=0;
u16 Delay500MsCnt=0;
u16 RunLedDelay=0;
volatile u32 Timer1, Timer2; 
void TIM2_IRQHandler(void)
{
  static u16 delay=0;
  static u8 count=0;
	u8 i=0;
	/*��ʱ��2��׼5MS*/
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    if(Timer1)	Timer1--;
		if(Timer2)	Timer2--;
		 if(++Delay50MsCnt>TIME_50MS_MAX)
    {
			Delay50MsCnt=0;
			Flag.Blink50Ms=~Flag.Blink50Ms;
		}
		//1S����
    if(++Delay125MsCnt>TIME_125MS_MAX)
    {
      Delay125MsCnt=0;
			Flag.Blink125Ms=~Flag.Blink125Ms;	
    }
    if(++Delay250MsCnt>TIME_250MS_MAX)
    {
      Delay250MsCnt=0;
      Flag.Blink250Ms=~Flag.Blink250Ms;	
    }
    //500MS����
    if(++Delay500MsCnt>TIME_500MS_MAX)
    {
      Delay500MsCnt=0;
      Flag.Blink500Ms=~Flag.Blink500Ms;	
      //���е�
      if(Flag.Blink500Ms)
        RunLedOn();
      else 
        RunLedOff();
    }
		//1S����Delay1sCnt
		if(++Flag.Delay1sCnt>TIME_1S_MAX)
    {
      Flag.Delay1sCnt=0;
			Flag.Blink1S=~Flag.Blink1S;
    }
	}
}
/*************    ��������USART1_IRQHandler        ***************************
�����ܡ���
����������
�����ء���
��˵������
*********************************************************************************/
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); //�ж��Ƿ���Ҫ�л�����
} 

/*************    ��������USART2_IRQHandler        ***************************
�����ܡ���
����������
�����ء���
��˵������
*********************************************************************************/
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		USART_ReceiveData(USART2);

	}

	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(USART2);				//��DR
	}
	
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); //�ж��Ƿ���Ҫ�л�����
} 

/*************    ��������USART2_IRQHandler        ***************************
�����ܡ���
����������
�����ء���
��˵������
*********************************************************************************/
void USART3_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		if(RS485Rec.RecCount<255)
		{
			RS485Rec.Msg[RS485Rec.MsgEnd][RS485Rec.RecCount++]=USART_ReceiveData(USART3);
			RS485Rec.RecTimeOut=RECMSGTIMEOUT; //10MSû�����ݣ���ǿ�Ʒ��						
		}
		else
		{
			RS485Rec.RecCount=0;
		}
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); //�ж��Ƿ���Ҫ�л�����
} 
/*************    ��������UART4_IRQHandler        ***************************
�����ܡ���
����������
�����ء���
��˵������
*********************************************************************************/
void UART4_IRQHandler(void)                	//����4�жϷ������
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(USART_GetITStatus(UART4,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		if(GsmRec.RecCount<MAXMSGBUFFERLEN-1)
		{
			GsmRec.Msg[GsmRec.MsgEnd][GsmRec.RecCount++]=USART_ReceiveData(UART4);
			GsmRec.RecTimeOut=RECMSGTIMEOUT; //10MSû�����ݣ���ǿ�Ʒ��						
		}
		else
		{
			GsmRec.RecCount=0;
		}		
	}
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART4,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(UART4);				//��DR
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); //�ж��Ƿ���Ҫ�л�����
} 

/*************    ��������UART5_IRQHandler        ***************************
�����ܡ���
����������
�����ء���
��˵������
*********************************************************************************/
void UART5_IRQHandler(void)                	//����5�жϷ������
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(USART_GetITStatus(UART5,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		USART_ReceiveData(UART5);				//��DR
	}
	
		//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(UART5,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART5,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(UART5);				//��DR
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); //�ж��Ƿ���Ҫ�л�����
} 
/*************��������TIM3_IRQHandler************************
�����ܡ���
����������
�����ء���
��˵������
**************��ƣ�David 2017-11-16 ***********************/
void TIM3_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	/*��ʱ��2��׼2MS*/
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{	
		/*!���ڳ�ʱ���*/
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		/*!����4��ʱ���*/
		if(GsmRec.RecTimeOut>0)
		{
			GsmRec.RecTimeOut--;
			if(0==GsmRec.RecTimeOut)
			{
				GsmRec.Msg[GsmRec.MsgEnd][MAXMSGBUFFERLEN-1]=GsmRec.RecCount;
				GsmRec.RecCount=0;
				if( ++GsmRec.MsgEnd>= MAXMSGSIZE)	GsmRec.MsgEnd=0;
				xSemaphoreGiveFromISR( xGsmRecSemaphore, &xHigherPriorityTaskWoken );
			}
		}  
		
		/*!����4��ʱ���*/
		if(RS485Rec.RecTimeOut>0)
		{
			RS485Rec.RecTimeOut--;
			if(0==RS485Rec.RecTimeOut)
			{
				RS485Rec.Msg[RS485Rec.MsgEnd][MAXMSGBUFFERLEN-1]=RS485Rec.RecCount;
				RS485Rec.RecCount=0;
				if( ++RS485Rec.MsgEnd>= MAXMSGSIZE)	RS485Rec.MsgEnd=0;
				xSemaphoreGiveFromISR( xRS485RecSemaphore, &xHigherPriorityTaskWoken );
			}
		}
	}
	/*�ж��Ƿ���Ҫ�����л�*/
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); 
}
/*************    ��������TIM4_IRQHandler             ************************
�����ܡ���
����������
�����ء���
��˵������
*********************************************************************************/
void TIM4_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	/*��ʱ��2��׼2MS*/
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{	
		/*!���ڳ�ʱ���*/
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
	/*�ж��Ƿ���Ҫ�����л�*/
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); 
}




/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}
void HardFault_Handler(void)
{
	
  /* Go to infinite loop when Hard Fault exception occurs */
	//wait();
  while (1)
  {
		//wait();
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern xSemaphoreHandle xCANSendSemaphore;  
void USB_HP_CAN1_TX_IRQHandler(void)
{	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	

	/*�ж��Ƿ���Ҫ�����л�*/
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); 

}
/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	/*�ж��Ƿ���Ҫ�����л�*/
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); 
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

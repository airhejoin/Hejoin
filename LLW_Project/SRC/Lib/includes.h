#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
//#include<absacc.h>
/*STM32F公共头文件*/
#include "..\STM32Lib\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h"

/*FreeRTOS头文件*/
#include "..\FreeRTOS\Source\include\FreeRTOS.h"
#include "..\FreeRTOS\Source\include\task.h"
#include "..\FreeRTOS\Source\include\queue.h"
#include "..\FreeRTOS\Source\include\croutine.h"
#include "..\FreeRTOS\Source\include\semphr.h"
#include "..\FreeRTOS\Source\include\list.h"
#include "..\FreeRTOS\Source\include\mpu_wrappers.h"
#include "..\FreeRTOS\Source\include\portable.h"
#include "..\FreeRTOS\Source\include\StackMacros.h"


/*库头文件*/
#include"common.h"
#include"compiletime.h"
#include"function.h"

//驱动头文件
//#include"..\Password\Password.h"

/*增加应用头文件*/
#include"..\User\FreeRTOSTemplate.h"
#include"..\User\TripleDes.h"
#include"..\W25QXX\W25Q128.h"
#include"..\SegDisplay\SegDisplay.h"
#include"..\LCD\TFT32.h"
#include"..\MT\MT_random.h"
#include"..\User\Led.h"
#include"..\User\USART.h"
#include"..\User\SPI.h"
#include"..\User\Set.h"
#include"..\User\NVIC.h"
#include"..\User\TIM.h"
#include"..\User\RCC.h"
#include"..\User\Key.h"
#include"..\FRAM\FM24C04.h"
#include"..\CRC16\CRC16.h"
#include"..\Text\Text.h"
#include"..\MT\MT_random.h"
#include"..\User\HardwareInit.h"
//!增加任务头文件
#include"..\User\TaskCOM.h"
#include"..\User\TaskKey.h"
#include"..\User\TaskDisplay.h"
#include"..\User\TaskGsm.h"
#include"..\User\TaskGame.h"
#pragma diag_suppress 870	//消除字符串的警告

#endif 


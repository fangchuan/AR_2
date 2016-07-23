/*
*********************************************************************************************************
*
*	模块名称 : 舵机驱动模块
*	文件名称 : bsp_servo.h
*	版    本 : V1.0
*	说    明 : 初始化舵机驱动的定时器、GPIO引脚
*						 舵机配置函数
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/  
#ifndef  _BSP_SERVO_H
#define  _BSP_SERVO_H

#include "stm32f10x.h"
#include "_apollorobot.h"

#define  SERVO_PORT_CLK  	RCC_APB2Periph_GPIOA
#define  SERVO_PORT        GPIOA
#define  SERVO_1_Pin 			GPIO_Pin_0
#define  SERVO_2_Pin 			GPIO_Pin_1
#define  SERVO_3_Pin 			GPIO_Pin_2
#define  SERVO_4_Pin 			GPIO_Pin_3

#define  SERVO1_OUT       TIM2->CCR1
#define  SERVO2_OUT       TIM2->CCR2
#define  SERVO3_OUT       TIM2->CCR3
#define  SERVO4_OUT       TIM2->CCR4

/*Public function*/
extern void SERVO_PWM_Init(void);
extern _Error  SERVO_Config(_Servo *servo);
extern _Error   SERVO_Manual (uint8_t id, uint8_t degree);
#endif /*_BSP_SERVO_H*/

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

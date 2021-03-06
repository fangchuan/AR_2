/*
*********************************************************************************************************
*
*	模块名称 : 电机驱动函数模块
*	文件名称 : bsp_motor.h
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-07-21 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef  _BSP_MOTOR_H
#define  _BSP_MOTOR_H

#include "stm32f10x.h"
#include "_apollorobot.h"
#include "IO_Operation.h"

//驱动L293D的IO引脚
#define  MOTOR_PORT_CLK 	RCC_APB2Periph_GPIOB
#define  MOTOR_11_Pin     GPIO_Pin_1
#define  MOTOR_12_Pin     GPIO_Pin_0
#define  MOTOR_21_Pin			GPIO_Pin_8
#define  MOTOR_22_Pin   	GPIO_Pin_9
#define  MOTOR_PORT       GPIOB


//PWM输出控制
#define  MOTOR_11_OUT			TIM3->CCR4
#define  MOTOR_12_OUT			TIM3->CCR3
#define  MOTOR_21_OUT			TIM4->CCR3
#define  MOTOR_22_OUT			TIM4->CCR4
//默认PWM输出，duty=1250/2000 *100%
#define  DEFAULT_MAX      1999
#define  DEFAULT_AVR      1250
#define  DEFAULT_MIN      100
//电机转速比例参数
#define  MOTOR_SPEED_P    20


extern  void MOTOR_Init(void);
extern  void MOTOR_1_Forward(void);
extern  void MOTOR_1_Reverse(void);
extern  void MOTOR_1_Stop(void);
extern  void MOTOR_2_Forward(void);
extern  void MOTOR_2_Reverse(void);
extern  void MOTOR_2_Stop(void);
extern  _Error MOTOR_Config(_Motor *motor);
extern  _Error CAR_Config(_Car *car);
extern void Car_Forward(void);
extern void Car_Backward(void);
extern void Car_Left(void);
extern void Car_Right(void);
extern void Car_Stop(void);
	
#endif /*_BSP_MOTOR_H*/
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

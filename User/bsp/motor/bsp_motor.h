#ifndef  _BSP_MOTOR_H
#define  _BSP_MOTOR_H

#include "stm32f10x.h"
#include "_apollorobot.h"
#include "IO_Operation.h"

//驱动L293D的IO引脚
#define  MOTOR_PORT_CLK 	RCC_APB2Periph_GPIOB
#define  MOTOR_11_Pin     GPIO_Pin_6
#define  MOTOR_12_Pin     GPIO_Pin_7
#define  MOTOR_21_Pin			GPIO_Pin_8
#define  MOTOR_22_Pin   	GPIO_Pin_9
#define  MOTOR_PORT       GPIOB
#define  MOTOR_Enable_1   GPIO_Pin_0
#define  MOTOR_Enable_2		GPIO_Pin_1

//PWM输出控制
#define  MOTOR_11_OUT			TIM4->CCR1
#define  MOTOR_12_OUT			TIM4->CCR2
#define  MOTOR_21_OUT			TIM4->CCR3
#define  MOTOR_22_OUT			TIM4->CCR4
//默认PWM输出，duty=150/2000 *100%
#define  DEFAULT_MAX      2000
#define  DEFAULT_AVR      1500
#define  DEFAULT_MIN      1000

extern  void MOTOR_Init(void);
extern  void MOTOR_1_Enable(void);
extern  void MOTOR_1_Disable(void);
extern  void MOTOR_2_Enable(void);
extern  void MOTOR_2_Disable(void);
extern  _Error MOTOR_Config(_Motor *motor);
extern  void Car_Forward(void);
extern  void Car_Backward(void);
extern  void Car_Left(void);
extern  void Car_Right(void);
extern  void Car_Stop(void);



	
#endif /*_BSP_MOTOR_H*/
/* --------------------------------------end of file--------------------------------------- */

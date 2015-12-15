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
#endif /*_BSP_SERVO_H*/

/* --------------------------------------end of file--------------------------------------- */

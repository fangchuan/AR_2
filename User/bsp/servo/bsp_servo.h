#ifndef  _BSP_SERVO_H
#define  _BSP_SERVO_H

#include "stm32f10x.h"

#define SERVO_PORT_CLK  	RCC_APB2Periph_GPIOA
#define SERVO_PORT        GPIOA
#define SERVO_1_Pin 			GPIO_Pin_0
#define SERVO_2_Pin 			GPIO_Pin_1
#define SERVO_3_Pin 			GPIO_Pin_2
#define SERVO_4_Pin 			GPIO_Pin_3

/*Public function*/
extern void SERVO_PWM_Init(void);

#endif /*_BSP_SERVO_H*/

/* --------------------------------------end of file--------------------------------------- */

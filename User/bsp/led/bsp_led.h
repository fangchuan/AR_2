#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include "IO_Operation.h"

/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(GPIOG,GPIO_Pin_14)
#define LED1_OFF		digitalHi(GPIOG,GPIO_Pin_14)
#define LED1_ON			digitalLo(GPIOG,GPIO_Pin_14)

#define LED2_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_7)
#define LED2_OFF		digitalHi(GPIOF,GPIO_Pin_7)
#define LED2_ON			digitalLo(GPIOF,GPIO_Pin_7)

#define LED3_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_8)
#define LED3_OFF		digitalHi(GPIOF,GPIO_Pin_8)
#define LED3_ON			digitalLo(GPIOF,GPIO_Pin_8)

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_7)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_8)
					
					
void LED_GPIO_Config(void);

#endif /* __LED_H */

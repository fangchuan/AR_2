#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include "_apollorobot.h"
#include "IO_Operation.h"


#define LED1_PIN   GPIO_Pin_4
#define LED2_PIN   GPIO_Pin_5
#define LED3_PIN   GPIO_Pin_6
#define LED4_PIN   GPIO_Pin_7

 /* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_4)
#define LED1_OFF		  digitalHi(GPIOC,GPIO_Pin_4)
#define LED1_ON			  digitalLo(GPIOC,GPIO_Pin_4)

#define LED2_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_5)
#define LED2_OFF		  digitalHi(GPIOC,GPIO_Pin_5)
#define LED2_ON			  digitalLo(GPIOC,GPIO_Pin_7)

#define LED3_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_6)
#define LED3_OFF		  digitalHi(GPIOC,GPIO_Pin_6)
#define LED3_ON			  digitalLo(GPIOC,GPIO_Pin_6)

#define LED4_TOGGLE   digitalToggle(GPIOC,GPIO_Pin_7)
#define LED4_OFF      digitalHi(GPIOC,GPIO_Pin_7)
#define LED4_ON       digitalLo(GPIOC,GPIO_Pin_7)

///* 带参宏，可以像内联函数一样使用 */
//#define LED1(a)	if (a)	\
//					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
//					else		\
//					GPIO_ResetBits(GPIOC,GPIO_Pin_4)

//#define LED2(a)	if (a)	\
//					GPIO_SetBits(GPIOC,GPIO_Pin_5);\
//					else		\
//					GPIO_ResetBits(GPIOC,GPIO_Pin_5)

//#define LED3(a)	if (a)	\
//					GPIO_SetBits(GPIOC,GPIO_Pin_6);\
//					else		\
//					GPIO_ResetBits(GPIOC,GPIO_Pin_6)
//					
//#define LED4(a) if(a) \
//					GPIO_SetBits(GPIOC,GPIO_Pin_7);\
//					else   \
//					GPIO_ResetBits(GPIOC,GPIO_Pin_7)
						
					
					
extern _Error LED_Config (_Led *led);

#endif /* __LED_H */

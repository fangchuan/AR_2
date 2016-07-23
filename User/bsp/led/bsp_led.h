/*
*********************************************************************************************************
*
*	ģ������ : led bsp ģ��ͷ�ļ�
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/  
#ifndef _BSP_LED_H
#define	_BSP_LED_H


#include "IO_Operation.h"
#include "stm32f10x.h"
#include "_apollorobot.h"

#define LED1_PIN   GPIO_Pin_4
#define LED2_PIN   GPIO_Pin_5
#define LED3_PIN   GPIO_Pin_6
#define LED4_PIN   GPIO_Pin_7

#define LED_GPIO   GPIOC

 /* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(LED_GPIO,LED1_PIN)
#define LED1_OFF		  digitalHi(LED_GPIO,LED1_PIN)
#define LED1_ON			  digitalLo(LED_GPIO,LED1_PIN)

#define LED2_TOGGLE		digitalToggle(LED_GPIO,LED2_PIN)
#define LED2_OFF		  digitalHi(LED_GPIO,LED2_PIN)
#define LED2_ON			  digitalLo(LED_GPIO,LED2_PIN)

#define LED3_TOGGLE		digitalToggle(LED_GPIO,LED3_PIN)
#define LED3_OFF		  digitalHi(LED_GPIO,LED3_PIN)
#define LED3_ON			  digitalLo(LED_GPIO,LED3_PIN)

#define LED4_TOGGLE   digitalToggle(LED_GPIO,LED4_PIN)
#define LED4_OFF      digitalHi(LED_GPIO,LED4_PIN)
#define LED4_ON       digitalLo(LED_GPIO,LED4_PIN)
						
					
					
extern _Error LED_Config (_Led *led);
extern _Error LED_Mannul(u8 id, u8 sw);
#endif /* __LED_H */
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

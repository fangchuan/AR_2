#ifndef  _BSP_DIGITALSENSOR_H
#define  _BSP_DIGITALSENSOR_H

#include "stm32f10x.h"

//数字传感器的IO引脚分配
#define DigitalSensorPort    GPIOC
#define DigitalSensorPortClk RCC_APB2Periph_GPIOC
#define DigitalSensor_1      GPIO_Pin_4
#define DigitalSensor_2      GPIO_Pin_5
#define DigitalSensor_3      GPIO_Pin_6
#define DigitalSensor_4      GPIO_Pin_7

//
extern void DigitalSensor_Init(void);

#endif /*_BSP_DIGITALSENSOR_H*/

/* --------------------------------------end of file--------------------------------------- */
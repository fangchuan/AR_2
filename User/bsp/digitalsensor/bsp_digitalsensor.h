#ifndef  _BSP_DIGITALSENSOR_H
#define  _BSP_DIGITALSENSOR_H

#include "stm32f10x.h"
#include "_apollorobot.h"

//数字传感器的IO引脚分配
#define DigitalSensorPort    GPIOC
#define DigitalSensorPortClk RCC_APB2Periph_GPIOC
#define DigitalSensor_1      GPIO_Pin_4
#define DigitalSensor_2      GPIO_Pin_5
#define DigitalSensor_3      GPIO_Pin_6
#define DigitalSensor_4      GPIO_Pin_7

//侦测各端口数字传感器有无信号
#define Detect_DS1           GPIO_ReadInputDataBit(DigitalSensorPort, DigitalSensor_1)
#define Detect_DS2					 GPIO_ReadInputDataBit(DigitalSensorPort, DigitalSensor_2)
#define Detect_DS3					 GPIO_ReadInputDataBit(DigitalSensorPort, DigitalSensor_3)
#define Detect_DS4           GPIO_ReadInputDataBit(DigitalSensorPort, DigitalSensor_4)
//
extern void DigitalSensor_Init(void);
extern _Error DS_Config(_Port *port);
#endif /*_BSP_DIGITALSENSOR_H*/

/* --------------------------------------end of file--------------------------------------- */
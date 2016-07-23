/*
*********************************************************************************************************
*
*	模块名称 : ADC驱动模块
*	文件名称 : bsp_adc.h
*	版    本 : V1.0
*	说    明 : 内部ADC初始化包括温度计
*						 四个ADC通道采集模拟传感器的值
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef  _BSP_ADC_H
#define  _BSP_ADC_H

#include "stm32f10x.h"

//每个模拟传感器对应的ADC通道
#define  ANOLOG_Sensor_1   10
#define  ANOLOG_Sensor_2   11
#define  ANOLOG_Sensor_3   12
#define  ANOLOG_Sensor_4   13
#define  ADC_CH_TEMP       16


extern void adc_Init(void);
extern float Get_adc(u8 ch) ;
extern u16 Get_Adc_Average(u8 ch,u8 times);
extern int Get_Temp(void);

#endif /*_BSP_ADC_H*/
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

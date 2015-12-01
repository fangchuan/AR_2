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
extern u16 Get_adc(u8 ch);
extern u16 Get_Adc_Average(u8 ch,u8 times);
extern int Get_Temp(void);

#endif /*_BSP_ADC_H*/

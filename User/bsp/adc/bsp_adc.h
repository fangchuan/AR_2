/*
*********************************************************************************************************
*
*	ģ������ : ADC����ģ��
*	�ļ����� : bsp_adc.h
*	��    �� : V1.0
*	˵    �� : �ڲ�ADC��ʼ�������¶ȼ�
*						 �ĸ�ADCͨ���ɼ�ģ�⴫������ֵ
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-03-01 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef  _BSP_ADC_H
#define  _BSP_ADC_H

#include "stm32f10x.h"

//ÿ��ģ�⴫������Ӧ��ADCͨ��
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
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

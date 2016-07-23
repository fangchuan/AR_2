/*
*********************************************************************************************************
*
*	模块名称 : 数字传感器驱动模块
*	文件名称 : bsp_digitalsensor.c
*	版    本 : V1.0
*	说    明 : 数字传感器GPIO初始化
*						 数字传感器配置函数
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#include "bsp_digitalsensor.h"
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************************************************
*	函 数 名: DigitalSensor_Init
*	功能说明: 初始化数字传感器所用的GPIOC
*						数字传感器1--C4
*						数字传感器2--C5
*						数字传感器3--C6
*						数字传感器4--C7
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void DigitalSensor_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/* config GPIOC clock */
		RCC_APB2PeriphClockCmd(DigitalSensorPortClk, ENABLE);
		
		/*  GPIOC config */
		GPIO_InitStructure.GPIO_Pin = DigitalSensor_1 | DigitalSensor_2 | DigitalSensor_3 | DigitalSensor_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
		GPIO_Init(DigitalSensorPort, &GPIO_InitStructure);
	
}

/*********************************************************************************************************
*	函 数 名: DS_Config
*	功能说明: 将对应引脚配置为上拉输入模式
*	形    参：port
*	返 回 值: 
*********************************************************************************************************
*/

_Error DS_Config(_Port *port)
{
	     if(port->id < 1 || port->id > 4)
				 return ERROR_ID;
			 else{
				   uint8_t  Pin;
					 GPIO_InitTypeDef GPIO_InitStructure;
				 
					 switch(port->id)
					 {
						 case PORT_1:
							    Pin = DigitalSensor_1;
//									GPIOC->CRL &= 0xfff8ffff;
							 break;
						 case PORT_2:
							    Pin = DigitalSensor_2;
//									GPIOC->CRL &= 0xff8fffff;
							 break;
						 case PORT_3:
							    Pin = DigitalSensor_3;
//									GPIOC->CRL &= 0xf8ffffff;
							 break;
						 case PORT_4:
							    Pin = DigitalSensor_4;
//									GPIOC->CRL &= 0x8fffffff;
							 break;
					 }
					  
						GPIO_InitStructure.GPIO_Pin = Pin;
						GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
						GPIO_Init(DigitalSensorPort, &GPIO_InitStructure);
					 					 
					 return NO_ERROR;
			}
}
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

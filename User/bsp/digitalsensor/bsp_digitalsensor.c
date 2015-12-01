#include "bsp_digitalsensor.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//初始化数字传感器所用的GPIOC
//数字传感器1--C4
//数字传感器2--C5
//数字传感器3--C6
//数字传感器4--C7
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



/* --------------------------------------end of file--------------------------------------- */
#include "bsp_digitalsensor.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//��ʼ�����ִ��������õ�GPIOC
//���ִ�����1--C4
//���ִ�����2--C5
//���ִ�����3--C6
//���ִ�����4--C7
void DigitalSensor_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/* config GPIOC clock */
		RCC_APB2PeriphClockCmd(DigitalSensorPortClk, ENABLE);
		
		/*  GPIOC config */
		GPIO_InitStructure.GPIO_Pin = DigitalSensor_1 | DigitalSensor_2 | DigitalSensor_3 | DigitalSensor_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
		GPIO_Init(DigitalSensorPort, &GPIO_InitStructure);
	
}



/* --------------------------------------end of file--------------------------------------- */
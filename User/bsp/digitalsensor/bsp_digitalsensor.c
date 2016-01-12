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

//
//将对应引脚配置为上拉输入模式
//
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
/* --------------------------------------end of file--------------------------------------- */
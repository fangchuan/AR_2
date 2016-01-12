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

//
//����Ӧ��������Ϊ��������ģʽ
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
						GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
						GPIO_Init(DigitalSensorPort, &GPIO_InitStructure);
					 					 
					 return NO_ERROR;
			}
}
/* --------------------------------------end of file--------------------------------------- */
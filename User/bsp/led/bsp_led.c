  
#include "bsp_led.h"   

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern _Port port_1;
extern _Port port_2;
extern _Port port_3;
extern _Port port_4;
					
 /**
  * @brief  ��ʼ������LED��IO  ,���ɼĴ����ķ�ʽ��
  * @param  ��
  * @retval ��
  */
_Error LED_Config (_Led *led)
{		
	  if(led->id < 1 || led->id > 4)
			return ERROR_ID;
		else{
		uint8_t Pin;
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����GPIOC������ʱ��*/
//		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIOC����*/	
		switch(led->id)
		{
			case LED1:
				Pin = LED1_PIN;
//				GPIOC->CRL &= 0xfff3ffff;
			  port_1.dir = PORT_OUT;
				break;
			case LED2:
				Pin = LED2_PIN;
//				GPIOC->CRL &= 0xff3fffff;
			  port_2.dir = PORT_OUT;
				break;
			case LED3:
				Pin = LED3_PIN;
//				GPIOC->CRL &= 0xf3ffffff;
			  port_3.dir = PORT_OUT;
				break;
			case LED4:
				Pin = LED4_PIN;
//				GPIOC->CRL &= 0x3fffffff;
			  port_4.dir = PORT_OUT;
				break;
		}

		GPIO_InitStructure.GPIO_Pin  = Pin;
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOC*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		
		if(led->status)
			GPIO_SetBits(GPIOC,Pin);
//			digitalHi(GPIOC,Pin);
		else
			GPIO_ResetBits(GPIOC,Pin);
//			digitalLo(GPIOC,Pin);
		
		return NO_ERROR;
	}

}

_Error LED_Mannul(u8 id, u8 sw)
{
	  if(id < 1 || id > 4)
			return ERROR_ID;
		
		else{
				uint8_t Pin;
				/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
				GPIO_InitTypeDef GPIO_InitStructure;
				switch( id )
				{
					case LED1:
						Pin = LED1_PIN;
		//				GPIOC->CRL &= 0xfff3ffff;
					  port_1.dir = PORT_OUT;
						break;
					case LED2:
						Pin = LED2_PIN;
		//				GPIOC->CRL &= 0xff3fffff;
					  port_2.dir = PORT_OUT;
						break;
					case LED3:
						Pin = LED3_PIN;
		//				GPIOC->CRL &= 0xf3ffffff;
					  port_3.dir = PORT_OUT;
						break;
					case LED4:
						Pin = LED4_PIN;
		//				GPIOC->CRL &= 0x3fffffff;
					  port_4.dir = PORT_OUT;
						break;
				}

				GPIO_InitStructure.GPIO_Pin  = Pin;
				/*��������ģʽΪͨ���������*/
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

				/*������������Ϊ50MHz */   
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

				/*���ÿ⺯������ʼ��GPIOC*/
				GPIO_Init(GPIOC, &GPIO_InitStructure);	
				
				if(sw)
					GPIO_SetBits(GPIOC,Pin);
		//			digitalHi(GPIOC,Pin);
				else
					GPIO_ResetBits(GPIOC,Pin);
		//			digitalLo(GPIOC,Pin);
				
				return NO_ERROR;
	}
}
/*********************************************END OF FILE**********************/

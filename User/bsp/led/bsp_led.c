  
#include "bsp_led.h"   


					
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
				break;
			case LED2:
				Pin = LED2_PIN;
				break;
			case LED3:
				Pin = LED3_PIN;
				break;
			case LED4:
				Pin = LED4_PIN;
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
		else
			GPIO_ResetBits(GPIOC,Pin);
		
		return NO_ERROR;
	}

}
/*********************************************END OF FILE**********************/

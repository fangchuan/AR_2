  
#include "bsp_led.h"   


					
 /**
  * @brief  初始化控制LED的IO  ,换成寄存器的方式？
  * @param  无
  * @retval 无
  */
_Error LED_Config (_Led *led)
{		
	  if(led->id < 1 || led->id > 4)
			return ERROR_ID;
		else{
		uint8_t Pin;
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOC的外设时钟*/
//		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 

		/*选择要控制的GPIOC引脚*/	
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
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOC*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		
		if(led->status)
			GPIO_SetBits(GPIOC,Pin);
		else
			GPIO_ResetBits(GPIOC,Pin);
		
		return NO_ERROR;
	}

}
/*********************************************END OF FILE**********************/

/*
*********************************************************************************************************
*
*	模块名称 : led bsp 模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/  
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
					
/*
*********************************************************************************************************
*	函 数 名: LED_Config
*	功能说明: 初始化控制LED的IO  ,换成寄存器的方式？
*	形    参：led结构体指针
*	返 回 值: 
*********************************************************************************************************
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
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOC*/
		GPIO_Init(LED_GPIO, &GPIO_InitStructure);	
		
		if(led->status)
			GPIO_SetBits(LED_GPIO,Pin);
//			digitalHi(GPIOC,Pin);
		else
			GPIO_ResetBits(LED_GPIO,Pin);
//			digitalLo(GPIOC,Pin);
		
		return NO_ERROR;
	}

}
/*
*********************************************************************************************************
*	函 数 名: LED_Mannul
*	功能说明: 控制某一端口LED
*	形    参： 
*	返 回 值: 
*********************************************************************************************************
*/
_Error LED_Mannul(u8 id, u8 sw)
{
	  if(id < 1 || id > 4)
			return ERROR_ID;
		
		else{
				uint8_t Pin;
				/*定义一个GPIO_InitTypeDef类型的结构体*/
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
				/*设置引脚模式为通用推挽输出*/
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

				/*设置引脚速率为50MHz */   
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

				/*调用库函数，初始化GPIOC*/
				GPIO_Init(LED_GPIO, &GPIO_InitStructure);	
				
				if(sw)
					GPIO_SetBits(LED_GPIO,Pin);
		//			digitalHi(GPIOC,Pin);
				else
					GPIO_ResetBits(LED_GPIO,Pin);
		//			digitalLo(GPIOC,Pin);
				
				return NO_ERROR;
	}
}
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

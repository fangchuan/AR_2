/*
*********************************************************************************************************
*
*	模块名称 : 模拟spi引脚配置模块
*	文件名称 : bsp_gpio_spi.c
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

#include "bsp_gpio_spi.h"

#if 0			//modify by fire 不需要使用触摸中断
/*
*********************************************************************************************************
*	函 数 名: TP_INT_NVIC_Config
*	功能说明: XPT 2046 IRQ 引脚配置
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
static void TP_INT_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

    /*使能EXTI9_5 中断 */		      
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn  ;    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
}


static void TP_INT_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC */
	TP_INT_NVIC_Config();

	/* EXTI line gpio config */	
  GPIO_InitStructure.GPIO_Pin = TP_INT_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
  GPIO_Init(TP_INT_PORT, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource9);  // PF9 
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 	
}

#endif
/*
*********************************************************************************************************
*	函 数 名: GPIO_SPI_Config
*	功能说明: 模拟SPI的GPIO配置 当SPI的4根信号线换成其他的IO时候，只需要修改该函数头文件的宏定义即可
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void GPIO_SPI_Config(void) 
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* 开启GPIO时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

  /* 模拟SPI GPIO初始化 */          
  GPIO_InitStructure.GPIO_Pin=SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(SPI_CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      
  GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
  GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure); 
   
	GPIO_InitStructure.GPIO_Pin = TP_INT_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(TP_INT_PORT, &GPIO_InitStructure); 
	 
  /* 拉低片选，选择XPT2046 */
  GPIO_ResetBits(SPI_CS_PORT,SPI_CS_PIN);
 
	#if 0 //modify by fire 不需要使用触摸中断 
  /* XPT2046 中断IO配置 */
  TP_INT_GPIO_Config();
	#endif
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

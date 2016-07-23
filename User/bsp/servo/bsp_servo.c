/*
*********************************************************************************************************
*
*	模块名称 : 舵机驱动模块
*	文件名称 : bsp_servo.c
*	版    本 : V1.0
*	说    明 : 初始化舵机驱动的定时器、GPIO引脚
*						 舵机配置函数
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/  
#include "bsp_servo.h" 


#define  MIN_PWM    65
#define  AVR_PWM   150
#define  MAX_PWM   230
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: TIM2_GPIO_Config
*	功能说明: TIM2  PWM输出的引脚初始化
*					  TIM2 CH1--PA0
*						TIM2 CH2--PA1
*						TIM2 CH3--PA2
*						TIM2 CH4--PA3	
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/	
static void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM3CLK 为 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(SERVO_PORT_CLK, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  SERVO_1_Pin | SERVO_2_Pin | SERVO_3_Pin | SERVO_4_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SERVO_PORT, &GPIO_InitStructure);
}

 
/*
*********************************************************************************************************
*	函 数 名: TIM2_GPIO_Config
*	功能说明: 配置TIM2输出的PWM信号的模式，如周期、极性、占空比
* 					TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
*           TIMx_CCR(电平发生变化)
*						信号周期=(TIMx_ARR +1 ) * 时钟周期
* 					占空比=TIMx_CCR/(TIMx_ARR +1)
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/	
static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
 	u16 CCR1_Val = MIN_PWM;        
 	u16 CCR2_Val = MIN_PWM;
 	u16 CCR3_Val = MIN_PWM;
 	u16 CCR4_Val = MIN_PWM;

/* ----------------------------------------------------------------------- 
    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR+1)* 100% = 2.5%
    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR+1)* 100% = 2.5%
    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR+1)* 100% = 2.5%
    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR+1)* 100% = 2.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 1999;       //当定时器从0计数到1999，即为2000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 719;	    //设置预分频：72M/719 = 10^5
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);	  //使能通道2
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	//使能通道4
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能自动重装
	
  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // 使能TIM2重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //使能定时器2
}


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void SERVO_PWM_Init(void)
{
	TIM2_GPIO_Config();
	TIM2_Mode_Config();	
}
/*
*********************************************************************************************************
*	函 数 名: SERVO_Config
*	功能说明: 根据编程指令配置舵机参数
*					  舵机转角：60~260
*	形    参：servo
*	返 回 值: 
*********************************************************************************************************
*/
_Error  SERVO_Config(_Servo *servo)
{
			u16  degree = servo->degree + MIN_PWM;
	    if(degree > MAX_PWM)
				 degree = MAX_PWM;
	
			if(servo->id <1 || servo->id > 4)
				return ERROR_ID;
			
			switch(servo->id)
			{
				case SERVO1:
					   SERVO1_OUT = degree ; 
					break;
				case SERVO2:
					   SERVO2_OUT = degree ;
					break;
				case SERVO3:
					   SERVO3_OUT = degree ;
					break;
				case SERVO4:
					   SERVO4_OUT = degree ;
					break;
			}
			return NO_ERROR;
}

/*
*********************************************************************************************************
*	函 数 名: SERVO_Manual
*	功能说明: 手动配置舵机参数
*					  舵机转角：60~260
*	形    参：id：舵机编号        degree:转角
*	返 回 值: 
*********************************************************************************************************
*/
_Error   SERVO_Manual (uint8_t id, uint8_t degree)
{
	     u16   deg = degree + MIN_PWM;
       if(deg > MAX_PWM)
          deg = MAX_PWM;				 
	
	     if(id < 1 || id > 4)
				 return ERROR_ID;
			 
			 switch(id)
			 {
				 case SERVO1:
					    SERVO1_OUT = deg;
					 break;
				 case SERVO2:
					    SERVO2_OUT = deg;
					 break;
				 case SERVO3:
					    SERVO3_OUT = deg;
					 break;
				 case SERVO4:
					    SERVO4_OUT = deg;
					 break;
			 }
			 return NO_ERROR;
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

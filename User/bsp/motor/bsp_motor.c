/*
*********************************************************************************************************
*
*	模块名称 : 电机驱动函数模块
*	文件名称 : bsp_motor.h
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-07-21 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/  
#include "bsp_motor.h" 

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern _Display   display;
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************************************************
*	函 数 名: TIM_GPIO_Config
*	功能说明: 配置TIM4复用输出PWM时用到的I/O
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
//TIM3 CH3--PB0   MOTOR11_OUT
//TIM3 CH4--PB1   MOTOR12_OUT
//TIM4 CH3--PB8   MOTOR21_OUT
//TIM4 CH4--PB9	  MOTOR22_OUT
static void TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM4 、TIM3CLK 为 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOB clock enable */
  RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

  /*GPIOB Configuration: */
  GPIO_InitStructure.GPIO_Pin =  MOTOR_11_Pin | MOTOR_12_Pin | MOTOR_21_Pin | MOTOR_22_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);

}

 
/*********************************************************************************************************
*	函 数 名: TIM_Mode_Config
*	功能说明: 配置TIM4输出的PWM信号的模式，如周期、极性、占空比
*	形    参：TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
*           TIMx_CCR(电平发生变化)
* 					信号周期=(TIMx_ARR +1 ) * 时钟周期   
* 					占空比=TIMx_CCR/(TIMx_ARR +1)
*	返 回 值: 
*********************************************************************************************************
*/
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
 	u16 CCR1_Val = DEFAULT_MIN;     //信号周期为2K,初始占空比为5%   
// 	u16 CCR2_Val = 100;
 	u16 CCR3_Val = DEFAULT_MIN;
 	u16 CCR4_Val = DEFAULT_MIN;

/* ----------------------------------------------------------------------- 
    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR+1)* 100% = 5%
    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR+1)* 100% = 5%
    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR+1)* 100% = 5%
    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR+1)* 100% = 5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 1999;       //当定时器从0计数到1999，即为2000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //设置预分频：72M/72 = 10^6
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /*蜂鸣器专用PWM*/
  /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装

//  /* PWM1 Mode configuration: Channel2 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //使能通道2
//  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	//使能通道4
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//使能通道4
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能自动重装
	
  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // 使能TIM4重载寄存器ARR
	TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM4重载寄存器ARR

  /* TIM3、4 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3、4
	TIM_Cmd(TIM4, ENABLE); 
}


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************************************************
*	函 数 名: MOTOR_Init
*	功能说明: 初始化定时器PWM输出
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void MOTOR_Init(void)
{
	TIM_GPIO_Config();
	TIM_Mode_Config();	
}
/*********************************************************************************************************
*	函 数 名: MOTOR_1_Forward
*	功能说明: 电机1正转
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void MOTOR_1_Forward(void)
{
	   MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = 0;
}
/*********************************************************************************************************
*	函 数 名: MOTOR_1_Reverse
*	功能说明: 电机1反转
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void MOTOR_1_Reverse(void)
{
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = DEFAULT_AVR;
}
/*********************************************************************************************************
*	函 数 名: MOTOR_1_Stop
*	功能说明: 电机1停止
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void MOTOR_1_Stop(void)
{
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = 0;
}
/*********************************************************************************************************
*	函 数 名: MOTOR_2_Forward
*	功能说明: 电机2正转
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void MOTOR_2_Forward(void)
{
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = 0;
}
/*********************************************************************************************************
*	函 数 名: MOTOR_2_Reverse
*	功能说明: 电机2反转
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void MOTOR_2_Reverse(void)
{
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = DEFAULT_AVR;
}
/*********************************************************************************************************
*	函 数 名: MOTOR_2_Stop
*	功能说明: 电机2停止
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void MOTOR_2_Stop(void)
{
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = 0;
}
/*********************************************************************************************************
*	函 数 名: MOTOR_Config
*	功能说明: 电机配置，根据编程配置电机参数
*					 用户输入的转速是0~100,对应到PWM值为0~2000
*	形    参：电机结构体指针
*	返 回 值: 
*********************************************************************************************************
*/
_Error MOTOR_Config(_Motor *motor)
{
	  u16    speed_1 = 0;
	  u16    speed_2 = 0;  

		if(motor->id < 1 || motor->id > 2)
				return ERROR_ID;
			if(motor->id == MOTOR1)
			{
				  if(motor->direction == FORWARD)//电机前进
					{
							speed_1 = motor->speed * MOTOR_SPEED_P;
							speed_2 = 0;
					}
					else
					{
						  speed_1 = 0;
							speed_2 = motor->speed * MOTOR_SPEED_P;
					}
					
					if(speed_1 > DEFAULT_MAX) speed_1 = DEFAULT_MAX;
					if(speed_2 > DEFAULT_MAX) speed_2 = DEFAULT_MAX;
					
					MOTOR_11_OUT = speed_1;
					MOTOR_12_OUT = speed_2;
			}
			else   //电机后退
			{
				  if(motor->direction == FORWARD)
					{
						 speed_1 = motor->speed * MOTOR_SPEED_P;
						 speed_2 = 0;
					}
					else
					{
						 speed_1 = 0;
						 speed_2 = motor->speed * MOTOR_SPEED_P;
					}
					
					if(speed_1 > DEFAULT_MAX) speed_1 = DEFAULT_MAX;
					if(speed_2 > DEFAULT_MAX) speed_2 = DEFAULT_MAX;
					
					MOTOR_21_OUT = speed_1;
					MOTOR_22_OUT = speed_2;
			}
			return NO_ERROR;
}
/*********************************************************************************************************
*	函 数 名: Car_Forward
*	功能说明: 小车前进
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void Car_Forward(void)
{
	   display = DISP_DIRECTION;
	
		 MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = DEFAULT_MIN;
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = DEFAULT_MIN;
}
/*********************************************************************************************************
*	函 数 名: Car_Backward
*	功能说明: 小车后退
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void Car_Backward(void)
{
	   display = DISP_DIRECTION;  
	
	   MOTOR_11_OUT = DEFAULT_MIN;
	   MOTOR_12_OUT = DEFAULT_AVR;
	   MOTOR_21_OUT = DEFAULT_MIN;
	   MOTOR_22_OUT = DEFAULT_AVR;
}
/*********************************************************************************************************
*	函 数 名: Car_Left
*	功能说明: 小车左转
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void Car_Left(void)
{
	   display = DISP_DIRECTION;
	
		 MOTOR_11_OUT = DEFAULT_MIN;
	   MOTOR_12_OUT = DEFAULT_AVR;
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = 0;

}
/*********************************************************************************************************
*	函 数 名: Car_Right
*	功能说明: 小车右转
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void Car_Right(void)
{
	   display = DISP_DIRECTION;
	   
		 MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = 0;
	   MOTOR_21_OUT = DEFAULT_MIN;
	   MOTOR_22_OUT = DEFAULT_AVR;
}
/*********************************************************************************************************
*	函 数 名: Car_Stop
*	功能说明: 小车停止
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void Car_Stop(void)
{
	   display = DISP_DIRECTION;
	   
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = 0;
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = 0;
}
/*********************************************************************************************************
*	函 数 名: CAR_Config
*	功能说明: 小车配置函数
*	形    参：小车结构体指针
*	返 回 值: 
*********************************************************************************************************
*/
_Error  CAR_Config(_Car *car)
{
	     if(!car->direction)
				 return ERROR_DIR;
			 
			 
			 switch(car->direction )
			 {
				 case FORWARD:
								 MOTOR_11_OUT += car->speed_step ;
								 MOTOR_12_OUT  = DEFAULT_MIN;
								 MOTOR_21_OUT += car->speed_step ;
							   MOTOR_22_OUT  = DEFAULT_MIN;
								//constrain pwm out 
				         if(MOTOR_11_OUT > DEFAULT_MAX)
									  MOTOR_11_OUT = DEFAULT_MAX;
								 if(MOTOR_11_OUT < DEFAULT_MIN)
									  MOTOR_11_OUT = DEFAULT_MIN;
								 
								 if(MOTOR_21_OUT > DEFAULT_MAX)
									  MOTOR_21_OUT = DEFAULT_MAX;
								 if(MOTOR_21_OUT < DEFAULT_MIN)
									  MOTOR_21_OUT = DEFAULT_MIN;
					 break;
				 case BACKWARD:
					       MOTOR_11_OUT = DEFAULT_MIN;
								 MOTOR_12_OUT += car->speed_step ;
								 MOTOR_21_OUT = DEFAULT_MIN;
								 MOTOR_22_OUT += car->speed_step ;
								 //constrain pwm out 
				 				 if(MOTOR_12_OUT > DEFAULT_MAX)
									  MOTOR_12_OUT = DEFAULT_MAX;
								 if(MOTOR_12_OUT < DEFAULT_MIN)
									  MOTOR_12_OUT = DEFAULT_MIN;
								 
								 if(MOTOR_22_OUT > DEFAULT_MAX)
									  MOTOR_22_OUT = DEFAULT_MAX;
								 if(MOTOR_22_OUT < DEFAULT_MIN)
									  MOTOR_22_OUT = DEFAULT_MIN;
					 break;
				 case LEFT:
								 MOTOR_11_OUT = DEFAULT_MIN;
								 MOTOR_12_OUT = DEFAULT_AVR ;
				         MOTOR_21_OUT += car->speed_step ;
								 MOTOR_22_OUT = 0;
								 //constrain pwm out 
				 				 if(MOTOR_21_OUT > DEFAULT_MAX)
									  MOTOR_21_OUT = DEFAULT_MAX;
								 if(MOTOR_21_OUT < DEFAULT_MIN)
									  MOTOR_21_OUT = DEFAULT_MIN;
					 break;
				 case RIGHT:
								 MOTOR_11_OUT += car->speed_step;
								 MOTOR_12_OUT =  0;
								 MOTOR_21_OUT = DEFAULT_MIN;
								 MOTOR_22_OUT = DEFAULT_AVR ;
								 //constrain pwm out 
				 				 if(MOTOR_11_OUT > DEFAULT_MAX)
									  MOTOR_11_OUT = DEFAULT_MAX;
								 if(MOTOR_11_OUT < DEFAULT_MIN)
									  MOTOR_11_OUT = DEFAULT_MIN;
					 break;
				 case STOP :
					    Car_Stop();
							car->speed_step = 0;           // 小车停止时是不允许加减速的
					 break;
			 }
//			 if(MOTOR_11_OUT > DEFAULT_MAX)
//				  MOTOR_11_OUT = DEFAULT_MAX;
//			 if(MOTOR_11_OUT < DEFAULT_MIN)
//				  MOTOR_11_OUT = DEFAULT_MIN;
//			 
//			 if(MOTOR_12_OUT > DEFAULT_MAX)
//				  MOTOR_12_OUT = DEFAULT_MAX;
//			 if(MOTOR_12_OUT < DEFAULT_MIN)
//				  MOTOR_12_OUT = DEFAULT_MIN;
//			 
//			 if(MOTOR_21_OUT > DEFAULT_MAX)
//				  MOTOR_21_OUT = DEFAULT_MAX;
//			 if(MOTOR_21_OUT < DEFAULT_MIN)
//				  MOTOR_21_OUT = DEFAULT_MIN;
//			 
//			 if(MOTOR_22_OUT > DEFAULT_MAX)
//				  MOTOR_22_OUT = DEFAULT_MAX;
//			 if(MOTOR_22_OUT < DEFAULT_MIN)
//				  MOTOR_22_OUT = DEFAULT_MIN;
			 
			 return NO_ERROR;
}
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

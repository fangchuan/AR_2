  
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
 /**
  * @brief  配置TIM4复用输出PWM时用到的I/O
  */
	
//TIM4 CH1--PB6 
//TIM4 CH2--PB7 
//TIM4 CH3--PB8 
//TIM4 CH4--PB9	
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

/**
  * @brief  配置TIM4输出的PWM信号的模式，如周期、极性、占空比
  * @param  无
  * @retval 无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期   
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
 	u16 CCR1_Val = 500;     //信号周期为2K,初始占空比为25%   
// 	u16 CCR2_Val = 500;
 	u16 CCR3_Val = 500;
 	u16 CCR4_Val = 500;

/* ----------------------------------------------------------------------- 
    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR+1)* 100% = 25%
    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR+1)* 100% = 25%
    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR+1)* 100% = 25%
    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR+1)* 100% = 25%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 1999;       //当定时器从0计数到499，即为500次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //设置预分频：72M/72 = 10^6
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

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

////初始化L293D芯片的ENBALE引脚
//static void  IC_Init(void)
//{
//		GPIO_InitTypeDef GPIO_InitStructure;

//		/* GPIOA clock enable */
//		RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

//		/*GPIOB 0\1 push-pull */
//		GPIO_InitStructure.GPIO_Pin =  MOTOR_Enable_1 | MOTOR_Enable_2;	
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // 推挽输出
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
//	
////		MOTOR_1_Enable();
////		MOTOR_2_Enable();
//}
 

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//初始化定时器PWM输出
void MOTOR_Init(void)
{
	TIM_GPIO_Config();
	TIM_Mode_Config();	
//	IC_Init();
}
////使能电机1的控制端
//void MOTOR_1_Enable(void)
//{
//		 digitalHi(MOTOR_PORT, MOTOR_Enable_1);
//}
////禁用电机1的控制端
//void MOTOR_1_Disable(void)
//{
//		 digitalLo(MOTOR_PORT, MOTOR_Enable_1);
//}
////是能电机2的控制端
//void MOTOR_2_Enable(void)
//{
//		 digitalHi(MOTOR_PORT, MOTOR_Enable_2);
//}
////禁用电机2的控制端
//void MOTOR_2_Disable(void)
//{
//		 digitalLo(MOTOR_PORT, MOTOR_Enable_2);
//}

//电机1正转
void MOTOR_1_Forward(void)
{
	   MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = 0;
}
//电机1反转
void MOTOR_1_Reverse(void)
{
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = DEFAULT_AVR;
}
//电机1停止
void MOTOR_1_Stop(void)
{
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = 0;
}

//电机2正转
void MOTOR_2_Forward(void)
{
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = 0;
}
//电机2反转
void MOTOR_2_Reverse(void)
{
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = DEFAULT_AVR;
}
//电机2停止
void MOTOR_2_Stop(void)
{
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = 0;
}

//电机配置，根据编程配置电机参数
_Error MOTOR_Config(_Motor *motor)
{
	    if(motor->id < 1 || motor->id > 2)
				return ERROR_ID;
			if(motor->direction == FORWARD)//电机前进
			{
				  if(motor->id == MOTOR1)
					{
							MOTOR_11_OUT = motor->speed;
							MOTOR_12_OUT = 0;
					}
					else
					{
						  MOTOR_21_OUT = motor->speed;
							MOTOR_22_OUT = 0;
					}
			}
			else   //电机后退
			{
				  if(motor->id == MOTOR1)
					{
						 MOTOR_11_OUT = 0;
						 MOTOR_12_OUT = motor->speed;
					}
					else
					{
						 MOTOR_21_OUT = 0;
						 MOTOR_22_OUT = motor->speed;
					}
			}
			return NO_ERROR;
}

//小车前进
void Car_Forward(void)
{
	   display = DISP_DIRECTION;
	
		 MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = DEFAULT_MIN;
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = DEFAULT_MIN;
}
//小车后退
void Car_Backward(void)
{
	   display = DISP_DIRECTION;  
	
	   MOTOR_11_OUT = DEFAULT_MIN;
	   MOTOR_12_OUT = DEFAULT_AVR;
	   MOTOR_21_OUT = DEFAULT_MIN;
	   MOTOR_22_OUT = DEFAULT_AVR;
}
//小车左转
void Car_Left(void)
{
	   display = DISP_DIRECTION;
	
		 MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = DEFAULT_MIN;
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = 0;

}
//小车右转
void Car_Right(void)
{
	   display = DISP_DIRECTION;
	   
		 MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = 0;
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = DEFAULT_MIN;
}
//小车停止
void Car_Stop(void)
{
	   display = DISP_DIRECTION;
	   
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = 0;
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = 0;
}
//
//小车配置函数
//
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
								 MOTOR_11_OUT = DEFAULT_AVR;
								 MOTOR_12_OUT = DEFAULT_MIN ;
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
								 MOTOR_21_OUT = DEFAULT_AVR;
								 MOTOR_22_OUT = DEFAULT_MIN ;
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
/*********************************************END OF FILE**********************/

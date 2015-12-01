  
#include "bsp_servo.h" 

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
 /**
  * @brief  配置TIM2复用输出PWM时用到的I/O
  */
	
//TIM2 CH1--PA0 
//TIM2 CH2--PA1 
//TIM2 CH3--PA2 
//TIM2 CH4--PA3	
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

/**
  * @brief  配置TIM2输出的PWM信号的模式，如周期、极性、占空比
  * @param  无
  * @retval 无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
 	u16 CCR1_Val = 50;        
 	u16 CCR2_Val = 50;
 	u16 CCR3_Val = 50;
 	u16 CCR4_Val = 50;

/* ----------------------------------------------------------------------- 
    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR+1)* 100% = 2.5%
    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR+1)* 100% = 2.5%
    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR+1)* 100% = 2.5%
    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR+1)* 100% = 2.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 1999;       //当定时器从0计数到1999，即为2000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 719;	    //设置预分频：72M/720 = 10^5
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4 ;	//设置时钟分频系数
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

/*********************************************END OF FILE**********************/

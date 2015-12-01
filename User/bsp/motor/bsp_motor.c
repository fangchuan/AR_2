  
#include "bsp_motor.h" 

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
 /**
  * @brief  配置TIM2复用输出PWM时用到的I/O
  */
	
//TIM4 CH1--PB6 
//TIM4 CH2--PB7 
//TIM4 CH3--PB8 
//TIM4 CH4--PB9	
static void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM4CLK 为 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

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
static void TIM4_Mode_Config(void)
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
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //使能通道2
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	//使能通道4
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能自动重装
	
  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // 使能TIM2重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM4, ENABLE);                   //使能定时器2
}

//初始化L293D芯片的ENBALE引脚
static void  IC_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* GPIOA clock enable */
		RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

		/*GPIOB 0\1 push-pull */
		GPIO_InitStructure.GPIO_Pin =  MOTOR_Enable_1 | MOTOR_Enable_2;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // 推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//初始化定时器PWM输出
void MOTOR_Init(void)
{
	TIM4_GPIO_Config();
	TIM4_Mode_Config();	
	IC_Init();
}
//使能电机1的控制端
void MOTOR_1_Enable(void)
{
		 digitalHi(MOTOR_PORT, MOTOR_Enable_1);
}
//禁用电机1的控制端
void MOTOR_1_Disable(void)
{
		 digitalLo(MOTOR_PORT, MOTOR_Enable_1);
}
//是能电机2的控制端
void MOTOR_2_Enable(void)
{
		 digitalHi(MOTOR_PORT, MOTOR_Enable_2);
}
//禁用电机2的控制端
void MOTOR_2_Disable(void)
{
		 digitalLo(MOTOR_PORT, MOTOR_Enable_2);
}
/*********************************************END OF FILE**********************/

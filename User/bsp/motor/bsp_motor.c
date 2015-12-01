  
#include "bsp_motor.h" 

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
 /**
  * @brief  ����TIM2�������PWMʱ�õ���I/O
  */
	
//TIM4 CH1--PB6 
//TIM4 CH2--PB7 
//TIM4 CH3--PB8 
//TIM4 CH4--PB9	
static void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM4CLK Ϊ 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

  /* GPIOB clock enable */
  RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

  /*GPIOB Configuration: */
  GPIO_InitStructure.GPIO_Pin =  MOTOR_11_Pin | MOTOR_12_Pin | MOTOR_21_Pin | MOTOR_22_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
}

/**
  * @brief  ����TIM4�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
  * @param  ��
  * @retval ��
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
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
  TIM_TimeBaseStructure.TIM_Period = 1999;       //����ʱ����0������1999����Ϊ2000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 719;	    //����Ԥ��Ƶ��72M/720 = 10^5
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4 ;	//����ʱ�ӷ�Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //ʹ��ͨ��2
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	//ʹ��ͨ��4
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ
	
  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // ʹ��TIM2���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM4, ENABLE);                   //ʹ�ܶ�ʱ��2
}

//��ʼ��L293DоƬ��ENBALE����
static void  IC_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* GPIOA clock enable */
		RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

		/*GPIOB 0\1 push-pull */
		GPIO_InitStructure.GPIO_Pin =  MOTOR_Enable_1 | MOTOR_Enable_2;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // �������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//��ʼ����ʱ��PWM���
void MOTOR_Init(void)
{
	TIM4_GPIO_Config();
	TIM4_Mode_Config();	
	IC_Init();
}
//ʹ�ܵ��1�Ŀ��ƶ�
void MOTOR_1_Enable(void)
{
		 digitalHi(MOTOR_PORT, MOTOR_Enable_1);
}
//���õ��1�Ŀ��ƶ�
void MOTOR_1_Disable(void)
{
		 digitalLo(MOTOR_PORT, MOTOR_Enable_1);
}
//���ܵ��2�Ŀ��ƶ�
void MOTOR_2_Enable(void)
{
		 digitalHi(MOTOR_PORT, MOTOR_Enable_2);
}
//���õ��2�Ŀ��ƶ�
void MOTOR_2_Disable(void)
{
		 digitalLo(MOTOR_PORT, MOTOR_Enable_2);
}
/*********************************************END OF FILE**********************/

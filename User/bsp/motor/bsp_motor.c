/*
*********************************************************************************************************
*
*	ģ������ : �����������ģ��
*	�ļ����� : bsp_motor.h
*	��    �� : V1.0
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-07-21 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
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
*	�� �� ��: TIM_GPIO_Config
*	����˵��: ����TIM4�������PWMʱ�õ���I/O
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
//TIM3 CH3--PB0   MOTOR11_OUT
//TIM3 CH4--PB1   MOTOR12_OUT
//TIM4 CH3--PB8   MOTOR21_OUT
//TIM4 CH4--PB9	  MOTOR22_OUT
static void TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM4 ��TIM3CLK Ϊ 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOB clock enable */
  RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

  /*GPIOB Configuration: */
  GPIO_InitStructure.GPIO_Pin =  MOTOR_11_Pin | MOTOR_12_Pin | MOTOR_21_Pin | MOTOR_22_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);

}

 
/*********************************************************************************************************
*	�� �� ��: TIM_Mode_Config
*	����˵��: ����TIM4�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
*	��    �Σ�TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
*           TIMx_CCR(��ƽ�����仯)
* 					�ź�����=(TIMx_ARR +1 ) * ʱ������   
* 					ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
 	u16 CCR1_Val = DEFAULT_MIN;     //�ź�����Ϊ2K,��ʼռ�ձ�Ϊ5%   
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
  TIM_TimeBaseStructure.TIM_Period = 1999;       //����ʱ����0������1999����Ϊ2000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //����Ԥ��Ƶ��72M/72 = 10^6
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /*������ר��PWM*/
  /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ

//  /* PWM1 Mode configuration: Channel2 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //ʹ��ͨ��2
//  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	//ʹ��ͨ��4
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ���Զ���װ
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//ʹ��ͨ��4
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ���Զ���װ
	
  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // ʹ��TIM4���ؼĴ���ARR
	TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM4���ؼĴ���ARR

  /* TIM3��4 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3��4
	TIM_Cmd(TIM4, ENABLE); 
}


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************************************************
*	�� �� ��: MOTOR_Init
*	����˵��: ��ʼ����ʱ��PWM���
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MOTOR_Init(void)
{
	TIM_GPIO_Config();
	TIM_Mode_Config();	
}
/*********************************************************************************************************
*	�� �� ��: MOTOR_1_Forward
*	����˵��: ���1��ת
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MOTOR_1_Forward(void)
{
	   MOTOR_11_OUT = DEFAULT_AVR;
	   MOTOR_12_OUT = 0;
}
/*********************************************************************************************************
*	�� �� ��: MOTOR_1_Reverse
*	����˵��: ���1��ת
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MOTOR_1_Reverse(void)
{
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = DEFAULT_AVR;
}
/*********************************************************************************************************
*	�� �� ��: MOTOR_1_Stop
*	����˵��: ���1ֹͣ
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MOTOR_1_Stop(void)
{
	   MOTOR_11_OUT = 0;
	   MOTOR_12_OUT = 0;
}
/*********************************************************************************************************
*	�� �� ��: MOTOR_2_Forward
*	����˵��: ���2��ת
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MOTOR_2_Forward(void)
{
	   MOTOR_21_OUT = DEFAULT_AVR;
	   MOTOR_22_OUT = 0;
}
/*********************************************************************************************************
*	�� �� ��: MOTOR_2_Reverse
*	����˵��: ���2��ת
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MOTOR_2_Reverse(void)
{
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = DEFAULT_AVR;
}
/*********************************************************************************************************
*	�� �� ��: MOTOR_2_Stop
*	����˵��: ���2ֹͣ
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MOTOR_2_Stop(void)
{
	   MOTOR_21_OUT = 0;
	   MOTOR_22_OUT = 0;
}
/*********************************************************************************************************
*	�� �� ��: MOTOR_Config
*	����˵��: ������ã����ݱ�����õ������
*					 �û������ת����0~100,��Ӧ��PWMֵΪ0~2000
*	��    �Σ�����ṹ��ָ��
*	�� �� ֵ: 
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
				  if(motor->direction == FORWARD)//���ǰ��
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
			else   //�������
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
*	�� �� ��: Car_Forward
*	����˵��: С��ǰ��
*	��    �Σ�
*	�� �� ֵ: 
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
*	�� �� ��: Car_Backward
*	����˵��: С������
*	��    �Σ�
*	�� �� ֵ: 
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
*	�� �� ��: Car_Left
*	����˵��: С����ת
*	��    �Σ�
*	�� �� ֵ: 
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
*	�� �� ��: Car_Right
*	����˵��: С����ת
*	��    �Σ�
*	�� �� ֵ: 
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
*	�� �� ��: Car_Stop
*	����˵��: С��ֹͣ
*	��    �Σ�
*	�� �� ֵ: 
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
*	�� �� ��: CAR_Config
*	����˵��: С�����ú���
*	��    �Σ�С���ṹ��ָ��
*	�� �� ֵ: 
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
							car->speed_step = 0;           // С��ֹͣʱ�ǲ�����Ӽ��ٵ�
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
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

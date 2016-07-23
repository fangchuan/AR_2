/*
*********************************************************************************************************
*
*	ģ������ : �������ģ��
*	�ļ����� : bsp_servo.c
*	��    �� : V1.0
*	˵    �� : ��ʼ����������Ķ�ʱ����GPIO����
*						 ������ú���
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-03-01 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
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
*	�� �� ��: TIM2_GPIO_Config
*	����˵��: TIM2  PWM��������ų�ʼ��
*					  TIM2 CH1--PA0
*						TIM2 CH2--PA1
*						TIM2 CH3--PA2
*						TIM2 CH4--PA3	
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/	
static void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM3CLK Ϊ 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(SERVO_PORT_CLK, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  SERVO_1_Pin | SERVO_2_Pin | SERVO_3_Pin | SERVO_4_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SERVO_PORT, &GPIO_InitStructure);
}

 
/*
*********************************************************************************************************
*	�� �� ��: TIM2_GPIO_Config
*	����˵��: ����TIM2�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
* 					TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
*           TIMx_CCR(��ƽ�����仯)
*						�ź�����=(TIMx_ARR +1 ) * ʱ������
* 					ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/	
static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
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
  TIM_TimeBaseStructure.TIM_Period = 1999;       //����ʱ����0������1999����Ϊ2000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 719;	    //����Ԥ��Ƶ��72M/719 = 10^5
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);	  //ʹ��ͨ��2
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ���Զ���װ

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	//ʹ��ͨ��4
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ���Զ���װ
	
  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // ʹ��TIM2���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //ʹ�ܶ�ʱ��2
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
*	�� �� ��: SERVO_Config
*	����˵��: ���ݱ��ָ�����ö������
*					  ���ת�ǣ�60~260
*	��    �Σ�servo
*	�� �� ֵ: 
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
*	�� �� ��: SERVO_Manual
*	����˵��: �ֶ����ö������
*					  ���ת�ǣ�60~260
*	��    �Σ�id��������        degree:ת��
*	�� �� ֵ: 
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

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

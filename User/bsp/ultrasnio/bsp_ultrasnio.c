/*
*********************************************************************************************************
*
*	ģ������ : ����������������ģ��
*	�ļ����� : bsp_ultrasnio.c
*	��    �� : V1.0
*	˵    �� : ����TIM8��PWM����������ȡ������ECHO�ŵĵ�ƽ
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-03-01 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#include  "bsp_ultrasnio.h"
#include  "_apollorobot.h"
#include  "os.h"
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
uint8_t  TIM8CH4_CAPTURE_STA=1;  //���ز�׽��־λ��=1��ʾ��׽���������أ�=0��ʾ��׽�����½���
uint16_t TIM8CH4_Rise,TIM8CH4_Fall,TIM8_T;

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern _Ultrasnio ult;//��ǰ��þ��룬��λΪcm

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************************************************
*	�� �� ��: NVIC_TIM8_Configuration
*	����˵��: TIM8�ж�����
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void NVIC_TIM8_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM8 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_ULTRASNIO_PP;//��ռ���ȼ����
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_ULTRASNIO_SP;//�����ȼ����
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
*	�� �� ��: TIM8_PWMInCap_Init
*	����˵��: PC9���ճ��������ظߵ�ƽ
*					 ����TIM8Ϊ���벶��ģʽ������PWM����ģʽ��
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void TIM8_PWMInCap_Init(void)
{	 
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_ICInitTypeDef  TIM8_ICInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//ʹ��TIM8ʱ��
		
		//��ʼ��TIM8��������Ϊ1us~65.535ms֮��	 
		TIM_TimeBaseStructure.TIM_Period = 0xffff;     //�趨�������Զ���װֵ 
		TIM_TimeBaseStructure.TIM_Prescaler = 71 ; 	//�ҵ���Ƶ�ܵ�72Mhz��Ԥ��Ƶ��72��Ƶ=1Mhz��1us��һ����   
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //��ʼ��TIM8����
	 
		//��ʼ��TIM8���벶�����,PC9
		TIM8_ICInitStructure.TIM_Channel = TIM_Channel_4; 
  	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�������ز���
  	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM8_ICInitStructure.TIM_ICFilter = 0x0A;//IC1F=1010 �˵�1us���������ȵĸ���
  	TIM_ICInit(TIM8, &TIM8_ICInitStructure);
		
		TIM_ITConfig(TIM8,TIM_IT_CC4,ENABLE);//����CC4IE���ز����ж�	 
	
		NVIC_TIM8_Configuration();

		TIM_Cmd(TIM8,ENABLE ); 	//ʹ�ܶ�ʱ��8
}
/*********************************************************************************************************
*	�� �� ��: Ultrasnio_Port_Init
*	����˵��: ������ռ�õ�GPIO��ʼ��
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void Ultrasnio_Port_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Pin  = ULTRASNIO_ECHO;    
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�ο�ST�ֲ᣺���óɸ������룬���ڼ�������ػ����½���  
	  GPIO_Init(ULTRASNIO_PORT, &GPIO_InitStructure);	 
	
		GPIO_InitStructure.GPIO_Pin  = ULTRASNIO_TRIG;    
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�ο�ST�ֲ᣺���ó��������
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
		GPIO_Init(ULTRASNIO_PORT, &GPIO_InitStructure);	
}
/*********************************************************************************************************
*	�� �� ��: Ultra_delay_us
*	����˵��: ��������ʱ
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void Ultra_delay_us(void)
{
    volatile int i = 16;	 
    while (i)
        i--;
}
static void Ultra_delay_nus(int n)
{
			int i = n;
			while(i--)
			{
				Ultra_delay_us();
			}
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************************************************
*	�� �� ��: Ultrasnio_Init
*	����˵��: ��������ʼ��
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Ultrasnio_Init(void)
{
			Ultrasnio_Port_Init();
			TIM8_PWMInCap_Init();
}
/*********************************************************************************************************
*	�� �� ��: Ultrasnio_update
*	����˵��: ����һ��Trig���ţ�����һ�β�����
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Ultrasnio_update(void)
{	  
     Ultrasnio_Trigger_H;
     Ultra_delay_nus(7);//ʾ����������ʱ��ʱΪ40us
		 Ultrasnio_Trigger_L;
}

/*********************************************************************************************************
*	�� �� ��: TIM8_CC_IRQHandler
*	����˵��: TIM8�жϷ�����
*          ������ECHO���Ÿ��ݽ��ܵ��ĸߵ�ƽʱ����¾���
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/

void TIM8_CC_IRQHandler(void)
{  //�����ж�
	 CPU_SR_ALLOC();
	 CPU_CRITICAL_ENTER();
	 OSIntEnter();
	 CPU_CRITICAL_EXIT();
	
	  if (TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET)//ch4���������¼�
		{	
				TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);   //����жϱ�־λ

				if(TIM8CH4_CAPTURE_STA == 1)	//����������
				{ 
						TIM8CH4_Rise = TIM_GetCapture4(TIM8);		           //��ȡ�����ص�����	  
						TIM8CH4_CAPTURE_STA = 0;		                        //ת����־λΪ�½���
						TIM_OC4PolarityConfig(TIM8,TIM_ICPolarity_Falling);		//����Ϊ�½��ز���	  			    			  
				}
				else  						    //�����½���
				{
						TIM8CH4_Fall = TIM_GetCapture4(TIM8);	      //��ȡ�½��ص�����	  
						TIM8CH4_CAPTURE_STA = 1;		//ת����־λΪ������
						if(TIM8CH4_Fall < TIM8CH4_Rise)     //���1����ʾ�ߵ�ƽ�����65535�������ֵ����ʱ״̬Ϊ�����ؽӽ�65535�������½��س�����65535��0��ʼ���㣬Tim8�����
						{
							TIM8_T = 65535;
						}
						else  //���2����ʾ��������������غ��½��ض���0-65535֮�䣬���½�����ֵ>��������ֵ��
						{
							TIM8_T = 0;
						}	
//						CPU_INT_DIS();
						ult.cur_distance = (float)(TIM8CH4_Fall - TIM8CH4_Rise + TIM8_T)*0.018;  //�õ��ܵĸߵ�ƽʱ�䣬ֵ��
//						CPU_INT_EN();
						TIM_OC4PolarityConfig(TIM8,TIM_ICPolarity_Rising); //CC4P=0 ����Ϊ�����ز���		
				}		    
		}
		//�˳��ж�
		OSIntExit();
}

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

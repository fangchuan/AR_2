/*
*********************************************************************************************************
*
*	ģ������ : ��Դ����������ģ��
*	�ļ����� : bsp_music.c
*	��    �� : V1.0
*	˵    �� : һ�׸����Ĳ���Ӧ�ÿ��ǵ��������أ������Ĳ���Ƶ�ʺ����Ľ��ģ�������������ʱ�䣩��
*							ֻҪ����������������ļ����ź�Ƶ�ʵĸߵͺͳ�����ʱ�䣬�Ϳ���ʹ�������������������ĸ���.
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#include  "bsp_music.h"
#include  "os.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#define  LOW_TONE
//#define  ALTO_TONE
//#define  HIGH_TONE

static const uint32_t Low[7] = {262,293,329,349,392,440,494};  //���� 1~7  
//static const uint32_t Alto[7] = {523,586,658,697,783,879,987}; //����1~7 
//static const uint32_t High[7] = {1045,1171,1316,1393,1563,1755,1971}; //����1~7

#define FULL_T     3600    //msΪ��λ    ȡֵT��T/2��T/4��T/8��T/16��T/32�ֱ��ʾȫ�����������������ķ��������˷�����
#define HALF_T     1800
#define FOUR_T     900
#define EIGHT_T    450
#define SIXTEEN_T  225
#define THIRTY_T   112


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*
*********************************************************************************************************
*	�� �� ��: TIM4_GPIO_Config
*	����˵��: TIM4  PWM�����GPIO��ʼ��
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void  TIM4_GPIO_Config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

//		/* ����TIM4CLK Ϊ 72MHZ */
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //��Ϊ��������������Ź���һ����ʱ��TIM4�����������ʼ����������Ͳ����ٳ�ʼ��TIM4

//    /* GPIOB clock enable */
//		RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

		/*GPIOB Configuration: */
		GPIO_InitStructure.GPIO_Pin =  BUZZER_Pin ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*
*********************************************************************************************************
*	�� �� ��: Music_Init
*	����˵��: ��������ʼ��
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void    Music_Init(void)
{
	    TIM4_GPIO_Config();
	    BUZZER_OUT = QUIET;
}
/*
*********************************************************************************************************
*	�� �� ��: Music_Config
*	����˵��: ���÷����������ͽ��ģ�һ��������������tone�ͽ���time
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/

_Error  Music_Config(_Music *music)
{
	    OS_ERR   err;
	    if(music->tones < 1 || music->tones > 7)
				return  ERROR_TONES;
			if(music->time < 1 || music->time > 6)
				return ERROR_TIME;
			
			switch(music->tones )      //����ǵ�����500hz�㹻�ˣ��������������TIM4Ҫ���õ�1000hz������Ҫ���õ�2000hz
			{
				case TONE_1:
        #ifdef  LOW_TONE
					BUZZER_OUT = Low[0];
        #endif
        #ifdef  ALTO_TONE
				  BUZZER_OUT = Alto[0];
				#endif
				#ifdef  HIGH_TONE
				  BUZZER_OUT = High[0];
				#endif
					break;
				case TONE_2:
				#ifdef  LOW_TONE
					BUZZER_OUT = Low[1];
        #endif
        #ifdef  ALTO_TONE
				  BUZZER_OUT = Alto[1];
				#endif
				#ifdef  HIGH_TONE
				  BUZZER_OUT = High[1];
				#endif
				  break;
				case TONE_3:
				#ifdef  LOW_TONE
					BUZZER_OUT = Low[2];
        #endif
        #ifdef  ALTO_TONE
				  BUZZER_OUT = Alto[2];
				#endif
				#ifdef  HIGH_TONE
				  BUZZER_OUT = High[2];
				#endif
				  break;
				case TONE_4:
				#ifdef  LOW_TONE
					BUZZER_OUT = Low[3];
        #endif
        #ifdef  ALTO_TONE
				  BUZZER_OUT = Alto[3];
				#endif
				#ifdef  HIGH_TONE
				  BUZZER_OUT = High[3];
				#endif
				  break;
				case TONE_5:
        #ifdef  LOW_TONE
					BUZZER_OUT = Low[4];
        #endif
        #ifdef  ALTO_TONE
				  BUZZER_OUT = Alto[4];
				#endif
				#ifdef  HIGH_TONE
				  BUZZER_OUT = High[4];
				#endif
					break;
				case TONE_6:
        #ifdef  LOW_TONE
					BUZZER_OUT = Low[5];
        #endif
        #ifdef  ALTO_TONE
				  BUZZER_OUT = Alto[5];
				#endif
				#ifdef  HIGH_TONE
				  BUZZER_OUT = High[5];
				#endif
					break;
				case TONE_7:
        #ifdef  LOW_TONE
					BUZZER_OUT = Low[6];
        #endif
        #ifdef  ALTO_TONE
				  BUZZER_OUT = Alto[6];
				#endif
				#ifdef  HIGH_TONE
				  BUZZER_OUT = High[6];
				#endif
					break;
			}
			
			switch(music->time )
			{
				case TIME_1:
					OSTimeDly(FULL_T,OS_OPT_TIME_DLY, &err);
					break;
				case TIME_2:
					OSTimeDly(HALF_T,OS_OPT_TIME_DLY, &err);
					break;
				case TIME_3:
					OSTimeDly(FOUR_T,OS_OPT_TIME_DLY, &err);
					break;
				case TIME_4:
					OSTimeDly(EIGHT_T,OS_OPT_TIME_DLY, &err);
				  break;
				case TIME_5:
					OSTimeDly(SIXTEEN_T,OS_OPT_TIME_DLY, &err);
					break;
				case TIME_6:
					OSTimeDly(THIRTY_T,OS_OPT_TIME_DLY, &err);
					break;
			}
			
			BUZZER_OUT = QUIET;
			
			return NO_ERROR;
}

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

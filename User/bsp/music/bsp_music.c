/*
*********************************************************************************************************
*
*	模块名称 : 无源蜂鸣器驱动模块
*	文件名称 : bsp_music.c
*	版    本 : V1.0
*	说    明 : 一首歌曲的产生应该考虑的两个因素：音符的产生频率和音的节拍（即声音持续的时间）。
*							只要控制输出到扬声器的激励信号频率的高低和持续的时间，就可以使扬声器发声产生优美的歌曲.
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
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

static const uint32_t Low[7] = {262,293,329,349,392,440,494};  //低音 1~7  
//static const uint32_t Alto[7] = {523,586,658,697,783,879,987}; //中音1~7 
//static const uint32_t High[7] = {1045,1171,1316,1393,1563,1755,1971}; //高音1~7

#define FULL_T     3600    //ms为单位    取值T、T/2、T/4、T/8、T/16、T/32分别表示全音符、二分音符、四分音符、八分音符
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
*	函 数 名: TIM4_GPIO_Config
*	功能说明: TIM4  PWM输出的GPIO初始化
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
static void  TIM4_GPIO_Config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

//		/* 设置TIM4CLK 为 72MHZ */
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //因为跟电机的驱动引脚共用一个定时器TIM4，所以如果初始化过电机话就不用再初始化TIM4

//    /* GPIOB clock enable */
//		RCC_APB2PeriphClockCmd(MOTOR_PORT_CLK, ENABLE); 

		/*GPIOB Configuration: */
		GPIO_InitStructure.GPIO_Pin =  BUZZER_Pin ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
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
*	函 数 名: Music_Init
*	功能说明: 蜂鸣器初始化
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void    Music_Init(void)
{
	    TIM4_GPIO_Config();
	    BUZZER_OUT = QUIET;
}
/*
*********************************************************************************************************
*	函 数 名: Music_Config
*	功能说明: 配置蜂鸣器音调和节拍：一个音符包括音调tone和节拍time
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/

_Error  Music_Config(_Music *music)
{
	    OS_ERR   err;
	    if(music->tones < 1 || music->tones > 7)
				return  ERROR_TONES;
			if(music->time < 1 || music->time > 6)
				return ERROR_TIME;
			
			switch(music->tones )      //如果是低音，500hz足够了，但如果是中音，TIM4要配置到1000hz。高音要配置到2000hz
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

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

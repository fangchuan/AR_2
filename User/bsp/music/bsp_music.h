#ifndef  __BSP_MUSIC_H
#define  __BSP_MUSIC_H

#include "stm32f10x.h"
#include  "_apollorobot.h"

//·äÃùÆ÷µÄÒý½Å
#define  BUZZER_Pin       GPIO_Pin_6
#define  BUZZER_PORT      GPIOB

#define  BUZZER_OUT       TIM4->CCR1
#define  QUIET            0
//tones id
#define  TONE_1           1
#define  TONE_2           2
#define  TONE_3           3
#define  TONE_4           4
#define  TONE_5           5
#define  TONE_6           6
#define  TONE_7           7
//time id
#define  TIME_1           1
#define  TIME_2           2
#define  TIME_3           3
#define  TIME_4           4
#define  TIME_5           5
#define  TIME_6           6

extern void    Music_Init(void);
extern _Error  Music_Config(_Music *music);

#endif //__BSP_MUSIC_H

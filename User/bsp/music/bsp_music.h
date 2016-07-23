/*
*********************************************************************************************************
*
*	模块名称 : 无源蜂鸣器驱动模块
*	文件名称 : bsp_music.h
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
#ifndef  __BSP_MUSIC_H
#define  __BSP_MUSIC_H

#include "stm32f10x.h"
#include  "_apollorobot.h"

//蜂鸣器的引脚
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
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

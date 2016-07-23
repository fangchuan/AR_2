/*
*********************************************************************************************************
*
*	模块名称 : XPT2046 bsp模块头文件
*	文件名称 : bsp_touch.h
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef __BSP_TOUCH_H
#define	__BSP_TOUCH_H

#include "stm32f10x.h"

#define	CHX 	0x90 	//通道Y+的选择控制字	
#define	CHY 	0xd0	//通道X+的选择控制字


extern void Touch_Init(void);
uint16_t XPT2046_ReadAdc(uint8_t _ucCh);
void XPT2046_WriteCMD(unsigned char cmd);
unsigned short XPT2046_ReadCMD(void) ;
uint16_t XPT2046_ReadAdc_Fliter(uint8_t _ucCh);


#if  0
/* 液晶坐标结构体 */
typedef	struct POINT 
{
   uint16_t x;		
   uint16_t y;
}Coordinate;

/*  校正系数结构体 */
typedef struct Parameter 
{						
long double An,  			 //注:sizeof(long double) = 8
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
}Parameter ;



//extern Coordinate ScreenSample[4];
//extern Coordinate DisplaySample[4];
//extern Parameter touch_para ;
//extern Coordinate  display ;
int Touch_Calibrate(void);
void Palette_Init(void);
FunctionalState Get_touch_point(Coordinate * displayPtr,
                                Coordinate * screenPtr,
                                Parameter * para );
Coordinate *Read_2046_2(void);

#endif


#endif /* __BSP_TOUCH_H */
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

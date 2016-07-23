/*
*********************************************************************************************************
*
*	ģ������ : XPT2046 bspģ��ͷ�ļ�
*	�ļ����� : bsp_touch.h
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef __BSP_TOUCH_H
#define	__BSP_TOUCH_H

#include "stm32f10x.h"

#define	CHX 	0x90 	//ͨ��Y+��ѡ�������	
#define	CHY 	0xd0	//ͨ��X+��ѡ�������


extern void Touch_Init(void);
uint16_t XPT2046_ReadAdc(uint8_t _ucCh);
void XPT2046_WriteCMD(unsigned char cmd);
unsigned short XPT2046_ReadCMD(void) ;
uint16_t XPT2046_ReadAdc_Fliter(uint8_t _ucCh);


#if  0
/* Һ������ṹ�� */
typedef	struct POINT 
{
   uint16_t x;		
   uint16_t y;
}Coordinate;

/*  У��ϵ���ṹ�� */
typedef struct Parameter 
{						
long double An,  			 //ע:sizeof(long double) = 8
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
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

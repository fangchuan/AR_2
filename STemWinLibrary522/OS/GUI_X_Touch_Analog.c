/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include "GUI.h"
#include "bsp_touch.h"


/*
*********************************************************************************************************
*	函 数 名: GUI_TOUCH_X_ActivateX
*	功能说明: 启动X通道
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GUI_TOUCH_X_ActivateX(void) 
{
 // XPT2046_WriteCMD(0x90);
}

/*
*********************************************************************************************************
*	函 数 名: GUI_TOUCH_X_ActivateY
*	功能说明: 启动Y通道
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GUI_TOUCH_X_ActivateY(void)
{
  //XPT2046_WriteCMD(0xd0);
}

/*
*********************************************************************************************************
*	函 数 名: GUI_TOUCH_X_MeasureX
*	功能说明: 并返回X通道ADC采样结果
*	形    参：无
*	返 回 值: 滤波后的X通道12位ADC值
*********************************************************************************************************
*/
int  GUI_TOUCH_X_MeasureX(void) 
{
    return XPT2046_ReadAdc_Fliter(CHY);

}

/*
*********************************************************************************************************
*	函 数 名: GUI_TOUCH_X_MeasureY
*	功能说明: 返回Y通道ADC采样结果
*	形    参：无
*	返 回 值: 滤波后的Y通道12位ADC值
*********************************************************************************************************
*/
int  GUI_TOUCH_X_MeasureY(void) 
{	

    return XPT2046_ReadAdc_Fliter(CHX);

}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

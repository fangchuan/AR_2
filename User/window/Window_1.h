/*
*********************************************************************************************************
*
*	模块名称 : 编程界面
*	文件名称 : Window_1.h
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
#ifndef _WINDOW_1_H
#define _WINDOW_1_H

#include "stm32f10x.h"
#include "DIALOG.h"
#include "Window_1_1.h"
#include "SongTi16.h"
#include "SongTi12.h"
#include "WIDGET_NewFilePad.h"
#include "Window_TreeView.h"
#include "String.h"

//extern function
WM_HWIN CreateWindow_1(void);

//global variable
extern WM_HWIN hWin_1;
extern char program_name[20];
#endif //

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

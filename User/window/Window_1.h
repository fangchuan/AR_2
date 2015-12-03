#ifndef _WINDOW_1_H
#define _WINDOW_1_H

#include "stm32f10x.h"
#include "DIALOG.h"
#include "Window_1_1.h"
#include "SongTi16.h"
#include "SongTi12.h"
#include "WIDGET_NumPad.h"
#include "String.h"

//extern function
WM_HWIN CreateWindow_1(void);

//global variable
extern WM_HWIN hWin_1;
extern char program_name[10];
#endif //


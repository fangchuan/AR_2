#ifndef  _WINDOW_1_1_H
#define  _WINDOW_1_1_H

#include "DIALOG.h"
#include "stm32f10x.h"
#include "MESSAGEBOX.h"
#include "WIDGET_Instructor.h"
#include "string.h"
#include "stdint.h"
#include "WIDGET_SelectDialog.h"

//extern function
WM_HWIN CreateWindow_1_1(void);
void _MessageBox(const char* pText, const char* pCaption);

extern WM_HWIN hWin1_1;
#endif //

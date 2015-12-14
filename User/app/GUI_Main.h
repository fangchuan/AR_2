#ifndef _GUI_MAIN_H
#define _GUI_MAIN_H

#include "GUI.h"
#include <stddef.h>
#include <string.h>

#include "FRAMEWIN.h"
#include "bsp_usart1.h"

#include "Window_Top.h"
#include "Window_1.h"
#include "Window_2.h"
#include "Window_3.h"
#include "Window_4.h"
#include "Window_Running.h"
#include  "Window_TreeView.h"

void GUI_Main_Task(void);

#endif/*_GUI_MAIN_H*/

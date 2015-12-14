#include "GUI_Main.h"


uint8_t Key_Value = 0;//the value of button on the Top Window
/*********************************************************************
*
*      GUIMainTask
*/
void GUI_Main_Task()
{
			WM_SetCreateFlags(WM_CF_MEMDEV);
			WM_EnableMemdev(WM_HBKWIN);
	
			CreateWindow_Top();
			CreateWindow_1();
			CreateWindow_2();
			CreateWindow_3();
			CreateWindow_4();
//			CreateWindow_TreeView();
	
			WM_HideWindow(hWin_Top);
			WM_HideWindow(hWin_1);
			WM_HideWindow(hWin_2);
			WM_HideWindow(hWin_3);
			WM_HideWindow(hWin_4);
//			WM_HideWindow(hTree);
			
}
/*************************** End of file ****************************/


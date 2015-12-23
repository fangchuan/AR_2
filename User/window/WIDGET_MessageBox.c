#include  "WIDGET_MessageBox.h"
#include  "SongTi12.h"
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const char *StringHZ[] = {
	"\xe7\xa1\xae\xe5\xae\x9a",//0:确认
};


/*********************************************************************
*
*       _MessageBox
*/
void _MessageBox(const char* pText, const char* pCaption) {
  WM_HWIN hWin;
  WM_HWIN hItem;
	GUI_RECT  Rect;
	
  hWin = MESSAGEBOX_Create(pText, pCaption, 0);
	
	WM_GetWindowRectEx(hWin, &Rect);
  WM_SetWindowPos(hWin, Rect.x0 - 15, 
                        Rect.y0 - 15, 
                        Rect.x1 - Rect.x0 + 1 + 50, 
                        Rect.y1 - Rect.y0 + 1 + 10);
	
	//set the framewin title
	FRAMEWIN_SetFont(hWin, &GUI_FontSongTi12);
	FRAMEWIN_SetText(hWin, pCaption);
	//set the button font
	hItem = WM_GetDialogItem(hWin, GUI_ID_OK);
  BUTTON_SetFont(hItem, &GUI_FontSongTi12);
	BUTTON_SetText(hItem, StringHZ[0]);
	WM_GetWindowRectEx(hItem, &Rect);//返回窗口在坐标系中的位置
  WM_SetWindowPos(hItem, Rect.x0 + 15, 
                         Rect.y0 + 5, 
                         Rect.x1 - Rect.x0 + 1 + 20, 
                         Rect.y1 - Rect.y0 + 1 );
	//set the text font
	hItem = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
  TEXT_SetFont(hItem, &GUI_FontSongTi12);
	TEXT_SetText(hItem, pText);
	TEXT_SetTextAlign(hItem, GUI_TA_LEFT);
	WM_GetWindowRectEx(hItem, &Rect);
  WM_SetWindowPos(hItem, Rect.x0, 
                         Rect.y0, 
                         Rect.x1 - Rect.x0 + 1 + 70, 
                         Rect.y1 - Rect.y0 + 1 + 12);
												 
  WM_MakeModal(hWin);
  GUI_ExecCreatedDialog(hWin);
}

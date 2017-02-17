/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.24 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : WIDGET_NumPad.c
Purpose     : Shows how to use a numpad as input device on a touch screen
Requirements: WindowManager - (x)
              MemoryDevices - ( )
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
---------------------------END-OF-HEADER------------------------------
*/

#include "WIDGET_NewFilePad.h"
#include "string.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     	(GUI_ID_USER + 15)
#define ID_EDIT_0     		(GUI_ID_USER + 14)
#define ID_BUTTON_0				(GUI_ID_USER + 0)
#define ID_BUTTON_1				(GUI_ID_USER + 1)
#define ID_BUTTON_2				(GUI_ID_USER + 2)
#define ID_BUTTON_3				(GUI_ID_USER + 3)
#define ID_BUTTON_4				(GUI_ID_USER + 4)
#define ID_BUTTON_5				(GUI_ID_USER + 5)
#define ID_BUTTON_6				(GUI_ID_USER + 6)
#define ID_BUTTON_7				(GUI_ID_USER + 7)
#define ID_BUTTON_8				(GUI_ID_USER + 8)
#define ID_BUTTON_9				(GUI_ID_USER + 9)
#define ID_BUTTON_DOT			(GUI_ID_USER + 10)
#define ID_BUTTON_DEL     (GUI_ID_USER + 11)
#define ID_BUTTON_OK  		(GUI_ID_USER + 12)
#define ID_BUTTON_CANCEL 	(GUI_ID_USER + 13)
/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSongTi12;
extern char program_name[20];//the Name of Program
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const char *StringHZ[] = {
	"\xe7\xa1\xae\xe5\xae\x9a",//0:确定
	"\xe8\xbf\x94\xe5\x9b\x9e",//1:返回
	"\xe8\xaf\xb7\xe8\xbe\x93\xe5\x85\xa5\xe7\xa8\x8b\xe5\xba\x8f\xe5\x90\x8d",//2:请输入程序名
};
//
// Dialog resource of numpad
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window",   ID_WINDOW_0, 0, 0, 110, 190, 0, 0x0, 0 },
  { EDIT_CreateIndirect,   "Edit",		 ID_EDIT_0,   0, 20, 107, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect,   "7",      ID_BUTTON_7,   5,   40,  30,  25},
  { BUTTON_CreateIndirect,   "8",      ID_BUTTON_8,  40,   40,  30,  25},
  { BUTTON_CreateIndirect,   "9",      ID_BUTTON_9,  75,   40,  30,  25},
  { BUTTON_CreateIndirect,   "4",      ID_BUTTON_4,   5,  70,  30,  25},
  { BUTTON_CreateIndirect,   "5",      ID_BUTTON_5,  40,  70,  30,  25},
  { BUTTON_CreateIndirect,   "6",      ID_BUTTON_6,  75,  70,  30,  25},
  { BUTTON_CreateIndirect,   "1",      ID_BUTTON_1,   5,  100,  30,  25},
  { BUTTON_CreateIndirect,   "2",      ID_BUTTON_2,  40,  100,  30,  25},
  { BUTTON_CreateIndirect,   "3",      ID_BUTTON_3,  75,  100,  30,  25},
  { BUTTON_CreateIndirect,   "0",      ID_BUTTON_0,   5, 130,  30,  25},
  { BUTTON_CreateIndirect,   ".",      ID_BUTTON_DOT,40, 130,  30,  25},
  { BUTTON_CreateIndirect,   "Del",    ID_BUTTON_DEL,75, 130,  30,  25},
  { BUTTON_CreateIndirect,   "OK",     ID_BUTTON_OK,  5, 163,  45,  25},
  { BUTTON_CreateIndirect,   "Cancel", ID_BUTTON_CANCEL,  60, 163, 45,  25},
};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbDialogNewPad
*
* Function description
*   Callback function of the newpad.
*/
static void _cbDialogNewPad(WM_MESSAGE * pMsg) {
  GUI_RECT r;
  unsigned i; 
  int      NCode;
  unsigned Id;
  WM_HWIN  hItem;
	WM_HWIN  hEdit;
  
	hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
	
  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&r);
    GUI_SetColor(0x000000);
    GUI_DrawRect(r.x0, r.y0, r.x1, r.y1);          /* Draw rectangle around it */
    /* Draw the bright sides */
    GUI_SetColor(0xffffff);
    GUI_DrawHLine(r.y0 + 1, r.x0 + 1, r.x1 - 2);   /* Draw top line */
    GUI_DrawVLine(r.x0 + 1, r.y0 + 1, r.y1 - 2);
    /* Draw the dark sides */
    GUI_SetColor(0x555555);
    GUI_DrawHLine(r.y1-1, r.x0 + 1, r.x1 - 1);
    GUI_DrawVLine(r.x1-1, r.y0 + 1, r.y1 - 2);
	
		GUI_SetColor(GUI_RED);
		GUI_SetFont(&GUI_FontSongTi12);
		GUI_DispStringAt(StringHZ[2], 19, 3);
    break;
  case WM_INIT_DIALOG:
    for (i = 0; i < GUI_COUNTOF(_aDialogCreate) - 1; i++) {
      hItem = WM_GetDialogItem(pMsg->hWin , GUI_ID_USER + i);
      BUTTON_SetFocussable(hItem, 0);                       /* Set all buttons non focussable */
    }
		
		EDIT_SetText(hEdit, "LeCoder0");
		EDIT_SetTextColor(hEdit, EDIT_CI_ENABLED, GUI_RED);
		EDIT_SetBkColor(hEdit, EDIT_CI_ENABLED, GUI_CYAN);
		EDIT_SetFocussable(hEdit,1);
//	  EDIT_SetSel(hEdit, 7, -1);
		EDIT_SetCursorAtChar(hEdit, 8);
		
		hItem = WM_GetDialogItem(pMsg->hWin , ID_BUTTON_OK);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[0]);
		
		hItem = WM_GetDialogItem(pMsg->hWin , ID_BUTTON_CANCEL);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[1]);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
    NCode = pMsg->Data.v;                 /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
			   break;
    case WM_NOTIFICATION_RELEASED:  
				if( Id >= ID_BUTTON_0 && Id < ID_BUTTON_OK){
						int Key;
						if (Id < ID_BUTTON_DEL){
								char acBuffer[10] = {0};
								BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); /* Get the text of the button */
								Key = acBuffer[0];
						}
						else{
								Key = GUI_KEY_BACKSPACE;
						}
						GUI_SendKeyMsg(Key, 1);                                /* Send a key message to the focussed window */
				}else{
					if( ID_BUTTON_OK == Id ){
						
							EDIT_GetText(hEdit, program_name, sizeof(program_name));
							GUI_EndDialog(pMsg->hWin,0);
						
					}else{
						if (Id == ID_BUTTON_CANCEL){
							memset(program_name,0 ,sizeof(program_name));
							GUI_EndDialog(pMsg->hWin, 0);
					}
					}
				}
      break;
    }
	default:
    WM_DefaultProc(pMsg);
  }
}


/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/

WM_HWIN CreateNewFilePad(WM_HWIN hParent) {
  WM_HWIN hNewPad;

  hNewPad = GUI_CreateDialogBox(_aDialogCreate, 
                                GUI_COUNTOF(_aDialogCreate), 
                                _cbDialogNewPad, hParent, 85, 69); /* Create the numpad dialog */
  WM_SetStayOnTop(hNewPad, 1);
	return hNewPad;

}

/***************************** 阿波罗科技 www.apollorobot.cn(END OF FILE) *********************************/


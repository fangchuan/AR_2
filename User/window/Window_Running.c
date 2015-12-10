/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

#include "Window_Running.h"
#include "SongTi12.h"
#include "WIDGET_SelectDialog.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     	 (GUI_ID_USER + 0x00)
#define ID_EDIT_A          (GUI_ID_USER + 0x01)
#define ID_EDIT_B          (GUI_ID_USER + 0x02)
#define ID_TEXT_A          (GUI_ID_USER + 0x03)
#define ID_TEXT_B          (GUI_ID_USER + 0x04)

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
WM_HWIN                       hRun;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char *StringHZ[] = {
	"\xe5\x81\x9c\xe6\xad\xa2",//0:停止
	"\xe7\xa8\x8b\xe5\xba\x8f\xe8\x87\xaa\xe5\x8a\xa8\xe8\xbf\x90\xe8\xa1\x8c\xe4\xb8\xad......",//1:程序自动运行中
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "A", ID_EDIT_A, 60, 20, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "B", ID_EDIT_B, 60, 60, 120, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "A", ID_TEXT_A, 0, 20, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "B", ID_TEXT_B, 0, 60, 40, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/



/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		//
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_LIGHTBLUE);
    //
    // Initialization of 'A'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_A);
		EDIT_SetFont(hItem,&GUI_FontSongTi12);
    EDIT_SetText(hItem, "");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_B);
		EDIT_SetFont(hItem,&GUI_FontSongTi12);
    EDIT_SetText(hItem, "");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'A'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_A);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_B);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_EDIT_A: // Notifications sent by 'A'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_B: // Notifications sent by 'B'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    }
    break;
	case WM_PAINT:
				hItem = pMsg->hWin;
				GUI_SetColor(GUI_RED);
				GUI_SetFont(&GUI_FontSongTi12);
				GUI_DispStringHCenterAt(StringHZ[1], 120, 140);
				break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN Create_RunningWindow(void);
WM_HWIN Create_RunningWindow(void) {


  hRun = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hRun;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/

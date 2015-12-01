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

// USER START (Optionally insert additional includes)
// USER END
#include "Window_2.h"
#include "SongTi12.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern uint8_t Key_Value;
WM_HWIN hWin_2;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_HEADER_0     (GUI_ID_USER + 0x01)
#define ID_BUTTON_0     (GUI_ID_USER + 0x02)
#define ID_HEADER_1     (GUI_ID_USER + 0x03)

#define ID_LISTVIEW_0     (GUI_ID_USER + 0x05)
#define ID_BUTTON_1     (GUI_ID_USER + 0x07)
#define ID_BUTTON_2     (GUI_ID_USER + 0x08)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const char *StringHZ[] = {
	"\xe8\xbf\x94\xe5\x9b\x9e","\xe6\x96\xad\xe5\xbc\x80",//0:返回，1:断开
	"\xe6\x9b\xb4\xe6\x96\xb0\xe5\x9b\xba\xe4\xbb\xb6",//2:更新固件
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 0, 300, 240, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_0, 0, 300, 80, 20, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_1, 0, 0, 240, 20, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 0, 40, 200, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Update Firmware", ID_BUTTON_1, 0, 120, 100, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Disconnect", ID_BUTTON_2, 160, 300, 80, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, StringHZ[0]);
	    
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, StringHZ[2]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, StringHZ[1]);
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_1);
    HEADER_AddItem(hItem, 80, "ApolloRobot", 14);
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    LISTVIEW_AddColumn(hItem, 50, "Port1", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "Port2", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "Port3", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    LISTVIEW_AddColumn(hItem, 50, "Port4", GUI_TA_HCENTER | GUI_TA_VCENTER);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_HEADER_0: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        break;
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 0;
        break;
      }
      break;
    case ID_HEADER_1: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        break;
      }
      break;
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						
        break;
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 0;
        break;
      }
      break;
    }
    break;
		
	case WM_PAINT:
			GUI_SetColor(GUI_RED);
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringAt("Sensor Statu:",0,20);
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
WM_HWIN CreateWindow_2(void);
WM_HWIN CreateWindow_2(void) {

  hWin_2 = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin_2;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/

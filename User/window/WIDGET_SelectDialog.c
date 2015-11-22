//*********************************************************
// in order to create selecte dialog when click edits    **
//*********************************************************

#include "WIDGET_SelectDialog.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)
#define ID_BUTTON_1     (GUI_ID_USER + 0x02)
//#define ID_BUTTON_2     (GUI_ID_USER + 0x03)
#define ID_BUTTON_3     (GUI_ID_USER + 0x04)


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 80, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Change", ID_BUTTON_0, 10, 10, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Insert", ID_BUTTON_1, 10, 40, 60, 20, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Delete", ID_BUTTON_2, 10, 70, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_3, 40, 100, 40, 20, 0, 0x0, 0 },
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
  GUI_RECT r;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x00FF8080);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Change'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						CreateWindow_Instructor();
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Insert'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						CreateWindow_Instructor();
        break;
      }
      break;
//    case ID_BUTTON_2: // Notifications sent by 'Delete'
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        break;
//      case WM_NOTIFICATION_RELEASED:

//        break;
//      }
//      break;
    case ID_BUTTON_3: // Notifications sent by 'BACK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    }
    break;
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
*       Create Select Dialog
*/
WM_HWIN CreateSelectDialog (WM_HWIN hParent, int x0, int y0)
{			
			WM_HWIN hWin;
			
			hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hParent, x0, y0);
			return hWin;
}

/*************************** End of file ****************************/
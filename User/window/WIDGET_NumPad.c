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

#include "WIDGET_NumPad.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static int _aKey[] = {GUI_KEY_BACKSPACE, GUI_KEY_TAB};
//
// Dialog resource of numpad
//
static const GUI_WIDGET_CREATE_INFO _aDialogNumPad[] = {
//
//  Function                 Text      Id                 Px   Py   Dx   Dy
//
  { WINDOW_CreateIndirect,   0,        0,                100, 80,  120, 150},
  { BUTTON_CreateIndirect,   "7",      GUI_ID_USER +  7,   5,   5,  25,  20},
  { BUTTON_CreateIndirect,   "8",      GUI_ID_USER +  8,  40,   5,  25,  20},
  { BUTTON_CreateIndirect,   "9",      GUI_ID_USER +  9,  75,   5,  25,  20},
  { BUTTON_CreateIndirect,   "4",      GUI_ID_USER +  4,   5,  35,  25,  20},
  { BUTTON_CreateIndirect,   "5",      GUI_ID_USER +  5,  40,  35,  25,  20},
  { BUTTON_CreateIndirect,   "6",      GUI_ID_USER +  6,  75,  35,  25,  20},
  { BUTTON_CreateIndirect,   "1",      GUI_ID_USER +  1,   5,  65,  25,  20},
  { BUTTON_CreateIndirect,   "2",      GUI_ID_USER +  2,  40,  65,  25,  20},
  { BUTTON_CreateIndirect,   "3",      GUI_ID_USER +  3,  75,  65,  25,  20},
  { BUTTON_CreateIndirect,   "0",      GUI_ID_USER +  0,   5,  95,  25,  20},
  { BUTTON_CreateIndirect,   ".",      GUI_ID_USER + 10,  40,  95,  25,  20},
  { BUTTON_CreateIndirect,   "Del",    GUI_ID_USER + 11,  75,  95,  25,  20},
  { BUTTON_CreateIndirect,   "Tab",    GUI_ID_USER + 12,   5, 125,  25,  20},
  { BUTTON_CreateIndirect,   "OK",     GUI_ID_USER + 13,  40, 125,  25,  20},
  { BUTTON_CreateIndirect,   "Cancel", GUI_ID_USER + 14,  75, 125,  25,  20},
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbDialogNumPad
*
* Function description
*   Callback function of the numpad.
*/
static void _cbDialogNumPad(WM_MESSAGE * pMsg) {
  GUI_RECT r;
  unsigned i; 
  int      NCode;
  unsigned Id;
  int      Pressed;
  WM_HWIN  hDlg;
  WM_HWIN  hItem;
  
  Pressed = 0;
  hDlg = pMsg->hWin;
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
    break;
  case WM_INIT_DIALOG:
    for (i = 0; i < GUI_COUNTOF(_aDialogNumPad) - 1; i++) {
      hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + i);
      BUTTON_SetFocussable(hItem, 0);                       /* Set all buttons non focussable */
    }
    WM_GetDialogItem(hDlg, GUI_ID_USER + 12);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
    NCode = pMsg->Data.v;                 /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
      Pressed = 1;
    case WM_NOTIFICATION_RELEASED:  
				if( Id < GUI_ID_USER + 13)
				{
						int Key;
						if (Id < GUI_ID_USER + 11)
						{
								char acBuffer[10];
								BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); /* Get the text of the button */
								Key = acBuffer[0];
						}
						else
						{
								Key = _aKey[Id - GUI_ID_USER - 11];
						}
						GUI_SendKeyMsg(Key, Pressed);                                /* Send a key message to the focussed window */
				}
				else 
				{
						GUI_EndDialog(pMsg->hWin,0);
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

void Create_NumPad(WM_HWIN hParent) {
  WM_HWIN hNumPad;

  hNumPad = GUI_CreateDialogBox(_aDialogNumPad, 
                                GUI_COUNTOF(_aDialogNumPad), 
                                _cbDialogNumPad, hParent, 0, 0); /* Create the numpad dialog */
  WM_SetStayOnTop(hNumPad, 1);

}

/*************************** End of file ****************************/


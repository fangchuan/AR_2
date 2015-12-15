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

#include "Window_1.h"

/*********************************************************************
*
*       Global variable
*
**********************************************************************
*/
//Key value of the TopWindow,to switch the subwindow
extern uint8_t Key_Value;
//
WM_HWIN hWin_1;

//the Name of Program
char program_name[10];
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_0     (GUI_ID_USER + 0x01)
#define ID_HEADER_0     (GUI_ID_USER + 0x02)
#define ID_BUTTON_0     (GUI_ID_USER + 0x03)
#define ID_BUTTON_1     (GUI_ID_USER + 0x04)
#define ID_BUTTON_2     (GUI_ID_USER + 0x06)
#define ID_HEADER_1     (GUI_ID_USER + 0x08)
#define ID_BUTTON_3     (GUI_ID_USER + 0x09)


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

const static char *_StringHZ[] = {"\xe6\x96\xb0\xe5\xbb\xba",//0:新建
	"\xe6\x89\x93\xe5\xbc\x80","\xe8\xbf\x94\xe5\x9b\x9e",////1:打开,2:返回
	"\xe8\xbf\x9b\xe5\x85\xa5","\xe8\xaf\xb7\xe8\xbe\x93\xe5\x85\xa5\xe7\xa8\x8b\xe5\xba\x8f\xe5\x90\x8d", //3：进入  4:请输入程序名
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Edit0",  ID_MULTIEDIT_0, 100, 40, 110, 40, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 0, 300, 240, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 0, 300, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 160, 300, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "New", ID_BUTTON_2, 0, 40, 80, 40, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_1, 0, 0, 240, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Open", ID_BUTTON_3, 0, 120, 80, 40, 0, 0x0, 0 },
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
  WM_HWIN          hItem;
  int              NCode;
  int              Id;
  WM_HWIN         hNumPad;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		//
		//Initialization of Window
		//
		hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_LIGHTBLUE);
    //
    // Initialization of 'Button'
    //
		GUI_UC_SetEncodeUTF8();
	  GUI_SetFont(&GUI_FontSongTi16);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, _StringHZ[2]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, _StringHZ[3]);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetFont(hItem,&GUI_FontSongTi16);
    BUTTON_SetText(hItem, _StringHZ[0]);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
		BUTTON_SetFont(hItem,&GUI_FontSongTi16);
    BUTTON_SetText(hItem, _StringHZ[1]);
	  //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_1);
    HEADER_AddItem(hItem, 80, "ApolloRobot", 14);
		//
		//Initialize EDIT0 
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "");
		MULTIEDIT_SetFocussable(hItem,1);
    break;
	case WM_PAINT:
				GUI_SetColor(GUI_RED);
				GUI_SetFont(&GUI_FontSongTi12);
				GUI_DispStringAt(_StringHZ[4], 100, 25);
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
    case ID_BUTTON_1: // Notifications sent by 'ENTER'
		case ID_BUTTON_2: // Notifications sent by 'New'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						//get the program name in the edit
						hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
						MULTIEDIT_GetText(hItem,program_name,10);
						//warning the situation of empty program name
						if(strcmp(program_name, "") == 0)
						{
							_MessageBox("Please Input the Program Name","Error");
						}
						else
						{
							CreateWindow_1_1();
						}							
        break;
      }
      break;
		case ID_BUTTON_3: // Notifications sent by 'Open'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						CreateWindow_TreeView();
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
		case ID_MULTIEDIT_0: // Notifications sent by 'Edit1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
							hNumPad = Create_NumPad(pMsg->hWin);
							WM_MakeModal(hNumPad);
							GUI_ExecCreatedDialog(hNumPad);
       break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    }
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
WM_HWIN CreateWindow_1(void);
WM_HWIN CreateWindow_1(void) {

  hWin_1 = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin_1;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/

/*
*********************************************************************************************************
*
*	模块名称 : 编程界面
*	文件名称 : Window_1.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#include "Window_1.h"

/*********************************************************************
*
*       Global variable
*
**********************************************************************
*/
//Key value of the TopWindow,to switch the subwindow
extern uint8_t Key_Value;

WM_HWIN hWin_1;


char program_name[10];//the Name of Program
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_0  (GUI_ID_USER + 0x01)
#define ID_HEADER_1     (GUI_ID_USER + 0x08)
//#define ID_EDIT_0     (GUI_ID_USER + 0x02)
#define ID_BUTTON_BACK  (GUI_ID_USER + 0x03)
#define ID_BUTTON_ENTER (GUI_ID_USER + 0x04)
#define ID_BUTTON_NEW   (GUI_ID_USER + 0x06)
#define ID_BUTTON_OPEN  (GUI_ID_USER + 0x09)


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

const static char *_StringHZ[] = {
	"\xe6\x96\xb0\xe5\xbb\xba",			//0:新建
	"\xe6\x89\x93\xe5\xbc\x80",			//1:打开
	"\xe8\xbf\x94\xe5\x9b\x9e",			//2:返回
	"\xe8\xbf\x9b\xe5\x85\xa5",			//3：进入
	"\xe8\xaf\xb7\xe8\xbe\x93\xe5\x85\xa5\xe7\xa8\x8b\xe5\xba\x8f\xe5\x90\x8d",//4:请输入程序名
	"\xe9\x94\x99\xe8\xaf\xaf",			//5:错误
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "0",  ID_MULTIEDIT_0, 100, 40, 110, 40, 0, 0x0, 0 },
	{ HEADER_CreateIndirect, "Header", ID_HEADER_1, 0, 0, 240, 20, 0, 0x0, 0 },
//  { EDIT_CreateIndirect, "0", ID_EDIT_0, 100, 40, 110, 40, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_BACK, 0, 290, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_ENTER, 160, 290, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "New", ID_BUTTON_NEW, 0, 40, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Open", ID_BUTTON_OPEN, 0, 120, 80, 40, 0, 0x0, 0 },

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
  WM_HWIN          hItem;
  int              NCode;
  int              Id;
  WM_HWIN         hNumPad;
	u8              Mb_Val;

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
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, _StringHZ[2]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_ENTER);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, _StringHZ[3]);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NEW);
		BUTTON_SetFont(hItem,&GUI_FontSongTi16);
    BUTTON_SetText(hItem, _StringHZ[0]);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OPEN);
		BUTTON_SetFont(hItem,&GUI_FontSongTi16);
    BUTTON_SetText(hItem, _StringHZ[1]);
	  //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_1);
    HEADER_AddItem(hItem, 80, "LeCoder", 14);
		//
		//Initialize EDIT0 
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "");     //清空EDIT内容
		MULTIEDIT_SetFocussable(hItem,1); //设为捕捉光标焦点
		MULTIEDIT_SetAutoScrollV(hItem,1);//自动竖直滑块
		MULTIEDIT_SetAutoScrollH(hItem,1);//自动水平滑块
		MULTIEDIT_SetWrapWord(hItem);     //自动换行
//		MULTIEDIT_EnableBlink(hItem, 800, 1);
//		EDIT_SetText(hItem, "0");
//		EDIT_SetFocussable(hItem, 1);
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

    case ID_BUTTON_BACK: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 0;
        break;
      }
      break;
    case ID_BUTTON_ENTER: // Notifications sent by 'ENTER'
		case ID_BUTTON_NEW: // Notifications sent by 'New'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						//get the program name in the edit
						hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
						MULTIEDIT_GetText(hItem,program_name,sizeof(program_name));
						//warning the situation of empty program name
						if(strcmp(program_name, "") == 0)
						{
							_MessageBox(_StringHZ[4],_StringHZ[5], &Mb_Val);
						}
						else
						{
							CreateWindow_1_1();
						}							
        break;
      }
      break;
		case ID_BUTTON_OPEN: // Notifications sent by 'Open'
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

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

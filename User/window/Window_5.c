/*
*********************************************************************************************************
*
*	模块名称 : 第5个界面，自平衡演示
*	文件名称 : Window_5.c
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

#include "Window_5.h"
#include "_apollorobot.h"
#include "WIDGET_DrawScale.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern volatile uint8_t Key_Value;
extern _Euler    euler;

WM_HWIN hWin_5;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0        (GUI_ID_USER + 0x01)
#define ID_BUTTON_BACK     (GUI_ID_USER + 0x02)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char*  _StringHZ[] = {
	"\xe8\xbf\x94\xe5\x9b\x9e",													//0:返回

};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_BACK, 90, 280, 60, 30, 0, 0x0, 0 },
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
					
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_LIGHTBLUE);
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, _StringHZ[0]);

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_BACK: // Notifications sent by 'Program'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
			break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = WINDOW_TOP;
        break;
      }
      break;
    }
    break;
	case  WM_PAINT:
				GUI_Clear();
				drawHLineLong((int)(euler.angle_y));
				drawVLineLong(-(int)(euler.angle_x));
		break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateWindow_5(void);
WM_HWIN CreateWindow_5(void) {

	
    hWin_5 = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin_5;
}



/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

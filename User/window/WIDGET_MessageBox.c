/*
*********************************************************************************************************
*
*	模块名称 : 实现消息通知窗口模块
*	文件名称 : WIDGET_MessageBox.c
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
#include  "WIDGET_MessageBox.h"
#include  "SongTi12.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_TEXT_0         (GUI_ID_USER + 0x01)
#define ID_BUTTON_OK      (GUI_ID_USER + 0x02)
#define ID_BUTTON_CANCEL  (GUI_ID_USER + 0x03)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const char *StringHZ[] = {
	"\xe7\xa1\xae\xe5\xae\x9a",//0:确认
	"\xe5\x8f\x96\xe6\xb6\x88",//1:取消
	
};
static uint8_t  ret_val;//用户选择信息
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "MessageBox", ID_FRAMEWIN_0, 0, 0, 100, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 1, 0, 99, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_OK, 1, 20, 40, 19, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CL", ID_BUTTON_CANCEL, 53, 20, 40, 19, 0, 0x0, 0 },

};

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
	WM_HWIN hItem;

  switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		//Initialize of Button
		//
		hItem = WM_GetDialogItem(pMsg->hWin , ID_BUTTON_OK);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem, StringHZ[0]);
	
		hItem = WM_GetDialogItem(pMsg->hWin , ID_BUTTON_CANCEL);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem, StringHZ[1]);
		break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_OK: // Notifications sent by 'OK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
							ret_val = GUI_ID_OK;
							GUI_EndDialog(pMsg->hWin ,0);
        break;
      }
      break;
    case ID_BUTTON_CANCEL: // Notifications sent by 'CL'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						ret_val = GUI_ID_CANCEL;
						GUI_EndDialog(pMsg->hWin ,0);
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
*       _MessageBox
*/
void _MessageBox(const char* pText, const char* pCaption, uint8_t *ret) {
  WM_HWIN hWin;
  WM_HWIN hItem;
	
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 70, 110);
	
	//set the framewin title
	FRAMEWIN_SetFont(hWin, &GUI_FontSongTi12);
	FRAMEWIN_SetText(hWin, pCaption);

	//set the text font
	hItem = WM_GetDialogItem(hWin, ID_TEXT_0);
  TEXT_SetFont(hItem, &GUI_FontSongTi12);
	TEXT_SetText(hItem, pText);
	TEXT_SetTextAlign(hItem, GUI_TA_LEFT);					
	
  WM_MakeModal(hWin);
  GUI_ExecCreatedDialog(hWin);

	*ret = ret_val;
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

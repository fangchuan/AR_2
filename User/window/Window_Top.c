/*
*********************************************************************************************************
*
*	ģ������ : ��һ���棬6��ģ�鹩�û�ѡ��
*	�ļ����� : Window_Top.c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/

#include "Window_Top.h"
#include "os.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern uint8_t Key_Value;
extern OS_SEM  TOUCH_SEM;

WM_HWIN hWin_Top;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x01)
#define ID_BUTTON_0     (GUI_ID_USER + 0x02)
#define ID_BUTTON_1     (GUI_ID_USER + 0x03)
#define ID_BUTTON_2     (GUI_ID_USER + 0x05)
#define ID_BUTTON_3     (GUI_ID_USER + 0x06)
#define ID_BUTTON_4     (GUI_ID_USER + 0x07)
#define ID_BUTTON_5     (GUI_ID_USER + 0x08)

#define ID_TEXT_0     (GUI_ID_USER + 0x09)
#define ID_TEXT_1     (GUI_ID_USER + 0x0A)
#define ID_TEXT_2     (GUI_ID_USER + 0x0B)
#define ID_TEXT_3     (GUI_ID_USER + 0x0C)
#define ID_TEXT_4     (GUI_ID_USER + 0x0D)
#define ID_TEXT_5     (GUI_ID_USER + 0x0E)

#define ID_ICONVIEW_0 (GUI_ID_USER + 0x0F)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char *_acStringHZ[] = {
	"\xe7\xbc\x96\xe7\xa8\x8b",													//0:���
	"\xe8\x93\x9d\xe7\x89\x99",    											//1:����
	"\xe6\x89\x8b\xe5\x8a\xa8\xe6\x8e\xa7\xe5\x88\xb6",	//2:�ֶ����� 
	"\xe9\x81\xa5\xe6\x8e\xa7",           							//3:ң��
	"\xe8\x87\xaa\xe5\xb9\xb3\xe8\xa1\xa1",							//4:��ƽ��
	"\xe8\xa7\xa6\xe6\x91\xb8\xe6\xa0\xa1\xe5\x87\x86", //5:����У׼
};

static const char *_acStringE[]={"LeCoder"};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_0, 20, 60, 80, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_1, 130, 60, 80, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_2, 20, 140, 80, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_3, 130, 140, 80, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_4, 20, 220, 80, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_5, 130, 220, 80, 50, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "2", ID_TEXT_0, 130, 110, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "1", ID_TEXT_1, 20, 110, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "4", ID_TEXT_2, 130, 190, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "3", ID_TEXT_3, 20, 190, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "5", ID_TEXT_4, 20, 270, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "6", ID_TEXT_5, 130, 270, 80, 20, 0, 0x0, 0 },

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
    WINDOW_SetBkColor(hItem, GUI_WHITE);
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetBitmap(hItem,0,&bmbmp_program);
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetBitmap(hItem,0,&bmbmp_bt);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetBitmap(hItem,0,&bmbmp_manual);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
		BUTTON_SetBitmap(hItem,0,&bmbmp_rc);
 
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
		BUTTON_SetBitmap(hItem,0,&bmbmp_sb);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
		BUTTON_SetBitmap(hItem,0,&bmbmp_pc);
    //
    // Initialization of TEXT
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0); // Initialization of '2'
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem,&GUI_FontSongTi16);
		TEXT_SetText(hItem,_acStringHZ[1]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1); // Initialization of '1'
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem,&GUI_FontSongTi16);
		TEXT_SetText(hItem,_acStringHZ[0]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);// Initialization of '4'
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem,&GUI_FontSongTi16);
		TEXT_SetText(hItem,_acStringHZ[3]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3); // Initialization of '3'
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem,&GUI_FontSongTi16);
		TEXT_SetText(hItem,_acStringHZ[2]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4); // Initialization of '5'
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem,&GUI_FontSongTi16);
		TEXT_SetText(hItem,_acStringHZ[4]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);// Initialization of '6'
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem,&GUI_FontSongTi16);
		TEXT_SetText(hItem,_acStringHZ[5]);

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Program'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
			break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 1;
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Bluetooth'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
			break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 2;
        break;
       }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Control'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 3;
        break;
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'RC'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 4;
        break;
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Self Banlance'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
      break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 5;
        break;
      }
      break;
    case ID_BUTTON_5: // Notifications sent by '����У׼'
      if( NCode == WM_NOTIFICATION_RELEASED)
			{
					 OS_ERR  err;
				   Key_Value = 6;
					 OS_SemPost(&TOUCH_SEM, OS_OPT_POST_1, 0, &err);
			}
		
      break;

    }
    break;
	case  WM_PAINT:
		hItem = pMsg->hWin;
		GUI_SetColor(GUI_RED);
		GUI_SetFont(&GUI_FontSongTi16);
		GUI_DispStringHCenterAt(_acStringE[0], 120, 20);
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
WM_HWIN CreateWindow_Top(void);
WM_HWIN CreateWindow_Top(void) {

	
	GUI_SetFont(&GUI_FontSongTi16);
  hWin_Top = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin_Top;
}



/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

//*********************************************************
// in order to create selecte dialog when click edits    **
//*********************************************************

#include "WIDGET_SelectDialog.h"
#include "_apollorobot.h"
#include "SongTi12.h"
#include "WIDGET_MessageBox.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     		 (GUI_ID_USER + 0x00)
#define ID_BUTTON_CHANGE     (GUI_ID_USER + 0x01)
#define ID_BUTTON_INSERT     (GUI_ID_USER + 0x02)
#define ID_BUTTON_DELETE     (GUI_ID_USER + 0x03)
#define ID_BUTTON_BACK       (GUI_ID_USER + 0x04)

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern volatile int Edit_Index;//文本框的索引,用于链表的插入
extern EDIT_Handle hEdit[198];
volatile uint8_t flag_operation;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char *StringHZ[] = {
	"\xe6\x9b\xb4\xe6\x94\xb9\xe6\x8c\x87\xe4\xbb\xa4",	//0:更改指令
	"\xe6\x8f\x92\xe5\x85\xa5\xe6\x8c\x87\xe4\xbb\xa4",	//1:插入指令
	"\xe5\x88\xa0\xe9\x99\xa4\xe6\x8c\x87\xe4\xbb\xa4",	//2:删除指令
	"\xe8\xbf\x94\xe5\x9b\x9e",                      		//3:返回
	"\xe6\x93\x8d\xe4\xbd\x9c\xe9\x94\x99\xe8\xaf\xaf",	//4:操作错误
	"\xe9\x94\x99\xe8\xaf\xaf",                        	//5:错误

};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 100, 160, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "插入指令", ID_BUTTON_INSERT, 10, 10, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "更改指令", ID_BUTTON_CHANGE, 10, 50, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "删除指令", ID_BUTTON_DELETE, 10, 90, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "返回", ID_BUTTON_BACK, 40, 130, 50, 27, 0, 0x0, 0 },

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
  GUI_RECT r;
	WM_HWIN hDlg;
	char string[50] = {0};
	u8   Mb_Val;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x00FF8080);
		//
		//Initialize of "Button"
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CHANGE);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[0]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INSERT);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[1]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DELETE);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[2]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[3]);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_CHANGE: // Notifications sent by 'Change'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						flag_operation = FLAG_CHANGE;
						GUI_EndDialog(pMsg->hWin ,0);//After create the Instruction_Dialog,Distory the SelectDialog
						EDIT_GetText(hEdit[Edit_Index],string,sizeof(string));
						if(string[0] != 0)
						{
							hDlg = CreateWindow_Instructor();
							WM_MakeModal(hDlg);
							GUI_ExecCreatedDialog(hDlg);
						}
						else
						{
							_MessageBox(StringHZ[4],StringHZ[5],&Mb_Val);
						}
						
        break;
      }
      break;
    case ID_BUTTON_INSERT: // Notifications sent by 'Insert'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						flag_operation = FLAG_INSERT;
						GUI_EndDialog(pMsg->hWin ,0);//After create the Instruction_Dialog,Distory the SelectDialog
			      EDIT_GetText(hEdit[Edit_Index],string,sizeof(string));
			      if(string[0] != 0)
						{
							 _MessageBox(StringHZ[4],StringHZ[5],&Mb_Val);
						}
						else{
								hDlg = CreateWindow_Instructor();
								WM_MakeModal(hDlg);
								GUI_ExecCreatedDialog(hDlg);
						}
        break;
      }
      break;
    case ID_BUTTON_DELETE: // Notifications sent by 'Delete'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						flag_operation = FLAG_DELETE;
						GUI_EndDialog(pMsg->hWin ,0);//After create the Instruction_Dialog,Distory the SelectDialog
			      EDIT_GetText(hEdit[Edit_Index],string,sizeof(string));
			      if(string[0] != 0)
						{
							 if(!Delete_Node(Edit_Index)){ //删除成功
								 //被删除行下所有文本行都要往上移一行
								 int i = 1;
								 do{
										EDIT_GetText(hEdit[Edit_Index+i],string,sizeof(string));
									  if(string[0]!=0){
											EDIT_SetText(hEdit[Edit_Index+i-1], string);
											
										}
										else{//最后一行文本清零
											EDIT_SetText(hEdit[Edit_Index+i-1],"");
											EDIT_SetBkColor(hEdit[Edit_Index+i-1],EDIT_CI_ENABLED,GUI_WHITE);
											break;
										}
								 }while(i++);
							 
							 }

						}
						else
						{
							 _MessageBox(StringHZ[4],StringHZ[5],&Mb_Val);
						}

        break;
      }
      break;
    case ID_BUTTON_BACK: // Notifications sent by 'BACK'
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

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

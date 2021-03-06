/*
*********************************************************************************************************
*
*	模块名称 : 实现编辑面板窗口模块头文件
*	文件名称 : WIDGET_EDITPad.h
*	版    本 : V1.0
*	说    明 : 由各个指令按钮调用，跟NumPad略有不同
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/

#include "WIDGET_EDITPad.h"
#include "_apollorobot.h"
#include "common.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern char _acText[50];
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSongTi12;
extern WM_HWIN 	 hWin_Instructor;

extern enum _FLAG _flag;//指令标志
extern volatile int Edit_Index ;//当前EDIT的索引
extern volatile uint8_t flag_operation;//操作标志

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     	(GUI_ID_USER + 15)
#define ID_EDIT_0     		(GUI_ID_USER + 14)
#define ID_BUTTON_0				(GUI_ID_USER + 0)
#define ID_BUTTON_1				(GUI_ID_USER + 1)
#define ID_BUTTON_2				(GUI_ID_USER + 2)
#define ID_BUTTON_3				(GUI_ID_USER + 3)
#define ID_BUTTON_4				(GUI_ID_USER + 4)
#define ID_BUTTON_5				(GUI_ID_USER + 5)
#define ID_BUTTON_6				(GUI_ID_USER + 6)
#define ID_BUTTON_7				(GUI_ID_USER + 7)
#define ID_BUTTON_8				(GUI_ID_USER + 8)
#define ID_BUTTON_9				(GUI_ID_USER + 9)
#define ID_BUTTON_DOT			(GUI_ID_USER + 10)
#define ID_BUTTON_DEL     (GUI_ID_USER + 11)
#define ID_BUTTON_OK  		(GUI_ID_USER + 12)
#define ID_BUTTON_CANCEL 	(GUI_ID_USER + 13)
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const char *StringHZ[] = {
	"\xe7\xa1\xae\xe5\xae\x9a",//0:确定
	"\xe8\xbf\x94\xe5\x9b\x9e",//1:返回
};

static int8_t  pos_1;//第一个下划线位置
static int8_t  pos_2;//第二个下划线位置
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 110, 180, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 0, 2, 108, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect,   "7",      ID_BUTTON_7,   5,   25,  30,  25},
  { BUTTON_CreateIndirect,   "8",      ID_BUTTON_8,  40,   25,  30,  25},
  { BUTTON_CreateIndirect,   "9",      ID_BUTTON_9,  75,   25,  30,  25},
  { BUTTON_CreateIndirect,   "4",      ID_BUTTON_4,   5,  55,  30,  25},
  { BUTTON_CreateIndirect,   "5",      ID_BUTTON_5,  40,  55,  30,  25},
  { BUTTON_CreateIndirect,   "6",      ID_BUTTON_6,  75,  55,  30,  25},
  { BUTTON_CreateIndirect,   "1",      ID_BUTTON_1,   5,  85,  30,  25},
  { BUTTON_CreateIndirect,   "2",      ID_BUTTON_2,  40,  85,  30,  25},
  { BUTTON_CreateIndirect,   "3",      ID_BUTTON_3,  75,  85,  30,  25},
  { BUTTON_CreateIndirect,   "0",      ID_BUTTON_0,   5, 115,  30,  25},
  { BUTTON_CreateIndirect,   ".",      ID_BUTTON_DOT,40, 115,  30,  25},
  { BUTTON_CreateIndirect,   "Del",    ID_BUTTON_DEL,75, 115,  30,  25},
  { BUTTON_CreateIndirect,   "OK",     ID_BUTTON_OK,  5, 151,  45,  25},
  { BUTTON_CreateIndirect,   "Cancel", ID_BUTTON_CANCEL,  60, 151, 45,  25},
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
	WM_HWIN hEdit;
	GUI_RECT r;
  int     NCode;
  int     Id;
  int			 i;
//	int     cursor_pos;

	hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Edit'
    //
    EDIT_SetFont(hEdit, &GUI_FontSongTi12);
		EDIT_SetText(hEdit,_acText);
	  EDIT_SetBkColor(hEdit, EDIT_CI_ENABLED, GUI_CYAN);
		EDIT_SetFocussable(hEdit,1);
	  EDIT_SetSel(hEdit, pos_1, pos_1);
		//
		//Initialize of Button
		//
    for (i = 0; i < 14; i++) {
      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_USER + i);
      BUTTON_SetFocussable(hItem, 0);                       /* Set all buttons non focussable */
    }
		
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_USER + 12);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[0]);
		
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_USER + 13);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[1]);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				if( Id < GUI_ID_USER + 12)
				{
						int Key;
						char temp[50];
						if (Id < GUI_ID_USER + 11)    ///0~9数字按键
						{
								char acBuffer[10];
								BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); /* Get the text of the button */
								Key = acBuffer[0];
						}
						else
						{
								Key = GUI_KEY_BACKSPACE;   ///Del按键
						}
						/* Send a key message to the focussed window */
						GUI_SendKeyMsg(Key, 1);      
						EDIT_GetText(hEdit, temp, sizeof(temp));
						if(!string_subs_char(temp))						//减去当前文本框中的'_'
						{
							EDIT_SetText(hEdit, temp);
							if(pos_2 == -1)
							{
								//只有一个'_'的情况
								EDIT_SetCursorAtChar(hEdit, ++pos_1);//将光标移到第一个'_'后一位
							}
							//有两个'_'的情况
							EDIT_SetSel(hEdit, pos_2, pos_2);	//选择下一个'_'位置						
							pos_2 ++;
						}
						
				}
				else
				{
					if (Id == ID_BUTTON_OK)       ///点确定按钮后将EDIT内容复制到_acText中
					{
							char dest[50];
							EDIT_GetText(hEdit,dest,sizeof(dest));
							strcpy(_acText,dest);
							if(flag_operation == FLAG_INSERT){ //如果是顺序添加下去的话，则在链表尾部添加新的结点
									Add_Node(Ins_List_Head, Edit_Index ,_flag,dest );
							}
							//在已经编辑好的文本上更改内容,则直接在该索引的链表结点上修改数据域
							if( flag_operation == FLAG_CHANGE){
									Replace_Node(Edit_Index,_flag,dest);
							}
								
							GUI_EndDialog(pMsg->hWin ,0);
							GUI_EndDialog(hWin_Instructor,0);//点击确定按钮后，直接将Instructor面板及其子面板都关闭
	
					}
					if (Id == ID_BUTTON_CANCEL)
					{
							memset(_acText,0 ,sizeof(_acText));
							GUI_EndDialog(pMsg->hWin, 0);
					}
					if (Id == ID_EDIT_0)
					{
							
					}
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
*       CreateWindow
*/
WM_HWIN Create_EDITPad(WM_HWIN hParent, int8_t pos1, int8_t pos2);
WM_HWIN Create_EDITPad(WM_HWIN hParent, int8_t pos1, int8_t pos2) {
  WM_HWIN hWin;

	pos_1 = pos1;
	pos_2 = pos2;
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hParent, 105, 20);
  return hWin;
}


/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

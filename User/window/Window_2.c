/*
*********************************************************************************************************
*
*	模块名称 : 蓝牙连接窗口模块
*	文件名称 : Window_2.c
*	版    本 : V1.0
*	说    明 : 显示四个端口当前状态，超声波传感器状态、姿态传感器状态
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#include "Window_2.h"
#include "SongTi12.h"
#include "_apollorobot.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern uint8_t Key_Value;
extern _Port   port_1;
extern _Port   port_2;
extern _Port   port_3;
extern _Port   port_4;
extern _Ultrasnio  ult;
extern _Euler    euler;

WM_HWIN hWin_2;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     	 			 (GUI_ID_USER + 0x00)
#define ID_BUTTON_BACK     			 (GUI_ID_USER + 0x02)
#define ID_HEADER_1     	 			 (GUI_ID_USER + 0x03)
#define ID_LISTVIEW_0   	 			 (GUI_ID_USER + 0x04)
#define ID_BUTTON_DISCONNECT     (GUI_ID_USER + 0x05)
#define ID_TEXT_EXP     				 (GUI_ID_USER + 0x06)
#define ID_TEXT_ULT     				 (GUI_ID_USER + 0x07)
#define ID_TEXT_MPU     				 (GUI_ID_USER + 0x08)
#define ID_TEXT_EULER   				 (GUI_ID_USER + 0x09)
#define ID_TEXT_GYRO    				 (GUI_ID_USER + 0x0A)
#define ID_TEXT_ACCEL   				 (GUI_ID_USER + 0x0B)
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
	"\xe4\xbc\xa0\xe6\x84\x9f\xe5\x99\xa8\xe7\x8a\xb6\xe6\x80\x81",//3:传感器状态
	"\xe7\xab\xaf\xe5\x8f\xa3","\xe6\x97\xa0",       //4:端口     5:无
	"\xe8\xb6\x85\xe5\xa3\xb0\xe6\xb3\xa2\xe4\xbc\xa0\xe6\x84\x9f\xe5\x99\xa8:",//6:超声波传感器
	"\xe5\xa7\xbf\xe6\x80\x81\xe4\xbc\xa0\xe6\x84\x9f\xe5\x99\xa8",//7:姿态传感器
	"\xe8\xa7\x92\xe5\xba\xa6:",//8:角度
	"\xe8\xa7\x92\xe9\x80\x9f\xe5\xba\xa6:",//9:角速度
	"\xe5\x8a\xa0\xe9\x80\x9f\xe5\xba\xa6:",//10:加速度
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
//  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 0, 300, 240, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "返回", ID_BUTTON_BACK, 0, 290, 80, 30, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_1, 0, 0, 240, 20, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 1, 40, 239, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "断开", ID_BUTTON_DISCONNECT, 160, 290, 80, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect,   "传感器状态",  ID_TEXT_EXP, 0,  25,  80, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "ULTRASNIO", ID_TEXT_ULT, 1, 100, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "MPU6050", ID_TEXT_MPU, 1, 145, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "EULER:", ID_TEXT_EULER, 1, 165, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "ACCEL:", ID_TEXT_ACCEL, 1, 185, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GYRO:",  ID_TEXT_GYRO, 1, 205, 40, 20, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static LISTVIEW_Handle hList;

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int        Id;
  char  str[7] = {0};

	
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		//
		//Initialization of windows
		//
		hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_LIGHTBLUE);
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, StringHZ[0]);
	    
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
//    BUTTON_SetText(hItem, StringHZ[2]);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DISCONNECT);
    BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem, StringHZ[1]);
		//
		//Initialization of Explantion Text
		//
	  hItem = WM_GetDialogItem(pMsg->hWin ,ID_TEXT_EXP);
		TEXT_SetFont(hItem, &GUI_FontSongTi12);
	  TEXT_SetText(hItem, StringHZ[3]);
		
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_TEXT_ULT);
		TEXT_SetFont(hItem, &GUI_FontSongTi12);
	  TEXT_SetText(hItem, StringHZ[6]);
		
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_TEXT_MPU);
		TEXT_SetFont(hItem, &GUI_FontSongTi12);
	  TEXT_SetText(hItem, StringHZ[7]);
		
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_TEXT_EULER);
		TEXT_SetFont(hItem, &GUI_FontSongTi12);
	  TEXT_SetText(hItem, StringHZ[8]);
		
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_TEXT_GYRO);
		TEXT_SetFont(hItem, &GUI_FontSongTi12);
	  TEXT_SetText(hItem, StringHZ[9]);
		
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_TEXT_ACCEL);
		TEXT_SetFont(hItem, &GUI_FontSongTi12);
	  TEXT_SetText(hItem, StringHZ[10]);
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_1);
    HEADER_AddItem(hItem, 80, "LeCoder", 14);
    //
    // Initialization of 'Listview'
    //
		hList = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
		LISTVIEW_SetFont(hList, &GUI_FontSongTi12);
//		sprintf(str,"%s%s",StringHZ[4],"1");
    LISTVIEW_AddColumn(hList, 60, "Port1", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hList, 60, "Port2", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hList, 60, "Port3", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hList, 60, "Port4", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddRow(hList, NULL);
    LISTVIEW_SetGridVis(hList, 1);
		LISTVIEW_SetRowHeight(hList, 29);
		
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_BACK: // Notifications sent by 'BACK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = 0;
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

    case ID_BUTTON_DISCONNECT: // Notifications sent by '断开'
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
       if(port_1.status)
			 {
				 if(port_1.species == DS)
				 {
					 if(port_1.cur_val == 1)
					 LISTVIEW_SetItemText(hList, 0, 0, "1");
				   else
					 LISTVIEW_SetItemText(hList, 0, 0, "0");
				 }
				 if(port_1.species == AS)
				 {
					 if(port_1.cur_val )
					 {
						 sprintf(str, "%d",port_1.cur_val);
						 LISTVIEW_SetItemText(hList, 0, 0, str);
					 }
				 }
			 }
			 else
			 {
//				 	sprintf(str, "%s",StringHZ[5]);
					LISTVIEW_SetItemText(hList, 0, 0, "0");
			 }
			  
			 if(port_2.status)
			 {
				 if(port_2.species == DS)
				 {
					 if(port_2.cur_val == 1)
					 LISTVIEW_SetItemText(hList, 1, 0, "1");
				   else
					 LISTVIEW_SetItemText(hList, 1, 0, "0");
				 }
				 if(port_2.species == AS)
				 {
					 if(port_2.cur_val )
					 {
						 sprintf(str, "%d",port_2.cur_val);
						 LISTVIEW_SetItemText(hList, 1, 0, str);
					 }
				 }
			 }
			 else
			 {
//				 	sprintf(str, "%s",StringHZ[5]);
					LISTVIEW_SetItemText(hList, 1, 0, "0");
			 }
			 
			  
			 if(port_3.status)
			 {
				 if(port_3.species == DS)
				 {
					 if(port_3.cur_val == 1)
					 LISTVIEW_SetItemText(hList, 2, 0, "1");
				   else
					 LISTVIEW_SetItemText(hList, 2, 0, "0");
				 }
				 if(port_3.species == AS)
				 {
					 if(port_3.cur_val )
					 {
						 sprintf(str, "%d",port_3.cur_val);
						 LISTVIEW_SetItemText(hList, 2, 0, str);
					 }
				 }
			 }
			 else
			 {
//				 	sprintf(str, "%s",StringHZ[5]);
					LISTVIEW_SetItemText(hList, 2, 0, "0");
			 }
			 
			 if(port_4.status)
			 {
				 if(port_4.species == DS)
				 {
					 if(port_4.cur_val == 1)
					 LISTVIEW_SetItemText(hList, 3, 0, "1");
				   else
					 LISTVIEW_SetItemText(hList, 3, 0, "0");
				 }
				 if(port_4.species == AS)
				 {
					 if(port_4.cur_val )
					 {
						 sprintf(str, "%d",port_4.cur_val);
						 LISTVIEW_SetItemText(hList, 3, 0, str);
					 }
				 }
			 }
			 else
			 {
//				 	sprintf(str, "%s",StringHZ[5]);
					LISTVIEW_SetItemText(hList, 3, 0, "0");
			 }
			 
			 if(ult.cur_distance)
			 {
				  sprintf(str, "%d",(int)ult.cur_distance );
				  GUI_SetColor(GUI_RED);
				  GUI_DispStringAt(str, 80,103);
			 }
			 else
			 {
				  GUI_SetColor(GUI_RED);
				  GUI_DispStringAt("0cm", 80,103);
			 }
			 
			 GUI_GotoXY(50, 170);
			 GUI_DispSFloatMin(euler.angle_x ,2);//角度显示到小数点后2位
			 GUI_GotoXY(100, 170);
			 GUI_DispSFloatMin(euler.angle_y ,2);//
			 GUI_GotoXY(50, 190);
			 GUI_DispSFloatMin(euler.accel_x ,4);//加速度显示到小数点后4位
			 GUI_GotoXY(100, 190);
			 GUI_DispSFloatMin(euler.accel_y ,4);//
			 GUI_GotoXY(150, 190);
			 GUI_DispSFloatMin(euler.accel_z ,4);//
			 GUI_GotoXY(50, 210);
			 GUI_DispSFloatMin(euler.gyro_x , 4);//角速度显示到小数点后4位
			 GUI_GotoXY(100, 210);
			 GUI_DispSFloatMin(euler.gyro_y , 4);//
			 GUI_GotoXY(150, 210);
			 GUI_DispSFloatMin(euler.gyro_z , 4);//

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


/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

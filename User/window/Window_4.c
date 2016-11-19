/*
*********************************************************************************************************
*
*	模块名称 : 遥控器连接窗口模块
*	文件名称 : Window_4.c
*	版    本 : V1.0
*	说    明 : 提供链接12个通道的遥控器的连接功能和断开功能
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/

#include "Window_4.h"
#include "stdio.h"
#include "SongTi12.h"
#include "WIDGET_MessageBox.h"
#include "Window_Running.h"
#include "bsp_spi_nrf.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/

extern volatile uint8_t  Key_Value;
extern __IO u8  NRF_ADDRESS[5];//NRF24L01的地址

u8 flag_nrf_link = 0;
WM_HWIN           hWin_4;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     				 (GUI_ID_USER + 0x00)
#define ID_HEADER_0     				 (GUI_ID_USER + 0x01)
#define ID_BUTTON_DISCONNECT     (GUI_ID_USER + 0x03)
#define ID_BUTTON_CONNECT    	   (GUI_ID_USER + 0x04)
#define ID_RADIO_0      				 (GUI_ID_USER + 0x07)
#define ID_TEXT_EXP     				 (GUI_ID_USER + 0x08)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char *StringHZ[] = {
	"\xe8\xbf\x9e\xe6\x8e\xa5","\xe6\x96\xad\xe5\xbc\x80",//0:连接，1:断开
	"\xe8\xaf\xb7\xe9\x80\x89\xe6\x8b\xa9\xe9\x81\xa5\xe6\x8e\xa7\xe5\x99\xa8\xe9\x80\x9a\xe9\x81\x93:",//2:请选择遥控器通道
	"\xe8\xbf\x9e\xe6\x8e\xa5\xe6\x88\x90\xe5\x8a\x9f",//3:连接成功
	"\xe9\x80\x9a\xe9\x81\x93",//4:通道
	"\xe8\xbf\x9e\xe6\x8e\xa5\xe9\x94\x99\xe8\xaf\xaf",//5:连接错误
	"\xe9\x94\x99\xe8\xaf\xaf",//6:错误
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "HeaderTop", ID_HEADER_0, 0, 0, 240, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "断开", ID_BUTTON_DISCONNECT, 0, 290, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "连接", ID_BUTTON_CONNECT, 160, 290, 80, 30, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_0, 5, 40, 80, 240, 0, 0x140c, 0 },
  { TEXT_CreateIndirect,  "请选择遥控器通道",ID_TEXT_EXP,0, 25, 100,20,0, 0X0, 0}
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
  char    str[12] = {0};
	int     nrf_channel;
	u8      Mb_Val;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		//
		//Initialization of windows
		//
		hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_LIGHTBLUE);
    //
    // Initialization of 'HeaderTop'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_0);
    HEADER_AddItem(hItem, 80, "LeCoder", 14);
    //
    // Initialization of 'Radio'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
		RADIO_SetFont(hItem, &GUI_FontSongTi12);
		sprintf(str, "%s%s",StringHZ[4],"1");
    RADIO_SetText(hItem, str, 0);
		sprintf(str, "%s%s",StringHZ[4],"2");
    RADIO_SetText(hItem, str, 1);
		sprintf(str, "%s%s",StringHZ[4],"3");
    RADIO_SetText(hItem, str, 2);
		sprintf(str, "%s%s",StringHZ[4],"4");
    RADIO_SetText(hItem, str, 3);
		sprintf(str, "%s%s",StringHZ[4],"5");
    RADIO_SetText(hItem, str, 4);
		sprintf(str, "%s%s",StringHZ[4],"6");
    RADIO_SetText(hItem, str, 5);
		sprintf(str, "%s%s",StringHZ[4],"7");
    RADIO_SetText(hItem, str, 6);
		sprintf(str, "%s%s",StringHZ[4],"8");
    RADIO_SetText(hItem, str, 7);
		sprintf(str, "%s%s",StringHZ[4],"9");
    RADIO_SetText(hItem, str, 8);
		sprintf(str, "%s%s",StringHZ[4],"10");
    RADIO_SetText(hItem, str, 9);
		sprintf(str, "%s%s",StringHZ[4],"11");
    RADIO_SetText(hItem, str, 10);
		sprintf(str, "%s%s",StringHZ[4],"12");
    RADIO_SetText(hItem, str, 11);
		//
		//Initialize of Button
		//
    hItem = WM_GetDialogItem(pMsg->hWin ,ID_BUTTON_DISCONNECT);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[1]);
		
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_BUTTON_CONNECT);
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[0]);
		//
		//Initialization of Explanation Text
		//
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_TEXT_EXP);
		TEXT_SetFont(hItem, &GUI_FontSongTi12);
		TEXT_SetText(hItem, StringHZ[2]);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {

    case ID_BUTTON_DISCONNECT: // Notifications sent by 'Disconnect'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = WINDOW_TOP;
			      flag_nrf_link = 0;
        break;
      }
      break;
			case ID_BUTTON_CONNECT: // Notifications sent by 'Connect'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						//选择NRF通道，共8个通道，其实是更改NRF的地址
						hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
						nrf_channel = RADIO_GetValue(hItem) ;
						if(nrf_channel < 6)
						{
							NRF_ADDRESS[4] = nrf_channel + 0x04;
						}
						else
						{
							 NRF_ADDRESS[4] = nrf_channel + 0x0A;
						}
						NRF_RX_Mode();
						if(NRF_Rx_Dat((u8 *)str) == RX_DR)
						{
							 RADIO_GetText(hItem, nrf_channel,str,10);
							 _MessageBox(str,StringHZ[3], &Mb_Val);
							 Create_RunningWindow();
							 flag_nrf_link =  1;
							 
						}
						else
						{
							 _MessageBox(StringHZ[5],StringHZ[6], &Mb_Val);
						}
        break;
      }
      break;
    case ID_RADIO_0: // Notifications sent by 'Radio'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    }
    break;
	case WM_PAINT:
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
WM_HWIN CreateWindow_4(void);
WM_HWIN CreateWindow_4(void) {

  hWin_4 = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin_4;
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

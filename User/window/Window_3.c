/*
*********************************************************************************************************
*
*	模块名称 : 手动控制界面
*	文件名称 : Window_3.c
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

#include "Window_3.h"
#include "SongTi12.h"
#include "bsp_motor.h"
#include "bsp_servo.h"
#include "bsp_led.h"
/*********************************************************************
*
*       Global  data
*
**********************************************************************
*/
extern volatile uint8_t Key_Value;
WM_HWIN hWin_3;
WM_HWIN hDialog_Page1;
WM_HWIN hDialog_Page2;
WM_HWIN hDialog_Page3;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0        (GUI_ID_USER + 0x00)
#define ID_HEADER_0        (GUI_ID_USER + 0x02)
#define ID_BUTTON_BACK     (GUI_ID_USER + 0x04)
#define ID_MULTIPAGE_0     (GUI_ID_USER + 0x05)
#define ID_TEXT_M1         (GUI_ID_USER + 0x06)
#define ID_TEXT_M2         (GUI_ID_USER + 0x07)
#define ID_BUTTON_M1C      (GUI_ID_USER + 0x08)
#define ID_BUTTON_M1CC     (GUI_ID_USER + 0x09)
#define ID_BUTTON_M2C      (GUI_ID_USER + 0x0A)
#define ID_BUTTON_M2CC     (GUI_ID_USER + 0x0B)

#define ID_TEXT_S1         (GUI_ID_USER + 0x0C)
#define ID_TEXT_S2         (GUI_ID_USER + 0x0D)
#define ID_TEXT_S3         (GUI_ID_USER + 0x0E)
#define ID_TEXT_S4         (GUI_ID_USER + 0x0F)
#define ID_BUTTON_S1_0    (GUI_ID_USER + 0x10)
#define ID_BUTTON_S1_90    (GUI_ID_USER + 0x11)
#define ID_BUTTON_S1_180   (GUI_ID_USER + 0x12)
#define ID_BUTTON_S2_0    (GUI_ID_USER + 0x13)
#define ID_BUTTON_S2_90    (GUI_ID_USER + 0x14)
#define ID_BUTTON_S2_180   (GUI_ID_USER + 0x15)
#define ID_BUTTON_S3_0    (GUI_ID_USER + 0x16)
#define ID_BUTTON_S3_90    (GUI_ID_USER + 0x17)
#define ID_BUTTON_S3_180   (GUI_ID_USER + 0x18)
#define ID_BUTTON_S4_0    (GUI_ID_USER + 0x19)
#define ID_BUTTON_S4_90    (GUI_ID_USER + 0x1A)
#define ID_BUTTON_S4_180   (GUI_ID_USER + 0x1B)

#define ID_TEXT_P1         (GUI_ID_USER + 0x1C)
#define ID_BUTTON_P1_ON    (GUI_ID_USER + 0x1D)
#define ID_BUTTON_P1_OFF   (GUI_ID_USER + 0x1E)
#define ID_TEXT_P2         (GUI_ID_USER + 0x1F)
#define ID_BUTTON_P2_ON    (GUI_ID_USER + 0x20)
#define ID_BUTTON_P2_OFF   (GUI_ID_USER + 0x21)
#define ID_TEXT_P3         (GUI_ID_USER + 0x22)
#define ID_BUTTON_P3_ON    (GUI_ID_USER + 0x23)
#define ID_BUTTON_P3_OFF   (GUI_ID_USER + 0x24)
#define ID_TEXT_P4         (GUI_ID_USER + 0x25)
#define ID_BUTTON_P4_ON    (GUI_ID_USER + 0x26)
#define ID_BUTTON_P4_OFF   (GUI_ID_USER + 0x27)
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const char *StringHZ[] = {
	"\xe8\xbf\x94\xe5\x9b\x9e",                              //0:返回
	"\xe7\x94\xb5\xe6\x9c\xba",     												 //1:电机 
	"\xe8\x88\xb5\xe6\x9c\xba",                              //2:舵机
	"\xe6\xad\xa3\xe8\xbd\xac",                              //3:正转
	"\xe5\x8f\x8d\xe8\xbd\xac",                              //4:反转
	"\xe5\xa4\x8d\xe4\xbd\x8d",															 //5:复位
	"90\xe5\xba\xa6","180\xe5\xba\xa6",     								 //6:90度  7:180度
	"\xe7\xab\xaf\xe5\x8f\xa3\xe6\x8e\xa7\xe5\x88\xb6",			 //8:端口控制
	"LED\xe6\x89\x93\xe5\xbc\x80",													 //9:LED打开
	"LED\xe5\x85\xb3\xe9\x97\xad",													 //10：LED关闭
	"\xe7\xab\xaf\xe5\x8f\xa3",															 //11:端口
	"\xe7\x94\xb5\xe6\x9c\xba\xe6\x8e\xa7\xe5\x88\xb6",   	 //12:电机控制
	"\xe8\x88\xb5\xe6\x9c\xba\xe6\x8e\xa7\xe5\x88\xb6",   	 //13:舵机控制
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "HeaderTop", ID_HEADER_0, 0, 0, 240, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "返回", ID_BUTTON_BACK, 0, 290, 80, 30, 0, 0x0, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 0, 20, 240, 270, 0, 0x0, 0 },

};
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage1[] = {
  { WINDOW_CreateIndirect,    "Dialog 1", 0, 0,   0, 240, 280, FRAMEWIN_CF_MOVEABLE },
	{ TEXT_CreateIndirect,   "电机1",         ID_TEXT_M1, 2, 30, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "motor1forward", ID_BUTTON_M1C, 20, 70, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "motor1reverse", ID_BUTTON_M1CC, 130, 70, 80, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,   "电机2",         ID_TEXT_M2, 2, 130, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "motor2forward", ID_BUTTON_M2C, 20, 160, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "motor2reverse", ID_BUTTON_M2CC, 130, 160, 80, 30, 0, 0x0, 0 },
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage2[] = {
  { WINDOW_CreateIndirect,    "Dialog 2", 0,             0,   0, 240, 280, FRAMEWIN_CF_MOVEABLE },	
	{ TEXT_CreateIndirect,   "舵机1",       ID_TEXT_S1,     2, 10, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机1转60度", ID_BUTTON_S1_0, 10, 30, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机1转90度", ID_BUTTON_S1_90, 90, 30, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机1转120度",ID_BUTTON_S1_180,170, 30, 60, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,   "舵机2",       ID_TEXT_S2, 		 2, 70, 80, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "舵机2转60度", ID_BUTTON_S2_0,  10, 90, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机2转90度", ID_BUTTON_S2_90, 90, 90, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机2转120度",ID_BUTTON_S2_180,170, 90, 60, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,   "舵机3",       ID_TEXT_S3, 		 2, 130, 80, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "舵机3转60度", ID_BUTTON_S3_0,   10, 150, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机3转90度", ID_BUTTON_S3_90,  90, 150, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机3转120度",ID_BUTTON_S3_180, 170, 150, 60,30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,   "舵机4",       ID_TEXT_S4, 		  2, 190, 80, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "舵机4转60度", ID_BUTTON_S4_0, 	  10, 210, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机4转90度", ID_BUTTON_S4_90,   90, 210, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机4转120度",ID_BUTTON_S4_180,  170, 210, 60, 30, 0, 0x0, 0 },
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage3[] = {
  { WINDOW_CreateIndirect,  "Dialog 3", 0,              0,   0, 240, 280, FRAMEWIN_CF_MOVEABLE },	
	{ TEXT_CreateIndirect,   "端口1",     ID_TEXT_P1,      2, 10, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "打开",      ID_BUTTON_P1_ON, 30, 30, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "关闭",      ID_BUTTON_P1_OFF,150, 30, 60, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,   "端口2",     ID_TEXT_P2,      2, 70, 80, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "打开",      ID_BUTTON_P2_ON, 30, 90, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "关闭",      ID_BUTTON_P2_OFF,150, 90, 60, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,   "端口3",     ID_TEXT_P3,      2, 130, 80, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "打开",      ID_BUTTON_P3_ON, 30, 150, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "关闭",      ID_BUTTON_P3_OFF,150, 150, 60, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,   "端口4",     ID_TEXT_P4,      2, 190, 80, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "打开",      ID_BUTTON_P4_ON, 30, 210, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "关闭",      ID_BUTTON_P4_OFF,150, 210, 60, 30, 0, 0x0, 0 },
};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbServo
*/
static void _cbServo (WM_MESSAGE * pMsg)
{
	 	  WM_HWIN  hItem;
	    int      NCode;
			int         Id;
			char   str[10];
			switch(pMsg->MsgId)
			{
				case WM_INIT_DIALOG:
					//
					//Intialization of Text
					//
				  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_S1);
					TEXT_SetFont(hItem, &GUI_FontSongTi12);
					strcpy(str, StringHZ[2]);
				  strcat(str,  "1");
				  TEXT_SetText(hItem, str);
				
					hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_S2);
					TEXT_SetFont(hItem, &GUI_FontSongTi12);
				  strcpy(str, StringHZ[2]);
				  strcat(str,  "2");
				  TEXT_SetText(hItem, str);
				
					hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_S3);
					TEXT_SetFont(hItem, &GUI_FontSongTi12);
				  strcpy(str, StringHZ[2]);
				  strcat(str,  "3");
				  TEXT_SetText(hItem, str);
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_S4);
					TEXT_SetFont(hItem, &GUI_FontSongTi12);
				  strcpy(str, StringHZ[2]);
				  strcat(str,  "4");
				  TEXT_SetText(hItem, str);
					//
					// Initialization of Button
					//
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S1_0);
				  BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[5]);

					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S1_90);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[6]);

					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S1_180);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[7]);

					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S2_0);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[5]);	
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S2_90);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[6]);	
				
				  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S2_180);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[7]);	
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S3_0);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[5]);	
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S3_90);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[6]);	
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S3_180);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[7]);	
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S4_0);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[5]);	
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S4_90);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[6]);	
					
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S4_180);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[7]);	
					break;
				case WM_NOTIFY_PARENT:
					   Id    = WM_GetId(pMsg->hWinSrc);
						 NCode = pMsg->Data.v;
				     switch(Id)
						 {
							 case ID_BUTTON_S1_0:
								switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										    SERVO_Manual( 1, 0);
										break;
									}
								 break;
							 case ID_BUTTON_S1_90:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual( 1, 90);
										break;
									}
								 break;
							 case ID_BUTTON_S1_180:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual( 1, 180);
										break;
									}
								 break;
							 case ID_BUTTON_S2_0:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(2, 0);
										break;
									}
								 break;
							 case ID_BUTTON_S2_90:
								switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(2, 90);
										break;
									}
								 break;
							 case ID_BUTTON_S2_180:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(2, 180);
										break;
									}
								 break;
							 case ID_BUTTON_S3_0:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(3, 0);
										break;
									}
								 break;
							 case ID_BUTTON_S3_90:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										    SERVO_Manual(3, 90);
										break;
									}
								 break;
						   case ID_BUTTON_S3_180:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(3, 180);
										break;
									}
								 break;
							 case ID_BUTTON_S4_0:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(4, 0);
										break;
									}
								 break;
							 case ID_BUTTON_S4_90:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(4, 90);
										break;
									}
								 break;
						   case ID_BUTTON_S4_180:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										
										break;
									case WM_NOTIFICATION_RELEASED:
										     SERVO_Manual(4, 180);
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
*       _cbMotor
*/
static void _cbMotor (WM_MESSAGE * pMsg)
{
	    WM_HWIN  hItem;
	    int      NCode;
			int         Id;
			char   str[10];
			switch(pMsg->MsgId)
			{
				case WM_INIT_DIALOG:
					//
					//Intialization of Text
					//
				  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_M1);
					TEXT_SetFont(hItem, &GUI_FontSongTi12);
					strcpy(str, StringHZ[1]);
				  strcat(str,  "1");
				  TEXT_SetText(hItem, str);
				
					hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_M2);
					TEXT_SetFont(hItem, &GUI_FontSongTi12);
				  strcpy(str, StringHZ[1]);
				  strcat(str,  "2");
				  TEXT_SetText(hItem, str);
					//
					// Initialization of Button
					//
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_M1C);
				  BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[3]);

					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_M1CC);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[4]);

					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_M2C);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[3]);

					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_M2CC);
					BUTTON_SetFont(hItem, &GUI_FontSongTi12);
					BUTTON_SetText(hItem, StringHZ[4]);	
				
					break;
				case WM_NOTIFY_PARENT:
					   Id    = WM_GetId(pMsg->hWinSrc);
						 NCode = pMsg->Data.v;
				     switch(Id)
						 {
							 case ID_BUTTON_M1C:
								switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										     MOTOR_1_Forward();
										break;
									case WM_NOTIFICATION_RELEASED:
										     MOTOR_1_Stop();
										break;
									}
								 break;
							 case ID_BUTTON_M1CC:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										    MOTOR_1_Reverse();
										break;
									case WM_NOTIFICATION_RELEASED:
										    MOTOR_1_Stop();
										break;
									}
								 break;
							 case ID_BUTTON_M2C:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										     MOTOR_2_Forward();
										break;
									case WM_NOTIFICATION_RELEASED:
										     MOTOR_2_Stop();
										break;
									}
								 break;
							 case ID_BUTTON_M2CC:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
										     MOTOR_2_Reverse();
										break;
									case WM_NOTIFICATION_RELEASED:
										     MOTOR_2_Stop();
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
*       _cbPort
*/
static void _cbPort (WM_MESSAGE * pMsg)
{
	  WM_HWIN  hItem;
		int      NCode;
		int         Id;
		char   str[10] = {0};
		switch(pMsg->MsgId)
		{
			case WM_INIT_DIALOG:
				//
				//Intialization of Text
				//
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_P1);
				TEXT_SetFont(hItem, &GUI_FontSongTi12);
				strcpy(str, StringHZ[11]);
				strcat(str,  "1");
				TEXT_SetText(hItem, str);
			
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_P2);
				TEXT_SetFont(hItem, &GUI_FontSongTi12);
				strcpy(str, StringHZ[11]);
				strcat(str,  "2");
				TEXT_SetText(hItem, str);
			
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_P3);
				TEXT_SetFont(hItem, &GUI_FontSongTi12);
				strcpy(str, StringHZ[11]);
				strcat(str,  "3");
				TEXT_SetText(hItem, str);
			
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_P4);
				TEXT_SetFont(hItem, &GUI_FontSongTi12);
				strcpy(str, StringHZ[11]);
				strcat(str,  "4");
				TEXT_SetText(hItem, str);
				//
				// Initialization of Button
				//
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P1_ON);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[9]);

				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P1_OFF);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[10]);

				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P2_ON);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[9]);

				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P2_OFF);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[10]);	
			
			  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P3_ON);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[9]);	
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P3_OFF);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[10]);	
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P4_ON);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[9]);	
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_P4_OFF);
				BUTTON_SetFont(hItem, &GUI_FontSongTi12);
				BUTTON_SetText(hItem, StringHZ[10]);	
				break;
			case WM_NOTIFY_PARENT:
					 Id    = WM_GetId(pMsg->hWinSrc);
					 NCode = pMsg->Data.v;
					 switch(Id)
					 {
						 case ID_BUTTON_P1_ON:
							switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(1,ON);
									break;
								}
							 break;
						 case ID_BUTTON_P1_OFF:
							 switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(1,OFF);
									break;
								}
							 break;
						 case ID_BUTTON_P2_ON:
							 switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(2,ON);
									break;
								}
							 break;
						 case ID_BUTTON_P2_OFF:
							 switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(2,OFF);
									break;
								}
							 break;
								
						case ID_BUTTON_P3_ON:
							 switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(3,ON);
									break;
								}
							 break;
						case ID_BUTTON_P3_OFF:
							 switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(3,OFF);
									break;
								}
							 break;
						case ID_BUTTON_P4_ON:
							 switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(4,ON);
									break;
								}
							 break;
						case ID_BUTTON_P4_OFF:
							 switch(NCode) {
								case WM_NOTIFICATION_CLICKED:
									break;
								case WM_NOTIFICATION_RELEASED:
									   LED_Mannul(4,OFF);
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
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

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
    // Initialization of 'Back'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
	  BUTTON_SetFont(hItem, &GUI_FontSongTi12);
    BUTTON_SetText(hItem, StringHZ[0]);
    //
    // Initialization of 'Multipage'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);  
		MULTIPAGE_SetFont(hItem, &GUI_FontSongTi12);
		MULTIPAGE_AddPage(hItem, hDialog_Page1, StringHZ[12]);
		MULTIPAGE_AddPage(hItem, hDialog_Page2, StringHZ[13]);
		MULTIPAGE_AddPage(hItem, hDialog_Page3, StringHZ[8]);


    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_BACK: // Notifications sent by 'Back'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Key_Value = WINDOW_TOP;
        break;
      }
      break;
    case ID_MULTIPAGE_0: // Notifications sent by 'Multipage'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_MOVED_OUT:
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
WM_HWIN CreateWindow_3(void);
WM_HWIN CreateWindow_3(void) {
	
	hDialog_Page1 = GUI_CreateDialogBox(_aDialogCreatePage1, GUI_COUNTOF(_aDialogCreatePage1), _cbMotor, WM_UNATTACHED , 0, 0);
	hDialog_Page2 = GUI_CreateDialogBox(_aDialogCreatePage2, GUI_COUNTOF(_aDialogCreatePage2), _cbServo, WM_UNATTACHED , 0, 0);
	hDialog_Page3 = GUI_CreateDialogBox(_aDialogCreatePage3, GUI_COUNTOF(_aDialogCreatePage3), _cbPort,  WM_UNATTACHED , 0, 0);
	
  hWin_3 = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin_3;
}


/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

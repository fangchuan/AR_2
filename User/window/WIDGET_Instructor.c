/*
*********************************************************************************************************
*
*	模块名称 : 编程指令解析模块
*	文件名称 : WIDGET_Instructor.c
*	版    本 : V1.0
*	说    明 : 指令选择面板，共有输出控制、小车控制、端口判断、变量操作、流程控制、应用指令、画图指令
*            这七种指令，每种指令又细分不同的作用
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#include "WIDGET_Instructor.h"
#include "_apollorobot.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0       (GUI_ID_USER + 0x00)
#define ID_BUTTON_OUT     (GUI_ID_USER + 0x09)
#define ID_BUTTON_CAR     (GUI_ID_USER + 0x0A)
#define ID_BUTTON_PORT    (GUI_ID_USER + 0x0B)
#define ID_BUTTON_VAR     (GUI_ID_USER + 0x0C)
#define ID_BUTTON_PRO     (GUI_ID_USER + 0X0D)
#define ID_BUTTON_APP     (GUI_ID_USER + 0X0E)
#define ID_BUTTON_BACK    (GUI_ID_USER + 0x0F)
#define ID_BUTTON_DRAW    (GUI_ID_USER + 0X17)

#define ID_WINDOW_Out      (GUI_ID_USER + 0x01)
#define ID_BUTTON_DCMC     (GUI_ID_USER + 0x10)
#define ID_BUTTON_DCMCC    (GUI_ID_USER + 0x11)
#define ID_BUTTON_SER      (GUI_ID_USER + 0x12)
#define ID_BUTTON_LEDON    (GUI_ID_USER + 0x13)
#define ID_BUTTON_LEDOFF   (GUI_ID_USER + 0x14)

#define ID_WINDOW_Pro   	 (GUI_ID_USER + 0x02)
#define ID_BUTTON_While 	 (GUI_ID_USER + 0x20)
#define ID_BUTTON_EndWhile (GUI_ID_USER + 0x21)
#define ID_BUTTON_End   	 (GUI_ID_USER + 0x22)
#define ID_BUTTON_Or       (GUI_ID_USER + 0x23)
#define ID_BUTTON_IfEnd  	 (GUI_ID_USER + 0x24)
#define ID_BUTTON_PROC   	 (GUI_ID_USER + 0x25)
#define ID_BUTTON_FOR      (GUI_ID_USER + 0x26)
#define ID_BUTTON_ENDFOR   (GUI_ID_USER + 0x27)

#define ID_WINDOW_Car   (GUI_ID_USER + 0x03)
#define ID_BUTTON_Left  (GUI_ID_USER + 0x30)
#define ID_BUTTON_Right (GUI_ID_USER + 0x31)
#define ID_BUTTON_Forward (GUI_ID_USER + 0x32)
#define ID_BUTTON_Backward (GUI_ID_USER + 0x33)
#define ID_BUTTON_Stop  (GUI_ID_USER + 0x34)
#define ID_BUTTON_Accel (GUI_ID_USER + 0x35)
#define ID_BUTTON_Slow  (GUI_ID_USER + 0x36)

#define ID_WINDOW_Var   (GUI_ID_USER + 0x04)
#define ID_BUTTON_SA     (GUI_ID_USER + 0x41)
#define ID_BUTTON_SB     (GUI_ID_USER + 0x42)
#define ID_BUTTON_AA     (GUI_ID_USER + 0x43)
#define ID_BUTTON_AS     (GUI_ID_USER + 0x44)
#define ID_BUTTON_BA     (GUI_ID_USER + 0x45)
#define ID_BUTTON_BS     (GUI_ID_USER + 0x46)
#define ID_BUTTON_A     (GUI_ID_USER + 0x47)
#define ID_BUTTON_B     (GUI_ID_USER + 0x48)
#define ID_BUTTON_AG     (GUI_ID_USER + 0x49)
#define ID_BUTTON_AL     (GUI_ID_USER + 0x4A)
#define ID_BUTTON_AP     (GUI_ID_USER + 0x4B)
#define ID_BUTTON_BP     (GUI_ID_USER + 0x4C)

#define ID_WINDOW_Port  (GUI_ID_USER + 0x05)
#define ID_BUTTON_S     (GUI_ID_USER + 0x50)
#define ID_BUTTON_NS    (GUI_ID_USER + 0x51)
#define ID_BUTTON_WS    (GUI_ID_USER + 0X52)
#define ID_BUTTON_WNS   (GUI_ID_USER + 0X53)
#define ID_BUTTON_G     (GUI_ID_USER + 0X54)
#define ID_BUTTON_L     (GUI_ID_USER + 0X55)
#define ID_BUTTON_OBSG  (GUI_ID_USER + 0X56)
#define ID_BUTTON_OBSL  (GUI_ID_USER + 0X57)
#define ID_BUTTON_HACG  (GUI_ID_USER + 0x58)
#define ID_BUTTON_HACL  (GUI_ID_USER + 0x59)
#define ID_BUTTON_VACG  (GUI_ID_USER + 0x5A)
#define ID_BUTTON_VACL  (GUI_ID_USER + 0x5B)

#define ID_WINDOW_App     (GUI_ID_USER + 0X06)
#define ID_BUTTON_DLY     (GUI_ID_USER + 0x60)
#define ID_BUTTON_MUS     (GUI_ID_USER + 0x61)
#define ID_BUTTON_DISTANCE (GUI_ID_USER + 0x62)
#define ID_BUTTON_HAPPY    (GUI_ID_USER + 0x63)
#define ID_BUTTON_SAD      (GUI_ID_USER + 0x64)
#define ID_BUTTON_CRY      (GUI_ID_USER + 0x65)
#define ID_BUTTON_FURY     (GUI_ID_USER + 0x66)
#define ID_BUTTON_ALARM    (GUI_ID_USER + 0x67)


#define ID_WINDOW_DRAW     (GUI_ID_USER + 0x07)
#define ID_BUTTON_HCIRCLE  (GUI_ID_USER + 0x70)
#define ID_BUTTON_SCIRCLE  (GUI_ID_USER + 0x71)
#define ID_BUTTON_HRECT    (GUI_ID_USER + 0x72)
#define ID_BUTTON_SRECT    (GUI_ID_USER + 0x73)
#define ID_BUTTON_LINE     (GUI_ID_USER + 0x74)
#define ID_BUTTON_X1       (GUI_ID_USER + 0x75)
#define ID_BUTTON_X2       (GUI_ID_USER + 0x76)
#define ID_BUTTON_Y1       (GUI_ID_USER + 0x77)
#define ID_BUTTON_Y2       (GUI_ID_USER + 0x78)
#define ID_BUTTON_RADIUS   (GUI_ID_USER + 0x79)
#define ID_BUTTON_COLOR    (GUI_ID_USER + 0x7A)
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSongTi12;
extern WM_HWIN hWin1_1;
extern EDIT_Handle hEdit[100];
extern volatile int Edit_Index;//文本框的索引,用于链表的插入


WM_HWIN hWin_Instructor;
WM_HWIN hWin_Out;
WM_HWIN hWin_Car;
WM_HWIN hWin_Port;
WM_HWIN hWin_Var;
WM_HWIN hWin_Pro;
WM_HWIN hWin_App;
WM_HWIN hWin_Draw;

char _acText[50] ;//文本框当前内容

enum _FLAG _flag;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char *StringHZ[] = {////用于WIDGET_Instructor指令选择界面
	"\xe8\xbe\x93\xe5\x87\xba\xe6\x8e\xa7\xe5\x88\xb6","\xe5\xb0\x8f\xe8\xbd\xa6\xe6\x8e\xa7\xe5\x88\xb6",//0:输出控制，1:小车控制
	"\xe7\xab\xaf\xe5\x8f\xa3\xe5\x88\xa4\xe6\x96\xad","\xe5\x8f\x98\xe9\x87\x8f\xe6\x93\x8d\xe4\xbd\x9c",//2:端口判断，3:变量操作
	"\xe6\xb5\x81\xe7\xa8\x8b\xe6\x8e\xa7\xe5\x88\xb6","\xe5\xba\x94\xe7\x94\xa8\xe6\x8c\x87\xe4\xbb\xa4",//4:流程控制，5:应用指令
	"\xe8\xbf\x94\xe5\x9b\x9e",//6:返回
	"\xe7\x94\xb5\xe6\x9c\xba_\xe6\xad\xa3\xe8\xbd\xac,\xe9\x80\x9f\xe5\xba\xa6_",//7:电机_正转,速度_
	"\xe7\x94\xb5\xe6\x9c\xba_\xe5\x8f\x8d\xe8\xbd\xac,\xe9\x80\x9f\xe5\xba\xa6_",//8:电机_反转,速度_
	"\xe8\x88\xb5\xe6\x9c\xba_\xe8\xbd\xac_","LED_\xe6\x89\x93\xe5\xbc\x80",     //9:舵机_转_,10:LED_打开
	"\xe5\xb0\x8f\xe8\xbd\xa6\xe5\x89\x8d\xe8\xbf\x9b","\xe5\xb0\x8f\xe8\xbd\xa6\xe5\x90\x8e\xe9\x80\x80",//11:小车前进，12:小车后退
	"\xe5\xb0\x8f\xe8\xbd\xa6\xe5\xb7\xa6\xe8\xbd\xac","\xe5\xb0\x8f\xe8\xbd\xa6\xe5\x8f\xb3\xe8\xbd\xac",//13:小车左转，14:小车右转
	"\xe5\xb0\x8f\xe8\xbd\xa6\xe5\x81\x9c\xe6\xad\xa2",//15:小车停止
	"\xe5\xa6\x82\xe6\x9e\x9c\xe7\xab\xaf\xe5\x8f\xa3_\xe6\x9c\x89\xe4\xbf\xa1\xe5\x8f\xb7",//16:如果端口_有信号
	"\xe5\xa6\x82\xe6\x9e\x9c\xe7\xab\xaf\xe5\x8f\xa3_\xe6\x97\xa0\xe4\xbf\xa1\xe5\x8f\xb7",//17:如果端口_无信号
	"\xe7\xad\x89\xe5\xbe\x85\xe7\xab\xaf\xe5\x8f\xa3_\xe6\x9c\x89\xe4\xbf\xa1\xe5\x8f\xb7",//18:等待端口_有信号
	"\xe7\xad\x89\xe5\xbe\x85\xe7\xab\xaf\xe5\x8f\xa3_\xe6\x97\xa0\xe4\xbf\xa1\xe5\x8f\xb7",//19:等待端口_无信号
	"\xe5\xa6\x82\xe6\x9e\x9c\xe7\xab\xaf\xe5\x8f\xa3_>_","\xe5\xa6\x82\xe6\x9e\x9c\xe7\xab\xaf\xe5\x8f\xa3_<_",//20:如果端口_>_，21:如果端口_<_
	"\xe5\xbe\xaa\xe7\x8e\xaf\xe8\xaf\xad\xe5\x8f\xa5\xe5\xa4\xb4\xe9\x83\xa8",//22:循环语句头部
	"\xe5\xbe\xaa\xe7\x8e\xaf\xe8\xaf\xad\xe5\x8f\xa5\xe5\xb0\xbe\xe9\x83\xa8",//23:循环语句尾部
	"\xe7\xa8\x8b\xe5\xba\x8f\xe7\xbb\x93\xe6\x9d\x9f",//24:程序结束
	"\xe5\x90\xa6\xe5\x88\x99",//25:否则
	"\xe5\xbb\xb6\xe6\x97\xb6","\xe9\x9f\xb3\xe4\xb9\x90",//26:延时，27:音乐
	"\xe7\x94\xb5\xe6\x9c\xba\xe6\xad\xa3\xe8\xbd\xac",//28:电机正转
	"\xe7\x94\xb5\xe6\x9c\xba\xe5\x8f\x8d\xe8\xbd\xac",//29:电机反转
	"\xe6\x98\xbe\xe7\xa4\xba","\xe8\xae\xbe\xe5\xae\x9a",//30:显示，31:设定
	"\xe9\x9f\xb3\xe8\xb0\x83_\xe8\x8a\x82\xe6\x8b\x8d_",//32:音调_节拍_
	"\xe5\xa6\x82\xe6\x9e\x9c","\xe5\x88\xa0\xe9\x99\xa4",//33:如果  34:删除
	"\xe6\x9d\xa1\xe4\xbb\xb6\xe7\xbb\x93\xe6\x9d\x9f",//35:条件结束
	"LED_\xe5\x85\xb3\xe9\x97\xad",//36:LED_关闭
	"\xe5\xa6\x82\xe6\x9e\x9c\xe9\x9a\x9c\xe7\xa2\x8d\xe7\x89\xa9>_",//37:如果障碍物>_
	"\xe5\xa6\x82\xe6\x9e\x9c\xe9\x9a\x9c\xe7\xa2\x8d\xe7\x89\xa9<_",//38:如果障碍物<_
	"\xe6\x98\xbe\xe7\xa4\xba\xe8\xb7\x9d\xe7\xa6\xbb",//39:显示距离
	"\xe5\xb0\x8f\xe8\xbd\xa6\xe5\x8a\xa0\xe9\x80\x9f",//40:小车加速 
	"\xe5\xb0\x8f\xe8\xbd\xa6\xe5\x87\x8f\xe9\x80\x9f",// 41:小车减速
	"\xe7\x94\xbb\xe5\x9b\xbe\xe6\x8c\x87\xe4\xbb\xa4",//42:画图指令
	"\xe7\x94\xbb\xe7\xa9\xba\xe5\xbf\x83\xe5\x9c\x86",//43:画空心圆
	"\xe7\x94\xbb\xe5\xae\x9e\xe5\xbf\x83\xe5\x9c\x86",//44:画实心圆
	"\xe7\x94\xbb\xe7\xa9\xba\xe5\xbf\x83\xe7\x9f\xa9\xe5\xbd\xa2",//45:画空心矩形
	"\xe7\x94\xbb\xe5\xae\x9e\xe5\xbf\x83\xe7\x9f\xa9\xe5\xbd\xa2",//46:画实心矩形
	"\xe7\x94\xbb\xe7\x9b\xb4\xe7\xba\xbf",//47:画直线
	"\xe8\xb5\xb7\xe5\xa7\x8b\xe5\x9d\x90\xe6\xa0\x87X1=_",//48:起始坐标X1=_
	"\xe7\xbb\x88\xe6\xad\xa2\xe5\x9d\x90\xe6\xa0\x87X2=_",//49:终止坐标X2=_
	"\xe8\xb5\xb7\xe5\xa7\x8b\xe5\x9d\x90\xe6\xa0\x87Y1=_",//50:起始坐标Y1=_
	"\xe7\xbb\x88\xe6\xad\xa2\xe5\x9d\x90\xe6\xa0\x87Y2=_",//51:终止坐标Y2=_
	"\xe5\x8d\x8a\xe5\xbe\x84_",//52:半径_
	"\xe9\xa2\x9c\xe8\x89\xb2_",//53:颜色_
	"\xe7\x94\xbb\xe5\xbc\x80\xe5\xbf\x83",//54:画开心
	"\xe7\x94\xbb\xe9\x9a\xbe\xe8\xbf\x87",//55:画难过
	"\xe7\x94\xbb\xe5\xa4\xa7\xe5\x93\xad",//56：画大哭
	"\xe7\x94\xbb\xe5\x8f\x91\xe6\x80\x92",//57:画发怒
	"\xe7\x94\xbb\xe8\xad\xa6\xe7\x81\xaf",//58:画警灯
	"\xe8\xb0\x83\xe7\x94\xa8\xe5\xad\x90\xe7\xa8\x8b\xe5\xba\x8f_",//59:调用子程序
	"A=\xe7\xab\xaf\xe5\x8f\xa3",//60:A=端口
	"B=\xe7\xab\xaf\xe5\x8f\xa3",//61:B= 端口
	"\xe5\xa6\x82\xe6\x9e\x9c\xe6\xb0\xb4\xe5\xb9\xb3\xe5\x8a\xa0\xe9\x80\x9f\xe5\xba\xa6>_",//62:如果水平加速度>_
	"\xe5\xa6\x82\xe6\x9e\x9c\xe6\xb0\xb4\xe5\xb9\xb3\xe5\x8a\xa0\xe9\x80\x9f\xe5\xba\xa6<_",//63:如果水平加速度<_
	"\xe5\xa6\x82\xe6\x9e\x9c\xe7\xab\x96\xe7\x9b\xb4\xe5\x8a\xa0\xe9\x80\x9f\xe5\xba\xa6>_",//64:如果竖直加速度>_
	"\xe5\xa6\x82\xe6\x9e\x9c\xe7\xab\x96\xe7\x9b\xb4\xe5\x8a\xa0\xe9\x80\x9f\xe5\xba\xa6<_",//65:如果竖直加速度<_
	"FOR\xe5\xbe\xaa\xe7\x8e\xaf_\xe6\xac\xa1",//66:FOR循环_次
	"FOR\xe5\xbe\xaa\xe7\x8e\xaf\xe7\xbb\x93\xe6\x9d\x9f",//67:FOR循环结束

};
 

/*********************************************************************
*
*       _aDialog_Create_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 1 , 59, 219, 260, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "输出控制", ID_BUTTON_OUT, 10, 10, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "小车控制", ID_BUTTON_CAR, 105, 10, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "端口判断", ID_BUTTON_PORT, 10, 70, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "变量操作", ID_BUTTON_VAR, 105, 70, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "流程控制", ID_BUTTON_PRO, 10, 130, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "应用指令", ID_BUTTON_APP, 105, 130, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "画图指令", ID_BUTTON_DRAW, 10, 190, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};
/*********************************************************************
*
*       _aDialog_Output_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogOutput_Panel[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_Out, 2, 1, 218, 258, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "电机正转", ID_BUTTON_DCMC, 0, 20, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "电机反转", ID_BUTTON_DCMCC, 0, 70, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "舵机", ID_BUTTON_SER, 0, 120, 100, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "LED", ID_BUTTON_LEDON, 110, 20, 100, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "LED", ID_BUTTON_LEDOFF, 110,70, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },

};
/*********************************************************************
*
*       _aDialog_Car_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCar_Panel[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_Car, 2, 1, 218, 258, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "小车左转", ID_BUTTON_Left, 10, 20, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "小车右转", ID_BUTTON_Right, 10, 60, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "小车前进", ID_BUTTON_Forward, 10, 100, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "小车后退", ID_BUTTON_Backward, 10, 140, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "小车停止", ID_BUTTON_Stop, 105, 20, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "小车加速", ID_BUTTON_Accel, 105, 60, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "小车减速", ID_BUTTON_Slow, 105, 100, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};
/*********************************************************************
*
*       _aDialog_Pro_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogPro_Panel[] = {
  { WINDOW_CreateIndirect, "Window",  	ID_WINDOW_Pro,     2, 1, 218, 258, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "循环开始", 	ID_BUTTON_While, 10, 10, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "循环结束", 	ID_BUTTON_EndWhile, 10, 60, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "程序结束", 	ID_BUTTON_End, 10, 110, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "否则", 		 	ID_BUTTON_Or, 110, 10, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "条件结束",   ID_BUTTON_IfEnd, 110, 60, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "调用子程序", ID_BUTTON_PROC, 110, 110, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "FOR循环_次", ID_BUTTON_FOR, 10, 160, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "FOR循环结束",ID_BUTTON_ENDFOR, 110, 160, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};
/*********************************************************************
*
*       _aDialog_Var_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogVar_Panel[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_Var, 2, 1, 218, 258, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "设定A=", ID_BUTTON_SA, 10, 5, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "设定B=", ID_BUTTON_SB, 100, 5, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "变量A+1", ID_BUTTON_AA, 10, 40, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "变量A-1", ID_BUTTON_AS, 100, 40, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "变量B+1", ID_BUTTON_BA, 10, 75, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "变量B-1", ID_BUTTON_BS, 100, 75, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "显示A", ID_BUTTON_A, 10, 110, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "显示B", ID_BUTTON_B, 100, 110, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "如果变量A>_", ID_BUTTON_AG, 10, 145, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "如果变量A<_", ID_BUTTON_AL, 100, 145, 60, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "A=端口_",     ID_BUTTON_AP, 10, 180, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "B=端口_",     ID_BUTTON_BP, 100, 180, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};
/*********************************************************************
*
*       _aDialog_Port_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogPort_Panel[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_Port, 2, 1, 218, 258, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "有信号", ID_BUTTON_S, 0, 5, 100, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "无信号", ID_BUTTON_NS, 0, 40, 100, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "等待有信号", ID_BUTTON_WS, 0, 80, 100, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "等待无信号", ID_BUTTON_WNS, 0, 120, 100, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "大于", ID_BUTTON_G, 110, 5, 100, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "小于", ID_BUTTON_L, 110, 40, 100, 25, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "如果障碍物>", ID_BUTTON_OBSG, 110, 80, 100, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "如果障碍物<", ID_BUTTON_OBSL, 110, 120, 100, 25, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "如果水平加速度>", ID_BUTTON_HACG, 0, 160, 100, 25, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "如果水平加速度<", ID_BUTTON_HACL, 0, 200, 100, 25, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "如果竖直加速度>", ID_BUTTON_VACG, 110, 160, 100, 25, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "如果竖直加速度<", ID_BUTTON_VACL, 110, 200, 100, 25, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },
};
/*********************************************************************
*
*       _aDialog_App_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogApp_Panel[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_App, 2, 1, 218, 258, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "延时_ms", ID_BUTTON_DLY, 10, 20, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "音乐",    ID_BUTTON_MUS, 105, 20, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "显示距离", ID_BUTTON_DISTANCE, 10, 60, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "画开心", ID_BUTTON_HAPPY, 10, 100, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "画难过", ID_BUTTON_SAD, 105, 100, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "画大哭", ID_BUTTON_CRY, 10, 140, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "画发怒", ID_BUTTON_FURY, 105, 140, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "画警灯", ID_BUTTON_ALARM, 105, 60, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },
};
/*********************************************************************
*
*       _aDialog_Draw_Panel
*/
static const GUI_WIDGET_CREATE_INFO _aDialogDraw_Panel[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_DRAW, 2, 1, 218, 258, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "画空心圆", ID_BUTTON_HCIRCLE, 10, 10, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "画实心圆", ID_BUTTON_SCIRCLE, 105,10, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "画空心矩形", ID_BUTTON_HRECT, 10, 50, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "画实心矩形", ID_BUTTON_SRECT, 105,50, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "画直线", ID_BUTTON_LINE,      10, 90, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "起始坐标X1=_", ID_BUTTON_X1, 105, 90, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "终止坐标X2=_", ID_BUTTON_X2, 10, 130, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "起始坐标Y1=_", ID_BUTTON_Y1, 105,130, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "终止坐标Y2=_", ID_BUTTON_Y2, 10, 170, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "半径_", ID_BUTTON_RADIUS,    105,170, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "颜色_", ID_BUTTON_COLOR,     10, 210, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 125, 230, 60, 26, 0, 0x0, 0 },
};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
//输出控制的回调函数
static void _cbDialog_Out(WM_MESSAGE *pMsg)
{
		int NCode;
		int Id;
		WM_HWIN hItem, hEdit;
	
	switch(pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, 0x00FF8080);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DCMC);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[28]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DCMCC);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[29]);
					
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SER);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[9]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LEDON);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[10]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LEDOFF);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[36]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[6]);
			break;
		case WM_NOTIFY_PARENT:
			    Id    = WM_GetId(pMsg->hWinSrc);
					NCode = pMsg->Data.v;
					switch(Id)
					{
						case ID_BUTTON_DCMC://电机正转
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_MOTOR_C ;
												strcpy(_acText , StringHZ[7]);
												hEdit = Create_EDITPad(pMsg->hWin, 2, 8);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_DCMCC://电机反转
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_MOTOR_CC ;
												strcpy(_acText , StringHZ[8]);
												hEdit = Create_EDITPad(pMsg->hWin, 2, 8);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_SER://舵机_转_
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_SERVO ;
												strcpy(_acText , StringHZ[9]);
												hEdit = Create_EDITPad(pMsg->hWin, 2, 4);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_LEDON://LED打开
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_LED_ON;
												strcpy(_acText , StringHZ[10]);
												hEdit = Create_EDITPad(pMsg->hWin, 3, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_LEDOFF://LED关闭
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_LED_OFF;
												strcpy(_acText , StringHZ[36]);
												hEdit = Create_EDITPad(pMsg->hWin, 3, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BACK:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
										GUI_EndDialog(pMsg->hWin ,0);
									break;		
							}
							break;
						default:
							WM_DefaultProc(pMsg);
						break;
					}
			break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}
//小车控制的回调函数
static void _cbDialog_Car(WM_MESSAGE *pMsg)
{
		int NCode;
		int Id;
		WM_HWIN hItem, hEdit;

	switch(pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, 0x00FF8080);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Forward);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[11]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Backward);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[12]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Left);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[13]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Right);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[14]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Stop);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[15]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Accel);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[40]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Slow);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[41]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[6]);
			break;
		case WM_NOTIFY_PARENT:
			    Id    = WM_GetId(pMsg->hWinSrc);
					NCode = pMsg->Data.v;
					switch(Id)
					{
						case ID_BUTTON_Left:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_CAR_LEFT ;
												strcpy(_acText , StringHZ[13]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Right:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_CAR_RIGHT ;
												strcpy(_acText , StringHZ[14]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Forward:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_CAR_FORWARD ;
												strcpy(_acText , StringHZ[11]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Backward:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_CAR_BACKWARD ;
												strcpy(_acText , StringHZ[12]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Stop:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_CAR_STOP ;
												strcpy(_acText , StringHZ[15]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Accel:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_CAR_ACCEL ;
												strcpy(_acText , StringHZ[40]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Slow:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_CAR_SLOW ;
												strcpy(_acText , StringHZ[41]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BACK:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
										GUI_EndDialog(pMsg->hWin ,0);
									break;		
							}
							break;
						default:
							WM_DefaultProc(pMsg);
						break;
					}
			break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}
//端口判断的回调函数
static void _cbDialog_Port(WM_MESSAGE *pMsg)
{
		int NCode;
		int Id;
		WM_HWIN hItem, hEdit;
	
		switch(pMsg->MsgId)
		{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, 0x00FF8080);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_S);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[16]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NS);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[17]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_WS);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[18]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_WNS);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[19]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_G);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[20]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_L);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[21]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OBSG);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[37]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OBSL);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[38]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HACG);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[62]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HACL);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[63]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_VACG);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[64]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_VACL);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[65]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[6]);
			break;
		case WM_NOTIFY_PARENT:
			    Id    = WM_GetId(pMsg->hWinSrc);
					NCode = pMsg->Data.v;
					switch(Id)
					{
						case ID_BUTTON_S:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_PORT_SIGNAL ;
												strcpy(_acText , StringHZ[16]);
												hEdit = Create_EDITPad(pMsg->hWin, 4, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_NS:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_PORT_NOSIGNAL ;
												strcpy(_acText , StringHZ[17]);
												hEdit = Create_EDITPad(pMsg->hWin, 4, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_WS:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_PORT_WAIT_SIGNAL;
												strcpy(_acText , StringHZ[18]);
												hEdit = Create_EDITPad(pMsg->hWin, 4, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_WNS:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_PORT_WAIT_NOSIGNAL;
												strcpy(_acText , StringHZ[19]);
												hEdit = Create_EDITPad(pMsg->hWin, 4, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_G:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_PORT_GREATER ;
												strcpy(_acText , StringHZ[20]);
												hEdit = Create_EDITPad(pMsg->hWin, 4, 6);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_L:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_PORT_LITTLER ;
												strcpy(_acText , StringHZ[21]);
												hEdit = Create_EDITPad(pMsg->hWin, 4, 6);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_OBSG:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_OBSTRACLE_GREATER ;
												strcpy(_acText , StringHZ[37]);
												hEdit = Create_EDITPad(pMsg->hWin, 6, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_OBSL:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_OBSTRACLE_LITTER ;
												strcpy(_acText , StringHZ[38]);
												hEdit = Create_EDITPad(pMsg->hWin, 6, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_HACG:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_HACCEL_GREATOR ;
												strcpy(_acText , StringHZ[62]);
												hEdit = Create_EDITPad(pMsg->hWin, 8, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_HACL:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_HACCEL_LITTER ;
												strcpy(_acText , StringHZ[63]);
												hEdit = Create_EDITPad(pMsg->hWin, 8, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_VACG:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VACCEL_GREATOR ;
												strcpy(_acText , StringHZ[64]);
												hEdit = Create_EDITPad(pMsg->hWin, 8, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_VACL:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VACCEL_LITTER ;
												strcpy(_acText , StringHZ[65]);
												hEdit = Create_EDITPad(pMsg->hWin, 8, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
							
						case ID_BUTTON_BACK:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
										GUI_EndDialog(pMsg->hWin ,0);
									break;		
							}
							break;
						default:
							WM_DefaultProc(pMsg);
						break;
					}
			break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}
//变量操作的回调函数
static void _cbDialog_Var(WM_MESSAGE *pMsg)
{
		int NCode;
		int Id;
		WM_HWIN hItem, hEdit;
		char string[10];
	
	switch(pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, 0x00FF8080);
			//
			//Initialize of Button
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SA);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[31]);
			strcat(string,"A=");
			BUTTON_SetText(hItem,string);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SB);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[31]);
			strcat(string,"B=");
			BUTTON_SetText(hItem,string);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_A);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[30]);
			strcat(string,"A");
			BUTTON_SetText(hItem,string);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_B);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[30]);
			strcat(string,"B");
			BUTTON_SetText(hItem,string);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_AG);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[33]);
			strcat(string,"A>_");
			BUTTON_SetText(hItem,string);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_AL);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[33]);
			strcat(string,"A<_");
			BUTTON_SetText(hItem,string);
		

			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_AP);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[60]);
			strcat(string,"_");
			BUTTON_SetText(hItem, string);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BP);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[61]);
			strcat(string,"_");
			BUTTON_SetText(hItem,string);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[6]);
			break;
		case WM_NOTIFY_PARENT:
			    Id    = WM_GetId(pMsg->hWinSrc);
					NCode = pMsg->Data.v;
					switch(Id)
					{
						case ID_BUTTON_SA:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_SET_A ;
												strcpy(_acText,StringHZ[31]);
												strcat(_acText,"A=");
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_SB:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_SET_B ;
												strcpy(_acText,StringHZ[31]);
												strcat(_acText,"B=");
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_AA:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_A_INC ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,10);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_AS:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_A_DEC ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,10);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BA:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_B_INC ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,10);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BS:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_B_DEC;
												BUTTON_GetText(pMsg->hWinSrc, _acText,10);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_A:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_SHOW_A;
												BUTTON_GetText(pMsg->hWinSrc, _acText,10);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_B:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_SHOW_B;
												BUTTON_GetText(pMsg->hWinSrc, _acText,10);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_AG:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_A_GREATER ;
												strcpy(_acText,StringHZ[33]);
												strcat(_acText,"A>");
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_AL:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_A_LITTLER ;
												strcpy(_acText,StringHZ[33]);
												strcat(_acText,"A<");
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_AP:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_SET_A_PORT ;
												strcpy(_acText,StringHZ[60]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BP:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_VAR_SET_B_PORT ;
												strcpy(_acText,StringHZ[61]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BACK:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
										GUI_EndDialog(pMsg->hWin ,0);
									break;		
							}
							break;
						default:
							WM_DefaultProc(pMsg);
						break;
					}
			break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}
//流程控制的回调函数
static void _cbDialog_Pro(WM_MESSAGE *pMsg)
{
		int NCode;
		int Id;
		WM_HWIN hItem, hEdit;
	
	switch(pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, 0x00FF8080);
			//
			//Initialize of Button
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_While);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[22]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EndWhile);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[23]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_End);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[24]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Or);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[25]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_IfEnd);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[35]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PROC);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[59]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_FOR);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[66]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_ENDFOR);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[67]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[6]);
			break;
		case WM_NOTIFY_PARENT:
			    Id    = WM_GetId(pMsg->hWinSrc);
					NCode = pMsg->Data.v;
					switch(Id)
					{
						case ID_BUTTON_While:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_WHILE_HEAD;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_EndWhile:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_WHILE_TAIL ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_End:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_END_PROGRAM ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Or:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_OR ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_IfEnd:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_IF_END ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_PROC:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_PROC ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, 5, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_FOR:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_FOR_HEAD ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, 5, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_ENDFOR:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_FOR_TAIL ;
												BUTTON_GetText(pMsg->hWinSrc, _acText,50);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BACK:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
										GUI_EndDialog(pMsg->hWin ,0);
									break;		
							}
							break;
						default:
							WM_DefaultProc(pMsg);
						break;
					}
			break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}
//应用指令的回调函数
static void _cbDialog_App(WM_MESSAGE *pMsg)
{
		int NCode;
		int Id;
		WM_HWIN hItem, hEdit;
		char     string[10]={0};
	
	switch(pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, 0x00FF8080);
			//
			//Initialize of Button
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DLY);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			strcpy(string,StringHZ[26]);
			strcat(string,"_ms");
			BUTTON_SetText(hItem,string);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MUS);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[27]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DISTANCE);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[39]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HAPPY);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[54]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAD);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[55]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CRY);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[56]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_FURY);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[57]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_ALARM);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[58]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[6]);
			break;
		case WM_NOTIFY_PARENT:
			    Id    = WM_GetId(pMsg->hWinSrc);
					NCode = pMsg->Data.v;
					switch(Id)
					{
						case ID_BUTTON_DLY:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_DELAY_NMS ;
												strcpy(_acText,StringHZ[26]);
												strcat(_acText,"_ms");
												hEdit = Create_EDITPad(pMsg->hWin, 2, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_MUS:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_MUSIC ;
												strcpy(_acText,StringHZ[32]);
												hEdit = Create_EDITPad(pMsg->hWin, 2, 5);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_DISTANCE:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_SHOW_DISTANCE ;
												strcpy(_acText,StringHZ[39]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_HAPPY:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_GIF_HAPPY ;
												strcpy(_acText,StringHZ[54]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_SAD:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_GIF_SAD ;
												strcpy(_acText,StringHZ[55]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
					 case ID_BUTTON_CRY:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_GIF_CRY ;
												strcpy(_acText,StringHZ[56]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_FURY:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_GIF_FURY ;
												strcpy(_acText,StringHZ[57]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_ALARM:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_GIF_ALARM ;
												strcpy(_acText,StringHZ[58]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BACK:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
										GUI_EndDialog(pMsg->hWin ,0);
									break;		
							}
							break;
						default:
							WM_DefaultProc(pMsg);
						break;
					}
			break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}
//画图指令的回调函数
static void _cbDialog_Draw(WM_MESSAGE *pMsg)
{
		int NCode;
		int Id;
		WM_HWIN hItem, hEdit;
	
	switch(pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, 0x00FF8080);
			//
			//Initialize of Button
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HCIRCLE);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[43]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SCIRCLE);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[44]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HRECT);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[45]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SRECT);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[46]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINE);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[47]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_X1);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[48]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_X2);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[49]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Y1);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[50]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Y2);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[51]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RADIUS);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[52]);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_COLOR);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[53]);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
			BUTTON_SetFont(hItem, &GUI_FontSongTi12);
			BUTTON_SetText(hItem,StringHZ[6]);
			break;
		case WM_NOTIFY_PARENT:
			    Id    = WM_GetId(pMsg->hWinSrc);
					NCode = pMsg->Data.v;
					switch(Id)
					{
						case ID_BUTTON_HCIRCLE:
							   switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_DRAW_HCIRCLE ;
												strcpy(_acText,StringHZ[43]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_SCIRCLE:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_DRAW_SCIRCLE ;
												strcpy(_acText,StringHZ[44]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_HRECT:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_DRAW_HRECT ;
												strcpy(_acText,StringHZ[45]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_SRECT:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_DRAW_SRECT ;
												strcpy(_acText,StringHZ[46]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_LINE:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_DRAW_LINE ;
												strcpy(_acText,StringHZ[47]);
												hEdit = Create_EDITPad(pMsg->hWin, -1, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_X1:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_SET_X1 ;
												strcpy(_acText,StringHZ[48]);
												hEdit = Create_EDITPad(pMsg->hWin, 7, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_X2:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_SET_X2 ;
												strcpy(_acText,StringHZ[49]);
												hEdit = Create_EDITPad(pMsg->hWin, 7, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Y1:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_SET_Y1 ;
												strcpy(_acText,StringHZ[50]);
												hEdit = Create_EDITPad(pMsg->hWin, 7, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_Y2:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_SET_Y2 ;
												strcpy(_acText,StringHZ[51]);
												hEdit = Create_EDITPad(pMsg->hWin, 7, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_RADIUS:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_SET_RADIUS ;
												strcpy(_acText,StringHZ[52]);
												hEdit = Create_EDITPad(pMsg->hWin, 2, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_COLOR:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
												_flag = FLAG_COLOR ;
												strcpy(_acText,StringHZ[53]);
												hEdit = Create_EDITPad(pMsg->hWin, 2, -1);
												WM_MakeModal(hEdit);
												GUI_ExecCreatedDialog(hEdit);
									break;		
							}
							break;
						case ID_BUTTON_BACK:
								 switch(NCode) {
									case WM_NOTIFICATION_CLICKED:
									break;
									case WM_NOTIFICATION_RELEASED:
										GUI_EndDialog(pMsg->hWin ,0);
									break;		
							}
							break;
						default:
							WM_DefaultProc(pMsg);
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
  int NCode;
  int Id;
	WM_HWIN hItem;
  GUI_RECT r;

  switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x00FF8080);
    
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OUT);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[0]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAR);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[1]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PORT);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[2]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_VAR);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[3]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PRO);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[4]);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_APP);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[5]);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DRAW);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[42]);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[6]);
		break;
	
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_OUT: // Notifications sent by 'Calc '
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						hWin_Out = GUI_CreateDialogBox(_aDialogOutput_Panel, GUI_COUNTOF(_aDialogOutput_Panel),
																							_cbDialog_Out, hWin_Instructor, 0, 0);
        break;
      }
      break;
    case ID_BUTTON_CAR: // Notifications sent by 'Control'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						hWin_Car = GUI_CreateDialogBox(_aDialogCar_Panel,GUI_COUNTOF(_aDialogCar_Panel),
																						_cbDialog_Car,hWin_Instructor,0,0);
        break;
      }
      break;
    case ID_BUTTON_PORT: // Notifications sent by 'Output'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
							hWin_Out = GUI_CreateDialogBox(_aDialogPort_Panel,GUI_COUNTOF(_aDialogPort_Panel),
																						_cbDialog_Port,hWin_Instructor,0,0);
        break;
      }
      break;
    case ID_BUTTON_VAR: // Notifications sent by 'Variable'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						hWin_Var = GUI_CreateDialogBox(_aDialogVar_Panel,GUI_COUNTOF(_aDialogVar_Panel),
																						_cbDialog_Var,hWin_Instructor,0,0);
        break;
      }
      break;
		case ID_BUTTON_PRO: // Notifications sent by 'Process'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						hWin_Var = GUI_CreateDialogBox(_aDialogPro_Panel,GUI_COUNTOF(_aDialogPro_Panel),
																						_cbDialog_Pro,hWin_Instructor,0,0);
        break;
      }
      break;
		case ID_BUTTON_APP: // Notifications sent by 'Variable'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						hWin_Var = GUI_CreateDialogBox(_aDialogApp_Panel,GUI_COUNTOF(_aDialogApp_Panel),
																						_cbDialog_App,hWin_Instructor,0,0);
        break;
      }
      break;
    case ID_BUTTON_DRAW: // Notifications sent by 'DrawPicture'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						hWin_Draw = GUI_CreateDialogBox(_aDialogDraw_Panel,GUI_COUNTOF(_aDialogDraw_Panel),
																						_cbDialog_Draw,hWin_Instructor,0,0);
        break;
      }
      break;
    case ID_BUTTON_BACK: // Notifications sent by 'BACK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						GUI_EndDialog(hWin_Instructor,0);
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
*       CreateWindow
*/
WM_HWIN CreateWindow_Instructor(void);
WM_HWIN CreateWindow_Instructor(void) {

  hWin_Instructor = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin_Instructor;
}


/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

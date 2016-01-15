#include "WIDGET_DrawScale.h"


#define  PEND_MAX_SIZE  40
#define  PEND_MIN_SIZE  20
#define  PI    (float)3.14
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//“向前”箭头
//        *
//       ***
//      *****
//     *******
//       ***
//       ***
//       ***
//       ***
//       ***
//       ***
static const GUI_POINT _aPointForwardArrow[] = {
  {  0,   0 },
  {-40,  50 },
  {-20,  50 },
  {-20, 200 },
  { 20, 200 },
  { 20,  50 },
  { 40,  50 },
};
//”向后”箭头
//       ***
//       ***
//       ***
//       ***
//       ***
//       ***
//     *******
//      *****
//        *
static const GUI_POINT _aPointBackArrow[] = {
  {  0,   0 },
  {-40, -50 },
  {-20, -50 },
  {-20, -200},
  { 20, -200},
  { 20, -50 },
  { 40, -50 },
};
//"停止"短横线:线宽20，线长120
//    *************************
//    *************************
static const GUI_POINT _aPointHLine[] = {
  {  0,   0 },
  {  0, -20 },
  {120, -20 },
  {120,   0 },

};
//短竖线:线宽40 线长100
//       ****
//       ****
//       ****
//       ****
//       ****
//       ****
static const GUI_POINT _aPointVLine[] = {
  {  0,   0 },
  {  0, -100},
  { 40, -100},
  { 40,   0 },
};
//向左三角形:等腰直角三角形,腰60
//     * * * * *
//     * * * *
//     * * *
//     * *
//     *
static const GUI_POINT _aPointLeftTriangle[] ={
  {  0,   0 },
	{  0,  -60},
  { 60,  -60},

};
//向右三角形:等腰直角三角形,腰60
//   * * * * *
//     * * * *
//       * * *
//         * *
//           *
static const GUI_POINT _aPointRightTriangle[] = {
  { 0 ,  0},
	{ 60,  0},
	{ 60, 60},
};
/*********************************************************************
*
*       Public Code
*
**********************************************************************
*/
//向前图标:Length=200,Width=40, ArrowLength=50,ArrowWidth=80
void   DrawForwardIcon(void)
{	
//		   GUI_ClearRect(100, 90, 220, 175);
	     GUI_Clear();
       GUI_SetColor(GUI_RED);
       GUI_FillPolygon (&_aPointForwardArrow[0], 7, 120, 30);//顶点位于(120,30)
}
//向后图标:Length=200,Width=40,ArrowLength=50,ArrowWidth=80
void  DrawBackIcon(void)
{
//	    GUI_ClearRect(100, 90, 220, 175);
	    GUI_Clear();
      GUI_SetColor(GUI_RED);
      GUI_FillPolygon (&_aPointBackArrow[0], 7, 120, 270);//顶点位于(120,270)
}
//向左图标
void  DrawLeftIcon(void)
{
	   int  r = 120 ;    //半径为120
	
	   GUI_Clear();
		 GUI_SetPenSize(PEND_MAX_SIZE);
     GUI_SetColor(GUI_RED);
	   GUI_FillPolygon(_aPointVLine, 4, 100, 260);//顶点(100,260)
     GUI_AA_DrawArc(0, 160, r, r, 0, 45);//起始角度为0，终止角度为60.圆心(0,160)
	   GUI_FillPolygon(_aPointLeftTriangle, 3, 56, 105);//定点(50,100)
}
//向右图标
void  DrawRightIcon(void)
{
	   int  r = 120;   //半径为120
		  
	   GUI_Clear();
		 GUI_SetPenSize(PEND_MAX_SIZE);
     GUI_SetColor(GUI_RED);
		 GUI_FillPolygon(_aPointVLine, 4, 100, 260);//顶点(100,40)
     GUI_AA_DrawArc(240, 160, r, r, 135, 180);
	   GUI_FillPolygon(_aPointRightTriangle, 3, 118, 50);//定点(120,58)
}
//停止图标
void DrawStopIcon(void)
{  
	   int   r = 90;
		 int   YMid = LCD_GetYSize() / 2;
	   int   XMid = LCD_GetXSize() / 2;
	   
	   GUI_Clear();
		 GUI_SetColor(GUI_RED);
	   GUI_FillPolygon(_aPointHLine, 4, 60, YMid);
	   
	   GUI_SetPenSize(PEND_MIN_SIZE);
     GUI_AA_DrawArc(XMid, YMid, r, r, 0, 360);//起始角度为0，终止角度为360
}

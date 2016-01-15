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
//����ǰ����ͷ
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
//����󡱼�ͷ
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
//"ֹͣ"�̺���:�߿�20���߳�120
//    *************************
//    *************************
static const GUI_POINT _aPointHLine[] = {
  {  0,   0 },
  {  0, -20 },
  {120, -20 },
  {120,   0 },

};
//������:�߿�40 �߳�100
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
//����������:����ֱ��������,��60
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
//����������:����ֱ��������,��60
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
//��ǰͼ��:Length=200,Width=40, ArrowLength=50,ArrowWidth=80
void   DrawForwardIcon(void)
{	
//		   GUI_ClearRect(100, 90, 220, 175);
	     GUI_Clear();
       GUI_SetColor(GUI_RED);
       GUI_FillPolygon (&_aPointForwardArrow[0], 7, 120, 30);//����λ��(120,30)
}
//���ͼ��:Length=200,Width=40,ArrowLength=50,ArrowWidth=80
void  DrawBackIcon(void)
{
//	    GUI_ClearRect(100, 90, 220, 175);
	    GUI_Clear();
      GUI_SetColor(GUI_RED);
      GUI_FillPolygon (&_aPointBackArrow[0], 7, 120, 270);//����λ��(120,270)
}
//����ͼ��
void  DrawLeftIcon(void)
{
	   int  r = 120 ;    //�뾶Ϊ120
	
	   GUI_Clear();
		 GUI_SetPenSize(PEND_MAX_SIZE);
     GUI_SetColor(GUI_RED);
	   GUI_FillPolygon(_aPointVLine, 4, 100, 260);//����(100,260)
     GUI_AA_DrawArc(0, 160, r, r, 0, 45);//��ʼ�Ƕ�Ϊ0����ֹ�Ƕ�Ϊ60.Բ��(0,160)
	   GUI_FillPolygon(_aPointLeftTriangle, 3, 56, 105);//����(50,100)
}
//����ͼ��
void  DrawRightIcon(void)
{
	   int  r = 120;   //�뾶Ϊ120
		  
	   GUI_Clear();
		 GUI_SetPenSize(PEND_MAX_SIZE);
     GUI_SetColor(GUI_RED);
		 GUI_FillPolygon(_aPointVLine, 4, 100, 260);//����(100,40)
     GUI_AA_DrawArc(240, 160, r, r, 135, 180);
	   GUI_FillPolygon(_aPointRightTriangle, 3, 118, 50);//����(120,58)
}
//ֹͣͼ��
void DrawStopIcon(void)
{  
	   int   r = 90;
		 int   YMid = LCD_GetYSize() / 2;
	   int   XMid = LCD_GetXSize() / 2;
	   
	   GUI_Clear();
		 GUI_SetColor(GUI_RED);
	   GUI_FillPolygon(_aPointHLine, 4, 60, YMid);
	   
	   GUI_SetPenSize(PEND_MIN_SIZE);
     GUI_AA_DrawArc(XMid, YMid, r, r, 0, 360);//��ʼ�Ƕ�Ϊ0����ֹ�Ƕ�Ϊ360
}

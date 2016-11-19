#include "WIDGET_DrawScale.h"
#include "GIF_HAPPY.h"
#include "GIF_SAD.h"
#include "GIF_CRY.h"
#include "GIF_ALARM.h"
#include "GIF_FURY.h"
#include "WIDGET_MessageBox.h"

#define  PEND_MAX_SIZE  40
#define  PEND_MIN_SIZE  20
#define  PI    (float)3.14
	
#define DEFAULT_COLOR COLOR_0 //Ĭ����ɫΪ��ɫ
#define DEFAULT_WIDTH 20      //Ĭ���߿�Ϊ20
#define MAX_RADIUS    180

#define GIF_RECT_LEFT     20
#define GIF_RECT_RIGHT    160
#define GIF_RECT_TOP      80
#define GIF_RECT_BOTTOM   240
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
//������: ��40����200
static const GUI_POINT _aPointHLineLong[] = {
  {  0,   0 },
  {  0,  40 },
  {200,  40 },
  {200,   0 },

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
//������:��40����200
static const GUI_POINT _aPointVLineLong[] = {
  {  0,   0 },
  {  0,  200},
  { 40,  200},
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
       GUI_FillPolygon (&_aPointForwardArrow[0], 7, 120, 50);//����λ��(120,50)
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
	   GUI_FillPolygon(_aPointHLine, 4, 65, YMid);
	   
	   GUI_SetPenSize(PEND_MIN_SIZE);
     GUI_AA_DrawArc(XMid, YMid, r, r, 0, 360);//��ʼ�Ƕ�Ϊ0����ֹ�Ƕ�Ϊ360
}

//_Paint���ݽṹ��ʼ��
void Init_Paint(_Paint *paint)
{
	   paint->species = 0;
	   paint->x1 = -1;
	   paint->x2 = -1;
	   paint->y1 = -1;
	   paint->y2 = -1;
	   paint->radius = 0;
	   paint->color = DEFAULT_COLOR;
	   paint->width = DEFAULT_WIDTH;
}

//Paint����
_Error Paint_Config(_Paint * paint)
{
	   GUI_Clear();
	
	   if(paint->species < HOLLOW_CIRCLE || paint->species > STRIGHT_LINE)
		 {
//			 u8 ret;
//			 _MessageBox("��ѡ��ͼ����","����",&ret);
			 return ERROR_PAINT;
		 }
		 switch(paint->species)
		 {
			 case HOLLOW_CIRCLE:
				    if(paint->x1 < 0 || paint->y1 < 0)
						{
//							 u8 ret;
//							 _MessageBox("����ȷ����Բ��","����",&ret);
							 return ERROR_PAINT;
						}
						if(paint->radius < 1 || paint->radius > MAX_RADIUS)
						{
//							 u8 ret;
//							 _MessageBox("����ȷ���ð뾶","����",&ret);
							 return ERROR_PAINT;
						}
						GUI_SetColor(paint->color);
						GUI_DrawCircle(paint->x1, paint->y1, paint->radius );
				 break;
			 case SOLID_CIRCLE:
				    if(paint->x1 < 0 || paint->y1 < 0)
						{
//							 u8 ret;
//							 _MessageBox("����ȷ����Բ��","����",&ret);
							 return ERROR_PAINT;
						}
						if(paint->radius < 1 || paint->radius > MAX_RADIUS)
						{
//							 u8 ret;
//							 _MessageBox("����ȷ���ð뾶","����",&ret);
							 return ERROR_PAINT;
						}
						GUI_SetColor(paint->color);
						GUI_FillCircle(paint->x1, paint->y1, paint->radius );
				 break;
			 case HOLLOW_RECT:
				    if(paint->x1 < 0 || paint->y1 < 0 || paint->x2 < 0 || paint->y2 < 0)
						{
//							 u8 ret;
//							 _MessageBox("����ȷ��������յ�","����",&ret);
							 return ERROR_PAINT;
						}
						GUI_SetColor(paint->color);
						GUI_DrawRect( paint->x1, paint->y1, paint->x2 ,paint->y2);
				 break;
			 case SOLID_RECT:
				    if(paint->x1 < 0 || paint->y1 < 0 || paint->x2 < 0 || paint->y2 < 0)
						{
//							 u8 ret;
//							 _MessageBox("����ȷ��������յ�","����",&ret);
							 return ERROR_PAINT;
						}
						GUI_SetColor(paint->color);
			      GUI_FillRect(paint->x1, paint->y1, paint->x2, paint->y2);
				 break;
			 case STRIGHT_LINE:
				    if(paint->x1 < 0 || paint->y1 < 0 || paint->x2 < 0 || paint->y2 < 0)
						{
//							 u8 ret;
//							 _MessageBox("����ȷ��������յ�","����",&ret);
							 return ERROR_PAINT;
						}
						GUI_SetColor(paint->color);
					  GUI_DrawLine(paint->x1 ,paint->y1,paint->x2, paint->y2);
				 break;
		 }
		 return NO_ERROR;
}

//����GIF����
void DrawGIF (_Gif gif)
{
	   u8   g = gif;
	   if(g)
	   {
			 GUI_ClearRect( GIF_RECT_LEFT, GIF_RECT_TOP, GIF_RECT_RIGHT, GIF_RECT_BOTTOM);
			 switch (g)
			 {
				 case GIF_HAPPY:
							GUI_DrawBitmap(&bmHAPPY, 56, 86);
					 break;
				 case GIF_SAD:
							GUI_DrawBitmap(&bmSAD, 56, 86);
					 break;
				 case GIF_CRY:
							GUI_DrawBitmap(&bmCRY, 56, 86);
					 break;
				 case GIF_FURY:
					    GUI_DrawBitmap(&bmFURY, 56, 86);
					 break;
				 case GIF_ALARM:
							GUI_DrawBitmap(&bmALARM, 56, 86);
					 break;
				 default:break;
			 }
			  
	   }
}

//
//�������ߣ�������ֱ�������ƶ�
//ע:��Ϊangel_y��{-90,90}������ˮƽ���߲��ᳬ����Ļ
void drawHLineLong(int deltaX)
{
		 int   x_start = 20;
	   int   y_start = 120 + deltaX;
	   
		 GUI_SetColor(GUI_RED);
	   GUI_FillPolygon(_aPointHLineLong, 4, x_start, y_start);
}
//
//�������ߣ�����ˮƽ�������ƶ�
//ע����Ϊangle_x��{-180,180}��������ֱ���߻ᳬ����Ļ
void drawVLineLong(int deltaY)
{
		  int   x_start = 100 + deltaY;
			int   y_start = 40;
			
			if(x_start < 0)
				x_start += 240;
			else
				if(x_start > 240)
					x_start -= 240;
				
		  GUI_SetColor(GUI_RED);
	    GUI_FillPolygon(_aPointVLineLong, 4, x_start, y_start);
}
/***************************** �����޿Ƽ� www.apollorobot.com (END OF FILE) *********************************/

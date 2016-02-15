#ifndef  __WIDGET_DRAWSCALE_H
#define  __WIDGET_DRAWSCALE_H

#include "DIALOG.h"
#include "_apollorobot.h"


extern void   DrawForwardIcon(void);
extern void   DrawBackIcon(void);
extern void   DrawLeftIcon(void);
extern void   DrawRightIcon(void);
extern void   DrawStopIcon(void);
extern void   Init_Paint(_Paint *paint);
extern _Error Paint_Config(_Paint * paint);
extern void   DrawGIF (_Gif gif);
#endif


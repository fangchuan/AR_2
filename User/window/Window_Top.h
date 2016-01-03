#ifndef  _WINDOWDLG_H
#define  _WINDOWDLG_H

#include "stm32f10x.h"
#include "DIALOG.h"
#include "bmp_bt.h"
#include "bmp_pc.h"
#include "bmp_program.h"
#include "bmp_rc.h"
#include "bmp_sb.h"
#include "bmp_manual.h"
#include "SongTi16.h"

WM_HWIN CreateWindow_Top(void);
extern WM_HWIN hWin_Top;

#endif //

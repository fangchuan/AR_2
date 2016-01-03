/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin V5.22.                           *
*        Compiled Jul  4 2013, 12:18:24                              *
*        (c) 1998 - 2013 Segger Microcontroller GmbH && Co. KG       *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: bt                                                    *
* Dimensions:  80 * 50                                               *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "bmp_bt.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmbt;

static GUI_CONST_STORAGE unsigned short _acbt[] = {
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0xC618, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x8410, 0x8410, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0xC618, 0xC618, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0xC618, 0xC618, 0x8410, 0x8410, 0x8410, 0xC618, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0410, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0410, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0410, 0x0410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0410, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0410, 0x0410, 0x8410, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 
        0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0x0010, 0x8410, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0xC618, 0xC618, 0xFFFF, 0x8410, 0xC618, 0xC618, 0xC618, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x8410, 0x8410, 
        0x8410, 0x0010, 0x0010, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0xC618, 0xC618, 0x0410, 0x8410, 0xC618, 0xC618, 0x8410, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x8410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x0410, 0x07FF, 0x0410, 0xC618, 0xC618, 0x8410, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x8410, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0x8410, 0x0410, 0x0410, 0xC618, 0x8410, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0x8410, 0x0010, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xFFFF, 0xFFFF, 0x8410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0410, 0x0410, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x8410, 
        0x8410, 0x0010, 0x8410, 0xC618, 0xC618, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x0010, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xFFFF, 0xC618, 0x8410, 0x8410, 0x8410, 0xC618, 0x8410, 0x07FF, 0x0410, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0x0410, 0x8410, 0x0010, 0xC618, 0xC618, 0x8410, 
        0x8410, 0x8410, 0x8410, 0x8410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0xFFFF, 0xC618, 0x0010, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x07FF, 0x0410, 0x8410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x0010, 0x8410, 0x0010, 0xC618, 0xC618, 0x8410, 0x8410, 0x8410, 0x8410, 
        0xC618, 0xFFFF, 0x8410, 0x0010, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0x8410, 0x0410, 0x8410, 0x8410, 0x0010, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0x8410, 0xC618, 0xC618, 0xC618, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x0010, 0x8410, 0xC618, 0xC618, 0x8410, 0x8410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0x8410, 0x0410, 0xC618, 0xC618, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xC618, 0xC618, 0xC618, 0x8410, 0x0410, 0xC618, 0xC618, 0xC618, 0xC618, 0x07FF, 0x0410, 0x0410, 0x8410, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0x0010, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x8410, 
        0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xC618, 0x0410, 0x0410, 0x0410, 0x8410, 0xC618, 0x8410, 0xC618, 0x07FF, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0010, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0x8410, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x8410, 0x0410, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0x8410, 0x8410, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x0410, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0x8410, 0x8410, 0x8410, 0xC618, 0x0410, 0x0010, 0x0410, 0x0410, 
        0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0410, 0x8010, 0x8410, 0x8410, 0x0410, 0x0010, 0x0410, 0x0410, 0x8410, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x0410, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xFFFF, 0xFFFF, 
        0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0410, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x0410, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x8410, 0xFFFF, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0410, 0x0010, 0x0010, 0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0x0010, 0x0010, 
        0x8410, 0x0010, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0xC618, 0x8410, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0x8410, 0xC618, 0x0010, 0xC618, 0xC618, 
        0xC618, 0xC618, 0x0010, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0410, 0x0010, 0x0010, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x8410, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0x0010, 0x0010, 0x0010, 0x0010, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0x8410, 0x0010, 0x8410, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 
        0xC618, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0xC618, 0xC618, 
        0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 
        0xC618, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x8410, 0x8410, 0x0410, 0x0010, 0x0010, 0x0010, 0x8410, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 0x8410, 
        0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0410, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0x0010, 0x0010, 0x0410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 
        0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0x8410, 0x8410, 0xC618, 0xC618, 0x8410, 0x0410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x07FF, 0x07FF, 0x0410, 0x07FF, 0x0410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0x8410, 0x8410, 0xC618, 0xC618, 0x8410, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0x8410, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 
        0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0410, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0410, 0x8410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0x8410, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0x8410, 0x8410, 0x0010, 0x0010, 0x0010, 0x0010, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 
        0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0410, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x8410, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0x0010, 0xC618, 0xC618, 0xC618, 
        0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0x8410, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0x0410, 0xC618, 0x8410, 0x8410, 0x0410, 0x07FF, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x8410, 0x8410, 0x8410, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0x0410, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0x0010, 0xC618, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0010, 0x0010, 0x0010, 
        0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xFFFF, 0xC618, 0xFFFF, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 
        0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xC618, 
        0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

GUI_CONST_STORAGE GUI_BITMAP bmbt = {
  80, // xSize
  50, // ySize
  160, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_acbt,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMP565
};

/*************************** End of file ****************************/

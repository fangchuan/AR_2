#ifndef  __WINDOW_TREEVIEW_H
#define  __WINDOW_TREEVIEW_H

#include "DIALOG.h"
#include "stdint.h"

#define FILE_NAME_LEN 	50							//文件名长度，如果检测到文件名超过50 则丢弃这个文件 
#define PATH_LEN		    50              //路径长度
#define FILE_LIST_PATH 			"0:/FILELIST.TXT"	//文件记录列表文件的目录
//#define _DF1S        0x80   //支持长文件名的,我没用长文件名

extern  WM_HWIN hTree;
extern  WM_HWIN CreateWindow_TreeView(void);
extern  void WriteFileProcess(void);
#endif //__WINDOW_TREEVIEW_H

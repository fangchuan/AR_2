#ifndef  __WINDOW_TREEVIEW_H
#define  __WINDOW_TREEVIEW_H

#include "DIALOG.h"
#include "stdint.h"

#define FILE_NAME_LEN 	50							//�ļ������ȣ������⵽�ļ�������50 ��������ļ� 
#define PATH_LEN		    50              //·������
#define FILE_LIST_PATH 			"0:/FILELIST.TXT"	//�ļ���¼�б��ļ���Ŀ¼
//#define _DF1S        0x80   //֧�ֳ��ļ�����,��û�ó��ļ���

extern  WM_HWIN hTree;
extern  WM_HWIN CreateWindow_TreeView(void);
extern  void WriteFileProcess(void);
#endif //__WINDOW_TREEVIEW_H

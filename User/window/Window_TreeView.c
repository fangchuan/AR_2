/*
*********************************************************************************************************
*
*	模块名称 : 编程指令解析模块
*	文件名称 : WIDGET_TreeView.c
*	版    本 : V1.0
*	说    明 : 生成文件目录树，都是叶子文件。
*            功能包括文件保存、文件删除、硬盘(FLASH)格式化
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/

#include "includes.h"

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern  FIL        file;
extern  FRESULT  result;
extern  UINT         bw;	
extern char program_name[20];//the name of program


WM_HWIN hTree;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0       (GUI_ID_USER + 0x00)
#define ID_TREEVIEW_FILE  (GUI_ID_USER + 0x02)
#define ID_BUTTON_FORMAT  (GUI_ID_USER + 0x03)
#define ID_BUTTON_BACK    (GUI_ID_USER + 0x04)
#define ID_BUTTON_DEL			(GUI_ID_USER + 0x05) //
#define ID_BUTTON_OPEN    (GUI_ID_USER + 0x07)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//static uint16_t *buffer=0;
//static char *txtBuffer=0;
//static char* record_file=0;
//static char DeleteProgram[10] = {0};
static const char *StringHZ[] = {
	"\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96",//0:格式化
	"\xe8\xbf\x94\xe5\x9b\x9e","\xe4\xb8\xbb\xe7\xa8\x8b\xe5\xba\x8f",//1:返回   2:主程序
	"\xe5\x88\xa0\xe9\x99\xa4\xe7\xa8\x8b\xe5\xba\x8f",//3:删除程序
	"\xe6\x98\xaf\xe5\x90\xa6\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96?",//4:是否格式化?
	"\xe9\x94\x99\xe8\xaf\xaf","\xe6\x88\x90\xe5\x8a\x9f",//5:错误  6:成功
	"\xe6\x88\x90\xe5\x8a\x9f\xe5\x88\xa0\xe9\x99\xa4\xe7\xa8\x8b\xe5\xba\x8f",//7:成功删除程序
	"\xe9\x80\x9a\xe7\x9f\xa5",//8:通知
	"\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96\xe7\xa3\x81\xe7\x9b\x98\xe6\x88\x90\xe5\x8a\x9f!",//9:格式化磁盘成功!
	"\xe6\x89\x93\xe5\xbc\x80\xe7\xa8\x8b\xe5\xba\x8f",//10:打开程序
	"\xe9\x94\x99\xe8\xaf\xaf\xe7\x9a\x84\xe6\x96\x87\xe4\xbb\xb6\xe7\xb1\xbb\xe5\x9e\x8b",//11:错误的文件类型
	"\xe6\x98\xaf\xe5\x90\xa6\xe5\x88\xa0\xe9\x99\xa4\xe7\xa8\x8b\xe5\xba\x8f?",   //12.是否删除程序?
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { TREEVIEW_CreateIndirect, "Treeview", ID_TREEVIEW_FILE, 0, 0, 240, 193, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,"打开程序", ID_BUTTON_OPEN, 0, 200, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,"格式化", ID_BUTTON_FORMAT, 0, 280, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,"返回", ID_BUTTON_BACK,180, 290, 60,  29, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "删除程序", ID_BUTTON_DEL, 0, 240, 80, 30, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};
/*********************************************************************
*
*       _bmSmilie
*/
static const GUI_COLOR _ColorsSmilie[] = {
     0xFFFFFF,0x000000,0x70FF70
};

static const GUI_LOGPALETTE _PalSmilie = {
  3,	// Number of entries
  1, 	// Has transparency
  &_ColorsSmilie[0]
};

static const unsigned char _acSmilie[] = {
  0x00, 0x55, 0x40, 0x00,
  0x01, 0xAA, 0x90, 0x00,
  0x06, 0xAA, 0xA4, 0x00,
  0x19, 0x6A, 0x59, 0x00,
  0x69, 0x6A, 0x5A, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xAA, 0xAA, 0x40,
  0x1A, 0x6A, 0x69, 0x00,
  0x06, 0x95, 0xA4, 0x00,
  0x01, 0xAA, 0x90, 0x00,
  0x00, 0x55, 0x40, 0x00
};

static const GUI_BITMAP _bmSmilie = {
 13,          // XSize
 13,          // YSize
 4,           // BytesPerLine
 2,           // BitsPerPixel
 _acSmilie,   // Pointer to picture data (indices)
 &_PalSmilie  // Pointer to palette
};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void OpenFileProcess(int sel_num )
{
	char                     openfile[FILE_NAME_LEN]={0};
	_Listptr   p = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
	
	result = f_open (&file, FILE_LIST_PATH, FA_READ|FA_OPEN_EXISTING); //打开索引文件
	if(result != FR_OK)
      return ;
	result = f_lseek (&file, sel_num*FILE_NAME_LEN);
	if(result != FR_OK)
      return ;
	result = f_read (&file, openfile, FILE_NAME_LEN, &bw);//从filelist文件中找到被选择的那个文件名openfile	
	if(result != FR_OK)
      return ;

	f_close (&file);
	
	result = f_open(&file, openfile, FA_READ | FA_OPEN_EXISTING);
	if(result != FR_OK)
		return ;
	if(!p)
		return ;
	else{
				u16      NumBytesPerList = sizeof(_Instructor);
				u8       Mb_Val;
				do{
						result = f_read(&file,p, NumBytesPerList, &bw);
						if(result != FR_OK){
								_MessageBox("Fail to read","Error",&Mb_Val);	
								return ;
						}
						Add_Node(Ins_List_Head, p->index , p->_flag ,p->EditContent );
				}while(p->next); //链表尾结点处的next指针为空，表示最后一个结点，不用再往后读了。
				myfree(SRAMIN, p);
				f_close(&file);
	}
	strcpy(program_name,openfile+3);//将要打开的程序文件名赋给program_name.因为openfile是路径名，要去掉"0:/"
//		txt2buffer(openfile);//将openfile文件的内容转换到UTF8编码的txtbuffer字符串中
	CreateWindow_1_1();//利用保存起来的程序创建EDIT
}


//删除一个程序文件
static int DeleteFileProcess(int sel_num )
{
	char                     deletefile[FILE_NAME_LEN]={0};
	
	result = f_open (&file, FILE_LIST_PATH, FA_READ|FA_OPEN_EXISTING); //打开索引文件
	if(result != FR_OK)
      return -1;
	result = f_lseek (&file, sel_num*FILE_NAME_LEN);
	if(result != FR_OK)
      return -1;
	result = f_read (&file, deletefile, FILE_NAME_LEN, &bw);//从filelist文件中找到被选择的那个文件名deletefile	
	if(result != FR_OK)
      return -1;

	f_close (&file);
	
	result = f_unlink(deletefile);
	if(result != FR_OK)
		return -1;
  else
		return 0;
}


/**
  * @brief  scan_files 递归扫描flash内的文件
  * @param  path:初始扫描路径 file_name：指向用来存储文件名的一段空间 hFile:用于记录文件路径的文件指针 hTree 目录树 hNode 目录结点
	*					hTree == NULL &&	hNode == NULL 的话，不创建目录树			
  * @retval result:文件系统的返回值
  */
static FRESULT scan_files (char* path,char* file_name,FIL *hFile,WM_HWIN hTree, TREEVIEW_ITEM_Handle hNode,int *fileNum) 
{ 
		
    FRESULT        res; 		          //部分在递归过程被修改的变量，不用全局变量	
    FILINFO        fno; 
		uint32_t    rw_num;			//已读或已写的字节数
    DIR dir; 
    int i; 
    char *fn; 	
		TREEVIEW_ITEM_Handle hItem=0;

	
#if _USE_LFN      
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1]; 	//长文件名支持
    fno.lfname = lfn; 
    fno.lfsize = sizeof(lfn); 
#endif 

    res = f_opendir(&dir, path);                            //打开目录
    if (res == FR_OK) 
		{ 
        i = strlen(path); 
        for (;;) 
				{ 
            res = f_readdir(&dir, &fno); 										//读取目录下的内容
            if (res != FR_OK || fno.fname[0] == 0) break; 	//为空时表示所有项目读取完毕，跳出
#if _USE_LFN 
            fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
            fn = fno.fname; 
#endif 
            if (*fn == '.') continue; //点表示当前目录，跳过			
            if (fno.fattrib & AM_DIR) //目录，递归读取
						{ 																							
							
							if(hTree != NULL &&	hNode != NULL)
							{
								hItem = TREEVIEW_ITEM_Create(TREEVIEW_ITEM_TYPE_NODE,fn,0);
								TREEVIEW_AttachItem(hTree,hItem,hNode,TREEVIEW_INSERT_FIRST_CHILD);		//把结点加入到目录树中
							}
											
							sprintf(&path[i], "/%s", fn); 							//合成完整目录名
							res = scan_files(path,file_name,hFile,hTree,hItem,fileNum);		//递归遍历 
							if (res != FR_OK) 
													break; 																		//打开失败，跳出循环
							path[i] = 0; 
            } 
						else 																														//是文件
						{ 				
							/* 根据要求是否创建目录树 */
							if(hTree != NULL &&	hNode != NULL)																			//创建目录树
							{
								hItem = TREEVIEW_ITEM_Create(TREEVIEW_ITEM_TYPE_LEAF,fn,0);
								TREEVIEW_AttachItem(hTree,hItem,hNode,TREEVIEW_INSERT_FIRST_CHILD);		//把树叶添加到目录树
						    TREEVIEW_ITEM_SetImage(hItem, TREEVIEW_BI_LEAF, &_bmSmilie);
								if(strcmp(fn, "FILELIST.TXT") != 0)
								{
									if (strlen(path)+strlen(fn)<FILE_NAME_LEN)
									{
										sprintf(file_name, "%s/%s", path,fn); 	
										
										//存储文件名到filelist(含路径)										
										res = f_lseek (hFile, hItem*FILE_NAME_LEN);  
										res = f_write (hFile, file_name, FILE_NAME_LEN, &rw_num);	
										//printf("\nfileItem=%ld:%s",hItem,file_name);
									}		
								}								
							}
						}//else
        } //for
    } 

    return res; 
} 


/**
  * @brief  Fill_FileList处理非递归过程，然后调用递归函数scan_files扫描目录
	*					
  * @param  path:初始扫描路径
  * @retval none
  */
void Fill_FileList(char* path, WM_HWIN hTree, TREEVIEW_ITEM_Handle hNode,int *p)
{
	char  p_path[PATH_LEN]={0};									//目录名 指针
	char  file_name[FILE_NAME_LEN]={0};								//用于存储的目录文件名，
	result = f_unlink(FILE_LIST_PATH);//暂时删除旧的目录， 增加自建目录
	result = f_open (&file, FILE_LIST_PATH, FA_READ|FA_WRITE| FA_OPEN_ALWAYS ); //索引文件,若不存在则创建一个索引文件
  if(result != FR_OK)
      return ;
	strcpy(p_path,path);						//复制目录名到指针
	
	result = scan_files(p_path,file_name,&file,hTree,hNode,p);			//递归扫描文件		
	
	f_close (&file);					//关闭索引文件	

}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  static TREEVIEW_ITEM_Handle hNode;			//结点句柄
  TREEVIEW_ITEM_INFO ItemInfo;
  WM_HWIN                hItem;
  int                    NCode;
  int                    Id;
//  WM_HWIN      hNumPad;
	u8            Mb_Val;
	
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_WHITE);
    //
    // Initialization of 'Treeview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TREEVIEW_FILE);
		TREEVIEW_SetFont(hItem, &GUI_FontSongTi12);
		TREEVIEW_SetDefaultFont(&GUI_FontSongTi12);
		TREEVIEW_SetAutoScrollH(hItem,1);
		TREEVIEW_SetAutoScrollV(hItem,1);
		TREEVIEW_SetSelMode(hItem, TREEVIEW_SELMODE_ROW);
		hNode = TREEVIEW_InsertItem(hItem, TREEVIEW_ITEM_TYPE_NODE, 0, 0, StringHZ[2]);
		//更新filelist文件
		Fill_FileList("0:", hItem ,hNode,(void*)0);
		TREEVIEW_ITEM_Expand(hNode);//展开指定结点
    //
    // Initialization of 'Button'
    //
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OPEN);
    BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[10]);
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_FORMAT);
    BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem,StringHZ[0]);

		hItem = WM_GetDialogItem(pMsg->hWin ,ID_BUTTON_BACK);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem, StringHZ[1]);
		
		hItem = WM_GetDialogItem(pMsg->hWin ,ID_BUTTON_DEL);
		BUTTON_SetFont(hItem, &GUI_FontSongTi12);
		BUTTON_SetText(hItem, StringHZ[3]);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_TREEVIEW_FILE: // Notifications sent by 'Treeview'
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:	
									 /* 查看选中了哪个项目 */
									 hNode = TREEVIEW_GetSel(pMsg->hWinSrc);						
							break;
						case WM_NOTIFICATION_MOVED_OUT:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
							break;
					}
					break;
		case ID_BUTTON_BACK:
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
									GUI_EndDialog(pMsg->hWin ,0);
							break;
					}
				break;
		case ID_BUTTON_OPEN: 
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
                 if(hNode)
								 {
									 /* 获取该项目的信息 */
						       TREEVIEW_ITEM_GetInfo(hNode,&ItemInfo);	
									 if(ItemInfo.IsNode == 0)        //点击的是目录树的叶子（即文件）
									 {
											OpenFileProcess(hNode);
									 }
							   }
							break;
					}
				break;
		case ID_BUTTON_DEL: // Notifications sent by 'Delete Program'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:

			         if(hNode)
							 {
								 _MessageBox(StringHZ[12],StringHZ[8], &Mb_Val);
								 if(Mb_Val == GUI_ID_OK)
								 {
									 if(DeleteFileProcess(hNode))
									 {  //删除出错
											_MessageBox(StringHZ[11],StringHZ[5], &Mb_Val);
									 }			
									 else
									 {  //删除成功
											TREEVIEW_ITEM_Delete(hNode);
											_MessageBox(StringHZ[7],StringHZ[6], &Mb_Val);
									 }
								 }
							}
				break;
      }
      break;
		case ID_BUTTON_FORMAT:  //格式化FLASH
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
									_MessageBox(StringHZ[4],StringHZ[8], &Mb_Val);
							    if(Mb_Val == GUI_ID_OK)
									{
										W25QXX_Erase_Chip();
										_MessageBox(StringHZ[9],StringHZ[6], &Mb_Val);
									}
									
							break;
					}
				break;
    }
    break;
	case WM_PAINT:
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
WM_HWIN CreateWindow_TreeView(void);
WM_HWIN CreateWindow_TreeView(void) {

  hTree = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hTree;
}


/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

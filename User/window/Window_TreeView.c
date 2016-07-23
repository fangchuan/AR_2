/*
*********************************************************************************************************
*
*	ģ������ : ���ָ�����ģ��
*	�ļ����� : WIDGET_TreeView.c
*	��    �� : V1.0
*	˵    �� : �����ļ�Ŀ¼��������Ҷ���ļ���
*            ���ܰ����ļ����桢�ļ�ɾ����Ӳ��(FLASH)��ʽ��
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-03-01 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
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
extern char program_name[10];//the name of program


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
	"\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96",//0:��ʽ��
	"\xe8\xbf\x94\xe5\x9b\x9e","\xe4\xb8\xbb\xe7\xa8\x8b\xe5\xba\x8f",//1:����   2:������
	"\xe5\x88\xa0\xe9\x99\xa4\xe7\xa8\x8b\xe5\xba\x8f",//3:ɾ������
	"\xe6\x98\xaf\xe5\x90\xa6\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96?",//4:�Ƿ��ʽ��?
	"\xe9\x94\x99\xe8\xaf\xaf","\xe6\x88\x90\xe5\x8a\x9f",//5:����  6:�ɹ�
	"\xe6\x88\x90\xe5\x8a\x9f\xe5\x88\xa0\xe9\x99\xa4\xe7\xa8\x8b\xe5\xba\x8f",//7:�ɹ�ɾ������
	"\xe9\x80\x9a\xe7\x9f\xa5",//8:֪ͨ
	"\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96\xe7\xa3\x81\xe7\x9b\x98\xe6\x88\x90\xe5\x8a\x9f!",//9:��ʽ�����̳ɹ�!
	"\xe6\x89\x93\xe5\xbc\x80\xe7\xa8\x8b\xe5\xba\x8f",//10:�򿪳���
	"\xe9\x94\x99\xe8\xaf\xaf\xe7\x9a\x84\xe6\x96\x87\xe4\xbb\xb6\xe7\xb1\xbb\xe5\x9e\x8b",//11:������ļ�����
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { TREEVIEW_CreateIndirect, "Treeview", ID_TREEVIEW_FILE, 0, 0, 240, 193, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,"�򿪳���", ID_BUTTON_OPEN, 0, 200, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,"��ʽ��", ID_BUTTON_FORMAT, 0, 280, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,"����", ID_BUTTON_BACK,180, 300, 60,  20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "ɾ������", ID_BUTTON_DEL, 0, 240, 80, 30, 0, 0x0, 0 },
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
/*
//�����ֵ�Unicode��ת����UTF8������ַ���
//static void gbk2utf8(const char *src, char *str,uint32_t maxnum)
//{
//	uint32_t j=0,k=0;
//	uint16_t gbkdata=0;
//	uint16_t UCbuffer[50]={0};
//	for(j=0,k=0;(j<maxnum)&&src[j]!='\0';k++)
//	{
//		if((uint8_t)src[j]>0x80)
//		{
//			gbkdata=src[j+1]+src[j]*256;
//			UCbuffer[k]=ff_convert(gbkdata,1);
//			j+=2;
//		}
//		else
//		{
//			UCbuffer[k]=0x00ff&src[j];
//			j+=1;
//		}
//	}
//	UCbuffer[k]='\0';
//	GUI_UC_ConvertUC2UTF8(UCbuffer,2*k+2,str,k*3);
//}

//���ı��ļ�ת����UTF8������ַ���txtbuffer
//static uint8_t  txt2buffer(const char * sFilename) 
//{
//	OS_ERR      	err;
//	uint32_t j=0,k=0;
//	uint16_t gbkdata=0;
//	buffer = (uint16_t *)(0x680c0000);//����ź���unicode���bufferָ��EXTERN_SRAM�����200K
//	txtBuffer = (char *)(0x680c0000+1024*50*2);//������ļ����ݵ�txtbufferָ��0x680d9000
//	OSSchedLock(&err);
//	
//	result = f_open(&file, sFilename, FA_READ);
//	if (result != FR_OK)
//	{
//		OSSchedUnlock(&err);
//		return 0;
//	}
//	//printf("filesize=%d\n",file.fsize);
//	if(file.fsize>=1024*50*2)
//	{
//		printf("Note read fail!!!\n");
//		OSSchedUnlock(&err);
//		return 0;
//  }
//	result = f_read(&file, txtBuffer, file.fsize, &bw);
//	if (result != FR_OK)
//  {
//		OSSchedUnlock(&err);
//		return 0;
//  }
//	//���ı�����ת������Ӧ��Unicode���ŵ�buffer��
//	for(j=0,k=0;(j<file.fsize)&&txtBuffer[j]!='\0';k++)
//	{
//		if((uint8_t)txtBuffer[j]>0x80)
//		{
//			gbkdata=txtBuffer[j+1]+txtBuffer[j]*256;
//			buffer[k]=ff_convert(gbkdata,1);
//			j+=2;
//		}
//		else
//		{
//			buffer[k]=0x00ff&txtBuffer[j];
//			j+=1;
//		}
//	}
//	buffer[k]='\0';		
//	GUI_UC_ConvertUC2UTF8(buffer,2*k+2,txtBuffer,k*3);
//	f_close(&file);
//	OSSchedUnlock(&err);
//	return 1;	
//}
*/


/**
  * @brief  OpenFileProcess���ļ�	 
	*					
  * @param  none
  * @retval none
  */
static void OpenFileProcess(int sel_num )
{
	char                     openfile[FILE_NAME_LEN]={0};
	_Listptr   p = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
	
	result = f_open (&file, FILE_LIST_PATH, FA_READ|FA_OPEN_EXISTING); //�������ļ�
	if(result != FR_OK)
      return ;
	result = f_lseek (&file, sel_num*FILE_NAME_LEN);
	if(result != FR_OK)
      return ;
	result = f_read (&file, openfile, FILE_NAME_LEN, &bw);//��filelist�ļ����ҵ���ѡ����Ǹ��ļ���openfile	
	if(result != FR_OK)
      return ;

	f_close (&file);
	
	f_open(&file, openfile, FA_READ | FA_OPEN_EXISTING);
	if(result != FR_OK)
		return ;
	if(!p)
		return ;
	else{
				u16      NumBytesPerList = sizeof(_Instructor);
				do{
						result = f_read(&file,p, NumBytesPerList, &bw);
						if(result != FR_OK)
							return ;
						Add_Node(Ins_List_Head, p->index , p->_flag ,p->EditContent );
				}while(p->next); //����β��㴦��nextָ��Ϊ�գ���ʾ���һ����㣬������������ˡ�
				myfree(SRAMIN, p);
	}
	strcpy(program_name,openfile+3);//��Ҫ�򿪵ĳ����ļ�������program_name.��Ϊopenfile��·������Ҫȥ��"0:/"
//		txt2buffer(openfile);//��openfile�ļ�������ת����UTF8�����txtbuffer�ַ�����
	CreateWindow_1_1();//���ñ��������ĳ��򴴽�EDIT
}


//ɾ��һ�������ļ�
static int DeleteFileProcess(int sel_num )
{
	char                     deletefile[FILE_NAME_LEN]={0};
	
	result = f_open (&file, FILE_LIST_PATH, FA_READ|FA_OPEN_EXISTING); //�������ļ�
	if(result != FR_OK)
      return -1;
	result = f_lseek (&file, sel_num*FILE_NAME_LEN);
	if(result != FR_OK)
      return -1;
	result = f_read (&file, deletefile, FILE_NAME_LEN, &bw);//��filelist�ļ����ҵ���ѡ����Ǹ��ļ���deletefile	
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
  * @brief  scan_files �ݹ�ɨ��flash�ڵ��ļ�
  * @param  path:��ʼɨ��·�� file_name��ָ�������洢�ļ�����һ�οռ� hFile:���ڼ�¼�ļ�·�����ļ�ָ�� hTree Ŀ¼�� hNode Ŀ¼���
	*					hTree == NULL &&	hNode == NULL �Ļ���������Ŀ¼��			
  * @retval result:�ļ�ϵͳ�ķ���ֵ
  */
static FRESULT scan_files (char* path,char* file_name,FIL *hFile,WM_HWIN hTree, TREEVIEW_ITEM_Handle hNode,int *fileNum) 
{ 
		
    FRESULT        res; 		          //�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���	
    FILINFO        fno; 
		uint32_t    rw_num;			//�Ѷ�����д���ֽ���
    DIR dir; 
    int i; 
    char *fn; 	
		TREEVIEW_ITEM_Handle hItem=0;

	
#if _USE_LFN      
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1]; 	//���ļ���֧��
    fno.lfname = lfn; 
    fno.lfsize = sizeof(lfn); 
#endif 

    res = f_opendir(&dir, path);                            //��Ŀ¼
    if (res == FR_OK) 
		{ 
        i = strlen(path); 
        for (;;) 
				{ 
            res = f_readdir(&dir, &fno); 										//��ȡĿ¼�µ�����
            if (res != FR_OK || fno.fname[0] == 0) break; 	//Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
#if _USE_LFN 
            fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
            fn = fno.fname; 
#endif 
            if (*fn == '.') continue; //���ʾ��ǰĿ¼������			
            if (fno.fattrib & AM_DIR) //Ŀ¼���ݹ��ȡ
						{ 																							
							
							if(hTree != NULL &&	hNode != NULL)
							{
								hItem = TREEVIEW_ITEM_Create(TREEVIEW_ITEM_TYPE_NODE,fn,0);
								TREEVIEW_AttachItem(hTree,hItem,hNode,TREEVIEW_INSERT_FIRST_CHILD);		//�ѽ����뵽Ŀ¼����
							}
											
							sprintf(&path[i], "/%s", fn); 							//�ϳ�����Ŀ¼��
							res = scan_files(path,file_name,hFile,hTree,hItem,fileNum);		//�ݹ���� 
							if (res != FR_OK) 
													break; 																		//��ʧ�ܣ�����ѭ��
							path[i] = 0; 
            } 
						else 																														//���ļ�
						{ 				
							/* ����Ҫ���Ƿ񴴽�Ŀ¼�� */
							if(hTree != NULL &&	hNode != NULL)																			//����Ŀ¼��
							{
								hItem = TREEVIEW_ITEM_Create(TREEVIEW_ITEM_TYPE_LEAF,fn,0);
								TREEVIEW_AttachItem(hTree,hItem,hNode,TREEVIEW_INSERT_FIRST_CHILD);		//����Ҷ��ӵ�Ŀ¼��
						    TREEVIEW_ITEM_SetImage(hItem, TREEVIEW_BI_LEAF, &_bmSmilie);
								if(strcmp(fn, "FILELIST.TXT") != 0)
								{
									if (strlen(path)+strlen(fn)<FILE_NAME_LEN)
									{
										sprintf(file_name, "%s/%s", path,fn); 	
										
										//�洢�ļ�����filelist(��·��)										
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
  * @brief  Fill_FileList����ǵݹ���̣�Ȼ����õݹ麯��scan_filesɨ��Ŀ¼
	*					
  * @param  path:��ʼɨ��·��
  * @retval none
  */
void Fill_FileList(char* path, WM_HWIN hTree, TREEVIEW_ITEM_Handle hNode,int *p)
{
	char  p_path[PATH_LEN]={0};									//Ŀ¼�� ָ��
	char  file_name[FILE_NAME_LEN]={0};								//���ڴ洢��Ŀ¼�ļ�����
	result = f_unlink(FILE_LIST_PATH);//��ʱɾ���ɵ�Ŀ¼�� �����Խ�Ŀ¼
	result = f_open (&file, FILE_LIST_PATH, FA_READ|FA_WRITE| FA_OPEN_ALWAYS ); //�����ļ�,���������򴴽�һ�������ļ�
  if(result != FR_OK)
      return ;
	strcpy(p_path,path);						//����Ŀ¼����ָ��
	
	result = scan_files(p_path,file_name,&file,hTree,hNode,p);			//�ݹ�ɨ���ļ�		
	
	f_close (&file);					//�ر������ļ�	

}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  static TREEVIEW_ITEM_Handle hNode;			//�����
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
		//����filelist�ļ�
		Fill_FileList("0:", hItem ,hNode,(void*)0);
		TREEVIEW_ITEM_Expand(hNode);//չ��ָ�����
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
						 /* �鿴ѡ�����ĸ���Ŀ */
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
									 /* ��ȡ����Ŀ����Ϣ */
						       TREEVIEW_ITEM_GetInfo(hNode,&ItemInfo);	
									 if(ItemInfo.IsNode == 0)        //�������Ŀ¼����Ҷ�ӣ����ļ���
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
			           if(DeleteFileProcess(hNode))
			           {  //ɾ������
								    _MessageBox(StringHZ[11],StringHZ[5], &Mb_Val);
								 }			
								 else
								 {  //ɾ���ɹ�
									  TREEVIEW_ITEM_Delete(hNode);
									  _MessageBox(StringHZ[7],StringHZ[6], &Mb_Val);
								 }
						   }
				break;
      }
      break;
		case ID_BUTTON_FORMAT:  //��ʽ��FLASH
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


/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

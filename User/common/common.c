/*
*********************************************************************************************************
*
*	模块名称 : 通用函数模块
*	文件名称 : common.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
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

_StatuStack StaStk;    //表示代码嵌套层次的状态栈 

/*
*********************************************************************************************************
*	函 数 名: string_subs_char
*	功能说明: 字符串减去一个字符  ,每次只去除字符串中的‘_’
*	形    参：a:字符串  
*	返 回 值: 0:查找到一个下划线   -1:没查找到下划线
*********************************************************************************************************
*/
int  string_subs_char(char* a)
{
	   char* pos_address;
			//寻找第一个'_'所在位置指针
	   pos_address = strchr(a, '_');
	   if(pos_address)
		 {
			 strtok(a, "_");
			 strcat(a, pos_address+1);
			 return  0;
		 }
		 else
		 {
			 return  -1;
		 }
		 
}
/*
*********************************************************************************************************
*	函 数 名: WriteFileProcess
*	功能说明: 写入用户保存的文本，都是叶子项目，没有结点项目
*	形    参：program_name:程序名指针
*	返 回 值: 0:成功,  返回-1则是出错
*********************************************************************************************************
*/
int WriteFileProcess(char* program_name)
{
		_Listptr  p = Ins_List_Head -> next;
		int      listLength = GetListLength(Ins_List_Head);
		int      NumBytesPerList = sizeof(_Instructor);
		char     path[PATH_LEN] = {0};
	
		sprintf(path,"%s/%s","0:",program_name);

		result = f_open(&file,path,FA_WRITE | FA_CREATE_ALWAYS);//新建一个程序文件，如果该程序文件已存在，则覆盖原文件
		if(result != FR_OK)
			return  -1;
		while(listLength > 0)
		{
			if(p)
			{	
				result = f_write(&file, p, NumBytesPerList, &bw);
				if(result != FR_OK)
					return  -1;
				p = p -> next;
			}
			listLength --;
		}
		f_close(&file);
		return 0;
}
/*
*********************************************************************************************************
*	函 数 名: Create_Stack
*	功能说明: 初始化一个状态栈
*	形    参：
*	返 回 值: 0:成功,  返回-1则是出错
*********************************************************************************************************
*/
int Create_Stack(void)
{
		StaStk.base = (ELETYPE *)malloc(MAX_SIZE_STACK * sizeof(_StatuStack));
		if(!StaStk.base )
			return -1;
		else
		{
			StaStk.top = StaStk.base ;//当前栈为空栈
			StaStk.stacksize = MAX_SIZE_STACK;
			
			return 0;
		}
}
/*
*********************************************************************************************************
*	函 数 名: GetStackLength
*	功能说明: 返回当前栈的长度
*	形    参：_StatuStack指针
*	返 回 值: 当前堆栈长度,  返回-1则是出错
*********************************************************************************************************
*/
int GetStackLength(_StatuStack *stk)
{
		if(!stk->base)
			return -1;
		else
		{
			return (stk->top - stk->base );
		}
}
/*
*********************************************************************************************************
*	函 数 名: GetTop
*	功能说明: 获取栈顶元素，存入ele，不是出栈
*	形    参：_StatuStack指针， ele要存入元素的指针
*	返 回 值: 当前堆栈长度,  返回-1则是出错
*********************************************************************************************************
*/
int GetTop(_StatuStack *Stk,uint8_t *ele)
{
		if(!Stk->base || Stk->base == Stk->top )//若是空栈则返回-1
			return -1;
		else
		{
			*ele = *(Stk->top -1);
			return 0;
		}
}
/*
*********************************************************************************************************
*	函 数 名: Push
*	功能说明: 元素ele入栈操作
*	形    参：_StatuStack指针， ele要存入元素
*	返 回 值: 0:成功,  返回-1则是出错
*********************************************************************************************************
*/
int Push(_StatuStack *Stk, uint8_t ele)
{
		if(!Stk->base )
			return -1;
		else{
			*Stk->top++ = ele;
			return 0;
		}
}
/*
*********************************************************************************************************
*	函 数 名: Push
*	功能说明: 元素ele出栈
*	形    参：_StatuStack指针， ele要取出元素的指针
*	返 回 值: 0:成功,  返回-1则是出错
*********************************************************************************************************
*/
int Pop(_StatuStack *Stk, uint8_t *ele)
{
		if(!Stk->base|| Stk->base == Stk->top )//若是空栈则返回-1
			return -1;
		else{
			*ele = *(--Stk->top);
			return 0;
		}
}


/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

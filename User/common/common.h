/*
*********************************************************************************************************
*
*	模块名称 : 通用函数模块
*	文件名称 : common.h
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
#ifndef __COMMON_H
#define __COMMON_H

#define FILE_NAME_LEN 	50							//文件名长度，如果检测到文件名超过50 则丢弃这个文件 
#define PATH_LEN		    50              //路径长度
#define FILE_LIST_PATH 			"0:/FILELIST.TXT"	//文件记录列表文件的目录
//#define _DF1S        0x80   //支持长文件名的,我没用长文件名
#define G_CONST        9.80

extern int  string_subs_char(char* a);
extern int  WriteFileProcess(char* program_name);	

#endif

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

/*
*********************************************************************************************************
*
*	ģ������ : ͨ�ú���ģ��
*	�ļ����� : common.h
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef __COMMON_H
#define __COMMON_H

#define FILE_NAME_LEN 	50							//�ļ������ȣ������⵽�ļ�������50 ��������ļ� 
#define PATH_LEN		    50              //·������
#define FILE_LIST_PATH 			"0:/FILELIST.TXT"	//�ļ���¼�б��ļ���Ŀ¼
//#define _DF1S        0x80   //֧�ֳ��ļ�����,��û�ó��ļ���
#define G_CONST        9.80

extern int  string_subs_char(char* a);
extern int  WriteFileProcess(char* program_name);	

#endif

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "includes.h"


/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
OS_TCB	AppTaskStartTCB;		     //����������ƿ�
FATFS 							 fs;//�߼����̹�����.	 
FIL 							 file  /*__attribute__((at(0x680830D0)))*/;//�ļ��ṹ��
FRESULT          result;//�ļ���������ֵ
UINT         				 bw;//��/д�ļ�����ʱʵ�ʶ�/д���ֽ��������ڼ����Ƿ��/д�����㹻���ֽڡ�	
/*********************************************************************
*
*       Static  data
*
**********************************************************************
*/
static	CPU_STK	AppTaskStartStk[APP_TASK_START_STK_SIZE];	   //���������ջ
char buffer[10] = {0};
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	OS_ERR err;	
	SPI_FLASH_Init();
	SPI_FLASH_BulkErase();
	//��ʼ���ļ�ϵͳ
	result = f_mount(&fs,"0:",1);//�����ⲿFlashΪ�߼�����0
	
	if( result == FR_NO_FILESYSTEM )//����ô���û�б���ʽ��ΪFatFS�����ʽ����
	{
			result = f_mkfs("0:",1,4096); //��ʽ����ʽΪFDISK,����������4096Ϊÿ���صĴ�С
			result = f_mount(&fs,"0:",1);
//			result = f_open (&file, "0:/FILELIST.TXT", FA_READ|FA_WRITE|FA_CREATE_ALWAYS ); //�򿪴��������ļ�

	}
	result = f_open(&file, "0:/FILELIST.TXT",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(result == FR_OK)
	{
		result = f_write(&file, "012345678",10,&bw);
		f_close(&file);
	}
	result = f_open(&file,"0:/FILELIST.TXT",FA_READ|FA_OPEN_EXISTING);
	result = f_read(&file,buffer,10,&bw);
//	CPU_IntDis();//BSP_IntDisAll();                           /* Disable all interrupts.  
	/* ��ʼ��"uC/OS-III"�ں� */  
	OSInit(&err);	
	
	/*��������*/
	OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,              // ������ƿ�ָ��          
               (CPU_CHAR   *)"App Task Start",		          // ��������
               (OS_TASK_PTR )AppTaskStart, 	                  // �������ָ��
               (void       *)0,			                      // ���ݸ�����Ĳ���parg
               (OS_PRIO     )APP_TASK_START_PRIO,			  // �������ȼ�
               (CPU_STK    *)&AppTaskStartStk[0],	          // �����ջ����ַ
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE/10,	  // ��ջʣ�ྯ����
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,		  // ��ջ��С
               (OS_MSG_QTY  )5u,			                      // �ɽ��յ������Ϣ������
               (OS_TICK     )0u,			                      // ʱ��Ƭ��תʱ��
               (void       *)0,			                      // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	      // ����ѡ��
               (OS_ERR     *)&err);		                    // ����ֵ
	  
  /* ����������ϵͳ������Ȩ����uC/OS-II */
  OSStart(&err);                                       
}
/*********************************************END OF FILE**********************/

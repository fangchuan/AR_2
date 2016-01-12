#include "includes.h"


/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
OS_TCB	AppTaskStartTCB;		     //����������ƿ�
FATFS 							 fs;//�߼����̹�����.
FIL 							 file  __attribute__((at(0x68084d04)));//�ļ��ṹ��,������SRAM��531K��
FRESULT          result;//�ļ���������ֵ
UINT         				 bw;//��/д�ļ�����ʱʵ�ʶ�/д���ֽ��������ڼ����Ƿ��/д�����㹻���ֽڡ�
/*********************************************************************
*
*       Static  data
*
**********************************************************************
*/
static	CPU_STK	AppTaskStartStk[APP_TASK_START_STK_SIZE];	   //���������ջ
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    OS_ERR err;


//	FLASH_SPI_Init();
//	W25QXX_Erase_Chip();
    //��ʼ���ļ�ϵͳ
    result = f_mount(&fs,"0:",1);//�����ⲿFlashΪ�߼�����0

    if( result == FR_NO_FILESYSTEM )//����ô���û�б���ʽ��ΪFatFS�����ʽ����
    {
        result = f_mkfs("0:",0,4096); //��ʽ����ʽΪFDISK,����������4096Ϊÿ���صĴ�С
        result = f_mount(&fs,"0:",0);
        result = f_mount(&fs,"0:",1);
    }
//	result = 	f_open(&file,"0:filelist.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
//	result = f_open(&file,"0:filelist.txt",FA_READ|FA_WRITE|FA_OPEN_EXISTING);
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

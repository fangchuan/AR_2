/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
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
OS_TCB	AppTaskStartTCB;		     //定义任务控制块
FATFS 							 fs;//逻辑磁盘工作区.	 
FIL 							 file  /*__attribute__((at(0x680830D0)))*/;//文件结构体
FRESULT          result;//文件操作返回值
UINT         				 bw;//读/写文件操作时实际读/写的字节数，用于检验是否读/写够了足够的字节。	
/*********************************************************************
*
*       Static  data
*
**********************************************************************
*/
static	CPU_STK	AppTaskStartStk[APP_TASK_START_STK_SIZE];	   //定义任务堆栈
char buffer[10] = {0};
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	OS_ERR err;	
	SPI_FLASH_Init();
	SPI_FLASH_BulkErase();
	//初始化文件系统
	result = f_mount(&fs,"0:",1);//挂载外部Flash为逻辑磁盘0
	
	if( result == FR_NO_FILESYSTEM )//如果该磁盘没有被格式化为FatFS，则格式化它
	{
			result = f_mkfs("0:",1,4096); //格式化方式为FDISK,建立分区表，4096为每个簇的大小
			result = f_mount(&fs,"0:",1);
//			result = f_open (&file, "0:/FILELIST.TXT", FA_READ|FA_WRITE|FA_CREATE_ALWAYS ); //打开创建索引文件

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
	/* 初始化"uC/OS-III"内核 */  
	OSInit(&err);	
	
	/*创建任务*/
	OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,              // 任务控制块指针          
               (CPU_CHAR   *)"App Task Start",		          // 任务名称
               (OS_TASK_PTR )AppTaskStart, 	                  // 任务代码指针
               (void       *)0,			                      // 传递给任务的参数parg
               (OS_PRIO     )APP_TASK_START_PRIO,			  // 任务优先级
               (CPU_STK    *)&AppTaskStartStk[0],	          // 任务堆栈基地址
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE/10,	  // 堆栈剩余警戒线
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,		  // 堆栈大小
               (OS_MSG_QTY  )5u,			                      // 可接收的最大消息队列数
               (OS_TICK     )0u,			                      // 时间片轮转时间
               (void       *)0,			                      // 任务控制块扩展信息
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	      // 任务选项
               (OS_ERR     *)&err);		                    // 返回值
	  
  /* 启动多任务系统，控制权交给uC/OS-II */
  OSStart(&err);                                       
}
/*********************************************END OF FILE**********************/

#include "includes.h"
#include "GUI_Main.h"

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern OS_TCB	AppTaskStartTCB;
extern uint8_t Key_Value ;
//extern volatile uint8_t flag_run;//运行标志
extern enum _FLAG _flag;//指令标志
extern _Listptr Ins_List_Head;//程序链表的头指针

OS_SEM  RUN_SEM;		//定义一个信号量，用于点击“运行”按钮时同步运行任务
//OS_SEM  END_SEM;    //定义一个信号量，用于运行任务结束后发送给GUIUpdate任务
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static  OS_TCB   AppTaskGUIUpdateTCB;
static  CPU_STK  AppTaskGUIUpdateStk[APP_CFG_TASK_GUI_UPDATE_STK_SIZE];

static  OS_TCB   AppTaskCOMTCB;
static  CPU_STK  AppTaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];

static  OS_TCB   AppTaskUserIFTCB;
static  CPU_STK  AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE];

static  OS_TCB   AppTaskMainTaskTCB;//这里没把他定义为静态是为了让GUI面板能随时随地将运行任务挂起
static  CPU_STK  AppTaskMainTaskStk[APP_CFG_TASK_MAIN_TASK_STK_SIZE];

static volatile uint8_t flag_end;//程序运行结束标志，是正常结束，不是强制停止

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static  void  AppTaskCreate(void);
static void   AppTaskGUIUpdate(void *p_arg);
static void   AppTaskUserIF(void *p_arg);
static void   AppTaskCOM(void *p_arg);
static void   AppTaskMainTask(void *p_arg);

//软件定时器的回调函数
static void _cbOfTmr1(OS_TMR *p_tmr, void *p_arg)
{
  
	(void)p_arg;
	GUI_TOUCH_Exec();			//每10ms调用一次，触发调用触摸驱动
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

void  AppTaskStart(void *p_arg)
{
    OS_ERR      err;
	
	//定时器变量
    OS_TMR             MyTmr;
	
   (void)p_arg;
	                                                /* Initialize BSP functions                             */
    CPU_Init();
	/* 板级初始化 */	
    BSP_Init(); 
	
    //Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    
    APP_TRACE_INFO(("Creating Application Tasks...\n\r"));
    AppTaskCreate();                                            /* Create Application Tasks                             */
    
	//创建定时器
  OSTmrCreate ((OS_TMR              *)&MyTmr,
               (CPU_CHAR            *)"MyTimer",          
               (OS_TICK              )1,                //第一次延时设置为100，结合定时器的频率是100Hz，正好1s
               (OS_TICK              )1,                //重复延时的时候100个TmrTick，结合定时器的频率是100Hz，正好1s
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//模式设置为重复模式
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr1,          //回调函数
               (void                *)0,                  //参数设置为0
               (OS_ERR              *)err);
  
  //启动定时器
  OSTmrStart((OS_TMR *)&MyTmr,(OS_ERR *)err);
			  
	/*Delete task*/
	OSTaskDel(&AppTaskStartTCB,&err);	

}


/*
*********************************************************************************************************
*	函 数 名: AppTaskGUIUpdate
*	功能说明: 
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：3
*********************************************************************************************************
*/
static void AppTaskGUIUpdate(void *p_arg)
{
	OS_ERR      err;
	
	(void)p_arg;
	
//	GUIDEMO_Main();
	GUI_Main_Task();
	while(1)
	{
		GUI_Exec();
		if(flag_end) 
		{
			flag_end = 0;
			GUI_EndDialog(hRun,0);//运行结束则关闭“运行”界面
		}
		OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_PERIODIC | OS_OPT_TIME_HMSM_STRICT,
                      &err);								  	 	       											  
	}   
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCom
*	功能说明: 
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：3
*********************************************************************************************************
*/
static void AppTaskCOM(void *p_arg)
{	
	OS_ERR      err;
	_Listptr p = Ins_List_Head;
	(void)p_arg;
	 
	while(1)
	{
		p = Ins_List_Head;
		while(p -> next )
		{
				printf("flag:%d\n",(p -> next)->_flag );
			  p = p -> next ;
		}
		OSTimeDlyHMSM(0, 0, 5, 0,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
	}
	 						  	 	       											   
   
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskUserIF
*	功能说明: 界面切换任务
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：2
*********************************************************************************************************
*/

static void AppTaskUserIF(void *p_arg)
{
	OS_ERR      err;
	
	while (1) 
	{   
		if( Key_Value == 0)
		{
				
				WM_ShowWindow(hWin_Top);
				WM_HideWindow(hWin_1);
				WM_HideWindow(hWin_2);
				WM_HideWindow(hWin_3);
				WM_HideWindow(hWin_4);
		}
		if( Key_Value == 1)
		{
				WM_HideWindow(hWin_Top);
				WM_ShowWindow(hWin_1);
				WM_HideWindow(hWin_2);
				WM_HideWindow(hWin_3);
				WM_HideWindow(hWin_4);
		}
		if( Key_Value == 2)
		{
				WM_HideWindow(hWin_Top);
				WM_HideWindow(hWin_1);
				WM_ShowWindow(hWin_2);
				WM_HideWindow(hWin_3);
				WM_HideWindow(hWin_4);
		}
		if( Key_Value == 3)
		{
				WM_HideWindow(hWin_Top);
				WM_HideWindow(hWin_1);
				WM_HideWindow(hWin_2);
				WM_ShowWindow(hWin_3);
				WM_HideWindow(hWin_4);
		}
		if( Key_Value == 4)
		{
				WM_HideWindow(hWin_Top);
				WM_HideWindow(hWin_1);
				WM_HideWindow(hWin_2);
				WM_HideWindow(hWin_3);
				WM_ShowWindow(hWin_4);
		}
		OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);     
	}
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskMainTask
*	功能说明: 主要运行任务，运行图形化编程中编写的程序
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：3
*********************************************************************************************************
*/
static void AppTaskMainTask(void *p_arg)
{
			OS_ERR  err;
			
			while(1)
			{
					OSSemPend(&RUN_SEM,0 ,OS_OPT_PEND_BLOCKING, 0, &err);//请求"运行"信号量
					
					List_Parse(Ins_List_Head->next);//解析并运行任务,由于指令是从"开始"EDIT的下一个编辑的，所以传入的参数为头结点的下一个结点
				
					flag_end = 1;//运行结束，将标志位置1
					OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
			}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static  void  AppTaskCreate(void)
{
	OS_ERR      err;
	
	//创建一个信号量，用于同步主运行任务
	OSSemCreate(&RUN_SEM,"Run Semaphore",0, &err);
//	//创建一个信号量，用于主运行任务运行结束后通知GUIUpdate任务
//	OSSemCreate(&END_SEM, "End of Run Semaphore",0 ,&err);
	/***********************************/
	OSTaskCreate((OS_TCB       *)&AppTaskGUIUpdateTCB,             
                 (CPU_CHAR     *)"App Task GUI Update",
                 (OS_TASK_PTR   )AppTaskGUIUpdate, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_UPDATE_PRIO,
                 (CPU_STK      *)&AppTaskGUIUpdateStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_UPDATE_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_UPDATE_STK_SIZE,
                 (OS_MSG_QTY    )1,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	
	/***********************************/
	OSTaskCreate((OS_TCB       *)&AppTaskCOMTCB,            
                 (CPU_CHAR     *)"App Task COM",
                 (OS_TASK_PTR   )AppTaskCOM, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_PRIO,
                 (CPU_STK      *)&AppTaskCOMStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE,
                 (OS_MSG_QTY    )2,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	
	/***********************************/
	OSTaskCreate((OS_TCB       *)&AppTaskUserIFTCB,             
                 (CPU_CHAR     *)"App Task UserIF",
                 (OS_TASK_PTR   )AppTaskUserIF, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_USER_IF_PRIO,
                 (CPU_STK      *)&AppTaskUserIFStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);			

	OSTaskCreate((OS_TCB       *)&AppTaskMainTaskTCB,             
                 (CPU_CHAR     *)"App Task MainTask",
                 (OS_TASK_PTR   )AppTaskMainTask, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_MAIN_TASK_PRIO,
                 (CPU_STK      *)&AppTaskMainTaskStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_MAIN_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_MAIN_TASK_STK_SIZE,
                 (OS_MSG_QTY    )10,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);		
								 
}


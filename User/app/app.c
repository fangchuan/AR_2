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
//extern volatile uint8_t flag_run;//���б�־
extern enum _FLAG _flag;//ָ���־
extern _Listptr Ins_List_Head;//���������ͷָ��

OS_SEM  RUN_SEM;		//����һ���ź��������ڵ�������С���ťʱͬ����������
//OS_SEM  END_SEM;    //����һ���ź�����������������������͸�GUIUpdate����
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

static  OS_TCB   AppTaskMainTaskTCB;//����û��������Ϊ��̬��Ϊ����GUI�������ʱ��ؽ������������
static  CPU_STK  AppTaskMainTaskStk[APP_CFG_TASK_MAIN_TASK_STK_SIZE];

static volatile uint8_t flag_end;//�������н�����־������������������ǿ��ֹͣ

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

//�����ʱ���Ļص�����
static void _cbOfTmr1(OS_TMR *p_tmr, void *p_arg)
{
  
	(void)p_arg;
	GUI_TOUCH_Exec();			//ÿ10ms����һ�Σ��������ô�������
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
	
	//��ʱ������
    OS_TMR             MyTmr;
	
   (void)p_arg;
	                                                /* Initialize BSP functions                             */
    CPU_Init();
	/* �弶��ʼ�� */	
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
    
	//������ʱ��
  OSTmrCreate ((OS_TMR              *)&MyTmr,
               (CPU_CHAR            *)"MyTimer",          
               (OS_TICK              )1,                //��һ����ʱ����Ϊ100����϶�ʱ����Ƶ����100Hz������1s
               (OS_TICK              )1,                //�ظ���ʱ��ʱ��100��TmrTick����϶�ʱ����Ƶ����100Hz������1s
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//ģʽ����Ϊ�ظ�ģʽ
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr1,          //�ص�����
               (void                *)0,                  //��������Ϊ0
               (OS_ERR              *)err);
  
  //������ʱ��
  OSTmrStart((OS_TMR *)&MyTmr,(OS_ERR *)err);
			  
	/*Delete task*/
	OSTaskDel(&AppTaskStartTCB,&err);	

}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUIUpdate
*	����˵��: 
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����3
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
			GUI_EndDialog(hRun,0);//���н�����رա����С�����
		}
		OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_PERIODIC | OS_OPT_TIME_HMSM_STRICT,
                      &err);								  	 	       											  
	}   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCom
*	����˵��: 
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����3
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
*	�� �� ��: AppTaskUserIF
*	����˵��: �����л�����
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����2
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
*	�� �� ��: AppTaskMainTask
*	����˵��: ��Ҫ������������ͼ�λ�����б�д�ĳ���
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����3
*********************************************************************************************************
*/
static void AppTaskMainTask(void *p_arg)
{
			OS_ERR  err;
			
			while(1)
			{
					OSSemPend(&RUN_SEM,0 ,OS_OPT_PEND_BLOCKING, 0, &err);//����"����"�ź���
					
					List_Parse(Ins_List_Head->next);//��������������,����ָ���Ǵ�"��ʼ"EDIT����һ���༭�ģ����Դ���Ĳ���Ϊͷ������һ�����
				
					flag_end = 1;//���н���������־λ��1
					OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
			}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppTaskCreate(void)
{
	OS_ERR      err;
	
	//����һ���ź���������ͬ������������
	OSSemCreate(&RUN_SEM,"Run Semaphore",0, &err);
//	//����һ���ź����������������������н�����֪ͨGUIUpdate����
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


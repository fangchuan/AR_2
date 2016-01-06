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
extern _Ultrasnio ult;
extern enum _FLAG _flag;//指令标志
//extern _SensorFlag  sensorflag;//传感器种类
extern _Listptr Ins_List_Head;//程序链表的头指针
extern _DS    digital_sensor1;
extern _DS    digital_sensor2;
extern _DS    digital_sensor3;
extern _DS    digital_sensor4;
extern _AS     analog_sensor1;
extern _AS     analog_sensor2;
extern _AS     analog_sensor3;
extern _AS     analog_sensor4;
extern _Ultrasnio        ult;
extern _Euler          euler;
extern u8    flag_nrf_link,flag_change_nrf_addr;

OS_SEM  RUN_SEM;		//定义一个信号量，用于点击“运行”按钮时同步运行任务
//OS_SEM  END_SEM;    //定义一个信号量，用于运行任务结束后发送给GUIUpdate任务
//OS_FLAG_GRP	SensorFlags;		//定义一个事件标志组，用于决定传输哪个传感器数据
_nrf_pkt    nrf_rx;		//接收数据缓存
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static  OS_TCB   AppTaskGUIUpdateTCB;
static  CPU_STK  AppTaskGUIUpdateStk[APP_CFG_TASK_GUI_UPDATE_STK_SIZE];

static  OS_TCB   AppTaskCOMRxTCB;
static  CPU_STK  AppTaskCOMRxStk[APP_CFG_TASK_COM_RX_STK_SIZE];

static  OS_TCB   AppTaskUserIFTCB;
static  CPU_STK  AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE];

static  OS_TCB   AppTaskMainTaskTCB;//这里没把他定义为静态是为了让GUI面板能随时随地将运行任务挂起
static  CPU_STK  AppTaskMainTaskStk[APP_CFG_TASK_MAIN_TASK_STK_SIZE];

static  OS_TCB   AppTaskCOMTxTCB;
static  CPU_STK  AppTaskCOMTxStk[APP_CFG_TASK_COM_TX_STK_SIZE];

static  OS_TCB   AppTaskNRFReceiverTCB;
static  CPU_STK  AppTaskNRFReceiverStk[APP_CFG_TASK_NRF_STK_SIZE];

static  OS_TCB   AppTaskMPU6050TCB;
static  CPU_STK  AppTaskMPU6050Stk[APP_CFG_TASK_MPU6050_STK_SIZE];

static volatile uint8_t flag_end;//程序运行结束标志，是正常结束，不是强制停止
static uint8_t  transferdata[MAX_LEN];
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static void   AppTaskCreate(void);
static void   AppTaskGUIUpdate(void *p_arg);
static void   AppTaskUserIF(void *p_arg);
static void   AppTaskCOMRx(void *p_arg);
static void   AppTaskMainTask(void *p_arg);
static void   AppTaskCOMTx(void *p_arg);
static void   AppTaskNRFReceiver(void *p_arg);

//软件定时器的回调函数
static void _cbOfTmr1(OS_TMR *p_tmr, void *p_arg)
{
  
	(void)p_arg;
	GUI_TOUCH_Exec();			//每10ms调用一次，触发调用触摸驱动
	

}


static void _cbOfTmr2(OS_TMR *p_tmr, void *p_arg)
{
	  (void)p_arg;
		Ultrasnio_update();   //每100ms触发一次超声波更新
	  if(WM_IsWindow(hRun)) //如果“运行”窗口还有效，则使之无效化，来在做一些重绘工作
		{
				WM_Invalidate(hRun);
		}
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
    OS_TMR             Tmr_10ms, Tmr_100ms;
	
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
    
	//创建定时器  OS_CFG_TMR_TASK_RATE_HZ = 100HZ
  OSTmrCreate ((OS_TMR              *)&Tmr_10ms,
               (CPU_CHAR            *)"MyTimer 10ms",          
               (OS_TICK              )1,                  //第一次延时设置为10ms，
               (OS_TICK              )1,                  //定时器周期10ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//模式设置为重复模式
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr1,          //回调函数
               (void                *)0,                  //参数设置为0
               (OS_ERR              *)err);
		//创建定时器
  OSTmrCreate ((OS_TMR              *)&Tmr_100ms,
               (CPU_CHAR            *)"MyTimer 100ms",          
               (OS_TICK              )10,                 //第一次延时设置为100ms
               (OS_TICK              )10,                //定时周期10*10ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//模式设置为重复模式
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr2,          //回调函数
               (void                *)0,                  //参数设置为0
               (OS_ERR              *)err);
  
  //启动定时器
  OSTmrStart((OS_TMR *)&Tmr_10ms,(OS_ERR *)err);
	OSTmrStart((OS_TMR *)&Tmr_100ms,(OS_ERR *)err);
			  
	/*Delete task*/
	OSTaskDel(&AppTaskStartTCB,&err);	

}


/*
*********************************************************************************************************
*	函 数 名: AppTaskGUIUpdate
*	功能说明: 
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：2
*********************************************************************************************************
*/
static void AppTaskGUIUpdate(void *p_arg)
{
	OS_ERR      err;

	(void)p_arg;
	
	GUI_Main_Task();
	while(1)
	{
		GUI_Exec();

		if(flag_end) 
		{
			flag_end = 0;
			if(WM_IsWindow(hRun))
			{
				GUI_EndDialog(hRun,0);//运行结束则关闭“运行”界面
			}
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
	优 先 级：4
*********************************************************************************************************
*/
static void AppTaskCOMRx(void *p_arg)
{	
	OS_ERR      err;
//	_Listptr p = Ins_List_Head;
	(void)p_arg;
	 
	while(1)
	{
//		p = Ins_List_Head;
//		while(p -> next )
//		{
//				printf("flag:%d\n",(p -> next)->_flag );
//			  p = p -> next ;
//		}
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
	优 先 级：6
*********************************************************************************************************
*/

static void AppTaskUserIF(void *p_arg)
{
	OS_ERR      err;
	(void)    p_arg;
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
*	函 数 名: AppTaskTransfer
*	功能说明: 传感器数据回传任务
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：4
*********************************************************************************************************
*/
static void AppTaskCOMTx(void *p_arg)
{
			 OS_ERR   err;
	     (void) p_arg;
	
	     transferdata[0] = FRAME_STR;
			 transferdata[1] = VERSION;
	     while(1)
			 {
//				 		//等待事件标志组
//					OSFlagPend((OS_FLAG_GRP*)&SensorFlags,
//								 (OS_FLAGS	  )sensorflag,
//								 (OS_TICK     )0,
//								 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
//								 (CPU_TS*     )0,
//								 (OS_ERR*	    )&err);
				  if(digital_sensor1.sta )
					{
             transferdata[2] = DS_1_ID;                    //type
						 transferdata[3] = sizeof(digital_sensor1.val);//length
						 transferdata[4] = digital_sensor1.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(digital_sensor2.sta )
					{
						 transferdata[2] = DS_2_ID;                    //type
						 transferdata[3] = sizeof(digital_sensor2.val);//length
						 transferdata[4] = digital_sensor2.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(digital_sensor3.sta )
					{
						 transferdata[2] = DS_3_ID;                    //type
						 transferdata[3] = sizeof(digital_sensor3.val);//length
						 transferdata[4] = digital_sensor3.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(digital_sensor4.sta )
					{
						 transferdata[2] = DS_4_ID;                    //type
						 transferdata[3] = sizeof(digital_sensor4.val);//length
						 transferdata[4] = digital_sensor4.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(analog_sensor1.sta )
					{
						 transferdata[2] = AS_1_ID;                   //type
						 transferdata[3] = sizeof(analog_sensor1.val);//length
						 transferdata[4] = analog_sensor1.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(analog_sensor2.sta )
					{
						 transferdata[2] = AS_2_ID;                   //type
						 transferdata[3] = sizeof(analog_sensor2.val);//length
						 transferdata[4] = analog_sensor2.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(analog_sensor3.sta )
					{
						 transferdata[2] = AS_3_ID;                   //type
						 transferdata[3] = sizeof(analog_sensor3.val);//length
						 transferdata[4] = analog_sensor3.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(analog_sensor4.sta )
					{
						 transferdata[2] = AS_4_ID;                   //type
						 transferdata[3] = sizeof(analog_sensor4.val);//length
						 transferdata[4] = analog_sensor4.val ;       //value
						 transferdata[5] = FRAME_END;
						 printf("%s",transferdata);
					}
					if(ult.cur_distance )
					{
						 transferdata[2] = ULTRASNIO_ID;                   //type
						 transferdata[3] = sizeof(ult.cur_distance );//length
//						 transferdata[4] = analog_sensor1.val >> 24;       //value  因STM32小端模式，所以
//						 transferdata[5] = analog_sensor1.val >> 16;
//						 transferdata[6] = analog_sensor1.val >> 8;
//						 transferdata[7] = analog_sensor1.val;
						 memcpy(transferdata + 4, &(ult.cur_distance),transferdata[3]);
						 transferdata[8] = FRAME_END;
						 printf("%s",transferdata);
					}
					
					transferdata[2] = ANGLE_X_ID;
					transferdata[3] = sizeof(euler.angle_x);
					memcpy(transferdata + 4, &(euler.angle_x),transferdata[3]);
					transferdata[8] = FRAME_END;
					printf("%s",transferdata);
					
					transferdata[2] = ANGLE_Y_ID;
					transferdata[3] = sizeof(euler.angle_y);
					memcpy(transferdata + 4, &(euler.angle_y),transferdata[3]);
					transferdata[8] = FRAME_END;
					printf("%s",transferdata);
					
					transferdata[2] = ACCEL_X_ID;
					transferdata[3] = sizeof(euler.accel_x);
					memcpy(transferdata + 4, &(euler.accel_y),transferdata[3]);
					transferdata[8] = FRAME_END;
					printf("%s",transferdata);
					
					transferdata[2] = ACCEL_Y_ID;
					transferdata[3] = sizeof(euler.accel_y);
					memcpy(transferdata + 4, &(euler.accel_y),transferdata[3]);
					transferdata[8] = FRAME_END;
					printf("%s",transferdata);
					
				  OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
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
*	函 数 名: AppTaskNRFReceiver
*	功能说明: 主要运行任务，运行图形化编程中编写的程序
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：5
*********************************************************************************************************
*/
static void AppTaskNRFReceiver(void *p_arg)
{ 
	     OS_ERR  err;
	     u8   status;
			 (void)p_arg;
	
			 NRF_RX_Mode();//设置初始地址
	     while(1)
			 {
				  if(flag_nrf_link)
					{
						if(flag_change_nrf_addr)
						{
								flag_change_nrf_addr = 0;
								NRF_RX_Mode();
						}
						/*等待接收数据*/
						status = NRF_Rx_Dat((u8 *)&nrf_rx);
						
						/*判断接收状态*/
						if(status == RX_DR)

						{
							if(nrf_rx.car_speed == NRF_ROCKER_FORWARD ||
								 nrf_rx.key_value  == NRF_KEY_FORWARD ||
								 nrf_rx.Y_angle < -NRF_EULER_THRE) 
									Car_Forward();
							if(nrf_rx.car_speed == NRF_ROCKER_BACKWARD ||
    						 nrf_rx.key_value  == NRF_KEY_BACKWARD ||
 							   nrf_rx.Y_angle > NRF_EULER_THRE) 
									Car_Backward(); 
							if(nrf_rx.car_angle == NRF_ROCKER_LEFT || 
								 nrf_rx.key_value  == NRF_KEY_LEFT || 
							   nrf_rx.X_angle > NRF_EULER_THRE) 
									Car_Left();
							if(nrf_rx.car_angle == NRF_ROCKER_RIGHT || 
								 nrf_rx.key_value == NRF_KEY_RIGHT || 
							   nrf_rx.X_angle < -NRF_EULER_THRE) 
									Car_Right();
							if(nrf_rx.car_speed == NRF_STOP && nrf_rx.car_angle == NRF_STOP && nrf_rx.key_value == NRF_STOP 
								&& fabs(nrf_rx.X_angle) < NRF_EULER_SAFE && fabs(nrf_rx.Y_angle) < NRF_EULER_SAFE ) 
									Car_Stop();
							
						}
					}
				 OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
			 }
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskMPU6050
*	功能说明: MPU6050数据采集及结算任务
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：5
*********************************************************************************************************
*/
static void AppTaskMPU6050(void *p_arg)
{
	   OS_ERR  err;
	   (void)p_arg;
	
		 while(1)
		 {
			 //Get_Attitude();
			 OSTimeDlyHMSM(0, 0, 0, 20, OS_OPT_TIME_HMSM_STRICT, &err);
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
//	//创建一个事件标志组
//	OSFlagCreate((OS_FLAG_GRP*)&SensorFlags,		//指向事件标志组
//                 (CPU_CHAR*	  )"Sensor Flags",	//名字
//                 (OS_FLAGS	  )0,	           //事件标志组初始值
//                 (OS_ERR*  	  )&err);			//错误码
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
	OSTaskCreate((OS_TCB       *)&AppTaskCOMRxTCB,            
                 (CPU_CHAR     *)"App Task COM",
                 (OS_TASK_PTR   )AppTaskCOMRx, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_RX_PRIO,
                 (CPU_STK      *)&AppTaskCOMRxStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_RX_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_RX_STK_SIZE,
                 (OS_MSG_QTY    )2,
                 (OS_TICK       )2,
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
	/***********************************/
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
		/***********************************/						 
		OSTaskCreate((OS_TCB       *)&AppTaskCOMTxTCB,             
                 (CPU_CHAR     *)"App Task COM Transfer",
                 (OS_TASK_PTR   )AppTaskCOMTx, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_TX_PRIO,
                 (CPU_STK      *)&AppTaskCOMTxStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_TX_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_TX_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )2,     //因为与串口任务相同优先级，所以设置他的轮转时间片为2ms
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);		
		/************************************/
		OSTaskCreate((OS_TCB       *)&AppTaskNRFReceiverTCB,             
                 (CPU_CHAR     *)"App Task NRFReceiver",
                 (OS_TASK_PTR   )AppTaskNRFReceiver, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_NRF_PRIO,
                 (CPU_STK      *)&AppTaskNRFReceiverStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_NRF_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_NRF_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,    
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);			
		/************************************/
		OSTaskCreate((OS_TCB       *)&AppTaskMPU6050TCB,
								 (CPU_CHAR     *)"App Task MPU6050",
								 (OS_TASK_PTR   )AppTaskMPU6050,
								 (void         *)0,
								 (OS_PRIO       )APP_CFG_TASK_MPU6050_PRIO,
								 (CPU_STK      *)&AppTaskMPU6050Stk[0],
								 (CPU_STK_SIZE  )APP_CFG_TASK_MPU6050_STK_SIZE / 10,
								 (CPU_STK_SIZE  )APP_CFG_TASK_MPU6050_STK_SIZE,
								 (OS_MSG_QTY    )0,
								 (OS_TICK       )0,
								 (void         *)0,
								 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
								 (OS_ERR       *)&err);
								 
}


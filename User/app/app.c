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
extern enum _FLAG _flag;//ָ���־
//extern _SensorFlag  sensorflag;//����������
extern _Listptr Ins_List_Head;//���������ͷָ��
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

OS_SEM  RUN_SEM;		//����һ���ź��������ڵ�������С���ťʱͬ����������
//OS_SEM  END_SEM;    //����һ���ź�����������������������͸�GUIUpdate����
//OS_FLAG_GRP	SensorFlags;		//����һ���¼���־�飬���ھ��������ĸ�����������
_nrf_pkt    nrf_rx;		//�������ݻ���
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

static  OS_TCB   AppTaskMainTaskTCB;//����û��������Ϊ��̬��Ϊ����GUI�������ʱ��ؽ������������
static  CPU_STK  AppTaskMainTaskStk[APP_CFG_TASK_MAIN_TASK_STK_SIZE];

static  OS_TCB   AppTaskCOMTxTCB;
static  CPU_STK  AppTaskCOMTxStk[APP_CFG_TASK_COM_TX_STK_SIZE];

static  OS_TCB   AppTaskNRFReceiverTCB;
static  CPU_STK  AppTaskNRFReceiverStk[APP_CFG_TASK_NRF_STK_SIZE];

static  OS_TCB   AppTaskMPU6050TCB;
static  CPU_STK  AppTaskMPU6050Stk[APP_CFG_TASK_MPU6050_STK_SIZE];

static volatile uint8_t flag_end;//�������н�����־������������������ǿ��ֹͣ
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

//�����ʱ���Ļص�����
static void _cbOfTmr1(OS_TMR *p_tmr, void *p_arg)
{
  
	(void)p_arg;
	GUI_TOUCH_Exec();			//ÿ10ms����һ�Σ��������ô�������
	

}


static void _cbOfTmr2(OS_TMR *p_tmr, void *p_arg)
{
	  (void)p_arg;
		Ultrasnio_update();   //ÿ100ms����һ�γ���������
	  if(WM_IsWindow(hRun)) //��������С����ڻ���Ч����ʹ֮��Ч����������һЩ�ػ湤��
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
	
	//��ʱ������
    OS_TMR             Tmr_10ms, Tmr_100ms;
	
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
    
	//������ʱ��  OS_CFG_TMR_TASK_RATE_HZ = 100HZ
  OSTmrCreate ((OS_TMR              *)&Tmr_10ms,
               (CPU_CHAR            *)"MyTimer 10ms",          
               (OS_TICK              )1,                  //��һ����ʱ����Ϊ10ms��
               (OS_TICK              )1,                  //��ʱ������10ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//ģʽ����Ϊ�ظ�ģʽ
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr1,          //�ص�����
               (void                *)0,                  //��������Ϊ0
               (OS_ERR              *)err);
		//������ʱ��
  OSTmrCreate ((OS_TMR              *)&Tmr_100ms,
               (CPU_CHAR            *)"MyTimer 100ms",          
               (OS_TICK              )10,                 //��һ����ʱ����Ϊ100ms
               (OS_TICK              )10,                //��ʱ����10*10ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//ģʽ����Ϊ�ظ�ģʽ
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr2,          //�ص�����
               (void                *)0,                  //��������Ϊ0
               (OS_ERR              *)err);
  
  //������ʱ��
  OSTmrStart((OS_TMR *)&Tmr_10ms,(OS_ERR *)err);
	OSTmrStart((OS_TMR *)&Tmr_100ms,(OS_ERR *)err);
			  
	/*Delete task*/
	OSTaskDel(&AppTaskStartTCB,&err);	

}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUIUpdate
*	����˵��: 
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����2
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
				GUI_EndDialog(hRun,0);//���н�����رա����С�����
			}
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
	�� �� ����4
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
*	�� �� ��: AppTaskUserIF
*	����˵��: �����л�����
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����6
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
*	�� �� ��: AppTaskTransfer
*	����˵��: ���������ݻش�����
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����4
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
//				 		//�ȴ��¼���־��
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
//						 transferdata[4] = analog_sensor1.val >> 24;       //value  ��STM32С��ģʽ������
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
*	�� �� ��: AppTaskNRFReceiver
*	����˵��: ��Ҫ������������ͼ�λ�����б�д�ĳ���
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����5
*********************************************************************************************************
*/
static void AppTaskNRFReceiver(void *p_arg)
{ 
	     OS_ERR  err;
	     u8   status;
			 (void)p_arg;
	
			 NRF_RX_Mode();//���ó�ʼ��ַ
	     while(1)
			 {
				  if(flag_nrf_link)
					{
						if(flag_change_nrf_addr)
						{
								flag_change_nrf_addr = 0;
								NRF_RX_Mode();
						}
						/*�ȴ���������*/
						status = NRF_Rx_Dat((u8 *)&nrf_rx);
						
						/*�жϽ���״̬*/
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
*	�� �� ��: AppTaskMPU6050
*	����˵��: MPU6050���ݲɼ�����������
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����5
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
//	//����һ���¼���־��
//	OSFlagCreate((OS_FLAG_GRP*)&SensorFlags,		//ָ���¼���־��
//                 (CPU_CHAR*	  )"Sensor Flags",	//����
//                 (OS_FLAGS	  )0,	           //�¼���־���ʼֵ
//                 (OS_ERR*  	  )&err);			//������
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
                 (OS_TICK       )2,     //��Ϊ�봮��������ͬ���ȼ�����������������תʱ��ƬΪ2ms
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


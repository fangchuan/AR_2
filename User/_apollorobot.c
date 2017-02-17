/*
*********************************************************************************************************
*
*	模块名称 : 编程指令解析模块
*	文件名称 : apollorobot.c
*	版    本 : V1.0
*	说    明 : 编程指令的解析、执行
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.com
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
_Listptr Ins_List_Head;//程序主链表的头指针
_Listptr Sub_List_Head;//子程序链表头指针
//_SensorFlag  sensorflag;//传感器种类
extern _Port port_1;
extern _Port port_2;
extern _Port port_3;
extern _Port port_4;
extern  FIL        file;
extern  FRESULT  result;
extern  UINT         bw;	
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
_Motor          motor;
_Servo          servo;
_Led              led;
_Car              car;
_Port            port;
_Variable         var;
_Music          music;
_Paint          paint;
_Gif              gif;
_Ultrasnio        ult;
_Euler          euler;
_Display      display;

int32_t  delay_time;
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static _Listptr  or_branch (_Listptr  p);
static _Listptr  if_branch (_Listptr  p);
static _Listptr  while_branch(_Listptr  p);
static _Listptr  for_branch(_Listptr p, uint8_t cycles);
static _Listptr  Find_Node(_Listptr head, enum _FLAG flag);
static _Error    OpenSubPro (uint32_t pn, _Listptr head);
static _Listptr  proc_branch(_Listptr p);
static void InitCar (_Car * car);
static void InitLed(_Led * led);
static void InitVar(_Variable *var);
static void InitPort(_Port *port);
static void InitServo(_Servo *servo);
static void InitMotor(_Motor *motor);
static void InitMusic(_Music *music);
static void InitUltrasnio(_Ultrasnio *sensor);

/*
*********************************************************************************************************
*	函 数 名: InitMPUSensor
*	功能说明: 初始化姿态传感器变量
*	形    参：_Euler指针
*	返 回 值: 无
*********************************************************************************************************
*/
void InitMPUSensor(_Euler *sensor)
{
	    sensor->angle_x = 0;
	    sensor->angle_y = 0;
	    sensor->accel_x = 0;
			sensor->accel_y = 0;
	    sensor->accel_z = 0;
	    sensor->gyro_x  = 0;
	    sensor->gyro_y  = 0;
	    sensor->gyro_z  = 0;
}
/*
*********************************************************************************************************
*	函 数 名: InitUltrasnio
*	功能说明: 初始化超声波传感器变量
*	形    参：_Ultrasnio指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitUltrasnio(_Ultrasnio *sensor)
{
//	    sensor->cur_distance = 0;
	    sensor->tar_distance = 0;
}

/*
*********************************************************************************************************
*	函 数 名: InitMotor
*	功能说明: 初始化电机变量
*	形    参：_Motor指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitMotor(_Motor *motor)
{
	    motor->id = 0;
	    motor->direction = 0;
	    motor->speed = 0;
}
/*
*********************************************************************************************************
*	函 数 名: InitServo
*	功能说明: 初始化舵机变量
*	形    参：_Servo指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitServo(_Servo *servo)
{
      servo->id = 0;
	    servo->degree = 0;

}
/*
*********************************************************************************************************
*	函 数 名: InitPort
*	功能说明: 初始化端口变量
*	形    参：_Port指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitPort(_Port *port)
{
	    port->id = 0;
	    port->dir = 0;
	    port->species = 0;
	    port->status = 0;
	    port->cur_val = 0;
	    port->tar_val = 0;
}	
/*
*********************************************************************************************************
*	函 数 名: InitLed
*	功能说明: 初始化led变量
*	形    参：_Led指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitLed(_Led * led)
{
	    led->id = 0;
	    led->status = 0;
}
/*
*********************************************************************************************************
*	函 数 名: InitVar
*	功能说明: 初始化Variable变量
*	形    参：_Variable指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitVar(_Variable *var)
{
	     var->id = 0;
	     var->set_val = 0;
	     var->tar_val = 0;
}
/*
*********************************************************************************************************
*	函 数 名: InitCar
*	功能说明: 初始化小车变量
*	形    参：_Car指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitCar (_Car * car)
{
	     car->direction = 0;
	     car->speed_step = 0;
}
/*
*********************************************************************************************************
*	函 数 名: InitMusic
*	功能说明: 初始化Music变量
*	形    参：_Music指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitMusic(_Music *music)
{
	     music->time = 0;
	     music->tones = 0;
}
/*
*********************************************************************************************************
*	函 数 名: Detect_Port
*	功能说明: 侦测端口Port的状态：有无信号
*          Tips:默认是用来判断数字传感器的
*	形    参：_Port指针
*	返 回 值: 无
*********************************************************************************************************
*/

_Error Detect_Port(_Port *port)
{
	    int  adc_val;
	
	    if(port->dir == PORT_OUT)   //端口必须为输入状态
				return ERROR_DIR;
			if(port->id < 1 || port->id > 4)
					return  ERROR_ID;
			else
			{
				DS_Config(port);//将此引脚配置为上拉输入
				switch(port->id )
					{
						case PORT_1:
									if( !Detect_DS1 )
									{
										  port->species = DS;
										  port->status = PORT_ON;
										  port->cur_val = SIGNAL;
									}
									else
									{ 
										  port->cur_val = NOSIGNAL;
										  port->status  = PORT_OFF;
										  
									}

									if((adc_val = Get_adc(ANOLOG_Sensor_1)) > 16)  //ADC值大于16才算作有模拟传感器插入
									{
										  port->species = AS;
										  port->status = PORT_ON;
										  port->cur_val = adc_val;
									}
							break;
						case PORT_2:
									if( !Detect_DS2 )
									{
										  port->species = DS;
										  port->status = PORT_ON;
										  port->cur_val = SIGNAL;
									}
									else
									{
										  port->cur_val = NOSIGNAL;
										  port->status  = PORT_OFF;
									}

									if((adc_val = Get_adc(ANOLOG_Sensor_2)) > 16)  //ADC值大于16才算作有模拟传感器插入
									{
										  port->species = AS;
										  port->status = PORT_ON;
											port->cur_val = adc_val;
									}
              break;
						case PORT_3:
									if( !Detect_DS3 )
									{
										  port->species = DS;
										  port->status = PORT_ON;
										  port->cur_val = SIGNAL;
									}
									else
									{
										   port->cur_val = NOSIGNAL;
										   port->status  = PORT_OFF;
									}
									if((adc_val = Get_adc(ANOLOG_Sensor_3)) > 16)  //ADC值大于16才算作有模拟传感器插入
									{
										  port->species = AS;
										  port->status = PORT_ON;
										  port->cur_val = adc_val;
									}
              break;
						case PORT_4:
									if( !Detect_DS4 )
									{
										  port->species = DS;
										  port->status = PORT_ON;
										  port->cur_val = SIGNAL;
									}
									else
									{
										  port->cur_val = NOSIGNAL;
										  port->status  = PORT_OFF;
									}
									if((adc_val = Get_adc(ANOLOG_Sensor_4)) > 16)   //ADC值大于16才算作有模拟传感器插入
									{
										  port->species = AS;
										  port->status = PORT_ON;
										  port->cur_val = adc_val;
									}
             break;
					}
				return NO_ERROR;
			}
}
/*
*********************************************************************************************************
*	函 数 名: Get_Port
*	功能说明: 获取指定port的值,默认指的是模拟传感器的值
*	形    参：_Port指针
*	返 回 值: 无
*********************************************************************************************************
*/
 _Error Get_Port  (_Port * port)
{
			if(port->id < 1 || port->id > 4)
					return ERROR_ID;
			else{
				    port->species = AS;
						switch(port->id)
						{
							case PORT_1:
										port->cur_val  = (int)Get_adc(ANOLOG_Sensor_1);
								break;
							case PORT_2:
										port->cur_val  = (int)Get_adc(ANOLOG_Sensor_2);
								break;
							case PORT_3:
										port->cur_val  = (int)Get_adc(ANOLOG_Sensor_3);
								break;
							case PORT_4:
										port->cur_val  = (int)Get_adc(ANOLOG_Sensor_4);
								break;
						}
						
				return NO_ERROR;
			}
}
/*
*********************************************************************************************************
*	函 数 名: OpenSubPro
*	功能说明: 依据程序名调用子程序
*           要求Add_Node有可重入性
*	形    参：pn:程序名                      head:子链表的头结点指针
*	返 回 值: 无
*********************************************************************************************************
*/
static _Error OpenSubPro (uint32_t pn, _Listptr head)
{
	    static   int last_pname = -1;
      if(last_pname != pn)
			{	//前后两次进入不同子程序			
				char         openfile[50]={0};
				u8           ret;
				 _Listptr   p = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
		
					sprintf(openfile, "%s/%d", "0:", pn);
					f_open(&file, openfile, FA_READ | FA_OPEN_EXISTING);
					if(result != FR_OK)
					{
						_MessageBox("程序不存在","错误", &ret);
						return ERROR_NAME;
					}
					if(!p)
						return ERROR_MALLOC;
					else{
						   
								u16      NumBytesPerNode = sizeof(_Instructor);
						    //子程序链表清空，重新读入
						    Clear_List(Sub_List_Head);
								do{
										result = f_read(&file, p, NumBytesPerNode, &bw);
										if(result != FR_OK)
											return ERROR_FOPERATION;
										Add_Node(Sub_List_Head , p->index , p->_flag ,p->EditContent );//将子程序添加进子程序链表
								}while(p->next); //链表尾结点处的next指针为空，表示最后一个结点，不用再往后读了。
								
								myfree(SRAMIN, p);
								
								last_pname = pn;
								
								head->next = Sub_List_Head->next ;//PC指针跳至子程序链表
								
								return NO_ERROR;
							}
					
				}
			else{
			      head->next = Sub_List_Head->next ;//PC指针跳至子程序链表
				    return NO_ERROR;
			}
}
/*
*********************************************************************************************************
*	函 数 名: proc_branch
*	功能说明: 调用子程序代码块
*	形    参：p:主链表上的每个节点
*	返 回 值: 返回跳转结点的指针，即FLAG_PROC
*********************************************************************************************************
*/
static _Listptr proc_branch(_Listptr p)
{
	     _Listptr  ret = p;
	     p = p -> next;
	     if(!p){
				 return ret;
			 }else{
				     _Listptr jumpq;
	           OS_ERR     err;
				     u8     procerr = 0;
						 while(p)
						 {
								switch ( p->_flag )
								{
									case FLAG_MOTOR_C:   //电机_正转,速度_  
													motor.id =( p->EditContent )[6] - 0x30;
													motor.direction = FORWARD;
													motor.speed = atoi(p->EditContent + 20);
													MOTOR_Config(&motor);
										break;
									case FLAG_MOTOR_CC: //电机_反转,速度_
													motor.id = ( p->EditContent )[6] - 0x30;
													motor.direction = BACKWARD;
													motor.speed = atoi(p->EditContent + 20);
													MOTOR_Config(&motor);
										break;
									case FLAG_SERVO:  //舵机_转_
												servo.id = ( p->EditContent )[6] - 0x30;
												servo.degree = atoi(p->EditContent + 10);
												SERVO_Config(&servo);
										break;
									case FLAG_LED_ON:  //LED_
												led.id = ( p->EditContent )[3] - 0x30;
												led.status = ON;
												LED_Config(&led );
										break;
									case FLAG_LED_OFF:  //LED_
												led.id = ( p->EditContent )[3] - 0x30;
												led.status = OFF;
												LED_Config(&led );
										break;
									case FLAG_CAR_LEFT:
												car.direction = LEFT;
												car.speed_step = 0;
												Car_Left();
										break;
									case FLAG_CAR_RIGHT:
												car.direction = RIGHT;
												car.speed_step = 0;
												Car_Right();
										break;
									case FLAG_CAR_FORWARD:
												car.direction = FORWARD;
												car.speed_step = 0;
												Car_Forward();
										break;
									case FLAG_CAR_BACKWARD:
												car.direction = BACKWARD;
												car.speed_step = 0;
												Car_Backward();
										break;
									case FLAG_CAR_STOP:
												car.direction = STOP;
												car.speed_step = 0;
												Car_Stop();
										break;
									case FLAG_CAR_ACCEL:
											 car.speed_step = SPEED_STEP;
											 CAR_Config(&car);
										break;
									case FLAG_CAR_SLOW:
											 car.speed_step = -SPEED_STEP;
											 CAR_Config(&car);
										break;
									//遇到流程控制语句if\or\while等，先判断是否符合判断条件，符合条件的话直接进入下一个结点
									case FLAG_PORT_SIGNAL://如果端口_有信号
												port.id = (p->EditContent)[12] - 0x30;
												Detect_Port(&port);
												if(port.cur_val == SIGNAL){
													//如果条件成立，则进入if分支
													p = if_branch(p);                    
												}else{
													//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//将这个结点赋给ptr
												}
										break;
									case FLAG_PORT_NOSIGNAL://如果端口_无信号
												port.id = (p->EditContent)[12] - 0x30;
												Detect_Port(&port);
												if(port.cur_val == NOSIGNAL){
													//如果条件成立，则进入if分支
													p = if_branch(p);                    
												}else {
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给ptr
												}
										break;
									case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
												port.id = (p->EditContent)[12] - 0x30;
												do{
														Detect_Port(&port);
												}while(port.cur_val != SIGNAL);
										break;
									case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
												port.id = (p->EditContent)[12] - 0x30;
												do{
														Detect_Port(&port);
												}while(port.cur_val != NOSIGNAL);
										break;
									case FLAG_PORT_GREATER: //如果端口_>_
												port.id = (p->EditContent)[12] - 0x30;
												port.tar_val = atoi(p->EditContent + 14);
												Detect_Port(&port);
												if(port.cur_val > port.tar_val ){
													//如果条件成立，则进入if分支
													p = if_branch(p);                    
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给ptr
												}
										break;
									case FLAG_PORT_LITTLER: //如果端口_<_
												port.id = (p->EditContent)[12] - 0x30;
												port.tar_val = atoi(p->EditContent + 14);
												Detect_Port(&port);
												if(port.cur_val < port.tar_val ){
													//如果条件成立，则进入if分支
													p = if_branch(p);                    
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给ptr
												}
										break;
									case FLAG_OBSTRACLE_GREATER:
												ult.tar_distance = atoi(p->EditContent + 16);
												if(ult.cur_distance > ult.tar_distance ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_OBSTRACLE_LITTER:
												ult.tar_distance = atoi(p->EditContent + 16);
												if(ult.cur_distance > ult.tar_distance ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_HACCEL_GREATOR:
												if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_HACCEL_LITTER:
												if(euler.accel_y * G_CONST < atof(p->EditContent + 22) ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_VACCEL_GREATOR:
												if(euler.accel_z * G_CONST > atof(p->EditContent + 22) ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_VACCEL_LITTER:
												if( euler.accel_z * G_CONST < atof(p->EditContent + 22) ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_VAR_SET_A: //设定A=
												var.id = VAR_A;
												var.set_val = atoi(p->EditContent + 8);
										break;
									case FLAG_VAR_SET_B: //设定B=
												var.id = VAR_B;
												var.set_val = atoi(p->EditContent + 8);
										break;
									case FLAG_VAR_SET_A_PORT: //A=端口_
													var.id = VAR_A;
													switch( atoi(p->EditContent + 8)){
														case PORT_1:
																 var.set_val = port_1.cur_val;
															break;
														case PORT_2:
																 var.set_val = port_2.cur_val;
															break;
														case PORT_3:
																 var.set_val = port_3.cur_val;
															break;
														case PORT_4:
																 var.set_val = port_4.cur_val;
															break;
													}
											break;
										case FLAG_VAR_SET_B_PORT: //B=端口_
													var.id = VAR_B;
													switch( atoi(p->EditContent + 8)){
														case PORT_1:
																 var.set_val = port_1.cur_val;
															break;
														case PORT_2:
																 var.set_val = port_2.cur_val;
															break;
														case PORT_3:
																 var.set_val = port_3.cur_val;
															break;
														case PORT_4:
																 var.set_val = port_4.cur_val;
															break;
													}
											break;
									case FLAG_VAR_A_INC:
													var.id = VAR_A;
													var.set_val ++;
												
										break;
									case FLAG_VAR_A_DEC:
												var.id = VAR_A;
												var.set_val --;
										break;
									case FLAG_VAR_B_INC:
												var.id = VAR_B;
												var.set_val ++;
											
										break;
									case FLAG_VAR_B_DEC:
												var.id = VAR_B;
												var.set_val --;
										break;
									case FLAG_VAR_SHOW_A:
										    display = DISP_VARIABLE;
										break;
									case FLAG_VAR_SHOW_B:
										    display = DISP_VARIABLE;
										break;
									case FLAG_VAR_A_GREATER: //如果A>_
												var.id = VAR_A;
												var.tar_val = atoi(p->EditContent + 8);
												if(var.set_val > var.tar_val ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                    
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给ptr
												}
										break;
									case FLAG_VAR_A_LITTLER: //如果A<_
												var.id = VAR_A;
												var.tar_val = atoi(p->EditContent + 8);
												if(var.set_val < var.tar_val ){
													//如果条件成立，则进入if分支
													p = if_branch(p);                    
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给ptr
												}
										break;
									case FLAG_WHILE_HEAD:
													p = while_branch(p);
										break;
									case FLAG_WHILE_TAIL:
											 //
											 //do nothing...
											 //
										break;
									case FLAG_END_PROGRAM:
												p ->next = (void *)0;
										break;
									case FLAG_OR:
												//其实不可能直接在主链表里执行到OR语句,这是语法错误
												p = or_branch(p); //返回的p指向“条件结束”指令
										break;
									case FLAG_IF_END:
												//
												//do nothing...
												//
										break;
									case FLAG_PROC:
											 jumpq = p->next ;//保存断点
											 procerr = OpenSubPro(atoi(p->EditContent + 15), p);//更改PC指针
									     if(procerr == NO_ERROR){
												  p = proc_branch(p);//执行子程序
												  p->next = jumpq;  //返回断点地址
											 }else{
												  p->next = jumpq;//返回断点地址
											 }
										break;
									case FLAG_FOR_HEAD://For循环语句处理
												if( atoi(p->EditContent + 9) != 0){
														int8_t c = atoi(p->EditContent + 9);
														p = for_branch(p, c);
												}
										break;
									case FLAG_FOR_TAIL:
										//
										//do nothing...
										//
										break;
									case FLAG_DELAY_NMS:
												delay_time = atoi(p->EditContent + 6);
												OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
										break;
									case FLAG_MUSIC:
												music.tones = atoi(p->EditContent + 6);
												music.time = atoi(p->EditContent + 13);
												Music_Config(&music);
										break;
									case FLAG_SHOW_DISTANCE:
									     display = DISP_DISTANCE;
										break;
									case FLAG_GIF_HAPPY:
											 gif = GIF_HAPPY;
									     display = DISP_GIF;
										break;
									case FLAG_GIF_SAD:
											 gif = GIF_SAD;
									     display = DISP_GIF;
										break;
									case FLAG_GIF_CRY:
											 gif = GIF_CRY;
									     display = DISP_GIF;
										break;
									case FLAG_GIF_FURY:
											 gif = GIF_FURY;
									     display = DISP_GIF;
										break;
									case FLAG_GIF_ALARM:
											 gif = GIF_ALARM;
									     display = DISP_GIF;
										break;
									case FLAG_DRAW_HCIRCLE:
											 paint.species = HOLLOW_CIRCLE;
									     display = DISP_PAINT;
										break;
									case FLAG_DRAW_SCIRCLE:
											 paint.species = SOLID_CIRCLE;
									     display = DISP_PAINT;
										break;
									case FLAG_DRAW_HRECT:
											 paint.species = HOLLOW_RECT;
									     display = DISP_PAINT;
										break;
									case FLAG_DRAW_SRECT:
											 paint.species = SOLID_RECT;
									     display = DISP_PAINT;
										break;
									case FLAG_DRAW_LINE:
											 paint.species = STRIGHT_LINE;
									     display = DISP_PAINT;
										break;
									case FLAG_SET_X1:
											 paint.x1 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_X2:
											 paint.x2 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_Y1:
											 paint.y1 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_Y2:
											 paint.y2 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_RADIUS:
											 paint.radius = atoi(p->EditContent + 6);
										break;
									case FLAG_COLOR:
					//					   paint.color = atoi(ptr->EditContent + 6);
											 COLORMAP(atoi(p->EditContent + 6), paint.color)
										break;
									default:break;
								}
								p = p -> next ;
						 }
						 
						return ret;
		  }
}
/*
*********************************************************************************************************
*	函 数 名: if_branch
*	功能说明: if 满足条件后面的代码块处理，支持多条语句，支持嵌套
*	形    参：p:主链表上的每个节点
*	返 回 值: 返回IF_END指令的指针
*********************************************************************************************************
*/
static _Listptr if_branch (_Listptr  p)
{
		p = p->next ;
		if(!p)
			return NULL;
		else{
			_Listptr  jumpq;//指向or指令或IF_END指令
			OS_ERR  err;
			u8    procerr;
			while(p->_flag != FLAG_IF_END && p->_flag != FLAG_OR && p)//if条件成立的代码块里不能包括or的情况
			{
				switch ( p ->_flag ){
				case FLAG_MOTOR_C:   //电机_正转,速度_  
								motor.id =( p->EditContent )[6] - 0x30;
								motor.direction = FORWARD;
								motor.speed = atoi(p->EditContent + 20);
								MOTOR_Config(&motor);
					break;
				case FLAG_MOTOR_CC: //电机_反转,速度_
								motor.id = ( p->EditContent )[6] - 0x30;
								motor.direction = BACKWARD;
								motor.speed = atoi(p->EditContent + 20);
				        MOTOR_Config(&motor);
					break;
				case FLAG_SERVO:  //舵机_转_
							servo.id = ( p->EditContent )[6] - 0x30;
							servo.degree = atoi(p->EditContent + 10);
				      SERVO_Config(&servo);
					break;
				case FLAG_LED_ON:  //LED_打开
							led.id = ( p->EditContent )[3] - 0x30;
							led.status = ON;
							LED_Config(&led);
					break;
				case FLAG_LED_OFF:  //LED_关闭
							led.id = ( p->EditContent )[3] - 0x30;
							led.status = OFF;
							LED_Config(&led);
					break;
				case FLAG_CAR_LEFT:
							car.direction = LEFT;
				      car.speed_step = 0;
							Car_Left();
					break;
				case FLAG_CAR_RIGHT:
					    car.direction = RIGHT;
							car.speed_step = 0;
							Car_Right();
					break;
				case FLAG_CAR_FORWARD:
							car.direction = FORWARD;
							car.speed_step = 0;
							Car_Forward();
					break;
				case FLAG_CAR_BACKWARD:
							car.direction = BACKWARD;
							car.speed_step = 0;
					    Car_Backward();
					break;
				case FLAG_CAR_STOP:
							car.direction = STOP;
							car.speed_step = 0;
					    Car_Stop();
					break;
				case FLAG_CAR_ACCEL:
					   car.speed_step = SPEED_STEP;
						 CAR_Config(&car);
					break;
				case FLAG_CAR_SLOW:
					   car.speed_step = -SPEED_STEP;
				     CAR_Config(&car);
					break;
				case FLAG_PORT_SIGNAL://如果端口_有信号
							port.id = (p->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if( port.cur_val == SIGNAL){
								//如果条件成立，则进入if分支
								p = if_branch(p);                      //嵌套
							}else{
								//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
								jumpq = Find_Node(p, FLAG_OR);
								if(!jumpq)
									jumpq = Find_Node(p, FLAG_IF_END);
								
								p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_PORT_NOSIGNAL://如果端口_无信号
							port.id = (p->EditContent)[12] - 0x30;
				      Detect_Port(&port);
							if(port.cur_val  == NOSIGNAL){
								//如果条件成立，则进入if分支
								p = if_branch(p);                      //嵌套
							}else{
								//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
								jumpq = Find_Node(p, FLAG_OR);
								if(!jumpq)
									jumpq = Find_Node(p, FLAG_IF_END);
								
								p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
							port.id = (p->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
							port.id = (p->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //如果端口_>_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
				      Detect_Port(&port);
							if(port.cur_val > port.tar_val ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_PORT_LITTLER: //如果端口_<_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
				      Detect_Port( &port );
							if(port.cur_val < port.tar_val ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_OBSTRACLE_GREATER://障碍物距离大于_
					    ult.tar_distance = atoi(p->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_OBSTRACLE_LITTER://如果障碍物距离小于_
							ult.tar_distance = atoi(p->EditContent + 16);
							if(ult.cur_distance < ult.tar_distance ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_HACCEL_GREATOR:
							if(euler.accel_y * G_CONST > atoi(p->EditContent + 22) ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_HACCEL_LITTER:
							if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_VACCEL_GREATOR:
							if(euler.accel_z * G_CONST > atof(p->EditContent + 22) ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_VACCEL_LITTER:
							if( euler.accel_z * G_CONST < atof(p->EditContent + 22) ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_VAR_SET_A: //设定A=
							var.id = VAR_A;
							var.set_val = atoi(p->EditContent + 8);
					break;
				case FLAG_VAR_SET_B: //设定B=
							var.id = VAR_B;
							var.set_val = atoi(p->EditContent + 8);
					break;
				case FLAG_VAR_SET_A_PORT: //A=端口_
							var.id = VAR_A;
							switch( atoi(p->EditContent + 8)){
								case PORT_1:
										 var.set_val = port_1.cur_val;
									break;
								case PORT_2:
										 var.set_val = port_2.cur_val;
									break;
								case PORT_3:
										 var.set_val = port_3.cur_val;
									break;
								case PORT_4:
										 var.set_val = port_4.cur_val;
									break;
							}
					break;
				case FLAG_VAR_SET_B_PORT: //B=端口_
							var.id = VAR_B;
							switch( atoi(p->EditContent + 8)){
								case PORT_1:
										 var.set_val = port_1.cur_val;
									break;
								case PORT_2:
										 var.set_val = port_2.cur_val;
									break;
								case PORT_3:
										 var.set_val = port_3.cur_val;
									break;
								case PORT_4:
										 var.set_val = port_4.cur_val;
									break;
							}
					break;
				case FLAG_VAR_A_INC:
								var.id = VAR_A;
								var.set_val ++;
							
					break;
				case FLAG_VAR_A_DEC:
							var.id = VAR_A;
							var.set_val --;
					break;
				case FLAG_VAR_B_INC:
							var.id = VAR_B;
							var.set_val ++;
						
					break;
				case FLAG_VAR_B_DEC:
							var.id = VAR_B;
							var.set_val --;
					break;
				case FLAG_VAR_SHOW_A:
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_SHOW_B:
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_A_GREATER: //变量A>_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if( var.set_val > var.tar_val ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_VAR_A_LITTLER: //变量A<_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if(var.set_val < var.tar_val ){
									//如果条件成立，则进入if分支
									p = if_branch(p);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_WHILE_HEAD:
							p = while_branch(p);
					break;
				case FLAG_WHILE_TAIL:
							//
							//此处应该报错！！！
							//
							return NULL;
				case FLAG_END_PROGRAM:
							p ->next = (void *)0;//则下一条语句无效，断开链表
					break;
//				case FLAG_OR:
//					break;
//				case FLAG_IF_END:				  //if的代码块里不包含这两种情况!!!
//					break;
				case FLAG_PROC:
					   jumpq = p->next ;//保存断点
						 procerr = OpenSubPro(atoi(p->EditContent + 15), p);//PC指针跳转
						 if(procerr == NO_ERROR){
							  p = proc_branch(p);//执行子程序
	  					  p->next = jumpq;  //返回断点地址
						 }else{
							  p->next = jumpq;//返回断点地址
						 }
					break;
				case FLAG_FOR_HEAD://For循环语句处理
							if( atoi(p->EditContent + 9) != 0){
									int8_t c = atoi(p->EditContent + 9);
									p = for_branch(p, c);
							}
					break;
				case FLAG_FOR_TAIL:
					//
					//do nothing...
				  //
					break;
				case FLAG_DELAY_NMS:
							delay_time = atoi(p->EditContent + 6);
							OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
					break;
				case FLAG_MUSIC:
							music.tones = atoi(p->EditContent + 6);
							music.time = atoi(p->EditContent + 13);
							Music_Config(&music);
					break;
				case FLAG_SHOW_DISTANCE:
					    display = DISP_DISTANCE;
					break;
				case FLAG_GIF_HAPPY:
					   gif = GIF_HAPPY;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_SAD:
					   gif = GIF_SAD;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_CRY:
					   gif = GIF_CRY;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_FURY:
					   gif = GIF_FURY;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_ALARM:
					   gif = GIF_ALARM;
				     display = DISP_GIF;
					break;
				case FLAG_DRAW_HCIRCLE:
					   paint.species = HOLLOW_CIRCLE;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_SCIRCLE:
					   paint.species = SOLID_CIRCLE;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_HRECT:
					   paint.species = HOLLOW_RECT;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_SRECT:
					   paint.species = SOLID_RECT;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_LINE:
					   paint.species = STRIGHT_LINE;
				     display = DISP_PAINT;
					break;
				case FLAG_SET_X1:
					   paint.x1 = atoi(p->EditContent + 15);
					break;
				case FLAG_SET_X2:
					   paint.x2 = atoi(p->EditContent + 15);
					break;
				case FLAG_SET_Y1:
					   paint.y1 = atoi(p->EditContent + 15);
					break;
				case FLAG_SET_Y2:
					   paint.y2 = atoi(p->EditContent + 15);
					break;
				case FLAG_SET_RADIUS:
					   paint.radius = atoi(p->EditContent + 6);
					break;
				case FLAG_COLOR:
//					   paint.color = atoi(p->EditContent + 6);
				     COLORMAP(atoi(p->EditContent + 6), paint.color)
					break;
				default:break;
				}
				p = p -> next;
			}
			if(p->_flag == FLAG_OR)     //如果执行到OR语句了，则直接寻找IF_END语句，跳过OR语句的代码块
				p = Find_Node(p, FLAG_IF_END);
			
			return p;
		}
}

/*
*********************************************************************************************************
*	函 数 名: or_branch
*	功能说明: "否则"后面的代码块处理,支持多条语句，支持嵌套
*	形    参：p:主链表上的每个节点
*	返 回 值: 返回IF_END指令的指针
*********************************************************************************************************
*/
static _Listptr or_branch (_Listptr  p)
{
	    p = p -> next;
			if(!p)
					return NULL;
			else
			{
				_Listptr    jumpq;//用于指向IF_END指令结点或嵌套的if指令结点
				OS_ERR    err;
				u8       procerr;
				while(p->_flag != FLAG_IF_END && p)
				{
					switch ( p ->_flag )
					{
						case FLAG_MOTOR_C:   //电机_正转,速度_  
										motor.id =( p->EditContent )[6] - 0x30;
										motor.direction = FORWARD;
										motor.speed = atoi(p->EditContent + 20);
										MOTOR_Config(&motor);
							break;
						case FLAG_MOTOR_CC: //电机_反转,速度_
										motor.id = ( p->EditContent )[6] - 0x30;
										motor.direction = BACKWARD;
										motor.speed = atoi(p->EditContent + 20);
										MOTOR_Config(&motor);
							break;
						case FLAG_SERVO:  //舵机_转_
									servo.id = ( p->EditContent )[6] - 0x30;
									servo.degree = atoi(p->EditContent + 10);
									SERVO_Config(&servo);
							break;
						case FLAG_LED_ON:  //LED_
									led.id = ( p->EditContent )[3] - 0x30;
									led.status = ON;
									LED_Config(&led);
							break;
						case FLAG_LED_OFF:  //LED_
									led.id = ( p->EditContent )[3] - 0x30;
									led.status = OFF;
									LED_Config(&led);
							break;
						case FLAG_CAR_LEFT:
									car.direction = LEFT;
									car.speed_step = 0;
									Car_Left();
							break;
						case FLAG_CAR_RIGHT:
									car.direction = RIGHT;
									car.speed_step = 0;
									Car_Right();
							break;
						case FLAG_CAR_FORWARD:
									car.direction = FORWARD;
									car.speed_step = 0;
									Car_Forward();
							break;
						case FLAG_CAR_BACKWARD:
									car.direction = BACKWARD;
									car.speed_step = 0;
									Car_Backward();
							break;
						case FLAG_CAR_STOP:
									car.direction = STOP;
									car.speed_step = 0;
									Car_Stop();
							break;
						case FLAG_CAR_ACCEL:
								 car.speed_step = SPEED_STEP;
								 CAR_Config(&car);
							break;
						case FLAG_CAR_SLOW:
								 car.speed_step = -SPEED_STEP;
								 CAR_Config(&car);
							break;
						case FLAG_PORT_SIGNAL://如果端口_有信号
									port.id = (p->EditContent)[12] - 0x30;
						      Detect_Port( &port );
									if(port.cur_val == SIGNAL){
										//如果条件成立，则进入if分支
										p = if_branch(p);     //返回的p指向“条件结束”指令
									}else{
										//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
										jumpq = Find_Node(p, FLAG_OR);
										if(!jumpq)
											jumpq = Find_Node(p, FLAG_IF_END);
										
										p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_PORT_NOSIGNAL://如果端口_无信号
									port.id = (p->EditContent)[12] - 0x30;
						      Detect_Port(&port);
									if(port.cur_val == NOSIGNAL){
										//如果条件成立，则进入if分支
										p = if_branch(p);        //返回的p指向“条件结束”指令
									}else{
										//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
										jumpq = Find_Node(p, FLAG_OR);
										if(!jumpq)
											jumpq = Find_Node(p, FLAG_IF_END);
										
										p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
									port.id = (p->EditContent)[12] - 0x30;
						      do{
										  Detect_Port(&port);
									}while(port.cur_val != SIGNAL);
							break;
						case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
									port.id = (p->EditContent)[12] - 0x30;
						      do{
										  Detect_Port(&port) ;
									}while(port.cur_val != NOSIGNAL);
							break;
						case FLAG_PORT_GREATER: //如果端口_>_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
						      Detect_Port(&port);
									if(port.cur_val > port.tar_val ){
											//如果条件成立，则进入if分支
											p = if_branch(p);       //返回的p指向“条件结束”指令
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_PORT_LITTLER: //如果端口_<_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
						      Detect_Port(&port);
									if(port.cur_val < port.tar_val ){
											//如果条件成立，则进入if分支
											p = if_branch(p);       //返回的p指向“条件结束”指令
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_OBSTRACLE_GREATER:
									ult.tar_distance = atoi(p->EditContent + 16);
									if(ult.cur_distance > ult.tar_distance ){
											//如果条件成立，则进入if分支
											p = if_branch(p);         //返回的p指向“条件结束”指令
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_OBSTRACLE_LITTER:
							    ult.tar_distance = atoi(p->EditContent + 16);
									if(ult.cur_distance < ult.tar_distance ){
											//如果条件成立，则进入if分支
											p = if_branch(p);      //返回的p指向“条件结束”指令
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_HACCEL_GREATOR:
									if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
											//如果条件成立，则进入if分支
											p = if_branch(p);                      //嵌套
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_HACCEL_LITTER:
									if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
											//如果条件成立，则进入if分支
											p = if_branch(p);                      //嵌套
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_VACCEL_GREATOR:
									if(euler.accel_z * G_CONST > atof(p->EditContent + 22) ){
											//如果条件成立，则进入if分支
											p = if_branch(p);                      //嵌套
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_VACCEL_LITTER:
									if( euler.accel_z * G_CONST < atof(p->EditContent + 22) ){
											//如果条件成立，则进入if分支
											p = if_branch(p);                      //嵌套
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_VAR_SET_A: //设定A=
									var.id = VAR_A;
									var.set_val = atoi(p->EditContent + 8);
							break;
						case FLAG_VAR_SET_B: //设定B=
									var.id = VAR_B;
									var.set_val = atoi(p->EditContent + 8);
							break;
						case FLAG_VAR_SET_A_PORT: //A=端口_
									var.id = VAR_A;
									switch( atoi(p->EditContent + 8)){
										case PORT_1:
												 var.set_val = port_1.cur_val;
											break;
										case PORT_2:
												 var.set_val = port_2.cur_val;
											break;
										case PORT_3:
												 var.set_val = port_3.cur_val;
											break;
										case PORT_4:
												 var.set_val = port_4.cur_val;
											break;
										
									}
							break;
						case FLAG_VAR_SET_B_PORT: //B=端口_
									var.id = VAR_B;
									switch( atoi(p->EditContent + 8)){
										case PORT_1:
												 var.set_val = port_1.cur_val;
											break;
										case PORT_2:
												 var.set_val = port_2.cur_val;
											break;
										case PORT_3:
												 var.set_val = port_3.cur_val;
											break;
										case PORT_4:
												 var.set_val = port_4.cur_val;
											break;
										
									}
							break;
						case FLAG_VAR_A_INC:
										var.id = VAR_A;
										var.set_val ++;
									
							break;
						case FLAG_VAR_A_DEC:
									var.id = VAR_A;
									var.set_val --;
							break;
						case FLAG_VAR_B_INC:
									var.id = VAR_B;
									var.set_val ++;
								
							break;
						case FLAG_VAR_B_DEC:
									var.id = VAR_B;
									var.set_val --;
							break;
						case FLAG_VAR_SHOW_A:
							    display = DISP_VARIABLE;
							break;
						case FLAG_VAR_SHOW_B:
							    display = DISP_VARIABLE;
							break;
						case FLAG_VAR_A_GREATER: //变量A>_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val > var.tar_val ){
										//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
										p = if_branch(p);  //返回的p指向“条件结束”指令
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_VAR_A_LITTLER: //变量A<_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val < var.tar_val ){
										//如果条件成立，则进入if分支
										p = if_branch(p);        //返回的p指向“条件结束”指令
									}else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//将这个结点赋给p
									}
							break;
						case FLAG_WHILE_HEAD:
									p = while_branch(p);
							break;
						case FLAG_WHILE_TAIL:
									//
									//do nothing....
									//
									return NULL;
						case FLAG_END_PROGRAM:
									p ->next = (void *)0;//则下一条语句无效，断开链表
							break;
						case FLAG_OR:  //嵌套 "否则" 的情况
									//如果or前面没有If语句就会无穷次嵌套!!!最终栈溢出
									p = or_branch(p);   //返回的p指向“条件结束”指令
							break;
//						case FLAG_IF_END:   //不包含if_end的结点，只处理"否则"指令里包含的代码块
//							break;
						case FLAG_PROC:
							   jumpq = p->next ;//保存断点
								 procerr = OpenSubPro(atoi(p->EditContent + 15), p);//PC指针跳转
								 if(procerr == NO_ERROR){
										p = proc_branch(p);//执行子程序
										p->next = jumpq;  //返回断点地址
								 }else{
										p->next = jumpq;//返回断点地址
								 }
							break;
						case FLAG_FOR_HEAD://For循环语句处理
									if( atoi(p->EditContent + 9) != 0){
											int8_t c = atoi(p->EditContent + 9);
											p = for_branch(p, c);
									}
							break;
						case FLAG_FOR_TAIL:
							//
							//do nothing...
							//
							break;
						case FLAG_DELAY_NMS:
									delay_time = atoi(p->EditContent + 6);
									OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
							break;
						case FLAG_MUSIC:
									music.tones = atoi(p->EditContent + 6);
									music.time = atoi(p->EditContent + 13);
									Music_Config(&music);
							break;
					  case FLAG_SHOW_DISTANCE:
									display = DISP_DISTANCE;
							break;
						case FLAG_GIF_HAPPY:
								 gif = GIF_HAPPY;
						     display = DISP_GIF;
							break;
						case FLAG_GIF_SAD:
								 gif = GIF_SAD;
						     display = DISP_GIF;
							break;
						case FLAG_GIF_CRY:
								 gif = GIF_CRY;
						     display = DISP_GIF;
							break;
						case FLAG_GIF_FURY:
								 gif = GIF_FURY;
						     display = DISP_GIF;
							break;
						case FLAG_GIF_ALARM:
								 gif = GIF_ALARM;
						     display = DISP_GIF;
							break;
						case FLAG_DRAW_HCIRCLE:
					       paint.species = HOLLOW_CIRCLE;
						     display = DISP_PAINT;
								 break;
						case FLAG_DRAW_SCIRCLE:
								 paint.species = SOLID_CIRCLE;
						     display = DISP_PAINT;
							break;
						case FLAG_DRAW_HRECT:
								 paint.species = HOLLOW_RECT;
						     display = DISP_PAINT;
							break;
						case FLAG_DRAW_SRECT:
								 paint.species = SOLID_RECT;
						     display = DISP_PAINT;
							break;
						case FLAG_DRAW_LINE:
								 paint.species = STRIGHT_LINE;
						     display = DISP_PAINT;
							break;
						case FLAG_SET_X1:
								 paint.x1 = atoi(p->EditContent + 15);
							break;
						case FLAG_SET_X2:
								 paint.x2 = atoi(p->EditContent + 15);
							break;
						case FLAG_SET_Y1:
								 paint.y1 = atoi(p->EditContent + 15);
							break;
						case FLAG_SET_Y2:
								 paint.y2 = atoi(p->EditContent + 15);
							break;
						case FLAG_SET_RADIUS:
								 paint.radius = atoi(p->EditContent + 6);
							break;
						case FLAG_COLOR:
//								 paint.color = atoi(p->EditContent + 6);
						     COLORMAP(atoi(p->EditContent + 6), paint.color)
							break;
						default:break;
					 }
					p = p -> next;
				}
			 return p;
			}
}
/*********************************************************************************************************
*	函 数 名: while_branch
*	功能说明: "循环开始"后面的代码块处理.支持嵌套
*	形    参：p:主链表上的每个节点
*	返 回 值: 循环语句不会返回
*********************************************************************************************************
*/
static _Listptr while_branch (_Listptr  p)
{
	    _Listptr  while_head = p;
	    p = p->next ;
			if(!p)
				return NULL;
			else
			{
					_Listptr        while_tail;//指向WHILE_END语句
					_Listptr 	            jumpq;//用于指向IF_END指令结点或嵌套的if指令结点
					OS_ERR                  err;
				  u8                  procerr;
				
					while_tail = Find_Node(p, FLAG_WHILE_TAIL);//找到WHILE_TAIL结点
					if(!while_tail)
						return NULL;
					else{

								while( p)
								{
										switch ( p ->_flag )
										{
											case FLAG_MOTOR_C:   //电机_正转,速度_  
															motor.id =( p->EditContent )[6] - 0x30;
															motor.direction = FORWARD;
															motor.speed = atoi(p->EditContent + 20);
															MOTOR_Config(&motor);
												break;
											case FLAG_MOTOR_CC: //电机_反转,速度_
															motor.id = ( p->EditContent )[6] - 0x30;
															motor.direction = BACKWARD;
															motor.speed = atoi(p->EditContent + 20);
															MOTOR_Config(&motor);
												break;
											case FLAG_SERVO:  //舵机_转_
														servo.id = ( p->EditContent )[6] - 0x30;
														servo.degree = atoi(p->EditContent + 10);
											      SERVO_Config(&servo);
												break;
											case FLAG_LED_ON:  //LED_
														led.id = ( p->EditContent )[3] - 0x30;
														led.status = ON;
														LED_Config(&led);
												break;
											case FLAG_LED_OFF:  //LED_
														led.id = ( p->EditContent )[3] - 0x30;
														led.status = OFF;
														LED_Config(&led);
												break;
											case FLAG_CAR_LEFT:
														car.direction = LEFT;
														car.speed_step = 0;
														Car_Left();
												break;
											case FLAG_CAR_RIGHT:
														car.direction = RIGHT;
														car.speed_step = 0;
														Car_Right();
												break;
											case FLAG_CAR_FORWARD:
														car.direction = FORWARD;
														car.speed_step = 0;
														Car_Forward();
												break;
											case FLAG_CAR_BACKWARD:
														car.direction = BACKWARD;
														car.speed_step = 0;
														Car_Backward();
												break;
											case FLAG_CAR_STOP:
														car.direction = STOP;
														car.speed_step = 0;
														Car_Stop();
												break;
											case FLAG_CAR_ACCEL:
													 car.speed_step = SPEED_STEP;
													 CAR_Config(&car);
												break;
											case FLAG_CAR_SLOW:
													 car.speed_step = -SPEED_STEP;
													 CAR_Config(&car);
												break;
											case FLAG_PORT_SIGNAL://如果端口_有信号
														port.id = (p->EditContent)[12] - 0x30;
														Detect_Port(&port);
														if(port.cur_val == SIGNAL){
															//如果条件成立，则进入if分支
															p = if_branch(p);   //返回的p指向“条件结束”指令
														}else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_PORT_NOSIGNAL://如果端口_无信号
														port.id = (p->EditContent)[12] - 0x30;
														Detect_Port(&port);
														if(port.cur_val == NOSIGNAL){
															//如果条件成立，则进入if分支
															p = if_branch(p);      //返回的p指向“条件结束”指令
														}else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
														port.id = (p->EditContent)[12] - 0x30;
														do{
															  Detect_Port(&port);
														}while(port.cur_val != SIGNAL);
												break;
											case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
														port.id = (p->EditContent)[12] - 0x30;
														do{
															  Detect_Port(&port);
														}while(port.cur_val != NOSIGNAL);
												break;
											case FLAG_PORT_GREATER: //如果端口_>_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														Detect_Port(&port);
														if(port.cur_val > port.tar_val ){
															//如果条件成立，则进入if分支
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_PORT_LITTLER: //如果端口_<_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														Detect_Port(&port);
														if(port.cur_val < port.tar_val ){
															//如果条件成立，则进入if分支
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_OBSTRACLE_GREATER:
												    ult.tar_distance = atoi(p->EditContent + 16);
														if(ult.cur_distance > ult.tar_distance ){
																//如果条件成立，则进入if分支
																p = if_branch(p);     //返回的p指向“条件结束”指令
														}else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_OBSTRACLE_LITTER:
												    ult.tar_distance = atoi(p->EditContent + 16);
														if(ult.cur_distance < ult.tar_distance ){
																//如果条件成立，则进入if分支
																p = if_branch(p);     //返回的p指向“条件结束”指令
														}else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_HACCEL_GREATOR:
														if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
																//如果条件成立，则进入if分支
																p = if_branch(p);                      //嵌套
														}else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_HACCEL_LITTER:
														if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
																//如果条件成立，则进入if分支
																p = if_branch(p);                      //嵌套
														}else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_VACCEL_GREATOR:
														if(euler.accel_z * G_CONST > atof(p->EditContent + 22) ){
																//如果条件成立，则进入if分支
																p = if_branch(p);                      //嵌套
														}else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_VACCEL_LITTER:
														if( euler.accel_z * G_CONST <  atof(p->EditContent + 22)){
																//如果条件成立，则进入if分支
																p = if_branch(p);                      //嵌套
														}else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_VAR_SET_A: //设定A=
														var.id = VAR_A;
														var.set_val = atoi(p->EditContent + 8);
												break;
											case FLAG_VAR_SET_B: //设定B=
														var.id = VAR_B;
														var.set_val = atoi(p->EditContent + 8);
												break;
											case FLAG_VAR_SET_A_PORT: //A=端口_
														var.id = VAR_A;
														switch( atoi(p->EditContent + 8))
														{
															case PORT_1:
																	 var.set_val = port_1.cur_val;
																break;
															case PORT_2:
																	 var.set_val = port_2.cur_val;
																break;
															case PORT_3:
																	 var.set_val = port_3.cur_val;
																break;
															case PORT_4:
																	 var.set_val = port_4.cur_val;
																break;
															
														}
												break;
											case FLAG_VAR_SET_B_PORT: //B=端口_
														var.id = VAR_B;
														switch( atoi(p->EditContent + 8))
														{
															case PORT_1:
																	 var.set_val = port_1.cur_val;
																break;
															case PORT_2:
																	 var.set_val = port_2.cur_val;
																break;
															case PORT_3:
																	 var.set_val = port_3.cur_val;
																break;
															case PORT_4:
																	 var.set_val = port_4.cur_val;
																break;
															
														}
												break;
											case FLAG_VAR_A_INC:
															var.id = VAR_A;
															var.set_val ++;
														
												break;
											case FLAG_VAR_A_DEC:
														var.id = VAR_A;
														var.set_val --;
												break;
											case FLAG_VAR_B_INC:
														var.id = VAR_B;
														var.set_val ++;
													
												break;
											case FLAG_VAR_B_DEC:
														var.id = VAR_B;
														var.set_val --;
												break;
											case FLAG_VAR_SHOW_A:
												    display = DISP_VARIABLE;
												break;
											case FLAG_VAR_SHOW_B:
												    display = DISP_VARIABLE;
												break;
											case FLAG_VAR_A_GREATER: //变量A>_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  > var.tar_val  )
														{
															//如果条件成立，则进入if分支
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_VAR_A_LITTLER: //变量A<_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  < var.tar_val  )
														{
															//如果条件成立，则进入if分支
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//将这个结点赋给p
														}
												break;
											case FLAG_WHILE_HEAD:
														p = while_branch(p); //嵌套
												break;
											case FLAG_WHILE_TAIL://到了循环语句尾部就跳转至循环语句首部
														p = while_head;//因为这里记录的Index_first是WHILE_HEAD指令的后一句，
																																					//所以index_first的上一个索引才是WHILE_HEAD
												break;
											case FLAG_END_PROGRAM:
														p ->next = (void *)0;//则下一条语句无效，断开链表
												break;
											case FLAG_OR:
												    
											      p = or_branch(p);  //返回的p指向“条件结束”指令
												break;
											case FLAG_IF_END:
														//
														//do nothing...
														//
												break;
											case FLAG_PROC:
												   jumpq = p->next ;//保存断点
								           procerr = OpenSubPro(atoi(p->EditContent + 15), p);//PC指针跳转
								           if(procerr == NO_ERROR){
															p = proc_branch(p);//执行子程序
															p->next = jumpq;  //返回断点地址
													 }else{
															p->next = jumpq;//返回断点地址
													 }
												break;
											case FLAG_FOR_HEAD://for循环
													if(atoi(p->EditContent + 9)){
														uint8_t c = atoi(p->EditContent + 9);
														p = for_branch(p, c);
													}
												break;
											case FLAG_FOR_TAIL:
													//
												break;
											case FLAG_DELAY_NMS:
														delay_time = atoi(p->EditContent + 6);
														OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
												break;
											case FLAG_MUSIC:
												   	music.tones = atoi(p->EditContent + 6);
														music.time = atoi(p->EditContent + 13);
														Music_Config(&music);
												break;
											case FLAG_SHOW_DISTANCE:
														display = DISP_DISTANCE;
												break;
											case FLAG_GIF_HAPPY:
													 gif = GIF_HAPPY;
											     display = DISP_GIF;
												break;
											case FLAG_GIF_SAD:
													 gif = GIF_SAD;
											     display = DISP_GIF;
												break;
											case FLAG_GIF_CRY:
													 gif = GIF_CRY;
											     display = DISP_GIF;
												break;
											case FLAG_GIF_FURY:
													 gif = GIF_FURY;
											     display = DISP_GIF;
												break;
											case FLAG_GIF_ALARM:
													 gif = GIF_ALARM;
											     display = DISP_GIF;
												break;
											case FLAG_DRAW_HCIRCLE:
					                 paint.species = HOLLOW_CIRCLE;
											     display = DISP_PAINT;
					              break;
											case FLAG_DRAW_SCIRCLE:
													 paint.species = SOLID_CIRCLE;
											     display = DISP_PAINT;
												break;
											case FLAG_DRAW_HRECT:
													 paint.species = HOLLOW_RECT;
											     display = DISP_PAINT;
												break;
											case FLAG_DRAW_SRECT:
													 paint.species = SOLID_RECT;
											     display = DISP_PAINT;
												break;
											case FLAG_DRAW_LINE:
													 paint.species = STRIGHT_LINE;
											     display = DISP_PAINT;
												break;
											case FLAG_SET_X1:
													 paint.x1 = atoi(p->EditContent + 15);
												break;
											case FLAG_SET_X2:
													 paint.x2 = atoi(p->EditContent + 15);
												break;
											case FLAG_SET_Y1:
													 paint.y1 = atoi(p->EditContent + 15);
												break;
											case FLAG_SET_Y2:
													 paint.y2 = atoi(p->EditContent + 15);
												break;
											case FLAG_SET_RADIUS:
													 paint.radius = atoi(p->EditContent + 6);
												break;
											case FLAG_COLOR:
//													 paint.color = atoi(p->EditContent + 6);
											     COLORMAP(atoi(p->EditContent + 6), paint.color)
												break;
											default:break;
										 }
                                  
										p = p ->next ;//这时的p指向的才是循环语句里的第一条指令
										 
										OSTimeDly(5 ,OS_OPT_TIME_DLY, &err);//while循环语句里必须加一句延时，以防死循环不能实现任务调度
								}
								return p;
					}
		 }
}
/*
*********************************************************************************************************
*	函 数 名: for_branch
*	功能说明: "FOR循环语句"后面的代码块处理,支持多条语句，支持嵌套
*	形    参：p:主链表上的每个节点,
*					  cycles:循环次数
*	返 回 值: 返回FOR_END指令的指针
*********************************************************************************************************
*/
static _Listptr for_branch (_Listptr  p, uint8_t cycles)
{
		_Listptr for_head = p;
		p = p->next ;
	
		if(!p){
			return NULL;
		}else{
				_Listptr        for_tail;//指向WHILE_END语句
				_Listptr 	            jumpq;//用于指向IF_END指令结点或嵌套的if指令结点
				OS_ERR                  err;
				u8                  procerr;
			
				for_tail = Find_Node(p, FLAG_FOR_TAIL);//找到FOR_TAIL结点
				if(!for_tail){
					return NULL;
				}else{
						while( p ){
								switch ( p ->_flag ){
									case FLAG_MOTOR_C:   //电机_正转,速度_  
													motor.id =( p->EditContent )[6] - 0x30;
													motor.direction = FORWARD;
													motor.speed = atoi(p->EditContent + 20);
													MOTOR_Config(&motor);
										break;
									case FLAG_MOTOR_CC: //电机_反转,速度_
													motor.id = ( p->EditContent )[6] - 0x30;
													motor.direction = BACKWARD;
													motor.speed = atoi(p->EditContent + 20);
													MOTOR_Config(&motor);
										break;
									case FLAG_SERVO:  //舵机_转_
												servo.id = ( p->EditContent )[6] - 0x30;
												servo.degree = atoi(p->EditContent + 10);
												SERVO_Config(&servo);
										break;
									case FLAG_LED_ON:  //LED_
												led.id = ( p->EditContent )[3] - 0x30;
												led.status = ON;
												LED_Config(&led);
										break;
									case FLAG_LED_OFF:  //LED_
												led.id = ( p->EditContent )[3] - 0x30;
												led.status = OFF;
												LED_Config(&led);
										break;
									case FLAG_CAR_LEFT:
												car.direction = LEFT;
												car.speed_step = 0;
												Car_Left();
										break;
									case FLAG_CAR_RIGHT:
												car.direction = RIGHT;
												car.speed_step = 0;
												Car_Right();
										break;
									case FLAG_CAR_FORWARD:
												car.direction = FORWARD;
												car.speed_step = 0;
												Car_Forward();
										break;
									case FLAG_CAR_BACKWARD:
												car.direction = BACKWARD;
												car.speed_step = 0;
												Car_Backward();
										break;
									case FLAG_CAR_STOP:
												car.direction = STOP;
												car.speed_step = 0;
												Car_Stop();
										break;
									case FLAG_CAR_ACCEL:
											 car.speed_step = SPEED_STEP;
											 CAR_Config(&car);
										break;
									case FLAG_CAR_SLOW:
											 car.speed_step = -SPEED_STEP;
											 CAR_Config(&car);
										break;
									case FLAG_PORT_SIGNAL://如果端口_有信号
												port.id = (p->EditContent)[12] - 0x30;
												Detect_Port(&port);
												if(port.cur_val == SIGNAL){
													//如果条件成立，则进入if分支
													p = if_branch(p);   //返回的p指向“条件结束”指令
												}else{
													//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_PORT_NOSIGNAL://如果端口_无信号
												port.id = (p->EditContent)[12] - 0x30;
												Detect_Port(&port);
												if(port.cur_val == NOSIGNAL){
													//如果条件成立，则进入if分支
													p = if_branch(p);      //返回的p指向“条件结束”指令
												}else{
													//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
												port.id = (p->EditContent)[12] - 0x30;
												do{
														Detect_Port(&port);
												}while(port.cur_val != SIGNAL);
										break;
									case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
												port.id = (p->EditContent)[12] - 0x30;
												do{
														Detect_Port(&port);
												}while(port.cur_val != NOSIGNAL);
										break;
									case FLAG_PORT_GREATER: //如果端口_>_
												port.id = (p->EditContent)[12] - 0x30;
												port.tar_val = atoi(p->EditContent + 14);
												Detect_Port(&port);
												if(port.cur_val > port.tar_val ){
													//如果条件成立，则进入if分支
													p = if_branch(p);     //返回的p指向“条件结束”指令
												}else{
													//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_PORT_LITTLER: //如果端口_<_
												port.id = (p->EditContent)[12] - 0x30;
												port.tar_val = atoi(p->EditContent + 14);
												Detect_Port(&port);
												if(port.cur_val < port.tar_val ){
													//如果条件成立，则进入if分支
													p = if_branch(p);     //返回的p指向“条件结束”指令
												}else{
													//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_OBSTRACLE_GREATER:
												ult.tar_distance = atoi(p->EditContent + 16);
												if(ult.cur_distance > ult.tar_distance ){
														//如果条件成立，则进入if分支
														p = if_branch(p);     //返回的p指向“条件结束”指令
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_OBSTRACLE_LITTER:
												ult.tar_distance = atoi(p->EditContent + 16);
												if(ult.cur_distance < ult.tar_distance ){
														//如果条件成立，则进入if分支
														p = if_branch(p);     //返回的p指向“条件结束”指令
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_HACCEL_GREATOR:
												if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_HACCEL_LITTER:
												if(euler.accel_y * G_CONST > atof(p->EditContent + 22) ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_VACCEL_GREATOR:
												if(euler.accel_z * G_CONST > atof(p->EditContent + 22) ){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_VACCEL_LITTER:
												if( euler.accel_z * G_CONST <  atof(p->EditContent + 22)){
														//如果条件成立，则进入if分支
														p = if_branch(p);                      //嵌套
												}else{
														//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_VAR_SET_A: //设定A=
												var.id = VAR_A;
												var.set_val = atoi(p->EditContent + 8);
										break;
									case FLAG_VAR_SET_B: //设定B=
												var.id = VAR_B;
												var.set_val = atoi(p->EditContent + 8);
										break;
									case FLAG_VAR_SET_A_PORT: //A=端口_
												var.id = VAR_A;
												switch( atoi(p->EditContent + 8)){
													case PORT_1:
															 var.set_val = port_1.cur_val;
														break;
													case PORT_2:
															 var.set_val = port_2.cur_val;
														break;
													case PORT_3:
															 var.set_val = port_3.cur_val;
														break;
													case PORT_4:
															 var.set_val = port_4.cur_val;
														break;
												}
										break;
									case FLAG_VAR_SET_B_PORT: //B=端口_
												var.id = VAR_B;
												switch( atoi(p->EditContent + 8)){
													case PORT_1:
															 var.set_val = port_1.cur_val;
														break;
													case PORT_2:
															 var.set_val = port_2.cur_val;
														break;
													case PORT_3:
															 var.set_val = port_3.cur_val;
														break;
													case PORT_4:
															 var.set_val = port_4.cur_val;
														break;
												}
										break;
									case FLAG_VAR_A_INC:
													var.id = VAR_A;
													var.set_val ++;
												
										break;
									case FLAG_VAR_A_DEC:
												var.id = VAR_A;
												var.set_val --;
										break;
									case FLAG_VAR_B_INC:
												var.id = VAR_B;
												var.set_val ++;
											
										break;
									case FLAG_VAR_B_DEC:
												var.id = VAR_B;
												var.set_val --;
										break;
									case FLAG_VAR_SHOW_A:
												display = DISP_VARIABLE;
										break;
									case FLAG_VAR_SHOW_B:
												display = DISP_VARIABLE;
										break;
									case FLAG_VAR_A_GREATER: //变量A>_
												var.id = VAR_A;
												var.tar_val = atoi(p->EditContent + 8);
												if(var.set_val  > var.tar_val  ){
													//如果条件成立，则进入if分支
													p = if_branch(p);     //返回的p指向“条件结束”指令
												}else{
													//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_VAR_A_LITTLER: //变量A<_
												var.id = VAR_A;
												var.tar_val = atoi(p->EditContent + 8);
												if(var.set_val  < var.tar_val  ){
													//如果条件成立，则进入if分支
													p = if_branch(p);     //返回的p指向“条件结束”指令
												}else{
													//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//将这个结点赋给p
												}
										break;
									case FLAG_WHILE_HEAD:
												p = while_branch(p); //
										break;
									case FLAG_WHILE_TAIL://
												//
												//do nothing...
												//
										break;
									case FLAG_END_PROGRAM:
												p ->next = (void *)0;//则下一条语句无效，断开链表
										break;
									case FLAG_OR:
												
												p = or_branch(p);  //返回的p指向“条件结束”指令
										break;
									case FLAG_IF_END:
												//
												//do nothing...
												//
										break;
									case FLAG_PROC:
											 jumpq = p->next ;//保存断点
											 procerr = OpenSubPro(atoi(p->EditContent + 15), p);//PC指针跳转
											 if(procerr == NO_ERROR){
													p = proc_branch(p);//执行子程序
													p->next = jumpq;  //返回断点地址
											 }else{
													p->next = jumpq;//返回断点地址
											 }
										break;
									case FLAG_FOR_HEAD://for循环嵌套
											if(atoi(p->EditContent + 9)){
												uint8_t c = atoi(p->EditContent + 9);
												p = for_branch(p, c);
											}
										break;
									case FLAG_FOR_TAIL:
												if(cycles > 1){//当剩余循环次数大于1时，跳至循环语句头部
													p = for_head;//重定位至for循环头部
												}else{//当循环次数<=1时，等待循环结束
												}
												cycles --;//循环次数减一
										break;
									case FLAG_DELAY_NMS:
												delay_time = atoi(p->EditContent + 6);
												OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
										break;
									case FLAG_MUSIC:
												music.tones = atoi(p->EditContent + 6);
												music.time = atoi(p->EditContent + 13);
												Music_Config(&music);
										break;
									case FLAG_SHOW_DISTANCE:
												display = DISP_DISTANCE;
										break;
									case FLAG_GIF_HAPPY:
											 gif = GIF_HAPPY;
											 display = DISP_GIF;
										break;
									case FLAG_GIF_SAD:
											 gif = GIF_SAD;
											 display = DISP_GIF;
										break;
									case FLAG_GIF_CRY:
											 gif = GIF_CRY;
											 display = DISP_GIF;
										break;
									case FLAG_GIF_FURY:
											 gif = GIF_FURY;
											 display = DISP_GIF;
										break;
									case FLAG_GIF_ALARM:
											 gif = GIF_ALARM;
											 display = DISP_GIF;
										break;
									case FLAG_DRAW_HCIRCLE:
											 paint.species = HOLLOW_CIRCLE;
											 display = DISP_PAINT;
										break;
									case FLAG_DRAW_SCIRCLE:
											 paint.species = SOLID_CIRCLE;
											 display = DISP_PAINT;
										break;
									case FLAG_DRAW_HRECT:
											 paint.species = HOLLOW_RECT;
											 display = DISP_PAINT;
										break;
									case FLAG_DRAW_SRECT:
											 paint.species = SOLID_RECT;
											 display = DISP_PAINT;
										break;
									case FLAG_DRAW_LINE:
											 paint.species = STRIGHT_LINE;
											 display = DISP_PAINT;
										break;
									case FLAG_SET_X1:
											 paint.x1 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_X2:
											 paint.x2 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_Y1:
											 paint.y1 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_Y2:
											 paint.y2 = atoi(p->EditContent + 15);
										break;
									case FLAG_SET_RADIUS:
											 paint.radius = atoi(p->EditContent + 6);
										break;
									case FLAG_COLOR:
//													 paint.color = atoi(p->EditContent + 6);
											 COLORMAP(atoi(p->EditContent + 6), paint.color)
										break;
									default:break;
								 }
															
								if( cycles != 0 ){
									p = p ->next ;//这时的p指向的才是循环语句里的第一条指令
								}else{//达到循环次数后，直接返回指向 FOR_END指令的指针
									return p;
								}
								OSTimeDly(5 ,OS_OPT_TIME_DLY, &err);//while循环语句里必须加一句延时，以防死循环不能实现任务调度
						}
						return p;
				}
		 }
	  
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************************************************
*	函 数 名: Create_List
*	功能说明: 建立一个有表头结点的单链表
*          包括主链表和子程序链表的表头结点
*	形    参：无
*	返 回 值: 循环语句不会返回
*********************************************************************************************************
*/
int Create_List(void){
	
		Ins_List_Head = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
		if( !Ins_List_Head)
			return -1;
		Ins_List_Head -> index = 0;
		Ins_List_Head->next = (void*)0;
		
		Sub_List_Head = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
		if( !Sub_List_Head)
			return -1;
		Sub_List_Head -> index = 0;
		Sub_List_Head->next = (void*)0;
		return 0;
	
}
/*********************************************************************************************************
*	函 数 名: Add_Node
*	功能说明: 增加节点,将元素加到下标为index的地方。其实是依次将结点从表头结点接下来
*          具有可重入性
*	形    参：head:主链表指针    index:节点在主链表中的位置   flag:节点数据域所属类别    content:节点数据域内容
*	返 回 值: 0\1
*********************************************************************************************************
*/
int Add_Node (_Listptr head, int index, uint8_t flag, char *content)
{
				int i = 0;
				_Listptr    p_temp = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
				_Listptr    p_head = head;
			  u8      Mb_Val;
				if(index <= 0 || !p_temp)
				{
					_MessageBox("Fail to malloc","Error",&Mb_Val);
					return -1;
				}
				
				p_temp -> index = index;
				strcpy(p_temp -> EditContent, content);
				p_temp -> _flag = (enum _FLAG)flag;
				
				while(p_head && i < index-1)
				{
					p_head = p_head ->next ;//此时p即为下标为index的结点的前驱节点
					i ++;
				}
				
				if(!p_head)
					return -1;
				else{
							p_temp -> next = p_head->next ;//使前驱的后继成为新增结点的后继
							p_head->next  = p_temp ;  //使新增结点的称为前驱的后继
				}
				
				return 0;
}
/*********************************************************************************************************
*	函 数 名: Replace_Node
*	功能说明: 替换下标为index的结点的内容
*          
*	形    参：head:主链表指针    index:节点在主链表中的位置   flag:节点数据域所属类别    content:节点数据域内容
*	返 回 值: 0\1
*********************************************************************************************************
*/
int  Replace_Node(int index, enum _FLAG flag,char *content)
{
				int i = 0;
				_Listptr    p_head = Ins_List_Head;
	
				if(index <= 0 )
					return -1;

				while(p_head && i < index)
				{
					p_head = p_head ->next ;//此时p即为下标为index的结点
					i ++;
				}
				
				if(!p_head)
				{
					
					return -1;
				}
				else{
							p_head->_flag  = flag ;
							strcpy(p_head -> EditContent, content);
				}
				
				return 0;

}

/*********************************************************************************************************
*	函 数 名: Delete_Node
*	功能说明: 删除下标为Index的结点
*          
*	形    参：head:主链表指针    index:节点在主链表中的位置   flag:节点数据域所属类别    content:节点数据域内容
*	返 回 值: 0\1
*********************************************************************************************************
*/
int Delete_Node(int index)
{
		int i;
		_Listptr p_head = Ins_List_Head;
		_Listptr p_index;
	
		if(index <= 0)
			return -1;
		
		for(i = 0; i< index - 1;i++)
				p_head = p_head -> next;//此时p为index结点的前驱结点
		if(!p_head)
		{
			return -1;
		}
		else
		{
			p_index = p_head ->next ;//q结点即为Index结点
			p_head -> next = p_index -> next;//使index结点的后继成为p的后继
			
			while(p_head -> next){
					p_head = p_head -> next;
					p_head->index --;
			}
			
			myfree(SRAMIN,p_index);  //释放index结点的空间
			
			return 0;
		}
		
}
/*********************************************************************************************************
*	函 数 名: GetListLength
*	功能说明: 获取当前链表的长度,
*          
*	形    参：输入参数为表头结点
*	返 回 值: 0\1
*********************************************************************************************************
*/
int  GetListLength(_Listptr  head)
{
		_Listptr p = head;
		int length =0 ;
		if(!p)
			return -1;
		else{
			while(p -> next)
			{
				p = p -> next ;
				length ++;
			}
			return length;
		}
}
/*********************************************************************************************************
*	函 数 名: Clear_List
*	功能说明: 清空整个链表，释放原来的结点空间
*          
*	形    参：输入参数为表头结点
*	返 回 值: 无
*********************************************************************************************************
*/
void Clear_List(_Listptr  head)
{
		_Listptr p = head ;
		_Listptr q ;
		int      i = GetListLength(p) ;
	
		while(i--)
		{
				q = p -> next;
				p -> next = q -> next;
				myfree(SRAMIN, q);
		}
		
}
/*********************************************************************************************************
*	函 数 名: Find_Node
*	功能说明: 从index结点开始寻找标志为flag的结点,并返回该结点的指针
*          
*	形    参：输入参数为表头结点
*	返 回 值: 无
*********************************************************************************************************
*/
static _Listptr  Find_Node(_Listptr head, enum _FLAG flag)
{
				_Listptr    p = head;
				
				if(!p)
					return (_Listptr)0;
				else{
							while(p->_flag != flag && p)
							{
								   p = p->next ;	 
							}
							if(!p)
								return (_Listptr)0;
							else
								return p ;
				}
				
}

/*********************************************************************************************************
*	函 数 名: List_Parse
*	功能说明: 链表解析函数
*          解析工作绝对不可以破坏原链表的顺序
*	形    参：输入参数为表头结点
*	返 回 值: 无
*********************************************************************************************************
*/
void List_Parse(_Listptr  ptr)
{
		_Listptr  jumpq;//跳转指针，用于if语句和调用子程序
		OS_ERR  err;
    u8      procerr;  
	
		InitUltrasnio(&ult);
	  Init_Paint(&paint);
	  InitMotor(&motor);
	  InitServo(&servo);
	  InitLed(&led);
	  InitCar(&car);
	  InitVar(&var);
	  InitMusic(&music);
	  InitPort(&port);
	  //将4个端口初始化为输入
	  port_1.dir = PORT_IN;
	  port_2.dir = PORT_IN;
	  port_3.dir = PORT_IN;
	  port_4.dir = PORT_IN;
	
	  
		while(ptr)
		{

			switch ( ptr->_flag )
			{
				case FLAG_MOTOR_C:   //电机_正转,速度_  
								motor.id =( ptr->EditContent )[6] - 0x30;
								motor.direction = FORWARD;
								motor.speed = atoi(ptr->EditContent + 20);
								MOTOR_Config(&motor);
					break;
				case FLAG_MOTOR_CC: //电机_反转,速度_
								motor.id = ( ptr->EditContent )[6] - 0x30;
								motor.direction = BACKWARD;
								motor.speed = atoi(ptr->EditContent + 20);
								MOTOR_Config(&motor);
					break;
				case FLAG_SERVO:  //舵机_转_
							servo.id = ( ptr->EditContent )[6] - 0x30;
							servo.degree = atoi(ptr->EditContent + 10);
							SERVO_Config(&servo);
					break;
				case FLAG_LED_ON:  //LED_
							led.id = ( ptr->EditContent )[3] - 0x30;
							led.status = ON;
							LED_Config(&led );
					break;
				case FLAG_LED_OFF:  //LED_
							led.id = ( ptr->EditContent )[3] - 0x30;
							led.status = OFF;
							LED_Config(&led );
					break;
				case FLAG_CAR_LEFT:
							car.direction = LEFT;
				      car.speed_step = 0;
							Car_Left();
					break;
				case FLAG_CAR_RIGHT:
					    car.direction = RIGHT;
							car.speed_step = 0;
							Car_Right();
					break;
				case FLAG_CAR_FORWARD:
							car.direction = FORWARD;
							car.speed_step = 0;
							Car_Forward();
					break;
				case FLAG_CAR_BACKWARD:
							car.direction = BACKWARD;
							car.speed_step = 0;
					    Car_Backward();
					break;
				case FLAG_CAR_STOP:
							car.direction = STOP;
							car.speed_step = 0;
					    Car_Stop();
					break;
				case FLAG_CAR_ACCEL:
					   car.speed_step = SPEED_STEP;
						 CAR_Config(&car);
					break;
				case FLAG_CAR_SLOW:
					   car.speed_step = -SPEED_STEP;
				     CAR_Config(&car);
					break;
				//遇到流程控制语句if\or\while等，先判断是否符合判断条件，符合条件的话直接进入下一个结点
				case FLAG_PORT_SIGNAL://如果端口_有信号
							port.id = (ptr->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if(port.cur_val == SIGNAL){
								//如果条件成立，则进入if分支
								ptr = if_branch(ptr);                    
							}else{
								//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
								jumpq = Find_Node(ptr, FLAG_OR);
								if(!jumpq)
									jumpq = Find_Node(ptr, FLAG_IF_END);
								
								ptr = jumpq;//将这个结点赋给ptr
							}
					break;
				case FLAG_PORT_NOSIGNAL://如果端口_无信号
							port.id = (ptr->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if(port.cur_val == NOSIGNAL){
								//如果条件成立，则进入if分支
								ptr = if_branch(ptr);                    
							}else {
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给ptr
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
							port.id = (ptr->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
							port.id = (ptr->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //如果端口_>_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							Detect_Port(&port);
							if(port.cur_val > port.tar_val ){
								//如果条件成立，则进入if分支
								ptr = if_branch(ptr);                    
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给ptr
							}
					break;
				case FLAG_PORT_LITTLER: //如果端口_<_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							Detect_Port(&port);
							if(port.cur_val < port.tar_val ){
								//如果条件成立，则进入if分支
								ptr = if_branch(ptr);                    
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给ptr
							}
					break;
				case FLAG_OBSTRACLE_GREATER:
					   	ult.tar_distance = atoi(ptr->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance ){
									//如果条件成立，则进入if分支
									ptr = if_branch(ptr);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_OBSTRACLE_LITTER:
					    ult.tar_distance = atoi(ptr->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance ){
									//如果条件成立，则进入if分支
									ptr = if_branch(ptr);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_HACCEL_GREATOR:
							if(euler.accel_y * G_CONST > atof(ptr->EditContent + 22) ){
									//如果条件成立，则进入if分支
									ptr = if_branch(ptr);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_HACCEL_LITTER:
							if(euler.accel_y * G_CONST > atof(ptr->EditContent + 22) ){
									//如果条件成立，则进入if分支
									ptr = if_branch(ptr);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_VACCEL_GREATOR:
							if(euler.accel_z * G_CONST > atof(ptr->EditContent + 22) ){
									//如果条件成立，则进入if分支
									ptr = if_branch(ptr);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_VACCEL_LITTER:
							if( euler.accel_z * G_CONST < atof(ptr->EditContent + 22) ){
									//如果条件成立，则进入if分支
									ptr = if_branch(ptr);                      //嵌套
							}else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给p
							}
					break;
				case FLAG_VAR_SET_A: //设定A=
							var.id = VAR_A;
							var.set_val = atoi(ptr->EditContent + 8);
					break;
				case FLAG_VAR_SET_B: //设定B=
							var.id = VAR_B;
							var.set_val = atoi(ptr->EditContent + 8);
					break;
  			case FLAG_VAR_SET_A_PORT: //A=端口_
							var.id = VAR_A;
							switch( atoi(ptr->EditContent + 8)){
								case PORT_1:
									   var.set_val = port_1.cur_val;
									break;
								case PORT_2:
									   var.set_val = port_2.cur_val;
									break;
								case PORT_3:
									   var.set_val = port_3.cur_val;
									break;
								case PORT_4:
									   var.set_val = port_4.cur_val;
									break;
								
							}
					break;
				case FLAG_VAR_SET_B_PORT: //B=端口_
							var.id = VAR_B;
							switch( atoi(ptr->EditContent + 8)){
								case PORT_1:
									   var.set_val = port_1.cur_val;
									break;
								case PORT_2:
									   var.set_val = port_2.cur_val;
									break;
								case PORT_3:
									   var.set_val = port_3.cur_val;
									break;
								case PORT_4:
									   var.set_val = port_4.cur_val;
									break;
								
							}
					break;
				case FLAG_VAR_A_INC:
								var.id = VAR_A;
								var.set_val ++;
							
					break;
				case FLAG_VAR_A_DEC:
							var.id = VAR_A;
							var.set_val --;
					break;
				case FLAG_VAR_B_INC:
							var.id = VAR_B;
							var.set_val ++;
						
					break;
				case FLAG_VAR_B_DEC:
							var.id = VAR_B;
							var.set_val --;
					break;
				case FLAG_VAR_SHOW_A:
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_SHOW_B:
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_A_GREATER: //如果A>_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val > var.tar_val )
							{
									//如果条件成立，则进入if分支
									ptr = if_branch(ptr);                    
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给ptr
							}
					break;
				case FLAG_VAR_A_LITTLER: //如果A<_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val < var.tar_val )
							{
								//如果条件成立，则进入if分支
								ptr = if_branch(ptr);                    
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//将这个结点赋给ptr
							}
					break;
				case FLAG_WHILE_HEAD:
								ptr = while_branch(ptr);
					break;
				case FLAG_WHILE_TAIL:
					   //
						 //do nothing...
						 //
					break;
				case FLAG_END_PROGRAM:
							ptr ->next = (void *)0;
					break;
				case FLAG_OR:
              //其实不可能直接在主链表里执行到OR语句,这是语法错误
							ptr = or_branch(ptr); //返回的p指向“条件结束”指令
					break;
				case FLAG_IF_END:
							//
							//do nothing...
							//
					break;
				case FLAG_PROC:
				     jumpq = ptr->next ;//保存断点
//				     ptr->next = (_Listptr)0;//断开主链表
				     procerr = OpenSubPro(atoi(ptr->EditContent + 15), ptr);//PC指针跳转
				     if(procerr == NO_ERROR)
						 {
							  ptr = proc_branch(ptr);//执行子程序
							  ptr->next = jumpq;  //返回断点地址
						 }
						 else{
						       ptr->next = jumpq;  //返回断点地址
						 }
					break;
				case FLAG_FOR_HEAD://For循环语句处理
							if( atoi(ptr->EditContent + 9) != 0){
									int8_t c = atoi(ptr->EditContent + 9);
									ptr = for_branch(ptr, c);
							}
					break;
				case FLAG_FOR_TAIL:
					//
					//do nothing...
				  //
					break;
				case FLAG_DELAY_NMS:
							delay_time = atoi(ptr->EditContent + 6);
							OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
					break;
				case FLAG_MUSIC:
					    music.tones = atoi(ptr->EditContent + 6);
							music.time = atoi(ptr->EditContent + 13);
							Music_Config(&music);
					break;
				case FLAG_SHOW_DISTANCE:
					   display = DISP_DISTANCE;
					break;
				case FLAG_GIF_HAPPY:
					   gif = GIF_HAPPY;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_SAD:
					   gif = GIF_SAD;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_CRY:
					   gif = GIF_CRY;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_FURY:
					   gif = GIF_FURY;
				     display = DISP_GIF;
					break;
				case FLAG_GIF_ALARM:
					   gif = GIF_ALARM;
				     display = DISP_GIF;
					break;
				case FLAG_DRAW_HCIRCLE:
					   paint.species = HOLLOW_CIRCLE;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_SCIRCLE:
					   paint.species = SOLID_CIRCLE;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_HRECT:
					   paint.species = HOLLOW_RECT;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_SRECT:
					   paint.species = SOLID_RECT;
				     display = DISP_PAINT;
					break;
				case FLAG_DRAW_LINE:
					   paint.species = STRIGHT_LINE;
				     display = DISP_PAINT;
					break;
				case FLAG_SET_X1:
					   paint.x1 = atoi(ptr->EditContent + 15);
					break;
				case FLAG_SET_X2:
					   paint.x2 = atoi(ptr->EditContent + 15);
					break;
				case FLAG_SET_Y1:
					   paint.y1 = atoi(ptr->EditContent + 15);
					break;
				case FLAG_SET_Y2:
					   paint.y2 = atoi(ptr->EditContent + 15);
					break;
				case FLAG_SET_RADIUS:
					   paint.radius = atoi(ptr->EditContent + 6);
					break;
				case FLAG_COLOR:
//					   paint.color = atoi(ptr->EditContent + 6);
				     COLORMAP(atoi(ptr->EditContent + 6), paint.color)
					break;
				default:break;
			}
			ptr = ptr -> next ;
		}
		
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

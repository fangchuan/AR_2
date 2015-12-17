#include "_apollorobot.h"
#include "string.h"
#include "bsp_adc.h"
#include "bsp_digitalsensor.h"
#include "bsp_ultrasnio.h"
#include "bsp_led.h"
#include "bsp_motor.h" 
#include "bsp_servo.h"
#include "os.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
_Listptr Ins_List_Head;//程序链表的头指针
_StatuStack StaStk;    //表示代码嵌套层次的状态栈 


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
_Motor    motor;
_Servo    servo;
_Led        led;
_Port      port;
_Ultrasnio  ult;
_Variable   var;

int32_t  delay_time;
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static _Listptr or_branch (_Listptr  p);
static _Listptr if_branch (_Listptr  p);
static _Listptr while_branch(_Listptr  p);
static _Error Detect_Port(_Port *port);
static _Error Get_Port  (_Port * port);

//
//侦测端口Port的状态：有无信号
//Tips:默认是用来判断数字传感器的
static _Error Detect_Port(_Port *port)
{
			if(port->id < 1 || port->id > 4)
					return  ERROR_ID;
			else
			{
				port->species = DS;
				DS_Config(port);//将此引脚配置为上拉输入
				switch(port->id )
					{
						case PORT_1:
							   
									if( !Detect_DS1 )
									{
										  port->status = SIGNAL;
									}
									else
									{
										  port->status = NOSIGNAL;
									}
//									if(	Get_adc(ANOLOG_Sensor_1) != NOSIGNAL)
//									{
//										  port->species = AS;
//										  port->status = SIGNAL;
//									}
							break;
						case PORT_2:
									if( !Detect_DS2 )
									{
										  port->status = SIGNAL;
									}
									else
									{
										  port->status = NOSIGNAL;
									}
//									if(Get_adc(ANOLOG_Sensor_2) != NOSIGNAL)
//									{
//										  port->species = AS;
//										  port->status = SIGNAL;
//									}
              break;
						case PORT_3:
									if( !Detect_DS3 )
									{
										  port->status = SIGNAL;
									}
									else
									{
										  port->status = NOSIGNAL;
									}
//									if(Get_adc(ANOLOG_Sensor_3) != NOSIGNAL)
//									{
//										  port->species = AS;
//										  port->status = SIGNAL;
//									}
              break;
						case PORT_4:
									if( !Detect_DS4 )
									{
										  port->status = SIGNAL;
									}
									else
									{
										  port->status = NOSIGNAL;
									}
//									if(Get_adc(ANOLOG_Sensor_4) != NOSIGNAL)
//									{
//										  port->species = AS;
//										  port->status = SIGNAL;
//									}
             break;
					}
				return NO_ERROR;
			}
}
//
//获取指定port的值,默认指的是模拟传感器的值
//
static _Error Get_Port  (_Port * port)
{
			if(port->id < 1 || port->id > 4)
					return ERROR_ID;
			else{
//						DigitalSensor_Init();//每次有关于数字传感器的相关操作时，要先将这几个引脚重设为输入模式
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
//
//if 满足条件后面的代码块处理，支持多条语句，支持嵌套
//
static _Listptr if_branch (_Listptr  p)
{
		
		if(!p)
			return NULL;
		else{
			_Listptr  q;//指向or指令或IF_END指令
			OS_ERR  err;
			while(p->_flag != FLAG_IF_END && p->_flag != FLAG_OR && p)//if条件成立的代码块里不能包括or的情况
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
							Car_Left();
					break;
				case FLAG_CAR_RIGHT:
							Car_Right();
					break;
				case FLAG_CAR_FORWARD:
							Car_Forward();
					break;
				case FLAG_CAR_BACKWARD:
					    Car_Backward();
					break;
				case FLAG_CAR_STOP:
					    Car_Stop();
					break;
				case FLAG_PORT_SIGNAL://如果端口_有信号
							port.id = (p->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if( port.status == SIGNAL)
							{
								//如果条件成立，则进入if分支
								p = p -> next;
								p = if_branch(p);                      //嵌套
							}
							else{
								//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
								q = Find_Node(p->index ,FLAG_OR);
								if(!q)
									q = Find_Node(p->index ,FLAG_IF_END);
								
								p = q;//将这个结点赋给p
							}
					break;
				case FLAG_PORT_NOSIGNAL://如果端口_无信号
							port.id = (p->EditContent)[12] - 0x30;
				      Detect_Port(&port);
							if(port.status  == NOSIGNAL)
							{
								//如果条件成立，则进入if分支
								p = p -> next;
								p = if_branch(p);                      //嵌套
							}
							else{
								//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
								q = Find_Node(p->index ,FLAG_OR);
								if(!q)
									q = Find_Node(p->index ,FLAG_IF_END);
								
								p = q;//将这个结点赋给p的后继
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
							port.id = (p->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.status  != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
							port.id = (p->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.status  != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //如果端口_>_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
				      Get_Port(&port);
							if(port.cur_val > port.tar_val )
							{
									//如果条件成立，则进入if分支
									p = p -> next;
									p = if_branch(p);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p = q;//将这个结点赋给p的后继
							}
					break;
				case FLAG_PORT_LITTLER: //如果端口_<_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
				      Get_Port( &port );
							if(port.cur_val < port.tar_val )
							{
									//如果条件成立，则进入if分支
									p = p -> next;
									p = if_branch(p);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p = q;//将这个结点赋给p的后继
							}
					break;
				case FLAG_OBSTRACLE_GREATER:
					    ult.tar_distance = atoi(p->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance )
							{
									//如果条件成立，则进入if分支
									p = p -> next;
									p = if_branch(p);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p = q;//将这个结点赋给p的后继
							}
					break;
				case FLAG_OBSTRACLE_LITTER:
							ult.tar_distance = atoi(p->EditContent + 16);
							if(ult.cur_distance < ult.tar_distance )
							{
									//如果条件成立，则进入if分支
									p = p -> next;
									p = if_branch(p);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p = q;//将这个结点赋给p的后继
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
					
					break;
				case FLAG_VAR_SHOW_B:
					
					break;
				case FLAG_VAR_A_GREATER: //变量A>_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if( var.set_val > var.tar_val )
							{
									//如果条件成立，则进入if分支
									p = p -> next;
									p = if_branch(p);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p = q;//将这个结点赋给p的后继
							}
					break;
				case FLAG_VAR_A_LITTLER: //变量A<_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if(var.set_val < var.tar_val )
							{
									//如果条件成立，则进入if分支
									p = p -> next;
									p = if_branch(p);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p = q;//将这个结点赋给p的后继
							}
					break;
				case FLAG_WHILE_HEAD:
							p = p -> next;
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
				case FLAG_DELAY_NMS:
							delay_time = atoi(p->EditContent + 6);
							OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
					break;
				case FLAG_MUSIC:
					
					break;
				case FLAG_SHOW_DISTANCE:
					   ult.ifshow = SHOW_ON;
					break;
				default:break;
				}
				p = p -> next;
			}
			if(p->_flag == FLAG_OR)     //如果执行到OR语句了，则直接寻找IF_END语句，跳过OR语句的代码块
				p = Find_Node(p->index ,FLAG_IF_END);
			
			return p;
		}
}

//
//"否则"后面的代码块处理,支持多条语句，支持嵌套
//
static _Listptr or_branch (_Listptr  p)
{
	
			if(!p)
					return NULL;
			else
			{
				_Listptr    q;//用于指向IF_END指令结点或嵌套的if指令结点
				OS_ERR    err;
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
									Car_Left();
							break;
						case FLAG_CAR_RIGHT:
							    Car_Right();
							break;
						case FLAG_CAR_FORWARD:
							    Car_Forward();
							break;
						case FLAG_CAR_BACKWARD:
							    Car_Backward();
							break;
						case FLAG_CAR_STOP:
							    Car_Stop();
							break;
						case FLAG_PORT_SIGNAL://如果端口_有信号
									port.id = (p->EditContent)[12] - 0x30;
						      Detect_Port( &port );
									if(port.status == SIGNAL)
									{
										//如果条件成立，则进入if分支
										p = p -> next;
										p = if_branch(p);     //返回的p指向“条件结束”指令
									}
									else{
										//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
										q = Find_Node(p->index ,FLAG_OR);
										if(!q)
											q = Find_Node(p->index ,FLAG_IF_END);
										
										p = q;//将这个结点赋给p的后继
									}
							break;
						case FLAG_PORT_NOSIGNAL://如果端口_无信号
									port.id = (p->EditContent)[12] - 0x30;
						      Detect_Port(&port);
									if(port.status == NOSIGNAL)
									{
										//如果条件成立，则进入if分支
										p = p -> next;
										p = if_branch(p);        //返回的p指向“条件结束”指令
									}
									else{
										//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
										q = Find_Node(p->index ,FLAG_OR);
										if(!q)
											q = Find_Node(p->index ,FLAG_IF_END);
										
										p = q;//将这个结点赋给p的后继
									}
							break;
						case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
									port.id = (p->EditContent)[12] - 0x30;
						      do{
										  Detect_Port(&port);
									}while(port.status != SIGNAL);
							break;
						case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
									port.id = (p->EditContent)[12] - 0x30;
						      do{
										  Detect_Port(&port) ;
									}while(port.status != NOSIGNAL);
							break;
						case FLAG_PORT_GREATER: //如果端口_>_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
						      Get_Port(&port);
									if(port.cur_val > port.tar_val )
									{
											//如果条件成立，则进入if分支
											p = p -> next;
											p = if_branch(p);       //返回的p指向“条件结束”指令
									}
									else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p = q;//将这个结点赋给p的后继
									}
							break;
						case FLAG_PORT_LITTLER: //如果端口_<_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
						      Get_Port(&port);
									if(port.cur_val < port.tar_val )
									{
											//如果条件成立，则进入if分支
											p = p -> next;
											p = if_branch(p);       //返回的p指向“条件结束”指令
									}
									else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p = q;//将这个结点赋给p的后继
									}
							break;
						case FLAG_OBSTRACLE_GREATER:
									ult.tar_distance = atoi(p->EditContent + 16);
									if(ult.cur_distance > ult.tar_distance )
									{
											//如果条件成立，则进入if分支
											p = p -> next;
											p = if_branch(p);         //返回的p指向“条件结束”指令
									}
									else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p = q;//将这个结点赋给p
									}
							break;
						case FLAG_OBSTRACLE_LITTER:
							    ult.tar_distance = atoi(p->EditContent + 16);
									if(ult.cur_distance < ult.tar_distance )
									{
											//如果条件成立，则进入if分支
											p = p -> next;
											p = if_branch(p);      //返回的p指向“条件结束”指令
									}
									else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p = q;//将这个结点赋给p
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
							
							break;
						case FLAG_VAR_SHOW_B:
							
							break;
						case FLAG_VAR_A_GREATER: //变量A>_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val > var.tar_val )
									{
										//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
										p = p -> next;
										p = if_branch(p);  //返回的p指向“条件结束”指令
									}
									else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p = q;//将这个结点赋给p
									}
							break;
						case FLAG_VAR_A_LITTLER: //变量A<_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val < var.tar_val )
									{
										//如果条件成立，则进入if分支
										p = p -> next;
										p = if_branch(p);        //返回的p指向“条件结束”指令
									}
									else{
											//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p = q;//将这个结点赋给p
									}
							break;
						case FLAG_WHILE_HEAD:
									p = p -> next;
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
									
									p = or_branch(p);   //返回的p指向“条件结束”指令
							break;
//						case FLAG_IF_END:   //不包含if_end的结点，只处理"否则"指令里包含的代码块
//							break;
						case FLAG_DELAY_NMS:
									delay_time = atoi(p->EditContent + 6);
									OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
							break;
						case FLAG_MUSIC:
							
							break;
					  case FLAG_SHOW_DISTANCE:
									ult.ifshow = SHOW_ON;
							break;
						default:break;
					 }
					p = p -> next;
				}
			 return p;
			}
}
//
//"循环开始"后面的代码块处理.支持嵌套
//Tips:让链表执行到WHILE_TAIL结点再给链表指针p跳转也行
static _Listptr while_branch (_Listptr  p)
{
			if(!p)
				return NULL;
			else
			{
					int index_first = p->index ;//while循环里的第一句所在地址(索引)
					_Listptr        q_whiletail;//指向WHILE_END语句
					_Listptr 	                q;//用于指向IF_END指令结点或嵌套的if指令结点
					OS_ERR                  err;
				
					q_whiletail = Find_Node(index_first,FLAG_WHILE_TAIL);//找到WHILE_TAIL结点
					if(!q_whiletail)
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
														Car_Left();
												break;
											case FLAG_CAR_RIGHT:
												    Car_Right();
												break;
											case FLAG_CAR_FORWARD:
												    Car_Forward();
												break;
											case FLAG_CAR_BACKWARD:
												    Car_Backward();
												break;
											case FLAG_CAR_STOP:
												    Car_Stop();
												break;
											case FLAG_PORT_SIGNAL://如果端口_有信号
														port.id = (p->EditContent)[12] - 0x30;
														Detect_Port(&port);
														if(port.status == SIGNAL)
														{
															//如果条件成立，则进入if分支
															p = p -> next;
															p = if_branch(p);   //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p = q;//将这个结点赋给p的后继
														}
												break;
											case FLAG_PORT_NOSIGNAL://如果端口_无信号
														port.id = (p->EditContent)[12] - 0x30;
														Detect_Port(&port);
														if(port.status == NOSIGNAL)
														{
															//如果条件成立，则进入if分支
															p = p -> next;
															p = if_branch(p);      //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p = q;//将这个结点赋给p的后继
														}
												break;
											case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
														port.id = (p->EditContent)[12] - 0x30;
														do{
															  Detect_Port(&port);
														}while(port.status != SIGNAL);
												break;
											case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
														port.id = (p->EditContent)[12] - 0x30;
														do{
															  Detect_Port(&port);
														}while(port.status != NOSIGNAL);
												break;
											case FLAG_PORT_GREATER: //如果端口_>_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														Get_Port(&port);
														if(port.cur_val > port.tar_val )
														{
															//如果条件成立，则进入if分支
															p = p -> next;
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p = q;//将这个结点赋给p的后继
														}
												break;
											case FLAG_PORT_LITTLER: //如果端口_<_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														Get_Port(&port);
														if(port.cur_val < port.tar_val )
														{
															//如果条件成立，则进入if分支
															p = p -> next;
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p = q;//将这个结点赋给p的后继
														}
												break;
											case FLAG_OBSTRACLE_GREATER:
												    ult.tar_distance = atoi(p->EditContent + 16);
														if(ult.cur_distance > ult.tar_distance )
														{
																//如果条件成立，则进入if分支
																p = p -> next;
																p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																q = Find_Node(p->index ,FLAG_OR);
																if(!q)
																	q = Find_Node(p->index ,FLAG_IF_END);
																
																p = q;//将这个结点赋给p的后继
														}
												break;
											case FLAG_OBSTRACLE_LITTER:
												    ult.tar_distance = atoi(p->EditContent + 16);
														if(ult.cur_distance < ult.tar_distance )
														{
																//如果条件成立，则进入if分支
																p = p -> next;
																p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
																//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
																q = Find_Node(p->index ,FLAG_OR);
																if(!q)
																	q = Find_Node(p->index ,FLAG_IF_END);
																
																p = q;//将这个结点赋给p的后继
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
												
												break;
											case FLAG_VAR_SHOW_B:
												
												break;
											case FLAG_VAR_A_GREATER: //变量A>_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  > var.tar_val  )
														{
															//如果条件成立，则进入if分支
															p = p -> next;
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p = q;//将这个结点赋给p的后继
														}
												break;
											case FLAG_VAR_A_LITTLER: //变量A<_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  < var.tar_val  )
														{
															//如果条件成立，则进入if分支
															p = p -> next;
															p = if_branch(p);     //返回的p指向“条件结束”指令
														}
														else{
															//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p = q;//将这个结点赋给p的后继
														}
												break;
											case FLAG_WHILE_HEAD:
														p = p -> next;
														p = while_branch(p); //嵌套
												break;
											case FLAG_WHILE_TAIL://到了循环语句尾部就跳转至循环语句首部
														p = Find_Node(index_first -1,FLAG_WHILE_HEAD);//因为这里记录的Index_first是WHILE_HEAD指令的后一句，
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
											case FLAG_DELAY_NMS:
														delay_time = atoi(p->EditContent + 6);
														OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
												break;
											case FLAG_MUSIC:
												
												break;
											case FLAG_SHOW_DISTANCE:
														ult.ifshow = SHOW_ON;
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
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//建立一个有表头结点的单链表
int Create_List(void){
	
		Ins_List_Head = (_Listptr)malloc(sizeof(_Instructor));
		if( !Ins_List_Head)
			return -1;
		Ins_List_Head -> index = 0;
		Ins_List_Head->next = (void*)0;
		return 0;
	
}
//增加节点,将元素加到下标为index的地方。其实是依次将结点从表头结点接下来
int Add_Node(int index, uint8_t flag, char *content)
{
				int i = 0;
				_Listptr    q = (_Listptr)malloc(sizeof(_Instructor));
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 || !q)
					return -1;
				
				q -> index = index;
				strcpy(q -> EditContent, content);
				q -> _flag = (enum _FLAG)flag;
				
				while(p && i < index-1)
				{
					p = p ->next ;//此时p即为下标为index的结点的前驱节点
					i ++;
				}
				
				if(!p)
					return -1;
				else{
							q -> next = p->next ;//使前驱的后继成为新增结点的后继
							p->next  = q ;  //使新增结点的称为前驱的后继
				}
				
				return 0;
}
//替换下标为index的结点的内容
int  Replace_Node(int index, enum _FLAG flag,char *content)
{
				int i = 0;
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 )
					return -1;

				while(p && i < index)
				{
					p = p ->next ;//此时p即为下标为index的结点点
					i ++;
				}
				
				if(!p)
					return -1;
				else{
							p->_flag  = flag ;
							strcpy(p -> EditContent, content);
				}
				
				return 0;

}

//删除下标为Index的结点
int Delete_Node(int index)
{
		int i;
		_Listptr p = Ins_List_Head;
		_Listptr q;
	
		if(index <= 0)
			return -1;
		
		for(i = 0; i< index - 1;i++)
				p = p -> next;//此时p为index结点的前驱结点
		if(!p)
		{
			return -1;
		}
		else
		{
			q = p->next ;//q结点即为Index结点
			p -> next = q -> next;//使index结点的后继成为p的后继
			free(q);  //释放index结点的空间
			
			return 0;
		}
		
}
//获取当前链表的长度,输入参数为表头结点
int  GetListLength(void)
{
		_Listptr p = Ins_List_Head;
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
//清空整个链表，释放原来的结点空间
void Clear_List(void)
{
		_Listptr p = Ins_List_Head ;
		_Listptr q ;
		int      i = GetListLength() ;
	
		while(i--)
		{
				q = p -> next;
				p -> next = q -> next;
				free(q);
		}
		
}
//从index结点开始寻找标志为flag的结点,并返回该结点的指针
_Listptr  Find_Node(int index, enum _FLAG flag)
{
				int i ;
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 )
					return (_Listptr)0;
				
				while(p && i < index)
				{
					p = p ->next ;//此时p即为下标为index的结点
					i ++;
				}
				
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
//初始化一个状态栈
int Create_Stack(void)
{
		StaStk.base = (ELETYPE *)malloc(MAX_SIZE_STACK * sizeof(_StatuStack));
		if(!StaStk.base )
			return -1;
		else
		{
			StaStk.top = StaStk.base ;//当前栈为空栈
			StaStk.stacksize = MAX_SIZE_STACK;
			
			return 0;
		}
}

//返回当前栈的长度
int GetStackLength(_StatuStack *stk)
{
		if(!stk->base)
			return -1;
		else
		{
			return (stk->top - stk->base );
		}
}
//获取栈顶元素，存入ele，不是出栈
int GetTop(_StatuStack *Stk,uint8_t *ele)
{
		if(!Stk->base || Stk->base == Stk->top )//若是空栈则返回-1
			return -1;
		else
		{
			*ele = *(Stk->top -1);
			return 0;
		}
}
//元素入栈操作
int Push(_StatuStack *Stk, uint8_t ele)
{
		if(!Stk->base )
			return -1;
		else{
			*Stk->top++ = ele;
			return 0;
		}
}
//元素出栈
int Pop(_StatuStack *Stk, uint8_t *ele)
{
		if(!Stk->base|| Stk->base == Stk->top )//若是空栈则返回-1
			return -1;
		else{
			*ele = *(--Stk->top);
			return 0;
		}
}
//
//链表解析函数
//
void List_Parse(_Listptr  ptr)
{
		_Listptr  q;//用于if语句
		OS_ERR  err;
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
							Car_Left();
					break;
				case FLAG_CAR_RIGHT:
					    Car_Right();
					break;
				case FLAG_CAR_FORWARD:
					    Car_Forward();
					break;
				case FLAG_CAR_BACKWARD:
					    Car_Backward();
					break;
				case FLAG_CAR_STOP:
					    Car_Stop();
					break;
				//遇到流程控制语句if\or\while等，先判断是否符合判断条件，符合条件的话直接进入下一个结点
				case FLAG_PORT_SIGNAL://如果端口_有信号
							port.id = (ptr->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if(port.status == SIGNAL)
							{
								//如果条件成立，则进入if分支
								ptr = ptr -> next;
								ptr = if_branch(ptr);                    
							}
							else{
								//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
								q = Find_Node(ptr->index ,FLAG_OR);
								if(!q)
									q = Find_Node(ptr->index ,FLAG_IF_END);
								
								ptr = q;//将这个结点赋给ptr
							}
					break;
				case FLAG_PORT_NOSIGNAL://如果端口_无信号
							port.id = (ptr->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if(port.status == NOSIGNAL)
							{
								//如果条件成立，则进入if分支
								ptr = ptr -> next;
								ptr = if_branch(ptr);                    
							}
							else {
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr = q;//将这个结点赋给ptr
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
							port.id = (ptr->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.status != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
							port.id = (ptr->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.status != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //如果端口_>_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							Get_Port(&port);
							if(port.cur_val > port.tar_val )
							{
								//如果条件成立，则进入if分支
								ptr = ptr -> next;
								ptr = if_branch(ptr);                    
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr = q;//将这个结点赋给ptr
							}
					break;
				case FLAG_PORT_LITTLER: //如果端口_<_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							Get_Port(&port);
							if(port.cur_val < port.tar_val )
							{
								//如果条件成立，则进入if分支
								ptr = ptr -> next;
								ptr = if_branch(ptr);                    
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr = q;//将这个结点赋给ptr
							}
					break;
				case FLAG_OBSTRACLE_GREATER:
					   	ult.tar_distance = atoi(ptr->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance )
							{
									//如果条件成立，则进入if分支
									ptr = ptr -> next;
									ptr = if_branch(ptr);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr = q;//将这个结点赋给p
							}
					break;
				case FLAG_OBSTRACLE_LITTER:
					    ult.tar_distance = atoi(ptr->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance )
							{
									//如果条件成立，则进入if分支
									ptr = ptr -> next;
									ptr = if_branch(ptr);                      //嵌套
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr = q;//将这个结点赋给p
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
					
					break;
				case FLAG_VAR_SHOW_B:
					
					break;
				case FLAG_VAR_A_GREATER: //如果A>_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val > var.tar_val )
							{
									//如果条件成立，则进入if分支
									ptr = ptr -> next;
									ptr = if_branch(ptr);                    
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr  = q;//将这个结点赋给ptr
							}
					break;
				case FLAG_VAR_A_LITTLER: //如果A<_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val < var.tar_val )
							{
								//如果条件成立，则进入if分支
								ptr = ptr -> next;
								ptr = if_branch(ptr);                    
							}
							else{
									//如果条件不成立，则寻找OR指令，没有OR指令就寻找IF_END指令
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr = q;//将这个结点赋给ptr
							}
					break;
				case FLAG_WHILE_HEAD:
								ptr = ptr ->next ;
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

							ptr = or_branch(ptr); //返回的p指向“条件结束”指令
					break;
				case FLAG_IF_END:
							//
							//do nothing...
							//
					break;
				case FLAG_DELAY_NMS:
							delay_time = atoi(ptr->EditContent + 6);
							OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
					break;
				case FLAG_MUSIC:
					
					break;
				case FLAG_SHOW_DISTANCE:
					   ult.ifshow = SHOW_ON;
					break;
				default:break;
			}
			ptr = ptr -> next ;
		}
		
}

/*******************************************End of File***********************************************/
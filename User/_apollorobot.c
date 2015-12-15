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
_Listptr Ins_List_Head;//���������ͷָ��
_StatuStack StaStk;    //��ʾ����Ƕ�ײ�ε�״̬ջ 


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
_Variable   var;

int32_t  delay_time;
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static int or_branch (_Listptr  p);
static int if_branch (_Listptr  p);
static int while_branch(_Listptr  p);
static uint8_t Detect_Port(uint8_t port);
static uint16_t Get_Port  (uint8_t port);

//
//���˿�Port��״̬�������ź�
//
static uint8_t Detect_Port(uint8_t port)
{
			if(port < 1 || port > 4)
					return  NOSIGNAL;
			else
			{
				switch(port)
					{
						case PORT_1:
									if(Detect_DS1 != NOSIGNAL || Get_adc(ANOLOG_Sensor_1) != NOSIGNAL)
										return SIGNAL;
									else
										return NOSIGNAL;
						case PORT_2:
									if(Detect_DS2 != NOSIGNAL || Get_adc(ANOLOG_Sensor_2) != NOSIGNAL)
										return SIGNAL;
									else
										return NOSIGNAL;
						case PORT_3:
									if(Detect_DS3 != NOSIGNAL || Get_adc(ANOLOG_Sensor_3) != NOSIGNAL)
										return SIGNAL;
									else
										return NOSIGNAL;
						case PORT_4:
									if(Detect_DS4 != NOSIGNAL || Get_adc(ANOLOG_Sensor_4) != NOSIGNAL)
										return SIGNAL;
									else
										return NOSIGNAL;
					}
			}
}
//
//��ȡָ��port��ֵ,Ĭ��ָ����ģ�⴫������ֵ
//
static uint16_t Get_Port  (uint8_t port)
{
			if(port < 1 || port > 4)
					return 0;
			else{
						uint16_t temp;
						switch(port)
						{
							case PORT_1:
										temp = Get_adc(ANOLOG_Sensor_1);
								break;
							case PORT_2:
										temp = Get_adc(ANOLOG_Sensor_2);
								break;
							case PORT_3:
										temp = Get_adc(ANOLOG_Sensor_3);
								break;
							case PORT_4:
										temp = Get_adc(ANOLOG_Sensor_4);
								break;
						}
				return temp;
			}
}
//
//if ������������Ĵ���鴦��֧�ֶ�����䣬֧��Ƕ��
//
static int if_branch (_Listptr  p)
{
		
		if(!p)
			return -1;
		else{
			_Listptr  q;//ָ��orָ���IF_ENDָ��
			OS_ERR  err;
			while(p->_flag != FLAG_IF_END && p->_flag != FLAG_OR && p)//if���������Ĵ�����ﲻ�ܰ���or�����
			{
				switch ( p ->_flag )
					{
				case FLAG_MOTOR_C:   //���_��ת,�ٶ�_  
								motor.id =( p->EditContent )[6] - 0x30;
								motor.direction = FORWARD;
								motor.speed = atoi(p->EditContent + 20);
								MOTOR_Config(&motor);
					break;
				case FLAG_MOTOR_CC: //���_��ת,�ٶ�_
								motor.id = ( p->EditContent )[6] - 0x30;
								motor.direction = BACKWARD;
								motor.speed = atoi(p->EditContent + 20);
				        MOTOR_Config(&motor);
					break;
				case FLAG_SERVO:  //���_ת_
							servo.id = ( p->EditContent )[6] - 0x30;
							servo.degree = atoi(p->EditContent + 10);
				      SERVO_Config(&servo);
					break;
				case FLAG_LED_ON:  //LED_��
							led.id = ( p->EditContent )[3] - 0x30;
							led.status = ON;
							LED_Config(&led);
					break;
				case FLAG_LED_OFF:  //LED_�ر�
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
				case FLAG_PORT_SIGNAL://����˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
							if(Detect_Port(port.id) == SIGNAL)
							{
								//������������������if��֧
								p = p -> next;
								if_branch(p);                      //Ƕ��
							}
							else{
								//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
								q = Find_Node(p->index ,FLAG_OR);
								if(!q)
									q = Find_Node(p->index ,FLAG_IF_END);
								
								p -> next = q;//�������㸳��p�ĺ��
							}
					break;
				case FLAG_PORT_NOSIGNAL://����˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
							if(Detect_Port(port.id) == NOSIGNAL)
							{
								//������������������if��֧
								p = p -> next;
								if_branch(p);                      //Ƕ��
							}
							else{
								//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
								q = Find_Node(p->index ,FLAG_OR);
								if(!q)
									q = Find_Node(p->index ,FLAG_IF_END);
								
								p -> next = q;//�������㸳��p�ĺ��
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
							while(Detect_Port(port.id) != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
							while(Detect_Port(port.id) != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //����˿�_>_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
							if(Get_Port(port.id) > port.tar_val )
							{
									//������������������if��֧
									p = p -> next;
									if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p -> next = q;//�������㸳��p�ĺ��
							}
					break;
				case FLAG_PORT_LITTLER: //����˿�_<_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
							if(Get_Port(port.id) < port.tar_val )
							{
									//������������������if��֧
									p = p -> next;
									if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p -> next = q;//�������㸳��p�ĺ��
							}
					break;
				case FLAG_OBSTRACLE_GREATER:
					break;
				case FLAG_OBSTRACLE_LITTER:
					break;
				case FLAG_VAR_SET_A: //�趨A=
							var.id = VAR_A;
							var.set_val = atoi(p->EditContent + 8);
					break;
				case FLAG_VAR_SET_B: //�趨B=
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
				case FLAG_VAR_A_GREATER: //����A>_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if( var.set_val > var.tar_val )
							{
									//������������������if��֧
									p = p -> next;
									if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p -> next = q;//�������㸳��p�ĺ��
							}
					break;
				case FLAG_VAR_A_LITTLER: //����A<_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if(var.set_val < var.tar_val )
							{
									//������������������if��֧
									p = p -> next;
									if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(p->index ,FLAG_OR);
									if(!q)
										q = Find_Node(p->index ,FLAG_IF_END);
									
									p -> next = q;//�������㸳��p�ĺ��
							}
					break;
				case FLAG_WHILE_HEAD:
							p = p -> next;
							while_branch(p);
					break;
				case FLAG_WHILE_TAIL:
							
							//
							//�˴�Ӧ�ñ�������
							//
							return -1;
				case FLAG_END_PROGRAM:
							p ->next = (void *)0;//����һ�������Ч���Ͽ�����
					break;
//				case FLAG_OR:
//					break;
//				case FLAG_IF_END:				  //if�Ĵ�����ﲻ�������������!!!
//					break;
				case FLAG_DELAY_NMS:
							delay_time = atoi(p->EditContent + 6);
							OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
					break;
				case FLAG_MUSIC:
					
					break;
				default:break;
				}
				p = p -> next;
			}
			return 0;
		}
}

//
//"����"����Ĵ���鴦��,֧�ֶ�����䣬֧��Ƕ��
//
static int or_branch (_Listptr  p)
{
	
			if(!p)
					return -1;
			else
			{
				_Listptr    q;//����ָ��IF_ENDָ�����Ƕ�׵�ifָ����
				OS_ERR    err;
				while(p->_flag != FLAG_IF_END && p)
				{
					switch ( p ->_flag )
					{
						case FLAG_MOTOR_C:   //���_��ת,�ٶ�_  
										motor.id =( p->EditContent )[6] - 0x30;
										motor.direction = FORWARD;
										motor.speed = atoi(p->EditContent + 20);
										MOTOR_Config(&motor);
							break;
						case FLAG_MOTOR_CC: //���_��ת,�ٶ�_
										motor.id = ( p->EditContent )[6] - 0x30;
										motor.direction = BACKWARD;
										motor.speed = atoi(p->EditContent + 20);
										MOTOR_Config(&motor);
							break;
						case FLAG_SERVO:  //���_ת_
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
						case FLAG_PORT_SIGNAL://����˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
									if(Detect_Port(port.id) == SIGNAL)
									{
										//������������������if��֧
										p = p -> next;
										if_branch(p);                      //Ƕ��
									}
									else{
										//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
										q = Find_Node(p->index ,FLAG_OR);
										if(!q)
											q = Find_Node(p->index ,FLAG_IF_END);
										
										p -> next = q;//�������㸳��p�ĺ��
									}
							break;
						case FLAG_PORT_NOSIGNAL://����˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
									if(Detect_Port(port.id) == NOSIGNAL)
									{
										//������������������if��֧
										p = p -> next;
										if_branch(p);                      //Ƕ��
									}
									else{
										//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
										q = Find_Node(p->index ,FLAG_OR);
										if(!q)
											q = Find_Node(p->index ,FLAG_IF_END);
										
										p -> next = q;//�������㸳��p�ĺ��
									}
							break;
						case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
									while(Detect_Port(port.id) != SIGNAL);
							break;
						case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
									while(Detect_Port(port.id) != NOSIGNAL);
							break;
						case FLAG_PORT_GREATER: //����˿�_>_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
									if(Get_Port(port.id) > port.tar_val )
									{
											//������������������if��֧
											p = p -> next;
											if_branch(p);                      //Ƕ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p -> next = q;//�������㸳��p�ĺ��
									}
							break;
						case FLAG_PORT_LITTLER: //����˿�_<_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
									if(Get_Port(port.id) < port.tar_val )
									{
											//������������������if��֧
											p = p -> next;
											if_branch(p);                      //Ƕ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p -> next = q;//�������㸳��p�ĺ��
									}
							break;
						case FLAG_OBSTRACLE_GREATER:
							break;
						case FLAG_OBSTRACLE_LITTER:
							break;
						case FLAG_VAR_SET_A: //�趨A=
									var.id = VAR_A;
									var.set_val = atoi(p->EditContent + 8);
							break;
						case FLAG_VAR_SET_B: //�趨B=
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
						case FLAG_VAR_A_GREATER: //����A>_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val > var.tar_val )
									{
										//������������������if��֧������ʲôҲ��ִ�У�ȡ��һ��ָ��
										p = p -> next;
										if_branch(p);
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p -> next = q;//�������㸳��p�ĺ��
									}
							break;
						case FLAG_VAR_A_LITTLER: //����A<_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val < var.tar_val )
									{
										//������������������if��֧
										p = p -> next;
										if_branch(p);                      //Ƕ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											q = Find_Node(p->index ,FLAG_OR);
											if(!q)
												q = Find_Node(p->index ,FLAG_IF_END);
											
											p -> next = q;//�������㸳��p�ĺ��
									}
							break;
						case FLAG_WHILE_HEAD:
									p = p -> next;
									while_branch(p);
							break;
						case FLAG_WHILE_TAIL:
									//
									//do nothing....
									//
									return -1;
						case FLAG_END_PROGRAM:
									p ->next = (void *)0;//����һ�������Ч���Ͽ�����
							break;
						case FLAG_OR:  //Ƕ�� "����" �����
									p = p -> next;
									or_branch(p);
							break;
//						case FLAG_IF_END:   //������if_end�Ľ�㣬ֻ����"����"ָ��������Ĵ����
//							break;
						case FLAG_DELAY_NMS:
									delay_time = atoi(p->EditContent + 6);
									OSTimeDly(delay_time,OS_OPT_TIME_DLY, &err);
							break;
						case FLAG_MUSIC:
							
							break;
						default:break;
					 }
					p = p -> next;
				}
			 return 0;
			}
}
//
//"ѭ����ʼ"����Ĵ���鴦��.֧��Ƕ��
//Tips:��ʵû��Ҫ����index_last��index_temp,������ִ�е�WHILE_TAIL����ٸ�����ָ��p��תҲ��
static int while_branch (_Listptr  p)
{
			if(!p)
				return -1;
			else
			{
					int index_first = p->index ;//whileѭ����ĵ�һ�����ڵ�ַ(����)
					_Listptr        q_whiletail;//ָ��WHILE_END���
					_Listptr 	                q;//����ָ��IF_ENDָ�����Ƕ�׵�ifָ����
					OS_ERR                  err;
				
					q_whiletail = Find_Node(index_first,FLAG_WHILE_TAIL);//�ҵ�WHILE_TAIL���
					if(!q_whiletail)
						return -1;
					else{

								while( p)
								{
										switch ( p ->_flag )
										{
											case FLAG_MOTOR_C:   //���_��ת,�ٶ�_  
															motor.id =( p->EditContent )[6] - 0x30;
															motor.direction = FORWARD;
															motor.speed = atoi(p->EditContent + 20);
															MOTOR_Config(&motor);
												break;
											case FLAG_MOTOR_CC: //���_��ת,�ٶ�_
															motor.id = ( p->EditContent )[6] - 0x30;
															motor.direction = BACKWARD;
															motor.speed = atoi(p->EditContent + 20);
															MOTOR_Config(&motor);
												break;
											case FLAG_SERVO:  //���_ת_
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
											case FLAG_PORT_SIGNAL://����˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														if(Detect_Port(port.id) == SIGNAL)
														{
															//������������������if��֧
															p = p -> next;
															if_branch(p);                      //Ƕ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p -> next = q;//�������㸳��p�ĺ��
														}
												break;
											case FLAG_PORT_NOSIGNAL://����˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														if(Detect_Port(port.id) == NOSIGNAL)
														{
															//������������������if��֧
															p = p -> next;
															if_branch(p);                      //Ƕ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p -> next = q;//�������㸳��p�ĺ��
														}
												break;
											case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														while(Detect_Port(port.id ) != SIGNAL);
												break;
											case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														while(Detect_Port(port.id ) != NOSIGNAL);
												break;
											case FLAG_PORT_GREATER: //����˿�_>_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														if(Get_Port(port.id) > port.tar_val )
														{
															//������������������if��֧
															p = p -> next;
															if_branch(p);                      //Ƕ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p -> next = q;//�������㸳��p�ĺ��
														}
												break;
											case FLAG_PORT_LITTLER: //����˿�_<_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														if(Get_Port(port.id) < port.tar_val )
														{
															//������������������if��֧
															p = p -> next;
															if_branch(p);                      //Ƕ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p -> next = q;//�������㸳��p�ĺ��
														}
												break;
											case FLAG_OBSTRACLE_GREATER:
												break;
											case FLAG_OBSTRACLE_LITTER:
												break;
											case FLAG_VAR_SET_A: //�趨A=
														var.id = VAR_A;
														var.set_val = atoi(p->EditContent + 8);
												break;
											case FLAG_VAR_SET_B: //�趨B=
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
											case FLAG_VAR_A_GREATER: //����A>_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  > var.tar_val  )
														{
															//������������������if��֧
															p = p -> next;
															if_branch(p);                      //Ƕ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p -> next = q;//�������㸳��p�ĺ��
														}
												break;
											case FLAG_VAR_A_LITTLER: //����A<_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  < var.tar_val  )
														{
															//������������������if��֧
															p = p -> next;
															if_branch(p);                      //Ƕ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															q = Find_Node(p->index ,FLAG_OR);
															if(!q)
																q = Find_Node(p->index ,FLAG_IF_END);
															
															p -> next = q;//�������㸳��p�ĺ��
														}
												break;
											case FLAG_WHILE_HEAD:
														p = p -> next;
														while_branch(p); //Ƕ��
												break;
											case FLAG_WHILE_TAIL://����ѭ�����β������ת��ѭ������ײ�
														p = Find_Node(index_first -1,FLAG_WHILE_HEAD);//��Ϊ�����¼��Index_first��WHILE_HEADָ��ĺ�һ�䣬
																																					//����index_first����һ����������WHILE_HEAD
												break;
											case FLAG_END_PROGRAM:
														p ->next = (void *)0;//����һ�������Ч���Ͽ�����
												break;
											case FLAG_OR:
												    p = p->next;
											      or_branch(p);
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
											default:break;
										 }
                                  
										p = p ->next ;//��ʱ��pָ��Ĳ���ѭ�������ĵ�һ��ָ��
										 
										OSTimeDly(10,OS_OPT_TIME_DLY, &err);//whileѭ�����������һ����ʱ���Է���ѭ������ʵ���������
								}
								return 0;
					}
		 }
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//����һ���б�ͷ���ĵ�����
int Create_List(void){
	
		Ins_List_Head = (_Listptr)malloc(sizeof(_Instructor));
		if( !Ins_List_Head)
			return -1;
		Ins_List_Head -> index = 0;
		Ins_List_Head->next = (void*)0;
		return 0;
	
}
//���ӽڵ�,��Ԫ�ؼӵ��±�Ϊindex�ĵط�����ʵ�����ν����ӱ�ͷ��������
int Add_Node(int index, uint8_t flag, char *content)
{
				int i = 0;
				_Listptr    q = (_Listptr)malloc(sizeof(_Instructor));
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 || !q)
					return -1;
				
				q -> index = index;
				strcpy(q -> EditContent, content);
				q -> _flag = flag;
				
				while(p && i < index-1)
				{
					p = p ->next ;//��ʱp��Ϊ�±�Ϊindex�Ľ���ǰ���ڵ�
					i ++;
				}
				
				if(!p)
					return -1;
				else{
							q -> next = p->next ;//ʹǰ���ĺ�̳�Ϊ�������ĺ��
							p->next  = q ;  //ʹ�������ĳ�Ϊǰ���ĺ��
				}
				
				return 0;
}
//�滻�±�Ϊindex�Ľ�������
int  Replace_Node(int index, enum _FLAG flag,char *content)
{
				int i = 0;
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 )
					return -1;

				while(p && i < index)
				{
					p = p ->next ;//��ʱp��Ϊ�±�Ϊindex�Ľ���
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

//ɾ���±�ΪIndex�Ľ��
int Delete_Node(int index)
{
		int i;
		_Listptr p = Ins_List_Head;
		_Listptr q;
	
		if(index <= 0)
			return -1;
		
		for(i = 0; i< index - 1;i++)
				p = p -> next;//��ʱpΪindex����ǰ�����
		if(!p)
		{
			return -1;
		}
		else
		{
			q = p->next ;//q��㼴ΪIndex���
			p -> next = q -> next;//ʹindex���ĺ�̳�Ϊp�ĺ��
			free(q);  //�ͷ�index���Ŀռ�
			
			return 0;
		}
		
}
//��ȡ��ǰ����ĳ���,�������Ϊ��ͷ���
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
//������������ͷ�ԭ���Ľ��ռ�
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
//��index��㿪ʼѰ�ұ�־Ϊflag�Ľ��,�����ظý���ָ��
_Listptr  Find_Node(int index, enum _FLAG flag)
{
				int i ;
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 )
					return (_Listptr)0;
				
				while(p && i < index)
				{
					p = p ->next ;//��ʱp��Ϊ�±�Ϊindex�Ľ��
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
//��ʼ��һ��״̬ջ
int Create_Stack(void)
{
		StaStk.base = (ELETYPE *)malloc(MAX_SIZE_STACK * sizeof(_StatuStack));
		if(!StaStk.base )
			return -1;
		else
		{
			StaStk.top = StaStk.base ;//��ǰջΪ��ջ
			StaStk.stacksize = MAX_SIZE_STACK;
			
			return 0;
		}
}

//���ص�ǰջ�ĳ���
int GetStackLength(_StatuStack *stk)
{
		if(!stk->base)
			return -1;
		else
		{
			return (stk->top - stk->base );
		}
}
//��ȡջ��Ԫ�أ�����ele�����ǳ�ջ
int GetTop(_StatuStack *Stk,uint8_t *ele)
{
		if(!Stk->base || Stk->base == Stk->top )//���ǿ�ջ�򷵻�-1
			return -1;
		else
		{
			*ele = *(Stk->top -1);
			return 0;
		}
}
//Ԫ����ջ����
int Push(_StatuStack *Stk, uint8_t ele)
{
		if(!Stk->base )
			return -1;
		else{
			*Stk->top++ = ele;
			return 0;
		}
}
//Ԫ�س�ջ
int Pop(_StatuStack *Stk, uint8_t *ele)
{
		if(!Stk->base|| Stk->base == Stk->top )//���ǿ�ջ�򷵻�-1
			return -1;
		else{
			*ele = *(--Stk->top);
			return 0;
		}
}
//
//�����������
//
void List_Parse(_Listptr  ptr)
{
		_Listptr  q;//����if���
		OS_ERR  err;
		while(ptr)
		{

			switch ( ptr->_flag )
			{
				case FLAG_MOTOR_C:   //���_��ת,�ٶ�_  
								motor.id =( ptr->EditContent )[6] - 0x30;
								motor.direction = FORWARD;
								motor.speed = atoi(ptr->EditContent + 20);
								MOTOR_Config(&motor);
					break;
				case FLAG_MOTOR_CC: //���_��ת,�ٶ�_
								motor.id = ( ptr->EditContent )[6] - 0x30;
								motor.direction = BACKWARD;
								motor.speed = atoi(ptr->EditContent + 20);
								MOTOR_Config(&motor);
					break;
				case FLAG_SERVO:  //���_ת_
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
				//�������̿������if\or\while�ȣ����ж��Ƿ�����ж����������������Ļ�ֱ�ӽ�����һ�����
				case FLAG_PORT_SIGNAL://����˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							if(Detect_Port(port.id) == SIGNAL)
							{
								//������������������if��֧
								ptr = ptr -> next;
								if_branch(ptr);                    
							}
							else{
								//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
								q = Find_Node(ptr->index ,FLAG_OR);
								if(!q)
									q = Find_Node(ptr->index ,FLAG_IF_END);
								
								ptr -> next = q;//�������㸳��ptr�ĺ��
							}
					break;
				case FLAG_PORT_NOSIGNAL://����˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							if(Detect_Port(port.id ) == NOSIGNAL)
							{
								//������������������if��֧
								ptr = ptr -> next;
								if_branch(ptr);                    
							}
							else {
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr -> next = q;//�������㸳��ptr�ĺ��
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							while(Detect_Port(port.id) != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							while(Detect_Port(port.id) != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //����˿�_>_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							if(Get_Port(port.id) > port.tar_val )
							{
								//������������������if��֧
								ptr = ptr -> next;
								if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr -> next = q;//�������㸳��ptr�ĺ��
							}
					break;
				case FLAG_PORT_LITTLER: //����˿�_<_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							if(Get_Port(port.id) < port.tar_val )
							{
								//������������������if��֧
								ptr = ptr -> next;
								if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr -> next = q;//�������㸳��ptr�ĺ��
							}
					break;
				case FLAG_OBSTRACLE_GREATER:
					break;
				case FLAG_OBSTRACLE_LITTER:
					break;
				case FLAG_VAR_SET_A: //�趨A=
							var.id = VAR_A;
							var.set_val = atoi(ptr->EditContent + 8);
					break;
				case FLAG_VAR_SET_B: //�趨B=
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
				case FLAG_VAR_A_GREATER: //���A>_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val > var.tar_val )
							{
									//������������������if��֧
									ptr = ptr -> next;
									if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr -> next = q;//�������㸳��ptr�ĺ��
							}
					break;
				case FLAG_VAR_A_LITTLER: //���A<_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val < var.tar_val )
							{
								//������������������if��֧
								ptr = ptr -> next;
								if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									q = Find_Node(ptr->index ,FLAG_OR);
									if(!q)
										q = Find_Node(ptr->index ,FLAG_IF_END);
									
									ptr -> next = q;//�������㸳��ptr�ĺ��
							}
					break;
				case FLAG_WHILE_HEAD:
								ptr = ptr ->next ;
								while_branch(ptr);
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
							ptr = ptr -> next;
							or_branch(ptr);
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
				default:break;
			}
			ptr = ptr -> next ;
		}
		
}

/*******************************************End of File***********************************************/
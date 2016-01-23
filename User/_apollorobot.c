#include "includes.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
_Listptr Ins_List_Head;//�����������ͷָ��
_Listptr Sub_List_Head;//�ӳ�������ͷָ��
//_StatuStack StaStk;    //��ʾ����Ƕ�ײ�ε�״̬ջ 
//_SensorFlag  sensorflag;//����������
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

static void InitUltrasnio(_Ultrasnio *sensor)
{
//	    sensor->cur_distance = 0;
	    sensor->tar_distance = 0;
}

static void InitMotor(_Motor *motor)
{
	    motor->id = 0;
	    motor->direction = 0;
	    motor->speed = 0;
}

static void InitServo(_Servo *servo)
{
      servo->id = 0;
	    servo->degree = 0;

}

static void InitPort(_Port *port)
{
	    port->id = 0;
	    port->dir = 0;
	    port->species = 0;
	    port->status = 0;
	    port->cur_val = 0;
	    port->tar_val = 0;
}	

static void InitLed(_Led * led)
{
	    led->id = 0;
	    led->status = 0;
}

static void InitVar(_Variable *var)
{
	     var->id = 0;
	     var->set_val = 0;
	     var->tar_val = 0;
}

static void InitCar (_Car * car)
{
	     car->direction = 0;
	     car->speed_step = 0;
}

static void InitMusic(_Music *music)
{
	     music->time = 0;
	     music->tones = 0;
}
//
//���˿�Port��״̬�������ź�
//Tips:Ĭ���������ж����ִ�������
_Error Detect_Port(_Port *port)
{
	    int  adc_val;
	
	    if(port->dir == PORT_OUT)   //�˿ڱ���Ϊ����״̬
				return ERROR_DIR;
			if(port->id < 1 || port->id > 4)
					return  ERROR_ID;
			else
			{
				DS_Config(port);//������������Ϊ��������
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

									if((adc_val = Get_adc(ANOLOG_Sensor_1)) > 16)  //ADCֵ����16��������ģ�⴫��������
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

									if((adc_val = Get_adc(ANOLOG_Sensor_2)) > 16)  //ADCֵ����16��������ģ�⴫��������
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
									if((adc_val = Get_adc(ANOLOG_Sensor_3)) > 16)  //ADCֵ����16��������ģ�⴫��������
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
									if((adc_val = Get_adc(ANOLOG_Sensor_4)) > 16)   //ADCֵ����16��������ģ�⴫��������
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
//
//��ȡָ��port��ֵ,Ĭ��ָ����ģ�⴫������ֵ
//
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
//
//���ݳ����������ӳ���
//Ҫ��Add_Node�п�������
static _Error OpenSubPro (uint32_t pn, _Listptr head)
{
	    static   int last_pname = -1;
      if(last_pname != pn)
			{	//ǰ�����ν��벻ͬ�ӳ���			
				char         openfile[50]={0};
				u8           ret;
				 _Listptr   p = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
		
					sprintf(openfile, "%s/%d", "0:", pn);
					f_open(&file, openfile, FA_READ | FA_OPEN_EXISTING);
					if(result != FR_OK)
					{
						_MessageBox("���򲻴���","����", &ret);
						return ERROR_NAME;
					}
					if(!p)
						return ERROR_MALLOC;
					else{
						   
								u16      NumBytesPerNode = sizeof(_Instructor);
						    //�ӳ���������գ����¶���
						    Clear_List(Sub_List_Head);
								do{
										result = f_read(&file, p, NumBytesPerNode, &bw);
										if(result != FR_OK)
											return ERROR_FOPERATION;
										Add_Node(Sub_List_Head , p->index , p->_flag ,p->EditContent );//���ӳ�����ӽ��ӳ�������
								}while(p->next); //����β��㴦��nextָ��Ϊ�գ���ʾ���һ����㣬������������ˡ�
								
								myfree(SRAMIN, p);
								
								last_pname = pn;
								
								head->next = Sub_List_Head->next ;//PCָ�������ӳ�������
								
								return NO_ERROR;
							}
					
				}
			else{
			      head->next = Sub_List_Head->next ;//PCָ�������ӳ�������
				    return NO_ERROR;
			}
}
//
//�����ӳ�������
//������ת����ָ�룬��FLAG_PROC
static _Listptr proc_branch(_Listptr p)
{
	     _Listptr  ret = p;
	     p = p -> next;
	     if(!p)
			 {
				 return NULL;
			 }
			 else{
				     _Listptr jumpq;
	           OS_ERR     err;
				     u8     procerr = 0;
						 while(p)
						 {
								switch ( p->_flag )
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
									//�������̿������if\or\while�ȣ����ж��Ƿ�����ж����������������Ļ�ֱ�ӽ�����һ�����
									case FLAG_PORT_SIGNAL://����˿�_���ź�
												port.id = (p->EditContent)[12] - 0x30;
												Detect_Port(&port);
												if(port.cur_val == SIGNAL)
												{
													//������������������if��֧
													p = if_branch(p);                    
												}
												else{
													//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
													jumpq = Find_Node(p, FLAG_OR);
													if(!jumpq)
														jumpq = Find_Node(p, FLAG_IF_END);
													
													p = jumpq;//�������㸳��ptr
												}
										break;
									case FLAG_PORT_NOSIGNAL://����˿�_���ź�
												port.id = (p->EditContent)[12] - 0x30;
												Detect_Port(&port);
												if(port.cur_val == NOSIGNAL)
												{
													//������������������if��֧
													p = if_branch(p);                    
												}
												else {
														//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//�������㸳��ptr
												}
										break;
									case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
												port.id = (p->EditContent)[12] - 0x30;
												do{
														Detect_Port(&port);
												}while(port.cur_val != SIGNAL);
										break;
									case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
												port.id = (p->EditContent)[12] - 0x30;
												do{
														Detect_Port(&port);
												}while(port.cur_val != NOSIGNAL);
										break;
									case FLAG_PORT_GREATER: //����˿�_>_
												port.id = (p->EditContent)[12] - 0x30;
												port.tar_val = atoi(p->EditContent + 14);
												Detect_Port(&port);
												if(port.cur_val > port.tar_val )
												{
													//������������������if��֧
													p = if_branch(p);                    
												}
												else{
														//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//�������㸳��ptr
												}
										break;
									case FLAG_PORT_LITTLER: //����˿�_<_
												port.id = (p->EditContent)[12] - 0x30;
												port.tar_val = atoi(p->EditContent + 14);
												Detect_Port(&port);
												if(port.cur_val < port.tar_val )
												{
													//������������������if��֧
													p = if_branch(p);                    
												}
												else{
														//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//�������㸳��ptr
												}
										break;
									case FLAG_OBSTRACLE_GREATER:
												ult.tar_distance = atoi(p->EditContent + 16);
												if(ult.cur_distance > ult.tar_distance )
												{
														//������������������if��֧
														p = if_branch(p);                      //Ƕ��
												}
												else{
														//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//�������㸳��p
												}
										break;
									case FLAG_OBSTRACLE_LITTER:
												ult.tar_distance = atoi(p->EditContent + 16);
												if(ult.cur_distance > ult.tar_distance )
												{
														//������������������if��֧
														p = if_branch(p);                      //Ƕ��
												}
												else{
														//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//�������㸳��p
												}
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
										    display = DISP_VARIABLE;
										break;
									case FLAG_VAR_SHOW_B:
										    display = DISP_VARIABLE;
										break;
									case FLAG_VAR_A_GREATER: //���A>_
												var.id = VAR_A;
												var.tar_val = atoi(p->EditContent + 8);
												if(var.set_val > var.tar_val )
												{
														//������������������if��֧
														p = if_branch(p);                    
												}
												else{
														//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//�������㸳��ptr
												}
										break;
									case FLAG_VAR_A_LITTLER: //���A<_
												var.id = VAR_A;
												var.tar_val = atoi(p->EditContent + 8);
												if(var.set_val < var.tar_val )
												{
													//������������������if��֧
													p = if_branch(p);                    
												}
												else{
														//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
														jumpq = Find_Node(p, FLAG_OR);
														if(!jumpq)
															jumpq = Find_Node(p, FLAG_IF_END);
														
														p = jumpq;//�������㸳��ptr
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
												//��ʵ������ֱ������������ִ�е�OR���,�����﷨����
												p = or_branch(p); //���ص�pָ������������ָ��
										break;
									case FLAG_IF_END:
												//
												//do nothing...
												//
										break;
									case FLAG_PROC:
											 jumpq = p->next ;//����ϵ�
//									     p->next = (_Listptr)0;
											 procerr = OpenSubPro(atoi(p->EditContent + 15), p);//����PCָ��
									     if(procerr == NO_ERROR)
											 {
												  p = proc_branch(p);//ִ���ӳ���
												  p->next = jumpq;  //���ضϵ��ַ
											 }
											 else
											 {
												  p->next = jumpq;//���ضϵ��ַ
											 }
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
//
//if ������������Ĵ���鴦��֧�ֶ�����䣬֧��Ƕ��
//����IF_ENDָ���ָ��
static _Listptr if_branch (_Listptr  p)
{
		p = p->next ;
		if(!p)
			return NULL;
		else{
			_Listptr  jumpq;//ָ��orָ���IF_ENDָ��
			OS_ERR  err;
			u8    procerr;
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
				case FLAG_PORT_SIGNAL://����˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if( port.cur_val == SIGNAL)
							{
								//������������������if��֧
								p = if_branch(p);                      //Ƕ��
							}
							else{
								//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
								jumpq = Find_Node(p, FLAG_OR);
								if(!jumpq)
									jumpq = Find_Node(p, FLAG_IF_END);
								
								p = jumpq;//�������㸳��p
							}
					break;
				case FLAG_PORT_NOSIGNAL://����˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
				      Detect_Port(&port);
							if(port.cur_val  == NOSIGNAL)
							{
								//������������������if��֧
								p = if_branch(p);                      //Ƕ��
							}
							else{
								//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
								jumpq = Find_Node(p, FLAG_OR);
								if(!jumpq)
									jumpq = Find_Node(p, FLAG_IF_END);
								
								p = jumpq;//�������㸳��p
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
							port.id = (p->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //����˿�_>_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
				      Detect_Port(&port);
							if(port.cur_val > port.tar_val )
							{
									//������������������if��֧
									p = if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//�������㸳��p
							}
					break;
				case FLAG_PORT_LITTLER: //����˿�_<_
							port.id = (p->EditContent)[12] - 0x30;
							port.tar_val = atoi(p->EditContent + 14);
				      Detect_Port( &port );
							if(port.cur_val < port.tar_val )
							{
									//������������������if��֧
									p = if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//�������㸳��p
							}
					break;
				case FLAG_OBSTRACLE_GREATER://�ϰ���������_
					    ult.tar_distance = atoi(p->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance )
							{
									//������������������if��֧
									p = if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//�������㸳��p
							}
					break;
				case FLAG_OBSTRACLE_LITTER://����ϰ������С��_
							ult.tar_distance = atoi(p->EditContent + 16);
							if(ult.cur_distance < ult.tar_distance )
							{
									//������������������if��֧
									p = if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//�������㸳��p
							}
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
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_SHOW_B:
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_A_GREATER: //����A>_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if( var.set_val > var.tar_val )
							{
									//������������������if��֧
									p = if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//�������㸳��p
							}
					break;
				case FLAG_VAR_A_LITTLER: //����A<_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 8);
							if(var.set_val < var.tar_val )
							{
									//������������������if��֧
									p = if_branch(p);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(p, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(p, FLAG_IF_END);
									
									p = jumpq;//�������㸳��p
							}
					break;
				case FLAG_WHILE_HEAD:
							p = while_branch(p);
					break;
				case FLAG_WHILE_TAIL:
							
							//
							//�˴�Ӧ�ñ�������
							//
							return NULL;
				case FLAG_END_PROGRAM:
							p ->next = (void *)0;//����һ�������Ч���Ͽ�����
					break;
//				case FLAG_OR:
//					break;
//				case FLAG_IF_END:				  //if�Ĵ�����ﲻ�������������!!!
//					break;
				case FLAG_PROC:
					   jumpq = p->next ;//����ϵ�
//				      p->next = (_Listptr)0;//�Ͽ�������
						 procerr = OpenSubPro(atoi(p->EditContent + 15), p);//PCָ����ת
						 if(procerr == NO_ERROR)
						 {
							  p = proc_branch(p);//ִ���ӳ���
	  					  p->next = jumpq;  //���ضϵ��ַ
						 }
						 else
						 {
							  p->next = jumpq;//���ضϵ��ַ
						 }
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
			if(p->_flag == FLAG_OR)     //���ִ�е�OR����ˣ���ֱ��Ѱ��IF_END��䣬����OR���Ĵ����
				p = Find_Node(p, FLAG_IF_END);
			
			return p;
		}
}

//
//"����"����Ĵ���鴦��,֧�ֶ�����䣬֧��Ƕ��
//
static _Listptr or_branch (_Listptr  p)
{
	    p = p -> next;
			if(!p)
					return NULL;
			else
			{
				_Listptr    jumpq;//����ָ��IF_ENDָ�����Ƕ�׵�ifָ����
				OS_ERR    err;
				u8       procerr;
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
						case FLAG_PORT_SIGNAL://����˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
						      Detect_Port( &port );
									if(port.cur_val == SIGNAL)
									{
										//������������������if��֧
										p = if_branch(p);     //���ص�pָ������������ָ��
									}
									else{
										//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
										jumpq = Find_Node(p, FLAG_OR);
										if(!jumpq)
											jumpq = Find_Node(p, FLAG_IF_END);
										
										p = jumpq;//�������㸳��p
									}
							break;
						case FLAG_PORT_NOSIGNAL://����˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
						      Detect_Port(&port);
									if(port.cur_val == NOSIGNAL)
									{
										//������������������if��֧
										p = if_branch(p);        //���ص�pָ������������ָ��
									}
									else{
										//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
										jumpq = Find_Node(p, FLAG_OR);
										if(!jumpq)
											jumpq = Find_Node(p, FLAG_IF_END);
										
										p = jumpq;//�������㸳��p
									}
							break;
						case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
						      do{
										  Detect_Port(&port);
									}while(port.cur_val != SIGNAL);
							break;
						case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
									port.id = (p->EditContent)[12] - 0x30;
						      do{
										  Detect_Port(&port) ;
									}while(port.cur_val != NOSIGNAL);
							break;
						case FLAG_PORT_GREATER: //����˿�_>_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
						      Detect_Port(&port);
									if(port.cur_val > port.tar_val )
									{
											//������������������if��֧
											p = if_branch(p);       //���ص�pָ������������ָ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//�������㸳��p
									}
							break;
						case FLAG_PORT_LITTLER: //����˿�_<_
									port.id = (p->EditContent)[12] - 0x30;
									port.tar_val = atoi(p->EditContent + 14);
						      Detect_Port(&port);
									if(port.cur_val < port.tar_val )
									{
											//������������������if��֧
											p = if_branch(p);       //���ص�pָ������������ָ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//�������㸳��p
									}
							break;
						case FLAG_OBSTRACLE_GREATER:
									ult.tar_distance = atoi(p->EditContent + 16);
									if(ult.cur_distance > ult.tar_distance )
									{
											//������������������if��֧
											p = if_branch(p);         //���ص�pָ������������ָ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//�������㸳��p
									}
							break;
						case FLAG_OBSTRACLE_LITTER:
							    ult.tar_distance = atoi(p->EditContent + 16);
									if(ult.cur_distance < ult.tar_distance )
									{
											//������������������if��֧
											p = if_branch(p);      //���ص�pָ������������ָ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//�������㸳��p
									}
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
							    display = DISP_VARIABLE;
							break;
						case FLAG_VAR_SHOW_B:
							    display = DISP_VARIABLE;
							break;
						case FLAG_VAR_A_GREATER: //����A>_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val > var.tar_val )
									{
										//������������������if��֧������ʲôҲ��ִ�У�ȡ��һ��ָ��
										p = if_branch(p);  //���ص�pָ������������ָ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//�������㸳��p
									}
							break;
						case FLAG_VAR_A_LITTLER: //����A<_
									var.id = VAR_A;
									var.tar_val = atoi(p->EditContent + 8);
									if(var.set_val < var.tar_val )
									{
										//������������������if��֧
										p = if_branch(p);        //���ص�pָ������������ָ��
									}
									else{
											//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
											jumpq = Find_Node(p, FLAG_OR);
											if(!jumpq)
												jumpq = Find_Node(p, FLAG_IF_END);
											
											p = jumpq;//�������㸳��p
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
									p ->next = (void *)0;//����һ�������Ч���Ͽ�����
							break;
						case FLAG_OR:  //Ƕ�� "����" �����
									//���orǰ��û��If���ͻ������Ƕ��!!!����ջ���
									p = or_branch(p);   //���ص�pָ������������ָ��
							break;
//						case FLAG_IF_END:   //������if_end�Ľ�㣬ֻ����"����"ָ��������Ĵ����
//							break;
						case FLAG_PROC:
							   jumpq = p->next ;//����ϵ�
//						     p->next = (_Listptr)0;//�Ͽ�������
								 procerr = OpenSubPro(atoi(p->EditContent + 15), p);//PCָ����ת
								 if(procerr == NO_ERROR)
								 {
										p = proc_branch(p);//ִ���ӳ���
										p->next = jumpq;  //���ضϵ��ַ
								 }
								 else
								 {
										p->next = jumpq;//���ضϵ��ַ
								 }
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
//
//"ѭ����ʼ"����Ĵ���鴦��.֧��Ƕ��
//Tips:������ִ�е�WHILE_TAIL����ٸ�����ָ��p��תҲ��
static _Listptr while_branch (_Listptr  p)
{
	    _Listptr  while_head = p;
	    p = p->next ;
			if(!p)
				return NULL;
			else
			{
					_Listptr        while_tail;//ָ��WHILE_END���
					_Listptr 	            jumpq;//����ָ��IF_ENDָ�����Ƕ�׵�ifָ����
					OS_ERR                  err;
				  u8                  procerr;
				
					while_tail = Find_Node(p, FLAG_WHILE_TAIL);//�ҵ�WHILE_TAIL���
					if(!while_tail)
						return NULL;
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
											case FLAG_PORT_SIGNAL://����˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														Detect_Port(&port);
														if(port.cur_val == SIGNAL)
														{
															//������������������if��֧
															p = if_branch(p);   //���ص�pָ������������ָ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//�������㸳��p
														}
												break;
											case FLAG_PORT_NOSIGNAL://����˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														Detect_Port(&port);
														if(port.cur_val == NOSIGNAL)
														{
															//������������������if��֧
															p = if_branch(p);      //���ص�pָ������������ָ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//�������㸳��p
														}
												break;
											case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														do{
															  Detect_Port(&port);
														}while(port.cur_val != SIGNAL);
												break;
											case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
														port.id = (p->EditContent)[12] - 0x30;
														do{
															  Detect_Port(&port);
														}while(port.cur_val != NOSIGNAL);
												break;
											case FLAG_PORT_GREATER: //����˿�_>_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														Detect_Port(&port);
														if(port.cur_val > port.tar_val )
														{
															//������������������if��֧
															p = if_branch(p);     //���ص�pָ������������ָ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//�������㸳��p
														}
												break;
											case FLAG_PORT_LITTLER: //����˿�_<_
														port.id = (p->EditContent)[12] - 0x30;
														port.tar_val = atoi(p->EditContent + 14);
														Detect_Port(&port);
														if(port.cur_val < port.tar_val )
														{
															//������������������if��֧
															p = if_branch(p);     //���ص�pָ������������ָ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//�������㸳��p
														}
												break;
											case FLAG_OBSTRACLE_GREATER:
												    ult.tar_distance = atoi(p->EditContent + 16);
														if(ult.cur_distance > ult.tar_distance )
														{
																//������������������if��֧
																p = if_branch(p);     //���ص�pָ������������ָ��
														}
														else{
																//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//�������㸳��p
														}
												break;
											case FLAG_OBSTRACLE_LITTER:
												    ult.tar_distance = atoi(p->EditContent + 16);
														if(ult.cur_distance < ult.tar_distance )
														{
																//������������������if��֧
																p = if_branch(p);     //���ص�pָ������������ָ��
														}
														else{
																//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
																jumpq = Find_Node(p, FLAG_OR);
																if(!jumpq)
																	jumpq = Find_Node(p, FLAG_IF_END);
																
																p = jumpq;//�������㸳��p
														}
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
												    display = DISP_VARIABLE;
												break;
											case FLAG_VAR_SHOW_B:
												    display = DISP_VARIABLE;
												break;
											case FLAG_VAR_A_GREATER: //����A>_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  > var.tar_val  )
														{
															//������������������if��֧
															p = if_branch(p);     //���ص�pָ������������ָ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//�������㸳��p
														}
												break;
											case FLAG_VAR_A_LITTLER: //����A<_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 8);
														if(var.set_val  < var.tar_val  )
														{
															//������������������if��֧
															p = if_branch(p);     //���ص�pָ������������ָ��
														}
														else{
															//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
															jumpq = Find_Node(p, FLAG_OR);
															if(!jumpq)
																jumpq = Find_Node(p, FLAG_IF_END);
															
															p = jumpq;//�������㸳��p
														}
												break;
											case FLAG_WHILE_HEAD:
														p = while_branch(p); //Ƕ��
												break;
											case FLAG_WHILE_TAIL://����ѭ�����β������ת��ѭ������ײ�
														p = while_head;//��Ϊ�����¼��Index_first��WHILE_HEADָ��ĺ�һ�䣬
																																					//����index_first����һ����������WHILE_HEAD
												break;
											case FLAG_END_PROGRAM:
														p ->next = (void *)0;//����һ�������Ч���Ͽ�����
												break;
											case FLAG_OR:
												    
											      p = or_branch(p);  //���ص�pָ������������ָ��
												break;
											case FLAG_IF_END:
														//
														//do nothing...
														//
												break;
											case FLAG_PROC:
												   jumpq = p->next ;//����ϵ�
//											     p->next = (_Listptr)0;//�Ͽ�������
								           procerr = OpenSubPro(atoi(p->EditContent + 15), p);//PCָ����ת
								           if(procerr == NO_ERROR)
								           {
															p = proc_branch(p);//ִ���ӳ���
															p->next = jumpq;  //���ضϵ��ַ
													 }
													 else
													 {
															p->next = jumpq;//���ضϵ��ַ
													 }
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
                                  
										p = p ->next ;//��ʱ��pָ��Ĳ���ѭ�������ĵ�һ��ָ��
										 
										OSTimeDly(5 ,OS_OPT_TIME_DLY, &err);//whileѭ�����������һ����ʱ���Է���ѭ������ʵ���������
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
//
//����һ���б�ͷ���ĵ�����
//������������ӳ�������ı�ͷ���
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
//���ӽڵ�,��Ԫ�ؼӵ��±�Ϊindex�ĵط�����ʵ�����ν����ӱ�ͷ��������
//���п�������
int Add_Node (_Listptr head, int index, uint8_t flag, char *content)
{
				int i = 0;
				_Listptr    q = (_Listptr)mymalloc(SRAMIN, sizeof(_Instructor));
				_Listptr    p = head;
			  u8      Mb_Val;
				if(index <= 0 || !q)
				{
					_MessageBox("Fail to malloc","Error",&Mb_Val);
					return -1;
				}
				
				q -> index = index;
				strcpy(q -> EditContent, content);
				q -> _flag = (enum _FLAG)flag;
				
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
					p = p ->next ;//��ʱp��Ϊ�±�Ϊindex�Ľ��
					i ++;
				}
				
				if(!p)
				{
					
					return -1;
				}
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
			myfree(SRAMIN,q);  //�ͷ�index���Ŀռ�
			
			return 0;
		}
		
}
//��ȡ��ǰ����ĳ���,�������Ϊ��ͷ���
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
//������������ͷ�ԭ���Ľ��ռ�
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
//��index��㿪ʼѰ�ұ�־Ϊflag�Ľ��,�����ظý���ָ��
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
/*
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
*/
//
//�����������
//�����������Բ������ƻ�ԭ�����˳��
void List_Parse(_Listptr  ptr)
{
		_Listptr  jumpq;//��תָ�룬����if���͵����ӳ���
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
	  //��4���˿ڳ�ʼ��Ϊ����
	  port_1.dir = PORT_IN;
	  port_2.dir = PORT_IN;
	  port_3.dir = PORT_IN;
	  port_4.dir = PORT_IN;
	
	  
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
				//�������̿������if\or\while�ȣ����ж��Ƿ�����ж����������������Ļ�ֱ�ӽ�����һ�����
				case FLAG_PORT_SIGNAL://����˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if(port.cur_val == SIGNAL)
							{
								//������������������if��֧
								ptr = if_branch(ptr);                    
							}
							else{
								//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
								jumpq = Find_Node(ptr, FLAG_OR);
								if(!jumpq)
									jumpq = Find_Node(ptr, FLAG_IF_END);
								
								ptr = jumpq;//�������㸳��ptr
							}
					break;
				case FLAG_PORT_NOSIGNAL://����˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							Detect_Port(&port);
							if(port.cur_val == NOSIGNAL)
							{
								//������������������if��֧
								ptr = if_branch(ptr);                    
							}
							else {
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//�������㸳��ptr
							}
					break;
				case FLAG_PORT_WAIT_SIGNAL://�ȴ��˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != SIGNAL);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://�ȴ��˿�_���ź�
							port.id = (ptr->EditContent)[12] - 0x30;
							do{
								  Detect_Port(&port);
							}while(port.cur_val != NOSIGNAL);
					break;
				case FLAG_PORT_GREATER: //����˿�_>_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							Detect_Port(&port);
							if(port.cur_val > port.tar_val )
							{
								//������������������if��֧
								ptr = if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//�������㸳��ptr
							}
					break;
				case FLAG_PORT_LITTLER: //����˿�_<_
							port.id = (ptr->EditContent)[12] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 14);
							Detect_Port(&port);
							if(port.cur_val < port.tar_val )
							{
								//������������������if��֧
								ptr = if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//�������㸳��ptr
							}
					break;
				case FLAG_OBSTRACLE_GREATER:
					   	ult.tar_distance = atoi(ptr->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance )
							{
									//������������������if��֧
									ptr = if_branch(ptr);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//�������㸳��p
							}
					break;
				case FLAG_OBSTRACLE_LITTER:
					    ult.tar_distance = atoi(ptr->EditContent + 16);
							if(ult.cur_distance > ult.tar_distance )
							{
									//������������������if��֧
									ptr = if_branch(ptr);                      //Ƕ��
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//�������㸳��p
							}
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
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_SHOW_B:
					    display = DISP_VARIABLE;
					break;
				case FLAG_VAR_A_GREATER: //���A>_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val > var.tar_val )
							{
									//������������������if��֧
									ptr = if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//�������㸳��ptr
							}
					break;
				case FLAG_VAR_A_LITTLER: //���A<_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 8);
							if(var.set_val < var.tar_val )
							{
								//������������������if��֧
								ptr = if_branch(ptr);                    
							}
							else{
									//�����������������Ѱ��ORָ�û��ORָ���Ѱ��IF_ENDָ��
									jumpq = Find_Node(ptr, FLAG_OR);
									if(!jumpq)
										jumpq = Find_Node(ptr, FLAG_IF_END);
									
									ptr = jumpq;//�������㸳��ptr
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
              //��ʵ������ֱ������������ִ�е�OR���,�����﷨����
							ptr = or_branch(ptr); //���ص�pָ������������ָ��
					break;
				case FLAG_IF_END:
							//
							//do nothing...
							//
					break;
				case FLAG_PROC:
				     jumpq = ptr->next ;//����ϵ�
//				     ptr->next = (_Listptr)0;//�Ͽ�������
				     procerr = OpenSubPro(atoi(ptr->EditContent + 15), ptr);//PCָ����ת
				     if(procerr == NO_ERROR)
						 {
							  ptr = proc_branch(ptr);//ִ���ӳ���
							  ptr->next = jumpq;  //���ضϵ��ַ
						 }
						 else{
						       ptr->next = jumpq;  //���ضϵ��ַ
						 }
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

/*******************************************End of File***********************************************/
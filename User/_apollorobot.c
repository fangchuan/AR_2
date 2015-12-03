#include "_apollorobot.h"
#include "stdlib.h"
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
_Variable   var;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static int or_branch (_Listptr  p);
static int if_branch (_Listptr  p);
static int while_branch(_Listptr  p);
//
//if 满足条件后面的分支语句,if后只支持一条语句。支持嵌套
//
static int if_branch (_Listptr  p)
{
		if(!p)
			return -1;
		else{
		switch ( p ->_flag )
			{
				case FLAG_MOTOR_C:   //电机_正转,速度_  
								motor.id =( p->EditContent )[6] - 0x30;
								motor.direction = FORWARD;
								motor.speed = atoi(p->EditContent + 20);
				
					break;
				case FLAG_MOTOR_CC: //电机_反转,速度_
								motor.id = ( p->EditContent )[6] - 0x30;
								motor.direction = BACKWARD;
								motor.speed = atoi(p->EditContent + 20);
				
					break;
				case FLAG_SERVO:  //舵机_转_
							servo.id = ( p->EditContent )[6] - 0x30;
							servo.degree = atoi(p->EditContent + 10);
				
					break;
				case FLAG_LED:  //LED_
							led.id = ( p->EditContent )[3] - 0x30;
							
					break;
				case FLAG_CAR_LEFT:
							
					break;
				case FLAG_CAR_RIGHT:
					
					break;
				case FLAG_CAR_FORWARD:
					
					break;
				case FLAG_CAR_BACKWARD:
					
					break;
				case FLAG_CAR_STOP:
					
					break;
				case FLAG_PORT_SIGNAL://如果端口_有信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							p = p -> next;
							if_branch(p);                      //嵌套
					break;
				case FLAG_PORT_NOSIGNAL://如果端口_无信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							p = p -> next;
							if_branch(p);                   //嵌套
					break;
				case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							p = p -> next;
							if_branch(p);                 //嵌套
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							p = p -> next;
							if_branch(p);                 //嵌套
					break;
				case FLAG_PORT_GREATER: //如果端口_>_
							port.id = (p->EditContent)[4] - 0x30;
							port.tar_val = atoi(p->EditContent + 6);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							p = p -> next;
							if_branch(p);               //嵌套
					break;
				case FLAG_PORT_LITTLER: //如果端口_<_
							port.id = (p->EditContent)[4] - 0x30;
							port.tar_val = atoi(p->EditContent + 6);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							p = p -> next;
							if_branch(p);             //嵌套
					break;
				case FLAG_VAR_SET_A: //设定A=
							var.id = VAR_A;
							var.set_val = atoi(p->EditContent + 4);
					break;
				case FLAG_VAR_SET_B: //设定B=
							var.id = VAR_B;
							var.set_val = atoi(p->EditContent + 4);
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
							var.tar_val = atoi(p->EditContent + 4);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							p = p -> next;
							if_branch(p);
					break;
				case FLAG_VAR_A_LITTLER: //变量A<_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 4);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							if_branch(p->next);
					break;
				case FLAG_WHILE_HEAD:
							p = p -> next;
							while_branch(p);
					break;
				case FLAG_WHILE_TAIL:
							return -1;
							//
							//此处应该报错！！！
							//
					break;
				case FLAG_END_PROGRAM:
							p ->next = (void *)0;//则下一条语句无效，断开链表
					break;
				case FLAG_OR:
							p = p -> next;
							or_branch(p);
					break;
				case FLAG_DELAY_NMS:
					
					break;
				case FLAG_MUSIC:
					
					break;
				default:break;
			}
			return 0;
		}
}

//
//"否则"后面所跟的语句处理,也只能跟一条语句。不支持嵌套
//
static int or_branch (_Listptr  p)
{
	
			if(!p)
					return -1;
			else
			{
			switch ( p ->_flag )
			{
				case FLAG_MOTOR_C:   //电机_正转,速度_  
								motor.id =( p->EditContent )[6] - 0x30;
								motor.direction = FORWARD;
								motor.speed = atoi(p->EditContent + 20);
				
					break;
				case FLAG_MOTOR_CC: //电机_反转,速度_
								motor.id = ( p->EditContent )[6] - 0x30;
								motor.direction = BACKWARD;
								motor.speed = atoi(p->EditContent + 20);
				
					break;
				case FLAG_SERVO:  //舵机_转_
							servo.id = ( p->EditContent )[6] - 0x30;
							servo.degree = atoi(p->EditContent + 10);
				
					break;
				case FLAG_LED:  //LED_
							led.id = ( p->EditContent )[3] - 0x30;
							
					break;
				case FLAG_CAR_LEFT:
							
					break;
				case FLAG_CAR_RIGHT:
					
					break;
				case FLAG_CAR_FORWARD:
					
					break;
				case FLAG_CAR_BACKWARD:
					
					break;
				case FLAG_CAR_STOP:
					
					break;
				case FLAG_PORT_SIGNAL://如果端口_有信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							p = p -> next;
							if_branch(p);                      //嵌套
					break;
				case FLAG_PORT_NOSIGNAL://如果端口_无信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							p = p -> next;
							if_branch(p);                   //嵌套
					break;
				case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							p = p -> next;
							if_branch(p);                 //嵌套
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
							port.id = (p->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							p = p -> next;
							if_branch(p);                 //嵌套
					break;
				case FLAG_PORT_GREATER: //如果端口_>_
							port.id = (p->EditContent)[4] - 0x30;
							port.tar_val = atoi(p->EditContent + 6);
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							p = p -> next;
							if_branch(p);               //嵌套
					break;
				case FLAG_PORT_LITTLER: //如果端口_<_
							port.id = (p->EditContent)[4] - 0x30;
							port.tar_val = atoi(p->EditContent + 6);
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							p = p -> next;
							if_branch(p);             //嵌套
					break;
				case FLAG_VAR_SET_A: //设定A=
							var.id = VAR_A;
							var.set_val = atoi(p->EditContent + 4);
					break;
				case FLAG_VAR_SET_B: //设定B=
							var.id = VAR_B;
							var.set_val = atoi(p->EditContent + 4);
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
							var.tar_val = atoi(p->EditContent + 4);
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							p = p -> next;
							if_branch(p);
					break;
				case FLAG_VAR_A_LITTLER: //变量A<_
							var.id = VAR_A;
							var.tar_val = atoi(p->EditContent + 4);
							//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
							if_branch(p->next);
					break;
				case FLAG_WHILE_HEAD:
							p = p -> next;
							while_branch(p);
					break;
				case FLAG_WHILE_TAIL:
							return -1;
							//
							//此处应该报错！！！
							//
					break;
				case FLAG_END_PROGRAM:
							p ->next = (void *)0;//则下一条语句无效，断开链表
					break;
				case FLAG_OR:
					break;
				case FLAG_DELAY_NMS:
					
					break;
				case FLAG_MUSIC:
					
					break;
				default:break;
			 }
				return 0;
			}
}
//
//"循环开始"后面的代码块处理.支持嵌套
//Tips:其实没必要定义index_last、index_temp,让链表执行到WHILE_TAIL结点再给链表指针p跳转也行
static int while_branch (_Listptr  p)
{
			if(!p)
				return -1;
			else
			{
					int index_first = p->index ;//while循环里的第一句所在地址(索引)
					int index_last,index_temp;
					_Listptr q_whiletail;
					q_whiletail = Find_Node(index_first,FLAG_WHILE_TAIL);//找到WHILE_TAIL结点
					if(!q_whiletail)
						return -1;
					else{
								index_last = q_whiletail->index - 1;//循环语句里的最后一句指令的索引
								index_temp = index_first;
								while( index_temp < index_last && p)
								{
										switch ( p ->_flag )
										{
											case FLAG_MOTOR_C:   //电机_正转,速度_  
															motor.id =( p->EditContent )[6] - 0x30;
															motor.direction = FORWARD;
															motor.speed = atoi(p->EditContent + 20);
											
												break;
											case FLAG_MOTOR_CC: //电机_反转,速度_
															motor.id = ( p->EditContent )[6] - 0x30;
															motor.direction = BACKWARD;
															motor.speed = atoi(p->EditContent + 20);
											
												break;
											case FLAG_SERVO:  //舵机_转_
														servo.id = ( p->EditContent )[6] - 0x30;
														servo.degree = atoi(p->EditContent + 10);
											
												break;
											case FLAG_LED:  //LED_
														led.id = ( p->EditContent )[3] - 0x30;
														
												break;
											case FLAG_CAR_LEFT:
														
												break;
											case FLAG_CAR_RIGHT:
												
												break;
											case FLAG_CAR_FORWARD:
												
												break;
											case FLAG_CAR_BACKWARD:
												
												break;
											case FLAG_CAR_STOP:
												
												break;
											case FLAG_PORT_SIGNAL://如果端口_有信号
														port.id = (p->EditContent)[4] - 0x30;
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);                      //嵌套
												break;
											case FLAG_PORT_NOSIGNAL://如果端口_无信号
														port.id = (p->EditContent)[4] - 0x30;
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);                   //嵌套
												break;
											case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
														port.id = (p->EditContent)[4] - 0x30;
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);                 //嵌套
												break;
											case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
														port.id = (p->EditContent)[4] - 0x30;
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);                 //嵌套
												break;
											case FLAG_PORT_GREATER: //如果端口_>_
														port.id = (p->EditContent)[4] - 0x30;
														port.tar_val = atoi(p->EditContent + 6);
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);               //嵌套
												break;
											case FLAG_PORT_LITTLER: //如果端口_<_
														port.id = (p->EditContent)[4] - 0x30;
														port.tar_val = atoi(p->EditContent + 6);
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);             //嵌套
												break;
											case FLAG_VAR_SET_A: //设定A=
														var.id = VAR_A;
														var.set_val = atoi(p->EditContent + 4);
												break;
											case FLAG_VAR_SET_B: //设定B=
														var.id = VAR_B;
														var.set_val = atoi(p->EditContent + 4);
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
														var.tar_val = atoi(p->EditContent + 4);
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);
												break;
											case FLAG_VAR_A_LITTLER: //变量A<_
														var.id = VAR_A;
														var.tar_val = atoi(p->EditContent + 4);
														//如果条件成立，则进入if分支，否则什么也不执行，取下一个指令
														p = p -> next;
														if_branch(p);
												break;
											case FLAG_WHILE_HEAD:
														p = p -> next;
														while_branch(p); //嵌套
												break;
											case FLAG_WHILE_TAIL:
												break;
											case FLAG_END_PROGRAM:
														p ->next = (void *)0;//则下一条语句无效，断开链表
												break;
											case FLAG_OR:
												break;
											case FLAG_DELAY_NMS:
												
												break;
											case FLAG_MUSIC:
												
												break;
											default:break;
										 }

										index_temp ++;
										if(index_temp == index_last)
										{
											index_temp = index_first;
											p = Find_Node(index_first -1,FLAG_WHILE_HEAD);//因为这里记录的Index_first是WHILE_HEAD指令的后一句，
																																		//所以index_first的上一个索引才是WHILE_HEAD
										}                                            
										p = p ->next ;//这时的p指向的才是循环语句里的第一条指令
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
int Add_Node(int index, enum _FLAG flag, char *content)
{
				int i = 0;
				_Listptr    q = (_Listptr)malloc(sizeof(_Instructor));
				_Listptr    p = Ins_List_Head;
			
				if(index <= 0 || !q)
					return -1;
				
				q -> index = index;
				q -> EditContent = content;
				q -> _flag = flag;
				
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
							p->EditContent = content;
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
void List_Parse(_Listptr ptr)
{
		
		while(ptr)
		{

			switch ( ptr->_flag )
			{
				case FLAG_MOTOR_C:   //电机_正转,速度_  
								motor.id =( ptr->EditContent )[6] - 0x30;
								motor.direction = FORWARD;
								motor.speed = atoi(ptr->EditContent + 20);
					break;
				case FLAG_MOTOR_CC: //电机_反转,速度_
								motor.id = ( ptr->EditContent )[6] - 0x30;
								motor.direction = BACKWARD;
								motor.speed = atoi(ptr->EditContent + 20);
					break;
				case FLAG_SERVO:  //舵机_转_
							servo.id = ( ptr->EditContent )[6] - 0x30;
							servo.degree = atoi(ptr->EditContent + 10);
					break;
				case FLAG_LED:  //LED_
							led.id = ( ptr->EditContent )[3] - 0x30;
							
					break;
				case FLAG_CAR_LEFT:
							
					break;
				case FLAG_CAR_RIGHT:
					
					break;
				case FLAG_CAR_FORWARD:
					
					break;
				case FLAG_CAR_BACKWARD:
					
					break;
				case FLAG_CAR_STOP:
					
					break;
				//遇到流程控制语句if\or\while等，先判断是否符合判断条件，符合条件的话直接进入下一个结点
				case FLAG_PORT_SIGNAL://如果端口_有信号
							port.id = (ptr->EditContent)[4] - 0x30;
							
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_PORT_NOSIGNAL://如果端口_无信号
							port.id = (ptr->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_PORT_WAIT_SIGNAL://等待端口_有信号
							port.id = (ptr->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_PORT_WAIT_NOSIGNAL://等待端口_无信号
							port.id = (ptr->EditContent)[4] - 0x30;
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_PORT_GREATER: //如果端口_>_
							port.id = (ptr->EditContent)[4] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 6);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_PORT_LITTLER: //如果端口_<_
							port.id = (ptr->EditContent)[4] - 0x30;
							port.tar_val = atoi(ptr->EditContent + 6);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_VAR_SET_A: //设定A=
							var.id = VAR_A;
							var.set_val = atoi(ptr->EditContent + 4);
					break;
				case FLAG_VAR_SET_B: //设定B=
							var.id = VAR_B;
							var.set_val = atoi(ptr->EditContent + 4);
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
							var.tar_val = atoi(ptr->EditContent + 4);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_VAR_A_LITTLER: //变量A<_
							var.id = VAR_A;
							var.tar_val = atoi(ptr->EditContent + 4);
							//如果条件成立，则进入if分支，否则什么也不执行，去下一个指令
							ptr = ptr -> next;
							if_branch(ptr);
					break;
				case FLAG_WHILE_HEAD:
								ptr = ptr ->next ;
								while_branch(ptr);
					break;
				case FLAG_WHILE_TAIL:
					
					break;
				case FLAG_END_PROGRAM:
							ptr ->next = (void *)0;
					break;
				case FLAG_OR:
							ptr = ptr -> next;
							or_branch(ptr);
					break;
				case FLAG_DELAY_NMS:
					
					break;
				case FLAG_MUSIC:
					
					break;
				default:break;
			}
			ptr = ptr -> next ;
		}
}

/*******************************************End of File***********************************************/
#ifndef  _APOLLOROBOT_H
#define  _APOLLOROBOT_H

#include  "stdint.h"
//#include  ""

/*********************************************************************
*
*       一些全局标志位
*
**********************************************************************
*/

//
//指令编辑界面的一些标志位
//
enum _FLAG {			FLAG_MOTOR_C = 1,//电机正转标志位
									FLAG_MOTOR_CC,   //电机反转标志位
                  FLAG_SERVO,      //舵机标志位
									FLAG_LED,        //LED标志位
									FLAG_CAR_LEFT,   //小车左转标志位
									FLAG_CAR_RIGHT,  //小车右转标志位
									FLAG_CAR_FORWARD,//小车前进标志位
									FLAG_CAR_BACKWARD,//小车后退标志位
									FLAG_CAR_STOP,    //小车停止标志位
									FLAG_PORT_SIGNAL,	//如果端口有信号标志位
									FLAG_PORT_NOSIGNAL,//如果端口无信号标志位
									FLAG_PORT_WAIT_SIGNAL,//等待端口有信号标志位
									FLAG_PORT_WAIT_NOSIGNAL,//等待端口无信号标志位
									FLAG_PORT_GREATER,    //如果端口大于
									FLAG_PORT_LITTLER,//如果端口小于
									FLAG_VAR_SET_A,//设定变量A标志位
									FLAG_VAR_SET_B,//设定变量B标志位
									FLAG_VAR_A_INC,//A++标志位
									FLAG_VAR_A_DEC,//A--标志位
									FLAG_VAR_B_INC,//B++标志位
									FLAG_VAR_B_DEC,//B--标志位
									FLAG_VAR_SHOW_A,//显示变量A标志位
									FLAG_VAR_SHOW_B,//显示变量B标志位
									FLAG_VAR_A_GREATER,//A>标志位
									FLAG_VAR_A_LITTLER,//A<标志位
									FLAG_START_WHILE,//开始循环标志位
									FLAG_END_WHILE,//结束循环标志位
									FLAG_END_PROGRAM,//结束程序标志位
									FLAG_OR,				//否则标志位
									FLAG_DELAY_NMS, //延时nms标志位
									FLAG_MUSIC     //播放音乐标志位
};
//
//每一条编辑完的程序数据结构
//
typedef struct _INSTRUCTOR {
				uint8_t         _flag; //该条程序的指令类别
				char        *EditContent; //该条指令的文本内容
				struct _INSTRUCTOR* next; //指向下一条指令的指针
}_Instructor,*_Listptr;


//
//电机控制数据结构
//
#define  FORWARD  1
#define  BACKWARD -1
typedef struct _MOTOR {
				uint8_t 			 id;       //电机编号 1、2
				int8_t  direction;//电机转向  正转、反转
				int     		speed;    //电机速度
}_Motor;

//
//舵机控制数据结构
//
typedef struct _SERVO {
				uint8_t 		id;     //舵机编号  1、2、3、4
				int     degree; //舵机转角  
}_Servo;

//
//LED控制数据结构
//
/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  1
#define OFF 0
typedef struct _LED {
				uint8_t 		id;    //LED编号 1、2、3、4
				uint8_t status;//LED状态 打开或关闭  0、1
}_Led;

//
//端口控制数据结构
//
#define  SIGNAL   0
#define  NOSINGAL 1
typedef struct _PORT {
				uint8_t 		id;     //端口编号 1\2\3\4
				uint8_t status; //端口状态 有/无信号 0/1
				int     cur_val;//当前值
				int     tar_val;//目标值
}_Port;

//
//变量数据结构
//
#define VAR_A  0
#define VAR_B  1
typedef struct _VARIABLE {
				uint8_t      id;  //变量编号 A/B
				int 		set_val;  //设定值，也是当前值
				int     tar_val;  //目标值
}_Variable;


extern _Listptr Ins_List_Head;//程序链表的表头节点
extern int Create_List(void);
extern int Add_Node(int index, enum _FLAG flag, char *content);//增加节点，后插法
extern int  Replace_Node(int index, enum _FLAG flag,char *content);//替换节点
extern int Delete_Node(int index);

#endif /*_APOLLOROBOT_H*/

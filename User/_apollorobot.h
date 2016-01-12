#ifndef  _APOLLOROBOT_H
#define  _APOLLOROBOT_H

#include  "stm32f10x.h"
#include  "stdlib.h"
#include  "stdio.h"
#include  "protocol.h"
/*********************************************************************
*
*       一些错误信息
*
**********************************************************************
*/
typedef enum _ERROR {	NO_ERROR = 0,
											ERROR_ID ,    //操作对象的id越界
											ERROR_GRAMMAR,//语句的语法有误
											ERROR_TONES,  //错误的音调 
											ERROR_TIME,   //错误的节拍
											ERROR_DIR,    //错误的小车方向
							
}_Error;

//用于标识传感器种类的枚举类型
typedef enum _SENSORFLAG {
                           SENSOR_IR     = 1,//红外光电传感器
													 SENSOR_SWITCH,      //触碰开关
													 SENSOR_LED,         //LED
													 SENSOR_LDR,         //光敏电阻
													 SENSOR_ULTRASNIO,   //超声波
													 SENSOR_MPU,         //MPU6050
													
														
}_SensorFlag;
//指令操作所属类别
#define  FLAG_CHANGE  1
#define  FLAG_INSERT  2
#define  FLAG_DELETE  3

//
//指令编辑界面的一些标志位
//
enum _FLAG {			FLAG_MOTOR_C = 1,//电机正转标志位
									FLAG_MOTOR_CC,   //电机反转标志位
                  FLAG_SERVO,      //舵机标志位
									FLAG_LED_ON,     //LED打开标志位
									FLAG_LED_OFF,    //LED关闭标志位   
									FLAG_CAR_LEFT,   //小车左转标志位
									FLAG_CAR_RIGHT,  //小车右转标志位
									FLAG_CAR_FORWARD,//小车前进标志位
									FLAG_CAR_BACKWARD,//小车后退标志位
									FLAG_CAR_STOP,    //小车停止标志位
									FLAG_CAR_ACCEL,   //小车加速标志位
									FLAG_CAR_SLOW,    //小车减速标志位
									FLAG_PORT_SIGNAL,	//如果端口有信号标志位
									FLAG_PORT_NOSIGNAL,//如果端口无信号标志位
									FLAG_PORT_WAIT_SIGNAL,//等待端口有信号标志位
									FLAG_PORT_WAIT_NOSIGNAL,//等待端口无信号标志位
									FLAG_PORT_GREATER,    //如果端口大于
									FLAG_PORT_LITTLER,//如果端口小于
									FLAG_OBSTRACLE_GREATER,//如果障碍物大于
									FLAG_OBSTRACLE_LITTER,//如果障碍物小于
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
									FLAG_WHILE_HEAD,//循环语句头部标志位
									FLAG_WHILE_TAIL,//循环语句尾部标志位
									FLAG_END_PROGRAM,//结束程序标志位
									FLAG_OR,				//否则标志位
									FLAG_IF_END,    //条件结束标志位
									FLAG_DELAY_NMS, //延时nms标志位
									FLAG_MUSIC,     //播放音乐标志位
									FLAG_SHOW_DISTANCE,//显示当前距离
};
//
//每一条编辑完的程序数据结构
//
typedef struct _INSTRUCTOR {
				uint8_t        		index; //该节点位于链表中的索引
				enum _FLAG     		_flag; //该条程序的指令类别
				char    EditContent[50]; //该条指令的文本内容
				struct _INSTRUCTOR* next; //指向下一条指令的指针
}_Instructor,*_Listptr;

//
//用于解析指令的状态栈，观察代码嵌套层次
//
#define  ELETYPE   uint8_t //堆栈中存放的数据类型为uint8_t
#define  MAX_SIZE_STACK   50
typedef struct _STATUSTACK {
	
				ELETYPE            *base;//栈底指针
				ELETYPE             *top;//栈顶指针，当top==base时表示空栈
				int            stacksize;//堆栈大小，即表示最大嵌套层次,我们这里最大可嵌套50层
	
}_StatuStack;

//
//电机控制数据结构
//
#define  MOTOR1   1
#define  MOTOR2   2
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
#define  SERVO1   1
#define  SERVO2   2
#define  SERVO3   3
#define  SERVO4   4
typedef struct _SERVO {
				uint8_t 		id;     //舵机编号  1、2、3、4
				int     degree; //舵机转角  
}_Servo;

//
//LED控制数据结构
//
#define  LED1    1
#define  LED2    2
#define  LED3    3
#define  LED4    4
#define  ON      1
#define  OFF     0
typedef struct _LED {
				uint8_t 		id;    //LED编号 1、2、3、4
				uint8_t status;//LED状态 打开或关闭  0、1
}_Led;

//
//小车控制结构体
//
#define  LEFT    2
#define  RIGHT   -2
#define  STOP    3

#define  SPEED_STEP  50
typedef struct _CAR  {
				int8_t   direction;
				int     speed_step;//车速步进.步进为正则是加速，步进为负就是减速

}_Car;
//
//端口控制数据结构
//
#define  PORT_1    1
#define  PORT_2    2
#define  PORT_3    3
#define  PORT_4    4

#define  PORT_ON   1
#define  PORT_OFF  0

#define  SIGNAL    1
#define  NOSIGNAL  0

#define  DS        1
#define  AS        2
#define  ULTRASNIO 3
typedef struct _PORT {
				uint8_t 		id;     //端口编号 1\2\3\4
				uint8_t status; //端口有无传感器连接0/1
	      uint8_t species;//端口传感器的种类
				int     cur_val;//当前值
				int     tar_val;//目标值
}_Port;
//数字传感器
typedef struct _DS {
				uint8_t   sta;   //是否插上数字传感器
	      uint8_t   val;//

}_DS;
//模拟传感器
typedef struct _AS {
	      uint8_t    sta;//是否插上模拟传感器
	      uint8_t    val;

}_AS;
//MPU6050
typedef struct _EULER {
				float  angle_x;
	      float  angle_y;
	      float  accel_x;
	      float  accel_y;
				float  accel_z;
				float   gyro_x;
				float   gyro_y;
				float   gyro_z;
}_Euler;
//
//超声波的数据结构
//
#define SHOW_ON   1
#define SHOW_OFF  0
typedef struct _ULTRASNIO{
	      uint8_t         ifshow;// 是否现显示当前距离
	      float     cur_distance;// 超声波测得的当前的距离
	      uint16_t  tar_distance;// 目标距离，要比较的值
}_Ultrasnio;
//
//变量数据结构
//
#define VAR_A  1
#define VAR_B  2
typedef struct _VARIABLE {
				uint8_t      id;  //变量编号 A/B
				int 		set_val;  //设定值，也是当前值
				int     tar_val;  //目标值
}_Variable;


//
//音符数据结构
//

typedef struct  _MUSIC {
        uint8_t  tones;  //音调  用户配置时都是选择1~7
	      uint8_t  time;   //节拍  用户配置时都是选择1~7
}_Music;

extern _Listptr Ins_List_Head;//程序链表的表头节点
//extern _StatuStack StaStk;    //表示代码嵌套层次的状态栈 

extern int Create_List(void);
extern int Add_Node(int index, uint8_t flag, char *content);//增加节点，后插法
extern int  Replace_Node(int index, enum _FLAG flag,char *content);//替换节点
extern int Delete_Node(int index);//删除节点
extern void Clear_List(void);//删除整个链表
extern _Listptr  Find_Node(int index, enum _FLAG flag);////从index结点开始寻找标志为flag的结点,并返回该结点的指针
extern int  GetListLength(void);//返回当前链表的长度,若只有头结点则长度为0
extern int Create_Stack(void);//创建一个空栈
extern int GetStackLength(_StatuStack *stk);////返回当前栈的长度
extern int GetTop(_StatuStack *Stk,uint8_t *ele);////获取栈顶元素，存入ele，不是出栈
extern int Push(_StatuStack *Stk, uint8_t ele);//
extern int Pop(_StatuStack *Stk, uint8_t *ele);
extern void List_Parse(_Listptr ptr);
_Error Detect_Port(_Port *port);
extern void InitMPUSensor(_Euler *sensor);

#endif /*_APOLLOROBOT_H*/

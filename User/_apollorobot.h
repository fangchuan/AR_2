#ifndef  _APOLLOROBOT_H
#define  _APOLLOROBOT_H

#include  "stm32f10x.h"
#include  "stdlib.h"
#include  "stdio.h"
#include  "protocol.h"
/*********************************************************************
*
*       һЩ������Ϣ
*
**********************************************************************
*/
typedef enum _ERROR {	NO_ERROR = 0,
											ERROR_ID ,    //���������idԽ��
											ERROR_GRAMMAR,//�����﷨����
											ERROR_TONES,  //��������� 
											ERROR_TIME,   //����Ľ���
											ERROR_DIR,    //�����С������
							
}_Error;

//���ڱ�ʶ�����������ö������
typedef enum _SENSORFLAG {
                           SENSOR_IR     = 1,//�����紫����
													 SENSOR_SWITCH,      //��������
													 SENSOR_LED,         //LED
													 SENSOR_LDR,         //��������
													 SENSOR_ULTRASNIO,   //������
													 SENSOR_MPU,         //MPU6050
													
														
}_SensorFlag;
//ָ������������
#define  FLAG_CHANGE  1
#define  FLAG_INSERT  2
#define  FLAG_DELETE  3

//
//ָ��༭�����һЩ��־λ
//
enum _FLAG {			FLAG_MOTOR_C = 1,//�����ת��־λ
									FLAG_MOTOR_CC,   //�����ת��־λ
                  FLAG_SERVO,      //�����־λ
									FLAG_LED_ON,     //LED�򿪱�־λ
									FLAG_LED_OFF,    //LED�رձ�־λ   
									FLAG_CAR_LEFT,   //С����ת��־λ
									FLAG_CAR_RIGHT,  //С����ת��־λ
									FLAG_CAR_FORWARD,//С��ǰ����־λ
									FLAG_CAR_BACKWARD,//С�����˱�־λ
									FLAG_CAR_STOP,    //С��ֹͣ��־λ
									FLAG_CAR_ACCEL,   //С�����ٱ�־λ
									FLAG_CAR_SLOW,    //С�����ٱ�־λ
									FLAG_PORT_SIGNAL,	//����˿����źű�־λ
									FLAG_PORT_NOSIGNAL,//����˿����źű�־λ
									FLAG_PORT_WAIT_SIGNAL,//�ȴ��˿����źű�־λ
									FLAG_PORT_WAIT_NOSIGNAL,//�ȴ��˿����źű�־λ
									FLAG_PORT_GREATER,    //����˿ڴ���
									FLAG_PORT_LITTLER,//����˿�С��
									FLAG_OBSTRACLE_GREATER,//����ϰ������
									FLAG_OBSTRACLE_LITTER,//����ϰ���С��
									FLAG_VAR_SET_A,//�趨����A��־λ
									FLAG_VAR_SET_B,//�趨����B��־λ
									FLAG_VAR_A_INC,//A++��־λ
									FLAG_VAR_A_DEC,//A--��־λ
									FLAG_VAR_B_INC,//B++��־λ
									FLAG_VAR_B_DEC,//B--��־λ
									FLAG_VAR_SHOW_A,//��ʾ����A��־λ
									FLAG_VAR_SHOW_B,//��ʾ����B��־λ
									FLAG_VAR_A_GREATER,//A>��־λ
									FLAG_VAR_A_LITTLER,//A<��־λ
									FLAG_WHILE_HEAD,//ѭ�����ͷ����־λ
									FLAG_WHILE_TAIL,//ѭ�����β����־λ
									FLAG_END_PROGRAM,//���������־λ
									FLAG_OR,				//�����־λ
									FLAG_IF_END,    //����������־λ
									FLAG_DELAY_NMS, //��ʱnms��־λ
									FLAG_MUSIC,     //�������ֱ�־λ
									FLAG_SHOW_DISTANCE,//��ʾ��ǰ����
};
//
//ÿһ���༭��ĳ������ݽṹ
//
typedef struct _INSTRUCTOR {
				uint8_t        		index; //�ýڵ�λ�������е�����
				enum _FLAG     		_flag; //���������ָ�����
				char    EditContent[50]; //����ָ����ı�����
				struct _INSTRUCTOR* next; //ָ����һ��ָ���ָ��
}_Instructor,*_Listptr;

//
//���ڽ���ָ���״̬ջ���۲����Ƕ�ײ��
//
#define  ELETYPE   uint8_t //��ջ�д�ŵ���������Ϊuint8_t
#define  MAX_SIZE_STACK   50
typedef struct _STATUSTACK {
	
				ELETYPE            *base;//ջ��ָ��
				ELETYPE             *top;//ջ��ָ�룬��top==baseʱ��ʾ��ջ
				int            stacksize;//��ջ��С������ʾ���Ƕ�ײ��,������������Ƕ��50��
	
}_StatuStack;

//
//����������ݽṹ
//
#define  MOTOR1   1
#define  MOTOR2   2
#define  FORWARD  1
#define  BACKWARD -1
typedef struct _MOTOR {
				uint8_t 			 id;       //������ 1��2
				int8_t  direction;//���ת��  ��ת����ת
				int     		speed;    //����ٶ�
}_Motor;

//
//����������ݽṹ
//
#define  SERVO1   1
#define  SERVO2   2
#define  SERVO3   3
#define  SERVO4   4
typedef struct _SERVO {
				uint8_t 		id;     //������  1��2��3��4
				int     degree; //���ת��  
}_Servo;

//
//LED�������ݽṹ
//
#define  LED1    1
#define  LED2    2
#define  LED3    3
#define  LED4    4
#define  ON      1
#define  OFF     0
typedef struct _LED {
				uint8_t 		id;    //LED��� 1��2��3��4
				uint8_t status;//LED״̬ �򿪻�ر�  0��1
}_Led;

//
//С�����ƽṹ��
//
#define  LEFT    2
#define  RIGHT   -2
#define  STOP    3

#define  SPEED_STEP  50
typedef struct _CAR  {
				int8_t   direction;
				int     speed_step;//���ٲ���.����Ϊ�����Ǽ��٣�����Ϊ�����Ǽ���

}_Car;
//
//�˿ڿ������ݽṹ
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
				uint8_t 		id;     //�˿ڱ�� 1\2\3\4
				uint8_t status; //�˿����޴���������0/1
	      uint8_t species;//�˿ڴ�����������
				int     cur_val;//��ǰֵ
				int     tar_val;//Ŀ��ֵ
}_Port;
//���ִ�����
typedef struct _DS {
				uint8_t   sta;   //�Ƿ�������ִ�����
	      uint8_t   val;//

}_DS;
//ģ�⴫����
typedef struct _AS {
	      uint8_t    sta;//�Ƿ����ģ�⴫����
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
//�����������ݽṹ
//
#define SHOW_ON   1
#define SHOW_OFF  0
typedef struct _ULTRASNIO{
	      uint8_t         ifshow;// �Ƿ�����ʾ��ǰ����
	      float     cur_distance;// ��������õĵ�ǰ�ľ���
	      uint16_t  tar_distance;// Ŀ����룬Ҫ�Ƚϵ�ֵ
}_Ultrasnio;
//
//�������ݽṹ
//
#define VAR_A  1
#define VAR_B  2
typedef struct _VARIABLE {
				uint8_t      id;  //������� A/B
				int 		set_val;  //�趨ֵ��Ҳ�ǵ�ǰֵ
				int     tar_val;  //Ŀ��ֵ
}_Variable;


//
//�������ݽṹ
//

typedef struct  _MUSIC {
        uint8_t  tones;  //����  �û�����ʱ����ѡ��1~7
	      uint8_t  time;   //����  �û�����ʱ����ѡ��1~7
}_Music;

extern _Listptr Ins_List_Head;//��������ı�ͷ�ڵ�
//extern _StatuStack StaStk;    //��ʾ����Ƕ�ײ�ε�״̬ջ 

extern int Create_List(void);
extern int Add_Node(int index, uint8_t flag, char *content);//���ӽڵ㣬��巨
extern int  Replace_Node(int index, enum _FLAG flag,char *content);//�滻�ڵ�
extern int Delete_Node(int index);//ɾ���ڵ�
extern void Clear_List(void);//ɾ����������
extern _Listptr  Find_Node(int index, enum _FLAG flag);////��index��㿪ʼѰ�ұ�־Ϊflag�Ľ��,�����ظý���ָ��
extern int  GetListLength(void);//���ص�ǰ����ĳ���,��ֻ��ͷ����򳤶�Ϊ0
extern int Create_Stack(void);//����һ����ջ
extern int GetStackLength(_StatuStack *stk);////���ص�ǰջ�ĳ���
extern int GetTop(_StatuStack *Stk,uint8_t *ele);////��ȡջ��Ԫ�أ�����ele�����ǳ�ջ
extern int Push(_StatuStack *Stk, uint8_t ele);//
extern int Pop(_StatuStack *Stk, uint8_t *ele);
extern void List_Parse(_Listptr ptr);
_Error Detect_Port(_Port *port);
extern void InitMPUSensor(_Euler *sensor);

#endif /*_APOLLOROBOT_H*/

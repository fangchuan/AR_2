/*
*********************************************************************************************************
*
*	ģ������ : ���ݽṹģ��ͷ�ļ�
*	�ļ����� : _apollorobot.h
*	��    �� : V1.0
*	˵    �� : ����������������ݽṹ
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef  _APOLLOROBOT_H
#define  _APOLLOROBOT_H

#include  "stm32f10x.h"
#include  "stdlib.h"
#include  "stdio.h"
#include  "string.h"
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
	                    ERROR_PAINT,  //����Ļ滭����
	                    ERROR_NAME,   //����ĳ�����
	                    ERROR_MALLOC, //
	                    ERROR_FOPERATION,//
							
}_Error;

//
//��ʾ����
//
typedef enum _DISPLAY {
												DISP_TEXT = 0,    //��ʾ�ı�:�����Զ�������
	                      DISP_DIRECTION,   //��ʾС������
	                      DISP_PAINT,       //��ʾ��������
	                      DISP_DISTANCE,    //��ʾ����������
	                      DISP_VARIABLE,    //��ʾ����ֵ
	                      DISP_GIF,         //��ʾGIFͼ

}_Display;
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
									FLAG_DRAW_HCIRCLE, //������Բ
									FLAG_DRAW_SCIRCLE, //��ʵ��Բ
									FLAG_DRAW_HRECT,   //�����ľ���
									FLAG_DRAW_SRECT,   //��ʵ�ľ���
									FLAG_DRAW_LINE,    //��ֱ��
									FLAG_SET_X1,       //��������X1
									FLAG_SET_X2,       //��������X2
									FLAG_SET_Y1,       //��������Y1
									FLAG_SET_Y2,       //��������Y2
									FLAG_SET_RADIUS,   //���ð뾶
									FLAG_COLOR,        //������ɫ
									FLAG_GIF_HAPPY,    //����GIF
									FLAG_GIF_SAD,      //�ѹ�GIF
									FLAG_GIF_CRY,      //���GIF
									FLAG_GIF_FURY,     //��ŭGIF
									FLAG_GIF_ALARM,    //����GIF
									FLAG_PROC,         //�����ӳ���
									FLAG_VAR_SET_A_PORT,//�趨����AΪ�˿�_
									FLAG_VAR_SET_B_PORT,//�趨����BΪ�˿�_
									FLAG_HACCEL_GREATOR,//���ˮƽ���ٶȴ���_
									FLAG_HACCEL_LITTER,//���ˮƽ���ٶ�С��_
									FLAG_VACCEL_GREATOR,//�����ֱ���ٶȴ���_
									FLAG_VACCEL_LITTER,//�����ֱ���ٶ�С��_

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

#define  SPEED_STEP  50   //��Ӧ��PWMֵ����50
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

#define  PORT_IN   0
#define  PORT_OUT  1

#define  SIGNAL    1
#define  NOSIGNAL  0

#define  DS        1
#define  AS        2
#define  ULTRASNIO 3
typedef struct _PORT {
				uint8_t 		id;     //�˿ڱ�� 1\2\3\4
	      uint8_t    dir;     //�˿ڷ���
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
				float  angle_x;//��λ: ��
	      float  angle_y;
	      float  accel_x;//��λ:m/s2
	      float  accel_y;
				float  accel_z;
				float   gyro_x;//��λ: rad/s
				float   gyro_y;
				float   gyro_z;
}_Euler;
typedef struct	{
				float 	x;
				float 	y;
				float		z;
}_Vector3f;
//
//�����������ݽṹ
//
#define SHOW_ON   1
#define SHOW_OFF  0
typedef struct _ULTRASNIO{
	      volatile float     cur_distance;// ��������õĵ�ǰ�ľ���
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

#define HOLLOW_CIRCLE  1
#define SOLID_CIRCLE   2
#define HOLLOW_RECT    3
#define SOLID_RECT     4
#define STRIGHT_LINE   5

#define COLOR_0       GUI_RED       //��
#define COLOR_1       GUI_ORANGE    //��
#define COLOR_2       GUI_YELLOW    //��
#define COLOR_3       GUI_GREEN     //��
#define COLOR_4       GUI_BLUE      //��
#define COLOR_5       GUI_CYAN      //��
#define COLOR_6       GUI_BROWN     //��

#define COLORMAP(index,color)   if(index == 0)     \
																{                  \
																	color = COLOR_0; \
																}                  \
																if(index == 1)     \
																{                  \
																	color = COLOR_1; \
																}                  \
																if(index == 2)     \
																{                  \
																	color = COLOR_2; \
																}                  \
																if(index == 3)     \
																{                  \
																	color = COLOR_3; \
																}                  \
																if(index == 4)     \
																{                  \
																	color = COLOR_4; \
																}                  \
																if(index == 5)     \
																{                  \
																	color = COLOR_5; \
																}                  \
																if(index == 6)     \
																{                  \
																	color = COLOR_6; \
																}                  \
//
//�������ݽṹ
//
typedef struct _PAINT {
				uint8_t species; //����:����Բ��ʵ��Բ�����ľ��Ρ�ʵ�ľ��Ρ�ֱ��
	      uint32_t  color; //7����ɫ��
	      uint16_t  width; //�߿�
	      uint16_t radius; //Բ�İ뾶
	      int16_t     x1; //����
	      int16_t     y1;
	      int16_t     x2;
	      int16_t     y2;
//	      struct _PAINT *next;//ָ����һ������ָ��
}_Paint,*_PaintList;

//
//GIFö������
//
typedef enum _GIF  {
	                   GIF_ALARM = 1,
	                   GIF_HAPPY,
	                   GIF_SAD,
	                   GIF_CRY,
	                   GIF_FURY,
}_Gif;
extern _Listptr Ins_List_Head;//��������ı�ͷ�ڵ�
//extern _StatuStack StaStk;    //��ʾ����Ƕ�ײ�ε�״̬ջ 

extern int Create_List(void);
extern int Add_Node(_Listptr head, int index, uint8_t flag, char *content);//���ӽڵ㣬��巨
extern int  Replace_Node(int index, enum _FLAG flag,char *content);//�滻�ڵ�
extern int Delete_Node(int index);//ɾ���ڵ�
extern void Clear_List(_Listptr head);//ɾ����������
extern int  GetListLength(_Listptr head);//���ص�ǰ����ĳ���,��ֻ��ͷ����򳤶�Ϊ0
extern void List_Parse(_Listptr  ptr);
_Error Detect_Port(_Port *port);
_Error Get_Port(_Port *port);
extern void InitMPUSensor(_Euler *sensor);

#endif /*_APOLLOROBOT_H*/
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

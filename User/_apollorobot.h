#ifndef  _APOLLOROBOT_H
#define  _APOLLOROBOT_H

#include  "stdint.h"
//#include  ""

/*********************************************************************
*
*       һЩȫ�ֱ�־λ
*
**********************************************************************
*/

//
//ָ��༭�����һЩ��־λ
//
enum _FLAG {			FLAG_MOTOR_C = 1,//�����ת��־λ
									FLAG_MOTOR_CC,   //�����ת��־λ
                  FLAG_SERVO,      //�����־λ
									FLAG_LED,        //LED��־λ
									FLAG_CAR_LEFT,   //С����ת��־λ
									FLAG_CAR_RIGHT,  //С����ת��־λ
									FLAG_CAR_FORWARD,//С��ǰ����־λ
									FLAG_CAR_BACKWARD,//С�����˱�־λ
									FLAG_CAR_STOP,    //С��ֹͣ��־λ
									FLAG_PORT_SIGNAL,	//����˿����źű�־λ
									FLAG_PORT_NOSIGNAL,//����˿����źű�־λ
									FLAG_PORT_WAIT_SIGNAL,//�ȴ��˿����źű�־λ
									FLAG_PORT_WAIT_NOSIGNAL,//�ȴ��˿����źű�־λ
									FLAG_PORT_GREATER,    //����˿ڴ���
									FLAG_PORT_LITTLER,//����˿�С��
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
									FLAG_START_WHILE,//��ʼѭ����־λ
									FLAG_END_WHILE,//����ѭ����־λ
									FLAG_END_PROGRAM,//���������־λ
									FLAG_OR,				//�����־λ
									FLAG_DELAY_NMS, //��ʱnms��־λ
									FLAG_MUSIC     //�������ֱ�־λ
};
//
//ÿһ���༭��ĳ������ݽṹ
//
typedef struct _INSTRUCTOR {
				uint8_t         _flag; //���������ָ�����
				char        *EditContent; //����ָ����ı�����
				struct _INSTRUCTOR* next; //ָ����һ��ָ���ָ��
}_Instructor,*_Listptr;


//
//����������ݽṹ
//
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
typedef struct _SERVO {
				uint8_t 		id;     //������  1��2��3��4
				int     degree; //���ת��  
}_Servo;

//
//LED�������ݽṹ
//
/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  1
#define OFF 0
typedef struct _LED {
				uint8_t 		id;    //LED��� 1��2��3��4
				uint8_t status;//LED״̬ �򿪻�ر�  0��1
}_Led;

//
//�˿ڿ������ݽṹ
//
#define  SIGNAL   0
#define  NOSINGAL 1
typedef struct _PORT {
				uint8_t 		id;     //�˿ڱ�� 1\2\3\4
				uint8_t status; //�˿�״̬ ��/���ź� 0/1
				int     cur_val;//��ǰֵ
				int     tar_val;//Ŀ��ֵ
}_Port;

//
//�������ݽṹ
//
#define VAR_A  0
#define VAR_B  1
typedef struct _VARIABLE {
				uint8_t      id;  //������� A/B
				int 		set_val;  //�趨ֵ��Ҳ�ǵ�ǰֵ
				int     tar_val;  //Ŀ��ֵ
}_Variable;


extern _Listptr Ins_List_Head;//��������ı�ͷ�ڵ�
extern int Create_List(void);
extern int Add_Node(int index, enum _FLAG flag, char *content);//���ӽڵ㣬��巨
extern int  Replace_Node(int index, enum _FLAG flag,char *content);//�滻�ڵ�
extern int Delete_Node(int index);

#endif /*_APOLLOROBOT_H*/

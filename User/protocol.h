#ifndef _PROTOCOL_H
#define _PROTOCOL_H
//此版本为0x01

//一帧数据最长为19bytes
#define  MAX_LEN   19

#define  FRAME_STR   0xAA    //
#define  VERSION     0x01    //
#define  FRAME_END   0xBB    //

//控制器上各组件ID
#define  MOTOR_ID     0x00
#define  MOTOR_1_ID   0x01
#define  MOTOR_2_ID   0x02

#define  SERVO_ID     0x10
#define  SERVO_1_ID   0x11
#define  SERVO_2_ID   0x12
#define  SERVO_3_ID   0x13
#define  SERVO_4_ID   0x14

#define  LED_ID       0x20
#define  LED_1_ID     0x21
#define  LED_2_ID     0x22
#define  LED_3_ID     0x23
#define  LED_4_ID     0x24

#define  AS_ID        0x30
#define  DS_ID        0x40
#define  ULTRASNIO_ID 0x50
#define  ANGLE_ID     0x60
#define  ACCEL_ID     0x70

#endif //_PROTOCOL_H

/*
*********************************************************************************************************
*
*	ģ������ : ����λ��ͨ��Э��ģ��
*	�ļ����� : protocol.h
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef _PROTOCOL_H
#define _PROTOCOL_H
//�˰汾Ϊ0x01

//һ֡�����Ϊ19bytes
#define  MAX_LEN   19

#define  FRAME_STR   0xAA    //
#define  VERSION     0x01    //
#define  FRAME_END   0xBB    //

//�������ϸ����ID
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
#define  AS_1_ID      0x31
#define  AS_2_ID      0x32
#define  AS_3_ID      0x33
#define  AS_4_ID      0x34

#define  DS_ID        0x40
#define  DS_1_ID      0x41
#define  DS_2_ID      0x42
#define  DS_3_ID      0x43
#define  DS_4_ID      0x44

#define  ULTRASNIO_ID 0x50

#define  ANGLE_ID     0x60
#define  ANGLE_X_ID   0x61
#define  ANGLE_Y_ID   0x62

#define  ACCEL_ID     0x70
#define  ACCEL_X_ID   0x71
#define  ACCEL_Y_ID   0x72

#endif //_PROTOCOL_H
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

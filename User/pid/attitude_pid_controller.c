/*
*********************************************************************************************************
*
*	ģ������ : pidͨ�ú���ģ��
*	�ļ����� : attitude_pid_controller.c
*	��    �� : V1.0
*	˵    �� : Attitude controler using PID correctors
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-07-21 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/

#include <stdbool.h>
#include "attitude_controller.h"
#include "pid.h"
#include "_apollorobot.h"
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
PidObject pidRollRate;
PidObject pidPitchRate;
PidObject pidYawRate;
PidObject pidRoll;
PidObject pidPitch;
PidObject pidYaw;

extern _Euler euler;

_Vector3f	rateDesired;
_Vector3f	angleDesired;

int16_t rollOutput;
int16_t pitchOutput;
int16_t yawOutput;
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static bool isInit;
/*********************************************************************
*
*       Static  code
*
**********************************************************************
*/
/*********************************************************************************************************
*	�� �� ��: saturateSignedInt16
*	����˵��: �ж��з��ŵ�16λ�����Ƿ����
*	��    �Σ�float
*	�� �� ֵ: 16λ�з�������ֵ
*********************************************************************************************************
*/
static int16_t saturateSignedInt16(float in)
{
		// don't use INT16_MIN, because later we may negate it, which won't work for that value.
		if (in > INT16_MAX)
			return INT16_MAX;
		else if (in < -INT16_MAX)
			return -INT16_MAX;
		else
			return (int16_t)in;
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************************************************
*	�� �� ��: attitudeControllerInit
*	����˵��: ��̬������������ʼ��, �ǶȻ������ٶȻ�;
*						��ʼ����Ͳ����ٳ�ʼ��
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void attitudeControllerInit()
{
		if(isInit)
			return;

		pidInit(&pidRollRate, 0, PID_ROLL_RATE_KP, PID_ROLL_RATE_KI, PID_ROLL_RATE_KD, IMU_UPDATE_DT);
		pidInit(&pidPitchRate, 0, PID_PITCH_RATE_KP, PID_PITCH_RATE_KI, PID_PITCH_RATE_KD, IMU_UPDATE_DT);
		pidInit(&pidYawRate, 0, PID_YAW_RATE_KP, PID_YAW_RATE_KI, PID_YAW_RATE_KD, IMU_UPDATE_DT);
		pidSetIntegralLimit(&pidRollRate, PID_ROLL_RATE_INTEGRATION_LIMIT);
		pidSetIntegralLimit(&pidPitchRate, PID_PITCH_RATE_INTEGRATION_LIMIT);
		pidSetIntegralLimit(&pidYawRate, PID_YAW_RATE_INTEGRATION_LIMIT);

		pidInit(&pidRoll, 0, PID_ROLL_KP, PID_ROLL_KI, PID_ROLL_KD, IMU_UPDATE_DT);
		pidInit(&pidPitch, 0, PID_PITCH_KP, PID_PITCH_KI, PID_PITCH_KD, IMU_UPDATE_DT);
		pidInit(&pidYaw, 0, PID_YAW_KP, PID_YAW_KI, PID_YAW_KD, IMU_UPDATE_DT);
		pidSetIntegralLimit(&pidRoll, PID_ROLL_INTEGRATION_LIMIT);
		pidSetIntegralLimit(&pidPitch, PID_PITCH_INTEGRATION_LIMIT);
		pidSetIntegralLimit(&pidYaw, PID_YAW_INTEGRATION_LIMIT);

		isInit = true;
}
/*********************************************************************************************************
*	�� �� ��: attitudeControllerTest
*	����˵��: ���������Ƿ��ʼ��
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
bool attitudeControllerTest()
{
		return isInit;
}
/*********************************************************************************************************
*	�� �� ��: RollCorrectRatePID
*	����˵��: ���ٶȻ�PID������
*	��    �Σ�ʵ�ʽ��ٶȡ��������ٶ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void RollCorrectRatePID(float rollRateActual, float rollRateDesired)
{
		pidSetDesired(&pidRollRate, rollRateDesired);
		rollOutput = saturateSignedInt16(pidUpdate(&pidRollRate, rollRateActual, true));
}
/*********************************************************************************************************
*	�� �� ��: PitchCorrectRatePID
*	����˵��: ���ٶȻ�PID������
*	��    �Σ�ʵ�ʽ��ٶȡ��������ٶ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void PitchCorrectRatePID(float pitchRateActual, float pitchRateDesired)
{
		pidSetDesired(&pidPitchRate, pitchRateDesired);
		pitchOutput = saturateSignedInt16(pidUpdate(&pidPitchRate, pitchRateActual, true));
}
/*********************************************************************************************************
*	�� �� ��: YawCorrectRatePID
*	����˵��: ���ٶȻ�PID������
*	��    �Σ�ʵ�ʽ��ٶȡ��������ٶ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void YawCorrectRatePID(float yawRateActual, float yawRateDesired)
{
		pidSetDesired(&pidYawRate, yawRateDesired);
		yawOutput = saturateSignedInt16(pidUpdate(&pidYawRate, yawRateActual, true));
}
/*********************************************************************************************************
*	�� �� ��: RollCorrectAttitudePID
*	����˵��: �ǶȻ�PID������
*	��    �Σ�ʵ�ʽǶȡ������Ƕȡ�  �õ����������ٶ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void RollCorrectAttitudePID(float eulerRollActual, float eulerRollDesired, float* rollRateDesired)
{	
		// Update PID for roll axis
		pidSetDesired(&pidRoll, eulerRollDesired);
		*rollRateDesired = pidUpdate(&pidRoll, eulerRollActual, true);
	
}
/*********************************************************************************************************
*	�� �� ��: PitchCorrectAttitudePID
*	����˵��: �ǶȻ�PID������
*	��    �Σ�ʵ�ʽǶȡ������Ƕȡ�  �õ����������ٶ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void PitchCorrectAttitudePID(float eulerPitchActual, float eulerPitchDesired, float* pitchRateDesired)
{

		// Update PID for pitch axis
		pidSetDesired(&pidPitch, eulerPitchDesired);
		*pitchRateDesired = pidUpdate(&pidPitch, eulerPitchActual, true);

}
/*********************************************************************************************************
*	�� �� ��: YawCorrectAttitudePID
*	����˵��: �ǶȻ�PID������
*	��    �Σ�ʵ�ʽǶȡ������Ƕȡ�  �õ����������ٶ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void YawCorrectAttitudePID(float eulerYawActual, float eulerYawDesired, float* yawRateDesired)
{
		float yawError;
	
		// Update PID for yaw axis
		yawError = eulerYawDesired - eulerYawActual;
		if (yawError > 180.0)
			yawError -= 360.0;
		else if (yawError < -180.0)
			yawError += 360.0;
		pidSetError(&pidYaw, yawError);
		*yawRateDesired = pidUpdate(&pidYaw, eulerYawActual, false);
}
/*********************************************************************************************************
*	�� �� ��: attitudeControllerResetAllPID
*	����˵��: ����ǶȻ������ٶȻ�PID������
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void attitudeControllerResetAllPID(void)
{
		pidReset(&pidRoll);
		pidReset(&pidPitch);
		pidReset(&pidYaw);
		pidReset(&pidRollRate);
		pidReset(&pidPitchRate);
		pidReset(&pidYawRate);
}
/*********************************************************************************************************
*	�� �� ��: attitudeControllerGetActuatorOutput
*	����˵��: PID������ʵ�����
*	��    �Σ�*roll    *pitch   *yaw
*	�� �� ֵ: 
*********************************************************************************************************
*/
void attitudeControllerGetActuatorOutput(int16_t* roll, int16_t* pitch, int16_t* yaw)
{
		*roll = rollOutput;
		*pitch = pitchOutput;
		*yaw = yawOutput;
}

/*********************************************************************************************************
*	�� �� ��: RollStabilizer
*	����˵��: �����roll�Ŀ���
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void RollStabilizer(void)
{
		RollCorrectAttitudePID(euler.angle_x, angleDesired.x, &rateDesired.x);	
		RollCorrectRatePID(euler.gyro_x, rateDesired.x);
//		if(rollOutput > 0)
//		{
//			MOTOR_11_OUT = rollOutput;
//			MOTOR_12_OUT = 0;
//			MOTOR_21_OUT = rollOutput;
//			MOTOR_22_OUT = 0;
//		}
//		else
//		{
//			MOTOR_11_OUT = 0;
//			MOTOR_12_OUT = -rollOutput;
//			MOTOR_21_OUT = 0;
//			MOTOR_22_OUT = -rollOutput;
//		}
			//constrain
}
/*********************************************************************************************************
*	�� �� ��: PitchStabilizer
*	����˵��: ������pitch�Ŀ���
*	��    �Σ�
*	�� �� ֵ: 
*********************************************************************************************************
*/
void PitchStabilizer(void)
{
		PitchCorrectAttitudePID(euler.angle_y, angleDesired.y, &rateDesired.y);	
		PitchCorrectRatePID(euler.gyro_y, rateDesired.y);
//		if(rollOutput > 0)
//		{
//			MOTOR_11_OUT = pitchOutput;
//			MOTOR_12_OUT = 0;
//			MOTOR_21_OUT = pitchOutput;
//			MOTOR_22_OUT = 0;
//		}
//		else
//		{
//			MOTOR_11_OUT = 0;
//			MOTOR_12_OUT = -pitchOutput;
//			MOTOR_21_OUT = 0;
//			MOTOR_22_OUT = -pitchOutput;
//		}
			//constrain
}

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

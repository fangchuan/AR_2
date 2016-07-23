/*
*********************************************************************************************************
*
*	模块名称 : pid通用函数模块
*	文件名称 : attitude_pid_controller.c
*	版    本 : V1.0
*	说    明 : Attitude controler using PID correctors
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-07-21 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
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
*	函 数 名: saturateSignedInt16
*	功能说明: 判断有符号的16位整形是否溢出
*	形    参：float
*	返 回 值: 16位有符号整形值
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
*	函 数 名: attitudeControllerInit
*	功能说明: 姿态控制器参数初始化, 角度环、角速度环;
*						初始化后就不能再初始化
*	形    参：
*	返 回 值: 
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
*	函 数 名: attitudeControllerTest
*	功能说明: 检测控制器是否初始化
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
bool attitudeControllerTest()
{
		return isInit;
}
/*********************************************************************************************************
*	函 数 名: RollCorrectRatePID
*	功能说明: 角速度环PID控制器
*	形    参：实际角速度、期望角速度
*	返 回 值: 
*********************************************************************************************************
*/
void RollCorrectRatePID(float rollRateActual, float rollRateDesired)
{
		pidSetDesired(&pidRollRate, rollRateDesired);
		rollOutput = saturateSignedInt16(pidUpdate(&pidRollRate, rollRateActual, true));
}
/*********************************************************************************************************
*	函 数 名: PitchCorrectRatePID
*	功能说明: 角速度环PID控制器
*	形    参：实际角速度、期望角速度
*	返 回 值: 
*********************************************************************************************************
*/
void PitchCorrectRatePID(float pitchRateActual, float pitchRateDesired)
{
		pidSetDesired(&pidPitchRate, pitchRateDesired);
		pitchOutput = saturateSignedInt16(pidUpdate(&pidPitchRate, pitchRateActual, true));
}
/*********************************************************************************************************
*	函 数 名: YawCorrectRatePID
*	功能说明: 角速度环PID控制器
*	形    参：实际角速度、期望角速度
*	返 回 值: 
*********************************************************************************************************
*/
void YawCorrectRatePID(float yawRateActual, float yawRateDesired)
{
		pidSetDesired(&pidYawRate, yawRateDesired);
		yawOutput = saturateSignedInt16(pidUpdate(&pidYawRate, yawRateActual, true));
}
/*********************************************************************************************************
*	函 数 名: RollCorrectAttitudePID
*	功能说明: 角度环PID控制器
*	形    参：实际角度、期望角度、  得到的期望角速度
*	返 回 值: 
*********************************************************************************************************
*/
void RollCorrectAttitudePID(float eulerRollActual, float eulerRollDesired, float* rollRateDesired)
{	
		// Update PID for roll axis
		pidSetDesired(&pidRoll, eulerRollDesired);
		*rollRateDesired = pidUpdate(&pidRoll, eulerRollActual, true);
	
}
/*********************************************************************************************************
*	函 数 名: PitchCorrectAttitudePID
*	功能说明: 角度环PID控制器
*	形    参：实际角度、期望角度、  得到的期望角速度
*	返 回 值: 
*********************************************************************************************************
*/
void PitchCorrectAttitudePID(float eulerPitchActual, float eulerPitchDesired, float* pitchRateDesired)
{

		// Update PID for pitch axis
		pidSetDesired(&pidPitch, eulerPitchDesired);
		*pitchRateDesired = pidUpdate(&pidPitch, eulerPitchActual, true);

}
/*********************************************************************************************************
*	函 数 名: YawCorrectAttitudePID
*	功能说明: 角度环PID控制器
*	形    参：实际角度、期望角度、  得到的期望角速度
*	返 回 值: 
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
*	函 数 名: attitudeControllerResetAllPID
*	功能说明: 清零角度环、角速度环PID控制器
*	形    参：
*	返 回 值: 
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
*	函 数 名: attitudeControllerGetActuatorOutput
*	功能说明: PID控制器实际输出
*	形    参：*roll    *pitch   *yaw
*	返 回 值: 
*********************************************************************************************************
*/
void attitudeControllerGetActuatorOutput(int16_t* roll, int16_t* pitch, int16_t* yaw)
{
		*roll = rollOutput;
		*pitch = pitchOutput;
		*yaw = yawOutput;
}

/*********************************************************************************************************
*	函 数 名: RollStabilizer
*	功能说明: 横滚角roll的控制
*	形    参：
*	返 回 值: 
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
*	函 数 名: PitchStabilizer
*	功能说明: 俯仰角pitch的控制
*	形    参：
*	返 回 值: 
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

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

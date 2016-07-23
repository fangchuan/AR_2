/*
*********************************************************************************************************
*
*	模块名称 : MPU6050驱动模块
*	文件名称 : bsp_mpu6050.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#include "bsp_mpu6050.h"
#include "_apollorobot.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
extern _Euler  euler;

static uint8_t buffer[14];

static int16_t  MPU6050_FIFO[6][11];
static int16_t  Gx_offset=0,Gy_offset=0,Gz_offset=0;
static int16_t  MPU6050_Lastax,MPU6050_Lastay,MPU6050_Lastaz
							 ,MPU6050_Lastgx,MPU6050_Lastgy,MPU6050_Lastgz;

#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.01f                   // half the sample period采样周期的一半

static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
static float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static void MPU_INT_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(MPU_INT_PORT_CLK, ENABLE);			     
		GPIO_InitStructure.GPIO_Pin = MPU_INT_PIN ;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	//应用配置到GPIOB 
  	GPIO_Init(MPU_INT_PORT, &GPIO_InitStructure);
}
static void Delay_us(uint32_t n)
{
	     volatile uint32_t nus = n*14;
			 while(nus --);
}
/**************************实现函数********************************************
*函数原型:		void MPU6050_InitGyro_Offset(void)
*功　　能:	    读取 MPU6050的陀螺仪偏置
此时模块应该被静止放置。以测试静止时的陀螺仪输出
*******************************************************************************/
static void MPU6050_InitGyro_Offset(void)
{
		unsigned char i;
		int16_t temp[6];
		int32_t	tempgx=0,tempgy=0,tempgz=0;
		int32_t	tempax=0,tempay=0,tempaz=0;
		Gx_offset = 0;
		Gy_offset = 0;
		Gz_offset = 0;
		for(i=0;i<50;i++){
				Delay_us(100);
				MPU6050_getMotion6(&temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
				//LED_Change();
		}
		for(i=0;i<100;i++){
			Delay_us(200);
			MPU6050_getMotion6(&temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
			tempax += temp[0];
			tempay += temp[1];
			tempaz += temp[2];
			tempgx += temp[3];
			tempgy += temp[4];
			tempgz += temp[5];
			//LED_Change();
		}

		Gx_offset = tempgx/100;//MPU6050_FIFO[3][10];
		Gy_offset = tempgy/100;//MPU6050_FIFO[4][10];
		Gz_offset = tempgz/100;//MPU6050_FIFO[5][10];
		tempax/=100;
		tempay/=100;
		tempaz/=100;
}
/**************************实现函数********************************************
*函数原型:		void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
*功　　能:	    将新的ADC数据更新到 FIFO数组，进行滤波处理
*******************************************************************************/
static void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
{

		unsigned char i ;
		int32_t sum=0;
		for(i=1;i<10;i++){	//FIFO 操作
      MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
      MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
      MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
      MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
      MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
      MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
      }
      MPU6050_FIFO[0][9]=ax;//将新的数据放置到 数据的最后面
      MPU6050_FIFO[1][9]=ay;
      MPU6050_FIFO[2][9]=az;
      MPU6050_FIFO[3][9]=gx;
      MPU6050_FIFO[4][9]=gy;
      MPU6050_FIFO[5][9]=gz;

      sum=0;
      for(i=0;i<10;i++){	//求当前数组的合，再取平均值
          sum+=MPU6050_FIFO[0][i];
          }
      MPU6050_FIFO[0][10]=sum/10;

      sum=0;
      for(i=0;i<10;i++){
					sum+=MPU6050_FIFO[1][i];
          }
      MPU6050_FIFO[1][10]=sum/10;

      sum=0;
      for(i=0;i<10;i++){
					sum+=MPU6050_FIFO[2][i];
          }
      MPU6050_FIFO[2][10]=sum/10;

      sum=0;
      for(i=0;i<10;i++){
					sum+=MPU6050_FIFO[3][i];
      }
      MPU6050_FIFO[3][10]=sum/10;

      sum=0;
      for(i=0;i<10;i++){
          sum+=MPU6050_FIFO[4][i];
       }
      MPU6050_FIFO[4][10]=sum/10;

      sum=0;
      for(i=0;i<10;i++){
					sum+=MPU6050_FIFO[5][i];
      }
      MPU6050_FIFO[5][10]=sum/10;
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setClockSource(uint8_t source)
*功　　能:	    设置  MPU6050 的时钟源
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
static void MPU6050_setClockSource(uint8_t source){
    IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);

}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
static void MPU6050_setFullScaleGyroRange(uint8_t range) {
    IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*功　　能:	    设置  MPU6050 加速度计的最大量程
*******************************************************************************/
static void MPU6050_setFullScaleAccelRange(uint8_t range) {
    IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setSleepEnabled(uint8_t enabled)
*功　　能:	    设置  MPU6050 是否进入睡眠模式
				enabled =1   睡觉
			    enabled =0   工作
*******************************************************************************/
static void MPU6050_setSleepEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}
/**************************实现函数********************************************
*函数原型:		void MPU6050_resetDevice(uint8_t enabled)
*功　　能:	    设置  MPU6050 是否复位
				  enabled =1   睡觉
			    enabled =0   工作
*******************************************************************************/
static void MPU6050_resetDevice(uint8_t enabled)
{
	   IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, enabled);
}
/**************************实现函数********************************************
*函数原型:		uint8_t MPU6050_getDeviceID(void)
*功　　能:	    读取  MPU6050 WHO_AM_I 标识	 将返回 0x68
*******************************************************************************/
static uint8_t MPU6050_getDeviceID(void) {

    IICreadBytes(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
static void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
static void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}


/**************************实现函数********************************************
*函数原型:	   float invSqrt(float x)
*功　　能:	   快速计算 1/Sqrt(x) 	
输入参数： 要计算的值
输出参数： 结果
*******************************************************************************/
static float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/**************************实现函数********************************************
*函数原型:		uint8_t MPU6050_testConnection(void)
*功　　能:	    检测MPU6050 是否已经连接
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
			 return 1;
   	else 
			 return 0;
}


/**************************实现函数********************************************
*函数原型:		void MPU6050_initialize(void)
*功　　能:	    初始化 	MPU6050 以进入可用状态。
*******************************************************************************/
void MPU6050_initialize(void) {
	  int16_t temp[6];
	  unsigned char i;
    
		MPU_INT_GPIO_Init();
	  IIC_Init();
		MPU6050_resetDevice(1);	//复位MPU6050
		Delay_us(1000);
	  MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //设置时钟
    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//陀螺仪最大量程 +-2000度每秒 默认采样率1000hz 
    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//加速度度最大量程 +-2G       默认采样率1000hz
    MPU6050_setSleepEnabled(0); //进入工作状态
	  MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
	  MPU6050_setI2CBypassEnabled(1);	 //主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问HMC5883L
	  

	//配置MPU6050 的中断模式 和中断电平模式
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 0);
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, 0);
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, 1);
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, 1);
//	//开数据转换完成中断
//    IICwriteBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, 1);

			for(i=0;i<10;i++){//更新FIFO数组
					Delay_us(50);
					MPU6050_getMotion6(&temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
		}
																			 
		MPU6050_InitGyro_Offset();
}

/**************************实现函数********************************************
*函数原型:		unsigned char MPU6050_is_DRY(void)
*功　　能:	    检查 MPU6050的中断引脚，测试是否完成转换
返回 1  转换完成
0 数据寄存器还没有更新
*******************************************************************************/
u8 MPU6050_is_DRY(void)
{
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==Bit_SET){
			 return 1;
	 }
	 else
  		 return 0;
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
*功　　能:	    读取 MPU6050的当前测量值
*******************************************************************************/
void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
  
	//if(MPU6050_is_DRY()){
		IICreadBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
    MPU6050_Lastax=(((int16_t)buffer[0]) << 8) | buffer[1];
    MPU6050_Lastay=(((int16_t)buffer[2]) << 8) | buffer[3];
    MPU6050_Lastaz=(((int16_t)buffer[4]) << 8) | buffer[5];
	//跳过温度ADC
    MPU6050_Lastgx=(((int16_t)buffer[8]) << 8) | buffer[9];
    MPU6050_Lastgy=(((int16_t)buffer[10]) << 8) | buffer[11];
    MPU6050_Lastgz=(((int16_t)buffer[12]) << 8) | buffer[13];
		MPU6050_newValues(MPU6050_Lastax,MPU6050_Lastay,MPU6050_Lastaz,
											MPU6050_Lastgx,MPU6050_Lastgy,MPU6050_Lastgz);
		*ax  =MPU6050_FIFO[0][10];
		*ay  =MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10];
		*gx  =MPU6050_FIFO[3][10]-Gx_offset;
		*gy = MPU6050_FIFO[4][10]-Gy_offset;
		*gz = MPU6050_FIFO[5][10]-Gz_offset;
//	} else {
//	*ax = MPU6050_FIFO[0][10];//=MPU6050_FIFO[0][10];
//	*ay = MPU6050_FIFO[1][10];//=MPU6050_FIFO[1][10];
//	*az = MPU6050_FIFO[2][10];//=MPU6050_FIFO[2][10];
//	*gx = MPU6050_FIFO[3][10]-Gx_offset;//=MPU6050_FIFO[3][10];
//	*gy = MPU6050_FIFO[4][10]-Gy_offset;//=MPU6050_FIFO[4][10];
//	*gz = MPU6050_FIFO[5][10]-Gz_offset;//=MPU6050_FIFO[5][10];
//	}
}


void MPU6050_getlastMotion6(int16_t* ax, int16_t* ay, 
		int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{
		*ax = MPU6050_FIFO[0][10];
		*ay = MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10];
		*gx = MPU6050_FIFO[3][10]-Gx_offset;
		*gy = MPU6050_FIFO[4][10]-Gy_offset;
		*gz = MPU6050_FIFO[5][10]-Gz_offset;
}

/*
****直接通过加速计读到的数据计算倾角
*/
double MPU6050_Get_Angle(float x,float y,float z,unsigned char dir)//x,y,z为该方向重力加速度分量//dir为得到的角度
{
	double temp=0,res=0;
	switch(dir)
	{
		case 0: //与自然Z轴的角度
			temp=sqrt(x*x+y*y)/z;
			res=atan(temp);
			break;
		case 1: //与自然X轴的角度
			temp=x/sqrt(y*y+z*z);
			res=atan(temp);
			break;
		case 2: //与自然Y轴的角度
			temp=y/sqrt(x*x+z*z);
			res=atan(temp);
			break;

	}
	temp=(res*1800/3.14);//弧度制换算成角度制
	return temp;

}

#ifdef kalman

void kalman_init()
{
  TIM2_Init();   //20ms中断
}


//-------------------------------------------------------
//Kalman滤波，8MHz的处理时间约1.8ms；
//-------------------------------------------------------

//-------------------------------------------------------
//static xdata volatile float angle, angle_dot; 		//外部需要引用的变量

//Q是一个2 x2矩阵,代表了流程协方差噪声。在这种情况下,它表明我们多么相信测斜仪相对于陀螺仪
//R代表测量协方差噪声R = E(WT)。在本例中,它是一个1 x1矩阵,说,我们预计10 ras抖动从测斜仪。对于一个1 x1矩阵在这种情况下V = 0.1
//注意：dt的取值为kalman滤波器采样时间;
//const float Q_angle=0.001, Q_gyro=0.0015, R_angle=0.1, dt=0.02;
//Q_angle:三轴加速度的预测噪声，值越大就表示越不相信它
//Q_gyro：陀螺仪的预测噪声，值越大就表示越不相信它
//R_angle：测量噪声，值越大就表示你越不相信你测量到的数据，而越相信你预测的值。
//dt：采样时间
#define Q_angle 0.003
#define Q_gyro 0.0015
#define R_angle 0.5	
#define dt 0.02
#define C_0 1
static  volatile float kalman_P[2][2] = {{ 1, 0 },{ 0, 1 }};		
static  volatile float Pdot[4] ={0,0,0,0};	


static  volatile float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//-------------------------------------------------------
float Kalman_Filter(float angle_m,float gyro_m)			//gyro_m:gyro_measure
{
#ifdef kalman
static volatile float angle, angle_dot; 		//kalman后的角度和角速度。
#endif
	
	angle+=(gyro_m-q_bias) * dt;
	
	Pdot[0]=Q_angle - kalman_P[0][1] - kalman_P[1][0];
	Pdot[1]= - kalman_P[1][1];
	Pdot[2]= - kalman_P[1][1];
	Pdot[3]=Q_gyro;
	
	kalman_P[0][0] += Pdot[0] * dt;
	kalman_P[0][1] += Pdot[1] * dt;
	kalman_P[1][0] += Pdot[2] * dt;
	kalman_P[1][1] += Pdot[3] * dt;
	
	
	angle_err = angle_m - angle;//Z(K)-H*X(K|K-1)
	
	
	PCt_0 = C_0 * kalman_P[0][0];//测量系统的参数H=C_0=1,预测协方差P=1
	PCt_1 = C_0 * kalman_P[1][0];//测量系统的参数H=C_0=1,预测协方差P=0
	
	E = R_angle + C_0 * PCt_0;//H*P(K|K-1)*H'+R
	
	K_0 = PCt_0 / E;//增益Kg更新 Kg(K)=P(K|K-1)*H'/(H*P(K|K-1)*H'+R)
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * kalman_P[0][1];

	kalman_P[0][0] -= K_0 * t_0;//协方差矩阵更新  P(K|K)=(I-Kg(K)*H)*P(K|K-1)  I=[1,0,0,0
	kalman_P[0][1] -= K_0 * t_1;//                                               0,1,0,0
	kalman_P[1][0] -= K_1 * t_0;//                                               0,0,1,0
	kalman_P[1][1] -= K_1 * t_1;//                                               0,0,0,1]
	
	
	angle	+= K_0 * angle_err;//更新最优化angle、angle_dot ,X(K|K)=X(K|K-1) + Z(K)-H*X(K|K-1)
	q_bias	+= K_1 * angle_err;
	angle_dot = gyro_m-q_bias;	  
	
	return angle;
}


#endif

////////////////////////////////////////////////////////////////////////////////

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
			float norm;
		//  float hx, hy, hz, bx, bz;
			float vx, vy, vz;// wx, wy, wz;
			float ex, ey, ez;

			// 先把这些用得到的值算好
			float q0q0 = q0*q0;
			float q0q1 = q0*q1;
			float q0q2 = q0*q2;
		//  float q0q3 = q0*q3;
			float q1q1 = q1*q1;
		//  float q1q2 = q1*q2;
			float q1q3 = q1*q3;
			float q2q2 = q2*q2;
			float q2q3 = q2*q3;
			float q3q3 = q3*q3;
			
			if(ax*ay*az==0)
				return;
				
			norm = invSqrt(ax*ax + ay*ay + az*az);       //acc数据归一化
			ax = ax * norm;
			ay = ay * norm;
			az = az * norm;


			// estimated direction of gravity and flux (v and w)              估计重力方向和流量/变迁
			vx = 2*(q1q3 - q0q2);												//四元素中xyz的表示
			vy = 2*(q0q1 + q2q3);
			vz = q0q0 - q1q1 - q2q2 + q3q3 ;

			// error is sum of cross product between reference direction of fields and direction measured by sensors
			ex = (ay*vz - az*vy) ;                           					 //向量外积在相减得到差分就是误差
			ey = (az*vx - ax*vz) ;
			ez = (ax*vy - ay*vx) ;

			exInt = exInt + ex * Ki;								  //对误差进行积分
			eyInt = eyInt + ey * Ki;
			ezInt = ezInt + ez * Ki;

			// adjusted gyroscope measurements
			gx = gx + Kp*ex + exInt;					   							//将误差PI后补偿到陀螺仪，即补偿零点漂移
			gy = gy + Kp*ey + eyInt;
			gz = gz + Kp*ez + ezInt;				   							//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

			// integrate quaternion rate and normalise						   //四元素的微分方程
			q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
			q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
			q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
			q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

			// normalise quaternion
			norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
			q0 = q0 * norm;
			q1 = q1 * norm;
			q2 = q2 * norm;
			q3 = q3 * norm;

			//Q_ANGLE.Yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
			euler.angle_y  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
			euler.angle_x = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
			/*if(GYRO_I.Z>=360)GYRO_I.Z=0;
			Q_ANGLE.Z = GYRO_I.Z;*/
}

void Get_Attitude(void)
{
		int16_t ax,ay,az,gx,gy,gz;
		float   gx_rad,gy_rad,gz_rad;
		MPU6050_getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
		gx_rad = gx/GYRO_PARAM *RAD; 
		gy_rad = gy/GYRO_PARAM *RAD; 
		gz_rad = gz/GYRO_PARAM * RAD;
		euler.accel_x = ax / ACCEL_PARAM;
		euler.accel_y = ay / ACCEL_PARAM;
	  euler.accel_z = az / ACCEL_PARAM;
	  euler.gyro_x  = gx_rad;
	  euler.gyro_y  = gy_rad;
		euler.gyro_z  = gz_rad;
		IMUupdate(gx_rad,gy_rad,gz_rad,ax,ay,az);
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

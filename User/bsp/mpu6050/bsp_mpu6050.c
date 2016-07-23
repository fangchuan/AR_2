/*
*********************************************************************************************************
*
*	ģ������ : MPU6050����ģ��
*	�ļ����� : bsp_mpu6050.c
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
#define halfT 0.01f                   // half the sample period�������ڵ�һ��

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
  	//Ӧ�����õ�GPIOB 
  	GPIO_Init(MPU_INT_PORT, &GPIO_InitStructure);
}
static void Delay_us(uint32_t n)
{
	     volatile uint32_t nus = n*14;
			 while(nus --);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_InitGyro_Offset(void)
*��������:	    ��ȡ MPU6050��������ƫ��
��ʱģ��Ӧ�ñ���ֹ���á��Բ��Ծ�ֹʱ�����������
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
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
*��������:	    ���µ�ADC���ݸ��µ� FIFO���飬�����˲�����
*******************************************************************************/
static void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
{

		unsigned char i ;
		int32_t sum=0;
		for(i=1;i<10;i++){	//FIFO ����
      MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
      MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
      MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
      MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
      MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
      MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
      }
      MPU6050_FIFO[0][9]=ax;//���µ����ݷ��õ� ���ݵ������
      MPU6050_FIFO[1][9]=ay;
      MPU6050_FIFO[2][9]=az;
      MPU6050_FIFO[3][9]=gx;
      MPU6050_FIFO[4][9]=gy;
      MPU6050_FIFO[5][9]=gz;

      sum=0;
      for(i=0;i<10;i++){	//��ǰ����ĺϣ���ȡƽ��ֵ
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

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setClockSource(uint8_t source)
*��������:	    ����  MPU6050 ��ʱ��Դ
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

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*��������:	    ����  MPU6050 ���ٶȼƵ��������
*******************************************************************************/
static void MPU6050_setFullScaleAccelRange(uint8_t range) {
    IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setSleepEnabled(uint8_t enabled)
*��������:	    ����  MPU6050 �Ƿ����˯��ģʽ
				enabled =1   ˯��
			    enabled =0   ����
*******************************************************************************/
static void MPU6050_setSleepEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_resetDevice(uint8_t enabled)
*��������:	    ����  MPU6050 �Ƿ�λ
				  enabled =1   ˯��
			    enabled =0   ����
*******************************************************************************/
static void MPU6050_resetDevice(uint8_t enabled)
{
	   IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, enabled);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_getDeviceID(void)
*��������:	    ��ȡ  MPU6050 WHO_AM_I ��ʶ	 ������ 0x68
*******************************************************************************/
static uint8_t MPU6050_getDeviceID(void) {

    IICreadBytes(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
static void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
static void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:	   float invSqrt(float x)
*��������:	   ���ټ��� 1/Sqrt(x) 	
��������� Ҫ�����ֵ
��������� ���
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
/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_testConnection(void)
*��������:	    ���MPU6050 �Ƿ��Ѿ�����
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
			 return 1;
   	else 
			 return 0;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬��
*******************************************************************************/
void MPU6050_initialize(void) {
	  int16_t temp[6];
	  unsigned char i;
    
		MPU_INT_GPIO_Init();
	  IIC_Init();
		MPU6050_resetDevice(1);	//��λMPU6050
		Delay_us(1000);
	  MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //����ʱ��
    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//������������� +-2000��ÿ�� Ĭ�ϲ�����1000hz 
    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//���ٶȶ�������� +-2G       Ĭ�ϲ�����1000hz
    MPU6050_setSleepEnabled(0); //���빤��״̬
	  MPU6050_setI2CMasterModeEnabled(0);	 //����MPU6050 ����AUXI2C
	  MPU6050_setI2CBypassEnabled(1);	 //����������I2C��	MPU6050��AUXI2C	ֱͨ������������ֱ�ӷ���HMC5883L
	  

	//����MPU6050 ���ж�ģʽ ���жϵ�ƽģʽ
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 0);
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, 0);
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, 1);
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, 1);
//	//������ת������ж�
//    IICwriteBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, 1);

			for(i=0;i<10;i++){//����FIFO����
					Delay_us(50);
					MPU6050_getMotion6(&temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
		}
																			 
		MPU6050_InitGyro_Offset();
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char MPU6050_is_DRY(void)
*��������:	    ��� MPU6050���ж����ţ������Ƿ����ת��
���� 1  ת�����
0 ���ݼĴ�����û�и���
*******************************************************************************/
u8 MPU6050_is_DRY(void)
{
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==Bit_SET){
			 return 1;
	 }
	 else
  		 return 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
*��������:	    ��ȡ MPU6050�ĵ�ǰ����ֵ
*******************************************************************************/
void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
  
	//if(MPU6050_is_DRY()){
		IICreadBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
    MPU6050_Lastax=(((int16_t)buffer[0]) << 8) | buffer[1];
    MPU6050_Lastay=(((int16_t)buffer[2]) << 8) | buffer[3];
    MPU6050_Lastaz=(((int16_t)buffer[4]) << 8) | buffer[5];
	//�����¶�ADC
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
****ֱ��ͨ�����ټƶ��������ݼ������
*/
double MPU6050_Get_Angle(float x,float y,float z,unsigned char dir)//x,y,zΪ�÷����������ٶȷ���//dirΪ�õ��ĽǶ�
{
	double temp=0,res=0;
	switch(dir)
	{
		case 0: //����ȻZ��ĽǶ�
			temp=sqrt(x*x+y*y)/z;
			res=atan(temp);
			break;
		case 1: //����ȻX��ĽǶ�
			temp=x/sqrt(y*y+z*z);
			res=atan(temp);
			break;
		case 2: //����ȻY��ĽǶ�
			temp=y/sqrt(x*x+z*z);
			res=atan(temp);
			break;

	}
	temp=(res*1800/3.14);//�����ƻ���ɽǶ���
	return temp;

}

#ifdef kalman

void kalman_init()
{
  TIM2_Init();   //20ms�ж�
}


//-------------------------------------------------------
//Kalman�˲���8MHz�Ĵ���ʱ��Լ1.8ms��
//-------------------------------------------------------

//-------------------------------------------------------
//static xdata volatile float angle, angle_dot; 		//�ⲿ��Ҫ���õı���

//Q��һ��2 x2����,����������Э���������������������,���������Ƕ�ô���Ų�б�������������
//R�������Э��������R = E(WT)���ڱ�����,����һ��1 x1����,˵,����Ԥ��10 ras�����Ӳ�б�ǡ�����һ��1 x1���������������V = 0.1
//ע�⣺dt��ȡֵΪkalman�˲�������ʱ��;
//const float Q_angle=0.001, Q_gyro=0.0015, R_angle=0.1, dt=0.02;
//Q_angle:������ٶȵ�Ԥ��������ֵԽ��ͱ�ʾԽ��������
//Q_gyro�������ǵ�Ԥ��������ֵԽ��ͱ�ʾԽ��������
//R_angle������������ֵԽ��ͱ�ʾ��Խ������������������ݣ���Խ������Ԥ���ֵ��
//dt������ʱ��
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
static volatile float angle, angle_dot; 		//kalman��ĽǶȺͽ��ٶȡ�
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
	
	
	PCt_0 = C_0 * kalman_P[0][0];//����ϵͳ�Ĳ���H=C_0=1,Ԥ��Э����P=1
	PCt_1 = C_0 * kalman_P[1][0];//����ϵͳ�Ĳ���H=C_0=1,Ԥ��Э����P=0
	
	E = R_angle + C_0 * PCt_0;//H*P(K|K-1)*H'+R
	
	K_0 = PCt_0 / E;//����Kg���� Kg(K)=P(K|K-1)*H'/(H*P(K|K-1)*H'+R)
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * kalman_P[0][1];

	kalman_P[0][0] -= K_0 * t_0;//Э����������  P(K|K)=(I-Kg(K)*H)*P(K|K-1)  I=[1,0,0,0
	kalman_P[0][1] -= K_0 * t_1;//                                               0,1,0,0
	kalman_P[1][0] -= K_1 * t_0;//                                               0,0,1,0
	kalman_P[1][1] -= K_1 * t_1;//                                               0,0,0,1]
	
	
	angle	+= K_0 * angle_err;//�������Ż�angle��angle_dot ,X(K|K)=X(K|K-1) + Z(K)-H*X(K|K-1)
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

			// �Ȱ���Щ�õõ���ֵ���
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
				
			norm = invSqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
			ax = ax * norm;
			ay = ay * norm;
			az = az * norm;


			// estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
			vx = 2*(q1q3 - q0q2);												//��Ԫ����xyz�ı�ʾ
			vy = 2*(q0q1 + q2q3);
			vz = q0q0 - q1q1 - q2q2 + q3q3 ;

			// error is sum of cross product between reference direction of fields and direction measured by sensors
			ex = (ay*vz - az*vy) ;                           					 //�������������õ���־������
			ey = (az*vx - ax*vz) ;
			ez = (ax*vy - ay*vx) ;

			exInt = exInt + ex * Ki;								  //�������л���
			eyInt = eyInt + ey * Ki;
			ezInt = ezInt + ez * Ki;

			// adjusted gyroscope measurements
			gx = gx + Kp*ex + exInt;					   							//�����PI�󲹳��������ǣ����������Ư��
			gy = gy + Kp*ey + eyInt;
			gz = gz + Kp*ez + ezInt;				   							//�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

			// integrate quaternion rate and normalise						   //��Ԫ�ص�΢�ַ���
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

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

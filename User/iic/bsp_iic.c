/*
*********************************************************************************************************
*
*	ģ������ : ��Դ����������ģ��
*	�ļ����� : bsp_iic.c
*	��    �� : V1.0
*	˵    �� : �ṩI2C�ӿڲ���API ,ʹ��IOģ�ⷽʽ
*						 PB10---SCL,PB11---SDA
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-06-30 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/

#include "bsp_iic.h"
  
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/


/*********************************************************************
*
*       Static Code
*
**********************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ��������AX-Pro�߼������ǲ��Եõ��ġ�
		CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
        
    IAR���̱���Ч�ʸߣ���������Ϊ7
	*/
	for (i = 0; i < 10; i++);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Start(void)
*��������:		����IIC��ʼ�ź�
*******************************************************************************/
static void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	i2c_Delay();
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	i2c_Delay();
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/	  
static void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	i2c_Delay();
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	i2c_Delay();							   	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
static u8 IIC_Wait_Ack(void)
{
			u8 ucErrTime=0;
			SDA_IN();      //SDA����Ϊ����  
			IIC_SDA=1;i2c_Delay();	   
			IIC_SCL=1;i2c_Delay();	 
			while(READ_SDA)
			{
				ucErrTime++;
				if(ucErrTime>50)
				{
					IIC_Stop();
					return 1;
				}
				i2c_Delay();
			}
			IIC_SCL=0;//ʱ�����0 	   
			return 0;  
} 


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
static void IIC_Ack(void)
{
		IIC_SCL=0;
		SDA_OUT();
		IIC_SDA=0;
		i2c_Delay();
		IIC_SCL=1;
		i2c_Delay();
		IIC_SCL=0;
}
	
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/	    
static void IIC_NAck(void)
{
		IIC_SCL=0;
		SDA_OUT();
		IIC_SDA=1;
		i2c_Delay();
		IIC_SCL=1;
		i2c_Delay();
		IIC_SCL=0;
}					 				     

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Send_Byte(u8 txd)
*��������:	    IIC����һ���ֽ�
*******************************************************************************/		  
static void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
				i2c_Delay();   
				IIC_SCL=1;
				i2c_Delay(); 
				IIC_SCL=0;	
				i2c_Delay();
    }	 
} 	 
   
/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/  
static u8 IIC_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
		{
        IIC_SCL=0; 
        i2c_Delay();
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
				i2c_Delay(); 
    }					 
    if (ack)
        IIC_Ack(); //����ACK 
    else
        IIC_NAck();//����nACK  
    return receive;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
static  u8 I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
			unsigned char res=0;
			
			IIC_Start();	
			IIC_Send_Byte(I2C_Addr);	   //����д����
			res++;
			IIC_Wait_Ack();
			IIC_Send_Byte(addr); res++;  //���͵�ַ
			IIC_Wait_Ack();	  
			//IIC_Stop();//����һ��ֹͣ����	
			IIC_Start();
			IIC_Send_Byte(I2C_Addr+1); res++;          //�������ģʽ			   
			IIC_Wait_Ack();
			res=IIC_Read_Byte(0);	   
				IIC_Stop();//����һ��ֹͣ����

			return res;
}

/*********************************************************************
*
*       Public Code
*
**********************************************************************
*/
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Init(void)
*��������:		��ʼ��I2C��Ӧ�Ľӿ����š�
*******************************************************************************/
void IIC_Init(void)
{			
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(IIC_PORT_CLK, ENABLE);			     
		GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	//Ӧ�����õ�GPIOB 
  	GPIO_Init(IIC_PORT, &GPIO_InitStructure);
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫ�����ֽ���
		*data  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/ 
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
		IIC_Start();
		IIC_Send_Byte(dev);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(reg);   //���͵�ַ
    IIC_Wait_Ack();	  
		IIC_Start();
		IIC_Send_Byte(dev+1);  //�������ģʽ	
		IIC_Wait_Ack();
		
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //��ACK�Ķ�����
		 	else  data[count]=IIC_Read_Byte(0);	 //���һ���ֽ�NACK
		}
    IIC_Stop();//����һ��ֹͣ����
    return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫд���ֽ���
		*data  ��Ҫд�����ݵ��׵�ַ
����   �����Ƿ�ɹ�
*******************************************************************************/ 
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data){
  
		u8 count = 0;
		IIC_Start();
		IIC_Send_Byte(dev);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(reg);   //���͵�ַ
		IIC_Wait_Ack();	  
		for(count=0;count<length;count++){
			IIC_Send_Byte(data[count]); 
			IIC_Wait_Ack(); 
		}
		IIC_Stop();//����һ��ֹͣ����

    return 1; //status == 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
u8 IICreadByte(u8 dev, u8 reg, u8 *data){
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		data  ��Ҫд����ֽ�
����   1
*******************************************************************************/ 
u8 IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data){
    return IICwriteBytes(dev, reg, 1, &data);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitStart  Ŀ���ֽڵ���ʼλ
		length   λ����
		data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b;
    if (IICreadByte(dev, reg, &b) != 0) {
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IICwriteByte(dev, reg, b);
    } else {
        return 0;
    }
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
    u8 b;
    IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IICwriteByte(dev, reg, b);
}

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

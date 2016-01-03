#ifndef __IOI2C_H
#define __IOI2C_H
#include "stm32f10x.h"
#include "IO_Operation.h"

#define  IIC_PORT_CLK   RCC_APB2Periph_GPIOB
#define  IIC_PORT       GPIOB
#define  IIC_SCL_PIN    GPIO_Pin_10
#define  IIC_SDA_PIN    GPIO_Pin_11

//IO��������
#define SDA_IN()  {GPIOB->CRH &= 0xffff0fff;GPIOB->CRH |= 0x00008000;}
#define SDA_OUT() {GPIOB->CRH &= 0xffff0fff;GPIOB->CRH |= 0x00003000;}


//IO��������	 
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //����SDA 

//IIC���в�������
extern void IIC_Init(void);                //��ʼ��IIC��IO��				 
extern u8 IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
extern u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
extern u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
extern u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
extern u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

#endif

//------------------End of File----------------------------

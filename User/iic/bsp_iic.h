/*
*********************************************************************************************************
*
*	模块名称 : 无源蜂鸣器驱动模块
*	文件名称 : bsp_iic.h
*	版    本 : V1.0
*	说    明 : 提供I2C接口操作API ,使用IO模拟方式
*						 PB10---SCL,PB11---SDA
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef __IOI2C_H
#define __IOI2C_H
#include "stm32f10x.h"
#include "IO_Operation.h"

#define  IIC_PORT_CLK   RCC_APB2Periph_GPIOB
#define  IIC_PORT       GPIOB
#define  IIC_SCL_PIN    GPIO_Pin_10
#define  IIC_SDA_PIN    GPIO_Pin_11

//IO方向设置
#define SDA_IN()  {GPIOB->CRH &= 0xffff0fff;GPIOB->CRH |= 0x00008000;}
#define SDA_OUT() {GPIOB->CRH &= 0xffff0fff;GPIOB->CRH |= 0x00003000;}


//IO操作函数	 
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //输入SDA 

//IIC所有操作函数
extern void IIC_Init(void);                //初始化IIC的IO口				 
extern u8 IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
extern u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
extern u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
extern u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
extern u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

#endif

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

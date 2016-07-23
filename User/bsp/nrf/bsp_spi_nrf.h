/*
*********************************************************************************************************
*
*	ģ������ : NRF24L01 bspģ��
*	�ļ����� : bsp_spi_nrf.h
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
#ifndef __SPI_NRF_H
#define __SPI_NRF_H

#include "stm32f10x.h"

//24L01�������
#define CHANAL 40	//Ƶ��ѡ�� 

// SPI(nRF24L01) commands ,	NRF��SPI����궨�壬���NRF����ʹ���ĵ�
#define NRF_READ_REG    0x00  // Define read command to register
#define NRF_WRITE_REG   0x20  // Define write command to register
#define RD_RX_PLOAD 0x61  // Define RX payload register address
#define WR_TX_PLOAD 0xA0  // Define TX payload register address
#define FLUSH_TX    0xE1  // Define flush TX register command
#define FLUSH_RX    0xE2  // Define flush RX register command
#define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
#define NOP         0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses) ��NRF24L01 ��ؼĴ�����ַ�ĺ궨��
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define CD          0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address

#define MAX_RT      0x10 //�ﵽ����ط������жϱ�־λ
#define TX_DS		0x20 //��������жϱ�־λ	  // 

#define RX_DR		0x40 //���յ������жϱ�־λ

//������������
typedef struct _NRF_PKT {
	
				uint8_t car_speed;   //С��ǰ�����Ǻ���
	      uint8_t car_angle;   //С����ת������ת
	      uint8_t key_value;   //��ֵ
	      int X_angle;         //MPU6050   roll
				int Y_angle;         //MPU6050   pitch
	      
}_nrf_pkt;
#define  NRF_ROCKER_FORWARD   1
#define  NRF_ROCKER_BACKWARD  255
#define  NRF_ROCKER_LEFT      1
#define  NRF_ROCKER_RIGHT     255
#define  NRF_STOP             0
#define  NRF_KEY_FORWARD      5
#define  NRF_KEY_BACKWARD     8
#define  NRF_KEY_LEFT         6
#define  NRF_KEY_RIGHT        7
#define  NRF_EULER_THRE       15   //���ģʽ�µ���ֵ��15��
#define  NRF_EULER_SAFE       10   //���ģʽ�µİ�ȫֵ��10��
//
#define ADR_WIDTH 	5  	//�����ַ���
#define TX_PLOAD_WIDTH  sizeof(_nrf_pkt)   //��������ͨ����Ч���ݿ��0~32Byte 
#define RX_PLOAD_WIDTH  sizeof(_nrf_pkt)

#define NRF_CE_PIN          GPIO_Pin_2
#define NRF_CE_PORT         GPIOD
#define NRF_CS_PIN          GPIO_Pin_3
#define NRF_CS_PORT         GPIOD
#define NRF_SPI             SPI1
#define NRF_SPI_CLK         RCC_APB2Periph_SPI1
#define NRF_SPI_SCK         GPIO_Pin_5
#define NRF_SPI_MISO        GPIO_Pin_6
#define NRF_SPI_MOSI        GPIO_Pin_7
#define NRF_SPI_PORT        GPIOA
#define NRF_IRQ_PIN         GPIO_Pin_4
#define NRF_IRQ_PORT        GPIOA
#define NRF_PORT_CLK        (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD)

#define NRF_CSN_HIGH()      GPIO_SetBits(NRF_CS_PORT, NRF_CS_PIN)
#define NRF_CSN_LOW()       GPIO_ResetBits(NRF_CS_PORT, NRF_CS_PIN)		        //csn�õ�
#define NRF_CE_HIGH()	      GPIO_SetBits(NRF_CE_PORT,NRF_CE_PIN)
#define NRF_CE_LOW()	      GPIO_ResetBits(NRF_CE_PORT,NRF_CE_PIN)			      //CE�õ�
#define NRF_Read_IRQ()		  GPIO_ReadInputDataBit( NRF_IRQ_PORT, NRF_IRQ_PIN)  //�ж�����

extern void SPI_NRF_Init(void);
extern void NRF_TX_Mode(void);
extern void NRF_RX_Mode(void);
extern u8 NRF_Rx_Dat(u8 *rxbuf);
extern u8 NRF_Tx_Dat(u8 *txbuf);
extern u8 NRF_Check(void); 

#endif /* __SPI_NRF_H */
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

/*
*********************************************************************************************************
*
*	ģ������ : ����ͨ��ģ��
*	�ļ����� : bsp_usart1.h
*	��    �� : V1.0
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-07-21 ����  ��ʽ����
*
*	Copyright (C), 2015-2020, �����޿Ƽ� www.apollorobot.cn
*
*********************************************************************************************************
*/
#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define USART1_TX    GPIO_Pin_9
#define USART1_RX    GPIO_Pin_10
#define USART1_PORT  GPIOA
#define RCC_USART1_PORT  RCC_APB2Periph_GPIOA   
#define BAUD_RATE    115200
#define NVIC_USART1_PP  2
#define NVIC_USART1_SP  2

#define UART1_BAUD			115200
#define UART1_TX_BUF_SIZE	19
#define UART1_RX_BUF_SIZE	19


/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
//	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
//	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
//	__IO uint16_t usTxWrite;	/* ���ͻ�����дָ�� */
//	__IO uint16_t usTxRead;		/* ���ͻ�������ָ�� */
//	__IO uint16_t usTxCount;	/* �ȴ����͵����ݸ��� */

	__IO uint16_t usRxWrite;	/* ���ջ�����дָ�� */
	__IO uint16_t usRxRead;		/* ���ջ�������ָ�� */
	__IO uint16_t usRxCount;	/* ��δ��ȡ�������ݸ��� */

//	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
//	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
//	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
}UART_T;


void USART1_Config(void);

#endif /* __USART1_H */
/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

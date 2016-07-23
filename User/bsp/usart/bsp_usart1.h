/*
*********************************************************************************************************
*
*	模块名称 : 串口通信模块
*	文件名称 : bsp_usart1.h
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-07-21 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
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


/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
//	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
//	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
//	__IO uint16_t usTxWrite;	/* 发送缓冲区写指针 */
//	__IO uint16_t usTxRead;		/* 发送缓冲区读指针 */
//	__IO uint16_t usTxCount;	/* 等待发送的数据个数 */

	__IO uint16_t usRxWrite;	/* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;		/* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;	/* 还未读取的新数据个数 */

//	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
//	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
//	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_T;


void USART1_Config(void);

#endif /* __USART1_H */
/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

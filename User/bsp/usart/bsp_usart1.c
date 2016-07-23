/*
*********************************************************************************************************
*
*	模块名称 : 串口通信模块
*	文件名称 : bsp_usart1.c
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
  
#include "bsp_usart1.h"
#include "os.h"
#include "string.h"

uint8_t  UartRxData[UART1_RX_BUF_SIZE];           //接收到的数据
static UART_T g_tUart1;
//static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];	/* 发送缓冲区 */
static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */

//
//配置USART1的中断优先级
//抢占优先级:2
//子优先级： 2
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_USART1_PP;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_USART1_SP;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 
static void InitUartVar(UART_T* uart)
{
	    uart->uart = USART1;
	    uart->pRxBuf = g_RxBuf1;              /* 接收缓冲区指针 */
			uart->usRxBufSize = UART1_RX_BUF_SIZE;   /* 接收缓冲区大小 */
	    uart->usRxCount = 0;        /* 接收到的新数据个数 */
	    uart->usRxRead = 0;         /* 接收FIFO读索引 */
	    uart->usRxWrite = 0;        /* 接收FIFO写索引 */
}
/**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_USART1_PORT, ENABLE);
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = USART1_TX;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = USART1_RX;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(USART1_PORT, &GPIO_InitStructure);
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = BAUD_RATE;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		USART_Cmd(USART1, ENABLE);
		
		ConfigUartNVIC();
		
		InitUartVar(&g_tUart1);
}

/*
*********************************************************************************************************
*	函 数 名: UartSend
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
//static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
//{
//	uint16_t i;

//	CPU_SR_ALLOC();

//	for (i = 0; i < _usLen; i++)
//	{
//		/* 如果发送缓冲区已经满了，则等待缓冲区空 */
//		/* 当 _pUart->usTxBufSize == 1 时, 下面的函数会死掉(待完善) */
//		while (1)
//		{
//			__IO uint16_t usCount;

//			CPU_CRITICAL_ENTER();
//			usCount = _pUart->usTxCount;
//			CPU_CRITICAL_EXIT();

//			if (usCount < _pUart->usTxBufSize)
//			{
//				break;
//			}
//		}

//		/* 将新数据填入发送缓冲区 */
//		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

//		CPU_CRITICAL_ENTER();
//		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
//		{
//			_pUart->usTxWrite = 0;
//		}
//		_pUart->usTxCount++;
//		CPU_CRITICAL_EXIT();
//	}

//	USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);
//}

/*
*********************************************************************************************************
*	函 数 名: UartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	CPU_SR_ALLOC();

	/* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
	CPU_CRITICAL_ENTER();
	usCount = _pUart->usRxCount;
	CPU_CRITICAL_EXIT();

	/* 如果读和写索引相同，则返回0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* 已经没有数据 */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* 从串口接收FIFO取1个数据 */

		/* 改写FIFO读索引 */
		CPU_CRITICAL_ENTER();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		CPU_CRITICAL_EXIT();
		return 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart, uint8_t *rxbuf)
{
	/* 处理接收中断  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		/* 从串口接收数据寄存器读取数据存放到接收FIFO */
		uint8_t ch;

		ch = USART_ReceiveData(_pUart->uart);
		if(ch != 0xBB)
		{
				_pUart->pRxBuf[_pUart->usRxWrite] = ch;
				if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
				{
					_pUart->usRxWrite = 0;
				}
				if (_pUart->usRxCount < _pUart->usRxBufSize)
				{
					_pUart->usRxCount++;
				}
		}
		else
		{
			 _pUart->pRxBuf[_pUart->usRxWrite] = ch;
			 _pUart->usRxWrite = 0;
			 if (_pUart->usRxCount < _pUart->usRxBufSize)
			 {
					_pUart->usRxCount++;
			 }
			 memcpy(rxbuf,_pUart->pRxBuf,_pUart->usRxCount);
			 _pUart->usRxCount -= _pUart->usRxWrite;
		}
	}

}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{

	  CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
	
	  UartIRQ(&g_tUart1, UartRxData);

	  OSIntExit(); 

}

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

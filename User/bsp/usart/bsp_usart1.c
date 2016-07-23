/*
*********************************************************************************************************
*
*	ģ������ : ����ͨ��ģ��
*	�ļ����� : bsp_usart1.c
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
  
#include "bsp_usart1.h"
#include "os.h"
#include "string.h"

uint8_t  UartRxData[UART1_RX_BUF_SIZE];           //���յ�������
static UART_T g_tUart1;
//static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];	/* ���ͻ����� */
static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */

//
//����USART1���ж����ȼ�
//��ռ���ȼ�:2
//�����ȼ��� 2
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ʹ�ܴ���1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_USART1_PP;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_USART1_SP;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 
static void InitUartVar(UART_T* uart)
{
	    uart->uart = USART1;
	    uart->pRxBuf = g_RxBuf1;              /* ���ջ�����ָ�� */
			uart->usRxBufSize = UART1_RX_BUF_SIZE;   /* ���ջ�������С */
	    uart->usRxCount = 0;        /* ���յ��������ݸ��� */
	    uart->usRxRead = 0;         /* ����FIFO������ */
	    uart->usRxWrite = 0;        /* ����FIFOд���� */
}
/**
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
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
*	�� �� ��: UartSend
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
//{
//	uint16_t i;

//	CPU_SR_ALLOC();

//	for (i = 0; i < _usLen; i++)
//	{
//		/* ������ͻ������Ѿ����ˣ���ȴ��������� */
//		/* �� _pUart->usTxBufSize == 1 ʱ, ����ĺ���������(������) */
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

//		/* �����������뷢�ͻ����� */
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
*	�� �� ��: UartGetChar
*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
*	��    ��: _pUart : �����豸
*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	CPU_SR_ALLOC();

	/* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
	CPU_CRITICAL_ENTER();
	usCount = _pUart->usRxCount;
	CPU_CRITICAL_EXIT();

	/* �������д������ͬ���򷵻�0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* �Ѿ�û������ */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* �Ӵ��ڽ���FIFOȡ1������ */

		/* ��дFIFO������ */
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
*	�� �� ��: UartIRQ
*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
*	��    ��: _pUart : �����豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart, uint8_t *rxbuf)
{
	/* ��������ж�  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
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
*	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	����˵��: USART�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
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

///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/

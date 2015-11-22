/**
  ******************************************************************************
  * @file FSMC/SRAM/fsmc_sram.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  This file provides a set of functions needed to drive the 
  *         IS61WV51216BLL SRAM memory mounted on STM3210E-EVAL board.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "bsp_fsmc_sram.h"

/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup FSMC_SRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configures the FSMC and GPIOs to interface with the SRAM memory.
  *         This function must be called before any write/read operation
  *         on the SRAM.
  * @param  None 
  * @retval : None
  */
//��ʼ���ⲿSRAM
void FSMC_SRAM_Init(void)
{			 			    
	RCC->AHBENR|=1<<8;       //ʹ��FSMCʱ��	  
	RCC->APB2ENR|=1<<5;      //ʹ��PORTDʱ��
	RCC->APB2ENR|=1<<6;      //ʹ��PORTEʱ��
 	RCC->APB2ENR|=1<<7;      //ʹ��PORTFʱ��
	RCC->APB2ENR|=1<<8;      //ʹ��PORTGʱ��	   

	//PORTD����������� 	
	GPIOD->CRH&=0X00000000;
	GPIOD->CRH|=0XBBBBBBBB; 
	GPIOD->CRL&=0XFF00FF00;
	GPIOD->CRL|=0X00BB00BB;   	 
	//PORTE����������� 	
	GPIOE->CRH&=0X00000000;
	GPIOE->CRH|=0XBBBBBBBB; 
	GPIOE->CRL&=0X0FFFFF00;
	GPIOE->CRL|=0XB00000BB; 
	//PORTF�����������
	GPIOF->CRH&=0X0000FFFF;
	GPIOF->CRH|=0XBBBB0000;  	    	 											 
	GPIOF->CRL&=0XFF000000;
	GPIOF->CRL|=0X00BBBBBB; 

	//PORTG����������� PG10->NE3     	 											 
	GPIOG->CRH&=0XFFFFF0FF;
	GPIOG->CRH|=0X00000B00;  
 	GPIOG->CRL&=0XFF000000;
	GPIOG->CRL|=0X00BBBBBB;  	 				  
  
	//�Ĵ�������
	//bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����
	//��������ʹ��NE3 ��Ҳ�Ͷ�ӦBTCR[4],[5]��				    
	FSMC_Bank1->BTCR[4]=0X00000000;
	FSMC_Bank1->BTCR[5]=0X00000000;
	FSMC_Bank1E->BWTR[4]=0X00000000;
	//����BCR�Ĵ���	ʹ���첽ģʽ,ģʽA(��д����һ��ʱ��Ĵ���)
	//BTCR[ż��]:BCR�Ĵ���;BTCR[����]:BTR�Ĵ���
	FSMC_Bank1->BTCR[4]|=1<<12;//�洢��дʹ��
	FSMC_Bank1->BTCR[4]|=1<<4; //�洢�����ݿ��Ϊ16bit 	    
	//����BTR�Ĵ���								    
	FSMC_Bank1->BTCR[5]|=3<<8; //���ݱ���ʱ�䣨DATAST��Ϊ3��HCLK 4/72M=55ns(��EM��SRAMоƬ)	 	 
	FSMC_Bank1->BTCR[5]|=0<<4; //��ַ����ʱ�䣨ADDHLD��δ�õ�	  	 
	FSMC_Bank1->BTCR[5]|=0<<0; //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns	 	 
	//����дʱ��Ĵ���  
	FSMC_Bank1E->BWTR[4]=0x0FFFFFFF;//Ĭ��ֵ
	//ʹ��BANK1����3
	FSMC_Bank1->BTCR[4]|=1<<0; 												
} 
/**
  * @brief  Writes a Half-word buffer to the FSMC SRAM memory. 
  * @param pBuffer : pointer to buffer. 
  * @param WriteAddr : SRAM memory internal address from which the data 
  *        will be written.
  * @param NumHalfwordToWrite : number of half-words to write. 
  * @retval : None
  */
void FSMC_SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(uint16_t *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr += 2;
  }   
}

/**
  * @brief  Reads a block of data from the FSMC SRAM memory.
  * @param pBuffer : pointer to the buffer that receives the data read 
  *        from the SRAM memory.
  * @param ReadAddr : SRAM memory internal address to read from.
  * @param NumHalfwordToRead : number of half-words to read.
  * @retval : None
  */
void FSMC_SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(__IO uint16_t*) (Bank1_SRAM3_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr += 2;
  }  
}

/*-----------------------------------------------------------------------------*/

void SRAM_Test(void)
{
  
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

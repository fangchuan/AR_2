/*
*********************************************************************************************************
*
*	模块名称 : FSMC驱动SRAM模块
*	文件名称 : bsp_fsmc_sram.c
*	版    本 : V1.0
*	说    明 : 配置FSMC,将外部SRAM挂到BANK1的第三块
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-03-01 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.com
*
*********************************************************************************************************
*/
#include "bsp_fsmc_sram.h"

/*
*********************************************************************************************************
*	函 数 名: FSMC_SRAM_Init
*	功能说明: Configures the FSMC and GPIOs to interface with the SRAM memory.
*         	This function must be called before any write/read operation
*         	on the SRAM.
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
//初始化外部SRAM
void FSMC_SRAM_Init(void)
{			 			    
		RCC->AHBENR|=1<<8;       //使能FSMC时钟	  
		RCC->APB2ENR|=1<<5;      //使能PORTD时钟
		RCC->APB2ENR|=1<<6;      //使能PORTE时钟
		RCC->APB2ENR|=1<<7;      //使能PORTF时钟
		RCC->APB2ENR|=1<<8;      //使能PORTG时钟	   

		//PORTD复用推挽输出 	
		GPIOD->CRH&=0X00000000;
		GPIOD->CRH|=0XBBBBBBBB; 
		GPIOD->CRL&=0XFF00FF00;
		GPIOD->CRL|=0X00BB00BB;   	 
		//PORTE复用推挽输出 	
		GPIOE->CRH&=0X00000000;
		GPIOE->CRH|=0XBBBBBBBB; 
		GPIOE->CRL&=0X0FFFFF00;
		GPIOE->CRL|=0XB00000BB; 
		//PORTF复用推挽输出
		GPIOF->CRH&=0X0000FFFF;
		GPIOF->CRH|=0XBBBB0000;  	    	 											 
		GPIOF->CRL&=0XFF000000;
		GPIOF->CRL|=0X00BBBBBB; 

		//PORTG复用推挽输出 PG10->NE3     	 											 
		GPIOG->CRH&=0XFFFFF0FF;
		GPIOG->CRH|=0X00000B00;  
		GPIOG->CRL&=0XFF000000;
		GPIOG->CRL|=0X00BBBBBB;  	 				  
		
		//寄存器清零
		//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
		//这里我们使用NE3 ，也就对应BTCR[4],[5]。				    
		FSMC_Bank1->BTCR[4]=0X00000000;
		FSMC_Bank1->BTCR[5]=0X00000000;
		FSMC_Bank1E->BWTR[4]=0X00000000;
		//操作BCR寄存器	使用异步模式,模式A(读写共用一个时序寄存器)
		//BTCR[偶数]:BCR寄存器;BTCR[奇数]:BTR寄存器
		FSMC_Bank1->BTCR[4]|=1<<12;//存储器写使能
		FSMC_Bank1->BTCR[4]|=1<<4; //存储器数据宽度为16bit 	  
		//操作BTR寄存器								    
		FSMC_Bank1->BTCR[5]|=3<<8; //数据保持时间（DATAST）为4个HCLK 4/72M=55ns(对EM的SRAM芯片)	 	 
		FSMC_Bank1->BTCR[5]|=0<<4; //地址保持时间（ADDHLD）未用到	  	 
		FSMC_Bank1->BTCR[5]|=0<<0; //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns	 	 
		//闪存写时序寄存器  
		FSMC_Bank1E->BWTR[4]=0x0FFFFFFF;//默认值
		//使能BANK1区域3
		FSMC_Bank1->BTCR[4]|=1<<0; 				
		
} 
/*
*********************************************************************************************************
*	函 数 名: FSMC_SRAM_WriteBuffer
*	功能说明: Writes a Half-word buffer to the FSMC SRAM memory. 
*	形    参：	pBuffer : pointer to buffer.
*						WriteAddr : SRAM memory internal address from which the data will be written.
*						NumHalfwordToWrite : number of half-words to write.
*	返 回 值: 无
*********************************************************************************************************
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

/*
*********************************************************************************************************
*	函 数 名: FSMC_SRAM_ReadBuffer
*	功能说明:	Reads a block of data from the FSMC SRAM memory. 
*	形    参：	pBuffer :  pointer to the buffer that receives the data read from the SRAM memory.
*						ReadAddr : SRAM memory internal address to read from.
*						NumHalfwordToRead : number of half-words to read.
*	返 回 值: 无
*********************************************************************************************************
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

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

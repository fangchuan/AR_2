/*
*********************************************************************************************************
*
*	模块名称 : FSMC驱动SRAM模块
*	文件名称 : bsp_fsmc_sram.h
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
#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
// 64MB =0X0400 0000
// 1 0X6000 0000 ~ 0X63FF FFFF
// 2 0X6400 0000 ~ 0X67FF FFFF
// 3 0X6800 0000 ~ 0X6CFF FFFF
// 4 0X6C00 0000 ~ 0X6FFF FFFF
// #define Bank1_SRAM1_ADDR    ((uint32_t)0x60000000)
// #define Bank1_SRAM2_ADDR    ((uint32_t)0x64000000)
#define Bank1_SRAM3_ADDR    ((uint32_t)0x68000000)
// #define Bank1_SRAM4_ADDR    ((uint32_t)0x6C000000)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

#endif /* __FSMC_SRAM_H */

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/

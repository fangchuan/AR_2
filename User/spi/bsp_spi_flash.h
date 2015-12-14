#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"

#define FLASH_SPI                           SPI2
#define FLASH_SPI_CLK                       RCC_APB1Periph_SPI2
#define FLASH_SPI_SCK_PIN                   GPIO_Pin_13                  /* PB13 */
#define FLASH_SPI_MISO_PIN                  GPIO_Pin_14                  /* PB14 */
#define FLASH_SPI_MOSI_PIN                  GPIO_Pin_15                  /* PB15 */
#define FLASH_CS_PIN                        GPIO_Pin_12                  /* PB12 */
#define FLASH_PORT                  GPIOB                       /* GPIOB */
#define FLASH_CLK                   RCC_APB2Periph_GPIOB


#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(FLASH_PORT, FLASH_CS_PIN)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(FLASH_PORT, FLASH_CS_PIN)


extern void SPI_FLASH_Init(void);
extern u32 SPI_FLASH_ReadID(void);
extern u32 SPI_FLASH_ReadDeviceID(void);
extern void SPI_Flash_PowerDown(void);
extern void SPI_Flash_WAKEUP(void);
extern void  Write_List(void);
extern void Read_List (void);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void SPI_FLASH_SectorErase(u32 SectorAddr);

#endif /* __SPI_FLASH_H */


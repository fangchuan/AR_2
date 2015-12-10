#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"

#define SPI_FLASH_SPI                           SPI2
#define SPI_FLASH_SPI_CLK                       RCC_APB1Periph_SPI2
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_13                  /* PB13 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOB
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_14                  /* PB14 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOB
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_15                  /* PB15 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOB
#define SPI_FLASH_CS_PIN                        GPIO_Pin_12                  /* PB12 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOB                       /* GPIOB */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOB


#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)


extern void SPI_FLASH_Init(void);
extern u32 SPI_FLASH_ReadID(void);
extern u32 SPI_FLASH_ReadDeviceID(void);
extern void SPI_Flash_PowerDown(void);
extern void SPI_Flash_WAKEUP(void);
extern void  Write_List(void);
extern void Read_List (void);

#endif /* __SPI_FLASH_H */


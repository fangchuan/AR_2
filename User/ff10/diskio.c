/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"


#ifndef FATFS_FLASH_SPI
	#define FATFS_FLASH_SPI				1
#endif

#ifndef TM_FATFS_CUSTOM_FATTIME
	#define TM_FATFS_CUSTOM_FATTIME		0
#endif

#ifndef FATFS_USE_SDIO
	#define FATFS_USE_SDIO			0
#endif

/* Include SD card files if is enabled */
#if FATFS_USE_SDIO == 1
	#include "bsp_sdio_sdcard.h"
#endif

#if FATFS_FLASH_SPI == 1
	#include "bsp_spi_flash.h"
#endif


/* Definitions of physical drive number for each media */
#define ATA			1     //SD���Ĵ����߼���
#define SPI_FLASH		0 //�ⲿFlash�Ĵ����߼���
//����SD��������
#define SD_BLOCK_SIZE            512 /* Block Size in Bytes */
//extern SD_CardInfo SDCardInfo;
//����W25Q32
//ǰ3M�ֽڸ�fatfs��,3M�ֽں�,���ͻ��Լ���	 	
#define FLASH_SECTOR_SIZE 	512			  		    
u16	    FLASH_SECTOR_COUNT = 2048*4;	//W25Q32,4M�ֽ�ȫ��FATFSռ��
#define FLASH_BLOCK_SIZE   	8     	//ÿ��BLOCK��8������
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS status = STA_NOINIT;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				  status = SD_Init();
					if (status!=SD_OK )
					{
						status = STA_NOINIT;
					}
					else
					{
						status = RES_OK;
					}
			#endif
			break;
		
		case SPI_FLASH:
			#if	FATFS_FLASH_SPI ==1
						FLASH_SPI_Init();
						status = RES_OK;
			#endif
			break;

		default:
			status = STA_NOINIT;
	}
		return status;

}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				if (count > 1)
				{
					SD_ReadMultiBlocks(buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE, count);				
					/* Check if the Transfer is finished */
					SD_WaitReadOperation();  //ѭ����ѯdma�����Ƿ����				
					/* Wait until end of DMA transfer */
					while(SD_GetStatus() != SD_TRANSFER_OK);
				}
				else
				{					
					SD_ReadBlock(buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE);
					/* Check if the Transfer is finished */
					SD_WaitReadOperation();  //ѭ����ѯdma�����Ƿ����				
					/* Wait until end of DMA transfer */
					while(SD_GetStatus() != SD_TRANSFER_OK);
				}
				status = RES_OK;
			#endif
			break;

		case SPI_FLASH:
					#if	FATFS_FLASH_SPI ==1
							for(;count>0;count--)
							{
								W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
								sector++;
								buff+=FLASH_SECTOR_SIZE;
							}
							status = RES_OK;
					#endif
		break;
		default:
			status = RES_PARERR;
	}
	
	return status;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT status = RES_PARERR;
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
					if (count > 1)
					{
						SD_WriteMultiBlocks((uint8_t *)buff, sector*SD_BLOCK_SIZE,SD_BLOCK_SIZE, count);						
						/* Check if the Transfer is finished */
						SD_WaitWriteOperation();	   //�ȴ�dma�������
						while(SD_GetStatus() != SD_TRANSFER_OK); //�ȴ�sdio��sd���������
					}
					else
					{
						SD_WriteBlock((uint8_t *)buff,sector*SD_BLOCK_SIZE,SD_BLOCK_SIZE);						
						/* Check if the Transfer is finished */
						SD_WaitWriteOperation();	   //�ȴ�dma�������
						while(SD_GetStatus() != SD_TRANSFER_OK); //�ȴ�sdio��sd���������
					}
					status = RES_OK;
			#endif
			break;

		case SPI_FLASH:
					#if	FATFS_FLASH_SPI ==1
							for(;count>0;count--)
							{										    
								W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
								sector++;
								buff+=FLASH_SECTOR_SIZE;
							}

							status = RES_OK;
					#endif
		break;
		default:
			status = RES_PARERR;
	}
	
	return status;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				switch (cmd) 
				{				
					case GET_SECTOR_SIZE :     // Get R/W sector size (WORD) 
						*(WORD * )buff = 512;
					break;
					case GET_BLOCK_SIZE:      // Get erase block size in unit of sector (DWORD)
						*(DWORD * )buff = SDCardInfo.CardBlockSize;
					break;
					case GET_SECTOR_COUNT:
						*(DWORD * )buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
					case CTRL_SYNC :
					break;
				}
				status = RES_OK;
			#endif
			break;

		case SPI_FLASH:
					#if	FATFS_FLASH_SPI ==1
						switch (cmd) 
						{
							case GET_SECTOR_SIZE :     // Get R/W sector size (WORD)
								*(WORD * )buff = FLASH_SECTOR_SIZE;		//flash��Сд��ԪΪҳ��256�ֽڣ��˴�ȡ2ҳΪһ����д��λ
							break;
							case GET_BLOCK_SIZE :      // Get erase block size in unit of sector (DWORD)
								*(DWORD * )buff = FLASH_BLOCK_SIZE;		//flash��4kΪ��С������λ,512bytesΪһ����������һ��������8������
							break;
							case GET_SECTOR_COUNT:
								*(DWORD * )buff = FLASH_SECTOR_COUNT;		//����fatfs��sector����
							break;
							case CTRL_SYNC :
							break;
							default:break;
						}
						status = RES_OK;
					#endif
		break;
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif

DWORD get_fattime(void) {
	/* Returns current time packed into a DWORD variable */
	return	 0;
}
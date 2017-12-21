/* sysSpiFlash.h - FreeScale P2020DS board SPI Flash driver header file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,10jun09,syt  created
*/

#ifndef __INCsysSpiFlashh
#define __INCsysSpiFlashh

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define FLASH_READ          0x03        /* Read Data Bytes */
#define FLASH_FAST_READ     0x0B        /* Read Data Bytes at Higher Speed */
#define FLASH_RDID          0x9F        /* Read Identification */
#define FLASH_READ_ID       0x90        /* Read Manufacturer ID and Device ID */
#define FLASH_WREN          0x06        /* Write Enable */
#define FLASH_WRDI          0x04        /* Write Disable */
#define FLASH_SE_64K_TYPE   0x20        /* 64 KB Sector Erase */
#define FLASH_SE_256K_TYPE  0xD8        /* 256KB Sector Erase */
#define FLASH_BE_64K_TYPE   0xC7        /* Bulk Erase (64 KB Sector Product) */
#define FLASH_BE_256K_TYPE  0x60        /* Bulk Erase (256 KB Sector Product) */
#define FLASH_PP            0x02        /* Page Program */
#define FLASH_RDSR          0x05        /* Read from Status Register */
#define FLASH_WRSR          0x01        /* Write to Status Register */
#define FLASH_DP            0xB9        /* Deep Power Down */
#define FLASH_RES           0xAB        /* Release from Deep Power Down */


/* status register bit */

#define SRWD    (0x01 << (7 - 7))       /* Status Register Write */
                                        /* 1 = Protects when WP#/ACC is low */
                                        /* 0 = No protection */
#define BP3     (0x01 << (7 - 5))       /* Block Protect*/
#define BP2     (0x01 << (7 - 4))
#define BP1     (0x01 << (7 - 3))
#define BP0     (0x01 << (7 - 2))
#define WEL     (0x01 << (7 - 1))       /* Write Enable Latch */
#define WIP     (0x01 << (7 - 0))       /* Write in Progress */


/* spical marco defines */

#define CMD_LEN                     0x01
#define ADDR_LEN                    0x03
#define PP_LEN                      256

#define SPI_FLASH_SECTOR_NUMBER     256
#define SPI_FLASH_SECTOR_SIZE       (64 * 1024)
#define SPI_FLASH_SECTOR_MASK       0x00ff0000
#define PP_SIZE                     256
#define RDID_DATA_LEN               5
#define SPI_TRANS_LEN_MAX           (64 * 1024 - 0x04)
#define FLASH_ERASE_TIMEOUT         1000

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __INCsysSpiFlashh */


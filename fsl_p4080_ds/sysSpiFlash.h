/* sysSpiFlash.h - Spansion SPI flash driver header file */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,05jul12,d_l  define the macro MAX_CHECKSTATUS. (WIND00359779).
01b,10jun12,y_y  define the macro READ_MAX_LEN. (WIND00334290)
01a,10jun11,y_y  written.
*/

#ifndef __INCspS25flxxSpiFlashh
#define __INCspS25flxxSpiFlashh

#ifdef __cplusplus
extern "C"
{
#endif

/* instruction set */

#define CMD_RDID                0x9f     /* Read ID */
#define CMD_NORMAL_READ         0x03     /* Read data bytes */
#define CMD_FAST_READ           0x0b     /* Read data bytes(high frequency) */

#define CMD_WREN                0x06     /* Write enable  */
#define CMD_WRDI                0x04     /* Write disable */
#define CMD_PP                  0x02     /* Byte program  */

#define CMD_P4E                 0x20     /* Erase 4KB block */
#define CMD_P8E                 0x40     /* Erase 8KB block */
#define CMD_SE                  0xd8     /* Sector erase */
#define CMD_BE                  0xc7     /* Erase whole flash chip */

#define CMD_RDSR                0x05     /* Read status register  */
#define CMD_RCR                 0x35     /* Read config register  */
#define CMD_WRSR                0x01     /* Write status register */

/* FAST_READ as default */

#define CMD_READ                CMD_FAST_READ
#define CMD_READ_LEN            5

/* write in progress */

#define SR_WIP                  1

/* translate mode */

#define NORMAL                  0
#define TRANSONLY               1

/* misc */

#define DEV_ID_LEN              5
#define MAX_ERASE_TIME          256
#define SLAVES_MAX              4
#define READ_MAX_LEN            0xfff8
#define MAX_CHECKSTATUS         2
/*
 * Wait until the SPI data transfer finish. This value is 
 * experimentally determined for Freescale P5020 
 */

#define TIMEOUT                 2000000

/* device Info */

#define SPAN_MANF_ID            0x01
#define SPAN_DEV_ID             0x2018

/* typedef */

/* This structure holds the SPI FLASH configuration values */

typedef struct
    {
    char manufId;            /* Manufacture ID */
    int deviceId;            /* Device ID      */
    int extendDevId;         /* Extend DeviceID*/
    int pageSize;            /* Page Size      */
    int sectorSize;          /* Sector Size    */
    int sectorNum;           /* Sector Number  */
    const char * name;       /* Device name    */
    } SPI_FLASH_DEVICE;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INCspS25flxxSpiFlashh */

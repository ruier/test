/* sysSpiFlash.c - LSI ACP34XX ELPASO board SPI Flash driver */

/*
* Copyright (c) 2010 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01a,15dec10,x_s  written.
*/

/*
DESCRIPTION
This driver provides routines to read and write a SPI M95M01 (or similar)
FLASH. It is typically included by the BSP file sysLib.c. 

The macro INCLUDE_LSI_SPI must be defined to include this driver. The size of 
the entire chip is 128K bytes.

The more details about the SPI controller can be obtained from sysLsiSpi.c.

INCLUDE FILES: sysLib.h ctype.h string.h stdioLib.h stdlib.h
               
SEE ALSO:
\tb "ARM PrimeCell Synchronous Serial Port(PL1022)",
\tb "M95M01R",
\tb "Axxia Communication Processor (ACP3400) Technical Guide",
\tb "ACP3400 Validation Board (Mission) Architecture",
*/

/* includes */

#include <vxWorks.h>
#include <sysLib.h>
#include <ctype.h>
#include <string.h>
#include <stdioLib.h>
#include <stdlib.h>
#include "config.h"

/* defines */

#define SPI_FLASH_SIZE        0x20000  /* 128k bytes */
#define SPI_FLASH_PAGE_SIZE   256      /* 256 bytes */
#define SPI_FLASH_PAGE_WRITE_CYCLE   5 /* 5 ms */

/* A16-A0, Bits A23 to A17 are Don't Care*/

#define SPI_FLASH_ADDR_24(addr)  ((addr & 0x10000) >> 16)   /* addr 16 - 24 */
#define SPI_FLASH_ADDR_16(addr)  ((addr & 0xff00) >> 8)     /* addr 8 - 15 */
#define SPI_FLASH_ADDR_8(addr)   ((addr & 0xff))            /* addr 0 - 7 */

#define SPI_FLASH_WRSR        0x01     /* Instruction - Write Status */
#define SPI_FLASH_WRITE       0x02     /* Instruction - Write to Memory Array */
#define SPI_FLASH_READ        0x03     /* Instruction - Read from Memory Array */
#define SPI_FLASH_WRDI        0x04     /* Instruction - write disable */
#define SPI_FLASH_RDSR        0x05     /* Instruction - Read Status */
#define SPI_FLASH_WREN        0x06     /* Instruction - write enable */

#define SPI_FLASH_MEM_DBG              /* debug swith to dump memory data */

/* local */

LOCAL void  sysSpiFlashWriteEnable (BOOL enable);

/*******************************************************************************
*
* sysSpiFlashWriteEnable - enable or diable M95M01 device write operation
*
* The purpose of this function is to enable or diable M95M01 device write
* operation, instructions WREN or WRDI would be sent to M95M01 device.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void sysSpiFlashWriteEnable
    (
    BOOL    enable       /* 1 - enable write, 0 - diable write */
    )
    {
    UINT32 tmpCommand;

    tmpCommand =(enable)? (SPI_FLASH_WREN):(SPI_FLASH_WRDI);

    /* Chip Select (S) is driven low */

    sysSpiChipSelect(1);

    /* Instructions WREN or WRDI is sent */

    sysSpiSendCmd(tmpCommand);

    /* Chip Select (S) is driven high */

    sysSpiChipSelect(0);

    return;
    }

/*******************************************************************************
*
* sysSpiFlashWrite - write the M95M01 memory
*
* This routine copies a specified buffer into the M95M01 memory.
*
* RETURNS: OK, or ERROR if the parameters are invalid or the operation fails.
*/

STATUS sysSpiFlashWrite
    (
    UINT8 *  pSrc,   /* source buffer */
    INT32    nbytes, /* number of bytes to copy */
    INT32    offset  /* byte offset into M95M01 */
    )
    {
    UINT8 * destAddr;
    INT32   dataNum;

    if ((nbytes + offset) > SPI_FLASH_SIZE)
        return (ERROR);

     /*
      * if nbytes number larger than page program length, then page program
      * operation would be used and the offset must be align with page size.
      */

    if (((nbytes/SPI_FLASH_PAGE_SIZE) && (offset & (SPI_FLASH_PAGE_SIZE -1))) ||
        ((offset & (SPI_FLASH_PAGE_SIZE -1)) &&
        ((nbytes + offset%SPI_FLASH_PAGE_SIZE) > SPI_FLASH_PAGE_SIZE)))
        return (ERROR);

    /* Byte or page write would be run based on the nbytes number */

    do
        {
        destAddr = pSrc;
        dataNum = min(nbytes, SPI_FLASH_PAGE_SIZE);

        /* Write Enable for the device */

        sysSpiFlashWriteEnable(1);

        /* drive low chip select signal */

        sysSpiChipSelect(1);

        /* send WRITE command to device */

        sysSpiSendCmd(SPI_FLASH_WRITE);

        /* send the destination address to the device */

        sysSpiSendCmd(SPI_FLASH_ADDR_24(offset));
        sysSpiSendCmd(SPI_FLASH_ADDR_16(offset));
        sysSpiSendCmd(SPI_FLASH_ADDR_8(offset));

        /* write data from M95M01 */

        if (OK != sysSpiWrite(destAddr, dataNum))
            return (ERROR);

        /* drive high chip select signal */

        sysSpiChipSelect(0);
        sysMsDelay(SPI_FLASH_PAGE_WRITE_CYCLE);  /* page write cycle 5 ms */
        pSrc += SPI_FLASH_PAGE_SIZE;
        offset += SPI_FLASH_PAGE_SIZE;
        nbytes -= SPI_FLASH_PAGE_SIZE;
        }while (nbytes > 0);

    /* Write Diable for the device */

    sysSpiFlashWriteEnable(0);

    return (OK);
    }

/*******************************************************************************
*
* sysSpiFlashRead - get the contents of M95M01 memory
*
* This routine copies the contents of the M95M01 memory into a specified
* buffer.
*
* RETURNS: OK, or ERROR if the parameters are invalid or the operation fails.
*/

STATUS sysSpiFlashRead
    (
    UINT8 *  pDst,   /* destination buffer */
    INT32    nbytes, /* number of bytes to copy */
    INT32    offset  /* byte offset into M95M01 */
    )
    {
    if ((nbytes + offset) > SPI_FLASH_SIZE)
        return (ERROR);

    /* drive low chip select signal */

    sysSpiChipSelect(1);

    /* send READ command to device */

    sysSpiSendCmd(SPI_FLASH_READ);

    /* send the destination address to device */

    sysSpiSendCmd(SPI_FLASH_ADDR_24(offset));
    sysSpiSendCmd(SPI_FLASH_ADDR_16(offset));
    sysSpiSendCmd(SPI_FLASH_ADDR_8(offset));

    /* read data from M95M01 */

    if (OK != sysSpiRead(pDst, nbytes))
        return (ERROR);

    /* drive high chip select signal */

    sysSpiChipSelect(0);

    return (OK);
    }

#ifdef SPI_FLASH_MEM_DBG
/*******************************************************************************
*
* sysSpiFlashDump - dump the contents of SPI flash memory
*
* This routine dumps the contents of the SPI flash memory.
*
* RETURNS: N/A
*/

void sysSpiFlashDump
    (
    UINT32  nbytes, /* number of bytes to dump */
    UINT32  offset  /* byte offset into M95M01 */
    )
    {
    UINT8 * pBuf;
    int     bufSize;
    int     i;

    if ((nbytes + offset) > SPI_FLASH_SIZE)
        return;

    bufSize = ((nbytes - 1) / 8 + 1) * 8;
    pBuf    = malloc (bufSize);
    if (pBuf == NULL)
        return;

    if (OK != sysSpiFlashRead(pBuf, nbytes, offset))
        return;

    for(i = 0 ; i < nbytes; i += 8)
        {
        printf ("%03x: %02x %02x %02x %02x %02x %02x %02x %02x",
                i,
                pBuf[i + 0],
                pBuf[i + 1],
                pBuf[i + 2],
                pBuf[i + 3],
                pBuf[i + 4],
                pBuf[i + 5],
                pBuf[i + 6],
                pBuf[i + 7]);
        printf (" *%c%c%c%c%c%c%c%c*\n",
                isprint (pBuf[i + 0]) ? pBuf[i + 0]:'.',
                isprint (pBuf[i + 1]) ? pBuf[i + 1]:'.',
                isprint (pBuf[i + 2]) ? pBuf[i + 2]:'.',
                isprint (pBuf[i + 3]) ? pBuf[i + 3]:'.',
                isprint (pBuf[i + 4]) ? pBuf[i + 4]:'.',
                isprint (pBuf[i + 5]) ? pBuf[i + 5]:'.',
                isprint (pBuf[i + 6]) ? pBuf[i + 6]:'.',
                isprint (pBuf[i + 7]) ? pBuf[i + 7]:'.');
        }
    }
#endif /* SPIFLASH_MEM_DBG */

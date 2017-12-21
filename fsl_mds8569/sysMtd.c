/* sysMtd.c - Freescale MDS8569 Board MTD driver */

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
01a,10aug09,x_z  derived from version 01a of amcc_kilauea/sysMtd.c.
*/

/*
DESCRIPTION
This file provides the TFFS MTD driver for the 8-bit 64MB Spansion S29GL256
flash on the Freescale MDS8569 board. The driver handles the low level
operations like erase and program of the flash. It also provides an identify
routine to check if it is compatible with the devices.

The macro FLASH_TFFS_SIZE indicate the size(in bytes) of the space used on
the entire FLASH chip and must keep enough space for bootrom and NVRAM. The
size of the chip is 32M bytes.

The macro TFFS_DBG_PRINT must be defined as 'printf' to provide informative
debug messages if necessary.

The driver is only compatible and tested with Freescale MDS8569 board. DO NOT
use this driver on any other boards unless the board flash configuration is
absolutely the same.

INCLUDE FILES: stdio.h flflash.h backgrnd.h flashDev.h

SEE ALSO:
\tb "Spansion S29GL-N Data Sheet".
*/

/* includes */

#include <stdio.h>
#include <tffs/flflash.h>
#include <tffs/backgrnd.h>
#include <drv/mem/flashDev.h>
#include "config.h"

/* defines */

#define S29GL_FLASH_ID          0x017e

/* globals */

IMPORT UINT16 sysFlashTypeGet (void);
IMPORT STATUS sysSectorErase (FLASH_DEF * pFA);
IMPORT STATUS sysFlashWrite (FLASH_DEF * pFB, int size, int offset,
                             FLASH_DEF   value);

/*******************************************************************************
*
* S29GLWrite - write data to the flash
*
* This routine is called to write data to the flash part on fsl_mds8569 board.
*
* RETURNS: flOK or flWriteFault if the writing operation fails.
*
* ERRNO: N/A
*/

LOCAL FLStatus s29GlWrite
    (
    FLFlash           vol,        /* flash array identification descriptor */
    CardAddress       address,    /* Physical offset on flash */
    const void FAR1 * buffer,     /* source buffer */
    int               length,     /* buffer length */
    int               overwrite   /* overwrite flag */
    )
    {
    UINT8 * buf = (UINT8 *) buffer;
    UINT8 * unaligned = (UINT8 *)vol.map (&vol, address, 0);;

    if (flWriteProtected (vol.socket))
        {
#ifdef TFFS_DBG_PRINT
        TFFS_DBG_PRINT ("writing Protected @0x%x\n", address);
#endif /* TFFS_DBG_PRINT */
        return flWriteProtect;
        }

    if (sysFlashWrite ((FLASH_DEF *) buf, length, address, 0) == ERROR)
        return flWriteFault;

    if (tffscmp ((void FAR0 *) unaligned, buffer, length))
        {
#ifdef TFFS_DBG_PRINT
        TFFS_DBG_PRINT ("[S29GLWrite]: data double check error @ 0x%08x :...\n",
                        (UINT32) unaligned, length);
#endif /* TFFS_DBG_PRINT */
        return flWriteFault;
        }

    return flOK;
    }

/*******************************************************************************
*
* S29GLErase - erase the special sector(s) of the flash
*
* This routine is called to erase the special sector(s) of the flash part.
*
* RETURNS: flOK or flTimedOut if timeout.
*
* ERRNO: N/A
*/

LOCAL FLStatus s29GlErase
    (
    FLFlash vol,                /* flash array identification descriptor */
    int     firstErasableBlock, /* start block to be erased */
    int     numOfErasableBlocks /* number of blocks to be erased */
    )
    {
    UINT8 *  sector = NULL;
    int      i;
    STATUS   sts;

    if (flWriteProtected (vol.socket))
        {
#ifdef TFFS_DBG_PRINT
        TFFS_DBG_PRINT ("writing Protected @sector%d\n", firstErasableBlock);
#endif /* TFFS_DBG_PRINT */
        return flWriteProtect;
        }

    for (i = firstErasableBlock;
         i < firstErasableBlock + numOfErasableBlocks;
         i++)
        {
        sector = (UINT8 *) vol.map (&vol, i * vol.erasableBlockSize, 0);
#ifdef TFFS_DBG_PRINT
        TFFS_DBG_PRINT("Erasing sector#%03d @ 0x%08x ...\r",
                       i, (UINT32) sector);
#endif /* TFFS_DBG_PRINT */
        sts = sysSectorErase (sector);

        if (sts != OK)
            {
#ifdef TFFS_DBG_PRINT
            TFFS_DBG_PRINT ("\nErasing sector#%03d timeout @ 0x%08x ...\n",
                            i, (UINT32) sector);
#endif /* TFFS_DBG_PRINT */
            return flTimedOut;;
            }
        }

    return flOK;
    }

/*******************************************************************************
*
* s29GlIdentify - identify the flash
*
* This routine is called to probe the flash part.
*
* RETURNS: flOK or flUnknownMedia if unknown flash is probed.
*
* ERRNO: N/A
*/

FLStatus s29GlIdentify
    (
    FLFlash vol
    )
    {
    UINT8 *  base = (UINT8 *) vol.map (&vol, 0, 0);
    UINT16   device;

#ifdef TFFS_DBG_PRINT
    TFFS_DBG_PRINT("Entering s29GlIdentify routine @ base address 0x%08x ...\n",
                   (UINT32) base);
#endif /* TFFS_DBG_PRINT */

    /* check the flash id */

    device = sysFlashTypeGet ();
    if (device == S29GL_FLASH_ID)
        {
        vol.type = S29GL_FLASH_ID;
        }
    else
        {
#ifdef TFFS_DBG_PRINT
        TFFS_DBG_PRINT ("unknown flash type 0x%x...\n", device);
#endif /* TFFS_DBG_PRINT */
        return flUnknownMedia;
        }

    vol.chipSize            = FLASH_TFFS_SIZE;
    vol.noOfChips           = 1;
    vol.interleaving        = 1;
    vol.erasableBlockSize   = FLASH_SECTOR_SIZE * vol.interleaving;
    vol.write               = s29GlWrite;
    vol.erase               = s29GlErase;

    return flOK;
    }

/* sysMtd.c - MTD driver for flash(s) on wrSbcP4080 board */

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
01a,25nov10,e_d derived from version xlnx_ml507/01a.
*/

/*
DESCRIPTION
This file provides the TFFS MTD driver for the 4 banks of 8-bit Intel 28F128P30T
flash on the wrSbcP4080 board. The driver handles the low level operations like
erase and program of the flash. It also provides an identify routine to check if
it is compatible with the devices.

The macros TFFS_DBG_PRINT must be defined as 'printf' to provide informative debug
messages if necessary.

The driver is only compatible and tested with xlnx ml507 board. DO NOT use this
driver on any other boards unless the flashes are absolutely the same.

INCLUDE FILES:

SEE ALSO:
*/

#include <stdio.h>
#include <drv/mem/flashDev.h>
#include "tffs/flflash.h"
#include "tffs/backgrnd.h"

/* defines */

#undef  TFFS_DBG_PRINT

#define v28F128P30_FLASH_ID      0x8818

IMPORT UINT16 sysFlashTypeGet (void);
IMPORT STATUS sysSectorErase (FLASH_DEF * pFA);
IMPORT STATUS sysFlashWrite (FLASH_DEF * pFB,int size, int offset, FLASH_DEF value);

/*******************************************************************************
*
* v28F128P30Program - low level byte programming routine
*
* This routine is the low level byte programming routine. The addr is the
* flash program address.
*
* RETURN: flOK or flTimedOut.
*/

LOCAL FLStatus v28F128P30Program
    (
    UINT16 *    addr,
    UINT16      value
    )
    {
    STATUS  sts;
    sts = sysFlashWrite (NULL,2, (UINT32) addr - FLASH_BASE_ADRS, value);
    if (sts != OK)
        {
        #ifdef TFFS_DBG_PRINT
            TFFS_DBG_PRINT ("programing error @0x%x\n", addr);
        #endif /* TFFS_DBG_PRINT */
        return flTimedOut;
        }
    return flOK;
    }

/*******************************************************************************
*
* v28F128P30Write - write routine for 28F128P30T flash
*
* This routine is write routine for 28F128P30T flash.
*
* RETURN: flOK or flWriteFault.
*/

LOCAL FLStatus v28F128P30Write
    (
    FLFlash          vol,
    CardAddress      address,
    const void FAR1  * buffer,
    int              length,
    int              overwrite
    )
    {
    UINT8  *unaligned;
    UINT8  *buf = (UINT8 *)buffer;
    UINT32  left = length;
    UINT16 *aligned;
    UINT16  data;
    UINT32  num;
    int     i;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    /* calculate the program addr, make sure it's 32-bit aligned */

    unaligned = (UINT8 *)vol.map (&vol, address, 0);
    num = (UINT32)unaligned & 0x1;
    aligned = (UINT16 *)((UINT32)unaligned - num);
    if (num != 0)
        {
        data = *aligned;

        for (i = num ; i < 2; i++)
            {
            data &= ~(0xFF << ((1 - i) * 8));
            data |= ((*(buf + i - num)) << ((1 - i) * 8));
            }

        if (v28F128P30Program (aligned, data) != flOK)
            {
            return flWriteFault;
            }

        buf  += (2 - num);
        left -= (2 - num);
        aligned++;
        }

    while (left >= 2)
        {
        data = *(UINT16 *) buf;

        if (v28F128P30Program (aligned, data) != flOK)
            {
            return flWriteFault;
            }

        buf  += 2;
        left -= 2;
        aligned++;
        }

    if (left > 0)
        {
        data = *aligned;

        for (i = 0 ; i < left; i++)
            {
            data &= ~(0xFF << ((1 - i) * 8));
            data |= ((*(buf + i)) << ((1 - i) * 8));
            }

        if (v28F128P30Program (aligned, data) != flOK)
            {
            return flWriteFault;
            }
        }

    if (tffscmp ((void FAR0 *) unaligned, buffer, length))
        {
#ifdef TFFS_DBG_PRINT
        TFFS_DBG_PRINT ("[28F128Write]: data double check error @ 0x%08x :  ...\n",
                        (UINT32) unaligned, length);
#endif /* TFFS_DBG_PRINT */
        return flWriteFault;
        }

    return flOK;
    }

/*******************************************************************************
*
* v28F128P30Erase - erase routine for v28F128P30 flash
*
* This routine is erase routine for v28F128P30 flash.
*
* RETURN: flOK or flTimedOut.
*/

LOCAL FLStatus v28F128P30Erase
    (
    FLFlash vol,
    int     firstErasableBlock,
    int     numOfErasableBlocks
    )
    {
    UINT16 * sector = NULL;
    int      i;
    STATUS   sts;

    if (flWriteProtected (vol.socket))
        {
#ifdef TFFS_DBG_PRINT
        TFFS_DBG_PRINT ("writing Protected @sector%d\n", firstErasableBlock);
#endif /* TFFS_DBG_PRINT */
        return flWriteProtect;
        }

    for (i = firstErasableBlock; i < firstErasableBlock + numOfErasableBlocks; i++)
        {
        sector = (UINT16 *) vol.map (&vol, i * vol.erasableBlockSize, 0);
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
* v28F128P30Identify - identify routine for v28F128P30 flash
*
* This routine is identify routine for v28F128P30 flash.
*
* RETURN: flOK or flUnknownMedia.
*/

FLStatus v28F128P30Identify
    (
    FLFlash vol
    )
    {
    UINT16 * base = (UINT16 *) vol.map (&vol, 0, 0);
    UINT16   device;

#ifdef TFFS_DBG_PRINT
    TFFS_DBG_PRINT("Entering v28F128P30Identify routine @ base address 0x%08x ...\n",
                   (UINT32) base);
#endif /* TFFS_DBG_PRINT */

    /* check the flash id */

    device = sysFlashTypeGet ();
    if (device == v28F128P30_FLASH_ID)
        {
        vol.type = v28F128P30_FLASH_ID;
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
    vol.write               = v28F128P30Write;
    vol.erase               = v28F128P30Erase;

    return flOK;
    }


/* sysMtd.c - MTD driver for Intel 28F256 flash */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,27jul11,fao  optimize flashWrite.(WIND00288227)
01a,17dec10,fao  created
*/

/*
DESCRIPTION
This file provides the TFFS MTD driver for the Intel 28F256 series flash parts. 
The driver handles the low level operations like erase and program of the flash.
It also provides an identify routine to check if it is compatible with the 
devices.

INCLUDE FILES:

SEE ALSO:
*/

#include <stdio.h>
#include <tffs/flflash.h>
#include <tffs/backgrnd.h>

/* defines */

/* disable debugging */

#undef CFI_DEBUG

#ifdef CFI_DEBUG
#   define DEBUG_PRINT    printf
#else
#   undef DEBUG_PRINT
#endif /* CFI_DEBUG */

IMPORT STATUS sysFlashBlkErase ( int);     
IMPORT STATUS sysFlashProgram (volatile UINT16*, volatile UINT16*, UINT32);
IMPORT STATUS sysFlashBuffProgram (volatile UINT16*, volatile UINT16*, UINT32);

/*******************************************************************************
*
* flashWrite - write routine for flash
*
* This routine writes data to the flash.
*
* RETURNS: OK or ERROR if write failed.
*
* ERRNO: N/A
*/

STATUS flashWrite
    (
    char *  buf,        /* buffer to be copied into flash memory */
    UINT32  byteLen,    /* maximum number of bytes to copy       */
    UINT32  addr        /* byte offset into flash memory         */
    )
    {
    /* limited to one sector */

    if ((addr < TFFS_BASE_ADRS ) || (byteLen > TFFS_SIZE) || 
        (addr > (TFFS_BASE_ADRS + TFFS_SIZE))) 
        return (ERROR);

    /* see if contents are actually changing */

    if (bcmp ((char *)addr, buf, byteLen) == 0)
        return (OK);

    if (byteLen > 0x10)
        return sysFlashBuffProgram((UINT16* )addr, (UINT16* )buf, byteLen);
    else
        return sysFlashProgram((UINT16* )addr, (UINT16* )buf, byteLen);

    }

/*******************************************************************************
*
* sysMtdWrite - write routine for flash
*
* This routine writes data to the flash.
*
* RETURNS: flOK or flWriteFault if write failed.
*
* ERRNO: N/A
*/

LOCAL FLStatus sysMtdWrite
    (
    FLFlash         vol,
    CardAddress     address,
    const void FAR1 *buffer,
    int             length,
    int             overwrite
    )
    {
    UINT8  *unaligned;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    /* calculate the program addr, make sure it's aligned */

    unaligned = (UINT8 *)vol.map (&vol, address, 0);

    if (flashWrite((char *)buffer,length, (UINT32)unaligned) == ERROR)
        return flWriteFault;

    if (tffscmp((void FAR0 *)unaligned, buffer, length))
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: CFISCS write failed in verification.\n");
#endif            
        return flWriteFault;
        }

    return flOK;
    }

/*******************************************************************************
*
* sysMtdErase - erase routine for flash
*
* This routine erases a block of the flash.
*
* RETURNS: flOK or flWriteFault if erase failed.
*
* ERRNO: N/A
*/

LOCAL FLStatus sysMtdBlkErase
    (
    int     eraseBlock
    )
    {
    if (sysFlashBlkErase(eraseBlock) != OK)
        return flWriteFault;
    return flOK;
    }


/*******************************************************************************
*
* sysMtdErase - erase routine for flash
*
* This routine erases the specified blocks.
*
* RETURNS: flOK or flWriteFault if erase failed.
*
* ERRNO: N/A
*/

LOCAL FLStatus sysMtdErase
    (
    FLFlash vol,
    int     firstEraseBlock,
    int     numberOfEraseBlocks
    )
    {
    FLStatus status = flOK;
    int      iBlock;
    UINT32   sector;

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: CFISCS erase firstblock %d.\n", firstEraseBlock);
    DEBUG_PRINT("Debug: CFISCS address 0x%x.\n", (unsigned int)flMap(vol.socket,
                 firstEraseBlock * vol.erasableBlockSize));
#endif   

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    sector = (UINT32)vol.map(&vol, firstEraseBlock * vol.erasableBlockSize, 0);

    if (sector < TFFS_BASE_ADRS)
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: erase error.\n");
#endif
        return flWriteFault;
        }

    sector = (sector - FLASH_BASE_ADRS) / vol.erasableBlockSize;
   
    for (iBlock = sector; 
         iBlock < sector + numberOfEraseBlocks && status == flOK;
         iBlock++) 
         {
         status = sysMtdBlkErase(iBlock);
         }
   
    return status;
    }

/*******************************************************************************
*
* sysMtdIdentify - identify routine for flash
*
* This routine probes the information of the flash part
*
* RETURNS: always flOK.
*
* ERRNO: N/A
*/

FLStatus sysMtdIdentify
    (
    FLFlash vol
    )
    {
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: entering CFISCS identification routine.\n");
#endif
    
    /* CFI INFO */
    
    vol.type = 0x897E; /* Vendor and Device ID */
 
    vol.erasableBlockSize = FLASH_SECTOR_SIZE;

    vol.chipSize = TFFS_SIZE;
   
    vol.interleaving = 1;
    vol.noOfChips=1;

    vol.erasableBlockSize *= vol.interleaving;

    vol.write = sysMtdWrite;
    vol.erase = sysMtdErase;

    return flOK;
    }


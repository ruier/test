/* nvRamToFlash.c - non-volatile RAM library to Flash vxBus Driver */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,29feb12,x_z  written for NVRAM library mapped to vxBus NOR-Flash driver.
*/

/*
DESCRIPTION

This library contains non-volatile RAM manipulation routines for targets lacking
non-volatile RAM, but that do have Flash memory.  Read and write wrappers are
provided for the Flash memory routines sysFlashGet() and sysFlashSet().

SEE ALSO: vxbNorFlash
*/

/* includes */

#include <vxWorks.h>
#include <logLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <../src/hwif/h/flash/vxbFlash.h>
#include <../src/hwif/h/flash/vxbFlashCommon.h>
#include <vxBusLib.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"

/* defines */

#undef FLASH_NVRAM_DBG
#ifdef FLASH_NVRAM_DBG
#   define NVRAM_DBG_MSG(fmt, a, b, c, d, e, f)           \
        if (_func_logMsg != NULL)                       \
            _func_logMsg ("nvRamToFlash: "fmt, a, b, c, d, e, f)
#else /* !FLASH_NVRAM_DBG */
#   define NVRAM_DBG_MSG(fmt, ...)
#endif /* FLASH_NVRAM_DBG */

/* Flash functions */

#define FLASH_NVRAM_ENA    \
        nvRamFlashInfo.pFlashChip->flashOPs.ena (nvRamFlashInfo.pFlashChip,   \
        TRUE, NULL)
#define FLASH_NVRAM_DIS    \
        nvRamFlashInfo.pFlashChip->flashOPs.ena (nvRamFlashInfo.pFlashChip,   \
        FALSE, NULL)

#define FLASH_NVRAM_LOCKQRY(plock)    \
        nvRamFlashInfo.pFlashChip->flashOPs.blkLockQry (nvRamFlashInfo.pFlashChip,   \
        nvRamFlashInfo.offset, plock)

#define FLASH_NVRAM_UNLOCK    \
        nvRamFlashInfo.pFlashChip->flashOPs.blkLock (nvRamFlashInfo.pFlashChip,   \
        nvRamFlashInfo.offset, FALSE)

#define FLASH_NVRAM_READ(offset, strLen, ppbuf)    \
        nvRamFlashInfo.pFlashChip->flashOPs.read (nvRamFlashInfo.pFlashChip,   \
        nvRamFlashInfo.offset + (offset), 1, (strLen), (ppbuf), NULL)
#define FLASH_NVRAM_READ_ALL(ppbuf)    \
        nvRamFlashInfo.pFlashChip->flashOPs.read (nvRamFlashInfo.pFlashChip,   \
        nvRamFlashInfo.offset, 1, nvRamFlashInfo.nvRamSize, (ppbuf), NULL)

#define FLASH_NVRAM_WRITE(ppbuf)    \
        nvRamFlashInfo.pFlashChip->flashOPs.write (nvRamFlashInfo.pFlashChip,   \
        nvRamFlashInfo.offset, 1, nvRamFlashInfo.nvRamSize, (ppbuf), NULL)

#define FLASH_NVRAM_ERASE     \
        nvRamFlashInfo.pFlashChip->flashOPs.blkErase (nvRamFlashInfo.pFlashChip,  \
        nvRamFlashInfo.offset, 1)

/* typedefs */

/* NVRAM to Flash information */

typedef struct
    {
    char *          pFlashName; /* Flash device driver name */
    UINT32          flashUnit;  /* Flash device driver unit */
    UINT32          offset;     /* Flash region offset to Flash device */

    FLASH_CHIP_ID   pFlashChip;
    UINT32          nvRamSize;
    UINT8 *         pBlkBuf;    /* used to save block data */
    UINT32          status; 
    } NVRAM_FLASH_INFO;

/* defines for status field */

#define NVRAM_FLASH_STA_INIT_DONE   0x01    /* finished for initialization */
#define NVRAM_FLASH_STA_INIT_ERR    0x02    /* failed for initialization */
#define NVRAM_FLASH_STA_BOOT_LOCKED 0x04    /* locked for boot region */

LOCAL NVRAM_FLASH_INFO  nvRamFlashInfo = {0};

/*******************************************************************************
*
* sysNvRamInit - initialize NVRAM library
*
* This function initializes the NVRAM library.
*
* RETURNS: OK or ERROR if failed.
*
* ERRNO: N/A
*/

LOCAL STATUS sysNvRamInit (void)
    {
    VXB_DEVICE_ID   pFlashDev;
    STATUS          ret = OK;
    BOOL            locked = TRUE;
    FLASH_CHIP_ID   (*flashChipInfoGet) (VXB_DEVICE_ID pDev, UINT32 chipId);
    
    /* Initialize NVRAM library */
    
    if (nvRamFlashInfo.status == 0)
        {
        nvRamFlashInfo.pFlashName = NV_RAM_FLASH_NAME;
        nvRamFlashInfo.flashUnit = NV_RAM_FLASH_UNIT;
        nvRamFlashInfo.offset = NV_RAM_OFFSET;
        
        /* Put to error status first */
        
        nvRamFlashInfo.status = NVRAM_FLASH_STA_INIT_ERR;
        
        /* Find Flash device and obtain chip information */

        pFlashDev = vxbInstByNameFind (nvRamFlashInfo.pFlashName,
                                       nvRamFlashInfo.flashUnit);
        if (pFlashDev == NULL)
            {
            NVRAM_DBG_MSG ("Failed to find the Flash device %s%d\n",
                           nvRamFlashInfo.pFlashName,
                           nvRamFlashInfo.flashUnit, 0, 0, 0, 0);
            return (ERROR);
            }

        flashChipInfoGet =
            vxbDevMethodGet (pFlashDev, DEVMETHOD_CALL (vxbFlashChipInfoGet));
        nvRamFlashInfo.pFlashChip = flashChipInfoGet (pFlashDev, 0);
        
        if (nvRamFlashInfo.pFlashChip->uEraseSize < NV_RAM_SIZE)
            {
            NVRAM_DBG_MSG ("NVRAM size(0x%x) is larger than Flash chip block "
                          "size(0x%x)\n",
                           NV_RAM_SIZE,
                           nvRamFlashInfo.pFlashChip->uEraseSize, 0, 0, 0, 0);
            return (ERROR);
            }
            
        if ((nvRamFlashInfo.offset >=
            nvRamFlashInfo.pFlashChip->uChipSize) ||
            ((nvRamFlashInfo.offset +
            nvRamFlashInfo.pFlashChip->uEraseSize) >
            nvRamFlashInfo.pFlashChip->uChipSize))
            {
            NVRAM_DBG_MSG ("0x%x is out of Flash chip(0x%x)\n",
                           nvRamFlashInfo.offset,
                           nvRamFlashInfo.pFlashChip->uChipSize, 0, 0, 0, 0);
            return (ERROR);
            }

        if ((nvRamFlashInfo.offset &
            (nvRamFlashInfo.pFlashChip->uEraseSize - 1)) != 0)
            {
            NVRAM_DBG_MSG ("0x%x is unaligned with erase size(0x%x)\n",
                           nvRamFlashInfo.offset,
                           nvRamFlashInfo.pFlashChip->uEraseSize, 0, 0, 0, 0);
            return (ERROR);
            }

        /* Allocate block buffer */

        nvRamFlashInfo.nvRamSize = NV_RAM_SIZE;
        nvRamFlashInfo.pBlkBuf = malloc (nvRamFlashInfo.nvRamSize);
        if (nvRamFlashInfo.pBlkBuf == NULL)
            {
            NVRAM_DBG_MSG ("Failed to allocate block data buffer\n",
                           0, 0, 0, 0, 0, 0);
            return (ERROR);
            }

        /* Check Flash functions */

        if (((nvRamFlashInfo.pFlashChip->uCapability &
            FLASH_CHIP_CAP_RD) == 0) ||
            (nvRamFlashInfo.pFlashChip->flashOPs.read == NULL) ||
            ((nvRamFlashInfo.pFlashChip->uCapability &
            FLASH_CHIP_CAP_WR) == 0) ||
            (nvRamFlashInfo.pFlashChip->flashOPs.write == NULL))
            {
            NVRAM_DBG_MSG ("Invalid Flash device\n", 0, 0, 0, 0, 0, 0);
            free (nvRamFlashInfo.pBlkBuf);
            return (ERROR);
            }

        if (((nvRamFlashInfo.pFlashChip->uCapability &
            FLASH_CHIP_CAP_BLKLOCK) != 0) &&
            (nvRamFlashInfo.pFlashChip->flashOPs.blkLock != NULL))
            {

            /* Query lock status */

            if (((nvRamFlashInfo.pFlashChip->uCapability &
                FLASH_CHIP_CAP_BLKLOCKQRY) != 0) &&
                (nvRamFlashInfo.pFlashChip->flashOPs.blkLockQry != NULL) &&
                (FLASH_NVRAM_ENA == OK))
                {
                
                /* 
                 * Don't check the result because "locked" has been set to TRUE 
                 * by default
                 */
                
                (void) FLASH_NVRAM_LOCKQRY (&locked);
                    
                /* The operation has been finished anyway */

                FLASH_NVRAM_DIS;
                }
            }
        else
            locked = FALSE;
        
        /* Updated status */ 
        
        nvRamFlashInfo.status = NVRAM_FLASH_STA_INIT_DONE;
        if (locked)
            nvRamFlashInfo.status |= NVRAM_FLASH_STA_BOOT_LOCKED;    
        }
    else if ((nvRamFlashInfo.status & NVRAM_FLASH_STA_INIT_ERR) != 0)
        {
        NVRAM_DBG_MSG ("Initialization has failed\n", 0, 0, 0, 0, 0, 0);
        return (ERROR);
        }
        
    /* Unlocked the block if necessary */

    if ((nvRamFlashInfo.status & NVRAM_FLASH_STA_BOOT_LOCKED) != 0)
        {
        if (FLASH_NVRAM_ENA != OK)
            {
            NVRAM_DBG_MSG ("Failed to enable Flash to unlock block @ 0x%x\n",
                            nvRamFlashInfo.offset, 0, 0, 0, 0, 0);
            return (ERROR);
            }
        else
            {
            if (FLASH_NVRAM_UNLOCK != OK)
                {
                NVRAM_DBG_MSG ("Failed to unlock block @ 0x%x\n",
                               nvRamFlashInfo.offset, 0, 0, 0, 0, 0);

                ret = ERROR;
                }
            else
                nvRamFlashInfo.status &= ~NVRAM_FLASH_STA_BOOT_LOCKED;

            /* The operation has been finished anyway */

            FLASH_NVRAM_DIS;
            }
        }

    return (ret);
    }

/*******************************************************************************
*
* sysNvRamGet - get the contents of non-volatile RAM
*
* This routine copies the contents of the Flash device into a specified string.
* The string is terminated with an EOS.
*
* RETURNS: OK or ERROR if failed to read the Flash device.
*
* ERRNO: N/A
*/

STATUS sysNvRamGet
    (
    char *  string,         /* where to copy non-volatile RAM    */
    int     strLen,         /* maximum number of bytes to copy   */
    int     offset          /* byte offset into non-volatile RAM */
    )
    {
    STATUS              retVal = ERROR;
    UINT8 *             ppbuf[1];

    if (sysNvRamInit () != OK)
        return (ERROR);

    if ((offset < 0) || (strLen < 0) ||
        ((offset + strLen) > nvRamFlashInfo.nvRamSize))
        {
        NVRAM_DBG_MSG ("[0x%x,0x%x] is out of range(0x%x)\n",
                       offset, offset + strLen - 1, nvRamFlashInfo.nvRamSize,
                       0, 0, 0);
        return (ERROR);
        }

    /* Enable Flash */

    if (FLASH_NVRAM_ENA != OK)
        {
        NVRAM_DBG_MSG ("Failed to enable Flash \n", 0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    ppbuf[0] = (UINT8 *) string;
    if (FLASH_NVRAM_READ (offset, strLen, ppbuf) == OK)
        {
        string [strLen] = EOS;
        retVal = OK;
        }

    /* The get operation has been finished anyway */

    FLASH_NVRAM_DIS;

    return (retVal);
    }

/*******************************************************************************
*
* sysNvRamSet - write to non-volatile RAM
*
* This routine copies a specified string into the Flash device.
*
* RETURNS: OK or ERROR if failed to write the Flash device.
*
* ERRNO: N/A
*/

STATUS sysNvRamSet
    (
    char *  string,         /* string to be copied into non-volatile RAM */
    int     strLen,         /* maximum number of bytes to copy           */
    int     offset          /* byte offset into non-volatile RAM         */
    )
    {
    STATUS              retVal = ERROR;
    UINT8 *             ppbuf[1];

    if (sysNvRamInit () != OK)
        return (ERROR);

    if ((offset < 0) || (strLen < 0) ||
        ((offset + strLen) > nvRamFlashInfo.nvRamSize))
        {
        NVRAM_DBG_MSG ("[0x%x,0x%x] is out of range(0x%x)\n",
                       offset, offset + strLen - 1, nvRamFlashInfo.nvRamSize,
                       0, 0, 0);
        return (ERROR);
        }

    /* Enable Flash */

    if (FLASH_NVRAM_ENA != OK)
        {
        NVRAM_DBG_MSG ("Failed to enable Flash \n", 0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    /* See if contents are actually changing if necessary */

    ppbuf[0] = (UINT8 *) nvRamFlashInfo.pBlkBuf;
    if (FLASH_NVRAM_READ_ALL (ppbuf) == OK)
        {
        if (bcmp ((char *) (&nvRamFlashInfo.pBlkBuf[offset]),
                            string, strLen) == 0)
            goto nvRamSetOk;
        }
    else
        bzero ((char *) nvRamFlashInfo.pBlkBuf, nvRamFlashInfo.nvRamSize);

    /* Erase if necessary */

    if (((nvRamFlashInfo.pFlashChip->uCapability & FLASH_CHIP_CAP_BLKERA) !=
        0) &&
        (nvRamFlashInfo.pFlashChip->flashOPs.blkErase != NULL) &&
        (FLASH_NVRAM_ERASE != OK))
        {
        NVRAM_DBG_MSG ("Failed to erase NVRAM\n", 0, 0, 0, 0, 0, 0);
        goto nvRamSetErr;
        }

    /* Copy data to buffer */

    bcopy (string, (char *) (&nvRamFlashInfo.pBlkBuf[offset]), strLen);
    if (FLASH_NVRAM_WRITE (ppbuf) != OK)
        {
        NVRAM_DBG_MSG ("Failed to write NVRAM\n", 0, 0, 0, 0, 0, 0);
        goto nvRamSetErr;
        }

nvRamSetOk:
    retVal = OK;

nvRamSetErr:

    /* The operations have been finished anyway */

    FLASH_NVRAM_DIS;

    return (retVal);
    }

#ifdef INCLUDE_SHOW_ROUTINES
/*******************************************************************************
*
* sysNvRamShow - show Flash driver information
*
* This routine prints the Flash driver information.
*
* RETURNS: N/A
*/

void sysNvRamShow (void)
    {
    
    /* Clear status for debug purpose */
    
    nvRamFlashInfo.status = 0;
    if (sysNvRamInit () != OK)
        {
        printf ("Failed to initialize NVRAM library, check configuration"
                " please\n");
        return;
        }
        
    printf ("Mapped Flash region : 0x%08x - 0x%08x\n",
            (UINT32) nvRamFlashInfo.offset,
            (UINT32) (nvRamFlashInfo.offset + nvRamFlashInfo.nvRamSize - 1));

    printf ("Mapped Flash Device : %s unit %d\n",
            nvRamFlashInfo.pFlashName, (UINT32) nvRamFlashInfo.flashUnit);
    }
#endif /* INCLUDE_SHOW_ROUTINES */



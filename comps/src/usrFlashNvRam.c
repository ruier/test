/* usrFlashNvRam.c - non-volatile RAM to Flash mapping library */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,11jan13,x_z  written based on wrSbcP5020/nvRamToFlash.c.
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

/* defines */

#undef FLASH_NVRAM_DBG
#ifdef FLASH_NVRAM_DBG
#   define NVRAM_DBG_MSG(fmt, a, b, c, d, e, f)                     \
        if (_func_logMsg != NULL)                                   \
            _func_logMsg ("nvRamToFlash: "fmt, (_Vx_usr_arg_t) a,   \
                          (_Vx_usr_arg_t) b, (_Vx_usr_arg_t) c,     \
                          (_Vx_usr_arg_t) d, (_Vx_usr_arg_t) e,     \
                          (_Vx_usr_arg_t) f)
#else /* !FLASH_NVRAM_DBG */
#   define NVRAM_DBG_MSG(fmt, ...)
#endif /* FLASH_NVRAM_DBG */

/* Flash functions */

#define FLASH_NVRAM_CAP_MUST    (FLASH_CHIP_CAP_RD | FLASH_CHIP_CAP_WR)

#define FLASH_NVRAM_ENA                                 \
        pFlashChip->flashOPs.ena (pFlashChip, TRUE, NULL)
#define FLASH_NVRAM_DIS                                 \
        pFlashChip->flashOPs.ena (pFlashChip, FALSE, NULL)

#define FLASH_NVRAM_LOCKQRY(blkId, pLock)               \
        pFlashChip->flashOPs.blkLockQry (pFlashChip,    \
        NV_RAM_OFFSET + ((blkId) * pFlashChip->uEraseSize), pLock)

#define FLASH_NVRAM_UNLOCK(blkId)                       \
        pFlashChip->flashOPs.blkLock (pFlashChip,       \
        NV_RAM_OFFSET + ((blkId) * pFlashChip->uEraseSize), FALSE)

#define FLASH_NVRAM_READ(offset, strLen, ppbuf)         \
        pFlashChip->flashOPs.read (pFlashChip,          \
        NV_RAM_OFFSET + (offset), 1, (strLen), (ppbuf), NULL)

#define FLASH_NVRAM_WRITE(offset, bufLen, ppbuf)                        \
        pFlashChip->flashOPs.write (pFlashChip,         \
        NV_RAM_OFFSET + (offset), 1, (bufLen), (ppbuf), NULL)

#define FLASH_NVRAM_ERASE(offset, blkCnt)                               \
        pFlashChip->flashOPs.blkErase (pFlashChip,      \
        NV_RAM_OFFSET + (offset), blkCnt)

/* NVRAM status defines */

#define NVRAM_FLASH_STA_INIT_DONE   1    /* finished for initialization */
#define NVRAM_FLASH_STA_INIT_ERR    2    /* failed for initialization */

/* locals */

LOCAL FLASH_CHIP_ID pFlashChip      = NULL; /* Flash Chip ID */
LOCAL UINT32        uTotalBlkCnt    = 0;    /* block count for NVRAM */
LOCAL UINT32        uTotalSize      = 0;    /* total size for NVRAM region */
LOCAL UINT8 *       pBlkBuf         = NULL; /* used to save block data */
LOCAL BOOL          status          = 0;    /* NVRAM status */

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
    BOOL            locked = TRUE;
    UINT32          i;
    FLASH_CHIP_ID   (*flashChipInfoGet) (VXB_DEVICE_ID pDev, UINT32 chipId);

    /* initialize NVRAM library */

    if (status == 0)
        {

        /* put to error status first */

        status = NVRAM_FLASH_STA_INIT_ERR;

        /* find Flash device and obtain chip information */

        pFlashDev = vxbInstByNameFind (NV_RAM_FLASH_NAME, NV_RAM_FLASH_UNIT);
        if (pFlashDev == NULL)
            {
            NVRAM_DBG_MSG ("Failed to find the Flash device %s%d\n",
                           NV_RAM_FLASH_NAME, NV_RAM_FLASH_UNIT, 0, 0, 0, 0);
            return (ERROR);
            }

        flashChipInfoGet =
            vxbDevMethodGet (pFlashDev, DEVMETHOD_CALL (vxbFlashChipInfoGet));
        pFlashChip = flashChipInfoGet (pFlashDev, 0);

        if ((NV_RAM_OFFSET & (pFlashChip->uEraseSize - 1)) != 0)
            {
            NVRAM_DBG_MSG ("NVRAM region offset 0x%x is unaligned with erase "
                           "size(0x%x)\n",
                           NV_RAM_OFFSET, pFlashChip->uEraseSize, 0, 0, 0, 0);
            return (ERROR);
            }

        if (NV_RAM_OFFSET >= pFlashChip->uChipSize)
            {
            NVRAM_DBG_MSG ("NVRAM region offset is out of Flash chip(0x%x)\n",
                           NV_RAM_OFFSET, pFlashChip->uChipSize, 0, 0, 0, 0);
            return (ERROR);
            }

        /* calculate and check NVRAM block count */

        uTotalBlkCnt = ((NV_RAM_SIZE - 1) / pFlashChip->uEraseSize) + 1;
        uTotalSize   = uTotalBlkCnt * pFlashChip->uEraseSize;
        if ((NV_RAM_OFFSET + uTotalSize) > pFlashChip->uChipSize)
            {
            NVRAM_DBG_MSG ("NVRAM region end 0x%x is out of Flash "
                           "chip(0x%x)\n",
                           NV_RAM_OFFSET + uTotalSize - 1,
                           pFlashChip->uChipSize, 0, 0, 0, 0);
            return (ERROR);
            }

        /* check Flash functions */

        if ((pFlashChip->uCapability & FLASH_NVRAM_CAP_MUST) !=
            FLASH_NVRAM_CAP_MUST)
            {
            NVRAM_DBG_MSG ("no enough capabilities (0x%x) for NVRAM (0x%x)\n",
                           pFlashChip->uCapability, FLASH_NVRAM_CAP_MUST,
                           0, 0, 0, 0);
            return (ERROR);
            }

        if ((pFlashChip->uCapability & FLASH_CHIP_CAP_BLKLOCK) != 0)
            {

            /* enable Flash */

            if (FLASH_NVRAM_ENA != OK)
                {
                NVRAM_DBG_MSG ("Failed to enable Flash \n", 0, 0, 0, 0, 0, 0);
                return (ERROR);
                }

            for (i = 0; i < uTotalBlkCnt; i++)
                {

                /* unlock block unless it is unlocked really */

                if (((pFlashChip->uCapability & FLASH_CHIP_CAP_BLKLOCKQRY) == 0) ||
                    ((FLASH_NVRAM_LOCKQRY (i, &locked) != OK) || locked))
                    {
                    if (FLASH_NVRAM_UNLOCK (i) != OK)
                        {
                        FLASH_NVRAM_DIS;
                        NVRAM_DBG_MSG ("Failed to unlock block @ 0x%x\n",
                                       NV_RAM_OFFSET +
                                       i * pFlashChip->uEraseSize,
                                       0, 0, 0, 0, 0);
                        return (ERROR);
                        }
                    }
                }

            FLASH_NVRAM_DIS;
            }

        /* allocate block buffer */

        pBlkBuf = (UINT8 *) malloc (uTotalSize);
        if (pBlkBuf == NULL)
            {
            NVRAM_DBG_MSG ("Failed to allocate block buffer\n",
                           0, 0, 0, 0, 0, 0);
            return (ERROR);
            }

        /* updated status */

        status = NVRAM_FLASH_STA_INIT_DONE;
        return (OK);
        }
    else if (status == NVRAM_FLASH_STA_INIT_ERR)
        {
        NVRAM_DBG_MSG ("Initialization has failed\n", 0, 0, 0, 0, 0, 0);
        return (ERROR);
        }
    else
        return (OK);
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
    STATUS  retVal = ERROR;
    UINT8 * ppbuf[1];
    int     offsetRel = offset + NV_BOOT_OFFSET;

    if ((offsetRel < 0) || (strLen < 0) ||
        ((offsetRel + strLen) > NV_RAM_SIZE))
        {
        NVRAM_DBG_MSG ("[0x%x,0x%x] is out of range(0x%x)\n",
                       offsetRel, offsetRel + strLen - 1, NV_RAM_SIZE,
                       0, 0, 0);
        return (ERROR);
        }

    /* initialize NVRAM */

    if (sysNvRamInit () != OK)
        return (ERROR);

    /* enable Flash */

    if (FLASH_NVRAM_ENA != OK)
        {
        NVRAM_DBG_MSG ("Failed to enable Flash\n", 0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    ppbuf[0] = (UINT8 *) string;
    if (FLASH_NVRAM_READ (offsetRel, strLen, ppbuf) != OK)
        {
        NVRAM_DBG_MSG ("Failed to read NVRAM\n", 0, 0, 0, 0, 0, 0);
        }
    else
        {
        string [strLen] = EOS;
        retVal = OK;
        }

    /* the get operation has been finished anyway */

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
    STATUS  retVal = ERROR;
    UINT8 * ppbuf[1];
    int     offsetRel = offset + NV_BOOT_OFFSET;
    UINT32  uBlkOffset;
    UINT32  uBlkCnt;
    UINT32  uBufLen;

    if ((offsetRel < 0) || (strLen < 0) ||
        ((offsetRel + strLen) > NV_RAM_SIZE))
        {
        NVRAM_DBG_MSG ("[0x%x,0x%x] is out of range(0x%x)\n",
                       offsetRel, offsetRel + strLen - 1, NV_RAM_SIZE,
                       0, 0, 0);
        return (ERROR);
        }

    /* initialize NVRAM */

    if (sysNvRamInit () != OK)
        return (ERROR);

    /* calculate offset and count for the blocks to be erased */

    uBlkOffset  = offsetRel - (offsetRel & (pFlashChip->uEraseSize - 1));
    uBlkCnt     =
        ((offsetRel + strLen - uBlkOffset - 1) / pFlashChip->uEraseSize) + 1;
    uBufLen     = uBlkCnt * pFlashChip->uEraseSize;

    /* enable Flash */

    if (FLASH_NVRAM_ENA != OK)
        {
        NVRAM_DBG_MSG ("Failed to enable Flash \n", 0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    /* check if contents are actually changing */

    ppbuf[0] = (UINT8 *) pBlkBuf;
    if ((FLASH_NVRAM_READ (uBlkOffset, uBufLen, ppbuf) == OK) &&
        (bcmp ((char *) (&pBlkBuf[offsetRel - uBlkOffset]), string, strLen) == 0))
        goto nvRamSetOk;

    /* erase if necessary */

    if (((pFlashChip->uCapability & FLASH_CHIP_CAP_BLKERA) != 0) &&
        (FLASH_NVRAM_ERASE (uBlkOffset, uBlkCnt) != OK))
        {
        NVRAM_DBG_MSG ("Failed to erase NVRAM\n", 0, 0, 0, 0, 0, 0);
        goto nvRamSetErr;
        }

    /* copy data to buffer and write to Flash */

    bcopy (string, (char *) (&pBlkBuf[offsetRel - uBlkOffset]), strLen);
    if (FLASH_NVRAM_WRITE (uBlkOffset, uBufLen, ppbuf) != OK)
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

    status = 0;
    if (sysNvRamInit () != OK)
        {
        printf ("Failed to initialize NVRAM library, check configuration"
                " please\n");
        return;
        }

    printf ("Mapped Flash region : 0x%08x - 0x%08x\n",
            (UINT32) NV_RAM_OFFSET,
            (UINT32) (NV_RAM_OFFSET + NV_RAM_SIZE - 1));

    printf ("Mapped Flash Device : %s unit %d\n",
            NV_RAM_FLASH_NAME, (UINT32) NV_RAM_FLASH_UNIT);
    }
#endif /* INCLUDE_SHOW_ROUTINES */


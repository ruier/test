/* sysNandFlash.c - Nand Flash Driver for LSI ACP34XX */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,21aug12,x_s  added support for EP501G3 NAND controller.
01d,21jan11,x_z  fixed NVRAM routines.
01c,21dec10,x_z  fixed compiling warning.
01b,09nov10,x_z  modified to support EP501G1 controller.
01a,07apr10,x_z  initial creation.
*/

/*
DESCRIPTION

This file provides Nand Flash Driver routines and non-volatile RAM to NAND Flash
mapping routinefor LSI ACP34XX. To use it, INCLUDE_SYS_NAND must be defined in
config.h. Otherwise, Before using read and write routines, sysNandInit() should
be run to initialize NAND controller.

Note: EP501G1 & EP501G3 controllers are only supported, and ST NAND08GW3B2 is 
only verified on LSI ACP34XX.
*/

/* includes */

#include <vxWorks.h>
#include <ffsLib.h>
#include <string.h>

#include "config.h"

/* defines */

#define SYS_NAND_DELAY              5

#ifdef  SYS_NAND_BOARD_DELAY
IMPORT void sysUsDelay (int);
#   define SYS_NAND_DELAY_RTN(x)    sysUsDelay(x)
#else
#   define SYS_NAND_DELAY_CNT       10000
#   define SYS_NAND_DELAY_RTN(x)    \
        {                           \
        int ix;                     \
        for(ix = 0; ix < (x * SYS_NAND_DELAY_CNT); ix++);   \
        }
#endif  /* SYS_FLASH_BOARD_DELAY */

#define SYS_NAND_MSG(fmt, a, b, c, d, e, f)           \
        if (_func_logMsg != NULL)                       \
            _func_logMsg ("sysNandFlash : "fmt, a, b, c, d, e, f)

#define SYS_NAND_BUF_LEN    2112 /* large page size */

/* locals */

LOCAL STATUS sysNandCmdIssue (UINT32, int);

/* forward declarations */

LOCAL UINT32 sysNandMafId;
LOCAL UINT32 sysNandDevId;
LOCAL UINT32 sysNandPageSize;
LOCAL UINT32 sysNandPageShift;
LOCAL UINT32 sysNandPageMask;
LOCAL UINT32 sysNandBlockSize;
LOCAL UINT32 sysNandBlockShift;
LOCAL UINT32 sysNandBlockMask;
LOCAL UINT32 sysNandChipShift;
LOCAL UINT32 sysNandFailed;
LOCAL UINT32 sysNandEccCorrected;
LOCAL UINT32 sysNandEccUnCorrected;
LOCAL UINT32 sysNandCommandDis;

/*******************************************************************************
*
* sysNandCmdIssue - issue command
*
* This routine issues the nand controller command.
*
* RETURNS: OK or ERROR if failed for ECC
*/

LOCAL STATUS sysNandCmdIssue
    (
    UINT32  cmd,
    int     page
    )
    {
    UINT32 index = 0;
    UINT32 extIndex = 0;

    sysOutLong (NAND_CMD, cmd);

    if (page != -1)
        {
        index += ((page & NAND_INDEX_CALC(sysNandPageSize)) <<
            (sysNandPageShift + 1));
        extIndex += (page & NAND_EXT_INDEX_CALC(sysNandPageSize));

        /* Write index reg */

        sysOutLong (NAND_INDEX, index);

        /* Write ext index reg */

        sysOutLong (NAND_EXT_INDEX, extIndex);
        }

    if (cmd == NAND_CMD_RD0)
        sysOutLong (NAND_CMD, NAND_CMD_PAGE_OPEN);
    else if(cmd  == NAND_CMD_STA)
        {

        /* write-protected check */

        if (0 == (sysInLong (NAND_STATUS0) & NAND_STA_WP))
            return (ERROR);
        }

    /* wait for ECC calculation*/

    if ((cmd == NAND_CMD_RD0) || (cmd == NAND_CMD_ECC_STA_RD) ||
        (cmd == NAND_CMD_PAGEPROG_CONFIRM))
        {
        while (0 != (sysInLong (GPREG_AXIS_STATUS) & GPREG_AXIS_NAND_ECC_BUSY))
            SYS_NAND_DELAY_RTN (SYS_NAND_DELAY);
        }

    /* wait for chip BUSY */

    if ((cmd == NAND_CMD_RD0) || (cmd == NAND_CMD_ECC_STA_RD) ||
        (cmd == NAND_CMD_PAGEPROG_CONFIRM) || (cmd == NAND_CMD_ERASE_CONFIRM))
        {
        while (0 == (sysInLong (NAND_INT_STATUS) & NAND_INT_STATUS_RDY))
            SYS_NAND_DELAY_RTN (SYS_NAND_DELAY);
        }

    /* check ECC status */

    if ((cmd == NAND_CMD_ECC_STA_RD) || (cmd == NAND_CMD_PAGEPROG_CONFIRM) ||
        (cmd == NAND_CMD_ERASE_CONFIRM))
        {

        /* wait for  ready of ECC status */

        do
            {
            sysOutLong (NAND_CMD, NAND_CMD_STA);
            SYS_NAND_DELAY_RTN (SYS_NAND_DELAY);
            } while (0 == (sysInLong (NAND_STATUS0) & NAND_STA_READY));

        /* check ECC status */

        sysOutLong (NAND_CMD, NAND_CMD_STA);
        SYS_NAND_DELAY_RTN (SYS_NAND_DELAY);
        if (0 != (sysInLong (NAND_STATUS0) & NAND_STA_FAIL))
            {
            sysNandFailed++;
            SYS_NAND_MSG ("Failure status encountered at 0x%.8x\n",
                (page << sysNandPageShift), 0, 0, 0, 0, 0);
            return (ERROR);
            }
        }

    return (OK);
    }

/*******************************************************************************
*
* sysNandInit - intialize the nand controller
*
* This routine initializes the nand controller.
*
* RETURNS: N/A
*/

VOID sysNandInit
    (
    UINT32 nandType   /* NAND flash controller type: EP501G3 or EP501G1 or others */
    )
    {
    UINT8  regVal;

    /* Select "turn off chip" command based on NAND controller */
    
    sysNandCommandDis = (nandType == NAND_TYPE_EP501G3)? NAND_CMD_CE_DIS_EP501G3 : 
                        NAND_CMD_CE_DIS;
    
    /* Clear all interrupt status for ECC  */

    sysOutLong (NAND_INT_STATUS, 0);

    /* Disable ECC interrupt and enable ECC detection/correction */

    sysOutLong (NAND_INT_EN, 0);

    /* Configuration timing  */

    sysOutLong (NAND_TIMING0, NAND_TIMING0_VAL);
    sysOutLong (NAND_TIMING1, NAND_TIMING1_VAL);
    
    if (nandType == NAND_TYPE_EP501G3)
        sysOutLong (NAND_TIMING2, NAND_TIMING2_VAL);

    /* Reset NAND and read NAND FLASH ID twice */

    sysNandCmdIssue (NAND_CMD_RST, -1);
    sysNandCmdIssue (NAND_CMD_ID_RD, -1);

    sysNandMafId = sysInByte (NAND_ID0);
    sysNandDevId = sysInByte (NAND_ID2);

    sysNandCmdIssue (NAND_CMD_ID_RD, -1);
    if ((sysNandMafId != sysInByte (NAND_ID0)) ||
        (sysNandDevId != sysInByte (NAND_ID2)))
        return;

    regVal = sysInByte (NAND_ID6);

    /* Calcuate page size */

    sysNandPageSize =
        NAND_ID_PAGE_SIZE (regVal & NAND_ID_PAGE_SIZE_MASK);

    /* Calculate address shift and mask from page size */

    sysNandPageShift = FFS_LSB (sysNandPageSize) - 1;
    sysNandPageMask = (NAND_CHIP_SIZE >> sysNandPageShift) - 1;

    /* Calcuate block size(multiples of 64K Bytes) */

    sysNandBlockSize =
        NAND_ID_BLOCK_SIZE (regVal & NAND_ID_BLOCK_SIZE_MASK);

    /* Calculate address shift and mask from block size */

    sysNandBlockShift = FFS_LSB (sysNandBlockSize) - 1;
    sysNandBlockMask =
        (1 << (sysNandBlockShift - sysNandPageShift)) - 1;

    /* Calculate address shift and mask from chip size */

    sysNandChipShift = FFS_LSB (NAND_CHIP_SIZE) - 1;

    /* Zero the stats counters */

    sysNandFailed = 0;
    sysNandEccCorrected = 0;
    sysNandEccUnCorrected = 0;

    /* Configuration NAND flash and disable write protection */

    sysOutLong (NAND_CFG, ((nandType == NAND_TYPE_EP501G3) ? 
        NAND_CFG_RAW_MODE_EP501G3 : NAND_CFG_RAW_MODE) |
        ((sysNandPageSize > NAND_CFG_LP_CALC_BASE) ? NAND_CFG_LP : 0) |
        NAND_CFG_WP_DIS | NAND_CFG_PAGE_MODE (sysNandPageSize) |
        (FFS_LSB (NAND_CHIP_SIZE >> NAND_CFG_SIZE_SHIFT)));       
    }

/*******************************************************************************
*
* sysNandPageRead - read one page
*
* This routine gets one page data of the nand flash.
*
* RETURNS: OK or ERROR if failed
*/

STATUS sysNandPageRead
    (
    UINT32 adrs,
    UINT32 len, /* must be less than sysNandPageSize */
    char * buf
    )
    {
    UINT8 * pDest;
    UINT32  status;
    UINT32  i;
    UINT32  j;
    UINT32  bufIndex = 0;
    UINT8   rest[4];
    UINT32  align = adrs & (sysNandPageSize - 1);

    if (len > (sysNandPageSize - align))
        {
        SYS_NAND_MSG("Only %d bytes can be read at 0x%.8x\n",
            sysNandPageSize - align,
            ((adrs - align) >> sysNandPageShift) & sysNandPageMask,
            0, 0, 0, 0);
        return (ERROR);
        }

    pDest = (UINT8 *) buf;
    sysNandCmdIssue (NAND_CMD_RD0,
        ((adrs - align) >> sysNandPageShift) & sysNandPageMask);

    /* read and drop needless bytes */

    if (align)
        {
        for (i = 0; (i < align) & ((align -i) >= sizeof(UINT32));
            i += sizeof(UINT32))
            sysInLong (NAND_DATA);

        if ((align - i) < sizeof(UINT32))
            {
            *(UINT32 *) rest = sysInLong (NAND_DATA);
            for (j = align - i; j < sizeof(UINT32); j++)
                pDest[bufIndex++] = rest[j];
            }
        }

    /* Must be accessed by 4 bytes */

    for (; (bufIndex < len) & ((len -bufIndex) >= sizeof(UINT32));
        bufIndex += sizeof(UINT32))
        *(UINT32 *) (&pDest[bufIndex]) = sysInLong (NAND_DATA);

    if (bufIndex < len)
        {
        *(UINT32 *) rest = sysInLong (NAND_DATA);
        for (j = 0; bufIndex < len; j++, bufIndex++)
            pDest[bufIndex] = rest[j];
        }

    /* Calculate ECC and wait for finish */

    sysNandCmdIssue (NAND_CMD_ECC_STA_RD, -1);

    /* Read ECC Status and check for errors */

    for(i = 0; i < (sysNandPageSize / 1024); i++)
        {
        status = sysInLong (NAND_PECC + i * 4);

        if (status & NAND_PECC_ERR_SINGLE)
            sysNandEccCorrected++;
        else if (status & NAND_PECC_ERR_MULTI)
            {
            sysNandEccUnCorrected++;
            SYS_NAND_MSG ("Uncorrected ECC error at 0x%.8x while reading\n",
                ((adrs - align) >> sysNandPageShift) & sysNandPageMask,
                0, 0, 0, 0, 0);
            return (ERROR);
            }
        }

    /* Turn off chip enable */

    sysOutLong (NAND_CMD, sysNandCommandDis);
    return (OK);
    }

/*******************************************************************************
*
* sysFlashGet - get the contents of NAND Flash
*
* This routine copies the contents of NAND Flash into a specified
* string.  The string is terminated with an EOS.
*
* RETURNS: OK, or ERROR if access is outside the NAND Flash range.
*
* SEE ALSO: sysFlashSet()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(),
* they should use a semaphore to ensure mutually exclusive access.
*/

STATUS sysFlashGet
    (
    char *  string, /* where to copy flash memory      */
    UINT32  strLen, /* maximum number of bytes to copy */
    UINT32  offset  /* byte offset into flash memory   */
    )
    {
    UINT32  bytes = 0;
    char *  buf = string;
    UINT32  rdAdrs = offset;
    UINT32  align = offset & (sysNandPageSize - 1);
    UINT32  rdLen;

    if ((offset + strLen) > NAND_CHIP_SIZE)
        return (ERROR);

    /* un-align offset */

    while (bytes < strLen)
        {
        if ((strLen - bytes) >= (sysNandPageSize - align))
            rdLen = sysNandPageSize - align;
        else
            rdLen = strLen - bytes;

        if (sysNandPageRead (rdAdrs, rdLen, buf) != OK)
            return (ERROR);

        buf += rdLen;
        bytes += rdLen;
        rdAdrs += rdLen;
        align = 0;
        }

    string [strLen] = EOS;
    return (OK);
    }

/*******************************************************************************
*
* sysNandPageWrite - write one page
*
* This routine writes data into one page of the nand flash. The controller will
* automatically fill in the remaining of the page with "0xFF" if the length
* specified by <len> is less than the page size.
*
* RETURNS: OK or ERROR if ECC failed
*/

STATUS sysNandPageWrite
    (
    UINT32 adrs, /* must be aligned with page size */
    UINT32 len,  /* must be aligned with 4 bytes */
    char * buf
    )
    {
    volatile UINT32 *   pSrc;
    UINT32              i;
    STATUS              status;

    if (((adrs & (sysNandPageSize - 1)) != 0) ||
        ((len & (sizeof (UINT32) - 1)) != 0))
        return (ERROR);

    pSrc = (volatile UINT32 *) buf;
    sysNandCmdIssue (NAND_CMD_PAGEPROG,
        (adrs >> sysNandPageShift) & sysNandPageMask);

    /* Must be access by 4 bytes */

    for (i = 0; i < (len >> 2); i++)
        sysOutLong (NAND_DATA, pSrc[i]);

    /* Calculate ECC and wait for finish */

    status = sysNandCmdIssue (NAND_CMD_PAGEPROG_CONFIRM, -1);

    /* Turn off chip enable */

    sysOutLong (NAND_CMD, sysNandCommandDis);

    /* check ECC status */

    if (status == ERROR)
        {
        SYS_NAND_MSG ("page writing failed at 0x%.8x\n",
            (adrs >> sysNandPageShift) & sysNandPageMask,
            0, 0, 0, 0, 0);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* sysNandBlockErase - erase one block
*
* This routine erases one block of the nand flash.
*
* RETURNS: OK or ERROR if erase failed
*/

STATUS sysNandBlockErase
    (
    UINT32 adrs /* must be aligned with block size */
    )
    {
    if ((adrs & (sysNandBlockSize- 1)) != 0)
        return (ERROR);

    sysNandCmdIssue (NAND_CMD_ERASE,
        (adrs >> sysNandPageShift) & sysNandPageMask);
    if (ERROR == sysNandCmdIssue (NAND_CMD_ERASE_CONFIRM, -1))
        {
        SYS_NAND_MSG ("block erasing failed at 0x%.8x\n",
            (adrs >> sysNandPageShift) & sysNandPageMask,
            0, 0, 0, 0, 0);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* sysFlashSet - write to NAND Flash
*
* This routine copies a specified string into NAND Flash.
*
* RETURNS: OK, or ERROR if the write fails or the input parameters are
* out of range.
*
* SEE ALSO: sysFlashGet()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(),
* they should use a semaphore to ensure mutually exclusive access to flash
* memory.
*/

STATUS sysFlashSet
    (
    char *  string, /* string to be copied into flash memory */
    UINT32  strLen, /* maximum number of bytes to copy       */
    UINT32  offset  /* byte offset into flash memory         */
    )
    {
    char *  buf;
    char *  srcAdrs;
    UINT32  bytes = 0;
    UINT32  alignAdrs = offset & (~(sysNandBlockSize - 1));

    if (((offset + strLen) > NAND_CHIP_SIZE) ||
        ((offset - alignAdrs + strLen ) > sysNandBlockSize))
        return (ERROR);

    /* read existing data and see if contents are actually changing */

    if ((buf = malloc (sysNandBlockSize + 1)) == NULL)
        return (ERROR);

    if ((sysFlashGet (buf,  sysNandBlockSize, alignAdrs) != OK) ||
        (bcmp ((char *) (buf + (offset - alignAdrs)), string, strLen) == 0))
        {
        free (buf);
        return (ERROR);
        }

    bcopyBytes (string, buf + (offset - alignAdrs), strLen);

    if (sysNandBlockErase (alignAdrs) != OK)
        {
        free (buf);
        return (ERROR);
        }

    srcAdrs = buf;
    while (bytes < sysNandBlockSize)
        {
        if (sysNandPageWrite (alignAdrs, sysNandPageSize, srcAdrs) != OK)
            {
            free (buf);
            return (ERROR);
            }

        alignAdrs += sysNandPageSize;
        srcAdrs += sysNandPageSize;
        bytes += sysNandPageSize;
        }

    free (buf);
    return (OK);
    }

/******************************************************************************
*
* sysNvRamGet - get the contents of non-volatile RAM
*
* This routine calls sysFlashGet() to copy the contents of flash memory into a
* specified string.  The string is terminated with an EOS.
*
* RETURNS: The return value of sysFlashGet().
*
* SEE ALSO: sysNvRamSet(), sysFlashGet()
*/

STATUS sysNvRamGet
    (
    char *  string, /* where to copy non-volatile RAM    */
    int     strLen, /* maximum number of bytes to copy   */
    int     offset  /* byte offset into non-volatile RAM */
    )
    {
    STATUS retVal;

    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > NV_RAM_SIZE))
        return (ERROR);

    retVal = sysFlashGet (string, strLen, NV_RAM_ADRS + offset);
    string [strLen] = EOS;

    return (retVal);
    }

/*******************************************************************************
*
* sysNvRamSet - write to non-volatile RAM
*
* This routine calls sysFlashSet() to copy a specified string into Flash memory.
*
* RETURNS: The return value of sysFlashSet().
*
* SEE ALSO: sysNvRamGet(), sysFlashSet()
*/

STATUS sysNvRamSet
    (
    char *  string, /* string to be copied into non-volatile RAM */
    int     strLen, /* maximum number of bytes to copy           */
    int     offset  /* byte offset into non-volatile RAM         */
    )
    {
    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > NV_RAM_SIZE))
        return (ERROR);

    return (sysFlashSet (string, strLen, NV_RAM_ADRS + offset));
    }


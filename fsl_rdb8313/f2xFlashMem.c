/* f2xFlashMem.c - Intel28 and AMD29-style flash driver library */

/*
 * Copyright (c) 2000-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,24jan05,tjf fixed bug related calls to f2xGroupRegionInfoGet() with 0 nbytes
01l,23dec04,tjf fixed bug related to tickGet() wrap where erase/program
                routines can falsely timeout and abort with ERROR.  The bug
                occurs if an erase/program operation runs F2X_TIMEOUT seconds
                or less before tickGet() wraps to zero.  The frequency of
                exposure is every 828 days after startup for a 60Hz sysClk.
01k,23dec04,tjf changed some call argument from '(F2X_RID)pRDesc' to 'id' - nit
01j,14dec04,tjf changed f2xRegionSet() to read flash with f2xRegionGet()
01i,02dec04,tjf changed f2xRegionWidthRead/Write() API for non-standard designs
                and "macroized" internals of f2xRegionWidthRead()
01h,16jan04,tjf "macroized" PPC sync command for diab compilation
01g,10oct03,tjf added write to buffer support for AMD MirrorBit flash
01f,02jan03,tjf changed sysNvRamXxx() routines to be kosher with the BSP VTS
01e,07dec02,tjf added INCLUDE_F2X_FORCE_WIDTH_READ switch to support H/W that
                can only perform region-width-sized reads
01d,16aug02,tjf added write to buffer support for Intel StrataFlash
01c,17may02,tjf added F2X_SYS_NV_RAM_OFFSET
01b,23jan01,tjf removed f2xRegionCreate() chipBlockSize power of 2 requirement
01a,19oct00,tjf written by Ted Friedl, Wind River PS, Madison, WI
*/

/*
DESCRIPTION
[This is an out-dated description.  A proper description is will
be inserted after the MSWord-based document is finished.]

This library provides routines useful for programming most
sane implementations of flash memory supporting Intel's Basic
Command Set.  The Basic Command Set (or BCS) is a group of
commands that have been used for years by Intel and other
vendors' legacy products.  This command set is also commonly
refered to as the 28f008 or simply the 008 command set.

BCS flash are manufactured 1, 2 or 4 bytes wide, with
erasable block lengths as small a 4 kilobytes, and as large
as 128 kilobytes.  They usually contain multiple blocks which
are not necessarily all one size.

When BCS flash is connected to the address space of a 32-bit
or 64-bit CPU, there are many possiblities; They may be connected
to the CPU address space using hardware to make the flash
appear wide, or in parallel using a memory bank strategy.

This library supports one or more "regions" of BCS flash.
Here a region is defined as a contiguous area of BCS flash
memory consisting of one block size which starts and ends on
block boundaries of the logically first (or only) bank.

To use this library, include this file in sysLib.c and define
INCLUDE_FLASH in config.h.  Then, edit sysLib.c and in
the routine sysHwInit(), call f2xInit() for each region.
If f2xInit() is called more then once, define BCS_NUM_REGION
(in config.h) to be equal to the number of calls.

Each region can be controlled using the routines f2xFlashSet(),
f2xFlashGet(), f2xFlashErase() and f2xFlashSize().  By defining
the constant INCLUDE_F2X_SYS_FLASH_ROUTINES, all regions can appear
as one through the routines sysFlashSet(), sysFlashGet(),
sysFlashErase() and sysFlashSize().
*/

/* includes */

#include "vxWorks.h"
#include "sysLib.h"
#include "taskLib.h"
#include "memLib.h"
#include "semLib.h"
#include "tickLib.h"
#include "string.h"
#include "f2xFlashMem.h"

/* defines */

#ifndef F2X_MAX_REGIONS
#define F2X_MAX_REGIONS        16
#endif /* F2X_MAX_REGIONS */

#ifndef F2X_MAX_GROUPS
#define F2X_MAX_GROUPS         16
#endif /* F2X_MAX_GROUPS */

/* WARNING: The F2X_UINTxx_READ() macros are at times bypassed unless
 *          INCLUDE_F2X_FORCE_WIDTH_READ is defined.
 */

#ifndef F2X_UINT8_READ
#define F2X_UINT8_READ(id, pSrc, pDst, isData) \
                               {*(UINT8 *)(pDst) = *(volatile UINT8 *)(pSrc);}
#endif /* F2X_UINT8_READ */

#ifndef F2X_UINT16_READ
#define F2X_UINT16_READ(id, pSrc, pDst, isData) \
                               {*(UINT16 *)(pDst) = *(volatile UINT16 *)(pSrc);}
#endif /* F2X_UINT16_READ */

#ifndef F2X_UINT32_READ
#define F2X_UINT32_READ(id, pSrc, pDst, isData) \
                               {*(UINT32 *)(pDst) = *(volatile UINT32 *)(pSrc);}
#endif /* F2X_UINT32_READ */

#ifndef F2X_UINT64_READ
#define F2X_UINT64_READ(id, pSrc, pDst, isData) \
                               {*(double *)(pDst) = *(volatile double *)(pSrc);}
#endif /* F2X_UINT64_WRITE */

#ifndef F2X_UINT8_WRITE
#define F2X_UINT8_WRITE(id, pSrc, pDst, isData) \
                               {*(volatile UINT8 *)(pDst) = *(UINT8 *)(pSrc);}
#endif /* F2X_UINT8_WRITE */

#ifndef F2X_UINT16_WRITE
#define F2X_UINT16_WRITE(id, pSrc, pDst, isData) \
                               {*(volatile UINT16 *)(pDst) = *(UINT16 *)(pSrc);}
#endif /* F2X_UINT16_WRITE */

#ifndef F2X_UINT32_WRITE
#define F2X_UINT32_WRITE(id, pSrc, pDst, isData) \
                               {*(volatile UINT32 *)(pDst) = *(UINT32 *)(pSrc);}
#endif /* F2X_UINT32_WRITE */

#ifndef F2X_UINT64_WRITE
#define F2X_UINT64_WRITE(id, pSrc, pDst, isData) \
                               {*(volatile double *)(pDst) = *(double *)(pSrc);}
#endif /* F2X_UINT64_WRITE */

#define F2X_ALIGN(x,align)     ((unsigned)(x) - ((unsigned)(x) % (align)))

#ifndef F2X_SYS_NV_RAM_OFFSET
#define F2X_SYS_NV_RAM_OFFSET  0
#endif /* F2X_SYS_NV_RAM_OFFSET */

#ifndef NV_RAM_SIZE
#define NV_RAM_SIZE            255
#endif /* NV_RAM_SIZE */

/* typedefs */

typedef struct                 /* F2X_GDESC */
    {
    F2X_RID   ridArray[F2X_MAX_REGIONS];    /* regions of the group */
    int       ridArrayCount;                /* number of regions in the group */
    int       groupSize;                    /* total size of group in bytes */
    int       pseudoBlockSize;              /* see f2xGroupBlockSize() */
    SEMAPHORE semaphore;                    /* group-level semaphore */
    SEM_ID    semId;                        /* pointer to semaphore above */
    } F2X_GDESC;

/* globals */

#ifdef INCLUDE_F2X_SYS_FLASH_ROUTINES
F2X_GID f2xSysFlashGid = (F2X_GID)NULL;
#endif /* INCLUDE_F2X_SYS_FLASH_ROUTINES */
#ifdef INCLUDE_F2X_SYS_NV_RAM_ROUTINES
F2X_GID f2xSysNvRamGid = (F2X_GID)NULL;
#endif /* INCLUDE_F2X_SYS_NV_RAM_ROUTINES */

/* locals */

LOCAL F2X_RDESC f2xRDescArray [F2X_MAX_REGIONS];
LOCAL int       f2xRDescArrayCount = 0;
LOCAL F2X_GDESC f2xGDescArray [F2X_MAX_GROUPS];
LOCAL int       f2xGDescArrayCount = 0;
LOCAL char      f2xErasedArray [F2X_MAX_REGION_WIDTH];

/* region-level routines ---------------------------------------------------- */

/*******************************************************************************
*
* f2xRegionWidthRead - read flash region
*
* This routine reads the entire width of a flash region into a buffer.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: N/A.
*
* NOMANUAL
*/

void f2xRegionWidthRead
    (
    F2X_RID id,
    char *  pDst,
    int     offset,
    BOOL    isData
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    char *      pSrc = pRDesc->baseAdrs + offset;
    double      buf;    /* aligned buffer */
    char *      pBuf = (char *)&buf;

    /* perform the aligned read into the aligned buffer */

    switch (pRDesc->regionWidth)
        {
        case 1:
            F2X_UINT8_READ(id, pSrc, pBuf, isData);
            break;
        case 2:
            F2X_UINT16_READ(id, pSrc, pBuf, isData);
            break;
        case 4:
            F2X_UINT32_READ(id, pSrc, pBuf, isData);
            break;
        case 8:
            F2X_UINT64_READ(id, pSrc, pBuf, isData);
            break;
        default:
            break;
        }

    /* copy aligned buffer into possibly unaligned buffer */

    bcopy (pBuf, pDst, pRDesc->regionWidth);
    }

/*******************************************************************************
*
* f2xRegionWidthWrite - write flash region
*
* This routine writes a buffer over the entire width of a flash region.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: N/A.
*
* NOMANUAL
*/

void f2xRegionWidthWrite
    (
    F2X_RID id,
    char *  pSrc,
    int     offset,
    BOOL    isData
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    char *      pDst = pRDesc->baseAdrs + offset;
    int         numWrites = pRDesc->regionWidth / pRDesc->writeWidth;
    int         writeIndex;
    double      buf;    /* aligned buffer */
    char *      pBuf = (char *)&buf;

    /* copy possibly unaligned buffer into aligned buffer */

    bcopy (pSrc, pBuf, pRDesc->regionWidth);

    /* perform the aligned write from the aligned buffer */

    for (writeIndex = 0; writeIndex < numWrites; writeIndex++)
        {
        switch (pRDesc->writeWidth)
            {
            case 1:
                F2X_UINT8_WRITE(id, pBuf, pDst, isData);
                break;
            case 2:
                F2X_UINT16_WRITE(id, pBuf, pDst, isData);
                break;
            case 4:
                F2X_UINT32_WRITE(id, pBuf, pDst, isData);
                break;
            case 8:
                F2X_UINT64_WRITE(id, pBuf, pDst, isData);
                break;
            default:
                break;
            }
        pBuf += pRDesc->writeWidth;
        pDst += pRDesc->writeWidth;
        }

#if (CPU_FAMILY == PPC)
    /* sync in case this space is not guarded */

#ifdef WRS_ASM
    WRS_ASM("sync");
#else /* WRS_ASM */
    __asm__("sync");
#endif /* WRS_ASM */
#endif /* (CPU_FAMILY == PPC) */
    }

/*******************************************************************************
*
* f2xRegionCmdCreate - create flash command
*
* This routine creates a flash command buffer.  This routine is also
* used to create byte/command counts in f2xRegionWBProgram().
*
* RETURNS: N/A
*/

LOCAL void f2xRegionCmdCreate
    (
    F2X_RID id,
    char *  pDst,
    char    cmdByte
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    int         numChips = pRDesc->regionWidth / pRDesc->chipWidth;
    int         chipIndex;

    for (chipIndex = 0; chipIndex < numChips; chipIndex++)
        {
        switch (pRDesc->chipWidth)
            {
            case 1:
                *(UINT8 *)pDst = (UINT8)cmdByte;
                break;
            case 2:
                *(UINT16 *)pDst = (UINT16)cmdByte;
                break;
            case 4:
                *(UINT32 *)pDst = (UINT32)cmdByte;
                break;
            default:
                break;
            }
        pDst += pRDesc->chipWidth;
        }
    }

/*******************************************************************************
*
* f2xRegionBlockSize - get flash region block size
*
* This routine returns the size of a flash region block in bytes.
*
* RETURNS: Size of flash region block in bytes.
*/

LOCAL int f2xRegionBlockSize
    (
    F2X_RID id
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    int         blockSize;

    blockSize = pRDesc->chipBlockSize * (pRDesc->regionWidth /
                                         pRDesc->chipWidth);

    return (blockSize);
    }

/*******************************************************************************
*
* f28RegionIsBusy - checks if Intel28-style flash region is busy
*
* This routine checks if the Intel28-style flash region is busy.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: TRUE if any flash chip in the region is busy, else FALSE.
*/

LOCAL BOOL f28RegionIsBusy
    (
    F2X_RID id
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    char        statusArray[F2X_MAX_REGION_WIDTH];
    char *      pChip = statusArray;
    int         numChips = pRDesc->regionWidth / pRDesc->chipWidth;
    char        statusByte;
    int         chipIndex;

    f2xRegionWidthRead (id, statusArray, 0, FALSE);

    for (chipIndex = 0; chipIndex < numChips; chipIndex++)
        {
        switch (pRDesc->chipWidth)
            {
            case 1:
                statusByte = (char)*(UINT8 *)pChip;
                break;
            case 2:
                statusByte = (char)*(UINT16 *)pChip;
                break;
            case 4:
            default:
                statusByte = (char)*(UINT32 *)pChip;
                break;
            }

        if ((statusByte & F28_STAT_WSMS) == 0)
            return (TRUE);

        pChip += pRDesc->chipWidth;
        }

    return (FALSE);
    }

/*******************************************************************************
*
* f28RegionStatusGet - get status of Intel28-style flash region
*
* This routine returns the status bytes of all Intel28-style
* chips or'ed into one byte.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: The or'ed status byte.
*/

LOCAL char f28RegionStatusGet
    (
    F2X_RID id
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    char        statusArray[F2X_MAX_REGION_WIDTH];
    char *      pChip = statusArray;
    int         numChips = pRDesc->regionWidth / pRDesc->chipWidth;
    char        statusByte = 0;
    int         chipIndex;

    f2xRegionWidthRead (id, statusArray, 0, FALSE);

    for (chipIndex = 0; chipIndex < numChips; chipIndex++)
        {
        switch (pRDesc->chipWidth)
            {
            case 1:
                statusByte |= (char)*(UINT8 *)pChip;
                break;
            case 2:
                statusByte |= (char)*(UINT16 *)pChip;
                break;
            case 4:
                statusByte |= (char)*(UINT32 *)pChip;
                break;
            default:
                break;
            }
        pChip += pRDesc->chipWidth;
        }

    return (statusByte);
    }

/*******************************************************************************
*
* f28RegionProgram - program Intel28-style flash region
*
* This routine programs erased Intel28-style flash in multiples
* of the flash region width.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if programming failed.
*/

LOCAL STATUS f28RegionProgram
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset
    )
    {
    STATUS      status = OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    ULONG       tickSave;
    ULONG       timeoutTicks;

    while (nbytes > 0)
        {
        /* send the command */

        f2xRegionWidthWrite (id, pRDesc->cmdProgram, 0, FALSE);

        /* write the word */

        f2xRegionWidthWrite (id, pSrc, offset, TRUE);

        /* wait for write to complete */

        tickSave = tickGet();
        timeoutTicks = sysClkRateGet() * F2X_TIMEOUT;
        while (f28RegionIsBusy(id))
            {
            if ((tickGet() - tickSave) > (ULONG)timeoutTicks)
                {
                status = ERROR;
                goto f28RegionProgramExit;
                }
            }

        pSrc += pRDesc->regionWidth;
        nbytes -= pRDesc->regionWidth;
        offset += pRDesc->regionWidth;
        }

    /* check status of the write */

    if ((f28RegionStatusGet (id) &
        (F28_STAT_PS | F28_STAT_PSS | F28_STAT_BLS)) != 0)
        status = ERROR;

f28RegionProgramExit:

    f2xRegionWidthWrite (id, pRDesc->cmdClearStatus, 0, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdReadArray, 0, FALSE);

    return (status);
    }

/*******************************************************************************
*
* f28RegionWBProgram - "write to buffer"-program Intel28-style flash region
*
* This routine uses the Write to Buffer command to program erased
* Intel28-style flash having a chip write buffer size of <chipWBSize>
* bytes, in multiples of the flash region width.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if programming failed.
*/

LOCAL STATUS f28RegionWBProgram
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset,
    int     chipWBSize
    )
    {
    STATUS      status = OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    ULONG       tickSave;
    ULONG       timeoutTicks;
    int         numChips = pRDesc->regionWidth / pRDesc->chipWidth;
    int         bytesInWriteBuffers = chipWBSize * numChips;
    char        pWriteBufferCount[F2X_MAX_REGION_WIDTH];
    int         alignedOffset;
    int         bytesToProgram;
    int         bytesToProgramLast = 0;

    while (nbytes > 0)
        {
        /* Write to Buffer commands run faster when the offset is naturally
         * aligned.  We make it so here.
         */

        alignedOffset = F2X_ALIGN (offset, bytesInWriteBuffers);

        bytesToProgram = min(nbytes,
                             bytesInWriteBuffers - (offset - alignedOffset));

        /* send the command */

        f2xRegionWidthWrite (id, pRDesc->cmdWriteBuffer, 0, FALSE);

        /* According to the Write to Buffer command flow chart, we should
         * check the XSR register here and repeat the Write to Buffer command
         * if busy.  This is problematic when multiple chips span a word
         * because they will come ready at different times, and the next
         * Write to Buffer command will be interpretted as a count to those
         * who are ready.  We instead terminate every Write to Buffer command
         * so we can assume the write buffer(s) are always ready.
         */

        if (bytesToProgram != bytesToProgramLast)
            {
            /* create the chip byte/word count (bus) word */

            f2xRegionCmdCreate (id, pWriteBufferCount,
                                bytesToProgram/numChips/pRDesc->chipWidth - 1);

            bytesToProgramLast = bytesToProgram;
            }

        /* write the byte count */

        f2xRegionWidthWrite (id, pWriteBufferCount, 0, FALSE);

        /* write the data */

        nbytes -= bytesToProgram;
        while (bytesToProgram > 0)
            {
            f2xRegionWidthWrite (id, pSrc, offset, TRUE);

            pSrc += pRDesc->regionWidth;
            bytesToProgram -= pRDesc->regionWidth;
            offset += pRDesc->regionWidth;
            }

        /* write the confirmation */

        f2xRegionWidthWrite (id, pRDesc->cmdConfirm, 0, FALSE);

        /* wait for write to complete */

        tickSave = tickGet();
        timeoutTicks = sysClkRateGet() * F2X_TIMEOUT;
        while (f28RegionIsBusy(id))
            {
            if ((tickGet() - tickSave) > (ULONG)timeoutTicks)
                {
                status = ERROR;
                goto f28RegionWBProgramExit;
                }
            }

        /* check status of the write */

        if ((f28RegionStatusGet (id) &
            (F28_STAT_PS | F28_STAT_PSS | F28_STAT_BLS)) != 0)
            {
            status = ERROR;
            goto f28RegionWBProgramExit;
            }
        }

f28RegionWBProgramExit:

    f2xRegionWidthWrite (id, pRDesc->cmdClearStatus, 0, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdReadArray, 0, FALSE);

    return (status);
    }

/*******************************************************************************
*
* f28RegionWB32Program - "write to buffer"-program Intel28-style flash region
*
* This routine uses the Write to Buffer command to program erased
* Intel28-style flash having a chip write buffer size of 32 bytes,
* in multiples of the flash region width.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if programming failed.
*/

LOCAL STATUS f28RegionWB32Program
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset
    )
    {
    STATUS status;

    status = f28RegionWBProgram (id, pSrc, nbytes, offset, 32);

    return (status);
    }

/*******************************************************************************
*
* f28RegionBlockErase - erase block of Intel28-style flash region
*
* This routine erases one block of Intel28-style flash specified
* by <offset>.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if erase failed.
*/

LOCAL STATUS f28RegionBlockErase
    (
    F2X_RID id,
    int     offset
    )
    {
    STATUS      status = OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    ULONG       tickSave;
    ULONG       timeoutTicks;

    /* send the commands */

    f2xRegionWidthWrite (id, pRDesc->cmdBlockErase, 0, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdConfirm, offset, FALSE);

    /* wait for erase to complete */

    tickSave = tickGet();
    timeoutTicks = sysClkRateGet() * F2X_TIMEOUT;
    while (f28RegionIsBusy(id))
        {
        taskDelay(1);
        if ((tickGet() - tickSave) > (ULONG)timeoutTicks)
            {
            status = ERROR;
            goto f28RegionBlockEraseExit;
            }
        }

    /* check status of the erase */

    if ((f28RegionStatusGet (id) &
         (F28_STAT_ESS | F28_STAT_ES | F28_STAT_VPPS | F28_STAT_BLS)) != 0)
        status = ERROR;

f28RegionBlockEraseExit:

    f2xRegionWidthWrite (id, pRDesc->cmdClearStatus, 0, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdReadArray, 0, FALSE);

    return (status);
    }

/*******************************************************************************
*
* f29RegionStatusGet - get status of AMD29-style flash region
*
* This routine returns the status of the AMD29-style flash region.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: F29_STATUS_BUSY if any chip is busy, or F29_STATUS_ERROR
* if no chip is busy and at least one is in error, or F29_STATUS_OK
* if no chip is busy and none are in error.
*/

LOCAL F29_STATUS f29RegionStatusGet
    (
    F2X_RID id,
    char *  pData,
    int     offset
    )
    {
    F29_STATUS  f29Status = F29_STATUS_OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    char        statusArray[F2X_MAX_REGION_WIDTH];
    char *      pChip = statusArray;
    char        xDataArray[F2X_MAX_REGION_WIDTH];
    char *      pXData = xDataArray;
    int         numChips = pRDesc->regionWidth / pRDesc->chipWidth;
    char        statusByte = 0;
    char        dataByte = 0;
    int         chipIndex;

    f2xRegionWidthRead (id, statusArray, offset, FALSE);

    /* transform the data similar to the status read above */

    switch (pRDesc->regionWidth)
        {
        case 1:
            F2X_UINT8_READ(id, pData, xDataArray, FALSE);
            break;
        case 2:
            F2X_UINT16_READ(id, pData, xDataArray, FALSE);
            break;
        case 4:
            F2X_UINT32_READ(id, pData, xDataArray, FALSE);
            break;
        case 8:
            F2X_UINT64_READ(id, pData, xDataArray, FALSE);
            break;
        default:
            break;
        }

    for (chipIndex = 0; chipIndex < numChips; chipIndex++)
        {
        switch (pRDesc->chipWidth)
            {
            case 1:
                dataByte = (char)*(UINT8 *)pXData;
                statusByte = (char)*(UINT8 *)pChip;
                break;
            case 2:
                dataByte = (char)*(UINT16 *)pXData;
                statusByte = (char)*(UINT16 *)pChip;
                break;
            case 4:
                dataByte = (char)*(UINT32 *)pXData;
                statusByte = (char)*(UINT32 *)pChip;
                break;
            default:
                break;
            }

        /* data polling algorithm */

        if ((dataByte & F29_STAT_DQ7) != (statusByte & F29_STAT_DQ7))
            {
            if ((statusByte & F29_STAT_DQ5) == 0)
                {
                /* a chip is busy, return */

                f29Status = F29_STATUS_BUSY;

                return (f29Status);
                }
            else
                {
                /* authenticate the error */

                f2xRegionWidthRead (id, statusArray, offset, FALSE);

                switch (pRDesc->chipWidth)
                    {
                    case 1:
                        statusByte = (char)*(UINT8 *)pChip;
                        break;
                    case 2:
                        statusByte = (char)*(UINT16 *)pChip;
                        break;
                    case 4:
                        statusByte = (char)*(UINT32 *)pChip;
                        break;
                    default:
                        break;
                    }

                if ((dataByte & F29_STAT_DQ7) != (statusByte & F29_STAT_DQ7))
                    {
                    f29Status = F29_STATUS_ERROR;

                    /* continue and make sure no others are busy */
                    }
                }
            }

        pXData += pRDesc->chipWidth;
        pChip += pRDesc->chipWidth;
        }

    return (f29Status);
    }

/*******************************************************************************
*
* f29RegionProgram - program AMD29-style flash region
*
* This routine programs erased AMD29-style flash in multiples
* of the flash region width.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if programming failed.
*/

LOCAL STATUS f29RegionProgram
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    F29_STATUS  f29Status;
    ULONG       tickSave;
    ULONG       timeoutTicks;

    while (nbytes > 0)
        {
        /* send the commands */

        f2xRegionWidthWrite (id, pRDesc->cmdUnlock1, pRDesc->offUnlock1, FALSE);
        f2xRegionWidthWrite (id, pRDesc->cmdUnlock2, pRDesc->offUnlock2, FALSE);
        f2xRegionWidthWrite (id, pRDesc->cmdProgram, pRDesc->offProgram, FALSE);

        /* write the word */

        f2xRegionWidthWrite (id, pSrc, offset, TRUE);

        /* wait for write to complete */

        tickSave = tickGet();
        timeoutTicks = sysClkRateGet() * F2X_TIMEOUT;
        while ((f29Status = f29RegionStatusGet(id, pSrc, offset)) ==
                F29_STATUS_BUSY)
            {
            if ((tickGet() - tickSave) > (ULONG)timeoutTicks)
                {
                f2xRegionWidthWrite (id, pRDesc->cmdReset,
                                     pRDesc->offReset, FALSE);
                return (ERROR);
                }
            }

        /* check status of the write */

        if (f29Status != F29_STATUS_OK)
            {
            f2xRegionWidthWrite (id, pRDesc->cmdReset, pRDesc->offReset, FALSE);
            return (ERROR);
            }

        pSrc += pRDesc->regionWidth;
        nbytes -= pRDesc->regionWidth;
        offset += pRDesc->regionWidth;
        }

    return (OK);
    }

/*******************************************************************************
*
* f29RegionWBProgram - "write to buffer"-program AMD29-style flash region
*
* This routine uses the Write to Buffer command to program erased
* AMD29-style flash having a chip write buffer size of <chipWBSize>
* bytes, in multiples of the flash region width.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if programming failed.
*/

LOCAL STATUS f29RegionWBProgram
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset,
    int     chipWBSize
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    F29_STATUS  f29Status;
    ULONG       tickSave;
    ULONG       timeoutTicks;
    int         numChips = pRDesc->regionWidth / pRDesc->chipWidth;
    int         bytesInWriteBuffers = chipWBSize * numChips;
    char        pWriteBufferCount[F2X_MAX_REGION_WIDTH];
    int         alignedOffset;
    int         bytesToProgram;
    int         blockOffset;

    while (nbytes > 0)
        {
        /* Write to Buffer commands run faster when the offset is naturally
         * aligned.  We make it so here.
         */

        alignedOffset = F2X_ALIGN (offset, bytesInWriteBuffers);

        bytesToProgram = min(nbytes,
                             bytesInWriteBuffers - (offset - alignedOffset));

        blockOffset = F2X_ALIGN (offset, f2xRegionBlockSize (id));

        /* send the commands */

        f2xRegionWidthWrite (id, pRDesc->cmdUnlock1, pRDesc->offUnlock1, FALSE);
        f2xRegionWidthWrite (id, pRDesc->cmdUnlock2, pRDesc->offUnlock2, FALSE);
        f2xRegionWidthWrite (id, pRDesc->cmdWriteBuffer, blockOffset, FALSE);

        /* create the chip byte/word count (bus) word */

        f2xRegionCmdCreate (id, pWriteBufferCount,
                            bytesToProgram/numChips/pRDesc->chipWidth - 1);

        /* write the byte count */

        f2xRegionWidthWrite (id, pWriteBufferCount, blockOffset, FALSE);

        /* write the data */

        nbytes -= bytesToProgram;
        while (bytesToProgram > 0)
            {
            f2xRegionWidthWrite (id, pSrc, offset, TRUE);

            pSrc += pRDesc->regionWidth;
            bytesToProgram -= pRDesc->regionWidth;
            offset += pRDesc->regionWidth;
            }

        /* write the "program buffer" command */

        f2xRegionWidthWrite (id, pRDesc->cmdProgramBuffer, blockOffset, FALSE);

        /* wait for write to complete */

        tickSave = tickGet();
        timeoutTicks = sysClkRateGet() * F2X_TIMEOUT;
        while ((f29Status = f29RegionStatusGet(id, pSrc - pRDesc->regionWidth,
                                               offset - pRDesc->regionWidth)) ==
                F29_STATUS_BUSY)
            {
            if ((tickGet() - tickSave) > (ULONG)timeoutTicks)
                {
                f2xRegionWidthWrite (id, pRDesc->cmdReset,
                                     pRDesc->offReset, FALSE);
                return (ERROR);
                }
            }

        /* check status of the write */

        if (f29Status != F29_STATUS_OK)
            {
            f2xRegionWidthWrite (id, pRDesc->cmdReset, pRDesc->offReset, FALSE);
            return (ERROR);
            }

        }

    return (OK);
    }

/*******************************************************************************
*
* f29RegionWB32Program - "write to buffer"-program AMD29-style flash region
*
* This routine uses the Write to Buffer command to program erased
* AMD29-style flash having a chip write buffer size of 32 bytes,
* in multiples of the flash region width.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if programming failed.
*/

LOCAL STATUS f29RegionWB32Program
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset
    )
    {
    STATUS status;

    status = f29RegionWBProgram (id, pSrc, nbytes, offset, 32);

    return (status);
    }

/*******************************************************************************
*
* f29RegionBlockErase - erase block of AMD29-style flash region
*
* This routine erases one block of Intel28-style flash specified
* by <offset>.
*
* WARNING: For the sake of speed, this routine leaves alignment
* checking, range checking, mutual exclusion protection and page
* selection (if any) to higher-order routines.
*
* RETURNS: OK, or ERROR if erase failed.
*/

LOCAL STATUS f29RegionBlockErase
    (
    F2X_RID id,
    int     offset
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    char *      pSrc = f2xErasedArray;
    F29_STATUS  f29Status;
    ULONG       tickSave;
    ULONG       timeoutTicks;

    /* send the commands */

    f2xRegionWidthWrite (id, pRDesc->cmdUnlock1, pRDesc->offUnlock1, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdUnlock2, pRDesc->offUnlock2, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdBlockErase,
                         pRDesc->offBlockErase, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdUnlock1, pRDesc->offUnlock1, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdUnlock2, pRDesc->offUnlock2, FALSE);
    f2xRegionWidthWrite (id, pRDesc->cmdConfirm, offset, FALSE);

    /* wait for erase to complete */

    tickSave = tickGet();
    timeoutTicks = sysClkRateGet() * F2X_TIMEOUT;
    while ((f29Status = f29RegionStatusGet(id, pSrc, offset)) ==
           F29_STATUS_BUSY)
        {
        taskDelay(1);
        if ((tickGet() - tickSave) > (ULONG)timeoutTicks)
            {
            f2xRegionWidthWrite (id, pRDesc->cmdReset, pRDesc->offReset, FALSE);
            return (ERROR);
            }
        }

    /* check status of the erase */

    if (f29Status != F29_STATUS_OK)
        {
        f2xRegionWidthWrite (id, pRDesc->cmdReset, pRDesc->offReset, FALSE);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* f2xRegionProgram - program flash region
*
* This routine programs erased flash at any offset and any number
* of bytes within the region.
*
* WARNING: For the sake of speed, this routine leaves range
* checking to higher-order routines.
*
* RETURNS: OK, or ERROR if programming failed.
*/

LOCAL STATUS f2xRegionProgram
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset
    )
    {
    STATUS      status = OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    int         alignedOffset = F2X_ALIGN (offset, pRDesc->regionWidth);
    int         bytesToProgram;
    int         bytesToCopyHead;
    int         bytesToCopyTail;
    char        pWord[F2X_MAX_REGION_WIDTH];

    semTake (pRDesc->semId, WAIT_FOREVER);

    if (pRDesc->pageFunc != (VOIDFUNCPTR)NULL)
        {
        /* turn the page */

        (pRDesc->pageFunc)(pRDesc->pageId);
        }

    /* program partial word start */

    bytesToCopyHead = offset - alignedOffset;
    if (bytesToCopyHead != 0)
        {
        bytesToProgram = min(nbytes, pRDesc->regionWidth - bytesToCopyHead);
        bytesToCopyTail = pRDesc->regionWidth - bytesToCopyHead -
                          bytesToProgram;

#ifdef INCLUDE_F2X_FORCE_WIDTH_READ
        f2xRegionWidthRead (id, pWord, alignedOffset, TRUE);
        bcopy (pSrc, pWord + bytesToCopyHead, bytesToProgram);
#else /* INCLUDE_F2X_FORCE_WIDTH_READ */
        /* This code is being kept for legacy purposes.  In time it will be
         * removed and the code above will be used exclusively - it is simpler.
         */
        bcopy (pRDesc->baseAdrs + alignedOffset, pWord, bytesToCopyHead);
        bcopy (pSrc, pWord + bytesToCopyHead, bytesToProgram);
        bcopy (pRDesc->baseAdrs + alignedOffset + bytesToCopyHead +
               bytesToProgram, pWord + bytesToCopyHead + bytesToProgram,
               bytesToCopyTail);
#endif /* INCLUDE_F2X_FORCE_WIDTH_READ */

        status = (pRDesc->programRtn)(id, pWord, pRDesc->regionWidth,
                                      alignedOffset);

        if (status != OK)
            {
            status = ERROR;
            goto f2xRegionProgramExit;
            }

        pSrc += bytesToProgram;
        nbytes -= bytesToProgram;
        offset += bytesToProgram;
        }

    if (nbytes == 0)
        goto f2xRegionProgramExit;

    /* program whole word middle */

    bytesToProgram = min(nbytes, F2X_ALIGN(offset + nbytes,
                                           pRDesc->regionWidth) - offset);

    if (bytesToProgram != 0)
        {
        status = (pRDesc->programRtn)(id, pSrc, bytesToProgram, offset);

        if (status != OK)
            {
            status = ERROR;
            goto f2xRegionProgramExit;
            }

        pSrc += bytesToProgram;
        nbytes -= bytesToProgram;
        offset += bytesToProgram;
        }

    /* program partial word end */

    bytesToProgram = nbytes;
    if (bytesToProgram != 0)
        {
        bytesToCopyTail = pRDesc->regionWidth - bytesToProgram;

#ifdef INCLUDE_F2X_FORCE_WIDTH_READ
        f2xRegionWidthRead (id, pWord, offset, TRUE);
        bcopy (pSrc, pWord, bytesToProgram);
#else /* INCLUDE_F2X_FORCE_WIDTH_READ */
        /* This code is being kept for legacy purposes.  In time it will be
         * removed and the code above will be used exclusively - it is simpler.
         */
        bcopy (pSrc, pWord, bytesToProgram);
        bcopy (pRDesc->baseAdrs + offset + bytesToProgram,
               pWord + bytesToProgram, bytesToCopyTail);
#endif /* INCLUDE_F2X_FORCE_WIDTH_READ */

        status = (pRDesc->programRtn)(id, pWord, pRDesc->regionWidth, offset);

        if (status != OK)
            {
            status = ERROR;
            goto f2xRegionProgramExit;
            }
        }

f2xRegionProgramExit:
    semGive (pRDesc->semId);

    return (status);
    }

/*******************************************************************************
*
* f2xRegionBlockErase - erase block of flash region
*
* This routine erases one block of a flash region.
*
* WARNING: For the sake of speed, this routine leaves range
* checking to higher-order routines.
*
* RETURNS: OK, or ERROR if erase failed.
*/

LOCAL STATUS f2xRegionBlockErase
    (
    F2X_RID id,
    int     offset
    )
    {
    STATUS      status = OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;

    offset = F2X_ALIGN (offset, f2xRegionBlockSize (id));

    semTake (pRDesc->semId, WAIT_FOREVER);

    if (pRDesc->pageFunc != (VOIDFUNCPTR)NULL)
        {
        /* turn the page */

        (pRDesc->pageFunc)(pRDesc->pageId);
        }

    status = (pRDesc->blockEraseRtn)(id, offset);

    semGive (pRDesc->semId);

    return (status);
    }

/*******************************************************************************
*
* f2xRegionGet - get the contents of flash region
*
* This routine copies the contents of a flash region into a
* specified buffer.
*
* WARNING: For the sake of speed, this routine leaves range
* checking to higher-order routines.
*
* RETURNS: OK, or ERROR parameters are invalid.
*/

LOCAL STATUS f2xRegionGet
    (
    F2X_RID id,
    char *  pDst,
    int     nbytes,
    int     offset
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
#ifdef INCLUDE_F2X_FORCE_WIDTH_READ
    int         alignedOffset = F2X_ALIGN (offset, pRDesc->regionWidth);
    int         bytesToRead;
    int         bytesToSkipHead;
    int         bytesToSkipTail;
    char        pWord[F2X_MAX_REGION_WIDTH];
#else /* INCLUDE_F2X_FORCE_WIDTH_READ */
    char *      pSrc = pRDesc->baseAdrs + offset;
#endif /* INCLUDE_F2X_FORCE_WIDTH_READ */

    semTake (pRDesc->semId, WAIT_FOREVER);

    if (pRDesc->pageFunc != (VOIDFUNCPTR)NULL)
        {
        /* turn the page */

        (pRDesc->pageFunc)(pRDesc->pageId);
        }

#ifdef INCLUDE_F2X_FORCE_WIDTH_READ
    /* read partial word start */

    bytesToSkipHead = offset - alignedOffset;
    if (bytesToSkipHead != 0)
        {
        bytesToRead = min(nbytes, pRDesc->regionWidth - bytesToSkipHead);

        f2xRegionWidthRead (id, pWord, alignedOffset, TRUE);
        bcopy (pWord + bytesToSkipHead, pDst, bytesToRead);

        pDst += bytesToRead;
        nbytes -= bytesToRead;
        offset += bytesToRead;
        }

    if (nbytes == 0)
        goto f2xRegionGetExit;

    /* read whole word middle */

    while (nbytes >= pRDesc->regionWidth)
        {
        f2xRegionWidthRead (id, pDst, offset, TRUE);

        pDst += pRDesc->regionWidth;
        nbytes -= pRDesc->regionWidth;
        offset += pRDesc->regionWidth;
        }

    /* read partial word end */

    bytesToRead = nbytes;
    if (bytesToRead != 0)
        {
        bytesToSkipTail = pRDesc->regionWidth - bytesToRead;

        f2xRegionWidthRead (id, pWord, offset, TRUE);
        bcopy (pWord, pDst, bytesToRead);
        }

f2xRegionGetExit:

#else /* INCLUDE_F2X_FORCE_WIDTH_READ */

    bcopy (pSrc, pDst, nbytes);

#endif /* INCLUDE_F2X_FORCE_WIDTH_READ */

    semGive (pRDesc->semId);

    return (OK);
    }

/*******************************************************************************
*
* f2xRegionSet - write to flash region
*
* This routine copies a specified buffer into a flash region.
*
* WARNING: For the sake of speed, this routine leaves range
* checking to higher-order routines.
*
* RETURNS: OK, or ERROR parameters are invalid or set failed.
*/

LOCAL STATUS f2xRegionSet
    (
    F2X_RID id,
    char *  pSrc,
    int     nbytes,
    int     offset
    )
    {
    STATUS      status = OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    char *      pBlock;
    char *      pBuf;
    int         blockOffset;
    int         bytesToSet;
    int         bytesToCheck;
    BOOL        needsErase;

    semTake (pRDesc->semId, WAIT_FOREVER);

    if (pRDesc->pageFunc != (VOIDFUNCPTR)NULL)
        {
        /* turn the page */

        (pRDesc->pageFunc)(pRDesc->pageId);
        }

    /* get a work buffer */

    pBlock = (char *)malloc (f2xRegionBlockSize (id));

    if (pBlock == (char *)NULL)
        {
        status = ERROR;
        goto f2xRegionSetExit;
        }

    while (nbytes > 0)
        {
        blockOffset = offset - F2X_ALIGN (offset, f2xRegionBlockSize (id));

        /* read flash block into buffer */

        f2xRegionGet (id, pBlock, f2xRegionBlockSize (id),
                      F2X_ALIGN (offset, f2xRegionBlockSize (id)));

        bytesToSet = min(nbytes, f2xRegionBlockSize (id) - blockOffset);
        bytesToCheck = bytesToSet;

        /* see if block needs to be erased */

        needsErase = FALSE;
        pBuf = pBlock + blockOffset;
        while (bytesToCheck > 0)
           {
           if (*pBuf != (char)0xff)
               {
               needsErase = TRUE;
               break;
               }

           pBuf++;
           bytesToCheck--;
           }

        /* copy source over the block image */

        bcopy (pSrc, pBlock + blockOffset, bytesToSet);

        if (needsErase)
            {
            /* erase and write the modified block */

            status = f2xRegionBlockErase (id, offset);

            if (status != OK)
                {
                status = ERROR;
                goto f2xRegionSetExit;
                }

            status = f2xRegionProgram (id, pBlock, f2xRegionBlockSize (id),
                                       F2X_ALIGN (offset,
                                                      f2xRegionBlockSize (id)));
            }
        else
            {
            /* write only the necessary parts */

            status = f2xRegionProgram (id, pBlock + blockOffset, bytesToSet,
                                       offset);
            }

        if (status != OK)
            {
            status = ERROR;
            goto f2xRegionSetExit;
            }

        pSrc += bytesToSet;
        nbytes -= bytesToSet;
        offset += bytesToSet;
        }

f2xRegionSetExit:
    semGive (pRDesc->semId);
    free (pBlock);

    return (status);
    }

/*******************************************************************************
*
* f2xRegionErase - erase flash region
*
* This routine erases a flash region.
*
* RETURNS: OK, or ERROR if erase failed.
*/

LOCAL STATUS f2xRegionErase
    (
    F2X_RID id
    )
    {
    STATUS      status = OK;
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;
    int         offset;

    semTake (pRDesc->semId, WAIT_FOREVER);

    if (pRDesc->pageFunc != (VOIDFUNCPTR)NULL)
        {
        /* turn the page */

        (pRDesc->pageFunc)(pRDesc->pageId);
        }

    for (offset = 0; offset < pRDesc->regionSize;
         offset += f2xRegionBlockSize (id))
        {
        status = f2xRegionBlockErase (id, offset);

        if (status != OK)
            {
            status = ERROR;
            goto f2xRegionBlockEraseExit;
            }
        }

f2xRegionBlockEraseExit:
    semGive (pRDesc->semId);

    return (status);
    }

/*******************************************************************************
*
* f2xRegionSize - get size of flash region
*
* This routine returns the size of a flash region in bytes.
*
* RETURNS: Size of flash region in bytes.
*/

LOCAL int f2xRegionSize
    (
    F2X_RID id
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)id;

    return (pRDesc->regionSize);
    }

/*******************************************************************************
*
* f2xRegionIsPowerOf2 - checks if argument is power of 2
*
* This routine checks that the argument is a power of 2.
*
* RETURNS: TRUE if <arg> is a power of 2, else FALSE.
*/

LOCAL BOOL f2xRegionIsPowerOf2
    (
    int arg
    )
    {
    if (arg <= 0)
        return (FALSE);

    while ((arg & 1) == 0)
        arg >>= 1;

    return ((arg & ~1) == 0);
    }

/*******************************************************************************
*
* f2xRegionCreate - create flash region
*
* This routine records the parameters of a flash region and
* puts the region in a ready state.
*
* ARGUMENT DETAILS:
* <baseAdrs>:
* Base address of the flash region.
*
* <regionWidth>:
* Data bus width of the flash region in bytes.  This is the
* equivalent to the number of interleaved chips multiplied by the
* byte width of each (i.e., <chipWidth>, see below).
*
* <regionSize>:
* Entire size of the flash region in bytes.  The address
* <baseAdrs> + <regionSize> - 1 should be the last byte of
* the region.  To properly reset a region, a region should
* not span inter-chip or intra-chip banks.
*
* <chipWidth>:
* The width of a single flash chip in bytes.  This is usually
* 1 or 2.
*
* <chipBlockSize>:
* The erasable unit in bytes of a flash chip for this
* region.  Using AMD terminology, this is the "sector size."
* Using Intel terminology, this  is the "block size."
*
* <writeWidth>:
* The width in bytes the CPU should use to write to the flash
* region.  In almost all cases, this is the same as <regionWidth>.
* In rare cases (e.g., writes that traverse a Motorola Falcon chip
* set), this is half, a quarter or an eighth of <regionWidth>.
*
* <type>:
* The type of the flash chips.  This should be F28_TYPE for
* Intel28-style flash; F29_8BIT_TYPE for 8-bit only AMD29-style flash;
* F29_16BIT_TYPE for 8/16-bit and 16-bit only AMD29-style flash; and
* F29_32BIT_TYPE for 8/16/32-bit, 16/32-bit and 32-bit only AMD29-style
* flash.  If the flash supports a "write to buffer" command with
* a 32-byte write buffer (as do Intel StrataFlash and AMD MirrorBit
* flash), write performance may be enhanced by or-ing the type values
* above with F2X_WB32_TYPE.
*
* <dependRid>:
* The region ID (F2X_RID) of a dependent flash region.  In most
* cases this should be the region ID of any previously defined
* region that shares the same flash chip (or chips) or shares the
* same page (see next argument).  If the region has no dependents
* or this is the first call to f2xRegionCreate() for a dependent
* region, <dependRid> should be NULL.  This information is used
* to share mutual exclusion mechanisms (semaphores) between
* dependent regions and is particularly important when two dependent
* regions belong to different flash groups.
*
* <pageFunc>:
* A user-supplied function used when paging that places the region
* at <baseAdrs>. For most hardware configurations, i.e., those where
* the entire flash region is entirely memory mapped, <pageFunc> is
* NULL.
*
* <pageId>:
* An argument for <pageFunc> (above).  This is additional
* information that may be used to identify one page from another.
* If <pageFunc> NULL, <pageId> should be NONE.
*
* RETURNS: An ID for the region, or NULL if parameters are invalid
* or F2X_MAX_REGION regions already exist.
*/

F2X_RID f2xRegionCreate
    (
    char *      baseAdrs,       /* base address of flash region */
    int         regionWidth,    /* width of flash region in bytes */
    int         regionSize,     /* entire size of flash region in bytes */
    int         chipWidth,      /* width of single flash part in bytes */
    int         chipBlockSize,  /* block size of a single flash chip in bytes */
    int         writeWidth,     /* width of a flash region write in bytes */
    int         type,           /* f2x_xxx_TYPE - see f2xFlashMem.h */
    F2X_RID     dependRid,      /* dependent region ID (or NULL) */
    VOIDFUNCPTR pageFunc,       /* user-supplied page function (or NULL) */
    int         pageId          /* argument for page function (or NONE) */
    )
    {
    F2X_RDESC * pRDesc;
    int         typeWithMask = type & F2X_TYPE_MASK;
    int         f2xBlockSize;
    int         shift;

    /* interrogate user input */

    if ((f2xRegionIsPowerOf2 (regionWidth) &&
         f2xRegionIsPowerOf2 (chipWidth) &&
         f2xRegionIsPowerOf2 (writeWidth)) == FALSE)
        return ((F2X_RID)NULL);

    if (((regionWidth == 1) || (regionWidth == 2) ||
         (regionWidth == 4) || (regionWidth == 8)) == FALSE)
        return ((F2X_RID)NULL);

    if (((chipWidth == 1) || (chipWidth == 2) ||
         (chipWidth == 4)) == FALSE)
        return ((F2X_RID)NULL);

    if ((regionWidth % chipWidth) != 0)
        return ((F2X_RID)NULL);

    if ((regionWidth % writeWidth) != 0)
        return ((F2X_RID)NULL);

    if (writeWidth < chipWidth)
        return ((F2X_RID)NULL);

    f2xBlockSize = chipBlockSize * (regionWidth / chipWidth);

    if ((regionSize % f2xBlockSize) != 0)
        return ((F2X_RID)NULL);

    if ((typeWithMask != F28_TYPE) && (typeWithMask != F28_WB32_TYPE) &&
        (typeWithMask != F29_8BIT_TYPE) && (typeWithMask != F29_16BIT_TYPE) &&
        (typeWithMask != F29_32BIT_TYPE))
        return ((F2X_RID)NULL);

    if (((type & ~F2X_TYPE_MASK) != 0) &&
        ((type & ~F2X_TYPE_MASK) != F2X_WB32_TYPE))
        return ((F2X_RID)NULL);

    switch (typeWithMask)
        {
        case F29_32BIT_TYPE:
            if (chipWidth == 4) break;
        case F29_16BIT_TYPE:
            if (chipWidth == 2) break;
        case F29_8BIT_TYPE:
            if (chipWidth == 1) break;
            return ((F2X_RID)NULL);
            break;
        default:
            break;
        }

    /* initialize "erased" array (this might happen more that once) */

    bfill (f2xErasedArray, sizeof (f2xErasedArray), 0xff);

    /* make sure we have enough descriptors */

    if (f2xRDescArrayCount >= NELEMENTS(f2xRDescArray))
        return ((F2X_RID)NULL);

    /* get next descriptor */

    pRDesc = &f2xRDescArray [f2xRDescArrayCount];

    /* increment array element count */

    f2xRDescArrayCount++;

    /* save user input to descriptor */

    pRDesc->baseAdrs = baseAdrs;
    pRDesc->regionWidth = regionWidth;
    pRDesc->regionSize = regionSize;
    pRDesc->chipWidth = chipWidth;
    pRDesc->chipBlockSize = chipBlockSize;
    pRDesc->writeWidth = writeWidth;
    pRDesc->type = type;
    pRDesc->pageFunc = pageFunc;
    pRDesc->pageId = pageId;

    /* form commands in descriptor */

    switch (typeWithMask)
        {
        case F28_TYPE:
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdClearStatus,
                               F28_CMD_CLEAR_STATUS);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdReadArray,
                               F28_CMD_READ_ARRAY);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdBlockErase,
                               F28_CMD_BLOCK_ERASE);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdConfirm,
                               F28_CMD_CONFIRM);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdProgram,
                               F28_CMD_PROGRAM);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdWriteBuffer,
                               F28_CMD_WRITE_BUFFER);
            break;
        case F29_8BIT_TYPE:
        case F29_16BIT_TYPE:
        case F29_32BIT_TYPE:
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdReset,
                               F29_CMD_RESET);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdUnlock1,
                               F29_CMD_UNLOCK1);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdUnlock2,
                               F29_CMD_UNLOCK2);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdProgramBuffer,
                               F29_CMD_PROGRAM_BUFFER);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdBlockErase,
                               F29_CMD_BLOCK_ERASE);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdConfirm,
                               F29_CMD_ERASE_CONFIRM);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdProgram,
                               F29_CMD_PROGRAM);
            f2xRegionCmdCreate ((F2X_RID)pRDesc, pRDesc->cmdWriteBuffer,
                               F29_CMD_WRITE_BUFFER);

            switch (typeWithMask)
                {
                case F29_8BIT_TYPE:
                    shift = 1 >> chipWidth;  /* i.e., 0 */
                    break;
                case F29_16BIT_TYPE:
                    shift = 2 >> chipWidth;
                    break;
                case F29_32BIT_TYPE:
                default:
                    shift = 4 >> chipWidth;
                    break;
                }

            pRDesc->offReset = (F29_OFF_RESET << shift) * regionWidth;
            pRDesc->offUnlock1 = (F29_OFF_UNLOCK1 << shift) * regionWidth;
            pRDesc->offUnlock2 = (F29_OFF_UNLOCK2 << shift) * regionWidth;
            pRDesc->offBlockErase = (F29_OFF_BLOCK_ERASE << shift) *
                                    regionWidth;
            pRDesc->offProgram = (F29_OFF_PROGRAM << shift) * regionWidth;

            break;
        default:
            break;
        }

    /* record the programming and block erase methods */

    switch (pRDesc->type)
        {
        case F28_TYPE:
            pRDesc->programRtn = f28RegionProgram;
            pRDesc->blockEraseRtn = f28RegionBlockErase;
            break;
        case (F28_TYPE | F2X_WB32_TYPE):
            pRDesc->programRtn = f28RegionWB32Program;
            pRDesc->blockEraseRtn = f28RegionBlockErase;
            break;
        case F29_8BIT_TYPE:
        case F29_16BIT_TYPE:
        case F29_32BIT_TYPE:
            pRDesc->programRtn = f29RegionProgram;
            pRDesc->blockEraseRtn = f29RegionBlockErase;
            break;
        case (F29_8BIT_TYPE | F2X_WB32_TYPE):
        case (F29_16BIT_TYPE | F2X_WB32_TYPE):
        case (F29_32BIT_TYPE | F2X_WB32_TYPE):
            pRDesc->programRtn = f29RegionWB32Program;
            pRDesc->blockEraseRtn = f29RegionBlockErase;
            break;
        default:
            break;
        }

    /* initialize or copy semaphores */

    if (dependRid == (F2X_RID)NULL)
        {
        /* initialize region-level semaphore */

        semMInit (&pRDesc->semaphore, SEM_DELETE_SAFE);

        /* point to it */

        pRDesc->semId = (SEM_ID)&pRDesc->semaphore;
        }
    else
        {
        /* share semaphore with dependent region */

        pRDesc->semId = ((F2X_RDESC *)dependRid)->semId;
        }

    /* initialize state of region */

    switch (typeWithMask)
        {
        case F28_TYPE:
            f2xRegionWidthWrite ((F2X_RID)pRDesc, pRDesc->cmdClearStatus,
                                 0, FALSE);
            f2xRegionWidthWrite ((F2X_RID)pRDesc, pRDesc->cmdReadArray,
                                 0, FALSE);
            break;
        case F29_8BIT_TYPE:
        case F29_16BIT_TYPE:
        case F29_32BIT_TYPE:
            f2xRegionWidthWrite ((F2X_RID)pRDesc, pRDesc->cmdReset,
                                 pRDesc->offReset, FALSE);
            break;
        default:
            break;
        }

    return ((F2X_RID)pRDesc);
    }

/* group-level routines ----------------------------------------------------- */

/*******************************************************************************
*
* f2xGroupRegionInfoGet - get region index and region offset
*
* Given the number of bytes and offset into a flash group, this
* routine returns the region index in <*pRegionIndex> and region
* offset in <*pRegionOffset>.
*
* RETURNS: OK, or ERROR if <nbytes> and <offset> are out of range.
*/

LOCAL STATUS f2xGroupRegionInfoGet
    (
    F2X_GID id,
    int     nbytes,
    int     offset,
    int *   pRegionIndex,
    int *   pRegionOffset
    )
    {
    F2X_GDESC * pGDesc = (F2X_GDESC *)id;
    F2X_RDESC * pRDesc;
    int         regionIndex;

    /* check bounds */

    if ((nbytes < 0) || (offset < 0))
        return (ERROR);

    if ((nbytes + offset) > pGDesc->groupSize)
        return (ERROR);

    /* find the starting region */

    for (regionIndex = 0; regionIndex < pGDesc->ridArrayCount; regionIndex++)
        {
        pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];

        if (offset < f2xRegionSize (pRDesc))
            {
            /* we have our starting region and offset */
            break;
            }

        offset -= f2xRegionSize ((F2X_RID)pRDesc);
        }

    *pRegionIndex = regionIndex;
    *pRegionOffset = offset;

    return (OK);
    }

/*******************************************************************************
*
* f2xGroupAct - perform action on flash group
*
* This routine calls the routine specified by <routine> to
* program, set or get flash at any offset and any number
* of bytes within the group.
*
* RETURNS: OK, or ERROR if copy failed.
*/

LOCAL STATUS f2xGroupAct
    (
    F2X_GID id,        /* group ID */
    char *  pBuf,      /* source or destination buffer */
    int     nbytes,    /* number of bytes to copy */
    int     offset,    /* byte offset into flash group */
    FUNCPTR routine    /* ptr to f2xRegionProgram/Set/Get() */
    )
    {
    F2X_GDESC * pGDesc = (F2X_GDESC *)id;
    F2X_RDESC * pRDesc;
    STATUS      status;
    int         regionIndex;
    int         regionOffset;
    int         bytesToAct;

    status = f2xGroupRegionInfoGet (id, nbytes, offset, &regionIndex,
                                    &regionOffset);

    if (status != OK)
        return (ERROR);

    semTake (pGDesc->semId, WAIT_FOREVER);

    while (nbytes > 0)
        {
        pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];

        bytesToAct = min(nbytes, f2xRegionSize (pRDesc) - regionOffset);

        status = (routine)((F2X_RID)pRDesc, pBuf, bytesToAct, regionOffset);

        if (status != OK)
            {
            status = ERROR;
            goto f2xGroupActExit;
            }

        regionIndex += 1;
        pBuf += bytesToAct;
        nbytes -= bytesToAct;
        regionOffset = 0;
        }

f2xGroupActExit:
    semGive (pGDesc->semId);

    return (status);
    }

/*******************************************************************************
*
* f2xGroupMap - TODO
*
* RETURNS: Memory mapped address.
*/

char * f2xGroupMap
    (
    F2X_GID id,       /* group ID */
    int     offset    /* byte offset into group */
    )
    {
    F2X_GDESC * pGDesc = (F2X_GDESC *)id;
    F2X_RDESC * pRDesc;
    STATUS      status;
    int         regionIndex;
    int         regionOffset;
    char        ch;

    status = f2xGroupRegionInfoGet (id, 1, offset, &regionIndex, &regionOffset);

    if (status != OK)
        return ((char *)NULL);

    pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];

    /* turn the page if necessary */

    f2xRegionGet ((F2X_RID)pRDesc, &ch, 0, regionOffset);

    return (pRDesc->baseAdrs + regionOffset);
    }

/*******************************************************************************
*
* f2xGroupProgram - program flash group
*
* This routine programs erased flash at any offset and any number
* of bytes within the group.
*
* RETURNS: OK, or ERROR if programming failed.
*/

STATUS f2xGroupProgram
    (
    F2X_GID id,        /* group ID */
    char *  pSrc,      /* source buffer */
    int     nbytes,    /* number of bytes to program */
    int     offset     /* byte offset into flash group */
    )
    {
    STATUS      status;

    status = f2xGroupAct (id, pSrc, nbytes, offset, f2xRegionProgram);

    if (status != OK)
        status = ERROR;

    return (status);
    }

/*******************************************************************************
*
* f2xGroupBlockSize - get flash group block size
*
* This routine returns the size of a flash group block in bytes.
*
* The flash group block size is the largest region block size in
* the group if and only if its boundaries fall on region block
* boundaries, and the group size is an integer multiple of it,
* else the flash group block size is considered "dependent on
* region" and is 0.
*
* RETURNS: Size of flash group block in bytes, or 0 if flash
* group block size depends on flash region.
*/

int f2xGroupBlockSize
    (
    F2X_GID id    /* group ID */
    )
    {
    F2X_GDESC * pGDesc = (F2X_GDESC *)id;

    return (pGDesc->pseudoBlockSize);
    }

/*******************************************************************************
*
* f2xGroupBlockErase - erase a block of flash group
*
* This routine erases one block of the flash group.  If
* f2xGroupBlockSize() is zero for the group, the size of the block
* erased depends on the region specified by <offset>.
*
* RETURNS: OK, or ERROR if erase failed.
*/

STATUS f2xGroupBlockErase
    (
    F2X_GID id,       /* group ID */
    int     offset    /* byte offset into group */
    )
    {
    F2X_GDESC * pGDesc = (F2X_GDESC *)id;
    F2X_RDESC * pRDesc;
    STATUS      status;
    int         regionIndex;
    int         regionOffset;
    int         bytesToErase;

    status = f2xGroupRegionInfoGet (id, 1, offset, &regionIndex, &regionOffset);

    if (status != OK)
        return (ERROR);

    semTake (pGDesc->semId, WAIT_FOREVER);

    if (pGDesc->pseudoBlockSize > 0)
        {
        offset = F2X_ALIGN (offset, pGDesc->pseudoBlockSize);

        bytesToErase = pGDesc->pseudoBlockSize;

        while (bytesToErase > 0)
            {
            f2xGroupRegionInfoGet (id, 1, offset, &regionIndex, &regionOffset);

            pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];

            status = f2xRegionBlockErase ((F2X_RID)pRDesc, regionOffset);

            if (status != OK)
                {
                status = ERROR;
                break;
                }

            offset += f2xRegionBlockSize ((F2X_RID)pRDesc);
            bytesToErase -= f2xRegionBlockSize ((F2X_RID)pRDesc);
            }
        }
    else
        {
        pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];

        regionOffset = F2X_ALIGN (regionOffset,
                                   f2xRegionBlockSize ((F2X_RID)pRDesc));

        status = f2xRegionBlockErase ((F2X_RID)pRDesc, regionOffset);

        if (status != OK)
            status = ERROR;
        }

    semGive (pGDesc->semId);

    return (status);
    }

/*******************************************************************************
*
* f2xGroupSet - write to flash group
*
* This routine copies a specified buffer into a flash group.
*
* RETURNS: OK, or ERROR parameters are invalid or set failed.
*/

STATUS f2xGroupSet
    (
    F2X_GID id,        /* group ID */
    char *  pSrc,      /* source buffer */
    int     nbytes,    /* number of bytes to copy */
    int     offset     /* byte offset into group */
    )
    {
    STATUS      status;

    status = f2xGroupAct (id, pSrc, nbytes, offset, f2xRegionSet);

    if (status != OK)
        status = ERROR;

    return (status);
    }

/*******************************************************************************
*
* f2xGroupGet - get the contents of flash group
*
* This routine copies the contents of a flash group into a
* specified buffer.
*
* RETURNS: OK, or ERROR parameters are invalid.
*/

STATUS f2xGroupGet
    (
    F2X_GID id,        /* group ID */
    char *  pDst,      /* destination buffer */
    int     nbytes,    /* number of bytes to copy */
    int     offset     /* byte offset into group */
    )
    {
    STATUS      status;

    status = f2xGroupAct (id, pDst, nbytes, offset, f2xRegionGet);

    if (status != OK)
        status = ERROR;

    return (status);
    }

/*******************************************************************************
*
* f2xGroupErase - erase flash group
*
* This routine erases a flash group.
*
* RETURNS: OK, or ERROR if erase failed.
*/

STATUS f2xGroupErase
    (
    F2X_GID id    /* group ID */
    )
    {
    STATUS      status = OK;
    F2X_GDESC * pGDesc = (F2X_GDESC *)id;
    F2X_RDESC * pRDesc;
    int         regionIndex;

    semTake (pGDesc->semId, WAIT_FOREVER);

    for (regionIndex = 0; regionIndex < pGDesc->ridArrayCount; regionIndex++)
        {
        pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];

        status = f2xRegionErase ((F2X_RID)pRDesc);

        if (status != OK)
            {
            status = ERROR;
            goto f2xGroupEraseExit;
            }
        }

f2xGroupEraseExit:
    semGive (pGDesc->semId);

    return (status);
    }

/*******************************************************************************
*
* f2xGroupSize - get the size of flash group
*
* This routine returns the size of the flash group in bytes.
*
* RETURNS: Size of flash in bytes.
*/

int f2xGroupSize
    (
    F2X_GID id    /* group ID */
    )
    {
    F2X_GDESC * pGDesc = (F2X_GDESC *)id;

    return (pGDesc->groupSize);
    }

/*******************************************************************************
*
* f2xGroupCreate - create a flash group
*
* This routine creates a flash group from the flash region
* specified by <rid>.
*
* RETURNS: An ID for the group, or NULL if F2X_MAX_GROUP groups
* already exist.
*/

F2X_GID f2xGroupCreate
    (
    F2X_RID rid    /* region ID */
    )
    {
    F2X_RDESC * pRDesc = (F2X_RDESC *)rid;
    F2X_GDESC * pGDesc;

    /* make sure we have enough descriptors */

    if (f2xGDescArrayCount >= NELEMENTS(f2xGDescArray))
        return ((F2X_GID)NULL);

    /* get next descriptor */

    pGDesc = &f2xGDescArray [f2xGDescArrayCount];

    /* increment array element count */

    f2xGDescArrayCount++;

    /* initialize group descriptor */

    pGDesc->ridArray[0] = rid;
    pGDesc->ridArrayCount = 1;
    pGDesc->groupSize = f2xRegionSize ((F2X_RID)pRDesc);
    pGDesc->pseudoBlockSize = f2xRegionBlockSize ((F2X_RID)pRDesc);

    /* initialize group-level semaphore */

    semMInit (&pGDesc->semaphore, SEM_DELETE_SAFE);

    /* point to it */

    pGDesc->semId = (SEM_ID)&pGDesc->semaphore;

    return (pGDesc);
    }

/*******************************************************************************
*
* f2xGroupAdd - add a region to a flash group
*
* This routine adds a flash region logically after all of the
* existing regions in a flash group.
*
* RETURNS: OK, or ERROR if F2X_MAX_REGIONS regions already exist
* in the group.
*/

STATUS f2xGroupAdd
    (
    F2X_GID gid,    /* existing group ID */
    F2X_RID rid     /* region ID to add */
    )
    {
    F2X_GDESC * pGDesc = (F2X_GDESC *)gid;
    F2X_RDESC * pRDesc = (F2X_RDESC *)rid;
    int         regionOffset;
    int         regionIndex;
    int         pseudoBlockSize;
    int         offset;

    /* make sure we have enough region IDs */

    if (pGDesc->ridArrayCount >= NELEMENTS(pGDesc->ridArray))
        return (ERROR);

    /* update group descriptor */

    pGDesc->ridArray[pGDesc->ridArrayCount] = rid;
    pGDesc->ridArrayCount++;
    pGDesc->groupSize += f2xRegionSize ((F2X_RID)pRDesc);

    /* Determine the flash group block size.  The flash group block size is
     * the largest region block size in the group if and only if its
     * boundaries fall on region block boundaries, and the group size is
     * an integer multiple of it, else the flash group block size is
     * considered "dependent on region" and is 0.
     */

    /* find the largest region block size - the potential group block size */

    pseudoBlockSize = 0;
    for (regionIndex = 0; regionIndex < pGDesc->ridArrayCount; regionIndex++)
        {
        pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];
        pseudoBlockSize = max(pseudoBlockSize,
                              f2xRegionBlockSize ((F2X_RID)pRDesc));
        }

    /* test that its and integer multiple of the group size */

    if ((pGDesc->groupSize % pseudoBlockSize) == 0)
        {
        /* test that pseudo block boundaries fall on region block boundaries */

        for (offset = 0; offset < pGDesc->groupSize; offset += pseudoBlockSize)
            {
            f2xGroupRegionInfoGet ((F2X_GID)pGDesc, 1, offset,
                                   &regionIndex, &regionOffset);

            pRDesc = (F2X_RDESC *)pGDesc->ridArray[regionIndex];

            if ((regionOffset % f2xRegionBlockSize ((F2X_RID)pRDesc)) != 0)
                {
                pseudoBlockSize = 0;
                break;
                }
            }
        }
    else
        pseudoBlockSize = 0;

    pGDesc->pseudoBlockSize = pseudoBlockSize;

    return (OK);
    }

/* optional sysFlashXxx() routines ------------------------------------------ */

#ifdef INCLUDE_F2X_SYS_FLASH_ROUTINES

/*******************************************************************************
*
* sysFlashSet - write to flash
*
* This routine copies a specified buffer into the sysFlashXxx()
* flash group.
*
* RETURNS: OK, or ERROR parameters are invalid or set failed.
*/

STATUS sysFlashSet
    (
    char * pSrc,      /* source buffer */
    int    nbytes,    /* number of bytes to copy */
    int    offset     /* byte offset into flash (group) */
    )
    {
    STATUS status;

    if (f2xSysFlashGid == (F2X_GID)NULL)
        return (ERROR);

    status = f2xGroupSet (f2xSysFlashGid, pSrc, nbytes, offset);

    if (status != OK)
        status = ERROR;

    return (status);
    }

/*******************************************************************************
*
* sysFlashGet - get the contents of flash
*
* This routine copies the contents of the sysFlashXxx() flash group
* into a specified buffer.
*
* RETURNS: OK, or ERROR parameters are invalid.
*/

STATUS sysFlashGet
    (
    char * pDst,      /* destination buffer */
    int    nbytes,    /* number of bytes to copy */
    int    offset     /* byte offset into flash (group) */
    )
    {
    STATUS status;

    if (f2xSysFlashGid == (F2X_GID)NULL)
        return (ERROR);

    status = f2xGroupGet (f2xSysFlashGid, pDst, nbytes, offset);

    if (status != OK)
        status = ERROR;

    return (status);
    }

/*******************************************************************************
*
* sysFlashErase - erase flash
*
* This routine erases the sysFlashXxx() flash group.
*
* RETURNS: OK, or ERROR if erase failed.
*/

STATUS sysFlashErase (void)

    {
    STATUS status;

    if (f2xSysFlashGid == (F2X_GID)NULL)
        return (ERROR);

    status = f2xGroupErase (f2xSysFlashGid);

    if (status != OK)
        status = ERROR;

    return (status);
    }

/*******************************************************************************
*
* sysFlashSize - get the size of flash
*
* This routine returns the size of the sysFlashXxx() flash group in bytes.
*
* RETURNS: Size of flash in bytes.
*/

int sysFlashSize (void)
    {
    if (f2xSysFlashGid == (F2X_GID)NULL)
        return (ERROR);

    return (f2xGroupSize (f2xSysFlashGid));
    }

/****************************************************************************** *
*
* sysFlashAdd - add a region to the sysFlashXxx() group
*
* This routine adds a flash region after all of the existing
* regions in a flash group.
*
* RETURNS: OK, or ERROR if F2X_MAX_REGIONS regions already exist
* in the sysFlashXxx() group.
*/

STATUS sysFlashAdd
    (
    F2X_RID rid    /* region ID to add */
    )
    {
    STATUS status;

    if (f2xSysFlashGid == (F2X_GID)NULL)
        {
        f2xSysFlashGid = f2xGroupCreate (rid);

        if (f2xSysFlashGid == (F2X_GID)NULL)
            return (ERROR);
        }
    else
        {
        status = f2xGroupAdd (f2xSysFlashGid, rid);

        if (status != OK)
            return (ERROR);
        }

    return (OK);
    }

#endif /* INCLUDE_F2X_SYS_FLASH_ROUTINES */

/* optional sysNvRamXxx() routines ------------------------------------------ */

#ifdef INCLUDE_F2X_SYS_NV_RAM_ROUTINES

/*******************************************************************************
*
* sysNvRamSet - write to flash
*
* This routine copies a specified buffer into the sysNvRamXxx()
* flash group.
*
* RETURNS: OK, or ERROR parameters are invalid or set failed.
*/

STATUS sysNvRamSet
    (
    char * pSrc,      /* source buffer */
    int    nbytes,    /* number of bytes to copy */
    int    offset     /* byte offset into flash (group) */
    )
    {
    STATUS status;
    char   eos = EOS;

    if (f2xSysNvRamGid == (F2X_GID)NULL)
        return (ERROR);

    if ((nbytes + offset) > NV_RAM_SIZE)
        return (ERROR);

    offset += F2X_SYS_NV_RAM_OFFSET;

    status = f2xGroupSet (f2xSysNvRamGid, pSrc, nbytes, offset);

    if (status != OK)
        return (ERROR);

    return (f2xGroupSet (f2xSysNvRamGid, &eos, 1, offset + nbytes));
    }

/*******************************************************************************
*
* sysNvRamGet - get the contents of flash
*
* This routine copies the contents of the sysNvRamXxx() flash group
* into a specified buffer.
*
* RETURNS: OK, or ERROR parameters are invalid.
*/

STATUS sysNvRamGet
    (
    char * pDst,      /* destination buffer */
    int    nbytes,    /* number of bytes to copy */
    int    offset     /* byte offset into flash (group) */
    )
    {
    STATUS status;

    if (f2xSysNvRamGid == (F2X_GID)NULL)
        return (ERROR);

    if ((nbytes + offset) > NV_RAM_SIZE)
        return (ERROR);

    offset += F2X_SYS_NV_RAM_OFFSET;

    status = f2xGroupGet (f2xSysNvRamGid, pDst, nbytes, offset);

    if (status != OK)
        return (ERROR);

    pDst[nbytes] = EOS;

    return (OK);
    }

/******************************************************************************
*
* sysNvRamAdd - add a region to the sysNvRamXxx() group
*
* This routine adds a flash region after all of the existing
* regions in a flash group.
*
* RETURNS: OK, or ERROR if F2X_MAX_REGIONS regions already exist
* in the sysNvRamXxx() group.
*/

STATUS sysNvRamAdd
    (
    F2X_RID rid    /* region ID to add */
    )
    {
    STATUS status;

    if (f2xSysNvRamGid == (F2X_GID)NULL)
        {
        f2xSysNvRamGid = f2xGroupCreate (rid);

        if (f2xSysNvRamGid == (F2X_GID)NULL)
            return (ERROR);
        }
    else
        {
        status = f2xGroupAdd (f2xSysNvRamGid, rid);

        if (status != OK)
            return (ERROR);
        }

    return (OK);
    }

#endif /* INCLUDE_F2X_SYS_NV_RAM_ROUTINES */

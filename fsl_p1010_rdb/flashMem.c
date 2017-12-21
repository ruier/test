/* flashMem.c - flash memory device driver */

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
01b,27jul11,fao  add sysFlashBuffProgram.(WIND00288227)
01a,17dec10,fao  created from arm_pbxa9 rev 01a
*/

/*
DESCRIPTION
This library contains routines to manipulate flash memory. Read and write
routines are included.
*/

#define FLASH_MEM_SIZE            0x800000
#define FLASH_SEGMENT_SIZE        0x20000
#define FLASH_CHIP_WIDTH          2
#define FLASH_UNLOCK1             (0x0555 * FLASH_CHIP_WIDTH)
#define FLASH_UNLOCK2             (0x02AA * FLASH_CHIP_WIDTH)

#define FLASH_PROGRAM1            (0x0555 * FLASH_CHIP_WIDTH)
#define FLASH_PROGRAM2            (0x02AA * FLASH_CHIP_WIDTH)

#define FLASH_ERASE1              (0x0555 * FLASH_CHIP_WIDTH)
#define FLASH_ERASE2              (0x02AA * FLASH_CHIP_WIDTH)

#define CMD_UNLOCK_DATA1          0xAA
#define CMD_UNLOCK_DATA2          0x55
#define CMD_UNLOCK_DATA3          0x90

#define CMD_PROGRAM_DATA1         0xAA
#define CMD_PROGRAM_DATA2         0x55
#define CMD_PROGRAM_DATA3         0xA0

#define CMD_BUFFER_DATA1          0xAA
#define CMD_BUFFER_DATA2          0x55
#define CMD_BUFFER_DATA3          0x25
#define CMD_BUFFER_CONFIRM        0x29

#define CMD_READ                  0xF0

#define CMD_BLOCK_ERASE1          0xAA
#define CMD_BLOCK_ERASE2          0x55
#define CMD_BLOCK_ERASE3          0x80
#define CMD_BLOCK_ERASE4          0xAA
#define CMD_BLOCK_ERASE5          0x55
#define CMD_BLOCK_ERASE6          0x30

#define CMD_UNLOCK_BYPASS1        0xAA
#define CMD_UNLOCK_BYPASS2        0x55
#define CMD_UNLOCK_BYPASS3        0x20
#define CMD_UNLOCK_BYPASS_PROGRAM 0xA0
#define CMD_UNLOCK_BYPASS_RESET1  0x90
#define CMD_UNLOCK_BYPASS_RESET2  0x00

/* status register bits */

#define WSM_ERROR         0x003a003a
#define SR_ERASE_ERROR    0x00200020
#define SR_READY          0x00800080

#define FLASH_REGISTER_READ(reg) \
    *(volatile UINT16 *)(reg)

#define FLASH_REGISTER_WRITE(reg, data)\
    *(volatile UINT16 *)(reg) = (data)

/* disable debugging */

#undef FLASH_DEBUG

#ifdef FLASH_DEBUG
#   define DEBUG_PRINT          printf
#else
#   undef  DEBUG_PRINT
#endif

/*******************************************************************************
*
* sysFlashLock - Lock block for flash
*
* This routine locks block flash.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

VOID sysFlashLock
    (
    int     eraseBlock
    )
    {    
    volatile UINT16* flashPtr;

    flashPtr = (UINT16 *)(FLASH_BASE_ADRS + FLASH_SEGMENT_SIZE * eraseBlock);
    FLASH_REGISTER_WRITE(flashPtr + FLASH_UNLOCK1, CMD_UNLOCK_DATA1);
    FLASH_REGISTER_WRITE(flashPtr + FLASH_UNLOCK2, CMD_UNLOCK_DATA2);
    FLASH_REGISTER_WRITE(flashPtr + FLASH_UNLOCK1, CMD_UNLOCK_DATA3);
    }

/*******************************************************************************
*
* sysFlashUnlock - Unlock block for flash
*
* This routine unlock block flash.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

VOID sysFlashUnlock
    (
    int     eraseBlock
    )
    {
    volatile UINT16* flashPtr;

    flashPtr = (UINT16 *)(FLASH_BASE_ADRS + FLASH_SEGMENT_SIZE * eraseBlock);
    FLASH_REGISTER_WRITE(flashPtr + FLASH_UNLOCK1, CMD_UNLOCK_DATA1 );
    FLASH_REGISTER_WRITE(flashPtr + FLASH_UNLOCK2, CMD_UNLOCK_DATA2 );
    FLASH_REGISTER_WRITE(flashPtr + FLASH_UNLOCK1, CMD_UNLOCK_DATA3 );
    }

/*******************************************************************************
*
* sysFlashProgram - write routine for flash
*
* This routine writes data into flash.
*
* RETURNS: OK or ERROR if program fail.
*
* ERRNO: N/A
*/

LOCAL STATUS sysFlashCheckStatus
    (
    volatile UINT16* flashPtr
    )
    {

    UINT32 timeout;
    volatile UINT16 oldStatus = 0;
    volatile UINT16 newStatus = 0;

    for (timeout = 0x8000000; timeout > 0; timeout-- )
        {
        oldStatus = FLASH_REGISTER_READ(flashPtr);
        newStatus = FLASH_REGISTER_READ(flashPtr);

        if ((oldStatus & 0x40) == (newStatus & 0x40))
            return OK;
        else if ((newStatus & 0x20) != 0x20)
            continue;
        else
            {
            oldStatus = FLASH_REGISTER_READ(flashPtr);
            newStatus = FLASH_REGISTER_READ(flashPtr);
            if (( oldStatus & 0x40) == (newStatus & 0x40))
                return OK;
            else 
                {
                FLASH_REGISTER_WRITE(flashPtr, 0xF0);
                return ERROR;
                }
            }
        }

    return ERROR;

    }

/*******************************************************************************
*
* sysFlashBlkErase - erase routine for flash driver
*
* This routine erases the specified sector.
*
* RETURNS: OK or ERROR if erase fail.
*
* ERRNO: N/A
*/

STATUS sysFlashBlkErase
    (
    int     eraseBlock
    )
    {    
    volatile UINT16* flashPtr;
    volatile UINT32 timeout;

    flashPtr = (volatile UINT16 *)(FLASH_BASE_ADRS + 
                                   FLASH_SEGMENT_SIZE * eraseBlock);

    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_ERASE1, CMD_BLOCK_ERASE1);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_ERASE2, CMD_BLOCK_ERASE2);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_ERASE1, CMD_BLOCK_ERASE3);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_ERASE1, CMD_BLOCK_ERASE4);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_ERASE2, CMD_BLOCK_ERASE5);

    FLASH_REGISTER_WRITE(flashPtr , CMD_BLOCK_ERASE6);

    for (timeout = 10000; timeout > 0; timeout-- )
        {
        if ((FLASH_REGISTER_READ(flashPtr) & 0x8) != 0)
            {
            break;
            }

        sysUsDelay(4);
        }

    return sysFlashCheckStatus(flashPtr);

    }

/*******************************************************************************
*
* sysFlashWordProgram - write routine for flash
*
* This routine writes a word into flash.
*
* RETURNS: OK or ERROR if program fail.
*
* ERRNO: N/A
*/

STATUS sysFlashWordProgram
    (
     volatile UINT16* flashPtr,
     volatile UINT16   data
    )
    {
        
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM1, CMD_PROGRAM_DATA1);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM2, CMD_PROGRAM_DATA2);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM1, CMD_PROGRAM_DATA3);

    FLASH_REGISTER_WRITE(flashPtr, data);

    /* verify that the word was written */

    return sysFlashCheckStatus(flashPtr);

    }

/*******************************************************************************
*
* sysFlashProgram - write routine for flash
*
* This routine writes data into flash.
*
* RETURNS: OK or ERROR if program fail.
*
* ERRNO: N/A
*/

STATUS sysFlashProgram
    (
     volatile UINT16* flashPtr,
     volatile UINT16* data,
     UINT32  len 
    )
    {
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM1, CMD_UNLOCK_BYPASS1);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM2, CMD_UNLOCK_BYPASS2);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM1, CMD_UNLOCK_BYPASS3);

    while(len>0)
        {
        FLASH_REGISTER_WRITE(FLASH_BASE_ADRS, CMD_UNLOCK_BYPASS_PROGRAM);
        FLASH_REGISTER_WRITE(flashPtr, *data);
        if (sysFlashCheckStatus(flashPtr) != OK)
            return ERROR;

        flashPtr++;
        data++;
        len -=2;
        }

    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS, CMD_UNLOCK_BYPASS_RESET1);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS, CMD_UNLOCK_BYPASS_RESET2);

    return OK;
    }

/*******************************************************************************
*
* sysFlashBuffProgram - buffer write routine for flash
*
* This routine writes data into flash by buffer.
*
* RETURNS: OK or ERROR if program fail.
*
* ERRNO: N/A
*/

STATUS sysFlashBuffProgram
    (
     volatile UINT16* flashPtr,
     volatile UINT16* data,
     UINT32  len 
    )
    {

    UINT32 startAddr = (UINT32)flashPtr;

    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM1, CMD_BUFFER_DATA1);
    FLASH_REGISTER_WRITE(FLASH_BASE_ADRS + FLASH_PROGRAM2, CMD_BUFFER_DATA2);
    FLASH_REGISTER_WRITE(startAddr, CMD_BUFFER_DATA3);
    FLASH_REGISTER_WRITE(startAddr, (len / 2 - 1));

    while(len > 0)
        {
        FLASH_REGISTER_WRITE(flashPtr, *data);
        flashPtr++;
        data++;
        len -=2;
        }

    FLASH_REGISTER_WRITE(startAddr, CMD_BUFFER_CONFIRM);

    if (sysFlashCheckStatus((volatile UINT16*)startAddr) != OK)
        return ERROR;

    FLASH_REGISTER_WRITE(flashPtr, CMD_READ);

    return OK;
    }

/*******************************************************************************
*
* sysFlashWrite - write to flash
*
* This routine writes a word into flash.
*
* RETURNS: OK or ERROR if program fail.
*
* ERRNO: N/A
*/

STATUS sysFlashWrite
    (
    volatile UINT16 * address,
    volatile UINT16   data
    )
    {
    STATUS sts = OK;
    UINT16 * flashPtr;

    flashPtr = (UINT16 *)address;

    sts = sysFlashWordProgram((UINT16 *)(address), data);


    if (sts != OK)
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("[0x%x]word write status NOT ok\n", (UINT16)flashPtr);  
#endif
        }
        
    return sts;
    }

/*******************************************************************************
*
* sysFlashGet - get the contents of flash memory
*
* This routine copies the contents of flash memory into a specified
* string.
*
* RETURNS: OK, or ERROR if access is outside the flash memory range.
*
* SEE ALSO: sysFlashSet()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(),
* they should use a semaphore to ensure mutually exclusive access.
*/

STATUS sysFlashGet
    (
    char *  string,     /* where to copy flash memory      */
    int     strLen,     /* maximum number of bytes to copy */
    int     offset      /* byte offset into flash memory   */
    )
    {
    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > FLASH_MEM_SIZE))
        return (ERROR);
    
    bcopy((char *) (FLASH_BASE_ADRS + offset), string, strLen);
   
    return (OK);
    }

/*******************************************************************************
*
* sysFlashSet - write to flash memory
*
* This routine copies a specified string into flash memory.
*
* If the specified string must be overlaid on the contents of flash memory,
*  FLASH_OVERLAY in config.h.
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
    char *  string,     /* string to be copied into flash memory */
    int     byteLen,    /* maximum number of bytes to copy       */
    int     offset      /* byte offset into flash memory         */
    )
    {

    UINT16 startSec;
    char * tempBuf;
#ifndef FLASH_OVERLAY    
    UINT16 i;
    UINT32 flashStartAddr, tempBufStartAddr;
#endif /* FLASH_OVERLAY */

    /* limited to one sector */

    if ((offset < 0) || (byteLen < 0) || 
        (((offset % FLASH_SEGMENT_SIZE) + byteLen) > FLASH_SEGMENT_SIZE))
        return (ERROR);

    /* see if contents are actually changing */

    if (bcmp ((char *) (FLASH_BASE_ADRS + offset), string, byteLen) == 0)
        return (OK);


    if ((tempBuf = memalign(4, FLASH_SEGMENT_SIZE)) == NULL)
        return (ERROR);

    startSec = (offset) / FLASH_SEGMENT_SIZE;

#ifdef FLASH_OVERLAY

    /* first save the current data in this sector */

    bcopy((char *)(FLASH_BASE_ADRS + startSec * FLASH_SEGMENT_SIZE),
               tempBuf, FLASH_SEGMENT_SIZE);       
    bcopy(string, tempBuf + (offset % FLASH_SEGMENT_SIZE), byteLen);
#else
    bcopy(string, tempBuf, byteLen);
#endif  /* FLASH_OVERLAY */

    if(sysFlashBlkErase(startSec) == ERROR)
        {
        free(tempBuf);
        return (ERROR);
        }

#ifdef FLASH_OVERLAY                /* program device */
    sysFlashProgram((UINT16 *)(FLASH_BASE_ADRS + startSec * FLASH_SEGMENT_SIZE),
                    (UINT16 *)tempBuf, FLASH_SEGMENT_SIZE / 2);
#else   /* FLASH_OVERLAY */

    for(i = 0; i < ((byteLen + 1) / 2); i++)
        {
        flashStartAddr = FLASH_BASE_ADRS + startSec * FLASH_SEGMENT_SIZE + 
                         (offset % FLASH_SEGMENT_SIZE) + i * 2;
        tempBufStartAddr = (UINT32)(tempBuf + i * 2);
        
        /* 
         * Note if FLASH_OVERLAY is not defined:
         * Here we assume that the specified start offset in a sector will 
         * always be aligned with 4 bytes. This routines should be updated if
         * we find that some cases conflict with this assumption.
         */
        
        if (sysFlashWordProgram((UINT16 *)flashStartAddr, 
                                *(UINT16 *)tempBufStartAddr) != OK)
            {
            free(tempBuf);
            return (ERROR);
            }
        }
#endif  /* FLASH_OVERLAY */

        free(tempBuf);
        return (OK);
    }

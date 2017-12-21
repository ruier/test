/* flashMem.c - Freescale MDS8569 Board flash memory device driver */

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
01a,04jul09,x_z  derived from version 01b of amcc_kilauea/flashMem.c.
*/

/*
DESCRIPTION
This library contains routines to manipulate flash memory.Read and write
routines are included.

The macro values FLASH_ADRS, FLASH_SIZE, FLASH_WIDTH, FLASH_CHIP_WIDTH,
FLASH_SECTOR_SIZE and FLASH_SECTOR_MASK must be defined to indicate the address,
size (in bytes), the data-access width (in bytes), the chip data-access width
(in bytes), the sector size (in bytes), the sector base address mask of the
flash memory.

If the flash memory needs to be overlaid, and the section of the memory that
needs to be overlaid is less than FLASH_SIZE, then, for efficiency, define
FLASH_SIZE_WRITABLE to the size (in bytes) of the overlay section.

The routine sysFlashDelay() creates a delay for a specified number of
microseconds.  The timing loop can be adjusted on a board-dependent basis by
defining the function sysFlashBoardDelay and values for the following macros,
.iP
SYS_FLASH_DELAY_SHIFT
.iP
SYS_FLASH_DELAY_ADJ
.iP
SYS_FLASH_DELAY_INCR
.LP
To use the routine sysFlashBoardDelay(), the macro SYS_FLASH_BOARD_DELAY
should be defined.

The macro FLASH_NO_OVERLAY should be defined when calls to sysFlashSet()
are expected to erase the flash and reprogram it with only the new data.

The macro FLASH_LOCKED should be defined when block lock/unlock commands
are required to erase the flash or reprogram it with new data.

To support flash devices that that need to turn on/off write protect features
(special programming voltages or other write-enable features), the macro
SYS_FLASH_WRITE, and the routines, sysFlashWriteEnable() and
sysFlashFlashWriteDisable() should be defined.

INTERNAL:
The FLASH_SIZE_WRITABLE concept doesn't work very well.  It just limits the
amount of flash that is writable, so why bother.  What it was really
intended to address was flash that is only block writable, i.e. you
can only write a complete block at a time.  To properly handle block
memory, you must copy the old block of memory to a buffer, update the
part of the buffer that is to be changed, and then write back the
complete buffer in a single write operation.

The accesses to non-volatile memory, and flash control registers needs
to be abstracted.  Macros should be used for all actual i/o operations.

INCLUDE FILES: stdlib.h flashDev.h

SEE ALSO:
\tb "Spansion S29GL-N Data Sheet".
*/

/* includes */

#include <stdlib.h>
#include <drv/mem/flashDev.h>

/* defines */

#define FLASH_SECTOR(adrs)          ((adrs) & FLASH_SECTOR_MASK)

#define FLASH29_REG_ADRS(reg)       ((char *) FLASH_ADRS + (reg * FLASH_WIDTH))

#define S29GL_VENDOR_OFFSET         0
#define S29GL_DEVICE_OFFSET         2

/* FLASH29 command definitions */

#define FLASH29_REG_FIRST_CYCLE     FLASH29_REG_ADRS(0xAAA)
#define FLASH29_REG_SECOND_CYCLE    FLASH29_REG_ADRS(0x555)

#define	FLASH29_CMD_FIRST           (FLASH_DEF)0xaaaaaaaa
#define	FLASH29_CMD_SECOND          (FLASH_DEF)0x55555555
#define	FLASH29_CMD_FOURTH          (FLASH_DEF)0xaaaaaaaa
#define	FLASH29_CMD_FIFTH           (FLASH_DEF)0x55555555
#define	FLASH29_CMD_SIXTH           (FLASH_DEF)0x10101010
#define	FLASH29_CMD_SECTOR          (FLASH_DEF)0x30303030

#define	FLASH29_CMD_PROGRAM         (FLASH_DEF)0xa0a0a0a0
#define	FLASH29_CMD_CHIP_ERASE      (FLASH_DEF)0x80808080
#define	FLASH29_CMD_READ_RESET      (FLASH_DEF)0xf0f0f0f0
#define	FLASH29_CMD_AUTOSELECT      (FLASH_DEF)0x90909090

/* Establish default values for DELAY parameters */

#ifndef SYS_FLASH_DELAY_SHIFT
#   define  SYS_FLASH_DELAY_SHIFT   0
#endif /*SYS_FLASH_DELAY_SHIFT*/

#ifndef SYS_FLASH_DELAY_ADJ
#   define  SYS_FLASH_DELAY_ADJ     0
#endif  /* SYS_FLASH_DELAY_ADJ */

#ifndef SYS_FLASH_DELAY_INCR
#   define  SYS_FLASH_DELAY_INCR    1
#endif  /* SYS_FLASH_DELAY_INCR */

#ifdef  SYS_FLASH_WRITE
#	define SYS_FLASH_WRITE_ENABLE_RTN()    sysFlashWriteEnable()
#   define SYS_FLASH_WRITE_DISABLE_RTN()   sysFlashWriteDisable()
#else
#   define SYS_FLASH_WRITE_ENABLE_RTN()
#   define SYS_FLASH_WRITE_DISABLE_RTN()
#endif  /* SYS_FLASH_WRITE */

#ifdef  SYS_FLASH_BOARD_DELAY
#   define SYS_FLASH_BOARD_DELAY_RTN()     sysFlashBoardDelay()
#else
#   define SYS_FLASH_BOARD_DELAY_RTN()
#endif  /* SYS_FLASH_BOARD_DELAY */

/* Operation status bits for Flash 29Fxxx devices */

#define Q7(ix)                             ((ix & 0x80) >> 7)  /* DQ7 bit */
#define Q5(ix)                             ((ix & 0x20) >> 5)  /* DQ5 bit */

/* globals */

IMPORT void sysUsDelay (int);

/* locals */

LOCAL int flashDelayShift = SYS_FLASH_DELAY_SHIFT;
LOCAL int flashDelayAdj   = SYS_FLASH_DELAY_ADJ;
LOCAL int flashDelayIncr  = SYS_FLASH_DELAY_INCR;

/*******************************************************************************
*
* sysFlashDelay - create a delay for a specified number of microseconds
*
* This routine implements a busy wait for a specified number of microseconds.
* The timing loop can be adjusted on a board-dependent basis by
* defining values for the following macros:
* .iP
* SYS_FLASH_DELAY_SHIFT
* .iP
* SYS_FLASH_DELAY_ADJ
* .iP
* SYS_FLASH_DELAY_INCR
* .LP
* The values SYS_FLASH_DELAY_SHIFT and SYS_FLASH_DELAY_ADJ
* convert microseconds into a board-dependent tick-count.
* This routine can call a user-defined hook, sysFlashBoardDelay(),
* which creates a delay for a number of board-dependent ticks as
* specified by SYS_FLASH_DELAY_INCR.  To use sysFlashBoardDelay(), define
* SYS_FLASH_BOARD_DELAY in config.h.
*
* RETURNS: N/A
*
* SEE ALSO: sysFlashWrite()
*/

void sysFlashDelay
    (
    int delayCount  /* number of uSec to delay */
    )
    {
    int ix;

    delayCount <<= flashDelayShift;	/* board-dependent shift */
    delayCount += flashDelayAdj;    /* board-dependent addition */

    for (ix = 0; ix < delayCount; ix += flashDelayIncr)
        SYS_FLASH_BOARD_DELAY_RTN ();
    }

/*******************************************************************************
*
* sysFlashDataPoll - wait for a flash device operation to complete
*
* This routine polls a specified address on the flash device until the device
* operation at that location completes or times out.
*
* While a flash operation is in progress, a read on the device returns on Q7
* (data bit 7) the complement of the previous value of Q7.  Once the flash
* operation has completed, the Q7 bit returns the true data of the last write.
* Subsequent reads return the correct data in Q0-7.
*
* The Q5 bit implements a timeout functionality.  When a currently executing
* flash operation exceeds specified limits, the Q5 bit is set (to 1).
*
* RETURNS: OK, or ERROR if the timeout (!Q5) occurs before the device operation
* completes.
*
* SEE ALSO: sysFlashWrite()
*/

STATUS sysFlashDataPoll
    (
    volatile FLASH_DEF *    pFA,    /* programmed address to poll */
    FLASH_DEF               value   /* data programmed to poll address */
    )
    {
    STATUS                      retVal  = OK;
    volatile FLASH_POLL_DEF *   pTest   = (FLASH_POLL_DEF *) pFA;
    FLASH_DEF *                 pValTmp = (FLASH_DEF *) &value;
    volatile FLASH_POLL_DEF *   pVal    = (FLASH_POLL_DEF *) pValTmp;
    int                         ix;         /* byte counter */
    int                         vBit;       /* programmed value of DQ7 */

    for (ix = (FLASH_WIDTH / FLASH_CHIP_WIDTH - 1);
         (ix >= 0 ) && (retVal == OK);
         ix--, pTest++, pVal++)
        {
        vBit = Q7 (*pVal);

        while (Q7 (*pTest) != vBit)
            if (Q5 (*pTest) == 1)    /* timeout ? */
                break;

        if (Q7 (*pTest) != vBit)     /* check Q7 & Q5 race */
            retVal = ERROR;
        }

    return (retVal);
    }

/*******************************************************************************
*
* sysSectorErase - erase the contents of a sector
*
* This routine clears the contents of one sector in the flash memory.
*
* RETURNS: OK, or ERROR if the contents of sector cannot be erased.
*/

STATUS sysSectorErase
    (
    volatile FLASH_DEF * pFA     /* sector start address */
    )
    {
    STATUS retVal = OK;

    SYS_FLASH_WRITE_ENABLE_RTN ();      /* enable write */

    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_FIRST;
    *(FLASH_CAST FLASH29_REG_SECOND_CYCLE) = FLASH29_CMD_SECOND;
    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_CHIP_ERASE;
    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_FOURTH;
    *(FLASH_CAST FLASH29_REG_SECOND_CYCLE) = FLASH29_CMD_FIFTH;
    *pFA                                   = FLASH29_CMD_SECTOR;

    sysUsDelay (10);

    do  {
        retVal = sysFlashDataPoll (pFA, (FLASH_DEF) 0xffffffff);
        } while ((*pFA != (FLASH_DEF) 0xffffffff) && (retVal == OK));

    SYS_FLASH_WRITE_DISABLE_RTN ();     /* disable write */

    return (retVal);
    }

/*******************************************************************************
*
* sysFlashWrite - write data to flash memory
*
* This routine copies specified data of a specified length, <size>, into a
* specified offset, <offset>, in the flash memory.  Data is passed as a string,
* <pFB>, if not NULL.  If NULL, data is taken as a repeated sequence of
* <value>.
* The parameter <offset> must be appropriately aligned for the width of
* the Flash devices in use.
*
* RETURNS: OK, or ERROR if the write operation fails.
*
* SEE ALSO: sysFlashSet()
*/

STATUS sysFlashWrite
    (
    FLASH_DEF *     pFB,        /* string to be copied; use <value> if NULL */
    int             size,       /* size to program in bytes */
    int             offset,     /* byte offset into flash memory */
    FLASH_DEF       value       /* value to program */
    )
    {
    volatile FLASH_DEF * pFA;   /* flash address */
    STATUS              retVal = OK;

    SYS_FLASH_WRITE_ENABLE_RTN ();      /* enable write */

    for (pFA = FLASH_CAST (FLASH_ADRS + offset);
         pFA < FLASH_CAST (FLASH_ADRS + size + offset) && (retVal == OK);
         pFA++)
        {
        *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_FIRST;
        *(FLASH_CAST FLASH29_REG_SECOND_CYCLE) = FLASH29_CMD_SECOND;
        *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_PROGRAM;
        if (pFB != NULL)
            value = *pFB++;

        *pFA = value;                   /* data to write */

        sysUsDelay (10);

        do  {
            retVal = sysFlashDataPoll (pFA, (FLASH_DEF) value);
            } while ((*pFA != value) && (retVal == OK));
        }

    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_FIRST;
    *(FLASH_CAST FLASH29_REG_SECOND_CYCLE) = FLASH29_CMD_SECOND;
    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_READ_RESET;

    SYS_FLASH_WRITE_DISABLE_RTN ();     /* disable write */

    return (retVal);
    }

/*******************************************************************************
*
* sysFlashGet - get the contents of flash memory
*
* This routine copies the contents of flash memory into a specified
* string.  The string is terminated with an EOS.
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
    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > FLASH_SIZE))
        return (ERROR);

    bcopyBytes ((char *) (FLASH_ADRS + offset), string, strLen);
    string [strLen] = EOS;

    return (OK);
    }

/*******************************************************************************
*
* sysFlashSet - write to flash memory
*
* This routine copies a specified string into flash memory.
*
* RETURNS: OK, or ERROR if the write fails or the input parameters are
* out of range.
*
* SEE ALSO: sysFlashGet(), sysFlashTypeGet(), sysFlashWrite()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(),
* they should use a semaphore to ensure mutually exclusive access to flash
* memory.
*/

STATUS sysFlashSet
    (
    char *  string,     /* string to be copied into flash memory */
    int     strLen,     /* maximum number of bytes to copy       */
    int     offset      /* byte offset into flash memory         */
    )
    {
    char * tempBuffer;

    /* limited to one sector */

    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > FLASH_SIZE) ||
        ((offset - FLASH_SECTOR (offset) + strLen) > FLASH_SECTOR_SIZE))
        return (ERROR);

    /* see if contents are actually changing */

    if (bcmp ((char *) (FLASH_ADRS + offset), string, strLen) == 0)
        return (OK);

    /* first read existing data */

    if ((tempBuffer = malloc (FLASH_SECTOR_SIZE)) == NULL)
        return (ERROR);

    bcopyBytes ((char *) FLASH_SECTOR (FLASH_ADRS + offset),
                tempBuffer, FLASH_SECTOR_SIZE);
    bcopyBytes (string, (tempBuffer + offset - FLASH_SECTOR (offset)), strLen);

    if (sysSectorErase ((volatile FLASH_DEF *) (FLASH_SECTOR (FLASH_ADRS + offset))) == ERROR)
        {
        free (tempBuffer);
        return (ERROR);
        }

    if (sysFlashWrite ((FLASH_DEF *) (tempBuffer), FLASH_SECTOR_SIZE,
                       FLASH_SECTOR (offset), 0) == ERROR)
        {
        free (tempBuffer);
        return (ERROR);
        }

    free (tempBuffer);

    return (OK);
    }

/*******************************************************************************
*
* sysFlashTypeGet - determine the device type of on-board flash memory
*
* This routine uses the `autoselect' command to determine the device type of
* on-board flash memory.
*
* RETURNS: An integer indicating the device type of on-board flash memory.
*/

UINT16 sysFlashTypeGet (void)
    {
    UINT16 retVal;

    SYS_FLASH_WRITE_ENABLE_RTN ();  /* enable writes */

    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_FIRST;
    *(FLASH_CAST FLASH29_REG_SECOND_CYCLE) = FLASH29_CMD_SECOND;
    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_AUTOSELECT;

    /*
     * 29LV512(at least) requires 20ms delay
     * It seems we cannot always safely use taskDelay()
     */

    sysFlashDelay (20000);

    retVal =  ((*(volatile UINT8 *) (FLASH_ADRS + S29GL_VENDOR_OFFSET)) << 8) |
              (*(volatile UINT8 *) (FLASH_ADRS + S29GL_DEVICE_OFFSET));

    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_FIRST;
    *(FLASH_CAST FLASH29_REG_SECOND_CYCLE) = FLASH29_CMD_SECOND;
    *(FLASH_CAST FLASH29_REG_FIRST_CYCLE)  = FLASH29_CMD_READ_RESET;

    sysFlashDelay (20000);

    SYS_FLASH_WRITE_DISABLE_RTN ();     /* disable writes */

    return (retVal);
    }

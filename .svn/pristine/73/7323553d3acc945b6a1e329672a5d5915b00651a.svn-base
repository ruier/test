/* flashMem.c - Flash memory device driver */

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
01b,28dec10,e_d  modified the FLASH28F008_TIMEOUT value. (WIND00248412)
01a,28jan10,e_d  derived from version 01s of.ibm_evb750cl/flashMem.c
*/

/*
DESCRIPTION
This library contains routines to manipulate flash memory.  Read and write
routines are included.

The macro values FLASH_ADRS, FLASH_SIZE, and FLASH_WIDTH must be defined to
indicate the address, size (in bytes), and the data-access width (in bytes) of
the flash memory.

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

To support flash devices that that need to turn on/off write protect features
(special programming voltages or other write-enable features), the macro
SYS_FLASH_WRITE, and the routines, sysFlashWriteEnable() and
sysFlashWriteDisable() should be defined.

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
*/

#include <drv/mem/flashDev.h>

/* defines */

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

/* Names of routines, or null values */

#ifdef  SYS_FLASH_WRITE
#   define SYS_FLASH_WRITE_ENABLE_RTN()  sysFlashWriteEnable ()
#   define SYS_FLASH_WRITE_DISABLE_RTN() sysFlashWriteDisable ()
#else
#   define SYS_FLASH_WRITE_ENABLE_RTN()
#   define SYS_FLASH_WRITE_DISABLE_RTN()
#endif  /* SYS_FLASH_WRITE */

#ifdef  SYS_FLASH_BOARD_DELAY
#   define SYS_FLASH_BOARD_DELAY_RTN()   sysFlashBoardDelay ()
#else
#   define SYS_FLASH_BOARD_DELAY_RTN()
#endif  /* SYS_FLASH_BOARD_DELAY */

#ifdef SYS_FLASH_TYPE
#   define FLASH_MEM_TYPE                SYS_FLASH_TYPE
#else
#   define FLASH_MEM_TYPE                0
#endif  /* SYS_FLASH_TYPE */

#ifdef FLASH_SIZE_WRITEABLE
#   define FLASH_MEM_SIZE                FLASH_SIZE_WRITEABLE
#else
#   define FLASH_MEM_SIZE                FLASH_SIZE
#endif  /* FLASH_SIZE_WRITEABLE */

#define FLASH_SECTOR(adrs)              ((adrs) & FLASH_SECTOR_MASK)

#define FLASH28_CMD_READ_ID             0x0090
#define FLASH28_CMD_ERASE_SETUP         0x0020
#define FLASH28_CMD_PROG_SETUP          0x0040
#define FLASH28_CMD_RESET               0x00FF
#define FLASH28F008_CMD_READ_STATUS     0x0070
#define FLASH28F008_CMD_CLEAR_STATUS    0x0050
#define FLASH28F008_CMD_ERASE           0x00D0
#define FLASH28_CMD_LOCK_SETUP          0x0060
#define FLASH28_CMD_UNLOCK              0x00D0

#define FLASH28F008_STAT_WSMS           0x0080
#define FLASH28F008_STAT_EWS            0x0020

#define FLASH28F008_TIMEOUT             0x500000

/* globals */

IMPORT  void sysFlashWriteEnable (void);
IMPORT  void sysFlashWriteDisable (void);
IMPORT  void sysFlashBoardDelay (void);

int flashDelayShift = SYS_FLASH_DELAY_SHIFT;
int flashDelayAdj   = SYS_FLASH_DELAY_ADJ;
int flashDelayIncr  = SYS_FLASH_DELAY_INCR;

/* forward declarations */

#ifdef  __STDC__

void    sysFlashDelay (int delayCount);
STATUS  sysFlashWrite (FLASH_DEF * pFB, int size, int offset, FLASH_DEF value);
UINT16  sysFlashTypeGet (void);

#else   /* __STDC__ */

void    sysFlashDelay ();
STATUS  sysFlashWrite ();
UINT16  sysFlashTypeGet ();

#endif  /* __STDC__ */

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
* SEE ALSO: sysFlashErase(), sysFlashWrite()
*/

void sysFlashDelay
    (
    int delayCount  /* number of uSec to delay */
    )
    {
    int ix;

    delayCount <<= flashDelayShift;     /* board-dependent shift */
    delayCount += flashDelayAdj;        /* board-dependent addition */

    for (ix = 0; ix < delayCount; ix += flashDelayIncr)
        SYS_FLASH_BOARD_DELAY_RTN ();
    }

/*******************************************************************************
*
* sysSectorErase - erase the contents of a sector
*
* This routine clears the contents of one sector in the flash memory.
*
* Flash 28F256P30T devices are erased by writing the six-byte erase code
* into specific address locations, which sets all byte locations to a high
* value (0xFF).
*
* RETURNS: OK, or ERROR if the contents of sector cannot be erased.
*/

STATUS sysSectorErase
    (
    FLASH_DEF * pFA        /* Sector start address */
    )
    {
    int timeOut = FLASH28F008_TIMEOUT;
    STATUS retVal = OK;
    volatile FLASH_DEF * pFD = FLASH_CAST (FLASH_ADRS);
    SYS_FLASH_WRITE_ENABLE_RTN ();

    * pFD = FLASH28F008_CMD_CLEAR_STATUS; /* clear status reg */
    * pFA = FLASH28_CMD_ERASE_SETUP;      /* setup */
    * pFA = FLASH28F008_CMD_ERASE;        /* erase */

    /* Check Write State Machine Status */

    do
        {
        if ((--timeOut) <= 0)
            {
            retVal = ERROR;
            break;
            }
        * pFD = FLASH28F008_CMD_READ_STATUS;
        }while(((* pFD) & FLASH28F008_STAT_WSMS) != FLASH28F008_STAT_WSMS );

    /* Check Erase Error Status */

    if ((*pFD & FLASH28F008_STAT_EWS) != 0)
        {
        * pFA = FLASH28F008_CMD_CLEAR_STATUS;
        retVal = ERROR;
        }
    * pFD = FLASH28_CMD_RESET;

    SYS_FLASH_WRITE_DISABLE_RTN ();

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
    FLASH_DEF * pFB,        /* string to be copied; use <value> if NULL */
    int         size,       /* size to program in bytes */
    int         offset,     /* byte offset into flash memory */
    FLASH_DEF   value       /* value to program */
    )
    {
    volatile FLASH_DEF * pFA = FLASH_CAST (FLASH_ADRS);       /* flash address */
    volatile FLASH_DEF * pFD = FLASH_CAST (FLASH_ADRS);
    STATUS retVal = OK;
    int timeOut;
    int i;

    SYS_FLASH_WRITE_ENABLE_RTN ();      /* raise Vpp */

    * pFD = FLASH28F008_CMD_CLEAR_STATUS;

    for(i = 0; i < (size/2); i++)
        {
        timeOut = FLASH28F008_TIMEOUT;

        if (pFB != NULL)
            value = *pFB++;

        * (pFA + offset/2 + i) = FLASH28_CMD_PROG_SETUP;  /* write setup */
        * (pFA + offset/2 + i) = value;                   /* data to write */

        do
            {
            if (( --timeOut) <= 0)
                {
                retVal = ERROR;
                break;
                }
            * pFD = FLASH28F008_CMD_READ_STATUS;
            }while(((* pFD) & FLASH28F008_STAT_WSMS) != FLASH28F008_STAT_WSMS);
        * pFD = FLASH28_CMD_RESET;
        }

    SYS_FLASH_WRITE_DISABLE_RTN ();     /* lower Vpp */

    return (retVal);
    }

/*******************************************************************************
*
* sysFlashTypeGet - determine the device type of on-board flash memory
*
* Get the flash device type.
*
* RETURNS: An integer indicating the device type of on-board flash memory.
*/

UINT16 sysFlashTypeGet (void)
    {
    volatile FLASH_DEF * pFA = FLASH_CAST (FLASH_ADRS); /* flash address */
    UINT16 retVal;

    SYS_FLASH_WRITE_ENABLE_RTN ();          /* enable writes */

    * pFA = FLASH28_CMD_READ_ID;

    sysFlashDelay(10);

    retVal = * (pFA + 1);

    SYS_FLASH_WRITE_DISABLE_RTN ();         /* disable writes */

    * pFA = FLASH28F008_CMD_CLEAR_STATUS;
    * pFA = FLASH28_CMD_RESET;

    return (retVal);
    }

/*******************************************************************************
*
* sysFlashSet - write to flash memory
*
* This routine copies a specified string into flash memory after calling
* sysFlashErase() and clearing flash memory.
*
* If FLASH_NO_OVERLAY is defined, the parameter <offset> must be
* appropriately aligned for the Flash devices in use (device width,
* sector size etc.).
*
* If the specified string must be overlaid on the contents of flash memory,
* undefine FLASH_NO_OVERLAY.
*
* RETURNS: OK, or ERROR if the write fails or the input parameters are
* out of range.
*
* SEE ALSO: sysFlashErase(), sysFlashGet(), sysFlashTypeGet(), sysFlashWrite()
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

#ifndef FLASH_NO_OVERLAY
    char *tempBuffer;
#endif  /* FLASH_NO_OVERLAY */


    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > FLASH_MEM_SIZE))
        return (ERROR);

    /* see if contents are actually changing */

    if (bcmp ((char *) (FLASH_ADRS + offset), string, strLen) == 0)
        return (OK);


#ifndef FLASH_NO_OVERLAY

    /* first read existing data */

    if (tempBuffer = malloc(FLASH_MEM_SIZE), tempBuffer == 0)
        return (ERROR);

    bcopyBytes ((char *) FLASH_SECTOR (FLASH_ADRS + offset),
                tempBuffer, FLASH_SECTOR_SIZE);
    bcopyBytes (string, (tempBuffer + offset - FLASH_SECTOR (offset)), strLen);
#endif  /* FLASH_NO_OVERLAY */
    if (sysSectorErase ((FLASH_DEF *) (FLASH_SECTOR (FLASH_ADRS + offset))) == ERROR)
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
* sysFlashProtect - Change sector lock bits
*
* This routine sets or clears the sector lock bits of sectors where the
* specified data occurs.
*
* Because each sector must be protected as a whole, data outside the specified
* range may be affected as well.  The routine will compensate for <size> and
* <offset> parameters which are not on sector addresses.
*
* RETURNS: OK, or ERROR if the operation was not a success.
*
* SEE ALSO: sysFlashErase(), sysFlashGet(), sysFlashTypeGet(), sysFlashWrite()
*
* INTERNAL
* If multiple tasks are calling Flash routineds they should use a semaphore to
* ensure mutually exclusive access to flash memory.  This routine is only
* defined for 28FxxxJ3A devices.  Specifying an unsupported device will return
* OK.
*/

STATUS sysFlashProtect
    (
    int size,
    int offset,
    int protectLevel
    )
    {

    return OK;
   }

/*******************************************************************************
*
* sysFlashUnlockAll - Unlock all sectors
*
* This routine clears the sector lock bits of sectors where the
* specified data occurs.
*
* SEE ALSO: sysFlashErase(), sysFlashGet(), sysFlashTypeGet(), sysFlashWrite()
*/

void sysFlashUnlockAll (void)
    {
    int i = 0;
    volatile FLASH_DEF * pFA = FLASH_CAST (FLASH_ADRS);

    for(i = 0; i < (FLASH_SIZE / FLASH_SECTOR_SIZE); i++)
        {
        pFA = (volatile FLASH_DEF *)(FLASH_ADRS + i * FLASH_SECTOR_SIZE);
        * pFA = FLASH28_CMD_LOCK_SETUP;
        * pFA = FLASH28_CMD_UNLOCK;
        }

    * pFA = FLASH28_CMD_RESET;
    }


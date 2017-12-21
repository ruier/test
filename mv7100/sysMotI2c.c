/* sysMotI2c.c - I2C Routines */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01f,28aug09,mdg Changed flag that tracks if I2C should use locking from
                global variable to UART scratch register, fixing issue with 
                bootrom_uncmp. (WIND00179587)
01e,03mar09,mdg Changed from Spin Lock to Semaphore locking, routines
                are now usable ONLY from task context.
01d,22feb08,mdg Added check for CPU1 in DUAL_CORE operation.
01c,26apr05,cak Moved the definition for the core complex bus default
                speed to the board header file for use by other files.
01b,10nov04,scb Replace sysI2cMsDelay with sysI2cUsDelay diddle with delays.
01a,09nov04,scb Created.
*/

/*
DESCRIPTION
This file contains the following system I2C utility routines:

.CS
  sysMotI2cWrite     - Write to device on I2C bus
  sysMotI2cRead      - Read from device on I2C bus
.CE

CAVEATS
These routines use semphores/task callable spin locks as a locking mechanism,
therefore, they are NOT callable from interrupt context.
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "stdlib.h"
#include "mv7100.h"
#include "sysMpc8641I2c.h"
#include "mpc8641.h"
#include "semLib.h"

#ifdef INCLUDE_AMP
IMPORT BOOL sysCoreLockTake(char *);
IMPORT BOOL sysCoreLockTaskTake(char *);
IMPORT void sysCoreLockGive(char *);
#endif /* INCLUDE_AMP */

/* defines */

#define I2C_OFFSET_BYTE0(a)    (((a) >> 0) & 0xff)
#define I2C_OFFSET_BYTE1(a)    (((a) >> 8) & 0xff)
#define I2C_OFFSET_BYTE2(a)    (((a) >> 16) & 0xff)

/* 
 * Use the Scratch register for the Second UART to hold the 
 * flag that indicates if I2C accesses should be locked
 * with semaphore/lock barriers 
 */
 
#define SYS_MOT_I2C_INITED  CCSR_READ32(CCSBAR,CCSR_USCR1)

/* typedefs */

/* globals */

#ifdef INCLUDE_AMP
BOOL sysMotI2CLock = FALSE;
#else
LOCAL SEM_ID sysMotI2CMutex;
#endif /* INCLUDE_AMP */

/* locals */

/* forward declarations */

STATUS sysMotI2cRead  (UINT32 devAddr, UINT32 devOffset, UINT32    devAddrBytes,
                       UINT8  *pBfr, UINT32 numBytes);
STATUS sysMotI2cWrite (UINT32 devAddr, UINT32 devOffset, UINT32    devAddrBytes,
                       UINT8  *pBfr, UINT32 numBytes);

void sysI2cUsDelay (UINT32 delay);

LOCAL BOOL i2cDevAddrWriteValid (UINT32 devAddr);

/* externals */

IMPORT STATUS I2C_CYCLE_START (UINT32 base);
IMPORT STATUS I2C_CYCLE_STOP (UINT32 base);
IMPORT STATUS I2C_CYCLE_READ (UINT32 base, UINT8 * pDataChar);             
IMPORT STATUS I2C_CYCLE_WRITE (UINT32 base, UINT8  pDataChar);
IMPORT STATUS I2C_CYCLE_ACKIN (UINT32 base);
IMPORT STATUS I2C_CYCLE_ACKOUT (UINT32 base);
IMPORT STATUS I2C_KNOWN_STATE (UINT32 base);
IMPORT UINT32 sysTimeBaseLGet (void);

/*******************************************************************************
*
* sysMotI2cInit - i2c Init
*
* This function init I2c variable.
*
* RETURNS: N/A.
*/
void sysMotI2CInit (void)
    {
#ifdef INCLUDE_AMP
#else
    sysMotI2CMutex = semMCreate (SEM_Q_PRIORITY |
                                 SEM_DELETE_SAFE |
                                 SEM_INVERSION_SAFE);
#endif /* INCLUDE_AMP */
    
    /* 
     * Set the Scratch register for the Second UART to indicated that I2C 
     * accesses should be locked with semaphore/spinlock barriers 
     */

    CCSR_WRITE32(CCSBAR,CCSR_USCR1,1);
    }

/*******************************************************************************
*
* sysMotI2cRead - i2c read bytes
*
* This function reads the specified number of bytes from the specified device.
*
* RETURNS: OK, or ERROR if transfer failed.
*/

STATUS sysMotI2cRead
    (
    UINT32 devAddr,       /* I2C address of target device */
    UINT32 devOffset,     /* Offset within device where 'read' begins */
    UINT32 devAddrBytes,  /* Number of bytes in protocol offset into device */ 
    UINT8  *pBfr,         /* pointer to 'read' buffer */
    UINT32 numBytes       /* Number of bytes to read */ 
    )
    {
    UINT32 byteCount;

#ifdef INCLUDE_AMP
    if (SYS_MOT_I2C_INITED) 
        {
        if (sysMotI2CLock) 
            {
            if (sysCoreLockTaskTake((char *)SM_SLOCK_I2C) == FALSE) 
                {
                return(ERROR);
                }
            }
        }
#else
    if (SYS_MOT_I2C_INITED)
        {
        semTake(sysMotI2CMutex,WAIT_FOREVER);
        }
#endif /* INCLUDE_AMP */

    /* Zero byte count is considered an error */

    if (numBytes == 0)
        goto error_done;

    /* Start with I2c bus in known state */

    (void) I2C_KNOWN_STATE(CCSBAR);

    /* Read the specified number of bytes from the ERPROM. */

    for    (byteCount = 0; byteCount < numBytes; byteCount++)
        {
        if (devAddrBytes == I2C_SINGLE_BYTE_ADDR)
            {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr | 0x01) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_READ(CCSBAR, (UINT8 *)pBfr + byteCount) == ERROR)
                goto error_done;
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                goto error_done;
            }

        else if (devAddrBytes == I2C_DUAL_BYTE_ADDR)
            {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr | 0x01) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_READ(CCSBAR, (UINT8 *)pBfr + byteCount) == ERROR)
                goto error_done;
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                goto error_done;
            }

        else if (devAddrBytes == I2C_TRIPLE_BYTE_ADDR)
            {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE2(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr | 0x01) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_READ(CCSBAR, (UINT8 *)pBfr + byteCount) == ERROR)
                goto error_done;
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                goto error_done;
            }

        else 
            goto error_done;
        }

#ifdef INCLUDE_AMP
    if (SYS_MOT_I2C_INITED) 
        {
        if (sysMotI2CLock) 
            sysCoreLockGive((char *)SM_SLOCK_I2C);
        }
#else
    if (SYS_MOT_I2C_INITED)
        semGive(sysMotI2CMutex);
#endif /* INCLUDE_AMP */
    return (OK);

error_done:
#ifdef INCLUDE_AMP
    if (SYS_MOT_I2C_INITED) 
        {
        if (sysMotI2CLock) 
            sysCoreLockGive((char *)SM_SLOCK_I2C);
        }
#else
    if (SYS_MOT_I2C_INITED)
        semGive(sysMotI2CMutex);
#endif /* INCLUDE_AMP */
    return (ERROR);
    }

/******************************************************************************
*
* sysMotI2cWrite - write to an I2C device 
*
* This function writes to an I2C device.
*
* RETURNS: OK, or ERROR if transfer failed.
*/

STATUS sysMotI2cWrite 
    (
    UINT32 devAddr,       /* I2C address of target device */
    UINT32 devOffset,     /* Offset within device where 'read' begins */
    UINT32 devAddrBytes,  /* Number of bytes in protocol offset for device */ 
    UINT8  *pBfr,         /* Pointer to 'write' buffer */
    UINT32 numBytes       /* Number of bytes to read */ 
    )
    {
    UINT32 byteCount;

#ifdef INCLUDE_AMP
    if (SYS_MOT_I2C_INITED) 
        {
        if (sysMotI2CLock) 
            {
            if (sysCoreLockTaskTake((char *)SM_SLOCK_I2C) == FALSE) 
                {
                return(ERROR);
                }
            }
        }
#else
    if (SYS_MOT_I2C_INITED)
        {
        semTake(sysMotI2CMutex,WAIT_FOREVER);
        }
#endif /* INCLUDE_AMP */

    /* Zero byte count is considered an error */

    if (numBytes == 0)
        goto error_done;

    /* Make sure that the device address is valid for writing */

    if (i2cDevAddrWriteValid (devAddr) == FALSE)
        goto error_done;

    /* Make sure that writing is enabled on this board */

    I2C_WRITE_ENABLE

    (void) I2C_KNOWN_STATE(CCSBAR);

    for (byteCount = 0; byteCount < numBytes; byteCount++)
        {
        if (devAddrBytes == I2C_SINGLE_BYTE_ADDR)
            {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr))
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, pBfr [byteCount]) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                goto error_done;
            }

        else if (devAddrBytes == I2C_DUAL_BYTE_ADDR)
            {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr))
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, pBfr [byteCount]) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                goto error_done;
            }

        else if (devAddrBytes == I2C_TRIPLE_BYTE_ADDR)
            {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr))
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE2(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_WRITE(CCSBAR, pBfr [byteCount]) == ERROR)
                goto error_done;
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                goto error_done;
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                goto error_done;
            }
        else
            goto error_done;

         /*
          * delay for at least 5ms to allow EEPROM to complete
          * the write cycle (internal operation)
          */

         sysI2cUsDelay (5000);
         }

#ifdef INCLUDE_AMP
    if (SYS_MOT_I2C_INITED) 
        {
        if (sysMotI2CLock) 
            sysCoreLockGive((char *)SM_SLOCK_I2C);
        }
#else
    if (SYS_MOT_I2C_INITED)
        semGive(sysMotI2CMutex);
#endif
    return (OK);

error_done:
#ifdef INCLUDE_AMP
    if (SYS_MOT_I2C_INITED) 
        {
        if (sysMotI2CLock) 
            sysCoreLockGive((char *)SM_SLOCK_I2C);
        }
#else
    if (SYS_MOT_I2C_INITED)
        semGive(sysMotI2CMutex);
#endif
    return (ERROR);
    }

/*******************************************************************************
*
* sysI2cUsDelay - delay the specified number of microseconds
*
* This routine will delay for at least the specified number of microseconds,
* using the lower 32 bit "word" of the Time Base register as the timer.
* Note this routine assumes a default CCB (Core Complex Bus) speed of 600MHz 
* with timebase  updates every 4 processor cycles.  Slower actual processor 
* speeds than  600MHz will result in delays being proportionately longer 
* than the amount specified in the 'delay' parameter. 
*
* RETURNS: N/A
*/

void sysI2cUsDelay
    (
    UINT32 delay        /* number of milliseconds to delay */
    )
    {
    UINT32 baselineTickCount;
    UINT32 ticksToWait;

    /*
     * Get the Time Base Lower register tick count, this will be used
     * as the baseline.
     */

    baselineTickCount = sysTimeBaseLGet();

    /*
     * Convert delay time into ticks
     *
     * The Time Base register and the Decrementer count at the same rate:
     * once per 8 processory cycles.
     *
     * e.g., 333333333 cycles    1 tick      1 second              42 tick
     *       ---------------  *  ------   *  --------          =  ----------
     *       1 second            4 cycles    1000000 millisec      millisec
     */

    if ((ticksToWait = delay * ((DEFAULT_CCB_SPD / 4) / 1000000)) == 0)
        return;

    while ((sysTimeBaseLGet() - baselineTickCount) < ticksToWait)
    ;

    return;
    }

/******************************************************************************
*
* i2cDevAddrWriteValid - Determine if I2c device address is valid for writing
*
* This function searches for the i2c device address inside of a list of
* "valid for writing" addresses.  The list searched is encoded in 
* VALID_I2C_WRT_ADDR.
*
* RETURNS: TRUE if addres is valid, FALSE otherwise
*/

LOCAL BOOL i2cDevAddrWriteValid
    (
    UINT32 devAddr    /* Device I2C address which we will check */
    )
    {
    LOCAL UINT32 validAddr [] = { VALID_I2C_WRT_ADDR };
    UINT32 *p = validAddr;

    while (*p != 0xffffffff)
    {
    if (*p == devAddr)
        return (TRUE);
        p++;
    }
    return (FALSE);
    }

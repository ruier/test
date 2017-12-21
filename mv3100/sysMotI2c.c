/* sysMotI2c.c - I2C Routines */

/* Copyright 2004-2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
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
These routines are needed before the kernel is un-compressed. For proper
operation, this file must be added to the BOOT_EXTRA list in the Makefile to
prevent it from being compressed during kernel generation.

These routines were primarily intended for use during system start-up, before
multi-tasking is started, and are not multi-tasking safe. They are safe for
command-line use during debug, but are not intended to be run-time
user-callable.
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "stdlib.h"
#include "xxx8540A.h"
#include "sysMpc8540I2c.h"
#include "mpc8540.h"

/* defines */

#define I2C_OFFSET_BYTE0(a)	(((a) >> 0) & 0xff)
#define I2C_OFFSET_BYTE1(a)	(((a) >> 8) & 0xff)
#define I2C_OFFSET_BYTE2(a)	(((a) >> 16) & 0xff)

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

STATUS sysMotI2cRead  (UINT32 devAddr, UINT32 devOffset, UINT32	devAddrBytes,
		       UINT8  *pBfr, UINT32 numBytes);
STATUS sysMotI2cWrite (UINT32 devAddr, UINT32 devOffset, UINT32	devAddrBytes,
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
* sysMotI2cRead - i2c read bytes
*
* This function reads the specified number of bytes from the specified device.
*
* RETURNS: OK, or ERROR if transfer failed.
*/

STATUS sysMotI2cRead
    (
    UINT32 devAddr,	 /* I2C address of target device */
    UINT32 devOffset,	 /* Offset within device where 'read' begins */
    UINT32 devAddrBytes, /* Number of bytes in protocol offset into device */ 
    UINT8  *pBfr,	 /* pointer to 'read' buffer */
    UINT32 numBytes      /* Number of bytes to read */ 
    )
    {
    UINT32 byteCount;

    /* Zero byte count is considered an error */

    if (numBytes == 0)
	return (ERROR);

    /* Start with I2c bus in known state */

    (void) I2C_KNOWN_STATE(CCSBAR);

    /* Read the specified number of bytes from the ERPROM. */

    for	(byteCount = 0;	byteCount < numBytes; byteCount++)
	{
	if (devAddrBytes == I2C_SINGLE_BYTE_ADDR)
	    {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr | 0x01) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_READ(CCSBAR, (UINT8 *)pBfr + byteCount) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
		return (ERROR);
	    }

	else if (devAddrBytes == I2C_DUAL_BYTE_ADDR)
	    {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr | 0x01) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_READ(CCSBAR, (UINT8 *)pBfr + byteCount) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
		return (ERROR);
	    }

	else if (devAddrBytes == I2C_TRIPLE_BYTE_ADDR)
	    {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE2(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr | 0x01) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_READ(CCSBAR, (UINT8 *)pBfr + byteCount) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
		return (ERROR);
	    }

	else 
	    return (ERROR);
	}

    return (OK);
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
    UINT32 devAddr,	 /* I2C address of target device */
    UINT32 devOffset,	 /* Offset within device where 'read' begins */
    UINT32 devAddrBytes, /* Number of bytes in protocol offset for device */ 
    UINT8  *pBfr,	 /* Pointer to 'write' buffer */
    UINT32 numBytes      /* Number of bytes to read */ 
    )
    {
    UINT32 byteCount;

    /* Zero byte count is considered an error */

    if (numBytes == 0)
	return (ERROR);

    /* Make sure that the device address is valid for writing */

    if (i2cDevAddrWriteValid (devAddr) == FALSE)
	return (ERROR);

    /* Make sure that writing is enabled on this board */

    I2C_WRITE_ENABLE

    (void) I2C_KNOWN_STATE(CCSBAR);

    for (byteCount = 0; byteCount < numBytes; byteCount++)
	{
	if (devAddrBytes == I2C_SINGLE_BYTE_ADDR)
	    {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr))
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, pBfr [byteCount]) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                return (ERROR);
	    }

	else if (devAddrBytes == I2C_DUAL_BYTE_ADDR)
            {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr))
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, pBfr [byteCount]) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                return (ERROR);
            }

	else if (devAddrBytes == I2C_TRIPLE_BYTE_ADDR)
	    {
            if (I2C_CYCLE_START(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, devAddr))
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE2(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                     I2C_OFFSET_BYTE1(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if ((I2C_CYCLE_WRITE (CCSBAR,
                    I2C_OFFSET_BYTE0(devOffset + byteCount))) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_WRITE(CCSBAR, pBfr [byteCount]) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_ACKIN(CCSBAR) == ERROR)
                return (ERROR);
            if (I2C_CYCLE_STOP(CCSBAR) == ERROR)
                return (ERROR);
	    }
        else
	    return (ERROR);

         /*
          * delay for at least 5ms to allow EEPROM to complete
          * the write cycle (internal operation)
          */

         sysI2cUsDelay (5000);
         }

    return (OK);
    }

/*******************************************************************************
*
* sysI2cUsDelay - delay the specified number of microseconds
*
* This routine will delay for at least the specified number of microseconds,
* using the lower 32 bit "word" of the Time Base register as the timer.
* Note this routine assumes a default CCB (Core Complex Bus) speed of 333MHz 
* with timebase  updates every 8 processor cycles.  Slower actual processor 
* speeds than  333MHz will result in delays being proportionately longer 
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
     *       1 second            8 cycles    1000000 millisec      millisec
     */

    if ((ticksToWait = delay * ((DEFAULT_CCB_SPD / 8) / 1000000)) == 0)
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
    UINT32 devAddr	/* Device I2C address which we will check */
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

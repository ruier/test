/*  sysMpc8641I2c.c -  Low Level mpc8641 I2C Routines */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01d,17mar08,mdg Added support for the 8641D.
01c,15nov04,scb Fixed ARG_UNUSED issue.
01b,10nov04,scb Shortened I2C delay.
01a,09nov04,scb Created.
*/

/*
DESCRIPTION
This file contains the support for reading and writing the I2c interface
on the mpc8641.

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
#include "mpc8641.h"
#include "sysMpc8641I2c.h"

/* defines */

#define I2C_FDR_VAL         (0x31)      /* Frequency divider value */
#define I2C_READ08      mpc8641I2cRead08
#define I2C_WRITE08     mpc8641I2cWrite08
#define I2C_DELAY       sysI2cUsDelay(20)
#define I2C_ADR_RES_BITS    (0x01)      /* Reserved bits ... */
#define I2C_FDR_RES_BITS    (0xc0)
#define I2C_CR_RES_BITS     (0x02)
#define I2C_SR_RES_BITS     (0xed)
#define I2C_DR_RES_BITS     (0x00)
#define I2C_DFSRR_RES_BITS  (0xc0)

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

STATUS sys8641I2cStart (UINT32 base);
STATUS sys8641I2cStop (UINT32 base);
STATUS sys8641I2cReadN (UINT32 base, UINT8 * pDataChar, UINT32 numBytes);
STATUS sys8641I2cRead (UINT32 base, UINT8 * pDataChar);
STATUS sys8641I2cWriteN (UINT32 base, UINT8 * pDataChar, UINT32 numBytes);
STATUS sys8641I2cWrite (UINT32 base, UINT8  pDataChar);
STATUS sys8641I2cAckIn (UINT32 base);
STATUS sys8641I2cAckOut (UINT32 base);
STATUS sys8641I2cKnownState (UINT32 base);
LOCAL UINT8 mpc8641I2cRead08 (UINT32 base, UINT32 offset);
LOCAL void  mpc8641I2cWrite08 (UINT32 base, UINT32 offset, UINT8 value);

/* externals */

IMPORT void sysI2cUsDelay (UINT32 delay);

/* locals */

/*******************************************************************************
*
* sys8641I2cStart - perform an I2C start cycle on the I2C bus
*
* This routine performs a start cycle on the I2C bus.
*
* RETURNS: OK, or ERROR if operation unsuccessful.
*/

STATUS sys8641I2cStart
    (
    UINT32 base     /* Base of mpc8641 register set */
    )
    {
    UINT32 timeOutCount;
    UINT8  reg08;

    /* If this is a repeat start, then set the required bits and return */

    reg08 = I2C_READ08 (base, CCSR_I2CCR);
    if ((reg08 & CCSR_I2CCR_MSTA_MASK) != 0)
        {
        reg08 |= (CCSR_I2CCR_RSTA_MASK | CCSR_I2CCR_MSTA_MASK |
                  CCSR_I2CCR_MTX_MASK);
        I2C_WRITE08 (base, CCSR_I2CCR, reg08);

    I2C_DELAY;
    return (OK);
        }

    /*
     * wait until the I2C bus is free.  if it doesn't become free
     * within a *reasonable* amount of time, exit with an error.
     */

    for (timeOutCount = 10; timeOutCount; timeOutCount--)
        {
        I2C_DELAY;

        reg08 = I2C_READ08 (base, CCSR_I2CSR);

        if ((reg08 & CCSR_I2CSR_MBB_MASK) == 0)
            {
            break;
            }
        }

    if (timeOutCount == 0)
        {
        return (ERROR);
        }

    /*
     * since this is the first time through, generate a START(MSTA) and
     * place the I2C interface into a master transmitter mode(MTX).
     */

    reg08 = I2C_READ08 (base, CCSR_I2CCR);
    reg08 |= CCSR_I2CCR_MTX_MASK | CCSR_I2CCR_MSTA_MASK;
    I2C_WRITE08 (base, CCSR_I2CCR, reg08);
    I2C_DELAY;

    return (OK);
    }

/*******************************************************************************
*
* sys8641I2cStop - perform an I2C stop cycle on the I2C bus
*
* This routine performs a stop cycle on the I2C bus.
*
* RETURNS: OK always.
*/

STATUS sys8641I2cStop
    (
    UINT32 base     /* Base of mpc8641 register set */
    )
    {
    UINT8 reg08;

    /*
     * Turn off MSTA bit(which will generate a STOP bus cycle)
     * Turn off MTX bit(which places the mpc8641 interface into receive mode
     * Turn off TXAK bit(which allows 9th clock cycle acknowledges)
     */

    reg08 = I2C_READ08 (base, CCSR_I2CCR);
    reg08 &= ~(CCSR_I2CCR_MTX_MASK | CCSR_I2CCR_MSTA_MASK |
               CCSR_I2CCR_TXAK_MASK);
    I2C_WRITE08 (base, CCSR_I2CCR, reg08);
    I2C_DELAY;

    return (OK);
    }

/*******************************************************************************
*
* sys8641I2cReadN - perform an I2C read cycle of N bytes.
*
* This routine reads N bytes off the I2C bus.
*
* RETURNS: OK, or ERROR if operation unsuccessful.
*/

STATUS sys8641I2cReadN
    (
    UINT32 base,        /* Base of mpc8641 register set */
    UINT8  * pDataChar,         /* Read characters returned here */
    UINT32 numBytes     /* Number of bytes to read */
    )
    {
    UINT8 reg08;
    int i;
    UINT32 byteCount = 0;

    reg08 = I2C_READ08 (base, CCSR_I2CCR);
    reg08 &= ~CCSR_I2CCR_MTX_MASK;

    if (numBytes != 1)      /* if last byte ... */
    reg08 &= ~CCSR_I2CCR_TXAK_MASK;
    else
    reg08 |= CCSR_I2CCR_TXAK_MASK;

    I2C_WRITE08 (base, CCSR_I2CCR, reg08);
    I2C_DELAY;

    /* Do a dummy read.  this latches the data off the bus. */

    reg08 = I2C_READ08 (base, CCSR_I2CDR);

    for (i=0; i<10; i++)    /* Wait for module interrupt */
    {
    I2C_DELAY;
    if ((I2C_READ08(base, CCSR_I2CSR) & CCSR_I2CSR_MCF_MASK) != 0)
        break;
    }

    while (numBytes)
    {
    if (numBytes == 1)  /* If last byte ... */
        {
        reg08 = I2C_READ08 (base, CCSR_I2CCR);
        reg08 &= ~CCSR_I2CCR_MTX_MASK;
        reg08 |= CCSR_I2CCR_TXAK_MASK;
        I2C_WRITE08 (base, CCSR_I2CCR, reg08);
        }
    I2C_DELAY;

    /* Read the next byte */

    reg08 = I2C_READ08 (base, CCSR_I2CDR);
    pDataChar [byteCount] = I2C_READ08 (base, CCSR_I2CDR);

    for (i=0; i<10; i++)    /* Wait for module interrupt */
    {
    I2C_DELAY;
    if ((I2C_READ08(base, CCSR_I2CSR) & CCSR_I2CSR_MCF_MASK) != 0)
        break;
    }

    byteCount++;
    numBytes--;
    }

    return (OK);
    }

/*******************************************************************************
*
* sys8641I2cRead - perform an I2C read cycle on the I2C bus.
*
* This routine performs an I2C read cycle on the I2C bus.
*
* RETURNS: OK, or ERROR if operation unsuccessful.
*/

STATUS sys8641I2cRead
    (
    UINT32 base,        /* Base of mpc8641 register set */
    UINT8  * pDataChar          /* Read character returned here */
    )
    {
    return (sys8641I2cReadN (base, pDataChar, 1));
    }

/*******************************************************************************
*
* sys8641I2cWriteN - perform I2C write cycles of N bytes on I2c bus.
*
* This routine performs "N" I2C write cycles on the I2C bus.
*
* RETURNS: OK, or ERROR if operation unsuccessful.
*/

STATUS sys8641I2cWriteN
    (
    UINT32 base,        /* Base of mpc8641 register set */
    UINT8  * pDataChar,     /* Data to be written */
    UINT32 numBytes     /* Number of bytes to write */
    )
    {
    UINT32 byteCount;
    int i;

    byteCount = 0;

    while (byteCount < numBytes)
    {
    I2C_WRITE08 (base, CCSR_I2CDR, pDataChar[byteCount]);
    for (i=0; i<10; i++)
        {
        I2C_DELAY;

        if ((I2C_READ08(base, CCSR_I2CSR) & CCSR_I2CSR_MCF_MASK) != 0)
        break;

        if (i == 9)
        return (ERROR);
        }
    byteCount++;
    }

    return (OK);
    }

/*******************************************************************************
*
* sys8641I2cWrite - perform an I2C write cycles on I2c bus.
*
* This routine performs an I2C write cycle on the I2C bus.
*
* RETURNS: OK, or ERROR if operation unsuccessful.
*/

STATUS sys8641I2cWrite
    (
    UINT32 base,        /* Base of mpc8641 register set */
    UINT8  pDataChar        /* Data to be written */
    )
    {
    STATUS stat;

    stat = sys8641I2cWriteN (base, &pDataChar, 1);
    return (stat);
    }

/*******************************************************************************
*
* sys8641I2cAckIn - Perform an "acknowledge-in" cycle on the I2C bus
*
* This function performs an "acknowledge-in" cycle on the I2C bus
*
* RETURNS: OK for successful operation, ERROR otherwise.
*/

STATUS sys8641I2cAckIn
    (
    UINT32 base         /* Base of mpc8641 register set */
    )
    {

    UINT32 timeOutCount;
    UINT8  reg08;

    /*
     * Wait until an *internal* device interrupt has been generated, then
     * clear it.  if it is not received, return with an error.
     * we are polling, so NO processor interrupt is generated.
     */

    for (timeOutCount = 10; (timeOutCount > 0); timeOutCount--)
        {
        I2C_DELAY;

        reg08 = I2C_READ08 (base, CCSR_I2CSR);

    if  ((reg08 & CCSR_I2CSR_RXAK_MASK) == 0)
        break;
        }

    if (timeOutCount == 0)
    return (ERROR);
    return (OK);
    }

/*******************************************************************************
*
* sys8641I2cAckOut - Perform an "acknowledge-out" cycle on the I2C bus
*
* This routine simply returns OK.  The mpc8641 does not require this routine,
* it is included here as a hook to the generic I2C interface routines in
* sysMotI2c.c.
*
* RETURNS: OK always.
*/

STATUS sys8641I2cAckOut
    (
    UINT32 base  ARG_UNUSED     /* Base of mpc8641 register set */
    )
    {
    return(OK);
    }

/*******************************************************************************
*
* sys8641I2cKnownState - initialize the I2C bus to a known state
*
* This function initializes the I2C bus to a known state.
*
* RETURNS: Always OK.
*/

STATUS sys8641I2cKnownState
    (
    UINT32 base     /* Base of mpc8641 register set */
    )
    {

    I2C_WRITE08 (base, CCSR_I2CCR, CCSR_I2CCR_MSTA_MASK);
    I2C_DELAY;
    I2C_WRITE08 (base, CCSR_I2CCR, CCSR_I2CCR_MSTA_MASK | CCSR_I2CCR_MEN_MASK);
    I2C_DELAY;

    /*
     * Set clock divisor, set slave address, clear status register
     */

    I2C_WRITE08 (base, CCSR_I2CFDR, I2C_FDR_VAL);
    I2C_WRITE08 (base, CCSR_I2CADR, 0x02);
    I2C_WRITE08 (base, CCSR_I2CSR, 0);

    /*
     * Do some dummy reads to clear the bus in case it is in an odd
     * state.
     */

    (void) I2C_READ08 (base, CCSR_I2CDR);
    (void) I2C_READ08 (base, CCSR_I2CDR);

    /*
     * Reset the I2C module and then enable it
     */

    I2C_WRITE08 (base, CCSR_I2CCR, 0);
    I2C_DELAY;
    I2C_WRITE08 (base, CCSR_I2CCR, CCSR_I2CCR_MEN_MASK);
    I2C_DELAY;

    return (OK);
    }

/*******************************************************************************
*
* mpc8641I2cRead08 - Read one 8-bit value from I2C register
*
* This routine will read one 8-bit value from the specified I2C register.
*
* RETURNS: Byte value read
*/

LOCAL UINT8 mpc8641I2cRead08
    (
    UINT32 base,    /* Base address of register set (CCSBAR) */
    UINT32 offset   /* Register offset within set */
    )
    {
    UINT8 regVal;
    regVal = CCSR_READ08(base, offset);
    return (regVal);
    }

/*******************************************************************************
*
* mpc8641I2cWrite08Res - Write to I2C register, preserve reserved bits.
*
* This routine will write one 8-bit value to the specified I2C register.  It
* will preserve the "reserved" bits, requiring a read of this register before
* computing and writing the new value.
*
* RETURNS: NA
*/

LOCAL void mpc8641I2cWrite08
    (
    UINT32 base,    /* Base address of register set (CCSBAR) */
    UINT32 offset,  /* Register offset within set */
    UINT8  value    /* Value to write to register */
    )
    {
    UINT8 reg08 = 0;
    UINT8 regVal = value;
    UINT8 mask;

    if (offset == CCSR_I2CADR)
    {
    reg08 = I2C_READ08 (base, CCSR_I2CADR);
    mask = I2C_ADR_RES_BITS;
    }
    else if (offset == CCSR_I2CFDR)
    {
    reg08 = I2C_READ08 (base, CCSR_I2CFDR);
    mask = I2C_FDR_RES_BITS;
    }
    else if (offset == CCSR_I2CCR)
    {
    reg08 = I2C_READ08 (base, CCSR_I2CCR);
    mask = I2C_CR_RES_BITS;
    }
    else if (offset == CCSR_I2CSR)
    {
    reg08 = I2C_READ08 (base, CCSR_I2CSR);
    mask = I2C_SR_RES_BITS;
    }
    else if (offset == CCSR_I2CDR)
    {
    reg08 = 0;          /* Don't read data register */
    mask = I2C_DR_RES_BITS;
    }
    else if (offset == CCSR_I2CDFSRR)
    {
    reg08 = I2C_READ08 (base, CCSR_I2CDFSRR);
    mask = I2C_DFSRR_RES_BITS;
    }
    else
    return;                 /* Do nothing if not legal register */

    reg08 &= mask;          /* Zero out nonreserved bits in read image */
    regVal &= ~mask;        /* Zero out reserved bits in value */
    regVal |= reg08;        /* Combine for final value */
    CCSR_WRITE08(base, offset, regVal); /* Write, preserving reserved bits */
    }

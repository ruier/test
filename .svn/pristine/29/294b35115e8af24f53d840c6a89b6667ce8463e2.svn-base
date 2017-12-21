/* eeprom.c - EEPROM NVRAM routines */

/*
 * Copyright (c) 2005, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,16apr07,b_m  reduce eeprom access delay to make bspvts happy.
01a,23jun05,j_b  adapted from wrPpmc8260 (rev 01f)
*/

/*
DESCRIPTION

This file contains some basic routines to access the EEPROM NVRAM device.

INCLUDES FILES: eeprom.h
*/


/* includes */

#include "eeprom.h"

/* defines */

/* globals */

/* locals */

/* forward declarations */

/* externals */

IMPORT void sysMsDelay(UINT);


/***************************************************************************
*
* eepromReadByte - read one byte of non-volatile RAM
*
* This routine reads one byte of non-volatile RAM.
*
* RETURNS: One byte of data.
*
* ERRNO
*
* SEE ALSO: eepromWriteByte()
*/

UINT8 eepromReadByte
    (
    int offset
    )
    {
    return ( *(UINT8 *)((int)NV_RAM_ADRS + ((offset) * NV_RAM_INTRVL)) );
    }

/***************************************************************************
*
* eepromWriteByte - write one byte to non-volatile RAM
*
* This routine writes one byte of data to non-volatile RAM.
*
* RETURNS: OK or ERROR if write fails
*
* ERRNO
*
* SEE ALSO: eepromReadByte(), eepromUnlock(), eepromLock()
*/

STATUS eepromWriteByte
    (
    int   offset,
    UINT8 data
    )
    {
    volatile UINT8 * ptr;
    ULONG  retryCount = RETRY_NVWRITE_COUNT;

    ptr = (UINT8*)((int)NV_RAM_ADRS + ((offset) * NV_RAM_INTRVL));

#ifdef SMART_EEPROM_WRITE  /* skip unnecessary EEPROM writes */

     /* only write if data is changing */
    if ( *ptr != data )
#endif /* SMART_EEPROM_WRITE */
        {
        *ptr = data;
        EIEIO;

        /* ensure data is written */
        do
            {
            sysMsDelay(NV_WR_CYCLE_TIME);
            }
        while ( *ptr != data && --retryCount );

        if (retryCount == 0)
            {
            /* write failed */
            return (ERROR);
            }
        }

    return (OK);
    }


/***************************************************************************
*
* eepromUnlock - Unlock the eeprom via the software protection mechanism
*
* This routine unlock the eeprom via the software protection mechanism as
* described in the Catalyst Semiconductor CAT28LV64 64K-Bit (8Kx8) CMOS
* PARALLEL EEPROM datasheet.
*
* RETURNS: N/A
*
* SEE ALSO: eepromReadByte(), eepromWriteByte(), eepromLock()
*
*/
void eepromUnlock (void)
    {
    volatile UINT8 *ptr = (UINT8*)NV_RAM_ADRS;

    *(ptr+0x1555) = 0xaa;
    *(ptr+0x0aaa) = 0x55;
    *(ptr+0x1555) = 0x80;
    *(ptr+0x1555) = 0xaa;
    *(ptr+0x0aaa) = 0x55;
    *(ptr+0x1555) = 0x20;

    /* wait for write cycle to complete */
    sysMsDelay(NV_WR_CYCLE_TIME * RETRY_NVWRITE_COUNT);

    EIEIO;
    }

/***************************************************************************
*
* eepromLock - Lock the eeprom via the software protection mechanism
*
* This routine lock the eeprom via the software protection mechanism as
* described in the Catalyst Semiconductor CAT28LV64 64K-Bit (8Kx8) CMOS
* PARALLEL EEPROM datasheet.
*
* RETURNS: N/A
*
* SEE ALSO: eepromReadByte(), eepromWriteByte(), eepromUnlock()
*
*/
void eepromLock (void)
    {
#ifdef INCLUDE_EEPROM_LOCKING

    volatile UINT8 *ptr = (UINT8*)NV_RAM_ADRS;

    *(ptr+0x1555) = 0xaa;
    *(ptr+0x0aaa) = 0x55;
    *(ptr+0x1555) = 0xa0;
    EIEIO;

    /* wait for write cycle to complete */
    sysMsDelay(NV_WR_CYCLE_TIME * RETRY_NVWRITE_COUNT);

#endif /* INCLUDE_EEPROM_LOCKING */
    }


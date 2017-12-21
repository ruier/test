/* eeprom.c - EEPROM NVRAM routines */

/* Copyright 1984-2002, 2007 Wind River Systems, Inc. */

/*
modification history
--------------------
01g,30jan07,d_z  Remove the branch that will never take effect in eepromDelay.
01f,25jan02,g_h  Move EEPROM_DELAY macro to config.h becuase it should be
                 defrent for ech board.
01e,18jan02,g_h  Increase delay time
01d,17dec01,g_h  Remove compilation warning.
01c,11dec01,g_h  Move to use DEC trimer instead of SW timer
01b,12oct01,g_h  Cleaned for T2.2
01a,15jun98,est  Created
*/

/*
DESCRIPTION

This file contains some basic routines to access the eeprom NVRAM device.

INCLUDES: eeprom.h
*/


/* includes */

#include "vxWorks.h"
#include "memLib.h"
#include "eeprom.h"

/* defines */

#undef  DEBUG_EEPROM

/* globals */

/* locals */

/* forward declarations */

void eepromLock (void);
void eepromUnlock (void);
void eepromDelay (ULONG delay);

/* externals */

/***************************************************************************    
*
* eepromNvRamSetup - setup non-volatile ram system
*
* This routine setup the Non-Volatile RAM.
*
* RETURNS: N/A
*
* SEE ALSO: eepromReadByte(), eepromWriteByte(), eepromUnlock(), eepromLock()
*/

void  eepromNvRamSetup 
    ( 
    void 
    )
    {

    }

/***************************************************************************    
*
* eepromReadByte - read one byte of Non-Volatile Ram
*
* This routine read one byte of Non-Volatile RAM.
*
* RETURNS: One byte of data.
*
* SEE ALSO: eepromNvRamSetup(), eepromWriteByte(), eepromUnlock(), eepromLock()
*/

UINT8 eepromReadByte
    (
    int offset
    )
    {
#ifdef DEBUG_EEPROM
    volatile UINT8   result;
    volatile UINT8 * addr;

    addr  = (UINT8 *)((int)NV_RAM_ADRS + ((offset) * NV_RAM_INTRVL));
    result = *addr;

    return (result);
#else /* NON-DEBUG */
    return ( *(UINT8 *)((int)NV_RAM_ADRS + ((offset) * NV_RAM_INTRVL)) );
#endif /* DEBUG_EEPROM */
    }

/***************************************************************************    
*
* eepromWriteByte - write one byte to Non-Volatile Ram
*
* This routine write one byte of data to nonvolatile ram.
*
* RETURNS: OK or ERROR if cannot write nvram
*
* SEE ALSO: eepromNvRamSetup(), eepromReadByte(), eepromUnlock(), eepromLock()
*/

STATUS eepromWriteByte
    (
    int   offset,
    UINT8 data
    )
    {
#ifdef DEBUG_EEPROM
    volatile UINT8   valueRead;
    volatile ULONG   retryCount = RETRY_NVWRITE_COUNT;
    volatile UINT8 * addr;

#ifdef SMART_EEPROM_WRITE  /* skip an EEPROM write if already the value */
    if ((valueRead = NV_RAM_READ( offset )) == data)  return(OK);
#endif /* SMART_EEPROM_WRITE */
	  			  
    eepromUnlock ();
	
    /* write the data value */
    addr = (UINT8 *)((int)NV_RAM_ADRS + ((offset) * NV_RAM_INTRVL));
    *addr = data;

    /* loop read until is sticks */
    do
        {
 	valueRead = NV_RAM_READ( offset );	
	eepromDelay (EEPROM_DELAY);
	retryCount--;
	}
    while ((valueRead != data) && retryCount );

    eepromLock ();

#else    /* NON-DEBUG */
    ULONG  retryCount = RETRY_NVWRITE_COUNT;

#ifdef SMART_EEPROM_WRITE  /* skip an EEPROM write if already the value */
    volatile UINT8  valueRead;

    if ((valueRead = NV_RAM_READ( offset )) == data)  return(OK);
#endif /* SMART_EEPROM_WRITE */

    eepromUnlock ();

    /* write the data value */
    *(UINT8 *)((int)NV_RAM_ADRS + ((offset) * NV_RAM_INTRVL)) = data;

    /* loop read until is sticks */
    do
	{
	eepromDelay (EEPROM_DELAY);
	}
    while ((NV_RAM_READ( offset) != data) && retryCount-- );

    eepromLock();

#endif  /* DEBUG_EEPROM */
    return (OK);
    }

/***************************************************************************    
*
* eepromUnlock - Unlock the eeprom via the software protection mechanism
*
* This routine unlock the eeprom via the software protection mechanism.
*
* RETURNS: N/A
*
* SEE ALSO: eepromNvRamSetup(), eepromReadByte(), eepromWriteByte(), eepromLock()
*/
void eepromUnlock
    (
     void 
    )
    {
#ifdef DEBUG_EEPROM
    return;
#endif /* DEBUG_EEPROM */

    eepromDelay (EEPROM_DELAY);
	
    *(UINT8 *)(NV_RAM_ADRS + 0x1555) = 0xAA;
    *(UINT8 *)(NV_RAM_ADRS + 0x0AAA) = 0x55;
    *(UINT8 *)(NV_RAM_ADRS + 0x1555) = 0x80;
    *(UINT8 *)(NV_RAM_ADRS + 0x1555) = 0xAA;
    *(UINT8 *)(NV_RAM_ADRS + 0x0AAA) = 0x55;
    *(UINT8 *)(NV_RAM_ADRS + 0x1555) = 0x20;
	
    eepromDelay (EEPROM_DELAY);
    }

/***************************************************************************    
*
* eepromLock - Lock the eeprom via the software protection mechanism
*
* This routine lock the eeprom via the software protection mechanism.
*
* RETURNS: N/A
*
* SEE ALSO: eepromNvRamSetup(), eepromReadByte(), eepromWriteByte(), eepromUnlock()
*/
void eepromLock
    (
     void 
    )
    {
#ifdef DEBUG_EEPROM
    return;
#endif /* DEBUG_EEPROM */

#ifdef INCLUDE_EEPROM_LOCKING

    eepromDelay (EEPROM_DELAY);

    *(UINT8 *)(NV_RAM_ADRS + 0x1555) = 0xAA;
    *(UINT8 *)(NV_RAM_ADRS + 0x0AAA) = 0x55;
    *(UINT8 *)(NV_RAM_ADRS + 0x1555) = 0xA0;
    
    eepromDelay (EEPROM_DELAY);

#endif /* DEBUG_EEPROM */
    }

/***************************************************************************    
*
* eepromDelay -  eeprom delay routine
*
* This routine is the eeprom delay routine.
*
* RETURNS: N/A
*/

void eepromDelay 
    ( 
    ULONG delay 
    )
    {

    register UINT oldval;     /* decrementer value */
    register UINT newval;     /* decrementer value */
    register UINT totalDelta; /* Dec. delta for entire delay period */
    register UINT decElapsed; /* cumulative decrementer ticks */

    /* Calculate delta of decrementer ticks for desired elapsed time.  */

    totalDelta = ((DEC_CLOCK_FREQ / DEC_CLK_TO_INC) / 1000) * delay;

    /*
     * Now keep grabbing decrementer value and incrementing "decElapsed" until
     * we hit the desired delay value.  Compensate for the fact that we may
     * read the decrementer at 0xffffffff before the interrupt service
     * routine has a chance to set in the rollover value.
     */

    decElapsed = 0;

    oldval = vxDecGet ();

    while (decElapsed < totalDelta)
        {
        newval = vxDecGet ();

        if ((abs((int)oldval - (int)newval)) < 1000 )
            decElapsed += abs((int)oldval - (int)newval);  /* no rollover */
        else
            if (newval > oldval)
                decElapsed += abs((int)oldval);  /* rollover */

        oldval = newval;
        }
    }


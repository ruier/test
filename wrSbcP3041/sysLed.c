/* sysLed.c - Wind River SBC P4080 User LED driver */

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
01b,29nov10,e_d  Code clean
01a,22feb10,kds  Created
*/

/*
DESCRIPTION
This module contains the LED driver.

INCLUDE FILES: sysLed.h
*/

/* includes */

#include "sysLed.h"

/* locals */

/* start with all segments off */

static UINT8    keepLED = LED_ALLOFF;

/*******************************************************************************
*
* sysLedInit - Initialize LEDs
*
* This routine initializes the LED variable to zero and clears
* all LEDs.
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysLedInit (void)
    {

    /* turn off the status LED */

    (void)sysLedSet (keepLED, keepLED);

    }

/*******************************************************************************
*
* sysLedSet - set and clear segments of the status LED
*
* This routine sets and clears segments of the status LED on the board.
* The <mask> parameter specifies which bits will be changed,
* and <value> specifies the value to which they will be set.
*
* RETURNS: The previous value of the LED bits.
*
* ERRNO
*/

UINT8 sysLedSet
    (
    UINT8       mask,         /* selects which bits will be changed    */
    UINT8       value         /* holds the new value for selected bits */
    )
    {
    UINT32      retValue    = keepLED;

    keepLED = ((keepLED & ~ mask) | (value & mask));
    *((volatile UINT8 *)(USER_LED_ADRS)) = keepLED;

    return (retValue);
    }

/*******************************************************************************
*
* sysLedsAllFlash - flash all of the user leds
*
* This routine will flash the user leds twice for about 1/4 second each time
*
* SEE ALSO:
*
* RETURNS: NA
*
* ERRNO
*/

void sysLedsAllFlash(void)
    {

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_ALLON;

    taskDelay(15);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_ALLOFF;

    taskDelay(15);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_ALLON;

    taskDelay(15);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_ALLOFF;

    }

/*******************************************************************************
*
* sysLedsCascade - cascade the user leds
*
* This routine will cascase the user leds
*
* SEE ALSO:
*
* RETURNS: NA
*
* ERRNO
*/

void sysLedsCascade(void)
    {

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_ALLOFF;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_4;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_5;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_6;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_7;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_3;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_2;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_1;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_0;

    sysMsDelay(10);

    *((volatile UINT8 *)(USER_LED_ADRS)) = LED_ALLOFF;

    }


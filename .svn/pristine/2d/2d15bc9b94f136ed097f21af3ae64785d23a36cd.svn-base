/* sysLed.c - Wind River SBC8548 User LED driver */

/*
 * Copyright (c) 2005-2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,13feb06,kds  cleaned up code
01a,11aug05,kds  adapted for wrSbc8548 from wrSbc834x (rev 01a)
*/

/*
DESCRIPTION
This module contains the LED driver.

INCLUDE FILES: sysLed.h
*/

/* includes */
#include "sysLed.h"

#ifdef INCLUDE_SYSLED

/* locals */

/* start with all segments off */
static UINT8    keepLED	= (LED_DIGIT | LED_POINT);

/***********************************************************************
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


/******************************************************************************
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
* sysLedHex - display a hexadecimal digit on the status LED
*
* This routine displays a hexadecimal digit on the status LED.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysLedHex
    (
    UINT8	value		/* value to display (0-15) */
    )
    {
    static UINT8 sysLEDPat [16] =
        {
        LED_0, LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7,
        LED_8, LED_9, LED_A, LED_B, LED_C, LED_D, LED_E, LED_F
        };

    (void) sysLedSet (LED_DIGIT, sysLEDPat [value % 16]);
    }


/**************************************************************************
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

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0x00;

    taskDelay(sysClkRateGet()/4);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xFF;

    taskDelay(sysClkRateGet()/4);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0x00;

    taskDelay(sysClkRateGet()/4);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xFF;

    }

/**************************************************************************
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

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xFF;

    sysMsDelay(20);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0x7F;

    sysMsDelay(20);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xBF;

    sysMsDelay(20);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xDF;

    sysMsDelay(20);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xEF;

    sysMsDelay(20);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xF7;

    sysMsDelay(20);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xFB;

    sysMsDelay(20);

    *((volatile UINT8 *)(USER_LED_ADRS)) = 0xFF;

    }


/**************************************************************************
*
* sysDemoLedsCascade - cascade the demo leds
*
* This routine will cascade the demo leds
*
* SEE ALSO:
*
* RETURNS: NA
*
* ERRNO
*/

void sysDemoLedsCascade(void)
     {
     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x80;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x8;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x40;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x20;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x1;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x10;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x4;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x2;

     sysMsDelay(50);

     *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x0;

     }

/**************************************************************************
*
* sysDemoLedsAllFlash - flash all of the demo leds
*
* This routine will flash the demo leds on and off
*
* SEE ALSO:
*
* RETURNS: NA
*
* ERRNO
*/

void sysDemoLedsAllFlash(void)
     {

	 *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0xFF;

	 sysMsDelay(250);

	 *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x0;

	 sysMsDelay(250);

	 *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0xFF;

	 sysMsDelay(250);

	 *((volatile UINT8 *)(DEMO_LED_ADRS)) = 0x0;

	 }

#endif /* INCLUDE_SYSLED */



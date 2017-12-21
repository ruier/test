/* sysLed.c - Wind River SBC PowerQUICCII User LED driver */

/*
 * Copyright (c) 2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,23jun05,j_b  adapted from wrSbcPowerQuiccII (rev 01c)
*/

/*
DESCRIPTION
This module contains the LED driver.

INCLUDES: sysLed.h
*/

/* includes */
#include "sysLed.h"

#ifdef INCLUDE_SYSLED

/* locals */

/* start with all segments off */
static UINT8    keepLED	= (LED_DIGIT | LED_POINT);

/***********************************************************************
*
* sysLedInit - Initialize LEDs.
*
* This routine initializes the LED variable to zero and clears
* all LEDs.
*
* RETURNS: N/A.
*
* ERRNO
*
* SEE ALSO: sysLedOn(), sysLedOff(), sysLedControl().
*/

void sysLedInit
    (
    void
    )
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
    *(M83XX_GP1DAT(CCSBAR)) = (keepLED << 20);

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
*/

void sysLedsAllFlash(void)
    {

    *M83XX_GP1DAT(CCSBAR) = (0x00 << 20);

    taskDelay(15);

    *M83XX_GP1DAT(CCSBAR) = (0xFF << 20);

    taskDelay(15);

    *M83XX_GP1DAT(CCSBAR) = (0x00 << 20);

    taskDelay(15);

    *M83XX_GP1DAT(CCSBAR) = (0xFF << 20);

    }

/**************************************************************************
*
* sysLedsCascade - cascade the user leds
*
*
* SEE ALSO:
*
* RETURNS: NA
*/

void sysLedsCascade(void)
    {

    *M83XX_GP1DAT(CCSBAR) = (0xFF << 20);

    sysMsDelay(10);

    *M83XX_GP1DAT(CCSBAR) = (0x7F << 20);

    sysMsDelay(10);

    *M83XX_GP1DAT(CCSBAR) = (0xBF << 20);

    sysMsDelay(10);

    *M83XX_GP1DAT(CCSBAR) = (0xDF << 20);

    sysMsDelay(10);

    *M83XX_GP1DAT(CCSBAR) = (0xEF << 20);

    sysMsDelay(10);

    *M83XX_GP1DAT(CCSBAR) = (0xF7 << 20);

    sysMsDelay(10);

    *M83XX_GP1DAT(CCSBAR) = (0xFB << 20);

    sysMsDelay(10);

    *M83XX_GP1DAT(CCSBAR) = (0xFF << 20);

    }

#endif /* INCLUDE_SYSLED */



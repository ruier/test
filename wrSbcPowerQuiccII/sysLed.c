/* sysLed.c - Wind River SBC PowerQUICCII User LED driver */

/* Copyright 1984-2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01c,24sep04,mdo  Documentation fixes for apigen
01b,15apr03,j_b  use defines for hex LED values
01a,01apr03,j_b  adapted from unsupported idp0x0 BSPs
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
    UINT8	    retValue	= keepLED;

    *((volatile UINT8 *)(BSCR_LED_REGISTER)) = keepLED = \
		((keepLED & ~ mask) | (value & mask));

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


#endif /* INCLUDE_SYSLED */



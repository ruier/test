/* sysLcd.c - Wind River SBC8641D LCD driver */

/*
 * Copyright (c) 2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,31aug07,wap  Fix compiler warning (WIND00103172)
01a,10nov06,kds  Created.
*/

/*
DESCRIPTION
This module contains the LCD driver for the Wind River SBC8641D.

INCLUDE FILES: sysLcd.h
*/

/* includes */
#include "sysLcd.h"

/* locals */

#ifdef INCLUDE_LCD

/***********************************************************************
*
* sysLcdInit - Initialize the LCD
*
* This routine initializes the LCD by adjusting the contrast and
* configuring the LCD.
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysLcdInit (void)
    {

    /* Function Set */

    sysLcdWriteInstr(LCD_DL_8BIT | LCD_DUAL_LINE | LCD_FONT_5x8);
    sysDelay();

    /* Display Mode */

    sysLcdWriteInstr(LCD_DISPL_ON);
    sysDelay();

    /* Clear the LCD */

    sysLcdClearDisplay();
    sysDelay();

    /* Entry Mode Set */

    sysLcdWriteInstr(LCD_INCREMENT);
    sysDelay();

    }

/***********************************************************************
*
* sysLcdWriteInstr - Write instruction to the LCD
*
* This routine performs the write instruction to the LCD
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysLcdWriteInstr
    (
    UINT8 instr
    )
    {
    LCD_WRITE_INSTR (instr);
    }

/***********************************************************************
*
* sysLcdWriteData - Write data to the LCD
*
* This routine writes data to the LCD
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysLcdWriteData
    (
    UINT8 data
    )
    {
    LCD_WRITE_DATA (data);
    }

/***********************************************************************
*
* sysLcdClearDisplay - Clear the LCD
*
* This routine clears the display
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysLcdClearDisplay (void)
    {
    sysLcdWriteInstr(LCD_DISPL_CLEAR);
    }

/***********************************************************************
*
* sysLcdContrastUp - Increase the contrast on the LCD
*
* This routine increases the contrast on the LCD
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysLcdContrastUp (void)
    {
    LCD_CONTRAST_UP;
    }

/***********************************************************************
*
* sysLcdContrastDown - Decrease the contrast on the LCD
*
* This routine decreases the contrast on the LCD
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysLcdContrastDown (void)
    {
    LCD_CONTRAST_DWN;
    }


/******************************************************************************
*
* sysLcdSbcSet - display "Wind River SBC8641D" on LCD
*
* This routine displays "Wind River" on first line of the LCD and
* "SBC8641D" on the second line.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysLcdSbcSet (void)
    {

    UINT8 z;

    /* "WIND RIVER" - centered */

    for (z = 0; z < 3; ++z)
        {
	sysLcdWriteData(LCD_SPACE);
        sysDelay();
        }

    sysLcdWriteData(LCD_W);
    sysDelay();

    sysLcdWriteData(LCD_I);
    sysDelay();

    sysLcdWriteData(LCD_N);
    sysDelay();

    sysLcdWriteData(LCD_D);
    sysDelay();

    sysLcdWriteData(LCD_SPACE);
    sysDelay();

    sysLcdWriteData(LCD_R);
    sysDelay();

    sysLcdWriteData(LCD_I);
    sysDelay();

    sysLcdWriteData(LCD_V);
    sysDelay();

    sysLcdWriteData(LCD_E);
    sysDelay();

    sysLcdWriteData(LCD_R);
    sysDelay();

    /* next line */

    sysLcdWriteInstr(0xc0);
    sysDelay();

    /* "SBC8641D" - centered */

    for (z = 0; z < 4; ++z)
        {
	sysLcdWriteData(LCD_SPACE);
        sysDelay();
        }

    sysLcdWriteData(LCD_S);
    sysDelay();

    sysLcdWriteData(LCD_B);
    sysDelay();

    sysLcdWriteData(LCD_C);
    sysDelay();

    sysLcdWriteData(LCD_8);
    sysDelay();

    sysLcdWriteData(LCD_6);
    sysDelay();

    sysLcdWriteData(LCD_4);
    sysDelay();

    sysLcdWriteData(LCD_1);
    sysDelay();

    sysLcdWriteData(LCD_D);
    sysDelay();

    }

/*******************************************************************************
*
* sysLcdPrint - print a string of characters on the LCD display
*
* This function prints the string <str> on the LCD display, starting
* at cursor position 0. Any text previous displayed on the screen is erased.
* Up to 32 characters can be displayed, split over 2 lines. The only special
* character handled by this function is the linefeed, which will force a
* break to the second line. The supplied string must be NUL terminated.
*
* Note that the sysLcdInit() routine must be called to initialize the
* display before this routine can be used.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysLcdPrint
    (
    char * str
    )
    {
    int i;
    BOOL didBreak = FALSE;
    char * p;

    /* Clearing the display is slow; extra delay is required. */

    sysLcdClearDisplay ();
    sysDelay ();
    sysDelay ();
    sysDelay ();

    p = str;
    for (i = 0; i < 32; i++)
        {
        if (!*p)
            break;
        /*
         * If there are more than 16 characters or we encounter
         * a newline character, move to the next display line.
         */

        if (didBreak == FALSE && (i == 16 || *p == '\n'))
            {
            sysLcdWriteInstr (0xC0);
            sysDelay ();
            didBreak = TRUE;
            }

        if (*p != '\n')
            {
            sysLcdWriteData ((UINT8)*p);
            sysDelay ();
            }

        p++;
        }

    return;
    }

#endif /* INCLUDE_LCD */

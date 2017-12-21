/* m41t00Rtc.c - M41T00 RTC Functions */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01a,05feb09,x_f  initial creation, based on mcpn805/m48t37.c 
*/

/*
DESCRIPTION
This file contains the configuration parameters and control functions for 
the m41t00 real-time clock. It also supports m41t62 real-time clock but with
the basic get and set timer functions.
*/

/* includes */

#include "m41t00Rtc.h"
#include "sysMotI2c.h"

/* forward declarations */

STATUS       m41t00RtcInit         (void);
LOCAL STATUS m41t00RtcDateTest     (RTC_DATE_TIME * rtcTime);
STATUS       m41t00RtcGet          (RTC_DATE_TIME * rtcTime);
STATUS       m41t00RtcSet          (RTC_DATE_TIME * rtcTime);
STATUS       m41t00RtcShow         (void);
void         m41t00RtcDateTimeHook (DOS_DATE_TIME * pDateTime);

extern unsigned char m41t00I2cRead
    (
    unsigned char deviceAddr,
    unsigned char reg
    );

extern void m41t00I2cWrite
    (
    unsigned char deviceAddr,
    unsigned char reg,
    unsigned char val
    );

/* local */

LOCAL RTC_DATE_TIME * pstRTC;

/*******************************************************************************
*
* m41t00RtcRead - read data from I2C
* 
* This routine read data from I2C.
*
* RETURNS: read data
*/

unsigned char m41t00RtcRead
    (
    unsigned char reg
    )
    {
    return (m41t00I2cRead (I2C_ADDR_M41T00, reg));
    }

/*******************************************************************************
*
* m41t00RtcWrite - write data to I2C
* 
* This routine write data to I2C.
*
* RETURNS: N/A
*/

void m41t00RtcWrite
    (
    unsigned char reg,
    unsigned char val
    )
    {
    m41t00I2cWrite (I2C_ADDR_M41T00, reg, val);
    }

/*******************************************************************************
*
* m41t00RtcInit - init data structure
* 
* This routine init the RTC_DATE_TIME data structure.
*
* RETURNS: OK, or ERROR if malloc fail
*/

STATUS m41t00RtcInit (void)
    {
    pstRTC = (RTC_DATE_TIME *) malloc (sizeof (RTC_DATE_TIME));

    if (NULL == pstRTC)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* m41t00RtcDateTest - test whether date/time values are valid
* 
* This routine tests the validity of the values in the RTC_DATE_TIME
* structure.
*
* RETURNS: OK, or ERROR if any values are invalid
*/

LOCAL STATUS m41t00RtcDateTest
    (
    RTC_DATE_TIME * rtcTime    /* pointer to time keeping structure */
    )
    {

    /* Check validity of seconds value */

    if (rtcTime->second > 59)
        {
        return (ERROR);
        }

    /* Check validity of minutes value */

    if (rtcTime->minute > 59)
        {
        return (ERROR);
        }

    /* Check validity of hours value */

    if (rtcTime->hour > 23)
        {
        return (ERROR);
        }

    /* Check validity of day of week value */

    if (rtcTime->dayOfWeek < 1 || rtcTime->dayOfWeek > 7)
        {
        return (ERROR);
        }

    /* Check validity of day of month value */

    if (rtcTime->dayOfMonth < 1 || rtcTime->dayOfMonth > 31)
        {
        return (ERROR);
        }

    /* Check validity of month value */

    if (rtcTime->month < 1 || rtcTime->month > 12)
        {
        return (ERROR);
        }

    /* Check validity of year value */

    if ((rtcTime->year > 2099) || (rtcTime->year < 2000))
        {
        return (ERROR);
        }

    return (OK); 
    }

/*******************************************************************************
*
* m41t00RtcGet - get current RTC date/time. 
*
* This routine allows the caller to obtain the current RTC time and date.
* The caller must allocate space for an RTC_DATE_TIME structure, then call
* this routine.  
*
* RETURNS: OK, or ERROR if unable to retrieve the current RTC date and time. 
*/

STATUS m41t00RtcGet
    (
    RTC_DATE_TIME * rtcTime      /* pointer to time keeping structure */
    )
    {

    rtcTime->second = BCD_TO_BIN (m41t00RtcRead (M41T_SEC_REG) & 0x7F);

    rtcTime->minute = BCD_TO_BIN (m41t00RtcRead (M41T_MIN_REG) & 0x7F);

    rtcTime->hour = BCD_TO_BIN (m41t00RtcRead (M41T_HOUR_REG) & 0x3F);

    rtcTime->dayOfMonth = BCD_TO_BIN (m41t00RtcRead (M41T_DATE_REG) & 0x3F);

    rtcTime->dayOfWeek  = BCD_TO_BIN ((m41t00RtcRead (M41T_DAY_REG) - 1) & 0x07);

    rtcTime->month = BCD_TO_BIN (m41t00RtcRead (M41T_MONTH_REG) & 0x1F);

    rtcTime->year = BCD_TO_BIN (m41t00RtcRead (M41T_YEAR_REG)) + 2000;

    return (OK);
    }

/*******************************************************************************
*
* m41t00RtcSet - set the RTC's date/time per caller's values.
*
* This routine allows the caller to set the RTC time and date.  The caller
* must allocate space for an RTC_DATE_TIME structure, fill the structure
* with the desired time and date, and call this routine.
*
* RETURNS: OK, or ERROR if date/time values are invalid.
*/

STATUS m41t00RtcSet
    (
    RTC_DATE_TIME * rtcTime   /* pointer to time keeping structure */
    )
    {

    /* Determine whether date/time values are valid */

    if (m41t00RtcDateTest (rtcTime) == ERROR)
        {
        logMsg ("date/time values are invalid", 1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    m41t00RtcWrite (M41T_SEC_REG, BIN_TO_BCD (rtcTime->second));

    m41t00RtcWrite (M41T_MIN_REG, BIN_TO_BCD (rtcTime->minute));

    m41t00RtcWrite (M41T_HOUR_REG, BIN_TO_BCD (rtcTime->hour));

    m41t00RtcWrite (M41T_DATE_REG, BIN_TO_BCD (rtcTime->dayOfMonth));

    m41t00RtcWrite (M41T_DAY_REG, BIN_TO_BCD (rtcTime->dayOfWeek + 1));

    m41t00RtcWrite (M41T_MONTH_REG, BIN_TO_BCD (rtcTime->month));

    m41t00RtcWrite (M41T_YEAR_REG, BIN_TO_BCD (rtcTime->year % 100));

    return (OK);
    }

#ifdef INCLUDE_SHOW_ROUTINES
/*******************************************************************************
*
* m41t00RtcShow - show the date/time on the user's display.
*
* This routine retrieves the current RTC date and time and sends it in a 
* user-readable fashion to the user's display.
*
* RETURNS: OK, or ERROR if unable to retrieve or print the current RTC
* date and time. 
*/

STATUS m41t00RtcShow (void)
    {
    RTC_DATE_TIME * rtcTime = pstRTC;    /* RTC date and time */

    /* Retrieve the current RTC date and time */

    if (m41t00RtcGet(rtcTime) == ERROR)
       {
       return (ERROR);
       }

    /* Send the date and time to the user's display */

    printf ("\n\r            Time: %d:%d:%d  Date: %d/%d/%d\n\r",
            rtcTime->hour, rtcTime->minute, rtcTime->second,
            rtcTime->month, rtcTime->dayOfMonth, rtcTime->year);

    return (OK);
    }
#endif /* INCLUDE_SHOW_ROUTINES */

/*******************************************************************************
*
* m41t00RtcDateTimeHook - hook routine for the dosFsLib.
*
* This routine fills in a DOS_DATE_TIME structure with the current date
* and time.  This is used by the standard VxWorks dosFsLib as a way to update 
* the date/time values used for file timestamps.  This is "hooked" into the
* dosFsLib during board initialization if both the dosFsLib and the RTC support
* is included in the BSP.
*
* RETURNS: void.
*
* SEE ALSO: dosFsDateTimeInstall(), and the dosFsLib documentation.
*/

void m41t00RtcDateTimeHook
    (
    DOS_DATE_TIME * pDateTime    /* pointer to time keeping structure */
    )
    {
    RTC_DATE_TIME * rtcTime = pstRTC;       /* RTC date and time */

    m41t00RtcGet(rtcTime);

    /* Populate DOS_DATE_TIME structure within current RTC time and date */

    pDateTime->dosdt_year   = rtcTime->year;
    pDateTime->dosdt_month  = rtcTime->month;
    pDateTime->dosdt_day    = rtcTime->dayOfMonth;
    pDateTime->dosdt_hour   = rtcTime->hour;
    pDateTime->dosdt_minute = rtcTime->minute;
    pDateTime->dosdt_second = rtcTime->second; 
    }

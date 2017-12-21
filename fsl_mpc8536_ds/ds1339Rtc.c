/* ds1339Rtc.c - ds1339 RTC Functions */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,10feb09,b_m  make pstRTC LOCAL.
01b,28mar08,z_l  modified for code convention. fixed "make man" error.
01a,26jun07,l_g  initial creation, based on mcpn805/m48t37.c 
*/

/*
DESCRIPTION
DS1339 RTC Functions
*/

/* includes */

#include "ds1339Rtc.h"

/* forward declarations */

STATUS       ds1339RtcInit         (void);
STATUS       ds1339RtcSet          (RTC_DATE_TIME * rtc_time);
STATUS       ds1339RtcShow         (void);
STATUS       ds1339RtcGet          (RTC_DATE_TIME * rtc_time);
void         ds1339RtcDateTimeHook (DOS_DATE_TIME * pDateTime);
LOCAL STATUS ds1339RtcDateTest     (RTC_DATE_TIME * rtc_time);

extern unsigned char ds1339I2cRead
    (
    unsigned char deviceadd,
    unsigned char reg
    );

extern void ds1339I2cWrite
    (
    unsigned char deviceadd,
    unsigned char reg,
    unsigned char val
    );

/* global */

LOCAL RTC_DATE_TIME *pstRTC;

/*******************************************************************************
*
* ds1339RtcInit - init data structure
* 
* This routine init the RTC_DATE_TIME data structure.
*
* RETURNS: OK, or ERROR if malloc fail
*/

STATUS ds1339RtcInit(void)
    {
    pstRTC = (RTC_DATE_TIME *) malloc (sizeof (RTC_DATE_TIME));

    if (NULL == pstRTC)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* ds1339RtcRead - read data from I2C
* 
* This routine read data from I2C.
*
* RETURNS: read data
*/

unsigned char ds1339RtcRead 
    (
    unsigned char reg
    )
    {
    return (ds1339I2cRead (I2C_RTC_ADDR, reg));
    }

/*******************************************************************************
*
* ds1339RtcWrite - write data to I2C
* 
* This routine write data to I2C.
*
* RETURNS: N/A
*/

void ds1339RtcWrite 
    (
    unsigned char reg,
    unsigned char val
    )
    {
    ds1339I2cWrite (I2C_RTC_ADDR, reg, val);
    }

/*******************************************************************************
*
* ds1339RtcDateTest - test whether date/time values are valid
* 
* This routine tests the validity of the values in the RTC_DATE_TIME
* structure.
*
* RETURNS: OK, or ERROR if any values are invalid
*/

LOCAL STATUS ds1339RtcDateTest
    (
    RTC_DATE_TIME * rtc_time    /* pointer to time keeping structure */
    )
    {
    /* Check validity of seconds value */

    if (rtc_time->second > 59)
        {
        return (ERROR);
        }
    
    /* Check validity of minutes value */

    if (rtc_time->minute > 59)
        {
        return (ERROR);
        }

    /* Check validity of hours value */

    if (rtc_time->hour > 23)
        {
        return (ERROR);
        }

    /* Check validity of day of week value */

    if (rtc_time->day_of_week < 1 || rtc_time->day_of_week > 7)
        {
        return (ERROR);
        }

    /* Check validity of day of month value */

    if (rtc_time->day_of_month < 1 || rtc_time->day_of_month > 31)
        {
        return (ERROR);
        }

    /* Check validity of month value */

    if (rtc_time->month < 1 || rtc_time->month > 12)
        {
        return (ERROR);
        }

    /* Check validity of year value */

    if (rtc_time->year > 2099)
        {
        return (ERROR);
        }

    return (OK); 
    }

/*******************************************************************************
*
* ds1339RtcSet - set the RTC's date/time per caller's values.
*
* This routine allows the caller to set the RTC time and date.  The caller
* must allocate space for an RTC_DATE_TIME structure, fill the structure
* with the desired time and date, and call this routine.
*
* RETURNS: OK, or ERROR if date/time values are invalid.
*/

STATUS ds1339RtcSet
    (
    RTC_DATE_TIME * rtc_time   /* pointer to time keeping structure */
    )
    {
    unsigned char century;
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char monthday;
    unsigned char weekday;
    unsigned char monCent;
    unsigned char year;
    
    /* Determine whether date/time values are valid */

    if (ds1339RtcDateTest (rtc_time) == ERROR)
        {
        return (ERROR);
        }
    
    second = BIN_TO_BCD (rtc_time->second);
    ds1339RtcWrite (DS1339_SEC_REG, second);
    
    minute = BIN_TO_BCD (rtc_time->minute);
    ds1339RtcWrite (DS1339_MIN_REG, minute);
    
    hour = BIN_TO_BCD (rtc_time->hour);
    ds1339RtcWrite (DS1339_HOUR_REG, hour);
    
    monthday = BIN_TO_BCD (rtc_time->day_of_month);
    ds1339RtcWrite (DS1339_DATE_REG, monthday);

    weekday = BIN_TO_BCD (rtc_time->day_of_week + 1);
    ds1339RtcWrite (DS1339_DAY_REG, weekday);

    if (rtc_time->year >= 2000)
        century = 0x80;
    else
        century = 0x0;

    monCent = (BIN_TO_BCD (rtc_time->month) | century);
    ds1339RtcWrite (DS1339_MONTH_REG, monCent);

    year = BIN_TO_BCD (rtc_time->year % 100);
    ds1339RtcWrite (DS1339_YEAR_REG, year);

    return (OK);
    }

/*******************************************************************************
*
* ds1339RtcGet - get current RTC date/time. 
*
* This routine allows the caller to obtain the current RTC time and date.
* The caller must allocate space for an RTC_DATE_TIME structure, then call
* this routine.  
*
* RETURNS: OK, or ERROR if unable to retrieve the current RTC date and time. 
*/

STATUS ds1339RtcGet
    (
    RTC_DATE_TIME * rtc_time      /* pointer to time keeping structure */
    )
    {
    unsigned long second;
    unsigned long minute;
    unsigned long hour;
    unsigned long monthday;
    unsigned long weekday;
    unsigned long monCent;
    unsigned long year;
    
    second = BCD_TO_BIN (ds1339RtcRead (DS1339_SEC_REG) & 0x7F);
    rtc_time->second = second;

    minute = BCD_TO_BIN (ds1339RtcRead (DS1339_MIN_REG) & 0x7F);
    rtc_time->minute = minute;

    hour = BCD_TO_BIN (ds1339RtcRead (DS1339_HOUR_REG) & 0x3F);
    rtc_time->hour = hour;

    monthday = BCD_TO_BIN (ds1339RtcRead (DS1339_DATE_REG) & 0x3F);
    rtc_time->day_of_month = monthday;

    weekday = BCD_TO_BIN ((ds1339RtcRead (DS1339_DAY_REG) - 1) & 0x07);
    rtc_time->day_of_week  = weekday;

    monCent= BCD_TO_BIN (ds1339RtcRead (DS1339_MONTH_REG) & 0x1F);
    rtc_time->month = monCent;

    if (ds1339RtcRead (DS1339_MONTH_REG) & 0x80)
        year = BCD_TO_BIN (ds1339RtcRead (DS1339_YEAR_REG)) + 2000;
    else
        year = BCD_TO_BIN (ds1339RtcRead (DS1339_YEAR_REG)) + 1900;

    rtc_time->year = year;

    return (OK);
    }

/*******************************************************************************
*
* ds1339RtcShow - show the date/time on the user's display.
*
* This routine retrieves the current RTC date and time and sends it in a 
* user-readable fashion to the user's display.
*
* RETURNS: OK, or ERROR if unable to retrieve or print the current RTC
* date and time. 
*/

STATUS ds1339RtcShow (void)
    {
    RTC_DATE_TIME * rtc_time = pstRTC;    /* RTC date and time */

    /* Retrieve the current RTC date and time */

    if (ds1339RtcGet(rtc_time) == ERROR)
       {
       return (ERROR);
       }

    /* Send the date and time to the user's display */

    printf ("\n\r            Time: %d:%d:%d  Date: %d/%d/%d\n\r",
            rtc_time->hour, rtc_time->minute, rtc_time->second,
            rtc_time->month, rtc_time->day_of_month, rtc_time->year);

    return (OK);
    }

/*******************************************************************************
*
* ds1339RtcDateTimeHook - hook routine for the dosFsLib.
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

void ds1339RtcDateTimeHook
    (
    DOS_DATE_TIME * pDateTime    /* pointer to time keeping structure */
    )
    {
    RTC_DATE_TIME * rtc_time = pstRTC;       /* RTC date and time */
    int dos_year             = 0;            /* Current year */
 
    ds1339RtcGet(rtc_time);

    dos_year = rtc_time->year;
    
    /* Populate DOS_DATE_TIME structure within current RTC time and date */
    
    pDateTime->dosdt_year   = dos_year;
    pDateTime->dosdt_month  = rtc_time->month;
    pDateTime->dosdt_day    = rtc_time->day_of_month;
    pDateTime->dosdt_hour   = rtc_time->hour;
    pDateTime->dosdt_minute = rtc_time->minute;
    pDateTime->dosdt_second = rtc_time->second; 
    }

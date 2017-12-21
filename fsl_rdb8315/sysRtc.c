/* sysRtc.c - Interface routines for real-time clock and alarm clock support */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,06mar07,l_g  created for fsl_rdb8313
*/

/*
DESCRIPTION

This code provides the real-time clock and alarm clock interfaces.

The routines defined in this file are intended as user-callable routines.
The routines themselves call device-specific routines as defined by the
following in the board header file:

.IP "RTC_SET(x)"
Sets the RTC date and time.

.IP "RTC_SHOW()"
Displays the current RTC time in a user-readable form on the user's display. 

.IP "RTC_GET(x)"
Gets the current RTC date and time.

.IP "RTC_DATE_TIME_HOOK(x)"
Hook to connect the RTC_DATE_GET routine to the dosFsLib to provide 
date/time values for file timestamps.

.LP

INCLUDED FILES: ds1339Rtc.h, dosFsLib.h
*/

/* included files */

#include "ds1339Rtc.c"
#include <dosFsLib.h>

#define RTC_SET(time)              ds1339RtcSet ((time))
#define RTC_SHOW()                 ds1339RtcShow ()
#define RTC_GET(time)              ds1339RtcGet ((time))
#define RTC_DATE_TIME_HOOK(hook)   ds1339RtcDateTimeHook ((hook))


/* forward declarations */

STATUS sysRtcSet (RTC_DATE_TIME * rtc_time);
STATUS sysRtcShow (void);
STATUS sysRtcGet (RTC_DATE_TIME * rtc_time);
void   sysRtcDateTimeHook (DOS_DATE_TIME * pDateTime);

 
/*******************************************************************************
*
* sysRtcSet - Sets the real-time clock date and time.
*
* This routine sets the real-time clock date and time as per the user's
* specification.
*
* The routine expects as input, an RTC_DATE_TIME structure configured with
* valid date and time values.
*
* Valid parameters:
*
* .CS
*
*           year = 00-99
*          month = 01-12
*   day of month = 01-31
*    day of week = 01-07
*           hour = 00-23
*         minute = 00-59
*         second = 00-59
*
* .CE
*
* For example, to set the date and time to January 1, 2006 at 4:40 PM, you
* would populate the structure with the following values:
*
* .CS
*
* 	RTC_DATE_TIME *rtc_time;
*	rtc_time->year = 07;
*	rtc_time->month = 1;
*	rtc_time->day_of_month = 1;
*	rtc_time->day_of_week = 2;
*	rtc_time->hour = 16;
*	rtc_time->minute = 40;
*	rtc_time->second = 0;
*
* .CE
*
* RETURNS: OK, or ERROR if the date and time values were invalid.
*/

STATUS sysRtcSet 
    ( 
    RTC_DATE_TIME * rtc_time	/* Real-time clock parameters */
    )
    {
    if (RTC_SET(rtc_time) == ERROR)
        {
        return (ERROR);
        }
    return (OK);
    }

/*******************************************************************************
*
* sysRtcShow - Display current RTC time and date on the user's display.
*
* This routine displays the current RTC time and date on the user's display
* in a user-readable fashion.
*
*  For example:
*
* .CS
* 
*	Time: 18:34:12  Date: 2/20/2001
*
* .CE
*
* RETURNS: OK, or ERROR if unable to get or display the current RTC time
* and date.
*/

STATUS sysRtcShow (void) 
     {
     if (RTC_SHOW() == ERROR)
        {
        return (ERROR);
        }
     return (OK);
     }

/*******************************************************************************
*
* sysRtcGet - Populate RTC_DATE_TIME structure with current RTC date and time.
*
* This routine expects an RTC_DATE_TIME structure as input, which it will 
* populate with the RTC's current date and time.
*
* RETURNS: OK, or ERROR if unable to get the current RTC date and time.
*/

STATUS sysRtcGet
     (
     RTC_DATE_TIME * rtc_time	/* Current real-time clock parameters */
     )
     {
     if (RTC_GET(rtc_time) == ERROR)
        {
        return (ERROR);
        }
     return (OK);
     }

/*******************************************************************************
*
* sysRtcDateTimeHook - Hook real-time clock to the dosFsLib.
*
* This routine expects a DOS_DATE_TIME structure as input, which it will 
* populate with the current date and time.  It is then used by the standard
* VxWorks dosFsLib as a way to update the date/time values used for file
* timestamps.
*
* RETURNS: void.
*/

void sysRtcDateTimeHook
     (
     DOS_DATE_TIME * pDateTime	/* Current date and time */
     )
     {
     RTC_DATE_TIME_HOOK(pDateTime);
     }

       

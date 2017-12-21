/* sysDs1375.c - Support for the DS1375 RTC with Alarm */

/*
 * Copyright (c) 2005, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01b,23dec08,z_l  Reduced intLock time in sysDs1375RegGet. (WIND00117486)
01a,04may05,cak  Initial writing.
*/

/* 
DESCRIPTION

This file contains the library routines for the Dallas Semiconductor
Maxim 2-Wire Digital Input RTC with Alarm.  This driver is safe for
multi-threaded environments.  That is, a semaphore is used to limit
access to the chip while registers are being read and written, this
includes the SRAM.

The RTC (real-time clock) counts seconds, minutes, hours, day, date,
month, and year with leap-year compensation valid up to 2100.  The 
chip also contains two programmable alarms.  Alarm 1 can be configured
to alarm once per second, once per minute, once per hour, once per day,
once per week, and once per month.  Alarm 2 can be configured to alarm
once per minute, once per hour, once per day, once per week, and once
per month.  In addition to the RTC and the alarms the chip also has
16 bytes of SRAM.

REAL-TIME CLOCK (RTC)

The following is the definition for the structure used to configure
the real-time clock, as well as retrieve the RTC settings:

.CS
typedef struct rtcdt
    {
    int  century;               /@ century @/
    int  year;                  /@ year @/
    int  month;                 /@ month @/
    int  day_of_month;          /@ day of month @/
    int  day_of_week;           /@ day of week @/
    int  hour;                  /@ hour @/
    int  minute;                /@ minute @/
    int  second;                /@ second @/
    BOOL clock;                 /@ 12 or 24 hour clock @/
    BOOL pm;                    /@ If 12-hour clock, AM or PM @/
    } RTC_DATE_TIME;
.CE

STATUS sysDs1375RtcGet (RTC_DATE_TIME * rtc_time)

This routine will capture the settings for the RTC.  <rtc_time> will be
populated with the current RTC settings.

STATUS sysDs1375RtcSet (RTC_DATE_TIME * rtc_time)

This routine will configure the RTC.  The RTC is configured based
on the parameters specified with <rtc_time>.

STATUS sysDs1375RtcShow (void)

This routine will display the current RTC settings to the console with
a carriage return at the end.

STATUS sysDs1375ClockStop (void)

This routine will stop the RTC, that is, the timekeeping registers will
not be allowed to advance.  This routine is provided to assist in
synchronizing the DS1375 time to a reference.  It is to be used in
conjunction with the sysDs1375ClockStart() routine.

STATUS sysDs1375ClockStart (void)

This routine will start the RTC, that is, the timekeeping registers will
be allowed to advance.  This routine is provided to assist in
synchronizing the DS1375 time to a reference.  It is to be used in
conjunction with the sysDs1375ClockStop() routine.

STATUS timeSet (void)

This routine is a command-line utility for setting the RTC from the
VxWorks kernel prompt.  The user will be prompted for the appropriate
input.

STATUS sysDs1375RtcPrintString (void)

This routine will print the current settings of the RTC to the console, 
without a carriage return.

STATUS sysDs1375RtcRegShow (void)

This routine will display the current values in the RTC registers.

ALARM CLOCKS

The following is the definition for the structure used to configure 
the alarm clocks, as well as retrieve the alarm clock settings:

.CS
typedef struct alarmdt
    {
    int  day_of_month;          /@ day of month @/
    int  day_of_week;           /@ day of week @/
    int  hour;                  /@ hour @/
    int  minute;                /@ minute @/
    int  second;                /@ second @/
    int  alarm;                 /@ number of the alarm @/
    BOOL clock;                 /@ 12 or 24 hour clock @/
    BOOL pm;                    /@ If 12-hour clock, AM or PM @/
    HANDLER userAlarmHandler;   /@ interrupt handler for alarm @/
    } ALARM_DATE_TIME;
.CE

The following methods are valid for programming the alarm clocks.
Any routine that has <method> as a parameter is referring to the
following macros, which are defined and available for use:

ALARM_EVERY_MONTH       1       /@ Alarm to recur once a month @/
ALARM_EVERY_WEEK	6	/@ Alarm to recur once a week @/
ALARM_EVERY_DAY         2       /@ Alarm to recur once a day @/
ALARM_EVERY_HOUR        3       /@ Alarm to recur once an hour @/
ALARM_EVERY_MINUTE      4       /@ Alarm to recur once a minute @/
ALARM_EVERY_SECOND      5       /@ Alarm to recur every second @/

STATUS sysDs1375AlarmSet (UCHAR method, ALARM_DATE_TIME * alarm_time)

This routine will configure the given alarm, as indicated by the <alarm>
field in the ALARM_DATE_TIME structure.  The alarm is configured based
on the parameters specified with <method> and <alarm_time>.

STATUS sysDs1375AlarmCancel (int alarm)

This routine will cancel the given alarm as indicated with <alarm>.
It simply disables the alarm's interrupt.  The alarm settings are
left as is.

STATUS sysDs1375AlarmGet (UCHAR * method, ALARM_DATE_TIME * alarm_time)

This routine will capture the settings for the alarm that is indicated by
the <alarm> field in the ALARM_DATE_TIME structure that is passed into
the routine.  <method> and <alarm_time> will be populated with the
current alarm settings.

STATUS sysDs1375AlarmShow (void)

This routine will display the current alarm settings to the console with
a carriage return at the end.

STATUS alarmSet (void)

This routine is a command-line utility for setting the alarms from the
VxWorks kernel prompt.  The user will be prompted for the appropriate
input.

STATUS sysDs1375AlarmPrintString (int alarm)

This routine will print the settings for the alarm as indicated by
<alarm> to the console, without a carriage return.

STATUS sysDs1375AlarmRegShow (void)

This routine will display the current values in the alarm registers.

GENERAL PURPOSE ROUTINES

STATUS sysDs1375ControlRegShow (void)

This routine will display the current values in the control registers.

STATUS sysDs1375RegShow (void)

This routine will display the current values in the RTC, alarm and control
registers.

SRAM

STATUS sysDs1375SramRead (int offset, int numBytes, UINT8 *pBfr)

This routine will read from the DS1375's SRAM.  You must provide the
<offset> into the SRAM, the number of bytes to be read <numBytes>, and
a buffer to hold the data that is read <pBfr>.

STATUS sysDs1375SramWrite (int offset, int numBytes, UINT8 *pBfr)

This routine will write to the DS1375's SRAM.  You provide the <offset>
into the SRAM, the number of bytes to be written <numBytes>, and a
buffer pointing to the data to be written <pBfr>.

STATUS sysDs1375SramShow (void)

This routine will display the contents (all 16 bytes) to the console.

DEBUG SUPPORT

To aid in debug a local variable `verbose' has been created.  To enable
log messages while debugging set `verbose' to TRUE.
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "dosFsLib.h"
#include "stdio.h"
#include "ctype.h"
#include "time.h"
#include "ds1375.h"

/* defines */

#undef	DEBUG_RTC

#define BOARD_DFLT_INPUT_FREQ	DS1375_INPUT_FREQ_32768
#define BOARD_DFLT_OUTPUT_FREQ	DS1375_OUTPUT_FREQ_8192

/* locals */

LOCAL BOOL ds1375InitFlag = FALSE;	/* Initialization Flag */
LOCAL DS1375_REG sysDs1375Reg;		/* structure for device values */
LOCAL BOOL verbose = FALSE;		/* control DEBUG error statements */

/* forward declarations */

LOCAL STATUS sysDs1375RtcParamsCheck (RTC_DATE_TIME * rtc_time);
LOCAL STATUS sysDs1375AlarmParamsCheck (UCHAR method,
					ALARM_DATE_TIME * alarm_time);
LOCAL STATUS sysDs1375Init (void);
LOCAL STATUS sysDs1375RegGet (void);
LOCAL void sysDs1375Int (void);
LOCAL STATUS sysDs1375AlarmIntEnable (int alarm);
LOCAL STATUS sysDs1375AlarmIntDisable (int alarm);

/* external references */

IMPORT STATUS sysMotI2cRead (UINT32 devAddr, UINT32 devOffset,
                             UINT32 devAddrBytes, UCHAR *spdData,
                             UINT32 numBytes);
IMPORT STATUS sysMotI2cWrite (UINT32 devAddr, UINT32 devOffset, 
			      UINT32 devAddrBytes, UINT8 *pBfr, 
			      UINT32 numBytes);

/******************************************************************************
*
* sysDs1375ClockStop - stop the RTC
*
* This routine will stop the RTC, that is, the timekeeping registers will
* not be allowed to advance.  This routine is provided to assist in
* synchronizing the DS1375 time to a reference.  It is to be used in 
* conjunction with the sysDs1375ClockStart() routine.
*
* RETURNS: OK, if the clock was initially running and we are able to
* stop it, ERROR otherwise. 
*/

STATUS sysDs1375ClockStop (void)
    {
    UCHAR data;
    int  lockKey;        /* interrupt lock key */
    STATUS status = OK;

    if (!ds1375InitFlag)
	{
	if (sysDs1375Init() == ERROR)
	    {
	    if (verbose)
		logMsg("ERROR: Chip initialization failed\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
	}

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts */

    lockKey = intLock();
 
    if (DS1375_READ(DS1375_CONTROL_OFFSET, &data) != ERROR)
	{

	/* check to make sure clock not already stopped */

	if (!(~data & DS1375_CONTROL_ECLK_MASK)) 
	    {

	    /* clock is running, so stop the clock */

	    data &= ~DS1375_CONTROL_ECLK_MASK;
	    if (DS1375_WRITE(DS1375_CONTROL_OFFSET, &data) != ERROR)
		{

	    	/* ensure bit is cleared, clock stopped */

		if (DS1375_READ(DS1375_CONTROL_OFFSET, &data) != ERROR)
		    {
		    if (data & DS1375_CONTROL_ECLK_MASK)
			status = ERROR; /* bit still set, clock not stopped */
		    }
		else
		    status = ERROR;	/* read error */
		}
	    else
		status = ERROR;		/* write error */
	    }
	else
	    status = ERROR;		/* clock already stopped */
	}
    else
	status = ERROR;			/* read error */

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if (status == ERROR)
	logMsg("ERROR: sysDs1375ClockStop failed\n\r",0,0,0,0,0,0);

    return (status);
    }

/******************************************************************************
*
* sysDs1375ClockStart - start the RTC
* 
* This routine will start the RTC, that is, the timekeeping registers will
* be allowed to advance.  This routine is provided to assist in
* synchronizing the DS1375 time to a reference.  It is to be used in
* conjunction with the sysDs1375ClockStop() routine.
*
* RETURNS: OK, if the clock was initially stopped and we are able to
* start it, ERROR otherwise.
*
*/

STATUS sysDs1375ClockStart (void)
    {
    UCHAR data;
    int  lockKey;        /* interrupt lock key */
    STATUS status = OK;

    if (!ds1375InitFlag)
	{
	if (sysDs1375Init() == ERROR)
	    {
	    logMsg("ERROR: Chip initialization failed\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
	}

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts */

    lockKey = intLock();
 
    if (DS1375_READ(DS1375_CONTROL_OFFSET, &data) != ERROR)
	{

	/* ensure clock is stopped */

	if (!(data & DS1375_CONTROL_ECLK_MASK))
	    {

	    /* start the clock, set bit */

	    data |= DS1375_CONTROL_ECLK_MASK;
	    if (DS1375_WRITE(DS1375_CONTROL_OFFSET, &data) != ERROR)
		{

		/* ensure bit is set, clock started */

		if (DS1375_READ(DS1375_CONTROL_OFFSET, &data) != ERROR)
		    {
		    if (~data & DS1375_CONTROL_ECLK_MASK)
			status = ERROR;	/* bit not set, clock not started */
		    }
		else
		    status = ERROR;	/* read error */
		}
	    else
		status = ERROR;		/* write error */
	    }
	else
	    status = ERROR;		/* clock already running */
	}
    else
	status = ERROR;		/* read error */

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if (status == ERROR)
	logMsg("ERROR: sysDs1375ClockStart failed\n\r",0,0,0,0,0,0);
    return (status);
    }

/******************************************************************************
*
* sysDs1375AlarmParamsCheck - validate alarm parameters
*
* This routine is used to validate the alarm parameters prior to programming 
* the alarm registers.
*
* RETURNS: OK, if parameters are valid, ERROR otherwise. 
*/

LOCAL STATUS sysDs1375AlarmParamsCheck 
    (
    UCHAR method,
    ALARM_DATE_TIME * alarm_time
    )
    {
    switch (method)
	{
	case ALARM_EVERY_SECOND:
	    break;
	
	case ALARM_EVERY_MINUTE:
	    if (alarm_time->alarm == 1)
		{
		if((alarm_time->second < 0) || (alarm_time->second > 59))
		    {
		    if (verbose)
	    		{
	    		logMsg("ERROR: Second - 0 through 59\n\r",0,0,0,0,0,0);
	    		logMsg("Entered: %d\n\r", alarm_time->second,0,0,0,0,0);
	    		}
		    return (ERROR);
		    }
		}
	    break;

	case ALARM_EVERY_HOUR:
	    if((alarm_time->minute < 0) || (alarm_time->minute > 59))
		{
		if (verbose)
	    	    {
	    	    logMsg("ERROR: Minute - 0 through 59\n\r",0,0,0,0,0,0);
	    	    logMsg("Entered: %d\n\r", alarm_time->minute,0,0,0,0,0);
	    	    }
		return (ERROR);
		}
	    if (alarm_time->alarm == 1)
		{
		if((alarm_time->second < 0) || (alarm_time->second > 59))
		    {
		    if (verbose)
	    		{
	    		logMsg("ERROR: Second - 0 through 59\n\r",0,0,0,0,0,0);
	    		logMsg("Entered: %d\n\r", alarm_time->second,0,0,0,0,0);
	    		}
		    return (ERROR);
		    }
		}
	    break;

	case ALARM_EVERY_DAY:
	    if (alarm_time->clock)
		{
		if ((alarm_time->hour < 1) || (alarm_time->hour > 24))
	    	    {
	    	    if (verbose)
			{
			logMsg("ERROR: Hour - 1 through 24\n\r",0,0,0,0,0,0);
			logMsg("Entered: %d\n\r", alarm_time->hour,0,0,0,0,0);
			}
	    	    return (ERROR);
	    	    }
		}
	    else
		{
		if ((alarm_time->hour < 1) || (alarm_time->hour > 12))
	    	    {
	    	    if (verbose)
			{
			logMsg("ERROR: Hour - 1 through 12\n\r",0,0,0,0,0,0);
			logMsg("Entered: %d\n\r", alarm_time->hour,0,0,0,0,0);
			}
	    	    return (ERROR);
	    	    }
		}
	    if((alarm_time->minute < 0) || (alarm_time->minute > 59))
		{
		if (verbose)
	    	    {
	    	    logMsg("ERROR: Minute - 0 through 59\n\r",0,0,0,0,0,0);
	    	    logMsg("Entered: %d\n\r", alarm_time->minute,0,0,0,0,0);
	    	    }
		return (ERROR);
		}
	    if (alarm_time->alarm == 1)
		{
		if((alarm_time->second < 0) || (alarm_time->second > 59))
		    {
		    if (verbose)
	    		{
	    		logMsg("ERROR: Second - 0 through 59\n\r",0,0,0,0,0,0);
	    		logMsg("Entered: %d\n\r", alarm_time->second,0,0,0,0,0);
	    		}
		    return (ERROR);
		    }
		}
	    break;

	case ALARM_EVERY_WEEK:
	    if((alarm_time->day_of_week < 1) || (alarm_time->day_of_week > 7))
		{
		if (verbose)
	    	    {
	    	    logMsg("ERROR: Day of week - 1 through 7\n\r",0,0,0,0,0,0);
	    	    logMsg("Entered: %d\n\r",alarm_time->day_of_week,0,0,0,0,0);
	    	    }
		return (ERROR);
		}
	    if (alarm_time->clock)
		{
		if ((alarm_time->hour < 1) || (alarm_time->hour > 24))
	    	    {
	    	    if (verbose)
			{
			logMsg("ERROR: Hour - 1 through 24\n\r",0,0,0,0,0,0);
			logMsg("Entered: %d\n\r", alarm_time->hour,0,0,0,0,0);
			}
	    	    return (ERROR);
	    	    }
		}
	    else
		{
		if ((alarm_time->hour < 1) || (alarm_time->hour > 12))
	    	    {
	    	    if (verbose)
			{
			logMsg("ERROR: Hour - 1 through 12\n\r",0,0,0,0,0,0);
			logMsg("Entered: %d\n\r", alarm_time->hour,0,0,0,0,0);
			}
	    	    return (ERROR);
	    	    }
		}
	    if((alarm_time->minute < 0) || (alarm_time->minute > 59))
		{
		if (verbose)
	    	    {
	    	    logMsg("ERROR: Minute - 0 through 59\n\r",0,0,0,0,0,0);
	    	    logMsg("Entered: %d\n\r", alarm_time->minute,0,0,0,0,0);
	    	    }
		return (ERROR);
		}
	    if (alarm_time->alarm == 1)
		{
		if((alarm_time->second < 0) || (alarm_time->second > 59))
		    {
		    if (verbose)
	    		{
	    		logMsg("ERROR: Second - 0 through 59\n\r",0,0,0,0,0,0);
	    		logMsg("Entered: %d\n\r", alarm_time->second,0,0,0,0,0);
	    		}
		    return (ERROR);
		    }
		}
	    break;

	case ALARM_EVERY_MONTH:
	    if((alarm_time->day_of_month < 0)||(alarm_time->day_of_month > 31))
	        {
	        if (verbose)
		    {
		    logMsg("ERROR: Day - 1 through 31\n\r",0,0,0,0,0,0);
		    logMsg("Entered: %d\n\r",alarm_time->day_of_month,0,0,0,0,0);
		    }
	    	return (ERROR);
	    	}
	    if (alarm_time->clock)
		{
		if ((alarm_time->hour < 1) || (alarm_time->hour > 24))
	    	    {
	    	    if (verbose)
			{
			logMsg("ERROR: Hour - 1 through 24\n\r",0,0,0,0,0,0);
			logMsg("Entered: %d\n\r", alarm_time->hour,0,0,0,0,0);
			}
	    	    return (ERROR);
	    	    }
		}
	    else
		{
		if ((alarm_time->hour < 1) || (alarm_time->hour > 12))
	    	    {
	    	    if (verbose)
			{
			logMsg("ERROR: Hour - 1 through 12\n\r",0,0,0,0,0,0);
			logMsg("Entered: %d\n\r", alarm_time->hour,0,0,0,0,0);
			}
	    	    return (ERROR);
	    	    }
		}
	    if((alarm_time->minute < 0) || (alarm_time->minute > 59))
		{
		if (verbose)
	    	    {
	    	    logMsg("ERROR: Minute - 0 through 59\n\r",0,0,0,0,0,0);
	    	    logMsg("Entered: %d\n\r", alarm_time->minute,0,0,0,0,0);
	    	    }
		return (ERROR);
		}
	    if (alarm_time->alarm == 1)
		{
		if((alarm_time->second < 0) || (alarm_time->second > 59))
		    {
		    if (verbose)
	    		{
	    		logMsg("ERROR: Second - 0 through 59\n\r",0,0,0,0,0,0);
	    		logMsg("Entered: %d\n\r", alarm_time->second,0,0,0,0,0);
	    		}
		    return (ERROR);
		    }
		}
	    break;
	default:
	    if (verbose)
		logMsg("Unknown method\n\r",0,0,0,0,0,0);
	    return (ERROR);
	};
    return (OK);
    }

/******************************************************************************
*
* sysDs1375RtcParamsCheck - validate RTC parameters
*
* This routine is used to validate the RTC parameters prior to programming
* the RTC registers.
*
* RETURNS: OK, if parameters are valid, ERROR otherwise.
*/

LOCAL STATUS sysDs1375RtcParamsCheck
    (
    RTC_DATE_TIME * rtc_time   /* pointer to time keeping structure */
    )
    {

    /* Validate the Century */

    if ((rtc_time->century != 20) && (rtc_time->century != 21))
	{
	if (verbose)
	    {
	    logMsg("ERROR: Century - 20 or 21 (2000 - 2199)\n\r",0,0,0,0,0,0);
	    logMsg("Entered: %d\n\r", rtc_time->century,0,0,0,0,0);
	    }
	return (ERROR);	
	}

    /* Validate the Year */

    if((rtc_time->year < 0) || (rtc_time->year > 99))
	{
	if (verbose)
	    {
	    logMsg("ERROR: Year - 00 through 99 (2000 - 2199)\n\r",0,0,0,0,0,0);
	    logMsg("Entered: %d\n\r", rtc_time->year,0,0,0,0,0);
	    }
	return (ERROR);
	}

    /* Validate the Month */

    if((rtc_time->month < 1) || (rtc_time->month > 12))
	{
	if (verbose)
	    {
	    logMsg("ERROR: Month - 1 through 12\n\r",0,0,0,0,0,0);
	    logMsg("Entered: %d\n\r", rtc_time->month,0,0,0,0,0);
	    }
	return (ERROR);
	}

    /* Validate the Day of the Month */

    switch (rtc_time->month)
	{
	case 1: case 3: case 5: case 7:
	case 8: case 10: case 12:
	if((rtc_time->day_of_month < 0) || (rtc_time->day_of_month > 31))
	    {
	    if (verbose)
		{
		logMsg("ERROR: Day - 1 through 31\n\r",0,0,0,0,0,0);
		logMsg("Entered: %d\n\r", rtc_time->day_of_month,0,0,0,0,0);
		}
	    return (ERROR);
	    }
	break;
	case 4: case 6: case 9: case 11:
	if((rtc_time->day_of_month < 0) || (rtc_time->day_of_month > 30))
	    {
	    if (verbose)
		{
		logMsg("ERROR: Day - 1 through 30\n\r",0,0,0,0,0,0);
		logMsg("Entered: %d\n\r", rtc_time->day_of_month,0,0,0,0,0);
		}
	    return (ERROR);
	    }
	break;
	case 2:
	if((rtc_time->day_of_month < 0) || (rtc_time->day_of_month > 28))
	    {
	    if (verbose)
		{
		logMsg("ERROR: Day - 1 through 28\n\r",0,0,0,0,0,0);
		logMsg("Entered: %d\n\r", rtc_time->day_of_month,0,0,0,0,0);
		}
	    return (ERROR);
	    }
	break;
	default:
	    if (verbose)
		logMsg("ERROR: Unknown month\n\r",0,0,0,0,0,0);
	    return (ERROR);
	};

    /* Validate the Day of the Week */

    if((rtc_time->day_of_week < 1) || (rtc_time->day_of_week > 7))
	{
	if (verbose)
	    {
	    logMsg("ERROR: Day of week - 1 through 7\n\r",0,0,0,0,0,0);
	    logMsg("Entered: %d\n\r", rtc_time->day_of_week,0,0,0,0,0);
	    }
	return (ERROR);
	}

    /* Validate the Hour */

    if (rtc_time->clock)
	{
	if ((rtc_time->hour < 1) || (rtc_time->hour > 24))
	    {
	    if (verbose)
		{
		logMsg("ERROR: Hour - 1 through 24\n\r",0,0,0,0,0,0);
		logMsg("Entered: %d\n\r", rtc_time->hour,0,0,0,0,0);
		}
	    return (ERROR);
	    }
	}
    else
	{
	if ((rtc_time->hour < 1) || (rtc_time->hour > 12))
	    {
	    if (verbose)
		{
		logMsg("ERROR: Hour - 1 through 12\n\r",0,0,0,0,0,0);
		logMsg("Entered: %d\n\r", rtc_time->hour,0,0,0,0,0);
		}
	    return (ERROR);
	    }
	}

    /* Validate the Minute */

    if((rtc_time->minute < 0) || (rtc_time->minute > 59))
	{
	if (verbose)
	    {
	    logMsg("ERROR: Minute - 0 through 59\n\r",0,0,0,0,0,0);
	    logMsg("Entered: %d\n\r", rtc_time->minute,0,0,0,0,0);
	    }
	return (ERROR);
	}

    /* Validate the Second */

    if((rtc_time->second < 0) || (rtc_time->second > 59))
	{
	if (verbose)
	    {
	    logMsg("ERROR: Second - 0 through 59\n\r",0,0,0,0,0,0);
	    logMsg("Entered: %d\n\r", rtc_time->second,0,0,0,0,0);
	    }
	return (ERROR);
	}

    /* Everything checked out OK, so return without error */

    return (OK);
    }

/*******************************************************************************
*
* sysDs1375DateTimeHook - This is a hook routine for the dosFsLib.
*
* This routine fills in a DOS_DATE_TIME structure with the current date
* and time.  This is used by the standard VxWorks dosFsLib as a way to update
* the date/time values used for file timestamps.  This is "hooked" into the
* dosFsLib during board initialization if both the dosFsLib and the RTC support
* is included in the BSP.
*
* RETURNS: N/A 
*
* SEE ALSO: dosFsDateTimeInstall(), and the dosFsLib documentation.
*/

void sysDs1375DateTimeHook
    (
    DOS_DATE_TIME * pDateTime    /* pointer to time keeping structure */
    )
    {
    RTC_DATE_TIME rtc_time;     /* RTC date and time */
    int dos_year = 0;           /* Current year */

    sysDs1375RtcGet(&rtc_time);

    /* Convert century and year to a form suitable for DOS_DATE_TIME */

    dos_year = ((rtc_time.century * 100) + rtc_time.year);

    /* Populate DOS_DATE_TIME structure within current RTC time and date */

    pDateTime->dosdt_year = dos_year;
    pDateTime->dosdt_month = rtc_time.month;
    pDateTime->dosdt_day = rtc_time.day_of_month;
    pDateTime->dosdt_hour = rtc_time.hour;
    pDateTime->dosdt_minute = rtc_time.minute;
    pDateTime->dosdt_second = rtc_time.second;
    }

/******************************************************************************
*
* sysDs1375Int - main interrupt handler for the DS1375
*
* This routine is the main interrupt handler for the DS1375.  When an alarm
* is set, a user-defined interrupt handler may be specified.  If a routine is
* specified, this interrupt handler will dispatch it.  Otherwise, the interrupt
* is simply cleared in preparation for the next interrupt.
*
* RETURNS: N/A
*/

LOCAL void sysDs1375Int (void)
    {
    UCHAR controlStatus;
    UCHAR control;

    DS1375_READ(DS1375_CONTROL_STATUS_OFFSET, &controlStatus);
    DS1375_READ(DS1375_CONTROL_OFFSET, &control);

    /* Alarm 1 Interrupt */

    if ((controlStatus & DS1375_CONTROL_STATUS_A1F_MASK) &&
	(control & DS1375_CONTROL_A1IE_MASK)) 
	{

	/* Clear Alarm 1 Interrupt */

	controlStatus &= ~DS1375_CONTROL_STATUS_A1F_MASK;
	DS1375_WRITE(DS1375_CONTROL_STATUS_OFFSET, &controlStatus);

	/* Call User-defined Interrupt Handler, if specified */

	if (sysDs1375Reg.alarm1IntHandler != NULL)
	    sysDs1375Reg.alarm1IntHandler();
#ifdef DEBUG_RTC
	logMsg("Alarm 1 interrupt\n\r",0,0,0,0,0,0);
#endif /* DEBUG_RTC */
	}

    /* Alarm 2 Interrupt */

    if ((controlStatus & DS1375_CONTROL_STATUS_A2F_MASK) &&
	(control & DS1375_CONTROL_A2IE_MASK))
	{

	/* Clear Alarm 2 Interrupt */

	controlStatus &= ~DS1375_CONTROL_STATUS_A2F_MASK;
	DS1375_WRITE(DS1375_CONTROL_STATUS_OFFSET, &controlStatus);

	/* Call User-defined Interrupt Handler, if specified */

	if (sysDs1375Reg.alarm2IntHandler != NULL)
	    sysDs1375Reg.alarm2IntHandler();
#ifdef DEBUG_RTC
	logMsg("Alarm 2 interrupt\n\r",0,0,0,0,0,0);
#endif /* DEBUG_RTC */
	} 
    }

/******************************************************************************
*
* alarmSet - command line utility for setting the alarms
*
* This routine is a command-line utility for setting the alarms from the
* VxWorks kernel prompt.  The user will be prompted for the appropriate
* input.
*
* RETURNS: OK, if the alarm is successfully configured and enabled, ERROR
* otherwise.
*/

STATUS alarmSet (void)
    {
    char alarm[1];
    char query[1];
    char dayOfWeek[1];
    char dayOfMonth[2];
    char hour[2];
    char minute[2];
    char second[2];
    char clock[1];
    char amPm[1];
    ALARM_DATE_TIME alarm_time; 

    amPm[0] = 'y';
    alarm_time.userAlarmHandler = NULL;   

    printf("Alarm 1 or 2? (1/2): ");
    gets((char *)&alarm);
    alarm_time.alarm  = strtoul((char *)&alarm, NULL, 10); 

    if (alarm[0] == '1')
	{
        printf("Once per second? (y/n): ");
        gets((char *)&query);
	if ((query[0] == 'y') || (query[0] == 'Y'))
	    {
	    if (sysDs1375AlarmSet (ALARM_EVERY_SECOND , &alarm_time) == ERROR)
		{
		if (verbose)
		    logMsg("ERROR: Unable to set alarm.\n\r",0,0,0,0,0,0);
		return (ERROR);
		}
	    return (OK);
	    }
	}

    printf("Once per minute? (y/n): ");
    gets((char *)&query);
    if ((query[0] == 'y') || (query[0] == 'Y'))
        {	
	if (alarm[0] == '1')
	    {
	    printf("Enter the second (00 - 59): ");
	    gets((char *)&second);
	    alarm_time.second = strtoul((char *)&second, NULL, 10);
	    }

	if (sysDs1375AlarmSet (ALARM_EVERY_MINUTE , &alarm_time) == ERROR)
		{
		if (verbose)
		    logMsg("ERROR: Unable to set alarm.\n\r",0,0,0,0,0,0);
                return (ERROR);
		}	
	return (OK);
        }

    printf("Once per hour? (y/n): ");
    gets((char *)&query);
    if ((query[0] == 'y') || (query[0] == 'Y'))
        {
	printf("Enter the minute (00 - 59): ");
	gets((char *)&minute);
	alarm_time.minute = strtoul((char *)&minute, NULL, 10);

	if (alarm[0] == '1')
	    {
	    printf("Enter the second (00 - 59): ");
	    gets((char *)&second);
	    alarm_time.second = strtoul((char *)&second, NULL, 10);
	    }

	if (sysDs1375AlarmSet (ALARM_EVERY_HOUR , &alarm_time) == ERROR)
            {
            if (verbose)
                logMsg("ERROR: Unable to set alarm.\n\r",0,0,0,0,0,0);
            return (ERROR);
            }
        return (OK);
        }

    printf("Once per day? (y/n): ");
    gets((char *)&query);
    if ((query[0] == 'y') || (query[0] == 'Y'))
        {
	printf("24-hour clock? (y/n): ");
	gets((char *)&clock);
	if ((clock[0] == 'n') || (clock[0] == 'N'))
	    {
	    printf("Enter the hour (1 - 12): ");
	    gets((char *)&hour);
	    alarm_time.clock = FALSE;
	    printf("PM? (y/n): ");
	    gets((char *)&amPm);
	    if ((amPm[0] == 'y') || (amPm[0] == 'Y'))
	        alarm_time.pm = TRUE;
	    else
	        alarm_time.pm = FALSE;
	    }
	else
	    {
	    printf("Enter the hour (1 - 24): ");
	    gets((char *)&hour);
	    alarm_time.clock = TRUE;
	    }
	alarm_time.hour = strtoul((char *)&hour, NULL, 10);

	printf("Enter the minute (00 - 59): ");
	gets((char *)&minute);
	alarm_time.minute = strtoul((char *)&minute, NULL, 10);

	if (alarm[0] == '1')
	    {
	    printf("Enter the second (00 - 59): ");
	    gets((char *)&second);
	    alarm_time.second = strtoul((char *)&second, NULL, 10);
	    }

	if (sysDs1375AlarmSet (ALARM_EVERY_DAY, &alarm_time) == ERROR)
            {
            if (verbose)
                logMsg("ERROR: Unable to set alarm.\n\r",0,0,0,0,0,0);
            return (ERROR);
            }
        return (OK);
        }

    printf("Once per week? (y/n): ");
    gets((char *)&query);
    if ((query[0] == 'y') || (query[0] == 'Y'))
        {
	printf("Enter the day of the week (eg. 1 for Sunday): ");
	gets((char *)&dayOfWeek);
	alarm_time.day_of_week = strtoul((char *)&dayOfWeek, NULL, 10);

	printf("24-hour clock? (y/n): ");
	gets((char *)&clock);
	if ((clock[0] == 'n') || (clock[0] == 'N'))
	    {
	    printf("Enter the hour (1 - 12): ");
	    gets((char *)&hour);
	    alarm_time.clock = FALSE;
	    printf("PM? (y/n): ");
	    gets((char *)&amPm);
	    if ((amPm[0] == 'y') || (amPm[0] == 'Y'))
	        alarm_time.pm = TRUE;
	    else
	        alarm_time.pm = FALSE;
	    }
	else
	    {
	    printf("Enter the hour (1 - 24): ");
	    gets((char *)&hour);
	    alarm_time.clock = TRUE;
	    }
	alarm_time.hour = strtoul((char *)&hour, NULL, 10);

	printf("Enter the minute (00 - 59): ");
	gets((char *)&minute);
	alarm_time.minute = strtoul((char *)&minute, NULL, 10);

	if (alarm[0] == '1')
	    {
	    printf("Enter the second (00 - 59): ");
	    gets((char *)&second);
	    alarm_time.second = strtoul((char *)&second, NULL, 10);
	    }

	if (sysDs1375AlarmSet (ALARM_EVERY_WEEK, &alarm_time) == ERROR)
            {
            if (verbose)
                logMsg("ERROR: Unable to set alarm.\n\r",0,0,0,0,0,0);
            return (ERROR);
            }
        return (OK);
        }

    printf("Once per month? (y/n): ");
    gets((char *)&query);
    if ((query[0] == 'y') || (query[0] == 'Y'))
        {
	printf("Enter the day of the month (eg. 15 for the 15th): ");
	gets((char *)&dayOfMonth);
	alarm_time.day_of_month = strtoul((char *)&dayOfMonth, NULL, 10);

	printf("24-hour clock? (y/n): ");
	gets((char *)&clock);
	if ((clock[0] == 'n') || (clock[0] == 'N'))
	    {
	    printf("Enter the hour (1 - 12): ");
	    gets((char *)&hour);
	    alarm_time.clock = FALSE;
	    printf("PM? (y/n): ");
	    gets((char *)&amPm);
	    if ((amPm[0] == 'y') || (amPm[0] == 'Y'))
	        alarm_time.pm = TRUE;
	    else
	        alarm_time.pm = FALSE;
	    }
	else
	    {
	    printf("Enter the hour (1 - 24): ");
	    gets((char *)&hour);
	    alarm_time.clock = TRUE;
	    }
	alarm_time.hour = strtoul((char *)&hour, NULL, 10);

	printf("Enter the minute (00 - 59): ");
	gets((char *)&minute);
	alarm_time.minute = strtoul((char *)&minute, NULL, 10);

	if (alarm[0] == '1')
	    {
	    printf("Enter the second (00 - 59): ");
	    gets((char *)&second);
	    alarm_time.second = strtoul((char *)&second, NULL, 10);
	    }

	if (sysDs1375AlarmSet (ALARM_EVERY_MONTH, &alarm_time) == ERROR)
            {
            if (verbose)
                logMsg("ERROR: Unable to set alarm.\n\r",0,0,0,0,0,0);
            return (ERROR);
            }
        return (OK);
        }

    logMsg("No further options.  Alarm will not be set!\n\r",0,0,0,0,0,0);
    return (ERROR);
    }

/******************************************************************************
*
* timeSet - command line utility for setting the RTC
*
* This routine is a command-line utility for setting the RTC from the
* VxWorks kernel prompt.  The user will be prompted for the appropriate
* input.
*
* RETURNS: OK, if the RTC is successfully configured and enabled, ERROR
* otherwise.
*/

STATUS timeSet (void)
    {
    char year[4];
    char dayOfWeek[1];
    char month[2];
    char dayOfMonth[2];
    char hour[2];
    char minute[2];
    char second[2];
    char clock[1];
    char amPm[1];
    RTC_DATE_TIME rtc_time;
    ULONG yearInt;

    amPm[0] = 'y';

    /* Prompt for Day of the week */

    printf("Enter the day of the week (eg. 1 for Sunday): ");
    gets((char *)&dayOfWeek);

    /* Prompt for Month */

    printf("Enter the month (eg. 1 for January): ");
    gets((char *)&month);

    /* Prompt for Day of the Month */

    printf("Enter the day of the month (eg. 15 for the 15th): ");
    gets((char *)&dayOfMonth);

    /* Prompt for Year */

    printf("Enter the year (eg. 2005): ");
    gets((char *)&year);

    /* Prompt for 12- or 24-hour clock? & Hour */

    printf("24-hour clock? (y/n): ");
    gets((char *)&clock);
    if ((clock[0] == 'n') || (clock[0] == 'N'))
	{
	printf("Enter the hour (1 - 12): ");
	gets((char *)&hour);
	}
    else
	{
	printf("Enter the hour (1 - 24): ");
	gets((char *)&hour);
	}

    /* Prompt for Minute */

    printf("Enter the minute (00 - 59): ");
    gets((char *)&minute);

    /* Prompt for Second */

    printf("Enter the second (00 - 59): ");
    gets((char *)&second);

    /* If 12-hour clock, prompt for AM or PM? */

    if ((clock[0] == 'n') || (clock[0] == 'N'))
	{
    	printf("PM? (y/n): ");
    	gets((char *)&amPm);
	}

    /* Now populate the RTC structure */

    /* Year */

    yearInt = strtoul((char *)&year, NULL, 10);
    if ((yearInt >= 2000)  && (yearInt <= 2099))
	{
	rtc_time.century = 20;
	rtc_time.year = yearInt - 2000;
	}
    else if ((yearInt >= 2100) && (yearInt <= 2199))
	{
	rtc_time.century = 21;
	rtc_time.year = yearInt - 2100;
	}

    /* Month */

    rtc_time.month = strtoul((char *)&month, NULL, 10);

    /* Day of Month */

    rtc_time.day_of_month = strtoul((char *)&dayOfMonth, NULL, 10);

    /* Day of Week */

    rtc_time.day_of_week = strtoul((char *)&dayOfWeek, NULL, 10);

    /* Hour */

    rtc_time.hour = strtoul((char *)&hour, NULL, 10);

    /* Minute */

    rtc_time.minute = strtoul((char *)&minute, NULL, 10);

    /* Second */

    rtc_time.second = strtoul((char *)&second, NULL, 10);

    /* Clock: 12- or 24- Hour, AM or PM */

    if ((clock[0] == 'y') || (clock[0] == 'Y'))
	{
	rtc_time.clock = TRUE;
	}
    else
	{
	rtc_time.clock = FALSE;
	if ((amPm[0] == 'y') || (amPm[0] == 'Y'))
	    rtc_time.pm = TRUE;
	else
	    rtc_time.pm = FALSE;
	}

    /* Program the RTC Registers */

    if (sysDs1375RtcSet(&rtc_time) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to program RTC registers\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    return (OK);
    }

/******************************************************************************
*
* sysDs1375Init - Initialize the DS1375 chip and driver
*
* This routine initializes the chip with board specific register settings
* and also initializes the driver by setting up the driver semaphore, etc.
*
* RETURNS: OK, if successfully initialized, ERROR otherwise.
*/

LOCAL STATUS sysDs1375Init (void)
    {
    UCHAR data;
    int  lockKey;        /* interrupt lock key */
    STATUS status = OK;

    /* allocate semaphore for protecting access to DS1375 */

    sysDs1375Reg.semDrvId = semBCreate (SEM_Q_PRIORITY, SEM_FULL);

    sysDs1375Reg.alarm1IntHandler = NULL;
    sysDs1375Reg.alarm2IntHandler = NULL;

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts */

    lockKey = intLock();

    /* Set the Input and Output Frequencies */
 
    if (DS1375_READ(DS1375_CONTROL_OFFSET, &data) != ERROR)
	{
	data &= ~(DS1375_CONTROL_CLKSEL_MASK | DS1375_CONTROL_RS_MASK);
	data |= (BOARD_DFLT_INPUT_FREQ << DS1375_CONTROL_CLKSEL_BITS);
	data |= (BOARD_DFLT_OUTPUT_FREQ << DS1375_CONTROL_RS_BITS);
	data &= ~(DS1375_CONTROL_A2IE_MASK | DS1375_CONTROL_A1IE_MASK);
	data |= DS1375_CONTROL_INTCN_MASK;

	if (DS1375_WRITE(DS1375_CONTROL_OFFSET, &data) == ERROR)
	    status = ERROR;	/* write error */
	}
    else
	status = ERROR;		/* read error */

    if (status != ERROR)
	{

	/* Connect and Enable the Main Interrupt shared by both alarms */

	intConnect (INUM_TO_IVEC(DS1375_INT_VEC), (VOIDFUNCPTR)sysDs1375Int, 0);
	intEnable (DS1375_INT_VEC);
	}

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if (status != ERROR)
	ds1375InitFlag = TRUE;
    else
	{
	if (verbose)
	    logMsg("ERROR: Initialization failed\n\r",0,0,0,0,0,0);
	}
    return (status);
    }

/******************************************************************************
*
* sysDs1375AlarmGet - current snapshot of alarm settings
*
* This routine will capture the settings for the alarm that is indicated by
* the <alarm> field in the ALARM_DATE_TIME structure is passed into
* the routine.  <method> and <alarm_time> will be populated with the 
* current alarm settings.
*
* RETURNS: OK, if able to capture the alarm settings, ERROR otherwise.
*/

STATUS sysDs1375AlarmGet
    (
    UCHAR * method,
    ALARM_DATE_TIME * alarm_time      /* pointer to time keeping structure */
    )
    {

    /* Get current snaphot of registers */

    if (sysDs1375RegGet() == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to retrieve settings\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if (alarm_time->alarm == 1)
	{
	if ((sysDs1375Reg.alarm1Seconds & DS1375_ALARM_AxMx_MASK)  &&
	    (sysDs1375Reg.alarm1Minutes & DS1375_ALARM_AxMx_MASK)  &&
	    (sysDs1375Reg.alarm1Hours & DS1375_ALARM_AxMx_MASK) &&
	    (sysDs1375Reg.alarm1DayDate & DS1375_ALARM_AxMx_MASK))
	    *method = ALARM_EVERY_SECOND;
	else if ((!(sysDs1375Reg.alarm1Seconds & DS1375_ALARM_AxMx_MASK)) &&
	         (sysDs1375Reg.alarm1Minutes & DS1375_ALARM_AxMx_MASK)  &&
		 (sysDs1375Reg.alarm1Hours & DS1375_ALARM_AxMx_MASK) &&
		 (sysDs1375Reg.alarm1DayDate & DS1375_ALARM_AxMx_MASK))
	         *method = ALARM_EVERY_MINUTE;
	else if ((!(sysDs1375Reg.alarm1Seconds & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1Minutes & DS1375_ALARM_AxMx_MASK)) &&
		 (sysDs1375Reg.alarm1Hours & DS1375_ALARM_AxMx_MASK) &&
                 (sysDs1375Reg.alarm1DayDate & DS1375_ALARM_AxMx_MASK))
	         *method = ALARM_EVERY_HOUR;
	else if ((!(sysDs1375Reg.alarm1Seconds & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1Minutes & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1Hours & DS1375_ALARM_AxMx_MASK)) &&
                 (sysDs1375Reg.alarm1DayDate & DS1375_ALARM_AxMx_MASK))
	         *method = ALARM_EVERY_DAY;
	else if ((!(sysDs1375Reg.alarm1Seconds & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1Minutes & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1Hours & DS1375_ALARM_AxMx_MASK)) &&
                 (!(sysDs1375Reg.alarm1DayDate & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1DayDate & 0x40)))
	         *method = ALARM_EVERY_MONTH;
	else if ((!(sysDs1375Reg.alarm1Seconds & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1Minutes & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1Hours & DS1375_ALARM_AxMx_MASK)) &&
                 (!(sysDs1375Reg.alarm1DayDate & DS1375_ALARM_AxMx_MASK)) &&
		 (sysDs1375Reg.alarm1DayDate & 0x40))
	         *method = ALARM_EVERY_WEEK;
	else
	    {
	    if (verbose)
		logMsg("ERROR: Invalid method\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
	switch (*method)
	    {
	    case ALARM_EVERY_SECOND:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 0;
		alarm_time->hour = 0;
		alarm_time->minute = 0;
		alarm_time->second = 0;
		alarm_time->clock = FALSE;
		alarm_time->pm = FALSE;
		break;
	    case ALARM_EVERY_MINUTE:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 0;
		alarm_time->hour = 0;
		alarm_time->minute = 0;
		alarm_time->second = BCD_TO_BIN(sysDs1375Reg.alarm1Seconds);
		alarm_time->clock = FALSE;
		alarm_time->pm = FALSE;
		break;
	    case ALARM_EVERY_HOUR:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 0;
		alarm_time->hour = 0;
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm1Minutes);
		alarm_time->second = BCD_TO_BIN(sysDs1375Reg.alarm1Seconds);
		alarm_time->clock = FALSE;
		alarm_time->pm = FALSE;
		break;
	    case ALARM_EVERY_DAY:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 0;
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm1Minutes);
		alarm_time->second = BCD_TO_BIN(sysDs1375Reg.alarm1Seconds);
		if (sysDs1375Reg.alarm1Hours & 0x40)
		    {
		    alarm_time->clock = FALSE;
		    alarm_time->hour = 
                        BCD_TO_BIN(sysDs1375Reg.alarm1Hours & 0x1F);
		    if (sysDs1375Reg.alarm1Hours & 0x20)
			alarm_time->pm = TRUE;
		    else
			alarm_time->pm = FALSE;	
		    }
		else
		    {
		    alarm_time->clock = TRUE;
		    alarm_time->hour = 
			BCD_TO_BIN(sysDs1375Reg.alarm1Hours & 0x3F);
		    } 
		break;
	    case ALARM_EVERY_WEEK:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 
		    BCD_TO_BIN(sysDs1375Reg.alarm1DayDate & 0x0F);
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm1Minutes);
		alarm_time->second = BCD_TO_BIN(sysDs1375Reg.alarm1Seconds);
		if (sysDs1375Reg.alarm1Hours & 0x40)
		    {
		    alarm_time->clock = FALSE;
		    alarm_time->hour = 
                        BCD_TO_BIN(sysDs1375Reg.alarm1Hours & 0x1F);
		    if (sysDs1375Reg.alarm1Hours & 0x20)
			alarm_time->pm = TRUE;
		    else
			alarm_time->pm = FALSE;	
		    }
		else
		    {
		    alarm_time->clock = TRUE;
		    alarm_time->hour = 
			BCD_TO_BIN(sysDs1375Reg.alarm1Hours & 0x3F);
		    } 
		break;
	    case ALARM_EVERY_MONTH:
		alarm_time->day_of_month =
		    BCD_TO_BIN(sysDs1375Reg.alarm1DayDate & 0x3F); 
		alarm_time->day_of_week = 0; 
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm1Minutes);
		alarm_time->second = BCD_TO_BIN(sysDs1375Reg.alarm1Seconds);
		if (sysDs1375Reg.alarm1Hours & 0x40)
		    {
		    alarm_time->clock = FALSE;
		    alarm_time->hour = 
                        BCD_TO_BIN(sysDs1375Reg.alarm1Hours & 0x1F);
		    if (sysDs1375Reg.alarm1Hours & 0x20)
			alarm_time->pm = TRUE;
		    else
			alarm_time->pm = FALSE;	
		    }
		else
		    {
		    alarm_time->clock = TRUE;
		    alarm_time->hour = 
			BCD_TO_BIN(sysDs1375Reg.alarm1Hours & 0x3F);
		    } 
		break;
	    default:
		if (verbose)
		    logMsg("ERROR: Invalid method\n\r",0,0,0,0,0,0);
		return (ERROR);
	    }; 
	}
    else if (alarm_time->alarm == 2)
	{
	if ((sysDs1375Reg.alarm2Minutes & DS1375_ALARM_AxMx_MASK)  &&
	    (sysDs1375Reg.alarm2Hours & DS1375_ALARM_AxMx_MASK)  &&
	    (sysDs1375Reg.alarm2DayDate & DS1375_ALARM_AxMx_MASK)) 
	    *method = ALARM_EVERY_MINUTE;
	else if ((!(sysDs1375Reg.alarm2Minutes & DS1375_ALARM_AxMx_MASK)) &&
	         (sysDs1375Reg.alarm2Hours & DS1375_ALARM_AxMx_MASK)  &&
		 (sysDs1375Reg.alarm2DayDate & DS1375_ALARM_AxMx_MASK)) 
	         *method = ALARM_EVERY_HOUR;
	else if ((!(sysDs1375Reg.alarm2Minutes & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm2Hours & DS1375_ALARM_AxMx_MASK)) &&
		 (sysDs1375Reg.alarm2DayDate & DS1375_ALARM_AxMx_MASK))
	         *method = ALARM_EVERY_DAY;
	else if ((!(sysDs1375Reg.alarm2Minutes & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm2Hours & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm2DayDate & DS1375_ALARM_AxMx_MASK)) &&
                 (sysDs1375Reg.alarm2DayDate & 0x40))
	         *method = ALARM_EVERY_WEEK;
	else if ((!(sysDs1375Reg.alarm2Minutes & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm2Hours & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm2DayDate & DS1375_ALARM_AxMx_MASK)) &&
		 (!(sysDs1375Reg.alarm1DayDate & 0x40)))
	         *method = ALARM_EVERY_MONTH;
	else
	    {
	    if (verbose)
		logMsg("ERROR: Invalid method\n\r",0,0,0,0,0,0); 
	    return (ERROR);
	    }
	switch (*method)
	    {
	    case ALARM_EVERY_MINUTE:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 0;
		alarm_time->hour = 0;
		alarm_time->minute = 0;
		alarm_time->second = 0;
		alarm_time->clock = FALSE;
		alarm_time->pm = FALSE;
		break;
	    case ALARM_EVERY_HOUR:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 0;
		alarm_time->hour = 0;
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm2Minutes);
		alarm_time->second = 0;
		alarm_time->clock = FALSE;
		alarm_time->pm = FALSE;
		break;
	    case ALARM_EVERY_DAY:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 0;
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm2Minutes);
		alarm_time->second = 0;
		if (sysDs1375Reg.alarm2Hours & 0x40)
		    {
		    alarm_time->clock = FALSE;
		    alarm_time->hour = 
                        BCD_TO_BIN(sysDs1375Reg.alarm2Hours & 0x1F);
		    if (sysDs1375Reg.alarm2Hours & 0x20)
			alarm_time->pm = TRUE;
		    else
			alarm_time->pm = FALSE;	
		    }
		else
		    {
		    alarm_time->clock = TRUE;
		    alarm_time->hour = 
			BCD_TO_BIN(sysDs1375Reg.alarm2Hours & 0x3F);
		    } 
		break;
	    case ALARM_EVERY_WEEK:
		alarm_time->day_of_month = 0;
		alarm_time->day_of_week = 
		    BCD_TO_BIN(sysDs1375Reg.alarm2DayDate & 0x0F);
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm2Minutes);
		alarm_time->second = 0;
		if (sysDs1375Reg.alarm2Hours & 0x40)
		    {
		    alarm_time->clock = FALSE;
		    alarm_time->hour = 
                        BCD_TO_BIN(sysDs1375Reg.alarm2Hours & 0x1F);
		    if (sysDs1375Reg.alarm2Hours & 0x20)
			alarm_time->pm = TRUE;
		    else
			alarm_time->pm = FALSE;	
		    }
		else
		    {
		    alarm_time->clock = TRUE;
		    alarm_time->hour = 
			BCD_TO_BIN(sysDs1375Reg.alarm2Hours & 0x3F);
		    } 
		break;
	    case ALARM_EVERY_MONTH:
		alarm_time->day_of_month =
		    BCD_TO_BIN(sysDs1375Reg.alarm2DayDate & 0x3F); 
		alarm_time->day_of_week = 0; 
		alarm_time->minute = BCD_TO_BIN(sysDs1375Reg.alarm2Minutes);
		alarm_time->second = 0;
		if (sysDs1375Reg.alarm2Hours & 0x40)
		    {
		    alarm_time->clock = FALSE;
		    alarm_time->hour = 
                        BCD_TO_BIN(sysDs1375Reg.alarm2Hours & 0x1F);
		    if (sysDs1375Reg.alarm2Hours & 0x20)
			alarm_time->pm = TRUE;
		    else
			alarm_time->pm = FALSE;	
		    }
		else
		    {
		    alarm_time->clock = TRUE;
		    alarm_time->hour = 
			BCD_TO_BIN(sysDs1375Reg.alarm2Hours & 0x3F);
		    } 
		break;
	    default:
		if (verbose)
		    logMsg("ERROR: Invalid method\n\r",0,0,0,0,0,0);
		return (ERROR);
	    }; 
	}
    else
	{
	if (verbose)
	    logMsg("ERROR: Invalid alarm number\n\r",0,0,0,0,0,0);
	return (ERROR);
	}
    return (OK);
    }

/******************************************************************************
*
* sysDs1375RtcGet - current snapshot of RTC settings
*
* This routine will capture the settings for the RTC.  <rtc_time> will be
* populated with the current RTC settings. 
*
* RETURNS: OK, if able to capture the RTC settings, ERROR otherwise.
*/

STATUS sysDs1375RtcGet
    (
    RTC_DATE_TIME * rtc_time      /* pointer to time keeping structure */
    )
    {

    /* Get current snaphot of registers */

    if (sysDs1375RegGet() == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to retrieve settings\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    /* Day of Week */

    rtc_time->day_of_week = sysDs1375Reg.day;

    /* Month & Century */

    rtc_time->month = BCD_TO_BIN(sysDs1375Reg.monthCentury & 0x1F);
    if (sysDs1375Reg.monthCentury & 0x80)
	rtc_time->century = (21);
    else
	rtc_time->century = (20);

    /* Day of Month (Date) */
   
    rtc_time->day_of_month = BCD_TO_BIN(sysDs1375Reg.date);

    /* Hour */

    if (sysDs1375Reg.hours & 0x40)
	{
	
	/* 12 hour mode */

	rtc_time->clock = FALSE;
	if (sysDs1375Reg.hours & 0x20)
	    rtc_time->pm = TRUE;
	else
	    rtc_time->pm = FALSE;
        rtc_time->hour = BCD_TO_BIN(sysDs1375Reg.hours&0x1F);
	}
    else
	{

	/* 24 hour mode */

	rtc_time->clock = TRUE;
	rtc_time->hour = BCD_TO_BIN(sysDs1375Reg.hours&0x3F);
	}

    /* Minute */

    rtc_time->minute = BCD_TO_BIN(sysDs1375Reg.minutes);

    /* Second */

    rtc_time->second = BCD_TO_BIN(sysDs1375Reg.seconds);

    /* Year */

    rtc_time->year = BCD_TO_BIN(sysDs1375Reg.year);

    return (OK);
    }

/******************************************************************************
*
* sysDs1375AlarmPrintString - print alarm settings to console
*
* This routine will print the settings for the alarm as indicated by
* <alarm> to the console, without a carriage return.
*
* RETURNS: OK, if able to successfully retrieve the alarm settings, ERROR
* otherwise.
*/

STATUS sysDs1375AlarmPrintString 
    (
    int alarm
    )
    {
    ALARM_DATE_TIME alarm_time;     /* Alarm date and time */
    UCHAR method;
    int day = 0;
    char amPm[3];
    char methodString[17];
    BOOL enabled = FALSE;

    /* Retrieve the current alarm one settings */

    alarm_time.alarm = alarm;
    if (sysDs1375AlarmGet(&method, &alarm_time) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to retrieve alarm settings\n\r",0,0,0,0,0,0);	
	return (ERROR);
	}

    switch (method)
	{
	case ALARM_EVERY_SECOND:
	    sprintf(methodString, "(once per second)");
	    break;
	case ALARM_EVERY_MINUTE:
	    sprintf(methodString, "(once per minute)");
	    break;
	case ALARM_EVERY_HOUR:
	    sprintf(methodString, "(once per hour)");
	    break;
	case ALARM_EVERY_DAY:
	    sprintf(methodString, "(once per day)");
	    break;
	case ALARM_EVERY_WEEK:
	    sprintf(methodString, "(once per week)");
	    day = alarm_time.day_of_week; 
	    break;
	case ALARM_EVERY_MONTH:
	    sprintf(methodString, "(once per month)");
	    day = alarm_time.day_of_month;
	    break;
	default:
	    if (verbose)
		logMsg("ERROR: Invalid method\n\r",0,0,0,0,0,0);
	    return (ERROR);
	};

    if ((method == ALARM_EVERY_DAY) || (method == ALARM_EVERY_WEEK) ||
	(method == ALARM_EVERY_MONTH))
	{
	if (alarm_time.clock == FALSE)
	    {
	    if (alarm_time.pm)
	        sprintf(amPm, "PM ");
	    else
	        sprintf(amPm, "AM ");
	    }
	}
    else
	sprintf(amPm, "");

    if (alarm == 1)
	enabled = (sysDs1375Reg.control & DS1375_CONTROL_STATUS_A1F_MASK);
    else if (alarm == 2)
	enabled = (sysDs1375Reg.control & DS1375_CONTROL_STATUS_A2F_MASK); 

	printf(
	    alarmMsgString,
	    "Alarm",
	    alarm,
	    methodString,
	    day,
	    alarm_time.hour,
	    alarm_time.minute,
	    alarm_time.second,
	    alarm_time.clock ? "" : amPm,
	    enabled ? "enabled" : "disabled"
	    );

    return (OK);
    }

/******************************************************************************
*
* sysDs1375RtcPrintString - print RTC settings to console
*
* This routine will print the current settings of the RTC to the console, 
* without a carriage return.
*
* RETURNS: OK, if able to successfully retrieve the RTC settings, ERROR
* otherwise.
*/

STATUS sysDs1375RtcPrintString (void)
    {
    RTC_DATE_TIME rtc_time;     /* RTC date and time */
    char day[3];
    char month[3];
    char amPm[3];

    /* Retrieve the current RTC date and time */

    if (sysDs1375RtcGet(&rtc_time) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to retrieve RTC settings\n\r",0,0,0,0,0,0);	
	return (ERROR);
	}

    switch(rtc_time.day_of_week)
	{
	case 1:
	    sprintf(day, "SUN");
	    break;
	case 2:
	    sprintf(day, "MON");
	    break;
	case 3:
	    sprintf(day, "TUE");
	    break;
	case 4:
	    sprintf(day, "WED");
	    break;
	case 5:
	    sprintf(day, "THU");
	    break;
	case 6:
	    sprintf(day, "FRI");
	    break;
	case 7:
	    sprintf(day, "SAT");
	    break;
	default:
	    sprintf(day, "N/A");
	    break;
	}

    switch(rtc_time.month)
	{
	case 1:
	    sprintf(month, "JAN");
	    break;
	case 2:
	    sprintf(month, "FEB");
	    break;
	case 3:
	    sprintf(month, "MAR");
	    break;
	case 4:
	    sprintf(month, "APR");
	    break;
	case 5:
	    sprintf(month, "MAY");
	    break;
	case 6:
	    sprintf(month, "JUN");
	    break;
	case 7:
	    sprintf(month, "JUL");
	    break;
	case 8:
	    sprintf(month, "AUG");
	    break;
	case 9:
	    sprintf(month, "SEP");
	    break;
	case 10:
	    sprintf(month, "OCT");
	    break;
	case 11:
	    sprintf(month, "NOV");
	    break;
	case 12:
	    sprintf(month, "DEC");
	    break;
	default:
	    sprintf(month, "N/A");
	    break;
	}

    if (rtc_time.clock == FALSE)
	{
	if (rtc_time.pm)
	    sprintf(amPm, "PM ");
	else
	    sprintf(amPm, "AM ");
	}

    printf(
	rtcMsgString,
	day,
	month,
	rtc_time.day_of_month,
	rtc_time.hour,
	rtc_time.minute,
	rtc_time.second,
	rtc_time.clock ? "" : amPm,
	rtc_time.century,
	rtc_time.year
	);	
	
    return (OK);
    }

/******************************************************************************
*
* sysDs1375RtcShow - Display RTC settings to console with carriage return
*
* This routine will display the current RTC settings to the console with
* a carriage return at the end.
*
* RETURNS: OK, if able to successfully display the settings, ERROR otherwise.
*/

STATUS sysDs1375RtcShow (void)
    {
    if (sysDs1375RtcPrintString() == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to print RTC string\n\r",0,0,0,0,0,0);
	return (ERROR);
	}
    printf("\n\r");
    return (OK); 
    }

/******************************************************************************
*
* sysDs1375AlarmShow - Display alarm settings to console with carriage return
*
* This routine will display the current alarm settings to the console with
* a carriage return at the end.
*
* RETURNS: OK, if able to successfully display the settings, ERROR otherwise.
*/

STATUS sysDs1375AlarmShow (void)
    {

    /* Alarm 1 */

    if (sysDs1375AlarmPrintString(1) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to print Alarm 1 string\n\r",0,0,0,0,0,0);
	return (ERROR);
	}
    printf("\n\r");

    /* Alarm 2 */

    if (sysDs1375AlarmPrintString(2) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to pring Alarm 2 string\n\r",0,0,0,0,0,0);
	return (ERROR);
	}
    printf("\n\r");
    return (OK); 
    }

/******************************************************************************
*
* sysDs1375RegGet - retrieve current DS1375 register settings
*
* This routine will retrieve the current DS1375 register settings.  They
* are stored in the <sysDs1375Reg> structure.
*
* RETURNS: OK, if able to retrieve the register settings, ERROR otherwise. 
*/

LOCAL STATUS sysDs1375RegGet (void)
    {
    UCHAR data;
    int  lockKey;        /* interrupt lock key */
    STATUS status = OK;

    if (!ds1375InitFlag)
	{
	if (sysDs1375Init() == ERROR)
	    {
	    if (verbose)
		logMsg("ERROR: Chip initialization failed\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
	}

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts - to ensure accurate time readings */

    lockKey = intLock();
 
    if (DS1375_READ(DS1375_SECONDS_OFFSET, &data) == ERROR)
	status = ERROR;
    sysDs1375Reg.seconds = data;

    if (DS1375_READ(DS1375_MINUTES_OFFSET, &data) == ERROR)
	status = ERROR;
    sysDs1375Reg.minutes = data;

    if (DS1375_READ(DS1375_HOURS_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.hours = data;

    if (DS1375_READ(DS1375_DAY_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.day = data;

    if (DS1375_READ(DS1375_DATE_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.date = data;

    /* unlock the interrupts */

    intUnlock (lockKey);

    if (DS1375_READ(DS1375_MONTH_CENTURY_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.monthCentury = data;

    if (DS1375_READ(DS1375_YEAR_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.year = data;

    if (DS1375_READ(DS1375_ALARM_1_SECONDS_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.alarm1Seconds = data;

    if (DS1375_READ(DS1375_ALARM_1_MINUTES_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.alarm1Minutes = data;

    if (DS1375_READ(DS1375_ALARM_1_HOURS_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.alarm1Hours = data;

    if (DS1375_READ(DS1375_ALARM_1_DAY_DATE_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.alarm1DayDate = data;

    if (DS1375_READ(DS1375_ALARM_2_MINUTES_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.alarm2Minutes = data;

    if (DS1375_READ(DS1375_ALARM_2_HOURS_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.alarm2Hours = data;

    if (DS1375_READ(DS1375_ALARM_2_DAY_DATE_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.alarm2DayDate = data;

    if (DS1375_READ(DS1375_CONTROL_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.control = data;

    if (DS1375_READ(DS1375_CONTROL_STATUS_OFFSET, &data) == ERROR)
        status = ERROR;
    sysDs1375Reg.controlStatus = data;

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if ((status == ERROR) && (verbose))
	logMsg("ERROR: Unable to retrieve register settings\n\r",0,0,0,0,0,0);
	
    return (status);
    }

/******************************************************************************
*
* sysDs1375AlarmIntDisable - disable the interrupt for the given alarm
*
* This routine will disable the interrupt for the alarm as indicated by
* <alarm>.  This is not meant to be user-callable.
*
* RETURNS: OK, if interrupt successfully disable, ERROR otherwise.
*/

LOCAL STATUS sysDs1375AlarmIntDisable
    (
    int alarm
    )
    {
    UCHAR control;
    int  lockKey;        /* interrupt lock key */

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts */

    lockKey = intLock();

    /* setting AxIE to 0 will disable the interrupt for a given alarm */

    if (DS1375_READ(DS1375_CONTROL_OFFSET, &control) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: read error\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if (alarm == 1)
        control &= ~DS1375_CONTROL_A1IE_MASK;
    else if (alarm == 2)
	control &= ~DS1375_CONTROL_A2IE_MASK;

    if (DS1375_WRITE(DS1375_CONTROL_OFFSET, &control) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: write error\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    return (OK);
    }

/******************************************************************************
*
* sysDs1375AlarmIntEnable - enable the interrupt for the given alarm
*
* This routine will enable the interrupt for the alarm as specified by
* <alarm>.  This is not meant to be user-callable.
*
* RETURNS: OK, if alarm successfully enabled, ERROR otherwise.
*/

LOCAL STATUS sysDs1375AlarmIntEnable
    (
    int alarm
    )
    {
    UCHAR control;
    UCHAR controlStatus;
    int  lockKey;        /* interrupt lock key */

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts */

    lockKey = intLock();

    /* Clear the alarm flags first */

    if (DS1375_READ(DS1375_CONTROL_STATUS_OFFSET, &controlStatus) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Read error\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    controlStatus &= ~(DS1375_CONTROL_STATUS_A1F_MASK |
		       DS1375_CONTROL_STATUS_A2F_MASK);

    if (DS1375_WRITE(DS1375_CONTROL_STATUS_OFFSET, &controlStatus) == ERROR)
	{
        if (verbose)
            logMsg("ERROR: Write error\n\r",0,0,0,0,0,0);
        return (ERROR);
        }

    /* setting AxIE to 1 will enable the interrupt for the given alarm */

    if (DS1375_READ(DS1375_CONTROL_OFFSET, &control) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Read error\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if (alarm == 1)
        control |= DS1375_CONTROL_A1IE_MASK;
    else if (alarm == 2)
	control |= DS1375_CONTROL_A2IE_MASK;

    if (DS1375_WRITE(DS1375_CONTROL_OFFSET, &control) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Write error\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    return (OK);
    }

/******************************************************************************
*
* sysDs1375AlarmSet - configure an alarm
*
* This routine will configure the given alarm, as indicated by the <alarm> 
* field in the ALARM_DATE_TIME structure.  The alarm is configured based 
* on the parameters specified with <method> and <alarm_time>.
*
* RETURNS: OK, if able to successfully set and enable the given alarm, ERROR
* otherwise.
*/

STATUS sysDs1375AlarmSet 
    (
    UCHAR method, 
    ALARM_DATE_TIME * alarm_time
    )
    {
    int  lockKey;        /* interrupt lock key */
    UCHAR seconds;
    UCHAR minutes;
    UCHAR hours;
    UCHAR dayDate;
    STATUS status = OK;

    /* Check Parameters */

    if (sysDs1375AlarmParamsCheck(method, alarm_time) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Invalid alarm parameters\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if (!ds1375InitFlag)
	{
	if (sysDs1375Init() == ERROR)
	    {
	    if (verbose)
		logMsg("ERROR: Chip initialization failed\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
	}

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts - to ensure accurate time readings */

    lockKey = intLock();

    seconds = DS1375_ALARM_AxMx_MASK;
    minutes = DS1375_ALARM_AxMx_MASK;
    hours = DS1375_ALARM_AxMx_MASK;
    dayDate = DS1375_ALARM_AxMx_MASK;

    switch (method)
	{
	case ALARM_EVERY_SECOND:
	    if (alarm_time->alarm != 1)
		status = ERROR;
	    break;

	case ALARM_EVERY_MINUTE:
	    if (alarm_time->alarm == 1)
		{
		seconds = BIN_TO_BCD(alarm_time->second);
		}
	    else if (alarm_time->alarm != 2)
		status = ERROR;
	    break;

	case ALARM_EVERY_HOUR:
	    if (alarm_time->alarm == 1)
		{
		seconds = BIN_TO_BCD(alarm_time->second);
		minutes = BIN_TO_BCD(alarm_time->minute);
		}
	    else if (alarm_time->alarm == 2)
		{
	    	minutes = BIN_TO_BCD(alarm_time->minute);
		}
	    else
		status = ERROR;
	    break;

	case ALARM_EVERY_DAY:
	    if (alarm_time->alarm == 1)
		{
		seconds = BIN_TO_BCD(alarm_time->second);
		minutes = BIN_TO_BCD(alarm_time->minute);
		hours = BIN_TO_BCD(alarm_time->hour);
		if (!alarm_time->clock)
		    {
		    hours |= 0x40;
		    if (alarm_time->pm)
			hours |= 0x20;
		    }	
		}
	    else if (alarm_time->alarm == 2)
		{
	    	minutes = BIN_TO_BCD(alarm_time->minute);
	    	hours = BIN_TO_BCD(alarm_time->hour); 
		if (!alarm_time->clock)
		    {
		    hours |= 0x40;
		    if (alarm_time->pm)
			hours |= 0x20;
		    }	
		}
	    else
		status = ERROR;
	    break;

	case ALARM_EVERY_WEEK:
	    if (alarm_time->alarm == 1)
		{
		seconds = BIN_TO_BCD(alarm_time->second);
		minutes = BIN_TO_BCD(alarm_time->minute);
		hours = BIN_TO_BCD(alarm_time->hour);
		if (!alarm_time->clock)
		    {
		    hours |= 0x40;
		    if (alarm_time->pm)
			hours |= 0x20;
		    }	
	    	dayDate = BIN_TO_BCD(alarm_time->day_of_week);
		dayDate |= 0x40; 
		}
	    else if (alarm_time->alarm == 2)
		{
	    	minutes = BIN_TO_BCD(alarm_time->minute);
	    	hours = BIN_TO_BCD(alarm_time->hour); 
		if (!alarm_time->clock)
		    {
		    hours |= 0x40;
		    if (alarm_time->pm)
			hours |= 0x20;
		    }	
	    	dayDate = BIN_TO_BCD(alarm_time->day_of_week); 
                dayDate |= 0x40; 
		}
	    else
		status = ERROR;
	    break;

	case ALARM_EVERY_MONTH:
	    if (alarm_time->alarm == 1)
		{
		seconds = BIN_TO_BCD(alarm_time->second);
		minutes = BIN_TO_BCD(alarm_time->minute);
		hours = BIN_TO_BCD(alarm_time->hour);
		if (!alarm_time->clock)
		    {
		    hours |= 0x40;
		    if (alarm_time->pm)
			hours |= 0x20;
		    }	
	    	dayDate = BIN_TO_BCD(alarm_time->day_of_month);
		}
	    else if (alarm_time->alarm == 2)
		{
	    	minutes = BIN_TO_BCD(alarm_time->minute);
	    	hours = BIN_TO_BCD(alarm_time->hour); 
		if (!alarm_time->clock)
		    {
		    hours |= 0x40;
		    if (alarm_time->pm)
			hours |= 0x20;
		    }	
	    	dayDate = BIN_TO_BCD(alarm_time->day_of_month); 
		}
	    else
		status = ERROR;
	    break;

	default:
	    status = ERROR;
	};

    /* Program the registers */

    if (alarm_time->alarm == 1)
	{
	if (DS1375_WRITE(DS1375_ALARM_1_SECONDS_OFFSET, &seconds) == ERROR)
	    status = ERROR;
	if (DS1375_WRITE(DS1375_ALARM_1_MINUTES_OFFSET, &minutes) == ERROR)
	    status = ERROR;
	if (DS1375_WRITE(DS1375_ALARM_1_HOURS_OFFSET, &hours) == ERROR)
	    status = ERROR;
	if (DS1375_WRITE(DS1375_ALARM_1_DAY_DATE_OFFSET, &dayDate) == ERROR)
	    status = ERROR;
	}
    else if (alarm_time->alarm == 2)
	{
        if (DS1375_WRITE(DS1375_ALARM_2_MINUTES_OFFSET, &minutes) == ERROR)
	    status = ERROR;
        if (DS1375_WRITE(DS1375_ALARM_2_HOURS_OFFSET, &hours) == ERROR)
	    status = ERROR;
        if (DS1375_WRITE(DS1375_ALARM_2_DAY_DATE_OFFSET, &dayDate) == ERROR)
	    status = ERROR;
	}
    else
	{
	status = ERROR;
	}

    if (alarm_time->alarm == 1)
	sysDs1375Reg.alarm1IntHandler = alarm_time->userAlarmHandler;
    else if (alarm_time->alarm == 2)
	sysDs1375Reg.alarm2IntHandler = alarm_time->userAlarmHandler;

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if (sysDs1375AlarmIntEnable(alarm_time->alarm) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to enable interrupt\n\r",0,0,0,0,0,0);
 	status = ERROR;
	}

    if ((status == ERROR) && (verbose))
	logMsg("ERROR: Unable to program alarm\n\r",0,0,0,0,0,0);
	
    return (status);
    }

/******************************************************************************
*
* sysDs1375AlarmCancel - cancel the given alarm
*
* This routine will cancel the given alarm as indicated with <alarm>.
* It simply disables the alarm's interrupt.  The alarm settings are
* left as is.
*
* RETURNS: OK, if able to cancel the given alarm, ERROR otherwise.
*/

STATUS sysDs1375AlarmCancel 
    (
    int alarm
    )
    {
    if (sysDs1375AlarmIntDisable(alarm) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to cancel alarm\n\r",0,0,0,0,0,0);
	return (ERROR);
	}
    return (OK);
    }

/******************************************************************************
*
* sysDs1375RtcSet - configure the RTC 
*
* This routine will configure the RTC.  The RTC is configured based 
* on the parameters specified with <rtc_time>.
*
* RETURNS: OK, if able to successfully set the RTC, ERROR otherwise.
*/

STATUS sysDs1375RtcSet 
    (
    RTC_DATE_TIME * rtc_time   /* pointer to time keeping structure */
    )
    {
    UCHAR data;
    int  lockKey;        /* interrupt lock key */
    STATUS status = OK;

    /* Check Parameters */

    if (sysDs1375RtcParamsCheck(rtc_time) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Invalid parameters for RTC\n\r",0,0,0,0,0,0);
	return (ERROR);
	}
 
    if (!ds1375InitFlag)
	{
	if (sysDs1375Init() == ERROR)
	    {
	    if (verbose)
		logMsg("ERROR: Chip initialization failed\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
	}

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts - to ensure accurate time readings */

    lockKey = intLock();

    /* Seconds */

    data = BIN_TO_BCD(rtc_time->second);
    if (DS1375_WRITE(DS1375_SECONDS_OFFSET, &data) == ERROR)
	status = ERROR;	

    /* Minutes */

    data = BIN_TO_BCD(rtc_time->minute);
    if (DS1375_WRITE(DS1375_MINUTES_OFFSET, &data) == ERROR)
	status = ERROR;

    /* Hours */

    data = BIN_TO_BCD(rtc_time->hour);
    if (!rtc_time->clock)
	{
	data |= 0x40;
	if (rtc_time->pm)
	    data |= 0x20;
	}
    if (DS1375_WRITE(DS1375_HOURS_OFFSET, &data) == ERROR)
	status = ERROR;

    /* Day */

    data = BIN_TO_BCD(rtc_time->day_of_week);
    if (DS1375_WRITE(DS1375_DAY_OFFSET, &data) == ERROR)
	status = ERROR;

    /* Date */

    data = BIN_TO_BCD(rtc_time->day_of_month);
    if (DS1375_WRITE(DS1375_DATE_OFFSET, &data) == ERROR)
	status = ERROR;

    /* Month and Century */

    data = BIN_TO_BCD(rtc_time->month);
    if (rtc_time->century == 21)
	data |= 0x80;
    if (DS1375_WRITE(DS1375_MONTH_CENTURY_OFFSET, &data) == ERROR)
	status = ERROR;

    /* Year */

    data = BIN_TO_BCD(rtc_time->year);
    if (DS1375_WRITE(DS1375_YEAR_OFFSET, &data) == ERROR)
	status = ERROR;

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if ((status == ERROR) && (verbose))
	logMsg("ERROR: Unable to program RTC\n\r",0,0,0,0,0,0);
    return (status);
    }

/******************************************************************************
*
* sysDs1375RtcRegShow - display the RTC registers
*
* This routine will display the current values in the RTC registers.
*
* RETURNS: OK, if able to successfully retrieve the register values, ERROR
* otherwise.
*/

STATUS sysDs1375RtcRegShow (void)
    {

    /* Get current snapshot of registers */

    if (sysDs1375RegGet() == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: Unable to retrieve settings\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    printf("Seconds          (00h) = 0x%02x\n\r", sysDs1375Reg.seconds);	
    printf("Minutes          (01h) = 0x%02x\n\r", sysDs1375Reg.minutes);
    printf("Hours            (02h) = 0x%02x\n\r", sysDs1375Reg.hours);
    printf("Day              (03h) = 0x%02x\n\r", sysDs1375Reg.day);
    printf("Date             (04h) = 0x%02x\n\r", sysDs1375Reg.date);
    printf("Month/Century    (05h) = 0x%02x\n\r", sysDs1375Reg.monthCentury);
    printf("Year             (06h) = 0x%02x\n\r", sysDs1375Reg.year);
    return (OK);
    }

/******************************************************************************
*
* sysDs1375AlarmRegShow - display current alarm register values
*
* This routine will display the current values in the alarm registers.
*
* RETURNS: OK, if able to successfully retrieve the register values, ERROR
* otherwise.
*/

STATUS sysDs1375AlarmRegShow (void)
    {

    /* Get current snapshot of Alarm registers */

    if(sysDs1375RegGet() == ERROR)
        {
        if (verbose)
            logMsg("ERROR: Unable to retrieve settings\n\r",0,0,0,0,0,0);
        return (ERROR);
        }

    printf("Alarm 1 Seconds  (07h) = 0x%02x\n\r", sysDs1375Reg.alarm1Seconds);	
    printf("Alarm 1 Minutes  (08h) = 0x%02x\n\r", sysDs1375Reg.alarm1Minutes);
    printf("Alarm 1 Hours    (09h) = 0x%02x\n\r", sysDs1375Reg.alarm1Hours);
    printf("Alarm 1 Day/Date (0Ah) = 0x%02x\n\r", sysDs1375Reg.alarm1DayDate);
    printf("Alarm 2 Minutes  (0Bh) = 0x%02x\n\r", sysDs1375Reg.alarm2Minutes);
    printf("Alarm 2 Hours    (0Ch) = 0x%02x\n\r", sysDs1375Reg.alarm2Hours);
    printf("Alarm 2 Day/Date (0Dh) = 0x%02x\n\r", sysDs1375Reg.alarm2DayDate);
    return (OK);
    }

/******************************************************************************
*
* sysDs1375ControlRegShow - display current control register values
*
* This routine will display the current values in the control registers.
*
* RETURNS: OK, if able to successfully retrieve the register values, ERROR
* otherwise.
*/

STATUS sysDs1375ControlRegShow (void)
    {

    /* Get current snapshot of Alarm registers */

    if (sysDs1375RegGet() == ERROR)
        {
        if (verbose)
            logMsg("ERROR: Unable to retrieve settings\n\r",0,0,0,0,0,0);
        return (ERROR);
        }

    printf("Control          (0Eh) = 0x%02x\n\r", sysDs1375Reg.control);	
    printf("Control/Status   (0Fh) = 0x%02x\n\r", sysDs1375Reg.controlStatus);
    return (OK);
    }

/******************************************************************************
*
* sysDs1375RegShow - display current register values for all registers
*
* This routine will display the current values in the RTC, alarm and control
* registers.
*
* RETURNS: OK, if able to successfully retrieve the register values, ERROR
* otherwise.
*/

STATUS sysDs1375RegShow (void)
    {
    if (sysDs1375RtcRegShow() == ERROR)
        {
        if (verbose)
            logMsg("ERROR: Cannot display RTC registers\n\r",0,0,0,0,0,0);
        return (ERROR);
        }

    if (sysDs1375AlarmRegShow() == ERROR)
        {
        if (verbose)
            logMsg("ERROR: Cannot display alarm registers\n\r",0,0,0,0,0,0);
        return (ERROR);
        }

    if (sysDs1375ControlRegShow() == ERROR)
        {
        if (verbose)
            logMsg("ERROR: Cannot display control registers\n\r",0,0,0,0,0,0);
        return (ERROR);
        }
    return (OK);
    }

/******************************************************************************
*
* sysDs1375SramWrite - write to the SRAM
*
* This routine will write to the DS1375's SRAM.  You provide the <offset>
* into the SRAM, the number of bytes to be written <numBytes>, and a 
* buffer pointing to the data to be written <pBfr>.
*
* RETURNS: OK, if data written successfully, ERROR otherwise.
*/

STATUS sysDs1375SramWrite
    (
    int offset,		/* offset into SRAM */
    int numBytes,	/* number of bytes to write */
    UINT8  *pBfr	/* source buffer */
    )
    {
    int  lockKey;        /* interrupt lock key */
    int x;
    STATUS status = OK;

    /* check parameters */

    if ((offset < 0) || (offset > 15))
	{
	if (verbose)
	    logMsg("SRAM Write ERROR: Offset - 0 through 16\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if ((offset + numBytes) > 16)
	{
	if (verbose)
	    logMsg("SRAM Write ERROR: offset + bytes <= 16\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if (!ds1375InitFlag)
        {
        if (sysDs1375Init() == ERROR)
	    {
	    if (verbose)
		logMsg("ERROR: Chip initialization failed\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
        }

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts - to ensure accurate time readings */

    lockKey = intLock();

    for (x = 0; x < numBytes; x++)
	{
	if (DS1375_WRITE(DS1375_SRAM_OFFSET + offset + x, &pBfr[x]) == ERROR)
	    status = ERROR;
	}

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if ((status == ERROR) && (verbose))
	logMsg("ERROR: SRAM write failed\n\r",0,0,0,0,0,0);

    return (status); 
    }

/******************************************************************************
*
* sysDs1375SramRead - read from the SRAM
*
* This routine will read from the DS1375's SRAM.  You must provide the
* <offset> into the SRAM, the number of bytes to be read <numBytes>, and
* a buffer to hold the data that is read <pBfr>.
*
* RETURNS: OK, if data read successfully, ERROR otherwise.
*/

STATUS sysDs1375SramRead
    (
    int offset,		/* offset into SRAM */
    int numBytes,	/* number of bytes to read */
    UINT8  *pBfr	/* destination buffer */
    )
    {
    int x;
    int  lockKey;        /* interrupt lock key */
    STATUS status = OK;

    /* check parameters */

    if ((offset < 0) || (offset > 15))
	{
	if (verbose)
	    logMsg("SRAM Read ERROR: Offset - 0 through 16\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if ((offset + numBytes) > 16)
	{
	if (verbose)	
	    logMsg("SRAM Read ERROR: offset + bytes <= 16\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    if (!ds1375InitFlag)
        {
        if (sysDs1375Init() == ERROR)
	    {
	    if (verbose)
		logMsg("ERROR: Chip initialization failed\n\r",0,0,0,0,0,0);
	    return (ERROR);
	    }
        }

    /* Protect access to this resource via semaphore "take" */

    semTake (sysDs1375Reg.semDrvId, WAIT_FOREVER);

    /* Block interrupts - to ensure accurate time readings */

    lockKey = intLock();

    for (x = 0; x < numBytes; x++)
	{
	if (DS1375_READ(DS1375_SRAM_OFFSET + offset + x, &pBfr[x]) == ERROR)
	    status = ERROR;
	}

    /* unlock the interrupts */

    intUnlock (lockKey);

    /* "free" resource */

    semGive (sysDs1375Reg.semDrvId);

    if ((status == ERROR) && (verbose))
	logMsg("ERROR: SRAM read failed\n\r",0,0,0,0,0,0);
    return (status); 
    }

/******************************************************************************
*
* sysDs1375SramShow - display contents of SRAM
*
* This routine will display the contents (all 16 bytes) to the console.
*
* RETURNS: OK, if able to retrieve the SRAM contents successfully, ERROR
* otherwise.
*/

STATUS sysDs1375SramShow (void)
    {
    UCHAR data[16];
    int i;

    if (sysDs1375SramRead(0, DS1375_SRAM_SIZE/2, &data[0]) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: SRAM read failed\n\r",0,0,0,0,0,0);
	return (ERROR);
	}

    printf("10: ");
    for (i = 0; i < DS1375_SRAM_SIZE/2; i++)
	{
	printf("%02x ", data[i]);
	}

    printf("\n\r");

    if (sysDs1375SramRead(8, DS1375_SRAM_SIZE/2, &data[0]) == ERROR)
	{
	if (verbose)
	    logMsg("ERROR: SRAM read failed\n\r",0,0,0,0,0,0);
        return (ERROR);
	}

    printf("18: ");
    for (i = 0; i < DS1375_SRAM_SIZE/2; i++)
        {
        printf("%02x ", data[i]);
        }

    printf("\n\r");
    return (OK);
    }

/***************************************************************************
*
* sysSecsToCal - Calculate Julian date.
*
* This routine calculates the Julian Date from the time in seconds
* since January 1, 1970.
*
* RETURNS: Nothing.
*
* SEE ALSO: calToSysSecs()
*/

void sysSecsToCal
     (
     time_t time,       /* time in seconds since January 1, 1970 */
     RTC_DATE_TIME *jt  /* pointer to Julian date/time structure */
     )
     {
     int l, n, i, jd, jd_f, j;

     /* Calculate Julian Day First */

     jd = 2440588 + (time/86400);

     /* Now the remainder */

     jd_f = time % 86400;

     /* Calculate the day, month and year */

     l = jd + 68569;
     n = (4 * l) / 146097;
     l = l - (146097 * n + 3) / 4;
     i = (4000 * (l + 1)) / 1461001;
     l = l - (1461 * i) / 4 + 31;
     j = (80 * l) / 2447;
     jt->day_of_month = l - (2447 * j) / 80;
     l = j / 11;
     jt->month = j + 2 - (12 * l);
     jt->year = 100 * (n - 49) + i + l;
     jt->century = jt->year / 100;
     jt->year = jt->year % 100;

     /* Now calculate time hours, minutes & seconds */

     l = jd_f % 3600;
     jt->hour = jd_f / 3600;
     jt->second = l % 60;
     jt->minute = l / 60;
     }

/***************************************************************************
*
* calToSysSecs - Calculates elapse time in seconds.
*
* This routine calculates total seconds elapsed since January 1, 1970.
*
* RETURNS: Seconds since January 1, 1970.
*
* SEE ALSO: sysSecsToCal()
*/

time_t calToSysSecs
     (
     const RTC_DATE_TIME *jt    /* Julian Date/Time */
     )
     {
     int jd, jd_f;
     int year;

     /* Calculate the fractional part of the day in seconds */

     jd_f = ((jt->hour * 60) + jt->minute) * 60 + jt->second;

     /* Add the century to the year */

     year = jt->year + jt->century*100;

     /* Now calculate the Julian Day */

     /* Convert to time since January 1, 1900 */

     jd = (1461 * (year + 4800 + (jt->month - 14) / 12)) / 4 +
          (367 * (jt->month - 2 - 12 * ((jt->month - 14) / 12))) / 12 -
          (3 * ((year + 4900 + (jt->month - 14) / 12) / 100)) / 4 +
          jt->day_of_month - 32075;
     jd -= 2440588; /* January 1, 1970 */
     jd = jd*86400 + jd_f;

     return ((time_t) jd);
     }

/***************************************************************************
*
* setAnsiSysTime - Sets the ANSI C clock time from the RTC clock
*
* This routine sets the ANSI C clock time from the RTC clock
*
* RETURNS: Nothing.
*
* SEE ALSO: sysDs1375RtcGet(), calToSysSecs(), clock_settime()
*/

void setAnsiSysTime
     (
     void
     )
     {
     RTC_DATE_TIME timecal;
     struct timespec tp;

     while (sysDs1375RtcGet (&timecal) == ERROR);
     tp.tv_sec = calToSysSecs(&timecal);
     tp.tv_nsec = 0;
     clock_settime(CLOCK_REALTIME, &tp);
     }

#ifdef DEBUG_RTC
/***************************************************************************
*
* sysDs1375Test - test the DS1375 
*
* This routine is a test for the DS1375.
*
* RETURNS: OK, if test passed, ERROR otherwise.
*/

void sysDs1375Test (void)
    {
    UCHAR testData[8] = {0xFF,0xFF,0xEE,0xEE,0xFF,0xFF,0xEE,0xEE};
    int x;

    sysDs1375SramWrite(0, 8, &testData[0]);
        for (x = 0; x < 8; x++)
    printf("%02x\n\r", testData[x]);
    sysDs1375SramShow();
    }
#endif /* DEBUG_RTC */

/* m5200Rtc.c - MPC5200 RTC support library */

/*
 * Copyright (c) 2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,07jul06,g_h  added comments
01a,22feb06,pkr  written.
*/

/*

DESCRIPTION

This library contains support for the MPC5200 RTC

*/
#ifdef INCLUDE_MPC5200_RTC

/* includes */

#include "vxWorks.h"
#include "time.h"
#include "stdio.h"

/*******************************************************************************
* m5200RtcSet - set MPC5200 Real time clock.
*
* This routine set a time value in the MPC5200 real time clock.
*
* RETURNS: OK or ERROR for invalid arguments.
*
* SEE ALSO: m5200RtcGet()
*/
STATUS m5200RtcSet
	(
	struct tm *timeToSet
	)
	{
	UINT32 rtcValue;

	if (timeToSet == NULL)
		return ERROR;

	printf("Setting RTC date/time to %s\n", asctime(timeToSet));

	/* start with the date */

	rtcValue = 0;

	rtcValue |= (timeToSet->tm_mon + 1) << RTC_DS_MONTH_SHIFT;
	rtcValue |= (timeToSet->tm_wday + 1) << RTC_DS_WEEKDAY_SHIFT;
	rtcValue |= (timeToSet->tm_mday) << RTC_DS_DAY_SHIFT;

	*RTC_NYS &= ~RTC_NYS_YEAR_MASK;
	*RTC_NYS |= (timeToSet->tm_year + 1920) << RTC_NYS_YEAR_SHIFT;

	/* Now the state machine */

	*RTC_DS = (rtcValue | RTC_DS_PAUSE_TIME);
	*RTC_DS = (rtcValue | RTC_DS_PAUSE_TIME | RTC_DS_SET_TIME);
	*RTC_DS = (rtcValue | RTC_DS_PAUSE_TIME);
	*RTC_DS = (rtcValue);

	/* now the time */

	rtcValue = 0;

	rtcValue |= RTC_TS_SLCT_HOUR_24;
	rtcValue |= (timeToSet->tm_hour) << RTC_TS_C24HOUR_SHIFT;
	rtcValue |= (timeToSet->tm_min) << RTC_TS_MINUTE_SHIFT;
	rtcValue |= (timeToSet->tm_sec) << RTC_TS_SECOND_SHIFT;

	/* Now the state machine */

	*RTC_TS = (rtcValue | RTC_TS_PAUSE_TIME);
	*RTC_TS = (rtcValue | RTC_TS_PAUSE_TIME | RTC_DS_SET_TIME);
	*RTC_TS = (rtcValue | RTC_TS_PAUSE_TIME);
	*RTC_TS = (rtcValue);

	return OK;
	}

/*******************************************************************************
* m5200RtcGet - Read the time from MPC5200 Real time clock.
*
* This routine read the current time from the MPC5200 real time clock.
*
* RETURNS: OK or ERROR if failed to read time.
*
* SEE ALSO: m5200RtcSet()
*/
STATUS m5200RtcGet
	(
	struct tm *timeRead
	)
	{
	UINT32 rtcDateValue1, rtcDateValue2;
	UINT32 rtcTimeValue;
	int year, hour;

	if (timeRead == NULL)
		return ERROR;

	memset(timeRead, 0, sizeof(struct tm));

	/* take care of overrun */

	do
		{
		rtcDateValue1 = *RTC_CD;
		rtcTimeValue = *RTC_CT;
		rtcDateValue2 = *RTC_CD;
		}
	while (rtcDateValue1 != rtcDateValue2);

	year = (rtcDateValue2 & RTC_CD_YEAR_MASK) >> RTC_CD_YEAR_SHIFT;
	if (year >= 1920)
		timeRead->tm_year = year - 1920;
	else
		return ERROR;

	timeRead->tm_mon  = ((rtcDateValue2 & RTC_CD_MONTH_MASK) >>
        RTC_CD_MONTH_SHIFT) - 1;
	timeRead->tm_mday = (rtcDateValue2 & RTC_CD_DAY_MASK) >>
        RTC_CD_DAY_SHIFT;
	timeRead->tm_wday = ((rtcDateValue2 & RTC_CD_WEEKDAY_MASK) >>
        RTC_CD_WEEKDAY_SHIFT) - 1;

	timeRead->tm_sec  = (rtcTimeValue & RTC_CT_SECOND_MASK) >>
        RTC_CT_SECOND_SHIFT;
	timeRead->tm_min  = (rtcTimeValue & RTC_CT_MINUTE_MASK) >>
        RTC_CT_MINUTE_SHIFT;

	hour = (rtcTimeValue & RTC_CT_HOUR_MASK) >> RTC_CT_HOUR_SHIFT;

	if ((*RTC_TS & RTC_TS_SLCT_HOUR_12) == RTC_TS_SLCT_HOUR_12)
		{
		if ((hour & 0x10) == 1)
			/* PM */
			timeRead->tm_hour = 12 + (hour & 0xF);
		else
			/* AM */
			timeRead->tm_hour = hour & 0xF;
		}
	else
		timeRead->tm_hour = hour;

	printf("RTC date/time read %s\n", asctime(timeRead));

	return OK;
	}
#endif /* INCLUDE_MPC5200_RTC */

/* ds1339Rtc.h - ds1339Rtc header */

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
01a,26jun07,l_g  initial creation 
*/

/*
This file contains the configuration parameters and control functions for 
the ds1339 Real-time clock 
*/

#ifndef	INCds1339Rtc
#define	INCds1339Rtc

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* includes */

#include <dosFsLib.h>

/* This structure holds the Real-Time Clock configuration values */

typedef struct rtcdt 
    {
    unsigned long year;		    	/* year */
    unsigned long month;			/* month */
    unsigned long day_of_month; 	/* day of month */
    unsigned long day_of_week;		/* day of week */
    unsigned long hour;			    /* hour */
    unsigned long minute;		    /* minute */
    unsigned long second;		    /* second */
    } RTC_DATE_TIME; 


/* defines */

/*
 * RTC register addresses
 */
#define DS1339_SEC_REG  	0x0
#define DS1339_MIN_REG	    0x1
#define DS1339_HOUR_REG		0x2
#define DS1339_DAY_REG	    0x3
#define DS1339_DATE_REG	    0x4
#define DS1339_MONTH_REG	0x5
#define DS1339_YEAR_REG		0x6


/*
 * the following macros convert from BCD to binary and back.
 * Be careful that the arguments are chars, only char width returned.
 */

#define BCD_TO_BIN(bcd) (( ((((bcd)&0xf0)>>4)*10) + ((bcd)&0xf) ) & 0xff)
#define BIN_TO_BCD(bin) (( (((bin)/10)<<4) + ((bin)%10) ) & 0xff)


/* global functions */

STATUS ds1339RtcSet (RTC_DATE_TIME * rtc_time);
STATUS ds1339RtcShow (void);
STATUS ds1339RtcGet (RTC_DATE_TIME * rtc_time);
void   ds1339RtcDateTimeHook (DOS_DATE_TIME * pDateTime);


#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif	/* INCds1339Rtc */

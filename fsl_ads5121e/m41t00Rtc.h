/* m41t00Rtc.h - m41t00 RTC header */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,05feb09,x_f  initial creation
*/

/*
This file contains the configuration parameters and control functions for 
the m41t00 real-time clock. It also supports m41t62 real-time clock but with
the basic get and set timer functions.
*/

#ifndef __INCm41t00Rtc
#define __INCm41t00Rtc

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* includes */

#include <dosFsLib.h>
#include "fsl_ads5121e.h"

/* This structure holds the Real-Time Clock configuration values */

typedef struct rtcdt
    {
    unsigned long year;         /* year */
    unsigned long month;        /* month */
    unsigned long dayOfMonth;   /* day of month */
    unsigned long dayOfWeek;    /* day of week */
    unsigned long hour;         /* hour */
    unsigned long minute;       /* minute */
    unsigned long second;       /* second */
    } RTC_DATE_TIME;

/* defines */

/* RTC register addresses */

#ifdef ADS5121E_REV2
#   define M41T00_SEC_REG       0x0
#   define M41T00_MIN_REG       0x1
#   define M41T00_HOUR_REG      0x2
#   define M41T00_DAY_REG       0x3
#   define M41T00_DATE_REG      0x4
#   define M41T00_MONTH_REG     0x5
#   define M41T00_YEAR_REG      0x6
#else /* ADS5121E_REV2 */
#   define M41T62_TENTHS_HUNDREDTHS_REG      0x0
#   define M41T62_SEC_REG       0x1
#   define M41T62_MIN_REG       0x2
#   define M41T62_HOUR_REG      0x3
#   define M41T62_DAY_REG       0x4
#   define M41T62_DATE_REG      0x5
#   define M41T62_MONTH_REG     0x6
#   define M41T62_YEAR_REG      0x7
#endif /* ADS5121E_REV2 */

#ifdef ADS5121E_REV2
#   define M41T_SEC_REG         M41T00_SEC_REG
#   define M41T_MIN_REG         M41T00_MIN_REG
#   define M41T_HOUR_REG        M41T00_HOUR_REG
#   define M41T_DAY_REG         M41T00_DAY_REG
#   define M41T_DATE_REG        M41T00_DATE_REG
#   define M41T_MONTH_REG       M41T00_MONTH_REG
#   define M41T_YEAR_REG        M41T00_YEAR_REG
#else /* ADS5121E_REV2 */
#   define M41T_SEC_REG         M41T62_SEC_REG
#   define M41T_MIN_REG         M41T62_MIN_REG
#   define M41T_HOUR_REG        M41T62_HOUR_REG
#   define M41T_DAY_REG         M41T62_DAY_REG
#   define M41T_DATE_REG        M41T62_DATE_REG
#   define M41T_MONTH_REG       M41T62_MONTH_REG
#   define M41T_YEAR_REG        M41T62_YEAR_REG
#endif /* ADS5121E_REV2 */

/*
 * the following macros convert from BCD to binary and back.
 * Be careful that the arguments are chars, only char width returned.
 */

#define BCD_TO_BIN(bcd) ((((((bcd) & 0xf0) >> 4) * 10) + ((bcd) & 0xf) ) & 0xff)
#define BIN_TO_BCD(bin) (((((bin) / 10) << 4) + ((bin) % 10) ) & 0xff)

#define RTC_SET(time)               m41t00RtcSet ((time))
#define RTC_SHOW()                  m41t00RtcShow ()
#define RTC_GET(time)               m41t00RtcGet ((time))
#define RTC_DATE_TIME_HOOK(hook)    m41t00RtcDateTimeHook ((hook))

/* global functions */

STATUS m41t00RtcShow (void);
STATUS m41t00RtcGet (RTC_DATE_TIME * rtcTime);
STATUS m41t00RtcSet (RTC_DATE_TIME * rtcTime);
void   m41t00RtcDateTimeHook (DOS_DATE_TIME * pDateTime);

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCm41t00Rtc */

/* stm48t59Rtc.c - ST Microelectronics M48T59 TIMEKEEPER RAM */
/* Copyright 1993-2004 Wind River Systems, Inc.              */

/*
modification history
--------------------
01e,23sep04,mdo  Documentation fixes for apigen
01d,13may04,j_b  Base 6 cleanup
01c,09feb04,j_b  fix compiler warnings
01b,15jul03,j_b  interactive alarm set fix
01a,20jun03,j_b  based on wrPpmcIdt334/stm48t59Rtc.c, rev 02a,28aug02
*/


/*
DESCRIPTION
This library contains routines to manipulate the functions on the
ST Microelectronics M48T59 TIMEKEEPER RAM.  These chips
contain a real-time clock, alarm/interval timer, watchdog timer,
and 8176 bytes (8KB - 16 registers) of nonvolatile memory.

There are two potential interrupt sources on this chip: the alarm clock
and the watchdog timer.  The watchdog timer may be configured to assert
the chip's reset signal instead of an interrupt.


EXTERNAL SUPPORT REQUIREMENTS

The following macros must be defined in the Board Support Package (BSP).

The base address of the chip and the number of bytes between
registers must be defined as M48T59_BASE and M48T59_INTERVAL,
respectively.

Wrapper macros are integrated into the register write function for
any enabling and disabling action required by the hardware.  The macro
M48T59_WRITE_EN is intended for boards that must enable writes for
each access.  The macro M48T59_WRITE_DIS is intended to disable write
access after the write operation has occurred.
If the hardware does not require any special treatment for write
operations, simply undefine these macros as:
\cs
    #undef M48T59_WRITE_EN
\ce
*/

#include "private/timeP.h"
#include "stm48t59Rtc.h"
#include "stdio.h"

/* Definitions */

/* The various "bias" values are _added_ to the struct tm fields
 * (found in time.h) when converting to the values that the
 * timekeeper requires.  These values are correspondingly
 * _subtracted_ from the timekeeper native values when loading
 * into a struct tm field on a read.
 */


/* To handle year 2000 rollover, this implementation sets the range
 * of years from 1980 to 2079 when mapping the last 2 digits into the
 * RTC's year register.  Year register values of 80 - 99 are considered
 * to be from the 20th century (1980 and above) while values from 00 - 79
 * correspond to the years 2000 and above.
 *
 * Note: This implementation does not support the M48T59 Century Bit feature.
 */

/* MIN_/MAX_YEAR map the 100 year range of the RTC's year register */
#define MIN_YEAR                  1980
#define MAX_YEAR                  2079
#define BIAS_YEAR                 (-CENTURY)  /* to remove POSIX century bias */
#define MIN_20TH_CENT_YR          (MIN_YEAR - TM_YEAR_BASE)  /* last 2 digits */

/* (UN)BIASED_YEAR determine which century the year corresponds to */
#define BIASED_YEAR(yr)           (((yr) > CENTURY) ? ((yr) + BIAS_YEAR) : (yr))
#define UNBIASED_YEAR(yr)         (((yr) < MIN_20TH_CENT_YR) ? \
                                      ((yr) - BIAS_YEAR) : (yr))

#define MIN_MONTH                 1
#define MAX_MONTH                 12
#define BIAS_MONTH                1

#define LEAP_MONTH                1  /* used for leap day addition */

#define MIN_DAY                   1
#define MAX_DAY                   7
#define BIAS_DAY                  1

#define MIN_MDAY                  1
#define MAX_MDAY                  31
#define BIAS_MDAY                 0

#define LEAP_DATE                 29 /* used in date checking */

#define MIN_HOUR                  0
#define MAX_HOUR                  23
#define BIAS_HOUR                 0

#define MIN_MINUTE                0
#define MAX_MINUTE                59
#define BIAS_MINUTE               0

#define MIN_SECOND                0
#define MAX_SECOND                59
#define BIAS_SECOND               0

#define MIN_HSEC                  0
#define MAX_HSEC                  99
#define BIAS_HSEC                 0

/* Macros */

/* Check that <val> is within the inclusive range <low> and <high> */
#define RANGE_CHECK(val,low,high)  (((val)>=(low)) && ((val)<=(high)))

/* Write a RTC Register, using write enable/disable macros as required */
#if defined (M48T59_WRITE_EN) && defined (M48T59_WRITE_DIS)
#  define WRITE_M48T59_REG(reg, val) \
   M48T59_WRITE_EN; (*(reg) = (val)); M48T59_WRITE_DIS
#else
#  define WRITE_M48T59_REG(reg, val) (*(reg) = (val))
#endif /* M48T59_WRITE_EN && M48T59_WRITE_DIS */

/* 2-Digit binary coded decimal <-> binary conversions */
#define BIN2BCD(bin)  ( ((((bin)&0xff) / 10) << 4) + (((bin)&0xff) % 10) )
#define BCD2BIN(bcd)  ( (((bcd)>>4) * 10) + ((bcd)&0xf) )

/* locals */
LOCAL BOOL dateCheck (int date, int month, int year);

LOCAL int monthToDays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273,
                           304, 334, 365};

/*******************************************************************************
*
* m48t59Reset - initialize the M48T59 to a known state
*
* This routine initializes the M48T59 to a known state.
* The watchdog and alarm timers are disabled, though the
* alarm and watchdog intervals are retained.  The oscillator
* is started if it is not already running.  The time of day
* is not altered, even if the clock was stopped (and the
* time presumed to be incorrect).
* The SRAM is not initialized or otherwise affected.
*
* RETURNS: OK, or ERROR if clock does not start.
*
* ERRNO
*/

STATUS m48t59Reset (void)
    {
    BOOL clock_state;

    /* turn off watchdog timer */
    (void) m48t59WdogDisable();

    /* turn off alarm timer */
    (void) m48t59AlarmDisable();

    /* turn on the clock */
    clock_state = m48t59OscModeSet (TRUE);

    /* return state of clock now */
    return (clock_state) ? OK : ERROR;
    }

/*******************************************************************************
*
* m48t59OscModeSet - control the M48T59 oscillator output
*
* This function turns the M48T59 oscillator output on and off
* for TRUE and FALSE respectively.
*
* RETURNS: Oscillator state (TRUE=on).
*
* ERRNO
*/

BOOL m48t59OscModeSet
    (
    BOOL state  /* TRUE = on, FALSE = off */
    )
    {
    /* high bit disables oscillator */
    if (state)
        {
        WRITE_M48T59_REG(M48T59_SEC, *M48T59_SEC & ~(M48T59_SEC_OSCSTOP));
        }
    else
        {
        WRITE_M48T59_REG(M48T59_SEC, *M48T59_SEC | (M48T59_SEC_OSCSTOP));
        }

    return !(*M48T59_SEC >> 7); /* read hardware for current state */
    }

/*******************************************************************************
*
* m48t59TimeSet - set the time of day in the M48T59 chip
*
* This routine sets the time of day in the M48T59.  It does not
* affect the alarm or watchdog times. Note that the hardware has
* no notion of daylight savings time.
*
* RETURNS: OK, or ERROR if data in time structure is out of range
*
* ERRNO
*/

STATUS m48t59TimeSet
    (
    struct tm * pTheTime    /* time structure (from time.h) */
    )
    {
    STATUS status = OK;

    /* check values before writing */
    if ( (RANGE_CHECK(pTheTime->tm_sec+BIAS_SECOND,MIN_SECOND,MAX_SECOND)) &&
         (RANGE_CHECK(pTheTime->tm_min+BIAS_MINUTE,MIN_MINUTE,MAX_MINUTE)) &&
         (RANGE_CHECK(pTheTime->tm_hour+BIAS_HOUR,MIN_HOUR,MAX_HOUR)) &&
         (RANGE_CHECK(pTheTime->tm_wday+BIAS_DAY,MIN_DAY,MAX_DAY)) &&
         (RANGE_CHECK(pTheTime->tm_mday,MIN_MDAY,MAX_MDAY)) &&
         (RANGE_CHECK(pTheTime->tm_mon+BIAS_MONTH,MIN_MONTH,MAX_MONTH)) &&
         (RANGE_CHECK(pTheTime->tm_year+TM_YEAR_BASE,MIN_YEAR,MAX_YEAR)) )
        {
        /* set the write bit - this does not stop the clock */
        WRITE_M48T59_REG(M48T59_CR, *M48T59_CR | M48T59_CR_WRITE);

        WRITE_M48T59_REG(M48T59_SEC, BIN2BCD(pTheTime->tm_sec+BIAS_SECOND));

        WRITE_M48T59_REG(M48T59_MIN, BIN2BCD(pTheTime->tm_min+BIAS_MINUTE));

        WRITE_M48T59_REG(M48T59_HOURS, BIN2BCD(pTheTime->tm_hour+BIAS_HOUR));

        WRITE_M48T59_REG(M48T59_DAYS, BIN2BCD(pTheTime->tm_wday+BIAS_DAY));

        WRITE_M48T59_REG(M48T59_DATE, BIN2BCD(pTheTime->tm_mday+BIAS_MDAY));

        WRITE_M48T59_REG(M48T59_MONTH, BIN2BCD(pTheTime->tm_mon+BIAS_MONTH));

        WRITE_M48T59_REG(M48T59_YEAR, BIN2BCD(BIASED_YEAR(pTheTime->tm_year)));

        /* unset the write bit - the update occurs within approx 1 sec */
        WRITE_M48T59_REG(M48T59_CR, *M48T59_CR & ~M48T59_CR_WRITE);
        }
    else
        status = ERROR;

    return status;
    }

/*******************************************************************************
*
* m48t59TimeGet - read the time of day from the M48T59 chip
*
* This routine reads the time of day from the M48T59 into a
* time structure as described in time.h.  The hardware has no
* notion of daylight savings time, so the daylight savings time
* field is not filled in.
*
* RETURNS: OK.
*
* ERRNO
*/

STATUS m48t59TimeGet
    (
    struct tm * pTheTime    /* time returned into this structure */
    )
    {
    /* set the read bit - this does not stop the clock */
    WRITE_M48T59_REG(M48T59_CR, *M48T59_CR | M48T59_CR_READ);

    pTheTime->tm_sec = BCD2BIN(*M48T59_SEC) - BIAS_SECOND;
    pTheTime->tm_min = BCD2BIN(*M48T59_MIN) - BIAS_MINUTE;
    pTheTime->tm_hour = BCD2BIN(*M48T59_HOURS) - BIAS_HOUR;
    pTheTime->tm_mday = BCD2BIN(*M48T59_DATE) - BIAS_MDAY;
    pTheTime->tm_mon = BCD2BIN(*M48T59_MONTH) - BIAS_MONTH;
    pTheTime->tm_year = UNBIASED_YEAR(BCD2BIN(*M48T59_YEAR));
    pTheTime->tm_wday = BCD2BIN( (*M48T59_DAYS & ~(M48T59_DAYS_FREQTST |
                                                  M48T59_DAYS_CENTEN |
                                                  M48T59_DAYS_CENTURY)) )
                        - BIAS_DAY;
    pTheTime->tm_yday = pTheTime->tm_mday + monthToDays[pTheTime->tm_mon];

    /* check for leap year */
    if ( (isleap(pTheTime->tm_year + TM_YEAR_BASE)) &&
         (pTheTime->tm_mon > LEAP_MONTH) )
        {
        pTheTime->tm_yday = pTheTime->tm_yday + 1;
        }

    /* unset the read bit - this does not stop the clock */
    WRITE_M48T59_REG(M48T59_CR, *M48T59_CR & ~M48T59_CR_READ);

    return OK;
    }

/******************************************************************************
*
* m48t59WdogSet - set the watchdog timeout interval in the M48T59 chip
*
* This function sets the watchdog timeout interval. This can range from
* 1/16 second to 124 seconds, determined by a multiplier and a resolution.
* The multiplier can be 0 to 31 and the resolution can be 1/16, 1/4, 1 or
* 4 seconds. Setting both parameters to 0 disables the watchdog timer.
*
* Since the timeout interval begins once the data is written to the hardware,
* the desired watchdog timer signaling mode (interrupt or reset) should be
* set prior to calling this function.
*
* RETURNS: OK, or ERROR if either parameter is out of range.
*
* ERRNO
*
* SEE ALSO: m48t59WdogModeSet(), m48t59WdogDisable()
*
*/

STATUS m48t59WdogSet
    (
    INT8 * pMult,        /* multiplier, 0 - 31 */
    INT8 * pResolution   /* resolution, 0=1/16 sec, 1=1/4 sec, 2=1sec, 3=4sec */
    )
    {
    UINT8    wdog_byte;
    STATUS   status = OK;

    if ( (*pMult <= MAX_WDOG_MULT) && (*pResolution <= MAX_WDOG_RESOLUTION) )
        {
        wdog_byte = (m48t59WdogModeGet() << 7) | (*pMult << 2) | *pResolution;
        WRITE_M48T59_REG(M48T59_WDOGBITS,  wdog_byte);
        }
    else
        status = ERROR;

    return status;
    }

/******************************************************************************
*
* m48t59WdogGet - get the watchdog timeout interval in the M48T59 chip
*
* This function reads the watchdog timeout interval from the M48T59 chip.
* The multiplier and resolution portions are returned into separate
* pointers. If both values are zero, the watchdog timer is disabled.
*
* Note that the watchdog register does not provide a running count down
* of the time remaining before expiration.  The watchdog register contains:
* the last timeout interval written or zero upon reset or if disabled.
*
* RETURNS: OK.
*
* ERRNO
*
* SEE ALSO: m48t59WdogModeGet()
*
*/

STATUS m48t59WdogGet
    (
    INT8 * pMult,        /* multiplier, 0 - 31 */
    INT8 * pResolution   /* resolution, 0=1/16 sec, 1=1/4 sec, 2=1sec, 3=4sec */
    )
    {
    *pMult = (*M48T59_WDOGBITS & M48T59_WDOG_MULT) >> 2;
    *pResolution = *M48T59_WDOGBITS & M48T59_WDOG_RESOLUTION;

    return OK;
    }

/******************************************************************************
*
* m48t59WdogModeSet - set the M48T59 watchdog timer signaling mode
*
* This routine steers the M48T59 watchdog timeout condition to
* assert an interrupt or reset.
*
* Note that the watchdog timer signaling mode is set to zero (interrupt)
* upon expiration of the watchdog timer.
*
* RETURNS: OK
*
* ERRNO
*
* SEE ALSO: m48t59WdogSet()
*
*/

STATUS m48t59WdogModeSet
    (
    BOOL mode  /* TRUE = reset, FALSE = interrupt */
    )
    {
    if (mode)
        {
        /* M48T59_WDOG_RST */
        WRITE_M48T59_REG(M48T59_WDOGBITS, (*M48T59_WDOGBITS | M48T59_WDOG_WDS));
        }
    else
        {
        /* M48T59_WDOG_IRQ */
        WRITE_M48T59_REG(M48T59_WDOGBITS,
                         (*M48T59_WDOGBITS & ~M48T59_WDOG_WDS));
        }

    return OK;
    }

/******************************************************************************
*
* m48t59WdogModeGet - get the M48T59 watchdog timer signaling mode
*
* This routine returns the M48T59 watchdog timer expiration behavior:
* assert an interrupt or reset.
*
* RETURNS: watchdog timer signaling mode: 0 = interrupt, 1 = reset
*
* ERRNO
*
* SEE ALSO: m48t59WdogSet(), m48t59WdogGet()
*
*/

UINT8 m48t59WdogModeGet (void)
    {
    return ((UINT8)((*M48T59_WDOGBITS & M48T59_WDOG_WDS) >> 7));
    }

/******************************************************************************
*
* m48t59WdogDisable - disable the M48T59 watchdog timer
*
* This routine disables the M48T59 watchdog timer countdown by writing
* zero to the watchdog register.
* There is no separate watchdog enable function, since setting a non-zero
* watchdog timer value immediately enables it.
*
* RETURNS: OK
*
* ERRNO
*
* SEE ALSO: m48t59WdogSet()
*
*/

STATUS m48t59WdogDisable (void)
    {
    WRITE_M48T59_REG(M48T59_WDOGBITS,  0x0);
    return OK;
    }


/******************************************************************************
*
* m48t59AlarmSet - set the M48T59 alarm time
*
* This function sets the alarm time in the M48T59 chip. The alarm
* may be configured to repeat every month, day, hour or second.
*
* The alarm repeat mode is determined by masking the appropriate time fields.
* A time field is masked by setting its most-significant data bit.  Valid
* repeat modes are listed in the table below.  Invalid combinations of
* masked time fields will result in a return value of ERROR .
* The value of masked time fields will be set to NONE.
*
* \cs
* Alarm                tm_mday  tm_hour  tm_min  tm_sec
* =====================================================
* once per month         >=0     >=0      >=0     >=0
* once per day           <0      >=0      >=0     >=0
* once per hour          <0       <0      >=0     >=0
* once per minute        <0       <0       <0     >=0
* once per second        <0       <0       <0      <0
* \ce
*
* Only the date, hour, minute and second fields of the time structure
* are used to determine the alarm time.
*
* In ERROR cases, the programmed alarm time should be considered to be in
* a corrupted state and should not be enabled.
*
* RETURNS: OK or ERROR for invalid time field or alarm repeat mode
*
* ERRNO
*
* SEE ALSO: m48t59AlarmEnable(), m48t59AlarmGet()
*
*/

STATUS m48t59AlarmSet
    (
    struct tm * alarmTime
    )
    {
    struct tm clkTime;

    /* use current month and year to validate the date */
    if (m48t59TimeGet(&clkTime))
        return ERROR;

    /* negative time fields are interpreted as masked */

    if (alarmTime->tm_mday < 0)
        {
        WRITE_M48T59_REG(M48T59_DALARM, (char)NONE);  /* >= once per day */
        }
    else if (dateCheck(alarmTime->tm_mday, clkTime.tm_mon+BIAS_MONTH,
                       clkTime.tm_year+TM_YEAR_BASE))
        {
        WRITE_M48T59_REG(M48T59_DALARM, BIN2BCD(alarmTime->tm_mday));
        }
    else
        /* invalid day of the month */
        return ERROR;

    if (alarmTime->tm_hour < 0)
        {
        if (alarmTime->tm_mday < 0)
            {
            WRITE_M48T59_REG(M48T59_HALARM, (char)NONE);  /* >= once per hour */
            }
        else
            /* invalid alarm repeat mode */
            return ERROR;
        }
    else if (RANGE_CHECK(alarmTime->tm_hour+BIAS_HOUR,MIN_HOUR,MAX_HOUR))
        {
        WRITE_M48T59_REG(M48T59_HALARM, BIN2BCD(alarmTime->tm_hour));
        }
    else
        /* invalid hour */
        return ERROR;

    if (alarmTime->tm_min < 0)
        {
        if ( (alarmTime->tm_mday < 0) && (alarmTime->tm_hour < 0) )
            {
            WRITE_M48T59_REG(M48T59_MALARM, (char)NONE);  /* >= once per minute */
            }
        else
            /* invalid alarm repeat mode */
            return ERROR;
        }
    else if (RANGE_CHECK(alarmTime->tm_min+BIAS_MINUTE,MIN_MINUTE,MAX_MINUTE))
        {
        WRITE_M48T59_REG(M48T59_MALARM, BIN2BCD(alarmTime->tm_min));
        }
    else
        /* invalid minute */
        return ERROR;

    if (alarmTime->tm_sec < 0)
        {
        if ( (alarmTime->tm_mday < 0) && (alarmTime->tm_hour < 0) &&
             (alarmTime->tm_min < 0) )
            {
            WRITE_M48T59_REG(M48T59_SALARM, (char)NONE);  /* once per second */
            }
        else
            /* invalid alarm repeat mode */
            return ERROR;
        }
    else if (RANGE_CHECK(alarmTime->tm_sec+BIAS_SECOND,MIN_SECOND,MAX_SECOND))
        {
        WRITE_M48T59_REG(M48T59_SALARM, BIN2BCD(alarmTime->tm_sec));
        }
    else
        /* invalid second */
        return ERROR;

    return OK;
    }

/******************************************************************************
*
* m48t59AlarmGet - get the M48T59 alarm time
*
* This function reads the alarm time set in the M48T59 chip.  The alarm
* may be configured to repeat every month, day, hour or second.  The repeat
* mode is determined by masking the appropriate time fields.  The value for
* masked time fields is set to NONE.  If there is an incorrect combination
* of masked fields (i.e., a higher resolution field is not masked while a
* lower resolution field is masked), the value returned for all fields
* will be NONE.
*
* Only the date, hour, minute and second fields of the time structure
* are used to determine the alarm time.
*
* RETURNS: OK or ERROR for invalid alarm repeat mode.
*
* ERRNO
*
* SEE ALSO: m48t59AlarmSet()
*
*/

STATUS m48t59AlarmGet
    (
    struct tm * alarmTime   /* alarm time */
    )
    {
    UINT8 repeat_mode;  /* level of masking */
    STATUS status = OK;

    /* combine repeat mode bits */
    repeat_mode =  ((*M48T59_DALARM & M48T59_DALARM_MASK) >> 4) |
                   ((*M48T59_HALARM & M48T59_HALARM_MASK) >> 5) |
                   ((*M48T59_MALARM & M48T59_MALARM_MASK) >> 6) |
                   ((*M48T59_SALARM & M48T59_SALARM_MASK) >> 7);

    /* check for valid mask combination */
    switch (repeat_mode)
        {
        case M48T59_MONTH_ALARM:
            alarmTime->tm_mday = BCD2BIN(*M48T59_DALARM);
            alarmTime->tm_hour = BCD2BIN(*M48T59_HALARM);
            alarmTime->tm_min = BCD2BIN(*M48T59_MALARM);
            alarmTime->tm_sec = BCD2BIN(*M48T59_SALARM);
            break;
        case M48T59_DAY_ALARM:
            alarmTime->tm_mday = NONE;
            alarmTime->tm_hour = BCD2BIN(*M48T59_HALARM);
            alarmTime->tm_min = BCD2BIN(*M48T59_MALARM);
            alarmTime->tm_sec = BCD2BIN(*M48T59_SALARM);
            break;
        case M48T59_HOUR_ALARM:
            alarmTime->tm_mday = NONE;
            alarmTime->tm_hour = NONE;
            alarmTime->tm_min = BCD2BIN(*M48T59_MALARM);
            alarmTime->tm_sec = BCD2BIN(*M48T59_SALARM);
            break;
        case M48T59_MIN_ALARM:
            alarmTime->tm_mday = NONE;
            alarmTime->tm_hour = NONE;
            alarmTime->tm_min = NONE;
            alarmTime->tm_sec = BCD2BIN(*M48T59_SALARM);
            break;
        case M48T59_SEC_ALARM:
            /* once per second alarm */
            alarmTime->tm_mday = NONE;
            alarmTime->tm_hour = NONE;
            alarmTime->tm_min = NONE;
            alarmTime->tm_sec = NONE;
            break;
        default:
            /* invalid mask combination, defaults to once per second */
            alarmTime->tm_mday = NONE;
            alarmTime->tm_hour = NONE;
            alarmTime->tm_min = NONE;
            alarmTime->tm_sec = NONE;
            status = ERROR;
            break;
        }

    return status;
    }

/******************************************************************************
*
* m48T59AlarmEnable - enable the M48T59 alarm
*
* This function enables the M48T59 alarm and optionally enables alarms
* when the M48T59 is in battery backup mode.
*
* RETURNS: OK
*
* ERRNO
*
* SEE ALSO: m48t59AlarmDisable()
*
*/

STATUS m48t59AlarmEnable
    (
    BOOL batt_backup  /* alarm in battery backup mode enable flag */
    )
    {
    if (batt_backup)
        {
        WRITE_M48T59_REG(M48T59_INTS, *M48T59_INTS | M48T59_AFE | M48T59_ABE);
        }
    else
        {
        WRITE_M48T59_REG(M48T59_INTS, (*M48T59_INTS | M48T59_AFE) & ~M48T59_ABE);
        }

    return OK;
    }

/******************************************************************************
*
* m48t59AlarmDisable - disable the M48T59 alarm
*
* This function disables the M48T59 alarm.
* The alarm in battery backup mode flag is not affected, since it does not
* enable the alarm by itself.
*
* RETURNS: OK
*
* ERRNO
*
* SEE ALSO: m48t59AlarmEnable()
*
*/

STATUS m48t59AlarmDisable (void)
    {
    /* disable alarm flag */
    WRITE_M48T59_REG(M48T59_INTS, *M48T59_INTS & ~M48T59_AFE);

    /* clear alarm date and repeat mode flags */
    WRITE_M48T59_REG(M48T59_DALARM, 0x0);
    WRITE_M48T59_REG(M48T59_HALARM, *M48T59_HALARM & ~M48T59_HALARM_MASK);
    WRITE_M48T59_REG(M48T59_MALARM, *M48T59_MALARM & ~M48T59_MALARM_MASK);
    WRITE_M48T59_REG(M48T59_SALARM, *M48T59_SALARM & ~M48T59_SALARM_MASK);

    return OK;
    }

/******************************************************************************
*
* dateCheck - check for valid date
*
* This function returns the validity of the date, given the month and year.
* This function assumes the month and year are valid.
*
* RETURNS: TRUE if date is valid, FALSE otherwise
*
* ERRNO
*
*/
LOCAL BOOL dateCheck
    (
    int date,   /* day of the month to check */
    int month,  /* month, must be within MIN_MONTH - MAX_MONTH */
    int year    /* year, must be within MIN_YEAR - MAX_YEAR */
    )
    {
    /* check for leap day */
    if ( (month == (LEAP_MONTH + BIAS_MONTH)) && (isleap(year)) )
        return (RANGE_CHECK(date,MIN_MDAY,LEAP_DATE));
    else if (month == MIN_MONTH) /* January */
        return (RANGE_CHECK(date,MIN_MDAY,MAX_MDAY));
    else
        return (RANGE_CHECK(date,MIN_MDAY,
                            (monthToDays[month - BIAS_MONTH] -
                                monthToDays[(month - BIAS_MONTH) - 1])));

    }


/*------------------
   Shell Utilities 
 ------------------*/
/******************************************************************************
*
* sysAsciiTimeGet - convert tm structure into a character array for printing
*
* This function converts a binary time structure into a string suitable
* for printing.
*
* The string is of the form:
* \cs
*        "Wed Aug 15 10:13:56 1990"
* \ce
*
* \cs
* For example, to print the current date and time from the shell, do the
* following sequence of calls:
*
*        -> x=malloc(100);
*        -> z=malloc(100);
*        -> m48t59TimeGet(x);
*        -> sysAsciiTimeGet(x,z);
*        -> printf("%s\n",z);
* \ce
*
* RETURNS: The length of the string, not including the terminating EOS.
*
* ERRNO
*
* \NOMANUAL
*/

int sysAsciiTimeGet
    (
    struct tm *tm, /* UNIX-compatible time format */
    char string[]  /* array in which to print time string */
    )

    {
    LOCAL char *days[7] =
        {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
        };
    LOCAL char *months[12] =
        {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
    LOCAL char bogusValue[4] =
        {
        "???"
        };

    sprintf(string,"%s %s %u %u:%02u:%02u %4u",
        (tm->tm_wday >6) ? bogusValue : days[tm->tm_wday],
        (tm->tm_mon > 11) ? bogusValue : months[tm->tm_mon],
        tm->tm_mday,
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec,
        tm->tm_year + TM_YEAR_BASE);

    return(strlen(string));
    }


/**************************************************************************
*
* m48t59DatePrint - print Unix-style date of real time clock
*
* This prints an ASCII string on the console of the current time
* and date from the M48T59 chip.
*
* RETURNS: OK
*
* ERRNO
*/

STATUS m48t59DatePrint(void)
    {
    struct tm theTime;
    char buf[100];

    (void) m48t59TimeGet(&theTime);
    sysAsciiTimeGet(&theTime, buf);
    printf("%s\n",buf);
    return(OK);
    }

/**************************************************************************
*
* m48t59InteractiveTimeSet - set the time clock interactively from the shell
*
* This function sets the date and time in the M48T59 chip interactively
* from the shell.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS m48t59InteractiveTimeSet (void)
    {
    STATUS returned = OK;
    char newTimeBuf[30];
    char oldTimeBuf[30];
    char setTimeBuf[30];
    struct tm newTime, oldTime, setTime;
    int value;
    int itemsParsed;

    printf("Setting time of day clock\n");
    printf("Type CONTROL-C to abort at any time\n");

    do
        {
        printf("Enter Year (%4u-%4u): ",MIN_YEAR,MAX_YEAR);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while (itemsParsed != 1 || !RANGE_CHECK(value,MIN_YEAR,MAX_YEAR));

    newTime.tm_year = value - TM_YEAR_BASE;

    do
        {
        printf("Enter Month (%d-%d): ",MIN_MONTH,MAX_MONTH);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while (itemsParsed != 1 || !RANGE_CHECK(value,MIN_MONTH,MAX_MONTH));

    newTime.tm_mon = value - BIAS_MONTH;

    do
        {
        printf("Enter Date (%d-%d): ", MIN_MDAY,MAX_MDAY);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while (itemsParsed != 1 || !RANGE_CHECK(value,MIN_MDAY,MAX_MDAY));

    newTime.tm_mday = value - BIAS_MDAY;

    do
        {
        printf("Enter day of week (%d [Sunday] - %d [Saturday]): ",
               MIN_DAY,MAX_DAY);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while (itemsParsed != 1 || !RANGE_CHECK(value,MIN_DAY,MAX_DAY));

    newTime.tm_wday = value - BIAS_DAY;

    do
        {
        printf("Enter Hour (%d-%d): ",MIN_HOUR,MAX_HOUR);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while (itemsParsed != 1 || !RANGE_CHECK(value,MIN_HOUR,MAX_HOUR));

    newTime.tm_hour = value - BIAS_HOUR;

    do
        {
        printf("Enter Minute (%d-%d): ",MIN_MINUTE,MAX_MINUTE);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while (itemsParsed != 1 || !RANGE_CHECK(value,MIN_MINUTE,MAX_MINUTE));

    newTime.tm_min = value - BIAS_MINUTE;

    do
        {
        printf("Enter Second (%d-%d): ",MIN_SECOND,MAX_SECOND);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while (itemsParsed != 1 || !RANGE_CHECK(value,MIN_SECOND,MAX_SECOND));

    newTime.tm_sec = value - BIAS_SECOND;

    m48t59TimeGet(&oldTime);
    (void) sysAsciiTimeGet(&oldTime,oldTimeBuf);
    printf("\nOld time was: %s\n",oldTimeBuf);

    returned = m48t59TimeSet(&newTime);
    if(returned != OK)
        printf("Invalid time, clock not set!\n");
    else
        {
        m48t59TimeGet(&setTime);
        (void) sysAsciiTimeGet(&setTime,setTimeBuf);
        printf("New time is:  %s\n",setTimeBuf);
        }

    return returned;
    }

/**************************************************************************
*
* m48t59InteractiveAlarmSet - set the alarm clock interactively from the shell
*
* This function sets the alarm time in the M48T59 chip interactively
* from the shell.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS m48t59InteractiveAlarmSet (void)
    {
    STATUS returned = OK;
    char newTimeBuf[30];
    struct tm newTime, oldTime, setTime;
    int value;
    int itemsParsed;

    printf("Setting alarm clock\n");
    printf("Type CONTROL-C to abort at any time\n");

    do
        {
        printf("Enter Date (%d-%d) or -1 if not relevant: ", MIN_MDAY,MAX_MDAY);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while ( itemsParsed != 1 ||
                 (!RANGE_CHECK(value,MIN_MDAY,MAX_MDAY) && value != -1) );

    newTime.tm_mday = value - BIAS_MDAY;

    do
        {
        printf("Enter Hour (%d-%d) or -1 if not relevant: ",MIN_HOUR,MAX_HOUR);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while ( itemsParsed != 1 ||
                 (!RANGE_CHECK(value,MIN_HOUR,MAX_HOUR) && value != -1) );

    newTime.tm_hour = value - BIAS_HOUR;

    do
        {
        printf("Enter Minute (%d-%d) or -1 if not relevant: ",MIN_MINUTE,MAX_MINUTE);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while ( itemsParsed != 1 ||
                 (!RANGE_CHECK(value,MIN_MINUTE,MAX_MINUTE) && value != -1) );

    newTime.tm_min = value - BIAS_MINUTE;

    do
        {
        printf("Enter Second (%d-%d) or -1 if not relevant: ",MIN_SECOND,MAX_SECOND);
        (void) gets(newTimeBuf);
        itemsParsed = sscanf(newTimeBuf,"%d",&value);
        } while ( itemsParsed != 1 ||
                 (!RANGE_CHECK(value,MIN_SECOND,MAX_SECOND) && value != -1) );

    newTime.tm_sec = value - BIAS_SECOND;

    m48t59AlarmGet(&oldTime);
    printf("\nOld alarm was: ");

    if (oldTime.tm_mday < 0)
        printf("Every Day ");
    else if (RANGE_CHECK(oldTime.tm_mday+BIAS_MDAY,MIN_MDAY,MAX_MDAY))
        printf("Day %u ", oldTime.tm_mday );
    else
        printf("Day ?? ");

    if (oldTime.tm_hour < 0)
        printf("Every Hour:");
    else if (RANGE_CHECK(oldTime.tm_hour+BIAS_HOUR,MIN_HOUR,MAX_HOUR))
        printf("%u:", oldTime.tm_hour);
    else
        printf("??:");


    if (oldTime.tm_min < 0)
        printf("Every Min:");
    else if (RANGE_CHECK(oldTime.tm_min+BIAS_MINUTE,MIN_MINUTE,MAX_MINUTE))
        printf("%u:", oldTime.tm_min);
    else
        printf("??:");

    if (oldTime.tm_sec < 0)
        printf("Every Sec\n");
    else if (RANGE_CHECK(oldTime.tm_sec+BIAS_SECOND,MIN_SECOND,MAX_SECOND))
        printf("%u\n", oldTime.tm_sec);
    else
        printf("??\n");

    returned = m48t59AlarmSet(&newTime);
    if(returned != OK)
        printf("Invalid alarm, do not enable!\n");
    else
        {
        m48t59AlarmGet(&setTime);

        printf("\nNew alarm is: ");

        if (setTime.tm_mday < 0)
            printf("Every Day ");
        else
            printf("Day %u ", setTime.tm_mday);

        if (setTime.tm_hour < 0)
            printf("Every Hour:");
        else
            printf("%u:", setTime.tm_hour);

        if (setTime.tm_min < 0)
            printf("Every Min:");
        else
            printf("%u:", setTime.tm_min);

        if (setTime.tm_sec < 0)
            printf("Every Sec\n");
        else
            printf("%u\n", setTime.tm_sec);
        }

    return returned;
    }

/* ds1375.h - DS1375 RTC with Alarm Header File */

/* Copyright 2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01b,01jul05,cak  Renamed HANDLER to ALARM_HANDLER.
01a,21jun05,cak  New.
*/

/*
DESCRIPTION
This file contains the DS1375 register bit definitions and masks.
It also contains the definitions for the structures used to set
and extract the values from the RTC and alarm clocks.
*/

#ifndef INCds1375h
#define INCds1375h

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* macros for reading from and writing to DS1375 */

#define DS1375_READ(offset, data) sysMotI2cRead (DS1375_RTC_I2C_ADDR, \
                                                 offset, 1, data, 1)
#define DS1375_WRITE(offset, data) sysMotI2cWrite (DS1375_RTC_I2C_ADDR, \
                                                   offset, 1, data, 1)

/* DS1375 Register Offsets */

#define DS1375_SECONDS_OFFSET           0
#define DS1375_MINUTES_OFFSET           1
#define DS1375_HOURS_OFFSET             2
#define DS1375_DAY_OFFSET               3
#define DS1375_DATE_OFFSET              4
#define DS1375_MONTH_CENTURY_OFFSET     5
#define DS1375_YEAR_OFFSET              6
#define DS1375_ALARM_1_SECONDS_OFFSET   7
#define DS1375_ALARM_1_MINUTES_OFFSET   8
#define DS1375_ALARM_1_HOURS_OFFSET     9
#define DS1375_ALARM_1_DAY_DATE_OFFSET  10
#define DS1375_ALARM_2_MINUTES_OFFSET   11
#define DS1375_ALARM_2_HOURS_OFFSET     12
#define DS1375_ALARM_2_DAY_DATE_OFFSET  13
#define DS1375_CONTROL_OFFSET           14
#define DS1375_CONTROL_STATUS_OFFSET    15
#define DS1375_SRAM_OFFSET              16
#define DS1375_SRAM_SIZE                16      /* 16 Bytes of SRAM */

/* DS1375 Bit and Mask Definitions */

#define DS1375_ALARM_AxMx_BIT           7
#define DS1375_ALARM_AxMx_MASK          (1 << DS1375_ALARM_AxMx_BIT)
#define DS1375_CONTROL_ECLK_BIT         7
#define DS1375_CONTROL_ECLK_MASK        (1 << DS1375_CONTROL_ECLK_BIT)
#define DS1375_CONTROL_CLKSEL_BITS      5
#define DS1375_CONTROL_CLKSEL_MASK      (0x3 << DS1375_CONTROL_CLKSEL_BITS)
#define DS1375_CONTROL_RS_BITS          3
#define DS1375_CONTROL_RS_MASK          (0x3 << DS1375_CONTROL_RS_BITS)
#define DS1375_CONTROL_A2IE_BIT         1
#define DS1375_CONTROL_A2IE_MASK        (1 << DS1375_CONTROL_A2IE_BIT)
#define DS1375_CONTROL_A1IE_BIT         0
#define DS1375_CONTROL_A1IE_MASK        (1 << DS1375_CONTROL_A1IE_BIT)
#define DS1375_CONTROL_INTCN_BIT        2
#define DS1375_CONTROL_INTCN_MASK       (1 << DS1375_CONTROL_INTCN_BIT)

#define DS1375_CONTROL_STATUS_A1F_BIT   0
#define DS1375_CONTROL_STATUS_A1F_MASK  (1 << DS1375_CONTROL_STATUS_A1F_BIT)
#define DS1375_CONTROL_STATUS_A2F_BIT   1
#define DS1375_CONTROL_STATUS_A2F_MASK  (1 << DS1375_CONTROL_STATUS_A2F_BIT)

#define DS1375_INPUT_FREQ_32768         0
#define DS1375_INPUT_FREQ_8192          1
#define DS1375_INPUT_FREQ_60            2
#define DS1375_INPUT_FREQ_50            3
#define DS1375_OUTPUT_FREQ_1            0
#define DS1375_OUTPUT_FREQ_1024         1
#define DS1375_OUTPUT_FREQ_4096         2
#define DS1375_OUTPUT_FREQ_8192         3

/* Control values for the AlarmSet routine */

#define ALARM_EVERY_MONTH       1       /* Alarm to recur once a month */
#define ALARM_EVERY_WEEK        6       /* Alarm to recur once a week */
#define ALARM_EVERY_DAY         2       /* Alarm to recur once a day */
#define ALARM_EVERY_HOUR        3       /* Alarm to recur once an hour */
#define ALARM_EVERY_MINUTE      4       /* Alarm to recur once a minute */
#define ALARM_EVERY_SECOND      5       /* Alarm to recur every second */

/*
 * the following macros convert from BCD to binary and back.
 * Be careful that the arguments are chars, only char width returned.
 */

#define BCD_TO_BIN(bcd) (( ((((bcd)&0xf0)>>4)*10) + ((bcd)&0xf) ) & 0xff)
#define BIN_TO_BCD(bin) (( (((bin)/10)<<4) + ((bin)%10) ) & 0xff)

/* typedefs */

typedef void (*ALARM_HANDLER)();

/* This structure holds the Real-Time Clock configuration values */

typedef struct rtcdt
    {
    int  century;               /* century */
    int  year;                  /* year */
    int  month;                 /* month */
    int  day_of_month;          /* day of month */
    int  day_of_week;           /* day of week */
    int  hour;                  /* hour */
    int  minute;                /* minute */
    int  second;                /* second */
    BOOL clock;                 /* 12 or 24 hour clock */
    BOOL pm;                    /* If 12-hour clock, AM or PM */
    } RTC_DATE_TIME;

/* This structure holds the Alarm Clock configuration values */

typedef struct alarmdt
    {
    int  day_of_month;          /* day of month */
    int  day_of_week;           /* day of week */
    int  hour;                  /* hour */
    int  minute;                /* minute */
    int  second;                /* second */
    int  alarm;                 /* number of the alarm */
    BOOL clock;                 /* 12 or 24 hour clock */
    BOOL pm;                    /* If 12-hour clock, AM or PM */
    ALARM_HANDLER userAlarmHandler;   /* interrupt handler for alarm */
    } ALARM_DATE_TIME;

/*
 * This structure holds the chip's register values, an access
 * semaphore, and pointers to the user-specified alarm interrupt
 * handlers.
 */

typedef struct ds1375reg
    {
    UCHAR seconds;              /* Seconds Register Contents */
    UCHAR minutes;              /* Minutes Register Contents */
    UCHAR hours;                /* Hours Register Contents */
    UCHAR day;                  /* Day Register Contents */
    UCHAR date;                 /* Date Register Contents */
    UCHAR monthCentury;         /* Month/Century Register Contents */
    UCHAR year;                 /* Year Register Contents */
    UCHAR alarm1Seconds;        /* Alarm 1 Seconds register Contents */
    UCHAR alarm1Minutes;        /* Alarm 1 Minutes Register Contents */
    UCHAR alarm1Hours;          /* Alarm 1 Hours Register Contents */
    UCHAR alarm1DayDate;        /* Alarm 1 Day/Date Register Contents */
    UCHAR alarm2Minutes;        /* Alarm 2 Minutes Register Contents */
    UCHAR alarm2Hours;          /* Alarm 2 Hours Register Contents */
    UCHAR alarm2DayDate;        /* Alarm 2 Day/Date Register Contents */
    UCHAR control;              /* Control Register Contents */
    UCHAR controlStatus;        /* Control/Status Register Contents */
    SEM_ID  semDrvId;           /* driver access semaphore */
    ALARM_HANDLER alarm1IntHandler;   /* interrupt handler for alarm 1 */
    ALARM_HANDLER alarm2IntHandler;   /* interrupt handler for alarm 2 */
    } DS1375_REG;

static char rtcMsgString[] = {"%-3s %-3s %d %02d:%02d:%02d %s%02d%02d"};
static char alarmMsgString[] = {"%s %d: %s %d %02d:%02d:%02d %s %s"};

void sysSecsToCal (time_t time, RTC_DATE_TIME *jt);
time_t calToSysSecs (const RTC_DATE_TIME *jt);
void setAnsiSysTime (void);
void sysDs1375DateTimeHook (DOS_DATE_TIME *pDateTime);
STATUS sysDs1375RtcGet (RTC_DATE_TIME * rtc_time);
STATUS sysDs1375RtcSet (RTC_DATE_TIME * rtc_time);
STATUS sysDs1375RtcShow (void);
STATUS sysDs1375AlarmSet (UCHAR method, ALARM_DATE_TIME * alarm_time);
STATUS sysDs1375AlarmCancel (int alarm);
STATUS sysDs1375AlarmGet (UCHAR * method, ALARM_DATE_TIME * alarm_time);
STATUS sysDs1375AlarmShow (void);

STATUS sysDs1375ClockStop (void);
STATUS sysDs1375ClockStart (void);
STATUS timeSet (void);
STATUS alarmSet (void);
STATUS sysDs1375RtcPrintString (void);
STATUS sysDs1375AlarmPrintString (int alarm);
STATUS sysDs1375RtcRegShow (void);
STATUS sysDs1375AlarmRegShow (void);
STATUS sysDs1375ControlRegShow (void);
STATUS sysDs1375RegShow (void);
STATUS sysDs1375SramRead (int offset, int numBytes, UINT8 *pBfr);
STATUS sysDs1375SramWrite (int offset, int numBytes, UINT8 *pBfr);
STATUS sysDs1375SramShow (void);

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* INCds1375h */

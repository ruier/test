/* stm48t59Rtc.h - ST Microelectronics M48T59 TIMEKEEPER RAM */
/* Copyright 1993-2004 Wind River Systems, Inc.              */

/*
modification history
--------------------
01c,24sep04,mdo  Documentation fixes for apigen
01b,15jul03,j_b  list shell utilities
01a,20jun03,j_b  based on wrPpmcIdt334/stm48t59Rtc.h, rev 01a,28aug02
*/

/*
DESCRIPTION
This header file includes defines for the ST Microelectronics
M48T59 realtime clock parts. 
*/

#ifdef DOC
#define INCm48t59h
#endif /* DOC */

#ifndef __INCm48t59h
#define __INCm48t59h

#ifndef _ASMLANGUAGE
#include "time.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef	_ASMLANGUAGE
#define CAST_ADDR
#else
#define CAST_ADDR (volatile unsigned char *)
#endif	/* _ASMLANGUAGE */

/* on-board access, register definitions */

#define M48T59_REG_OFFSET     0x1FF0  /* register base offset */

#define M48T59_ADRS(reg)   \
	(CAST_ADDR (M48T59_BASE + ((reg + M48T59_REG_OFFSET) * M48T59_INTERVAL)))

/* M48T59 Register Set */
#define M48T59_FLAGS        M48T59_ADRS(0x00)	/* flags */
#define M48T59_RESERVED     M48T59_ADRS(0x01)	/* Reserved - not used */
#define M48T59_SALARM       M48T59_ADRS(0x02)	/* alarm seconds */
#define M48T59_MALARM       M48T59_ADRS(0x03)	/* alarm minutes */
#define M48T59_HALARM       M48T59_ADRS(0x04)	/* alarm hour */
#define M48T59_DALARM       M48T59_ADRS(0x05)	/* alarm date */
#define M48T59_INTS         M48T59_ADRS(0x06)	/* interrupts */
#define M48T59_WDOGBITS     M48T59_ADRS(0x07)	/* watchdog bits */
#define M48T59_CR           M48T59_ADRS(0x08)	/* control register */
#define M48T59_SEC          M48T59_ADRS(0x09)	/* seconds */
#define M48T59_MIN          M48T59_ADRS(0x0A)	/* minutes */
#define M48T59_HOURS        M48T59_ADRS(0x0B)	/* hours */
#define M48T59_DAYS         M48T59_ADRS(0x0C)	/* century/day */
#define M48T59_DATE         M48T59_ADRS(0x0D)	/* date */
#define M48T59_MONTH        M48T59_ADRS(0x0E)	/* month */
#define M48T59_YEAR         M48T59_ADRS(0x0F)	/* year */

/* M48T59 Register Definitions */

/*
** M48T59_FLAGS      0x00    flags                             
*/
#define M48T59_FLAGS_WDOG	0x80	/* watchdog timer expired */
#define M48T59_FLAGS_ALARM	0x40	/* alarm went off */
#define M48T59_FLAGS_LOBATT	0x10	/* low battery detected */

/* 
 * Flag retrieval utility
 * Note: The flags are cleared by reading them
 */
#define M48T59_FLAGS_GET    (*M48T59_FLAGS)

/*
** M48T59_RESERVED   0x01    Unused                             
*/

/*
** M48T59_SALARM     0x02    alarm seconds (BCD)              
*/
#define M48T59_SALARM_MASK	0x80	/* RPT1, sec alarm mask */
#define M48T59_SALARM_10SEC	0x70	/* BCD 10s of secs in upper nibble */
#define M48T59_SALARM_SEC	0x0F	/* BCD secs in lower nibble */

/*
** M48T59_MALARM     0x03    alarm minutes (BCD)                
*/
#define M48T59_MALARM_MASK	0x80	/* RPT2, min alarm mask */
#define M48T59_MALARM_10MIN	0x70	/* BCD 10s of mins in upper nibble */
#define M48T59_MALARM_MIN	0x0F	/* BCD mins in lower nibble */

/*
** M48T59_HALARM     0x04    alarm hours (BCD)                 
*/
#define M48T59_HALARM_MASK	0x80	/* RPT3, hr alarm mask */
#define M48T59_HALARM_10HR	0x30	/* BCD 10s of hours in upper nibble */
#define M48T59_HALARM_HR	0x0F	/* BCD hours in lower nibble */

/*
** M48T59_DALARM     0x05    alarm date (BCD)                   
*/
#define M48T59_DALARM_MASK   0x80	/* RPT4, day alarm mask */
#define M48T59_DALARM_10DATE 0x07	/* BCD 10s of date in upper nibble */
#define M48T59_DALARM_DATE   0x0F	/* BCD date in lower nibble */

/* 
 * Alarm Repeat Modes: consist of RPT4-RPT1, alarm masks from alarm registers.
 * Invalid alarm repeat mode will result in once per second alarm
 */
#define M48T59_MONTH_ALARM  0x0     /* once per month */
#define M48T59_DAY_ALARM    0x8     /* once per day */
#define M48T59_HOUR_ALARM   0xC     /* once per hour */
#define M48T59_MIN_ALARM    0xE     /* once per minute */
#define M48T59_SEC_ALARM    0xF     /* once per second */

/*
** M48T59_INTS       0x06    interrupt enable                  
*/
#define M48T59_AFE			0x80	/* alarm flag enable */
#define M48T59_ABE			0x20	/* alarm in battery backup mode enable */

/*
** M48T59_WDOGBITS   0x07    watchdog control                  
*/
#define M48T59_WDOG_WDS        0x80	/* watch dog steering bit: 0=irq, 1=reset */
#define M48T59_WDOG_MULT       0x7C	/* watch dog multiplier bits */
#define M48T59_WDOG_RESOLUTION 0x03	/* watch dog resolution bits: */
                                    /* 0=1/16 sec, 1=1/4 sec, 2=1sec, 3=4sec */
/* Watchdog signal modes */
#define M48T59_WDOG_IRQ        0    /* watchdog signals interrupt */
#define M48T59_WDOG_RST        1    /* watchdog signals reset */

/* Watchdog multiplier and resolution ranges */
#define MIN_WDOG_MULT          1
#define MAX_WDOG_MULT          31
#define WDOG_RES_16TH_SEC      0
#define WDOG_RES_4TH_SEC       1
#define WDOG_RES_1_SEC         2
#define WDOG_RES_4_SEC         3
#define MAX_WDOG_RESOLUTION    WDOG_RES_4_SEC

/*
** M48T59_CR         0x08    control register                  
*/
#define M48T59_CR_WRITE		0x80	/* write enable */
#define M48T59_CR_READ		0x40	/* read enable */
#define M48T59_CR_SIGN		0x20	/* calibration sign: 0=negative, 1=positive */
#define M48T59_CR_CALIBRATE	0x1F	/* oscillator calibration bits */

/*
** M48T59_SEC        0x09    seconds (BCD)                      
*/
#define M48T59_SEC_OSCSTOP  0x80	/* Oscillator stop */
#define M48T59_SEC_10SEC	0x70	/* BCD 10s of secs in upper nibble */
#define M48T59_SEC_SEC      0x0F	/* BCD secs in lower nibble */

/*
** M48T59_MIN        0x0A    minutes (BCD)                     
*/
#define M48T59_MALARM_10MIN	0x70	/* BCD 10s of mins in upper nibble */
#define M48T59_MALARM_MIN	0x0F	/* BCD mins in lower nibble */

/*
** M48T59_HOURS      0x0B    hours (BCD)                       
*/
#define M48T59_HOURS_10HR	0x30	/* BCD 10s of hours */
#define M48T59_HOURS_HR		0x0F	/* BCD hours in lower nibble */

/*
** M48T59_DAYS       0x0C    day of the week (BCD)              
*/
#define M48T59_DAYS_FREQTST	0x40	/* frequency test: 0=disabled, 1=enabled */
#define M48T59_DAYS_CENTEN	0x20	/* century enable: 0=disabled, 1=enabled */
#define M48T59_DAYS_CENTURY 0x10	/* century bit */
#define M48T59_DAYS_DAYS    0x07	/* days in lower three bits */

/*
** M48T59_DATE       0x0D    date (BCD)                         
*/
#define M48T59_DATE_10DATE	0x30	/* 10s date */
#define M48T59_DATE_DATE	0x0F	/* date */

/*
** M48T59_MONTH      0x0E    month (BCD)                      
*/
#define M48T59_MONTH_10MONTH 0x10	/* 10s of months */
#define M48T59_MONTH_MONTH   0x0F	/* month */

/*
** M48T59_YEAR    0x0F    years (BCD)                           
*/
#define M48T59_YEAR_10YEARS	 0xF0	/* 10 years in upper nibble */
#define M48T59_YEAR_YEARS	 0x0F	/* years in lower nibble */


#ifndef	_ASMLANGUAGE
extern STATUS m48t59Reset (void);
extern BOOL   m48t59OscModeSet(BOOL state);

extern STATUS m48t59TimeSet (struct tm *);
extern STATUS m48t59TimeGet (struct tm *);

extern STATUS m48t59WdogSet (INT8 * pMult, INT8 * pResolution);
extern STATUS m48t59WdogGet (INT8 * pMult, INT8 * pResolution);
extern STATUS m48t59WdogModeSet (BOOL mode);
extern UINT8  m48t59WdogModeGet (void);
extern STATUS m48t59WdogDisable (void);

extern STATUS m48t59AlarmSet (struct tm * alarmTime);
extern STATUS m48t59AlarmGet (struct tm * alarmTime);
extern STATUS m48t59AlarmEnable (BOOL batt_backup);
extern STATUS m48t59AlarmDisable (void);

/* Shell Utilities */
extern STATUS m48t59DatePrint(void);
extern STATUS m48t59InteractiveTimeSet (void);
extern STATUS m48t59InteractiveAlarmSet (void);
extern int    sysAsciiTimeGet (struct tm *tm, char string[]);

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCm48t59h */

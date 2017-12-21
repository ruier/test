/* perfMon.h - MPC8540 Performanc Monitor */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,23mayo5,efb Initial writing. 
*/


#ifndef __INCperfMonh
#define __INCperfMonh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* includes */

#include "vxWorks.h"
#include "sysMpc8540PerfMonEvent.h"

/* defines */

#define PERFMON_NUM_RESOURCES 10     /* Nine counters using ten 32 bit regs */
#define PERFMON_NUM_COUNTERS  9      /* Nine atomic counters */

/* Trigger mode controls */

#define PERFMON_TRIGCNTL_DISAB          0x00 
#define PERFMON_TRIGCNTL_CHANGE         0x01 
#define PERFMON_TRIGCNTL_OVERFLOW       0x02

/* Scale factors for threshold and burst distance */

#define PERFMON_TBMULT_X1                 0x00
#define PERFMON_TBMULT_X2                 0x01
#define PERFMON_TBMULT_X4                 0x02
#define PERFMON_TBMULT_X8                 0x03
#define PERFMON_TBMULT_X16                0x04
#define PERFMON_TBMULT_X32                0x05
#define PERFMON_TBMULT_X64                0x06
#define PERFMON_TBMULT_X128               0x07

#define PERFMON_MAX_THRESHOLD             0x3F
#define PERFMON_MAX_BURST_SIZE            0x1F
#define PERFMON_MAX_BURST_GRAN            0x1F
#define PERFMON_MAX_BURST_DIST            0x3F

/* Counter overflow indication */

#define PERFMON_COUNTER_OVERFLOW_BIT      0 
#define PERFMON_COUNTER_OVERFLOW_MASK     0x80000000UL

/* typedefs */

typedef UINT32 COUNTER_VALUE[PERFMON_NUM_RESOURCES];
typedef UINT32 COUNTERS_USED[PERFMON_NUM_RESOURCES];

typedef struct eventMonitor EVENT_MONITOR;

struct eventMonitor
    {
    PERFMON_EVENT   theEvent;      /* Event ID of this monitor */
    UINT32          numCounters;   /* Number of counter registers used */
    BOOL            isActive;      /* The event is being counted */
    BOOL            isFrozen;      /* Monitor is active, but not counting */ 
    BOOL            isIntEnabled;  /* Enabled to interrupt on overflow */
    BOOL            isLocked;      /* Has its resources locked */
    COUNTERS_USED   countersInUse; /* Counters indexed in increasing order */
    COUNTER_VALUE   savedCount;    /* Count snapshot taken when suspended */
    UINT32          threshold;     /* Threshold to exceed to start counting */
    UINT32          scaleFactor;   /* Power of 2 multiplier for thresh/burst */
    UINT32          burstSize;     /* Events per burst */
    UINT32          burstGran;     /* Cycles between each event in a burst */
    UINT32          burstDist;     /* Cycles between each burst group */
    EVENT_MONITOR  *trigOnEvent;   /* Event monitor to trigger count start */
    EVENT_MONITOR  *trigOffEvent;  /* Event montior to trigger count stop */
    UINT32          trigOnCtrl;    /* Trigger on overflow/change or disable */
    UINT32          trigOffCtrl;   /* Trigger on overflow/change or disable */
    EVENT_MONITOR  *nextEvent;     /* Next event monitor in list */
    EVENT_MONITOR  *prevEvent;     /* Previous event monitor in list */
    };

const COUNTER_VALUE perfMonZeroCount = {0};

STATUS sysPerfMonInit (void);

void perfMonfreezeAll (void);

void perfMonUnfreezeAll (void);

STATUS perfMonIntConnect (VOIDFUNCPTR, int); 

void perfMonIntEnable (BOOL);

void perfMonIntDisable (void);

STATUS perfMonEventMonitorCreate (EVENT_MONITOR  **, PERFMON_EVENT, UINT32);

STATUS perfMonEventMonitorActivate (EVENT_MONITOR *, COUNTER_VALUE, BOOL, 
                                    BOOL, BOOL);

STATUS perfMonEventMonitorTrigGo (EVENT_MONITOR *, BOOL, BOOL);

STATUS perfMonEventMonitorSuspend (EVENT_MONITOR *);

STATUS perfMonEventMonitorReset (EVENT_MONITOR *, COUNTER_VALUE);

STATUS perfMonEventMonitorSupsend (EVENT_MONITOR *);

STATUS perfMonEventMonitorDelete (EVENT_MONITOR *);

STATUS perfMonEventMonitorRead (EVENT_MONITOR *, COUNTER_VALUE);

BOOL perfMonEventMonitorIsActive (EVENT_MONITOR *);

void perfMonEventMonitorActiveGet (EVENT_MONITOR *, EVENT_MONITOR **);

void perfMonEventMonitorSuspendedGet (EVENT_MONITOR *, EVENT_MONITOR **);

PERFMON_EVENT perfMonEventMonitorGetEvent (EVENT_MONITOR *);

BOOL perfMonEventMonitorIsAvailable (PERFMON_EVENT, UINT32, BOOL *, 
                                     EVENT_MONITOR **);

STATUS perfMonEventMonitorIntEnable (EVENT_MONITOR *);           

STATUS perfMonEventMonitorIntDisable (EVENT_MONITOR *);


STATUS perfMonEventMonitorThresholdSet (EVENT_MONITOR *, UINT32, UINT32);

STATUS perfMonEventMonitorScaleChange (EVENT_MONITOR *, UINT32); 

STATUS perfMonEventMonitorBurstSet (EVENT_MONITOR *, UINT32, UINT32, UINT32,
                                    UINT32);

STATUS perfMonEventMonitorTrigSet (EVENT_MONITOR *, EVENT_MONITOR *,
                                   EVENT_MONITOR *, UINT32, UINT32);


BOOL perfMonEventMonitorHasOverflowed (EVENT_MONITOR *);

STATUS perfMonEventMonitorGo (EVENT_MONITOR *);

STATUS perfMonEventMonitorStop (EVENT_MONITOR *);

STATUS perfMonEventMonitorShow (EVENT_MONITOR *);

void perfMonEventMonitorShowAll (BOOL activeOnly);

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCperfMonh */

/* sysMv7100Wdog.c - MVME7100 Watchdog Timer Library */

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
01a,14dec07,mdg  Initial writing.
*/

/*
DESCRIPTION

The MVME7100's watchdog timer is a timer that will reset the board or
the VME system if it is enabled and not serviced at a periodic
interval. To use the watchdog timer define INCLUDE_WDOG_TIMER in
config.h.

STATUS sysWdogStart (resolution, count, systemReset)

This routine has three input parameters: the resolution of the watchdog
timer, the second is the count that will be loaded into the watchdog
when it is started or when it is serviced, and the third is the system
reset mode.
The resolution is an integer from 0-7 which has the following encoding:
    0 - 8usec
    1 - 16usec
    2 - 32usec
    3 - 64usec
    4 - 128usec
    5 - 256usec
    6 - 512usec
    7 - 1024usec
If system reset is true, the watchdog timer will reset
the VME system, but if it is false, only the board will be reset.

STATUS sysWdogStop ()

This routine will stop the watchdog timer by disabling it.  The only way to
restart it is to call sysWdogStart() again.

STATUS sysWdogService ()

This routine will service the watchdog timer.  Servicing the watchdog
periodically will prevent it from expiring and triggering an interrupt or
issuing a system reset. Servicing sets the watchdog to its initial state
regardless of its current stage. The timer continues to run after
servicing.  It can be serviced as often as desired, but at least once
before each timeout interval expires.

BOOL sysWdogIsRunning ()

This routine will return true if the watchdog timer is armed and running.

UINT32 sysWdogPeriodGet ()

This routine returns the actual watchdog timer period supported by hardware
in microseconds.
*/

/* includes */

#include "vxWorks.h"

/* defines */

#define WDOG_MIN_RES          0
#define WDOG_MAX_RES          7

/* typedefs */

/* globals */

/* locals */

LOCAL BOOL    wdogIsRunning = FALSE;
LOCAL UINT16  wdogCount = 0;
LOCAL int     wdogRes = 7;
LOCAL UINT32  wdogResolutions[8] =
    {8,16,32,64,128,256,512,1024};

/* forward declarations */

/*******************************************************************************
* sysWdogService - service the MVME7100 watchdog timer
*
* This routine services the MVME7100 watchdog timer. The time base register
* is cleared to zero and the "enable next watchdog time" bit is cleared.
* The watchdog interrupt is ensured to be enabled for proper servicing if
* in the second stage.
*
* RETURNS: OK always.
*/

void sysWdogService (void)
    {
    *((UINT8 *)(BRD_WATCH_DOG_LOAD_REG)) = BRD_WATCH_DOG_LOAD_PATTERN;
    }

/*******************************************************************************
*
* sysWdogIsRunning - return the running status of the watchdog.
*
* This routine returns true if the watchdog is armed and running.
*
* RETURNS: TRUE if the watchdog is running, FALSE otherwise.
*/

BOOL sysWdogIsRunning (void)
    {
    return (wdogIsRunning);
    }

/*******************************************************************************
*
* sysMpcWdogPeriodGet - returns the actual watchdog timeout period.
*
* This routine returns the actual watchdog timer period supported by hardware
* in microseconds.
*
* RETURNS: Actual timeout period in microseconds.
*/

UINT32 sysWdogPeriodGet (void)
    {
    return (wdogResolutions[wdogRes] * wdogCount);
    }

/*******************************************************************************
*
* sysWdogStop - Stop the MVME7100 Watchdog Timer
*
* This routine will stop the MVME7100 watchdog timer.
*
* RETURNS: N/A
*/

void sysWdogStop (void)
    {
    *((UINT8 *)(BRD_WATCH_DOG_CTRL_REG)) =
        *((UINT8 *)(BRD_WATCH_DOG_CTRL_REG)) & ~(BRD_WATCH_DOG_ENABLE_MASK);

    wdogIsRunning = FALSE;
    }

/*******************************************************************************
*
* sysWdogStart - start the MVME7100 watchdog timer
*
* This routine configures (based on user input), and starts the MVME7100
* watchdog timer. The time in seconds specifies the length of timeout.
* If reset is specified as true, the VME system will be reset, otherwise only
* a board level reset will be generated.
*
* RETURNS: OK if watchdog timer successfully started, ERROR otherwise.
*/

STATUS sysWdogStart
    (
    int    resolution,
    UINT16 count,
    BOOL   systemReset    /* watchdog timer can reset system if true */
    )
    {
    STATUS status = OK;
    UINT8 control;

    if (resolution >= WDOG_MIN_RES && resolution <= WDOG_MAX_RES)
        {

        /* Make sure timer is stopped */

        sysWdogStop ();

        wdogCount = count;
        wdogRes = resolution;

        *((UINT16 *)(BRD_WATCH_DOG_COUNT_REG)) = wdogCount;

        if (systemReset)
            {
            control = BRD_WATCH_DOG_ENABLE_MASK | BRD_WATCH_DOG_SYS_RST_MASK;
            }
        else
            {
            control = BRD_WATCH_DOG_ENABLE_MASK;
            }

        *((UINT8 *)(BRD_WATCH_DOG_RES_REG)) = resolution;

        *((UINT8 *)(BRD_WATCH_DOG_CTRL_REG)) = control;

        *((UINT8 *)(BRD_WATCH_DOG_LOAD_REG)) = BRD_WATCH_DOG_LOAD_PATTERN;

        wdogIsRunning = TRUE;
        }
    else
        {

        /* resolution out of range */

        status = ERROR;
        }

    return (status);
    }


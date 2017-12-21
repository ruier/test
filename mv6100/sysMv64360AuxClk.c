/* sysMv64360AuxClk.c - Motorola MVME6100 AuxClock library */

/*
 * Copyright (c) 2002-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01h,27apr08,y_w   Rename functions to support VxBus and code cleanup.
01g,14jun04,cak   Moved the MV64360 timer/counter support to a separate file.
01f,25feb04,cak   Moved the initialization of timerRate from the int()
                  routine - where it was mistakenly placed - to the
                  init() routine.
01e,17dec03,cak   BSP update.
01d,28jan03,cak   Redefined MV64360_TMR_RATE as CPU_BUS_SPD.
01c,24jan03,cak   Replaced DEFAULT_BUS_CLOCK with DFLT_BUS_CLK.
01b,14oct02,yyz   Fixed "&" to "|" in setting timer mode in sysAuxClkInit().
01a,18jun02,yyz   Initial writing.
*/

/*
DESCRIPTION
There are a total of four identical timer/counters on the MV64360,
each 32-bits wide. This file contains routines to use one of the
timer/counters as the auxiliary clock.  
*/

/* includes */

#include "config.h"
#include "mv64360.h"
#include "mv6100A.h"

/* defines */

/* typedefs */

/* globals */

/* locals */

LOCAL BOOL  mv64360AuxClkRunning        = FALSE;
LOCAL int   mv64360AuxClkTicksPerSecond = 60;

/* forward declarations */

STATUS  mv64360AuxClkConnect (FUNCPTR routine, int arg);
void    mv64360AuxClkDisable (void);
void    mv64360AuxClkEnable  (void);
int     mv64360AuxClkRateGet (void);
STATUS  mv64360AuxClkRateSet (int ticksPerSecond);

/*******************************************************************************
*
* mv64360AuxClkConnect - connect a routine to the auxiliary clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* auxiliary clock interrupt.
*
* RETURN: OK or ERROR if sysMv64360TmrCntrConnectRoutine return ERROR.
*
* SEE ALSO: intConnect(), mv64360AuxClkEnable()
*/

STATUS mv64360AuxClkConnect
    (
    FUNCPTR routine,    /* routine called at each aux clock interrupt */
    int     arg         /* argument with which to call routine */
    )
    {

    /* first disconnect any existing routine */

    sysMv64360TmrCntrDisconnectRoutine(MV64360_AUXCLK_TMR);

    if (sysMv64360TmrCntrConnectRoutine(routine, arg, MV64360_AUXCLK_TMR) == OK)
        {
        return (OK);
        }
    else
        return (ERROR);
    }

/*******************************************************************************
*
* mv64360AuxClkDisable - turn off auxiliary clock interrupts
*
* This routine disables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: mv64360AuxClkEnable()
*/

void mv64360AuxClkDisable (void)
    {
    if (mv64360AuxClkRunning)
        {
        sysMv64360TmrCntrDisable(MV64360_AUXCLK_TMR);
        mv64360AuxClkRunning = FALSE;
        }
    }

/*******************************************************************************
*
* mv64360AuxClkEnable - turn on auxiliary clock interrupts
*
* This routine enables auxiliary clock interrupts.
*
* RETURN: N/A
*
* SEE ALSO: mv64360AuxClkConnect(), mv64360AuxClkDisable()
*/

void mv64360AuxClkEnable (void)
    {

    if (mv64360AuxClkRunning == FALSE)
        {
        sysMv64360AuxClkStart(MV64360_AUXCLK_TMR, mv64360AuxClkTicksPerSecond);
        mv64360AuxClkRunning = TRUE;
        }
    }

/*******************************************************************************
*
* mv64360AuxClkRateGet - get the auxiliary clock rate
*
* This routine returns the interrupt rate of the auxiliary clock.
*
* RETURNS: The number of ticks per second of the auxiliary clock.
*
* SEE ALSO: mv64360AuxClkEnable(), mv64360AuxClkRateSet()
*/

int mv64360AuxClkRateGet (void)
    { 
    return (mv64360AuxClkTicksPerSecond);
    }

/*******************************************************************************
*
* mv64360AuxClkRateSet - set the auxiliary clock rate
*
* This routine sets the interrupt rate of the auxiliary clock. It does not
* enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the tick rate is invalid or the timer cannot be set.
*
* SEE ALSO: mv64360AuxClkEnable(), mv64360AuxClkRateGet()
*/

STATUS mv64360AuxClkRateSet
    (
    int ticksPerSecond  /* number of clock interrupts per second */
    )
    {
    if (ticksPerSecond < AUX_CLK_RATE_MIN || 
        ticksPerSecond > AUX_CLK_RATE_MAX)
        {
        return (ERROR);
        }

    mv64360AuxClkTicksPerSecond = ticksPerSecond;

    if (mv64360AuxClkRunning)
        {
        mv64360AuxClkDisable ();
        mv64360AuxClkEnable ();
        }

    return (OK);
    }


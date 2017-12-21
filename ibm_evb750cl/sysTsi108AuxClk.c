/* sysTsi108AuxClk.c - TSI108 MPIC auxiliary timer library */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */ 
 
 /* Copyright 2007 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01c,03jul08,y_w  Rename functions to support VxBus and code cleanup.
01b,12nov07,y_w  Clear out apigen errors/warnings
01a,11may07,jjm  Initial version, based on IBM 750GX/GL EVB BSP
*/

/*
DESCRIPTION

This library provides TSI109/110 auxiliary clock support.

The Tsi108 MPIC TIMER0 is used for the system (sys) auxiliary clock
and TIMER1 is used as user (usr) auxiliary clock.

The macros AUX_CLK_RATE_MIN, and AUX_CLK_RATE_MAX must be defined to
provide parameter checking for xxxAuxClkRateSet() routines.

INCLUDE FILES: sysTsi108Mpic.h
*/

/* includes */

#include "sysTsi108Mpic.h"

/* local defines */

#ifndef MPIC_TIMER_CNT
#   define MPIC_TIMER_CNT   TSI108_ISF_CLK
#endif

#ifndef INT_PRIO_TIMER0
#   define INT_PRIO_TIMER0  DEFAULT_PRIO_LVL
#endif

#ifndef INT_PRIO_TIMER1
#   define INT_PRIO_TIMER1  DEFAULT_PRIO_LVL
#endif

#define MAX_AUX_TIMERS      2
#define SYS_AUX_TIMER       0
#define USR_AUX_TIMER       1

/* locals */

LOCAL UINT  intPrio[MAX_AUX_TIMERS] = {INT_PRIO_TIMER0, INT_PRIO_TIMER1};

LOCAL FUNCPTR   auxClkRoutine[MAX_AUX_TIMERS]        = {NULL, NULL};
LOCAL int       auxClkArg[MAX_AUX_TIMERS]            = {0, 0};
LOCAL BOOL      auxClkRunning[MAX_AUX_TIMERS]        = {FALSE, FALSE};
LOCAL int       auxClkTicksPerSecond[MAX_AUX_TIMERS] = {60, 60};

/* forward declarations */

STATUS  tsiAuxClkConnect (FUNCPTR routine, int arg);
void    tsiAuxClkDisable (void);
void    tsiAuxClkEnable  (void);
int     tsiAuxClkRateGet (void);
STATUS  tsiAuxClkRateSet (int ticksPerSecond);

/***********************************************************************
*
* tsiAuxClkInt - auxiliary clock interrupt handler for Sys timer
*
* This routine handles the auxiliary clock interrupt. It calls a user routine
* if one was specified by the routine tsiAuxClkConnect().
*
* RETURNS: N/A
*/

LOCAL void tsiAuxClkInt (void)
    {
    if (auxClkRoutine[SYS_AUX_TIMER] != NULL)
        (*auxClkRoutine[SYS_AUX_TIMER]) (auxClkArg[SYS_AUX_TIMER]);
    }

/***********************************************************************
*
* tsiAuxClkInit - initialize the auxiliary clock on SYS timer
*
* This routine connects the auxiliary clock interrupt handler.  It places
* the timer into a disabled state and then connects the handler.
*
* RETURNS: N/A but the default aux clock handler is connected.
*/

void tsiAuxClkInit2 (void)
    {
    /* disable counter */
    
    TSI108_REG_WR(BL_MPIC, MPIC_INT_GTBCR(SYS_AUX_TIMER), 0xffffffff);

    auxClkRunning[SYS_AUX_TIMER] = FALSE; /* clock is not running */

    /* Connect interrupt handler */
    
    (void) intConnect (INUM_TO_IVEC(INT_VEC_TIMER0 + SYS_AUX_TIMER),
               (VOIDFUNCPTR) tsiAuxClkInt, 0);
    }

/***********************************************************************
*
* tsiAuxClkConnect - connect a routine to the auxiliary clock interrupt
*                    from SYS timer
*
* This routine specifies the interrupt service routine to be called at each
* auxiliary clock interrupt.  It does not enable auxiliary clock
* interrupts.
*
* RETURNS: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), tsiAuxClkEnable()
*/

STATUS tsiAuxClkConnect
    (
    FUNCPTR routine,    /* routine called at each aux. clock interrupt */
    int     arg         /* argument to auxiliary clock interrupt routine */
    )
    {

    /* connect the ISR to the TIMER0 interrupt handler */

    auxClkRoutine[SYS_AUX_TIMER]   = routine;
    auxClkArg[SYS_AUX_TIMER]       = arg;

    return (OK);
    }

/***********************************************************************
*
* auxClkDisable - turn off auxiliary clock interrupts from specified timer
*
* This routine disables auxiliary clock interrupts.
*
* RETURNS: N/A
*
*/

LOCAL void auxTimerDisable
    (
    int timer
    )
    {
    if (auxClkRunning[timer])
        {
        /* disable counter */

        TSI108_REG_WR(BL_MPIC, MPIC_INT_GTBCR(timer), 0xffffffff);

        auxClkRunning[timer] = FALSE; /* clock is not running */
        }
    }

/***********************************************************************
*
* tsiAuxClkDisable - turn off auxiliary clock interrupts from SYS timer
*
* This routine disables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: tsiAuxClkEnable()
*/

void tsiAuxClkDisable (void)
    {
    auxTimerDisable(SYS_AUX_TIMER);
    }

/***********************************************************************
*
* auxTimerEnable - turn on auxiliary clock interrupts from specified timer
*
* This routine enables auxiliary clock interrupts.
*
* RETURNS: N/A
*
*/

LOCAL void auxTimerEnable
    (
    int timer
    )
    {
    if (auxClkRunning[timer])
        {
        /* disable counter */
        TSI108_REG_WR(BL_MPIC, MPIC_INT_GTBCR(timer), 0xffffffff);
        auxClkRunning[timer] = FALSE;           /* clock is no longer running */
        }

    /* Interrupt unmasked, priority level set */

    TSI108_REG_WR(BL_MPIC, MPIC_INT_GTVPR(timer),
        (intPrio[timer] << 16) | (INT_VEC_TIMER0 + timer));

    /* Interrupt directed at processor output 0 */

    TSI108_REG_WR(BL_MPIC, MPIC_INT_GTDR0(timer), (1 << 0));

    /* enable counter and write value to count from */

    TSI108_REG_WR(BL_MPIC, MPIC_INT_GTBCR(timer),
        ((MPIC_TIMER_CNT/auxClkTicksPerSecond[timer]) & 0x7fffffff));

    auxClkRunning[timer] = TRUE;
    }


/***********************************************************************
*
* tsiAuxClkEnable - turn on auxiliary clock interrupts from SYS timer
*
* This routine enables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: tsiAuxClkConnect(), tsiAuxClkDisable(), tsiAuxClkRateSet()
*/

void tsiAuxClkEnable (void)
    {
    auxTimerEnable(SYS_AUX_TIMER);
    }

/***********************************************************************
*
* tsiAuxClkRateGet - get the auxiliary clock rate for SYS timer
*
* This routine returns the interrupt rate of the auxiliary clock.
*
* RETURNS: The number of ticks per second of the auxiliary clock.
*
* SEE ALSO: tsiAuxClkEnable(), tsiAuxClkRateSet()
*/

int tsiAuxClkRateGet (void)
    {
    return (auxClkTicksPerSecond[SYS_AUX_TIMER]);
    }

/*******************************************************************************
*
* auxTimerRateSet - set the auxiliary clock rate for specified timer
*
* This routine sets the interrupt rate of the auxiliary clock.
* It does not enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the tick rate is invalid.
*
*/

LOCAL STATUS auxTimerRateSet
    (
    int timer,          /* number of the timer */
    int ticksPerSecond      /* number of clock interrupts per second */
    )
    {
    if (ticksPerSecond < AUX_CLK_RATE_MIN || ticksPerSecond > AUX_CLK_RATE_MAX)
        return(ERROR);

    auxClkTicksPerSecond[timer] = ticksPerSecond;

    if (auxClkRunning[timer])
        {
        auxTimerDisable (timer);
        auxTimerEnable (timer);
        }

    return(OK);
    }

/*******************************************************************************
*
* tsiAuxClkRateSet - set the auxiliary clock rate for SYS timer
*
* This routine sets the interrupt rate of the auxiliary clock.
* It does not enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the tick rate is invalid.
*
* SEE ALSO: tsiAuxClkEnable(), tsiAuxClkRateGet()
*/

STATUS tsiAuxClkRateSet
    (
    int ticksPerSecond      /* number of clock interrupts per second */
    )
    {
    return auxTimerRateSet(SYS_AUX_TIMER, ticksPerSecond);
    }


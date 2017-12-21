/* hawkAuxClk.c - Hawk Timer (Aux Clk) library */

/*
 * Copyright (c) 2000-2001, 2003, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/* Copyright 1997-2001 Motorola, Inc. All Rights Reserved */
 
/*
modification history
--------------------
01h,05jul08,y_w  Code cleanup.
01g,23apr08,y_w  change function names to support VxBus
01f,01dec03,agf  fix compiler warnings
01e,05nov01,kab  Another compiler warning.
01d,18jun01,mil  Cleaned up compiler warnings.
01c,07mar01,rhk  Changes to comply with WRS coding standards.
01b,16aug00,dmw  Wind River coding standards review.
01a,13mar00,dmw  Cleanup (from version 01a of mpcn765/hawkAuxClk.c).
*/
 
/*
DESCRIPTION
This timer contains routines to use MPIC timer 0 on the Hawk as the auxiliary
clock. The Hawk MPIC timers are driven by the PCI bus clock through a divide by
8 pre-scaler. There are a total of 4 identical MPIC timers each 31-bits wide.
Each timer is composed of the following registers:

Current Count Register - 
Bits 30-0 hold the current value of the timer which counts down towards 0. each
time the timer value reaches zero, Bit 31 (MSB) toggles and an interrupt is
made pending.

Base Count Register - 
Bit 31 of this register contains a count inhibit bit.When this bit is set to 0,
the associated MPIC timer counts down towards 0. When the count inhibit bit is
set to 1, the associated MPIC timer stops decrementing and holds its current
value. Bits 30-0 contain a timer reload value. When a value is written to this
field and the count inhibit bit changes from 1 to 0, the base count value is
loaded into the current count register and the toggle bit in the current count
register is cleared. Bits 30-0 of the Base Count register are also transferred
to the Current Count register each time the current count register counts down
to zero. This allows the timer to create a periodic interrupt without processor
intervention.

Vector/Priority Register - 
Bit 31 (MSB) of this register contains the Mask bit, which when set to 1,
inhibits the delivery of the associated MPIC timer's interrupt. Bit 30 contains
the Active bit will be set to a 1 anytime the associated MPIC timer is
requesting an interrupt or has an interrupt in service. Bits 19 through 16 hold
a 4-bit interrupt priority value where 15 is the highest priority and 0
effectively disables the interrupt. Bits 7-0 hold an 8-bit vector which will be
provided during MPIC interrupt acknowledge cycles when the interrupt associated
with this timer is acknowledged.

Destination Register - 
If bit 0 (LSB) of this register is set, the interrupt associated with this MPIC
timer is directed to CPU0. If bit 1 is set, the interrupt is directed to CPU1.

In addition to the per-timer registers described above, there is a single Timer
Frequency Register which is used to hold the effective clock frequency of the
MPIC timer clock frequency. Start-up code must initialize this 32-bit register
to the PCI clock frequency divided by 8.
*/

/* includes */

#include "hawkMpic.h"

/* defines */

/*
 * NOTE: Raven MPIC was clocked by the 60x bus clock. Hawk MPIC is clocked
 *       by the PCI bus clock.
 */

#define MPIC_TIMER_CNT      (sysGetPciSpd() / 8) /* was sysGetBusSpd in Raven */
#define MPIC_TIMER_DISABLE  0xFFFFFFFF

/* locals */

LOCAL FUNCPTR hawkAuxClkRoutine        = (FUNCPTR) NULL;
LOCAL int     hawkAuxClkArg            = 0;
LOCAL int     hawkAuxClkTicksPerSecond = 60;
LOCAL BOOL    hawkAuxClkConnected      = FALSE;
LOCAL int     hawkAuxClkRunning        = FALSE;

/*******************************************************************************
*
* hawkAuxClkInt - handle an auxiliary clock interrupt from Hawk timer 0
*
* This routine handles a Hawk timer 0 interrupt.  It clears the interrupt
* and calls the routine  installed by hawkAuxClkConnect().
*
* RETURNS: N/A
*/
 
void hawkAuxClkInt (void)
    {
    if (hawkAuxClkRoutine != NULL)
        {
        (*hawkAuxClkRoutine) (hawkAuxClkArg);
        }
    }

/*******************************************************************************
*
* hawkAuxClkInit - Hawk aux. clock initialization routine
*
* This routine should be called before calling any other routine in this
* module.
*
* RETURNS: OK, always.
*/
 
STATUS hawkAuxClkInit (void)
    {

    /* disable counter */

    sysPciWrite32(MPIC_ADDR(MPIC_TIMER0_BASE_CT_REG), MPIC_TIMER_DISABLE);
    EIEIO;                  /* synchronize */

    /* setup timer frequency register */

    sysPciWrite32(MPIC_ADDR(MPIC_TIMER_FREQ_REG), MPIC_TIMER_CNT);

    /* interrupt unmasked, priority level 15, vector TIMER0_INT_VEC. */

    sysPciWrite32(MPIC_ADDR(MPIC_TIMER0_VEC_PRI_REG), 
                  (( PRIORITY_LVL15 | (TIMER0_INT_VEC) ) & ~TIMER_INHIBIT));

    /* interrupt directed at processor 0 */

    sysPciWrite32(MPIC_ADDR(MPIC_TIMER0_DEST_REG), DESTINATION_CPU0);

    EIEIO;                  /* synchronize */

    hawkAuxClkRunning = FALSE;

    return (OK);
    }

/*******************************************************************************
*
* hawkAuxClkConnect - connect a routine to the auxiliary clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* auxiliary clock interrupt.
*
* RETURNS: OK, always.
*
* SEE ALSO: intConnect(), hawkAuxClkEnable()
*/
 
STATUS hawkAuxClkConnect
    (
    FUNCPTR routine,    /* routine called at each aux clock interrupt */
    int     arg         /* argument with which to call routine        */
    )
    {
    hawkAuxClkRoutine   = routine;
    hawkAuxClkArg       = arg;
    hawkAuxClkConnected = TRUE;
 
    return (OK);
    }

/*******************************************************************************
*
* hawkAuxClkDisable - turn off auxiliary clock interrupts
*
* This routine disables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: hawkAuxClkEnable()
*/
 
void hawkAuxClkDisable (void)
    {
    UINT32 baseCount;

    if (hawkAuxClkRunning)
        {
        /* Set Count Inhibit in the Base Count register. */

        sysPciRead32 (MPIC_ADDR(MPIC_TIMER0_BASE_CT_REG), &baseCount);
        sysPciWrite32(MPIC_ADDR(MPIC_TIMER0_BASE_CT_REG),
                      (baseCount | MPIC_TIMER_BASE_CT_CI));
        EIEIO;  /* synchronize */

        hawkAuxClkRunning = FALSE;
        }
    }

/*******************************************************************************
*
* hawkAuxClkEnable - turn on auxiliary clock interrupts
*
* This routine enables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: hawkAuxClkDisable()
*/
 
void hawkAuxClkEnable (void)
    {
    if (!hawkAuxClkRunning)
        {

        /* Direct interrupt at no one */

        sysPciWrite32 (MPIC_ADDR (MPIC_TIMER0_DEST_REG), 0);

        /* enable counter and write value to count from */

        sysPciWrite32 (MPIC_ADDR (MPIC_TIMER0_BASE_CT_REG),
                       ((MPIC_TIMER_CNT/hawkAuxClkTicksPerSecond) & 
                        ~(MPIC_TIMER_BASE_CT_CI)));

        /* Direct interrupt at CPU 0 */

        sysPciWrite32 (MPIC_ADDR (MPIC_TIMER0_DEST_REG), DESTINATION_CPU0);
        EIEIO;                  /* synchronize */

        hawkAuxClkRunning = TRUE;
        }
    }

/*******************************************************************************
*
* hawkAuxClkRateGet - get the auxiliary clock rate
*
* This routine returns the interrupt rate of the auxiliary clock.
*
* RETURNS: The number of ticks per second of the auxiliary clock.
*
* SEE ALSO: hawkAuxClkEnable(), hawkAuxClkRateSet()
*/
 
int hawkAuxClkRateGet (void)
     {
     return (hawkAuxClkTicksPerSecond);
     }

/*******************************************************************************
*
* hawkAuxClkRateSet - set the auxiliary clock rate
*
* This routine sets the interrupt rate of the auxiliary clock.  It is not
* supported, since the auxiliary clock always runs at the same rate as the
* system clock.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO: hawkAuxClkEnable(), hawkAuxClkRateGet()
*/
 
STATUS hawkAuxClkRateSet
    (
    int ticksPerSecond  /* number of clock interrupts per second */
    )
    {
    if (ticksPerSecond < AUX_CLK_RATE_MIN || ticksPerSecond > AUX_CLK_RATE_MAX)
        {
        return (ERROR);
        }

    hawkAuxClkTicksPerSecond = ticksPerSecond;

    if (hawkAuxClkRunning)
        {
        hawkAuxClkDisable ();
        hawkAuxClkEnable ();
        }

    return (OK);
    }


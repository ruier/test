/* sysMv64260AuxClk.c - MV64260 AuxClock library */

/* Copyright 2002-2004 Wind River Systems, Inc. */
/* Copyright 2003 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01e,16jul06,sup  PPC Dec Timer vxBus complaint
01d,01nov04,mdo  Documentation fixes for apigen
01c,06oct03,cak   Replaced the MV64260_TMR_RATE macro in mv64260AuxClkEnable with a 
          timerRate variable so that the timer (CPU) rate is not
          calculated each time the routine is called, instead it is
          calculated once in the init() routine.
01b,29jan03,cak   Redefined MV64260_TMR_RATE as CPU_BUS_SPD.
01a,14oct02,yyz   Adapted from hxeb100 base.
*/

/*
DESCRIPTION

There are a total of eight identical timer/counters on the MV64260,
each 32-bits wide.  Each timer/counter can be selected to operate as
a timer or a counter.  Each timer/counter increments with every Tclk
rising edge.  The MV64260 timer/counters are driven by the CPU bus clock.  

This file contains routines to use timer/counter 0 as the auxiliary 
clock.  The other seven timer/counters (1 through 7) are not affected 
by the AuxClock routines.  

In Counter mode, the counter counts down to zero, stops, and issues 
an interrupt.

In Timer mode, the timer counts down, issues an interrupt when it
reaches zero, reloads itself to the programmed value, and continues 
to count.

Besides eight timer/counter registers, there are six other timer
related registers:

Timer/Counter 0-3 Control Register -
Bit 0 is the Timer/Counter Enable bit of timer/counter 0.  Writing
a value of "0" disables timer/counter 0, while writing a value of
"1" enables timer/counter 0.  When in timer mode, the count continues 
as long as this bit is set to '1'.  Bit 1 is the Timer/Counter Mode bit, 
the timer/counter 0 is in timer mode if this bit is set to '1'.

Timer/Counter 0-3 Interrupt Cause Register -
Bit 0 is set to 1 upon timer 0 terminal count.  It's cleared by writing 
a value of "0".  Writing a value of "1" has no effect.

Timer/Counter 0-3 Interrupt Mask Register -
Writing a value of "0" to bit 0 disables timer/counter 0 interrupt.
Writing a value of "1" to bit 0 enables timer/counter 0 interrupt.

Timer/Counter 4-7 Control Register -
Same as Timer/Counter 0-3 Control Register.

Timer/Counter 4-7 Interrupt Cause Register -
Same as Timer/Counter 0-3 Interrupt Cause Register.

Timer/Counter 4-7 Interrupt Mask Register -
Same as Timer/Counter 0-3 Interrupt Mask Register.

INCLUDE FILES
*/

/* includes */

#include "mv64260.h"
#include "mv5500A.h"

/* defines */

/* typedefs */

/* globals */

/* locals */

LOCAL FUNCPTR   mv64260AuxClkRoutine    = NULL;
LOCAL int       mv64260AuxClkArg        = 0; 
LOCAL BOOL      mv64260AuxClkRunning    = FALSE;
LOCAL int       mv64260AuxClkTicksPerSecond = 60;
LOCAL BOOL      mv64260AuxClkConnected  = FALSE;
UINT32 timerRate = 0;

/* forward declarations */

LOCAL void  mv64260AuxClkInt     (void);
LOCAL STATUS    mv64260AuxClkInit    (void);
STATUS      mv64260AuxClkConnect (FUNCPTR routine, int arg);
void        mv64260AuxClkDisable (void);
void        mv64260AuxClkEnable  (void);
int         mv64260AuxClkRateGet (void);
STATUS      mv64260AuxClkRateSet (int ticksPerSecond);

/*******************************************************************************
*
* mv64260AuxClkInt - handle an auxiliary clock interrupt from Timer/Counter 0
*
* This routine handles an MV64260 Timer/Counter 0 interrupt.  It clears
* the interrupt and calls the routine installed by mv64260AuxClkConnect().
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void mv64260AuxClkInt (void)
    {

    /* clear timer/counter 0 cause bit */

    MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_INT_CAUSE_0_3,
              ((MV64260_READ32 
                           (MV64260_REG_BASE, TMR_CNTR_INT_CAUSE_0_3)) 
                          & ~TMR_CNTR_INT_CAUSE_TC0_MASK)); 

    if (mv64260AuxClkRoutine != NULL)
        {
        (*mv64260AuxClkRoutine) (mv64260AuxClkArg);
        }
    }

/*******************************************************************************
*
* mv64260AuxClkInit - MV64260 auxiliary clock initialization routine
*
* This routine should be called before calling any other routine in this
* module.
*
* RETURNS: OK, always.
*
* ERRNO
*/

LOCAL STATUS mv64260AuxClkInit (void)
    {

    /* set MV64260 timer rate to the CPU bus speed */

    timerRate = CPU_BUS_SPD;

    /* disable timer/counter 0 */

    MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3, 
              ((MV64260_READ32 
                           (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3)) &
              ~TMR_CNTR_CTRL_TC0EN_MASK));
    
    /* set timer/counter in timer mode */

    MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3, 
              ((MV64260_READ32 
                           (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3)) |
              TMR_CNTR_CTRL_TC0MOD_MASK));

    /* connect and enable the associated main cause interrupt */

    intConnect (TMR_CNTR0_INT_VEC, (VOIDFUNCPTR)mv64260AuxClkInt, 0);

    intEnable (TMR_CNTR0_INT_LVL);

    /* unmask timer/counter 0 interrupt */

    MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_INT_MASK_0_3,
              ((MV64260_READ32 
                           (MV64260_REG_BASE, TMR_CNTR_INT_MASK_0_3)) |
              TMR_CNTR_INT_MASK_TC0_MASK));

    mv64260AuxClkRunning = FALSE;

    return (OK);
    }

/********************************************************************************
* mv64260AuxClkConnect - connect a routine to the auxiliary clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* auxiliary clock interrupt.
*
* RETURNS: OK, always.
*
* ERRNO
*
* SEE ALSO: intConnect(), mv64260AuxClkEnable()
*/

STATUS mv64260AuxClkConnect
    (
    FUNCPTR routine,    /* routine called at each aux clock interrupt */
    int arg             /* argument with which to call routine */
    )
    {
    mv64260AuxClkRoutine    = routine;
    mv64260AuxClkArg        = arg;
    mv64260AuxClkConnected  = TRUE;

    return (OK);
    }

/*******************************************************************************
*
* mv64260AuxClkDisable - turn off auxiliary clock interrupts
*
* This routine disables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: mv64260AuxClkEnable()
*/

void mv64260AuxClkDisable (void)
    {
    if (mv64260AuxClkRunning)
        {

        /* stop counting */

        MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3,
                              ((MV64260_READ32 
                                (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3)) & 
                               ~TMR_CNTR_CTRL_TC0EN_MASK));

        /* disable timer/counter 0 interrupt */

        MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_INT_MASK_0_3,
                              ((MV64260_READ32 
                                (MV64260_REG_BASE, TMR_CNTR_INT_MASK_0_3)) & 
                               ~TMR_CNTR_INT_MASK_TC0_MASK));

        /* disable associated main cause interrupt */

        intDisable (TMR_CNTR0_INT_LVL);

        mv64260AuxClkRunning = FALSE;
        }
    }

/********************************************************************************
* mv64260AuxClkEnable - turn on auxiliary clock interrupts
*
* This routine enables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: mv64260AuxClkConnect(), mv64260AuxClkDisable()
*/

void mv64260AuxClkEnable (void)
    {
    if (!mv64260AuxClkRunning)
        {

        /* write value to count from */

        MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR0, 
                              (timerRate/mv64260AuxClkTicksPerSecond));

        /* enable timer/counter 0 */

        MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3,
                              ((MV64260_READ32 
                                (MV64260_REG_BASE, TMR_CNTR_CTRL_0_3)) | 
                               TMR_CNTR_CTRL_TC0EN_MASK));

        mv64260AuxClkRunning = TRUE;

        /* enable the associated main cause interrupt in case it was disabled */

        intEnable (TMR_CNTR0_INT_LVL);

        /* unmask timer/counter 0 interrupt */

        MV64260_WRITE32_PUSH (MV64260_REG_BASE, TMR_CNTR_INT_MASK_0_3,
                              ((MV64260_READ32 
                                (MV64260_REG_BASE, TMR_CNTR_INT_MASK_0_3)) | 
                               TMR_CNTR_INT_MASK_TC0_MASK));
        }
    }

/*******************************************************************************
*
* mv64260AuxClkRateGet - get the auxiliary clock rate
*
* This routine returns the interrupt rate of the auxiliary clock.
*
* RETURNS: The number of ticks per second of the auxiliary clock.
*
* ERRNO
*
* SEE ALSO: mv64260AuxClkEnable(), mv64260AuxClkRateSet()
*/

int mv64260AuxClkRateGet (void)
    { 
    return (mv64260AuxClkTicksPerSecond);
    }

/*******************************************************************************
*
* mv64260AuxClkRateSet - set the auxiliary clock rate
*
* This routine sets the interrupt rate of the auxiliary clock. It does not
* enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the tick rate is invalid or the timer cannot be set.
*
* ERRNO
*
* SEE ALSO: mv64260AuxClkEnable(), mv64260AuxClkRateGet()
*/

STATUS mv64260AuxClkRateSet
    (
    int ticksPerSecond  /* number of clock interrupts per second */
    )
    {
    if (ticksPerSecond < AUX_CLK_RATE_MIN || 
        ticksPerSecond > AUX_CLK_RATE_MAX)
        return (ERROR);

    mv64260AuxClkTicksPerSecond = ticksPerSecond;

    if (mv64260AuxClkRunning)
        {
        mv64260AuxClkDisable ();
        mv64260AuxClkEnable ();
        }
    return (OK);
    }

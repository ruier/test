/* sysMpc8540Wdog.c - MPC8540 Watchdog Timer Library */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01b,22sep05,pjh  Remove 'unused' variables to eliminate compiler warnings.
01a,25jul05,efb  Initial writing. 
*/

/*
DESCRIPTION

The MPC8540's watchdog timer is a low-resolution timer that operates
in two stages, providing first an interrupt and then an optional
system reset. To use the watchdog timer define INCLUDE_WDOG_TIMER in
config.h.

The timer uses the zero-to-one transition of any one of the 64 bit
positions from the upward counting time base register to detect a
timeout. The user specified time interval is necessarily rounded to
the nearest power-of-two value. The effect of this rounding is a very
coarse interval granularity requiring progressive doubling of time
values to increase the actual timeout. This also increases the
timeout error proportionately as well. These limitations are imposed
entirely by the hardware implementation in the MCP8540 and cannot be
altered.

It is also important to note that if system reset is specified to
occur (always on the second watchdog timeout) when the timer is
started, the timer CANNOT be disabled. When reset mode is active, the
timer must be periodically serviced without exception. The
sysMpc8540WdogStop routine has no effect and will return an error.
Again the hardware implementation drives this behavior and it cannot
be altered. The only way to clear the reset mode is to invoke a system
reset or to allow the timer to reset the system.

STATUS sysMpc8540WdogStart (seconds, reset)

This routine has two input parameters: the first is the number of seconds
until watchdog expiration for each stage, and the second is the watchdog 
timer reset mode. If reset is true, the watchdog timer will reset the 
system on the second stage timeout of the timer. If reset is false and no 
interrupt handler has been connected, then ERROR is returned as there 
is no action to take on timeout. Once set for reset, the reset mode 
cannot be changed per hardware restriction. To clear the reset mode, a 
system reset must occur. Specifying reset without an interrupt service
routine will result in a system reset after twice the specified time
period elapses unless the wdogService routine is called periodically. 
The wdogStop routine has no effect if reset has been selected.
Hardware does not allow different timeouts for each stage. Minmum seconds 
is one. Maximum seconds is 8640 (24 hours).

STATUS sysMpc8540WdogStop ()

This routine will stop the watchdog timer by disabling it.  The only way to
restart it is to call sysMpc8540WdogStart() again. If reset was
previously specified, the timer cannot be stopped without a system reset
occurring. This is a hardware restriction. 

STATUS sysMpc8540WdogService ()

This routine will service the watchdog timer.  Servicing the watchdog
periodically will prevent it from expiring and triggering an interrupt or
issuing a system reset. Servicing sets the watchdog to its initial state 
regardless of its current stage. The timer continues to run after 
servicing.  It can be serviced as often as desired, but at least once 
before each timeout interval expires.

BOOL sysMpc8540WdogIsRunning ()

This routine will return true if the watchdog timer is armed and running.

UINT32 sysMpc8540WdogPeriodGet ()

This routine returns the actual watchdog timer period supported by hardware
in seconds. This value is derived by rounding the user value to
the nearest power-of-two supported by the hardware. The period is for one 
stage and applies to both stages.

STATUS sysMpc8540WdogIntConnect (routine, arg)

This routine connects a user service routine w/argument, that will be 
invoked on the first stage of watchdog timer timeout. The routine is 
invoked within the driver's service routine's wrapper, so interrupt 
clearing and timer management does not need to be performed by the user 
routine. This routine should be called with watchdog in a stopped state.

*/

/* includes */

#include "vxWorks.h"
#include "vxLib.h"
#include "intLib.h"


/* defines */

#define	WDOG_MIN_SECS           1 
#define	WDOG_MAX_SECS           (24 * 60 * 60)  /* 8640 secs (24 hours) */ 
#undef	WDOG_DEBUG

/* Timer Control Register Watchdog Related Bit Fields */

#define TCR_WP_BIT       0
#define TCR_WP_MASK      (0xC0000000UL >> TCR_WP_BIT)

#define TCR_WRC_BIT      2
#define TCR_WRC_MASK     (0xC0000000UL >> TCR_WRC_BIT)
#define TCR_WRC_RESET    (0x80000000UL >> TCR_WRC_BIT)

#define TCR_WIE_BIT      4
#define TCR_WIE_MASK     (0x80000000UL >> TCR_WIE_BIT)

#define TCR_WPEXT_BIT    11
#define TCR_WPEXT_MASK   (0xF0000000UL >> TCR_WPEXT_BIT)

/* Timer Status Register Watchdog Related Bit Fields */

#define TSR_ENW_BIT     0
#define TSR_ENW_MASK    (0x80000000UL >> TSR_ENW_BIT)

#define TSR_WIS_BIT     1
#define TSR_WIS_MASK    (0x80000000UL >> TSR_WIS_BIT)

#define TSR_WRS_BIT     2
#define TSR_WRS_MASK    (0xC0000000UL >> TSR_WRS_BIT)
#define TSR_WRS_RESET   (0x80000000UL >> TSR_WRS_BIT)


/* typedefs */

typedef struct
    {
    UINT64 wdogPeriod;        /* Watchdog Timer periods       */
    UINT32 fpMask;            /* Corresponding TCR mask       */
    } WDOG_PERIOD;

/* globals */

/* locals */

LOCAL WDOG_PERIOD wdogTable[] =                 /* available WDOG periods */
    {
        { ((UINT64)1 << 0), 0xc01e0000 },       /* TBL[63] */
        { ((UINT64)1 << 1), 0x801e0000 },       /* TBL[62] */
        { ((UINT64)1 << 2), 0x401e0000 },       /* TBL[61] */
        { ((UINT64)1 << 3), 0x001e0000 },       /* TBL[60] */
        { ((UINT64)1 << 4), 0xc01c0000 },       /* TBL[59] */
        { ((UINT64)1 << 5), 0x801c0000 },       /* TBL[58] */
        { ((UINT64)1 << 6), 0x401c0000 },       /* TBL[57] */
        { ((UINT64)1 << 7), 0x001c0000 },       /* TBL[56] */
        { ((UINT64)1 << 8), 0xc01a0000 },       /* TBL[55] */
        { ((UINT64)1 << 9), 0x801a0000 },       /* TBL[54] */
        { ((UINT64)1 << 10), 0x401a0000 },      /* TBL[53] */
        { ((UINT64)1 << 11), 0x001a0000 },      /* TBL[52] */
        { ((UINT64)1 << 12), 0xc0180000 },      /* TBL[51] */
        { ((UINT64)1 << 13), 0x80180000 },      /* TBL[50] */
        { ((UINT64)1 << 14), 0x40180000 },      /* TBL[49] */
        { ((UINT64)1 << 15), 0x00180000 },      /* TBL[48] */
        { ((UINT64)1 << 16), 0xc0160000 },      /* TBL[47] */
        { ((UINT64)1 << 17), 0x80160000 },      /* TBL[46] */
        { ((UINT64)1 << 18), 0x40160000 },      /* TBL[45] */
        { ((UINT64)1 << 19), 0x00160000 },      /* TBL[44] */
        { ((UINT64)1 << 20), 0xc0140000 },      /* TBL[43] */
        { ((UINT64)1 << 21), 0x80140000 },      /* TBL[42] */
        { ((UINT64)1 << 22), 0x40140000 },      /* TBL[41] */
        { ((UINT64)1 << 23), 0x00140000 },      /* TBL[40] */
        { ((UINT64)1 << 24), 0xc0120000 },      /* TBL[39] */
        { ((UINT64)1 << 25), 0x80120000 },      /* TBL[38] */
        { ((UINT64)1 << 26), 0x40120000 },      /* TBL[37] */
        { ((UINT64)1 << 27), 0x00120000 },      /* TBL[36] */
        { ((UINT64)1 << 28), 0xc0100000 },      /* TBL[35] */
        { ((UINT64)1 << 29), 0x80100000 },      /* TBL[34] */
        { ((UINT64)1 << 30), 0x40100000 },      /* TBL[33] */
        { ((UINT64)1 << 31), 0x00100000 },      /* TBL[32] */
        { ((UINT64)1 << 32), 0xc00e0000 },      /* TBU[31] */
        { ((UINT64)1 << 33), 0x800e0000 },      /* TBU[30] */
        { ((UINT64)1 << 34), 0x400e0000 },      /* TBU[29] */
        { ((UINT64)1 << 35), 0x000e0000 },      /* TBU[28] */
        { ((UINT64)1 << 36), 0xc00c0000 },      /* TBU[27] */
        { ((UINT64)1 << 37), 0x800c0000 },      /* TBU[26] */
        { ((UINT64)1 << 38), 0x400c0000 },      /* TBU[25] */
        { ((UINT64)1 << 39), 0x000c0000 },      /* TBU[24] */
        { ((UINT64)1 << 40), 0xc00a0000 },      /* TBU[23] */
        { ((UINT64)1 << 41), 0x800a0000 },      /* TBU[22] */
        { ((UINT64)1 << 42), 0x400a0000 },      /* TBU[21] */
        { ((UINT64)1 << 43), 0x000a0000 },      /* TBU[20] */
        { ((UINT64)1 << 44), 0xc0080000 },      /* TBU[19] */
        { ((UINT64)1 << 45), 0x80080000 },      /* TBU[18] */
        { ((UINT64)1 << 46), 0x40080000 },      /* TBU[17] */
        { ((UINT64)1 << 47), 0x00080000 },      /* TBU[16] */
        { ((UINT64)1 << 48), 0xc0060000 },      /* TBU[15] */
        { ((UINT64)1 << 49), 0x80060000 },      /* TBU[14] */
        { ((UINT64)1 << 50), 0x40060000 },      /* TBU[13] */
        { ((UINT64)1 << 51), 0x00060000 },      /* TBU[12] */
        { ((UINT64)1 << 52), 0xc0040000 },      /* TBU[11] */
        { ((UINT64)1 << 53), 0x80040000 },      /* TBU[10] */
        { ((UINT64)1 << 54), 0x40040000 },      /* TBU[9] */
        { ((UINT64)1 << 55), 0x00040000 },      /* TBU[8] */
        { ((UINT64)1 << 56), 0xc0020000 },      /* TBU[7] */
        { ((UINT64)1 << 57), 0x80020000 },      /* TBU[6] */
        { ((UINT64)1 << 58), 0x40020000 },      /* TBU[5] */
        { ((UINT64)1 << 59), 0x00020000 },      /* TBU[4] */
        { ((UINT64)1 << 60), 0xc0000000 },      /* TBU[3] */
        { ((UINT64)1 << 61), 0x80000000 },      /* TBU[2] */
        { ((UINT64)1 << 62), 0x40000000 },      /* TBU[1] */
        { ((UINT64)1 << 63), 0x00000000 }       /* TBU[0] */
    };

LOCAL FUNCPTR sysMpc8540WdogRoutine = NULL;
LOCAL int     sysMpc8540WdogArg = 0x00;
LOCAL BOOL    wdogIsRunning = FALSE;
LOCAL BOOL    wdogIsIntConnected = FALSE;
LOCAL BOOL    wdogIsModeReset = FALSE;
LOCAL UINT64  wdogPeriod = 0;
LOCAL UINT32  wdogPeriodMask = 0x00;

/* forward declarations */

IMPORT void sysTimeBaseClr(void);
IMPORT UINT32 sysCpuBusSpd (void);

/*******************************************************************************
*
* sysMpc8540WdogInt - watchdog timer interrupt handler
*
* This routine is the interrupt handler for the MPC8540 watchdog timer.
* If no user interrupt handler is provided, and reset has been specified 
* the routine only disables addition timer interrupts. This is to allow reset 
* to occur (if so specified) on the next timeout without interrupt flooding. 
* A second stage timeout is indicated by a timeout with WIS bit already set.
* Otherwise the interrupt is cleared and time base reset. The timer is still 
* running during servicing.
*
* RETURNS: N/A
*/

LOCAL void sysMpc8540WdogInt (void)
    {

#ifdef WDOG_DEBUG
    printf ("DEBUG: Received watchdog timer interrupt\r\n");
#endif /* WDOG_DEBUG */

    /* call user-specified routine */

    if (sysMpc8540WdogRoutine != NULL)
        {
        (*sysMpc8540WdogRoutine)(sysMpc8540WdogArg);

        sysTimeBaseClr ();
        vxTsrSet (TSR_WIS_MASK | TSR_ENW_MASK);
        }
    else if (wdogIsModeReset)   
        {
      
        /* 
         * There is no user handler, but reset is enabled. 
         * To avoid interrupt flooding, during the second stage
         * time period, the watchdog interrupt is turned off.
         * If the timer is serviced during the second stage,
         * the interrupt will be re-enabled then.
         */

        vxTcrSet (vxTcrGet () & ~TCR_WIE_MASK);
        }
    else
        {
        
        /* This case should never happen */

        sysTimeBaseClr ();
        vxTsrSet (TSR_WIS_MASK | TSR_ENW_MASK);
        }

    }

/*******************************************************************************
*
* sysMpc8540WdogIntConnect- connect user specified interrupt handler 
*
* This routine connects a user specifed interrupt handler routine to be
* called by the watchdog timer's interrupt handler.
*
* RETURNS: OK if routine not already connected, ERROR otherwise.
*/

STATUS sysMpc8540WdogIntConnect 
    (
    FUNCPTR routine,	/* user-defined routine */
    UINT32  arg         /* argument for user routine */
    )
    {
    STATUS status = OK;

    if (sysMpc8540WdogRoutine == NULL)
	{
	sysMpc8540WdogRoutine = routine;
	sysMpc8540WdogArg = arg;
	}
    else
	{

#ifdef WDOG_DEBUG
	printf("Routine already connected\n\r");
#endif /* WDOG_DEBUG */

        status = ERROR;
	}

    return (status);
    }
 
/*******************************************************************************
* sysMpc8540WdogService - service the MPC8540 watchdog timer
*
* This routine services the MPC8540 watchdog timer. The time base register
* is cleared to zero and the "enable next watchdog time" bit is cleared. 
* The watchdog interrupt is ensured to be enabled for proper servicing if
* in the second stage.
*
* RETURNS: OK always. 
*/

STATUS sysMpc8540WdogService (void)
    {
    sysTimeBaseClr ();
    vxTsrSet (TSR_WIS_MASK | TSR_ENW_MASK);
    vxTcrSet (vxTcrGet () | TCR_WIE_MASK);
    
    sysTimeBaseClr ();
    return (OK);
    }

/*******************************************************************************
*
* sysMpc8540WdogIsRunning - return the running status of the watchdog.
*
* This routine returns true if the watchdog is armed and running.
*
* RETURNS: TRUE if the watchdog is running, FALSE otherwise.
*/

BOOL sysMpc8540WdogIsRunning (void)
    {
    return (wdogIsRunning);
    }

/*******************************************************************************
*
* sysMpc8540WdogPeriodGet - returns the actual watchdog timeout period.
*
* This routine returns the actual watchdog timer period supported by hardware
* in seconds. The is value that the user value was rounded to be. The period
* is for one stage and applies to both stages.
*
* RETURNS: Actual timeout period in seconds.
*/

UINT32 sysMpc8540WdogPeriodGet (void)
    {
    return (wdogPeriod * 3 / (UINT64)(sysCpuBusSpd () / 8));
    }

/*******************************************************************************
*
* sysMpc8540WdogStop - Stop the MPC8540 Watchdog Timer
*
* This routine will stop the MPC8540 watchdog timer if reset was NOT 
* originally specified. Hardware does not allow the timer to be disabled if
* reset mode was enabled without an actual system reset occuring.
*
* RETURNS: OK if successfully stopped, ERROR if not running or in system reset
* mode.
*/

STATUS sysMpc8540WdogStop (void)
    {
    STATUS status = OK;

    if (wdogIsRunning && !wdogIsModeReset)
        {
        sysTimeBaseClr ();

        /* Disable the wdog interrupt */

        vxTcrSet (vxTcrGet () & ~(TCR_WP_MASK | TCR_WPEXT_MASK | TCR_WIE_MASK));

        /* Clear the pending wdog interrupt */

        vxTsrSet (TSR_WIS_MASK | TSR_ENW_MASK);

        /* Reset the running flag */

        wdogIsRunning = FALSE;
        }
    else
        {

#ifdef WDOG_DEBUG
	printf("DEBUG: watchdog not enabled or mode is reset\n");
#endif /* WDOG_DEBUG */

	status = ERROR;
	}
    
    return (status);
    }

/*******************************************************************************
*
* sysMpc8540WdogStart - start the MPC8540 watchdog timer
* 
* This routine configures (based on user input), and starts the MPC8540
* watchdog timer. The time in seconds specifies the length of timeout for
* one stage and applies to both stages. If reset is specified as true, the
* system will be reset on the second timeout of the timer if the timer is
* not serviced. Hardware does not allow the timer to be stopped or disabled
* if reset is specified without an actual reset occurring. This means that
* when in reset mode, the timer must be continually serviced or a reset
* will occur even if sysMpc8540WdogStop() is called.
*
* RETURNS: OK if watchdog timer successfully started, ERROR otherwise. 
*/

STATUS sysMpc8540WdogStart
    (
    UINT32 seconds,	/* number of seconds to watchdog expiration */
    BOOL   reset	/* watchdog timer can reset system if true */
    )
    {
    STATUS status = OK;
    UINT32 ix = 0;
    UINT32 tcrVal = 0x00;

    if (seconds >= WDOG_MIN_SECS && seconds <= WDOG_MAX_SECS)
        {
        if (!reset && sysMpc8540WdogRoutine == NULL)
            {
            
            /* No actions on timeout. No point in starting timer. */

#ifdef WDOG_DEBUG
    printf ("DEBUG: No user int handler and no reset specified. \r\n");
#endif /* WDOG_DEBUG */

            status = ERROR;
            }
        else
            {
      
            /* Make sure timer is stopped */

            sysMpc8540WdogStop ();

            /* Calculate the timer period */

            wdogIsModeReset = reset;
            wdogPeriod = (UINT64)(sysCpuBusSpd () / 8) * (UINT64)seconds / 2;

            /* Get the closest supported value to the given period */

            while (wdogTable[ix].wdogPeriod < wdogPeriod && 
                   ix < NELEMENTS (wdogTable))
                {
                ++ix;
                } 

            if (ix == NELEMENTS (wdogTable) || (ix > 0 && 
                ((wdogTable[ix].wdogPeriod - wdogPeriod) >=
                (wdogPeriod - wdogTable[ix - 1].wdogPeriod))))
                {
                --ix;
                }

            wdogPeriod = wdogTable[ix].wdogPeriod;
            wdogPeriodMask = wdogTable[ix].fpMask;  

            /* Clear the pending wdog interrupt */
    
            vxTsrSet (TSR_WIS_MASK | TSR_ENW_MASK);
    
            if (!wdogIsIntConnected)
                {
                excIntCrtConnect ((VOIDFUNCPTR *)_EXC_OFF_WD, 
                                  (VOIDFUNCPTR)sysMpc8540WdogInt);
                wdogIsIntConnected = TRUE;
                }

            /* Program TCR with the wdog period, and reset mode */
    
            tcrVal = vxTcrGet () & ~(TCR_WP_MASK | TCR_WPEXT_MASK);
            tcrVal |= wdogPeriodMask;
    
            if (wdogIsModeReset)
                {
                tcrVal |= TCR_WRC_RESET;    
                }

            sysTimeBaseClr ();

            /* Enable watchdog interrupt */

            vxTcrSet (tcrVal | TCR_WIE_MASK);
    
            wdogIsRunning = TRUE;
            sysTimeBaseClr ();
            }
        }    
    else
        {
             
        /* Time period out of range */ 
 
        status = ERROR;
        }

    return (status);
    }


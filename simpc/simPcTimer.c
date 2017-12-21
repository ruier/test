/* simPcTimer.c - simpc timer library */

/*
 * Copyright (c) 1998-2000,2003-2004,2007-2008,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

/*
modification history
--------------------
01o,28dec10,dbt  Fixed 64-bit clock support (CQ:WIND00249311).
01n,03jun10,elp  64bits support.
01m,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01l,08jan08,elp  include auxiliary clock conditionally (CQ:WIND113082).
01k,17sep07,elp  fixed typo in spin lock usage (CQ:WIND104799),
		 fixed SMP timestamp. 
01j,30aug07,elp  fixed warnings
01i,21aug07,elp  added SMP support.
01h,26jul04,jmp  fixed sysClkConnect() and sysAuxClkConnect() to be able
		 to re-connect another routine (SPR #98816).
01g,07may03,dbt  Fixed sysTimestampDisable() description.
01f,28apr03,dbt  Added vxsimHostTimerInit() call.
		 Added sysHwInit2() call.
01e,17jan03,jeg  add system timestamp timer based on Windows performance counter
01d,07sep00,elp  fixed documentation.
01c,21mar00,elp  fixed warnings.
01b,09dec99,elp  adapted since clock is no longer implemented as a WINDOWS
                 timer.
01a,07jul98,cym  written.
*/

/*
DESCRIPTION
Clock interrupts are delivered through a high priotity thread.
It sends the WM_TIMER message/interrupt, which also passes in a timer ID.
This driver's multiplexer then calls the appropriate driver function based
on the ID.

if INCLUDE_TIMESTAMP is defined, a timestamp driver based on the windows
performance counter is provided.

INCLUDES:
win_Lib.h
intLib.h
*/

/* includes */

#include <vsbConfig.h>
#include <private/vxsimHostLibP.h>
#include <spinLockLib.h>
#include <intLib.h>

/* defines */

#ifndef AUX_CLK_RATE_MIN
#define AUX_CLK_RATE_MIN	SYS_CLK_RATE_MIN
#endif

#ifndef AUX_CLK_RATE_MAX
#define AUX_CLK_RATE_MAX	SYS_CLK_RATE_MAX
#endif

#define SYS_CLK			0		/* system clock id */
#define AUX_CLK			1		/* auxiliary clock id */

#ifdef INCLUDE_TIMESTAMP
#define TIMESTAMP_MAX_VALUE     0xffffffff	/* maximum value of the	      */
						/* timestamp counter before a */
						/* rollover */
#endif /* INCLUDE_TIMESTAMP */

#ifdef _WRS_CONFIG_SMP
#define TIMER_SPIN_LOCK_ISR_TAKE(lock,taken)	\
    if (timerIsInitialized)			\
    	{					\
	SPIN_LOCK_ISR_TAKE (lock);		\
	taken = TRUE;				\
	}

#define TIMER_SPIN_LOCK_ISR_GIVE(lock,taken)	\
    if (taken)					\
    	{					\
	SPIN_LOCK_ISR_GIVE(lock);		\
	taken = FALSE;				\
	}

#else /* _WRS_CONFIG_SMP */
#define TIMER_SPIN_LOCK_ISR_TAKE(lock,taken)
#define TIMER_SPIN_LOCK_ISR_GIVE(lock,taken)
#endif /* _WRS_CONFIG_SMP */

/* typedefs */

typedef void (*CLK_FUNCPTR) (_Vx_usr_arg_t arg);

/* locals */

LOCAL CLK_FUNCPTR	clockRoutines[2];	/* Handler for SYS/AUX clock */
LOCAL _Vx_usr_arg_t	clockArgs[2];		/* Args for SYS/AUX handler */
LOCAL BOOL	timerIsInitialized = FALSE;	/* Timer is initialized ? */
#ifdef _WRS_CONFIG_SMP
LOCAL spinlockIsr_t	timerLock;		/* protect clock handlers */
#endif /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_TIMESTAMP
LOCAL   CLK_FUNCPTR sysTimestampRoutine = NULL; /* rollover routine          */
LOCAL   _Vx_usr_arg_t sysTimestampArg;	        /* rollover routine argument */
#endif /* INCLUDE_TIMESTAMP */

/* forward declarations */

#ifdef INCLUDE_TIMESTAMP
LOCAL void	sysTimestampInt (void);
#endif /* INCLUDE_TIMESTAMP */

/******************************************************************************
*
* simPcTimerInit - clock timer initialization
*
* This routine initialize clock timer.
*
* RETURNS: N/A
*/

LOCAL void simPcTimerInit (void)
    {
    if (!timerIsInitialized)
    	{
#ifdef _WRS_CONFIG_SMP
	/* Initialize the spinlock. */

	SPIN_LOCK_ISR_INIT(&timerLock, 0);
#endif /* _WRS_CONFIG_SMP */

	/* Initialize host timer */

	vxsimHostTimerInit (TIMER_SYS_CLK_INT, TIMER_AUX_CLK_INT);
	timerIsInitialized = TRUE;
	}
    }

/*******************************************************************************
*
* sysClkInt - interrupt level processing for system clock
*
* This routine handles a system clock interrupt.
* It simply calls the routine installed by sysClkConnect() for that timer.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysClkInt
    (
    int		vector,		/* vector number		*/
    int		intParam1,	/* not used			*/ 
    int		intParam2	/* not used			*/
    )
    {
    CLK_FUNCPTR	clkRtn = NULL;
    _Vx_usr_arg_t	clkArg;

#ifdef _WRS_CONFIG_SMP
    SPIN_LOCK_ISR_TAKE(&timerLock);
#endif /* _WRS_CONFIG_SMP */
    clkRtn = clockRoutines[SYS_CLK];
    clkArg = clockArgs[SYS_CLK];
#ifdef _WRS_CONFIG_SMP
    SPIN_LOCK_ISR_GIVE(&timerLock);
#endif /* _WRS_CONFIG_SMP */

    if (clkRtn != NULL)
	clkRtn (clkArg);
    }

/*******************************************************************************
*
* sysClkConnect - connect a routine to the system clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* clock interrupt from the system timer.  Normally, it is called from usrRoot()
* in usrConfig.c to connect usrClock() to the system clock interrupt.
*
* RETURN: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), usrClock(), sysClkEnable()
*/

STATUS sysClkConnect
    (
    FUNCPTR		routine,
    _Vx_usr_arg_t	arg
    )
    {
    static BOOL	sysClkIsConnected = FALSE;
#ifdef  _WRS_CONFIG_SMP 
    BOOL spinTaken = FALSE;
#else
    int	key;
#endif /* _WRS_CONFIG_SMP */

    TIMER_SPIN_LOCK_ISR_TAKE(&timerLock, spinTaken);
    if (!sysClkIsConnected)
	{
	sysClkIsConnected = TRUE;
	TIMER_SPIN_LOCK_ISR_GIVE(&timerLock, spinTaken);
	sysHwInit2 ();		/* Do this only once */
	TIMER_SPIN_LOCK_ISR_TAKE(&timerLock, spinTaken);
	}

#ifndef _WRS_CONFIG_SMP
    key = intLock();
#endif /* !_WRS_CONFIG_SMP */

    clockRoutines[SYS_CLK]	= (CLK_FUNCPTR)routine;
    clockArgs[SYS_CLK]		= arg;

#ifdef _WRS_CONFIG_SMP
    TIMER_SPIN_LOCK_ISR_GIVE(&timerLock, spinTaken);
#else
    intCpuUnlock (key);
#endif /* _WRS_CONFIG_SMP */

    intConnect ((VOIDFUNCPTR *)TIMER_SYS_CLK_INT, (VOIDFUNCPTR)sysClkInt, 0);

    return(OK);
    }

/*******************************************************************************
*
* sysClkDisable - turn off system clock interrupts
*
* This routine disables system clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysClkEnable()
*/

void sysClkDisable(void)
    {
    vxsimHostTimerDisable (SYS_CLK);
    }

/*******************************************************************************
*
* sysClkEnable - turn on system clock interrupts
*
* This routine enables system clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysClkConnect(), sysClkDisable(), sysClkRateSet()
*/

void sysClkEnable(void)
    {
    simPcTimerInit ();
    vxsimHostTimerEnable (SYS_CLK);
    }

/*******************************************************************************
*
* sysClkRateGet - get the system clock rate
*
* This routine returns the system clock rate.
*
* RETURNS: The number of ticks per second of the system clock.
*
* SEE ALSO: sysClkEnable(), sysClkRateSet()
*/

int sysClkRateGet(void)
    {
    return (vxsimHostTimerRateGet (SYS_CLK));
    }

/*******************************************************************************
*
* sysClkRateSet - set the system clock rate
*
* This routine sets the interrupt rate of the system clock.
* It is called by usrRoot() in usrConfig.c.
*
* RETURNS: OK, or ERROR if the tick rate is invalid or the timer cannot be set.
*
* SEE ALSO: sysClkEnable(), sysClkRateGet()
*/

STATUS sysClkRateSet
    (
    int ticksPerSecond		/* number of clock interrupts per second */
    )
    {
    if (ticksPerSecond < SYS_CLK_RATE_MIN || ticksPerSecond > SYS_CLK_RATE_MAX)
        return(ERROR);

    return (vxsimHostTimerRateSet (SYS_CLK, ticksPerSecond));
    }

#ifdef INCLUDE_AUX_CLK
/*******************************************************************************
*
* sysAuxClkInt - interrupt level processing for auxiliary clock
*
* This routine handles an auxiliary clock interrupt. 
* It simply calls the routine installed by sysClkConnect() for that timer.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysAuxClkInt
    (
    int		vector,		/* vector number		*/
    int		intParam1,	/* not used			*/ 
    int		intParam2	/* not used			*/
    )
    {
    CLK_FUNCPTR	auxClkRtn = NULL;
    _Vx_usr_arg_t	auxClkArg;

#ifdef _WRS_CONFIG_SMP
    SPIN_LOCK_ISR_TAKE(&timerLock);
#endif /* _WRS_CONFIG_SMP */
    auxClkRtn = clockRoutines[AUX_CLK];
    auxClkArg = clockArgs[AUX_CLK];
#ifdef _WRS_CONFIG_SMP
    SPIN_LOCK_ISR_GIVE(&timerLock);
#endif /* _WRS_CONFIG_SMP */
    if (auxClkRtn != NULL)
	auxClkRtn (auxClkArg);
    }

/*******************************************************************************
*
* sysAuxClkConnect - connect a routine to the auxiliary clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* auxiliary clock interrupt.  It does not enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), sysAuxClkEnable()
*/

STATUS sysAuxClkConnect
    (
    FUNCPTR	  routine, /* routine called at each aux clock interrupt */
    _Vx_usr_arg_t arg	   /* argument to auxiliary clock interrupt routine */
    )
    {
#ifdef  _WRS_CONFIG_SMP
    BOOL spinTaken = FALSE;
    TIMER_SPIN_LOCK_ISR_TAKE(&timerLock, spinTaken);
#else
    int key = intCpuLock();
#endif /* _WRS_CONFIG_SMP */

    clockRoutines[AUX_CLK]	= (CLK_FUNCPTR)routine;
    clockArgs[AUX_CLK]		= arg;

#ifdef _WRS_CONFIG_SMP
    TIMER_SPIN_LOCK_ISR_GIVE(&timerLock, spinTaken);
#else
    intCpuUnlock (key);
#endif /* _WRS_CONFIG_SMP */

    intConnect ((VOIDFUNCPTR *)TIMER_AUX_CLK_INT, (VOIDFUNCPTR)sysAuxClkInt, 0);

    return(OK);
    }

/*******************************************************************************
*
* sysAuxClkDisable - turn off auxiliary clock interrupts
*
* This routine disables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysAuxClkEnable()
*/

void sysAuxClkDisable(void)
    {
    vxsimHostTimerDisable (AUX_CLK);
    }

/*******************************************************************************
*
* sysAuxClkEnable - turn on auxiliary clock interrupts
*
* This routine enables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysAuxClkConnect(), sysAuxClkDisable(), sysAuxClkRateSet()
*/

void sysAuxClkEnable(void)
    {
    simPcTimerInit ();
    vxsimHostTimerEnable (AUX_CLK);
    }

/*******************************************************************************
*
* sysAuxClkRateGet - get the auxiliary clock rate
*
* This routine returns the interrupt rate of the auxiliary clock.
*
* RETURNS: The number of ticks per second of the auxiliary clock.
*
* SEE ALSO: sysAuxClkEnable(), sysAuxClkRateSet()
*/

int sysAuxClkRateGet(void)
    {
    return ((int) vxsimHostTimerRateGet (AUX_CLK));
    }

/*******************************************************************************
*
* sysAuxClkRateSet - set the auxiliary clock rate
*
* This routine sets the interrupt rate of the auxiliary clock.  It does not
* enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the tick rate is invalid or the timer cannot be set.
*
* SEE ALSO: sysAuxClkEnable(), sysAuxClkRateGet()
*/

STATUS sysAuxClkRateSet
    (
    int ticksPerSecond		/* number of clock interrupts per second */
    )
    {
    if (ticksPerSecond < AUX_CLK_RATE_MIN || ticksPerSecond > AUX_CLK_RATE_MAX)
        return(ERROR);

    return (vxsimHostTimerRateSet (AUX_CLK, ticksPerSecond));
    }
#endif /* INCLUDE_AUX_CLK */

#ifdef  INCLUDE_TIMESTAMP
/*******************************************************************************
*
* sysTimestampInt - timestamp timer interrupt handler
*
* This rountine handles the timestamp timer interrupt.  A user routine is
* called, if one was connected by sysTimestampConnect().
*
* RETURNS: N/A
*
* SEE ALSO: sysTimestampConnect()
*/

LOCAL void sysTimestampInt (void)
    {
    CLK_FUNCPTR timestampRtn;
    _Vx_usr_arg_t	timestampArg;

#ifdef _WRS_CONFIG_SMP
    SPIN_LOCK_ISR_TAKE(&timerLock);
#endif /* _WRS_CONFIG_SMP */
    timestampRtn = (CLK_FUNCPTR)sysTimestampRoutine;
    timestampArg = sysTimestampArg;
#ifdef _WRS_CONFIG_SMP
    SPIN_LOCK_ISR_GIVE(&timerLock);
#endif /* _WRS_CONFIG_SMP */

    if (timestampRtn != NULL)    /* call user connected routine */
        (*timestampRtn) (timestampArg);
    }

/*******************************************************************************
*
* sysTimestampConnect - connect a user routine to the timestamp timer interrupt
*
* This routine specifies the user interrupt routine to be called at each
* time the timestamp counter rolls over.
*
* RETURNS: OK always.
*/

STATUS sysTimestampConnect
    (
    FUNCPTR routine,	/* routine called at each timestamp timer rollover */
    _Vx_usr_arg_t arg	/* argument with which to call routine             */
    )
    {
#ifdef _WRS_CONFIG_SMP
    BOOL spinTaken = FALSE;

    TIMER_SPIN_LOCK_ISR_TAKE (&timerLock, spinTaken);
#else
    int key = intCpuLock();
#endif /* _WRS_CONFIG_SMP */

    sysTimestampRoutine = (CLK_FUNCPTR)routine;
    sysTimestampArg = arg;

#ifdef _WRS_CONFIG_SMP
    TIMER_SPIN_LOCK_ISR_GIVE (&timerLock, spinTaken);
#else
    intCpuUnlock (key);
#endif /* _WRS_CONFIG_SMP */

    intConnect ((VOIDFUNCPTR *)TIMER_TS_ROLL_OVER_INT,
		(VOIDFUNCPTR)sysTimestampInt, 0);

    return (OK);
    }

/*******************************************************************************
*
* sysTimestampEnable - initialize and enable the timestamp timer
*
* If the timer is not aleady enabled, this routine performs all the necessary
* initialization. Furthermore, it checks the validity of windows performance
* counter routines call. Else, an OK status is returned.
* updated.
*
* RETURNS: OK, or ERROR if windows performance counter routines check failed.
*/

STATUS sysTimestampEnable (void)
    {
    simPcTimerInit();
    return (vxsimHostTimerTimestampEnable (TIMER_TS_ROLL_OVER_INT));
    }

/*******************************************************************************
*
* sysTimestampDisable - disable the timestamp timer
*
* This routine disables the timestamp timer.
*
* RETURNS: OK or ERROR if timestamp timer could not be disabled
*
* SEE ALSO: sysTimestampEnable()
*/

STATUS sysTimestampDisable (void)
    {
    return (vxsimHostTimerTimestampDisable ());
    }

/*******************************************************************************
*
* sysTimestampPeriod - get the period of a timestamp timer 
*
* This routine gets the period of the timestamp timer, in ticks.  The
* period, or terminal count, is the number of ticks to which the timestamp
* timer counts before rolling over.
*
* RETURNS: The period of the timestamp timer in counter ticks.
*/
UINT32 sysTimestampPeriod (void)
    {
    return (TIMESTAMP_MAX_VALUE);
    }

/*******************************************************************************
*
* sysTimestampFreq - get timestamp timer clock frequency
*
* This routine gets the frequency of the timer clock, in ticks per 
* second.  The rate of the timestamp timer is set explicitly by the 
* hardware and typically cannot be altered.
*
* RETURNS: The timestamp timer clock frequency, in ticks per second.
*/

UINT32 sysTimestampFreq (void)
    {
    return (vxsimHostTimerTimestampFreqGet ());
    }

/*******************************************************************************
*
* sysTimestamp - get a timestamp timer tick count
*
* This routine returns the current value of the timestamp timer tick counter.
* The tick count can be converted to seconds by dividing it by the return of
* sysTimestampFreq().
*
* This routine should be called with interrupts locked.  If interrupts are
* not locked, sysTimestampLock() should be used instead.
*
* RETURNS: The current timestamp timer tick count.
*
* SEE ALSO: sysTimestampFreq(), sysTimestampLock()
*/

UINT32 sysTimestamp (void)
    {
    return (vxsimHostTimerTimestampGet ());
    }

/*******************************************************************************
*
* sysTimestampLock - lock interrupts and get the timestamp timer tick count
*
* This routine locks interrupts when the tick counter must be stopped 
* in order to read it or when two independent counters must be read.  
* It then returns the current value of the timestamp timer tick
* counter.
* 
* The tick count can be converted to seconds by dividing it by the return of
* sysTimestampFreq().
*
* If interrupts are already locked, sysTimestamp() should be
* used instead.
*
* RETURNS: The current timestamp timer tick count.
*
* SEE ALSO: sysTimestampFreq(), sysTimestamp()
*/

UINT32 sysTimestampLock (void)
    {
    return (sysTimestamp ());
    }
#endif  /* INCLUDE_TIMESTAMP */


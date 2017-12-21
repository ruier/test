/* usrVxbTimerSys.c - VxBus timer system configuration file */

/*
 * Copyright (c) 2008-2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,17aug11,dee  Defect WIND00278138 - add 64-bit timestamp support
01d,16mar09,h_k  cleaned up compiler warnings.
01c,22oct08,h_k  added show routines support.
01b,07aug08,h_k  added timer assignment feature. (CQ:121884)
01a,27jun08,pmr  created.
*/

/*
DESCRIPTION

This is the VxBus timer system configuration file for use with project builds.

*/

/* definitions */

#ifndef	SYSCLK_TIMER_NAME
#define SYSCLK_TIMER_NAME	NULL
#endif	/* SYSCLK_TIMER_NAME */

#ifndef	SYSCLK_TIMER_UNIT
#define SYSCLK_TIMER_UNIT	0
#endif	/* SYSCLK_TIMER_UNIT */

#ifndef	SYSCLK_TIMER_NUM
#define SYSCLK_TIMER_NUM	0
#endif	/* SYSCLK_TIMER_NUM */

#ifndef	AUXCLK_TIMER_NAME
#define AUXCLK_TIMER_NAME	NULL
#endif	/* AUXCLK_TIMER_NAME */

#ifndef	AUXCLK_TIMER_UNIT
#define AUXCLK_TIMER_UNIT	0
#endif	/* AUXCLK_TIMER_UNIT */

#ifndef	AUXCLK_TIMER_NUM
#define AUXCLK_TIMER_NUM	0
#endif	/* AUXCLK_TIMER_NUM */

#ifndef	TIMESTAMP_TIMER_NAME
#define TIMESTAMP_TIMER_NAME	NULL
#endif	/* TIMESTAMP_TIMER_NAME */

#ifndef	TIMESTAMP_TIMER_UNIT
#define TIMESTAMP_TIMER_UNIT	0
#endif	/* TIMESTAMP_TIMER_UNIT */

#ifndef	TIMESTAMP_TIMER_NUM
#define TIMESTAMP_TIMER_NUM	0
#endif	/* TIMESTAMP_TIMER_NUM */

/* externs */

IMPORT char *	pSysClkName;
IMPORT UINT32	sysClkDevUnitNo;
IMPORT UINT32	sysClkTimerNo;

IMPORT char *	pAuxClkName;
IMPORT UINT32	auxClkDevUnitNo;
IMPORT UINT32	auxClkTimerNo;

IMPORT char *	pTimestampTimerName;
IMPORT UINT32	timestampDevUnitNo;
IMPORT UINT32	timestampTimerNo;

IMPORT STATUS vxbSysClkConnect (FUNCPTR routine, _Vx_usr_arg_t arg);
IMPORT void vxbSysClkDisable (void);
IMPORT void vxbSysClkEnable (void); 
IMPORT int vxbSysClkRateGet (void);
IMPORT STATUS vxbSysClkRateSet (int);
IMPORT void vxbSysClkShowInit (void);
IMPORT void vxbAuxClkShowInit (void);
IMPORT void vxbTimestampShowInit (void);

/******************************************************************************
*
* usrVxbTimerSysInit - initialize VxBus Timer interfaces module configuration
*
* This routine initializies the configuration for the VxBus Timer interfaces
* module.
*
* RETURNS: N/A.
*
* ERRNO: N/A.
*
*/

void usrVxbTimerSysInit (void)
    {
    /* assign the timer */

    /* system clock timer */

    pSysClkName	        = SYSCLK_TIMER_NAME;
    sysClkDevUnitNo     = SYSCLK_TIMER_UNIT;
    sysClkTimerNo       = SYSCLK_TIMER_NUM;

#ifdef	INCLUDE_TIMER_SYS_SHOW
    vxbSysClkShowInit ();
#endif	/* INCLUDE_TIMER_SYS_SHOW */

#ifdef	INCLUDE_AUX_CLK
    /* AUX clock timer */

    pAuxClkName         = AUXCLK_TIMER_NAME;
    auxClkDevUnitNo     = AUXCLK_TIMER_UNIT;
    auxClkTimerNo       = AUXCLK_TIMER_NUM;

# ifdef	INCLUDE_TIMER_SYS_SHOW
    vxbAuxClkShowInit ();
# endif	/* INCLUDE_TIMER_SYS_SHOW */
#endif	/* INCLUDE_AUX_CLK */

#ifdef	INCLUDE_TIMESTAMP
    /* Timestamp timer */

    pTimestampTimerName = TIMESTAMP_TIMER_NAME;
    timestampDevUnitNo  = TIMESTAMP_TIMER_UNIT;
    timestampTimerNo    = TIMESTAMP_TIMER_NUM;

# ifdef	INCLUDE_TIMER_SYS_SHOW
    vxbTimestampShowInit ();
# endif	/* INCLUDE_TIMER_SYS_SHOW */
#endif	/* INCLUDE_TIMESTAMP */
    }

STATUS sysClkConnect (FUNCPTR routine, _Vx_usr_arg_t arg)
    {
    return (vxbSysClkConnect (routine, arg));
    }

void sysClkDisable (void)
    {
    vxbSysClkDisable ();
    }

void sysClkEnable (void) 
    {
    vxbSysClkEnable ();
    }

int sysClkRateGet (void)
    {
    return (vxbSysClkRateGet ());
    }

STATUS sysClkRateSet (int ticksPerSecond) 
    {
    return (vxbSysClkRateSet (ticksPerSecond));
    }
   
#ifdef INCLUDE_AUX_CLK

IMPORT STATUS vxbAuxClkConnect (FUNCPTR routine, _Vx_usr_arg_t arg);
IMPORT void vxbAuxClkDisable (void);
IMPORT void vxbAuxClkEnable (void); 
IMPORT int vxbAuxClkRateGet (void);
IMPORT STATUS vxbAuxClkRateSet (int);

STATUS sysAuxClkConnect (FUNCPTR routine, _Vx_usr_arg_t arg)
    {
    return (vxbAuxClkConnect (routine, arg));
    }

void sysAuxClkDisable (void)
    {
    vxbAuxClkDisable ();
    }

void sysAuxClkEnable (void) 
    {
    vxbAuxClkEnable ();
    }

int sysAuxClkRateGet (void)
    {
    return (vxbAuxClkRateGet ());
    }

STATUS sysAuxClkRateSet (int ticksPerSecond) 
    {
    return (vxbAuxClkRateSet (ticksPerSecond));
    }
   
#endif /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_TIMESTAMP

IMPORT STATUS vxbTimestampConnect (FUNCPTR routine, _Vx_usr_arg_t arg);
IMPORT STATUS vxbTimestampDisable (void);
IMPORT STATUS vxbTimestampEnable (void); 
IMPORT UINT32 vxbTimestampPeriod (void);
IMPORT UINT32 vxbTimestampFreq (void);
IMPORT UINT32 vxbTimestamp (void);
IMPORT UINT32 vxbTimestampLock (void);

STATUS sysTimestampConnect (FUNCPTR routine, _Vx_usr_arg_t arg)
    {
    return (vxbTimestampConnect (routine, arg));
    }

STATUS sysTimestampDisable (void)
    {
    return (vxbTimestampDisable ());
    }

STATUS sysTimestampEnable (void) 
    {
    return (vxbTimestampEnable ());
    }

UINT32 sysTimestampPeriod (void)
    {
    return (vxbTimestampPeriod ());
    }

UINT32 sysTimestampFreq (void)
    {
    return (vxbTimestampFreq ());
    }

UINT32 sysTimestamp (void)
    {
    return (vxbTimestamp ());
    }

UINT32 sysTimestampLock (void)
    {
    return (vxbTimestampLock ());
    }

#endif /* INCLUDE_TIMESTAMP */

#ifdef INCLUDE_TIMESTAMP64

/* define 64 bit versions of necessary functions */

IMPORT UINT64 vxbTimestamp64Period (void);
IMPORT UINT64 vxbTimestamp64Freq (void);
IMPORT STATUS vxbTimestamp64 (UINT64 * counterVal);
IMPORT STATUS vxbTimestamp64Lock (UINT64 * counterVal);

UINT64 sysTimestamp64Period (void)
    {
    return (vxbTimestamp64Period ());
    }

UINT64 sysTimestamp64Freq (void)
    {
    return (vxbTimestamp64Freq ());
    }

STATUS sysTimestamp64 (UINT64 * counterVal)
    {
    return (vxbTimestamp64 (counterVal));
    }

STATUS sysTimestamp64Lock (UINT64 * counterVal)
    {
    return (vxbTimestamp64Lock (counterVal));
    }

#endif /* INCLUDE_TIMESTAMP64 */

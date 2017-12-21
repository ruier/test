/* usrPosixSc.c - POSIX system call initialization */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,01sep04,job   Renamed posixScLib to mqSemPxScLib
01a,23aug04,job   written.
*/

/*
DESCRIPTION
This file configures POSIX system call support based on the level of 
POSIX support configured into the system.

SEE ALSO: usrExtra.c

NOMANUAL
*/

#ifndef  __INCusrPosixSc
#define  __INCusrPosixSc

/* includes */

#include "vxWorks.h"

extern void mqSemPxScInit (void);
extern int timerScLibInit (void);
extern int clockScLibInit (void);
extern int schedPxScLibInit (void);

/*******************************************************************************
*
* usrPosixScInit - initialize various POSIX system call interfaces
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
*/

void usrPosixScInit (void)
    {
#ifdef INCLUDE_POSIX_CLOCKS
    (void)clockScLibInit();
#endif /* INCLUDE_POSIX_CLOCKS */

#ifdef INCLUDE_POSIX_TIMERS
    (void)timerScLibInit();
#endif /* INCLUDE_POSIX_TIMERS */

#ifdef INCLUDE_POSIX_SCHED
    (void)schedPxScLibInit();
#endif /* INCLUDE_POSIX_SCHED */

#if defined (INCLUDE_POSIX_MQ) || defined (INCLUDE_POSIX_SEM)
    /* install POSIX message queue & semaphore system call handlers */
    mqSemPxScInit();	
#endif /* defined (INCLUDE_POSIX_MQ) || defined (INCLUDE_POSIX_SEM) */ 
    }

#endif /* __INCusrPosixSc */

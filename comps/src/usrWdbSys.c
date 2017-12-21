/* usrWdbSys.c - configuration file for the system mode WDB agent */

/*
 * Copyright (c) 1997-1998, 2004-2008, 2010-2012 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01m,15may12,jmp  CQID: Fix WIND00324029: Use coprocsDiscover() to ignore
                 unsupported coproc options that may be set in WDB_SPAWN_OPTS
                 parameter.
01l,03may11,rlp  Enabled system mode agent for LP64 (WIND00271621).
01k,06apr10,jmp  Added small footprint support (REQ CQ:WIND00190131).
01j,25aug08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
01i,05oct07,rlp  Trying to acquire the kernel lock when entering in system
                 mode (CQ:WIND00105352).
01h,21aug07,dbt  Removed references to intLock()/intUnlock() in SMP
                 (CQ:WIND00101951).
01g,28may07,rlp  Added SMP support.
01f,20apr07,jmg  Added include intLibP.h
01e,19oct06,pes  Fix compiler warnings.
01d,29nov05,dbt  Added system mode breakpoint specific hooks from
                 usrWdbCore.c.
01c,28sep04,dbt  Removed INCLUDE_KERNEL references.
01b,03jun98,dbt	 check if WDB core is initialized before initializing 
		 system mode. 
		 Set mode to system mode only if the task mode agent is not
		 available.
01a,21jul97,ms   taken from 01y of usrWdb.c
*/

/*
DESCRIPTION

This library configures and initializes the system mode WDB agent.
*/

#include <vsbConfig.h>
#include <private/coprocLibP.h>

/* locals */

LOCAL unsigned long wdbExternStackArray [WDB_STACK_SIZE/sizeof(unsigned long)];
#ifndef _WRS_CONFIG_SMP
LOCAL BOOL	prevKernelState;
#else /* !_WRS_CONFIG_SMP */
LOCAL BOOL	isKernelLockOwnedByWdb = FALSE;
#endif /* !_WRS_CONFIG_SMP */

/* forward static declarations */

LOCAL void	wdbRtExternEnterHook	(void);
LOCAL void	wdbRtExternExitHook	(void);

/******************************************************************************
*
* wdbSysModeInit - configure and initialize the system mode WDB agent.
*
*/

STATUS wdbSysModeInit (void)
    {
    caddr_t	pExternStack;

    if (!wdbIsInitialized)
	return (ERROR);

#if	_STACK_DIR == _STACK_GROWS_DOWN
    pExternStack = (caddr_t)&wdbExternStackArray
				[WDB_STACK_SIZE/sizeof(unsigned long)];
    pExternStack = (caddr_t)STACK_ROUND_DOWN (pExternStack);
#else	/* _STACK_DIR == _STACK_GROWS_UP */
    pExternStack = (caddr_t)wdbExternStackArray;
    pExternStack = (caddr_t)STACK_ROUND_UP (pExternStack);
#endif	/* _STACK_DIR == _STACK_GROWS_DOWN */

    if (wdbExternInit (pExternStack) != OK)
	return (ERROR);

    /* Install hooks to be called when external agent is entered/exited */

    if (wdbExternEnterHookAdd (wdbRtExternEnterHook) != OK)
    	return (ERROR);

    if (wdbExternExitHookAdd (wdbRtExternExitHook) != OK)
    	return (ERROR);

    if (!wdbRunsTasking())
	wdbModeSet (WDB_MODE_EXTERN);

    return (OK);
    }

#ifndef	_WRS_CONFIG_PROFILE_SMALL_FOOTPRINT
/******************************************************************************
*
* wdbSp - spawn a task with default params
*/ 

void wdbSp
    (
    int (*func)(),
    _Vx_usr_arg_t arg0,
    _Vx_usr_arg_t arg1,
    _Vx_usr_arg_t arg2,
    _Vx_usr_arg_t arg3,
    _Vx_usr_arg_t arg4
    )
    {
    UINT32 ix;
    UINT32 wdbSpawnOpts = WDB_SPAWN_OPTS;
    UINT32 supportedCoprocs = 0;

#ifdef INCLUDE_COPROCESSOR
    UINT32 descriptorMasks[COPROCS_MAX];

    for (ix = 0; ix < COPROCS_MAX; ++ix)
	descriptorMasks[ix] = 0;

    /* Use coprocsDiscover() to retrieve supported coproc options */

    coprocsDiscover (descriptorMasks);

    for (ix = 0; ix < COPROCS_MAX && descriptorMasks[ix] != 0; ++ix)
	supportedCoprocs |= descriptorMasks[ix];
#endif	/* INCLUDE_COPROCESSOR */

    /*
     * wdbSpawnOpts which is initialized from WDB_SPAWN_OPTS
     * may contains coproc options not supported by current CPU variant.
     * Those unsupported coproc options must be masked out.
     */
    wdbSpawnOpts &= (supportedCoprocs | ~COPROC_FIELD_MASK);

    taskSpawn (NULL, WDB_SPAWN_PRI, wdbSpawnOpts, WDB_SPAWN_STACK_SIZE, func,
			arg0, arg1, arg2, arg3, arg4, 0, 0, 0, 0, 0);
    }
#endif	/* !_WRS_CONFIG_PROFILE_SMALL_FOOTPRINT */

/******************************************************************************
*
* wdbRtExternEnterHook - hook to call when external agent is entered
*
* This routine locks interrupts and uninstalls breakpoints.
*
* RETURNS: N/A
*
* NOMANUAL
*/

LOCAL void wdbRtExternEnterHook (void)
    {
    /*
     * Always fake an interrupt context.
     *
     * SMP: Interrupts are disabled, we don't need to use the "LOCKED"
     * macros to access intCnt.
     */

    _WRS_KERNEL_GLOBAL_ACCESS(intCnt)++;

#ifndef	_WRS_CONFIG_SMP
    /*
     * XXX rlp - We should not need to set kernelState but we keep this
     * temporary to limit the impact of changes.
     */

    prevKernelState = kernelState;
    kernelState = TRUE;		/* always run in kernel state */
#else	/* !_WRS_CONFIG_SMP */
    /*
     * Althrough we fake an interrupt context, we also have to take the
     * kernel lock to differ kernel calls (e.g semGive()).
     */

    isKernelLockOwnedByWdb = KERNEL_LOCK_TRY ();
#endif	/* !_WRS_CONFIG_SMP */

#if     (CPU_FAMILY==I80X86)
    sysIntLock ();
#endif  /* CPU_FAMILY==I80X86 */

    if (wdbBpSysEnterHook != NULL)
	wdbBpSysEnterHook ();
    }

/******************************************************************************
*
* wdbRtExternExitHook - hook to call when the external agent resumes the system
*
* This routine installs again task mode breakpoints. Note that this routine
* is called with interrupts locked.
*
* RETURNS: N/A
*
* NOMANUAL
*/

LOCAL void wdbRtExternExitHook (void)
    {
    /*
     * Restore original intCnt value.
     *
     * SMP: Interrupts are disabled, we don't need to use the "LOCKED"
     * macros to access intCnt.
     */

    _WRS_KERNEL_GLOBAL_ACCESS(intCnt)--;

#ifndef _WRS_CONFIG_SMP
    /*
     * XXX rlp - We should not need to set kernelState but we keep this
     * temporary to limit the impact of changes.
     */

    kernelState = prevKernelState;	/* restore original kernelState value */
#else	/* !_WRS_CONFIG_SMP */
    if (isKernelLockOwnedByWdb)
	KERNEL_LOCK_GIVE ();
#endif	/* !_WRS_CONFIG_SMP */

#if	(CPU_FAMILY==I80X86)
    sysIntUnlock ();
#endif	/* CPU_FAMILY==I80X86 */

    if (wdbBpSysExitHook != NULL)	/* install breakpoints */
	wdbBpSysExitHook ();
    }





/* edrStub.c - ED&R error-injection stub */

/*
 * Copyright (c) 2003-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
02m,05jul13,jmp  Added edrCoreDumpRtpFaultHandle() wrapper to handle core dump
		 generation for RTP fault, and decide which kind of Core Dump
		 to generate (CQID Fix WIND00425385).
02l,15mar13,pad  In deployed mode, force-delete the RTP if the regular RTP
                 deletion failed (side-effect of fix for defect WIND00402989).
02k,16nov10,jpb  Replaced funcBindP.h with excLibP.h
02j,27sep10,rlp  Invoked context's stop API if handlerHook() returns FALSE.
02i,29sep10,gls  made EDR_POLICY_HOOKS a component (WIND00234560)
02h,05may10,rlp  Invoked either edrPolicyHandlerHook() or context's stop API.
02g,08jun09,pcs  Updated to add support for LP64 data model.
02f,27aug08,jpb  Renamed VSB config file.
02e,10jul08,jmp  updated coreDumpRtpGenerate() call to pass rtpId and taskId
                 to RTP core dump generator.
02d,05jul08,jpb  Renamed INCLUDE_EDR_TASK_HOOKS to _WRS_CONFIG_EDR_TASK_HOOKS.
                 Added include path for kernel configurations options set in
                 vsb.  Added ifdefs around edrPolicyHandlerHook calls.
02c,21may08,jmp  added edrCoreDumpRtpGenerate() for RTP core dump generation
                 (REQ CQ:WIND00005075).
02b,20sep07,jmp  Updated edrCoreDumpGenerate() to pass the EDR string to core
                 dump generator (CQ:WIND00104805).
02a,17may07,mcm  Fixing build issue seen with GCC 4.1.2 due to incorrect
                 declaration of rtpName.
01z,20apr07,jmg  Added include for intLibP.h
01y,17jul06,zl   abstract intCnt access via macros.
01x,02may06,lei  added SMP support
01w,09sep05,jmp  updated core dump types.
01v,26aug05,dbt  Completed core dump hooks support.
01u,11aug05,dbt  Include coreDumpLibP.h to get coreDumpGenerate() prototype.
01t,09aug05,dbt  Added handling of FATAL_SYSTEM_CORE_DUMP_ENABLE and
                 KERNEL_APPL_CORE_DUMP_ENABLE parameters.
01s,18jul05,md   scale stubs a bit more
01r,03aug05,jmp  added core dump support.
01q,01nov04,md   update messages for failed rtp/task failures
01p,27oct04,md   fixed warning
01o,27sep04,md   added callout to _func_excPanicHook
01n,23sep04,bpn  Changed shellInOutLib.h to shellInOutLibP.h.
01m,10sep04,md   display injection message for kernel and RTP failures
01l,29jul04,md   always include policies and add shell messages
01k,16jul04,job  Fix for SPR #99384 - call taskSuspend() if taskStop()
		 fails (ie. for VX_UNBREAKABLE tasks)
01j,30jun04,md   fix warning when building bootroms (SPR 98727)
01i,26apr04,elg  Add exception hooks so that WDB Agent can notify exceptions
                 to host.
01h,22apr04,md   added paramter to edrInterruptFatalPolicyHandler
01g,18mar04,md   put tasks in stopped mode instead of suspend mode
01f,12feb04,md   change edrUserFatalPolicyHandler to edrRtpFatalPolicyHandler
01e,26nov03,dbs  correct the name of the debug-flag-get routine
01d,20nov03,dbs  fully implement EDR policy handling
01c,11nov03,dbs  update policy handlers
01b,25sep03,dbs  implement ED&R system policies
01a,10sep03,dbs  written
*/

/*
DESCRIPTION

This module is an 'always included' component that provides a
forwarding stub for ED&R error injection. It provides a single
function, edrErrorInjectStub(), which is conditionally compiled to
either forward all calls to the real implementation of
edrErrorInject(), if INCLUDE_EDR_ERRLOG is defined, or otherwise to
simply return OK.

This allows other components and subsystems to be written as though
ED&R were always present, and so they can inject errors at any time,
even in the early boot sequence.

This is a preferable solution to the alternative of having a FUNCPTR
to edrErrorInject(), since errors occuring prior to the initialization
of the funcptr would be missed, and this would defeat some of the
purpose of ED&R.
*/

#include <vsbConfig.h>

#include <taskLib.h>
#include <intLib.h>
#include <rtpLib.h>
#include <rebootLib.h>
#include <sysLib.h>
#include <wait.h>
#include <logLib.h>
#include <coreDumpLib.h>
#include <private/edrLibP.h>
#include <private/coreDumpLibP.h>
#include <private/shellInOutLibP.h>
#include <private/taskLibP.h>	/* for taskStopForce()	  */
#include <private/excLibP.h>	/* for _func_excPanicHook */
#include <private/intLibP.h>	/* intCnt */
/* globals */

#ifdef	INCLUDE_EDR_ERRLOG
BOOL edrLibInstalled = FALSE;
#endif

/* locals */

#ifdef	INCLUDE_EDR_POLICY_HOOKS
static EDR_POLICY_HANDLER_HOOK	edrPolicyHandlerHook = NULL;
#endif	/* INCLUDE_EDR_POLICY_HOOKS */

#ifdef  INCLUDE_EDR_SYSDBG_FLAG
# define IS_LAB_FLAG_SET() (edrFlagsGet() & EDR_FLAG_DEBUG)
#else
# define IS_LAB_FLAG_SET() FALSE
#endif

/* define the shell notification command if the logging system is present */

#ifdef INCLUDE_LOGGING
# define SHELL_MSG(msg, id)						   \
		logMsg ((char *)(msg), (_Vx_usr_arg_t)(id), 0, 0, 0, 0, 0)
# define SHELL_SIG_MSG(msg, id, sig)					   \
	      logMsg ((char *)(msg), (_Vx_usr_arg_t)(id), (int)sig, 0, 0, 0, 0)

const char *taskStopMsg =
		"task 0x%lx has had a failure and has been stopped.\n";

const char *rtpDeleteMsg =
		"RTP 0x%lx has had a failure and has been deleted.\n";
const char *rtpSigDeleteMsg =
		"RTP 0x%lx has been deleted due to signal %d.\n";
const char *rtpStopMsg =
		"RTP 0x%lx has had a failure and has been stopped.\n";
const char *rtpSigStopMsg =
		"RTP 0x%lx has been stopped due to signal %d.\n";
#else
# define SHELL_MSG(msg, id)
# define SHELL_SIG_MSG(msg, id, sig)
#endif

/* define the shell printing function if the shell is included */

#ifdef	INCLUDE_SHELL
# define SHELL_PRINT(x)	shellPrint x
#else
# define SHELL_PRINT(x)
#endif /* INCLUDE_SHELL */

/* forward static declarations */

#ifdef	INCLUDE_CORE_DUMP
LOCAL void edrCoreDumpGenerate (void * info, CORE_DUMP_TYPE coreDumpType);
#endif	/* INCLUDE_CORE_DUMP */

#ifdef	INCLUDE_CORE_DUMP_RTP
LOCAL void edrCoreDumpRtpGenerate (void * info);
#endif	/* INCLUDE_CORE_DUMP_RTP */

#if defined(INCLUDE_CORE_DUMP) || defined(INCLUDE_CORE_DUMP_RTP)
LOCAL void edrCoreDumpRtpFaultHandle (void * info);
#endif /* INCLUDE_CORE_DUMP || INCLUDE_CORE_DUMP_RTP */

/**************************************************************************
*
* edrStubInit - inserts a BOOT record
*
* NOMANUAL
*/

void edrStubInit (void)
    {
#ifdef	INCLUDE_EDR_ERRLOG
    extern int sysStartType;
    const char *msg;

    /* set appropriate system boot message */

    switch (sysStartType & 
	    (BOOT_NORMAL | BOOT_NO_AUTOBOOT | BOOT_CLEAR | BOOT_QUICK_AUTOBOOT))
	{
	case BOOT_NORMAL:
	    msg = "System Booted - normal boot";
	    break;
	case BOOT_NO_AUTOBOOT:
	    msg = "System Booted - no autoboot";
	    break;
	case BOOT_QUICK_AUTOBOOT:
	    msg = "System Booted - quick autoboot";
	    break;
	case BOOT_CLEAR:
	    msg = "System Booted - cold boot";
	    break;
	case BOOT_CLEAR | BOOT_NO_AUTOBOOT:
	    msg = "System Booted - cold no autoboot";
	    break;
	case BOOT_CLEAR | BOOT_QUICK_AUTOBOOT:
	    msg = "System Booted - cold quick autoboot";
	    break;
	default:
	    msg = "System Booted";
	    break;
	}

    EDR_BOOT_INJECT (msg);
#endif  /* INCLUDE_EDR_ERRLOG */
    }

/**************************************************************************
*
* edrErrorInjectStub - ED&R stub/forwarding function
*
* This function is linked into all error-injection points, so that
* error injection will work at any time once the image is booted
* (using a FUNCPTR would mean that errors occuring before the funcptr
* was initialized would be missed).  However, to allow ED&R to be
* compiled out if necessary, it's body is conditionally compiled to
* call the real edrErrorInject() only if the error-log component has
* been included in the system build.
*
* NOMANUAL
*/

STATUS edrErrorInjectStub
    (
    int			kind,		/* severity | facility		*/
    const char *	fileName,	/* name of source file		*/
    int			lineNumber,	/* line number of source code	*/
    const REG_SET *	pRegSet,	/* current register values	*/
    const EXC_INFO *	pExcInfo,	/* CPU-specific exception info	*/
    void*		addr,		/* faulting address (e.g. PC)	*/
    const char *	textPayload	/* additional text string	*/
    )
    {
#ifdef INCLUDE_EDR_ERRLOG
    return edrErrorInject (kind,
			   fileName,
			   lineNumber,
			   pRegSet,
			   pExcInfo,
			   addr,
			   textPayload);
#else /* INCLUDE_EDR_ERRLOG */
    return OK;
#endif /* INCLUDE_EDR_ERRLOG */
    }

/**************************************************************************
*
* edrKernelFatalPolicyHandler - handles fatal errors in kernel tasks
*
* This function is invoked by the exception handler, when kernel
* task-level code takes an exception. Its default policy is to suspend
* the task (in 'deployed' mode), but if the ED&R 'system debug flag'
* is set, in which case the system is considered to be in 'lab' mode,
* it also suspends the task.
*
* RETURNS: n/a
*/

void edrKernelFatalPolicyHandler
    (
    EDR_TASK_INFO *info		/* optional information for kernel fault */
    )
    {
#ifdef	INCLUDE_EDR_SYSDBG_FLAG
    if (IS_LAB_FLAG_SET())
	{
	/* In 'lab' mode, stop the task. */

	if ((info != NULL) && (info->taskId != 0))
	    {
	    SHELL_MSG (taskStopMsg, info->taskId);

	    /* display any text provided by the inject call */

	    if (info->msg != NULL)
		SHELL_MSG ("%s\n", info->msg);
	    
#ifdef	INCLUDE_CORE_DUMP
	    /* call core dump generator */

	    edrCoreDumpGenerate ((void *) info, CORE_DUMP_KERNEL_TASK);
#endif  /* INCLUDE_CORE_DUMP */

	    /* policy handler being called for a specific task */

#ifdef INCLUDE_EDR_POLICY_HOOKS
    	    if ((edrPolicyHandlerHook == NULL) ||
	    	((edrPolicyHandlerHook != NULL) &&
		 (!edrPolicyHandlerHook (EDR_FACILITY_KERNEL, info, FALSE))))
#endif /* INCLUDE_EDR_POLICY_HOOKS */
		taskStopForce(info->taskId);
	    }
	else
	    {
	    SHELL_MSG (taskStopMsg, taskIdCurrent);

	    /* display any text provided by the inject call */

	    if (info != NULL && info->msg != NULL)
		SHELL_MSG ("%s\n", info->msg);

#ifdef	INCLUDE_CORE_DUMP
	    /* call core dump generator */

	    edrCoreDumpGenerate ((void *) info, CORE_DUMP_KERNEL_TASK);
#endif  /* INCLUDE_CORE_DUMP */

	    /* policy handler being called by the current task */

#ifdef INCLUDE_EDR_POLICY_HOOKS
    	    if ((edrPolicyHandlerHook == NULL) ||
	    	((edrPolicyHandlerHook != NULL) &&
		 (!edrPolicyHandlerHook (EDR_FACILITY_KERNEL, info, FALSE))))
#endif /* INCLUDE_EDR_POLICY_HOOKS */
		taskStopForce(0);

	    taskIdCurrent->pExcRegSet = (REG_SET *) NULL;
	    }
	}
    else
#endif /* INCLUDE_EDR_SYSDBG_FLAG */
	{
	/* In 'deployed' mode, stop the task. */

	if ((info != NULL) && (info->taskId != 0))
	    {
	    SHELL_MSG (taskStopMsg, info->taskId);

	    /* display any text provided by the inject call */

	    if (info->msg != NULL)
		SHELL_MSG ("%s\n", info->msg);

#ifdef	INCLUDE_CORE_DUMP
	    /* call core dump generator */

	    edrCoreDumpGenerate ((void *) info, CORE_DUMP_KERNEL_TASK);
#endif  /* INCLUDE_CORE_DUMP */

	    /* policy handler being called for a specific task */

#ifdef	INCLUDE_EDR_POLICY_HOOKS
    	    if ((edrPolicyHandlerHook == NULL) ||
	    	((edrPolicyHandlerHook != NULL) &&
		 (!edrPolicyHandlerHook (EDR_FACILITY_KERNEL, info, FALSE))))
#endif	/* INCLUDE_EDR_POLICY_HOOKS */
		taskStopForce(info->taskId);
	    }
	else
	    {
	    SHELL_MSG (taskStopMsg, taskIdCurrent);

	    /* display any text provided by the inject call */

	    if (info != NULL && info->msg != NULL)
		SHELL_MSG ("%s\n", info->msg);

#ifdef	INCLUDE_CORE_DUMP
	    /* call core dump generator */

	    edrCoreDumpGenerate ((void *) info, CORE_DUMP_KERNEL_TASK);
#endif  /* INCLUDE_CORE_DUMP */

	    /* policy handler being called by the current task */

#ifdef	INCLUDE_EDR_POLICY_HOOKS
    	    if ((edrPolicyHandlerHook == NULL) ||
	    	((edrPolicyHandlerHook != NULL) &&
		 (!edrPolicyHandlerHook (EDR_FACILITY_KERNEL, info, FALSE))))
#endif	/* INCLUDE_EDR_POLICY_HOOKS */
		taskStopForce(0);

	    taskIdCurrent->pExcRegSet = (REG_SET *) NULL;
	    }
	}
    }

/**************************************************************************
*
* edrRtpFatalPolicyHandler - handles fatal errors in User-mode (RTP) code
*
* This function is called by the exception handler, when user-level (RTP)
* code takes an exception, or when an RTP FATAL ED&R message is injected
* from the RTP (via the _edrErrorInject system call). Its default policy is
* to destroy the RTP in 'deployed' mode, but if the ED&R 'system debug flag'
* is set, in which case the system is considered to be in 'lab' mode,
* it suspends the RTP.
*
* It can be over-ridden to implement any other policy, as required.
*
* RETURNS: n/a
*/

#ifdef INCLUDE_RTP
 #ifdef INCLUDE_SHELL
static char *rtpName(RTP_ID rtpId);
 #endif
#endif

void edrRtpFatalPolicyHandler
    (
    EDR_RTP_INFO *info		/* optional information for RTP fault */
    )
    {
    /* no need to do anything if RTP's are not included */

#ifdef INCLUDE_RTP

#ifdef	INCLUDE_EDR_SYSDBG_FLAG
    if (IS_LAB_FLAG_SET())
	{
	/* In 'lab' mode, suspend the RTP */

	extern STATUS rtpTaskAllStop(RTP_ID);

	/* stop all tasks within the RTP */

	if ((info != NULL) && (info->rtpId != NULL))
	    {
	    /* check if being called due to a signal delivery */

	    if (WIFSIGNALED(info->status))
		SHELL_SIG_MSG (rtpSigStopMsg, info->rtpId,
			       WTERMSIG(info->status));
	    else
		SHELL_MSG (rtpStopMsg, info->rtpId);

#if defined(INCLUDE_CORE_DUMP) || defined(INCLUDE_CORE_DUMP_RTP)
	    /* call core dump generator */

	    edrCoreDumpRtpFaultHandle ((void *) info);
#endif /* INCLUDE_CORE_DUMP || INCLUDE_CORE_DUMP_RTP */

	    /* policy handler being called for a specific RTP */

#ifdef INCLUDE_EDR_POLICY_HOOKS
    	    if ((edrPolicyHandlerHook == NULL) ||
	    	((edrPolicyHandlerHook != NULL) &&
		 (!edrPolicyHandlerHook (EDR_FACILITY_RTP, info, FALSE))))
#endif /* INCLUDE_EDR_POLICY_HOOKS */
		rtpTaskAllStop (info->rtpId);
	    }
	else
	    {
	    /* display any text provided by the inject syscall */

	    if (info != NULL && info->msg != NULL)
		SHELL_PRINT ((ALL_SHELL_SESSIONS, "%s: %s\n", rtpName(0),
			      info->msg));

	    SHELL_MSG (rtpStopMsg, taskIdCurrent->rtpId);

#if defined(INCLUDE_CORE_DUMP) || defined(INCLUDE_CORE_DUMP_RTP)
	    /* call core dump generator */

	    edrCoreDumpRtpFaultHandle ((void *) info);
#endif /* INCLUDE_CORE_DUMP || INCLUDE_CORE_DUMP_RTP */

	    /* policy handler being called by the current RTP */

#ifdef INCLUDE_EDR_POLICY_HOOKS
    	    if ((edrPolicyHandlerHook == NULL) ||
	    	((edrPolicyHandlerHook != NULL) &&
		 (!edrPolicyHandlerHook (EDR_FACILITY_RTP, info, FALSE))))
#endif /* INCLUDE_EDR_POLICY_HOOKS */
		rtpTaskAllStop (taskIdCurrent->rtpId);
	    }
	}
    else
#endif /* INCLUDE_EDR_SYSDBG_FLAG */
	{
	/* In 'deployed' mode, kill the RTP */

	if ((info != NULL) && (info->rtpId != NULL))
	    {
	    /* policy handler being called for a specific RTP */

#ifdef	INCLUDE_EDR_POLICY_HOOKS
	    if (edrPolicyHandlerHook != NULL)
	    	edrPolicyHandlerHook (EDR_FACILITY_RTP, info, FALSE);
#endif /* INCLUDE_EDR_POLICY_HOOKS */

	    /* check if being called due to a signal delivery */

	    if (WIFSIGNALED(info->status))
		SHELL_SIG_MSG (rtpSigDeleteMsg, info->rtpId,
			       WTERMSIG(info->status));
	    else
		SHELL_MSG (rtpDeleteMsg, info->rtpId);

#if defined(INCLUDE_CORE_DUMP) || defined(INCLUDE_CORE_DUMP_RTP)
	    /* call core dump generator */

	    edrCoreDumpRtpFaultHandle ((void *) info);
#endif	/* INCLUDE_CORE_DUMP || INCLUDE_CORE_DUMP_RTP */

            /*
             * If the RTP that triggered the exception holds a task that
             * happens to be delete-safe, and Core Dump support for RTP has
             * stopped all the RTP's tasks, the RTP deletion will not be
             * allowed. So we force-delete the RTP if ever rtpDelete() returns
             * an error.
             */

	    if (rtpDelete (info->rtpId, info->options, info->status) == ERROR)
                (void)rtpDeleteForce (info->rtpId);
	    }
	else
	    {
	    /* policy handler being called by the current RTP */

#ifdef	INCLUDE_EDR_POLICY_HOOKS
	    if (edrPolicyHandlerHook != NULL)
	    	edrPolicyHandlerHook (EDR_FACILITY_RTP, info, FALSE);
#endif	/* INCLUDE_EDR_POLICY_HOOKS */

	    /* display any text provided by the inject syscall */

	    if (info != NULL && info->msg != NULL)
		SHELL_PRINT ((ALL_SHELL_SESSIONS, "%s: %s\n", rtpName(0),
			     info->msg));

	    SHELL_MSG (rtpDeleteMsg, taskIdCurrent->rtpId);

#if defined(INCLUDE_CORE_DUMP) || defined(INCLUDE_CORE_DUMP_RTP)
	    /* call core dump generator */

	    edrCoreDumpRtpFaultHandle ((void *) info);
#endif /* INCLUDE_CORE_DUMP || INCLUDE_CORE_DUMP_RTP */

            /*
             * If the RTP that triggered the exception holds a task that
             * happens to be delete-safe, and Core Dump support for RTP has
             * stopped all the RTP's tasks, the RTP deletion will not be
             * allowed. So we force-delete the RTP if ever rtpDelete() returns
             * an error.
             */

	    if (rtpDelete (taskIdCurrent->rtpId, 0,
                           SET_TERMSIG(SIGKILL)) == ERROR)
                (void)rtpDeleteForce (info->rtpId);
	    }
	}
#endif /* INCLUDE_RTP */
    }

/**************************************************************************
*
* edrInitFatalPolicyHandler - handles fatal errors in init phase
*
* It can be over-ridden to implement any other policy, as required.
*
* RETURNS: n/a
*/

void edrInitFatalPolicyHandler
    (
    EDR_INIT_INFO *info		/* optional information on init fault */
    )
    {
    /* call any policy hooks which have been installed */

#ifdef	INCLUDE_EDR_POLICY_HOOKS
    if (edrPolicyHandlerHook != NULL)
	edrPolicyHandlerHook (EDR_FACILITY_INIT, info, 
			      IS_LAB_FLAG_SET() ? TRUE : FALSE);
#endif /* INCLUDE_EDR_POLICY_HOOKS */

    /* call any panic hooks which have been installed */

    if ((_func_excPanicHook != NULL) && (info != NULL))
	{
	/* increment intCnt so printExc() will put the message in sysExcMsg */

	_WRS_KERNEL_GLOBAL_ACCESS (intCnt)++;

	(*_func_excPanicHook) (info->vector, info->pEsf, info->pRegs,
			       info->pExcInfo);

	_WRS_KERNEL_GLOBAL_ACCESS (intCnt)--;
	}

#ifdef	INCLUDE_CORE_DUMP
    /* call core dump generator */

    edrCoreDumpGenerate ((void *) info, CORE_DUMP_KERNEL_INIT);
#endif  /* INCLUDE_CORE_DUMP */

#ifdef  INCLUDE_EDR_SYSDBG_FLAG
    if (IS_LAB_FLAG_SET())
	{
	/* In 'lab' mode, reboot the system. */

	reboot (BOOT_NO_AUTOBOOT);
	}
    else
#endif  /* INCLUDE_EDR_SYSDBG_FLAG */
	{
	/* In 'deployed' mode, reboot the system. */

	reboot (BOOT_NO_AUTOBOOT);
	}
    }

/**************************************************************************
*
* edrInterruptFatalPolicyHandler - handles fatal errors in interrupts
*
* It can be over-ridden to implement any other policy, as required.
*
* RETURNS: n/a
*/

void edrInterruptFatalPolicyHandler
    (
    EDR_INTERRUPT_INFO *info	/* optional information for interrupt fault */
    )
    {

    /* call any policy hooks which have been installed */

#ifdef	INCLUDE_EDR_POLICY_HOOKS
    if (edrPolicyHandlerHook != NULL)
	edrPolicyHandlerHook (EDR_FACILITY_INTERRUPT, info,
			      IS_LAB_FLAG_SET() ? TRUE : FALSE);
#endif /* INCLUDE_EDR_POLICY_HOOKS */

    /* call any panic hooks which have been installed */

    if ((_func_excPanicHook != NULL) && (info != NULL))
	(*_func_excPanicHook) (info->vector, info->pEsf, info->pRegs,
			       info->pExcInfo);

#ifdef	INCLUDE_CORE_DUMP
    /* call core dump generator */

    edrCoreDumpGenerate ((void *) info, CORE_DUMP_KERNEL);
#endif  /* INCLUDE_CORE_DUMP */

#ifdef	INCLUDE_EDR_SYSDBG_FLAG
    if (IS_LAB_FLAG_SET())
	{
	/* In 'lab' mode, reboot the system. */

	reboot (BOOT_NORMAL);
	}
    else
#endif  /* INCLUDE_EDR_SYSDBG_FLAG */
	{
	/* In 'deployed' mode, reboot the system. */

	reboot (BOOT_NORMAL);
	}
    }

#ifdef	INCLUDE_EDR_POLICY_HOOKS
/******************************************************************************
*
* edrPolicyHandlerHookAdd - add a hook in to the EDR policy handlers
*
* This routine adds a hook to be called from the EDR policy handlers.
*
* RETURNS: OK if there is no existing hook, and the hook is added
* correctly, or if the existing hook is the same as the one
* supplied; ERROR if there is already an existing, but different, hook
* in place
*
* SEE ALSO: edrErrorPolicyHookRemove, edrPolicyHandlerHookGet
*/

STATUS edrPolicyHandlerHookAdd
    (
    EDR_POLICY_HANDLER_HOOK	hook		/* hook address */
    )
    {
    if ((edrPolicyHandlerHook != NULL) && (edrPolicyHandlerHook != hook))
	return (ERROR);

    edrPolicyHandlerHook = hook;

    return (OK);
    }

/**************************************************************************
*
* edrErrorPolicyHookRemove - removes the existing policy handler hook
*
* This function removes a hook which was added using edrPolicyHandlerHookAdd().
*
* RETURNS: OK if there was an existing hook, ERROR if the hook was
* already NULL
*
* SEE ALSO: edrPolicyHandlerHookAdd, edrPolicyHandlerHookGet
*/

STATUS edrErrorPolicyHookRemove (void)
    {
    if (edrPolicyHandlerHook == NULL)
	return (ERROR);

    edrPolicyHandlerHook = NULL;

    return (OK);
    }
    
/**************************************************************************
*
* edrPolicyHandlerHookGet - returns the existing error-injection hook
*
* This function returns a pointer to the hook function which was added using
* edrPolicyHandlerHookAdd().  If no hook has been added, the value returned
* is NULL.
*
* RETURNS: the hook function pointer, which may be NULL
*
* SEE ALSO: edrPolicyHandlerHookAdd, edrErrorPolicyHookRemove
*/

EDR_POLICY_HANDLER_HOOK edrPolicyHandlerHookGet (void)
    {
    return (edrPolicyHandlerHook);
    }

#endif /* INCLUDE_EDR_POLICY_HOOKS */

#if defined (INCLUDE_RTP) && defined (INCLUDE_SHELL)

/**************************************************************************
*
* rtpName - returns a string denoting the name of an RTP
*
* This function returns a string representing the base name (name of RTP file
* without the path) of the specified RTP.  
*
* RETURNS: a string representing the name of the RTP
*
* NOMANUAL
*/

static char *rtpName
    (
    RTP_ID rtpId	/* RTP id to examine */
    )
    {
    char  *p;
    int    i;

    /* map NULL to current RTP id */

    if (rtpId == NULL)
	rtpId = taskIdCurrent->rtpId;

    /* traverse pathname from reverse stopping at first "/" */

    i = strlen(rtpId->pPathName);
    p = rtpId->pPathName + i;

    for (;(*p != '/') && (i > 0); p--,i--);

    if (*p == '/')
	p++;

    return (p);
    }

#endif /* INCLUDE_RTP && INCLUDE_SHELL */

#ifdef	INCLUDE_CORE_DUMP
/**************************************************************************
*
* edrCoreDumpGenerate - call core dump generation routine
*
* This routine calls core dump generation routine when necessary (according
* to core dump configuration). It extracts necessary information from
* ED&R structure given in parameter.
*
* RETURNS: N/A
*
* SEE ALSO: coreDumpLib
*/

LOCAL void edrCoreDumpGenerate
    (
    void *		info,		/* optional information for fault */
    CORE_DUMP_TYPE	coreDumpType	/* coreDump type */
    )
    {
    int		vector = 0;		/* exception vector number */
    char *	pEsf = NULL;		/* exception frame pointer */
    REG_SET *	pRegs = NULL;		/* exception register set */
    EXC_INFO *	pExcInfo = NULL;	/* exception information */
    BOOL	isException = FALSE;	/* are we handling an exception? */
    char *	pEdrString = NULL;	/* ptr to ED&R record optional msg */

#if	(KERNEL_APPL_CORE_DUMP_ENABLE == FALSE)
    if (coreDumpType == CORE_DUMP_KERNEL_TASK)
	{
	/* Don't generate core dump for kernel task level exceptions */

	return;
	}
#endif	/* KERNEL_APPL_CORE_DUMP_ENABLE == FALSE */

#if	(FATAL_SYSTEM_CORE_DUMP_ENABLE == FALSE)
    if (coreDumpType != CORE_DUMP_KERNEL_TASK)
	{
	/* Don't generate core dump for fatal kernel errors */

	return;
	}
#endif	/* FATAL_SYSTEM_CORE_DUMP_ENABLE == FALSE */

    if (info != NULL)
    	{
	switch (coreDumpType)
	    {
#if	(KERNEL_APPL_CORE_DUMP_ENABLE == TRUE)
	    case CORE_DUMP_KERNEL_TASK:
		vector		= ((EDR_TASK_INFO *)info)->vector;
		pEsf		= ((EDR_TASK_INFO *)info)->pEsf;
		pRegs		= ((EDR_TASK_INFO *)info)->pRegs;
		pExcInfo	= ((EDR_TASK_INFO *)info)->pExcInfo;
		isException	= ((EDR_TASK_INFO *)info)->isException;
		pEdrString	= (char *)((EDR_TASK_INFO *)info)->msg;
		break;
#endif	/* KERNEL_APPL_CORE_DUMP_ENABLE == TRUE */

#if	(FATAL_SYSTEM_CORE_DUMP_ENABLE == TRUE)
	    case CORE_DUMP_KERNEL_INIT:
		vector		= ((EDR_INIT_INFO *)info)->vector;
		pEsf		= ((EDR_INIT_INFO *)info)->pEsf;
		pRegs		= ((EDR_INIT_INFO *)info)->pRegs;
		pExcInfo	= ((EDR_INIT_INFO *)info)->pExcInfo;
		isException	= ((EDR_INIT_INFO *)info)->isException;
		pEdrString	= (char *)((EDR_INIT_INFO *)info)->msg;
		break;

	    case CORE_DUMP_KERNEL:
		vector		= ((EDR_INTERRUPT_INFO *)info)->vector;
		pEsf		= ((EDR_INTERRUPT_INFO *)info)->pEsf;
		pRegs		= ((EDR_INTERRUPT_INFO *)info)->pRegs;
		pExcInfo	= ((EDR_INTERRUPT_INFO *)info)->pExcInfo;
		isException	= ((EDR_INTERRUPT_INFO *)info)->isException;
		pEdrString	= (char *)((EDR_INTERRUPT_INFO *)info)->msg;
		break;
#endif	/* FATAL_SYSTEM_CORE_DUMP_ENABLE == TRUE */

	    default:
	    	/* Keep generating core dump but do not fill information */

	    	break;
	    }
	}

    coreDumpGenerate (vector, pEsf, pRegs, pExcInfo, coreDumpType,
		      pEdrString, isException);
    }
#endif	/* INCLUDE_CORE_DUMP */

#if defined(INCLUDE_CORE_DUMP) || defined(INCLUDE_CORE_DUMP_RTP)
/*******************************************************************************
*
* edrCoreDumpRtpFaultHandle - handle core dump generation for RTP fault
*
* This routine handles core dump generation for RTP faults.
* If we are in supervisor mode, then we generate a Kernel Core Dump.
* Otherwise, we generate an RTP Core Dump.
*
* RETURNS: N/A
*/

LOCAL void edrCoreDumpRtpFaultHandle
    (
    void *	info		/* optional information for fault */
    )
    {
    BOOL supervisorMode = FALSE;	/* User or supervisor mode ? */

    if ((info != NULL) && (((EDR_RTP_INFO *)info)->pExcInfo != NULL))
	{
	/* Retrieve excInfo & pRegs: Required by _WRS_IS_SUPV_EXC() macro */
	EXC_INFO excInfo = *((EDR_RTP_INFO *)info)->pExcInfo;
	REG_SET * pRegs = ((EDR_RTP_INFO *)info)->pRegs;

	supervisorMode = _WRS_IS_SUPV_EXC();
	}

    /* User or supervisor mode ? */

    if (supervisorMode)
	{
	/*
	 * Supervisor mode: Generate a Kernel Core Dump if support is included.
	 */

#ifdef INCLUDE_CORE_DUMP
	EDR_TASK_INFO kernelInfo;

	if (info != NULL)
	    {
	    kernelInfo.vector		= ((EDR_RTP_INFO *)info)->vector;
	    kernelInfo.pEsf		= ((EDR_RTP_INFO *)info)->pEsf;
	    kernelInfo.pRegs		= ((EDR_RTP_INFO *)info)->pRegs;
	    kernelInfo.pExcInfo		= ((EDR_RTP_INFO *)info)->pExcInfo;
	    kernelInfo.isException	= ((EDR_RTP_INFO *)info)->isException;
	    kernelInfo.msg		= (char *)((EDR_RTP_INFO *)info)->msg;

	    edrCoreDumpGenerate ((void *)&kernelInfo, CORE_DUMP_KERNEL_TASK);
	    }
	else
	    edrCoreDumpGenerate ((void *)NULL, CORE_DUMP_KERNEL_TASK);
#endif
	}
#ifdef	INCLUDE_CORE_DUMP_RTP
    else
	{
	/*
	 * User Mode: Generate an RTP Core Dump.
	 */

	edrCoreDumpRtpGenerate (info);
	}
#endif	/* INCLUDE_CORE_DUMP_RTP */
    }
#endif	/* INCLUDE_CORE_DUMP || INCLUDE_CORE_DUMP_RTP */

#ifdef	INCLUDE_CORE_DUMP_RTP
/**************************************************************************
*
* edrCoreDumpRtpGenerate - call RTP core dump generation routine
*
* This routine calls RTP core dump generation routine when necessary. It
* extracts necessary information from ED&R structure given in parameter.
*
* RETURNS: N/A
*
* SEE ALSO: coreDumpLib
*/

LOCAL void edrCoreDumpRtpGenerate
    (
    void *	info		/* optional information for fault */
    )
    {
    RTP_ID	rtpId = 0;		/* RTP's id */
    TASK_ID	taskId = 0;		/* task id within RTP */
    int		vector = 0;		/* exception vector number */
    char *	pEsf = NULL;		/* exception frame pointer */
    REG_SET *	pRegs = NULL;		/* exception register set */
    EXC_INFO *	pExcInfo = NULL;	/* exception information */
    BOOL	isException = FALSE;	/* are we handling an exception? */
    char *	pEdrString = NULL;	/* ptr to ED&R record optional msg */

    if (info != NULL)
    	{
	rtpId		= ((EDR_RTP_INFO *)info)->rtpId;
	taskId		= ((EDR_RTP_INFO *)info)->taskId;
	vector		= ((EDR_RTP_INFO *)info)->vector;
	pEsf		= ((EDR_RTP_INFO *)info)->pEsf;
	pRegs		= ((EDR_RTP_INFO *)info)->pRegs;
	pExcInfo	= ((EDR_RTP_INFO *)info)->pExcInfo;
	isException	= ((EDR_RTP_INFO *)info)->isException;
	pEdrString	= (char *)((EDR_RTP_INFO *)info)->msg;
	}

    coreDumpRtpGenerate (rtpId, taskId, vector, pEsf, pRegs, pExcInfo,
			 CORE_DUMP_RTP, pEdrString, isException);
    }
#endif	/* INCLUDE_CORE_DUMP_RTP */

/* usrShell.c - shell module */

/*
 * Copyright (c) 2003-2005, 2009-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01m,18mar11,jmp  renamed SHELL_SP_* parameters to SHELL_SPAWNED_TASK_*.
01l,16mar11,jmp  Added support for SHELL_SP_STACK_SIZE, SHELL_SP_PRIORITY,
		 SHELL_SP_OPTIONS parameters (CQ:WIND00235451).
01k,09jul10,v_r  Added warning when SHELL_SECURE is set without
                 INCLUDE_SECURITY (req CQ:WIND00190686).
01j,08sep09,v_r  Added shell session limitation system (def CQ:WIND00155318,
                 merged from 6.5 sustaining).
01i,16jun09,jmp  Completed 64-bit support.
01h,06mar09,v_r  LP64 early mods: fixed some build warnings.
		 Commented out shellRemoteConfigStr as it is not included in the
		 64 bits build yet.
01g,06sep05,bpn  Added new component parameters to configure the shell
                 tasks spawning options (SPR#106622).
01f,04jun04,bpn  Fixed SPR#97937: the compatible mode was broken. Fixed
                 also the interaction of the compatible mode with
                 the shell security.
01e,26apr04,bpn  For secure shell on the console, spawn a task to launch the
                 shell, and so do not prevent initialization of other
                 components.
01d,13nov03,bpn  Use configuration variables.
01c,13oct03,bpn  Removed build warning.
01b,31mar03,bpn  Changed the initialisation of the configlet. Added unique
                 shell session compatibility.
01a,14feb03,bpn  Written.
*/

/*
DESCRIPTION
This file is used to intialize the VxWorks kernel shell component.
This file is included by usrConfig.c.

NOMANUAL
*/

/* Includes */

#include <private/shellLibP.h>
#include <private/usrLibP.h>

/* Local declarations */

#if (SHELL_START_AT_BOOT == TRUE) && (SHELL_SECURE == TRUE)
static int usrShellConsoleLogin (int fdIn, int fdOut, int fdErr);
#endif
static void usrShellConfigure (void);

/* 
 * Handle incompatible components. We cannot break the message lines or that
 * break will be reproduced on the warning line. Using a #define for the message
 * is not possible either as it does not work through file inclusion (as does
 * the project facility).
 */

#if (SHELL_COMPATIBLE == TRUE) && (SHELL_MAX_SESSIONS != -1)
    #warning "Compatible mode detected. Shell session limit system disabled."
    #undef SHELL_MAX_SESSIONS
    #define SHELL_MAX_SESSIONS -1
#endif /* (SHELL_COMPATIBLE == TRUE) && (SHELL_MAX_SESSIONS != -1) */


#if (SHELL_COMPATIBLE == TRUE) && (SHELL_SECURE == TRUE)
    #warning "Compatible mode detected. Secure console shell disabled."
    #undef SHELL_SECURE
    #define SHELL_SECURE FALSE
#endif /* (SHELL_COMPATIBLE == TRUE) && (SHELL_SECURE == TRUE) */

#if (SHELL_COMPATIBLE == TRUE) && (SHELL_START_AT_BOOT == FALSE)
    #warning "Compatible mode detected. Forcing shell startup to boot time."
    #undef SHELL_START_AT_BOOT
    #define SHELL_START_AT_BOOT TRUE
#endif /* (SHELL_COMPATIBLE == TRUE) && (SHELL_START_AT_BOOT == FALSE) */

/* SHELL_SECURE mandates INCLUDE_SECURITY */

#if (SHELL_SECURE == TRUE) && !defined(INCLUDE_SECURITY)
    #warning "Enabling SHELL_SECURE without INCLUDE_SECURITY has no effect!"
#endif /* (SHELL_SECURE == TRUE) && !define(INCLUDE_SECURITY) */

/* 
 * Handle backward compatibility for the SHELL_MAX_SESSIONS parameter. This
 * avoids build breakage for existing projects which did not have the parameter
 * defined.
 */

#ifndef SHELL_MAX_SESSIONS
#define SHELL_MAX_SESSIONS  -1
#endif

/******************************************************************************
*
* usrShell - initialize the kernel shell
*
* This routine launches an initial shell task attached to the console if 
* SHELL_START_AT_BOOT is defined to TRUE. If SHELL_COMPATIBLE is defined to
* TRUE, the shell session is launched with a VxWorks 5.5 compatible 
* behavior.
*
* \NOMANUAL
*/

void usrShell ()
    {
#if SHELL_START_AT_BOOT == TRUE
    int		    fdIn;
    int		    fdOut;
    int		    fdErr;
#if SHELL_SECURE == TRUE
    TASK_ID	    taskId;
#endif
#endif

    usrShellConfigure ();

#if SHELL_START_AT_BOOT == TRUE
#if SHELL_COMPATIBLE == TRUE
    /* 
     * XXX bpn - Using the standard I/O (STD_IN, STD_OUT, STD_ERR), this
     * "attaches" the target shell to the console. Any changes of the 
     * global standard I/O modified also the I/O of the shell task.
     */

    fdIn = STD_IN;
    fdOut = STD_OUT;
    fdErr = STD_ERR;
#else
    /* 
     * XXX bpn - Using the file descriptor behind the standard I/O, this
     * "detach" the target shell on the console from changes of the 
     * global standard I/O.
     */

    
    fdIn = ioGlobalStdGet (STD_IN);
    fdOut = ioGlobalStdGet (STD_OUT);
    fdErr = ioGlobalStdGet (STD_ERR);
#endif /* SHELL_COMPATIBLE == TRUE */

#if SHELL_SECURE == TRUE

    taskId = taskSpawn ("tLoginTask", shellTaskDefaultPriorityGet (),
			shellTaskDefaultOptionsGet(),
			shellTaskDefaultStackSizeGet (),
			usrShellConsoleLogin,
			fdIn, fdOut, fdErr, 0, 0 ,0 ,0, 0, 0, 0);

    if (taskId == TASK_ID_ERROR)
	printErr ("Error: unable to launch a secure shell session on the "\
		  "console.\n");
#else
    shellGenericInit (SHELL_FIRST_CONFIG, (size_t) SHELL_STACK_SIZE, NULL, NULL, 
		      TRUE, SHELL_SECURE, fdIn, fdOut, fdErr);
#endif	/* SHELL_SECURE == TRUE */
#endif	/* SHELL_START_AT_BOOT == TRUE */
    }
    
/******************************************************************************
*
* usrShellConfigure - configure the kernel shell parameters
*
* This routine initializes the kernel shell parameters.
*
* \NOMANUAL
*/
static void usrShellConfigure (void)
    {
    IMPORT char *   shellRemoteConfigStr;
    static BOOL	    isShellConfigured = FALSE;

    if (isShellConfigured)
	return;

#if SHELL_COMPATIBLE == TRUE
    /* Remove private env feature of the kernel shell task */

    shellTaskDefaultOptionsSet ((int)(SHELL_TASK_OPTIONS & ~VX_PRIVATE_ENV));
#else
    shellTaskDefaultOptionsSet ((int)SHELL_TASK_OPTIONS);
#endif	/* SHELL_COMPATIBLE == TRUE */

    shellTaskDefaultStackSizeSet ((size_t) SHELL_STACK_SIZE);
    shellTaskDefaultPrioritySet (SHELL_TASK_PRIORITY);
    shellTaskDefaultBaseNameSet (SHELL_TASK_NAME_BASE);
    shellMaxSessionsSet (SHELL_MAX_SESSIONS);

    shellConfigDefaultSet (SHELL_DEFAULT_CONFIG);
    shellCompatibleSet (SHELL_COMPATIBLE);

    /* Set the remote configuration string for a later use */

    shellRemoteConfigStr = SHELL_REMOTE_CONFIG;

    /* Initialize the user interface library */

    usrLibInit (SHELL_SPAWNED_TASK_STACK_SIZE, SHELL_SPAWNED_TASK_PRIORITY,
		SHELL_SPAWNED_TASK_OPTIONS);

    isShellConfigured = TRUE;
    }

#if (SHELL_START_AT_BOOT == TRUE) && (SHELL_SECURE == TRUE)
/******************************************************************************
*
* usrShellConsoleLogin - login to the console
*
* This routine is executed to ask the user to logs into a shell session
* attached to the console.
*
* RETURNS: 0
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static int usrShellConsoleLogin
    (
    int	fdIn,
    int	fdOut,
    int	fdErr
    )
    {
    /* Wait until a valid session is launched */

    while (shellGenericInit (SHELL_FIRST_CONFIG, (size_t) SHELL_STACK_SIZE, 
			     NULL, NULL, TRUE, SHELL_SECURE, fdIn, fdOut, 
			     fdErr) != OK)
	taskDelay (sysClkRateGet () / 4);

    return 0;
    }
#endif

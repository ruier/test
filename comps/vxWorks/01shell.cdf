/* 01shell.cdf - Kernel shell components */

/*
 * Copyright (c) 2003-2007, 2009-2012 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
02h,15may12,jmp  CQID: Fix WIND00309977 : Removed obsolete COPROCS_ALL from
                 SHELL_TASK_OPTIONS & SHELL_SPAWNED_TASK_OPTIONS, replaced by
                 VX_COPROCS_ALL_TASK in SHELL_SPAWNED_TASK_OPTIONS.
02g,18mar11,jmp  renamed SHELL_SP_* parameters to SHELL_SPAWNED_TASK_*.
02f,16mar11,jmp  Added SHELL_SP_STACK_SIZE, SHELL_SP_PRIORITY, SHELL_SP_OPTIONS
                 parameters (CQ:WIND00235451).
02e,09jul10,v_r  Now mentioning relationship between SHELL_SECURE and
                 INCLUDE_SECURITY (req CQ:WIND00190686).
02d,16apr10,rlp  Removed usrBreakpoint.c configlette.
02c,08sep09,v_r  Added shell session limitation system (def CQ:WIND00155318,
                 merged from 6.5 sustaining).
02c,25mar09,v_r  Followed ledLib private routines move into ledLibP header.
02b,06dec07,rlp  Removed tornado references (CQ:WIND00112534).
02a,24aug07,v_r  Tie INCLUDE_SHELL_HISTORY_FILE to the SHELL_FOLDER (def
                 CQ:WIND00102457).
01z,18jul07,v_r  Added the persistent shell history mechanism (req
                 CQ:WIND00005098).
01y,04apr07,v_r  Added shell static initialization component (req
                 CQ:WIND00045277).
01x,18oct06,rlp  Renamed dbgRtpInit to vxdbgRtpInit (REQ00007655).
01x,20jun06,pee  typos
01w,21mar06,bpn  Added synopsis to INCLUDE_STARTUP_SCRIPT (SPR#118018).
01v,07sep05,bpn  Added new shell component parameters SHELL_TASK_PRIORITY,
                 SHELL_TASK_NAME_BASE and SHELL_TASK_OPTIONS (SPR#106622).
01u,22jun05,yvp  Added dependency on INCLUDE_TASK_RESTART.
01t,13apr05,bpn  Remove use of quote character from INCLUDE_HW_FP_MREGS
                 component.
01s,17mar05,asa  Added new component INCLUDE_HW_FP_MREGS
                 updated copyright to 2005
01r,31jan05,bpn  Added dependency between the shell and the line editing modes.
                 Moved commands components to 02shell_commands.cdf. Added 
		 back FOLDER_SHELL_CMD definition from 00folder.cdf.
01q,25jan05,bpn  Added back the module definitions for INCLUDE_SHELL.
01p,10jan05,bpn  Added INCLUDE_RTP_DEBUG invisible component to remove 
                 dependency between the shell and the debug facilites.
		 Changed RTP related component names to "process".
01o,05nov04,dbt  Added startupScriptFieldSplit prototype (SPR #103343).
01n,26oct04,bpn  Changed path delimiter characters of VXE_PATH and configuration
                 variables.
01m,02oct04,bpn  Added rtp and loader related commands.
01l,01oct04,zl   added memory management related commands.
01k,09sep04,bpn  Added new components for the shell commands related to debug,
                 tasks and symbols.
01j,28jun04,kk   fix SPR# 98380 - move FOLDER_SHELL & FOLDER_SHELL_CMD into a
                 separate folder
01i,21jun04,md   Add INCLUDE_EDR_SHELL_CMD to usrShellInit
01h,21may04,fr   Added PROTOTYPE for INCLUDE_EDR_SHELL_CMD.
01g,18may04,bpn  Removed dependency between the command interpreter component
                 and the shell component.
01f,01apr04,bpn  Added PROTOTYPE for INCLUDE_DISK_UTIL_SHELL_CMD.
01e,26feb04,yvp  Changed INIT_RTN for INCLUDE_STARTUP_SCRIPT.
01d,11feb04,bpn  Renamed file to 01shell.cdf. Reworked components (SPR#93992).
01c,10feb04,bpn  Added VXE_PATH configuration variable.
01b,06nov03,bpn  Moved global shell configuration parameters to
                 SHELL_DEFAULT_CONFIG. Created SHELL_FIRST_CONFIG parameter.
01a,10oct03,bpn  Written from version 04c,22sep03,bpn of 00vxWorks.cdf.


DESCRIPTION

This file contains the definition for the kernel shell components.

The folders FOLDER_SHELL and FOLDER_SHELL_CMD are defined in 00folder.cdf. 
*/

Folder FOLDER_SHELL {
    _CHILDREN	FOLDER_TOOLS
}

Folder FOLDER_SHELL_CMD {
        NAME            shell commands
        SYNOPSIS        Commands for the shell command interpreter
        _CHILDREN       FOLDER_SHELL
}

Component INCLUDE_SHELL {
	NAME		target-resident kernel shell
	SYNOPSIS        Interpreter for interactive development, prototyping, \
			debugging and testing.
	CONFIGLETTES	usrShell.c
	MODULES		shellRemoteAccessLib.o shellCompletionLib.o usrLib.o
	INIT_RTN	usrShell ();
	CFG_PARAMS	SHELL_TASK_NAME_BASE		\
			SHELL_TASK_PRIORITY		\
			SHELL_TASK_OPTIONS		\
			SHELL_STACK_SIZE		\
			SHELL_SECURE			\
			SHELL_START_AT_BOOT		\
			SHELL_COMPATIBLE		\
			SHELL_DEFAULT_CONFIG		\
			SHELL_FIRST_CONFIG		\
			SHELL_REMOTE_CONFIG		\
			SHELL_MAX_SESSIONS		\
			SHELL_SPAWNED_TASK_STACK_SIZE	\
			SHELL_SPAWNED_TASK_PRIORITY	\
			SHELL_SPAWNED_TASK_OPTIONS
	REQUIRES	SELECT_SHELL_EDIT_MODE		\
			SELECT_SHELL_INTERP		\
			INCLUDE_TASK_RESTART		\
			INCLUDE_SHELL_CORE
	_CHILDREN	FOLDER_SHELL
	_DEFAULTS	FOLDER_SHELL
}

Component INCLUDE_SHELL_CORE {
	NAME		shell core files
	SYNOPSIS	Handles the shell core files
	MODULES		shellLib.o
	INIT_RTN	shellLibInit ();
	HDR_FILES	private/shellLibP.h
	REQUIRES        INCLUDE_SHELL
	_CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_DEBUG {
	NAME		debugging facilities
	SYNOPSIS	Breakpoints and stack tracer on target. Not needed for \
			remote debugging with Workbench.
	MODULES		dbgLib.o
	INIT_RTN	dbgInit ();
	HDR_FILES	dbgLib.h cacheLib.h
	INCLUDE_WHEN	INCLUDE_SHELL
	REQUIRES	INCLUDE_SHELL
	_CHILDREN	FOLDER_SHELL
	_DEFAULTS	FOLDER_SHELL
}

Component INCLUDE_RTP_DEBUG {
	NAME		process debugging facilities
	SYNOPSIS	Initialize process debugging library.
	INIT_RTN	vxdbgRtpLibInit ();
	HDR_FILES	private/vxdbgRtpLibP.h
	_CHILDREN	FOLDER_NOT_VISIBLE
	INCLUDE_WHEN	INCLUDE_DEBUG INCLUDE_RTP INCLUDE_SHELL
	REQUIRES	INCLUDE_DEBUG INCLUDE_RTP INCLUDE_SHELL
}

Component INCLUDE_SHELL_BANNER {
	NAME		shell banner
	SYNOPSIS	Display the WRS banner on startup
	HDR_FILES	sysLib.h stdio.h usrLib.h
	CONFIGLETTES	usrBanner.c
	INIT_RTN	usrBanner ();
	REQUIRES	INCLUDE_SHELL
	_CHILDREN	FOLDER_SHELL
	_DEFAULTS	FOLDER_SHELL
}

Selection SELECT_SHELL_EDIT_MODE {
        NAME            shell editing mode
        SYNOPSIS        Select editing mode for the kernel shell
        COUNT           1-
        CHILDREN        INCLUDE_SHELL_VI_MODE \
                        INCLUDE_SHELL_EMACS_MODE
        DEFAULTS        INCLUDE_SHELL_VI_MODE
	_CHILDREN	FOLDER_SHELL
	_DEFAULTS	FOLDER_SHELL
}

Component INCLUDE_SHELL_VI_MODE {
        NAME            vi-like editing mode
        SYNOPSIS        Editing mode similar to the Vi editing mode
	HDR_FILES	private/ledLibP.h
        INIT_RTN 	ledModeRegister (viLedLibInit);
        MODULES         viLedLib.o
	REQUIRES	INCLUDE_SHELL
}

Component INCLUDE_SHELL_EMACS_MODE {
        NAME            emacs-like editing mode
        SYNOPSIS        Editing mode similar to the emacs editing mode
	HDR_FILES	private/ledLibP.h
        INIT_RTN        ledModeRegister (emacsLedLibInit);
        MODULES         emacsLedLib.o
	REQUIRES	INCLUDE_SHELL
}

Selection SELECT_SHELL_INTERP {
        NAME            shell interpreter selection
        SYNOPSIS        Select the interpreter for the kernel shell
        COUNT           1-
        CHILDREN        INCLUDE_SHELL_INTERP_C \
			INCLUDE_SHELL_INTERP_CMD
        DEFAULTS        INCLUDE_SHELL_INTERP_C
	_CHILDREN	FOLDER_SHELL
	_DEFAULTS	FOLDER_SHELL
}

Component INCLUDE_SHELL_INTERP_C {
        NAME            c line interpreter
        SYNOPSIS        The C interpreter for the kernel shell
        HDR_FILES	shellInterpLib.h
        INIT_RTN 	shellInterpRegister (shellInterpCInit);
        MODULES         shellInterpC.o
	INCLUDE_WHEN	INCLUDE_SHELL
}

Component INCLUDE_SHELL_INTERP_CMD {
        NAME            command line interpreter
        SYNOPSIS        The command interpreter for the kernel shell
        HDR_FILES	shellInterpLib.h
        INIT_RTN	shellInterpRegister (shellInterpCmdInit);
        MODULES         shellInterpCmd.o shellInterpCmdLib.o
	INCLUDE_WHEN	INCLUDE_SHELL INCLUDE_RTP
}

Component INCLUDE_SHELL_HISTORY_FILE {
	NAME		shell history saving/loading mechanism
	SYNOPSIS	Allows to save and load the shell history
	HDR_FILES	private/shellHistLibP.h
	INIT_RTN	shellHistLibInit ();
	MODULES		shellHistLib.o usrShellHistLib.o
	LINK_SYMS	histSave
	_CHILDREN	FOLDER_SHELL
}

Component INCLUDE_STARTUP_SCRIPT {
	NAME		kernel shell startup script
        SYNOPSIS        Initial kernel shell script run at VxWorks startup
	CONFIGLETTES	usrScript.c
        PROTOTYPE       extern char * startupScriptFieldSplit (char * field);
	INIT_RTN	usrStartupScript (startupScriptFieldSplit (sysBootParams.startupScript));
	HDR_FILES	sysLib.h stdio.h shellLib.h usrLib.h ioLib.h \
			shellConfigLib.h
	REQUIRES	INCLUDE_SHELL
	HELP		shellLib
	_CHILDREN	FOLDER_SHELL
}

Component INCLUDE_HW_FP_MREGS {
	NAME		floating point registers modification
	SYNOPSIS	Provides ability to modify floating point registers \
			from the kernel shell.
	MODULES		fppRegsLib.o
	INCLUDE_WHEN	INCLUDE_HW_FP_SHOW \
			INCLUDE_SHELL
	REQUIRES	INCLUDE_HW_FP_SHOW \
			INCLUDE_SHELL
	_CHILDREN 	FOLDER_NOT_VISIBLE
}

Parameter SHELL_TASK_NAME_BASE {
	NAME		kernel shell task basename string
	SYNOPSIS	Default basename for the kernel shell tasks
	TYPE		string
	DEFAULT		"tShell"
}

Parameter SHELL_TASK_PRIORITY {
	NAME		kernel shell task priority
	SYNOPSIS	Default priority of the kernel shell tasks
	TYPE		uint
	DEFAULT		1
}

Parameter SHELL_TASK_OPTIONS {
	NAME		Default kernel shell task spawn options
	SYNOPSIS	This parameter defines options for kernel shell tasks \
			initialization. Supported coprocessor options such as \
			VX_FP_TASK are automatically added by the coprocessor \
			discovery facility and do not need to be specified.
	TYPE		uint
	DEFAULT		(VX_UNBREAKABLE | VX_PRIVATE_ENV)
}

Parameter SHELL_STACK_SIZE {
	NAME		kernel shell stack size
	SYNOPSIS	Default stack size of kernel shell tasks (bytes)
	TYPE		uint
	DEFAULT		0x10000
}

Parameter SHELL_SECURE {
	NAME		kernel shell login access
	SYNOPSIS	Access the kernel shell attached to the console through \
			a login access. It is not possible to have a secure \
			shell in compatible mode (SHELL_COMPATIBLE parameter). \
			Setting this parameter mandates INCLUDE_SECURITY be \
			present within the project.
	TYPE		bool
	DEFAULT		FALSE
}

Parameter SHELL_START_AT_BOOT {
	NAME		kernel shell launched at boot time
	SYNOPSIS	The kernel shell is launched automatically at boot \
			line on the console. It is not possible to disable \
			shell startup at boot time in compatible mode \
			(SHELL_COMPATIBLE parameter).
	TYPE		bool
	DEFAULT		TRUE
}

Parameter SHELL_COMPATIBLE {
	NAME		compatible behavior of the kernel shell
	SYNOPSIS	The kernel shell is configured to be compatible with \
			the vxWorks 5.5 shell: one shell session, global I/O \
			redirected, shell task options without VX_PRIVATE_ENV \
			bit. It is not possible to use that mode with a secure \
			shell or when shell startup is disabled at boot time \
			(SHELL_SECURE and/or SHELL_START_AT_BOOT parameters).
	TYPE		bool
	DEFAULT		FALSE
}

Parameter SHELL_DEFAULT_CONFIG {
	NAME		default kernel shell configuration variables
	SYNOPSIS	The default configuration variables of the kernel \
			shell can be set using this string.
	TYPE		string
	DEFAULT		"INTERPRETER=,LINE_EDIT_MODE=,LINE_LENGTH=256,STRING_FREE=manual,VXE_PATH=.;/romfs"
}

Parameter SHELL_FIRST_CONFIG {
	NAME		initial shell session configuration variables
	SYNOPSIS	The specific configuration variables of the initial \
			kernel shell session can be set using this string.
	TYPE		string
	DEFAULT		NULL
}

Parameter SHELL_REMOTE_CONFIG {
	NAME		remote shell session configuration variables
	SYNOPSIS	The specific configuration variables of the kernel shell sessions started for a remote connection can be set using this string.
	TYPE		string
	DEFAULT		NULL
}

Parameter SHELL_MAX_SESSIONS {
        NAME            maximum number of simultaneous shell sessions
        SYNOPSIS        The maximum amount of simultaneous shell sessions      \
                        allowed at a time. When the session limit is reached,  \
                        further connections are rejected with a message to the \
                        user. Setting the value to -1 disables the session     \
                        limit system. This feature is not available in \
			compatible mode (SHELL_COMPATIBLE parameter).
        TYPE            int
        DEFAULT         -1
}

Parameter SHELL_SPAWNED_TASK_STACK_SIZE {
	NAME		Stack size of tasks spawned by the shell
	SYNOPSIS	Defines in bytes the stack size of the tasks spawned by sp(), period() and repeat() shell routines.
	TYPE		uint
	DEFAULT		20000
}

Parameter SHELL_SPAWNED_TASK_PRIORITY {
	NAME		Priority of tasks spawned by the shell
	SYNOPSIS	Defines the priority of the tasks spawned by sp(), period() and repeat() shell routines.
	TYPE		uint
	DEFAULT		100
}

Parameter SHELL_SPAWNED_TASK_OPTIONS {
	NAME		Options of tasks spawned by the shell
	SYNOPSIS	Defines the options of the tasks spawned by sp(), period() and repeat() shell routines.
	TYPE		uint
	DEFAULT		(VX_COPROCS_ALL_TASK | VX_STDIO)
}

InitGroup usrShellInit {
	INIT_RTN	usrShellInit ();
	SYNOPSIS	The kernel shell initialization sequence
	INIT_ORDER	INCLUDE_SHELL_CORE \
			INCLUDE_SHELL_HISTORY_FILE \
			INCLUDE_DEBUG \
			INCLUDE_RTP_DEBUG \
			INCLUDE_SHELL_BANNER \
                        INCLUDE_SHELL_VI_MODE \
			INCLUDE_SHELL_EMACS_MODE \
                        INCLUDE_SHELL_INTERP_C \
			INCLUDE_SHELL_INTERP_CMD \
			usrShellCmdInit \
			INCLUDE_STARTUP_SCRIPT \
			INCLUDE_SHELL
	_INIT_ORDER	usrToolsInit
	INIT_AFTER	usrShowInit	
}

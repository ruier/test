/* 01rtp.cdf - Real Time Process component definitions */

/*
 * Copyright (c) 2003-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03v,09mar10,pad  Do not show RTP_OVERLAPPED_ADDRESS_SPACE, RTP_CODE_REGION_SIZE
		 and RTP_CODE_REGION_START parameters for VxWorks 64-bit.
03u,27jan10,cww  Added memRtpLibInit to header file
03t,25jan10,kk   change to use overlap as default
03s,08jan10,zl   split INCLUDE_MMAP from INCLUDE_SC_MMAN. Moved KERNEL_HEAP_SIZE
                 parameter to 02mm.cdf
03r,08dec09,cww  Added INCLUDE_TASK_STACK_ALLOC
03q,22sep09,kk   hide INCLUDE_RTP_POSIX_PSE52 (WIND00182189)
03p,04sep09,kk   remove unused RTP_KERNEL_STACK_SIZE parameter (WIND00180527)
03p,13aug09,cww  Updated ordering of RTP init routines
03o,23jun08,pad  Added the RTP_OVERLAPPED_ADDRESS_SPACE parameter to switch
		 between flat and overlapped virtual memory models.
03n,07may08,pad  Added the RTP_CODE_REGION_SIZE and RTP_CODE_REGION_START
		 parameters.
03m,17oct07,mfj  Fixing problem with KERNEL_HEAP_SIZE (WIND00102855)
03l,01aug07,act  io footprint work (from pcm)
03k,06jul06,pcs  RTP heap options is now configurable.
03j,17mar06,pad  Fixed pxCpuTimerLib.o module name (SPR #119108).
03i,14feb06,jln  added INCLUDE_POSIX_THREAD_CPUTIME component
03h,18jan06,pad  Added component INCLUDE_RTP_POSIX_PSE52 and parameter
		 RTP_FD_NUM_MAX. Changed call to rtpLibInit().
03g,18aug05,kk   updated synopsis for RTP_SIGNAL_QUEUE_SIZE
03f,03aug05,kk   make the RTP_SIGNAL_QUEUE_SIZE default to be 32 to conform to
                 POSIX
03e,09jul05,yvp  Hook components now depend on INCLUDE_HOOKS.
03d,22jun05,yvp  Added dependency on INCLUDE_TASK_RESTART and 
		 INCLUDE_TASK_CREATE_DELETE for system calls.
03c,12may05,kk   added dependency of VXEVENTS for RTP (SPR# 109130)
03b,20apr05,gls  added INCLUDE_OBJ_OWNERSHIP (SPR #106150)
03a,15apr05,yvp  Fixed INCLUDE_RTP_APPL_INIT_STRING default value (SPR 107365).
		 Changed INCLUDE_RTP_APPL_USER configlette (SPR 105759).
02z,26feb05,pcs  Modified to reflect the change to the prototype of fn.
                 rtpLibInit
02y,27jan05,dbt  Removed dependency on loader (SPR #101436).
02x,24nov04,kk   ensure RTP_HOOKS gets included when SHL is included
02w,24nov04,fle  Fixed simple quotes in components synopses
02v,23nov04,zl   added type definition to KERNEL_HEAP_SIZE.
02u,22nov04,yvp  Added synopsis for RTP startup facility components.
02t,15nov04,yvp  Changed prototype for usrRtpAppInitString().
02s,11nov04,job  System call components added to usrScInit init group.
		 Fix INCLUDE_MEM_RTP; added INCLUDE_SC_SD, INCLUDE_SC_SHL
02r,18oct04,fr	 added INCLUDE_OBJ_OPEN to INCLUDE_RTP (SPR 101491)
02q,14oct04,pad  Moved rtpHookLibInit in the usrRoot group, to be initialized
                 before rtpLib (SPR #102595).
02p,02oct04,gls  added private/sdLibP.h to INCLUDE_SHARED_DATA component
02o,02oct04,bpn  Moved shell commands components to 01shell.cdf.
02o,25sep04,dcc  added objRtpLib.o to the list of rtp modules
02n,23sep04,kk   added INCLUDE_RTP_SHOW_SHELL_CMD
02m,02sep04,yvp  rtpHookLibInit() prototype change.
02l,31aug04,job  Made INCLDUE_RTP require INCLUDE_LOADER_CODE (SPR#101151)
02k,28aug04,dbt  Fixed previous modification.
02j,27aug04,gls  added SD_HOOK_TBL_SIZE
02i,28jul04,md   Removed need for INCLUDE_EDR_SYSDBG_FLAG
02h,28jun04,job  Changed INCLUDE_SYSCTL_API to INCLUDE_SYSCTL
02g,21jun04,kk   added INIT_ORDER for RTP_HOOKS & SYSCALL_HOOKS, added
                 INCLUDE_SHL_SHELL_CMD
02f,20may04,job  INCLUDE_RTP now requires INCLUDE_SYSCTL_API
02e,20may04,gls  renamed mmScLib to mmanScLib
02d,18may04,yvp  Added INCLUDE_RTP_APPL_INIT_CMD_SHELL_SCRIPT.
02c,18may04,bpn  Added invisible INCLUDE_RTP_SHELL_C component.
02b,03may04,kk   added INCLUDE_SHL_SHOW component
02a,09apr04,kk   added INCLUDE_SHL components
01z,07may04,yvp  Added RTP Startup Facility Components INCLUDE_RTP_APPL_*
01y,06apr04,gls  added INCLUDE_SHARED_DATA_SHOW
01x,06apr04,job  Pass TASK_STACK_OVERFLOW_SIZE and TASK_STACK_UNDERFLOW_SIZE
		 to rtpLibInit(). Removed the INIT_AFTER line from the
		 definition of INCLUDE_RTP_APPL
01w,05apr04,yvp  Moved RTP_APPL component inclusion in tree to this file. Made
                 RTP component a child of the OS folder.
01v,02apr04,tam  fixed init of SHARTED_DATA (SPR 95720)
01u,03mar04,md   include ED&R policies when RTP's are included
01t,24feb04,yvp  Added component INCLUDE_RTP_APPL.
01s,18feb04,ans  Added RTP_SIGNAL_QUEUE_SIZE CFG_PARAMS in INCLUDE_RTP
                 component.
01r,12dec03,yvp  Added syscallTbl.h to INCLUDE_RTP inlcude list.
01q,08dec03,m_s  removed definition of SC_POSIX
01p,05dec03,zl   added INCLUDE_MEM_RTP component, RTP_HEAP_INIT_SIZE parameter
01p,04dec03,ans  removed REQUIRES SC_POSIX from INCLUDE_RTP component
01o,03dec03,bpn  Fixed init of the component INCLUDE_RTP_SHELL_CMD.
01n,21oct03,dat  SC_IOS subcomponent, general cleanup/refactor of RTP 
		 components.
01m,11nov03,kk   added INCLUDE_WHEN on INCLUDE_SHOW_ROUTINES for all show
                 components
01l,04nov03,kk   added SHARED DATA component
01k,23oct03,tam  added KERNEL_HEAP_SIZE parameter to RTP component.
01j,12nov03,yvp  Added support for RTP hooks.
01i,31oct03,yvp  Added INCLUDE_RTP_ARCH.
01h,03oct03,ans  INCLUDE_RTP now REQUIRES INCLUDE_SC_POSIX
01g,29sep03,pad  Now force the inclusion of system call handler components.
01f,22sep03,bpn  Added INCLUDE_RTP_SHELL_CMD component.
01e,04sep03,nrj  added auxiliary vector module
01d,19aug03,kk   removed rtpElfLoadLib.o, rtpALib.o, excScPpcALib.o 
                 from INCLUDE_RTP. The ALib.o files should be arch specific.
                 made kernel stack size a page.
		 added INCLUDE_RTP_SHOW component
01c,15jul03,job  Fixed spelling mistakes
01b,14jul03,nrj  added modules that are arch specific, system call
01a,27jun03,nrj	 written to include RTP
*/

/*
This file contains descriptions for all of the RTP components.
*/

/* Attach main RTP folder to the root folder, don't make it a default */

Folder	FOLDER_RTP {
	NAME		Real Time Process components
	_CHILDREN	FOLDER_OS
	CHILDREN	INCLUDE_RTP \
			INCLUDE_SHARED_DATA \
			INCLUDE_SHL \
			INCLUDE_RTP_HOOKS \
			INCLUDE_SYSCALL_HOOKS
	DEFAULTS	INCLUDE_RTP
}

Component INCLUDE_RTP {
	NAME		RTP
	SYNOPSIS	Real Time Process.
	MODULES		rtpLib.o rtpAuxvLib.o syscallLib.o objRtpLib.o
        INIT_RTN        rtpLibInit (RTP_HOOK_TBL_SIZE, SYSCALL_HOOK_TBL_SIZE, \
				    RTP_SIGNAL_QUEUE_SIZE, \
				    TASK_USER_EXC_STACK_OVERFLOW_SIZE, \
				    TASK_USER_EXEC_STACK_OVERFLOW_SIZE, \
				    TASK_USER_EXEC_STACK_UNDERFLOW_SIZE, \
				    RTP_FD_NUM_MAX);
	HDR_FILES	rtpLib.h private/rtpLibP.h auxv.h syscallLib.h \
			syscallTbl.h private/syscallLibP.h
	REQUIRES	INCLUDE_MEM_RTP \
			INCLUDE_TASK_STACK_ALLOC \
			INCLUDE_SC_KERNEL \
			INCLUDE_SC_IOS \
			INCLUDE_SC_MMAN \
                        INCLUDE_SYSCTL \
			INCLUDE_RTP_ARCH \
			INCLUDE_OBJ_OPEN \
			INCLUDE_OBJ_OWNERSHIP \
			INCLUDE_VXEVENTS
#ifndef _WRS_CONFIG_LP64
	CFG_PARAMS	RTP_HOOK_TBL_SIZE \
			SYSCALL_HOOK_TBL_SIZE \
			RTP_SIGNAL_QUEUE_SIZE \
			RTP_HEAP_INIT_SIZE \
			RTP_HEAP_DEFAULT_OPTIONS \
			RTP_FD_NUM_MAX \
			RTP_CODE_REGION_START \
			RTP_CODE_REGION_SIZE \
			RTP_OVERLAPPED_ADDRESS_SPACE
#else /* _WRS_CONFIG_LP64 */
	/*
	 * For VxWorks 64-bit RTPs are always executed in an overlapped
	 * virtual memory space and nothing about this is configurable.
	 */

	CFG_PARAMS	RTP_HOOK_TBL_SIZE \
			SYSCALL_HOOK_TBL_SIZE \
			RTP_SIGNAL_QUEUE_SIZE \
			RTP_HEAP_INIT_SIZE \
			RTP_HEAP_DEFAULT_OPTIONS \
			RTP_FD_NUM_MAX
#endif /* _WRS_CONFIG_LP64 */
}

Component INCLUDE_RTP_ARCH {
        NAME            Arch components for RTP
        _CHILDREN       FOLDER_NOT_VISIBLE
        REQUIRES        INCLUDE_RTP
}


Component INCLUDE_RTP_SHOW {
	NAME		RTP Show
	SYNOPSIS	Display information on RTP.
	MODULES		rtpShow.o
	INIT_RTN	rtpShowInit ();
	HDR_FILES	rtpLib.h
	INCLUDE_WHEN	INCLUDE_RTP \
			INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit

}

Component INCLUDE_SHARED_DATA {
	NAME		shared data region support in RTPs or kernel
	SYNOPSIS	provide data sharing between RTPs or kernel
	MODULES		sdLib.o
	INIT_RTN	sdLibInit (SD_HOOK_TBL_SIZE);
	CFG_PARAMS	SD_HOOK_TBL_SIZE
	REQUIRES	INCLUDE_RTP INCLUDE_SC_SD
	HDR_FILES	sdLib.h private/sdLibP.h
}

Component INCLUDE_SHARED_DATA_SHOW {
	NAME		Shared Data Show
	SYNOPSIS	Display information on Shared Data.
	MODULES		sdShow.o
	INIT_RTN	sdShowInit ();
	REQUIRES	INCLUDE_SHARED_DATA
	HDR_FILES	sdLib.h
	INCLUDE_WHEN	INCLUDE_SHARED_DATA \
			INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit
	INIT_AFTER      INCLUDE_RTP_SHOW	
}

Component INCLUDE_SHL {
	NAME		shared library support in RTPs
	SYNOPSIS	provide sharing of code between RTPs
	MODULES		shlLib.o
	INIT_RTN	shlLibInit ();
	REQUIRES	INCLUDE_RTP \
			INCLUDE_SHARED_DATA \
			INCLUDE_SC_SHL \
			INCLUDE_RTP_HOOKS
	HDR_FILES	shlLib.h
}

Component INCLUDE_SHL_SHOW {
	NAME		Shared Library Show
	SYNOPSIS	Display information on Shared Libraries
	MODULES		shlShow.o
	INIT_RTN	shlShowInit ();
	REQUIRES	INCLUDE_SHL
	HDR_FILES	shlLib.h
	INCLUDE_WHEN	INCLUDE_SHL \
			INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit
	INIT_AFTER      INCLUDE_RTP_SHOW	
}

Component INCLUDE_RTP_SHELL_C {
	NAME		RTP Shell C Routines
	SYNOPSIS	List of commands for the shell C interpreter related to RTPs.
	MODULES		usrRtpLib.o
	INIT_RTN	usrRtpLibInit ();
	PROTOTYPE	extern STATUS usrRtpLibInit();
	_INIT_ORDER	usrToolsInit
	_CHILDREN       FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_RTP
	INCLUDE_WHEN	INCLUDE_RTP INCLUDE_SHELL_INTERP_C
}


Component INCLUDE_RTP_HOOKS {
	NAME		RTP Hook Support
	SYNOPSIS	Add hook routines to be called on RTP operations.
	MODULES		rtpHookLib.o
	INIT_RTN	rtpHookLibInit (RTP_HOOK_TBL_SIZE);
	REQUIRES	INCLUDE_RTP \
                        INCLUDE_HOOKS
}

Component INCLUDE_SYSCALL_HOOKS {
	NAME		System Call Hook Support
	SYNOPSIS	Add hook routines to be called on system calls.
	MODULES		syscallHookLib.o
	INIT_RTN	syscallHookLibInit ();
	REQUIRES	INCLUDE_RTP \
                        INCLUDE_HOOKS
	_INIT_ORDER	usrKernelExtraInit
}

/* 
 * INCLUDE_RTP_POSIX_PSE52 is a utility component. This component
 * does not drag in code, it simply sets a set of parameters appropriate
 * for PSE52 support. This component alone does not provide PSE52 support,
 * it requires inclusion of the set of CDF components associated with PSE52,
 * as done in the BUNDLE_RTP_POSIX_PSE52 bundle.
 * This component is hidden to avoid confusion. Users are encouraged to 
 * use the BUNDLE_RTP_POSIX_PSE52 to pick up PSE52 support.
 */

Component INCLUDE_RTP_POSIX_PSE52 {
	NAME		POSIX PSE52 support for RTPs
	SYNOPSIS	Utility component used in configlette code to set parameters appropriately for POSIX PSE52.
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_POSIX_THREAD_CPUTIME {
        NAME            POSIX thread CPU-time clock
        SYNOPSIS        Support RTP-only thread CPU-time clock/timer
        MODULES         pxCpuTimerLib.o
        INIT_RTN        pxCpuTimeLibInit ();
        HDR_FILES       private/pxCpuTimeLibP.h
	_CHILDREN	FOLDER_RTP
	_INIT_ORDER	usrKernelExtraInit
	INIT_AFTER      INCLUDE_POSIX_TIMERS	
        REQUIRES        INCLUDE_POSIX_TIMERS INCLUDE_POSIX_CLOCKS \
                        INCLUDE_POSIX_PTHREAD_SCHEDULER
}

Parameter RTP_FD_NUM_MAX
	{
	NAME		Number of entries in an RTP fd table
	SYNOPSIS	Maximum number of file descriptors for an RTP. See also the NUM_FILES parameter.
	TYPE		uint
	DEFAULT		20
	}

Parameter RTP_HOOK_TBL_SIZE
	{
	NAME		number of entries in the RTP create/delete hook tables
	SYNOPSIS	number of entries in the RTP create/delete hook tables
        TYPE 		uint
        DEFAULT 	8
	}

Parameter SD_HOOK_TBL_SIZE
	{
	NAME		number of entries in the SD hook tables
	SYNOPSIS	number of entries in the SD hook tables
        TYPE 		uint
        DEFAULT 	8
	}

Parameter SYSCALL_HOOK_TBL_SIZE
	{
	NAME		number of entries in the system call hook tables
	SYNOPSIS	number of entries in the system call hook tables
        TYPE 		uint
        DEFAULT 	8
	}

Parameter RTP_HEAP_DEFAULT_OPTIONS {
        NAME            default memory partition options for RTP heap
        SYNOPSIS        default memory partition options for RTP heap
        TYPE            UINT
        DEFAULT         (MEM_ALLOC_ERROR_LOG_FLAG      | \
                         MEM_ALLOC_ERROR_EDR_WARN_FLAG | \
                         MEM_BLOCK_ERROR_LOG_FLAG      | \
                         MEM_BLOCK_ERROR_EDR_FATAL_FLAG | \
                         MEM_BLOCK_CHECK)
}

Parameter RTP_HEAP_INIT_SIZE
        {
        NAME            initial size of RTP heap
        SYNOPSIS        initial size of the heap of each RTP created; the \
                        HEAP_INITIAL_SIZE environment variable can override it.
        TYPE            uint
        DEFAULT         0x10000
        }

Parameter RTP_SIGNAL_QUEUE_SIZE
	{
	NAME		maximum number of queued signal for a RTP
	SYNOPSIS        maximum number of queued signal for a RTP. For POSIX, \
	                this value must be at least 32.
        TYPE 		uint
        DEFAULT 	32
	}

#ifndef _WRS_CONFIG_LP64
/*
 * For VxWorks 64-bit RTPs are always executed in an overlapped
 * virtual memory space and nothing about this is configurable.
 */

Parameter RTP_CODE_REGION_START
	{
	NAME		Memory area reserved for absolutely linked RTP
	SYNOPSIS	Base address of the virtual memory region reserved for \
			the text and data/bss segments (i.e. all loadable \
			sections) of absolutely linked RTP executables. If \
			the address is null no such region is reserved.
	TYPE		uint
	DEFAULT		0
	}

Parameter RTP_CODE_REGION_SIZE
	{
	NAME		Size of the area reserved for absolutely linked RTP
	SYNOPSIS	This is the size of the virtual memory region reserved \
			for the text and data/bss segments (i.e. all loadable \
			sections) of absolutely linked RTP executables. If \
			the size is null no such region is reserved.
	TYPE		uint
	DEFAULT		0
	}

Parameter RTP_OVERLAPPED_ADDRESS_SPACE
	{
	NAME		Use overlapped virtual address space for RTPs
	SYNOPSIS	Controls whether each RTP has its own private view of \
			the virtual address space (all views overlapping) or \
			whether they all share a flat virtual address space \
			(legacy). By default, RTPs run in overlapped mode.
	TYPE		bool
	DEFAULT		TRUE
	}
#endif /* _WRS_CONFIG_LP64 */

Component INCLUDE_MEM_RTP {
	NAME		RTP memory configuration setup
	SYNOPSIS	Initializes memory parameters to pass to RTP
	MODULES		memRtpLib.o
	HDR_FILES	private/rtpLibP.h
	INIT_RTN	memRtpLibInit (RTP_HEAP_INIT_SIZE,RTP_HEAP_DEFAULT_OPTIONS);
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER	usrScInit
}

Component INCLUDE_RTP_APPL_USER {
	NAME		RTP Startup Facility: User-defined code
	SYNOPSIS    	Launch RTP from a user-defined function.
	_CHILDREN       FOLDER_APPLICATION
	PROTOTYPE	void usrRtpAppInit (void);
	INIT_RTN	usrRtpAppInit ();
	REQUIRES	INCLUDE_RTP 
	_INIT_ORDER     usrRoot
	HELP		RTP Application Startup facility
}

Component INCLUDE_RTP_APPL_INIT_STRING {
	NAME		RTP Startup Facility: String-encoded RTP list
	SYNOPSIS    	Launch RTP from a string-encoded list.
	_CHILDREN       FOLDER_APPLICATION
	CONFIGLETTES	usrRtpAppInitString.c
	INIT_RTN	usrRtpAppInitString ();
	CFG_PARAMS	RTP_APPL_INIT_STRING
	REQUIRES	INCLUDE_RTP 
	_INIT_ORDER     usrRoot
	HELP		RTP Application Startup facility
}

Parameter RTP_APPL_INIT_STRING {
	NAME		String-encoded RTP list.
	SYNOPSIS	String-encoded list of RTPs to launch after bootup.
    	TYPE            string
	DEFAULT		""
}

Component INCLUDE_RTP_APPL_INIT_BOOTLINE {
	NAME		RTP Startup Facility: Bootline-encoded RTP list
	SYNOPSIS    	Launch RTP from a string-encoded list in the boot parameters.
	_CHILDREN       FOLDER_APPLICATION
	CONFIGLETTES	usrRtpAppInitBootline.c
	INIT_RTN	usrRtpAppInitBootline ();
	REQUIRES	INCLUDE_RTP 
	_INIT_ORDER     usrRoot
	HELP		RTP Application Startup facility
}

Component INCLUDE_RTP_APPL_INIT_CMD_SHELL_SCRIPT {
	NAME		RTP Startup Facility: Command shell startup script 
	SYNOPSIS    	Launch RTP by executing a command-shell script.
	_CHILDREN       FOLDER_APPLICATION
	CONFIGLETTES	usrRtpAppInitCmdShellScript.c
	INIT_RTN	usrRtpAppInitCmdShellScriptRun (RTP_APPL_CMD_SCRIPT_FILE);
	CFG_PARAMS	RTP_APPL_CMD_SCRIPT_FILE
	REQUIRES	INCLUDE_RTP INCLUDE_STARTUP_SCRIPT
	_INIT_ORDER     usrRoot
	HELP		RTP Application Startup facility
}

Parameter RTP_APPL_CMD_SCRIPT_FILE {
	NAME		Command shell script file. 
	SYNOPSIS	Name of file containing the startup script.
	TYPE        	string
	DEFAULT		""
}

/*
 * system call sub-components are always included, be sure to
 * add them to the REQUIRES list for INCLUDE_RTP.  Since they are
 * not individually selectable, attach them to the NOT_VISIBLE folder.
 * They are initialized last, just before the user application.
 */

Component INCLUDE_SC_KERNEL {
	NAME		WIND system call handlers
	SYNOPSIS	system call handlers for WIND kernel 
	MODULES		windScLib.o
	PROTOTYPE	void windScInit (void);
	INIT_RTN	windScInit ();
	REQUIRES	INCLUDE_TASK_CREATE_DELETE	\
			INCLUDE_TASK_RESTART
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_MMAN {
	NAME		Memory management system call handlers
	SYNOPSIS	system call handlers for memory management
	MODULES		mmanScLib.o
	PROTOTYPE	void mmanScLibInit (void);
	INIT_RTN	mmanScLibInit ();
	REQUIRES	INCLUDE_MMAP
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_SD {
	NAME		Shared Data system call handlers
	SYNOPSIS	system call handlers for shared data
	MODULES		sdScLib.o sdLib.o
	PROTOTYPE	void sdScInit (void);
	INIT_RTN	sdScInit ();
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_SHL {
	NAME		Shared Library system call handlers
	SYNOPSIS	system call handlers for shared libraries
	MODULES		shlScLib.o shlLib.o
	PROTOTYPE	void shlScLibInit (void);
	INIT_RTN	shlScLibInit ();
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

Component INCLUDE_SC_IOS {
	NAME		IOS system calls
	SYNOPSIS	IOS system calls
	MODULES		iosScLib.o
	INIT_RTN	iosScLibInit ();
	HDR_FILES	iosScLib.h
	REQUIRES	INCLUDE_RTP \
			INCLUDE_IO_RTP
	_CHILDREN	FOLDER_NOT_VISIBLE
	_INIT_ORDER     usrScInit
}

/* Attach RTP_SHOW to SHOW ROUTINES folder, init with other show routines */

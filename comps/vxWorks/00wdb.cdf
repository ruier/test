/* 00wdb.cdf - WDB Agent components */

/*
 * Copyright (c) 2003-2012 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
03a,05sep12,rlp  CQ:WIND00368203: Marked WDB as ready once the initialization
		 is done.
02z,02jul12,rlp  CQ:WIND00335939. Added a convenience dependency between
                 INCLUDE_WDB_COMM_SERIAL and INCLUDE_WDB_ALWAYS_ENABLED.
02y,15may12,jmp  CQID: Fix WIND00324029: Removed coproc options from
                 WDB_TASK_OPTIONS and WDB_SPAWN_OPTS, replaced by
                 VX_COPROCS_ALL_TASK in WDB_SPAWN_OPTS.
		 Updated INCLUDE_WDB_TASK init routine to use usrWdbTaskModeInit
		 which handles coproc options using coprocsDiscover() if support
		 is included.
02x,07feb12,rlp  CQ:WIND00331131. Added INCLUDE_WDB_KERNEL_INIT_DONE component.
02w,25mar11,rlp  Added INCLUDE_WDB_SSL component.
02v,16mar11,jmp  Performed some component naming/synopsis updates.
02u,07feb11,rlp  Enabled system mode agent for LP64.
02w,29oct10,jmp  Make sure that symbol table initialization is performed
		 before WDB post kernel initialization (CQ:WIND00239405).
02v,25oct10,rlp  Added INCLUDE_WDB_ALWAYS_ENABLED component.
02u,06oct10,jmp  Specified that INCLUDE_WDB_EXC_NOTIFY requires
                 INCLUDE_EDR_POLICY_HOOKS (CQ:WIND00215097).
02t,24sep10,rlp  Added a INCLUDE_SYSDBG_FLAG dependency.
02s,29jul10,jmp  Changed WDB_POOL_SIZE to 0 when system mode agent is not
		 supported: Remove sysMemTop() reference.
02r,02jun10,rlp  Added a dependency on task create hooks for task mode agent.
02q,07may10,jmp  Removed WDB_REGS_SIZE parameter (Coproc register size is now
		 dynamically computed, except in WDB Pre-kernel init config
		 where it is limited to WDB_MTU) (CQ:WIND00200823).
02p,27apr10,jmp  Cleaned up dependencies between WDB and Coprocessor support.
02o,23mar10,rlp  Added adrSpaceLib header file for LP64 adaptation.
02n,26feb10,rlp  Updated INCLUDE_WDB_REG module list for LP64.
02m,02feb10,rlp  Removed system mode and obsolete components for LP64,
		 Updated breakpoint synchronization dependencies for LP64.
02l,06aug09,hbh  Updated MIPC bus name.
02k,12jun09,elp  changed default bus name
02j,26mar09,rlp  Added a dependency on INCLUDE_RTP for INCLUDE_WDB_RTP_XXX
                 components (WIND00160677).
02i,19feb09,rlp  Removed unsupported WDB drivers.
02h,03sep08,dbt  Changed default WDB MIPC bus name (plb_0 instead of plb).
02g,25aug08,jpb  Removed INCLUDE_WDB dependancy on INCLUDE_CLASS_LIST.
02f,25jul08,pgt  added dwarf expressions evaluator test component
02e,24jul08,dbt  Added support for MIPC backend.
02d,12may08,dlk  END2 scalability: remove explicit dependence of
                 INCLUDE_WDB_COMM_END on INCLUDE_END.
02c,21apr08,dbt  Added support for dynamic printf.
02b,06dec07,rlp  Removed tornado references (CQ:WIND00112534).
02a,17oct07,mfj  Fixing problem with WDB_POOL_SIZE (WIND00102855)
01z,28jun07,rlp  Added SMP support for system mode agent.
01y,22jun07,vhe  Improved dependency for INCLUDE_WDB (defect CQ:WIND00090343)
01x,10apr07,zl   added dependency on INCLUDE_CLASS_LIST.
01y,18oct06,rlp  Reduced the number of system breakpoint (REQ00007655).
01x,09sep06,tkf  Replace INCLUDE_LOOPBACK dependency with
                 INCLUDE_IPNET_LOOPBACK dependency.
01w,10jul06,rlp  Fixed some typos (Defect #57471).
01v,06mar06,bpn  Fixed INCLUDE_WDB component.
01u,14nov05,dbt  Added INCLUDE_WDB_SYSTEM_REGS component.
01t,08feb06,dlk  Replace INCLUDE_NET_STACK_START with its opposite,
		 INCLUDE_NET_INIT_SKIP (SPR #117553).
01s,09oct05,dlk  Made INCLUDE_WDB_COMM_END and INCLUDE_WDB_COMM_NETWORK
                 depend upon INCLUDE_NETWORK_STACK_START.
01r,29sep05,bpn  Fixed INCLUDE_WDB_RTP_CONTROL component (SPR#113070).
01q,23aug05,dbt  Fixed WDB_SPAWN_STACK_SIZE component usage.
01p,16aug05,yvp  Moved INCLUDE_OFFSET_SYMBOLS back to INCLUDE_KERNEL.
01o,10Aug05,pee  WDB_REG REQUIRES INCLUDE_COPROCESSOR
01q,11aug05,???  merge from scalability
01p,01aug05,bpn  Added WDB_POOL_SIZE and WDB_REGS_SIZE parameters. Corrected
                 parameters definition.
01o,25jul05,bpn  Added INCLUDE_WDB_RTP_CONTROL
01n,21jul05,yvp  Added INCLUDE_OFFSET_SYMBOLS as a dependency for INCLUDE_WDB.
01n,22jun05,yvp  Added dependency on INCLUDE_TASK_RESTART.
01m,30may05,dbt  Added support for TIPC communication link.
01l,23may05,bpn  Added dependency between INCLUDE_WDB_EXIT/START_NOTIFY and
                 INCLUDE_WDB_TASK_HOOKS (SPR#109496).
01k,15apr05,wap  Add WDB_END_DEVICE_ADDRESS (SPR #82098)
01j,22sep04,elg  Add size configuration of gopher tapes.
01i,16aug04,dbt  Added wait.h header file.
01h,14jun04,elg  Fix compilation warning (SPR #98131).
01g,08jun04,elg  Add component INCLUDE_WDB_TASK_REG to solve dependency.
01f,07jun04,elg  Add ability to start WDB agent before kernel initialization.
		 Improve WDB components.
01f,11jun04,fle  SPR#84640 : Added WDB_SPAWN_STACK_SIZE parameter
01e,12may04,dbt  Added support for WDB Modules & Symbols Synchronization.
01d,28apr04,elg  Remove old fashioned coprocessor components.
01c,23apr04,h_k  removed DSP components.
01b,09feb04,dbt  add WDB_MTU to INCLUDE_WDB_COMM_END as a configuration.
01a,12dec03,elg  Written from 00vxWorks.cdf.

DESCRIPTION

This file contains descriptions for all of the WDB Agent components.
*/

Folder FOLDER_WDB {
	NAME		WDB agent components
	SYNOPSIS	software agent for Workbench tools connections
	CHILDREN	FOLDER_WDB_OPTIONS	\
			SELECT_WDB_COMM_TYPE	\
			SELECT_WDB_MODE		\
			SELECT_WDB_INIT
	DEFAULTS	FOLDER_WDB_OPTIONS	\
			SELECT_WDB_COMM_TYPE	\
			SELECT_WDB_MODE		\
			SELECT_WDB_INIT
	HELP		tgtsvr WDB
}

Folder FOLDER_WDB_OPTIONS {
	NAME		WDB agent services
	SYNOPSIS	optional services for the WDB target agent
	CHILDREN	INCLUDE_WDB		\
			INCLUDE_WDB_BANNER	\
			INCLUDE_WDB_BP		\
			INCLUDE_WDB_DPRINTF	\
			INCLUDE_WDB_BP_SYNC	\
			INCLUDE_WDB_CTXT	\
			INCLUDE_WDB_DIRECT_CALL \
			INCLUDE_WDB_ALWAYS_ENABLED \
			INCLUDE_WDB_EVENTPOINTS \
			INCLUDE_WDB_EVENTS	\
			INCLUDE_WDB_EXC_NOTIFY	\
			INCLUDE_WDB_EXIT_NOTIFY \
			INCLUDE_WDB_FUNC_CALL	\
			INCLUDE_WDB_GOPHER	\
			INCLUDE_WDB_MEM		\
			INCLUDE_WDB_REG		\
			INCLUDE_WDB_RTP		\
			INCLUDE_WDB_RTP_BP	\
			INCLUDE_WDB_RTP_CONTROL \
			INCLUDE_WDB_START_NOTIFY \
			INCLUDE_WDB_TASK_BP	\
			INCLUDE_WDB_TASK_HOOKS	\
			INCLUDE_WDB_TASK_REG	\
			INCLUDE_WDB_SSL		\
			INCLUDE_WDB_SYS_REG	\
			INCLUDE_WDB_TSFS	\
			INCLUDE_WDB_USER_EVENT	\
			INCLUDE_WDB_VIO		\
			INCLUDE_WDB_VIO_LIB
	DEFAULTS	INCLUDE_WDB		\
			INCLUDE_WDB_BANNER	\
			INCLUDE_WDB_BP		\
			INCLUDE_WDB_CTXT	\
			INCLUDE_WDB_DIRECT_CALL \
			INCLUDE_WDB_ALWAYS_ENABLED \
			INCLUDE_WDB_EVENTPOINTS \
			INCLUDE_WDB_EVENTS	\
			INCLUDE_WDB_EXC_NOTIFY	\
			INCLUDE_WDB_EXIT_NOTIFY \
			INCLUDE_WDB_FUNC_CALL	\
			INCLUDE_WDB_GOPHER	\
			INCLUDE_WDB_MEM		\
			INCLUDE_WDB_REG		\
			INCLUDE_WDB_START_NOTIFY \
			INCLUDE_WDB_USER_EVENT	\
			INCLUDE_WDB_VIO		\
			INCLUDE_WDB_VIO_LIB
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB {
	NAME		WDB agent
	SYNOPSIS	software agent to support the Workbench tools
	MODULES		wdbLib.o	\
			wdbSvcLib.o	\
			wdbCnctLib.o	\
			wdbMemCoreLib.o \
			wdbRpcLib.o	
	CONFIGLETTES	usrWdbCore.c
	INIT_RTN	wdbConfig ();
	REQUIRES	SELECT_WDB_COMM_TYPE		\
			SELECT_WDB_MODE			\
			SELECT_WDB_INIT			\
			SELECT_COMPILER_INTRINSICS	\
			INCLUDE_SYSDBG_FLAG
	CFG_PARAMS	WDB_COMM_PORT WDB_STACK_SIZE WDB_POOL_SIZE
	HDR_FILES	wdb/wdbLib.h wdb/wdbLibP.h private/funcBindP.h	\
			wdb/wdbUdpLib.h sysLib.h excLib.h string.h	\
			intLib.h rebootLib.h version.h adrSpaceLib.h	\
			taskHookLib.h vxLib.h private/taskLibP.h	\
			private/kernelLibP.h private/funcBindP.h	\
			wdb/wdb.h wdb/wdbBpLib.h wdb/wdbSvcLib.h	\
			wdb/wdbRtIfLib.h wdb/wdbCommIfLib.h		\
			wdb/wdbMbufLib.h wdb/wdbRpcLib.h cacheLib.h wait.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_SSL {
	NAME		Enable security for WDB
	SYNOPSIS	Enable network security for WDB Agent
	REQUIRES	INCLUDE_WDB \
			INCLUDE_WDB_ALWAYS_ENABLED_FOR_CONVENIENCE \
			INCLUDE_WDB_COMM_NETWORK \
			INCLUDE_WDB_PROXY_SSL \
			INCLUDE_WDB_PROXY_UDP
	EXCLUDES	INCLUDE_WDB_SYS
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_BANNER {
	NAME		WDB banner
	SYNOPSIS	print banner to console after the agent is initialized
	CONFIGLETTES	usrWdbBanner.c
	INIT_RTN	usrWdbBanner ();
	LINK_SYMS	printf
	HDR_FILES	version.h sysLib.h wdb/wdbLibP.h stdio.h
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_BP {
	NAME		WDB breakpoints
	SYNOPSIS	core breakpoint library
	CONFIGLETTES	usrWdbBp.c usrBreakpoint.c
	MODULES		wdbBpLib.o
	INIT_RTN	usrWdbBp ();
	CFG_PARAMS	WDB_BP_MAX
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	vxLib.h cacheLib.h wdb/wdb.h wdb/wdbLib.h \
			wdb/wdbLibP.h wdb/wdbSvcLib.h wdb/wdbBpLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_DPRINTF {
	NAME		WDB dynamic printf
	SYNOPSIS	dynamic printf support
	MODULES		wdbDynPrintfLib.o
	INIT_RTN	wdbDynPrintfLibInit (WDB_DPRINTF_MAX_MSGS, WDB_DPRINTF_MAX_MSG_LENGTH);
	CFG_PARAMS	WDB_DPRINTF_MAX_MSGS WDB_DPRINTF_MAX_MSG_LENGTH
	REQUIRES	INCLUDE_WDB_TASK_BP
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdb.h
	HELP		tgtsvr WDB
}
Component INCLUDE_WDB_BP_SYNC {
	NAME		Host/target breakpoint synchronization
	SYNOPSIS	Breakpoint synchronization component
	MODULES		wdbBpSyncLib.o
	INIT_RTN	wdbBpSyncLibInit ();
	MACRO_NEST	INCLUDE_WDB
#ifndef	_WRS_CONFIG_LP64
	INCLUDE_WHEN	INCLUDE_WDB_TASK_BP INCLUDE_DEBUG
#else	/* _WRS_CONFIG_LP64 */
	INCLUDE_WHEN	INCLUDE_WDB_TASK_BP
#endif	/* _WRS_CONFIG_LP64 */
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_CTXT {
	NAME		WDB tasks
	SYNOPSIS	create/delete/manipulate tasks
	MODULES		wdbCtxLib.o
	INIT_RTN	wdbCtxLibInit ();
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_DIRECT_CALL {
	NAME		WDB callouts
	SYNOPSIS	call arbitrary functions directly from WDB
	MODULES		wdbDirectCallLib.o
	INIT_RTN	wdbDirectCallLibInit ();
	MACRO_NEST	INCLUDE_WDB
	INCLUDE_WHEN	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_ALWAYS_ENABLED {
	NAME		WDB is always enabled
	SYNOPSIS	The WDB agent is always enabled whatever is the mode of the system debug flag
	INCLUDE_WHEN	INCLUDE_WDB_ALWAYS_ENABLED_FOR_CONVENIENCE
	HELP		tgtsvr WDB
}

/*
 * The INCLUDE_WDB_ALWAYS_ENABLED_FOR_CONVENIENCE component is used to create
 * a convenience dependency on INCLUDE_WDB_ALWAYS_ENABLED. This component
 * should be removed when INCLUDE_WHEN will support the OR syntax (currently
 * the behaviour is defined as AND only).
 */

Component INCLUDE_WDB_ALWAYS_ENABLED_FOR_CONVENIENCE {
	NAME		WDB is always enabled
	SYNOPSIS	The WDB agent is always enabled whatever is the mode of the system debug flag.
        _CHILDREN       FOLDER_NOT_VISIBLE
} 

Component INCLUDE_WDB_EVENTPOINTS {
	NAME		WDB eventpoints
	SYNOPSIS	support library for breakpoints and other asynchronous events.
	MODULES		wdbEvtptLib.o
	INIT_RTN	wdbEvtptLibInit ();
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_EVENTS {
	NAME		WDB events
	SYNOPSIS	asynchronous event handling needed for breakpoints etc.
	MODULES		wdbEvtLib.o 
	INIT_RTN	wdbEventLibInit ();
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_EXC_NOTIFY {
	NAME		WDB exception notification
	SYNOPSIS	notify the host when an exception occurs
	MODULES		wdbExcLib.o
	INIT_RTN	wdbExcLibInit ();
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_EDR_POLICY_HOOKS
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_EXIT_NOTIFY {
	NAME		WDB task exit notification
	SYNOPSIS	ability to notify the host when a task exits
	MODULES		wdbCtxExitLib.o
	INIT_RTN	wdbCtxExitLibInit ();
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB_TASK INCLUDE_WDB_TASK_HOOKS
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_FUNC_CALL {
	NAME		WDB call functions
	SYNOPSIS	asynchronous function calls
	MODULES		wdbCallLib.o
	INIT_RTN	wdbFuncCallLibInit ();
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB_TASK
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_GOPHER {
	NAME		WDB gopher
	SYNOPSIS	information gathering language used by many tools
	MODULES		wdbGopherLib.o
	CONFIGLETTES	usrWdbGopher.c
	INIT_RTN	usrWdbGopherInit ();
	MACRO_NEST	INCLUDE_WDB
	INCLUDE_WHEN	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	CFG_PARAMS	WDB_GOPHER_TAPE_LEN \
			WDB_GOPHER_TAPE_NB
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_MEM {
	NAME		WDB memory access
	SYNOPSIS	read/write target memory
	MODULES		wdbMemLib.o
	INIT_RTN	wdbMemLibInit ();
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_REG { 
	NAME		WDB register access
	SYNOPSIS	get/set registers
	MODULES		wdbRegLib.o
	INIT_RTN	wdbRegsLibInit ();
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_RTP {
	NAME		WDB RTP support
	SYNOPSIS	RTP operations: creation, deletion, ...
	MODULES		wdbRtpLib.o
	INIT_RTN	wdbRtpLibInit (WDB_RTP_PRIORITY);
	CFG_PARAMS	WDB_RTP_PRIORITY
	MACRO_NEST	INCLUDE_WDB
	INCLUDE_WHEN	INCLUDE_WDB_TASK INCLUDE_RTP
	REQUIRES	INCLUDE_RTP
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_RTP_BP {
	NAME		WDB RTP breakpoints
	SYNOPSIS	task-mode RTP breakpoint library
	MODULES		wdbRtpBpLib.o
	INIT_RTN	wdbRtpBpLibInit ();
	INCLUDE_WHEN	INCLUDE_WDB_RTP INCLUDE_WDB_TASK_BP
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	REQUIRES	INCLUDE_WDB_TASK INCLUDE_WDB_RTP
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_RTP_CONTROL {
	NAME		WDB RTP control support
	SYNOPSIS	Allow to stop real time processes when they are launched
	HDR_FILES	wdb/wdbRtpLibP.h
	INIT_RTN	wdbRtpCtrlLibInit ();
	MACRO_NEST	INCLUDE_WDB
	INCLUDE_WHEN	INCLUDE_WDB_RTP
	REQUIRES	INCLUDE_WDB_RTP
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_START_NOTIFY {
	NAME		WDB task creation
	SYNOPSIS	ability to notify the host when a task starts
	MODULES		wdbCtxStartLib.o
	INIT_RTN	wdbCtxStartLibInit ();
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB_TASK INCLUDE_WDB_TASK_HOOKS
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_TASK_BP {
	NAME		WDB task breakpoints
	SYNOPSIS	task-mode breakpoint library
	MODULES		wdbTaskBpLib.o
	INIT_RTN	wdbTaskBpLibInit ();
	INCLUDE_WHEN	INCLUDE_WDB_TASK INCLUDE_WDB_BP
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	REQUIRES	INCLUDE_WDB_TASK INCLUDE_WDB_BP
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_TASK_HOOKS {
	NAME		WDB task hooks
	SYNOPSIS	task hooks debug support
	INIT_RTN	wdbTaskHookInit ();
	INCLUDE_WHEN	INCLUDE_WDB_TASK
	REQUIRES	INCLUDE_WDB_TASK
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_TASK_REG {
	NAME		WDB task registers
	SYNOPSIS	registers support for task in WDB
	INIT_RTN	wdbTaskRegLibInit ();
	MODULES		wdbTaskRegLib.o
	INCLUDE_WHEN	INCLUDE_WDB_TASK INCLUDE_WDB_REG
	REQUIRES	INCLUDE_WDB_TASK INCLUDE_WDB_REG
	HELP		tgtsvr WDB
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_WDB_TASK_COPROC_REG {
	NAME		WDB task coprocessor registers
	SYNOPSIS	Coprocessor support for task in WDB
	INIT_RTN	wdbTaskCoprocRegLibInit ();
	MODULES		wdbTaskCoprocRegLib.o
	INCLUDE_WHEN	INCLUDE_WDB_TASK_REG INCLUDE_COPROCESSOR
	REQUIRES	INCLUDE_WDB_TASK_REG INCLUDE_COPROCESSOR
	HELP		tgtsvr WDB
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_WDB_SYS_REG {
	NAME		WDB system mode registers
	SYNOPSIS	Register support for system mode WDB
	INIT_RTN	wdbExternRegLibInit ();
	MODULES		wdbExternRegLib.o
	INCLUDE_WHEN	INCLUDE_WDB_SYS INCLUDE_WDB_REG
	REQUIRES	INCLUDE_WDB_SYS INCLUDE_WDB_REG
	HELP		tgtsvr WDB
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_WDB_SYS_COPROC_REG {
	NAME		WDB system mode coprocessor registers
	SYNOPSIS	Coprocessor register support for system mode WDB
	INIT_RTN	wdbExternCoprocRegLibInit ();
	MODULES		wdbExternCoprocRegLib.o
	INCLUDE_WHEN	INCLUDE_WDB_SYS_REG INCLUDE_COPROCESSOR
	REQUIRES	INCLUDE_WDB_SYS_REG INCLUDE_COPROCESSOR
	HELP		tgtsvr WDB
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_WDB_TSFS {
	NAME		WDB target server file system
	SYNOPSIS	virtual file system based on the WDB agent
	MODULES		wdbTsfsDrv.o
	INIT_RTN	wdbTsfsDrv ("/tgtsvr");
	HDR_FILES	private/wvUploadPathP.h private/wvTsfsUploadPathLibP.h \
			wdb/wdbVioLib.h
	INCLUDE_WHEN	INCLUDE_WVUPLOAD_TSFSSOCK
	REQUIRES	INCLUDE_WDB_TASK
	HELP		tgtsvr wdbTsfsDrvDrv WDB
}

Component INCLUDE_WDB_USER_EVENT {
	NAME		WDB user event
	SYNOPSIS	ability to send user events to the host
	MODULES		wdbUserEvtLib.o
	INIT_RTN	wdbUserEvtLibInit ();
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_VIO {
	NAME		VIO driver
	SYNOPSIS	vxWorks driver for accessing virtual I/O
	MODULES		wdbVioDrv.o
	INIT_RTN	wdbVioDrv ("/vio");
	HDR_FILES	drv/wdb/wdbVioDrv.h
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB_TASK
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_VIO_LIB {
	NAME		WDB virtual I/O library
	SYNOPSIS	low-level virtual I/O handling
	MODULES		wdbVioLib.o
	INIT_RTN	wdbVioLibInit ();
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLib.h wdb/wdbVioLib.h
	HELP		tgtsvr wdbVioDrv WDB
}

Selection SELECT_WDB_COMM_TYPE {
	NAME		WDB connection type
	SYNOPSIS	Select WDB connection type
	COUNT		1-1 
	CHILDREN	INCLUDE_WDB_COMM_SERIAL		\
			INCLUDE_WDB_COMM_TYCODRV_5_2	\
			INCLUDE_WDB_COMM_NETWORK	\
			INCLUDE_WDB_COMM_VTMD		\
			INCLUDE_WDB_COMM_END		\
			INCLUDE_WDB_COMM_PIPE		\
			INCLUDE_WDB_COMM_TIPC		\
			INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WDB_COMM_CUSTOM
	DEFAULTS	INCLUDE_WDB_COMM_END
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_COMM_SERIAL {
	NAME		WDB serial connection
	SYNOPSIS	serial line connection
	CONFIGLETTES	wdbSerial.c
	MODULES		wdbSlipPktDrv.o
	CFG_PARAMS	WDB_TTY_CHANNEL		\
			WDB_TTY_BAUD		\
			INCLUDE_WDB_TTY_TEST	\
			WDB_TTY_ECHO		\
			WDB_MTU
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB \
			INCLUDE_WDB_ALWAYS_ENABLED_FOR_CONVENIENCE
	HDR_FILES	private/funcBindP.h wdb/wdbLib.h wdb/wdbUdpLib.h \
			drv/wdb/wdbSlipPktDrv.h wdb/wdbLibP.h sysLib.h
	HELP		tgtsvr WDB
}

#ifndef	_WRS_CONFIG_LP64
Component INCLUDE_WDB_COMM_TYCODRV_5_2 {
	NAME		WDB tyCoDrv connection
	SYNOPSIS	connect over a serial line in a version 1.0 BSP
	CONFIGLETTES	wdbTyco.c
	MODULES		wdbTyCoDrv.o
	EXCLUDES	INCLUDE_WDB_SYS
	CFG_PARAMS	WDB_TTY_DEV_NAME	\
			WDB_TTY_BAUD		\
			INCLUDE_WDB_TTY_TEST	\
			WDB_MTU
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB
	HDR_FILES	wdb/wdbUdpLib.h drv/wdb/wdbSlipPktDrv.h \
			wdb/wdbLib.h wdb/wdbTyCoDrv.h wdb/wdbLibP.h \
			wdb/wdbCommIfLib.h
	HELP		tgtsvr WDB
}
#endif	/* _WRS_CONFIG_LP64 */

Component INCLUDE_WDB_COMM_NETWORK {
	NAME		WDB network connection
	SYNOPSIS	UDP/IP connection - only supports task mode debugging
	CONFIGLETTES	wdbNetwork.c
	MODULES		wdbUdpSockLib.o
	EXCLUDES	INCLUDE_WDB_SYS \
			INCLUDE_NET_INIT_SKIP
	CFG_PARAMS	WDB_MTU
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbUdpSockLib.h wdb/wdbLibP.h
	REQUIRES	INCLUDE_IPNET_LOOPBACK_CONFIG INCLUDE_WDB
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_COMM_END {
	NAME		WDB END driver connection
	SYNOPSIS	WDB END driver connection that supports both task and system mode debugging.
	CONFIGLETTES	wdbEnd.c
	MODULES		wdbEndPktDrv.o
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbUdpLib.h wdb/wdbLib.h \
			wdb/wdbLibP.h drv/wdb/wdbEndPktDrv.h \
			private/funcBindP.h end.h muxLib.h
	CFG_PARAMS	WDB_END_DEVICE_NAME	\
			WDB_END_DEVICE_UNIT	\
			WDB_END_DEVICE_ADDRESS	\
			WDB_MTU
	REQUIRES	INCLUDE_WDB INCLUDE_NET_BOOT
	EXCLUDES	INCLUDE_NET_INIT_SKIP
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_COMM_PIPE {
	NAME		WDB simulator pipe connection
	SYNOPSIS	default connection for the simulators
	CONFIGLETTES	wdbPipe.c
	MODULES		wdbPipePktDrv.o
	CFG_PARAMS	WDB_MTU
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB
	HDR_FILES	wdb/wdbUdpLib.h wdb/wdbLib.h wdb/wdbLibP.h \
			drv/wdb/wdbPipePktDrv.h
	HELP		tgtsvr WDB 
}

Component INCLUDE_WDB_COMM_TIPC {
	NAME		WDB TIPC connection
	SYNOPSIS	TIPC connection - only supports task mode debugging
	CONFIGLETTES	wdbTipc.c
	MODULES		wdbTipcSockLib.o
	EXCLUDES	INCLUDE_WDB_SYS
	CFG_PARAMS	WDB_MTU WDB_TIPC_PORT_TYPE WDB_TIPC_PORT_INSTANCE
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLibP.h wdb/wdbTipcSockLib.h wdb/wdbCommIfLib.h
	REQUIRES	INCLUDE_TIPC INCLUDE_WDB
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_COMM_MIPC {
	NAME		WDB MIPC connection
	SYNOPSIS	MIPC connection
	CONFIGLETTES	wdbMipc.c
	MODULES		wdbMipcSockLib.o
	EXCLUDES	INCLUDE_WDB_SYS
	CFG_PARAMS	WDB_MTU WDB_MIPC_BUS_NAME WDB_MIPC_PORT
	MACRO_NEST	INCLUDE_WDB
	HDR_FILES	wdb/wdbLibP.h wdb/wdbCommIfLib.h wdb/wdbMipcSockLib.h
	REQUIRES	INCLUDE_MIPC_SM INCLUDE_WDB
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_COMM_CUSTOM {
	NAME		WDB user-defined connection
	SYNOPSIS	callout to user-defined wdbCommDevInit
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB
	HELP		tgtsvr WDB
}

Selection SELECT_WDB_MODE {
	NAME		WDB mode
	SYNOPSIS	Select WDB mode: task mode, system mode, or both
	COUNT		1-2
	CHILDREN	INCLUDE_WDB_SYS		\
			INCLUDE_WDB_TASK
	DEFAULTS	INCLUDE_WDB_TASK
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_SYS {
	NAME		WDB system debugging
	SYNOPSIS	A breakpoint stops the entire operating system.
	CONFIGLETTES	usrWdbSys.c
	INIT_RTN	wdbSysModeInit ();
	CFG_PARAMS	WDB_SPAWN_PRI	\
			WDB_SPAWN_OPTS	\
			WDB_SPAWN_STACK_SIZE
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB
	HDR_FILES	wdb/wdb.h wdb/wdbLibP.h private/vxdbgCpuLibP.h \
			private/intLibP.h
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_TASK {
	NAME		WDB task debugging
	SYNOPSIS	A breakpoint stops one task, while others keep running.
	MODULES		wdbTaskLib.o
	INIT_RTN	usrWdbTaskModeInit (WDB_SPAWN_STACK_SIZE,	\
					    WDB_TASK_PRIORITY,	\
					    WDB_TASK_OPTIONS,	\
					    WDB_STACK_SIZE,	\
					    WDB_MAX_RESTARTS,	\
					    WDB_RESTART_TIME);
	CFG_PARAMS	WDB_TASK_PRIORITY	\
			WDB_TASK_OPTIONS	\
			WDB_RESTART_TIME	\
			WDB_MAX_RESTARTS	\
			WDB_SPAWN_STACK_SIZE
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB		\
			INCLUDE_TASK_RESTART	\
			INCLUDE_TASK_CREATE_HOOKS
	HDR_FILES	cacheLib.h excLib.h string.h sioLib.h \
			sysLib.h ioLib.h taskHookLib.h private/taskLibP.h \
			private/kernelLibP.h private/vmLibP.h \
			private/funcBindP.h wdb/wdb.h wdb/wdbLib.h \
			wdb/wdbLibP.h wdb/wdbBpLib.h \
			wdb/wdbSvcLib.h wdb/wdbRtIfLib.h \
			wdb/wdbCommIfLib.h wdb/wdbMbufLib.h \
			wdb/wdbRpcLib.h
	HELP	tgtsvr WDB
}

Component INCLUDE_WDB_DWARF_EXPR_TEST {
	NAME		WDB dwarf expressions evaluator test
	SYNOPSIS	Add support to test the WDB dwarf expressions evaluator. Enable the call to the test routine dwarfExprTest ().
	MODULES		dwarfExprTest.o
	LINK_SYMS	dwarfExprTest
	REQUIRES	INCLUDE_WDB
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Selection SELECT_WDB_INIT {
	NAME		WDB initialization mode
	SYNOPSIS	Select WDB initialization mode
	COUNT		1-1
	CHILDREN	INCLUDE_WDB_POST_KERNEL_INIT	\
			INCLUDE_WDB_PRE_KERNEL_INIT
	DEFAULTS	INCLUDE_WDB_POST_KERNEL_INIT
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_POST_KERNEL_INIT {
	NAME		WDB post kernel initialization
	SYNOPSIS	WDB agent is initialized when kernel has been initialized. It then can debug whole system and supports task and system mode.
	INIT_RTN	usrWdbInit (); \
			usrWdbInitDone ();
	PROTOTYPE	void usrWdbInit (void); \
			void usrWdbInitDone (void);
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB
	_INIT_ORDER	usrToolsInit
	INIT_BEFORE	usrWindviewInit
	INIT_AFTER	usrSymTblInit
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_PRE_KERNEL_INIT {
	NAME		WDB pre kernel initialization
	SYNOPSIS	WDB agent is initialized before kernel. It then can debug early kernel initialization.
	INIT_RTN	usrWdbInit ();		\
			usrWdbInitDone ();	\
			wdbSystemSuspend ();
	PROTOTYPE	void usrWdbInit (void);	\
			void usrWdbInitDone (void);
	MACRO_NEST	INCLUDE_WDB
	REQUIRES	INCLUDE_WDB	\
			INCLUDE_WDB_SYS
	EXCLUDES	INCLUDE_WDB_TASK		\
			INCLUDE_WDB_COMM_END		\
			INCLUDE_WDB_COMM_NETWORK	\
			INCLUDE_WDB_BANNER
	_INIT_ORDER	usrInit
	INIT_BEFORE	INCLUDE_KERNEL
	HELP		tgtsvr WDB
}

/*
 * The INCLUDE_WDB_KERNEL_INIT_DONE has been introduced in Vx 6.9.2.1 to mark
 * WDB as "Ready to receive data" once the WDB initialization is done. This
 * new component introduced a backward compatibility issue and has been
 * deprecated.  However, we must keep it to prevent another backward
 * compatibility issue with project created with Vx 6.9.2.1/6.9.2.2.
 */

Component INCLUDE_WDB_KERNEL_INIT_DONE {
	NAME		Useless component kept for backward compatibility
	SYNOPSIS	Useless component kept for backward compatibility
        _CHILDREN       FOLDER_NOT_VISIBLE
}

InitGroup usrWdbInit {
	INIT_RTN	usrWdbInit ();
	SYNOPSIS	the WDB target agent
	INIT_ORDER	INCLUDE_WDB		\
			INCLUDE_WDB_MEM		\
			INCLUDE_WDB_SYS		\
			INCLUDE_WDB_TASK	\
			INCLUDE_WDB_EVENTS	\
			INCLUDE_WDB_EVENTPOINTS \
			INCLUDE_WDB_RTP		\
			INCLUDE_WDB_DIRECT_CALL \
			INCLUDE_WDB_CTXT	\
			INCLUDE_WDB_REG		\
			INCLUDE_WDB_SYS_REG	\
			INCLUDE_WDB_SYS_COPROC_REG \
			INCLUDE_WDB_TASK_REG	\
			INCLUDE_WDB_TASK_COPROC_REG \
			INCLUDE_WDB_GOPHER	\
			INCLUDE_WDB_EXIT_NOTIFY \
			INCLUDE_WDB_EXC_NOTIFY	\
			INCLUDE_WDB_FUNC_CALL	\
			INCLUDE_WDB_VIO_LIB	\
			INCLUDE_WDB_VIO		\
			INCLUDE_WDB_BP		\
			INCLUDE_WDB_BP_SYNC	\
			INCLUDE_WDB_TASK_BP	\
			INCLUDE_WDB_DPRINTF	\
			INCLUDE_WDB_RTP_BP	\
			INCLUDE_WDB_RTP_CONTROL	\
			INCLUDE_WDB_START_NOTIFY \
			INCLUDE_WDB_USER_EVENT	\
			INCLUDE_WDB_MDL_SYM_SYNC \
			INCLUDE_WDB_BANNER
}

Parameter WDB_POOL_SIZE {
	NAME		WDB agent memory pool for host tool
	SYNOPSIS	WDB memory pool size (bytes)
	TYPE		uint
#ifndef _WRS_CONFIG_LP64
	DEFAULT		((unsigned int) (sysMemTop() - FREE_RAM_ADRS)/16)
#else	/* _WRS_CONFIG_LP64 */
	DEFAULT		0x1000
#endif	/* _WRS_CONFIG_LP64 */
}

Parameter WDB_STACK_SIZE {
	NAME		WDB task stack size
	SYNOPSIS	Defines stack size of WDB task (bytes)
	TYPE		uint
}

Parameter WDB_TASK_PRIORITY {
	NAME		WDB task priority
	SYNOPSIS	Defines priority of WDB task
	TYPE		uint
	DEFAULT		3
}

Parameter WDB_TASK_OPTIONS {
	NAME		WDB task options
	SYNOPSIS	This parameter defines options of WDB task. \
			Supported coprocessor options such as VX_FP_TASK are \
			automatically added by the coprocessor discovery \
			facility and do not need to be specified.
	DEFAULT		VX_UNBREAKABLE
}

Parameter WDB_RESTART_TIME {
	NAME		delay before error-restarting task agent
	SYNOPSIS	Delay before the task agent restarts after an error (seconds)
	TYPE		uint
	DEFAULT		10
}

Parameter WDB_MAX_RESTARTS {
	NAME		maximum # task agent restarts on error
	SYNOPSIS	maximum number of WDB task restarts on error
	TYPE		uint
	DEFAULT		5
}

Parameter WDB_SPAWN_PRI {
	NAME		default priority for spawned tasks
	SYNOPSIS	Default priority for tasks spawned from WDB agent
	TYPE		uint
	DEFAULT		100
}

Parameter WDB_SPAWN_OPTS {
	NAME		default options for spawned tasks
	SYNOPSIS	Default options for spawned tasks from WDB agent
	TYPE		uint
	DEFAULT		VX_COPROCS_ALL_TASK
}

Parameter WDB_SPAWN_STACK_SIZE {
	NAME		default stack size for spawned tasks
	SYNOPSIS	Default stack size for spawned tasks from WDB agent
	TYPE		uint
	DEFAULT		0x5000
}

Parameter WDB_COMM_PORT {
	NAME		default WDB UDP communication port
	SYNOPSIS	Defines the WDB UDP communication port number
	TYPE		uint
	DEFAULT		0x4321
}

Parameter WDB_BP_MAX {
	NAME		maximum # of system mode breakpoints
	SYNOPSIS	Defines the maximum number of system mode breakpoints
	TYPE		uint
	DEFAULT		25
}

Parameter INCLUDE_WDB_TTY_TEST {
	NAME		Enable WDB connection status display
	SYNOPSIS	Prints WDB READY when WDB agent is ready for connection
	TYPE		exists
	DEFAULT		TRUE
}

Parameter WDB_TTY_ECHO {
	NAME		Debug parameter to validate console driver
	SYNOPSIS	Enabling WDB_TTY_ECHO allows to validate the console configuration by echoing typed characters (must be disabled to allow target connection)
	TYPE		bool
	DEFAULT		FALSE
}

Parameter WDB_TTY_DEV_NAME {
	NAME		WDB tty device name
	SYNOPSIS	Defines WDB tty device name
	DEFAULT		/tyCo/1
}

Parameter WDB_TTY_BAUD {
	NAME		baud rate for WDB serial channel
	SYNOPSIS	Defines WDB serial baud rate
	TYPE		uint
	DEFAULT		9600
}

Parameter WDB_TTY_CHANNEL {
	NAME		serial channel for WDB connection
	SYNOPSIS	Defines the serial channel number to use for WDB connection
	TYPE		uint
	DEFAULT		1
}

Parameter WDB_MTU {
	NAME		MTU for WDB communication channel
	SYNOPSIS	Defines WDB communication MTU
	TYPE		uint
	DEFAULT		1500
}

Parameter WDB_END_DEVICE_NAME {
	NAME		device to use for WDB connection
	SYNOPSIS	Alternative device to use for WDB connection. If none specified, use boot device.
	DEFAULT		NULL 
	TYPE		string
}

Parameter WDB_END_DEVICE_UNIT {
	NAME		Device unit number to use for WDB connection
	SYNOPSIS	Device unit number to use for WDB connection. This is used only if a WDB END device name is specified.
	DEFAULT		0
	TYPE		uint
}

Parameter WDB_END_DEVICE_ADDRESS {
	NAME		IP address to use for WDB connection
	SYNOPSIS	Alternative IP address to use for WDB connection. If none specified, use boot address.
	DEFAULT		NULL 
	TYPE		string
}

Parameter WDB_RTP_PRIORITY {
	NAME		RTP initial task priority
	SYNOPSIS	default priority for RTP initial task
	DEFAULT		200
	TYPE		int
}

Parameter WDB_GOPHER_TAPE_LEN {
	NAME		WDB gopher tape length
	SYNOPSIS	Defines the maximal length of one gopher tape
	DEFAULT		1400
	TYPE		uint
}

Parameter WDB_GOPHER_TAPE_NB {
	NAME		WDB gopher tape number
	SYNOPSIS	Defines the maximal number of gopher tapes dynamically allocated
	DEFAULT		10
	TYPE		uint
}

Parameter WDB_TIPC_PORT_TYPE {
	NAME		default WDB TIPC communication port type
	SYNOPSIS	Defines the port type for WDB TIPC communication
	TYPE		uint8
	DEFAULT		70
}

Parameter WDB_TIPC_PORT_INSTANCE {
	NAME		default WDB TIPC communication port instance
	SYNOPSIS	Defines the port instance for WDB TIPC communication
	TYPE		uint8
	DEFAULT		71
}

Parameter WDB_MIPC_PORT {
	NAME		default WDB MIPC port
	SYNOPSIS	MIPC port used for WDB connection.
	TYPE		uint
	DEFAULT		2
}

Parameter WDB_MIPC_BUS_NAME {
	NAME		WDB MIPC bus name
	SYNOPSIS	Name of MIPC bus used for WDB connection.
	TYPE		string
	DEFAULT		"main" 
}

Parameter WDB_DPRINTF_MAX_MSGS {
	SYNOPSIS	Max WDB dynamic printf pending messages
	NAME		Maximum number of dynamic printf messages pending for output.
	TYPE		uint
	DEFAULT		100
}

Parameter WDB_DPRINTF_MAX_MSG_LENGTH {
	SYNOPSIS	Max WDB dynamic printf message length
	NAME		Maximum size of a dynamic printf message.
	TYPE		uint
	DEFAULT		100
}

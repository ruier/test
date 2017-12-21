/* 01vxdbg.cdf - VxDBG component description file */

/*
 * Copyright (c) 2007, 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */

/*
modification history
--------------------
01k,13aug12,cww  Updated INCLUDE_VXDBG_HOOK due to modified
                 INCLUDE_TASK_CREATE_HOOKS initialization
01j,16mar11,jmp  Performed some component naming/synopsis updates.
01i,12nov10,mze  _WRS_VX_SMP is now obsolete replaced with _WRS_CONFIG_SMP
01h,02jun10,rlp  Added a dependency on task create and switch hooks.
01g,16apr10,rlp  Added usrBreakpoint.c configlette to INCLUDE_VXDBG_RUNCTRL.
01f,13apr10,rlp  Added INCLUDE_VXDBG_RUNCTRL component.
01e,25feb10,rlp  Added VxDBG module component.
01d,18feb10,rlp  Moved INCLUDE_VXDBG initialization to usrToolsInit.
01c,10feb10,rlp  Added INCLUDE_VXDBG and INCLUDE_VXDBG_HOOK components.
01b,04oct07,mze  fix for 104829
01a,19jul07,jmp  written.
*/

/*
DESCRIPTION
This file contains descriptions for the VxDBG components.
*/

Component INCLUDE_VXDBG {
	NAME		VxWorks debug library
	SYNOPSIS	Support for the runtime debug library
	MODULES		vxdbgLib.o
	CONFIGLETTES	usrVxdbg.c
	INIT_RTN	usrVxdbgInit (VXDBG_EVT_TASK_PRIORITY,	\
					   VXDBG_EVT_TASK_OPTIONS,	\
					   VXDBG_EVT_TASK_STACK_SIZE);
        CFG_PARAMS      VXDBG_EVT_TASK_PRIORITY	\
			VXDBG_EVT_TASK_OPTIONS	\
			VXDBG_EVT_TASK_STACK_SIZE
	REQUIRES	INCLUDE_VXDBG_HOOK
}

Parameter VXDBG_EVT_TASK_STACK_SIZE {
        NAME            VxDBG task stack size
        SYNOPSIS        Defines stack size of VxDBG task (bytes)
        TYPE            uint
	DEFAULT		8192
}

Parameter VXDBG_EVT_TASK_PRIORITY {
        NAME            VxDBG task priority
	SYNOPSIS	Defines priority of VxDBG task
        TYPE            uint
        DEFAULT         25
}

Parameter VXDBG_EVT_TASK_OPTIONS {
        NAME            VxDBG events task options
	SYNOPSIS	Defines options of VxDBG task
        DEFAULT         VX_UNBREAKABLE | VX_FP_TASK
}

Component INCLUDE_VXDBG_RUNCTRL {
	NAME		VxWorks RunControl debug library
	SYNOPSIS	runControl support of the runtime debug library
	CONFIGLETTES    usrBreakpoint.c
	MODULES		vxdbgRunCtrlLib.o
	REQUIRES	INCLUDE_VXDBG \
			INCLUDE_TASK_SWITCH_HOOKS
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_VXDBG_HOOK {
	NAME		VxWorks debug library hooks
	SYNOPSIS	hooks for the runtime debug library
	INIT_RTN	vxdbgHooksInit ();
        _INIT_ORDER     usrKernelCoreInit
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_TASK_CREATE_HOOKS
}

Component INCLUDE_VXDBG_MODULE {
	NAME		VxWorks debug library module support
	SYNOPSIS	module management support for the runtime debug library
	MODULES		vxdbgModuleLib.o
	INCLUDE_WHEN	INCLUDE_VXDBG_RUNCTRL INCLUDE_UNLOADER
	_CHILDREN	FOLDER_NOT_VISIBLE
}

#ifdef _WRS_CONFIG_SMP
Component INCLUDE_VXDBG_CPU_CONTROL {
	NAME		VxDBG CPU control
	SYNOPSIS	CPU control support for VxDBG
	MODULES		vxdbgCpuLib.o
	INIT_RTN	vxdbgCpuLibInit ();
	REQUIRES	INCLUDE_SYSCLK_INIT \
			INCLUDE_VXIPI
	INCLUDE_WHEN	INCLUDE_CORE_DUMP \
			INCLUDE_WDB_SYS
	_CHILDREN	FOLDER_NOT_VISIBLE
}
#endif	/* _WRS_CONFIG_SMP */

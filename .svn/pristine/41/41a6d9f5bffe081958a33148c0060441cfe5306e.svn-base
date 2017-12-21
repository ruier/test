/* 10smp.cdf - symmetric multiprocessing (SMP) components */

/*
 * Copyright (c) 2007-2008,2010-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01q,02aug11,zl   added SMT scheduler component
01p,12nov10,mze  _WRS_VX_SMP is now obsolete replaced with _WRS_CONFIG_SMP
01o,31oct08,kk   rename INCLUDE_SMP_DEMO to INCLUDE_SMP_INIT in
                 INCLUDE_SMP_DEMO (CQ:WIND00141978)
01n,13oct08,zl   removed API_READY_QUEUE
01m,26aug08,jpb  Removed LAYER from components.
01l,27jun08,zl   made INCLUDE_READY_QUEUE_DELTA default
01k,25mar08,zl   changed INCLUDE_ENABLE_ALL_CPUS to INCLUDE_SMP_INIT.
01j,15mar08,zl   added configlette for delta ready queue.
01i,03mar08,zl   added ready queue API definitions.
01h,04oct07,mze  fix for 104829
01g,13sep07,pch  CQ102745: idle-task exception stack configurability
01f,13jul07,kk   clean up SMP components
01e,06jul07,tor  vxIpiLib dependency of SMP
01d,26apr07,pee  added source mode support
01c,11apr07,kk   fix INCLUDE_CPC component for UP source level build
01b,09mar07,pad  Removed the MODULES and LAYER attribute from INCLUDE_CPC since
		 this is conflicting with the UP build.
01a,05mar07,kk   written

*/

/*
DESCRIPTION
This file contains descriptions for the symmetric multiprocessing (SMP) 
components. All components in this file are only visible for projects created
with SMP support.

*/

#ifdef _WRS_CONFIG_SMP

Api API_SMP_SCHED_POLICIES {
	NAME 		SMP scheduler policy selector
	SYNOPSIS 	API selection for SMP scheduler policy
	_CHILDREN 	FOLDER_KERNEL
	DEFAULTS 	INCLUDE_SMP_SCHED_DEFAULT_POLICY
}

Component INCLUDE_SMP_SCHED_DEFAULT_POLICY {
	NAME 		Default SMP scheduler policy
	SYNOPSIS 	This is the default SMP scheduler policy. It's aim is \
			to run the highest priority ready tasks as soon as \
			possible by quicly propagating necessary task context \
			switches across CPUs.
	MODULES		readyQDeltaLib.o
	ARCHIVE 	librqdef.a
	PROVIDES 	API_SMP_SCHED_POLICIES
}

/*
 * Architectures that support symmetric multi-therading policy must override 
 * the MODULES definition without the leading underscore character.
 */

Component INCLUDE_SMP_SCHED_SMT_POLICY {
	NAME 		SMP scheduler policy with SMT support
	SYNOPSIS 	This is an extension to the default scheduling policy \
			to provide load balancing for processors with \
			Symmetric Multi-Threading support (such as \
			Hyper-Threading Technology).
	MODULES		_readyQDeltaSmtLib.o
	ARCHIVE 	librqsmt.a
	PROVIDES 	API_SMP_SCHED_POLICIES
}

Component INCLUDE_CPC {
	NAME		Cross-Processor Call (SMP Only)
	SYNOPSIS	CPUs Cross-Processor Call (SMP Only)
	SOURCE		$(TGT_DIR)/src/os
	MODULES		cpcLib.o
	INIT_RTN	cpcInit ();
	HDR_FILES	private/cpcLibP.h
	REQUIRES	INCLUDE_SYSCLK_INIT INCLUDE_VXIPI
	INCLUDE_WHEN	INCLUDE_KERNEL
	_CHILDREN	FOLDER_NOT_VISIBLE
}


Component INCLUDE_IDLE_TASKS {
	NAME		Idle Tasks Support (SMP Only)
	SYNOPSIS	Add Idle Tasks Support (SMP Only)
	INIT_RTN	kernelIdleTaskActivate();
	REQUIRES	INCLUDE_KERNEL
	INCLUDE_WHEN	INCLUDE_KERNEL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Parameter IDLE_TASK_EXCEPTION_STACK_SIZE {
	NAME		Size of the idle task's exception stack
	SYNOPSIS	The size of the stack used for reschedule and task switch hooks
	TYPE		uint
	DEFAULT		4096
	_CFG_PARAMS	INCLUDE_KERNEL
}

 /*
  * Definition is generic, but not necessarily available on all arch.  Those
  * which support it will specify the INIT_ORDER, and override _CHILDREN to
  * make it visible/selectable.
  */

Component INCLUDE_PROTECT_IDLE_TASK_STACK {
	NAME		enable guard pages on the idle task exception stack
	SYNOPSIS	Insert guard pages on the idle task exception stack, to protect against overflow or underflow while running task switch or swap hooks.
	_CHILDREN	FOLDER_NOT_VISIBLE
	CFG_PARAMS	IDLE_TASK_EXC_STACK_OVERFLOW_SIZE	\
			IDLE_TASK_EXC_STACK_UNDERFLOW_SIZE
	REQUIRES	INCLUDE_MMU_BASIC
	PROTOTYPE	void usrIdleTaskStkProtect (void);
	INIT_RTN	usrIdleTaskStkProtect ();
	CONFIGLETTES	usrIdleStacks.c
}

Parameter IDLE_TASK_EXC_STACK_OVERFLOW_SIZE {
	NAME		Idle task exception stack overflow protection size
	SYNOPSIS	Size (in bytes) of the protected area above the idle task exception stack.
	TYPE		uint
	DEFAULT		VM_PAGE_SIZE
}

Parameter IDLE_TASK_EXC_STACK_UNDERFLOW_SIZE {
	NAME		Idle task exception stack underflow protection size
	SYNOPSIS	Size (in bytes) of the protected area below the idle task exception stack.
	TYPE		uint
	DEFAULT		VM_PAGE_SIZE
}

/* 
 * ENABLE_ALL_CPUS configuration parameter is in 00vxWorks.cdf. 
 * The parameter is placed in 00vxWorks.cdf because it is of the 
 * INCLUDE_KERNEL component and no SMP components here should be
 * exposed.
 */

Component INCLUDE_SMP_INIT {
	NAME		Enable multi-processor capability of the kernel
	CONFIGLETTES	usrSmp.c
	INIT_RTN	usrSmpInit ();
	REQUIRES	API_SMP_SCHED_POLICIES
	_REQUIRES	INCLUDE_KERNEL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_SMP_DEMO {
	NAME		Symmetric Multiprocessing (SMP) Demos
	SYNOPSIS	Symmetric Multiprocessing (SMP) Demos
	LINK_SYMS	smpDemoModules
	REQUIRES	INCLUDE_CPC \
			INCLUDE_SMP_INIT \
			INCLUDE_IDLE_TASKS
	_CHILDREN	FOLDER_KERNEL
}
#endif	/* _WRS_CONFIG_SMP */

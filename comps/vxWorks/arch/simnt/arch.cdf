/* arch.cdf - VxWorks Simulators specific components & parameters */

/*
 * Copyright 2003-2008, 2010-2011 Wind River Systems, Inc. 
 *
 * The right to copy, distribute or otherwise make use of this software may be
 * licensed only pursuant to the terms of an applicable Wind River license
 * agreement.
 */

/*
modification history
--------------------
01v,14apr11,rlp  Removed previous checkin as it breaks the build of Vx image
                 based on user VSB.
01u,16feb11,v_r  Added INCLUDE_ICC_INTRINSICS to enhanced net, development and
                 compatible profiles (def CQ:WIND00253495).
01t,08dec10,dbt  Fixed INCLUDE_ADR_SPACE_LIB dependencies for VxSim
		 (CQ:WIND00245228).
01t,13nov10,mze  replace _WRS_VX_SMP with _WRS_CONFIG_SMP
01s,02feb10,elp  added address space parameter.
01r,04sep08,elp  added INCLUDE_WDB_COMM_MIPC
01q,26aug08,jpb  Removed LAYER from components.
01p,05oct07,mze  fix for 104829
01o,08jun07,elp  passfs cache always included with SMP,
		 validate interrupt stack protection on SIMNT.
01n,07mar07,jmg  Modified testing of simulators for INCLUDE_ATOMIC_OPERATORS
01m,01mar07,jmg  Added INCLUDE_ATOMIC_OPERATORS component
01l,19sep06,elp  Fixed typo CQ:WIND61721
01k,17jan06,bpn  Fixed system symbol table related components.
01j,30nov05,yvp  INCLUDE_SIGNAL_ARCH needs INCLUDE_ANSI_LONGJMP (SPR 115285)
01i,12sep05,yvp  Added INCLUDE_SIGNAL_ARCH.
01h,07sep05,h_k  changed HIDDEN to FOLDER_NOT_VISIBLE. (SPR #111769)
                 removed INCLUDE_BLIB_ARCH.
01g,10aug05,pee  added KERNEL_ARCH and BLIB_ARCH placeholder
01h,30may05,dbt   added support for TIPC WDB communication link
01h,31mar05,elp   added INCLUDE_PROTECT_INTERRUPT_STACK.
01g,22sep04,jmp   added INCLUDE_HOST_SIO component.
01f,14sep04,jmp   moved components from sim.cdf.
01e,30aug04,job   Renamed INCLUDE_LOADER to INCLUDER_LOADER_CODE (SPR#101151)
01d,06apr04,jmp   increased TASK_USER_EXC_STACK_SIZE parameter for VxSim.
01c,27oc,03,dbt	  Removed INCLUDE_KERNEL definition.
01b,03mar03,jmp   Switched to ELF loader, added modification history and
		  WindRiver headers.
01a,???????,cym   written.
*/

/*
DESCRIPTION
The Tornado Project Facility sources all component description files (CDF)
in a particular order.  The CDF files in these directories are processed in
the order in which they are presented: comps/vxWorks,
comps/vxWorks/arch/<arch>, config/<bsp>, and the project directory.  If
two CDF files describe the same component, the file read last overrides
all earlier files.  As a result, CDF files processed later by the Project
Facility are said to have a higher precedence than those processed
earlier.  Given such functionality, this file contains
architecture-specific VxWorks components and component definitions which
override "generic" components defined in comps/vxWorks.

If a new component is released, a new .cdf file should be deposited
in the appropriate directory, rather than modifying existing files.

For more information, see the .I "Tornado BSP Developer's Kit for VxWorks
User's Guide, Tornado 2.0: Components".
*/

/*
 * This will override  parts of INCLUDE_ATOMIC_OPERATORS the 
 * component in 00vxworks.cdf 
*/
#ifdef SIMSPARCSOLARIS
Component INCLUDE_ATOMIC_OPERATORS {
	MODULES		vxAtomicArchLib.o
	SOURCE 		$(TGT_DIR)/src/arch/simsolaris
}

#elif defined (SIMNT) 
Component INCLUDE_ATOMIC_OPERATORS {
	MODULES		vxAtomicALib.o
	SOURCE 		$(TGT_DIR)/src/arch/simnt
}

#elif defined (SIMLINUX) 
Component INCLUDE_ATOMIC_OPERATORS {
	MODULES		vxAtomicALib.o
	SOURCE 		$(TGT_DIR)/src/arch/simlinux
}
#endif /* SIMSPARCSOLARIS */

/*
 * VxWorks Simulators specific components & parameters
 */

Component INCLUDE_PASSFS {
        NAME            pass-through filesystem
        SYNOPSIS        direct access to host filesystem
        MODULES         passFsLib.o
        CONFIGLETTES    usrPassFs.c
#ifdef _WRS_CONFIG_SMP
        INIT_RTN        usrPassFsInit (TRUE);
#else
        INIT_RTN        usrPassFsInit (PASSFS_CACHE);
	CFG_PARAMS	PASSFS_CACHE
#endif /* _WRS_CONFIG_SMP */
	HDR_FILES	stdio.h ioLib.h
	_CHILDREN	FOLDER_IO_SYSTEM
	_INIT_ORDER	usrIosExtraInit
}

#ifndef _WRS_CONFIG_SMP
Parameter PASSFS_CACHE {
	NAME		pass-through filesystem cache
	SYNOPSIS	enable cache for pass-through filesystem
	DEFAULT		TRUE
	TYPE		BOOL
}
#endif /* _WRS_CONFIG_SMP */

Component INCLUDE_PASSFS_SYM_TBL {
	NAME		pass-through filesystem symbol table
	SYNOPSIS	prefered method for loading symbol table on VxSim.
	CONFIGLETTES	usrLoadSym.c
	INIT_RTN	usrLoadSyms ();
	HDR_FILES	loadLib.h string.h sysLib.h sysSymTbl.h symLib.h \
			stdio.h fcntl.h
	REQUIRES	INCLUDE_PASSFS \
			INCLUDE_LOADER \
			INCLUDE_BOOT_LINE_INIT \
			INCLUDE_SYM_TBL \
			INCLUDE_SYM_TBL_INIT
	HELP		loadLib symLib passFsLib
}

Component INCLUDE_VIRTUAL_DISK {
        NAME            virtualDisk
        SYNOPSIS        VxSim virtual disk to emulate a VxWorks disk driver.
        MODULES         virtualDiskLib.o
        INIT_RTN        virtualDiskInit ();
        HDR_FILES       stdio.h string.h
	_CHILDREN	FOLDER_IO_SYSTEM
	_INIT_ORDER	usrIosExtraInit
	HELP		virtualDiskLib
}

#ifdef	SIMSPARCSOLARIS
Component INCLUDE_DOS_DISK {
        NAME            dosDisk
        SYNOPSIS        vxSim DOS pass-through filesystem
        MODULES         unixDrv.o
        INIT_RTN        usrDosDiskInit ();
        CONFIGLETTES    usrDosDisk.c
        HDR_FILES       sysLib.h stdio.h string.h
	_CHILDREN	FOLDER_IO_SYSTEM
	_INIT_ORDER	usrIosExtraInit
}
#endif	/* SIMSPARCSOLARIS */

Component INCLUDE_HOST_SIO {
	NAME		VxSim Host serial device driver
	SYNOPSIS	Serial device driver for the host running VxSim
	_CHILDREN	FOLDER_PERIPHERALS
	CFG_PARAMS	HOST_SIO_PORT_NUMBER
}

Parameter HOST_SIO_PORT_NUMBER {
	NAME		Host serial device port number
#ifdef  SIMNT
	SYNOPSIS	Port number: 0 for COM1, 1 for COM2
#endif  /* SIMNT */
#ifdef	SIMLINUX
	SYNOPSIS	Port number: 0 for /dev/ttyS0, 1 for /dev/ttyS1
#endif	/* SIMLINUX */
#ifdef	SIMSPARCSOLARIS
	SYNOPSIS	Port number: 0 for /dev/term/a, 1 for /dev/term/b
#endif	/* SIMSPARCSOLARIS */
	TYPE		uint
	DEFAULT		0
}

Parameter NUM_TTY {
	SYNOPSIS	number of serial ports
	TYPE		uint
	DEFAULT		(INCLUDE_HOST_SIO)::(2) 1
}

/*
 * Overwrite common components here.
 */

/* Made PASSFS_SYM_TBL the default symbol table for VxWorks simulators */

Selection SELECT_SYM_TBL_INIT {
	CHILDREN	+= INCLUDE_PASSFS_SYM_TBL
	DEFAULTS	INCLUDE_PASSFS_SYM_TBL
}

/* Made WDB_COMM_PIPE the default WDB connection type for VxWorks simulators */

Selection SELECT_WDB_COMM_TYPE {
	CHILDREN	INCLUDE_WDB_COMM_NETWORK	\
			INCLUDE_WDB_COMM_PIPE		\
			INCLUDE_WDB_COMM_END		\
			INCLUDE_WDB_COMM_TIPC		\
			INCLUDE_WDB_COMM_MIPC		\
			INCLUDE_WDB_COMM_CUSTOM
	DEFAULTS	INCLUDE_WDB_COMM_PIPE
}

Component INCLUDE_LOADER_CODE {
        SYNOPSIS        ELF loader
        MODULES         loadLib.o loadElfLib.o
        INIT_RTN        loadElfInit ();
        HDR_FILES       loadElfLib.h
}

Component INCLUDE_PROTECT_INTERRUPT_STACK {
	_CHILDREN	FOLDER_KERNEL
	INCLUDE_WHEN	INCLUDE_KERNEL_HARDENING
	_INIT_ORDER	usrRoot
	INIT_BEFORE	INCLUDE_PROTECT_TASK_STACK
}

Component       INCLUDE_KERNEL_ARCH {
        NAME		kernel
        SYNOPSIS	architecture dependent protion of the kernel component
#ifdef _WRS_CONFIG_LP64
	REQUIRES	INCLUDE_ADR_SPACE_LIB
#endif
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_SIGNAL_ARCH {
	NAME		architecture dependent code for signals
	SYNOPSIS	architecture dependent code for signals
	MODULES		sigCtxALib.o sigCtxLib.o
	REQUIRES	INCLUDE_KERNEL
	INCLUDE_WHEN	INCLUDE_SIGNALS \
			INCLUDE_ANSI_LONGJMP
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Parameter INT_LOCK_LEVEL {
	NAME		INT lock level
	SYNOPSIS	Interrupt lock level
	DEFAULT		0x01
}

Parameter ROOT_STACK_SIZE {
	NAME		Root stack size
	SYNOPSIS	Root task stack size (bytes)
	DEFAULT		20000
}

Parameter SHELL_STACK_SIZE {
	NAME		Shell stack size
	SYNOPSIS	Shell stack size (bytes)
	DEFAULT		50000
}

Parameter WDB_STACK_SIZE {
	NAME		WDB Stack size
	SYNOPSIS	WDB Stack size (bytes)
	DEFAULT		0x2000
}

Parameter ISR_STACK_SIZE {
	NAME		ISR stack size
	SYNOPSIS	ISR Stack size (bytes)
	DEFAULT		50000
}

Parameter VEC_BASE_ADRS {
	NAME		Vector base address
	SYNOPSIS	Vector base address
	DEFAULT		0
}

Parameter ROM_WARM_ADRS {
	NAME		ROM warm boot address
	SYNOPSIS	ROM warm boot address
	DEFAULT		(0x0)
}

Parameter TASK_USER_EXC_STACK_SIZE {
	NAME		User task exception stack size
	SYNOPSIS	This defines the size (in bytes) of the exception stack for the user tasks
	TYPE		uint
	DEFAULT		(8192*3)
}

/*
 * Overwrite default KERNEL_COMMON_HEAP_INIT_SIZE value to fit default
 * simulator memory size
 */

Parameter KERNEL_COMMON_HEAP_INIT_SIZE {
	DEFAULT		0x1000000
}

/* add vxsim specific parameters */

Component INCLUDE_ADR_SPACE_LIB_ARCH {
	_CHILDREN	FOLDER_NOT_VISIBLE
	INCLUDE_WHEN	INCLUDE_ADR_SPACE_LIB
	_REQUIRES	INCLUDE_ADR_SPACE_LIB
#ifdef _WRS_CONFIG_LP64
	CFG_PARAMS	VXSIM_KERNEL_SYS_MEM_RGN_BASE	\
			VXSIM_KERNEL_SYS_MEM_RGN_SIZE	\
			VXSIM_RTP_PRIVATE_RGN_BASE
	MODULES		adrSpaceArchLib.o
#endif
	}

Component INCLUDE_ADR_SPACE_LIB {
#ifdef _WRS_CONFIG_LP64
	CFG_PARAMS	VXSIM_RTP_PRIVATE_RGN_SIZE	\
			VXSIM_SHARED_RGN_SIZE		\
			VXSIM_KERNEL_VIRT_POOL_RGN_SIZE	
#endif
}

Parameter VXSIM_KERNEL_SYS_MEM_RGN_BASE	{
	NAME		Kernel system virtual memory base address
	SYNOPSIS	This defines the base address of kernel system memory region
	TYPE		ULONG
	DEFAULT		0x180000000LL
}

Parameter VXSIM_KERNEL_SYS_MEM_RGN_SIZE {
	NAME		Kernel system virtual memory size
	SYNOPSIS	This defines the size of kernel system memory region
	TYPE		ULONG
	DEFAULT		0x80000000LL
}

Parameter VXSIM_RTP_PRIVATE_RGN_BASE {
	NAME		RTP private user virtual memory region size
	SYNOPSIS	This defines the base address of RTP user virtual memory region
	TYPE		ULONG
	DEFAULT		0x200000000LL
}

Parameter VXSIM_RTP_PRIVATE_RGN_SIZE {
	NAME		RTP private user virtual memory region size
	SYNOPSIS	This defines the size of RTP user virtual memory region
	TYPE		ULONG
	DEFAULT		0x100000000LL
}

Parameter VXSIM_SHARED_RGN_SIZE {
	NAME		Shared user virtual memory region size
	SYNOPSIS	This defines the size of shared user virtual memory region
	TYPE		ULONG
	DEFAULT		0x100000000LL
}

Parameter VXSIM_KERNEL_VIRT_POOL_RGN_SIZE {
	NAME		Kernel virtual pool size
	SYNOPSIS	This defines the size of kernel virtual pool
	TYPE		ULONG
	DEFAULT		0x100000000LL
}


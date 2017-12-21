/* 01loader.cdf - target loader component */

/* 
* Copyright (c) 2004-2007, 2009-2011, 2013 Wind River Systems, Inc. 
*
* The right to copy, distribute, modify or otherwise make use of this software
* may be licensed only pursuant to the terms of an applicable Wind River license
* agreement. 
*/ 

/*
modification history
--------------------
01t,08aug13,elp  added INCLUDE_SYM_TBL configlette (WIND00429784).
01s,01jul13,elp  added RW semaphore dependency (WIND00409243).
01r,16mar11,jmp  Performed some component naming/synopsis updates.
01q,15sep10,jmp  Removed init dependency between INCLUDE_SYM_TBL and
		 INCLUDE_ENV_VARS/INCLUDE_TASK_VARS components.
01p,30dec09,jpb  Remove INCLUDE_TASK_VARS from LP64 build.
01o,04oct07,mze  fix for 104829
01n,16aug07,pch  remove INCLUDE_TASK_VARS from SMP build
01m,16mar07,v_r  Added loader hooks init routine (req CQ:WIND00045277).
01l,25jun06,dlk  Make INCLUDE_NET_SYM_TBL not require INCLUDE_NET_INIT.
01k,17jan06,v_r  Added new loader hook facility component.
01j,11jan06,bpn  Added symbol table components.
01i,11aug05,pee  added dependency LOADER_CODE requires MODULE_MANAGER
01h,27jan05,dbt  Fixed INCLUDE_LOADER/INCLUDE_LOADER_CODE components.
01g,02oct04,bpn  Moved shell commands components to 01shell.cdf.
01f,29sep04,jn   Fix problem with symbol table config param 
01e,23sep04,jn   Add configuration parameter for loader
01d,18sep04,jn   Add initialization routine for unldLib
01c,31aug04,job  Fix problem with INCLUDE_LOADER_CODE in INIT_ORDER
01b,25aug04,job  Split INCLUDE_LOADER_CODE from INCLUDE_LOADER (SPR#101151)
01a,01jun04,bpn  Written from version 05v,12may04,dbt of 00vxWorks.cdf.


DESCRIPTION

This file contains the definition for the target loader and the symbol table components.
*/

Folder FOLDER_LOADER {
        NAME            loader components
	SYNOPSIS	VxWorks Kernel object module management
        CHILDREN        INCLUDE_UNLOADER \
                        INCLUDE_LOADER  \
			INCLUDE_LOADER_HOOKS \
                        INCLUDE_MODULE_MANAGER
        DEFAULTS        INCLUDE_LOADER
	_CHILDREN	FOLDER_TOOLS
}

Component INCLUDE_MODULE_MANAGER {
        NAME            module manager
        SYNOPSIS        Support library for module entities
        MODULES         moduleLib.o
        INIT_RTN        moduleLibInit ();
        HDR_FILES       moduleLib.h
}

/*
 * arch/<*>/arch.cdf contains the full definition for INCLUDE_LOADER_CODE.
 * It extends the skeletal definition provided here.
 */

Component INCLUDE_LOADER {
        NAME            target loader
	SYNOPSIS	Allows modules to be downloaded into running targets
        INIT_RTN        loadLibInit (STORE_ABS_SYMBOLS);
	CFG_PARAMS	STORE_ABS_SYMBOLS
        REQUIRES        INCLUDE_LOADER_CODE \
	                SELECT_SYM_TBL_INIT \
                        SELECT_COMPILER_INTRINSICS
}

Component INCLUDE_LOADER_CODE {
        NAME            target loader code
	SYNOPSIS	Architecture-independent loader internals
	MODULES         loadLib.o
        _CHILDREN       FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_LOADER
	INCLUDE_WHEN	INCLUDE_LOADER
}

Component INCLUDE_LOADER_HOOKS {
	NAME		target loader hook facility
	SYNOPSIS	Pluggable hooks to extend the loader/unloader behavior
	MODULES		moduleHookLib.o
	HDR_FILES	private/moduleHookLibP.h
	INIT_RTN	moduleHookLibInit ();
        _CHILDREN       FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_LOADER
}

Component INCLUDE_UNLOADER {
        NAME            target unloader
	SYNOPSIS	Permits removal of dynamically downloaded modules
        LINK_SYMS       unldByModuleId
        MODULES         unldLib.o
	HDR_FILES	unldLib.h
        INIT_RTN        unldLibInit ();
}

Component INCLUDE_SYM_TBL_SHOW {
	NAME		symbol table show routine
	SYNOPSIS	Routines to display information about symbols and symbol tables
	SOURCE		$(TGT_DIR)/src/ostool
	MODULES		symShow.o
	INIT_RTN	symShowInit ();
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit
	INIT_BEFORE	INCLUDE_VM_SHOW
	INCLUDE_WHEN	INCLUDE_SYM_TBL \
			INCLUDE_SHOW_ROUTINES
}

Folder FOLDER_SYMTBL {
	NAME		symbol table components
	SYNOPSIS	VxWorks Symbol table management
	_CHILDREN	FOLDER_TOOLS
	CHILDREN	INCLUDE_STAT_SYM_TBL	\
			INCLUDE_WDB_MDL_SYM_SYNC\
			INCLUDE_CPLUS_DEMANGLER	\
			INCLUDE_SYM_TBL \
			FOLDER_SYM_TBL_INIT
	DEFAULTS	INCLUDE_SYM_TBL \
			INCLUDE_SYM_TBL_INIT
}

Component INCLUDE_SYM_TBL {
	NAME		target symbol table
	SYNOPSIS	Set of routines to manipulate symbols and symbol tables
	SOURCE		$(TGT_DIR)/src/ostool
	CONFIGLETTES	usrSymTbl.c
	MODULES		symLib.o
	INIT_RTN	usrSymLibInit (); symLibInit ();
	HDR_FILES	symLib.h
	_INIT_ORDER	usrKernelExtraInit
	INIT_AFTER	INCLUDE_HASH
	REQUIRES	INCLUDE_SEM_READ_WRITE
}

Component INCLUDE_STAT_SYM_TBL {
	NAME		error status table
	SYNOPSIS	Table of error strings for perror()
	SOURCE		$(TGT_DIR)/src/usr
	CONFIGLETTES	usrStatTbl.c
	MODULES		statTbl.o
	INIT_RTN	usrStatTblInit ();
	CFG_PARAMS	STAT_TBL_HASH_SIZE_LOG2
	HDR_FILES	sysLib.h symLib.h stdio.h sysSymTbl.h
	HELP		errnoLib perror
}

Component INCLUDE_CPLUS_DEMANGLER {
	NAME		C++ symbol demangler
	SYNOPSIS	Support library for kernel shell and loader: provides human readable forms of C++ identifiers
	INIT_RTN	cplusDemanglerInit ();
	MODULES		cplusDem.o
	INCLUDE_WHEN	INCLUDE_CPLUS INCLUDE_SYM_TBL
}

Component INCLUDE_WDB_MDL_SYM_SYNC {
	NAME		host/target modules and symbols synchronization
	SYNOPSIS	Synchronize modules and symbols between the target server and a target
	MODULES		wdbMdlSymSyncLib.o
	INIT_RTN	wdbMdlSymSyncLibInit ();
	REQUIRES	INCLUDE_WDB \
			INCLUDE_LOADER \
			INCLUDE_SYM_TBL_INIT
	INCLUDE_WHEN	INCLUDE_WDB \
			INCLUDE_LOADER
	HDR_FILES	wdb/wdb.h
	HELP		loadLib symLib tgtsvr
}

Folder FOLDER_SYM_TBL_INIT {
	NAME		symbol table initialization components
	SYNOPSIS	Initialize symbol table
	CHILDREN 	INCLUDE_SYM_TBL_INIT \
			SELECT_SYM_TBL_INIT
	DEFAULTS	INCLUDE_SYM_TBL_INIT \
			SELECT_SYM_TBL_INIT
}

Component INCLUDE_SYM_TBL_INIT {
	NAME 		system symbol table initialization
	SYNOPSIS	initialize system symbol table
	CFG_PARAMS	SYM_TBL_HASH_SIZE_LOG2
	CONFIGLETTES	usrSysSymTbl.c
	INIT_RTN	usrSysSymTblInit ();
	REQUIRES	INCLUDE_SYM_TBL \
			SELECT_SYM_TBL_INIT
}

Selection SELECT_SYM_TBL_INIT {
	NAME		select symbol table initialization
	SYNOPSIS	Method for populating the system symbol table
	COUNT		1-1
	CHILDREN	INCLUDE_STANDALONE_SYM_TBL \
			INCLUDE_NET_SYM_TBL
	DEFAULTS	INCLUDE_STANDALONE_SYM_TBL
}

Component INCLUDE_STANDALONE_SYM_TBL {
	NAME		built-in symbol table
	SYNOPSIS	Preferred method if not booting from the network
	CONFIGLETTES	usrStandalone.c
	INIT_RTN	usrStandaloneInit ();
	HDR_FILES	sysLib.h symLib.h stdio.h sysSymTbl.h
	HELP		loadLib symLib
	REQUIRES	INCLUDE_SYM_TBL \
			INCLUDE_SYM_TBL_INIT
}

Component INCLUDE_NET_SYM_TBL {
	NAME		downloaded symbol table
	SYNOPSIS	Preferred method if booting from the network
	CONFIGLETTES	usrLoadSym.c
	INIT_RTN	usrLoadSyms ();
	HDR_FILES	loadLib.h string.h sysLib.h sysSymTbl.h symLib.h \
			stdio.h fcntl.h
	REQUIRES	INCLUDE_LOADER \
			INCLUDE_BOOT_LINE_INIT \
			INCLUDE_SYM_TBL \
			INCLUDE_SYM_TBL_INIT
	HELP		loadLib symLib
}

Parameter SYM_TBL_HASH_SIZE_LOG2 {
	NAME		symbol table hash size (as a power of 2)
	SYNOPSIS	The size of the symbol hash table is specified as a power of two. For example, if SYM_TBL_HASH_SIZE_LOG2 is 8, a 256-entry hash table is created.
	TYPE		uint
	DEFAULT		8
}

Parameter STAT_TBL_HASH_SIZE_LOG2 {
	NAME		error status table hash size (as a power of 2)
	SYNOPSIS	The size of the error status hash table is specified as a power of two. For example, if STAT_TBL_HASH_SIZE_LOG2 is 6, a 64-entry hash table is created.
	TYPE		uint
	DEFAULT		6
}

Parameter STORE_ABS_SYMBOLS {
	NAME		store absolute symbols
	SYNOPSIS	Whether to store absolute symbols in the system symbol table
	TYPE		BOOL
	DEFAULT		FALSE
}

InitGroup usrLoaderInit {
	INIT_RTN	usrLoaderInit ();
	SYNOPSIS	The target loader initialization sequence
	INIT_ORDER	INCLUDE_MODULE_MANAGER \
			INCLUDE_LOADER_CODE \
			INCLUDE_LOADER \
			INCLUDE_UNLOADER \
			INCLUDE_LOADER_HOOKS 
	_INIT_ORDER	usrToolsInit
	INIT_BEFORE	INCLUDE_TRIGGERING
}

InitGroup usrSymTblInit {
	INIT_RTN	usrSymTblInit ();
	SYNOPSIS	Enable onboard symbol tables
	_INIT_ORDER	usrToolsInit
	INIT_AFTER	usrLoaderInit
	INIT_BEFORE	INCLUDE_TRIGGERING
	INIT_ORDER	INCLUDE_SYM_TBL_INIT \
			INCLUDE_NET_SYM_TBL \
			INCLUDE_PASSFS_SYM_TBL \
			INCLUDE_STANDALONE_SYM_TBL \
			INCLUDE_STAT_SYM_TBL
}


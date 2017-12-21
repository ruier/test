/* 01vxTestV2.cdf - VxTestV2 driver components */

/*
 * Copyright (c) 2005-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01x,30oct09,dr   merging 64 bit changes
01w,29jun09,jc0  update testV2 driver for small footprint.
01v,18jun09,pgh  Fix the VxTestV2 selection. WIND00170523
01u,21apr09,d_c  Fix compiler warnings
01t,16apr09,d_c  Allow user to override small footprint character output
                 function
01s,13mar09,d_c  Small footprint testing support
01r,24feb09,d_c  Reorg in preparation for internal test module
                 componentization
01q,13oct08,d_c  Defect WIND00137001 Fix WIND00137358
                 Remove vxTestManualRun (INCLUDE_VXTESTV2_SHELLPROC).
01p,26aug08,jpb  Removed LAYER from components.
01o,18sep07,agf  Put BSPVTS tests into invisible folder
01n,03jul06,jmt  Add Parameters to INCLUDE_VXTESTV2_AUTORUN
01m,28jun06,jmt  Add INCLUDE_VXTESTV2_AUTORUN Component
01l,14jun06,d_c  Require INCLUDE_WDB in INCLUDE_VXTESTV2_WDB
                 Create new INCLUDE_VXTESTV2_SCALE select for scalability 
                 Remove unnecessary REQUIRES from INCLUDE_VXTESTV2_EMBEDDED.
01k,05jun06,jmt  Fix Shell Proc component for invalid requires
01j,30may06,jmt  Move some parameters to 01vxTestV2Test.cdf 
01i,27mar06,slk  SPR 119252: Add paramter for timestamp API support (strict)
01h,15feb06,jmt  Update cdf to remove syscall dependency
01g,17jan06,rlg  Add INCLUDE_TM_SYSCT parameters
01f,11jan06,jmt  Add NVRAM System Parameters
01e,03jan06,jmt  Fix dependency problems
01d,15nov05,jmt  Updated to allow for componentized test components
01c,15oct05,zl   added INCLUDE_VXTEST_POSIX_FS.
01b,28sep05,mf   Scalability Updates.
01a,17jun05,uo   Written.


DESCRIPTION

This file contains the definition for the components for vxTestV2 driver.
Do not add test module components or parameters to this file.

*/
Component           INCLUDE_VXTESTV2 {
    NAME            vxTestV2 driver component
    SYNOPSIS        This component defines the vxTestV2 driver 
    REQUIRES        SELECT_VXTESTV2_DRIVER
}

Selection           SELECT_VXTESTV2_DRIVER {
    NAME            vxTestV2 driver component selection
    SYNOPSIS        Selects from the available vxTestV2 driver types
	COUNT		    1-1
    CHILDREN        INCLUDE_VXTESTV2_LF_DRIVER \
                    INCLUDE_VXTESTV2_SF_DRIVER
    DEFAULTS        INCLUDE_VXTESTV2_LF_DRIVER
}

Component           INCLUDE_VXTESTV2_LF_DRIVER {
    NAME            vxTestV2 large footprint driver component
    SYNOPSIS        This component defines the vxTestV2 large footprint driver 
    CONFIGLETTES    vxtest/usrvxTestV2Cfg.c \
                    vxtest/usrvxTestV2Api.c 
    HDR_FILES       vxtest/vxTestV2Layered.h
    MODULES         vxTestV2Int.o vxTestV2Parser.o 
    SOURCE          $(WIND_BASE)/vxtest/src/drv 
    REQUIRES        SELECT_VXTESTV2_MODE
/*  INIT_RTN        vxTestV2LayeredInit (); */
}	

Component           INCLUDE_VXTESTV2_SF_DRIVER {
    NAME            vxTestV2 small footprint driver component
    SYNOPSIS        This component defines the vxTestV2 small footprint driver 
    CONFIGLETTES    vxtest/usrvxTestV2Sf.c \
                    vxtest/usrvxTestV2Api.c
    CFG_PARAMS      VXTESTV2_SF_DRIVER_PUTCHAR \
                    VXTESTV2_SF_DRIVER_PUTCHAR_IS_RAW
    MODULES         vxTestV2Sf.o
    INIT_RTN        usrvxTestV2SfRun (VXTESTV2_SF_DRIVER_PUTCHAR,\
                                      VXTESTV2_SF_DRIVER_PUTCHAR_IS_RAW);
    _INIT_ORDER     usrRoot
}	

Component           INCLUDE_VXTESTV2_SF_DRIVER_CALLS {
    NAME            Enable inclusion of sfDriverCalls.c
    SYNOPSIS        Enable inclusion of sfDriverCalls.c in usrvxTestV2Sf.c \
                    configlette. sfDriverCalls.c must exist in project dir.
}

Component		INCLUDE_VXTESTV2_WDB {
	NAME		vxTestV2 driver WDB mode component
	SYNOPSIS	This component selects the vxTestV2 WDB mode
	REQUIRES	INCLUDE_WDB \
                        INCLUDE_UNLOADER \
			INCLUDE_DISK_UTIL \
			INCLUDE_VXTESTV2_LF_DRIVER \
                        INCLUDE_SIGNALS \
			INCLUDE_MEM_SHOW
}

Component		INCLUDE_VXTESTV2_SHELL {
	NAME		vxTestV2 driver normal mode component
	SYNOPSIS	This component selects the vxTestV2 normal mode
	REQUIRES	INCLUDE_SHELL \
			INCLUDE_UNLOADER \
			INCLUDE_DISK_UTIL \
			INCLUDE_VXTESTV2_LF_DRIVER \
                        INCLUDE_SIGNALS \
			INCLUDE_MEM_SHOW
}

Component		INCLUDE_VXTESTV2_SCALE   	{
	NAME		vxTestV2 driver Scalable image mode
	SYNOPSIS	This component works with scalable images
	REQUIRES	INCLUDE_VXTESTV2_LF_DRIVER \
			INCLUDE_ANSI_STDLIB_NUMBERS \
			INCLUDE_VXTESTV2_EMBEDDED \
                        INCLUDE_VXTESTV2_AUTORUN
}

Component		INCLUDE_VXTESTV2_EMBEDDED 	{
	NAME		vxTestV2 driver embedded itc component
	SYNOPSIS	This component works with embedded itc files
}

Component		INCLUDE_VXTESTV2_AUTORUN  	{
	NAME		vxTestV2 driver Autorun component
	SYNOPSIS	This component automatically starts the tests
	REQUIRES	INCLUDE_VXTESTV2_LF_DRIVER \
			INCLUDE_VXTESTV2_EMBEDDED 
        INIT_RTN        usrvxTestV2AutoRun ();
	CFG_PARAMS	VXTESTV2_AUTORUN_TASK_PRIORITY \
                        VXTESTV2_AUTORUN_TASK_OPTIONS \
                        VXTESTV2_AUTORUN_TASK_STACK_SIZE
        CONFIGLETTES    vxtest/usrvxTestV2AutoRun.c
        _INIT_ORDER     usrRoot
	INIT_AFTER	INCLUDE_RTP_APPL_USER
}

Selection		SELECT_VXTESTV2_MODE	{
	NAME		select vxTestV2 mode
	COUNT		1-1
	CHILDREN	INCLUDE_VXTESTV2_SHELL \
			INCLUDE_VXTESTV2_SCALE \
			INCLUDE_VXTESTV2_WDB
	DEFAULTS	INCLUDE_VXTESTV2_SHELL
}
	
Component		INCLUDE_VXTESTV2_SYSPARAMS {
	NAME		vxTestV2 system parameters component
	SYNOPSIS	This component supplies System Params to vxTestV2 tests
	CONFIGLETTES	vxtest/usrvxTestSysParams.c
	CFG_PARAMS      SYS_MODEL BSP_VERSION BSP_REV \
			AUX_CLK_RATE AUX_CLK_RATE_MAX AUX_CLK_RATE_MIN \
			SYS_CLK_RATE SYS_CLK_RATE_MAX SYS_CLK_RATE_MIN \
			NV_RAM_SIZE NV_BOOT_OFFSET \
                        DCACHE_NAME ICACHE_NAME TIMESTAMP_API_STRICT
}	

Component		INCLUDE_VXTESTV2_POSIX_FS	{
	NAME		POSIX FS component
	SYNOPSIS	mount a POSIX conformant file system
	REQUIRES	INCLUDE_RAM_DISK \
			INCLUDE_HRFS_FORMAT \
			SELECT_VXTESTV2_MODE
	CFG_PARAMS	VXTESTV2_POSIX_FS_SIZE \
			VXTESTV2_POSIX_FS_NAME \
			VXTESTV2_POSIX_FS_FILES
}	

Folder			FOLDER_VXTESTV2_TESTS		{
	NAME		vxTestV2 test module components
	SYNOPSIS	Used to group vxTestV2 test module components 
}	

Folder			FOLDER_VXTESTV2		{
	NAME		vxTestV2 components
	SYNOPSIS	Used to group vxTestV2 components and the selection
	CHILDREN	INCLUDE_VXTESTV2 \
			INCLUDE_VXTESTV2_SYSPARAMS \
			SELECT_VXTESTV2_MODE \
			FOLDER_VXTESTV2_TESTS \
			INCLUDE_VXTESTV2_POSIX_FS \
			INCLUDE_VXTESTV2_EMBEDDED \
			INCLUDE_VXTESTV2_AUTORUN
	DEFAULTS	INCLUDE_VXTESTV2 \
			SELECT_VXTESTV2_MODE 
	_CHILDREN	FOLDER_NOT_VISIBLE
}

InitGroup 		usrVxTestV2Init {
	INIT_RTN	usrVxTestV2Init ();
	SYNOPSIS	vxTestV2 initialization sequence
	INIT_ORDER	INCLUDE_VXTESTV2 
	_INIT_ORDER	usrToolsInit
}


/* System Parameters */

/* Specify character output function for small footprint driver */

Parameter VXTESTV2_SF_DRIVER_PUTCHAR {
	NAME		SF driver putchar function pointer
	DEFAULT		sfPutChar
}

/* By default it is assumed that the putchar routine provided "cooks"
 * the output, meaning that '\n' translates to '\r\n'. Override this
 * to 1 if you provide a raw character output function
 */

Parameter VXTESTV2_SF_DRIVER_PUTCHAR_IS_RAW {
	NAME		Flag indicating that SF driver putchar is raw
	TYPE		int
	DEFAULT		0
}

Parameter VXTESTV2_POSIX_FS_SIZE {
	NAME		Size of vxTestV2 POSIX test FS
	TYPE		int
	DEFAULT		0x80000
}

Parameter VXTESTV2_POSIX_FS_NAME {
        NAME		Name of vxTestV2 POSIX test FS
        TYPE		string
        DEFAULT		"/pxTestFs"
}

Parameter VXTESTV2_POSIX_FS_FILES {
	NAME		Maximum number of files in vxTestV2 POSIX test FS
	TYPE		int
	DEFAULT		1000
}

Parameter VXTESTV2_AUTORUN_TASK_PRIORITY {
	NAME		Priority of vxTestV2 AutoRun task
	TYPE		int
	DEFAULT		100
}

Parameter VXTESTV2_AUTORUN_TASK_OPTIONS {
	NAME		Options for the vxTestV2 AutoRun task
	TYPE		int
	DEFAULT		0
}

Parameter VXTESTV2_AUTORUN_TASK_STACK_SIZE {
	NAME		Stack Size for the vxTestV2 AutoRun task
	TYPE		int
	DEFAULT		20000
}

/* NOTE: The following defaults for BSP_VERSION and BSP_REV are coupled to the
 * tmSysctl:sysctlHxRev test case. It is used to detect that a BSP has
 * not defined the version and revision values
 */
Parameter       BSP_VERSION {
        DEFAULT         "BSP_VERSION not defined"
}
Parameter       BSP_REV {
        DEFAULT         "BSP_REV not defined"
}
/* NOTE: The following defaults for DCACHE_NAME and ICACHE_NAME are coupled
 * to the tmSysctl:sysctlCache test case. These parameters are defined by
 * convention. The convention is defined by the cacheAimInfo data structure
 */
Parameter       DCACHE_NAME {
        DEFAULT         "dcache"
}
Parameter       ICACHE_NAME {
        DEFAULT         "icache"
}


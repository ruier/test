/* usrvxTestV2Cfg.c - vxTestV2 configlette */

/* Copyright (c) 2005-2010 Wind River Systems, Inc. 
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/
/*
modification history
--------------------
02e,22dec10,dr   fixing WIND00225007: _pex_msgQOpen and _pex_msgQClose should
                 be NULL if _WRS_CONFIG_OBJECT_CLASS_LIST or 
                 _WRS_CONFIG_OBJECT_MANAGEMENT is undefined.
02d,30oct09,dr   merging 64 bit changes from iter 15
02c,25jun09,ggz  Reinstated mods for userDB. INCLUDE_SECURITY does not use 
                 symbol table lib.    
02b,01jun09,v_r  Reverted IPNET team mods until unified user DB HLD is properly
		 reviewed.
02a,21apr09,ggz  INCLUDE_SECURITY does not use symbol table lib.
01z,11feb09,hbh  Fixed typo in _WRS_CONFIG_OBJECT_CLASS_LIST (WIND00144295)
01y,22oct08,d_c  Defect WIND00140899 fix WIND00140903
                 Get rid of outdated error message
01x,07oct08,dr   Prevent _pex symbols from being incorrectly initialized when
                 _WRS_CONFIG_RTP is defined but INCLUDE_RTP is NOT.
01w,06oct08,dr   Fixing VSB configuration
01v,02oct08,dr   Fixing VSB configuration
01u,29sep08,dr   adjusting to VSB configurations
01t,20aug08,d_w  Added a check for an empty/default CPU_VARIANT string 
01s,23jul08,dr   Remove _WRS_LAYER_ references
01r,29may08,d_c  Add init code for microsecond delay function
01q,18jul07,jmt  Fix problem with multiple _pex_wdbUserEvtPost defines
01p,26jun07,dcc  changed itcFileName declaration to a char array.
01o,29nov06,jk_  add _pex_taskCreate
01n,26oct06,jk_  make vxTestMsg() write to driver stdout
01m,20jul06,mfj  Adding vxTestCpuGet and vxTestCpuSpecificGet
01l,13jul06,mfj  cleanup
01k,14jun06,d_c  Base init of _pex_wdbUserEvtPost on INCLUDE_WDB
01i,09jun06,sbk  fix in memory leaks calculation, WIND00041317
01h,21feb06,jmt  Fix unnecessary syscall
01g,09jan06,mfj  Adding vxTestCpuGet and vxTestVariantCpuGet
01f,05dec05,zl   added vxTestV2PosixFsInit()
01e,28sep05,mfj  Updating comments
01d,22sep05,mfj  updating for MKL
01c,17aug05,uo   checking components before using ext symbols
01b,16aug05,uo   checking Rtp component before using Rtp functions
01a,17jun05,uo   written.
*/

/*
  DESCRIPTION
  This module maps called system functions to the current OS layer's provided 
  functions and emulated functions when there is no function supported by 
  that layer.

  INCLUDE FILES: 	vxTestV2Layered.h 
  
  SEE ALSO:

*/

/* includes */
#include <vxWorks.h>
#include <xbdRamDisk.h>
#include <hrFsLib.h>
#include "vxTestV2Layered.h"
#include <vsbConfig.h>

#ifndef PRJ_BUILD
#include "config.h"
#endif /* PRJ_BUILD */

#ifdef INCLUDE_VXBUS
#include <vxbTimerLib.h>
#endif

#ifndef PRJ_BUILD

#ifdef INCLUDE_VXTESTV2_POSIX_FS

/* provide default configuration parameter values for BSP builds */

#ifndef VXTESTV2_POSIX_FS_SIZE
#define VXTESTV2_POSIX_FS_SIZE		0x80000
#endif

#ifndef VXTESTV2_POSIX_FS_NAME
#define VXTESTV2_POSIX_FS_NAME		"/pxTestFs"
#endif

#ifndef VXTESTV2_POSIX_FS_FILES
#define VXTESTV2_POSIX_FS_FILES		1000
#endif

#endif /* PRJ_BUILD */

/* Copied from usrDepend.c */

#if defined(INCLUDE_NET_SYM_TBL) || defined(INCLUDE_STANDALONE_SYM_TBL) ||\
	defined(INCLUDE_PASSFS_SYM_TBL)
#define INCLUDE_SYM_TBL     /* include symbol table package */
#endif  /* INCLUDE_NET_SYM_TBL||INCLUDE_STANDALONE_SYM_TBL||
           INCLUDE_PASSFS_SYM_TBL */
#endif /* ifndef PRJ_BUILD */

/* defines */

#undef _USR_KERNEL_

/* #define _USR_KERNEL_ 	_OSL_ */ 
/* TODO: make CDF parameter for _USR_KERNEL_ */

/*
 * WRS_XXX is the system-wide macro that defines the current layer.
 * USR_XXX is the user-specified macro that defines which layer the 
 * driver should run in. USR_XXX has the precedence over WRS_XXX.
 * Depending on the dominant macro compatibility macros are defined. 
 * When a layer is defined, all the lower layers become compatible 
 * by compatibility macros. Later on these internal macros are used 
 * to initialize each external function pointer. 
 *
 * if USR_XXX then
 *    #define DRV_XXX
 * elif WRS_YYY then
 *    #define DRV_YYY ----> define all layers below YYY as compatible
 * endif
 *
 *
 * if XXX_COMPATIBLE then 
 *		_pex_f <- f
 * else
 *		_pex_f <- emul_f
 *
 */

#if _WRS_CONFIG_LP64
#define DRV_MKL
#define MKL_COMPATIBLE
#else
#define DRV_OSL
#define OSL_COMPATIBLE
#define KSL_COMPATIBLE
#define BKL_COMPATIBLE
#define MKL_COMPATIBLE
#endif /* _WRS_CONFIG_LP64 */

/* To turn a macro into a string */
#define V2_STRING_HELP(token) (#token)
#define V2_STRING(token) V2_STRING_HELP(token)

/* typedefs */

/* external symbols */
IMPORT SYMTAB_ID   sysSymTbl; 


/* globals */
/* vxTestV2 Driver Globals */
IMPORT char itcFileName [];
IMPORT char * _pex_itcFileName;
IMPORT void real_initDataModule (void);
IMPORT void (*_pex_initDataModule) (void);
IMPORT int driverLayer;
IMPORT int driverComponent;
IMPORT void vxTestUsSwDelayInit (void);
IMPORT void vxTestUsSwDelay (int uSecs);
IMPORT void (*vxTestUsDelayFcn)(int uSecs);

#define DEFAULT_CPU_VARIANT "CPU_VARIANT"
#define EMPTY_STRING ""

/* forward declarations */

/***************************************************************************
 * vxTestCpuVariantGet - return the CPU variant
 *
 * Checks to see if variant is same a default - if it is the same, 
 * returns an empty string
 *
 * RETURNS: A char * pointing to the CPU variant string
 *
 */

char * vxTestCpuVariantGet
    (
    void
    ) 
    {
    char * cpuVariant = V2_STRING(CPU_VARIANT);

    if (!strcmp (cpuVariant,DEFAULT_CPU_VARIANT))
      cpuVariant = EMPTY_STRING;
    
    return cpuVariant;
    }

/***************************************************************************
 * vxTestToolGet - return the tool used to build the image
 * 
 * RETURNS: A char * pointing at the tool string
 */

char * vxTestToolGet
    (
    void
    ) 
    {
    char * tool = V2_STRING(TOOL);
    
    return tool;
    }

/***************************************************************************
 * vxTestLinkPathGet - return the linkPath used to build the image
 * 
 * RETURNS: A char * pointing at the linkage path
 *
 * Note: Doesn't work. LD_LINK_PATH is a variable in the makefile, but it is
 * not placed on the compiler command line, and is thus not available here.
 */

/*
char * vxTestLinkPathGet
    (
    void
    ) 
    {
    char * linkPath = V2_STRING(LD_LINK_PATH);
    
    return linkPath;
    }
*/

/***************************************************************************
 * vxTestModuleGenericPathGet - get the path to load modules from
 *
 * This method returns the path by which the driver can load test modules.
 * 
 * Much of the path gets set while building the project or bsp, which is 
 * when this function gets its values set.
 * 
 * CURRENTLY NOT IN USE! - still using old-style loading mechanism.
 */

/*
char * vxTestModuleGenericPathGet
    (
    void
    ) 
    {
#ifndef V2CPU_FAM
#error "V2CPU_FAM missing in C preprocessor, run embedITC.tcl!"
#endif

    char * basePath = "target/lib/" V2CPU_FAM "/" 
        V2CPU "/common/objvxTestV2/";

    return basePath;
    }
*/

/***************************************************************************
 *
 * vxTestV2LayeredInit - assign values to function pointers 
 *
 * This routine is the only interface to the vxTestV2Layered module. 
 * Depending on the current layer, it assigns global function pointers
 * to the current layer's function addresses or emulated functions' addresses.
 * For the functions that are not supported by the current layer, an emulation
 * of that function will be provided and corresponding function pointer will 
 * be assigned to that emulation function address.
 *
 * RETURNS: OK, or ERROR if the current layer is invalid.
 *           
 * ERRNO:
 *
 */

STATUS vxTestV2LayeredInit
    (
    void
    )
    {
    
    /* 
     * SETUP FUNCTION POINTERS ACCORDING TO DRV_<LAYER>'s compatibilities 
     */

    printf("Initializing vxTestV2: ");

/*----------------------------------------------------------------------*/


    _pex_close			=	NULL;
    _pex_dup	    	    	=   	NULL;
    _pex_envGet			=       NULL;
    _pex_fclose			=	NULL;
    _pex_fdopen     	    	=   	NULL;
    _pex_fflush     	    	=   	NULL;
    _pex_fgets			=	NULL;
    _pex_fopen			=	NULL;
    _pex_fprintf		=	NULL;
    _pex_free			=	NULL;
    _pex_getenv			=       NULL;
    _pex_initDataModule		= 	NULL;
    _pex_ioTaskStdGet           =       NULL;
    _pex_ioTaskStdSet         	=       NULL;
    _pex_itcFileName		= 	NULL;
    _pex_loadModule		=	NULL;
    _pex_malloc			=	NULL;
    _pex_memPartInfoGet		=	NULL;
    _pex_memSysPartId		=	NULL;
    _pex_msgQClose		=       NULL;
    _pex_msgQCreate		=	NULL;
    _pex_msgQDelete		=	NULL;
    _pex_msgQOpen		=	NULL;
    _pex_msgQReceive            =       NULL;
    _pex_msgQSend               =       NULL;
    _pex_open			=       NULL;
    _pex_printf			=       NULL;
    _pex_putenv			=       NULL;
    _pex_realloc		=	NULL;
    _pex_rtpDelete		=	NULL;
    _pex_rtpSpawn		=	NULL;
    _pex_scMemValidate		=	NULL;
    _pex_semBCreate		=	NULL;
    _pex_signal                 =       NULL;
    _pex_sprintf	      	=       NULL;
    _pex_stdout     	    	=   	NULL;
    _pex_strerror_r             =       NULL;
    _pex_strerror               =       NULL;
    _pex_strncat                =       NULL;
    _pex_symByCNameFind		= 	NULL;
    _pex_symFindByName		=	NULL;
    _pex_syscallGroupRegister	=	NULL;
    _pex_sysClkRateGet		=	NULL;
    _pex_sysSymTbl		=	NULL;
    _pex_taskCreate		=	NULL;
    _pex_taskDelete		=	NULL;
    _pex_taskIsSuspended        =       NULL;
    _pex_taskName             	=       NULL;
    _pex_taskNameToId       	=       NULL;
    _pex_taskSpawn		=	NULL;
    _pex_unldByModuleId		=	NULL;
    _pex_vfprintf	        =       NULL;
    _pex_wdbUserEvtPost		=	NULL; 
    _pex_wdCreate		=	NULL;
    _pex_wdDelete		=	NULL;
    _pex_write			=       NULL;

   /* selectively intialize the function pointers based on VSB configuration */
    
    _pex_fclose			=	fclose;
    _pex_fgets			=	fgets;
    _pex_fopen			=	fopen;
    _pex_fprintf		=	fprintf;

#if defined( _WRS_CONFIG_OBJECT_CLASS_LIST) && defined (_WRS_CONFIG_OBJECT_MANAGEMENT)
    _pex_msgQOpen		=	msgQOpen;
    _pex_msgQClose              =       msgQClose;
#endif /* _WRS_CONFIG_RTP */

    _pex_msgQCreate		=	msgQCreate;
    _pex_msgQDelete		=	msgQDelete;
    _pex_msgQSend               =       msgQSend;
    _pex_msgQReceive            =       msgQReceive;

    _pex_symByCNameFind		= 	symByCNameFind;

#ifdef _WRS_CONFIG_LP64
    _pex_symFindByName		=	emul_symFindByName; 
#else
    _pex_symFindByName		=	symFindByName;
#endif /* _WRS_CONFIG_LP64 */


#ifdef INCLUDE_LOADER
    _pex_loadModule		=	loadModule;
#endif

#ifdef INCLUDE_UNLOADER
    _pex_unldByModuleId		=	unldByModuleId;
#endif

#if defined( _WRS_CONFIG_RTP) && defined (INCLUDE_RTP)
    _pex_rtpDelete		=	rtpDelete;
    _pex_rtpSpawn		=	rtpSpawn;
    _pex_syscallGroupRegister	=	syscallGroupRegister;
    _pex_scMemValidate		=	scMemValidate;
#endif /* _WRS_CONFIG_RTP */

#ifdef INCLUDE_SYM_TBL
    _pex_sysSymTbl		=	sysSymTbl;
#endif /* INCLUDE_SYM_TBL */

#ifdef INCLUDE_WDB
    _pex_wdbUserEvtPost		=	wdbUserEvtPost; 
#endif

#ifndef _pex_stderr
#define _pex_stderr		        stderr
#endif
    _pex_close			=	close;
    _pex_envGet			=       envGet;
    _pex_getenv			=       getenv;
    _pex_ioTaskStdGet           =       ioTaskStdGet;
    _pex_ioTaskStdSet         	=       ioTaskStdSet;
    _pex_open			=       open;
    _pex_putenv			=       putenv;
    _pex_taskIsSuspended        =       taskIsSuspended;
#ifdef _WRS_CONFIG_OBJECT_CLASS_LIST
    _pex_taskName             	=       taskName;
    _pex_taskNameToId       	=       taskNameToId;
#endif    
    _pex_vfprintf	        =       vfprintf;
    _pex_write			=       write;
    _pex_strerror_r             =       strerror_r;
    _pex_strerror               =       strerror;
    _pex_dup	    	    	=   	dup;
    _pex_fdopen     	    	=   	fdopen;
    _pex_fflush     	    	=   	fflush;
    _pex_stdout     	    	=   	stdout;

    _pex_signal                 =       signal;


/* ----------------------------------------------------------------- */
    _pex_free			=	free;
    _pex_malloc			=	malloc;
    _pex_memPartInfoGet		=	memPartInfoGet;
    _pex_memSysPartId		=	&memSysPartId;
    _pex_realloc		=	realloc;
    _pex_semBCreate		=	semBCreate;
    _pex_taskDelete		=	taskDelete;
    _pex_taskSpawn		=	taskSpawn;
    _pex_taskCreate		=	taskCreate;
    _pex_wdCreate		=	wdCreate;
    _pex_wdDelete		=	wdDelete;
    _pex_taskSpawn		=	emul_taskSpawn;
    _pex_taskCreate		=	emul_taskCreate;

    _pex_printf			=       printf;
    _pex_sprintf	      	=       sprintf;
    _pex_strncat                =       strncat;
    _pex_sysClkRateGet		=	sysClkRateGet;

    /*
     * Gather current layer info 
     */
    driverLayer = 1;

    /*
     * In case of Embedded Mode assign following symbols to
     * symbols from embedded ITC module
     */

#ifdef INCLUDE_VXTESTV2_EMBEDDED
    _pex_initDataModule		= 	real_initDataModule;
    _pex_itcFileName		= 	itcFileName;
    driverComponent		= 	V_MODE_EMBEDDED;
#else
    driverComponent		=	V_MODE_NORMAL;
#endif /* INCLUDE_VXTESTV2_EMBEDDED */

    /* 
     * In case of embedded mode driver can be set to start 
     * automatically here
     */
	
#ifdef INCLUDE_VXTESTV2_EMBEDDED
    /* vxTestV2("-em -v 4"); */
#endif
    printf("DONE\n");
    
#ifdef INCLUDE_VXTESTV2_POSIX_FS
    /* 
     * create a RAM disk and format it with HRFS. This is used by various tests to 
     * create temporary files.
     */

    if (xbdRamDiskDevCreate (512, VXTESTV2_POSIX_FS_SIZE, FALSE, 
                             VXTESTV2_POSIX_FS_NAME) == NULLDEV)
        return (ERROR);

    /* format and mount HRFS */

    if (hrfsFormat (VXTESTV2_POSIX_FS_NAME, 0, 0, 
                    VXTESTV2_POSIX_FS_FILES) == ERROR)
        return (ERROR);
#endif /* INCLUDE_VXTESTV2_POSIX_FS */

    /* Set up microsecond delay functionality - either vxbus (if present)
     * or software delay functionality in driver.
     *
     * At the time of this writing, any BSP that uses vxBus and defines
     * INCLUDE_TIMER_SYS will be restricted to using delay values that
     * are less than a single clock tick in duration. This is a restriction
     * imposed by vxBus itself (target/src/hwif/util/vxbDelayLIb.c).
     * This restriction makes the function difficult to use and unreliable
     * for test cases. So the vxTestV2 software delay functionality
     * is always enabled for now.
     *
     * See defect WIND00124751
     *
     * Once this defect is fixed, the VXBUS_DELAY_SUPPORT_FIXED macro and
     * associated logic may be removed.
     */
    
#undef VXBUS_DELAY_SUPPORT_FIXED
    
#if defined(INCLUDE_VXBUS) && defined(VXBUS_DELAY_SUPPORT_FIXED)

     vxTestUsDelayFcn = vxbUsDelay;

#else /* INCLUDE_VXBUS not defined or VXBUS_DELAY_SUPPORT_FIXED not defined */
    
    vxTestUsDelayFcn = vxTestUsSwDelay;
    vxTestUsSwDelayInit ();
    
#endif /* defined(INCLUDE_VXBUS) && defined(VXBUS_DELAY_SUPPORT_FIXED) */

    return OK;
    }

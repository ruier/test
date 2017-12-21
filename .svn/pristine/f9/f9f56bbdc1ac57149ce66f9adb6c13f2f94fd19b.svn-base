/* simpc/config.h - VxSim for Windows configuration file */

/*
 * Copyright (c) 1998,2001-2008,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

/*
modification history
--------------------
04t,23dec10,pee  WIND00232601
04s,26oct10,rlp  Added INCLUDE_WDB_ALWAYS_ENABLED for default configuration.
04r,22oct10,elp  increase memory for 64-bits, restored default LP64 components.
04q,28jul10,elp  limit supported features for 64 bits,
		 added INCLUDE_ADR_SPACE_LIB for 64 bits
04p,02jun10,dbt  Enable power management by default.
04o,16apr10,dbt  Make INCLUDE_VXCPULIB component mandatory (CQ:WIND00207126).
04n,15apr10,clx  Update BSP version for vxWorks 6.9
04m,18jan10,dbt  Support up to 8 IPIs.
		 Added VXSIM_USER_SM_SIZE parameter
04l,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
04k,14aug08,elp  added INCLUDE_SPY to default simulator.
04j,29may08,dbt  Added support for WDB Dynamic Printf in default image.
04i,08jan08,elp  added INCLUDE_AUX_CLK component (CQ:WIND113082).
04h,15oct07,elp  updated interrupt list.
04g,09oct07,dbt  Do define VX_SMP_NUM_CPUS only for BSP build
                 (CQ:WIND00107439).
04f,28aug07,jmg  Moved VX_SMP_NUM_CPUS to cdf files.
04e,20aug07,elp  fixed SYS_CLK_RATE_MAX (CQ:WIND100518).
04d,13jul07,elp  added INCLUDE_PLB_BUS.
04c,10jul07,tor  add vxbVxSimIpi driver for cpcLib
04b,06jun07,bwa  updated address space layout to reflect concurrent sm and dshm
03z,25may07,elp  reinstalled SMP MMU support.
03y,04apr07,dbt  Decreased default number of CPUs to 2 on SMP VxSim system.
		 Included SMP demo by default in SMP images
04a,07mar07,elp  removed MMU components for SMP.
03z,09feb07,elp  adding SMP support, removed ULIP references.
03y,09jan07,bwa  added usrDshmInit().
03x,26sep06,dbt  Enabled again RTP support.
03w,02aug06,dbt  Temporary removed RTP support.
03v,17mar05,bwa  added SM_TIPC_* definitions.
03u,29sep04,tam  replaced INCLUDE_EDR_KH by INCLUDE_KERNEL_HARDENING
03t,30sep04,jmp  reserved an interrupt range for WINDML.
03s,22sep04,jmp  updated bsp version. added CDF_OVERRIDE test for NUM_TTY
                 initialization.
03r,14sep04,jmp  PassFs component is now INCLUDE_PASSFS for all VxSim.
03q,10sep04,dbt  Include by default coprocessor components.
		 Include by default posix support
03p,02sep04,dbt  Removed no longer needed RAM_LOW_ADRS & RAM_HIGH_ADRS.
03o,29jul04,md   fixed ED&R memory defines
03n,30jul04,job  Removed INCLUDE_RTP_APPL (SPR #100117)
03n,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
03m,24jun04,jmp  increased bootline size to be coherent with BOOT_PARAMS
                 structure (SPR #98497).
03l,08jun04,jeg  added shared memory end driver support.
03k,07jun04,dbt  Fixed VM page size.
03j,11may04,dbt  Added several features included by default.
		 Increased default VxSim memory size to 32M.
03i,06may04,dbt  Removed again ROMFS support.
03h,04may04,dbt  Include CPLUS & ROMFS support by default.
03g,06apr04,dbt  Enable WindView.
03f,25mar04,jeg  added host serial device support.
03e,09mar04,dbt  RTP & shell support are included by default.
03c,03feb04,elp  removed simip support, no longer exclude network.
03b,07jan04,jmp  fixed VxSim memory autosizing (SPR #92939).
03a,05jan04,dbt  Fixed default cache mode.
02z,17dec03,jmp  changed base address to 0x10000000.
02y,20nov03,jmp  added SIMNET_MEM_SIZE, and reorganized a little.
02x,19nov03,p_b  canceled wrong checkin
02w,18nov03,jmp  modified to include MMU_BASIC support by default.
02v,17nov03,dbs  add support for PM_RESERVED_MEM
02u,08oct03,elp  removed useless simnet interrupt range macros.
02t,06oct03,dbt  Updated for new network support.
02s,29aug03,elp  added simnet support.
02r,21aug03,jeg  updated to not use any routine to get shared memory level
02q,18aug03,jeg  added shared memory support
02p,31jul03,jeg  added non-volative RAM support
02o,23jul03,kam  commented out WindView inclusions
02n,02jul03,jeg  added virtual disk support
02m,18feb03,dbt  Upgraded BSP revision.
02p,28apr03,jmp  no longer need to setup NVRAM parameters (now using nullNvRam
		 driver).
02o,18apr03,jmp  moved bootline support to host side.
02n,04apr03,jmp  enabled system mode support.
02m,19feb03,jmp  code cleanup.
02l,19dec02,jmp  added multiple network interface support (SPR #73834).
02k,18dec02,jeg  added support for TIMESTAMP driver
02j,23may02,jmp  forced WDB_MODE to WDB_MODE_TASK on full simulator.
02i,03may02,jmp  added INCLUDE_HW_FP (SPR #76719).
02h,15apr02,hbh  Enabled system mode debugging for pipe backend.
02g,20nov01,jmp  no longer allow to switch to system mode.
02f,12nov01,jmp  no longer allow to switch to system mode when INCLUDE_END.
02e,31oct01,hbh  Updated BSP revision for T2.2. Code cleanup.
02d,20oct98,pr   added WindView 2.0 defines
01c,21sep98,cym  added declaration for simMemSize.
01b,11aug98,sbs  changed WDB_COMM_TYPE to be WDB_COMM_END if END
                 support is included.
01a,29apr98,cym  written based on pc386.
*/

/*
This module contains the configuration parameters for the
Windows simulator.
*/

#ifndef	INCconfigh
#define	INCconfigh

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/0"        /* 0 for first revision */

#include <vsbConfig.h>
#include "configAll.h"

#ifdef	_WRS_CONFIG_SMP
#define DRV_INTCTLR_VXSIM
#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE 50000
#define INCLUDE_PLB_BUS
#endif	/* _WRS_CONFIG_SMP */

#undef INCLUDE_AOUT

/*
 * Windows simulator interrupt vectors table:
 * as defined in h/arch/simnt/ivSimnt.h
 *
 * Vec. Range	Vector Name			Description
 * -----------  ------------------------------  --------------------------------
 * 0x0000	TIMER_SYS_CLK_INT		System clock interrupt
 * 0x0001	TIMER_AUX_CLK_INT		Auxiliary clock interrupt
 * 0x0002	TIMER_TS_ROLL_OVER_INT		Timestamp rollover interrupt
 * 0x0003	MAILSLOT_INT			Backend pipe interrupt
 * 0x0004	CONSOLE_SIO_RX_INT		SIO driver interrupt
 * 0x0005	BUS_INT				Bus interrupt
 * 0x0006	IV_IPI0				Inter processor interrupt
 * 0x0007	IV_IPI1				Inter processor interrupt
 * 0x0008	IV_IPI2				Inter processor interrupt
 * 0x0009	IV_IPI3				Inter processor interrupt
 * 0x000a	IV_IPI4				Inter processor interrupt
 * 0x000b	IV_IPI5				Inter processor interrupt
 * 0x000c	IV_IPI6				Inter processor interrupt
 * 0x000d	IV_IPI7				Inter processor interrupt
 * 0x000e-00ef	<internal>			Reserved for internal use
 * 0x00f0	WINDML_INT_RANGE_BASE		WindML interrupts range base
 * 0x00ff	WINDML_INT_RANGE_END		WindML interrupts range end
 * 0x0180	SIMNET_INT_RANGE_BASE		Simnet interrupt for interface 0
 * 0x0181	SIMNET_INT_RANGE_BASE + 1	Simnet interrupt for interface 1
 * ...		SIMNET_INT_RANGE_BASE + n 	Simnet interrupt for interface n
 * 0x01ff	SIMNET_INT_RANGE_END		Simnet interrupt range end
 * 0x0200	USER_INT_RANGE_BASE		User interrupts range base
 * 0x02ff	USER_INT_RANGE_END		User interrupts range end
 */

/*
 *			VxSim Memory Address space
 *			--------------------------
 *
 *   |                  |
 *   |------------------| <-- SIMNET_MEM_ADRS + SIMNET_MEM_SIZE
 *   |                  |
 *   |                  |
 *   | VxSim Networking |
 *   |  Address Space   |
 *   |(SIMNET_MEM_SIZE) |
 *   |                  |
 *   |------------------| <-- SIMNET_MEM_ADRS
 *   |                  |
 *   |   Distributed    |
 *   |  Shared Memory   |
 *   |                  |
 *   |                  |
 *   |------------------| <-- DSHM_ADRS
 *   |                  |
 *   |  Shared Memory   |
 *   |    TIPC Pool     |
 *   |  (SM_TIPC_SIZE)  |
 *   |                  |
 *   |------------------| <-- SM_TIPC_ADRS
 *   |                  |
 *   |  Shared Memory   |
 *   |     Objects      |
 *   |(SM_OBJ_MEM_SIZE) |
 *   |                  |
 *   |------------------| <-- SM_OBJ_MEM_ADRS
 *   |                  |
 *   |  Shared Memory   |
 *   |      Pool        |
 *   |  (SM_MEM_SIZE)   |
 *   |                  |
 *   |------------------| <-- SM_MEM_ADRS
 *   |                  |
 *   |  Shared Memory   |
 *   |      Anchor      |
 *   | (SM_ANCHOR_SIZE) |
 *   |                  |
 *   |------------------| <-- SM_ANCHOR_ADRS
 *   |                  |
 *   | VxWorks Physical |
 *   |  Address Space   |
 *   | (LOCAL_MEM_SIZE) |
 *   |                  |
 *   |------------------| <-- LOCAL_MEM_LOCAL_ADRS
 *   |                  |
 *
 */

/* VxWorks Physical Address space */

#ifdef _WRS_CONFIG_LP64
#define LOCAL_MEM_LOCAL_ADRS	0x180000000LL	/* VxSim physical memory base */
#define	LOCAL_MEM_SIZE		0x04000000	/* VxSim physical memory size */

/*
 * Decrease default common heap size to allow running vxsim-64 with less
 * memory.
  */

#undef KERNEL_COMMON_HEAP_INIT_SIZE
#define KERNEL_COMMON_HEAP_INIT_SIZE    0x1000000
#undef KERNEL_PROXIMITY_HEAP_SIZE
#define KERNEL_PROXIMITY_HEAP_SIZE	0x1000000

#else /* _WRS_CONFIG_LP64 */
#define	LOCAL_MEM_LOCAL_ADRS	0x10000000	/* VxSim physical memory base */
#define	LOCAL_MEM_SIZE		0x02000000	/* VxSim physical memory size */
#endif /* _WRS_CONFIG_LP64 */

#define	LOCAL_MEM_BUS_ADRS	0x0		/* Not used                   */

/* Shared memory Address space */

#undef	SM_ANCHOR_ADRS				/* Anchor memory address      */
extern	void * sysSmAnchorAdrs;
#define SM_ANCHOR_ADRS		((int) sysSmAnchorAdrs)
#define SM_ANCHOR_SIZE		ROUND_UP(sizeof (SM_ANCHOR), SM_ALIGN_BOUNDARY)
						/* Anchor memory size	      */
#define SM_MEM_ADRS		SM_ANCHOR_ADRS + SM_ANCHOR_SIZE
						/* Shared memory address      */
#define SM_MEM_SIZE		0x00020000 - SM_ANCHOR_SIZE
						/* Shared memory size	      */
#define SM_OBJ_MEM_ADRS		((int) SM_MEM_ADRS + SM_MEM_SIZE)
						/* Sh. memory objects addr    */
#define SM_OBJ_MEM_SIZE		0x00010000	/* Sh. Mem. objects pool size */
#define SM_TIPC_ADRS_DEFAULT    ((int) SM_OBJ_MEM_ADRS + SM_OBJ_MEM_SIZE)
#define SM_TIPC_ADRS		SM_TIPC_ADRS_DEFAULT
#define SM_TIPC_SIZE_DEFAULT    0x00020000
#define SM_TIPC_SIZE		0               /* SM TIPC pool size     */
#define SM_OFF_BOARD		FALSE		/* Memory pool is on-board    */

/* dSHM */
extern void* sysDshmBaseAdrs;

/* VxSim Networking Address space */

/*
 * NOTE: SIMNET_MEM_ADRS must be determined in sysLib.c to take into account
 * modifications potentially done via the project tool (VxMP inclusion and
 * modifications VxMP parameters.
 */

#define SIMNET_MEM_SIZE		0x01000000	/* VxSim Networking Pool Size */

/* User reserved memory.  See sysMemTop(). */

#define USER_RESERVED_MEM       0

/* MMU & Cache configuration */

#define	INCLUDE_MMU_BASIC
#undef	INCLUDE_MMU_FULL
#undef	USER_D_CACHE_MODE
#define	USER_D_CACHE_MODE	CACHE_COPYBACK

/* Include VXCPU library by default */

#define	INCLUDE_VXCPULIB

/* Include Power Management by default */

#define INCLUDE_CPU_LIGHT_PWR_MGR

/* Pass througth File system initialization */

#define INCLUDE_PASSFS

/* Virtual disk initialization */

#define INCLUDE_VIRTUAL_DISK

/* SMP configuration */

#ifdef _WRS_CONFIG_SMP
#ifndef CDF_OVERRIDE
#define VX_SMP_NUM_CPUS		2
#endif	/* CDF_OVERRIDE */
#define INCLUDE_SMP_DEMO		/* Include SMP demos */
#endif  /* _WRS_CONFIG_SMP */

/* Use WDB Pipe backend by default */

#undef  WDB_COMM_TYPE
#define WDB_COMM_TYPE		WDB_COMM_PIPE

#define INCLUDE_END

#ifdef  INCLUDE_END
#define INCLUDE_SIMNET			/* SIMNET network driver */
#undef  IP_MAX_UNITS
#define IP_MAX_UNITS    16		/* max # of network interface */
#endif /* INCLUDE_END */

/*
 * Shared memory end driver: Define INCLUDE_SM_NET to include Shared memory
 * end driver support.
 */

#ifdef INCLUDE_SM_NET
#define INCLUDE_SM_COMMON
#define INCLUDE_SMEND
#endif /* INCLUDE_SM_NET */

/*
 * VxSim User Shared Memory. Define INCLUDE_VXSIM_USER_SM to create
 * a VxSim user specific (vs system) user shared memory area of
 * <VXSIM_USER_SM_SIZE> bytes.
 */

#undef	INCLUDE_VXSIM_USER_SM	
#define	VXSIM_USER_SM_SIZE	0x2000	/* VxSim user shared memory size */

/* Misc. options */

#undef	VM_PAGE_SIZE
#define VM_PAGE_SIZE	0x10000

#define INCLUDE_HW_FP			/* hardware fpp support */
#define INCLUDE_COPROCESSOR		/* coprocessor support */
#define INCLUDE_AUX_CLK			/* auxiliary clock */

/* miscellaneous definitions */

/*
 * Usual system tick rate is 10ms, setting system clock rate over host rate 
 * will lead to incorrect delays. Limiting clock rate to 100.
 */

#define SYS_CLK_RATE_MIN  19            /* minimum system clock rate */
#define SYS_CLK_RATE_MAX  100		/* maximum system clock rate */
#define AUX_CLK_RATE_MIN  2             /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX  1000          /* maximum auxiliary clock rate */

/* Shared memory definitions */

IMPORT int sysSmLevel;

#define SM_INT_TYPE     SM_INT_BUS		/* pseudo bus interrupts */
#define SM_INT_ARG1     sysSmLevel		/* "level"		 */
#define SM_INT_ARG2     (int) BUS_INT		/* "vector" (BUS_INT)    */
#define SM_INT_ARG3     0			/* unused 		 */

/*
 * Bootline parameters
 */

#define DEFAULT_BOOT_LINE	BOOT_LINE_ADRS

/* Redefine Bootline length to be coherent with BOOT_PARAMS strcture */

#undef BOOT_LINE_SIZE
#define BOOT_LINE_SIZE		ROUND_UP (sizeof (BOOT_PARAMS), 0x100)

#undef EXC_MSG_ADRS
#define EXC_MSG_ADRS		(BOOT_LINE_ADRS + BOOT_LINE_SIZE)

/* non-volatile RAM parameters */

#define NV_RAM_SIZE	0x100		/* 256 bytes 		*/
#undef  NV_RAM_ADRS			/* not applicable 	*/
#undef  NV_BOOT_LINE			/* not applicable 	*/

/*
 * The following parameters are defined here and in the Makefile.
 * The must be kept synchronized; effectively config.h depends on Makefile.
 * Any changes made here must be made in the Makefile and vice versa.
 */

#define ROM_BASE_ADRS		0		/* base address of ROM */
#define ROM_TEXT_ADRS		(ROM_BASE_ADRS)	/* booting from EPROM */
#define ROM_SIZE		0		/* size of ROM */

/* windview support */

#define INCLUDE_TRIGGERING      /* event triggering tool */

#define INCLUDE_WINDVIEW        /* windview control tool */
#define INCLUDE_TIMESTAMP	/* timestamp driver */
#define INCLUDE_WDB_TSFS        /* target-server file system */

/* windview upload paths */

#define INCLUDE_WVUPLOAD_FILE
#define INCLUDE_WVUPLOAD_TSFSSOCK

/* select the BSP specific timestamp driver */

#ifdef  INCLUDE_TIMESTAMP		/* timestamp driver */
#    undef INCLUDE_SEQ_TIMESTAMP  	/* sequential timestamping */
#    undef INCLUDE_USER_TIMESTAMP 	/* user defined timestamping */
#    define INCLUDE_SYS_TIMESTAMP  	/* BSP specific timestamp driver */
#endif  /* INCLUDE_TIMESTAMP */

/* On VxSim, several components are included by default */

#define INCLUDE_SHELL		/* interactive c-expression interpreter */
#define INCLUDE_SHELL_VI_MODE	/* vi editing mode for the shell */
#define INCLUDE_SHELL_EMACS_MODE /* emacs editing mode for the shell */
#define INCLUDE_SHELL_INTERP_C	/* C interpreter */
#define INCLUDE_SHELL_INTERP_CMD /* shell command interpreter */
#define INCLUDE_SHOW_ROUTINES	/* show routines for system facilities*/
#define INCLUDE_COPROCESSOR_SHOW /* show routines for coprocessor facilities */

/* VIP will include this component by default. */
/* forcing it could create conflicts with predefeined profiles */
#ifndef CDF_OVERRIDE
#define INCLUDE_PASSFS_SYM_TBL	/* compiled-in symbol table */
#endif

#define INCLUDE_STARTUP_SCRIPT	/* execute start-up script */
#define INCLUDE_STAT_SYM_TBL	/* create user-readable error status */
#define INCLUDE_DISK_UTIL	/* ls, cd, mkdir, xcopy, etc */
#define INCLUDE_LOADER		/* object module loading */
#define INCLUDE_UNLOADER	/* object module unloading */
#define INCLUDE_DEBUG		/* native debugging */
#define INCLUDE_MEM_SHOW	/* Memory show routines */
#define INCLUDE_SYM_TBL_SHOW	/* symbol table show routines */
#define INCLUDE_SPY		/* spyLib for task monitoring */
#define INCLUDE_RTP		/* Real Time Process */
#define INCLUDE_SHARED_DATA	/* shared data regions */
#define INCLUDE_SHL		/* shared libraries */
#define INCLUDE_SHL_SHOW	/* shared libraries show component */
#define INCLUDE_RTP_APPL_INIT_BOOTLINE	/* RTP Startup bootline Facility */
#define INCLUDE_CPLUS		/* include C++ support */
#define INCLUDE_CPLUS_LANG	/* include C++ class libraries */
#define INCLUDE_ROMFS		/* rom based file system */
#define INCLUDE_EDR_PM		/* ED&R persistent memory */
#define INCLUDE_EDR_ERRLOG	/* ED&R error log */
#define INCLUDE_EDR_SHOW	/* ED&R show routines */
#define INCLUDE_EDR_SYSDBG_FLAG	/* ED&R debug flag */
#undef  PM_RESERVED_MEM
#define PM_RESERVED_MEM         0x100000	/* PM reserved memory */
#undef  EDR_ERRLOG_SIZE
#define EDR_ERRLOG_SIZE         0x80000		/* ED&R error log size */

/*
 * kernel hardening features are included by default in VxSim, that is
 *	- text segment write protection
 *	- exception vector table write protection
 *	- kernel task stack overflow and underflow detection
 *	- non-executable kernel task stacks
 */

#define	INCLUDE_KERNEL_HARDENING

/* POSIX support is included by default in VxSim */

#define INCLUDE_POSIX_AIO       /* POSIX async I/O support */
#define INCLUDE_POSIX_AIO_SYSDRV/* POSIX async I/O system driver */
#define INCLUDE_POSIX_FTRUNC	/* POSIX ftruncate routine */
#define INCLUDE_POSIX_MEM	/* POSIX memory locking */
#define INCLUDE_POSIX_MQ        /* POSIX message queue support */
#define INCLUDE_POSIX_PTHREADS  /* POSIX pthreads support */
#define INCLUDE_POSIX_SCHED	/* POSIX scheduling */
#define INCLUDE_POSIX_SEM 	/* POSIX semaphores */
#define INCLUDE_POSIX_SIGNALS 	/* POSIX queued signals */
#define INCLUDE_POSIX_TIMERS 	/* POSIX timers */
#define INCLUDE_POSIX_CLOCKS	/* POSIX clocks */
#define INCLUDE_POSIX_DIRLIB 	/* POSIX directory utilities */
#ifdef INCLUDE_RTP
#define INCLUDE_SC_POSIX 	/* POSIX system calls */
#endif /* INCLUDE_RTP */

#ifdef	INCLUDE_WDB
/* WDB is always turned on */

#define	INCLUDE_WDB_ALWAYS_ENABLED

/* WDB Dynamic Printf support */

#define	INCLUDE_WDB_DPRINTF
#endif	/* INCLUDE_WDB */

/* define INCLUDE_HOST_SIO to include host serial device support */

#undef NUM_TTY
#undef INCLUDE_HOST_SIO

#ifndef CDF_OVERRIDE
#   ifdef INCLUDE_HOST_SIO
#       define NUM_TTY                  2
#       define HOST_SIO_PORT_NUMBER     0 	/* 0 for COM1, 1 for COM2 */
#   else
#       define NUM_TTY                  1
#   endif /* INCLUDE_HOST_SIO */
#endif /* ! CDF_OVERRIDE */

#endif	/* INCconfigh */
#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

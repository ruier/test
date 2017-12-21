/* sysLib.c - Simulator-target system dependent library */

/*
 * Copyright (c) 2001-2012, Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
03p,12sep12,elp  Fixed documentation (CQ:WIND00371598)
03o,06sep11,elp  added sysCpuId (CQ:WIND00286339).
03n,04apr11,dbt  Updated to use latest wrload() interfaces.
03m,03jan11,dbt  Removed sysMemTop() & sysPhysMemTop() APIs in 64-bit
		 mode (CQ:00248957).
03l,22oct10,elp  fixed PM and USER_RESERVED memory configuration (64-bits)
03k,19oct10,elp  sysCpuStart() LP64 adaptation, added sysDevConnect().
03j,30jun10,elp  moved hardWareInterfaceInit() declaration.
03i,02jun10,elp  64 bits support
03h,28may10,dbt  Fixed support of MIPC & SM_OBJ in the same image
		 (CQ:WIND00211318).
03g,17may10,dbt  Clear sysExcMsg after printing it.
03f,16apr10,dbt  Enable early call of sysProcNumGet() (CQ:WIND00207126).
03e,24mar10,dbt  Removed build warning.
03d,20jan10,dbt  Added support for user shared memory.
03c,12jan10,elp  64bits support (host calls).
03b,05jun09,elp  added MIPC2.0 support
03a,09mar09,dbt  Added wrload()/AMP support.
02z,03sep08,elp  added MIPC support.
02y,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
02x,30jul07,elp  clean up includes,
		 use MMU_PAGE_TBL_SIZE rather than internal MMU_NUM_L2_DESC.
02x,15aug07,mmi  remove legacy power management references: windPwrXX ()
02w,10jul07,tor  remove ipiCtrlInit and related, cvt to vxIpiLib
02v,03jul07,elp  added sysCpuAvailableGet().
02u,19jun07,elp  set VxMP, DSHM and SIMNET memory physical address to NONE.
02t,09may07,bwa  made dshm and sm possible concurrent components.
02s,13feb07,elp  merged sio drivers.
02r,07feb07,elp  hardcoded IPI interrupt.
02q,21dec06,bwa  added vxbus and dSHM support.
02p,06dec06,dbt  Added sysCpuConfigured variable.
02o,24nov06,dbt  Fixed SMP support for Solaris VxSim.
02n,23nov06,dbt  Fixed sysIpiEmit() definition.
02m,06nov06,elp  added SMP support.
02l,25oct06,elp  include vxsimHostLibP.h rather than vxsimHostLib.h
02k,01jul05,jmp  fixed sysModelStr initialization.
02j,29jun05,jmp  fixed sysModel() to no longer return an vxsim Host side
                 address.
02i,26apr05,dbt  Fixed typo in documentation (SPR #108731).
02h,17mar05,bwa  added SM_TIPC_SIZE.
02g,22feb05,elp  replaced INCLUDE_NET_INIT by INCLUDE_NETWORK (SPR #106467).
02f,16nov04,jmp  fixed documentation references.
02e,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
02f,30aug04,jmp  added display of exception message (if any) to
                 sysToMonitor().
02e,18aug04,md   PM_RESERVED_MEM is dependent on INCLUDE_EDR_PM
02d,27may04,jeg  added smEnd driver support.
02c,22mar04,jmp  added unixSio.c and winSio.c inclusion.
02b,07jan04,jmp  fixed VxSim memory autosizing (SPR #92939).
02a,05jan04,dbt  Fixed build errors and warnings.
01z,17dec03,elp  added global simnetMemAdrs and simnetMemSize.
01y,01dec03,dbt  Added support for new power management.
01x,20nov03,jmp  added VxMP and network memory to sysPhysMemDesc[].
01w,27oct03,dbt  Moved sysNetEndInit() call from usrNetwork.c to sysHwInit().
01v,14oct03,jmp  enabled MMU caching for VxWorks memory area.
01u,08oct03,elp  moved sysNetStop to host side.
01t,01oct03,jmp  added sysPhysMemDesc.
01s,03sep03,elp  added call to sysNetStop() when rebooting.
01r,21aug03,jeg  removed sysSmLevelGet () routine
01q,18aug03,jeg  added VxMP support for Windows VxSim. Updated
		 vxsimHostShMemInit () call. Added sysSmLevelGet () routine.
01p,13aug03,jeg  saved bootline into NVRAM at startup
01o,28jul03,jeg  removed mull nvram support and added sysNvRamSet () and
		 sysNvRamGet () routines.
01o,16sep03,dbs  add PM_RESERVED_MEM
01n,16jun03,dbt  Added support for RTC.
		 Merged file with Windows VxSim one.
01m,10jun03,jmp  renamed sysBaseAdrs in sysMemBaseAdrs.
01l,13may03,jmp  modified to use nullNvRam driver.
		 added sysBaseAdrs and sysMemSize.
01k,25apr03,dbt  Moved timer related intConnect() to unixTimer.c
01j,23apr03,jmp  moved bootline support to host side.
01i,22apr03,dbt  Added SIMLINUX support.
01h,04apr03,jeg  restored vxsim_ip_name variable
01g,31mar03,jeg  renamed vxsimHostRestart to vxsimHostReboot.
01f,17mar03,jeg  removed memory allocation. Fixed sysBootLineMagic string.
01e,05mar03,jeg  removed tyCoDv definition. Fix build warning. Add passDev to
		 bootdev if needed. Removed unused variables.
01d,20feb03,jeg  removed PPP code. Move timer code to unixTimer.c file. Updated
		 access to host specific routines. Moved sysToMonitor code to
		 host side. Removed NvRam code.
01c,28jan03,jmp  fixed sysSmAddr declaration.
01b,08jan03,jeg  update simDosTimeHook routine call.
01e,08apr02,jmp  added sysBootParams initialization if INCLUDE_NET_INIT and
                 INCLUDE_BOOT_LINE_INIT are not defined (SPR #75207).
01a,27jun01,jmp  merged solaris/sysLib.c@@/main/tor3_x/23 and
		 linux/sysLib.c@@/main/tor3_x/4.
*/

/*
DESCRIPTION
This library provides solaris and linux VxWorks simulator specific routines.

INCLUDE FILES: sysLib.h

SEE ALSO:
\tb VxWorks Kernel Programmer's Guide .
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <sysLib.h>
#include <config.h>
#include <vxLib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <vmLib.h>
#include <private/vxsimHostLibP.h>
#include <intLib.h>
#include <fioLib.h>

#if CPU==SIMNT
#include <arch/simnt/mmuArchLib.h>
#elif CPU == SIMLINUX 
#include <arch/simlinux/mmuArchLib.h>
#else
#include <arch/simsolaris/mmuArchLib.h>
#endif

#ifdef _WRS_CONFIG_SMP 
#if CPU==SIMNT
#include <arch/simnt/vxCpuSimntLib.h>
#elif CPU == SIMLINUX 
#include <arch/simlinux/vxCpuSimlinuxLib.h>
#else
#include <arch/simsolaris/vxCpuSimsolarisLib.h>
#endif

#include <private/cpcLibP.h>
#include <private/sigLibP.h>
#endif /* _WRS_CONFIG_SMP */

#ifdef	INCLUDE_WRLOAD
#include <wrload.h>
#endif	/* INCLUDE_WRLOAD */

#ifdef INCLUDE_VXBUS
#include <private/excLibP.h>
#endif /* INCLUDE_VXBUS */

#if	CPU == SIMNT
#include "simPcTimer.c"
#else
#include "unixTimer.c"
#endif	/* CPU == SIMNT */

#include "simSio.c"
#include "sysSerial.c"

#ifdef INCLUDE_NETWORK
#include "sysNet.c"		/* network driver support */
#endif /* INCLUDE_NETWORK */

#include "hwif/vxbus/vxBus.h"
#include "./hwconf.c"

/* defines */

#define DUMMY_PHYS_ADDR		-1
#define DUMMY_VIRT_ADDR		-1
#define DUMMY_LENGTH		-1
#define DUMMY_INIT_STATE_MASK	-1
#define DUMMY_INIT_STATE	-1

#define SYS_MODEL_STR_LEN	100

#define SYS_PLB_MAX_COUNT	3	/* max number of plbs */

/* externals */

#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit (void);
#endif

/* globals */

char *	sysMemBaseAdrs	= (char *)
			  LOCAL_MEM_LOCAL_ADRS; /* simulator memory base addr */
size_t	sysMemSize	= LOCAL_MEM_SIZE; /* simulator memory size	      */
int	sysCpu		= CPU;		/* system CPU type    		      */
char *	sysBootLine	= BOOT_LINE_ADRS;/* address of boot line	      */
char *	sysExcMsg	= EXC_MSG_ADRS;	/* catastrophic message area          */
int   	sysProcNum	= -1;		/* processor number of this CPU	      */
int	sysFlags;			/* boot flags			      */
char	sysBootHost [BOOT_FIELD_LEN];	/* name of host from which we booted  */
char	sysBootFile [BOOT_FIELD_LEN];	/* name of file from which we booted  */
void *	sysSmAddr = 0x0;		/* vxsim SM area */
UINT32	sysSmSize = 0;			/* vxsim shared memory total size */
void *  sysSmAnchorAdrs	= 0x0;		/* SM_ANCHOR_ADRS                     */
int 	sysSmLevel;			/* SM_INT_ARG1 = shared mem level     */
int	sysUserGid;			/* user group ID                      */
void *  sysDshmAddr = 0x0;		/* dSHM base address                  */
void *	sysMipcSmAddr = 0x0;		/* MIPC base address */

#if (CPU_FAMILY==SIMNT || CPU_FAMILY==SIMLINUX)
/*
 * Simulate i80x86 family cpu id to indicate floating point usage.
 * These values need to be coherent with FPREG_SET type and compiler options.
 * 32 bit simulator uses i387 fp registers.
 * 64 bit simulator supports additional xmm[0-15].
 */

CPUID sysCpuId =
    {
    0,							/* highestValue */
    {0},						/* vendorId */
    0, 							/* signature */
    0,							/* featuresEbx */
#ifdef _WRS_CONFIG_LP64
    VX_CPUID_XSAVE,					   /* featuresEcx */
    VX_CPUID_MMX|VX_CPUID_FXSR|VX_CPUID_SSE|VX_CPUID_SSE2, /* featuresEdx */
#else /* _WRS_CONFIG_LP64 */
    0,							/* featuresEcx */
    0,							/* featuresEdx */
#endif /* _WRS_CONFIG_LP64 */
    0,  						/* cacheEax */
    0,  						/* cacheEbx */
    0,							/* cacheEcx */
    0, 							/* cacheEdx */
    {0,1},						/* serialNo64 */
    {0}							/* brandString */
    };
#endif /* (CPU_FAMILY==SIMNT || CPU_FAMILY==SIMLINUX) */

#ifdef _WRS_CONFIG_SMP
int	sysCpuConfigured = VX_SMP_NUM_CPUS;	/* indicate number of CPUs */
					/* configured by default */
#endif /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
VXSIM_ULONG	sysAmpCpuStart = 0;
VXSIM_ULONG	sysAmpCpuEntryPt = NULL;
#endif	/* INCLUDE_WRLOAD_IMAGE_BUILD */

#ifdef _WRS_CONFIG_LP64
#if (VXSIM_KERNEL_SYS_MEM_RGN_BASE % MMU_PAGE_TBL_SIZE != 0)
#error "VXSIM_KERNEL_SYS_MEM_RGN_BASE must be aligned on MMU_PAGE_TBL_SIZE"
#endif
VIRT_ADDR	sysKernelSysMemRgnBase = VXSIM_KERNEL_SYS_MEM_RGN_BASE;

#if ((VXSIM_KERNEL_SYS_MEM_RGN_SIZE % MMU_PAGE_TBL_SIZE) != 0)
#error "VXSIM_KERNEL_SYS_MEM_RGN_SIZE must be aligned on MMU_PAGE_TBL_SIZE"
#endif
size_t		sysKernelSysMemRgnSize = VXSIM_KERNEL_SYS_MEM_RGN_SIZE;

#if (VXSIM_KERNEL_SYS_MEM_RGN_BASE + VXSIM_KERNEL_SYS_MEM_RGN_SIZE > \
     VXSIM_RTP_PRIVATE_RGN_BASE)
#error "VXSIM_KERNEL_SYS_MEM_RGN overlaps VXSIM_RTP_PRIVATE_RGN"
#endif
#if (VXSIM_RTP_PRIVATE_RGN_BASE % MMU_PAGE_TBL_SIZE != 0)
#error "VXSIM_RTP_PRIVATE_RGN_BASE must be aligned on #MMU_PAGE_TBL_SIZE"
#endif
VIRT_ADDR	sysRtpPrivMemRgnBase = VXSIM_RTP_PRIVATE_RGN_BASE;

size_t		sysRtpPrivMemRgnSize = ROUND_UP(VXSIM_RTP_PRIVATE_RGN_SIZE,
						MMU_PAGE_TBL_SIZE);
VIRT_ADDR 	sysSharedMemRgnBase = VXSIM_RTP_PRIVATE_RGN_BASE +
				      ROUND_UP(VXSIM_RTP_PRIVATE_RGN_SIZE,
					       MMU_PAGE_TBL_SIZE);
size_t		sysSharedMemRgnSize = ROUND_UP(VXSIM_SHARED_RGN_SIZE,
					       MMU_PAGE_TBL_SIZE);
VIRT_ADDR 	sysKernelVirtPoolMemRgnBase = VXSIM_RTP_PRIVATE_RGN_BASE +
					  ROUND_UP(VXSIM_RTP_PRIVATE_RGN_SIZE,
						   MMU_PAGE_TBL_SIZE) +
					  ROUND_UP (VXSIM_SHARED_RGN_SIZE,
						    MMU_PAGE_TBL_SIZE);
size_t		sysKernelVirtPoolMemRgnSize =
				ROUND_UP(VXSIM_KERNEL_VIRT_POOL_RGN_SIZE,
					 MMU_PAGE_TBL_SIZE);
#endif /* _WRS_CONFIG_LP64 */

/*
 *                      VxSim Memory Address space
 *                      --------------------------
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
 *   |   MIPC           |
 *   |  Shared Memory   |
 *   |                  |
 *   |                  |
 *   |------------------| <-- MIPC_SM_SYSTEM_POOL_BASE
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
 * NOTE: The size of those different block must be mutliples MMU_PAGE_TBL_SIZE
 * (1MBytes on solaris and linux and 2Mbytes on Windows) this is a MMU
 * requirement.
 */

#define VXSIM_SM_CTRL_SIZE	MMU_PAGE_SIZE

#define NOT_INITIALIZED		0

#define SM_TOTAL_SIZE		0
#define DSHM_SM_POOL_SIZE	0
#define MIPC_SM_POOL_SIZE	0
#define	USER_SM_POOL_SIZE	0
#define	VXSIM_SM_SIZE		0
#undef	VXSIM_HAS_SHARED_MEM

/* Add VXSIM_SM_CTRL_SIZE to the 1st configured shared memory region */

#if defined(INCLUDE_SM_COMMON)
#undef SM_TOTAL_SIZE
#define SM_TOTAL_SIZE		ROUND_UP ((SM_MEM_SIZE + SM_OBJ_MEM_SIZE + \
				 SM_ANCHOR_SIZE + SM_TIPC_SIZE), MMU_PAGE_SIZE)
#undef	VXSIM_HAS_SHARED_MEM
#define	VXSIM_HAS_SHARED_MEM
#endif	/* INCLUDE_SM_COMMON */

#ifdef INCLUDE_DSHM
#undef DSHM_SM_POOL_SIZE
#define DSHM_SM_POOL_SIZE	ROUND_UP ((DSHM_BUS_PLB_MAXNODES * \
					 DSHM_BUS_PLB_POOLSIZE), MMU_PAGE_SIZE)
#undef	VXSIM_HAS_SHARED_MEM
#define	VXSIM_HAS_SHARED_MEM
#endif /* INCLUDE_DSHM */

#ifdef INCLUDE_MIPC_SM
#undef MIPC_SM_POOL_SIZE
#define MIPC_SM_POOL_SIZE	ROUND_UP (MIPC_SM_SYSTEM_POOL_SIZE, \
					  MMU_PAGE_SIZE)
#undef	VXSIM_HAS_SHARED_MEM
#define	VXSIM_HAS_SHARED_MEM
#endif /* INCLUDE_MIPC_SM */

#if	defined(INCLUDE_VXSIM_USER_SM) && defined(VXSIM_USER_SM_SIZE)
#if	(VXSIM_USER_SM_SIZE != 0)
#undef	USER_SM_POOL_SIZE
#define	USER_SM_POOL_SIZE	ROUND_UP (VXSIM_USER_SM_SIZE, \
					  MMU_PAGE_SIZE)
#undef	VXSIM_HAS_SHARED_MEM
#define	VXSIM_HAS_SHARED_MEM
#endif	/* (VXSIM_USER_SM_SIZE != 0) */
#endif	/* INCLUDE_VXSIM_USER_SM && VXSIM_USER_SM_SIZE */

#ifdef	VXSIM_HAS_SHARED_MEM
#undef	VXSIM_SM_SIZE
#define	VXSIM_SM_SIZE		ROUND_UP (SM_TOTAL_SIZE + \
					  DSHM_SM_POOL_SIZE + \
					  MIPC_SM_POOL_SIZE + \
					  USER_SM_POOL_SIZE + \
					  VXSIM_SM_CTRL_SIZE, \
					  MMU_PAGE_TBL_SIZE)
#endif	/* VXSIM_HAS_SHARED_MEM */

/*
 * NOTE: SIMNET_MEM_ADRS must be determined here to take into account
 * modifications potentially done via the project tool (VxMP inclusion and
 * VxMP parameters modification).
 */

#ifdef	INCLUDE_NETWORK
#define	SIMNET_MEM_ADRS		((int) LOCAL_MEM_LOCAL_ADRS + sysMemSize + \
				 VXSIM_SM_SIZE)
#endif	/* INCLUDE_NETWORK */

/*
 * sysPhysMemDesc[]
 *
 * !!! WARNINGS !!!
 *
 *	If you need to add entries to sysPhysMemDesc[] array, please follow
 *	these instructions:
 *
 *	- Do not modify existing entries.
 *	- New entries must be added and the end of sysPhysMemDesc[], following
 *	  the given template.
 *	- The memory size of the entries must be mutliples of 1MBytes, this
 *	  is a MMU requirement.
 *	- The routine sysPhysMemDescUpdate() must be modified to handle new
 *	  entries.
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {

    /* VxWorks Physical Address space */

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,		/* virtual address	*/
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,		/* physical address	*/
    NOT_INITIALIZED,				/* length		*/

    /* initial state mask */

    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,

    /* initial state */

    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE
    }
#ifdef VXSIM_HAS_SHARED_MEM
    ,

    /* VxSim Shared Memory Address space */

    {
    (VIRT_ADDR) NONE,		/* virtual address	*/
    (PHYS_ADDR) NONE,		/* physical address	*/
    VXSIM_SM_SIZE,		/* length		*/

    /* initial state mask */

    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,

    /* initial state */

    VM_STATE_VALID_NOT | VM_STATE_WRITABLE | VM_STATE_CACHEABLE
    }
#endif	/* INCLUDE_SM_COMMON */

#if (defined (INCLUDE_SIMNET) && defined (INCLUDE_NETWORK))
    ,

    /* VxSim Networking Address space */

    {
    (VIRT_ADDR) NONE,					/* virtual address */
    (PHYS_ADDR) NONE,					/* physical address */
    ROUND_UP (SIMNET_MEM_SIZE, MMU_PAGE_TBL_SIZE),	/* length	*/

    /* initial state mask */

    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,

    /* initial state */

    VM_STATE_VALID_NOT | VM_STATE_WRITABLE | VM_STATE_CACHEABLE
    }
#endif	/* INCLUDE_SIMNET && INCLUDE_NETWORK */

    /*
     * WARNING:
     * Add your entries below, following this template.
     */

#if 0
    ,

    /* Memory space template */

    (VIRT_ADDR) NONE,				/* virtual address	*/
    (PHYS_ADDR) NONE,				/* physical address     */
    MY_MEMORY_SPACE_SIZE,			/* length		*/

    /* initial state mask */

    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,

    /* initial state */

    VM_STATE_VALID_NOT | VM_STATE_WRITABLE | VM_STATE_CACHEABLE
    }
#endif
    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/*
 * sysPhysRamDesc contains one region of RAM that must be mapped at
 * LOCAL_MEM_LOCAL_ADRS..., more regions can be mapped at the
 * discretion of the kernel. Other entries are only used if the BSP
 * discovers a RAM block during system boot.
 */

LOCAL PHYS_MEM_DESC sysPhysRamDesc [] =
    {
    	{
	/* Kernel system memory */

	(VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
	(PHYS_ADDR) (LOCAL_MEM_LOCAL_ADRS),
	LOCAL_MEM_SIZE,		/* may be modified at runtime */
	(VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
	 VM_STATE_MASK_CACHEABLE),
	(VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE)
	},
    };
LOCAL int sysPhysRamDescCount = NELEMENTS(sysPhysRamDesc);

#ifdef INCLUDE_USER_RESERVED_MEMORY
/*
 * sysUserMemDesc is used to provide user reserved memory
 * if INCLUDE_USER_RESERVED_MEMORY is defined.
 */

LOCAL PHYS_MEM_DESC sysUserMemDesc[] =
    {
	{
	(VIRT_ADDR) MEM_DESC_ADDR_KERNEL_ASSIGNED,  /* kernel assigned */
	(PHYS_ADDR) 0,	/* computed at runtime */
	0,		/* computed at runtime */
	(VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
	 VM_STATE_MASK_CACHEABLE),
	(VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE)
	}
    };
LOCAL int sysUserMemDescCount = NELEMENTS(sysUserMemDesc);
#endif /* INCLUDE_USER_RESERVED_MEMORY */

#ifdef INCLUDE_EDR_PM
/*
 * sysPmMemDesc is used to provide persistent memory
 * if INCLUDE_EDR_PM is defined.
 */

LOCAL PHYS_MEM_DESC sysPmMemDesc[] =
    {
	{
	(VIRT_ADDR) MEM_DESC_ADDR_KERNEL_ASSIGNED,	/* kernel assigned */
	(PHYS_ADDR) 0,	/* computed at runtime */
	0,		/* computed at runtime */
	(VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
	 VM_STATE_MASK_CACHEABLE),
	(VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE)
	}
    };
LOCAL int sysPmMemDescCount = NELEMENTS(sysPmMemDesc);
#endif /* INCLUDE_EDR_PM */

#if (defined (INCLUDE_SIMNET) && defined (INCLUDE_NETWORK))
/* global variables retrieved by VxSim before network initialization */

void *	simnetMemAdrs = 0;	/* initiallized by sysPhysMemDescUpdate() */
UINT32	simnetMemSize = SIMNET_MEM_SIZE;
#endif /* INCLUDE_SIMNET && INCLUDE_NETWORK*/

/* locals */

LOCAL char	sysModelStr[SYS_MODEL_STR_LEN] = {EOS};
LOCAL void *	sysUserSmAddr = 0x0;		/* User SM base address */
LOCAL size_t	sysUserSmSize = 0;		/* User SM size */

/* forward declarations */

void		sysHwInit1 (void);
#ifdef	INCLUDE_WRLOAD
LOCAL void	sysAmpInit (void);
#endif	/* INCLUDE_WRLOAD */

/******************************************************************************
*
* sysMemDescGet - return memory descriptors describing memory layout
*
* This routine is used during system boot to describe the memory
* layout to the kernel and included components. It supports
* memory autoconfiguration. It also supports carving memory from
* the first contiguous block of RAM for 32-bit DMA and persistent
* memory. This routine assumes that enough memory is provided
* in the 0th descriptor to support all of these segments.
*
* RETURNS: OK, if a valid PHYS_MEM_DESC is copied to the caller.
*/

STATUS sysMemDescGet
    (
    MEM_DESC_TYPE	memDescType,	/* type of memory being queried */
    int			index,		/* index of memory block for the type */
    PHYS_MEM_DESC *	pMemDesc	/* where to return description */
    )
    {
    PHYS_MEM_DESC *	pDesc;
    int			descCount;

    if (memDescType == MEM_DESC_RAM)
    	{
	pDesc = sysPhysRamDesc;

	descCount = sysPhysRamDescCount;
	}
#ifdef INCLUDE_USER_RESERVED_MEMORY
    else if (memDescType == MEM_DESC_USER_RESERVED_RAM)
    	{
	pDesc = sysUserMemDesc;
	descCount = sysUserMemDescCount;
	}
#endif /* INCLUDE_USER_RESERVED_MEMORY */
#ifdef INCLUDE_EDR_PM
    else if (memDescType == MEM_DESC_PM_RAM)
    	{
	pDesc = sysPmMemDesc;
	descCount = sysPmMemDescCount;
	}
#endif /* INCLUDE_EDR_PM */
    else
    	return ERROR;	/* no support for MEM_DESC_APPL_RAM */

    if (index >= descCount)
    	{
	return ERROR;
	}
	
    if ((pDesc->len == (size_t)DUMMY_LENGTH) || 
    	(pDesc->physicalAddr == (PHYS_ADDR) DUMMY_PHYS_ADDR))
	return ERROR;
	
    /*
     * Copy the descriptor, note that it is OK to return a a descriptor that
     * has a length of 0. This allows the BSP to allocate "empty" entries
     * in the descriptor arrays, and fill them in at run-time without having
     * to alter the logic of this routine.
     */

    *pMemDesc = pDesc[index];
    return OK;
    }

/******************************************************************************
*
* sysMemDescInit - hook for memory auto sizing
*
* This routine is used to perform any adjustments/clean up prior to calling 
* sysMemDescGet().
* 
* RETURNS: N/A
*/

void sysMemDescInit (void)
    {
    LOCAL BOOL memFirstTime = TRUE;

    if (memFirstTime)
    	{
	PHYS_ADDR	physicalAddrEnd;
#ifdef INCLUDE_USER_RESERVED_MEMORY
        size_t		userSize = USER_RESERVED_MEM;
#else
	size_t		userSize = 0;
#endif /* INCLUDE_USER_RESERVED_MEMORY */
#ifdef INCLUDE_EDR_PM
	size_t		pmSize = PM_RESERVED_MEM;
#else
	size_t		pmSize = 0;
#endif /* INCLUDE_EDR_PM */
	
	/* shrink 0th segment by reserved sizes */

	sysPhysRamDesc[0].len = sysMemSize - userSize - pmSize;
	physicalAddrEnd = sysPhysRamDesc[0].physicalAddr +
			  sysPhysRamDesc[0].len;

#ifdef INCLUDE_USER_RESERVED_MEMORY
	/* set physical address and size for user reserved memroy */

	sysUserMemDesc[0].len = userSize;
	sysUserMemDesc[0].physicalAddr = physicalAddrEnd;
	physicalAddrEnd += sysUserMemDesc[0].len;
#endif /* INCLUDE_USER_RESERVED_MEMORY */

#ifdef INCLUDE_EDR_PM
	/* set physical address and size for persistent memroy */

	sysPmMemDesc[0].len = pmSize;
	sysPmMemDesc[0].physicalAddr = physicalAddrEnd;
	physicalAddrEnd += sysPmMemDesc[0].len;
#endif /* INCLUDE_EDR_PM */

	/* first time we are called we patch the descriptors */

	memFirstTime = FALSE;
	}
    }

/*******************************************************************************
*
* sysPhysMemDescUpdate - update sysPhysMemDesc[] array
*
* This routine dynamically updates sysPhysMemDesc[] array according to memory
* configuration. Since VxWorks physical memory size (sysMemSize) can be set
* dynamically using vxsim command line (-memsize option), the sysPhysMemDesc[]
* array must be updated at VxWorks initialization.
*
* WARNING: This routine must be modified when entries are added to
*	   sysPhysMemDesc[] array (See template at the end of this routine).
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

void sysPhysMemDescUpdate (void)
    {
    int		index = 0;
    size_t	memOffset;
#ifdef	VXSIM_HAS_SHARED_MEM
    size_t	vxsimSmOffset;
    void *	sysSmCtrlAddr = 0x0;		/* vxsim SM control area */
#endif	/* VXSIM_HAS_SHARED_MEM */

    /* Update VxWorks Physical Memory size */

    sysPhysMemDesc[index].len = sysMemSize;
    index++;
    memOffset = sysMemSize;

#ifdef	VXSIM_HAS_SHARED_MEM
    /* Update Shared Memory Address space */

    sysSmCtrlAddr = (void *)(LOCAL_MEM_LOCAL_ADRS + sysMemSize);
    sysPhysMemDesc[index].virtualAddr	= (VIRT_ADDR) sysSmCtrlAddr;
    sysPhysMemDesc[index].physicalAddr	= (PHYS_ADDR) sysSmCtrlAddr;
    vxsimSmOffset = VXSIM_SM_CTRL_SIZE;
    index++;

    /* Set Base address of VxSim */

    sysSmAddr = (char *)sysSmCtrlAddr + vxsimSmOffset;

#ifdef	INCLUDE_SM_COMMON
    sysSmAnchorAdrs = (void *) ((char *)sysSmCtrlAddr + vxsimSmOffset);
    vxsimSmOffset += SM_TOTAL_SIZE;
#endif	/* INCLUDE_SM_COMMON */

#ifdef INCLUDE_DSHM
    sysDshmAddr = (void *)((char *)sysSmCtrlAddr + vxsimSmOffset);
    vxsimSmOffset += DSHM_SM_POOL_SIZE;
#endif	/* INCLUDE_DSHM */

#ifdef INCLUDE_MIPC_SM
    sysMipcSmAddr = (void *)((char *)sysSmCtrlAddr + vxsimSmOffset);
    vxsimSmOffset += MIPC_SM_POOL_SIZE;
#endif /* INCLUDE_MIPC_SM */

#ifdef	USER_SM_POOL_SIZE
    sysUserSmAddr = (void *)((char *)sysSmCtrlAddr + vxsimSmOffset);
    sysUserSmSize = USER_SM_POOL_SIZE;
    vxsimSmOffset += USER_SM_POOL_SIZE;
#endif	/* USER_SM_POOL_SIZE */

    memOffset += VXSIM_SM_SIZE;
#endif	/* VXSIM_HAS_SHARED_MEM */

#if (defined (INCLUDE_SIMNET) && defined (INCLUDE_NETWORK))
    /* Update VxSim Networking Address space */

    simnetMemAdrs = (void *)(LOCAL_MEM_LOCAL_ADRS + memOffset);
    sysPhysMemDesc[index].virtualAddr	= (VIRT_ADDR) simnetMemAdrs;
    sysPhysMemDesc[index].physicalAddr	= (PHYS_ADDR) simnetMemAdrs;
    index++;
    memOffset += SIMNET_MEM_SIZE;
#endif  /* INCLUDE_SIMNET && INCLUDE_NETWORK */

    /*
     * WARNING:
     * Add your entries below, following this template.
     */

#if 0
    /* Update my memory space */

    myMemorySpaceAdrs = (PHYS_ADDR)(LOCAL_MEM_LOCAL_ADRS + memOffset);
    sysPhysMemDesc[index].virtualAddr	= myMemorySpaceAdrs;
    sysPhysMemDesc[index].physicalAddr	= myMemorySpaceAdrs;
    index++;

    memOffset += MY_MEMORY_SPACE_SIZE;
#endif
    }

/*******************************************************************************
*
* sysModel - return model name of the system CPU
*
* Use this routine to find the model name of the system CPU.
*
* RETURNS: A pointer to a string describing the OS on which is running VxSim
*
*/

char *sysModel (void)
    {
    if (sysModelStr[0] == EOS)
	return (vxsimHostModel (sysModelStr, SYS_MODEL_STR_LEN));
    else
	return (sysModelStr);
    }

/*******************************************************************************
*
* sysBspRev - return the bsp version/revision identification
*
* This function returns a pointer to a bsp version with the revision.
* for eg. 1.1/<x>. BSP_REV is concatanated to BSP_VERSION to form the
* BSP identification string.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

#ifndef _WRS_CONFIG_LP64
/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Normally, the user specifies the amount of physical memory with the
* parameter LOCAL_MEM_SIZE which belongs to INCLUDE_MEMORY_CONFIG component.
*
* NOTE: Do no adjust LOCAL_MEM_SIZE to reserve memory for application
* use.  See sysMemTop() for more information on reserving memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char *sysPhysMemTop (void)
    {
    return ((char *) (sysMemBaseAdrs + sysMemSize));
    }

/*******************************************************************************
*
* sysMemTop - get the address of the top of VxWorks memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by VxWorks.
*
* The user can reserve memory space by defining the parameter
* USER_RESERVED_MEM which belongs to the INCLUDE_MEMORY_CONFIG component.
* This routine returns the address of the reserved memory area.  The value
* of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*/

char * sysMemTop (void)
    {
    static char * memTop = NULL;

    if ( memTop == NULL )
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM
        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif
        }

    return memTop ;
    }

#endif	/* _WRS_CONFIG_LP64 */

/***************************************************************************
*
* sysRtcTimeGet - read real time clock to obtain correct time
*
* This functions reads the real time clock (from the host where is running
* VxSim).
*
* RETURNS: This routine returns the time since the Epoch (00:00:00 UTC,
* January 1, 1970), measured in seconds or ERROR if it is not possible to get
* the time on the host.
*
* SEE ALSO: ansiTime(1)
*/

time_t sysRtcTimeGet (void)
    {
    return ((time_t) vxsimHostTimeGet ());
    }

#ifdef INCLUDE_VXBUS
/*******************************************************************************
*
* sysDevConnect - Task to do HWIF Post-Kernel Connection
*
* RETURNS: N/A
*/

LOCAL STATUS sysDevConnect (void)
    {
    vxbDevConnect();
    
#ifdef INCLUDE_EXC_TASK
    /*
     * Wait for exception handling package initialization to complete 
     * prior to exit
     */

    {
    int i;
    for (i=0; i<20; i++)
        {
        if (_func_excJobAdd != NULL)
            {
            return OK;
            }
        taskDelay(sysClkRateGet()/2);
        }
    }
#endif /* INCLUDE_EXC_TASK */

    return OK;
    }
#endif /* INCLUDE_VXBUS */

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the board.
* It is normally called from usrInit() in prjConfig.c.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {
    /* Update sysPhysMemDesc[] array according to configuration */

    sysPhysMemDescUpdate ();

    /* initialize boot line */

    bootStringToStruct (sysBootLine, &sysBootParams);

     /*
      * save the bootline into NVRAM. The NVRAM is not the default way used
      * to save the boot parameters. It is used to avoid startup problems
      * when using INCLUDE_BOOT_LINE_INIT component.
      */

    sysNvRamSet (BOOT_LINE_ADRS, BOOT_LINE_SIZE, 0);

    /* save the user group ID */

#if (CPU != SIMNT)
    sysUserGid = (int) vxsimHostGetgid ();
#endif /* CPU != SIMNT */

    /* Set processor number */

    sysProcNumSet (sysBootParams.procNum);

#ifndef _WRS_CONFIG_DELAYED_HW_INIT
    sysHwInit1 ();
#endif /* _WRS_CONFIG_DELAYED_HW_INIT */

#ifdef	INCLUDE_WRLOAD
    sysAmpInit ();
#endif
    }

/*******************************************************************************
*
* sysHwInit1 - additional system configuration and initialization 
*
* This routine performs any additional configuration necessary.
* It is called from usrRoot() prior the enabling of interrupts.
*
* RETURNS: N/A
*/

void sysHwInit1 (void)
    {
#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit ();
#endif

    /* initializes the serial devices */

    sysSerialHwInit ();
    
#ifdef INCLUDE_NETWORK
    /* Initialize END device table */

    sysNetEndInit ();
#endif	/* INCLUDE_NETWORK */
    }

/*******************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot()--which services ^X--and bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* RETURNS: ERROR, always.
*/

STATUS sysToMonitor
    (
    int startType	/* type of start process  */
    )
    {
    /* print exception message if present */

    if (*(char *) EXC_MSG_ADRS != EOS)
    	{
	vxsimHostPrintf ((char *)EXC_MSG_ADRS);

	/* indicate exception message is old */

	*(char *) EXC_MSG_ADRS = EOS;
	}


    vxsimHostReboot (startType);

    return (ERROR);
    }

/*******************************************************************************
*
* sysHwInit2 - configure and initialize additional system features
*
* This routine connects system interrupts and does any additional
* configuration necessary. It is called from usrRoot() in prjConfig.c
* after the multitasking kernel has started.
*
* RETURNS: N/A
*/

void sysHwInit2 (void)
    {
#ifdef	INCLUDE_ANSI_TIME
    struct timespec curTmSpec;
#endif	/* INCLUDE_ANSI_TIME */

#ifdef INCLUDE_VXBUS
    vxbDevInit ();
    taskSpawn("tDevConn", 11, 0, 10000,
              sysDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
#endif

    /* connect serial interrupt */

    sysSerialHwInit2 ();

#ifdef	INCLUDE_ANSI_TIME
    /* Initialize the posix system clock from the RTC */

    /* read clock (GMT) */

    curTmSpec.tv_sec = sysRtcTimeGet ();
    curTmSpec.tv_nsec = 0;

    /* set clock time */

    clock_settime (CLOCK_REALTIME, &curTmSpec);
#endif	/* INCLUDE_ANSI_TIME */
    }

/******************************************************************************
*
* sysProcNumGet - get processor number
*
* This routine returns the processor number for the target, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for this target.
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    /* If sysProcNumSet() has not yet been called, return default value */

    if (sysProcNum == -1)
    	{
	bootStringToStruct (sysBootLine, &sysBootParams);
    	return (sysBootParams.procNum);
	}
    else
	return (sysProcNum);
    }

/******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the target.
* Processor numbers should be unique on a host.
*
* NOTE
* The shared memory segment is also initialized here if component
* INCLUDE_SM_COMMON is set. This applies also to distributed shared memory.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int procNum		/* processor number */
    )
    {
    static BOOL	smIsInitialized = FALSE;
    if (!smIsInitialized)
        {
#ifdef	VXSIM_HAS_SHARED_MEM
	sysSmSize = VXSIM_SM_SIZE - VXSIM_SM_CTRL_SIZE;
	(void) vxsimHostShMemInit ((char *)sysSmAddr - VXSIM_SM_CTRL_SIZE,
				   sysSmSize, &sysSmLevel);
	smIsInitialized = TRUE;
#endif /* VXSIM_HAS_SHARED_MEM */
        }

    sysProcNum = procNum;
    }

/***************************************************************************
*
* sysIntDisable - disable a bus interrupt level
*
* This routine disables a specified bus interrupt level.
*
* RETURNS: OK, or ERROR if <intLevel> is not in the range.
*
* SEE ALSO: sysIntEnable()
*/

STATUS sysIntDisable
    (
    int intLevel	/* interrupt level to disable */
    )
    {
    return (OK);
    }

/***************************************************************************
*
* sysIntEnable - enable a bus interrupt level
*
* This routine enables a specified bus interrupt level.
*
* RETURNS: OK, or ERROR if <intLevel> is not in the range.
*
* SEE ALSO: sysIntDisable()
*/

STATUS sysIntEnable
    (
    int intLevel	/* interrupt level to enable */
    )
    {
    return (OK);
    }

#ifndef _WRS_CONFIG_LP64
/******************************************************************************
*
* sysBusIntAck - acknowledge a bus interrupt
*
* This routine acknowledges a specified bus interrupt.
*
* NOTE: This routine has no effect.
*
* RETURNS: NULL
*/

int sysBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge */
    )
    {
    return (0);
    }

/******************************************************************************
*
* sysBusIntGen - generate a bus interrupt
*
* This routine generates an bus interrupt for a specified level with a
* specified vector.
*
* RETURNS: OK or ERROR.
*/

STATUS sysBusIntGen
    (
    int level,          /* bus interrupt level to generate          */
    int vector          /* interrupt vector to return (0-255)       */
    )
    {
    /* 
     * If MIPC is included in VxWorks image, use the IPI mechanism. MIPC 
     * interrupt handler will act as a multiplexer to handle SM_OBJ interrupts.
     * If MIPC is not included, use old mechanism.
     * Note that in both cases, the level parameter does contain the CPU id.
     */

#ifdef INCLUDE_MIPC_SM
    return (vxsimHostIpiEmit((1 << level), MIPC_SM_NODE_IRQ));
#else
    return (vxsimHostBusIntGen (level, vector));
#endif
    }

/******************************************************************************
*
* sysMailboxConnect - connect a routine to the mailbox interrupt
*
* This routine specifies the interrupt service routine to be called at each
* mailbox interrupt.
*
* NOTE: This routine has no effect, since the hardware does not support mailbox
* interrupts.
*
* RETURNS: ERROR, since there is no mailbox facility.
*
* SEE ALSO: sysMailboxEnable()
*/

STATUS sysMailboxConnect
    (
    FUNCPTR routine,    /* routine called at each mailbox interrupt */
    int     arg         /* argument with which to call routine      */
    )
    {
    return (ERROR);
    }

/******************************************************************************
*
* sysMailboxEnable - enable the mailbox interrupt
*
* This routine enables the mailbox interrupt.
*
* NOTE: This routine has no effect, since the hardware does not support mailbox
* interrupts.
*
* RETURNS: ERROR, since there is no mailbox facility.
*
* SEE ALSO: sysMailboxConnect()
*/

STATUS sysMailboxEnable
    (
    char * mailboxAdrs           /* mailbox address */
    )
    {
    return (ERROR);
    }

/******************************************************************************
*
* sysBusTas - test and set a location across the bus
*
* This routine performs a test-and-set instruction.
*
* NOTE
* This routine is similar to vxTas().
*
* RETURNS:
* TRUE if the previous value had been zero, or FALSE if the value was
* set already.
*
* SEE ALSO: vxTas()
*/

BOOL sysBusTas
    (
    char *adrs          /* address to be tested and set */
    )
    {
    return (vxTas (adrs));
    }

/*******************************************************************************
*
* sysLocalToBusAdrs - convert a local address to a bus address
*
* This routine gets the bus address that accesses a specified local
* memory address.
*
* NOTE: This routine just returns the same address.  For the simulator
* BSPs we can only simulate a VME bus.  The input address is assumed to be
* the same for the local cpu and the simulated VME bus.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
* SEE ALSO: sysBusToLocalAdrs()
*/

STATUS sysLocalToBusAdrs
    (
    int adrsSpace,      /* bus address space in which busAdrs resides */
    char *localAdrs,    /* local address to convert                   */
    char **pBusAdrs     /* where to return bus address                */
    )
    {
    if ((long)localAdrs < (long)LOCAL_MEM_LOCAL_ADRS ||
    	localAdrs >= sysMemTop ())
        return (ERROR);

    *pBusAdrs = localAdrs;

    return (OK);
    }

/*******************************************************************************
*
* sysBusToLocalAdrs - convert a bus address to a local address
*
* This routine gets the local address that accesses a specified bus address.
*
* NOTE: This routine just returns the same address.  For the simulator
* BSPs we can only simulate a VME bus.  The input address is assumed to be
* a local cpu address, not a real VME address.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
    (
    int adrsSpace,      /* bus address space in which busAdrs resides */
    char *busAdrs,      /* bus address to convert                     */
    char **pLocalAdrs   /* where to return local address              */
    )
    {
    *pLocalAdrs = (char *) busAdrs;
    return (OK);
    }
#endif	/* _WRS_CONFIG_LP64 */
#if	CPU == SIMNT
/*******************************************************************************
*
* sysPipeIntVecGet - get interrupt vector for pipe backend
*
* This routine gets the interrupt vector number for pipe backend.
*
* RETURNS: interrupt vector number
*
* ERRNO: N/A
*
*/

int sysPipeIntVecGet (void)
    {
    return (MAILSLOT_INT);
    }
#endif	/* CPU == SIMNT */

/***************************************************************************
*
* sysNvRamGet - get contents of non-volatile RAM
*
* This routine copies the contents of non-volatile memory into a specified
* string.  The string will be terminated with an EOS. For VxSim, the non
* volatile RAM support is based on a host file.
*
* RETURNS: OK, or ERROR if access is outside the non-volatile RAM range.
*
* SEE ALSO: sysNvRamSet()
*/

STATUS sysNvRamGet
    (
    char *	string,	/* where to copy non-volatile RAM    */
    int 	strLen,	/* maximum number of bytes to copy   */
    int 	offset	/* byte offset into non-volatile RAM */
    )
    {
    return ((STATUS) vxsimHostSysNvRamGet (string, strLen, offset +
    	NV_BOOT_OFFSET, NV_RAM_SIZE));
    }

/***************************************************************************
*
* sysNvRamSet - write to non-volatile RAM
*
* This routine copies a specified string into non-volatile RAM. For VxSim,
* the non-volatile RAM support is based on a host file.
*
* RETURNS: OK, or ERROR if access is outside the non-volatile RAM range.
*
* SEE ALSO: sysNvRamGet()
*/

STATUS sysNvRamSet
    (
    char *	string,	/* string to be copied into non-volatile RAM */
    int 	strLen,	/* maximum number of bytes to copy           */
    int 	offset	/* byte offset into non-volatile RAM         */
    )
    {
    return ((STATUS) vxsimHostSysNvRamSet (string, strLen, offset +
    	NV_BOOT_OFFSET, NV_RAM_SIZE));
    }

/***************************************************************************
*
* sysUserSmInfoGet - get information about VxSim user shared memory
*
* This routine gets information (size and address) about the VxSim user
* shared memory. If no user shared memory is configured in the system,
* the size returned is set to 0.
* 
* RETURNS: N/A
*/

void sysUserSmInfoGet
    (
    size_t *	pSize,	/* where to return user shared memory size in bytes */
    void **	pAddr	/* where to return user shared memory address */
    )
    {
    *pSize = sysUserSmSize;
    *pAddr = sysUserSmAddr;
    }

#ifdef _WRS_CONFIG_SMP
/******************************************************************************
*
* sysCpuNumGet - routine returns the processor number for the CPU
*
* RETURNS: The processor number for the CPU
*/

int sysCpuNumGet(void)
    {
    return ((int) vxCpuIndexGet());
    }

/******************************************************************************
 *
 * sysCpuEnable - enable a multi core CPU
 *
 * This routine brings a CPU out of reset
 *
 * RETURNS:  0 - Success
 *          -1 - Fail
 *
 */
                                                                                
int sysCpuEnable
    (
    int			cpuNum,
    WIND_CPU_STATE *	pCpuState
    )
    {
    return (vxsimHostCpuEnable (cpuNum, (INSTR *)pCpuState->regs.reg_pc,
    				pCpuState->regs.reg_sp));
    }

/******************************************************************************
*
* sysCpuStart - set up context to run specified function
*
* RETURNS: N/A
*/

void sysCpuStart
    (
    int		cpu,		/* cpu to start */
    UCHAR *	entry,		/* cpu entry point */
    ULONG 	stackPtr	/* cpu stack pointer */
    )
    {
    WIND_CPU_STATE	cpuState;
    _Vx_usr_arg_t	pArgs[2] = {1, 0};

    _sigCtxSetup (&cpuState.regs, (void *) stackPtr, (VOIDFUNCPTR)entry, pArgs);
    vxCpuStart (cpu, &cpuState);
    }

/******************************************************************************
*
* sysCpuAvailableGet - return the  number of CPUs available for SMP 
*
* This routine returns the number of CPUs that are available to be used by
* VxWorks for the SMP kernel. On VxSim this is the number of cpu specified
* through -ncpu option or by default the value of <sysCpuConfigured>.
*
* RETURNS: The number of the CPUs available
*/

UINT32 sysCpuAvailableGet (void)
    {
    return vxCpuAvailableGet ();
    }

#endif /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_WRLOAD
/*****************************************************************************
*
* sysAmpCpuEnable - Starts core1 executing code at entryPt
*
* This routine performs the vxsim specific code to start the secondary
* core.
*
* This function is required for wrload.
*
* RETURNS: NONE
*
*/

STATUS sysAmpCpuEnable
    (
    WRLOAD_ADDR_T	entryPt,
    WRLOAD_ID		cpu
    )
    {
    static FUNCPTR	func = NULL;
    if (func == NULL)
    	func = vxsimHostProcAddrGet ("vxsimAmpCpuEnable");
    if (func == NULL)
	return ERROR;
    return (vxsimHostProcCall (func, (_Vx_usr_arg_t)entryPt,
    			(_Vx_usr_arg_t) cpu, 0, 0, 0, 0, 0, 0, 0));
    }

/*****************************************************************************
*
* sysAmpCpuPrep - Prepare cpuId for wrload download and start of AMP image
*
* This function is used by wrload to make a core ready for wrload to
* download and/or start a new image.  The state of the target cpu
* after this call is not specified, and is OS/arch/CPU specific.  If
* return is OK, then wrload should expect to succeed; if ERROR, then
* wrload will abort.
*
* arg argument currently unused, expected to provide finer grain control
* in the future.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysAmpCpuPrep
    (
    UINT32	cpuId,		/* CPU id */
    void *	arg		/* file to load */
    )
    {
#define	READ_BUF_SIZE	500
    static FUNCPTR	create_funcptr = NULL;
    static FUNCPTR	prep_funcptr = NULL;
    static FUNCPTR	probe_funcptr = NULL;
    int		tgtFd, hostFd;
    ssize_t	ix;
    char	buffer [READ_BUF_SIZE];
    STATUS	status;

    if (create_funcptr == NULL)
	create_funcptr = vxsimHostProcAddrGet ("vxsimAmpImageFileCreate");
    if (create_funcptr == NULL)
	return (ERROR);
    hostFd = vxsimHostProcCall (create_funcptr,
    		(_Vx_usr_arg_t)cpuId, 0,0,0,0,0,0,0,0);

    if (hostFd < 0)
    	return (ERROR);

    if ((tgtFd = open (arg, O_RDONLY, 0)) == -1)
    	{
	vxsimHostClose (hostFd);
	return (ERROR);
	}

    while ((ix = fioRead (tgtFd, buffer, READ_BUF_SIZE)) > 0)
	{
	if (vxsimHostWrite (hostFd, buffer, ix) != ix)
	    {
	    vxsimHostClose (hostFd);
	    close (tgtFd);
	    return (ERROR);
	    }
    	}
 
    vxsimHostClose (hostFd);
    close (tgtFd);

    if (prep_funcptr == NULL)
	prep_funcptr = vxsimHostProcAddrGet ("vxsimAmpCpuPrep");
    if (prep_funcptr == NULL)
	return (ERROR);
    status = vxsimHostProcCall (prep_funcptr,
    			(_Vx_usr_arg_t) cpuId, 0,0,0,0,0,0,0,0);

    if (status == OK)
    	{
	int	ix = 0;

	if (probe_funcptr == NULL)
	    probe_funcptr = vxsimHostProcAddrGet ("vxsimAmpCpuProbe");
	if (probe_funcptr == NULL)
	    return (ERROR);
	do
	    {
	    ix ++;
	    taskDelay (sysClkRateGet ());
	    status = vxsimHostProcCall (probe_funcptr, (_Vx_usr_arg_t) cpuId,
	    				(_Vx_usr_arg_t)(ix == 60 ? 1 : 0),
					0, 0, 0, 0, 0, 0, 0);
	    } while (ix < 60 && status != OK);
	 }
    if (status != OK)
	printErr ("Error: Unable to configure secondary core.\n");

    return (status);
    }

/*****************************************************************************
*
* sysAmpCpuMemMap - map secondary core memory
*
* This routine is used to map a secondary core piece of memory in the current
* memory space.
*
* RETURNS: mapped address or NULL
*
* NOMANUAL
*/

VIRT_ADDR sysAmpCpuMemMap
    (
    UINT32	cpuId,		/* CPU ID */
    PHYS_ADDR	paddr,		/* Address to map */
    size_t	nbytes		/* number of bytes to unmap */
    )
    {
    return 0;
    }

/*****************************************************************************
*
* sysAmpCpuMemUnmap - unmap secondary core memory
*
* This routine is used to unmap a secondary core memory prevoisly mapped with
* sysAmpCpuMemMap.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void sysAmpCpuMemUnmap
    (
    UINT32	cpuId,		/* CPU ID */
    VIRT_ADDR	addr,		/* address to unmap */
    size_t	nbytes		/* number of bytes to unmap */
    )
    {
    }

/******************************************************************************
*
* sysAmpCpuMemOpen - Prepare access to memory of remote core
*
* This routine is a BSP specific routine which prepares access to
* memory of remote core specified by CPU index.
* 
* RETURNS: OK or ERROR if memory could not be accessed.
*/

LOCAL STATUS sysAmpCpuMemOpen 
    (
    WRLOAD_ID 	  cpuIx		/* CPU index */
    )
    {
    static FUNCPTR	func = NULL;
    STATUS		status;

    if (func == NULL)
	func = vxsimHostProcAddrGet ("vxsimAmpCpuMemOpen");
    if (func == NULL)
	return (ERROR);

    status = vxsimHostProcCall (func, (_Vx_usr_arg_t) cpuIx,
	    				0, 0, 0, 0, 0, 0, 0, 0);
    return status;
    }

/******************************************************************************
*
* sysAmpCpuMemClose - Close access to memory of remote core
*
* This routine is a BSP specific routine which closes access to the 
* memory of the specified CPU index.
* 
* RETURNS: OK or ERROR if function has failed
*/

LOCAL STATUS sysAmpCpuMemClose 
    (
    WRLOAD_ID 	  cpuIx		/* CPU index */
    )
    {
    static FUNCPTR	func = NULL;
    STATUS		status;

    if (func == NULL)
	func = vxsimHostProcAddrGet ("vxsimAmpCpuMemClose");
    if (func == NULL)
	return (ERROR);

    status = vxsimHostProcCall (func, (_Vx_usr_arg_t) cpuIx,
	    				0, 0, 0, 0, 0, 0, 0, 0);
    return status;
    }

/******************************************************************************
*
* sysAmpCpuMemWrite - Writes to destination address of the secondore core
*
* This routine is a BSP specific routine which writes from 'buffer' to
* the destination memory location starts at 'address' so many bytes as 'size'
* for the specified CPU index.
* 
* RETURNS: returns the amount of bytes written successfully.
*/

LOCAL ssize_t sysAmpCpuMemWrite 
    (
    WRLOAD_ID 	  cpuIx,	/* CPU index */
    WRLOAD_ADDR_T address,      /* destination address */ 
    void*         buffer,	/* source address */	
    size_t        size		/* size to write */
    )
    {
    static FUNCPTR	func = NULL;

    if (func == NULL)
	func = vxsimHostProcAddrGet ("vxsimAmpCpuMemWrite");
    if (func == NULL)
	return (-1);

    return (vxsimHostProcCall (func, (_Vx_usr_arg_t) cpuIx,
	    				(_Vx_usr_arg_t) address,
					(_Vx_usr_arg_t) buffer,
					(_Vx_usr_arg_t)size, 0, 0, 0, 0, 0));
    }

/******************************************************************************
*
* sysAmpCpuMemFill - Fill destination address of the secondary core
*
* This routine is a BSP specific routine which is used to <size> bytes
* of the secondary core memory that starts at <address>.
* 
* RETURNS: returns the amount of bytes filled successfully.
*/

LOCAL ssize_t sysAmpCpuMemFill
    (
    WRLOAD_ID	cpuIx,		/* CPU index */
    WRLOAD_ADDR_T address,      /* destination address */
    char          pattern,	/* filling pattern */
    size_t        size		/* size of section to fill */
    )
    {
    static FUNCPTR	func = NULL;

    if (func == NULL)
	func = vxsimHostProcAddrGet ("vxsimAmpCpuMemFill");
    if (func == NULL)
	return (-1);

    return (vxsimHostProcCall (func, (_Vx_usr_arg_t) cpuIx,
	    				(_Vx_usr_arg_t) address,
					(_Vx_usr_arg_t) pattern,
					(_Vx_usr_arg_t)size, 0, 0, 0, 0, 0));
    }

/******************************************************************************
*
* sysAmpCpuMemRead - Reads from the the secondary core target memory
*
* This routine is a BSP specific routine which reads from the secondary
* core 'address' and load it into 'buffer'. 
* 
* RETURNS: returns the amount of bytes read successfully
*/

LOCAL ssize_t sysAmpCpuMemRead 
    (
    WRLOAD_ID 	  cpuIx,	/* CPU index */
    WRLOAD_ADDR_T address,      /* source address */ 
    void *        buffer,	/* destination address */	
    size_t        size		/* read size */
    )
    {
    static FUNCPTR	func = NULL;
    STATUS		status;

    if (func == NULL)
	func = vxsimHostProcAddrGet ("vxsimAmpCpuMemRead");
    if (func == NULL)
	return (-1);

    return (vxsimHostProcCall (func, (_Vx_usr_arg_t) cpuIx,
	    				(_Vx_usr_arg_t) address,
					(_Vx_usr_arg_t) buffer,
					(_Vx_usr_arg_t) size, 0, 0, 0, 0, 0));
    }

/******************************************************************************
*
* sysAmpInit - Initialize AMP support in BSP
*
* This routine initializes AMP support (wrload function pointers) in the
* BSP.
* 
* RETURNS: N/A
*
* \NOMANUAL
*/

LOCAL void sysAmpInit (void)
    {
    WRLOAD_CPU_RTN rtnDesc;
    
    /* Register BSP-specific routines for memory-access with wrload */

    rtnDesc.open = sysAmpCpuMemOpen;
    if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_OPEN, rtnDesc) == ERROR)
    	goto error;
    rtnDesc.close = sysAmpCpuMemClose;
    if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_CLOSE, rtnDesc) == ERROR)
    	goto error;
    rtnDesc.write = sysAmpCpuMemWrite;
    if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_WRITE, rtnDesc) == ERROR)
    	goto error;
    rtnDesc.read = sysAmpCpuMemRead;
    if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_READ, rtnDesc) == ERROR)
    	goto error;
    rtnDesc.fill = sysAmpCpuMemFill;
    if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_FILL, rtnDesc) == ERROR)
    	goto error;
    rtnDesc.prepare = sysAmpCpuPrep;
    if (wrloadCpuRtnSet (WRLOAD_CPU_PREPARE, rtnDesc) == ERROR)
    	goto error;
    rtnDesc.enable = sysAmpCpuEnable;
    if (wrloadCpuRtnSet (WRLOAD_CPU_ENABLE, rtnDesc) == ERROR)
    	goto error;
    return;
error:
    vxsimHostPrintf ("ERROR: Unable to initialize BSP AMP support.\n");
    }
#endif /* INCLUDE_WRLOAD */

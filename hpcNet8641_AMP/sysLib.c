/* sysLib.c - MPC8641 system-dependent library */

/*
 * Copyright (c) 2006-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03o,14nov12,d_l  fix the AMP device filter. (WIND00387606)
03n,07jan11,hl1  change vxIpiEmit to vxIpiPhysEmit. (WIND00248208)
03m,19oct10,pch  CQ213444: ensure both CPUs' D-caches are flushed during SMP
		 sysToMonitor()
03l,28jul10,liu  flush L2 cache when reboot.(WIND00213444)
03k,24mar10,jc0  add a header file 'cpuset.h'.
03j,09mar10,jc0  fix a defect WIND00153195.
03i,10sep09,x_z  Replace sysSvrGet() with vxSvrGet() of vxALib.s
03h,15apr09,rgo  Replace logMsg with logMsg function pointer(wind00121177)
03g,17mar09,rgo  Introduce SM_BASE_ADRS for D BAT2 definition(wind00145934)
03f,24feb09,x_z  Remove gnu compiler warning.(WIND00098576)
03e,13nov08,kab  WIND00143297 - Fix build of CPU1 for gnu
03d,06nov08,pmr  WIND00140224: shutdown ints on core1 for wrload
03c,27oct08,pmr  Fix WIND00137387: conditionalize BAT for upper memory
03b,23oct08,kab  Update sysAmpCpuPrep to new sig per design mod
03a,15oct08,kab  Add sysAmpCpuPrep for wrload/multios support.
02z,17oct08,wap  Correct PCI interrupt routing routines, fix legacy PCI
		 interrupts for PCIe and 32-bit PCI slots (WIND00136938)
02y,04sep08,dtr  Add device fliter and sysAmpCPuStatusGet/Set.
02x,03sep08,dtr  Change startcore to sysAmpCpuEnable.
02w,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
02v,13aug08,dtr  Add support for wrload.
02u,08jul08,pmr  support ULI bridge for PCI
02t,23apr08,dtr  Remove hard code IPI enable - WIND00121614
02s,17apr08,to   make BAT area not user-accessible
02r,26oct07,to   added CPU1_INIT_START_ADR (WIND00107927)
02q,03oct07,dtr  Fix coreShow.
02p,20sep07,pmr  fix apigen error
02o,10sep07,pch  CQ85374: fix DBAT4 setting
02n,10sep07,h_k  removed vxbIntToCpuRoute() call from sysCpuEnable().
		 (CQ:104081)
02m,30aug07,mmi  remove cpcLibP.h
02l,17aug07,h_k  fixed a potential system lock issue for timebase sync.
		 (CQ:100114).
		 changed sysMsDelay() to call vxbMsDelay().
02k,23jul07,h_k  added DRV_TIMER_DEC_PPC is disabled case. (CQ:99195)
		 fixed time base sync. (CQ:99192)
02j,16jul07,pmr  remove direct calls to EPIC
02i,16jul07,ami  removed cmdLine.c file
02h,11jul07,dtr  Make PCI optional in build.
02g,06jul07,pch  remove sysCpuNumGet
02g,09jul07,dtr  Update AMP bootline mod.
02f,30may07,dcc  added SDA initialization in sysCpu1Loop().
02e,27may07,bwa  fixed usage of CPU1CHECK when sending IPI in sysBusIntGen().
02d,25may07,dtr  Change DUAL_CORE to INCLUDE_AMP. Modify boot mode for second
		 core Defect 95399.
02c,11may07,mmi  removed vxCpuEnabled update from sysCpuEnable(), sysFlags
		 sysBootHost and sysBootFile
02b,01may07,pmr  VxBus epic cpu1 check method
02b,30apr07,pch  fix UP build
02a,12apr07,pch  add AMP/SMP detection and support for vxbOpenPicTimer
01z,29mar07,to   SMP: enable L2 cache for 2nd core
01y,05mar07,dtr  Fix AMP mode.
01x,20feb07,dtr  Remove PCI from second CPU.
01w,07feb07,dtr  Added PCI support.
01v,07mar07,to   SMP: sysToMonitor to use cacheArchDisableFromMmu
01u,15feb07,kk   added include cpcLibP.h to avoid build errors in project
01t,12jan07,pch  add time base synchronization & sysCpuAvailableGet() for SMP;
		 remove historical baggage.
01s,12dec06,tor  ifdef IPI and MMU SMP calls
01r,17nov06,pch  handle SMP warm reboot
01q,14nov06,tor  Integrated VxBus interrupt controller
01p,15nov06,cjj  sysToMonitor now resets both CPUs for SMP
		 (temporary measure).  Fixed compiler warning.
01o,09nov06,pcs  Remove setting of vxCpuConfigured in sysHwInit.
01n,26oct06,to   update sysCpuEnable according to the HLD
01m,20oct06,mmi  update vxCpuEnabled
01l,06oct06,to   made vector area cacheable and coherent
01k,21sep06,to   added sysCpuInit()
01j,08sep06,dtr  Mmod to PLL tables.
01i,07aug06,dtr  Support DUAL_CORE over smEnd.
01h,12jun06,kk   remerged with ITER9_FRZ33.
01g,06jun06,kk   rolled back merge from FRZ33.
01f,28apr06,dtr  Add Pixis support for dual reboot and fix sysBusIntGen for IPI.
01e,18apr06,dtr  Adding IPI support.
01d,09may06,pch  SPR 117691: cache/MMU global variable cleanup
01c,05may06,mig  Add CPU1 IPI support
01b,04apr06,mig  added SMP support
01a,05feb06,dtr  written from SPS/Motorola 7448 02a.
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    flashMem.c          - 29F040 flash memory device driver.
    sysCacheLib.s       - L1 and L2 cache lock support

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdio.h>
#include <vme.h>
#include <memLib.h>
#include <cacheLib.h>
#include <sysLib.h>
#include "config.h"
#include <string.h>
#include <intLib.h>
#include <logLib.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <vmLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#ifdef INCLUDE_CACHE_L2
#include "sysL2Cache.h"
#endif	/* INCLUDE_CACHE_L2 */

#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>

#ifdef INCLUDE_PCI_BUS
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciIntLib.h>
    #include <drv/pci/pciAutoConfigLib.h>
    #include "mot85xxPci.h"
#endif /* INCLUDE_PCI_BUS */

#if defined (INCLUDE_ALTIVEC)
#include <altivecLib.h>
#endif	/* INCLUDE_ALTIVEC */

#ifdef _WRS_CONFIG_SMP
#include <cpuset.h>
#include <private/kernelLibP.h>		/* KERNEL_ENTERED_ME() */
#ifndef VX_CPC_ASYNC
#define VX_CPC_ASYNC	2
typedef void   (*CPC_FUNC) (void *, int);
IMPORT void cpcInvoke (cpuset_t, CPC_FUNC, void *, int, int);
#endif	/* VX_CPC_ASYNC */
#endif	/* _WRS_CONFIG_SMP */

#ifdef _WRS_VX_AMP
#include <vxIpiLib.h>
IMPORT int sysStartType;
#endif /* _WRS_VX_AMP */

IMPORT int       (* _func_altivecProbeRtn) () ;

/*
 * This must be updated after the bore bring-up specified in the SMP HLD
 * implemented.
 */

IMPORT cpuset_t  vxCpuEnabled;

/* defines */

#define ZERO		0
#define SYS_MODEL	"HPC-NET - Freescale MPC8641D"


/* globals */

/*
 * sysBatDesc[] is used to initialize the block address translation (BAT)
 * registers within the PowerPC 603/604 MMU.  BAT hits take precedence
 * over Page Table Entry (PTE) hits and are faster.  Overlap of memory
 * coverage by BATs and PTEs is permitted in cases where either the IBATs
 * or the DBATs do not provide the necessary mapping (PTEs apply to both
 * instruction AND data space, without distinction).
 *
 * The primary means of memory control for VxWorks is the MMU PTE support
 * provided by vmLib and cacheLib.  Use of BAT registers will conflict
 * with vmLib support.  User's may use BAT registers for i/o mapping and
 * other purposes but are cautioned that conflicts with caching and mapping
 * through vmLib may arise.  Be aware that memory spaces mapped through a BAT
 * are not mapped by a PTE and any vmLib() or cacheLib() operations on such
 * areas will not be effective, nor will they report any error conditions.
 *
 * Note: BAT registers CANNOT be disabled - they are always active.
 * For example, setting them all to zero will yield four identical data
 * and instruction memory spaces starting at local address zero, each 128KB
 * in size, and each set as write-back and cache-enabled.  Hence, the BAT regs
 * MUST be configured carefully.
 *
 * With this in mind, it is recommended that the BAT registers be used
 * to map LARGE memory areas external to the processor if possible.
 * If not possible, map sections of high RAM and/or PROM space where
 * fine grained control of memory access is not needed.  This has the
 * beneficial effects of reducing PTE table size (8 bytes per 4k page)
 * and increasing the speed of access to the largest possible memory space.
 * Use the PTE table only for memory which needs fine grained (4KB pages)
 * control or which is too small to be mapped by the BAT regs.
 *
 * The BAT configuration for 4xx/6xx-based PPC boards is as follows:
 * All BATs point to PROM/FLASH memory so that end customer may configure
 * them as required.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */


UINT32 sysBatDesc [] =
    {

    /* I BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M | _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT),

    /* I BAT 1 */

    0, 0,

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,

    /* D BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M | _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT),

    /* D BAT 1 */

    ((CCSBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M | _MMU_UBAT_VS),
    ((CCSBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 */

#ifdef _WRS_VX_AMP
    ((SM_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((SM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_MEM_COHERENT),
#else	/* _WRS_VX_AMP */
    0, 0,
#endif	/* _WRS_VX_AMP */

    /* D BAT 3 */
#ifdef INCLUDE_PCI_BUS
    ((PCI_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((PCI_MEM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED)
#else	/* INCLUDE_PCI_BUS */
    0, 0
#endif	/* INCLUDE_PCI_BUS */

/* These entries are for the I/D BAT's (4-7) on the MPC755/745x/8641.
   They should be defined in the following order.
   IBAT4U, IBAT4L, IBAT5U, IBAT5L, IBAT6U, IBAT6L, IBAT7U, IBAT7L,
   DBAT4U, DBAT4L, DBAT5U, DBAT5L, DBAT6U, DBAT6L, DBAT7U, DBAT7L,
*/

    ,
    /* I BAT 4 */

    0, 0,

    /* I BAT 5 */

    0, 0,

    /* I BAT 6 */

    0, 0,

    /* I BAT 7 */

    0, 0,

    /* D BAT 4 */

    0, 0,

    /* D BAT 5 */
#ifdef INCLUDE_PCI_BUS
    ((PCI_MEM_ADRS2 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((PCI_MEM_ADRS2 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#else	/* INCLUDE_PCI_BUS */
    0, 0,
#endif	/* INCLUDE_PCI_BUS */

    /* D BAT 6 */

    0, 0,

    /* D BAT 7 */

    0, 0

    };

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, memory mapped
 * VME A16 space and local PROM/FLASH are set here.
 *
 * All entries in this table both addresses and lengths must be page aligned.
 *
 * PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
 * integer powers of two based on amount of memory to be mapped and a
 * minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
 * for 32-bit PowerPCs are:
 *
 * Total mapped memory		Page Table size
 * -------------------		---------------
 *        8 Meg			     64 K
 *       16 Meg			    128 K
 *       32 Meg			    256 K
 *       64 Meg			    512 K
 *      128 Meg			      1 Meg
 *	.				.
 *	.				.
 *	.				.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {
    {
    /* Vector Table and Interrupt Stack */
    /* Must be sysPhysMemDesc [0] to allow adjustment in sysHwInit() */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
    | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE
    | VM_STATE_MEM_COHERENCY
    },

    {
    /* Must be sysPhysMemDesc [1] to allow adjustment in sysHwInit() */

    (VIRT_ADDR) RAM_LOW_ADRS,
    (PHYS_ADDR) RAM_LOW_ADRS,
    LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE -  RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
    | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE
    | VM_STATE_MEM_COHERENCY
    }

#if (defined (INCLUDE_PCI) || defined (INCLUDE_PCI_BUS))
    ,
    {
    (VIRT_ADDR) PCI_MEM_ADRS,
    (PHYS_ADDR) PCI_MEM_ADRS,
    PCI_MEM_SIZE,
	VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
	| VM_STATE_MASK_GUARDED,
	VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
	| VM_STATE_GUARDED
    }
    ,
    {
    (VIRT_ADDR) PCI_MEMIO_ADRS,
    (PHYS_ADDR) PCI_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
	VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
	| VM_STATE_MASK_GUARDED,
	VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
	| VM_STATE_GUARDED
    }
    ,
    {
    (VIRT_ADDR) PCI_IO_ADRS,
    (PHYS_ADDR) PCI_IO_ADRS,
    PCI_IO_SIZE,
	VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
	| VM_STATE_MASK_GUARDED,
	VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
	| VM_STATE_GUARDED
    }

    ,
    {
    (VIRT_ADDR) PCI_MEM_ADRS2,
    (PHYS_ADDR) PCI_MEM_ADRS2,
    PCI_MEM_SIZE,
	VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
	| VM_STATE_MASK_GUARDED,
	VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
	| VM_STATE_GUARDED
    }
    ,
    {
    (VIRT_ADDR) PCI_MEMIO_ADRS2,
    (PHYS_ADDR) PCI_MEMIO_ADRS2,
    PCI_MEMIO_SIZE,
	VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
	| VM_STATE_MASK_GUARDED,
	VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
	| VM_STATE_GUARDED
    }
    ,
    {
    (VIRT_ADDR) PCI_IO_ADRS2,
    (PHYS_ADDR) PCI_IO_ADRS2,
    PCI_IO_SIZE,
	VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
	| VM_STATE_MASK_GUARDED,
	VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
	| VM_STATE_GUARDED
    }
#endif	/* PCI */

#ifdef INCLUDE_L2PM
    ,
    {
    /* If private memory being used for MCP 755 or MCP 7410 */

    (VIRT_ADDR) L2PM_SRAM_ADRS,
    (PHYS_ADDR) L2PM_SRAM_ADRS,
    L2PM_PHYMEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE
    },
#endif /*INCLUDE_L2PM*/

    };

int   sysPhysMemDescNumEnt;		/* calculated in sysHwInit */

int   sysBus      = VME_BUS;            /* legacy */
int   sysCpu      = CPU;                /* system CPU type (MC680x0) */
char *sysBootLine = BOOT_LINE_ADRS;	/* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;	/* catastrophic message area */
int   sysProcNum  = 0;			/* processor number of this CPU */
UINT32  coreFreq;

/* static locals */

LOCAL char sysModelStr [80]	= SYS_MODEL;	/* HPC-NET2 Model string */
LOCAL char sysInfoStr [200];			/* HPC-NET2 INFO string */

LOCAL UINT32    core1ampMode;
#ifdef  _WRS_CONFIG_SMP
#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;      /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;      /* lower 32 bit value of timebase */
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
#endif  /* _VX_SMP */

/* Clock Ratio Tables */

UINT32 e600RatioLookup[][3] =
{
    {0x10, 2, 0},
    {0x19, 5, 1},
    {0x20, 3, 0},
    {0x39, 7, 1},
    {0x28, 4, 0},
    {0x1d, 5, 1},
    {0}
    };

UINT32 pixisSpdTable[8] =
    {
    FREQ_33_MHZ,
    FREQ_40_MHZ,
    FREQ_50_MHZ,
    FREQ_66_MHZ,
    FREQ_83_MHZ,
    FREQ_100_MHZ,
    FREQ_133_MHZ,
    FREQ_166_MHZ
};

UINT32 sysClkFreqGet(void);
UINT32 sysPicClkFreqGet(UINT32);

/* forward declarations */

char *  sysPhysMemTop		(void);		/* Top of physical memory */
char *	sysInfo			(void);
void    sysMemMapDetect		(void);		/* detect PREP or CHRP map */
void	sysCpuCheck		(void);		/* detect CPU type */
void    sysDelay		(void);		/* delay 1 millisecond */
void	sysMsDelay		(UINT delay);	/* delay N millisecond(s) */
ULONG   sys107RegRead           (ULONG regNum);
void    sys107RegWrite          (ULONG regNum, ULONG regVal);
STATUS  sysIntEnablePIC		(int intNum);	/* Enable i8259 or EPIC */
ULONG   sysEUMBBARRead          (ULONG regNum);
void    sysEUMBBARWrite         (ULONG regNum, ULONG regVal);
UINT32  sysCpuAvailableGet      (void);
LOCAL   UINT32 physCpuAvailableGet (void);

#ifdef INCLUDE_MMU		/* add a sysPhysMemDesc entry */
STATUS  sysMmuMapAdd		(VIRT_ADDR address, UINT32 len,
				 UINT32 initialStateMask, UINT32 initialState);
#endif /* INCLUDE_MMU */



/* external imports */

IMPORT VOID   sysOutWord	(ULONG address, UINT16 data);	/* sysALib.s */
IMPORT VOID   sysOutLong	(ULONG address, ULONG data);	/* sysALib.s */
IMPORT USHORT sysInWord		(ULONG address);		/* sysALib.s */
IMPORT ULONG  sysInLong		(ULONG address);		/* sysALib.s */
IMPORT VOID   sysOutByte	(ULONG, UCHAR);			/* sysALib.s */
IMPORT UCHAR  sysInByte		(ULONG);			/* sysALib.s */
IMPORT STATUS sysMemProbeSup    (int length, char * src, char * dest);
IMPORT void   vxSdaInit		(void);
IMPORT void   vxbMsDelay	(int delay);	/* delay N millisecond(s) */
IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;

IMPORT VOIDFUNCPTR _pSysL3CacheFlushDisableFunc;
IMPORT VOIDFUNCPTR _pSysL3CacheInvalEnableFunc;
IMPORT void sysSerialConnectAll();
/*
 * mmuPpcBatInitMPC74x5 initializes the standard 4 (0-3)  I/D BAT's &
 * the additional 4 (4-7) I/D BAT's present on the MPC74[45]5 & 8641.
 */
IMPORT void mmuPpcBatInitMPC74x5 (UINT32 *pSysBatDesc);

/* _pSysBatInitFunc needs to be set to either of the above 2 depending
   on which processor it is running to make use of the additional BAT's
   on the MPC 7455/755.
   If this is not set or is set to NULL then the standard mmuPpcBatInit
   fn. would be called which would initialize the (0-3) I/D BAT's
*/
IMPORT FUNCPTR _pSysBatInitFunc;

/*  By default this is set to NULL in the mmu Library and  in that case
    the standard BAT init func. to initialize the 4 I/D BAT's is called.
    If the standard & Extra BAT's need to be initialized then set this
    to the ones supplied by the mmuLib or else implement your own fn. and
    supply it here.
*/

#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheArchDisableFromMmu (CACHE_TYPE cache);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
#endif  /* _WRS_CONFIG_SMP */
#if (defined (_WRS_CONFIG_SMP) || defined(_WRS_VX_AMP))
IMPORT void sysCpu1Start (void);
IMPORT void sysCpu1Stop (void);
#endif	/* _WRS_CONFIG_SMP || _WRS_VX_AMP */
/* BSP DRIVERS */

#ifdef _WRS_VX_AMP
METHOD_DECL(sysBspDevFilter);
STATUS sysDeviceFilter(VXB_DEVICE_ID pDev);
void sysAmpCpuStatusSet(int status,int cpu);
int sysAmpCpuStatusGet(int cpu);
#endif	/* _WRS_VX_AMP */

IMPORT void hardWareInterFaceInit();

#include "hwconf.c"

IMPORT device_method_t * pSysPlbMethods;

LOCAL BOOL sysCpu1Check (void);
METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

LOCAL struct vxbDeviceMethod hpcNetPlbMethods[] =
    {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysCpu1Check),
#ifdef _WRS_VX_AMP
	/* Only used for AMP filtering per core for now so not built for
	 * anything else
	 */
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
#endif	/* _WRS_VX_AMP */
    { 0, 0 }
    };

#ifdef INCLUDE_FLASH
#include <mem/flashMem.c>
#include <mem/nvRamToFlash.c>
#else	/* INCLUDE_FLASH */
#include <mem/nullNvRam.c>
#endif	/* INCLUDE_FLASH */


#ifdef INCLUDE_NETWORK
#    include "./sysNet.c"		/* network setup */
#endif	/* INCLUDE_NETWORK */

#define HID1_ABE 0x800
#define HID1_SYNCBE 0x400
#define HID1_EMCP 0x80000000

#ifdef INCLUDE_SM_COMMON
#include "sysSmEnd.c"
#endif	/* INCLUDE_SM_COMMON */

#ifdef _WRS_VX_AMP

#ifdef INCLUDE_AMP_CPU_01
_WRS_ABSOLUTE_BEGIN(Loadbias)
_WRS_ABSOLUTE(wrloadLoadBias,CORE1T_OFFSET);
_WRS_ABSOLUTE_END
#endif /* INCLUDE_AMP_CPU_01 */

typedef struct {
	char* deviceName;
	int   unit;
	int   cpu;
	}AMP_CPU_TABLE;

/* Table includes only devices we want to filter
 * if no action required for a device per CPU then it's not in the table
 */
AMP_CPU_TABLE ampCpuTable[] = {
    { XTSEC_NAME, 0, 0 },
    { XTSEC_NAME, 1, 0 },
    { XTSEC_NAME, 2, 1 },
    { XTSEC_NAME, 3, 1 },
    { "ns16550", 0, 0},
    { "ns16550", 1, 0 },
    { "m85xxCCSR", 0, 0 },
    { "openPicTimer", 0,  0 },
    { "openPicTimer", 1,  0 },
    { "m85xxPci", 0, 0 },
    { "m85xxPci", 1, 0 },
    { "m85xxPci", 2, 0 }
};

/*****************************************************************************
*
* sysDeviceFilter - Called from vxbus to prevent device from being
*                   announced and therefore used by vxWorks. The default
*                   is to return OK.
*
* RETURNS: OK or ERROR
*
* /NOMANUAL
*/

STATUS sysDeviceFilter(VXB_DEVICE_ID pDev)
    {

    /* Compare devices name and unit number with those you want to remove
     * at run-time. Used here to selectively use devices on one cpu vs another.
     */
    int deviceIndex;
    BOOL deviceMatch;

    if (pDev->pName == NULL)
        return OK;

    deviceMatch=FALSE;

    for (deviceIndex=0;deviceIndex<NELEMENTS(ampCpuTable);deviceIndex++)
	{
	if ( (strcmp(pDev->pName,ampCpuTable[deviceIndex].deviceName)==OK) &&
	     (pDev->unitNumber==ampCpuTable[deviceIndex].unit) )
	    {
	    deviceMatch=TRUE;
	    break;
	    }
	}

    /* return OK if no match device - assume pass through */
    if (deviceMatch == FALSE)
	return(OK);

    /* if device match then we need to decide wether to filter */

    if(ampCpuTable[deviceIndex].cpu!=(sysProcNumGet()))
	return(ERROR);

    return(OK);
    }


#endif /* _WRS_VX_AMP */


/*****************************************************************************
*
* divisorGet - Provides the divisor for serial driver.
*
* RETURNS: NONE
*
* ERROR
*/
UINT32 divisorGet
    (
    UINT32 xtal,
    UINT32 baud
    )
    {
    return (xtal / (16 * baud));
    }


/*****************************************************************************
*
* startCPU1 - Starts CPU1 executing code.
*
* RETURNS: NONE
*
* ERROR
*/

void startCPU1()
    {
    *MCM_PCR(CCSBAR) = 0x03000000;
    WRS_ASM("isync;sync;eieio");
    }

/*****************************************************************************
*
* startCPU1App - Starts CPU1 executing code via task.
*
* Delays CPU1 start till after CPU1 up and running
*
* RETURNS: NONE
*
* ERROR
*/

void startCPU1App()
    {
    taskDelay(360);
    startCPU1();
    }

#ifdef INCLUDE_WRLOAD
/*****************************************************************************
*
* sysAmpCpuEnable - Starts core1 executing code at entryPt
*
* This routine performs the hardware specific code to start the secondary
* core. It does so using two methods - using boot page translation register
* when first called. After this it uses CPU1_INIT_START_ADR to inform the
* second core where to jump to. The second core if reboot will wait until
* CPU1_INIT_START_ADR is set to something other than MP_MAGIC_RBV which is
* what this routines is doing. The success or failure of this routine cannot
* be determined here.
* This function is required for wrload.
*
* RETURNS: NONE
*
*/

void sysAmpCpuEnable (FUNCPTR entryPt,UINT32 cpu)
    {
	/* Here if core already running then nmaybe be spinning
     * bootimage should then spin waiting for CPU1_INIT_START_ADR to be
     * non-zero
     */
    *((volatile int *)((UINT32)CPU1_FUNC_START_ADR | CORE1T_OFFSET)) =
	  (int)0; /* SMP specific */
	/* SMP specific not used just loaded */
    *((volatile int *)((UINT32)CPU1_STACK_START_ADR | CORE1T_OFFSET)) =
	  ((int)entryPt - 0x20);
	/* This is used if core waiting after core1 did asynchronous reboot */
    *((volatile int *)((UINT32)CPU1_INIT_START_ADR | CORE1T_OFFSET)) =
	  (int)entryPt;

    /* if cpu already running no need to do this */
	if ((*MCM_PCR(CCSBAR) & MCM_PCR_CORE1_ENABLE) == 0x0)
	{
	*M86XX_BPTR(CCSBAR) =
	  ((((CORE1T_OFFSET |(UINT32)entryPt) >>  BPTR_BOOT_PAGE_SHIFT ) &
	    BPTR_BOOT_PAGE_MASK) | BPTR_EN);

	WRS_ASM("isync;sync");
	sysCpu1Start();
	}

    }

/*****************************************************************************
*
* sysAmpCpuPrep - Prep cpuId for wrload download and start of AMP image
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
    UINT32  cpuId,
    void * arg
    )
    {
    if (cpuId == 1)
	{
	/* shutdown ints */
	vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable), (void *)cpuId);
	*((volatile int *)(CPU1_FUNC_START_ADR | CORE1T_OFFSET)) = (int)0;
	/* SMP specific not used just loaded */
	*((volatile int *)(CPU1_STACK_START_ADR | CORE1T_OFFSET)) =
	  (int)(RAM_LOW_ADRS - FRAMEBASESZ);
	*((volatile int *)(CPU1_INIT_START_ADR | CORE1T_OFFSET)) =
	  (int)MP_MAGIC_RBV;

	WRS_ASM("sync");

	sysCpu1Stop();

	return OK;
	}

    /* Cannot "prep" cpu 0 */
    return ERROR;
    }

#endif /* INCLUDE_WRLOAD */

/*****************************************************************************
*
* sysBusTas - Test and Set.
*
* RETURNS: STATUS
*
* ERROR
*/

STATUS sysBusTas
    (
    char * adrs
    )
    {
	return (vxTas(adrs));
    }


/*****************************************************************************
*
* sysBusClear - Clears a reservation.
*
* RETURNS: NONE
*
* ERROR
*/

void sysBusClear
    (
    char * adrs
    )
    {
    *adrs=0;
    WRS_ASM("isync;sync;eieio");
    }


/*****************************************************************************
*
* sysBusIntGen - Interrupts adjacent CPU
*
* RETURNS: STATUS
*
* ERROR
*/
STATUS sysBusIntGen
    (
    int level,          /* interrupt level to generate   */
    int vector          /* interrupt vector for interrupt*/
    )
    {
#if defined(_WRS_VX_AMP)
    vxIpiPhysEmit(0, (1 << !CPU1CHECK));
#endif /* _WRS_VX_AMP */
    return (OK);
    }


/*******************************************************************************
*
* sysMailboxConnect - connect a routine to the mailbox interrupt
*
* This routine specifies the interrupt service routine to be called at each
* mailbox interrupt.
*
*
* RETURNS: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), sysMailboxEnable()
*/

STATUS sysMailboxConnect
    (
    FUNCPTR routine,    /* routine called at each mailbox interrupt */
    int arg             /* argument with which to call routine      */
    )
    {

    return (OK);
    }


/*****************************************************************************
*
* sysMailboxEnable - enable the mailbox interrupt
*
* This routine enables the mailbox interrupt.
*
*
* RETURNS: OK, if mailbox interrupt becomes enabled, else ERROR.
*
* SEE ALSO: sysMailboxConnect(), sysMailboxDisable()
*/

STATUS sysMailboxEnable
    (
    char *mailboxAdrs           /* address of mailbox (ignored) */
    )
    {

    /* enable the mailbox 0 interrupt */

    return (OK);
    }


/****************************************************************************
*
* sysMailboxDisable - disable the mailbox interrupt
*
* This routine disables the mailbox interrupt.
*
*
* RETURNS: OK, if the mailbox interrupt gets disabled, else ERROR
*
* SEE ALSO: sysMailboxConnect(), sysMailboxEnable()
*/

STATUS sysMailboxDisable
    (
    char *mailboxAdrs           /* address of mailbox (ignored) */
    )
    {

    /* disable the mailbox interrupt */

    return (OK);
    }

int sysBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge */
    )
    {
    return (0);
    }


/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*/

char * sysModel (void)
    {
    static BOOL first = TRUE;

    if (first)
	{
	/* cut 'D' off end of sysModelStr if running on a single-core 8641 */
	if ((vxSvrGet() & _PPC_SVR_VER_MASK) == _PPC_SVR_MPC8641
	    && sysModelStr[strlen(sysModelStr) - 1] == 'D')
	    {
	    sysModelStr[strlen(sysModelStr) - 1] = '\0';
	    }
	/* if MP, report AMP vs SMP configuration */
	else if (core1ampMode)
	    strcat(sysModelStr, " (AMP[#])");
	else
	    strcat(sysModelStr, " (SMP[#])");
	sysModelStr[strlen(sysModelStr) - 3] = '0' + physCpuAvailableGet();
	first = FALSE;
	}
    return (sysModelStr);
    }


#if     defined (INCLUDE_ALTIVEC)
/*******************************************************************************
*
* sysAltivecProbe - Check if the CPU has ALTIVEC unit.
*
* This routine returns OK it the CPU has an ALTIVEC unit in it.
* Presently it checks for 7400
* RETURNS: OK  -  for 7400 Processor type
*          ERROR - otherwise.
*/

int  sysAltivecProbe (void)
    {
    ULONG regVal;
    int altivecUnitPresent = ERROR;

     /* The CPU type is indicated in the Processor Version Register (PVR) */

     regVal = CPU_TYPE;

     switch (regVal)
       {
	case CPU_TYPE_8641:
	    altivecUnitPresent = OK;
	    break;

	default:
	    altivecUnitPresent = ERROR;
	    break;
	}

    return (altivecUnitPresent);
    }
#endif  /* INCLUDE_ALTIVEC */


/*******************************************************************************
*
* sysInfo - return some information on the CPU board configuration
*
* This routine returns a string containing the L2 backside cache size,
* if enabled, the PCI map selected, and whether NVRAM is selected.
* The string may contain line feeds. For example,
*
*      512KB L2 backside cache enabled
*      PCI Memory Map B
*      NVRAM enabled
*
* RETURNS: A pointer to the string.
*/

char * sysInfo (void)
    {
#ifdef USER_750_L2BACK
	switch (L2_CACHE_BACK_SIDE_SIZE)
	    {
	    case L2CR_QM:
		 strcpy(sysInfoStr, "256KB L2 backside cache enabled\n");
		 break;

	    case L2CR_HM:
		 strcpy(sysInfoStr, "512KB L2 backside cache enabled\n");
		 break;

	    case L2CR_1M:
		 strcpy(sysInfoStr, "1M L2 backside cache enabled\n");
		 break;

	    default:
		 break;
	    } /*switch */
#endif /* USER_750_L2BACK */

#ifdef  INCLUDE_NVRAM
	strcat(sysInfoStr, "NVRAM enabled\n");
#else	/* INCLUDE_NVRAM */
	strcat(sysInfoStr, "No NVRAM enabled\n");
#endif  /* INCLUDE_NVRAM */

    return (sysInfoStr);
    }


/*******************************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number, for
* example, 1.1/0. BSP_REV is concatenated to BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }


UINT32	e600Ratio, platRatio;
UINT32	oscFreq;

/*******************************************************************************
*
* sysClkFreqGet - return the MBX bus clock frequency
*
* This routine extracts the PLL ratios from the PORPLLSR, and the oscFreq
* from the Pixis, and calculates the core and MBX bus clock frequencies.
*
* RETURNS: the MBX bus clock frequency
*/

UINT32 sysClkFreqGet(void)
    {
    UINT32  sysClkFreq;
    int ix;

    platRatio = M8XXX_PORPLLSR_PLAT_RATIO(CCSBAR);

#ifdef FORCE_DEFAULT_FREQ
    return (DEFAULT_SYSCLKFREQ);
#else	/* FORCE_DEFAULT_FREQ */

    if((*PIXIS_VCTL(PIXIS_BASE) & PIXIS_VCTL_GO) == PIXIS_VCTL_GO)
	oscFreq = OSCILLATOR_FREQ;
    else
	{
	UINT32 val = *PIXIS_OSC(PIXIS_BASE) & 0x7;

	oscFreq = pixisSpdTable[val];
	}

    sysClkFreq = (UINT32)((int)oscFreq * (int)platRatio);

    e600Ratio = M8XXX_PORPLLSR_E600_RATIO(CCSBAR);

    ix = 0;

    while ((e600RatioLookup[ix][0] != e600Ratio) && e600RatioLookup[ix][0] != 0)
	ix++;

    coreFreq = (sysClkFreq * e600RatioLookup[ix][1]) >> e600RatioLookup[ix][2];
    return(sysClkFreq);
#endif	/* FORCE_DEFAULT_FREQ */
    }


/*******************************************************************************
*
* sysPicClkFreqGet - return the OpenPIC timer count frequency
*
* Given the selected picClockDivisor value, this routine calculates
* and returns the resulting OpenPIC timer count frequency.
*
* RETURNS: the OpenPIC timer count frequency
*/

UINT32 sysPicClkFreqGet
    (
    UINT32  picClockDivisor     /* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
	return sysClkFreqGet() / picClockDivisor;
    else
	return HPCN_RTC_FREQ;   /* 14.318 MHz */
    }


/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various hardware features of the board.
* It is the first board-specific C code executed, and runs with
* interrupts masked in the processor.
* This routine resets all devices to a quiescent state, typically
* by calling driver initialization routines.
*
* NOTE: Because this routine may be called from sysToMonitor, it must
* shutdown all potential DMA master devices.  If a device is doing DMA
* at reboot time, the DMA could interfere with the reboot. For devices
* on non-local busses, this is easy if the bus reset or sysFail line can
* be asserted.
*
* NOTE: This routine should not be called directly by any user application.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {

#if defined(INCLUDE_MMU)
    PHYS_MEM_DESC * pMmu;
    int ix;
#endif	/* INCLUDE_MMU */

#ifdef LEGACY_AMP
#if defined(INCLUDE_AMP) && !defined(_WRS_CONFIG_SMP)
    BOOT_PARAMS localBootParams;
    memset(&localBootParams, 0, sizeof(BOOT_PARAMS));
#endif	/* INCLUDE_AMP && !_WRS_CONFIG_SMP */
#endif /* LEGACY_AMP*/

#ifdef _WRS_VX_AMP
#ifndef INCLUDE_AMP_CPU_01
	sysAmpCpuStatusSet(ampCpuStateReboot,1);
	sysAmpCpuStatusSet(ampCpuStateRunning,0);
#else	/* INCLUDE_AMP_CPU_01 */
	sysAmpCpuStatusSet(ampCpuStateRunning,1);
#endif	/* INCLUDE_AMP_CPU_01 */
#endif /* _WRS_VX_AMP */

    /*
     * First, automatically detect our memory mode (PREP-A or CHRP-B)
     * and setup proper global variable values for the mapping.
     * Do this before _anything_ else in sysHwInit or else addresses
     * will not be setup correctly for many other routines.
     * Note that Sandpoint uses CHRP, and PREP is
     * currently untested and unsupported.
     */

#ifdef _WRS_CONFIG_SMP
	/* Here we initialize cpu1 init address so we don't wait until the
	 * correct value is initialized
	 */
	*(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_RBV;
#endif	/* _WRS_CONFIG_SMP */

     /* initialize _func_vxSvrGet for drivers and other modules */

    _func_vxSvrGet = vxSvrGet;

     vxHid1Set(vxHid1Get() | HID1_SYNCBE | HID1_ABE | HID1_EMCP);

#ifndef	DRV_TIMER_DEC_PPC
#ifdef	_WRS_CONFIG_SMP
    /* Assert the common time base disable signal. */

    *(M8XXX_DEVDISR(CCSBAR)) |= M8641_DEVDISR_TB;
#else	/* _WRS_CONFIG_SMP */
    /* Disable time base per CPU. */

    vxHid0Set (vxHid0Get () & ~_PPC_HID0_TBEN);
#endif	/* _WRS_CONFIG_SMP */
#endif	/* !DRV_TIMER_DEC_PPC */

#if defined(INCLUDE_MMU)
/*
 *  Setup the function pointers for the BAT initialization functions.
 *  (Standard 4 I/D BAT's and extra 4 I/D BAT's
 */

/* Setup MMU if needed */

     _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC74x5;

    /*
     * run-time update of the sysPhysMemDesc MMU entry for main RAM
     * This means SDRAM _must_ be represented by sysPhysMemDesc[1]
     */

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () - RAM_LOW_ADRS);

    /* calculate the number of sysPhysMemDesc entries in use */

    pMmu = &sysPhysMemDesc[0];

    for (ix = 0; ix < NELEMENTS (sysPhysMemDesc); ix++)
	{
	if (pMmu->virtualAddr != (VIRT_ADDR)DUMMY_VIRT_ADDR)
	    pMmu++;	/* used entry */
	else
	    break;	/* dummy entry, assume no more */
	}

    /* This is the REAL number of sysPhysMemDesc entries in use. */

    sysPhysMemDescNumEnt = ix;
#endif /* INCLUDE_MMU */

    /*
     * Detect core 1 low-memory offset (AMP) mode.
     *
     * This is used for reporting in sysModel(), and to avoid attempting
     * to start up CPU1 as an SMP processor if configured for AMP.
     */

    core1ampMode = M8XXX_PORDEVSR_CORE1TE & *M8XXX_PORDEVSR(CCSBAR);

#ifndef _WRS_CONFIG_SMP
#ifdef INCLUDE_AMP
#ifdef LEGACY_AMP
    /* Here the aim is to keep the boot line the same for both cpu0 and
     * cpu1 except to use shared memory as the network device.
     */

    {
    struct hcfDevice *hcfDeviceListCopy;
    UINT32 len;
    UINT32 loop;
    hcfDeviceListCopy = (struct hcfDevice *)hcfDeviceList;

    if (CPU1CHECK)
	{
	/* This make network use RAM based bootline which we manipulate
	   for AMP */
	sysStartType=BOOT_NORMAL;
	loop=0;
	/* Remove Init of etsec/pci if dual boot - ETSEC/PCI will belong to
	   CPU0 */
	/* Leaves only PPC Dec TIMER and serial */
	while (hcfDeviceListCopy[loop].devName != NULL)
	    {
	    if (strcmp("m85xxPci", hcfDeviceListCopy[loop].devName) == 0)
		strcpy(hcfDeviceListCopy[loop].devName, "dummy");
	    if (strcmp("motetsecHEnd", hcfDeviceListCopy[loop].devName) == 0)
		strcpy(hcfDeviceListCopy[loop].devName, "dummy");
	    if (strcmp("mottsecHEnd", hcfDeviceListCopy[loop].devName) == 0)
		strcpy(hcfDeviceListCopy[loop].devName, "dummy");
	    if (strcmp("phy", hcfDeviceListCopy[loop].devName) == 0)
		strcpy(hcfDeviceListCopy[loop].devName, "dummy");
	    if (strcmp("openPicTimer", hcfDeviceListCopy[loop].devName) == 0)
		strcpy(hcfDeviceListCopy[loop].devName, "dummy");
	    if (strcmp("m85xxCCSR", hcfDeviceListCopy[loop].devName) == 0)
		strcpy(hcfDeviceListCopy[loop].devName, "dummy");
	    loop++;
	    }

	bootStringToStruct((char *)BOOT_LINE_ADRS,
			   (BOOT_PARAMS *)&localBootParams);
	memset(BOOT_LINE_ADRS, 0, 255);

	strcpy(&localBootParams.gad[0], &localBootParams.bad[0]);

	/* Change backplane ipAddress to +/- cpu0. Assume not X.X.X.25X */
	len = strlen(&localBootParams.bad[0]);
	if(localBootParams.bad[len-1]=='9')
	    (*(UINT8*)&localBootParams.bad[(len-1)]) -=1;
	else
	    (*(UINT8*)&localBootParams.bad[(len-1)]) +=1;

	localBootParams.procNum = 1;
	/* Temporary fix for sm boot device to get correct ip */
	strcpy(&localBootParams.ead[0], &localBootParams.bad[0]);

	{
	int loop, loop2, count;
	BOOL compare;

	compare = FALSE;
	for ( loop = 0 ; loop < strlen(&localBootParams.other[0]) ; loop++ )
	    {
	    if (localBootParams.other[loop] == EOS)
		break;
	    /* Look for tipc address assignment */
	    if (strncmp("a=", &localBootParams.other[loop], 2) == 0)
		{
		compare = TRUE;
		break;
		}

	    }

	count = 0;
	if (compare == TRUE)
	    {
	    for ( loop2 = loop + 2 ; loop2 < loop+12 ; loop2++ )
		{
		if (localBootParams.other[loop2] == '.')
		    count++;
		/* Get to third index of tipc address and increment
		 * Assume index not greater than 8.
		 */
		if (count == 2)
		    {
		    localBootParams.other[loop2+1]++;
		    break;
		    }
		}
	    }
	}

	/* Only boot device available is sm0 - so setup boot params
	 * accordingly.
	 */
	strcpy(&localBootParams.bootDev[0], "sm");

	localBootParams.unitNum = 0;

	bootStructToString((char *)BOOT_LINE_ADRS,
			   (BOOT_PARAMS *)&localBootParams);
	}
    else
	{
	/* Copy user bootline. Assume using same image */
	strcpy((char*)(CORE1T_OFFSET+BOOT_LINE_ADRS), (char*)BOOT_LINE_ADRS);
	}

    if(!core1ampMode)
	{
	strcpy(sysExcMsg,"Board in SMP mode. Switch to low memory offset mode(AMP) and power down to restart");
	sysToMonitor(BOOT_WARM_NO_AUTOBOOT);
	}

    }
#endif /* LEGACY_AMP*/
#endif /* INCLUDE_AMP */
#endif /* ndef _WRS_CONFIG_SMP */

    pSysPlbMethods = hpcNetPlbMethods;
    hardWareInterFaceInit();

#if defined(INCLUDE_CACHE_SUPPORT) && defined(INCLUDE_CACHE_L2)

    sysL2CacheInit();

    _pSysL2CacheInvFunc = sysL2CacheGlobalInv;
    _pSysL2CacheEnable  = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;

    _pSysL2CacheFlush = sysL2CacheHWFlush;

#endif /* INCLUDE_CACHE_SUPPORT && INCLUDE_CACHE_L2 */
    }


/*******************************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupt vectors and configures any
* required features not configured by sysHwInit. It is called from usrRoot()
* in usrConfig.c (via sysClkConnect) after the multitasking kernel has started.
*
* RETURNS: N/A
*/

void sysHwInit2 (void)
    {
    static int	initialized;		/* Do this ONCE */

    if (!initialized)
	{
	initialized = TRUE;

	vxbDevInit();

    sysSerialConnectAll();

#ifdef INCLUDE_ALTIVEC
	_func_altivecProbeRtn = sysAltivecProbe;
#endif  /* INCLUDE_ALTIVEC */

	taskSpawn("devConnect", 0, 0, 10000, vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#if defined(LEGACY_AMP) && defined(INCLUDE_AMP_SM_END_AUTO)
	taskSpawn("startCPU1", 0, 0, 10000, (FUNCPTR)startCPU1App,0,0,0,0,0,0,0,0,0,0);
#endif	/* LEGACY_AMP && INCLUDE_AMP_SM_END_AUTO */

	}
    }


/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Normally, the user specifies the amount of physical memory with the
* macro LOCAL_MEM_SIZE in config.h.  BSPs that support run-time
* memory sizing do so only if the macro LOCAL_MEM_AUTOSIZE is defined.
* If not defined, then LOCAL_MEM_SIZE is assumed to be, and must be, the
* true size of physical memory.
*
* NOTE: Do no adjust LOCAL_MEM_SIZE to reserve memory for application
* use.  See sysMemTop() for more information on reserving memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    static char * physTop = NULL;

    if (physTop == NULL)
	{
#ifdef LOCAL_MEM_AUTOSIZE

	/* TODO - If auto-sizing is possible, this would be the spot.  */

#	error	"Dynamic memory sizing not supported"

#else	/* LOCAL_MEM_AUTOSIZE */
	/* Don't do autosizing, size is given */

	physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

#endif /* LOCAL_MEM_AUTOSIZE */
	}

    return physTop;
    }


/*******************************************************************************
*
* sysMemTop - get the address of the top of VxWorks memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by VxWorks.
*
* The user can reserve memory space by defining the macro USER_RESERVED_MEM
* in config.h.  This routine returns the address of the reserved memory
* area.  The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*/

char * sysMemTop (void)
    {
    static char * memTop = NULL;

    if (memTop == NULL)
	{
	memTop = sysPhysMemTop () - USER_RESERVED_MEM;
#ifdef	INCLUDE_EDR_PM
	/* account for ED&R persistent memory */
	memTop -= PM_RESERVED_MEM;
#endif	/* INCLUDE_EDR_PM */
	}

    return memTop;
    }


#ifdef  _WRS_CONFIG_SMP
/* flag shared by sysToMonitor() and sysOtherCpuShutdown() */
LOCAL BOOL otherCPUdone = FALSE;

/*******************************************************************************
*
* sysOtherCpuShutdown - flush caches and park until reset
*
* While one CPU is running sysToMonitor(), this routine runs on the other CPU.
* It prepares for reboot by first clearing the caches, and then (if running
* on a Rev 2 or higher system, in which case it will be running on CPU 1)
* attempting to put the CPU into nap mode.  If running on a Rev 1 system, or
* if the nap attempt fails, it enters a tight loop until the system is reset.
*
* RETURNS: N/A
*/

LOCAL void sysOtherCpuShutdown (void)
    {
    intCpuLock();

    /* The cacheDisable() API is not supported in SMP */

    cacheArchDisableFromMmu (INSTRUCTION_CACHE);
    cacheArchDisableFromMmu (DATA_CACHE);

    otherCPUdone = TRUE;	/* notify sysToMonitor */

    /*
     * Per the MPC8641D manual, the CPU should be in sleep or nap state prior
     * to issuing an HRESET via the processor reset register in the PIC.
     * Do this even if the system does not otherwise include power management,
     * but not on Rev 1 systems (which do not have the processor reset register
     * and will therefore be reset externally).
     */

    if ((vxSvrGet() & _PPC_SVR_REV_MASK) >= 0x20) /* check processor's SVR */
	{
	/* Rev 2 processor */
	vxHid0Set ((vxHid0Get () & ~_PPC_HID0_SLEEP) | _PPC_HID0_NAP);

	/*
	 * Despite the above, it has been observed that some boards will hang
	 * when the reset is attempted if we actually put CPU 1 into nap state
	 * (by setting MSR[POW]), and they seem to reboot properly if CPU 1 is
	 * put in a spin loop instead.  Therefore, setting the MSR is optional.
	 * (Add a #define CORE1_NAP_WHEN_REBOOT here or in config.h if you want
	 * to try setting the MSR.)
	 */
#ifdef	CORE1_NAP_WHEN_REBOOT
	vxMsrSet (vxMsrGet () | (_PPC_MSR_POW_U << 16));
#endif	/* CORE1_NAP_WHEN_REBOOT */
	}

    FOREVER	/* Rev 2 should not get here */
	{
	}

    }
#endif  /* _WRS_CONFIG_SMP */


/*******************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot() -- which services ^X -- and by fatal system errors such as
* exceptions at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* The entry point for a warm boot is defined by the macro ROM_WARM_ADRS
* in config.h.  We jump to this address to enter the ROM code.
*
* RETURNS: Does not return.
*
* INTERNAL
*
* SMP Considerations
*
* When rebooting an SMP system, it is likely that both CPUs' data caches,
* and especially the L2 caches if enabled, contain modified lines which
* have not been written out to SDRAM (although, due to hardware-enforced
* coherency, at most one CPU's cache contains a modified copy of any one
* cache line).  To ensure that critical diagnostic data -- such as kernel
* stacks, the ED&R buffer, and the coredump buffer -- are preserved, we
* must ensure that both CPUs flush their caches before the system reboots.
* This requires a CPC (cross processor call), but we are careful to attempt
* only one CPC so as to maximize the chances of success even if the call to
* sysToMonitor() occurred due to a fatal error having made the system unstable.
*
* How we shut down the other CPU depends on the hardware revision level.  Rev 2
* and higher have a per-CPU reset bit in the PIC, which CPU 0 can use to reset
* CPU 1 before jumping to the bootrom.  On a Rev 1 processor, the best we can
* do is use the Pixis to reset the whole board -- and it doesn't matter which
* CPU does it.  (Resetting the whole board would also work on Rev 2, but seems
* to lose the contents of persistent memory on some Rev 2 boards.)
*
* Flow diagram for Rev 1 processors.  It does not matter which CPU is which.
*
* \cs
*      static initialization
* +-----------------------------+
* |    otherCPUdone = FALSE     |
* +-----------------------------+
*
* on the CPU that called sysToMonitor()           on the other CPU
*
* +-----------------------------------+
* |      cacheFlush (DATA_CACHE)      |
* +-----------------------------------+
* | CPC to other: sysOtherCpuShutdown | - - - - - - - - - +
* |                                   |                   |
* |                                   |                   v
* +-----------------------------------+  +---------------------------------+
* |  cacheDisable(INSTRUCTION_CACHE)  |  | cacheDisable(INSTRUCTION_CACHE) |
* +-----------------------------------+  +---------------------------------+
* |     cacheDisable (DATA_CACHE)     |  |    cacheDisable (DATA_CACHE)    |
* +-----------------------------------+  +---------------------------------+
* |  wait for otherCPUdone or timeout |  |       otherCPUdone = TRUE       |
* +-----------------------------------+  +---------------------------------+
* |           reset system            |  |              spin               |
* +-----------------------------------+  +---------------------------------+
* \ce
*
* Flow diagram for Rev 2 and higher processors.  The last part of sysToMonitor()
* must run on CPU 0 because it will jump to the bootrom.
*
* If we happen to already be on CPU 0, we can cause CPU 1 to clear its caches
* and enter nap mode (or a parking loop) after which CPU 0 can proceed with
* shutdown.  If we are on CPU 1, we will proceed to the CPU 1 shutdown code
* after arranging for sysToMonitor() to be reinvoked on CPU 0.
*
* \cs
*      static initialization
* +-----------------------------+
* |    otherCPUdone = FALSE     |
* | CPU1needsShutdownCPC = TRUE |
* +-----------------------------+
*
*                                   +- - - - - - - - - - - - - - - - - - - - -+
* sysToMonitor()                    |                                         |
*   starts on either CPU            v
* +-----------------------------------------------------------------------+   |
* |                        cacheFlush (DATA_CACHE)                        |
* +-----------------------------------------------------------------------+   |
* |                             current CPU ?                             |
* |                  0                  |                1                |   |
* +-------------------------------------+---------------------------------+
* | if (CPU1needsShutdownCPC)           |   CPU1needsShutdownCPC = FALSE  |   |
* |   CPC to CPU 1: sysOtherCpuShutdown |   CPC to CPU 0:  sysToMonitor   |- -+
* +--------------------------|----------+---------------------------------+
*   |                                               |
*   |                        |                      |
*   |                        + - - - - - - - - +    |
*   |                                          |    |  sysOtherCpuShutdown()
*   v    always on CPU 0                       v    v    always on CPU 1
* +-----------------------------------+   +---------------------------------+
* |           intCpuLock ()           |   |          intCpuLock ()          |
* +-----------------------------------+   +---------------------------------+
* |  cacheDisable(INSTRUCTION_CACHE)  |   | cacheDisable(INSTRUCTION_CACHE) |
* +-----------------------------------+   +---------------------------------+
* |     cacheDisable (DATA_CACHE)     |   |    cacheDisable (DATA_CACHE)    |
* +-----------------------------------+   +---------------------------------+
* | wait for otherCPUdone or timeout  |   |       otherCPUdone = TRUE       |
* +-----------------------------------+   +---------------------------------+
* |    wait long enough for CPU1      |   |               nap               |
* |    to actually be in nap mode     |   +---------------------------------+
* +-----------------------------------+
* |        put CPU1 into reset        |
* +-----------------------------------+
* |          jump to bootrom          |
* +-----------------------------------+
* \ce
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR	pRom = (FUNCPTR) (ROM_WARM_ADRS);

#ifdef _WRS_VX_AMP
    volatile int sysMonDelay;
#endif	/* _WRS_VX_AMP */

#ifdef _WRS_CONFIG_SMP
    /*
     * Write out our own modified lines first, so that
     * much gets done even if we get into trouble later.
     */

    cacheFlush (DATA_CACHE, NULL, ENTIRE_CACHE);

    /* If only one CPU is running, we don't need to shut down the other. */

    if (vxCpuEnabledGet() != (1 << 0))
	{
	/* Both CPUs are running. */

	cpuset_t cpuset;
	int i;

	CPUSET_ZERO (cpuset);	/* Will be sending a CPC to all but self */

	/*
	 * Make sure we cannot migrate, but without locking
	 * interrupts -- see cpcInvoke() requirements.
	 */

	if (!INT_CONTEXT() && !KERNEL_ENTERED_ME())
	    taskCpuLock();

	if ((vxSvrGet() & _PPC_SVR_REV_MASK) < 0x20) /* check processor's SVR */
	    {
	    /* Rev 1 processor */

	    cpcInvoke (cpuset, (CPC_FUNC)sysOtherCpuShutdown, NULL, 0,
		       VX_CPC_ASYNC);

	    intCpuLock ();	/* CPC done, now safe to disable interrupts */

	    /* The cacheDisable() API is not supported in SMP */

	    cacheArchDisableFromMmu (INSTRUCTION_CACHE);
	    cacheArchDisableFromMmu (DATA_CACHE);

	    i = 100;	    /* 10 sec (100 * 0.1) max wait for other CPU */

	    while (!otherCPUdone && --i > 0)
		sysMsDelay(100);
	    }
	else
	    {
	    /* Rev 2 processor */
	    static BOOL CPU1needsShutdownCPC = TRUE;

	    if (vxCpuIndexGet() == 0)
		{
		if (CPU1needsShutdownCPC)
		    cpcInvoke (cpuset, (CPC_FUNC)sysOtherCpuShutdown, NULL, 0,
			       VX_CPC_ASYNC);
		}
	    else
		{
		CPU1needsShutdownCPC = FALSE;
		cpcInvoke (cpuset, (CPC_FUNC)sysToMonitor, (void *)startType,
			   sizeof(startType), VX_CPC_ASYNC);
		sysOtherCpuShutdown();		/* does not return */
		}

	    /* Only CPU 0 gets here */

	    intCpuLock ();	/* CPC done, now safe to disable interrupts */

	    /* The cacheDisable() API is not supported in SMP */

	    cacheArchDisableFromMmu (INSTRUCTION_CACHE);
	    cacheArchDisableFromMmu (DATA_CACHE);

	    i = 100;		/* 10 sec (100 * 0.1) max wait for CPU1 */

	    while (!otherCPUdone && --i > 0)
		sysMsDelay(100);

	    /*
	     * Wait another 0.1 sec to ensure CPU1 has had time to enter nap
	     * mode after reporting that it had finished flushing its caches.
	     */

	    sysMsDelay(100);
	    }
	}	/* end of "Both CPUs are running." */

#else   /* _WRS_CONFIG_SMP */
    cacheDisable (INSTRUCTION_CACHE);   /* Disable the Instruction Cache */
    cacheDisable (DATA_CACHE);          /* Disable the Data Cache */
#endif  /* _WRS_CONFIG_SMP */

    vxHid0Set (vxHid0Get () & ~_PPC_HID0_SIED);	/* Enable Serial Instr Exec */

    /* Turn off timer */

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif	/* INCLUDE_AUX_CLK */

#ifdef  _WRS_CONFIG_SMP
    if (vxCpuEnabledGet() != (1 << 0))
	{
	/* Both CPUs are running. */

	if ((vxSvrGet() & _PPC_SVR_REV_MASK) < 0x20) /* check processor's SVR */
	    {
	    /*
	     * Rev 1 processor.
	     *
	     * Store a sentinel value for romInit to find, and reset the board.
	     */
	    struct rebootVec *rbV = (struct rebootVec *)RBV_ADRS;

	    rbV->startType = startType;
	    rbV->magic1 = _WRS_BOOT_MAGIC_1;
	    rbV->magic2 = _WRS_BOOT_MAGIC_2;
	    rbV->magic3 = _WRS_BOOT_MAGIC_3;
	    WRS_ASM (" sync ; isync ");		/* ensure stores complete */

	    *PIXIS_RST(PIXIS_BASE) &= 0x7f;		/* Reset both cores */
	    for ( ; ; )
		;
	    }
	else
	    {
	    /*
	     * Rev 2 processor.
	     *
	     * Reset the second core, using the processor reset register
	     * in the EPIC.  The bits in the Hard Reset Register are not
	     * auto-clearing.
	     */

	    /* Set HRESET */
	    *(M8XXX_EPICPRR(CCSBAR)) |= M8XXX_EPICPRR_HRST_CORE1;

	    sysCpu1Stop ();

	    /* Clear HRESET */
	    *(M8XXX_EPICPRR(CCSBAR)) &= ~M8XXX_EPICPRR_HRST_CORE1;
	    }
	}	/* end of "Both CPUs are running." */
#endif	/* _WRS_CONFIG_SMP */

    vxMsrSet (0);		/* Clear the MSR */

#if defined(_WRS_VX_AMP) && defined (LEGACY_AMP)
    *PIXIS_RST(PIXIS_BASE) &= 0x7f;	/* Reset both cores */
    for ( ; ; )
	;
#endif	/* _WRS_VX_AMP && LEGACY_AMP */

#ifdef _WRS_VX_AMP

    /* Clear boot page so back to flash - precautionary step  */
    *M86XX_BPTR(CCSBAR) = 0x0;

    /* Here we have to send to bootrom and let wrload reload image when ready
     * bootimage should spin waiting for CPU1_INIT_START_ADR not to be the
     * magic pattern 1
     */
#ifndef INCLUDE_AMP_CPU_01

    *((volatile int *)(CPU1_FUNC_START_ADR | CORE1T_OFFSET)) = (int)0;
    /* SMP specific not used just loaded */
    *((volatile int *)(CPU1_STACK_START_ADR | CORE1T_OFFSET)) =
      (int)(RAM_LOW_ADRS - FRAMEBASESZ);
    *((volatile int *)(CPU1_INIT_START_ADR | CORE1T_OFFSET)) =
      (int)MP_MAGIC_RBV;

    WRS_ASM("sync");

    sysCpu1Stop();
    /* Do not want to call out from this routine - use hard loop to
     * delay instead
     */
    for ( sysMonDelay = 0 ; sysMonDelay < 0x10000 ; sysMonDelay++)
	;

    *(M8XXX_EPICPRR(CCSBAR)) |= M8XXX_EPICPRR_HRST_CORE1; /* Set HRESET */
    WRS_ASM("sync;isync");

    /* Do not want to call out from this routine - use hard loop to
     * delay instead - delay allow hreset to be held for a short time
     */
    for ( sysMonDelay = 0 ; sysMonDelay < 0x10000 ; sysMonDelay++)
	;

    *(M8XXX_EPICPRR(CCSBAR)) &= ~M8XXX_EPICPRR_HRST_CORE1; /* Clear HRESET */
    WRS_ASM("sync;isync");

    /* Do not want to call out from this routine - use hard loop to
     * delay instead
     */
    for ( sysMonDelay = 0 ; sysMonDelay < 0x10000 ; sysMonDelay++)
	;

    sysCpu1Start();

    sysAmpCpuStatusSet(ampCpuStateReboot,0);
#else /* INCLUDE_AMP_CPU_01 */

    *((volatile int *)CPU1_FUNC_START_ADR) = (int)0;
	/* SMP specific not used just loaded */
    *((volatile int *)CPU1_STACK_START_ADR) = (int)RAM_LOW_ADRS - FRAMEBASESZ;
    *((volatile int *)CPU1_INIT_START_ADR) = (int)MP_MAGIC_RBV;

    WRS_ASM("isync;sync");
    sysAmpCpuStatusSet(ampCpuStateReboot,1);

#endif /* INCLUDE_AMP_CPU_01 */
#endif /* _WRS_VX_AMP */

    (*pRom) (startType);	/* jump off to romInit.s */

    return (OK);		/* in case we continue from ROM monitor */
    }


/*******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board.
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return (CPU1CHECK);
    }


/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* For bus systems, it is assumes that processor 0 is the bus master and
* exports its memory to the bus.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int procNum			/* processor number */
    )
    {
    sysProcNum = procNum;

    if (procNum == 0)
	{
	}
    }


/*******************************************************************************
*
* sysCpuCheck - confirm the CPU type
*
* This routine validates the cpu type.  If the wrong cpu type is discovered
* a message is printed using the serial channel in polled mode.
*
* RETURNS: N/A
*/

void sysCpuCheck (void)
    {
    /* Check for a valid CPU type;  If one is found, just return */

    if (CPU_TYPE == CPU_TYPE_8641)
	{
	return;
	}

    sysToMonitor (BOOT_NO_AUTOBOOT);

    return;
    }


/*******************************************************************************
*
* sysDelay - delay for approximately one millisecond
*
* Delay for approximately one milli-second.
*
* RETURNS: N/A
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }


/*******************************************************************************
*
* sysMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by counting
* decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* the sysMsDelay() causing a slightly longer delay than requested.
*
* This routine will produce incorrect results if the delay time requested
* requires a count larger than 0xffffffff to hold the decrementer
* elapsed tick count.  For a System Bus Speed of 67 MHZ this amounts to
* about 258 seconds.
*
* RETURNS: N/A
*/

void sysMsDelay
    (
    UINT        delay                   /* length of time in MS to delay */
    )
    {
#ifdef	INCLUDE_TIMER_STUB
# ifdef DRV_TIMER_DEC_PPC
    register UINT oldval;               /* decrementer value */
    register UINT newval;               /* decrementer value */
    register UINT totalDelta;           /* Dec. delta for entire delay period */
    register UINT decElapsed;           /* cumulative decrementer ticks */

    /*
     * Calculate delta of decrementer ticks for desired elapsed time.
     * The macro DEC_CLOCK_FREQ MUST REFLECT THE PROPER 6xx BUS SPEED.
     */

    totalDelta = ((DEC_CLOCK_FREQ / 4) / 1000) * delay;

    /*
     * Now keep grabbing decrementer value and incrementing "decElapsed" until
     * we hit the desired delay value.  Compensate for the fact that we may
     * read the decrementer at 0xffffffff before the interrupt service
     * routine has a chance to set in the rollover value.
     */

    decElapsed = 0;

    oldval = vxDecGet ();

    while (decElapsed < totalDelta)
	{
	newval = vxDecGet ();

	if ( DELTA(oldval, newval) < 1000 )
	    decElapsed += DELTA(oldval, newval);  /* no rollover */
	else
	    if (newval > oldval)
		decElapsed += abs((int)oldval);  /* rollover */

	oldval = newval;
	}
# else  /* DRV_TIMER_DEC_PPC */
# error This BSP does not support INCLUDE_TIMER_STUB && !DRV_TIMER_DEC_PPC configuration!
# endif /* DRV_TIMER_DEC_PPC */
#else	/* INCLUDE_TIMER_STUB */
    vxbMsDelay (delay);
#endif	/* INCLUDE_TIMER_STUB */
    }


/*******************************************************************************
*
* sysIntConnect - connect the BSP interrupt to the proper epic/i8259 handler.
*
* This routine checks the INT level and connects the proper routine.
* pciIntConnect() or intConnect().
*
* RETURNS:
* OK, or ERROR if the interrupt handler cannot be built.
*
*/

STATUS sysIntConnect
    (
    VOIDFUNCPTR *vector,        /* interrupt vector to attach to     */
    VOIDFUNCPTR routine,        /* routine to be called              */
    int parameter               /* parameter to be passed to routine */
    )
    {
    int tmpStat = ERROR;
    UINT32 read;

    if (vxMemProbe ((char *) routine, VX_READ, 4, (char *) &read) != OK)
	{
	if( _func_logMsg != NULL )
	    _func_logMsg ("Error in sysIntConnect: Cannot access routine.\n",
		1,2,3,4,5,6);
	return (ERROR);
	}

    switch ((int)vector)
	{
	/*
	 * add INT_VEC_IRQ0 before calling pciIntConnect since it subtracts
	 * it and the EPIC which deals with PCI ints needs to be passed
	 * original vector
	 */
	default:
	    {
	    tmpStat = intConnect (vector, routine, parameter);
	    break;
	    }
	} /* End switch */

    if (tmpStat == ERROR)
	{
	if( _func_logMsg != NULL )
	    _func_logMsg ("Error in sysIntConnect: vector = %d.\n",
			   (int)vector, 2,3,4,5,6);
	}

    return (tmpStat);
    }


/*******************************************************************************
*
* sysIntEnable - enable an interrupt
*
* This function call is used to enable an ISA/PCI interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*/

STATUS sysIntEnable
    (
    int intNum
    )
    {
    return (intEnable(intNum));
    }


/*******************************************************************************
*
* sysIntDisable - disable an interrupt
*
* This function call is used to disable an interrupt.
*
* RETURNS: OK or ERROR if unable to disable interrupt.
*/

STATUS sysIntDisable
    (
    int intNum
    )
    {
    return (intDisable (intNum));
    }


/*******************************************************************************
*
* sysNvRead - read one byte from NVRAM
*
* This routine reads a single byte from a specified offset in NVRAM.
*
* RETURNS: The byte from the specified NVRAM offset.
*/

UCHAR sysNvRead
    (
    ULONG	offset	/* NVRAM offset to read the byte from */
    )
    {
    return(0);
    }


/*******************************************************************************
*
* sysNvWrite - write one byte to NVRAM
*
* This routine writes a single byte to a specified offset in NVRAM.
*
* RETURNS: N/A
*/

void sysNvWrite
    (
    ULONG	offset,	/* NVRAM offset to write the byte to */
    UCHAR	data	/* datum byte */
    )
    {

    }


#ifdef INCLUDE_MMU
/*******************************************************************************
*
* sysMmuMapAdd - insert a new mmu mapping
*
* This routine adds a new mmu mapping entry to allow dynamic mappings.
*
* RETURNS: OK or ERROR depending on availability of free mappings.
*/

STATUS sysMmuMapAdd
    (
    VIRT_ADDR address,
    UINT32 length,
    UINT32 initialStateMask,
    UINT32 initialState
    )
    {
    PHYS_MEM_DESC *pMmu;
    STATUS result = OK;

    pMmu = &sysPhysMemDesc[sysPhysMemDescNumEnt];

    if(pMmu->virtualAddr != (VIRT_ADDR)DUMMY_VIRT_ADDR)
	result = ERROR;
    else
	{
	pMmu->virtualAddr = address;
	pMmu->physicalAddr = address;
	pMmu->len = length;
	pMmu->initialStateMask = initialStateMask;
	pMmu->initialState = initialState;
	sysPhysMemDescNumEnt += 1;
	}

    return (result);
    }
#endif /* INCLUDE_MMU */


/******************************************************************************
*
* sysLocalToBusAdrs - convert a local address to a bus address
*
* This routine gets the VMEbus address that accesses a specified local
* memory address.
*
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* ERRNO
*
* SEE ALSO: sysBusToLocalAdrs()
*/

STATUS sysLocalToBusAdrs
	(
	int     adrsSpace,  /* bus address space where busAdrs resides */
	char *  localAdrs,  /* local address to convert */
	char ** pBusAdrs    /* where to return bus address */
	)
	{

	*pBusAdrs = localAdrs;

	return(OK);
	}


/******************************************************************************
*
* sysBusToLocalAdrs - convert a bus address to a local address
*
* This routine gets the local address that accesses a specified VMEbus
* physical memory address.
*
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* ERRNO
*
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
	(
	int     adrsSpace,  /* bus address space where busAdrs resides */
	char *  busAdrs,    /* bus address to convert */
	char ** pLocalAdrs  /* where to return local address */
	)
	{

	*pLocalAdrs = busAdrs;

	return(OK);
	}

/******************************************************************************
*
* coreShow - display routine for core clocking
*
* RETURNS: NONE
*
* ERRNO
*
*/

void coreShow()
{
	printf("\nOscillator Freq: %d\n", oscFreq);
	printf("MPX Bus Speed:   %d\n", sysClkFreqGet()>>1);
	printf("DDR:             %d\n", sysClkFreqGet());
	printf("Core Freq:       %d\n", coreFreq);
    }

#ifdef INCLUDE_PCI_BUS

/* PCI express channel A */
LOCAL UCHAR sysPci1IntRoute [NUM_PCIEX_SLOTS][4] = {{PCIEX_XINT1_LVL, PCIEX_XINT2_LVL, PCIEX_XINT3_LVL, PCIEX_XINT4_LVL}};


/* PCI express channel B */

LOCAL UCHAR sysPci2IntRoute [NUM_PCIEX_SLOTS][4] = {{PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL}};

/*
 * This is only for the PCI slots the ULi bridge on channel A
 * info taken for HPC-NET Workbook
 */

LOCAL UCHAR sysPci1IntBridgeRoute [3][4] = {{0,     1,     2,     3},   /* PCI 1 */
				      {1,     2,     3,     0},   /* PCI 2 */
				      {0,     2,     0xff,  0xff}};  /* PIXIS */

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UINT devVend                        /* deviceID/vendorID of device */
    )
    {

    /*
     * Ignore bus 0;
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
	return(ERROR);


    return (OK); /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci2AutoconfigInclude - PCI 2 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci2AutoconfigInclude
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UINT devVend                        /* deviceID/vendorID of device */
    )
    {

    /*
     * Ignore bus 0;
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
	return(ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci1AutoconfigIntrAssign - PCI 1 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci1AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UCHAR pin                           /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Verify this is slot 1 or slot 2 on ULi then configure interrupt
     */

    if((pLoc->bus == 2) &&
       ((pLoc->device == 17) || (pLoc->device == 18) || (pLoc->device == 19)))
	{
	/* This should be slot 1 and slot 2 of ULi PCI */
	tmpChar =  sysPci1IntBridgeRoute [(pLoc->device - 17)][pin-1];
	return(tmpChar);
	}

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin >= 1) && (pin < 5)) &&
	((pLoc->device) < NUM_PCIEX_SLOTS))
	{
	tmpChar =
	    sysPci1IntRoute [(pLoc->device)][pin-1];
	}

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci2AutoconfigIntrAssign - PCI 2 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci2AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UCHAR pin                           /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin >= 1) && (pin < 5)) &&
	((pLoc->device) < NUM_PCIEX_SLOTS))
	{
	tmpChar =
	    sysPci2IntRoute [(pLoc->device)][pin-1];
	}

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }
#endif	/* INCLUDE_PCI_BUS */


#if defined(_WRS_CONFIG_SMP)
/******************************************************************************
*
* sysCpuInit - initialize additional core
*
* RETURNS: NONE
*
*/

void sysCpuInit (void)
    {

    vxHid1Set (vxHid1Get() | HID1_SYNCBE | HID1_ABE | HID1_EMCP);

#ifdef  INCLUDE_CACHE_L2
    sysL2CacheInit();
#endif  /* INCLUDE_CACHE_L2 */

#ifdef  INCLUDE_CACHE_SUPPORT
/* XXX - can't use cacheEnable here, as it's already called by cpu0 */
#ifdef  USER_I_CACHE_ENABLE
    cacheArchEnable (INSTRUCTION_CACHE);        /* enable instruction cache */
#endif  /* USER_I_CACHE_ENABLE */

#ifdef  USER_D_CACHE_ENABLE
    cacheArchEnable (DATA_CACHE);               /* enable data cache */
#ifdef INCLUDE_CACHE_L2
    sysL2CacheEnable (DATA_CACHE);
#endif  /* INCLUDE_CACHE_L2 */
#endif  /* USER_D_CACHE_ENABLE */
#endif  /* INCLUDE_CACHE_SUPPORT */

#ifdef  INCLUDE_MMU
    mmuPpcCpuInit ();
#endif  /* INCLUDE_MMU */

#if (defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP))
    /*
     * Set the current timebase read from CPU0 onto CPU1.
     * The timebase is kept halt till it's re-started by the CPU0.
     */

    sysSmpTimeBaseSet (tbHI, tbLO);
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
    }


unsigned int sysCpu1LoopCount = 0;

/******************************************************************************
*
* sysCpu1Loop - CPU 1 loop
*
* This routine is the CPU1 idle loop function which will invoke the startup
* function specified in the sysCpuEnable function.
*
* RETURNS: NONE
*
*/

void sysCpu1Loop(void)
    {
    unsigned int a = 0;
    FUNCPTR p;

    /*
     * The call to vxSdaInit() must be the first operation here.
     * This is because vxSdaInit() sets the r2 and r13 registers to the
     * SDA and SDA2 base registers. No C code must be placed before this
     * call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in sysCpu1Loop() */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

    p = (FUNCPTR)(*(int *)CPU1_FUNC_START_ADR);

    sysCpuInit();

    if (p != NULL)
	{
	p ();
	}

    for (;;)
	{
	if (!(++a % 0x10000))
	    {
	    sysCpu1LoopCount++;
	    }
	}
    }


/******************************************************************************
*
* sysCpuEnable - enable a multi core CPU
*
* This routine brings a CPU out of reset
*
* RETURNS: OK or ERROR
*
*/

STATUS sysCpuEnable
    (
    unsigned int      cpuNum,
    WIND_CPU_STATE  * pCpuState
    )
    {
    STATUS status = OK;

    if (cpuNum != 1 || core1ampMode )
	{
	return ERROR;
	}

    *((int *)CPU1_FUNC_START_ADR) = (int)pCpuState->regs.pc;
    *((int *)CPU1_STACK_START_ADR) = (int)pCpuState->regs.spReg;
    *((int *)CPU1_INIT_START_ADR) = (int)sysCpu1Loop;

#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
    {
#ifndef	VX_ENABLE_CPU_TIMEOUT
#define VX_ENABLE_CPU_TIMEOUT	5	/* default timeout five seconds */
#endif	/* VX_ENABLE_CPU_TIMEOUT */

    cpuset_t    cpuset;
    UINT32      i = 0;

    /*
     * PPC Dec timer will not be available while synchronizing timebase
     * between two cores.
     * Use magic number, 0x9000000, to count 1 second independently
     * on PPC Dec timer.
     */

    UINT32      timeout = 0x9000000 * VX_ENABLE_CPU_TIMEOUT;

    /*
     * lock interrupts while Dec Timer is halt to prevent from lock-up
     * in the ISR which performs a timer delay.
     */

    int key = intCpuLock ();

    /*
     * For future reference:  DEVDISR is internal to the MPC8641D chip.
     * Platforms using multiple MPC8641D's (i.e. more than two CPUs) cannot
     * achieve perfect synchronization; the best that can be done is to
     * deassert the multiple DEVDISR[TB] signals as near simultaneously as
     * possible by using CPC.  On such a platform, the |= operation below
     * should be replicated, specifying the appropriate base address to
     * access each MPC8641D's CCSR space; and the &= operation below
     * should be replaced with a call to an assembly-language function
     * which writes the multiple DEVDISR registers using consecutive
     * machine instructions.
     */

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the synchronization between two cores gets
     * completed.
     */

    *(M8XXX_DEVDISR(CCSBAR)) |= M8641_DEVDISR_TB;

     /*
      * Save the current timebase on CPU0 into static valuable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * synchronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    sysCpu1Start ();

#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
    /*
     * usrCpuEnable() or kernelCpuEnable() relies on the timer to count timeout.
     * If the system clock is PPC Dec Timer, it can not be used while timebase
     * is disabled.
     * Use a magic number for busy loop to count timeout here.
     */

    CPUSET_ZERO (cpuset);
    CPUSET_SET (cpuset, cpuNum);

    /* wait till the CPU1 ready */

    while (!(vxCpuEnabledGet () & cpuset))
	{
	if (++i > timeout)
	    {
	    status = ERROR;
	    break;
	    }
	}

    /*
     * Deassert the common time base disable signal.
     * The timebase registers should contain equivalent value between two cores
     * and they will work in sync.
     */

    *(M8XXX_DEVDISR(CCSBAR)) &= ~M8641_DEVDISR_TB;

    /* Now PPC Dec timer re-started and safe to unblock interrupts. */

    intCpuUnlock (key);

    if (status == ERROR)
	if( _func_logMsg != NULL )
	    _func_logMsg ("sysCpuEnable(): Timeout (%d secs) waiting to enable "
		"additional CPU: %x\n", VX_ENABLE_CPU_TIMEOUT, cpuNum,
		0, 0, 0, 0);
    }
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    return status;
    }

/***************************************************************************
*
* sysCpuAvailableGet - return the number of CPUs available for SMP
*
* RETURNS: 1 if the board is configured for AMP operation, else
*          the actual number of CPU cores as reported by the EPIC.
*
*/

UINT32 sysCpuAvailableGet (void)
    {
    if (core1ampMode)
	return 1;

    return physCpuAvailableGet();
    }
#endif /* if defined(_WRS_CONFIG_SMP) */

#ifdef _WRS_VX_AMP
/*****************************************************************************
*
* sysAmpCpuStatusSet - Sets the status value of a specific CPU
*
* RETURNS: NONE
*
*/
void sysAmpCpuStatusSet(int status,int cpu)
    {
    /* only allow CPU1 to set it's own status */
    if( sysProcNumGet() == 1 )
	cpu = 1;

    switch(cpu)
	{
	case 1:
	    *((VINT32*)(AMP_CPU1_STATUS | CORE1T_OFFSET)) = status;
	    break;
	case 0:
	    *((VINT32*)(AMP_CPU0_STATUS | CORE1T_OFFSET)) = status;
	    break;
	default:
	    break;
	}

    }

/*****************************************************************************
*
* sysAmpCpuStatusGet - Gets the status value of a specific CPU
*
* RETURNS: State of the specified CPU or Error if not a valid CPU.
*
*/
int sysAmpCpuStatusGet(int cpu)
    {
    int status = ampCpuStateError;

    switch(cpu)
	{
	case 1:
	    status = *((VINT32*)(AMP_CPU1_STATUS | CORE1T_OFFSET));
	    break;
	case 0:
	    status = *((VINT32*)(AMP_CPU0_STATUS | CORE1T_OFFSET));
	    break;
	default:
	    break;
	}
    return(status);
    }

#endif /* _WRS_VX_AMP */

/***************************************************************************
*
* physCpuAvailableGet - return the number of CPUs on the board
*
* RETURNS: the number of CPU cores, as reported by the EPIC.
*
*/

LOCAL UINT32 physCpuAvailableGet (void)
    {
    UINT32 avail = 1;

    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuAvail), (void *)&avail);

    return (avail);
    }

/***************************************************************************
*
* bspSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* The sysSerialChanGet() routine returns a pointer to the SIO_CHAN
* device associated with a specified serial channel. It is called
* by usrRoot() to obtain pointers when creating the system serial
* devices, `/tyCo/x'. It is also used by the WDB agent to locate its
* serial channel.  The VxBus function requires that the BSP provide a
* function named bspSerialChanGet() to provide the information about
* any non-VxBus serial channels, provided by the BSP.  As this BSP
* does not support non-VxBus serial channels, this routine always
* returns ERROR.
*
* RETURNS: ERROR, always
*
*/

SIO_CHAN * bspSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    return((SIO_CHAN *)ERROR);
    }

LOCAL BOOL sysCpu1Check (void)
    {
    return (CPU1CHECK);
    }

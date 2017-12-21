/* sysLib.c - mv7100 system-dependent library */

/*
 * Copyright (c) 2008-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01q,12nov12,x_s Fix the AMP device filter. (WIND00387606)
01p,26jun12,wqi added sysPci2SetIntrRotation to correct int line assignment.
                (WIND00221935)
01o,25jan11,c_t Roll back 01m due to defect rejected.(WIND00225346)
01n,12jan11,syt added sysMsDelay() and sysCpu1ShutdownLoop(), updated sysToMonitor()
                to fix SMP image reboot hang issue.(WIND00249848)
01m,26aug10,liu add include pci/pciConfigLib.c to avoid build error. (WIND00225346)
01l,30apr10,dhy delete _MMU_UBAT_VP attribute(WIND00162540)
01k,26apr10,e_d replace sysSvrGet() with vxSvrGet() of vxALib.s
01j,26apr10,e_d remove sysGetMpxClkRtc() and set oscFreq variable to 66666666
01i,20dec09,e_d add sysAmpCpuStatus check function
01h,15dec09,e_d remove send ipi code in sysCpu1loop function
01g,03nov09,e_d add smNet between cores
01f,16feb09,e_d change CPU1_INIT_START_FUNC from sysCpu1loop to RAM_LOW_ADRS
01e,12feb09,e_d add wrLoad support
01d,07jan09,e_d add INCLUDE_PCI_BUS
01c,10sep08,y_w fixed Copyright notice
01b,25apr08,mdg fixed bug when INCLUDE_SM_COMMON was undef'ed
                 and compiler warnrings.
01a,17mar08,mdg created from MVME7100 and hpc8641net BSPs.
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    sysEpic.c           - programmable interrupt controller driver
    flashMem.c          - 29F040 flash memory device driver.
    sysCacheLib.s       - L1 and L2 cache lock support
    sysDuart.c          - Serial support
    ns16550.c           - optional now linked via vxBus
    ppcDecTimer         - Optional now linked via vxBus
    motTsecEnd.c        - Optional vxBus HEnd driver now used

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
#include <smUtilLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#ifdef INCLUDE_CACHE_L2
#include "sysL2Cache.h"
#endif /* INCLUDE_CACHE_L2 */

#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#   include <drv/pci/pciAutoConfigLib.h>
#   include "mot85xxPci.h"
#endif /* INCLUDE_PCI_BUS */


#if defined (INCLUDE_ALTIVEC)
#include <altivecLib.h>
#endif /* INCLUDE_ALTIVEC */

#ifdef _WRS_CONFIG_SMP
#   include <private/kernelLibP.h>
#endif /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_AMP
#include <vxIpiLib.h>
IMPORT int sysStartType;
#endif /* INCLUDE_AMP */

IMPORT int       (* _func_altivecProbeRtn) () ;

/*
 * This must be updated after the bore bring-up specified in the SMP HLD
 * implemented.
 */

IMPORT cpuset_t  vxCpuEnabled;

/* defines */

#undef  BUS_PROBE_TEST      /* test routines for sysBusProbe support */

#define ZERO        0

#define MODSTR_SIZE 80

#define PCI_CLASS_BRIDGE	0x06
#define PCI_P2P_BRIDGE		0x04

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


UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT +_MMU_NUM_EXTRA_IBAT+_MMU_NUM_EXTRA_DBAT)] =
    {

    /* I BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128M |
    _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT),

    /* I BAT 1 */

    0,0,

    /* I BAT 2 */

    0,0,

    /* I BAT 3 */

    0,0,

    /* D BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128M |
    _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT),

    /* D BAT 1 */

    ((CCSBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M |
    _MMU_UBAT_VS),
    ((CCSBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 */

    ((LOCAL_BOARD_ACCESS_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256K |
     _MMU_UBAT_VS),
    ((LOCAL_BOARD_ACCESS_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT),

    /* D BAT 3 */

    0,0

    /*
     * These entries are for the I/D BAT's (4-7) on the MPC7455/755.
     * They should be defined in the following order.
     * IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
     * DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
     */
    ,
    /* I BAT 4 */

    0,0,

    /* I BAT 5 */

    0,0,

    /* I BAT 6 */

    0,0,

    /* I BAT 7 */

    0,0,

    /* D BAT 4 */

    ((SBC_MRAM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_512K |
    _MMU_UBAT_VS),
    ((SBC_MRAM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 5 */

    ((PCI1_IO_LOCAL_START & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M |
     _MMU_UBAT_VS),
    ((PCI1_IO_LOCAL_START & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 6 */

    ((PCI0_IO_LOCAL_START & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M |
     _MMU_UBAT_VS),
    ((PCI0_IO_LOCAL_START & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 7 */

#ifdef _WRS_VX_AMP
    ((0x10000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((0x10000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_MEM_COHERENT),
#else
    0,0,
#endif /* _WRS_VX_AMP */

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
 * Total mapped memory      Page Table size
 * -------------------      ---------------
 *        8 Meg              64 K
 *       16 Meg             128 K
 *       32 Meg             256 K
 *       64 Meg             512 K
 *      128 Meg               1 Meg
 *  .               .
 *  .               .
 *  .               .
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
    }
    ,
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
    ,
    {

    /* Boot Flash bank mapping */

    (VIRT_ADDR) FLASH_BOOT_ADRS,
    (PHYS_ADDR) FLASH_BOOT_ADRS,
    0x0,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Local Bus I/O */

    (VIRT_ADDR) LOCAL_BOARD_ACCESS_ADRS,
    (PHYS_ADDR) LOCAL_BOARD_ACCESS_ADRS,
    LOCAL_BOARD_ACCESS_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* MRAM */

    (VIRT_ADDR) SBC_MRAM_ADRS,
    (PHYS_ADDR) SBC_MRAM_ADRS,
    SBC_MRAM_SZ,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to VME A32 space */

    (VIRT_ADDR) VME_A32_MSTR_LOCAL,
    (PHYS_ADDR) VME_A32_MSTR_LOCAL,
    VME_A32_MSTR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to VME A24 space */

    (VIRT_ADDR) VME_A24_MSTR_LOCAL,
    (PHYS_ADDR) VME_A24_MSTR_LOCAL,
    VME_A24_MSTR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to VME A16 space */

    (VIRT_ADDR) VME_A16_MSTR_LOCAL,
    (PHYS_ADDR) VME_A16_MSTR_LOCAL,
    VME_A16_MSTR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to PCI0 non-prefetchable memory space */

    (VIRT_ADDR) PCI0_MSTR_MEMIO_LOCAL,
    (PHYS_ADDR) PCI0_MSTR_MEMIO_LOCAL,
    PCI0_MSTR_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to PCI0 prefetchable memory space */

    (VIRT_ADDR) PCI0_MSTR_MEM_LOCAL,
    (PHYS_ADDR) PCI0_MSTR_MEM_LOCAL,
    PCI0_MSTR_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT
    },

    {

    /* Access to PCI0 16-bit ISA I/O space */

    (VIRT_ADDR) ISA0_MSTR_IO_LOCAL,
    (PHYS_ADDR) ISA0_MSTR_IO_LOCAL,
    ISA0_MSTR_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to PCI0 32-bit I/O space */

    (VIRT_ADDR) PCI0_MSTR_IO_LOCAL,
    (PHYS_ADDR) PCI0_MSTR_IO_LOCAL,
    PCI0_MSTR_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to Tempe Bridge non-prefetchable memory space */

    (VIRT_ADDR) PCI0_TEMPE_BRIDGE_MEM,
    (PHYS_ADDR) PCI0_TEMPE_BRIDGE_MEM,
    PCI0_TEMPE_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to Tempe non-prefetchable memory space */

    (VIRT_ADDR) PCI0_TEMPE_MEM,
    (PHYS_ADDR) PCI0_TEMPE_MEM,
    PCI0_TEMPE_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to PCI1 non-prefetchable memory space */

    (VIRT_ADDR) PCI1_MSTR_MEMIO_LOCAL,
    (PHYS_ADDR) PCI1_MSTR_MEMIO_LOCAL,
    PCI1_MSTR_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to PCI1 prefetchable memory space */

    (VIRT_ADDR) PCI1_MSTR_MEM_LOCAL,
    (PHYS_ADDR) PCI1_MSTR_MEM_LOCAL,
    PCI1_MSTR_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT
    },

    {

    /* Access to PCI1 16-bit ISA I/O space */

    (VIRT_ADDR) ISA1_MSTR_IO_LOCAL,
    (PHYS_ADDR) ISA1_MSTR_IO_LOCAL,
    ISA1_MSTR_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    },

    {

    /* Access to PCI1 32-bit I/O space */

    (VIRT_ADDR) PCI1_MSTR_IO_LOCAL,
    (PHYS_ADDR) PCI1_MSTR_IO_LOCAL,
    PCI1_MSTR_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID    | VM_STATE_WRITABLE     | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED
    }
    };

int   sysPhysMemDescNumEnt;     /* calculated in sysHwInit */

typedef struct phbWinStruct
    {
    UINT32 winType;   /* mem or i/o */
    UINT32 winBase;   /* start of window */
    UINT32 winLimit;  /* end of window */
    } PHB_WIN_STRUCT;

PHB_WIN_STRUCT sysPhbCpuToPciWin [2][7];
PHB_WIN_STRUCT sysPhbPciToCpuWin [2][7];
int   sysValidPhbWindows[2] = {0,0};    /* number of valid entries */
int   phbToPciWinCount[2] = {0,0};      /* number of valid Phb to PCI windows */
int   pciToPhbWinCount[2] = {0,0};      /* number of valid PCI to PHB windows */

int   sysBus      = VME_BUS;            /* legacy */
int   sysCpu      = CPU;                /* system CPU type (MC680x0) */
char *sysBootLine = BOOT_LINE_ADRS;     /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;       /* catastrophic message area */
int   sysProcNum  = 0;                  /* processor number of this CPU */
UINT32  coreFreq;

/* static locals */

LOCAL char   sysModelStr [MODSTR_SIZE];
LOCAL UINT32 core1ampMode;

#ifdef  _WRS_CONFIG_SMP
#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;  /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;  /* lower 32 bit value of timebase */
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
#endif  /* _VX_SMP */
LOCAL spinlockIsr_t sysIntConnectSpinlock;

/* Clock Ratio Tables */

#define MAX_VALUE_PLAT_RATIO 10
UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
    { 0, 0},
    { 0, 0},
    { 2, 0},
    { 3, 0},
    { 4, 0},
    { 5, 0},
    { 6, 0},
    { 7, 0},
    { 8, 0},
    { 9, 0}
};

UINT32 e600RatioLookup[][3] =
    {
    {0x10,2,0},
    {0x19,5,1},
    {0x20,3,0},
    {0x39,7,1},
    {0x28,4,0},
    {0x1d,5,1},
    {0}
    };

UINT32 sysClkFreqGet(void);
UINT32 sysPicClkFreqGet(UINT32);

/* forward declarations */

char *  sysPhysMemTop (void);     /* Top of physical memory */
char *  sysInfo (void);
void    sysMemMapDetect (void);  /* detect PREP or CHRP map */
void    sysDelay (void);              /* delay 1 millisecond */
UINT32  sysCpuAvailableGet (void);
LOCAL UINT32 physCpuAvailableGet (void);
char * sysBspRev (void);
UINT32 sysBaudClkFreq (void);
void   sysHwInit (void);
char * sysPhysMemTop (void);
char * sysMemTop (void);
UINT32 sysDramSize (void);
STATUS sysToMonitor (int startType);
void   sysHwInit2 (void);
int    sysProcNumGet (void);
void   sysProcNumSet (int procNum);
STATUS sysLocalToBusAdrs (int adrsSpace, char * localAdrs, char ** pBusAdrs);
STATUS sysBusToLocalAdrs (int adrsSpace, char * busAdrs, char ** pLocalAdrs);
BOOL   sysBusTas (char * adrs);
#if defined(INCLUDE_SM_COMMON) && defined(INCLUDE_SMFORVME)
LOCAL void sysBusTasClear (char *);
LOCAL BOOL sysVmeBusLockTas (char *);
LOCAL BOOL sysVmeRmwTas (char *);
LOCAL BOOL sysVmeRmw (char *);
#endif /* INCLUDE_SMFORVME */
UCHAR   sysNvRead (ULONG,UINT32);
void    sysNvWrite (ULONG,UCHAR,UINT32);
void    sysDebugMsg (char *, UINT32);
void sysFlashSwWriteProtectSet (BOOL setOn);
BOOL sysFlashIsHwWriteProtected (void);

STATUS sysMv7100IntConnect(VOIDFUNCPTR *vector,
                                   VOIDFUNCPTR routine, int parameter);
STATUS sysMv7100IntEnable(int intNum);
STATUS sysMv7100IntDisable(int intNum);
STATUS sysUsrIntEnable(int intNum);
STATUS sysUsrIntDisable(int intNum);
void   sysUsDelay (UINT32 delay);
void   sysPhbCapt (void);
UINT32 getFlashBaseAddress (void);
void   necPd720101ExtConfig (void);

#ifdef INCLUDE_AMP
BOOL sysCoreLockTake(char *);
BOOL sysCoreLockTaskTake(char *);
void sysCoreLockGive(char *);
#endif /* INCLUDE_AMP */

#ifdef INCLUDE_MMU      /* add a sysPhysMemDesc entry */
STATUS  sysMmuMapAdd (VIRT_ADDR address, UINT32 len,
                             UINT32 initialStateMask, UINT32 initialState);
#endif /* INCLUDE_MMU */

#ifdef INCLUDE_PCI_BUS
STATUS sysPci2SetIntrRotation (VXB_DEVICE_ID pDev, PCI_LOC * pLoc,
                               int pciBus, UCHAR * rotVal);
#endif /* INCLUDE_PCI_BUS */

/* external imports */

IMPORT int    sysStartType;
IMPORT VOID   sysOutWord (ULONG address, UINT16 data);  /* sysALib.s */
IMPORT VOID   sysOutLong (ULONG address, ULONG data);   /* sysALib.s */
IMPORT USHORT sysInWord (ULONG address);                 /* sysALib.s */
IMPORT ULONG  sysInLong (ULONG address);                 /* sysALib.s */
IMPORT void   vxSdaInit (void);
IMPORT VOID   sysOutByte (ULONG, UCHAR);                /* sysALib.s */
IMPORT UCHAR  sysInByte (ULONG);                         /* sysALib.s */
IMPORT STATUS sysMemProbeSup (int length, char * src, char * dest);
IMPORT UINT32 sysTblGet();
IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT void   sysMpc8641PhbInit (void);
IMPORT void   sysMotI2CInit (void);

IMPORT void   sysTempeReset (void);
IMPORT void   sysTempeInit (void);
IMPORT STATUS sysVmeToPciAdrs (int, char *, char **);
IMPORT STATUS sysPciToVmeAdrs (int, char *, char **);

IMPORT VOIDFUNCPTR _pSysL3CacheFlushDisableFunc;
IMPORT VOIDFUNCPTR _pSysL3CacheInvalEnableFunc;
IMPORT void   sysSerialConnectAll();

IMPORT STATUS sysMotI2cRead (UINT32 devAddr, UINT32 devOffset,
                                  UINT32 devAddrBytes, UCHAR *spdData,
                                  UINT32 numBytes);
IMPORT STATUS sysMotI2cWrite (UINT32 devAddr, UINT32 devOffset,
                                  UINT32 devAddrBytes, UINT8  *pBfr,
                                  UINT32 numBytes);
IMPORT UCHAR  sysProductStr [];
IMPORT UCHAR  sysCpuStr [];
IMPORT BOOL   isMappedToVME (char *, char **, UINT32 *);
IMPORT STATUS sysVMEProbe (char *, int, int, char *);
IMPORT int    vxbPciConfigBdfPack(int busNo, int deviceNo, int funcNo);

IMPORT STATUS vxbPciFindDevice(VXB_DEVICE_ID busCtrlID, int vendorId, 
                               int deviceID, int index, int *pBusNo, 
                               int *pDeviceNo, int *pFuncNo);
IMPORT STATUS vxbPciConfigInByte(VXB_DEVICE_ID busCtrlID, int busNo, 
                                 int deviceNo, int funcNo, int offset, 
                                 UINT8 *pData);

/*
 * mmuPpcBatInitMPC74x5 initializes the standard 4 (0-3)  I/D BAT's &
 * the additional 4 (4-7) I/D BAT's present on the MPC74[45]5.
 */
IMPORT void   mmuPpcBatInitMPC74x5 (UINT32 *pSysBatDesc);

/*
 * _pSysBatInitFunc needs to be set to either of the above 2 depending
 * on which processor it is running to make use of the additional BAT's
 * on the MPC 7455/755.
 * If this is not set or is set to NULL then the standard mmuPpcBatInit
 * fn. would be called which would initialize the (0-3) I/D BAT's
 */

IMPORT FUNCPTR _pSysBatInitFunc;

/*
 *By default this is set to NULL in the mmu Library and  in that case
 * the standard BAT init func. to initialize the 4 I/D BAT's is called.
 * If the standard & Extra BAT's need to be initialized then set this
 * to the ones supplied by the mmuLib or else implement your own fn. and
 * supply it here.
 */

IMPORT STATUS  (*_func_intConnectRtn) (VOIDFUNCPTR *, VOIDFUNCPTR, int);
IMPORT int     (*_func_intEnableRtn)  (int);
IMPORT int     (*_func_intDisableRtn) (int);
LOCAL  STATUS  (*_vxb_intConnectRtn)  (VOIDFUNCPTR *, VOIDFUNCPTR, int);
LOCAL  int     (*_vxb_intEnableRtn)   (int);
LOCAL  int     (*_vxb_intDisableRtn)  (int);

#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheArchDisableFromMmu (CACHE_TYPE cache);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void   sysSmpTimeBaseSet (UINT32 tbHI, UINT32 tbLO);

#define VX_CPC_ASYNC    2
typedef void (*CPC_FUNC) (void *, int);
IMPORT void cpcInvoke (cpuset_t, CPC_FUNC, void *, int, int);
#endif  /* _WRS_CONFIG_SMP */

IMPORT void sysCpu1Start (void);
IMPORT void sysCpu1Stop (void);
#ifdef _WRS_VX_AMP
METHOD_DECL(sysBspDevFilter);
STATUS sysDeviceFilter (VXB_DEVICE_ID pDev);
void sysAmpCpuStatusSet(int status,int cpu);
int sysAmpCpuStatusGet(int cpu);
#endif /* _WRS_VX_AMP */

/* BSP DRIVERS */

IMPORT void hardWareInterFaceInit();

#include "hwconf.c"

IMPORT device_method_t * pSysPlbMethods;

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

LOCAL struct vxbDeviceMethod mv7100PlbMethods[] =
    {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysProcNumGet),
#ifdef _WRS_VX_AMP
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
#endif /* _WRS_VX_AMP */
    { 0, 0 }
    };

#ifdef _WRS_VX_AMP

typedef struct {
    char* deviceName;
    int   unit;
    int   cpu;
    }AMP_CPU_TABLE;

/*
 * Table includes only devices we want to filter
 * if no action required for a device per CPU then it's not in the table
 * Because we should use the m85xx pci controller 1 to decide the pci to VME
 * bridge at which core, so we couldn't move the location for m85xxPci
 * controller 0 and m85xxPci controller 1 in ampCpuTable.
 */

AMP_CPU_TABLE ampCpuTable[] = {
    { "m85xxPci",  0, 0 },
    { "m85xxPci",  1, 0 },
    { XTSEC_NAME,  0, 0 },
    { XTSEC_NAME,  1, 1 },
    { XTSEC_NAME,  2, 0 },
    { XTSEC_NAME,  3, 1 },
    { "ns16550",   0, 0},
#ifdef INCLUDE_QUAD_UART
    { "ns16550",   1, 1},
    { "ns16550",   2, 1},
    { "ns16550",   3, 1},
    { "ns16550",   4, 1},
#endif /* INCLUDE_QUAD_UART */
    { "openPicTimer", 0,  0 },
    { "openPicTimer", 1,  0 }
};

#endif /* _WRS_VX_AMP */
/*
 * By default this is set to NULL in the mmu Library and  in that case
 * the standard BAT init func. to initialize the 4 I/D BAT's is called.
 * If the standard & Extra BAT's need to be initialized then set this
 * to the ones supplied by the mmuLib or else implement your own fn. and
 * supply it here.
 */

#if ((defined INCLUDE_RTC) && (defined INCLUDE_DOSFS))
    IMPORT void dosFsDateTimeInstall(FUNCPTR);
#endif /* INCLUDE_RTC, INCLUDE_DOSFS */

#ifdef INCLUDE_TFFS
    IMPORT void sysTffsMountDevs (void);
#endif /* INCLUDE_TFFS */

#ifdef INCLUDE_SM_COMMON
#   include "sysSmEnd.c"
#endif /* INCLUDE_SM_COMMON */

/* BSP DRIVERS */

#include "sysTempe.c"
#include "sysMotVpd.c"                /* VPD Support */

#ifdef INCLUDE_NETWORK
#   include "sysNet.c"                /* network setup */
#endif  /* INCLUDE_NETWORK */

#include "mem/byteNvRam.c"            /* NVRAM Support */

#ifdef INCLUDE_ADT7461
#   include "sysAdt7461TempSensor.c"  /* ADT7461 Temp Sensor Support */
#endif /* INCLUDE_ADT7461 */

#ifdef INCLUDE_RTC
#   include "sysDs1375.c"             /* Maxim DS1375 RTC with Alarm */
#endif /* INCLUDE_RTC */

#ifdef INCLUDE_SHOW_ROUTINES
#   include "sysMotVpdShow.c"         /* VPD Show Routines */
#   include "sysStatusRegsShow.c"
#endif /* INCLUDE_SHOW_ROUTINES */

#if ((defined INCLUDE_TFFS)  || (defined INCLUDE_BOOT_IMAGE_PROGRAM))
#   include "sysAmdS29GLxxxN.c"
#endif /* INCLUDE_TFFS, INCLUDE_BOOT_IMAGE_PROGRAM */

#ifdef INCLUDE_BOOT_IMAGE_PROGRAM
#   include "sysFlashUtil.c"          /* FLASH driver interface support */
#   include "sysProgramBootImage.c"   /* Boot Image Programming support */
#endif /* INCLUDE_BOOT_IMAGE_PROGRAM */

#ifdef INCLUDE_WDOG_TIMER
#   include "sysMv7100Wdog.c"
#endif /* INCLUDE_WDOG_TIMER */

#define HID1_ABE 0x800
#define HID1_SYNCBE 0x400
#define HID1_EMCP 0x80000000

/*******************************************************************************
*
* getFlashBaseAddress - return the base address of the flash bank
*
* This routine will return the base address of the flash bank.
*
* RETURNS: base address of flash bank
*/

UINT32 getFlashBaseAddress (void)
    {
    return(CCSR_READ32 (CCSBAR, CCSR_BR0) & 0xFFFF8000);
    }

/*******************************************************************************
*
* sysFlashMappingConfig - modify flash bank mapping
*
* This routine modifies the flash entry in sysPhysMemDesc and also
* adjusts the programming of the local bus controller to match the
* actual size of the flash bank.
*
* RETURNS: NA
*/

void sysFlashMappingConfig (void)
    {
    UINT   i;
    VPD_PACKET ** pVpdPkts;     /* board vpd packets */

    UCHAR type;                 /* board vpd packet type */
    UINT32 flashSize = 0x8000000;
    UINT32 flashBaseAddress;
    UINT32 br0;
    UINT32 or0;

    /*
     * if the first entry in the vpd packet pointer array is null, then
     * the vpd data needs to be read into memory and parsed.
     */

    if (sysVpdPkts[0] == NULL)
        {
        printf ("Initialize VPD packets.\n\r");
        if (sysMotVpdInit() != OK )
            {
            printf ("VPD structure invalid.\n");
            return;
            }
        }

    if (!CPU1CHECK)
        {

        /* Initialize the vpd packet pointer and type to the first packet. */

        pVpdPkts = (VPD_PACKET **)&sysVpdPkts[0];
        type = (*pVpdPkts)->type;

        /* Find the VPD packet for the FLASH memory */

        while (type != VPD_PID_TERM && type != VPD_PID_FMC)
            {
            pVpdPkts++;
            type = (*pVpdPkts)->type;
            }

        if (type != VPD_PID_FMC)
            {
            printf ("Cannot read the FLASH memory VPD.\n\r");
            return;
            }

        flashSize = (1 << ((*pVpdPkts)->data[VPD_FMC_SIZE] + 18));
#ifdef INCLUDE_AMP
        *((UINT32 *) SM_FLASH_SIZE) = flashSize;
        }
    else
        {
        flashSize = *((UINT32 *) SM_FLASH_SIZE);
        }
#else
        }
#endif /* INCLUDE_AMP */

    flashBaseAddress = (0xFFFFFFFF - flashSize) + 1;

    for (i = 0; i < (sizeof(sysPhysMemDesc)/sizeof(PHYS_MEM_DESC)); i++)
    {
    if (sysPhysMemDesc[i].virtualAddr == (VIRT_ADDR)FLASH_BOOT_ADRS)
        {
        sysPhysMemDesc[i].physicalAddr = (PHYS_ADDR)flashBaseAddress;
        sysPhysMemDesc[i].virtualAddr = (PHYS_ADDR)flashBaseAddress;
        sysPhysMemDesc[i].len = flashSize;
        break;
        }
    }

    /* local bus controller */

    br0 = CCSR_READ32 (CCSBAR, CCSR_BR0);
    br0 &= 0x00007FFF;
    br0 |= flashBaseAddress;
    CCSR_WRITE32 (CCSBAR, CCSR_BR0, br0);

    or0 =  CCSR_READ32 (CCSBAR, CCSR_OR0);
    or0 &= 0x00007FFF;
    or0 |= (CCSR_ORn_AM_VAL(flashSize));
    CCSR_WRITE32 (CCSBAR, CCSR_OR0, or0);
    }

/*******************************************************************************
*
* isPciAdrs - determine if local address is a PCI address.
*
* This routine determines if a given address maps to the PCI interface
* through a host bridge PCI mapping.
*
* RETURNS: TRUE if address is mapped to PCI interface, FALSE if not.
*/

BOOL isPciAdrs
    (
    char *  adrs,        /* Local address to examine to see if it's PCI */
    char ** pciTrans    /* If it's PCI this will be the PCI bus address */
    )
    {
    int i,ctrl;

    for (ctrl=0;ctrl<2;ctrl++)
        {
        for (i = 0; i < phbToPciWinCount[ctrl]; i++)
            {
            if ( ((UINT32)adrs >= sysPhbCpuToPciWin[ctrl][i].winBase) &&
                 ((UINT32)adrs <=  sysPhbCpuToPciWin[ctrl][i].winLimit) )
                {
                *(UINT32 *)(pciTrans) =
                        sysPhbPciToCpuWin[ctrl][i].winBase +
                        ((UINT32)adrs - sysPhbCpuToPciWin[ctrl][i].winBase);
                return (TRUE);
                }
            }
        }
    return (FALSE);
    }

/*******************************************************************************
*
* isVmeAdrs - determine if local address is a VME address
*
* This routine determines if a given address maps to the VME interface
* through a Tempe outbound window.
*
* RETURNS: TRUE if address is mapped to VME interface, FALSE if not.
* If return value is TRUE, the VME bus translated address and the address
* of the associated Outbound control register is returned.
*/

BOOL isVmeAdrs
    (
    char   *  adrs,      /* Local address to examine to see if it's VME */
    char   ** vmeTrans,  /* If it's VME, this will be the VME bus address */
    UINT32 *  outRegSet  /* Number of outbound register set */
    )
    {
    BOOL stat = FALSE;
    char * pciTrans;

    /*
     * First determine if it's a PCI address since on this board,
     * any VME address must first be a PCI address.  If it is not
     * a PCI address we can return FALSE right away.
     */

    if (!isPciAdrs(adrs, &pciTrans))
        return FALSE;

    /* Now check the legitimate PCI address to see if it maps to VME */

    stat = isMappedToVME (adrs, vmeTrans, outRegSet);

    return (stat);
    }

#ifdef INCLUDE_PCI_BUS
/*******************************************************************************
*
* sysPciAltConfigRead - read from the PCI configuration space on second PCIE Bus
*
* This routine reads either a byte, word or a long word specified by
* the argument <width>, from the PCI configuration space
*
* NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*/

STATUS sysPciAltConfigRead
    (
    int    busNo,    /* bus number */
    int    deviceNo, /* device number */
    int    funcNo,   /* function number */
    int    offset,   /* offset into the configuration space */
    int    width,    /* width to be read */
    void * pData /* data read from the offset */
    )
    {
    UINT8  retValByte = 0;
    UINT16 retValWord = 0;
    UINT32 retValLong = 0;
    STATUS retStat = ERROR;
    int configAddrReg;
    int configDataReg;

    configAddrReg = (int)(CCSBAR + CCSR_PCIE1_CFG_ADDR);
    configDataReg = (int)(CCSBAR + CCSR_PCIE1_CFG_DATA);

    switch (width)
        {
        case 1: /* byte */
            PCI_OUT_LONG (configAddrReg,
                          (LONGSWAP((UINT32)vxbPciConfigBdfPack (busNo,
                           deviceNo, funcNo) | (offset & 0xfc) | 0x80000000)));
            retValByte = PCI_IN_BYTE (configDataReg + (offset & 0x3));
            *((UINT8 *)pData) = retValByte;
            retStat = OK;
            break;

        case 2: /* word */
            PCI_OUT_LONG (configAddrReg,
                          (LONGSWAP((UINT32)vxbPciConfigBdfPack (busNo,
                           deviceNo, funcNo) | (offset & 0xfc) | 0x80000000)));
            retValWord = PCI_IN_WORD (configDataReg + (offset & 0x2));
            *((UINT16 *)pData) = retValWord;
            retStat = OK;
            break;

        case 4: /* long */
            PCI_OUT_LONG (configAddrReg,
                          (LONGSWAP((UINT32)vxbPciConfigBdfPack (busNo,
                           deviceNo, funcNo) | (offset & 0xfc) | 0x80000000)));
            retValLong = PCI_IN_LONG (configDataReg);
            *((UINT32 *)pData) = retValLong;
            retStat = OK;
            break;

        default:
            retStat = ERROR;
            break;
        }

    return (retStat);
    }

/*******************************************************************************
*
* sysPciAltConfigWrite - write to the PCI configuration space on second PCIE Bus
*
* This routine writes either a byte, word or a long word specified by
* the argument <width>, to the PCI configuration space
*
* NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*/

STATUS sysPciAltConfigWrite
    (
    int busNo,    /* bus number */
    int deviceNo, /* device number */
    int funcNo,   /* function number */
    int offset,   /* offset into the configuration space */
    int width,    /* width to write */
    ULONG data    /* data to write */
    )
    {
    STATUS status = OK;
    int configAddrReg;
    int configDataReg;

    configAddrReg = (int)(CCSBAR + CCSR_PCIE1_CFG_ADDR);
    configDataReg = (int)(CCSBAR + CCSR_PCIE1_CFG_DATA);

    switch (width)
        {
        case 1: /* byte */
            PCI_OUT_LONG (configAddrReg,
                          (LONGSWAP((UINT32)vxbPciConfigBdfPack (busNo,
                           deviceNo, funcNo) | (offset & 0xfc) | 0x80000000)));
            PCI_OUT_BYTE ((configDataReg + (offset & 0x3)), data);
            break;

        case 2: /* word */
            PCI_OUT_LONG (configAddrReg,
                          (LONGSWAP((UINT32)vxbPciConfigBdfPack (busNo,
                           deviceNo, funcNo) | (offset & 0xfc) | 0x80000000)));
            PCI_OUT_WORD ((configDataReg + (offset & 0x2)), data);
        break;

        case 4: /* long */
            PCI_OUT_LONG (configAddrReg,
                          (LONGSWAP((UINT32)vxbPciConfigBdfPack (busNo,
                           deviceNo, funcNo) | (offset & 0xfc) | 0x80000000)));
            PCI_OUT_LONG (configDataReg, data);
            break;

        default:
            status = ERROR;
        }

    return (status);
    }

/*******************************************************************************
*
* sysProbeErrClr - clear errors associated with probing an address on a bus.
*
* This routine clears the error flags and conditions in the Host Bridge and
* PowerPC registers arising from probing addresses.
*
* RETURNS: N/A
*/

void sysProbeErrClr (void)
    {
    UINT16 pciStatus = 0;

    /* Clear relevant host bridge error registers */

    CCSR_WRITE32 (CCSBAR, CCSR_PCIE0_ERR_DR, 0xFFFFFFFF);

    pciConfigInWord  (PCI_HOST_BRIDGE_BUS_NUM,
                      PCI_HOST_BRIDGE_DEV_NUM,
                      PCI_HOST_BRIDGE_FUNC_NUM,
                      0x06,&pciStatus);
    pciStatus |= 0xF900;
    pciConfigOutWord (PCI_HOST_BRIDGE_BUS_NUM,
                      PCI_HOST_BRIDGE_DEV_NUM,
                      PCI_HOST_BRIDGE_FUNC_NUM,
                      0x06,pciStatus);
    sysPciAltConfigRead  (PCI_HOST_BRIDGE_BUS_NUM,
                          PCI_HOST_BRIDGE_DEV_NUM,
                          PCI_HOST_BRIDGE_FUNC_NUM,
                          0x06,2,&pciStatus);
    pciStatus |= 0xF900;
    sysPciAltConfigWrite (PCI_HOST_BRIDGE_BUS_NUM,
                          PCI_HOST_BRIDGE_DEV_NUM,
                          PCI_HOST_BRIDGE_FUNC_NUM,
                          0x06,2,pciStatus);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE0_ERR_CAP_STAT, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE0_ERR_CAP_R0, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE0_ERR_CAP_R1, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE0_ERR_CAP_R2, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE0_ERR_CAP_R3, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE1_ERR_CAP_STAT, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE1_ERR_CAP_R0, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE1_ERR_CAP_R1, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE1_ERR_CAP_R2, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_PCIE1_ERR_CAP_R3, 0x0);

    /* Clear PowerPC `Data Storage' Exception Registers */

    vxSrr0Set  (0);
    vxSrr1Set  (0);
    }

/*******************************************************************************
*
* checkHbStat - check for master abort or target abort on host bridge.
*
* This routine checks the appropriate host bridge registers to determine
* if a master abort or target abort has occurred since the last time these
* error status were cleared.
*
* RETURNS: ERROR if master or target abort is indicated, OK otherwise.
*/

LOCAL STATUS checkHbStat (void)
    {
    STATUS stat;
    UINT16 status0, status1;

    pciConfigInWord (PCI_HOST_BRIDGE_BUS_NUM,
                     PCI_HOST_BRIDGE_DEV_NUM,
                     PCI_HOST_BRIDGE_FUNC_NUM,
                     0x06,&status0);
    sysPciAltConfigRead (PCI_HOST_BRIDGE_BUS_NUM,
                         PCI_HOST_BRIDGE_DEV_NUM,
                         PCI_HOST_BRIDGE_FUNC_NUM,
                         0x06,2,&status1);


    if (((status0 & (PCI_STATUS_TARGET_ABORT_RCV |
           PCI_STATUS_MASTER_ABORT_RCV)) == 0) &&
        ((status1 & (PCI_STATUS_TARGET_ABORT_RCV |
           PCI_STATUS_MASTER_ABORT_RCV)) == 0))
        stat = OK;
    else
        stat = ERROR;

    return (stat);
    }
#endif

/*******************************************************************************
*
* sysPciProbe - probe a PCI bus address
*
* This routine probes an address on the PCI bus. All probing is done with
* interrupts disabled.
*
* RETURNS: OK or ERROR if address cannot be probed
*/

STATUS sysPciProbe
    (
    char   * adrs,    /* address to be probed */
    int      mode,    /* VX_READ or VX_WRITE */
    int      length,  /* 1, 2 or 4 byte probe */
    char   * pVal     /* address of value to write OR */
                      /* address of location to place value read */
    )
    {
    STATUS status = ERROR;
    int    oldLevel;
    char   reg8;

    /* probe performed with interrupts disabled */

    oldLevel = intCpuLock ();

    /* flush PCI posted write buffer(s) */

    if ((status = vxMemArchProbe (adrs, mode, length, pVal)) == OK )
        {
        if (mode == VX_WRITE)
            reg8 = *adrs;               /* flush the write */
#ifdef INCLUDE_PCI_BUS
        status = checkHbStat();
#endif /*INCLUDE_PCI_BUS */
        }

    /* safe to re-enable interrupts */

    intCpuUnlock (oldLevel);

    return (status);
    }

/*******************************************************************************
*
* sysBusProbe - probe a bus address based on bus type.
*
* This routine is a function hook into vxMemProbe.  It determines which bus,
* PCI, VME or local is being probed based on the address to be probed.
* If the PCI bus is being probed, the sysPciProbe() routine is called to do the
* special PCI probing. If the local bus is being probed, the routine calls an
* architecture-specific probe routine. If the VME bus is being probed, the
* sysVMEProbe() routine is called to do the special VME probing.
*
* RETURNS: ERROR if the probed address does not respond or causes a MMU fault.
* Returns OK if the probed address responds.
*/

STATUS  sysBusProbe
    (
    char   * adrs,  /* address to be probed */
    int      mode,  /* VX_READ or VX_WRITE */
    int      length,    /* 1, 2 or 4 byte probe */
    char   * pVal   /* address of value to write OR */
                    /* address of location to place value read */
    )
    {
    STATUS   stat;
    UINT32   outRegSet;
    char   * pciTrans;
    char   * vmeTrans;
    int      key;

    key = intCpuLock ();   /* Perform probe with interrupts locked */

    /* Clear any existing errors/exceptions */

#ifdef INCLUDE_PCI_BUS
    sysProbeErrClr ();
#endif /*INCLUDE_PCI_BUS */

    /* If the address is a DRAM address ... */

    if (((adrs == LOCAL_MEM_LOCAL_ADRS) ||
         (adrs >  LOCAL_MEM_LOCAL_ADRS)) &&
         (adrs < sysPhysMemTop()))
        stat = vxMemArchProbe (adrs, mode, length, pVal);

    /* ... else if the address is a VME address */

    else if (isVmeAdrs(adrs, &vmeTrans, &outRegSet))
        stat = sysVMEProbe(adrs, mode, length, pVal);

    /* ... else if the address is a PCI address */

    else if (isPciAdrs(adrs, &pciTrans))
        stat = sysPciProbe (adrs, mode, length, pVal);

    /*
     * else, the address is not DRAM, VME, or PCI but it could be
     * something like host bridge register space, an address which is none
     * of the above but it is still mapped and will respond to a probe.
     * If that's the case then vxMemArchProbe() should tell us so.
     */

    else
        stat = vxMemArchProbe (adrs, mode, length, pVal);

    /* Clear any errors/exceptions before exiting */

#ifdef INCLUDE_PCI_BUS
    sysProbeErrClr ();
#endif /*INCLUDE_PCI_BUS */
    intCpuUnlock (key);

    return (stat);
    }

/*******************************************************************************
*
* sysNvRead - read one byte from NVRAM
*
* This routine reads a single byte from a specified offset in NVRAM.
* The "NVRAM" on this board is implemented via an EEPROM.
*
* RETURNS: The byte from the specified NVRAM offset if successfully read from
* NVRAM, otherwise zero.
*/

UCHAR sysNvRead
    (
    ULONG    offset,    /* NVRAM offset to read the byte from */
    UINT32   i2cAddr    /* I2C address of EEPROM */
    )
    {
    UCHAR data = 0x0;

    if ( sysMotI2cRead (i2cAddr, offset, 2, &data, 1) == OK )
        return (data);
    else
        return 0;
    }

/*******************************************************************************
*
* sysNvWrite - write one byte to NVRAM
*
* This routine writes a single byte to a specified offset in NVRAM.
* The "NVRAM" on this board is implemented via an EEPROM.
*
* RETURNS: N/A
*/

void sysNvWrite
    (
    ULONG    offset, /* NVRAM offset to write the byte to */
    UCHAR    data,   /* datum byte */
    UINT32   i2cAddr /* I2C Address of EEPROM */
    )
    {
    UCHAR writeData = data;

    sysMotI2cWrite (i2cAddr, offset, 2, &writeData, 1);
    }

/*******************************************************************************
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
#ifdef INCLUDE_SM_COMMON

/*******************************************************************************
*
* sysMv7100SmIntGen - Generate a shared memory interrupt
*
* This routine generates a shared memory interrupt.  If the CPU is on another
* board, it uses a VME Mailbox interrupt.  If it is the other core on this
* board, it uses a InterProessor Interrupt.
*
*
* RETURNS: OK or ERROR
*
*/
STATUS sysMv7100SmIntGen
    (
    int intArg1,
    int intArg2,
    int intArg3
    )
    {
    char * pMailbox = NULL;/* mailbox */
    char   bogus;
    if (sysBusToLocalAdrs (intArg1, (char *) intArg2, &pMailbox) == ERROR)
        {
        return (ERROR);
        }
    *pMailbox = (char) intArg3;
    CACHE_PIPE_FLUSH ();                 /* CACHE FLUSH  [SPR 68334] */
    bogus = *(volatile char *) pMailbox; /* BRIDGE FLUSH [SPR 68334] */

    return(OK);
    }

/*******************************************************************************
*
* sysMv7100SmInt - Core 0 Shared memory interrupt routine.
*
* This routine handles a shared memory interrupt.  If the interrupt
* destination is this core, then call the shared memory interrupt routine.
* If it is the other core on this board, then generate an IPI so that it
* will the call the shared memory interrupt routine with that core.
*
*
* RETURNS: N/A
*
*/
void sysMv7100SmInt(void)
    {
    char * pMailbox = NULL;/* mailbox */

    if (sysBusToLocalAdrs (VME_AM_EXT_SUP_DATA,
                           (char *) SM_INT_ARG2, &pMailbox) == ERROR)
        {
        return;
        }

#if defined(INCLUDE_AMP) && defined(INCLUDE_SMFORVME)
    if (*pMailbox == SIG1_INTR_SET)
#endif /*INCLUDE_AMP */
        {

        /* Handle the mailbox interrupt locally */

        smUtilIntRoutine();
        }
#if defined(INCLUDE_AMP) && defined(INCLUDE_SMFORVME)
    else if (*pMailbox == SIG2_INTR_SET)
        {

        /* Generate interprocessor interrupt to core 1 */

        vxIpiEmit(0,1 << 1);
        }
#endif /*INCLUDE_AMP */

    }

/*******************************************************************************
*
* sysMv7100SmIntInit - Initialize shared memory interrupts
*
* This routine connects the shared memory interrupt(s).  We always connect
* the shared memory interrupt routine to the IPI interrupts to get
* interrupts from the other core on this board.  We also connect the
* special interrupt handler above to the mailbox interrupt for off board
* CPU's to interrupt us.
*
*
* RETURNS: N/A
*
*/
void sysMv7100SmIntInit()
    {
#ifdef INCLUDE_AMP
    if (!CPU1CHECK)
        {
#   ifdef INCLUDE_SMFORVME
        sysMailboxConnect ((FUNCPTR) sysMv7100SmInt, 0);
        sysMailboxEnable ((char *) NULL);
#   endif /* INCLUDE_SMFORVME */
        }
#else
    {
    smUtilUser1Rtn = sysMv7100SmIntGen;
    sysMailboxConnect ((FUNCPTR) sysMv7100SmInt, 0);
    sysMailboxEnable ((char *) NULL);
    }
#endif /* INCLUDE_AMP */
    }
#endif /* INCLUDE_SM_COMMON */

#ifdef _WRS_VX_AMP

#   ifdef INCLUDE_AMP_CPU_01
_WRS_ABSOLUTE_BEGIN(Loadbias)
_WRS_ABSOLUTE(wrloadLoadBias,CORE1T_OFFSET);
_WRS_ABSOLUTE_END
#   endif /* INCLUDE_AMP_CPU_01 */

/*******************************************************************************
*
* sysDeviceFilter - Called from vxbus to prevent device
*
* This routine is called from vxbus to prevent device from being announced and
* therefore used by vxWorks.
*
* RETURNS: OK or ERROR
*
*/

STATUS sysDeviceFilter
    (
    VXB_DEVICE_ID pDev    /* vxbus device id */
    )
    {
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
    UINT device;
    char cpuStr[80];

    device = CPU_TYPE;

    if ((device) == CPU_TYPE_8641)
    {
    snprintf(cpuStr, sizeof(cpuStr), "MPC8641D");
    snprintf (sysModelStr, MODSTR_SIZE, SYS_MODEL" %s - %s", sysProductStr, cpuStr);
    if (core1ampMode)
        strncat(sysModelStr, " (AMP[#])", MODSTR_SIZE - strlen(sysModelStr) - 1);
    else
        strncat(sysModelStr, " (SMP[#])", MODSTR_SIZE - strlen(sysModelStr) - 1);
    sysModelStr[strlen(sysModelStr) - 3] = '0' + physCpuAvailableGet();
    return (sysModelStr);
    }
    else
        return ("Model Unknown");
    }


#if defined (INCLUDE_ALTIVEC)
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
    int   altivecUnitPresent = ERROR;

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
         }      /* switch  */

    return (altivecUnitPresent);
    }
#endif  /* INCLUDE_ALTIVEC */

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

/*******************************************************************************
*
* sysDramSize - returns the real top of local DRAM.
*
* This routine returns a UINT32 value for DramSize
*
* RETURNS: The address of the top of DRAM.
*
*/

UINT32 sysDramSize (void)
    {
    UINT32 localDram = 0x00000000;
    UINT32 temp = 0x0;

    temp = CCSR_READ32(CCSBAR,CCSR_CS0_CONFIG(0));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS0_BNDS(0));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS1_CONFIG(0));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS1_BNDS(0));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS2_CONFIG(0));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS2_BNDS(0));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS3_CONFIG(0));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS3_BNDS(0));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }
    temp = CCSR_READ32(CCSBAR,CCSR_CS0_CONFIG(1));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS0_BNDS(1));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS1_CONFIG(1));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS1_BNDS(1));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS2_CONFIG(1));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS2_BNDS(1));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS3_CONFIG(1));
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS3_BNDS(1));
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT)|0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }
    return (localDram);
    }

UINT32 e600Ratio,platRatio;
UINT32 oscFreq;

/*******************************************************************************
*
* sysClkFreqGet - returns the Freq of system
*
* This routine returns a UINT32 value for system freq
*
* RETURNS: The value of the system freq
*
*/

UINT32 sysClkFreqGet(void)
    {
    static UINT32  sysClkFreq;
    static BOOL    firstTime = TRUE;
    UINT32 ccsrPorpllsr = 0;
    int ix;

    oscFreq = 66666666;
    if (firstTime)
        {
        ccsrPorpllsr = CCSR_READ32 (CCSBAR, CCSR_PORPLLSR);
        platRatio = CCSR_PORPLLSR_PLAT_RATIO_VAL (ccsrPorpllsr);

        sysClkFreq = oscFreq*platRatio;

        e600Ratio = CCSR_PORPLLSR_E600_RATIO_VAL(ccsrPorpllsr);

        ix = 0;

        while ( (e600RatioLookup[ix][0] != e600Ratio) &&
              (e600RatioLookup[ix][0] != 0) )
        ix++;

        coreFreq = ((UINT32)(sysClkFreq * e600RatioLookup[ix][1]))>>
                   ((UINT32)e600RatioLookup[ix][2]);

        firstTime = FALSE;
        }

    return(sysClkFreq);
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
#endif /* INCLUDE_MMU */

#ifdef _WRS_VX_AMP
#ifndef INCLUDE_AMP_CPU_01
    sysAmpCpuStatusSet(ampCpuStateReboot,1);
    sysAmpCpuStatusSet(ampCpuStateRunning,0);
#else
    sysAmpCpuStatusSet(ampCpuStateRunning,1);
#endif /* INCLUDE_AMP_CPU_01 */
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

    /*
     * Here we initialize cpu1 init address so we don't wait until the
     * correct value is initialized
     */

    *(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_RBV;
#endif /* WRS_CONFIG_SMP */

    vxHid1Set(vxHid1Get() | HID1_SYNCBE | HID1_ABE | HID1_EMCP);
    _func_vxSvrGet = vxSvrGet;

#ifndef DRV_TIMER_DEC_PPC
#   ifdef  _WRS_CONFIG_SMP

    /* Assert the common time base disable signal. */

    CCSR_WRITE32 (CCSBAR, CCSR_DEVDISR,
                  CCSR_READ32(CCSBAR, CCSR_DEVDISR) | M8641_DEVDISR_TB);
#   else   /* _WRS_CONFIG_SMP */

    /* Disable time base per CPU. */

    vxHid0Set (vxHid0Get () & ~_PPC_HID0_TBEN);
#   endif  /* _WRS_CONFIG_SMP */
#endif  /* !DRV_TIMER_DEC_PPC */

#if defined(INCLUDE_MMU)

    /*
     *  Setup the function pointers for the BAT initialization functions.
     *  (Standard 4 I/D BAT's and extra 4 I/D BAT's
     */

    /* Setup MMU if needed */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC74x5;

    /*
     * run-time update of the sysPhysMemDesc MMU entry for main RAM
     * This means SDRAM _must_ be represented by the correct entry
     * in sysPhysMemDesc depending on mode.
     */

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () - RAM_LOW_ADRS);

    /* calculate the number of sysPhysMemDesc entries in use */

    pMmu = &sysPhysMemDesc[0];

    for (ix = 0; ix < NELEMENTS (sysPhysMemDesc); ix++)
        {
        if (pMmu->virtualAddr != (VIRT_ADDR)DUMMY_VIRT_ADDR)
            pMmu++;     /* used entry */
        else
            break;  /* dummy entry, assume no more */
        }

    /* This is the REAL number of sysPhysMemDesc entries in use. */

    sysPhysMemDescNumEnt = ix;
#endif /* INCLUDE_MMU */

    /*
     * Adjust flash entry in sysPhysMemDesc and adjust the
     * flash bank programming in the local bus controller.
     */

    sysFlashMappingConfig();

    /*
     * Detect core 1 low-memory offset (AMP) mode.
     *
     * This is used for reporting in sysModel(), and to avoid attempting
     * to start up CPU1 as an SMP processor if configured for AMP.
     */

    core1ampMode = CCSR_PORDEVSR_CORE1TE_MASK & CCSR_READ32(CCSBAR, CCSR_PORDEVSR);
#ifdef _WRS_VX_AMP

    if(!core1ampMode)
    {
    strcpy(sysExcMsg,"Board in SMP mode. Switch to low memory offset mode(AMP) and power down to restart");
    sysToMonitor(BOOT_WARM_NO_AUTOBOOT);
    }
#endif /* _WRS_VX_AMP */

    /* set pointer to bus probing hook */

#ifdef INCLUDE_AMP
    if(ampCpuTable[1].cpu == sysProcNumGet())
        _func_vxMemProbeHook = (FUNCPTR)sysBusProbe;
#else
    _func_vxMemProbeHook = (FUNCPTR)sysBusProbe;
#endif /* INCLUDE_AMP */

    pSysPlbMethods = mv7100PlbMethods;
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
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.  This routine also adjusts the
* DRAM entry in sysPhysMemDesc to be the actual address of the top of
* physical memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    LOCAL char * physTop = NULL;
    UINT32 localDram = 0;

    if (physTop == NULL)
        {
        localDram = sysDramSize();
#ifdef LOCAL_MEM_AUTOSIZE
        physTop = (char *)localDram;
#else /* not LOCAL_MEM_AUTOSIZE, use defined constants. */
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
#endif /* LOCAL_MEM_AUTOSIZE */

        }
    return (physTop);
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

#ifdef  INCLUDE_EDR_PM

    /* account for ED&R persistent memory */

        memTop -= PM_RESERVED_MEM;
#endif  /* INCLUDE_EDR_PM */
    }

    return memTop;
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
#   ifdef DRV_TIMER_DEC_PPC

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

	    if ( DELTA(oldval,newval) < 1000 )
	        decElapsed += DELTA(oldval,newval);  /* no rollover */
	    else
	        if (newval > oldval)
		        decElapsed += abs((int)oldval);  /* rollover */

	    oldval = newval;
	    }
#   else  /* DRV_TIMER_DEC_PPC */
#       error This BSP does not support INCLUDE_TIMER_STUB && !DRV_TIMER_DEC_PPC configuration!
#   endif /* DRV_TIMER_DEC_PPC */
#else	/* INCLUDE_TIMER_STUB */
    vxbMsDelay (delay);
#endif	/* INCLUDE_TIMER_STUB */
    }

#ifdef _WRS_CONFIG_SMP

/* flag shared by sysCpu1ShutdownLoop() and sysToMonitor() */

LOCAL BOOL CPU1done = FALSE;

/*******************************************************************************
*
* sysCpu1ShutdownLoop - parking loop for CPU1
*
* This routine runs on core 1 during system reboot.  After clearing the
* caches, it puts the core into nap mode (or, if that fails, a tight
* loop) so that it doesn't interfere with the sysToMonitor() routine.
*
* RETURNS: N/A
*/

LOCAL void sysCpu1ShutdownLoop (void)
    {

    intCpuLock ();	/* CPC done, now safe to disable interrupts */

    /* The cacheDisable() API is not supported in SMP */

    cacheArchDisableFromMmu (INSTRUCTION_CACHE);
    cacheArchDisableFromMmu (DATA_CACHE);

    CPU1done = TRUE;		/* report CPU 1 cacheDisable() complete */

    /*
     * Per the MPC8641D manual, the CPU should be in sleep or nap state prior
     * to issuing an HRESET via the processor reset register in the PIC.
     * Do this even if the system does not otherwise include power management.
     */

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

    FOREVER	/* should not get here */
        {
        }

    }
#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot()--which services ^X--and by bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* The entry point for a warm boot is defined by the macro ROM_WARM_ADRS
* in config.h.  We do an absolute jump to this address to enter the
* ROM code.
*
* RETURNS: Does not return.
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR     pRom = (FUNCPTR) (ROM_WARM_ADRS);

#ifdef _WRS_CONFIG_SMP
    cpuset_t cpuset;
    int i;
    LOCAL BOOL CPU1needsShutdownCPC = TRUE;
#endif /* _WRS_CONFIG_SMP */

#ifdef _WRS_VX_AMP
    volatile int sysMonDelay;
#endif	/* _WRS_VX_AMP */

    intCpuLock ();			    /* disable interrupts */

#ifdef  _WRS_CONFIG_SMP

    /*
     * Write out our own modified lines first, so that
     * much gets done even if we get into trouble later.
     */

    cacheFlush (DATA_CACHE, NULL, ENTIRE_CACHE);

    CPUSET_ZERO (cpuset);		/* Send CPC to all but self */

    /*
     * Make sure we cannot migrate, but without locking
     * interrupts -- see cpcInvoke() requirements.
     */

    if (!INT_CONTEXT() && !KERNEL_ENTERED_ME())
	    taskCpuLock();

    /*
     * In SMP mode, the last part of sysToMonitor() must run on core 0
     * because it will jump to the bootrom.
     *
     * If we happen to already be on core 0, we can cause CPU 1 to clear
     * its caches and enter nap mode (or a parking loop) after which CPU 0
     * can proceed with shutdown.  If we are on core 1, we will proceed
     * to the CPU 1 shutdown code after arranging for sysToMonitor() to
     * be reinvoked on core 0.
     */

    if (vxCpuIndexGet() == 0)
	    {
	    if (CPU1needsShutdownCPC)
	        cpcInvoke (cpuset, (CPC_FUNC)sysCpu1ShutdownLoop, NULL, 0, 
	                   VX_CPC_ASYNC);
	    }
    else
	    {
	    CPU1needsShutdownCPC = FALSE;
	    cpcInvoke (cpuset, (CPC_FUNC)sysToMonitor, (void *)startType,
		           sizeof(startType), VX_CPC_ASYNC);
	    sysCpu1ShutdownLoop();
	    }

    /* Only CPU 0 gets here */

    intCpuLock ();	/* CPC done, now safe to disable interrupts */

    /* The cacheDisable() API is not supported in SMP */

    cacheArchDisableFromMmu (INSTRUCTION_CACHE);
    cacheArchDisableFromMmu (DATA_CACHE);

    i = 100;			/* 10 sec (100 * 0.1) max wait for CPU1 */

    while ((!CPU1done && --i) > 0)
        sysMsDelay(100);

    /*
     * Wait another 0.1 sec to ensure CPU1 has had time to enter nap
     * mode after reporting that it had finished flushing its caches.
     */

    sysMsDelay(100);

#else   /* _WRS_CONFIG_SMP */

    cacheDisable (INSTRUCTION_CACHE);   /* Disable the Instruction Cache */
    cacheDisable (DATA_CACHE);          /* Disable the Data Cache */

#endif  /* _WRS_CONFIG_SMP */

    vxHid0Set (vxHid0Get () & ~_PPC_HID0_SIED); /* Enable Serial Instr Exec */

    /* Turn off timer */

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif /* INCLUDE_AUX_CLK */

#ifdef  _WRS_CONFIG_SMP

    /*
     * We need to forcibly reset the second core. This is done
     * using the processor reset register in the PIC. Note that
     * this register is only available on rev 2.0 and later of the
     * 8641D silicon. Luckily, the Wind River SBC8641D board has
     * a rev 2 chip.
     *
     * Note also that the bits in the EPIC Hard Reset Register are
     * not auto-clearing.
     */

    *(M8XXX_EPICPRR(CCSBAR)) |= M8XXX_EPICPRR_HRST_CORE1; /* Set HRESET */
    WRS_ASM (" sync ; isync ");

    sysCpu1Stop ();

    *(M8XXX_EPICPRR(CCSBAR)) &= ~M8XXX_EPICPRR_HRST_CORE1; /* Clear HRESET */
    WRS_ASM (" sync ; isync ");

#endif  /* _WRS_CONFIG_SMP */

    vxMsrSet (0);       /* Clear the MSR */

#ifdef _WRS_VX_AMP

    /* Clear boot page so back to flash - precautionary step  */

    *M86XX_BPTR(CCSBAR) = 0x0;

    /*
     * Here we have to send to bootrom and let wrload reload image when ready
     * bootimage should spin waiting for CPU1_INIT_START_ADR not to be the
     * magic pattern 1
     */

#   ifndef INCLUDE_AMP_CPU_01

    *((volatile int *)(CPU1_FUNC_START_ADR | CORE1T_OFFSET)) = (int)0;

    /* SMP specific not used just loaded */

    *((volatile int *)(CPU1_STACK_START_ADR | CORE1T_OFFSET)) =
      (int)(RAM_LOW_ADRS - FRAMEBASESZ);
    *((volatile int *)(CPU1_INIT_START_ADR | CORE1T_OFFSET)) =
      (int)MP_MAGIC_RBV;

    WRS_ASM("sync");

    sysCpu1Stop();

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

    *(M8XXX_EPICPRR(CCSBAR)) |= M8XXX_EPICPRR_HRST_CORE1; /* Set HRESET */

    WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead - delay allow hreset to be held for a short time
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

    *(M8XXX_EPICPRR(CCSBAR)) &= ~M8XXX_EPICPRR_HRST_CORE1; /* Clear HRESET */

    WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

    WRS_ASM("isync;sync");

    sysCpu1Start();

#   else /* INCLUDE_AMP_CPU_01 */

    *((volatile int *)CPU1_FUNC_START_ADR) = (int)0;

    /* SMP specific not used just loaded */

    *((volatile int *)CPU1_STACK_START_ADR) = (int)RAM_LOW_ADRS - FRAMEBASESZ;
    *((volatile int *)CPU1_INIT_START_ADR) = (int)MP_MAGIC_RBV;

    WRS_ASM("isync;sync");

#   endif /* INCLUDE_AMP_CPU_01 */
#endif /* _WRS_VX_AMP */

    (*pRom) (startType);    /* jump off to romInit.s */

    return (OK);            /* in case we continue from ROM monitor */
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
    UINT16 memLimit;
    UINT16 memBase;
#ifdef INCLUDE_PCI_BUS
    UINT32 tempeCgr;
    UINT16 cfgWord;
    int x;
#endif /* INCLUDE_PCI_BUS */

    /*   UINT32 vmebus_stat;*/

    static int  initialized;        /* Do this ONCE */

    if (!initialized)
        {
        initialized = TRUE;

        sysMotI2CInit();

        vxbDevInit();

        sysSerialConnectAll();

        /* Connect our interrupt handlers in front of the vxBus ones */

        spinLockIsrInit(&sysIntConnectSpinlock, 0);

        _vxb_intConnectRtn = _func_intConnectRtn;
        _vxb_intEnableRtn = _func_intEnableRtn;
        _vxb_intDisableRtn = _func_intDisableRtn;
        _func_intConnectRtn = sysMv7100IntConnect;
        _func_intEnableRtn = sysMv7100IntEnable;
        _func_intDisableRtn = sysMv7100IntDisable;

#ifdef INCLUDE_USB
#ifdef INCLUDE_AMP
    if (!CPU1CHECK)
        {
#endif /* INCLUDE_AMP */
    necPd720101ExtConfig ();
#ifdef INCLUDE_AMP
         }
#endif /* INCLUDE_AMP */
#endif /* INCLUDE_USB */

#ifdef INCLUDE_AMP
    if (ampCpuTable[1].cpu == CPU1CHECK)
        {
#endif /* INCLUDE_AMP */

            /*
             * Now, we need to work back through the bridge chain,
             * changing the bus windows so that they reflect the VME
             * memory space.  Note if the board layout changes
             * the bus/dev numbers will need to change
             */

            memBase = ((VME_MEM_LOCAL_START >> 16) & 0xFFF0);
            memLimit = ((PCI0_VME_MEM_LOCAL_END >> 16) & 0xFFF0);

            /* Start with the PCIE-PCIX bridge */

#ifdef INCLUDE_PCI_BUS
            pciConfigOutWord(TEMPE_BRIDGE_PCI_BUS, TEMPE_BRIDGE_PCI_DEV,
                            TEMPE_BRIDGE_PCI_FCN, PCI_CFG_MEM_BASE,memBase);
            pciConfigOutWord(TEMPE_BRIDGE_PCI_BUS, TEMPE_BRIDGE_PCI_DEV,
                            TEMPE_BRIDGE_PCI_FCN, PCI_CFG_MEM_LIMIT,memLimit);
            pciConfigOutLong(TEMPE_BRIDGE_PCI_BUS, TEMPE_BRIDGE_PCI_DEV,
                            TEMPE_BRIDGE_PCI_FCN, PCI_CFG_BASE_ADDRESS_0,
                            PCI0_TEMPE_BRIDGE_MEM);

            /* Next do the PCIE Switch Tempe Port */

            pciConfigOutWord(TEMPE_SWITCH_PCI_BUS, TEMPE_SWITCH_PCI_DEV,
                            TEMPE_SWITCH_PCI_FCN, PCI_CFG_MEM_BASE,memBase);
            pciConfigOutWord(TEMPE_SWITCH_PCI_BUS, TEMPE_SWITCH_PCI_DEV,
                            TEMPE_SWITCH_PCI_FCN, PCI_CFG_MEM_LIMIT,memLimit);

            /* Finally fix the limit on the PCIE Switch Port*/

            pciConfigOutWord(PCIE0_SWITCH_PCI_BUS, PCIE0_SWITCH_PCI_DEV,
                            PCIE0_SWITCH_PCI_FCN, PCI_CFG_MEM_LIMIT,memLimit);

            /* Change the Tempe address */

            pciConfigOutLong(TEMPE_PCI_BUS,TEMPE_PCI_DEV, TEMPE_PCI_FCN,
                            PCI_CFG_BASE_ADDRESS_0, PCI0_TEMPE_MEM);

            /*
             * And then open up the Outbound PCI window to cover the
             * Tempe space.  This is because the PCI Bridge VxBus driver
             * only sizes the bridge to the actually BARs it auto configs,
             * and it doesn't know about Tempe memory space either
             */

            CCSR_WRITE32(CCSBAR, CCSR_PCIE0_POWAR1,
                        PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |
                        PCI_OUT_ATTR_WTT_MEM |
                        MPC8641_PCI_SIZE_ENCODE(PCI0_VME_MEM_LOCAL_SIZE));

            /*
             * Disable TSi148 Outbound Mapping Windows
             *
             * When using the MotLoad firmware to boot vxWorks we must
             * disable the TSi148's outbound mapping windows before
             * reconfiguring the TSi148's register access BAR and attempting
             * to access the windows from this new address.  This is
             * necessary because MotLoad configures the TSi148's outbound
             * windows for a region of memory that overlaps the region of
             * memory where vxWorks wants to place the TSi148's register
             * set, which would result in overlapping windows and an
             * inability to access the registers at all.
             */

            pciConfigInLong(TEMPE_PCI_BUS,TEMPE_PCI_DEV,TEMPE_PCI_FCN,
                           PCI_CFG_BASE_ADDRESS_0, &tempeCgr);
            tempeCgr &= PCI_MEMBASE_MASK;
            if (tempeCgr != 0x0)
                {
                for (x = 0; x < 8; x++)
                    {

                    /*
                     * Clear all 8 outbound translation attribute
                     * registers.  This will disable each of the
                     * outbound translation windows.
                     */

                    TEMPE_WRITE32_PUSH(tempeCgr,(TEMPE_OTAT0 + (x*0x20)),
                                       0x00000000);
                    }
                }

            /*
             * Copy the mem window limits back to the bus 0 bridge from the
             * bus 1 bridge, for Tempe use
             */

            pciConfigOutWord(0,0,0,PCI_CFG_MEM_BASE,
                             (PCI0_MEM_LOCAL_START >> 16));
            pciConfigInWord(1,0,0,PCI_CFG_MEM_LIMIT,&cfgWord);
            pciConfigOutWord(0,0,0,PCI_CFG_MEM_LIMIT,cfgWord);
            pciConfigInWord(1,0,0,PCI_CFG_PRE_MEM_BASE,&cfgWord);
            pciConfigOutWord(0,0,0,PCI_CFG_PRE_MEM_BASE,cfgWord);
            pciConfigInWord(1,0,0,PCI_CFG_PRE_MEM_LIMIT,&cfgWord);
            pciConfigOutWord(0,0,0,PCI_CFG_PRE_MEM_LIMIT,cfgWord);

            /* Store the PCI translation windows for later Tempe calculations */

            sysPhbCapt();

            /* Reset and  Initialize the Tempe PCI-VME bridge chip */

            sysTempeReset ();
            sysTempeInit ();
#endif /* INCLUDE_PCI_BUS */
#ifdef INCLUDE_AMP

            /* Set the spin locks used by dual cores to zero */

            *(UINT32 *)SM_SLOCK_BUS = 0;
            *(UINT32 *)SM_SLOCK_I2C = 0;
            }
    else
        {

        /*
         * Store the PCI and VMEtranslation windows for later Tempe
           calculations
         */

        sysPhbCapt();
        if(CPU1CHECK)
            sysTempeCapt();
        }
#endif /* INCLUDE_AMP */

#if defined(INCLUDE_SM_COMMON) && defined(INCLUDE_SMFORVME)

    /* Set function hook for special sysBusTasClear routine */

    smUtilTasClearRtn = sysBusTasClear;
#endif /* INCLUDE_SM_COMMON */

#ifdef INCLUDE_ALTIVEC
    _func_altivecProbeRtn = sysAltivecProbe;
#endif  /* INCLUDE_ALTIVEC */

#if defined(INCLUDE_SM_COMMON) && defined(INCLUDE_SMFORVME)
    sysMv7100SmIntInit();
#endif /* INCLUDE_SM_COMMON */

        taskSpawn("devConnect", 0, 0, 10000, vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
        }
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
#ifdef INCLUDE_AMP
    return(CPU1CHECK);
#else
    return (sysProcNum);
#endif /* INCLUDE_AMP */
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
    int procNum         /* processor number */
    )
    {
    LOCAL int procNumSet = FALSE;

    sysProcNum = procNum;

#ifdef INCLUDE_AMP
    if (ampCpuTable[1].cpu == CPU1CHECK)
#else
    if (!CPU1CHECK)
#endif /*INCLUDE_AMP */
        {

        /*
         * Note that sysProcNumSet() is always called during bootrom
         * initialization from bootConfig.c, function bootLoad().  It is
         * only called from the kernel initialization if INCLUDE_NET_INIT
         * is #define'd.  That particular call is done indirectly through
         * the line "usrNetInit (BOOT_LINE_ADRS);" inside of the
         * usrConfig.c file, function usrRoot().  Note that this line of
         * code in usrConfig.c is NOT compiled if INCLUDE_NET_INIT is not
         * #define'd.  Since we must know the node (processor) number in
         * order to correctly configure the Tempe ITx slave decoders
         * (see commentary below), we must call the 2nd phase of Tempe
         * initialization code here.  If we don't have INCLUDE_NET_INIT
         * #define'd, this 2nd phase of Tempe initialization is only
         * done once (during bootrom initialization).  If INCLUDE_NET_INIT
         * is #defined, the 2nd phase of initialization will be done twice,
         * once during bootrom initialization and once during kernel
         * initialization.
         */

        if (procNumSet == FALSE)
            {

            /*
             * Set up the Tempe's CRG slave decoders.  We have to wait until
             * we know our processor number to do this since the decoder
             * address location is dependent on the processor number.
             */

            if (TEMPE_REG_BASE != 0)
                {
                sysTempeInit2 ();
                }
            }


        procNumSet = TRUE;
        }

    }

/*******************************************************************************
*
* sysPhbCapt - capture translation window information
*
* This routine captures the configuration of the host bridge PPC and PCI slave
* registers. This information is used to perform address translations from
* CPU to PCI addresses and vice versa.
*
* RETURNS: N/A
*
* SEE ALSO: sysBusToLocalAdrs(), sysLocalToBusAdrs(), sysCpuToPciAdrs(),
* sysPciToCpuAdrs(), and sysPhbTransAdrs().
*/

void sysPhbCapt (void)
    {
    PHB_WIN_STRUCT * pPhbCpu; /* pointer to cpu windows */
    PHB_WIN_STRUCT * pPhbPci; /* pointer to pci windows */
    UINT32 size;
    UINT32 powar;
    UINT32 powbar;
    UINT32 potar;
    UINT32 wtt;
    int x, ctrl;

    /* initialize number of valid windows found */

    sysValidPhbWindows[0] = 0;
    sysValidPhbWindows[1] = 0;
    phbToPciWinCount[0] = 0;
    pciToPhbWinCount[0] = 0;
    phbToPciWinCount[1] = 0;
    pciToPhbWinCount[1] = 0;

    for (ctrl=0;ctrl<2;ctrl++)
        {

        /* point to window save arrays */

        pPhbCpu = &sysPhbCpuToPciWin[ctrl][0];
        pPhbPci = &sysPhbPciToCpuWin[ctrl][0];

        /*
         * Outbound Windows
         *
         * Window 0 is ignored since it is the default mapping that
         * covers the entire memory map.  We need to know if there is
         * another window for which the given address falls into.
         */

        for (x = 0; x < 4; x++)
            {
            powar = CCSR_READ32(CCSBAR,(CCSR_PCIE0_POWAR1 +
                                        (x * 0x20) + (ctrl*0x1000)));
            powbar = CCSR_READ32(CCSBAR, (CCSR_PCIE0_POWBAR1 +
                                          (x * 0x20) + (ctrl*0x1000)));
            potar = CCSR_READ32(CCSBAR, (CCSR_PCIE0_POTAR1 +
                                         (x * 0x20) + (ctrl*0x1000)));

            if (CCSR_POWAR_EN_VAL(powar) != 0)
                {
                sysValidPhbWindows[ctrl]++;
                size = MPC8641_PCI_SIZE_DECODE(CCSR_POWAR_OWS_VAL(powar));
                if (size != 0)
                    {
                    wtt = CCSR_POWAR_WTT_VAL(powar);
                    pPhbCpu->winType =
                        ((wtt == 0x8)?PCI_BAR_SPACE_IO:PCI_BAR_SPACE_MEM);
                    pPhbCpu->winBase = CCSR_PAR_ADDR_DECODE(powbar);
                    pPhbCpu->winLimit = CCSR_PAR_ADDR_DECODE(powbar)+size-1;

                    pPhbPci->winType = pPhbCpu->winType;
                    pPhbPci->winBase = CCSR_PAR_ADDR_DECODE(potar);
                    pPhbPci->winLimit = CCSR_PAR_ADDR_DECODE(potar)+size-1;
                    pPhbCpu++;
                    pPhbPci++;
                    phbToPciWinCount[ctrl]++;
                    }
                }
            }

        /* Inbound Windows */

        for (x = 0; x < 3; x++)
            {
            powar = CCSR_READ32(CCSBAR, (CCSR_PCIE0_PIWAR3 +
                                         (x * 0x20) + (ctrl*0x1000)));
            powbar = CCSR_READ32(CCSBAR, (CCSR_PCIE0_PIWBAR3 +
                                          (x * 0x20) + (ctrl*0x1000)));
            potar = CCSR_READ32(CCSBAR, (CCSR_PCIE0_PITAR3 +
                                         (x * 0x20) + (ctrl*0x1000)));

            if (CCSR_POWAR_EN_VAL(powar) != 0)
                {
                sysValidPhbWindows[ctrl]++;
                size = MPC8641_PCI_SIZE_DECODE(CCSR_POWAR_OWS_VAL(powar));
                if (size != 0)
                    {
                    pPhbPci->winType = 0;
                    pPhbPci->winBase = CCSR_PAR_ADDR_DECODE(powbar);
                    pPhbPci->winLimit = CCSR_PAR_ADDR_DECODE(powbar)+size-1;

                    pPhbCpu->winType = pPhbPci->winType;
                    pPhbCpu->winBase = CCSR_PAR_ADDR_DECODE(potar);
                    pPhbCpu->winLimit = CCSR_PAR_ADDR_DECODE(potar)+size-1;

                    pPhbCpu++;
                    pPhbPci++;
                    pciToPhbWinCount[ctrl]++;
                    }
                }
            }
        }

    }

/*******************************************************************************
*
* translationWindowShow - Display PHB window translation values
*
* This function displays the translation values which are used in the
* sysBusToLocalAdrs() and sysLocalToBusAdrs() routines.
*
* RETURNS: N/A
*/

void translationWindowShow ()
    {
    int i,ctrl;

    for (ctrl=0;ctrl<2;ctrl++)
        {
        printf("PCIE Controller %d\n",ctrl+1);
        printf("           sysPhbCpuToPciWin       sysPhbPciToCpuWin\n\n");

        for (i=0; i< sysValidPhbWindows[ctrl]; i++)
            {
            printf("Window %d\n",i);
            printf("  winType    0x%08x              0x%08x\n",
                   sysPhbCpuToPciWin[ctrl][i].winType,
                   sysPhbPciToCpuWin[ctrl][i].winType);
            printf("  winBase    0x%08x              0x%08x\n",
                   sysPhbCpuToPciWin[ctrl][i].winBase,
                   sysPhbPciToCpuWin[ctrl][i].winBase);
            printf("  winLimit   0x%08x              0x%08x\n",
                   sysPhbCpuToPciWin[ctrl][i].winLimit,
                   sysPhbPciToCpuWin[ctrl][i].winLimit);
            printf("\n");
            }
        }

    }

/*******************************************************************************
*
* sysPhbTransAdrs - translate an address that passes through the Phb.
*
* This routine converts an address from a cpu to pci address or vice versa. It
* uses a pair of window arrays built during sysHwInit2 to guide the
* translation. The adrs parameter is the address to convert.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
* SEE ALSO: sysPhbCapt(), sysPciToCpuAdrs(), sysBusToLocalAdrs(),
* sysLocalToBusAdrs(),  and sysPhbTransAdrs().
*
*/

LOCAL STATUS sysPhbTransAdrs
    (
    UINT32            adrsSpace,   /* address space (memory or i/o ) */
    UINT32            adrs,        /* known address */
    UINT32 *          pTransAdrs,  /* pointer to the translated address */
    UINT32            winCnt,      /* number of open windows */
    PHB_WIN_STRUCT *  pSrc,         /* pointer to the source windows */
    PHB_WIN_STRUCT *  pDest         /* pointer to the destination windows */
    )
    {

    while (winCnt--)
        {

        /* check for a match on window type and in bounds */

        if ( (pSrc->winType == adrsSpace) &&
             (adrs >= pSrc->winBase) &&
             (adrs <= pSrc->winLimit) )
            {
            *pTransAdrs = ( adrs - pSrc->winBase + pDest->winBase );

            return (OK);
            };

        /* advance to next window */

        pSrc++;
        pDest++;
        }

    /*
     * no window was found to contain adrs. indicate that translation was
     * not possible.
     */

    return (ERROR);

    }

/*******************************************************************************
*
* sysCpuToPciAdrs - translate a cpu address to a pci bus address
*
* This routine converts an address as seen from the cpu to the equivalent pci
* address, if it exists. The input address is the address as seen by the cpu.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
*
* SEE ALSO: sysPciToCpuAdrs(), sysBusToLocalAdrs(), sysLocalToBusAdrs()
* sysPhbCapt(), and sysPhbTransAdrs().
*/

LOCAL STATUS sysCpuToPciAdrs
    (
    int     adrsSpace,    /* bus address space where busAdrs resides */
    char *  localAdrs,    /* local address to convert */
    char ** pBusAdrs      /* where to return bus address */
    )
    {
    /* Try to translate on PCIE controller 0 */

    if (sysPhbTransAdrs (adrsSpace, (UINT32)localAdrs,
                         (UINT32 *)pBusAdrs, sysValidPhbWindows[0],
                         &sysPhbCpuToPciWin[0][0],
                         &sysPhbPciToCpuWin[0][0]) != ERROR)
        return(OK);

    /* Then try PCIE Controller 1 */

    return (sysPhbTransAdrs (adrsSpace, (UINT32)localAdrs,
                             (UINT32 *)pBusAdrs, sysValidPhbWindows[1],
                             &sysPhbCpuToPciWin[1][0],
                             &sysPhbPciToCpuWin[1][0]) );
    }

/*******************************************************************************
*
* sysPciToCpuAdrs - translate a pci bus address to a cpu address
*
* This routine converts an address as seen from the pci bus to the equivalent
* cpu address, if it exists. The input address is the address as seen by the
* pci bus.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
* SEE ALSO: sysCpuToPciAdrs(), sysBusToLocalAdrs(), sysLocalToBusAdrs()
* sysPhbCapt(), and sysPhbTransAdrs().
*/

LOCAL STATUS sysPciToCpuAdrs
    (
    int     adrsSpace,    /* bus address space where busAdrs resides */
    char *  localAdrs,    /* local address to convert */
    char ** pBusAdrs      /* where to return bus address */
    )
    {

    /* Try to translate on PCIE controller 0 */

    if (sysPhbTransAdrs (adrsSpace, (UINT32)localAdrs,
                         (UINT32 *)pBusAdrs, sysValidPhbWindows[0],
                         &sysPhbPciToCpuWin[0][0],
                         &sysPhbCpuToPciWin[0][0]) != ERROR)
        return(OK);

    return (sysPhbTransAdrs (adrsSpace, (UINT32)localAdrs,
                             (UINT32 *)pBusAdrs, sysValidPhbWindows[1],
                             &sysPhbPciToCpuWin[1][0],
                             &sysPhbCpuToPciWin[1][0]));
    }

/*******************************************************************************
*
* sysLocalToBusAdrs - convert a local address to a bus address
*
* Given a CPU address, this routine returns a corresponding local PCI
* bus address provided that such an address exists.  The target PCI
* bus is selected by the adrsSpace parameter.  Legal values for this
* parameter are found in "pci.h".
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
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
    char * pciBusAdrs;

    if (IS_VME_ADDR_MOD(adrsSpace))  /* Is it a VME address? ... */
        {
        if (sysLocalToBusAdrs (PCI_SPACE_MEM_PRI, localAdrs,
                               &pciBusAdrs) == ERROR)
            return (ERROR);
        else
            return (sysPciToVmeAdrs (adrsSpace, pciBusAdrs, pBusAdrs));
        }

    switch (adrsSpace)
        {

        case PCI_SPACE_IO_PRI:

            /* convert from cpu address space to local pci space */

            if (sysCpuToPciAdrs (PCI_BAR_SPACE_IO, localAdrs, pBusAdrs) != OK )
                return (ERROR);

            break;

        case PCI_SPACE_MEMIO_PRI:
        case PCI_SPACE_MEM_PRI:

            /* convert from cpu address space to local pci address space */

            if (sysCpuToPciAdrs (PCI_BAR_SPACE_MEM, localAdrs, pBusAdrs) != OK )
                return (ERROR);

            break;

        default:
            return (ERROR);
        }
    return (OK);
    }

/*******************************************************************************
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
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
    (
    int     adrsSpace,  /* bus address space where busAdrs resides */
    char *  busAdrs,    /* bus address to convert */
    char ** pLocalAdrs  /* where to return local address */
    )
    {
    char * pciBusAdrs;

    if (IS_VME_ADDR_MOD(adrsSpace)) /* Is it a VME address? ... */
        {
        if (sysVmeToPciAdrs (adrsSpace, busAdrs, &pciBusAdrs) == OK)
            return (sysBusToLocalAdrs(PCI_SPACE_MEMIO_PRI, pciBusAdrs,
                    pLocalAdrs));
        else
            return (ERROR);
        }

    switch (adrsSpace)
        {

        case PCI_SPACE_IO_PRI:

            /*
             * translate from local PCI address space to CPU address
             * space.
             */

            return ( sysPciToCpuAdrs (PCI_BAR_SPACE_IO, busAdrs, pLocalAdrs) );

        case PCI_SPACE_MEMIO_PRI:
        case PCI_SPACE_MEM_PRI:

            /*
             * translate from local PCI address space to CPU address
             * space.
             */

            return (sysPciToCpuAdrs (PCI_BAR_SPACE_MEM, busAdrs, pLocalAdrs) );

        default:
            return (ERROR);
        }
    }

#ifdef INCLUDE_AMP

int     sysLockTakeDelayMin = 256;
int     sysLockTakeDelayMax = 1*1024*1024;

/*******************************************************************************
*
* sysCoreLockTake - take a spin lock for mutual exclusion between cores.
*
* This function is used to lock access to shared resources between the cores.
* This function should be used where quick access is needed, as between lock
* attempts, it uses a tight delay loop, so it should be used when the
* resource is locked for only a short time.  For longer locks,
* sysCoreLockTaskTake().
*
* RETURNS: TRUE if lock was taken, FALSE if failed to take lock
*/

BOOL sysCoreLockTake
    (
    char * adrs         /* address to be tested and set */
    )
    {
    int delay;          /* time between tries to get lock */
    int ix;             /* index */
    int dummy = 0;      /* dummy counter for delay */
    volatile int dummy2;  /* dummy to avoid warning in delay */
    int numTries = 10;  /* number of tries */

    /* During boot rom, we don't need the spin locks, as there is only one core */

    if (BOOT_ROM_IN_PROCESS)
        {
        return(TRUE);
        }

    delay = sysLockTakeDelayMin;

    do
        {
        if (vxTas(adrs))
            {
            return(TRUE);
            }

        for (ix = 0; ix <= delay; ix++) /* delay loop */
            dummy2 = dummy++;           /* volatile!! */

        /* Exponential delay, with a limit */

        delay <<= 1;
        if (delay > sysLockTakeDelayMax)
            {
            delay = sysLockTakeDelayMin;
            }

        } while (--numTries > 0);       /* do for spec'd number of tries */

    return (FALSE);                     /* cannot take lock */
    }

/*******************************************************************************
*
* sysCoreLockTaskTake - take a spin lock for mutual exclusion between cores.
*
* This function is used to lock access to shared resources between the cores.
* This function should be used where locks are held for a longer time, as
* between lock it uses a taskDelay(1).  For shorter locks, use
* sysCoreLockTake().
*
* RETURNS: TRUE if lock was taken, FALSE if failed to take lock
*/

BOOL sysCoreLockTaskTake
    (
    char * adrs      /* address to be tested and set */
    )
    {
    int numTries = 10;  /* number of tries */

    /* During boot rom, we don't need the spin locks, as there is only one core */

    if (BOOT_ROM_IN_PROCESS)
        {
        return(TRUE);
        }

    do
        {
        if (vxTas(adrs))
            {
            return(TRUE);
            }

        taskDelay(1);

        } while (--numTries > 0);       /* do for spec'd number of tries */

    return (FALSE);                     /* cannot take lock */
    }

/*******************************************************************************
*
* sysCoreLockGive - give a spin lock for mutual exclusion between cores.
*
* This function is used to unlock access to shared resources between the cores.
*
* RETURNS: N/A
*/

void sysCoreLockGive
    (
    char * adrs      /* address to be tested and set */
    )
    {
    *(UINT32 *)adrs = 0;        /* Clear the semaphore */
    }
#endif /*INCLUDE_AMP */

#ifdef INCLUDE_WRLOAD

/*******************************************************************************
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

void sysAmpCpuEnable
    (
    FUNCPTR entryPt,
    UINT32 cpu
    )
    {

    /* Here if core already running then maybe be spinning
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

    if ((CCSR_READ32(CCSBAR,CCSR_MCM_PCR) & MCM_PCR_CORE1_ENABLE) == 0x0)
        {
        *CCSR_PTR(CCSBAR,CCSR_BPTR) =
          ((((CORE1T_OFFSET |(UINT32)entryPt) >>  BPTR_BOOT_PAGE_SHIFT ) &
          BPTR_BOOT_PAGE_MASK) | BPTR_EN);

        WRS_ASM("isync;sync");
        sysCpu1Start();
        }

    }

/*******************************************************************************
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
    void *  arg
    )
    {
    int sysMonDelay = 0;
    if (cpuId == 1)
        {

        /* shutdown ints */

        if((CCSR_READ32(CCSBAR,CCSR_MCM_PCR) & MCM_PCR_CORE1_ENABLE) != 0x0)
            {
            vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable), (void *)cpuId);
            *M86XX_BPTR(CCSBAR) = 0x0;
            *((volatile int *)(CPU1_FUNC_START_ADR | CORE1T_OFFSET)) = (int)0;

            /* SMP specific not used just loaded */

            *((volatile int *)(CPU1_STACK_START_ADR | CORE1T_OFFSET)) =
            (int)(RAM_LOW_ADRS - FRAMEBASESZ);

            *((volatile int *)(CPU1_INIT_START_ADR | CORE1T_OFFSET)) =
            (int)MP_MAGIC_RBV;

             WRS_ASM("sync");
            sysCpu1Stop();
            for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);
            *(M8XXX_EPICPRR(CCSBAR)) |= M8XXX_EPICPRR_HRST_CORE1;
            WRS_ASM("sync;isync");

            for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);
            *(M8XXX_EPICPRR(CCSBAR)) &= ~M8XXX_EPICPRR_HRST_CORE1;
            WRS_ASM("sync;isync");
            for(sysMonDelay=0;sysMonDelay<0x100000;sysMonDelay++);
            WRS_ASM("sync;isync");
            }
        return OK;

        }
    /* Cannot "prep" cpu 0 */

    return ERROR;
    }

#endif /* INCLUDE_WRLOAD */

/*******************************************************************************
*
* sysBusTas - test and set a location across the VME bus
*
* This is a required VxWorks function which supports a test and set across the
* VME bus, a necessary component of shared-memory operation.  MVME7100s
* which have Rev 1 of the Tempe (Tsi148) chip on them have only one viable way
* of performing the busTas and that is through generation of a RMW cycle
* across the VME bus.  MVME7100s with Rev 2 or later chips will be able
* to be configured to use bus locking as part of the busTas function.
*
* RETURNS: TRUE if the value had not been set but is now, or
* FALSE if the value was set already.
*/
BOOL sysBusTas
    (
    char * adrs      /* address to be tested and set */
    )
    {
#if defined(INCLUDE_SM_COMMON) && defined(INCLUDE_SMFORVME)
    BOOL state = FALSE;
    UINT32 vmeTasHwAssist = VME_TAS_HW_ASSIST;

    if ((vmeTasHwAssist - VME_BUS_RMW) == 0)
        state = sysVmeRmwTas(adrs);
    else if ((vmeTasHwAssist - VME_BUS_LOCK) == 0)
             state = sysVmeBusLockTas(adrs);
    return (state);
#else /* INCLUDE_SMFORVME */
    return (vxTas(adrs));
#endif /* INCLUDE_SMFORVME */
    }

#if defined(INCLUDE_SM_COMMON) && defined(INCLUDE_SMFORVME)

/*******************************************************************************
*
* sysVmeBusLockTas - test and set a location across the VMEbus with bus locking
*
* This routine performs a test-and-set (TAS) instruction on the specified
* address.  Atomicity is enforced by locking processor interrupts during
* the operation (which keeps other processor resident agents from
* simultaneously affecting the semaphore) and by locking the VME bus (which
* keeps another VME agent from simultaneously updating the semaphore).
*
* RETURNS: TRUE if the value had not been set but is now.
* FALSE if the VMEbus cannot be locked or the value was already set.
*
* SEE ALSO: sysBusTasClear
*/

LOCAL BOOL sysVmeBusLockTas
    (
    char * adrs          /* address to be tested and set */
    )
    {
    BOOL state = FALSE;  /* semaphore state */
    UINT32 lockValue;
    volatile UINT32 * lockAdrs = (UINT32 *)adrs;

    if ((state = sysVmeBusLock ()) == TRUE)
        {

        state = FALSE;
        lockValue =  ((0x80 + sysProcNumGet ())<< 24);

        if (*(UINT32 *)lockAdrs == 0)
            {

            /* No one else has the lock, grab it */

            while (*(UINT32 *)lockAdrs != lockValue)

                /* Reread it to push it out */

                *(UINT32 *)lockAdrs = lockValue;
            state = TRUE;
            }

        sysVmeBusUnlock ();

        }

    return (state);
    }

/*******************************************************************************
*
* sysBusTasClear - Clear a semaphore over the VME bus
*
* Using a VME bus access, unlock the semaphore at the specified address by
* writing a zero to it.
*
* RETURNS: NA
*
* SEE ALSO: sysBusTas
*/

LOCAL void sysBusTasClear
    (
     char * locAdrs
    )
    {
    char * vmeAdrs;
    char * adrs;

    adrs = locAdrs;

    /*
     * If the address to clear is a local DRAM address then we must first
     * convert it to a VME bus address.
     */

    if (IS_DRAM_ADDRESS(locAdrs))
        {

        /* First convert local DRAM access to a VME bus address */

        if (sysLocalToBusAdrs (VME_AM_EXT_SUP_DATA, locAdrs,
                                &vmeAdrs) == ERROR)
            return;

        /*
         * Now convert VME bus address to a local address which provides
         * access over the VME bus.
         */

        if (sysBusToLocalAdrs (VME_AM_EXT_SUP_DATA, vmeAdrs, &adrs) == ERROR)
            return;

        }

    *(UINT32 *)adrs = 0;        /* Clear the semaphore */
    (void) sysInLong ((UINT32)adrs);    /* Push it out */
    }

/*******************************************************************************
*
* sysVmeRmwTas - test and set a location across the VMEbus using RMW.
*
* This routine performs a test-and-set (TAS) instruction on the specified
* address.  Atomicity is enforced by using the Tempe's RMW capability to
* perform an indivisible operation across the VME bus.  To be successful
* managing semaphores across the bus it is necessary for both the setting
* (acquiring) and clearing (releasing) to be done across the VME bus.
* Setting (acquiring) is done with a Tempe RMW cycle.  Clearing (releasing)
* is done with a simple write of zero done across the VME bus.
*
* RETURNS: TRUE if the value had not been set but is now, FALSE otherwise.
*
* SEE ALSO: sysBusTasClear
*/

LOCAL BOOL sysVmeRmwTas
    (
    char * locAdrs              /* local address of lock variable */
    )
    {
    char *      vmeAdrs;
    char *      adrs;

    adrs = locAdrs;

    /*
     * If the address to clear is a local DRAM address then we must first
     * convert it to a VME bus address.
     */

    if (IS_DRAM_ADDRESS(locAdrs))
        {

        /* First convert local DRAM access to a VME bus address */

        if (sysLocalToBusAdrs (VME_AM_EXT_SUP_DATA, locAdrs,
                               &vmeAdrs) == ERROR)
            return (FALSE);

        /*
         * Now convert VME bus address to a local address which provides
         * access over the VME bus.
         */

        if (sysBusToLocalAdrs (VME_AM_EXT_SUP_DATA, vmeAdrs, &adrs) == ERROR)
            return (FALSE);

        }

    return (sysVmeRmw(adrs));
    }

/*******************************************************************************
*
* sysVmeRmw - Perform RMW cycle accross the VME bus
*
* This routine performs a RMW cycle across the VME bus.  To prevent
* deadlocks, interrupts are disabled during the RMW operation.
*
* RETURNS: TRUE if the value had not been set but is now, FALSE otherwise.
*/

LOCAL BOOL sysVmeRmw
    (
    char * vmeAdrs          /* VME address to be tested and set */
    )
    {

    BOOL state = FALSE;  /* semaphore state */

#ifdef _WRS_CONFIG_SMP
    spinLockIsrTake(&sysTempeSpinlock);
#else
    int key;
    key = intLock();
#endif /* _WRS_CONFIG_SMP */

    /* Enable RMW cycle */

    sysBusRmwEnable(VME_RMW_ENABLE_BITS,                /* enable bits */
                    VME_RMW_COMPARE_BITS,               /* cmpare */
                    VME_RMW_SWAP_TO_SET,                /* swap */
                    (char *)vmeAdrs);

    /* perform RMW to try and set TAS location */

    state = *((UINT32 *)vmeAdrs);
    EIEIO_SYNC;

    /* Disable RMW cycle */

    sysBusRmwDisable();

    /* unlock the interrupt */
#ifdef _WRS_CONFIG_SMP
    spinLockIsrGive(&sysTempeSpinlock);
#else
    intUnlock(key);
#endif /* _WRS_CONFIG_SMP */

    /* return TAS test result */

    return (state ? FALSE : TRUE);
    }
#endif /* INCLUDE_SM_COMMON */

/*******************************************************************************
*
* sysUsDelay - delay for the specified amount of time (uS)
*
* This routine will delay for the specified amount of time by counting
* decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* the sysUsDelay() causing a slightly longer delay than requested.
*
* This routine will produce incorrect results if the delay time requested
* requires a count larger than 0xffffffff to hold the decrementer
* elapsed tick count.  For a System Bus Speed of 67 MHZ this amounts to
* about 258 seconds.
*
* RETURNS: N/A
*/

void sysUsDelay
    (
    UINT        delay                   /* length of time in MS to delay */
    )
    {
    register UINT oldval;               /* decrementer value */
    register UINT newval;               /* decrementer value */
    register UINT totalDelta;           /* Dec. delta for entire delay period */
    register UINT decElapsed;           /* cumulative decrementer ticks */

    /*
     * Calculate delta of decrementer ticks for desired elapsed time.
     * The macro DEC_CLOCK_FREQ MUST REFLECT THE PROPER 6xx BUS SPEED.
     */

    totalDelta = ((DEC_CLOCK_FREQ / 4) / 1000000) * delay;

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

        if ( DELTA(oldval,newval) < 1000 )
            decElapsed += DELTA(oldval,newval);  /* no rollover */
        else
            if (newval > oldval)
                decElapsed += abs((int)oldval);  /* rollover */

        oldval = newval;
        }
    }

/*******************************************************************************
*
* sysDelay - Delay for 1 ms.
*
* Call sysUsDelay with a 1000 uSec delay value to delay 1 ms.
*
* RETURNS : NONE
*
*/

void sysDelay (void)
    {
    sysUsDelay (1000);
    }

LOCAL INT_HANDLER_DESC * sysSharedIntTbl [256];

/*******************************************************************************
*
* sysMv7100SharedInt - interrupt handler for shared external epic interrupts
*
* This routine handles interrupts which are shared for external EPIC interrupts
*
* RETURNS: N/A
*
*/

void sysMv7100SharedInt
    (
    int intNum
    )
    {
    INT_HANDLER_DESC * currentHandler;

    if ((currentHandler = sysSharedIntTbl [intNum]) == NULL)
        logMsg ("uninitialized Shared Interrupt interrupt %d\n", intNum,
                0,0,0,0,0);
    else
        {
        while (currentHandler != NULL)
            {
            currentHandler->vec (currentHandler->arg);
            currentHandler = currentHandler->next;
            }
        }
    }

/*******************************************************************************
*
* sysMv7100IntConnect - connect the BSP interrupt to the proper epic/i8259 handler.
*
* This routine checks the INT level and connects the proper routine.
* pciIntConnect() or intConnect().
*
* RETURNS:
* OK, or ERROR if the interrupt handler cannot be built.
*
*/

STATUS sysMv7100IntConnect
    (
    VOIDFUNCPTR *vector,        /* interrupt vector to attach to     */
    VOIDFUNCPTR routine,        /* routine to be called              */
    int parameter               /* parameter to be passed to routine */
    )
    {

    if (((int)vector >= TEMPE_INT_NUM_MIN &&
         (int) vector <= TEMPE_INT_NUM_MAX ) ||
        ((int)vector >= USR_INT_NUM_MIN &&
         (int) vector <= USR_INT_NUM_MAX ))
        {
        INT_HANDLER_DESC *	pNewHandler;
        INT_HANDLER_DESC *	pCurrHandler;

        /* create a new interrupt handler */

        pNewHandler = malloc (sizeof (INT_HANDLER_DESC));

        /* check if the memory allocation succeed */

        if (pNewHandler == NULL)
            return (ERROR);

        /*  initialize the new handler  */

        pNewHandler->vec = routine;
        pNewHandler->arg = parameter;
        pNewHandler->next = NULL;

        /* install the handler in the system interrupt table  */

        spinLockIsrTake(&sysIntConnectSpinlock);

        sysMv7100IntDisable((int)vector);

        if (sysIntTbl [(int) vector] == NULL)
            {
            sysIntTbl [(int) vector] = pNewHandler;  /* single int. handler case */
            }
        else
            {
            pCurrHandler = sysIntTbl[(int) vector];/* multiple int. handler case */

            while (pCurrHandler->next != NULL)
                {
                pCurrHandler = pCurrHandler->next;
                }

            pCurrHandler->next = pNewHandler;
            }

        spinLockIsrGive(&sysIntConnectSpinlock);
        return(OK);
        }
    else if ((int)vector == EPIC_EXT_IRQ9_INT_VEC ||
             (int) vector == EPIC_EXT_IRQ11_INT_VEC )
        {
        INT_HANDLER_DESC *	pNewHandler;
        INT_HANDLER_DESC *	pCurrHandler;

        /* create a new interrupt handler */

        pNewHandler = malloc (sizeof (INT_HANDLER_DESC));

        /* check if the memory allocation succeed */

        if (pNewHandler == NULL)
            return (ERROR);

        /*  initialize the new handler  */

        pNewHandler->vec = routine;
        pNewHandler->arg = parameter;
        pNewHandler->next = NULL;

        /* install the handler in the system interrupt table  */

        spinLockIsrTake(&sysIntConnectSpinlock);

        sysMv7100IntDisable((int)vector);

        if (sysSharedIntTbl [(int) vector] == NULL)
            {
            sysSharedIntTbl [(int) vector] = pNewHandler;  /* single int. handler case */
            _vxb_intConnectRtn(vector, sysMv7100SharedInt, (int) vector);
            }
        else
            {
            pCurrHandler = sysSharedIntTbl[(int) vector];/* multiple int. handler case */

            while (pCurrHandler->next != NULL)
                {
                pCurrHandler = pCurrHandler->next;
                }

            pCurrHandler->next = pNewHandler;
            }

        spinLockIsrGive(&sysIntConnectSpinlock);
        return(OK);
        }
    else
        {
        if (_vxb_intConnectRtn == NULL)
            return(ERROR);
        return (_vxb_intConnectRtn(vector, routine, parameter));
        }
    }


/*******************************************************************************
*
* sysMv7100IntEnable - enable an interrupt
*
* This function call is used to enable an interrupt.  If it is a Tempe interrupt
* the Tempe routine is called, otherwise it is passed on to VxBus.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*/

STATUS sysMv7100IntEnable
    (
    int intNum
    )
    {
    if (intNum >= TEMPE_INT_NUM_MIN && intNum <= TEMPE_INT_NUM_MAX)
        {
        return(sysTempeIntEnable(intNum));
        }
    else if (intNum >= USR_INT_NUM_MIN && intNum <= USR_INT_NUM_MAX)
        {
        return(sysUsrIntEnable(intNum));
        }
    else
        {
        if (_vxb_intEnableRtn == NULL)
            return(ERROR);
        return (_vxb_intEnableRtn(intNum));
        }
    }


/*******************************************************************************
*
* sysMv7100IntDisable - disable an interrupt
*
* This function call is used to disable an interrupt.
*
* RETURNS: OK or ERROR if unable to disable interrupt.
*/

STATUS sysMv7100IntDisable
    (
    int intNum
    )
    {
    if (intNum >= TEMPE_INT_NUM_MIN && intNum <= TEMPE_INT_NUM_MAX)
        {
        return(sysTempeIntDisable(intNum));
        }
    else if (intNum >= USR_INT_NUM_MIN && intNum <= USR_INT_NUM_MAX)
        {
        return(sysUsrIntDisable(intNum));
        }
    else
        {
        if (_vxb_intDisableRtn == NULL)
            return(ERROR);
        return (_vxb_intDisableRtn(intNum));
        }
    }


/*******************************************************************************
*
* sysUsrIntEnable - enable an interrupt
*
* This function call is used to enable a user interupt interrupt.  Currently,
* it does nothing, but could be filled in with a call to a user supplied
* routine.
*
* RETURNS: OK .
*/

STATUS sysUsrIntEnable
    (
    int intNum
    )
    {
    return(OK);
    }


/*******************************************************************************
*
* sysUsrIntDisable - disable an interrupt
*
* This function call is used to disable a user interupt interrupt.  Currently,
* it does nothing, but could be filled in with a call to a user supplied
* routine.
*
* RETURNS: OK .
*/

STATUS sysUsrIntDisable
    (
    int intNum
    )
    {
    return(OK);
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
        pMmu->virtualAddr = (VIRT_ADDR) address;
        pMmu->physicalAddr = (PHYS_ADDR) address;
        pMmu->len = length;
        pMmu->initialStateMask = initialStateMask;
        pMmu->initialState = initialState;
        sysPhysMemDescNumEnt += 1;
        }

    return (result);
    }
#endif /* INCLUDE_MMU */

/*******************************************************************************
*
* coreShow - display routine for core clocking
*
* This route show the clk of the core
*
* RETURNS: NONE
*
* ERRNO
*
*/
void coreClkShow()
{

 printf("CCSRBAR 0x%x\n",CCSBAR);

 printf("\nOscillator Freq: %d\n",oscFreq);

 if( (oscFreq != FREQ_40_MHZ) && (oscFreq != FREQ_50_MHZ) )
     printf ("Unsupported OSCILLATOR_FREQ. BSP update reqduired.\n");

 printf("MPX Bus Speed:   %d\n",sysClkFreqGet());
 printf("DDR:             %d\n",sysClkFreqGet()>>1);
 printf("Core Freq:       %d\n",coreFreq);

}

/*******************************************************************************
*
* sysUserLedSet - Turn user LEDs on or off.
*
* Turn specified user LED (1, 2, or 3) on or off.
* User LED 1 is on the front panel. User LEDs' 2 and 3 are on the
* board surface (non-component side) near the front panel edge.
*
* RETURNS : N/A.
*/

void sysUserLedSet
    (
    UINT8 whichLed,  /* User LED denoted by 1, 2, or 3 */
    int   value      /* for LED 2 and 3, 0=OFF, 1=ON */
                     /* for LED 1, 0=OFF, 1=RED, 2=YELLOw, 3=ORANGE */
    )
    {
    UINT8 mask = 0x1;
    UINT8 shift = whichLed-1;
    volatile UINT8 regVal = *(volatile UINT8 *)BRD_SYSTEM_STATUS_INDICATOR_REG;

    if (whichLed == 2 || whichLed == 3)
        {
        shift = whichLed;
        if (value)
           {
           regVal |= (mask << shift);
           }
        else
           {
           regVal &= ~(mask << shift);
           }
        }
    else if (whichLed == 1)
        {
        value &= 0x3;
        regVal &= 0xFC;
        regVal |= value;
        }

    *(UINT8 *)BRD_SYSTEM_STATUS_INDICATOR_REG = regVal;
    return;
    }

/*******************************************************************************
*
* sysBrdHardReset - hard reset the board.
*
* This function forces an immediate and complete hard reset of the
* board.
*
* RETURNS : N/A.
*/

void sysBrdHardReset (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_SYSTEM_CONTROL_REG;

    regVal |= BRD_SYSTEM_CONTROL_RESET_VAL;
    *(UINT8 *)BRD_SYSTEM_CONTROL_REG = regVal;
    }

/*******************************************************************************
*
* sysBootBlkAIsSelected - returns the select state of Boot Block A.
*
* This function returns TRUE when boot block A is the active
* boot block. If FALSE is returned the selected boot is necessarily B.
*
* RETURNS : TRUE if Boot Block A is selected. FALSE when Boot Block B
* is selected.
*/

BOOL sysBootBlkAIsSelected (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_FLASH_CONTROL_REG;
    return ((regVal & BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_MASK) == 0);
    }

/*******************************************************************************
*
* sysEepromWriteProtectSet - set the EEPROM write protect state.
*
* This function sets the EEPROM write protection to on or off.
*
* RETURNS: N/A.
*/

void sysEepromWriteProtectSet
    (
    BOOL setOn  /* TRUE sets write protection. FALSE enables writing. */
    )
    {
    UINT8 regVal = *(UINT8 *)BRD_SYSTEM_CONTROL_REG;

    if (setOn)
        {
        regVal |= BRD_SYSTEM_CONTROL_EEPROMWP_MASK;
        }
    else
        {
        regVal &= ~BRD_SYSTEM_CONTROL_EEPROMWP_MASK;
        }

    *(UINT8 *)BRD_SYSTEM_CONTROL_REG = regVal;
    }

/*******************************************************************************
*
* sysFlashIsHwWriteProtected - returns the FLASH write protect state.
*
* This function returns TRUE when the FLASH bank is hardware
* write protected.
*
* RETURNS : TRUE when hardware write protectd, FALSE when hardware
* write enabled.
*/

BOOL sysFlashIsHwWriteProtected (void)
    {
    UINT8 regVal = *(UINT8 *)BRD_FLASH_CONTROL_REG;

    return ((regVal & BRD_FLASH_CONTROL_HARDWARE_WP_MASK) != 0);
    }

/*******************************************************************************
*
* sysFlashSwWriteProtectSet - set FLASH write protect on or off.
*
* This function sets the FLASH software write protection to on or off.
*
* RETURNS : N/A.
*/

void sysFlashSwWriteProtectSet
    (
    BOOL setOn   /* TRUE sets write protection, FALSE enables writing */
    )
    {
    UINT8 regVal = *(UINT8 *)BRD_FLASH_CONTROL_REG;

    if (setOn)
        {
        regVal |= BRD_FLASH_CONTROL_SOFTWARE_WP_MASK;
        }
    else
        {
        regVal &= ~BRD_FLASH_CONTROL_SOFTWARE_WP_MASK;
        }

    *(UINT8 *)BRD_FLASH_CONTROL_REG = regVal;
    }

/*******************************************************************************
*
* necPd720101ExtConfig - configures the Ext registers in the config header
*
* This routine configures the Ext registers in the vendor specific
* area of the USB controller's PCI configuration space. This configuration
* is unique to the NEC uPD720101 controller as used on the MV7100.
* The controller is set the higher clock speed in the EXT2 config
* register. The controller has three functions. Functions 0 and 2 have
* EXT2 registers that must be programmed. All controllers are configured.
*
* RETURNS: N/A
*/

#ifdef INCLUDE_PCI_BUS
void necPd720101ExtConfig (void)
    {
    int pciBusNo;
    int pciDevNo;
    int pciFuncNo;
    int instance = 0;
    STATUS status = OK;

    while (status == OK)
        {
        status = pciFindDevice ((PCI_ID_USB & 0xFFFF),
                                (PCI_ID_USB >> 16) & 0xFFFF,
                                instance, &pciBusNo, &pciDevNo, &pciFuncNo);

        if (status == OK)
            {
            pciConfigOutLong (pciBusNo, pciDevNo, 0, PCI_CONFIG_EXT2_OFFSET,
                              HIGH_CLOCK_SPD_SEL);
            pciConfigOutLong (pciBusNo, pciDevNo, 2, PCI_CONFIG_EXT2_OFFSET,
                              HIGH_CLOCK_SPD_SEL);
            }

        ++instance;
        }
    }
#endif /* INCLUDE_PCI_BUS */

#ifdef BUS_PROBE_TEST

#define START_PCI_TEST 0x00000000
#define STOP_PCI_TEST  0xffffffff

/*******************************************************************************
*
* pciAdrsTest - test isPciAdrs routine
*
* This test is designed to test the proper functioning of the
* isPciAdrs() routine which is used for bus probing support.  This test
* will evaluate each address in the range of 0x00000000 to 0xfffffff to
* determine if it is a PCI address.  Must visually verify that the
* addresses identified as PCI space are indeed PCI addresses.
*
* RETURNS : N/A
*/

void pciAdrsTest (void)
    {
    UINT32 i;
    UINT32 lastState = 1234;
    UINT32 state;
    UINT32 trans = 4321;
    UINT32 lastDiff = 1234;
    UINT32 newDiff;
    printf("isPciAdrs() address test start = 0x%08x, stop = 0x%08x\n\n",
    START_PCI_TEST,STOP_PCI_TEST);
    for (i = START_PCI_TEST; ((i<= STOP_PCI_TEST) &&
         (i != 0xffffffff)); i++)
        {
        state = isPciAdrs((char *)i,(char **)&trans);
        newDiff = i - trans;
        if ((state != lastState) ||
            ((state == TRUE) && (newDiff != lastDiff)))
            {
            printf("          \r");
            if ((state == TRUE) ||
                 ((state == TRUE) && (newDiff != lastDiff)))
                {
                printf("\n0x%08x TRUE, pciTrans = 0x%08x\n",i,trans);
                lastDiff = newDiff;
                }
                if (state == FALSE)
                    printf("0x%08x FALSE\n",i);
            lastState = state;
            }
        else
            {
            if ((i % 0x1000000) == 0)
                printf("0x%08x\r",i);
            }
        }
    printf("          \r");
    }

#define START_VME_TEST 0x00000000
#define STOP_VME_TEST  0xffffffff

/*******************************************************************************
*
* vmeAdrsTest - test isVmeAdrs routine
*
* This test is designed to test the proper functioning of the
* isVmeAdrs() routine which is used for bus probing support.  This test
* will evaluate each address in the range of 0x00000000 to 0xfffffff to
* determine if it is a PCI address.  Must visually verify that the
* addresses identified as VME space are indeed VME addresses.
*
* RETURNS : N/A
*/

void vmeAdrsTest (void)
    {
    UINT32 i;
    UINT32 lastState = 1234;
    UINT32 state;
    UINT32 trans = 4321;
    UINT32 outRegSet;
    UINT32 lastDiff = 1234;
    UINT32 newDiff = 0;
    printf("isVmeAdrs() address test start = 0x%08x, stop = 0x%08x\n\n",
            START_VME_TEST,STOP_VME_TEST);
    for (i = START_VME_TEST;
         ((i<= STOP_VME_TEST) && (i != 0xffffffff)); i++)
        {
        state = isVmeAdrs((char *)i,(char **)&trans,&outRegSet);
        newDiff = i - trans;
        if ((state != lastState) ||
            ((state == TRUE) && (newDiff != lastDiff)))
            {
            printf("          \r");
            if ((state == TRUE) ||
                ((state == TRUE) && (newDiff != lastDiff)))
                {
                printf("\n0x%08x TRUE, vmeTrans = 0x%08x, "
                       "outRegSet = 0x%08x\n",
                       i,trans,outRegSet);
                lastDiff = newDiff;
                }
            if (state == FALSE)
                printf("0x%08x FALSE\n",i);
            lastState = state;
            }
        else
            {
            if ((i % 0x1000000) == 0)
                printf("0x%08x\r",i);
            }
        }
    printf("          \r");
    }

#define START_RPROBE1_TEST 0x00000000
#define STOP_RPROBE1_TEST  0xFFFFFFFF
#define INTERVAL_RPROBE1_TEST 0x100

/*******************************************************************************
*
* rprobe1test - read probe test
*
* This test is designed to test the proper functioning of the
* sysBusProbe() routine when performing read probes.  This test
* will evaluate each address in the range of 0x00000000 to 0xfffffff to
* determine if it is read probeable.  Must visually verify that the
* addresses identified as read probeable are indeed read probeable.
*
* RETURNS : N/A
*/

void rprobe1test (void)
    {
    UINT32 i;
    UINT32 lastState = 1234;
    UINT32 state;
    UINT8  r8;
    UINT32 lasti = 0;
    printf("rprobe1()\n address test start = 0x%08x, stop = 0x%08x, "
           "interval = 0x%08x\n\n",
           START_RPROBE1_TEST,STOP_RPROBE1_TEST,INTERVAL_RPROBE1_TEST);
    for (i = START_RPROBE1_TEST;;)
        {
        state = sysBusProbe((char *)i, VX_READ, 1, &r8);
        if (state != lastState)
            {
            printf("          \r");
            if (state == OK)
                printf("\n0x%08x OK\n",i);
            if (state == ERROR)
                printf("0x%08x ERROR\n",i);
            lastState = state;
            }
        else
            {
            if ((i % 0x100000) == 0)
                printf("0x%08x\r",i);
            }
        lasti = i;
        i+= INTERVAL_RPROBE1_TEST;
        if ((i < lasti) || (i > STOP_RPROBE1_TEST))
            break;
        }
    printf("          \r");
    }

/*******************************************************************************
*
* wprobeit - write probe test support routine
*
* This routine is not meant to be called directly from the kernel
* prompt, it is a support routine for the wprobe routine that follows.
*
* RETURNS : N/A
*/

void wprobeit
    (
    UINT32 adrs,
    int    size,
    STATUS expect
    )
    {
    UINT32 temp = 12345678;
    STATUS stat;
    printf("write probing size= %d, addr = 0x%08x, result = ",
            size, (char *)adrs);
    stat = sysBusProbe((char *)adrs, VX_WRITE, size, &temp);
    printf("%s ",(stat == OK)?"OK   ":"ERROR");
    if (stat == expect)
        printf(" TEST passed\n");
    else
        printf(" TEST FAILED\n");
    }

/*******************************************************************************
*
* wprobe - write probe test
*
* This test is designed to test the proper functioning of the
* sysBusProbe() routine when performing write probes.  This test
* will evaluate specific "safe" addresses to determine whether or not
* they are write probeable.  We must be careful since write-probing
* can be very destructive.  Must visually verify that the addresses
* are indeed write probeable.
*
* RETURNS : N/A
*/

void wprobe (void)
    {
    wprobeit(0x1f0,4,OK);           /* DRAM */
    wprobeit(0x1ffffffe,2,OK);      /* Last 2-bytes of DRAM */
    wprobeit(0x20000000,1,ERROR);   /* 1st byte beyond DRAM */
    wprobeit(0x80000000,1,OK);      /* VME A32 maps to DRAM win 1*/
    wprobeit(0x80100000,2,ERROR);   /* Just outside VME window 1 */
    wprobeit(0x8fff0000,1,OK);      /* Just inside VME win 0 */
    wprobeit(0x8fff0fff,1,OK);      /* inside VME win 0 my CSR */
    wprobeit(0x8fff1000,1,ERROR);   /* outside VME win 0 outside CSR */
    wprobeit(0x8ffeffff,1,ERROR);   /* Just before VME win 0 */
    wprobeit(0x90000000,1,ERROR);   /* Just inside VME win 2 A24 */
    wprobeit(0x91000000,1,ERROR);   /* Just inside VME win 3 A16 */
    wprobeit(0xa0000000,4,OK);      /* PCI with responder */
    wprobeit(0xa0700000,4,ERROR);   /* PCI with no responder */
    wprobeit(0xf8000000,1,OK);      /* Boot Flash bank */
    }
#endif /* BUS_PROBE_TEST */


#ifdef INCLUDE_PCI_BUS
/* INT LINE TO IRQ assignment for mv7100 board. */

LOCAL UCHAR intLine [][8] =
    {
        { PCIE1_INT0_VEC,PCIE1_INT1_VEC,PCIE1_INT2_VEC,PCIE1_INT3_VEC },
        { PCIE1_INT1_VEC,PCIE1_INT2_VEC,PCIE1_INT3_VEC,PCIE1_INT0_VEC },
        { PCIE1_INT2_VEC,PCIE1_INT3_VEC,PCIE1_INT0_VEC,PCIE1_INT1_VEC },
        { PCIE1_INT3_VEC,PCIE1_INT0_VEC,PCIE1_INT1_VEC,PCIE1_INT2_VEC },
        { PCIE2_INT0_VEC,PCIE2_INT1_VEC,PCIE2_INT2_VEC,PCIE2_INT3_VEC },
        { PCIE2_INT1_VEC,PCIE2_INT2_VEC,PCIE2_INT3_VEC,PCIE2_INT0_VEC },
        { PCIE2_INT2_VEC,PCIE2_INT3_VEC,PCIE2_INT0_VEC,PCIE2_INT1_VEC },
        { PCIE2_INT3_VEC,PCIE2_INT0_VEC,PCIE2_INT1_VEC,PCIE2_INT2_VEC },
    };

/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI 1 autoconfig support routine
*
* This route is used in hwconf.c file
*
* RETURNS : OK or ERROR
*/

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UINT devVend            /* deviceID/vendorID of device */
    )
    {

    /*
     * Only support BUS 0;
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
* This route is used in hwconf.c
*
* RETURNS: OK or ERROR
*/

STATUS sysPci2AutoconfigInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UINT devVend            /* deviceID/vendorID of device */
    )
    {

    /*
     * Only support BUS 0;
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
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    UCHAR irqValue = 0xff;

    /* VME Tempe Bridge */

    if (pLoc->bus == 5)
        {
        if (pLoc->device == 2)
            {
            irqValue = intLine [3][(pin - 1)];
            }
        }

    /* USB Controller */

    if (pLoc->bus == 11)
        {
        if (pLoc->device == 2)
            {
            irqValue = intLine [0][(pin - 1)];
            }
        }

    /* Slot 1 */

    if (pLoc->bus == 7)
        {
        irqValue = intLine [(pLoc->device + 2) % 4][(pin - 1)];
        }

    /* Slot 2 */

    if (pLoc->bus == 9)
        {
        irqValue = intLine [(pLoc->device + 1) % 4][(pin - 1)];
        }

    /* return the value to be assigned to the pin */

    return (irqValue);
    }

/*******************************************************************************
*
* sysPci2AutoconfigIntrAssign - PCI 2 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci2AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    VXB_DEVICE_ID pDev;
    UCHAR irqValue = 0xff;
    int pciBus;
    UINT8 rotVal;
    STATUS status;

    pDev = vxbInstByNameFind ("m85xxPci", 0);

    /*
     * Set bus on which to begin search to its initial value. Start on
     * bus 1 since the upstream bridge of any first-level PCIe switch
     * resides here. 
     */

    pciBus = 1;
    rotVal = 0;

    /*
     * Determine rotation value from device numbers of all bridges between
     * device and host bridge.
     */

    status = sysPci2SetIntrRotation (pDev, pLoc, pciBus, &rotVal);

    /*
     * In case of error, load some sort of default value ....
     */

    if (status != OK)
        {
        rotVal = 0x0;
        }

    /*
     * Determine int line value. This should work for any XMCSpan card
     * long as it conforms to the PCI specification in terms of interrupt
     * routing.
     */

    irqValue = ((pLoc->device + rotVal + pin - 1) % 4 + PCIE2_INT0_VEC);  

    /* return the value to be assigned to the pin */

    return (irqValue);
    }

/*******************************************************************************
*
* sysPci2SetIntrRotation - PCI 2 interrupt assign support routine 
*
* This routine traverses the PCIe branch attached to the second PCIe host
* bridge of the MPC8548 searching for each instance of a bridge located
* between the host bridge and the device for which the int line is to be
* established. The second PCIe host bridge merely attaches to an expansion
* connector which accommodates an XMCSpan card. As many as two expansion
* cards can be stacked providing up to four XMC/PMC ports.
* 
* The routine supports any expansion module that conforms to the interrupt
* routing algorithm as described in the PCI/PCI-X/PCIe specifications.
*
* The process is as follows:

* Begin at the initial downstream bus under the host bridge. For PCIe, this
* is bus 1. Traverse the bus looking for the highest-level parent bridge to
* the device. Adjust the rotation value based on the bridge's device number.
* Move to the bridge's secondary bus and recursively repeat until the bus on
* which the device resides is reached. 
*
* RETURNS: OK if at least one parent bridge is found, ERROR otherwise.
*/

STATUS sysPci2SetIntrRotation
    (
    VXB_DEVICE_ID pDev,     /* pointer to bus controller instance */
    PCI_LOC * pLoc,         /* pointer to function in question */
    int pciBus,             /* bus on which to look for a parent bridge */
    UINT8 * rotVal          /* accumulated number of rotations */
    )
    {
    UINT8 secBus;
    UINT8 subBus;
    UINT8 bClass;
    UINT8 bSubClass;
    int bridgeDev;
    int bridgeFunc;
    STATUS status;

    status = ERROR;

    /*
     *Find device's ancestors on current bus.
     */

    bridgeFunc = 0;  /* Always use first function, no need for others. */

    for (bridgeDev = 0; bridgeDev < PCI_MAX_DEV; bridgeDev++)
        {
        vxbPciConfigInByte (pDev, pciBus, bridgeDev, bridgeFunc,
                             PCI_CFG_CLASS, &bClass);
        vxbPciConfigInByte (pDev, pciBus, bridgeDev, bridgeFunc,
                             PCI_CFG_SUBCLASS, &bSubClass);

	if ((bClass == PCI_CLASS_BRIDGE) && (bSubClass == PCI_P2P_BRIDGE))
            {
            vxbPciConfigInByte (pDev, pciBus, bridgeDev, bridgeFunc,
                                PCI_CFG_SECONDARY_BUS, &secBus);
            vxbPciConfigInByte (pDev, pciBus, bridgeDev, bridgeFunc,
                                PCI_CFG_SUBORDINATE_BUS, &subBus);

            if ( (pLoc->bus >= secBus) && (pLoc->bus <= subBus) )
                {
                *rotVal += bridgeDev % 4;
                status = OK;  /* At least one parent bridge has been found. */

                if (pLoc->bus == secBus)
                    {
                    /* We're done, we've reached device's immediate parent. */

                    break;

                    }
                else
                    {
                    /* Continue to bridge's secondary bus. */

                    sysPci2SetIntrRotation (pDev, pLoc, secBus, rotVal);
                    break;
                    }
                }
            }
        }

    return (status);
    }

#endif /* INCLUDE_PCI_BUS */

#if defined(_WRS_CONFIG_SMP)

/*******************************************************************************
*
* sysCpuInit - initialize additional core
*
* This route is used by initialize additional core
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
    cacheArchEnable (INSTRUCTION_CACHE);    /* enable instruction cache */
#endif  /* USER_I_CACHE_ENABLE */

#ifdef  USER_D_CACHE_ENABLE
    cacheArchEnable (DATA_CACHE);          /* enable data cache */
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

/*******************************************************************************
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

    _WRS_ASM ("sync");
    _WRS_ASM ("isync");

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


/*******************************************************************************
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
    *((int *)CPU1_INIT_START_ADR) = (int)(sysCpu1Loop);

#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
    {
#ifndef VX_ENABLE_CPU_TIMEOUT
#define VX_ENABLE_CPU_TIMEOUT   5   /* default timeout five seconds */
#endif  /* VX_ENABLE_CPU_TIMEOUT */

    cpuset_t    cpuset;
    UINT32  i = 0;

    /*
     * PPC Dec timer will not be available while syncronizing timebase
     * between two cores.
     * Use magic number, 0x9000000, to count 1 second independently
     * on PPC Dec timer.
     */

    UINT32  timeout = 0x9000000 * VX_ENABLE_CPU_TIMEOUT;

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
     * Keep stopping timebase till the syncronization between two cores gets
     * completed.
     */

    CCSR_WRITE32 (CCSBAR, CCSR_DEVDISR,
                  CCSR_READ32(CCSBAR, CCSR_DEVDISR) | M8641_DEVDISR_TB);

     /*
      * Save the current timebase on CPU0 into static valuable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * syncronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    sysCpu1Start ();

#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)

    /*
     * usrCpuEnable() or kernelCpuEnable() relies on the timer to count timeout.
     * If the system clock is PPC Dec Timer, it can not be used while timebase
     * is disabled.
     * Use a magic numer for busy loop to count timeout here.
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

    CCSR_WRITE32 (CCSBAR, CCSR_DEVDISR,
                  CCSR_READ32(CCSBAR, CCSR_DEVDISR) & ~M8641_DEVDISR_TB);

    /* Now PPC Dec timer re-started and safe to unblock interrupts. */

    intCpuUnlock (key);

#ifdef  INCLUDE_LOGGING
    if (status == ERROR)
    logMsg ("sysCpuEnable(): Timeout (%d secs) wating to enable "
            "additional CPU: %x\n", VX_ENABLE_CPU_TIMEOUT, cpuNum,
            0, 0, 0, 0);
#endif  /* INCLUDE_LOGGING */
    }
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    return status;
    }

/*******************************************************************************
*
* sysCpuAvailableGet - return the number of CPUs available for SMP
*
* This route return the number of CPUs available for SMP
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
void sysAmpCpuStatusSet
    (
    int status,
    int cpu
    )
    {

    /* only allow CPU1 to set it's own status */

    if( sysProcNumGet() == 1 )
        cpu = 1;

    switch(cpu)
        {
        case 1:
        *((volatile UINT32 *)(AMP_CPU1_STATUS | CORE1T_OFFSET)) = status;
            break;
        case 0:
        *((volatile UINT32 *)(AMP_CPU0_STATUS | CORE1T_OFFSET)) = status;
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
int sysAmpCpuStatusGet
    (
    int cpu
    )
    {
    int status = ampCpuStateError;

    switch(cpu)
    {
    case 1:
        status = *((volatile UINT32 *)(AMP_CPU1_STATUS | CORE1T_OFFSET));
        break;
    case 0:
        status = *((volatile UINT32 *)(AMP_CPU0_STATUS | CORE1T_OFFSET));
        break;
    default:
        break;
    }
    return(status);
    }

#endif /* _WRS_VX_AMP */

/*******************************************************************************
*
* physCpuAvailableGet - return the number of CPUs on the board
*
* This route return the number of CPUs on the board
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

/*******************************************************************************
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

/*******************************************************************************
*
* sysSerialConsoleTty - return the console tty channel for the core
*
* This routine is used to determine the console tty channel for the
* core we are currently running on.  This function is used by the
* CONSOLE_TTY macro.
*
* RETURNS: channel for console TTY.
*/

int sysSerialConsoleTty()
    {
#ifdef INCLUDE_AMP
    if (CPU1CHECK)
        return CORE1_CONSOLE_TTY;
    else
        return CORE0_CONSOLE_TTY;
#else
    return CORE0_CONSOLE_TTY;
#endif /*INCLUDE_AMP */
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
    UINT32  picClockDivisor /* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
        return sysClkFreqGet() / picClockDivisor;
    else
        return 10000000;
    }

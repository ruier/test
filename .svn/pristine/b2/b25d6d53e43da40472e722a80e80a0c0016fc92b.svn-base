/* sysLib.c - Freescale p4080ds board system-dependent library */

/*
 * Copyright (c) 2008-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03w,28feb13,ylu  Enable branch prediction for performance. (WIND00404454)
03v,11jan13,ylu  Update for Rev 3 silicon.
03u,04jun12,c_l  added macro definition for errata A-004510 and A-004511.
                 (WIND00352077) (WIND00349563)
03t,25may12,mpc  added workaround for Freescale A-004529. (WIND00350649)
03s,16may12,mpc  Updated Freescale FMAN microcode. (WIND00349107)
03r,13apr12,syt  Remove the TBEN bit operations for HID0. (WIND00282419)
03q,04apr12,h_k  fixed reboot failure on non-primary Core0 in AMP.
                 (WIND00344093)
                 allowed to disable L3 cache only on Core0. (WIND00341977)
                 removed RESERVED_ATOMICS.
                 added missing L2 cache flush on non-primary core in SMP at
                 reboot. (WIND00341662)
03p,05mar12,h_k  removed extern mmuPpcE500Tlbie. (WIND00338846)
03o,15feb12,x_s  WIND00247656: added comments in function sysCpuEnable().
03n,07feb12,x_s  WIND00331187: added Errata A-003999(FPU) implementation.
03m,01feb12,y_y  WIND00330854: modified the AMP core1-X reboot in sysToMonitor.
03l,30jan12,agf  WIND00329621: Remove SYSTEM_GLOBALS, superceded by
                 RESERVED_ATOMICS
03k,09nov11,ers  WIND00297094: cache fix for vmPageLock/Unlock.
                 WIND00309426: Moved L1 parity handling to arch.
03j,26oct11,ers  WIND00314079: fix support for secondary SMP image.
03i,04oct11,ers  Updating MMU table, add support for LOCAL_MEM_SIZE > 1GB
03h,22sep11,ers  Adding support for CPUn_SPACE (sysToMonitor call) from
                 interrupt context.
03g,08aug11,ers  Replaced DYNAMIC_WRLOAD with generic wrload support.
03f,21jul11,ers  WIND00262613: fix SERDES errata procedure to not disturb
                 lanes used by Aurora.
03e,24jan11,agf  WIND00242091: ensure all D-caches are flushed during SMP
                 sysToMonitor()
03d,13jan11,agf  WIND00247891: add PTEs for BMan's & QMan's reserved memory,
                 remove the QMAN_BMAN_RESERVED_MEM adjustment from the
                 calc of sysMemTop;
                 remove unused and meaningless PTE for BOOTAPP kernels
03c,05jan11,pgh  Change name of RAM_LOW_ADRS_CPU0
03b,07dec10,agf  multiple defect fixes --
                 WIND00239454: move malloc/free in sysCpuEnable to be outside
                 the section of code where interrupts are locked
                 WIND00240610: move vxPirSet to be before MMU static init
                 WIND00241106: use sysL2CacheDisable instead of direct
                 manipulation of enable bits in control reg; this will also
                 ensure the cache is _flushed_ before it's disabled
                 also cleaned-up compiler warnings
03a,06oct10,kab  TEMP undef INCLUDE_CPU_LIGHT_PWR_MGR until arch fix
02z,06oct10,rec  remove INCLUDE_CPU_PWR_MGMT
02y,21sep10,kab  Add bootApp
02x,20sep10,agf  move system bus freq determination after TLB static init
                 move auxClk rate init to sysHwInit2
                 change sysProcNumSet to set sysProcNum, instead of doing nothing
02w,08sep10,agf  correct clk timer freq determination; fix misc warnings
02v,09jul10,wap  Correct serdes_cfg_tbl[] entry for option 5
02u,24may10,wap  Add some additional SERDES8 fixes from Freescale
02t,20may10,wap  Implement workarounds for SERDES8 erratum
02s,27mar10,pmr  cache error handling
02r,24mar10,wap  Add XAUI SERDES erratum workaround
02q,10mar10,pmr  Support CPC2, improve sysToMonitor() for U-Boot and Linux
02p,09mar10,wap  Correct sysFmanClkFreqGet() so that it tests the right CPU
                 core complex PLL ratio (3rd, not 4th)
02o,22feb10,wap  Add support for Fman microcode patches for both rev 1 and rev
                 2 silicon
02n,15jan10,pmr  SMP M-N; virtual SM region for 36-bit
02m,14jan10,wap  Update PCI interrupt assignments to handle PCI devices in
                 different slot numbers on the other side of PCIe-PCI bridges
02l,11jan10,wap  Update SerDes fixup code to handle second SGMII riser in slot
                 4, if installed
02k,11dec09,pmr  fix sysFmanClkFreqGet() for Simics, coreShow() tweaks
02j,26nov09,wap  Add a routine to obtain the Fman clock frequency
02i,20nov09,pmr  modifications for 36-bit wrload
02h,19nov09,wap  Correct the VIRT_PHYS_BIAS macro
02g,17nov09,wap  Add 36-bit virt<->phys translation APIs
02f,12nov09,rab  remove unused TFFS MTD include
02e,10nov09,pmr  use config macros for MIPC SM sysPhysMemDesc entry
02d,29oct09,pmr  split static TLB for RAM for 36-bit; cachePpcReadOrigin
02c,11aug09,h_w  Add MIPC shared-memory description to sysStaticTlbDesc
02b,22oct09,pmr  remove duplicate PIXIS entry from sysPhysMemDesc, fix warnings
02a,22oct09,pmr  protect all TLB1 entries if in a Linux-VxWorks AMP system
01z,21oct09,wap  Fix sysDeviceFilter() for PCI devices
01y,16oct09,pmr  correct TLB setup for UBOOT
01x,09oct09,pmr  multicore reboot
01w,08oct09,rab  Add SPI and I2c support
01v,02oct09,wap  Correct typo in 3rd PCIe TLB entry
01u,01oct09,wap  Add PCI support
01t,29sep09,pmr  remove GPIO twiddling
01s,24sep09,pmr  temporarly make sysAmpCpuStatusSet() a nop
01r,30aug09,wap  Make ETSEC optional
01q,21aug09,wap  Correct sysStaticTlbDesc[] entries for Qman and Bman
                 cacheable ranges, modify sysMemTop() to reserve memory
                 for Bman and Qman internal memory regions
01p,11aug09,pmr  fix coreShow, flash TLB entry, remove L2 SRAM
01o,19jun09,pmr  36-bit addressing
01n,22jan09,wap  Add support for hardware buffer manager
01m,12jan09,pmr  fix warnings
01l,22dec08,wap  Add support for hardware queue manager
01k,19dec08,pmr  AMP tweaks
01j,04dec08,pmr  L2 cache, tsec coreNum
01i,20nov08,pmr  fix SYS_MODEL, sysPhysMemDesc[0] for AMP
01h,13oct08,pmr  add sysAmpCpuStatusSet/Get()
01g,09oct08,pmr  remove WRLOAD_IMAGE_BUILD conditionals in static TLB desc
01f,10sep08,pmr  BSP name change
01e,03sep08,pmr  modifications for AMP
01d,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01c,04jun08,dtr  Create FORCE_BOOT_LINE component.
01b,30may08,pmr  mods for AMP
01a,05may08,pmr  adapted from ads8572, version 01i.
*/

/*
DESCRIPTION
This library provides board-specific routines for p4080ds

INCLUDE FILES:

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vme.h>
#include <memLib.h>
#include <cacheLib.h>
#include <sysLib.h>
#include "config.h"
#include <string.h>
#include <intLib.h>
#include <logLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <arch/ppc/mmuE500Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>
#include <miiLib.h>
#include <cpuset.h>

#ifdef INCLUDE_WRLOAD
#   include <wrload.h>
    IMPORT UINT32 usrWrloadBiasHighGet(WRLOAD_ID id); /* from usrWrload.c */
    IMPORT UINT32 usrWrloadBiasLowGet (WRLOAD_ID id); /* from usrWrload.c */
#endif /* INCLUDE_WRLOAD */

#ifdef _WRS_CONFIG_SMP
#include <private/cpcLibP.h>
#include <private/kernelLibP.h>         /* KERNEL_ENTERED_ME() */
#endif

#ifdef INCLUDE_AMP
#include <vxIpiLib.h>
#endif

#ifdef INCLUDE_P4080_FMAN_UCODE
#include <hwif/vxbus/hwConf.h>
#include <h/vxbus/vxbAccess.h>
#endif /* INCLUDE_P4080_FMAN_UCODE */

#ifdef INCLUDE_PCI_BUS
    #include <drv/pci/pciConfigLib.h>
    #include <drv/pci/pciIntLib.h>
#endif /* INCLUDE_PCI_BUS */

/* globals */
char    sysHwErrorString[200]; /* about 180 characters if all errors print */
#define hwErrStringEnd (&(sysHwErrorString[strlen(sysHwErrorString)]))
#define hwErrStringSz  (sizeof(sysHwErrorString) - strlen(sysHwErrorString) -1)

/* definitions for core cluster ID setting in PIR */

#define PIR_CORE_CLUSTER_ID_SHIFT	5
#define PIR_CORE_CLUSTER_ID_MASK	0x1f
#define PIR_CORE_CLUSTER_ID_SET(id)	\
	((id & PIR_CORE_CLUSTER_ID_MASK) << PIR_CORE_CLUSTER_ID_SHIFT)

/* protect all TLB1 entries if in a Linux-VxWorks system. */
#ifdef UBOOT
#define PROTECTED       _MMU_TLB_IPROT
#else
#define PROTECTED       0
#endif

#ifdef  WRLOAD_IMAGE_BUILD_PHYS_BIAS
#define PHYS_BIAS_LO  (UINT32)         (WRLOAD_IMAGE_BUILD_PHYS_BIAS)
#define PHYS_BIAS_HI  (UINT32) ((UINT64)WRLOAD_IMAGE_BUILD_PHYS_BIAS >> 32)
#else
#define WRLOAD_IMAGE_BUILD_PHYS_BIAS 0
#define PHYS_BIAS_LO 0
#define PHYS_BIAS_HI 0
#endif


#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
/*
 * WARNING: The loaded image base address (LOCAL_MEM_LOCAL_ADRS +
 * WRLOAD_IMAGE_BUILD_PHYS_BIAS) must be modulo RAM_TLB_SZ size.
 */

#define RAM_TLB_SZ ( (LOCAL_MEM_SIZE <= 0x04000000)  ? _MMU_TLB_SZ_64M  \
                   : (LOCAL_MEM_SIZE <= 0x10000000)  ? _MMU_TLB_SZ_256M \
                   : (LOCAL_MEM_SIZE <= 0x40000000)  ? _MMU_TLB_SZ_1G   : -1)

    #if ( (WRLOAD_IMAGE_BUILD_PHYS_BIAS >> 32) != LOCAL_MEM_ERPN )
        #error WRLOAD_IMAGE_BUILD_PHYS_BIAS not matched to LOCAL_MEM_ERPN
    #endif

    #if (PHYS_MEM_START != 0x0)
        #error sysStaticTlbDesc[] assumes PHYS_MEM_START = 0
    #endif

    #if (LOCAL_MEM_SIZE > 0x80000000)
        #error sysStaticTlbDesc[] requires additional TLB entries.
    #endif

    #if (  (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE \
         + (WRLOAD_IMAGE_BUILD_PHYS_BIAS & 0x0FFFFFFFF)) \
         > PHYS_MEM_SIZE)
        #error Your image footprint exceeds available memory.
    #endif
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr,  realAddrExt,  realAddr, ts | size | attributes | permissions */

#ifndef UBOOT
    /* TLB #0.  Flash */

    /* needed be first entry here */

    {
    FLASH_BASE_ADRS, 0x0, FLASH_BASE_ADRS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_X
    },
#endif

    /*
     * The RAM execution area must be the 2nd entry because romInit uses the 2nd
     * TLB for this area.  When this TLB is written it replaces the romInit one.
     * You can't overlap TLBs.  You must have a TLB to use an area.
     */

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
    {
    LOCAL_MEM_LOCAL_ADRS,  PHYS_BIAS_HI,
    LOCAL_MEM_LOCAL_ADRS + PHYS_BIAS_LO,
    _MMU_TLB_TS_0   |  RAM_TLB_SZ         | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | CAM_DRAM_CACHE_MODE | _MMU_TLB_ATTR_M     |
                      _MMU_TLB_PERM_X
    },
#else
    {
    LOCAL_MEM_LOCAL_ADRS, LOCAL_MEM_ERPN, LOCAL_MEM_LOCAL_ADRS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G      | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | CAM_DRAM_CACHE_MODE | _MMU_TLB_ATTR_M     |
                      _MMU_TLB_PERM_X
    },
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

    /* WR Hypervisor expects this area in the 3rd TLB entry */
    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0     | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W   | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G
    },

    /*
     * The 0 area must be the 4th entry because romInit uses the 4th.  This
     * is required by sysToMonitor for CPUn_SPACE_BASE_ADRS.
     */

#if (LOCAL_MEM_LOCAL_ADRS != 0x0)
    {
    PHYS_MEM_START, 0, PHYS_MEM_START,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M  | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE |
    _MMU_TLB_ATTR_M
    },
#endif

#if (LOCAL_MEM_SIZE > 0x40000000)
    {
    LOCAL_MEM_LOCAL_ADRS + 0x40000000, LOCAL_MEM_ERPN,
    LOCAL_MEM_LOCAL_ADRS + 0x40000000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G    | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_M |
                       _MMU_TLB_PERM_X
    },
#endif /* (LOCAL_MEM_SIZE > 0x40000000) */

#ifdef INCLUDE_VIRTUAL_SM
    { VIRTUAL_SM_BASE, 0, VIRTUAL_SM_BASE_PHYS,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT |
      _MMU_TLB_PERM_W |CAM_DRAM_CACHE_MODE| _MMU_TLB_ATTR_M|
                       _MMU_TLB_PERM_X
    },
#endif /* INCLUDE_VIRTUAL_SM */

#ifdef INCLUDE_LBC_CS3 /* PIXIS */
    { LBC_CS3_LOCAL_ADRS, 0x0, LBC_CS3_LOCAL_ADRS,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_4K    | _MMU_TLB_IPROT  |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
      _MMU_TLB_ATTR_G
    },
#endif /* PIXIS */

    /* All these are not protected */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_1     | _MMU_TLB_SZ_16M   | PROTECTED |
    _MMU_TLB_PERM_W   | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G
    },

    { DCSR_LAW_BASE, 0x0, DCSR_LAW_BASE,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_4M    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
      _MMU_TLB_ATTR_G
    }

#ifdef DRV_RESOURCE_QORIQBMAN
    ,
    { BMAN_LAW_BASE, 0x0, BMAN_LAW_BASE,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | PROTECTED |
      _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_M
    },
    { BMAN_LAW_BASE + 0x100000, 0x0, BMAN_LAW_BASE + 0x100000,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
      _MMU_TLB_ATTR_G
    }
#endif

#ifdef DRV_RESOURCE_QORIQQMAN
    ,
    { QMAN_LAW_BASE, 0x0, QMAN_LAW_BASE,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | PROTECTED      |
      _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_M
    },
    { QMAN_LAW_BASE + 0x100000, 0x0, QMAN_LAW_BASE + 0x100000,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
      _MMU_TLB_ATTR_G
    }
#endif

#ifdef INCLUDE_PCI_BUS
    ,
    { PCIEX1_LAW_BASE, 0x0, PCIEX1_LAW_BASE,
      _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
      _MMU_TLB_ATTR_G
    }

    #ifdef SECONDARY_PCIE_BRIDGE
    ,
    { PCIEX2_LAW_BASE, 0x0, PCIEX2_LAW_BASE,
      _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
      _MMU_TLB_ATTR_G
    }
    #endif
    ,
    { PCIEX3_LAW_BASE, 0x0, PCIEX3_LAW_BASE,
      _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
      _MMU_TLB_ATTR_G
    }
#endif  /* INCLUDE_PCI_BUS */
};

/*
 * Warning, if sysStaticTlbDesc uses too many entries, mmuOptimize() may
 * fail to add a static entry for the sysPhysMemDesc table.
 */

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#ifdef MMU_ASID_MAX     /* Base 6 MMU library in effect */
  /* macro to compose 64-bit PHYS_ADDRs */
# define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))
#endif

/*
* sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
* used by the MMU to translate addresses with single page (4k) granularity.
* PTE memory space should not, in general, overlap BAT memory space but
* may be allowed if only Data or Instruction access is mapped via BAT.
*
* Address translations for local RAM, memory mapped PCI bus, the Board Control and
* Status registers, the MPC8260 Internal Memory Map, and local FLASH RAM are set here.
*
* PTEs are held in a Page Table.  Page Table sizes are
* integer powers of two based on amount of memory to be mapped and a
* minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
* for 32-bit PowerPCs are:
*
* Total mapped memory           Page Table size
* -------------------           ---------------
*        8 Meg                       64 K
*       16 Meg                      128 K
*       32 Meg                      256 K
*       64 Meg                      512 K
*      128 Meg                        1 Meg
*       .                               .
*       .                               .
*       .                               .
*
* [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
*
*/
PHYS_MEM_DESC sysPhysMemDesc [] =
{
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + (PHYS_ADDR) WRLOAD_IMAGE_BUILD_PHYS_BIAS,
#ifdef LOCAL_MEM_SIZE
        LOCAL_MEM_SIZE,
#else
        DEFAULT_LOCAL_MEM_SIZE,
#endif
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY

    }

/* grant access to CPUn bootstrap words for SMP M-N */
#if defined (_WRS_CONFIG_SMP) && defined (INCLUDE_AMP) && !defined (INCLUDE_AMP_CPU_00)
    ,
    {
        (VIRT_ADDR) (CPUn_SPACE_BASE_ADRS - (CPUn_SPACE_SIZE * MAX_CPUS)),
        (PHYS_ADDR) (CPUn_SPACE_BASE_ADRS - (CPUn_SPACE_SIZE * MAX_CPUS)),
        (CPUn_SPACE_SIZE * MAX_CPUS),
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    }
#endif /* _WRS_CONFIG_SMP */

    /* To load a secondary w/out the WRLOAD_CPU_MEM_MAP_FUNC defined */

#if defined (INCLUDE_WRLOAD) && !defined (INCLUDE_WRLOAD_IMAGE_BUILD)  \
                             && !defined (WRLOAD_CPU_MEM_MAP_FUNC)
    ,
    {
    #ifdef LOCAL_MEM_SIZE
        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
        (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
        PHYS_MEM_SIZE - (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE),
    #else
        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS + DEFAULT_LOCAL_MEM_SIZE,
        (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + DEFAULT_LOCAL_MEM_SIZE,
        PHYS_MEM_SIZE - (LOCAL_MEM_LOCAL_ADRS + DEFAULT_LOCAL_MEM_SIZE),
    #endif
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE           |
    VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_WRLOAD && ... */

#if defined (INCLUDE_MIPC_SM) \
    && ((LOCAL_MEM_LOCAL_ADRS+LOCAL_MEM_SIZE) < MIPC_SM_SYSTEM_POOL_BASE)
    ,
    {
    (VIRT_ADDR) MIPC_SM_SYSTEM_POOL_BASE,
    (PHYS_ADDR) MIPC_SM_SYSTEM_POOL_BASE_PHYS,
    MIPC_SM_SYSTEM_POOL_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE           |
    VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_MIPC_SM */

#ifdef INCLUDE_LBC_CS3
    ,
    {
        (VIRT_ADDR) LBC_CS3_LOCAL_ADRS,
        (PHYS_ADDR) LBC_CS3_LOCAL_ADRS,
        LBC_CS3_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_LBC_CS3 */

#ifdef INCLUDE_PCI_BUS
    ,
    {
        (VIRT_ADDR) PCIEX1_LAW_BASE,
        (PHYS_ADDR) PCIEX1_LAW_BASE,
        PCIEX1_LAW_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY_NOT
    }
    ,

#ifdef SECONDARY_PCIE_BRIDGE
    {
        (VIRT_ADDR) PCIEX2_LAW_BASE,
        (PHYS_ADDR) PCIEX2_LAW_BASE,
        PCIEX2_LAW_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY_NOT
    }
    ,
#endif

    {
        (VIRT_ADDR) PCIEX3_LAW_BASE,
        (PHYS_ADDR) PCIEX3_LAW_BASE,
        PCIEX3_LAW_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY_NOT
    }

#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_FLASH
    ,
    {
        (VIRT_ADDR) FLASH_BASE_ADRS,
        (PHYS_ADDR) FLASH_BASE_ADRS,
        MAIN_FLASH_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED
    }
#endif /* INCLUDE_FLASH */

#ifdef DRV_RESOURCE_QORIQBMAN
    ,
    /*
     * The buffer manager access area is 2MB wide, with the
     * first 1MB set as cacheable and the second 1MB set
     * as cache inhibited.
     *
     * The internally used buffer is 1MB, with the same
     * attributes as regular system memory. It is only
     * accessed by software _once_ when the DRAM is
     * cleared before the memory is dedicated to the BMan.
     */
    {
        (VIRT_ADDR) BMAN_LAW_BASE,
        (PHYS_ADDR) BMAN_LAW_BASE,
        0x100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) BMAN_LAW_BASE + 0x100000,
        (PHYS_ADDR) BMAN_LAW_BASE + 0x100000,
        0x100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | VM_STATE_GUARDED
    }
    ,
    {
        (VIRT_ADDR) BMAN_MEM_BASE,
        (PHYS_ADDR) BMAN_MEM_BASE,
        0x100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    }
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
    ,
    /*
     * The queue manager access area is 2MB wide, with the
     * first 1MB set as cacheable and the second 1MB set
     * as cache inhibited.
     *
     * The internally used buffer is 2MB, with the same
     * attributes as regular system memory. It is only
     * accesed by software _once_ when the DRAM is
     * cleared before the memory is dedicated to the QMan.
     */
    {
        (VIRT_ADDR) QMAN_LAW_BASE,
        (PHYS_ADDR) QMAN_LAW_BASE,
        0x100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) QMAN_LAW_BASE + 0x100000,
        (PHYS_ADDR) QMAN_LAW_BASE + 0x100000,
        0x100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | VM_STATE_GUARDED
    }
    ,
    {
        (VIRT_ADDR) QMAN_MEM_BASE,
        (PHYS_ADDR) QMAN_MEM_BASE,
        0x200000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    }
#endif /* DRV_RESOURCE_QORIQQMAN */

    ,
    {
        (VIRT_ADDR) DCSR_LAW_BASE,
        (PHYS_ADDR) DCSR_LAW_BASE,
        0x400000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | VM_STATE_GUARDED
    }
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* Clock Ratio Tables */
#define MAX_VALUE_DDR_RATIO 32
UINT32 ddrRatioTable[MAX_VALUE_DDR_RATIO] =
    {0,1,0,0,0,5,6,0,8,9,10,0,12,13,0,0,16,0,18,0,20,0,0,0,24,0,0,0,0,0,0,0};

#define MAX_VALUE_PLAT_RATIO 32
UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO] =
    {0,0,0,0,0,5,6,7,8,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define MAX_VALUE_CC_RATIO 32
UINT32 ccRatioTable[MAX_VALUE_CC_RATIO] =
    {0,0,0,0,0,0,0,0,8,9,10,11,12,0,14,15,16,0,18,0,20,0,22,0,0,0,0,0,0,0,0,0};

#define MAX_VALUE_CCS_CLOCK 16
#define MAX_VALUE_CCS_DIVS  2
UINT32 ccs1RatioTable[MAX_VALUE_CCS_CLOCK][MAX_VALUE_CCS_DIVS] = {
    {1, 1},
    {1, 2},
    {0, 0},
    {0, 0},
    {2, 1},
    {2, 2},
    {0, 0},
    {0, 0},
    {3, 1},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}
};

#define MAX_VALUE_CCS_CLOCK 16
#define MAX_VALUE_CCS_DIVS  2
UINT32 ccs2RatioTable[MAX_VALUE_CCS_CLOCK][MAX_VALUE_CCS_DIVS] = {
    {1, 1},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {3, 1},
    {3, 2},
    {0, 0},
    {0, 0},
    {4, 1},
    {4, 2},
    {0, 0},
    {0, 0}
};

int   sysBus      = BUS_TYPE_NONE;      /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (PPC8260) */
char *sysBootLine = BOOT_LINE_ADRS; /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;   /* catastrophic message area */
#ifdef INCLUDE_AMP
int   sysProcNum = AMP_CPU_NUMBER;  /* processor number of this CPU */
#else
int   sysProcNum = 0;           /* processor number of this CPU */
#endif
BOOL  sysVmeEnable = FALSE;     /* by default no VME */

#ifdef INCLUDE_SHOW_ROUTINES
UINT32  coreFreq[8];
UINT32  ddrFreq;
UINT32  systemFreq;
UINT32  multiSysToCCB;
UINT32  multiSysToDDR;
UINT32  multiSysToCC[4];
IMPORT UINT32 sysCpcRegGet(UINT32);
#endif

#ifdef INCLUDE_P4080_FMAN_UCODE
IMPORT UINT8 _binary_fsl_fman_ucode_p4080_92_2_bin_start[];
IMPORT UINT8 _binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_start[];
IMPORT UINT8 _binary_fsl_fman_ucode_P4080_106_2_4_bin_start[];
#endif /* INCLUDE_P4080_FMAN_UCODE */

#ifdef INCLUDE_AMP
UINT32 sysAmpCpuSet = 0;
#endif

#ifdef _WRS_CONFIG_SMP
#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;      /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;      /* lower 32 bit value of timebase */
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
#endif /* _WRS_CONFIG_SMP */

IMPORT char     _wrs_kernel_data_start[];
IMPORT char *	cachePpcReadOrigin;
IMPORT void     mmuE500TlbDynamicInvalidate();
IMPORT void     mmuE500TlbStaticInvalidate();
IMPORT void     mmuE500TlbStaticInit (int numDescs,
                                  TLB_ENTRY_DESC *pTlbDesc,
                                  BOOL cacheAllow);
IMPORT BOOL     mmuPpcIEnabled;
IMPORT BOOL     mmuPpcDEnabled;
IMPORT void     sysIvprSet(UINT32);

IMPORT UINT32   sysCpuIdGet();

#ifdef INCLUDE_LOGICAL_PARTITION
IMPORT void     sysLpidSet(UINT32);
#endif
IMPORT void     sysSelfReset(void);
IMPORT void     sysBoardReset(void);

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void     vxEnableBP();
IMPORT void     vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#undef INCLUDE_CPU_LIGHT_PWR_MGR /* XXX Fix w/ arch power mgmt */
#ifdef INCLUDE_CPU_LIGHT_PWR_MGR
IMPORT VOIDFUNCPTR      _func_sysDoze;
IMPORT void             sysDoze();
IMPORT void             sysDozeCpu(UINT32);
#endif

#ifdef _WRS_CONFIG_SMP
IMPORT void vxPirSet (UINT32);
#endif

#ifdef FSL_ERRATA_FPU_A003999
IMPORT void sysFslErrataFpuA003999 (void);
#endif /* FSL_ERRATA_FPU_A003999 */

#if defined(FSL_ERRATA_CCF_A004510) || defined(FSL_ERRATA_CCF_A004511)
IMPORT void sysFslErrataCcfA004510AndA004511 (void);
#endif /* FSL_ERRATA_CCF_A004510 || FSL_ERRATA_CCF_A004511 */

/* forward declarations */
void sysCpusetReset (phys_cpuset_t);

void sysMsDelay (UINT);
void sysUsDelay (int);

IMPORT void vxSdaInit (void);

/* 8260 Reset Configuration Table (From page 9-2 in Rev0 of 8260 book) */
#define END_OF_TABLE 0

LOCAL UINT32 sysClkFreq;
UINT32 sysClkFreqGet(void);
UINT32 ppcE500ICACHE_LINE_NUM = (64 * 12);
UINT32 ppcE500DCACHE_LINE_NUM = (64 * 12);

UINT32 ppcE500CACHE_ALIGN_SIZE = 64;

#ifdef _WRS_CONFIG_SMP
LOCAL void sysCpu1ShutdownLoop (void);
#if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))
STATUS sysSmpTimeBaseSync (void);
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_SYS_TIMESTAMP */
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_CACHE_ERROR_HANDLER
#   include "sysCacheErrorLib.c"
#endif  /* INCLUDE_CACHE_ERROR_HANDLER */

#ifdef INCLUDE_I2C
#include "sysMotI2c.c"
#include "sysMpc85xxI2c.c"
#endif /* INCLUDE_I2C */

#ifdef INCLUDE_FLASH
    #include <mem/flashMem.c>
    #include <mem/nvRamToFlash.c>
#else
    #include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */

UINT32 inFullVxWorksImage=FALSE;

UINT32   baudRateGenClk;

IMPORT void sysSerialConnectAll (void);
#ifdef _WRS_CONFIG_SMP
LOCAL   UINT32 physCpuAvailableGet (void);
#endif

#include "sysCache.c"
IMPORT UINT32 vxL2CFG0Get (void);
#if defined (INCLUDE_L2_CACHE_ERROR_RECOVERY) && defined (INCLUDE_L2_CACHE)
IMPORT void vxL2ERRINTENSet(UINT32);
IMPORT void vxL2ERRCTLSet(UINT32);
#endif  /* INCLUDE_L2_CACHE_ERROR_RECOVERY && INCLUDE_L2_CACHE*/

#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheAimE500FlushAllD (void);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
#endif  /* _WRS_CONFIG_SMP */
IMPORT STATUS cacheAimE500DisableDFromMmu (void);
IMPORT STATUS cacheAimE500DisableIFromMmu (void);


#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */

#define WB_MAX_IRQS 256

/* defines */

#define ZERO    0

#define SYS_MODEL         "Freescale QorIQ P4080"
#define SYS_MODEL_P4080   SYS_MODEL

#define SYS_MODEL_E500    "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN "Freescale Unknown processor"


#define DELTA(a,b)                 (abs((int)a - (int)b))
#define HID0_MCP 0x80000000

#ifdef INCLUDE_VXBUS
#include "hwconf.c"
IMPORT device_method_t * pSysPlbMethods;

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);
#ifdef INCLUDE_AMP
METHOD_DECL(sysBspDevFilter);
STATUS sysDeviceFilter(VXB_DEVICE_ID pDev);
#endif

#ifdef INCLUDE_AMP
LOCAL UINT64 sysVirtToPhys (void *);
LOCAL void * sysPhysToVirt (UINT64);
#endif

LOCAL UINT32 sysFmanClkFreqGet (void);

DEVMETHOD_DEF(sysVirtToPhys, "BSP virtual to physical address translation");
DEVMETHOD_DEF(sysPhysToVirt, "BSP physical to virtual address translation");
DEVMETHOD_DEF(sysFmanClkFreqGet, "Get Fman clock frequency");

LOCAL struct vxbDeviceMethod p4080PlbMethods[] =
    {
#ifdef INCLUDE_AMP
        /* Only used for AMP filtering per core for now so not built for
         * anything else
         */
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),

    /* Only used for AMP images running above 32-bit boundary. */

    DEVMETHOD(sysVirtToPhys, sysVirtToPhys),
    DEVMETHOD(sysPhysToVirt, sysPhysToVirt),
#endif
    DEVMETHOD(sysFmanClkFreqGet, sysFmanClkFreqGet),
    { 0, 0 }
    };
#endif

#ifdef INCLUDE_NETWORK
#include "sysNet.c"
#endif

#ifdef INCLUDE_SM_COMMON
#include "sysSmEnd.c"
#endif

#ifdef DRV_SPI_S25FLXX
#include "sysFslSpi.c"
#include "sysSpiFlash.c"
#endif  /* DRV_SPI_S25FLXX */

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL void sysL1CacheQuery();
#endif

IMPORT  UINT    sysTimeBaseLGet(void);

#ifdef SERDES_FIX_CPU
#include "p4080SerdesErrata.c" /* For sysSerdesFix() */
#endif

LOCAL char * physTop = NULL;
LOCAL char * memTop = NULL;

#ifdef INCLUDE_AMP

typedef struct {
        char* deviceName;
        int   unit;
        int   cpu;
} AMP_CPU_TABLE;

/* Table includes only devices we want to filter
 * if no action required for a device per CPU then it's not in the table
 */
AMP_CPU_TABLE ampCpuTable[] = {
#ifdef INCLUDE_ETSEC_VXB_END
    { XTSEC_NAME, 0, XTSEC0_CORE_NUM },
    { XTSEC_NAME, 1, XTSEC1_CORE_NUM },
    { XTSEC_NAME, 2, XTSEC2_CORE_NUM },
    { XTSEC_NAME, 3, XTSEC3_CORE_NUM },
#endif
#ifdef INCLUDE_MSD
    { "ns16550", 0, 0 },
    { "ns16550", 1, 0 },
    { "ns16550", 2, 0 },
    { "ns16550", 3, 0 },
#endif
    { "m85xxCCSR", 0, 0 },
    { "openPicTimer", 0,  0 },
    { "openPicTimer", 1,  0 },
    { "QorIQPciEx", 0, 0 },
    { "QorIQPciEx", 1, 0 },
    { "QorIQPciEx", 2, 0 }
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

STATUS sysDeviceFilter
    (
    VXB_DEVICE_ID pDev
    )
    {
    /* Compare devices name and unit number with those you want to remove
     * at run-time. Used here to selectively use devices on one cpu vs another.
     */
    int deviceIndex;
    BOOL deviceMatch;
#ifdef INCLUDE_VXCPULIB
    cpuset_t cpuset;
    phys_cpuset_t physCpuset;

    CPUSET_SETALL(cpuset);
    cpusetLogicalToPhys (cpuset, &physCpuset);
#endif /* INCLUDE_VXCPULIB */

    deviceMatch = FALSE;

    for (deviceIndex = 0 ; deviceIndex < NELEMENTS(ampCpuTable); deviceIndex++)
        {
        if (pDev->pName == NULL)
            continue;
        if ((strcmp (pDev->pName,ampCpuTable[deviceIndex].deviceName) == OK) &&
            (pDev->unitNumber == ampCpuTable[deviceIndex].unit))
            {
            deviceMatch = TRUE;
            break;
            }
        }

    /* return OK if no match device - assume pass through */
    if (deviceMatch == FALSE)
        return (OK);

    /* if device match then we need to decide whether to filter */

#ifdef INCLUDE_VXCPULIB
    if (!PHYS_CPUSET_ISSET(physCpuset, ampCpuTable[deviceIndex].cpu))
#else
    if (ampCpuTable[deviceIndex].cpu != sysCpuIdGet())
#endif
        return(ERROR);

    return(OK);
    }

#endif /* INCLUDE_AMP */

#if defined(INCLUDE_WRLOAD_IMAGE_BUILD) || defined(UBOOT)
/* Not sure if UBOOT actually needs this */

#define BOOT_LINE_LEN 0x100
char    wrloadBootLine[BOOT_LINE_LEN] = DEFAULT_BOOT_LINE;
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

/*****************************************************************************
*
* sysBusTas - Test and Set.
*
* RETURNS: STATUS
*
* /NOMANUAL
*/

STATUS sysBusTas
    (
    char *adrs
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
* /NOMANUAL
*/

void sysBusClear
    (
    char *adrs
    )
    {
    *adrs=0;
    _WRS_ASM("isync;sync;eieio");
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

/*******************************************************************************
*
* sysBusIntAck - Acknowledge bus interrupt
*
* This routine acknowledges bus interrupts
*
* RETURNS: 0
*
* \NOMANUAL
*/

int sysBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge */
    )
    {
    return (0);
    }

/****************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*
* ERRNO: N/A
*/

char * sysModel (void)
    {
    UINT32 device;
    char* retChar = NULL;

    device = *P4080_SVR(CCSBAR);

    switch(device & 0xffff0000)
        {
        case 0x82800000:
        case 0x82080000:
            retChar = SYS_MODEL_P4080;
            break;
        default:
            retChar = SYS_MODEL_E500;
            break;
        }


    device = *P4080_PVR(CCSBAR);

    if ((device & 0xfff00000) != 0x80200000)
        retChar =SYS_MODEL_UNKNOWN;


    return(retChar);

    }

/******************************************************************************
*
* sysBspRev - return the BSP version with the revision eg 1.0/<x>
*
* This function returns a pointer to a BSP version with the revision.
* for eg. 1.0/<x>. BSP_REV defined in config.h is concatenated to
* BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO: N/A
*/

char * sysBspRev (void)
    {
    return(BSP_VERSION BSP_REV);
    }

/******************************************************************************
*
* sysClkFreqGet - returns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet
    (
    void
    )
    {
#ifndef FORCE_DEFAULT_FREQ
    UINT32 platRatio;
    UINT32 ddrRatio;
    UINT32 ccRatio[4];
    int n;
    UINT32 oscFreq;
    UINT8  osc;
#endif /* FORCE_DEFAULT_FREQ */

    if (sysClkFreq != 0)
        return (sysClkFreq);

#ifdef FORCE_DEFAULT_FREQ
    sysClkFreq = DEFAULT_SYSCLKFREQ;
#else /* FORCE_DEFAULT_FREQ */

    platRatio = P4080_SYS_PLL_RAT(CCSBAR);

    if (   (platRatio >= NELEMENTS(platRatioTable))
        || (platRatioTable[platRatio] == 0)
       )
        {
        /* A default value better than zero or -1 */
        sysClkFreq = DEFAULT_SYSCLKFREQ;
        return (sysClkFreq);
        }

#ifdef INCLUDE_SHOW_ROUTINES
    multiSysToCCB = platRatioTable[platRatio];
#endif

    ddrRatio = P4080_MEM_PLL_RAT(CCSBAR);

#ifdef INCLUDE_SHOW_ROUTINES
    multiSysToDDR = ddrRatioTable[ddrRatio];
#endif

    for (n = 0; n < 4; n++)
        {
        ccRatio[n] = P4080_CCN_PLL_RAT(CCSBAR,n);
#ifdef INCLUDE_SHOW_ROUTINES
        multiSysToCC[n] = ccRatio[n];
#endif
        }

    osc = *PX_SPD(LBC_CS3_LOCAL_ADRS) & 0x7;

    switch (osc)
        {
        case PX_SYSCLK_66_MHZ:
            oscFreq = FREQ_66_MHZ;
            break;
        case PX_SYSCLK_75_MHZ:
            oscFreq = FREQ_75_MHZ;
            break;
        case PX_SYSCLK_83_MHZ:
            oscFreq = FREQ_83_MHZ;
            break;
        case PX_SYSCLK_90_MHZ:
            oscFreq = FREQ_90_MHZ;
            break;
        case PX_SYSCLK_100_MHZ:
            oscFreq = FREQ_100_MHZ;
            break;
        case PX_SYSCLK_111_MHZ:
            oscFreq = FREQ_111_MHZ;
            break;
        case PX_SYSCLK_125_MHZ:
            oscFreq = FREQ_125_MHZ;
            break;
        case PX_SYSCLK_133_MHZ:
            oscFreq = FREQ_133_MHZ;
            break;
        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    sysClkFreq = ((UINT32)(oscFreq * platRatioTable[platRatio])) / 2;

#ifdef INCLUDE_SHOW_ROUTINES
    systemFreq = oscFreq;

    /* 1:1 is a special case */
    if (ddrRatio == 1)
        ddrFreq = sysClkFreq;
    else
        ddrFreq = oscFreq * ddrRatioTable[ddrRatio] / 2;

    for (n = 0; n < 4; n++)
        {
        UINT32 ccPllSel = P4080_CN_PLL_SEL(CCSBAR,n);
        UINT32 cc = ccs1RatioTable[ccPllSel][0];
        UINT32 div = ccs1RatioTable[ccPllSel][1];
        UINT32 coreRatio = ccRatio[cc - 1];
        coreFreq[n] = (oscFreq * coreRatio) / div;
        }

    for (n = 4; n < 8; n++)
        {
        UINT32 ccPllSel = P4080_CN_PLL_SEL(CCSBAR,n);
        UINT32 cc = ccs2RatioTable[ccPllSel][0];
        UINT32 div = ccs2RatioTable[ccPllSel][1];
        UINT32 coreRatio = ccRatio[cc - 1];
        coreFreq[n] = (oscFreq * coreRatio) / div;
        }

#endif /* INCLUDE_SHOW_ROUTINES */

#endif /* FORCE_DEFAULT_FREQ */

    return(sysClkFreq);
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

#ifdef DRV_STORAGE_SDHC

/*******************************************************************************
*
* sysSdhcClkFreqGet - get eSDHC controller clock frequency
*
* This routine returns the clock frequency of eSDHC controller. 
* 
* RETURNS: clock frequency of eSDHC controller
*
* ERRNO: N/A
*
* /NOMANUAL
*/

UINT32 sysSdhcClkFreqGet (void)
    {
    return (sysClkFreqGet () / 2);
    }

#endif /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_I2C

/*******************************************************************************
*
* ds1339I2cRead - perform interface's I2C read for DS1339RTC
*
* This routine performs I2C read for DS1339RTC.
*
* RETURNS: data read from I2C
*
* ERRNO: N/A
*/

unsigned char ds1339I2cRead
    (
    unsigned char addr,
    unsigned char reg
    )
    {
    char val;

    i2cRead (I2C_RTC_BUS, addr, I2C_DEVICE_TYPE_RTC_DS1374,
             (unsigned int)reg, 1, &val);

    return ((unsigned char)val);
    }

/*******************************************************************************
*
* ds1339I2cWrite - perform interface's I2C write for DS1339RTC
*
* This routine performs I2C write for DS1339RTC.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ds1339I2cWrite
    (
    unsigned char addr,
    unsigned char reg,
    unsigned char val
    )
    {
    i2cWrite (I2C_RTC_BUS, addr, I2C_DEVICE_TYPE_RTC_DS1374,
              (unsigned int)reg, 1, (char *)&val);
    }

#endif /* INCLUDE_I2C */

/******************************************************************************
*
* sysHwMemInit - initialize and configure system memory.
*
* This routine is called before sysHwInit(). It performs memory auto-sizing
* and updates the system's physical regions table, `sysPhysRgnTbl'. It may
* include the code to do runtime configuration of extra memory controllers.
*
* NOTE: This routine should not be called directly by the user application.  It
* cannot be used to initialize interrupt vectors.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwMemInit (void)
    {
    /* Call sysPhysMemTop() to do memory autosizing if available */

    sysPhysMemTop ();

    }

/******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the P4080DS board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit (void)
    {
#if defined (_WRS_CONFIG_SMP)
    int i;
#endif
    UINT32 chipRev;
#ifdef INCLUDE_P4080_FMAN_UCODE
    int x, y;
    HCF_DEVICE * pDev;
    struct hcfResource * pRes;
    void * pUcode = NULL;
#endif /* INCLUDE_P4080_FMAN_UCODE */

    vxIvprSet((UINT32)VEC_BASE_ADRS);
    
    chipRev = (vxSvrGet() & 0x000000F0) >> 4;
    if (chipRev == 2) /* fixed in silicon 3.0 */
       {
        /*
         * Errata: A-004510 (Transactions can be lost in the CoreNet coherency
         * fabric). This has been fixed in Rev 3.0 silicon.
         *
         * In Rev 2.0 silicon, to avoid the behavior Freescale recommends the
         * following software workaround:
         *
         * Workaround: Specific settings in the CCF, CPC and the core are required
         * to reconfigure the CoreNet coherency fabric.
         */

         /*
          * Errata: A-004511 (Transactions may be incorrectly dropped from the wait
          * queue in the CoreNet coherency fabric). This has been fixed in Rev 3.0
          * silicon.
          *
          * In Rev 2.0 silicon, to avoid the behavior Freescale recommends the
          * following software workaround:
          *
          * Workaround: Disable the Wait Queue logic by performing a read-modify-write
          * to clear bit 7 of the register at DCSRBAR + 0x000B_0008.
          */

          sysFslErrataCcfA004510AndA004511();
       }
     
#ifdef _WRS_CONFIG_SMP
        /* Here we initialize cpu1 init address so we don't wait until the
         * correct value is initialized
         */
    for (i = 1; i < VX_SMP_NUM_CPUS; i++)
        *(UINT32*)CPUn_INIT_START_ADR(CPU_LOGICAL_TO_PHYS(i)) = MP_MAGIC_RBV;
#endif

#ifdef FSL_ERRATA_FPU_A003999
    /*
     * Errata: A-003999 (Running Floating Point instructions requires special
     * initialization).
     * Workaround: Set bit 7 to a one in SPR977 before executing any floating
     * point arithmetic operation and Alternatively, the bit can be set once
     * at boot time and never cleared.
     */

    sysFslErrataFpuA003999();
#endif /* FSL_ERRATA_FPU_A003999 */

#ifdef FSL_ERRATA_FPU_A003999
    /*
     * Errata: A-003999 (Running Floating Point instructions requires special
     * initialization).
     * Workaround: Set bit 7 to a one in SPR977 before executing any floating
     * point arithmetic operation and Alternatively, the bit can be set once
     * at boot time and never cleared.
     */

    sysFslErrataFpuA003999();
#endif /* FSL_ERRATA_FPU_A003999 */

    /* Disable L1 Icache */
    vxL1CSR1Set(vxL1CSR1Get() & ~0x1);

    /* Put cachePpcReadOrigin at the start of data section */
    cachePpcReadOrigin = (char *) &_wrs_kernel_data_start;

    /* Check for architecture support for 36 bit physical addressing */
#if defined(PPC_e500v2) || defined (PPC_e500mc)
    vxHid0Set(_PPC_HID0_MAS7EN|vxHid0Get());
#endif
    /* Enable machine check pin */
    vxHid0Set(HID0_MCP|vxHid0Get());

#ifdef INCLUDE_L1_CACHE_ERROR_RECOVERY
    /* Enable Parity in L1 caches, set data cache write shadow mode */
    vxL1CSR0Set(vxL1CSR0Get() | _PPC_L1CSR_CPE);
    vxL1CSR1Set(vxL1CSR1Get() | _PPC_L1CSR_CPE);
    vxL1CSR2Set(vxL1CSR2Get() | _PPC_L1CSR2_DCWS);
#endif  /* INCLUDE_L1_CACHE_ERROR_RECOVERY */

#if defined (INCLUDE_L2_CACHE_ERROR_RECOVERY) && defined (INCLUDE_L2_CACHE)
    vxL2ERRINTENSet(L2_CACHE_ERROR_INT_ENABLE);
    vxL2ERRCTLSet(L2_CACHE_ERROR_L2CTHRESH << 16);
#endif  /* INCLUDE_L2_CACHE_ERROR_RECOVERY && INCLUDE_L2_CACHE*/

/* allow only the CPU0 image to initialize the L3 cache */
#if !defined(INCLUDE_AMP) || defined(INCLUDE_AMP_CPU_00)
#if defined (INCLUDE_L3_CACHE_ERROR_RECOVERY) && defined (INCLUDE_L3_CACHE)
    sysCpcRegSet(CPCERRINTEN(CCSBAR+CPC1_BASE_ADDR), L3_CACHE_ERROR_INT_ENABLE);
    sysCpcRegSet(CPCERRINTEN(CCSBAR+CPC2_BASE_ADDR), L3_CACHE_ERROR_INT_ENABLE);
    sysCpcRegSet(CPCERRCTL(CCSBAR+CPC1_BASE_ADDR),L3_CACHE_ERROR_CPCTHRESH<<16);
    sysCpcRegSet(CPCERRCTL(CCSBAR+CPC2_BASE_ADDR),L3_CACHE_ERROR_CPCTHRESH<<16);
#endif  /* INCLUDE_L2_CACHE_ERROR_RECOVERY && INCLUDE_L2_CACHE*/
#endif /* !INCLUDE_AMP || INCLUDE_AMP_CPU_00 */

    /* enable time base for delay use before DEC interrupt is setup */
    vxDecSet(0xffffffff);
    vxDecarSet(0xffffffff);

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif /* INCLUDE_CACHE_SUPPORT */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
# if (defined(INCLUDE_AMP) && !defined(INCLUDE_AMP_CPU_00))
    /*
     * If AMP and non physical Core0, L1 cache is disabled but L2 cache may
     * be enabled here.  Make sure, L2 cache is invalidated.
     * (L1 cache is invalidated by bootrom.)
     */

    sysL2CacheInvFunc ();
# endif	/* INCLUDE_AMP && !INCLUDE_AMP_CPU_00 */

    /* Make sure L2 cache is off prior to turning on cacheing below */

    sysL2CacheDisable (0);
#endif

    /* Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
#ifdef INCLUDE_LOGICAL_PARTITION
    sysLpidSet(LOGICAL_PARTITION_ID);
#endif
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (!defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL))
    mmuPpcIEnabled=TRUE;
    mmuPpcDEnabled=TRUE;
#else /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */
    if (inFullVxWorksImage==FALSE)
        {
        mmuPpcIEnabled=TRUE;
        mmuPpcDEnabled=TRUE;
        }
    /* Enable I Cache if instruction mmu disabled */
#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    mmuPpcIEnabled=TRUE;
#endif /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */
#if (SW_MMU_ENABLE == TRUE)

        mmuPpcIEnabled=TRUE;
        mmuPpcDEnabled=TRUE;

#endif
#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2CacheInit();
#endif /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

#if (defined(INCLUDE_CACHE_SUPPORT) && defined(INCLUDE_L3_CACHE))
/* allow only the CPU0 image to enable the L3 cache */
#if (!defined(INCLUDE_AMP) || defined(INCLUDE_AMP_CPU_00))
    sysL3CacheInit();
#endif /* !INCLUDE_AMP || INCLUDE_AMP_CPU_00 */
#endif /* INCLUDE_CACHE_SUPPORT && INCLUDE_L3_CACHE */

    /* Initialize the system bus clock values; must be after TLB static init */

    sysClkFreq = 0;
    sysClkFreqGet();

    /* Machine check via RXFE for RIO */

#ifdef INCLUDE_BRANCH_PREDICTION
    
    /* Workaround for Freescale A-004529 */
    
    #ifdef FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE
        {
        if (chipRev != 2) /* fixed in silicon 3.0 */
            {
            vxEnableBP();
            }
        else
            {
            #ifndef INCLUDE_HW_FP
            vxEnableBP();
            #else
            vxDisableBP();
            #endif /* INCLUDE_HW_FP */
            }
        }
    #else /* FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE */

        vxEnableBP();

    #endif /* FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE */

#else /* INCLUDE_BRANCH_PREDICTION */

    vxDisableBP();

#endif /* INCLUDE_BRANCH_PREDICTION */

    _WRS_ASM("isync");

#ifdef INCLUDE_P4080_FMAN_UCODE
    /*
     * If this is rev 2 silicon, switch to the
     * rev 2 fman microcode patch. If this is some
     * other silicon rev (3 or higher ?) for which
     * we don't have a corresponding patch, don't
     * load any firmware at all.
     */
   
    switch (chipRev)
        {
        case 1:
            /* Install rev 1 patch */
            pUcode = _binary_fsl_fman_ucode_p4080_92_2_bin_start;
            break;
        case 2:
            /* Install rev 2 patch */
            pUcode = _binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_start;
            break;
         case 3:
            /* Install rev 3 patch */
            pUcode = _binary_fsl_fman_ucode_P4080_106_2_4_bin_start;
            break;
        default:
            /* Unknown CPU rev -- leave microcode disabled */
            pUcode = NULL;
            break;
        }

    for (x = 0; x < hcfDeviceNum; x++)
        {
        pDev = &hcfDeviceList[x];
        if (strcmp (pDev->devName, "QorIQFman") == 0)
            {
            pRes = (struct hcfResource *)pDev->pResource;
            for (y = 0; y < pDev->count; y++)
                {
                if (strcmp (pRes[y].name, "ucodeAddr") == 0)
                    {
                    pRes[y].u.addr = pUcode;
                    break;
                    }
                }
            }
        }

#endif /* INCLUDE_P4080_FMAN_UCODE */

    sysHwErrorString[0] = '\0';

#ifdef SERDES_FIX_CPU
    if (vxCpuPhysIndexGet() == SERDES_FIX_CPU)
        {
        sysSerdesFix();
        }
#endif

#ifdef INCLUDE_VXBUS
    pSysPlbMethods = p4080PlbMethods;
    hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_I2C
    i2cDrvInit (0, 0);
    i2cDrvInit (1, 0);
#endif

#ifdef INCLUDE_CACHE_ERROR_HANDLER
    vxIvor1Set(_EXC_OFF_L1_PARITY);
    sysCacheErrorHandlerInstall();
#endif  /* INCLUDE_CACHE_ERROR_HANDLER */

    CACHE_PIPE_FLUSH();

#if defined (INCLUDE_CPU_LIGHT_PWR_MGR) && !defined (SIMICS)
    /* install sysDoze */
    _func_sysDoze = sysDoze;
#endif
    }

/**************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* RETURNS: The address of the top of physical memory.
*
* ERRNO: N/A
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {

    if (physTop == NULL)
        {
#ifdef LOCAL_MEM_SIZE
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
#else
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + DEFAULT_LOCAL_MEM_SIZE);
#endif
        }

    return(physTop) ;
    }

/***************************************************************************
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
*
* ERRNO: N/A
*/

char * sysMemTop (void)
    {

    if (memTop == NULL)
        {
        memTop = sysPhysMemTop ();

        memTop = memTop - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM

        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif

        }

    return memTop;
    }

/**************************************************************************
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
* When rebooting an SMP system, it is likely that all CPUs' data caches,
* and especially the L2 caches if enabled, contain modified lines which
* have not been written out to SDRAM (although, due to hardware-enforced
* coherency, at most one CPU's cache contains a modified copy of any one
* cache line).  To ensure that critical diagnostic data -- such as kernel
* stacks, the ED&R buffer, and the coredump buffer -- are preserved, we
* must ensure that all CPUs flush their caches before the system reboots.
* This requires a CPC (cross processor call). Depending on the nature of
* the cause for shutdown, the system may be unstable such that the CPC's
* will be unsuccessful. As an added precaution to save as much state as
* possible before resetting, we perform a cache flush prior to sending
* any CPC.
*
* \cs
*      sysHwInit
* +--------------------------------+
* |  clear flush "DONE" magic num  |
* +--------------------------------+
*
*                                   +- - - - - - - - - - - - - - - - - - - - -+
* sysToMonitor()                    |                                         |
*   starts on any CPU               v
* +-----------------------------------------------------------------------+   |
* |                             current CPU ?                             |
* |                  0                  |                n                |   |
* +-------------------------------------+---------------------------------+
* |   CPC to CPU n: sysCpu1ShutdownLoop |   CPC to CPU 0:  sysToMonitor   |- -+
* +--------------------------|----------+---------------------------------+
*   |
*   |                        |
*   |                        + - - - - - - - +
*   |                                        |       sysCpu1ShutdownLoop()
*   v    always on CPU 0                     v         always on CPU n's
* +---------------------------------+   +---------------------------------+
* |          intCpuLock ()          |   |          intCpuLock ()          |
* +---------------------------------+   +---------------------------------+
* |  wait for CPUn flush or timeout |   |          flush d-cache          |
* +---------------------------------+   +---------------------------------+
* |       disable D & I cache       |   |   write flush "DONE" magic num  |
* +---------------------------------+   +---------------------------------+
* |   misc resetting of processor   |   |               nap               |
* |      registers and state        |   +---------------------------------+
* +---------------------------------+
* |   put CPUn's into hard reset    |
* +---------------------------------+
* |         jump to bootrom         |
* +---------------------------------+
* \ce
*/

STATUS sysToMonitor
    (
    int startType   /* parameter passed to ROM to tell it how to boot */
    )
    {
#if !defined(INCLUDE_AMP_CPU_00)
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);
#endif	/* !INCLUDE_AMP_CPU_00 */

#if (defined (INCLUDE_AMP) || defined (_WRS_CONFIG_SMP))
    cpuset_t cpuset;
    int i, thisPhysCpu, countSmpCpus, countCacheFlushed;
#endif	/* INCLUDE_AMP || _WRS_CONFIG_SMP */

    int basePhysCpu = CPU_LOGICAL_TO_PHYS(0);
    phys_cpuset_t physCpuset;
    PHYS_CPUSET_ZERO(physCpuset);

#ifdef _WRS_CONFIG_SMP

    /*
     * Make sure we cannot migrate, but without locking
     * interrupts -- see cpcInvoke() requirements.
     */

    if (!INT_CONTEXT() && !KERNEL_ENTERED_ME())
        (void) taskCpuLock();

    /*
     * In SMP mode, the last part of sysToMonitor() must run on core 0
     * because it will jump to the bootrom.
     *
     * If we happen to already be on core 0, we first cause CPU 1 to n to clear
     * their caches and enter nap mode (or a parking loop) after which CPU 0
     * can proceed with shutdown.  If we are on a core n, we will proceed
     * to the sysCpu1ShutdownLoop after arranging for sysToMonitor() to
     * be reinvoked on core 0.
     */

    if (vxCpuIndexGet() == 0)
        {
        CPUSET_ZERO (cpuset);      /* implicitly sends CPC to all except self */

        cpcInvoke (cpuset, (CPC_FUNC)sysCpu1ShutdownLoop, NULL, 0,
                   VX_CPC_ASYNC);
        }
    else
        {
        CPUSET_ZERO (cpuset);
        CPUSET_SET  (cpuset, 0);     /* send CPC to CPU 0 only */

        cpcInvoke (cpuset, (CPC_FUNC)sysToMonitor, (void *)startType,
                   sizeof(startType), VX_CPC_ASYNC);

        FOREVER;
        }

    (void) intCpuLock ();	/* CPC done, now safe to disable interrupts */

    /* Only CPU 0 gets here,
     *   but not sure if it is physical core 0 on an AMP system
     *   or merely logical core 0 on an SMP M-N system
     */

    thisPhysCpu = vxCpuPhysIndexGet();

    i =  50;            /* 0.5sec (50 * 0.01) max wait for CPUn */
    do  {
        sysMsDelay(10);

        countSmpCpus = VX_SMP_NUM_CPUS - 1;
        countCacheFlushed = 0;

        do  {
             if (*(UINT32*)CPUn_INIT_START_ADR(thisPhysCpu + countSmpCpus) ==
                                 MP_MAGIC_CACHE_FLUSHED)
                  {
                  countCacheFlushed++;
                  }
            } while (--countSmpCpus > 0);

        if (countCacheFlushed == (VX_SMP_NUM_CPUS - 1))
            {
            break;
            }

        } while (--i > 0);
#else
    /* lock interrupts in this entire routine */

    (void) intCpuLock();
#endif  /* _WRS_CONFIG_SMP */

#ifdef	INCLUDE_CACHE_SUPPORT
#if defined(INCLUDE_L3_CACHE)
/* allow only the CPU0 image to disable the L3 cache */
#if (!defined(INCLUDE_AMP) || defined(INCLUDE_AMP_CPU_00))
    sysL3CacheFlushDisable();
#endif /* !INCLUDE_AMP || INCLUDE_AMP_CPU_00 */
#endif /* INCLUDE_L3_CACHE */

    /*
     * The cacheDisable() API is not supported in SMP, so call the AIM
     * flush primitives directly.
     */

    cacheAimE500DisableDFromMmu();
    cacheAimE500DisableIFromMmu();
#endif	/* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    /* set MSR to its power-up default value
     * disables ALL interrupts and exceptions, including debugging breakpoints
     */

    vxMsrSet(0);

    /* Clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    /*
     * Finally forcibly reset the secondary cores. This is done
     * using the processor reset register in the PIC.
     *
     * Note also that the bits in the EPIC Hard Reset Register are
     * not auto-clearing.
     */

#if (defined(_WRS_CONFIG_SMP) || defined(INCLUDE_AMP))
# if defined (INCLUDE_AMP_CPU_00)
    /* if we're physical core 0, then reset all cores */

    for (i = 1; i < MAX_CPUS; i++)
        PHYS_CPUSET_SET(physCpuset, i);
# else
    CPUSET_SETALL(cpuset);
    cpusetLogicalToPhys(cpuset, &physCpuset);
# endif	/* INCLUDE_AMP_CPU_00 */
#endif	/* _WRS_CONFIG_SMP || INCLUDE_AMP */

/* if we're logical 0 on an SMP or AMP system, reset our subordinate cores */

#if defined(_WRS_CONFIG_SMP) || defined(INCLUDE_AMP)
    for (i = 1; i < MAX_CPUS; i++)
        {
        if (!PHYS_CPUSET_ISSET(physCpuset, i))
            continue;

        *((volatile int *)CPUn_FUNC_START_ADR(i)) =
            (int)0; /* SMP specific */
        /* SMP specific not used just loaded */
        *((volatile int *)CPUn_STACK_START_ADR(i)) =
            (int)RAM_LOW_ADRS - FRAMEBASESZ;
        /* This is used if core waiting after core1 did asynchronous reboot */
        *((volatile int *)CPUn_INIT_START_ADR(i)) =
            (int)MP_MAGIC_RBV;
        *((volatile int *)CPUn_LOCAL_MEM_ERPN(i)) =
            (int)0;

        _WRS_ASM("sync");
        }

#    if defined(_WRS_CONFIG_SMP) || defined(INCLUDE_AMP_CPU_00)
    /* don't attempt to reset self! */

    PHYS_CPUSET_CLR(physCpuset, basePhysCpu);

    /* reset SMP subordinates or all CPUs except self in AMP */

    sysCpusetReset (physCpuset);
#    endif /* _WRS_CONFIG_SMP || INCLUDE_AMP_CPU_00 */
#endif /* _WRS_CONFIG_SMP || INCLUDE_AMP */

    /*
     * there are two possible ways out:
     *
     * 1. Traditional jump to boot monitor. (UP/SMP or AMP CPU1-X)
     * 2. Reset the board. (AMP CPU0)
     */

#if defined(INCLUDE_AMP_CPU_00)

    /* this triggers a board reset in the AMP */

    sysBoardReset();
#else

# if defined(INCLUDE_AMP)
    /* reset PIR core cluster ID prior to entering into bootrom */

    vxPirSet (PIR_CORE_CLUSTER_ID_SET(basePhysCpu));

    /* For AMP CPU1-X, we use "warm boot" to wait on bootrom to start. */

# endif	/* INCLUDE_AMP */

    (*pRom) (startType);

#endif	/* INCLUDE_AMP_CPU_00 */

    return(OK);    /* in case we ever continue from ROM monitor */
    }

volatile unsigned int sysCpu1LoopCount = 0;

#if defined(_WRS_CONFIG_SMP)

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT void excIvorInit();
IMPORT void mmuPpcPidSet(UINT32);
IMPORT void sysCacheEnable();

#ifdef INCLUDE_VIRTUAL_SM
    IMPORT void sysVirtualSmTlb();
#endif

/******************************************************************************
*
* sysCpuInit - initialize additional core
*
* RETURNS: NONE
*
*/

void sysCpuInit
    (
    unsigned int physCpuIndex
    )
    {

    phys_cpuset_t physCpuBit;
    
    /* write logical CPU */
    vxPirSet(CPU_PHYS_TO_LOGICAL(physCpuIndex));

#ifdef INCLUDE_VIRTUAL_SM
    sysVirtualSmTlb();
#endif

    vxIvprSet((UINT32)VEC_BASE_ADRS);
    excIvorInit();

#if defined(PPC_e500v2) || defined(PPC_e500mc)
    vxHid0Set(_PPC_HID0_MAS7EN|vxHid0Get());
#endif

    /* Enable machine check pin */
    vxHid0Set(HID0_MCP|vxHid0Get());

#ifdef FSL_ERRATA_FPU_A003999
    /*
     * Errata: A-003999 (Running Floating Point instructions requires special
     * initialization).
     * Workaround: Set bit 7 to a one in SPR977 before executing any floating
     * point arithmetic operation and Alternatively, the bit can be set once
     * at boot time and never cleared.
     */

    sysFslErrataFpuA003999();
#endif /* FSL_ERRATA_FPU_A003999 */

#ifdef INCLUDE_L1_CACHE_ERROR_RECOVERY
    /* Enable Parity in L1 caches, set data cache write shadow mode */
    vxL1CSR0Set(vxL1CSR0Get() | _PPC_L1CSR_CPE);
    vxL1CSR1Set(vxL1CSR1Get() | _PPC_L1CSR_CPE);
    vxL1CSR2Set(vxL1CSR2Get() | _PPC_L1CSR2_DCWS);
#endif  /* INCLUDE_L1_CACHE_ERROR_RECOVERY */

#ifdef INCLUDE_L2_CACHE
#ifdef INCLUDE_L2_CACHE_ERROR_RECOVERY
    vxL2ERRINTENSet(L2_CACHE_ERROR_INT_ENABLE);
    vxL2ERRCTLSet(L2_CACHE_ERROR_L2CTHRESH << 16);
#endif /* INCLUDE_L2_CACHE_ERROR_RECOVERY */

    sysL2CacheInit();
#endif /* INCLUDE_L2_CACHE */

#ifdef INCLUDE_CACHE_ERROR_HANDLER
    vxIvor1Set(_EXC_OFF_L1_PARITY);
#endif  /* INCLUDE_CACHE_ERROR_HANDLER */


    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

#ifdef INCLUDE_LOGICAL_PARTITION
    sysLpidSet(LOGICAL_PARTITION_ID);
#endif

    sysCpu1LoopCount=2;

    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    sysCpu1LoopCount=3;

    if(_pSysL2CacheInvFunc!=NULL)
        _pSysL2CacheInvFunc();

    sysCacheEnable();

    if(_pSysL2CacheEnable!=NULL)
        _pSysL2CacheEnable();

#ifdef INCLUDE_BRANCH_PREDICTION

    #ifdef FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE
        {
        UINT32 chipRev;
        chipRev = (vxSvrGet() & 0x000000F0) >> 4;
        if (chipRev != 2) /* fixed in silicon 3.0 */
            {
            vxEnableBP();
            }
        else
            {
            #ifndef INCLUDE_HW_FP
            vxEnableBP();
            #else
            vxDisableBP();
            #endif /* INCLUDE_HW_FP */
            }
        }
    #else /* FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE */

        vxEnableBP();

    #endif /* FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE */

#else /* INCLUDE_BRANCH_PREDICTION */

    vxDisableBP();

#endif /* INCLUDE_BRANCH_PREDICTION */

    mmuPpcPidSet(MMU_ASID_GLOBAL);

#if (SW_MMU_ENABLE == FALSE)
    vxMsrSet(vxMsrGet() | _PPC_MSR_IS | _PPC_MSR_DS);
#endif

    /* enable timebase */
   
    PHYS_CPUSET_SET(physCpuBit, physCpuIndex);
    *P4080_CTBENR(CCSBAR) |= physCpuBit;

#if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))
    /*
     * Set the current timebase read from CPU0 onto CPU1.
     * The timebase is kept halt till it's re-started by the CPU0.
     */

    sysSmpTimeBaseSet (tbHI, tbLO);
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    sysCpu1LoopCount=4;

    }

/******************************************************************************
*
* sysCpuLoop - CPU loop
*
* This routine is the CPU idle loop function which will invoke the startup
* function specified in the sysCpuEnable function.
*
* RETURNS: NONE
*
*/

void sysCpuLoop(void)
    {
    unsigned int a = 0;
    FUNCPTR p;
    int cpu;
    /*
     * The call to vxSdaInit() must be the first operation here.
     * This is because vxSdaInit() sets the r2 and r13 registers to the
     * SDA and SDA2 base registers. No C code must be placed before this
     * call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in sysCpuLoop() */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

    /* have to use sysCpuIdGet() here because PIR is not set */
    cpu = sysCpuIdGet();
    p = (FUNCPTR)(*(UINT32*)CPUn_FUNC_START_ADR(cpu));

    sysCpuInit(cpu);

    sysCpu1LoopCount=0x10;

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
    unsigned int      physCpuIndex,     /* physical cpu index */
    WIND_CPU_STATE  * pCpuState
    )
    {
    STATUS status = OK;
    phys_cpuset_t physCpuBit;
    volatile char * temp;
#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    cpuset_t    cpuset;
    phys_cpuset_t physCpuset;
    UINT32      i = 0;
    UINT32      timeout;
    int         key;
#endif

    PHYS_CPUSET_ZERO(physCpuBit);

    /* make sure physCpuIndex is requesting an available CPU other than 0 */

    if ((physCpuIndex < 1) || (physCpuIndex > (sysCpuAvailableGet () - 1)))
        {
        return ERROR;
        }

    temp = malloc (L3_CACHE_SIZE);

    if (temp == NULL)
        {
        return ERROR;
        }

    *((int *)CPUn_FUNC_START_ADR(physCpuIndex)) = (int)pCpuState->regs.pc;
    *((int *)CPUn_STACK_START_ADR(physCpuIndex)) = (int)pCpuState->regs.spReg;
    *((int *)CPUn_INIT_START_ADR(physCpuIndex)) = (int)sysCpuLoop;
    *((int *)CPUn_LOCAL_MEM_ERPN(physCpuIndex)) = PHYS_BIAS_HI;
    *((int *)CPUn_LOCAL_MEM_RPN(physCpuIndex))  = PHYS_BIAS_LO;

#ifdef INCLUDE_CACHE_SUPPORT
    cacheFlush(DATA_CACHE, CPUn_INIT_START_ADR(physCpuIndex),
        (CPUn_LOCAL_MEM_RPN(physCpuIndex)-CPUn_INIT_START_ADR(physCpuIndex)));
#endif

    PHYS_CPUSET_SET(physCpuBit, physCpuIndex);

    sysCpusetReset (physCpuBit);

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    {
#ifndef VX_ENABLE_CPU_TIMEOUT
#define VX_ENABLE_CPU_TIMEOUT   5       /* default timeout five seconds */
#endif  /* VX_ENABLE_CPU_TIMEOUT */

    /*
     * PPC Dec timer will not be available while syncronizing timebase
     * between two cores.
     * Use magic number, 0x9000000, to count 1 second independently
     * on PPC Dec timer.
     */

    timeout = 0x9000000 * VX_ENABLE_CPU_TIMEOUT;

    /*
     * lock interrupts while Dec Timer is halt to prevent from lock-up
     * in the ISR which performs a timer delay.
     */

    key = intCpuLock ();

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
      * Save the current timebase on CPU0 into static variable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * syncronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif

    /*
     * Flush Data cache so as to make sure other cores have updated cache.
     * The reason why don't depend on hardware cache coherency itself here is
     * that other cores are still not actived and L1 & L2 caches are still off
     * state, So hardware cache coherency is not effective for this situation.
     * In AIM layer no explicit L2 & L3 cache flush API provided, so here using
     * memset to entire cache size as a replacement.
     */

    memset((char*)temp, 0x0, L3_CACHE_SIZE);
    _WRS_ASM("sync");

    /* Set BRR to make core released for booting */

    *P4080_BRR(CCSBAR) |= physCpuBit;
    _WRS_ASM("sync;isync");

    /* TBEN should be set by now */

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    /*
     * usrCpuEnable() or kernelCpuEnable() relies on the timer to count timeout.
     * If the system clock is PPC Dec Timer, it can not be used while timebase
     * is disabled.
     * Use a magic numer for busy loop to count timeout here.
     */

    if ((CPU_PHYS_TO_LOGICAL(physCpuIndex)) == (VX_SMP_NUM_CPUS - 1))
        {
        CPUSET_SETALL(cpuset);

        /* wait till all subordinate CPUs ready */

        while (vxCpuEnabledGet () != cpuset)
            {
            if (++i > timeout)
                {
                status = ERROR;
                break;
                }
            }

        cpusetLogicalToPhys (cpuset, &physCpuset);
        *P4080_CTBENR(CCSBAR) |= physCpuset;

        sysSmpTimeBaseSync();

        }

    intCpuUnlock(key);
    }

#endif /* DRV_TIMER_M85XX && INCLUDE_SYS_TIMESTAMP */

    free ((char *)temp);

    return status;
    }

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
/******************************************************************************
*
* sysCpcTimeBaseSet - CPC receiver for sysSmpTimeBaseInit
*
* This function receives the CPC from sysSmpTimeBaseInit,
* and passes the received parameters to sysSmpTimeBaseSet.
*
*/

LOCAL STATUS sysCpcTimeBaseSet
    (
    void        *ptr,   /* ptr to CPC parameters */
    UINT        size    /* size of CPC parameters */
    )
    {
    UINT32      tbHI = *(UINT32 *)ptr;
    UINT32      tbLO = ((UINT32 *)ptr)[1];

    sysSmpTimeBaseSet(tbHI, tbLO);
    return OK;
    }


/******************************************************************************
*
* sysSmpTimeBaseInit - Initialize synchronized timebases
*
* This function sets both CPUs' timebase registers to a common value
* while holding their common disable signal asserted, then enables them
* simultaneously so that their values are synchronized.  It should be called
* late in initialization, on one processor, after all processors are running.
*
* This function is required on all PPC SMP platforms using the timebase for
* timestamping, but may be a no-op on those platforms where synchronization
* is established by other means.
*
* INTERNAL
* For future reference:  DEVDISR is internal to the MPC8641D chip.  Platforms
* using multiple MPC8641D's (i.e. more than two CPUs) cannot achieve perfect
* synchronization; the best that can be done is to deassert the multiple
* DEVDISR[TB] signals as near simultaneously as possible.  On such a platform,
* the |= operation below should be replicated, specifying the appropriate
* base address to access each MPC8641D's CCSR space; and the &= operation
* below should be replaced with a call to an assembly-language function
* which writes the multiple DEVDISR registers using consecutive machine
* instructions.
*
* RETURNS: OK or ERROR
*
*/

LOCAL STATUS sysSmpTimeBaseInit
    (
    UINT32      tbHI,
    UINT32      tbLO
    )
    {
    cpuset_t    cpcInvokeCpuSet;
    UINT32      cpcParam[2];
    int         key;

    CPUSET_ZERO (cpcInvokeCpuSet);      /* => all but self */
    cpcParam[0] = tbHI;
    cpcParam[1] = tbLO;

    /* lock interrupts for a critical section */

    key = intCpuLock ();

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the syncronization between two cores gets
     * completed.
     */

    /*
     * Arrange for each processor to set its TB to the requested value,
     * and set the TBEN bit in its HID0.  Counting will not actually
     * start until the common disable is removed.
     */
    cpcInvoke (cpcInvokeCpuSet, (CPC_FUNC)sysCpcTimeBaseSet,
                (void *)cpcParam, sizeof(cpcParam), VX_CPC_SYNC);

    /* set own TB, since CPC broadcast does not include self */
    sysSmpTimeBaseSet(tbHI, tbLO);

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the syncronization between two cores gets
     * completed.
     */

    intCpuUnlock (key);

    return OK;
    }

/******************************************************************************
*
* sysSmpTimeBaseSync - Synchronize timebases
*
* This function sets both CPUs' timebase registers to a common value
* while holding their common disable signal asserted, then enables them
* simultaneously so that their values are synchronized.  It should be called
* late in initialization, on one processor, after all processors are running.
*
* This function is required on all PPC SMP platforms using the timebase for
* timestamping, but may be a no-op on those platforms where synchronization
* is established by other means.
*
* RETURNS: OK or ERROR
*
*/

STATUS sysSmpTimeBaseSync (void)
    {
    UINT32 tbHi, tbLo;

    /* Synchronize timebases on all enabled CPUs. */

    vxTimeBaseGet (&tbHi, &tbLo);
    return (sysSmpTimeBaseInit (tbHi, tbLo));
    }
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

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

    return physCpuAvailableGet();
    }

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

#ifdef INCLUDE_VXBUS
    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuAvail), (void *)&avail);
#endif

    return (avail);
    }
#endif /* if defined(_WRS_CONFIG_SMP) */

/******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit2 (void)
    {

#ifdef  INCLUDE_VXBUS
    vxbDevInit();
#endif /* INCLUDE_VXBUS */

    /* initialize serial interrupts */

    sysSerialConnectAll();

#ifdef  INCLUDE_VXBUS
    taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */

#ifdef DRV_SPI_S25FLXX
    sysSpiDrvInit(0, FREQ_33_MHZ/2);
#endif /* DRV_SPI_S25FLXX */

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif

    /* log warnings/errors from sysHwInit(), see hwErrStringEnd */
    if (strlen(sysHwErrorString) > 0)
        logMsg("WARNING: %s\n", sysHwErrorString,0,0,0,0,0);
    }

/******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board.
*
* ERRNO: N/A
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return(sysProcNum);
    }

/******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int     procNum         /* processor number */
    )
    {
    sysProcNum = procNum;
    }

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
* ERRNO: N/A
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
* ERRNO: N/A
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
* sysUsDelay - delay at least the specified amount of time (in microseconds)
*
* This routine will delay for at least the specified amount of time using the
* lower 32 bit "word" of the Time Base register as the timer.
*
* NOTE:  This routine will not relinquish the CPU; it is meant to perform a
* busy loop delay.  The minimum delay that this routine will provide is
* approximately 10 microseconds.  The maximum delay is approximately the
* size of UINT32; however, there is no roll-over compensation for the total
* delay time, so it is necessary to back off two times the system tick rate
* from the maximum.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysUsDelay
    (
    int    delay        /* length of time in microsec to delay */
    )
    {
    register UINT baselineTickCount;
    register UINT curTickCount;
    register UINT terminalTickCount;
    register int actualRollover = 0;
    register int calcRollover = 0;
    UINT ticksToWait;
    UINT requestedDelay;
    UINT oneUsDelay;

    /* Exit if no delay count */

    if ((requestedDelay = delay) == 0)
        return;

    /*
     * Get the Time Base Lower register tick count, this will be used
     * as the baseline.
     */
    
    baselineTickCount = sysTimeBaseLGet();
   
    /*
     * Calculate number of ticks equal to 1 microsecond
     *
     * The Time Base register and the Decrementer count at the same rate:
     * once per 8 System Bus cycles.
     *
     * e.g., 199999999 cycles     1 tick      1 second         ~  25 ticks
     *       ----------------  *  ------   *  --------         =  --------
     *       second               8 cycles    1000000 microsec    microsec
     */

    /* add to round up before div to provide "at least" */
    oneUsDelay = (((sysClkFreq >> 3) + 1000000) / 1000000);

    /* Convert delay time into ticks */

    ticksToWait = requestedDelay * oneUsDelay;

    /* Compute when to stop */

    terminalTickCount = baselineTickCount + ticksToWait;

    /* Check for expected rollover */

    if (terminalTickCount < baselineTickCount)
        {
        calcRollover = 1;
        }

    do
        {

        /*
         * Get current Time Base Lower register count.
         * The Time Base counts UP from 0 to
         * all F's.
         */

        curTickCount = sysTimeBaseLGet();

        /* Check for actual rollover */

        if (curTickCount < baselineTickCount)
            {
            actualRollover = 1;
            }

        if (((curTickCount >= terminalTickCount)
             && (actualRollover == calcRollover)) ||
            ((curTickCount < terminalTickCount)
             && (actualRollover > calcRollover)))
            {

            /* Delay time met */

            break;
            }
        }
    while (TRUE); /* breaks above when delay time is met */
    }

/*********************************************************************
*
* sysMsDelay - 1ms delay increments.
*
* This routine consumes 1ms of time * delay.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    UINT      delay                   /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (int) delay * 1000 );
    }

/*********************************************************************
*
* sysDelay - Fixed 1ms delay.
*
* This routine consumes 1ms of time. It just calls sysMsDelay.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

#ifdef INCLUDE_CACHE_SUPPORT
/***********************************************************************
*
* sysL1CacheQuery - configure L1 cache size and alignment
*
* Populates L1 cache size and alignment from configuration registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL1CacheQuery(void)
    {
    UINT32 temp;
    UINT32 align;
    UINT32 cachesize;

    temp = vxL1CFG0Get();

    cachesize = (temp & 0xFF) << 10;

    align = (temp >> 23) & 0x3;


    switch (align)
        {
        case 0:
            ppcE500CACHE_ALIGN_SIZE=32;
            break;
        case 1:
            ppcE500CACHE_ALIGN_SIZE=64;
            break;
        default:
            ppcE500CACHE_ALIGN_SIZE=64;
            break;
        }

    ppcE500DCACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);
    ppcE500ICACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);

    /* The core manual suggests for a 64 byte cache line and 8 lines per set
       we actually need 12 unique address loads to flush the set.
       The number of lines to flush should be ( 3/2 * cache lines ) */
    ppcE500DCACHE_LINE_NUM = (3*ppcE500DCACHE_LINE_NUM)>>1;
    ppcE500ICACHE_LINE_NUM = (3*ppcE500ICACHE_LINE_NUM)>>1;

    }

#endif /* INCLUDE_CACHE_SUPPORT */

/***************************************************************************
*
* saveExcMsg - write exception message to save area for catastrophic error
*
* The message will be displayed upon rebooting with a bootrom.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void saveExcMsg
    (
    char *errorMsg
    )
    {
    strncpy ((char *)EXC_MSG_OFFSET, errorMsg, 0x100);
    }


void chipErrataCpu29Print(void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    }

/***************************************************************************
*
* sysGetPeripheralBase   - Provides CCSRBAR address fro security engine
*                          drivers.
*
* RETURNS: CCSRBAR value
*
* ERRNO: N/A
*/
UINT32 sysGetPeripheralBase()
    {
    return(CCSBAR);
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
        logMsg ("Error in sysIntConnect: Cannot access routine.\n",
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
        logMsg ("Error in sysIntConnect: vector = %d.\n",
                (int)vector,2,3,4,5,6);
        }

    return (tmpStat);
    }

#if defined (_GNU_TOOL)
void sysIntHandler (void)
    {
    }

void vxDecInt (void)
    {
    }

int excRtnTbl = 0;
#endif  /* _GNU_TOOL */

#ifdef INCLUDE_SHOW_ROUTINES

/***************************************************************************
*
* coreLbcShow - Show routine for local bus controller
*
* This routine shows the local bus controller registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreLbcShow(void)
    {
    VINT32 tmp, tmp2;

    tmp = * (VINT32 *) P4080_BR0(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR0(CCSBAR);
    printf("Local bus BR0 = 0x%x\tOR0 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_BR1(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR1(CCSBAR);
    printf("Local bus BR1 = 0x%x\tOR1 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_BR2(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR2(CCSBAR);
    printf("Local bus BR2 = 0x%x\tOR2 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_BR3(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR3(CCSBAR);
    printf("Local bus BR3 = 0x%x\tOR3 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_BR4(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR4(CCSBAR);
    printf("Local bus BR4 = 0x%x\tOR4 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_BR5(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR5(CCSBAR);
    printf("Local bus BR5 = 0x%x\tOR5 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_BR6(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR6(CCSBAR);
    printf("Local bus BR6 = 0x%x\tOR6 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_BR7(CCSBAR);
    tmp2 = * (VINT32 *) P4080_OR7(CCSBAR);
    printf("Local bus BR7 = 0x%x\tOR7 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) P4080_LBCR(CCSBAR);
    printf("Local bus LBCR = 0x%x\n", tmp);

    tmp = * (VINT32 *) P4080_LCRR(CCSBAR);
    printf("Local bus LCRR = 0x%x\n", tmp);
    }

    #define xbit0(x, n)    ((x & (1 << (31 - n))) >> (31 - n))  /* 0..31 */
    #define xbit32(x, n)   ((x & (1 << (63 - n))) >> (63 - n))  /* 32..63 */
    #define onoff0(x, n)   xbit0(x, n) ? "ON", "OFF"
    #define onoff32(x, n)  xbit32(x, n) ? "ON", "OFF"

/***************************************************************************
*
* coreShow - Show routine for core registers
*
* This routine shows the core registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreShow(void)
    {
    VUINT32 tmp, tmp2;
    int n;

    tmp = vxMsrGet();
    printf("MSR - 0x%x\n", tmp);
    printf("      GS-%x UCLE-%x CE-%x EE-%x PR-%x FP-%x ME-%x\n",
           xbit32(tmp,35), xbit32(tmp,37), xbit32(tmp,46),
           xbit32(tmp,48), xbit32(tmp,49), xbit32(tmp,50), xbit32(tmp,51));
    printf("      FE0-%x DE-%x FE1-%x IS-%x DS-%x PMM-%x RI-%x\n",
           xbit32(tmp,52), xbit32(tmp,54), xbit32(tmp,55), xbit32(tmp,58),
           xbit32(tmp,59), xbit32(tmp,61), xbit32(tmp,62));
    tmp = vxHid0Get();
    printf("HID0 = 0x%x\n", tmp);
    tmp = vxL1CSR0Get();
    printf("L1CSR0: cache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = vxL1CSR1Get();
    printf("L1CSR1: Icache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = vxL1CSR2Get();
    printf("L1CSR2: 0x%x\n", tmp);
    tmp = vxL1CFG0Get();
    tmp2 = vxL1CFG1Get();
    printf("L1CFG0 = 0x%x, L1CFG1 = 0x%x\n", tmp, tmp2);
#ifdef INCLUDE_L1_CACHE_ERROR_RECOVERY
    printf("L1 cache error count = %d\n", sysL1CacheErrorCount);
#endif /* INCLUDE_L1_CACHE_ERROR_RECOVERY */
    tmp = vxL2CFG0Get();
    printf("L2CFG0 - 0x%x\n", tmp);
    printf("        l2cteha-%x l2cdeha-%x l2cidpa-%x l2cbsize-%x\n",
           (xbit32(tmp,33)<<1)|xbit32(tmp,34),
           (xbit32(tmp,35)<<1)|xbit32(tmp,36), xbit32(tmp,37),
           (xbit32(tmp,38)<<2)|(xbit32(tmp,39)<<1)|xbit32(tmp,40));

    printf("        l2crepl-%x l2cla-%x l2cnway-%x l2csize-%x\n",
           (xbit32(tmp,41)<<1)|xbit32(tmp,42), xbit32(tmp,43),
           (tmp & 0x0007C000)>>14, tmp & 0x3fff);
    tmp = vxL2CSR0Get();
    printf("L2CSR0 - 0x%x\n", tmp);
    printf("        l2 is %s\n", tmp&0x80000000?"ON":"OFF");
    printf("        l2pe-%x l2wp-%x l2io-%x l2do-%x\n",
           xbit32(tmp,33),
           (xbit32(tmp,35)<<2)|(xbit32(tmp,36)<<1)|xbit32(tmp,37),
           xbit32(tmp,43), xbit32(tmp,47));
    printf("        l2rep-%x l2loa-%x l2lo-%x\n",
           (xbit32(tmp,50)<<1)|xbit32(tmp,51),
           xbit32(tmp,56), xbit32(tmp,58));
    tmp = *CPCCSR0(0);
    printf("CPCCSR0 (CPC1) - 0x%x\n", tmp);
    printf("        l3 is %s\n", tmp&0x80000000?"ON":"OFF");
    printf("        cpcpe-%x cpcfi-%x cpcwt-%x cpcrep-%x cpcfl-%x\n",
           xbit0(tmp,1), xbit0(tmp,10), xbit0(tmp,12),
           (xbit0(tmp,18)<<1)|xbit0(tmp,19), xbit0(tmp,20));
    printf("        cpclfc-%x cpcloa-%x cpclo-%x\n",
           xbit0(tmp,21), xbit0(tmp,24), xbit0(tmp,26));
    tmp = *CPCCSR0(1);
    printf("CPCCSR0 (CPC2) - 0x%x\n", tmp);
    printf("        l3 is %s\n", tmp&0x80000000?"ON":"OFF");
    printf("        cpcpe-%x cpcfi-%x cpcwt-%x cpcrep-%x cpcfl-%x\n",
           xbit0(tmp,1), xbit0(tmp,10), xbit0(tmp,12),
           (xbit0(tmp,18)<<1)|xbit0(tmp,19), xbit0(tmp,20));
    printf("        cpclfc-%x cpcloa-%x cpclo-%x\n",
           xbit0(tmp,21), xbit0(tmp,24), xbit0(tmp,26));
    printf("Sys-to-CCB multiplier: %d\n", multiSysToCCB);
    printf("Sys-to-DDR multiplier: %d\n", multiSysToDDR);
    for (n = 0; n < 4; n++)
        printf("Sys-to-CC%d multiplier: %d\n", n+1, multiSysToCC[n]);
    printf("System Freq = %3d Hz\n",systemFreq);
    /* CCB is divided by 2 for our "sysClkFreq" */
    printf("CCB Freq = %3d Hz\n",sysClkFreqGet() * 2);
    printf("DDR Freq = %3d Hz\n",ddrFreq);
    printf("Frame manager Freq: %3d Hz\n", sysFmanClkFreqGet ());
    for (n = 0; n < 8; n++)
        printf("Core CPU%d Freq = %3d Hz\n",n,coreFreq[n]);

    }

#endif

#ifdef INCLUDE_PCI_BUS

/* INT LINE TO IRQ assignment for p4080ds board. */

LOCAL UCHAR sysPci1IntRoute [NUM_PCIEX1_SLOTS][4] =
   {
        {
        PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL,
        PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL
        },
        {
        PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL,
        PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL
        },
        {
        PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL,
        PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL
        },
        {
        PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL,
        PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL
        }
   };

LOCAL UCHAR sysPci2IntRoute [NUM_PCIEX2_SLOTS][4] =
   {
        {
        PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL,
        PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL
        },
        {
        PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL,
        PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL
        },
        {
        PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL,
        PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL
        },
        {
        PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL,
        PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL
        }
   };

LOCAL UCHAR sysPci3IntRoute [NUM_PCIEX3_SLOTS][4] =
   {
        {
        PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL,
        PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL
        },
        {
        PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL,
        PCIEX3_XINT4_LVL, PCIEX3_XINT1_LVL
        },
        {
        PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL,
        PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL
        },
        {
        PCIEX3_XINT4_LVL, PCIEX3_XINT1_LVL,
        PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL
        }
   };

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UINT devVend                        /* deviceID/vendorID of device */
    )
    {

    /*
     * Host controller itself (device number is 0) won't be configured.
     */

    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return (OK); /* Autoconfigure all devices */
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

    if ((pin >= 1) && (pin < 5))
        tmpChar = sysPci1IntRoute [(pLoc->device) & 3][pin-1];

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

    if ((pin >= 1) && (pin < 5))
        tmpChar = sysPci2IntRoute [(pLoc->device) & 3][pin-1];

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci3AutoconfigIntrAssign - PCI 3 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci3AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UCHAR pin                           /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    if ((pin >= 1) && (pin < 5))
        tmpChar = sysPci3IntRoute [(pLoc->device) & 3][pin-1];

    return (tmpChar);
    }

#endif

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

#ifdef _WRS_CONFIG_SMP
/*******************************************************************************
*
* sysCpu1ShutdownLoop - parking loop for CPU != 0
*
* This routine is launched onto secondary cores during system reboot. It locks
* interrupts, flushes d-cache, and puts the core into a tight loop so that it
* doesn't interfere with the sysToMonitor() routine.
*
* RETURNS: N/A
*/

LOCAL void sysCpu1ShutdownLoop (void)
    {
    UINT32	l2Csr0Val;

    (void) intCpuLock();

    /*
     * Flush L1 data cache first.
     * This must be done before flushing L2 cache unless L2 cache is
     * flushed and disabled.
     *
     * The cacheFlush API is a no-op in an SMP system.
     * Instead, call the AIM primitive directly.
     */

    cacheAimE500FlushAllD ();

    l2Csr0Val = vxL2CSR0Get();

    /* flush L2 cache if enabled */

    if (l2Csr0Val & P4080_L2CSR0_L2E_MSK)
	{
	vxL2CSR0Set (l2Csr0Val | P4080_L2CSR0_L2FL_MSK);

	/* wait until complete */

	while (vxL2CSR0Get() & P4080_L2CSR0_L2FL_MSK);
	}

    /* Report to CPU0 this CPU's cache flush is complete */

    *(UINT32*)CPUn_INIT_START_ADR(vxCpuPhysIndexGet()) =
                                              MP_MAGIC_CACHE_FLUSHED;

    /* make sure the magic num is visible on CPU0 immediately */

    _WRS_ASM("sync");

    /*
     * Do not over write any persistent data, which will be accessed by
     * bootrom from, now on.  Data cache is still enabled and the cached data
     * will not be visible from bootrom.
     *
     * So long as core0 hard-resets the non-primary cores and bootrom
     * and VxWorks correctly invalidate and disable the L1 and L2
     * caches, it is fine to leave the data cache enabled here.
     *
     * Note: If you want to disable data cache, the stack used within this
     * function must be flushed unless this routine is written in assmbly
     * and free from stack usage.  There is no easy way to find from
     * where to where need to be flushed but if you want to flush entire
     * data cache, be careful not to copy-back the data to physical memory,
     * where Core0 might already modify directly with data cache off.
     */

    /* Finally idle this CPU */

#ifdef INCLUDE_CPU_LIGHT_PWR_MGR
    sysDoze();
    _WRS_ASM("sync");
#endif

    /* expect to lock in DOZE mode until HRESET */

    FOREVER;
    }
#endif /* _WRS_CONFIG_SMP */

#if defined (INCLUDE_WRLOAD)

/*****************************************************************************
*
* sysAmpCpuEnable - Starts core executing code at entryPt
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
    UINT32 cpuBit = CPU_BIT(cpu);

    /* Here if core already running then maybe be spinning
     * bootimage should then spin waiting for CPU1_INIT_START_ADR to be
     * non-zero
     */

    *((volatile int *)CPUn_FUNC_START_ADR(cpu)) = (int)0; /* SMP specific */

        /* SMP specific not used just loaded */

    *((volatile unsigned int *)CPUn_STACK_START_ADR(cpu))
            = ((unsigned int)  entryPt - 0x20);

        /* This is used if core waiting after core1 did asynchronous reboot */

    *((volatile int *)CPUn_INIT_START_ADR(cpu)) = (int)entryPt;
    *((volatile int *)CPUn_LOCAL_MEM_ERPN(cpu)) = usrWrloadBiasHighGet(cpu);
    *((volatile int *)CPUn_LOCAL_MEM_RPN(cpu))  = usrWrloadBiasLowGet (cpu);

#ifdef INCLUDE_CACHE_SUPPORT
    cacheFlush(DATA_CACHE, CPUn_INIT_START_ADR(cpu),
                 (CPUn_LOCAL_MEM_RPN(cpu)-CPUn_INIT_START_ADR(cpu)));
#endif

    /* If core is not already running then enable it */
    if((*P4080_BRR(CCSBAR) & cpuBit) == 0)
        {
        *P4080_CTBENR(CCSBAR) |= cpuBit;
        *P4080_BRR(CCSBAR) |= cpuBit;
        _WRS_ASM("sync;isync");
        }

    sysAmpCpuSet |= cpuBit;
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
* flags argument currently unused, expected to provide finer grain control
* in the future.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysAmpCpuPrep
    (
    UINT32  physCpuIndex,
    void *  flags
    )
    {
    int basePhysCpu = CPU_LOGICAL_TO_PHYS(0);
    phys_cpuset_t physCpuset;

    PHYS_CPUSET_ZERO(physCpuset);

#ifdef _WRS_CONFIG_SMP
    /* Cannot prep cpus in master cpuset  */
    if (physCpuIndex >= basePhysCpu &&
        physCpuIndex < vxCpuConfiguredGet() + basePhysCpu)
#else /* _WRS_CONFIG_SMP */
    /* Cannot prep cpu 0 */
    if (physCpuIndex == 0)
#endif /* _WRS_CONFIG_SMP */
        return ERROR;

    /* shutdown ints */
    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable),
                     (void *)physCpuIndex);

    *((volatile int *)CPUn_FUNC_START_ADR(physCpuIndex)) =
                     (int)0; /* SMP specific */

    /* SMP specific not used just loaded */
    *((volatile int *)CPUn_STACK_START_ADR(physCpuIndex)) =
                     (int)RAM_LOW_ADRS - FRAMEBASESZ;

    /* This is used if waiting after CPU did asynchronous reboot */
    *((volatile int *)CPUn_INIT_START_ADR(physCpuIndex)) =
                     (int)MP_MAGIC_RBV;

    *((volatile int *)CPUn_LOCAL_MEM_ERPN(physCpuIndex)) = (int)0;

    _WRS_ASM("sync");

    PHYS_CPUSET_SET(physCpuset, physCpuIndex);

    sysCpusetReset (physCpuset);

    return OK;
    }

#endif /* INCLUDE_WRLOAD  */

#ifdef INCLUDE_AMP

/*****************************************************************************
*
* sysVirtToPhys - perform a virtual to physical address translation
*
* This routine converts a virtual memory address to a 36-bit physical
* address. The 36-bit conversion is accomplished using the LOCAL_MEM_ERPN
* parameter, which specifies the upper 4 bits of addessing info. This is
* required for VxWorks AMP images that have been loaded at addresses beyond
* the 32-bit addressing limit.
*
* RETURNS: a 36-bit physical address
*
* ERRNO: N/A
*/

LOCAL UINT64 sysVirtToPhys
    (
    void * pVirt
    )
    {
    return (WRLOAD_IMAGE_BUILD_PHYS_BIAS + (UINT64)(pVirt));
    }

/*****************************************************************************
*
* sysPhysToVirt - perform a physical to virtual address translation
*
* This routine converts a 36-bit physical address to a virtual memory
* address. The 36-bit conversion is accomplished by masking off the upper
* 4 bits of addressing information, based on the assumption that VxWorks
* can only handle 32-bit pointers.
*
* Note that physical to virtual translations should normally be avoided as
* they can sometimes require a great deal of calculation to complete,
* however the design of the DTSEC and TGEC makes it difficult to get by
* without them.
*
* RETURNS: a 32-bit virtual address
*
* ERRNO: N/A
*/

LOCAL void * sysPhysToVirt
    (
    UINT64 pPhys
    )
    {
    UINT32 virt = (UINT32)(   (pPhys - WRLOAD_IMAGE_BUILD_PHYS_BIAS)
                            & 0xFFFFFFFFUL);
    return (void *)virt;
    }

#endif /* INCLUDE_AMP */

/*****************************************************************************
*
* sysFmanClkFreqGet - return the frame manager clock frequency
*
* This function calculates the frame manager clock frequency and
* returns it.
*
* The frame manager clock source is selectable via the reset control word.
* It can either be the platform clock frequency divided by two, or the
* core cluster 3 PLL frequency divided by two. For the P4080DS board,
* the RCW supplied by Freescale uses the platform clock frequency.
*
* RETURNS: frame manager clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 sysFmanClkFreqGet (void)
    {
#ifndef FORCE_DEFAULT_FREQ
    UINT8 osc;
    UINT32 oscFreq;
    UINT32 ccRatio;

    /* Read RCW bits 225 and 226 to determine fman clock source.
     * If they are clear, the fman uses the platform clock.
     * Otherwise, the fman uses the core cluster 3 PLL.
     */
    if ((*P4080_RCWSR(CCSBAR,7) & 0x60000000) == 0)
        return (sysClkFreqGet());

    /* Get the base oscillator frequency from the PIXIS. */
    osc = *PX_SPD(LBC_CS3_LOCAL_ADRS) & 0x7;

    switch (osc)
        {
        case PX_SYSCLK_66_MHZ:
            oscFreq = FREQ_66_MHZ;
            break;
        case PX_SYSCLK_75_MHZ:
            oscFreq = FREQ_75_MHZ;
            break;
        case PX_SYSCLK_83_MHZ:
            oscFreq = FREQ_83_MHZ;
            break;
        case PX_SYSCLK_90_MHZ:
            oscFreq = FREQ_90_MHZ;
            break;
        case PX_SYSCLK_100_MHZ:
            oscFreq = FREQ_100_MHZ;
            break;
        case PX_SYSCLK_111_MHZ:
            oscFreq = FREQ_111_MHZ;
            break;
        case PX_SYSCLK_125_MHZ:
            oscFreq = FREQ_125_MHZ;
            break;
        case PX_SYSCLK_133_MHZ:
            oscFreq = FREQ_133_MHZ;
            break;
        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    /* Now get the core cluster 3 PLL multiplier */

    ccRatio = P4080_CCN_PLL_RAT(CCSBAR, 2);

    /*
     * The Fman clock is core cluster PLL multiplier times
     * the core oscillator frequency, divided by two.
     */

    return ((ccRatio * oscFreq) / 2);
#else /* FORCE_DEFAULT_FREQ */
    return (DEFAULT_SYSCLKFREQ);
#endif /* FORCE_DEFAULT_FREQ */
    }

/*****************************************************************************
*
* sysCpusetReset - reset a CPU set
*
* This routine resets a set of CPUs
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysCpusetReset
    (
    phys_cpuset_t physCpuset
    )
    {
    int delay;
    int i, j;

    /* don't attempt to reset non-running CPUs */
    physCpuset &= *P4080_BRR(CCSBAR);

    /* mask off and end all interrupts */
    for (i = 1; i < MAX_CPUS; i++)
        {
        if (PHYS_CPUSET_ISSET(physCpuset, i))
            {
            /* writing 15 to current task priority register effectively masks
               all interrupts */
            *P4080_EPICCTPRn(CCSBAR, i) = EPIC_CTPR_MAX;

            /* send sufficient number of EOIs to end all in-service
               interrupts */
            for (j = 0; j < RESET_EOI_NUM; j++)
                *P4080_EPICEOIn(CCSBAR, i) = 0;
            }
        }

    /* stop timebase */
    *P4080_CTBENR(CCSBAR) &= ~physCpuset;

    _WRS_ASM("sync;isync");

#ifndef SIMICS
    /* put cores into nap mode */
    *P4080_CNAPCRL(CCSBAR) |= physCpuset;

    _WRS_ASM("sync;isync");

    /* reset cores using PIR */
    *P4080_EPICPIR(CCSBAR) |= physCpuset;
    _WRS_ASM("sync;isync");
    for(delay=0;delay<0x10000;delay++);
#endif /* SIMICS */

    /* clear boot release */
    *P4080_BRR(CCSBAR) &= ~physCpuset;
    _WRS_ASM("sync;isync");

#ifndef SIMICS
    /* clear PIR */
    *P4080_EPICPIR(CCSBAR) &= ~physCpuset;
    _WRS_ASM("sync;isync");
    for(delay=0;delay<0x10000;delay++);

    /* clear nap */
    *P4080_CNAPCRL(CCSBAR) &= ~physCpuset;
#endif /* SIMICS */


    _WRS_ASM("sync;isync");
    for(delay=0;delay<0x10000;delay++);

    }


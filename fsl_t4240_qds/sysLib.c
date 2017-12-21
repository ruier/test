/* sysLib.c - Freescale T4240QDS board system-dependent library */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02l,02may13,j_z  add SRIO memory map entry. (WIND00415153)
02k,28mar13,h_k  corrected masking fman clock selection in RCW.
02j,06feb13,x_s  un-registered L2&L3 hook functions for AMP.
02i,05feb13,wyt  add support for SATA
02h,29jan13,h_k  fixed duplicated static mapping when LOCAL_MEM_LOCAL_ADRS is
                 non-zero in SMP.
02g,28jan13,wyt  correct several base address macros and use the
                 right I/O access routines
02f,25jan13,wyt  using I/O access APIs.
02e,23jan13,h_k  removed an old comment.
02d,23jan13,wap  Make sure initialization of VSC3316 frontside crossbar
                 switches succeeds
02c,22jan13,h_k  cleaned up UBOOT macros.
02b,22jan13,x_s  Replaced warm boot address with macro ROM_WARM_ADRS.
02a,21jan13,x_z  removed nvRamToFlash.c and replaced INCLUDE_FLASH with 
                 DRV_VXB_NORFLASH.
01z,17jan13,wap  Add support for alternate SERDES1/SERDES2 layout
01y,17jan13,zl   simplified CPUn space entry in sysPhysMemDesc[].
01x,16jan13,wap  Fix include path for vxbVsc3316.h
01w,16jan13,wap  Update EMI configuration for new RCW. Update to latest Fman
                 microcode. Add support for initializing the Vitesse VSC3316
                 crossbar switch chips on SERDES1 and SERDES2
01v,11jan13,wyt  added global variables for usrMacAddr.c.
01u,06jan13,x_s  moved USB related initialization function from usbPciStub.c.
01t,12dec12,h_k  replaced deprecated QX_SPD with QX_CLK_SPD1.
                 cleaned up unused arrays.
01s,30nov12,x_s  remove e6500 core functions to usrCoreE6500.c.
01r,30nov12,syt  added support for physical USB host controller.
01q,21nov12,h_k  added workarounds for A-004786 and A-004806.
01p,13nov12,h_k  fixed thread1 initialization.
01o,09nov12,pcs  Set invalidate protection for CCSBAR when IS=DS=1
01n,08nov12,h_k  fxied build error at coreShow().
                 replaced INCLUDE_SHOW_ROUTINES with DEBUG and commented out
                 the show routines.
01m,05nov12,syt  added I2C support.
01l,05nov12,h_k  fixed software reboot failure in SMP on actual HW.
01k,23oct12,x_s  Remove L2/L3 cache code to ARCH.
01j,30oct12,h_k  updated coreLbcShow.
                 updated to support non-primary AMP to configure phys mem
                 above 4GB.
01i,18oct12,syt  Add support for PCIEX and SDHC.
01h,15oct12,wap  Add support for SGMII ethernet ports, make some additional
                 cleanups
01g,09oct12,syt  code clean up.
01f,28sep12,h_k  added UBOOT config.
01e,14sep12,h_k  removed TGEC.
01d,13sep12,h_k  fixed shared memory MIPS pool mapping
01c,05sep12,h_k  cleaned up L2 and L3 cache settings.
01b,13aug12,wap  Update Qman/Bman for T4240
01a,07may12,h_k  adapted from fsl_p5020_ds, version 01t.
*/

/*
DESCRIPTION
This library provides board-specific routines for t4240qds.

INCLUDE FILES: sysLib.h

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
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

#ifdef INCLUDE_T4240_FMAN_UCODE
#   include <hwif/vxbus/hwConf.h>
#   include <h/vxbus/vxbAccess.h>
#endif /* INCLUDE_T4240_FMAN_UCODE */

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_ALTIVEC
#   include <altivecLib.h>
IMPORT STATUS (* _func_altivecProbeRtn) (void) ;
#endif /* INCLUDE_ALTIVEC */

#ifdef DRV_I2C_VSC3316
#include <h/util/vxbVsc3316.h>
#endif

/* globals */

/* protect all TLB1 entries if in a Linux-VxWorks system. */

#define PROTECTED        0

#ifdef WRLOAD_IMAGE_BUILD_PHYS_BIAS
#   define PHYS_BIAS_HI_LO  WRLOAD_IMAGE_BUILD_PHYS_BIAS
#   define PHYS_BIAS_LO     (UINT32)(WRLOAD_IMAGE_BUILD_PHYS_BIAS)
#   define PHYS_BIAS_HI     (UINT32)((UINT64)WRLOAD_IMAGE_BUILD_PHYS_BIAS >> 32)
#else
#   define PHYS_BIAS_HI_LO  0
#   define PHYS_BIAS_LO     0
#   define PHYS_BIAS_HI     0
#endif /* WRLOAD_IMAGE_BUILD_PHYS_BIAS */

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
/* 
 * WARNING: The loaded image base address (LOCAL_MEM_LOCAL_ADRS + 
 * WRLOAD_IMAGE_BUILD_PHYS_BIAS) must be modulo RAM_TLB_SZ size. 
 */

#define RAM_TLB_SZ ( (LOCAL_MEM_SIZE <= 0x04000000)  ? _MMU_TLB_SZ_64M  \
                   : (LOCAL_MEM_SIZE <= 0x08000000)  ? _MMU_TLB_SZ_128M \
                   : (LOCAL_MEM_SIZE <= 0x10000000)  ? _MMU_TLB_SZ_256M \
                   : (LOCAL_MEM_SIZE <= 0x20000000)  ? _MMU_TLB_SZ_512M \
                   : (LOCAL_MEM_SIZE <= 0x40000000)  ? _MMU_TLB_SZ_1G   : -1)

#if (LOCAL_MEM_SIZE > 0x80000000)
    #error Additional TLB entries will need to be added to sysStaticTlbDesc[]. 
#endif

#if ((LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE + (PHYS_BIAS_HI_LO & 0xffffffff)) \
     > PHYS_MEM_SIZE)
    #error Your image footprint exceeds available memory.
#endif

#else
#define RAM_TLB_SZ	_MMU_TLB_SZ_1G
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

TLB_ENTRY_DESC sysStaticTlbDesc [] = {

    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash */

    /* needed be first entry here */

    {
    FLASH_BASE_ADRS, 0x0, FLASH_BASE_ADRS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_X   
    },

    /*
     * The RAM execution area must be the 2nd entry because romInit uses the 2nd
     * TLB for this area.  When this TLB is written it replaces the romInit one.
     * You can't overlap TLBs.  You must have a TLB to use an area.
     */

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
    {
    LOCAL_MEM_LOCAL_ADRS,  PHYS_BIAS_HI, 
    LOCAL_MEM_LOCAL_ADRS + PHYS_BIAS_LO,
    _MMU_TLB_TS_0   | RAM_TLB_SZ          | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | CAM_DRAM_CACHE_MODE | _MMU_TLB_ATTR_M     |
                      _MMU_TLB_PERM_X  
    },
#else
    {
    LOCAL_MEM_LOCAL_ADRS, _LOCAL_MEM_ERPN, LOCAL_MEM_LOCAL_ADRS,
    _MMU_TLB_TS_0   | RAM_TLB_SZ          | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | CAM_DRAM_CACHE_MODE | _MMU_TLB_ATTR_M     |
                      _MMU_TLB_PERM_X  
    },
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0     | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W   | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G 
    },

#if (LOCAL_MEM_SIZE > 0x40000000)
    {
    LOCAL_MEM_LOCAL_ADRS + 0x40000000, _LOCAL_MEM_ERPN, 
    LOCAL_MEM_LOCAL_ADRS + 0x40000000, 
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G    | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_M | 
                       _MMU_TLB_PERM_X   
    },
#endif /* (LOCAL_MEM_SIZE > 0x40000000) */

#ifdef INCLUDE_VIRTUAL_SM
    {
    VIRTUAL_SM_BASE, 0, VIRTUAL_SM_BASE_PHYS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT | 
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_M| 
                      _MMU_TLB_PERM_X   
    },
#endif /* INCLUDE_VIRTUAL_SM */

    {
    QIXIS_BASE, 0x0, QIXIS_BASE,
    _MMU_TLB_TS_0       | _MMU_TLB_SZ_4K    | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G     | _MMU_TLB_PERM_W   | _MMU_TLB_IPROT
    },

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_1     | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT | 
    _MMU_TLB_PERM_W   | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G 
    }

    /* All these are not protected */

#ifdef DRV_RESOURCE_QORIQBMAN
    ,
    {
    BMAN_LAW_BASE, 0x0, BMAN_LAW_BASE,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M    | PROTECTED |
      _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_M 
    },

    {
    (BMAN_LAW_BASE + 0x01000000), 0x0, (BMAN_LAW_BASE + 0x01000000),
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   | 
      _MMU_TLB_ATTR_G 
    }
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
    ,
    {
    QMAN_LAW_BASE, 0x0, QMAN_LAW_BASE,
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M    | PROTECTED      |
      _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_M 
    },

    {
    (QMAN_LAW_BASE + 0x01000000), 0x0, (QMAN_LAW_BASE + 0x01000000),
      _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   | 
      _MMU_TLB_ATTR_G  
    }
#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef INCLUDE_PCI_BUS
    ,
    {
    PCIEX1_LAW_BASE, 0x0, PCIEX1_LAW_BASE,
      _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   | 
      _MMU_TLB_ATTR_G  
    }
    ,

    {
    PCIEX2_LAW_BASE, 0x0, PCIEX2_LAW_BASE,
      _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   | 
      _MMU_TLB_ATTR_G  
    }
    ,

    {
    PCIEX3_LAW_BASE, 0x0, PCIEX3_LAW_BASE,
      _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | PROTECTED |
      _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I   | 
      _MMU_TLB_ATTR_G  
    }
    ,

    {
    PCIEX4_LAW_BASE, 0x0, PCIEX4_LAW_BASE,
    _MMU_TLB_TS_0       | PCI_MMU_TLB_SZ    | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G     | _MMU_TLB_PERM_W
    }

#endif  /* INCLUDE_PCI_BUS */

    ,
    {
    DCSR_LAW_BASE, 0x0, DCSR_LAW_BASE,
    _MMU_TLB_TS_0       | _MMU_TLB_SZ_4M    | _MMU_TLB_ATTR_I   |
    _MMU_TLB_ATTR_G     | _MMU_TLB_PERM_W
    }

#if (defined (_WRS_CONFIG_SMP) || defined (INCLUDE_AMP)) &&  \
    (LOCAL_MEM_ERPN || \
     ((RAM_TLB_SZ == _MMU_TLB_SZ_64M) && \
      (CPUn_SPACE_START_ADDR <  \
       (LOCAL_MEM_LOCAL_ADRS & 0xFC000000))) || \
     ((RAM_TLB_SZ == _MMU_TLB_SZ_128M) && \
      (CPUn_SPACE_START_ADDR < \
       (LOCAL_MEM_LOCAL_ADRS & 0xF8000000))) || \
     ((RAM_TLB_SZ == _MMU_TLB_SZ_256M) && \
      (CPUn_SPACE_START_ADDR < \
       (LOCAL_MEM_LOCAL_ADRS & 0xF0000000))) || \
     ((RAM_TLB_SZ == _MMU_TLB_SZ_512M) && \
      (CPUn_SPACE_START_ADDR < \
       (LOCAL_MEM_LOCAL_ADRS & 0xE0000000))) || \
     ((RAM_TLB_SZ == _MMU_TLB_SZ_1G) && \
      (CPUn_SPACE_START_ADDR < (LOCAL_MEM_LOCAL_ADRS & 0xC0000000))))
    /* For CPUn_SPACE to allow ISR call to sysToMonitor(): */
    ,
    {
    ROUND_DOWN (CPUn_SPACE_START_ADDR, 0x10000),
    0,
    ROUND_DOWN (CPUn_SPACE_START_ADDR, 0x10000),
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_64K | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE |
    _MMU_TLB_ATTR_M
    }
#endif	/* (_WRS_CONFIG_SMP || INCLUDE_AMP) &&
         * (_LOCAL_MEM_ERPN || (RAM_TLB_SZ == _MMU_TLB_SZ_XX) &&
         *  (CPUn_SPACE_START_ADDR <
         *   ROUND_DOWN(LOCAL_MEM_LOCAL_ADRS, 0xXX000000)))
         */
};

/*
 * Warning, if sysStaticTlbDesc uses too many entries, mmuOptimize() may
 * fail to add a static entry for the sysPhysMemDesc table.
 */

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

/* macro to compose 64-bit PHYS_ADDRs */

#define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))

/*
* sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
* used by the MMU to translate addresses with single page (4k) granularity.
* PTE memory space should not, in general, overlap BAT memory space but
* may be allowed if only Data or Instruction access is mapped via BAT.
*
* Address translations for local RAM, memory mapped PCI bus, the Board Control
* and Status registers, the T4240 Internal Memory Map, and local FLASH RAM are
* set here.
*
* PTEs are held in a Page Table.  Page Table sizes are
* integer powers of two based on amount of memory to be mapped and a
* minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
* for 32-bit PowerPCs are:
*
* Total mapped memory       Page Table size
* -------------------       ---------------
*        8 Meg               64 K
*       16 Meg              128 K
*       32 Meg              256 K
*       64 Meg              512 K
*      128 Meg                1 Meg
*   .               .
*   .               .
*   .               .
*
* [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
*
*/
PHYS_MEM_DESC sysPhysMemDesc [] = {
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + (PHYS_ADDR) PHYS_BIAS_HI_LO,
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE           |
    VM_STATE_MEM_COHERENCY
    },

    /* To load a secondary w/out the WRLOAD_CPU_MEM_MAP_FUNC defined */

#if defined (INCLUDE_WRLOAD) && !defined (INCLUDE_WRLOAD_IMAGE_BUILD)  \
                             && !defined (WRLOAD_CPU_MEM_MAP_FUNC)
    {
    (VIRT_ADDR) (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE),
    (PHYS_ADDR) (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE),
        PHYS_MEM_SIZE - (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE),
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE           |
    VM_STATE_MEM_COHERENCY
    },
#endif /* INCLUDE_WRLOAD && ... */

#ifdef  INCLUDE_MIPC_SM
    {
    (VIRT_ADDR) MIPC_SM_SYSTEM_POOL_BASE,
    (PHYS_ADDR) MIPC_SM_SYSTEM_POOL_BASE_PHYS,
    MIPC_SM_SYSTEM_POOL_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE           |
    VM_STATE_MEM_COHERENCY
    },
#endif /* INCLUDE_MIPC_SM */

    {
    (VIRT_ADDR) QIXIS_BASE,
    (PHYS_ADDR) QIXIS_BASE,
    QIXIS_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

#ifdef INCLUDE_PCI_BUS
    {
    (VIRT_ADDR) PCIEX1_LAW_BASE,
    PHYS_64BIT_ADDR (0x0, PCIEX1_LAW_BASE),
    PCIEX1_LAW_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_LAW_BASE,
    PHYS_64BIT_ADDR (0x0, PCIEX2_LAW_BASE),
    PCIEX2_LAW_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX3_LAW_BASE,
    PHYS_64BIT_ADDR (0x0, PCIEX3_LAW_BASE),
    PCIEX3_LAW_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX4_LAW_BASE,
    PHYS_64BIT_ADDR (0x0, PCIEX4_LAW_BASE),
    PCIEX4_LAW_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },
#endif /* INCLUDE_PCI_BUS */

#ifdef DRV_VXB_NORFLASH
    {
    (VIRT_ADDR) FLASH_BASE_ADRS,
    PHYS_64BIT_ADDR (0x0, FLASH_BASE_ADRS),
    FLASH_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },
#endif /* DRV_VXB_NORFLASH */

#ifdef DRV_RESOURCE_QORIQBMAN

    /*
     * The buffer manager access area is 32MB wide, with the
     * first 16MB set as cacheable and the second 16MB set
     * as cache inhibited.
     */

    {
    (VIRT_ADDR) BMAN_LAW_BASE,
    (PHYS_ADDR) BMAN_LAW_BASE,
    0x1000000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE       |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) (BMAN_LAW_BASE + 0x01000000),
    (PHYS_ADDR) (BMAN_LAW_BASE + 0x01000000),
    0x1000000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) BMAN_MEM_BASE,
    (PHYS_ADDR) BMAN_MEM_BASE,
    BMAN_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE       |
    VM_STATE_MEM_COHERENCY
    },
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN

    /*
     * The queue manager access area is 32MB wide, with the
     * first 16MB set as cacheable and the second 16MB set
     * as cache inhibited.
     */

    {
    (VIRT_ADDR) QMAN_LAW_BASE,
    (PHYS_ADDR) QMAN_LAW_BASE,
    0x1000000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE       |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) (QMAN_LAW_BASE + 0x01000000),
    (PHYS_ADDR) (QMAN_LAW_BASE + 0x01000000),
    0x1000000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) QMAN_MEM_BASE,
    (PHYS_ADDR) QMAN_MEM_BASE,
    QMAN_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE       |
    VM_STATE_MEM_COHERENCY
    },
#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef DRV_SRIO_FSL
    {
    (VIRT_ADDR) FSL_RIO_MEM_ADRS,
    (PHYS_ADDR) FSL_RIO_MEM_ADRS,
    FSL_RIO_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },
#endif /* DRV_SRIO_FSL */

    {
    (VIRT_ADDR) DCSR_LAW_BASE,
    PHYS_64BIT_ADDR (0x0, DCSR_LAW_BASE),
    0x400000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    }
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* Clock Ratio Tables */

#ifndef	FORCE_DEFAULT_FREQ
#define MAX_VALUE_PLAT_RATIO    32

LOCAL UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO] = {
    0,  0,  0,  0,  0,  5,  6,  7,  8,  9, 10, 11, 12,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

# ifdef	DEBUG
#define MAX_VALUE_DDR_RATIO     32

LOCAL UINT32 ddrRatioTable[MAX_VALUE_DDR_RATIO] = {
    0,  1,  0,  0,  0,  5,  6,  0,  8,  9, 10,  0, 12, 13,  0,  0,
   16,  0, 18, 19, 20,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0
};

#define MAX_VALUE_CCS_CLOCK      16

LOCAL UINT32 ccs1ClockTable[MAX_VALUE_CCS_CLOCK] = {
    0,  0,  0,  0,  1,  1,  1,  0,  2,  2,  2,  0,  3,  3,  3,  0
};

#define MAX_VALUE_CCS_DIVS      16

LOCAL UINT32 ccs1DivsTable[MAX_VALUE_CCS_DIVS] = {
    1,  2,  4,  0,  1,  2,  3,  0,  1,  2,  4,  0,  1,  2,  3,  0
};
# endif	/* DEBUG */
#endif	/* !FORCE_DEFAULT_FREQ */

#ifdef INCLUDE_PCI_BUS

/* INT LINE TO IRQ assignment for t4240qds board. */

LOCAL UCHAR sysPci1IntRoute [NUM_PCIEX1_SLOTS][4] = {
    { PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL },
    { PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL },
    { PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL },
    { PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL }
};

LOCAL UCHAR sysPci2IntRoute [NUM_PCIEX2_SLOTS][4] = {
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL },
    { PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL },
    { PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL },
    { PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL }
};

LOCAL UCHAR sysPci3IntRoute [NUM_PCIEX3_SLOTS][4] = {
    { PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL },
    { PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL, PCIEX3_XINT1_LVL },
    { PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL, PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL },
    { PCIEX3_XINT4_LVL, PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL }
};

LOCAL UCHAR sysPci4IntRoute [NUM_PCIEX4_SLOTS][4] = {
    { PCIEX4_XINT1_LVL, PCIEX4_XINT2_LVL, PCIEX4_XINT3_LVL, PCIEX4_XINT4_LVL },
    { PCIEX4_XINT2_LVL, PCIEX4_XINT3_LVL, PCIEX4_XINT4_LVL, PCIEX4_XINT1_LVL },
    { PCIEX4_XINT3_LVL, PCIEX4_XINT4_LVL, PCIEX4_XINT1_LVL, PCIEX4_XINT2_LVL },
    { PCIEX4_XINT4_LVL, PCIEX4_XINT1_LVL, PCIEX4_XINT2_LVL, PCIEX4_XINT3_LVL }
};

#endif /* INCLUDE_PCI_BUS */

int   sysBus      = BUS_TYPE_NONE;      /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (T4240) */
char *sysBootLine = BOOT_LINE_ADRS;     /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;       /* catastrophic message area */

#ifdef INCLUDE_AMP
int   sysProcNum = AMP_CPU_NUMBER;      /* processor number of this CPU */
#else
int   sysProcNum  = 0;                  /* processor number of this CPU */
#endif /* INCLUDE_AMP */

#ifdef	DEBUG
UINT32  coreFreq[MAX_CPUS];
UINT32  ddrFreq;
UINT32  systemFreq;
UINT32  multiSysToCCB;
UINT32  multiSysToDDR;
UINT32  multiSysToCC[MAX_CPUS];
#endif	/* DEBUG */

#ifdef INCLUDE_T4240_FMAN_UCODE
IMPORT  UINT8 _binary_fsl_fman_ucode_T4240_106_4_6_bin_start[];
#endif /* INCLUDE_T4240_FMAN_UCODE */

LOCAL char * physTop = NULL;
LOCAL char * memTop = NULL;
LOCAL UINT32 sysClkFreq = 0;

#ifdef INCLUDE_USR_MACADDR
const char *sysNetDevName[MAX_MAC_DEVS] = { "memac" };
int  sysNetDevNum[MAX_MAC_DEVS] = { MEMAC_NUM };
UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN] = {
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_0, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_1, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_2, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_3, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_4, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_5, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_6, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_7, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_8, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_9, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_10, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_11, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_12, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_13, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_14, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_15, CUST_ENET4, CUST_ENET5 }
};
#endif

/* externs */

IMPORT void sysSerialConnectAll (void);
IMPORT void sysBoardReset (void);
IMPORT void hardWareInterFaceInit ();

IMPORT void coreE6500CpuPriInit (void);
IMPORT void coreE6500CpuShutdown(int startType);

#ifdef INCLUDE_WRLOAD
IMPORT void coreE6500AmpCpuEnable (FUNCPTR, UINT32);
IMPORT STATUS coreE6500AmpCpuPrep (UINT32, void *);
#endif /* INCLUDE_WRLOAD */

#ifdef _WRS_CONFIG_SMP
IMPORT STATUS coreE6500CpuEnable (UINT32, WIND_CPU_STATE *);
#   ifdef INCLUDE_VIRTUAL_SM
IMPORT void sysVirtualSmTlb ();
#   endif /* INCLUDE_VIRTUAL_SM */
#endif /* _WRS_CONFIG_SMP */

#if defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00)
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL3CacheInvalEnableFunc;
IMPORT VOIDFUNCPTR _pSysL3CacheFlushDisableFunc;
#endif /* INCLUDE_AMP && INCLUDE_AMP_CPU_00) */

/* forward declarations */

LOCAL UINT32 sysFmanClkFreqGet (int);
LOCAL UINT32 sysFman1ClkFreqGet (void);
LOCAL UINT32 sysFman2ClkFreqGet (void);
#ifdef  DRV_MII_QORIQMEMAC_MDIO
LOCAL void sysEmiSelect (int, int);
#endif
void   sysMsDelay (UINT);
void   sysUsDelay (UINT);
UINT32 sysClkFreqGet (void);

#ifdef	DRV_MII_QORIQMEMAC_MDIO
LOCAL EMI_MAP emiMap[] =
   {
   	/* fmanNum	memacNum	EMI port */

#ifdef SERDES1_SGMII
        /*
         * When using SerDes 1 protocol 0x28, memac4 is
         * supposed to be routed via SGMII, however the
         * default T4240QDS RCW leaves memac4 set for RGMII
         * mode, which takes precedence.
         *
   	{  0,		4,		QX_EMI1_SLOT1 },
         */
   	{  0,		5,		QX_EMI1_SLOT1 },
   	{  0,		8,		QX_EMI1_SLOT1 },
   	{  0,		9,		QX_EMI1_SLOT1 },

   	{  0,		0,		QX_EMI1_SLOT2 },
   	{  0,		1,		QX_EMI1_SLOT2 },
   	{  0,		2,		QX_EMI1_SLOT2 },
   	{  0,		3,		QX_EMI1_SLOT2 },
#endif

#ifdef SERDES2_SGMII
        /*
         * When using SerDes 2 protocol 0x28, memac12 is
         * supposed to be routed via SGMII, however the
         * default T4240QDS RCW leaves memac12 set for RGMII
         * mode, which takes precedence.
         *
   	{  1,		4,		QX_EMI1_SLOT3 },
         */
   	{  1,		5,		QX_EMI1_SLOT3 },
   	{  1,		8,		QX_EMI1_SLOT3 },
   	{  1,		9,		QX_EMI1_SLOT3 },

   	{  1,		0,		QX_EMI1_SLOT4 },
   	{  1,		1,		QX_EMI1_SLOT4 },
   	{  1,		2,		QX_EMI1_SLOT4 },
   	{  1,		3,		QX_EMI1_SLOT4 },
#endif

        /* RGMII configuration is always the same */

   	{  0,		4,		QX_EMI1_RGMII },
   	{  1,		4,		QX_EMI1_RGMII },

   	{  -1,		-1,		-1 },
   };
#endif

#ifdef DRV_I2C_VSC3316

/*
 * The Freescale T4240 QDS board has two Vitesse VSC3316 crossbar
 * switch chips between the T4240 SERDES1 and SERDES2 blocks and
 * expansion slots 1,2,3 and 4. The switches have to be configured
 * via software in order to properly map the input and output links
 * correctly before any devices in the slots can be used. The
 * tables below contain the mapping information for SERDES1 RX/TX
 * and SERDES2 RX/TX lanes. These mappings are programmed into
 * the switches during system startup.
 */

LOCAL VSC_TABLE vsc_serdes1_rx =
    {
        8,
        {
            { 2, 12 },
            { 3, 13 },
            { 4, 5 },
            { 5, 4 },
            { 10, 11 },
            { 11, 10 },
            { 12, 2 },
            { 13, 3 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 }
        }
    };

LOCAL VSC_TABLE vsc_serdes1_tx = 
    {
        8,
        {
            { 0, 0 },
            { 1, 1 },
            { 6, 6 },
            { 7, 7 },
            { 8, 8 },
            { 9, 9 },
            { 14, 14 },
            { 15, 15 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 }
        }
    };

LOCAL VSC_TABLE vsc_serdes2_rx =
    {
        8,
        {
            { 0, 15 },
            { 1, 14 },
            { 6, 7 },
            { 7, 6 },
            { 8, 9 },
            { 9, 8 },
            { 14, 1 },
            { 15, 0 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 }
        }
    };

LOCAL VSC_TABLE vsc_serdes2_tx =
    {
        8,
        {
            { 2, 2 },
            { 3, 3 },
            { 4, 4 },
            { 5, 5 },
            { 10, 10 },
            { 11, 11 },
            { 12, 12 },
            { 13, 13 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 },
            { -1, -1 }
        }
    };
#endif

#ifdef _WRS_CONFIG_SMP
LOCAL   UINT32 physCpuAvailableGet (void);
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_AMP
LOCAL UINT64 sysVirtToPhys (void *);
LOCAL void * sysPhysToVirt (UINT64);
#endif /* INCLUDE_AMP */

#ifdef INCLUDE_USB
/* USB register base addresses */

LOCAL UINT32 usbPhyBase[]  = { T4_USB1_PHY_BASE, T4_USB2_PHY_BASE };
LOCAL UINT32 usbEhciBase[] = { T4_USB1_BASE, T4_USB2_BASE  };
#endif /* INCLUDE_USB */

#ifndef INCLUDE_NVRAM_FLASH
#   include <mem/nullNvRam.c>
#endif /* !INCLUDE_NVRAM_FLASH */

#include "hwconf.c"

/* defines */

#define SYS_MODEL           sysModel ()
#define SYS_MODEL_T4240     "Freescale QorIQ T4240"
#define SYS_MODEL_T4240E    "Freescale QorIQ T4240E"
#define SYS_MODEL_E6500     "Freescale E6500 : Unknown system version"
#define SYS_MODEL_UNKNOWN   "Freescale Unknown processor"

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

#ifdef INCLUDE_AMP
METHOD_DECL(sysBspDevFilter);
STATUS sysDeviceFilter(VXB_DEVICE_ID pDev);
#endif /* INCLUDE_AMP */

DEVMETHOD_DEF(sysVirtToPhys, "BSP virtual to physical address translation");
DEVMETHOD_DEF(sysPhysToVirt, "BSP physical to virtual address translation");
DEVMETHOD_DEF(sysFmanClkFreqGet, "Get Fman clock frequency");
#ifdef  DRV_MII_QORIQMEMAC_MDIO
DEVMETHOD_DEF(sysEmiSelect, "Set EMI management port");
#endif

LOCAL struct vxbDeviceMethod t4240PlbMethods[] =
    {
#ifdef INCLUDE_AMP

    /*
     * Only used for AMP filtering per core for now so not built for
     * anything else
     */

    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),

    /* Only used for AMP images running above 32-bit boundary. */

    DEVMETHOD(sysVirtToPhys, sysVirtToPhys),
    DEVMETHOD(sysPhysToVirt, sysPhysToVirt),
#endif /* INCLUDE_AMP */

    DEVMETHOD(sysFmanClkFreqGet, (FUNCPTR)sysFmanClkFreqGet),
#ifdef  DRV_MII_QORIQMEMAC_MDIO
    DEVMETHOD(sysEmiSelect, (FUNCPTR)sysEmiSelect),
#endif
    { 0, 0 }
    };

IMPORT device_method_t * pSysPlbMethods;

#ifdef INCLUDE_AMP

/*******************************************************************************
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

    /*
     * Compare devices name and unit number with those you want to remove
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
#else /* INCLUDE_VXCPULIB */
    if (ampCpuTable[deviceIndex].cpu != vxCpuPhysIndexGet())
#endif /* INCLUDE_VXCPULIB */
        return(ERROR);

    return(OK);
    }

#endif /* INCLUDE_AMP */

#ifdef  DRV_MII_QORIQMEMAC_MDIO
/*******************************************************************************
*
* sysEmiSelect - program external MDIO multiplexing logic
*
* The T4240's MDIO pins are routed to the QIXIS chip, which in turn can connect
* them to one of several target devices. We need to select the right routing
* depending on which device needs to be managed. This is a board-specific
* operation, so we need a method here to handle this. This routing is only
* correct for the Freescale T4240 QDS board.
*
* RETURNS: NONE
*/

LOCAL void sysEmiSelect
    (
    int fmanNum,
    int memacNum
    )
    {
    EMI_MAP * p;
    UINT8 brdCfg4;

    p = &emiMap[0];

    FOREVER
        {
        if (p->fmanNum == -1)
            break;
        if (p->fmanNum == fmanNum && p->memacNum == memacNum)
            {
            brdCfg4 = T4_CSR_READ_8(QX_BRDCFG4);
            brdCfg4 &= ~QX_BRDCFG4_EMISEL;
            brdCfg4 |= QX_EMI(p->emiPort);
            T4_CSR_WRITE_8(QX_BRDCFG4, brdCfg4);
            break;
            }
        p++;
        }

    return;
    }
#endif /* DRV_MII_QORIQMEMAC_MDIO */

/*******************************************************************************
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

    device = T4_CSR_READ_32(T4_PVR) & 0xfff00000;

    if ((device != 0x80200000) && (device != 0x80400000))
        {
        return (SYS_MODEL_UNKNOWN);
        }

    device = T4_CSR_READ_32(T4_SVR) & 0xffff0000;

    switch(device)
        {
        case 0x82400000:
            retChar = SYS_MODEL_T4240;
            break;
        case 0x82480000:
            retChar = SYS_MODEL_T4240E;
            break;
        default:
            retChar = SYS_MODEL_E6500;
            break;
        }

    return(retChar);
    }

/*******************************************************************************
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

/*******************************************************************************
*
* sysClkFreqGet - returns the clock freq of the platform
*
* RETURNS: Clock freq of the platform
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet (void)
    {
#ifndef FORCE_DEFAULT_FREQ
    UINT32  platRatio;
#ifdef	DEBUG
    UINT32  ddrRatio;
    UINT32  ccRatio[MAX_CPUS];
    UINT32  n;
#endif	/* DEBUG */
    UINT32  oscFreq;
    UINT8   osc;
#endif /* FORCE_DEFAULT_FREQ */

    if (sysClkFreq != 0)
        return (sysClkFreq);

#ifdef FORCE_DEFAULT_FREQ
    sysClkFreq = DEFAULT_SYSCLKFREQ;
#else /* FORCE_DEFAULT_FREQ */

    platRatio = SYS_PLL_RAT;

    if ((platRatio >= MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio] == 0))
        {
        /* A default value better than zero or -1 */

        sysClkFreq = DEFAULT_SYSCLKFREQ;

        return (sysClkFreq);
        }

#   ifdef DEBUG
    multiSysToCCB = platRatioTable[platRatio];

    ddrRatio = MEM_PLL_RAT;

    multiSysToDDR = ddrRatioTable[ddrRatio];

    for (n = 0; n < MAX_CPUS; n++)
        {
        ccRatio[n] = CCN_PLL_RAT(n);

        multiSysToCC[n] = ccRatio[n];

        }
#   endif /* DEBUG */

    osc = T4_CSR_READ_8(QX_CLK_SPD1) & 0x7;

    switch (osc)
        {
        case QX_SYSCLK_67_MHZ:
            oscFreq = FREQ_67_MHZ;
            break;
        case QX_SYSCLK_83_MHZ:
            oscFreq = FREQ_83_MHZ;
            break;
        case QX_SYSCLK_100_MHZ:
            oscFreq = FREQ_100_MHZ;
            break;
        case QX_SYSCLK_125_MHZ:
            oscFreq = FREQ_125_MHZ;
            break;
        case QX_SYSCLK_133_MHZ:
            oscFreq = FREQ_133_MHZ;
            break;
        case QX_SYSCLK_150_MHZ:
            oscFreq = FREQ_150_MHZ;
            break;
        case QX_SYSCLK_160_MHZ:
            oscFreq = FREQ_160_MHZ;
            break;
        case QX_SYSCLK_167_MHZ:
            oscFreq = FREQ_167_MHZ;
            break;
        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    sysClkFreq = ((UINT32)(oscFreq * platRatioTable[platRatio]));

#   ifdef DEBUG
    systemFreq = oscFreq;

    /* 1:1 is a special case */

    if (ddrRatio == 1)
        ddrFreq = sysClkFreq;
    else
        ddrFreq = oscFreq * ddrRatioTable[ddrRatio];

    for (n = 0; n < MAX_CPUS; n++)
        {
        UINT32 ccPllSel = CN_PLL_SEL(n);
        UINT32 cc = ccs1ClockTable[ccPllSel];
        UINT32 div = ccs1DivsTable[ccPllSel];
        UINT32 coreRatio = ccRatio[cc];
        coreFreq[n] = (oscFreq * coreRatio) / div;
        }

#   endif /* DEBUG */

#endif /* FORCE_DEFAULT_FREQ */

    return(sysClkFreq);
    }

/*******************************************************************************
*
* sysPlbClkFreqGet - get the local bus clock frequency
*
* RETURNS: Clock freq of Local Bus. The value is one half of the platform(CCB) clock.
*
* ERRNO: N/A
*/

UINT32 sysPlbClkFreqGet (void)
    {
    return (sysClkFreqGet () / 2);
    }

/*******************************************************************************
*
* sysPpcTbClkFreqGet - get ppc core timebase clock frequency
*
* RETURNS: Clock freq of PPC core timebase
*
* ERRNO: N/A
*/

UINT32 sysPpcTbClkFreqGet (void)
    {
    return (sysPlbClkFreqGet ());
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
        return (sysPlbClkFreqGet() / picClockDivisor);
    else
        return RTC_FREQ;
    }

#ifdef INCLUDE_USB

/*******************************************************************************
*
* sysUsbPhyInit - initialize the USB UTMI PHY model
*
* This rotuine initialize the UTMI PHY device's clock.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysUsbPhyInit (void)
    {
    /* Initialize USB PHY's PLL */

    T4_CSR_WRITE_32(T4_USB_PHY_PLL_PRG1, PLL_PRG1_INIT_VALUE);
    T4_CSR_WRITE_32(T4_USB_PHY_PLL_PRG2, PLL_PRG2_INIT_VALUE);

    }

/*******************************************************************************
*	
* ehciInit - initialize the on-chip EHCI controller
*
* This routine initializes the on-chip EHCI controller.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ehciInit
    (
    int unit
    )
    {
    UINT32 tempVal;

    /* enable USB PHY */

    T4_CSR_WRITE_32(T4_USB_PHY_VBUS_CFG(usbPhyBase[unit]), PHY_VBUS_ENABLE);
    T4_CSR_WRITE_32(T4_USB_PHY_CTLR(usbPhyBase[unit]), 
                            T4_CSR_READ_32(T4_USB_PHY_CTLR(usbPhyBase[unit]))
                            | USB_PORT_ENABLE);

    /* wait for USB PHY PLL valid */

    do
        {
        tempVal = T4_CSR_READ_32(T4_USB_PHY_STS(usbPhyBase[unit]));
        sysUsDelay (10);
        }
    while (!(tempVal & PHY_PLL_VALID));

    /* snoop size 4 GB */

    T4_CSR_WRITE_32(T4_EHCI_SNOOP1(usbEhciBase[unit]), EHCI_SNOOP_SIZE_2G); 
    T4_CSR_WRITE_32(T4_EHCI_SNOOP2(usbEhciBase[unit]), 0x80000000 | EHCI_SNOOP_SIZE_2G);
    
    /* recommended priority and age count threshold */

    T4_CSR_WRITE_32(T4_EHCI_PRI_CTRL(usbEhciBase[unit]), EHCI_PRI_EN);
    T4_CSR_WRITE_32(T4_EHCI_AGE_CNT_THRESH(usbEhciBase[unit]), 0x00000080);
    
    /* rd prefetch 32 byte base on burst size 8 x 4 */

    T4_CSR_WRITE_32(T4_EHCI_SI_CTRL(usbEhciBase[unit]), 0x00000001);

    /* enable the usb interface */

    T4_CSR_WRITE_32(T4_EHCI_CONTROL(usbEhciBase[unit]), (EHCI_CONTROL_UTMI_PHY_EN |
                                                   EHCI_CONTROL_USB_EN));

    /*
     * enable ports
     *
     * The write to PORTSC register to select UTMI PHY must be put in the last,
     * otherwise accessing other EHCI registers will make the CPU hang forever.
     */

     T4_CSR_WRITE_32(T4_EHCI_PORTSC(usbEhciBase[unit]), LONGSWAP(EHCI_PORTSC_UTMI));
    }

/*******************************************************************************
*
* ehciPostResetHook - post reset hook of the on-chip EHCI controller
*
* This routine sets the on-chip EHCI controller to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ehciPostResetHook
    (
    int unit
    )
    {
    T4_CSR_WRITE_32(T4_EHCI_USBMODE(usbEhciBase[unit]),
                             T4_CSR_READ_32(T4_EHCI_USBMODE(usbEhciBase[unit]))
                             | LONGSWAP(EHCI_USBMODE_HOST));
    }

/*******************************************************************************
*
* ehci0Init - initialize the on-chip EHCI controller (USB1)
*
* This routine initializes the on-chip EHCI controller (USB1).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci0Init (void)
    {
    ehciInit(0);
    }
	
/*******************************************************************************
*
* ehci0PostResetHook - post reset hook of the on-chip EHCI controller (USB1)
*
* This routine sets the on-chip EHCI controller (USB1) to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci0PostResetHook (void)
    {
    ehciPostResetHook(0);
    }

/*******************************************************************************
*
* ehci1Init - initialize the on-chip EHCI controller (USB2)
*
* This routine initializes the on-chip EHCI controller (USB2).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci1Init (void)
    {
    ehciInit(1);
    }
	
/*******************************************************************************
*
* ehci1PostResetHook - post reset hook of the on-chip EHCI controller (USB2)
*
* This routine sets the on-chip EHCI controller (USB2) to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci1PostResetHook (void)
    {
    ehciPostResetHook(1);
    }
	
#endif /* INCLUDE_USB */

#ifdef INCLUDE_I2C_BUS

/*******************************************************************************
*
* sysI2cBus0ChanSel - select I2C bus 0 subsystem channle
*
* This rotuine select multiplex switch to enable I2C bus 0 subsystem channle
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysI2cBus0ChanSel
    (
    UINT32  chanIndex
    )
    {
    VUINT8  regVal;
    static  VUINT8  switchStatus1 = 0x00;
    static  VUINT8  switchStatus2 = 0x00;

    /* Check whether the bus channel switch1 is needed to be switched */

    regVal = (chanIndex & I2C_BUS1_SW1_MASK) >> 4;

    if (regVal != switchStatus1)
        {
        vxbI2cByNameWrite("mux_PCA9547", 0, 0, &regVal, 1);
        switchStatus1 = regVal;
        sysUsDelay (10);
        }

    if (chanIndex & I2C_BUS1_SW2_MASK)
        {
        /* Check whether the bus channel switch2 is needed to be switched */

        regVal = chanIndex & I2C_BUS1_SW2_MASK;

        if (regVal != switchStatus2)
            {
            vxbI2cByNameWrite("mux_PCA9547", 1, 0, &regVal, 1);
            switchStatus2 = regVal;
            sysUsDelay (10);
            }
        }
    return;
    }

#endif /* INCLUDE_I2C_BUS */

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the T4240QDS board. It sets up
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
#endif /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_T4240_FMAN_UCODE
    int x;
    int y;
    HCF_DEVICE * pDev;
    struct hcfResource * pRes;
    UINT32 chipRev;
    void * pUcode = NULL;
#endif /* INCLUDE_T4240_FMAN_UCODE */

    coreE6500CpuPriInit();

    /* Initialize the system bus clock values; must be after TLB static init */

    sysClkFreqGet();

#ifdef INCLUDE_T4240_FMAN_UCODE

    pUcode = _binary_fsl_fman_ucode_T4240_106_4_6_bin_start;

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

#endif /* INCLUDE_T4240_FMAN_UCODE */

#ifdef INCLUDE_USB
    sysUsbPhyInit ();
#endif /* INCLUDE_USB */

#ifdef DRV_STORAGE_SDHC

    /* Switch eSDHC interface to SD mode */

    T4_CSR_WRITE_8(QX_BRDCFG5, T4_CSR_READ_8(QX_BRDCFG5) | 0x42);
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA

    /* Switch Lanes 4 - 7 muxed to SATA */

    T4_CSR_WRITE_8(QX_BRDCFG12, T4_CSR_READ_8(QX_BRDCFG12) & 0xf8);
#endif /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_PCI_BUS

    /* Force reset PCI-EX SLOTs(5 ~ 8) */

    T4_CSR_WRITE_8(QX_RST_FORCE(3), 
                          T4_CSR_READ_8(QX_RST_FORCE(3))
                          | (PCIEX_SLOT_5_RST | PCIEX_SLOT_6_RST
                          | PCIEX_SLOT_7_RST |PCIEX_SLOT_8_RST) );
    sysMsDelay (20);

    T4_CSR_WRITE_8(QX_RST_FORCE(3), 
                          T4_CSR_READ_8(QX_RST_FORCE(3))
                          & ~(PCIEX_SLOT_5_RST | PCIEX_SLOT_6_RST
                          | PCIEX_SLOT_7_RST |PCIEX_SLOT_8_RST) );
    sysMsDelay (20);
#endif /* INCLUDE_PCI_BUS */

    pSysPlbMethods = t4240PlbMethods;

    hardWareInterFaceInit();
    }

/*******************************************************************************
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
#else /* LOCAL_MEM_SIZE */
        physTop = (char *)(PHYS_MEM_START + PHYS_MEM_SIZE);
#endif /* LOCAL_MEM_SIZE */
        }

    return(physTop);
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
*
* ERRNO: N/A
*/

char * sysMemTop (void)
    {

    if (memTop == NULL)
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM
        memTop = (memTop - PM_RESERVED_MEM);
#endif /* INCLUDE_EDR_PM */
        }

    return memTop;
    }

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
*/

STATUS sysToMonitor
    (
    int startType   /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_WARM_ADRS);

    coreE6500CpuShutdown (startType);

#ifdef INCLUDE_AMP_CPU_00
    /* this triggers a board reset in the AMP */

    sysBoardReset ();
#else
    (*pRom) (startType);
#endif /* INCLUDE_AMP_CPU_00 */

    return(OK);    /* in case we ever continue from ROM monitor */
    }

#if defined(_WRS_CONFIG_SMP)

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
    unsigned int      physCpuIndex, /* physical cpu index */
    WIND_CPU_STATE  * pCpuState
    )
    {
    return coreE6500CpuEnable(physCpuIndex, pCpuState);
    }

/*******************************************************************************
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

/*******************************************************************************
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
    return coreE6500AmpCpuEnable(entryPt, cpu);
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
    return coreE6500AmpCpuPrep(physCpuIndex, flags);
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
    return (PHYS_BIAS_HI_LO + (UINT64)(pVirt));
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
* however the design of the MEMAC makes it difficult to get by
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
    UINT32 virt = (UINT32)((pPhys - PHYS_BIAS_HI_LO)
                            & 0xFFFFFFFFUL);
    return (void *)virt;
    }

#endif /* INCLUDE_AMP */

#if defined (INCLUDE_ALTIVEC)
/*******************************************************************************
* sysAltivecProbe - Check if the CPU has ALTIVEC unit.
*
* This routine returns always OK.
*
* RETURNS: always OK
*
* ERRNO
*/

STATUS sysAltivecProbe (void)
    {
    return (OK);
    }
#endif /* INCLUDE_ALTIVEC */

/*******************************************************************************
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
    static BOOL initialized = FALSE;
#ifdef DRV_I2C_VSC3316
    int i;
#endif

    if (initialized)
        return;

    vxbDevInit();

    /*
     * for AMP, L2 & L3 shared cache can not be enabled or disabled by private cores.
     * so un-register L2 & L3 hook functions to avoid confilct.
     */    

#if defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00)
#   ifdef INCLUDE_L2_CACHE
    _pSysL2CacheEnable = NULL;
    _pSysL2CacheDisable = NULL;    
#   endif /* INCLUDE_L2_CACHE */

#   ifdef INCLUDE_L3_CACHE
    _pSysL3CacheInvalEnableFunc = NULL;
    _pSysL3CacheFlushDisableFunc = NULL;
#   endif /* INCLUDE_L3_CACHE */
#endif /* INCLUDE_AMP && INCLUDE_AMP_CPU_00 */

    /* initialize serial interrupts */

#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
#endif  /* INCLUDE_SIO_UTILS */

#ifdef  INCLUDE_ALTIVEC
    _func_altivecProbeRtn = sysAltivecProbe;
#endif  /* INCLUDE_ALTIVEC */

    taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);

#ifdef DRV_I2C_VSC3316
    /*
     * On the QDS board, the SERDES1 and SERDES2 blocks
     * in the T4240 are connected to expansions slots 1,2,3 and 4
     * through Vitesse VSC3316 crossbar switches. These must
     * be initialized before we can access any SGMII or XAUI
     * cards that may be present. However the switches must be
     * configured via I2C, so we have to wait until after
     * VxBus initialization completes and the I2C subsystem
     * is set up before we can do it.
     *
     * There are two chips associated with the SERDES1 and
     * SERDES2 blocks. Unit 0 is for the TX paths and unit
     * 1 is for the RX paths.
     *
     * Note: we make several attempts to configure the
     * bridges, because on some boards it has been observed
     * that it doesn't always succeed the first time.
     * It's unclear if this is a software problem or a
     * prototype board issue, but for now we retry a few
     * times to make sure the SERDES lanes are available.
     */

    for (i = 0; i < 10; i++)
        {
        if (vsc3316Config (0, &vsc_serdes1_tx) == OK)
            break;
        }

    for (i = 0; i < 10; i++)
        {
        if (vsc3316Config (1, &vsc_serdes1_rx) == OK)
            break;
        }


    for (i = 0; i < 10; i++)
        {
        if (vsc3316Config (0, &vsc_serdes2_tx) == OK)
            break;
        }

    for (i = 0; i < 10; i++)
        {
        if (vsc3316Config (1, &vsc_serdes2_rx) == OK)
            break;
        }
#endif

    initialized = TRUE;

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
* ERRNO: N/A
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return(sysProcNum);
    }

/*******************************************************************************
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
    UINT    delay        /* length of time in microsec to delay */
    )
    {
    UINT baselineTickCount;
    UINT curTickCount;
    UINT terminalTickCount;
    int actualRollover = 0;
    int calcRollover = 0;
    UINT ticksToWait;
    UINT requestedDelay;
    UINT oneUsDelay;
    UINT tbu;    

    /* Exit if no delay count */

    if ((requestedDelay = delay) == 0)
        return;

    /*
     * Get the Time Base Lower register tick count, this will be used
     * as the baseline.
     */

    vxTimeBaseGet (&tbu, &baselineTickCount);

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

    oneUsDelay = (((sysClkFreq >> 5) + 1000000) / 1000000);

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

        vxTimeBaseGet (&tbu, &curTickCount);

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

/*******************************************************************************
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
    sysUsDelay (delay * 1000 );
    }

/*******************************************************************************
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

/*******************************************************************************
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

#ifdef INCLUDE_PCI_BUS

/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI Express 1 autoconfig support routine
*
* This routine performs the PCI Express 1 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/
STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UINT devVend            /* deviceID/vendorID of device */
    )
    {

    /* Host controller itself (device number is 0) won't be configured */

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
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
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
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
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
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    if ((pin >= 1) && (pin < 5))
        tmpChar = sysPci3IntRoute [(pLoc->device) & 3][pin-1];

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci4AutoconfigIntrAssign - PCI 4 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci4AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    if ((pin >= 1) && (pin < 5))
        tmpChar = sysPci4IntRoute [(pLoc->device) & 3][pin-1];

    return (tmpChar);
    }

#endif /* INCLUDE_PCI_BUS */

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

/*****************************************************************************
*
* sysFmanClkFreqGet - return the frame manager clock frequency
*
* This function returns the clock frequency for the frame manager specified
* by <unit>. With earlier QorIQ devices, there was only one Fman clock per
* system. With the T4240, each Fman may be clocked separately.
*
* RETURNS: frame manager clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 sysFmanClkFreqGet
    (
    int unit
    )
    {
    if (unit == 0)
        return (sysFman1ClkFreqGet ());

    if (unit == 1)
        return (sysFman2ClkFreqGet ());

    return (0);
    }

/*****************************************************************************
*
* sysFman1ClkFreqGet - return the frame manager clock frequency
*
* This function calculates the frame manager clock frequency and
* returns it.
*
* The frame manager clock source is selectable via the reset control word.
* It can either be the platform clock frequency divided by two, or the
* core cluster 3 PLL frequency divided by two. For the T4240QDS board,
* the RCW supplied by Freescale uses the platform clock frequency.
*
* RETURNS: frame manager clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 sysFman1ClkFreqGet (void)
    {
#ifndef FORCE_DEFAULT_FREQ
    UINT8 osc;
    UINT32 oscFreq;
    UINT32 ccRatio;
    UINT32 fman1ClkSrc;

    /* Get the base oscillator frequency from the QIXIS. */

    osc =  T4_CSR_READ_8(QX_CLK_SPD1) & 0x7;
    switch (osc)
        {
        case QX_SYSCLK_67_MHZ:
            oscFreq = FREQ_67_MHZ;
            break;
        case QX_SYSCLK_83_MHZ:
            oscFreq = FREQ_83_MHZ;
            break;
        case QX_SYSCLK_100_MHZ:
            oscFreq = FREQ_100_MHZ;
            break;
        case QX_SYSCLK_125_MHZ:
            oscFreq = FREQ_125_MHZ;
            break;
        case QX_SYSCLK_133_MHZ:
            oscFreq = FREQ_133_MHZ;
            break;
        case QX_SYSCLK_150_MHZ:
            oscFreq = FREQ_150_MHZ;
            break;
        case QX_SYSCLK_160_MHZ:
            oscFreq = FREQ_160_MHZ;
            break;
        case QX_SYSCLK_167_MHZ:
            oscFreq = FREQ_167_MHZ;
            break;
        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    /*
     * Read RCW bits 227-229 to determine fman clock source.
     */

    fman1ClkSrc = (T4_CSR_READ_32(T4_RCWSR(7)) >> 26) & 0x7;

    /*
     * Now get the core cluster group B PLL multiplier
     */

    switch (fman1ClkSrc)
	{
	case 1:
	case 2:
	case 3:
	case 4:
            /*
             * The Fman clock is cluster group B PLL 1 multiplier times
             * the core oscillator frequency, divided by the value specified
             * at RCW bits 227-229.
             */

	    ccRatio = (T4_CSR_READ_32(T4_RCWSR(1)) >> 8) & 0x3f;
	    return ((ccRatio * oscFreq) / fman1ClkSrc);

	case 6:
	case 7:
            /*
             * The Fman clock is cluster group B PLL 2 multiplier times
             * the core oscillator frequency, divided by the value specified
             * at RCW bits 228-229.
             */

	    ccRatio = T4_CSR_READ_32(T4_RCWSR(1)) & 0x3f;
	    return ((ccRatio * oscFreq) / (fman1ClkSrc & 0x3));

	default:
	    break;
	}

    return (0);

#else /* FORCE_DEFAULT_FREQ */
    return (DEFAULT_SYSCLKFREQ);
#endif /* FORCE_DEFAULT_FREQ */
    }

/*****************************************************************************
*
* sysFman2ClkFreqGet - return the frame manager clock frequency
*
* This function calculates the frame manager clock frequency and
* returns it.
*
* The frame manager clock source is selectable via the reset control word.
* It can either be the platform clock frequency divided by two, or the
* core cluster 3 PLL frequency divided by two. For the T4240QDS board,
* the RCW supplied by Freescale uses the platform clock frequency.
*
* RETURNS: frame manager clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 sysFman2ClkFreqGet (void)
    {
#ifndef FORCE_DEFAULT_FREQ
    UINT8 osc;
    UINT32 oscFreq;
    UINT32 ccRatio;
    UINT32 fman2ClkSrc;

    /* Get the base oscillator frequency from the QIXIS. */

    osc = T4_CSR_READ_8(QX_CLK_SPD1) & 0x7;

    switch (osc)
        {
        case QX_SYSCLK_67_MHZ:
            oscFreq = FREQ_67_MHZ;
            break;
        case QX_SYSCLK_83_MHZ:
            oscFreq = FREQ_83_MHZ;
            break;
        case QX_SYSCLK_100_MHZ:
            oscFreq = FREQ_100_MHZ;
            break;
        case QX_SYSCLK_125_MHZ:
            oscFreq = FREQ_125_MHZ;
            break;
        case QX_SYSCLK_133_MHZ:
            oscFreq = FREQ_133_MHZ;
            break;
        case QX_SYSCLK_150_MHZ:
            oscFreq = FREQ_150_MHZ;
            break;
        case QX_SYSCLK_160_MHZ:
            oscFreq = FREQ_160_MHZ;
            break;
        case QX_SYSCLK_167_MHZ:
            oscFreq = FREQ_167_MHZ;
            break;
        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    /*
     * Read RCW bits 506-508 to determine fman clock source.
     */

    fman2ClkSrc = (T4_CSR_READ_32(T4_RCWSR(15)) >> 3) & 0x7;

    switch (fman2ClkSrc)
	{
	case 1:
	case 2:
	case 3:
	case 4:
            /*
             * The Fman clock is cluster group B PLL 2 multiplier times
             * the core oscillator frequency, divided by the value specified
             * at RCW bits 506-508.
             */

	    ccRatio = T4_CSR_READ_32(T4_RCWSR(1)) & 0x3f;
	    return ((ccRatio * oscFreq) / fman2ClkSrc);

	case 6:
	case 7:
            /*
             * The Fman clock is cluster group B PLL 1 multiplier times
             * the core oscillator frequency, divided by the value specified
             * at RCW bits 507-508.
             */

	    ccRatio = (T4_CSR_READ_32(T4_RCWSR(1)) >> 8) & 0x3f;
	    return ((ccRatio * oscFreq) / (fman2ClkSrc & 0x3));

	default:
	    break;
	}

    return (0);

#else /* FORCE_DEFAULT_FREQ */
    return (DEFAULT_SYSCLKFREQ);
#endif /* FORCE_DEFAULT_FREQ */
    }

#ifdef	DEBUG
/*******************************************************************************
*
* coreLbcShow - Show routine for local bus controller
*
* This routine shows the local bus controller registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreLbcShow (void)
    {
    VINT32 tmp, tmp1, tmp2;

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR0_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR0);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK0);
    printf ("Chip-select property CSPR0 = 0x%02x%08x\tAMASK0 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR0);
    printf ("Chip-select option CSOR0 = 0x%x\n", tmp);

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR1_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR1);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK1);
    printf ("Chip-select property CSPR1 = 0x%02x%08x\tAMASK1 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR1);
    printf ("Chip-select option CSOR1 = 0x%x\n", tmp);

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR2_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR2);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK2);
    printf ("Chip-select property CSPR2 = 0x%02x%08x\tAMASK2 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR2);
    printf ("Chip-select option CSOR2 = 0x%x\n", tmp);

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR3_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR3);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK3);
    printf ("Chip-select property CSPR3 = 0x%02x%08x\tAMASK3 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR3);
    printf ("Chip-select option CSOR3 = 0x%x\n", tmp);

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR4_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR4);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK4);
    printf ("Chip-select property CSPR4 = 0x%02x%08x\tAMASK4 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR4);
    printf ("Chip-select option CSOR4 = 0x%x\n", tmp);

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR5_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR5);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK5);
    printf ("Chip-select property CSPR5 = 0x%02x%08x\tAMASK5 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR5);
    printf ("Chip-select option CSOR5 = 0x%x\n", tmp);

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR6_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR6);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK6);
    printf ("Chip-select property CSPR6 = 0x%02x%08x\tAMASK6 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR6);
    printf ("Chip-select option CSOR6 = 0x%x\n", tmp);

    tmp1 = T4_CSR_READ_32(T4_IFC_CSPR7_EXT);
    tmp  = T4_CSR_READ_32(T4_IFC_CSPR7);
    tmp2 = T4_CSR_READ_32(T4_IFC_AMASK7);
    printf ("Chip-select property CSPR7 = 0x%02x%08x\tAMASK7 = 0x%x\n",
	    tmp1, tmp, tmp2);
    tmp  = T4_CSR_READ_32(T4_IFC_CSOR7);
    printf ("Chip-select option CSOR7 = 0x%x\n", tmp);
    }

#define xbit0(x, n)    ((x >> (31 - n)) & 1)  /* 0..31 */
#define xbit32(x, n)   ((x >> (63 - n)) & 1)  /* 32..63 */
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
#ifdef  INCLUDE_L2_CACHE
    int cache;
#endif  /* INCLUDE_L2_CACHE */

    tmp = vxMsrGet();
    printf("MSR - 0x%x\n", tmp);
    printf("      CM-%x GS-%x UCLE-%x SPV-%x CE-%x EE-%x PR-%x FP-%x\n",
           xbit32(tmp,32), xbit32(tmp,35), xbit32(tmp,37), xbit32(tmp,38),
       xbit32(tmp,46), xbit32(tmp,48), xbit32(tmp,49), xbit32(tmp,50));
    printf("      ME-%x FE0-%x DE-%x FE1-%x IS-%x DS-%x PMM-%x RI-%x\n",
           xbit32(tmp,51), xbit32(tmp,52), xbit32(tmp,54), xbit32(tmp,55),
       xbit32(tmp,58), xbit32(tmp,59), xbit32(tmp,61), xbit32(tmp,62));
    tmp = vxHid0Get();
    printf("HID0 = 0x%x\n", tmp);
    tmp = vxL1CSR0Get();
    printf("L1CSR0: Dcache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
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

#ifdef  INCLUDE_L2_CACHE
    for (cache = 0; cache < L2_CACHE_NUMBERS; cache++)
        {
        tmp = vxL2CacheRegGet (L2CFG0(cache, CCSBAR));
        printf("L2CFG0 (cluster %d) - 0x%x\n", cache, tmp);
        printf("        l2cteha-%x l2cdeha-%x l2cidpa-%x l2cbsize-%x\n",
                   (xbit32(tmp,33)<<1)|xbit32(tmp,34),
                   (xbit32(tmp,35)<<1)|xbit32(tmp,36), xbit32(tmp,37),
                   (xbit32(tmp,38)<<2)|(xbit32(tmp,39)<<1)|xbit32(tmp,40));
        
        printf("        l2crepl-%x l2cla-%x l2cnway-%x l2csize-%x\n",
                   (xbit32(tmp,41)<<1)|xbit32(tmp,42), xbit32(tmp,43),
                   (tmp & 0x0007C000)>>14, tmp & 0x3fff);
        tmp = vxL2CacheRegGet (L2CSR0(cache, CCSBAR));
        printf("L2CSR0 (cluster %d) - 0x%x\n", cache, tmp);
        printf("        l2 is %s\n", tmp&0x80000000?"ON":"OFF");
        printf("        l2pe-%x l2wp-%x l2io-%x l2do-%x\n",
                   xbit32(tmp,33),
                   (xbit32(tmp,35)<<2)|(xbit32(tmp,36)<<1)|xbit32(tmp,37),
                   xbit32(tmp,43), xbit32(tmp,47));
        printf("        l2rep-%x l2loa-%x l2lo-%x\n",
                   (xbit32(tmp,50)<<1)|xbit32(tmp,51),
                   xbit32(tmp,56), xbit32(tmp,58));
        tmp = vxL2CacheRegGet (L2CSR1(cache, CCSBAR));
        printf("L2CSR1 (cluster %d) - 0x%x\n", cache, tmp);
        }
#endif  /* INCLUDE_L2_CACHE */

#ifdef  INCLUDE_L3_CACHE
    for (cache = 0; cache < L3_CACHE_NUMBERS; cache++)
        {
        tmp = T4_CSR_READ_32(CPCCSR0(L3_CACHE_REG_BASE_ADDR, cache));
        printf("CPCCSR0 (CPC%d) - 0x%x\n", cache, tmp);
        printf("        l3 is %s\n", tmp&0x80000000?"ON":"OFF");
        printf("        cpcpe-%x cpcfi-%x cpcwt-%x cpcrep-%x cpcfl-%x\n",
                   xbit0(tmp,1), xbit0(tmp,10), xbit0(tmp,12),
                   (xbit0(tmp,18)<<1)|xbit0(tmp,19), xbit0(tmp,20));
        printf("        cpclfc-%x cpcloa-%x cpclo-%x\n",
                   xbit0(tmp,21), xbit0(tmp,24), xbit0(tmp,26));
        }
#endif  /* INCLUDE_L3_CACHE */

    printf("Sys-to-CCB multiplier: %d\n", multiSysToCCB);
    printf("Sys-to-DDR multiplier: %d\n", multiSysToDDR);

    for (n = 0; n < MAX_CPUS; n++)
        printf("Sys-to-CC%d multiplier: %d\n", n+1, multiSysToCC[n]);

    printf("\r\nSystem Freq = %-4dMHz\n",systemFreq/FREQ_1_MHZ);
    printf("CCB Freq = %-4dMHz\n",sysClkFreqGet()/FREQ_1_MHZ);
    printf("DDR Freq = %-4dMHz\n",ddrFreq/FREQ_1_MHZ);
    printf("Frame manager Freq: %-4dMHz\n", sysFman1ClkFreqGet ()/FREQ_1_MHZ);

    for (n = 0; n < MAX_CPUS; n++)
        printf("Core CPU%d Freq = %-5dMHz\n",n,coreFreq[n]/FREQ_1_MHZ);

    }
#endif	/* DEBUG */

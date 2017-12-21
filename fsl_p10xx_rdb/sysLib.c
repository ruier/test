/* sysLib.c - Freescale P10XXRDB board system-dependent library */

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
01f,20may13,wyt  Added support for P1023RDB.
01e,12nov12,x_s  Fix the AMP device filter. (WIND00387606)
01d,10sep12,syt  fixed sysProcNumGet() implementation error.(WIND00374570) 
01c,07aug12,syt  added support for P1021RDB and P1024RDB
01b,18jul12,syt  added enable access to eLBC bus before starting core1
                 in sysCpuEnable()
01a,23mar12,syt  derived from version 01h of fsl_p1020_rdb/sysLib.c
*/

/*
DESCRIPTION
This library provides board-specific routines for P10XXRDB

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    sysFslSpi.c         - FreeScale SPI controller support.
    spS25flxxSpiFlash.c - S25FL128P SPI flash driver.
    sysL2Cache.c        - L2 cache support.
    usbPciStub.c        - USB Host/Device controller support.

INCLUDE FILES: sysLib.h

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/* includes */

#include <vxWorks.h>
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
#include <hwif/vxbus/vxBus.h>

#ifdef _WRS_CONFIG_SMP
#   include <private/cpcLibP.h>
#   include <private/kernelLibP.h>              /* KERNEL_ENTERED_ME() */
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_AMP
#   ifndef _ASMLANGUAGE
#       include <vxIpiLib.h>
#   endif
#endif  /* INCLUDE_AMP */

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SPE
#   include <speLib.h>
#endif  /* INCLUDE_SPE */

/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [0x20] = {};
int sysStaticTlbDescNumEnt = 0;

TLB_ENTRY_DESC sysStaticTlbDesc_Default [] = {

    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash */

    /* needed be first entry here */

     {
     (FLASH_BASE_PART1 + 0x400000), 0x0, 
     (FLASH_BASE_PART1 + 0x400000),
     _MMU_TLB_TS_0   | _MMU_TLB_SZ_4M   | _MMU_TLB_IPROT |
     _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
     _MMU_TLB_ATTR_G
     },
    /* TLB #1.  DDR3 RAM */

    {
    PHYS_MEM_START, 0x0, PHYS_MEM_START,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G      | _MMU_TLB_PERM_W |
    _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },

    /* TLB #2.  CCSRs  */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W  | _MMU_TLB_IPROT
    },

    /* TLB #3.  CPLDs */

    {
    CPLD_BASE, 0x0, CPLD_BASE,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16K  | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W  | _MMU_TLB_IPROT
    },

    /* TLB #4.  VSC7385 */

    {
    VSC7385_BASE, 0x0, VSC7385_BASE,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_64K  | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W  | _MMU_TLB_IPROT
    },

    /* TLB #5.  VSC7385 */

    {
    (VSC7385_BASE + 0x10000), 0x0, (VSC7385_BASE + 0x10000),
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_64K   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W   | _MMU_TLB_IPROT
    },

    /* TLB #6.  Flash */

    {
     FLASH_BASE_PART1, 0x0,
     FLASH_BASE_PART1,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_4M   | _MMU_TLB_PERM_W |
    _MMU_TLB_PERM_X  | _MMU_TLB_ATTR_I  |  _MMU_TLB_ATTR_G
    },

    /* TLB #7.  Flash */
    
     {
     FLASH_BASE_PART0_DEFAULT, 0x0, FLASH_BASE_PART0_DEFAULT,
     _MMU_TLB_TS_0   | _MMU_TLB_SZ_4M   |  _MMU_TLB_PERM_W |
     _MMU_TLB_PERM_X  | _MMU_TLB_ATTR_I  |  _MMU_TLB_ATTR_G
     }, 
     

#ifdef INCLUDE_L2_SRAM

    /* L2 SRAM */

    {
    L2SRAM_ADDR, 0x0, L2SRAM_ADDR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256K  | _MMU_TLB_PERM_W  |
    _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I   | _MMU_TLB_ATTR_G
    },
#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_PCI_BUS
    {
    PCIEX1_MEM_ADRS, 0x0, PCIEX1_MEM_ADRS,
    _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },

    {
    PCIEX2_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS,
    _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
#endif  /* INCLUDE_PCI_BUS */

};

int sysStaticTlbDescNumEnt_Default = NELEMENTS (sysStaticTlbDesc_Default);

TLB_ENTRY_DESC sysStaticTlbDesc_P1023 [] = {

    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* DDR3 512M */

    {
    PHYS_MEM_START, 0x0, PHYS_MEM_START,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M      | _MMU_TLB_PERM_W |
    _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },
    {
    PHYS_MEM_START + 0x10000000, 0x0, PHYS_MEM_START + 0x10000000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M      | _MMU_TLB_PERM_W |
    _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },

    /* CCSR space 2M */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W  | _MMU_TLB_IPROT
    },
    {
    CCSBAR + 0x100000, 0x0, CCSBAR + 0x100000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W  | _MMU_TLB_IPROT
    },

    /* nor flash  0xfc000000 - 0xfeffffff and 0xff800000 - 0xffffffff */

    {
    FLASH_BASE_PART0_P1023, 0x0, FLASH_BASE_PART0_P1023,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },
    {
    FLASH_BASE_PART0_P1023 + 0x1000000, 0x0, FLASH_BASE_PART0_P1023 + 0x1000000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },
    {
    FLASH_BASE_PART0_P1023 + 0x2000000, 0x0, FLASH_BASE_PART0_P1023 + 0x2000000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },
    {
    FLASH_BASE_PART1, 0x0, FLASH_BASE_PART1,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_4M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },
    {
    FLASH_BASE_PART1 + 0x400000, 0x0, FLASH_BASE_PART1 + 0x400000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_4M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },

#ifdef INCLUDE_PCI_BUS
     {
    PCIEX1_MEM_ADRS, 0x0, PCIEX1_MEM_ADRS,
    _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
    {
    PCIEX2_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS,
    _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
     {
    PCIEX3_MEM_ADRS, 0x0, PCIEX3_MEM_ADRS,
    _MMU_TLB_TS_0   | PCI_MMU_TLB_SZ    | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
#endif  /* INCLUDE_PCI_BUS */

#ifdef DRV_RESOURCE_QORIQBMAN
    {
    BMAN_LAW_BASE, 0x0, BMAN_LAW_BASE,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | _MMU_TLB_ATTR_M |
    _MMU_TLB_PERM_W
    },
    {
    BMAN_LAW_BASE + 0x100000, 0x0, BMAN_LAW_BASE + 0x100000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
#endif

#ifdef DRV_RESOURCE_QORIQQMAN
    {
    QMAN_LAW_BASE, 0x0, QMAN_LAW_BASE,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | _MMU_TLB_ATTR_M |
    _MMU_TLB_PERM_W
    },
    {
    QMAN_LAW_BASE + 0x100000, 0x0, QMAN_LAW_BASE + 0x100000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
#endif
};

int sysStaticTlbDescNumEnt_P1023 = NELEMENTS (sysStaticTlbDesc_P1023);

/* macro to compose 64-bit PHYS_ADDRs */

#define PHYS_64BIT_ADDR(h, l)   (((PHYS_ADDR)(h) << 32) + (l))

/*
* sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
* used by the MMU to translate addresses with single page (4k) granularity.
* PTE memory space should not, in general, overlap BAT memory space but
* may be allowed if only Data or Instruction access is mapped via BAT.
*
* PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
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
PHYS_MEM_DESC sysPhysMemDesc [0x40] = {};
int sysPhysMemDescNumEnt = 0;

PHYS_MEM_DESC sysPhysMemDesc_Default [] = {

#if !defined(INCLUDE_AMP) || \
    (defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00))
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    PHYS_MEM_SIZE_DEFAULT,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | \
    VM_STATE_MEM_COHERENCY
    },

#else /* AMP && !INCLUDE_AMP_CPU_00 */
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | \
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) CPU1_FUNC_START_ADR& ~(MMU_PAGE_SIZE -1),
    (PHYS_ADDR) CPU1_FUNC_START_ADR & ~(MMU_PAGE_SIZE -1),
    MMU_PAGE_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | \
    VM_STATE_MEM_COHERENCY
    },

#endif /*  !AMP || (AMP && INCLUDE_AMP_CPU_00) */

#ifdef INCLUDE_L2_SRAM
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    L2SRAM_WINDOW_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    },
#endif /* INCLUDE_L2_SRAM */

    {
    (VIRT_ADDR) CPLD_BASE,
    (VIRT_ADDR) CPLD_BASE,
    CPLD_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) VSC7385_BASE,
    (PHYS_ADDR) VSC7385_BASE,
    VSC7385_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    CCSR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED
    },

#ifdef INCLUDE_PCI_BUS
    {
    (VIRT_ADDR) PCIEX1_MEM_ADRS,
    (PHYS_ADDR) PCIEX1_MEM_ADRS,
    PCIEX1_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX1_MEMIO_ADRS,
    PCIEX1_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_IO_ADRS,
    (PHYS_ADDR) PCIEX1_IO_ADRS,
    PCIEX1_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEM_ADRS,
    (PHYS_ADDR) PCIEX2_MEM_ADRS,
    PCIEX2_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
    PCIEX2_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_IO_ADRS,
    (PHYS_ADDR) PCIEX2_IO_ADRS,
    PCIEX2_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

#endif /* INCLUDE_PCI_BUS */

    {
    (VIRT_ADDR) FLASH_BASE_PART0_DEFAULT,
    (PHYS_ADDR) FLASH_BASE_PART0_DEFAULT,
    FLASH_SIZE_PART0_DEFAULT,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED    | VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) FLASH_BASE_PART1,
    (PHYS_ADDR) FLASH_BASE_PART1,
    FLASH_SIZE_PART1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED    | VM_STATE_MEM_COHERENCY
    },
};

int sysPhysMemDescNumEnt_Default = NELEMENTS(sysPhysMemDesc_Default);

PHYS_MEM_DESC sysPhysMemDesc_P1023 [] = {

#if !defined(INCLUDE_AMP) || \
    (defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00))
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    PHYS_MEM_SIZE_P1023,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | \
    VM_STATE_MEM_COHERENCY
    },

#else /* AMP && !INCLUDE_AMP_CPU_00 */
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | \
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) CPU1_FUNC_START_ADR& ~(MMU_PAGE_SIZE -1),
    (PHYS_ADDR) CPU1_FUNC_START_ADR & ~(MMU_PAGE_SIZE -1),
    MMU_PAGE_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | \
    VM_STATE_MEM_COHERENCY
    },
#endif /*  !AMP || (AMP && INCLUDE_AMP_CPU_00) */

    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    CCSR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) FLASH_BASE_PART0_P1023,
    (PHYS_ADDR) FLASH_BASE_PART0_P1023,
    FLASH_SIZE_PART0_P1023,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) FLASH_BASE_PART1,
    (PHYS_ADDR) FLASH_BASE_PART1,
    FLASH_SIZE_PART1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED
    },

#ifdef INCLUDE_PCI_BUS
    {
    (VIRT_ADDR) PCIEX2_MEM_ADRS,
    (PHYS_ADDR) PCIEX2_MEM_ADRS,
    PCIEX2_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
    PCIEX2_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_IO_ADRS,
    (PHYS_ADDR) PCIEX2_IO_ADRS,
    PCIEX2_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_MEM_ADRS,
    (PHYS_ADDR) PCIEX1_MEM_ADRS,
    PCIEX1_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX1_MEMIO_ADRS,
    PCIEX1_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_IO_ADRS,
    (PHYS_ADDR) PCIEX1_IO_ADRS,
    PCIEX1_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX3_MEM_ADRS,
    (PHYS_ADDR) PCIEX3_MEM_ADRS,
    PCIEX3_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX3_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX3_MEMIO_ADRS,
    PCIEX3_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX3_IO_ADRS,
    (PHYS_ADDR) PCIEX3_IO_ADRS,
    PCIEX3_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },
#endif /* INCLUDE_PCI_BUS */

#ifdef DRV_RESOURCE_QORIQBMAN

    /*
     * The buffer manager access area is 2MB wide, with the
     * first 1MB set as cacheable and the second 1MB set
     * as cache inhibited.
     */

    {
    (VIRT_ADDR) BMAN_LAW_BASE,
    (PHYS_ADDR) BMAN_LAW_BASE,
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE       |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) (BMAN_LAW_BASE + 0x100000),
    (PHYS_ADDR) (BMAN_LAW_BASE + 0x100000),
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN

    /*
     * The queue manager access area is 2MB wide, with the
     * first 1MB set as cacheable and the second 1MB set
     * as cache inhibited.
     */

    {
    (VIRT_ADDR) QMAN_LAW_BASE,
    (PHYS_ADDR) QMAN_LAW_BASE,
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE       |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) (QMAN_LAW_BASE + 0x100000),
    (PHYS_ADDR) (QMAN_LAW_BASE + 0x100000),
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },

#endif /* DRV_RESOURCE_QORIQQMAN */
};

int sysPhysMemDescNumEnt_P1023 = NELEMENTS(sysPhysMemDesc_P1023);

UINT32 lawRegisterTable [M85XX_LAW_SIZE][2] = {
};
int lawNum = 0;


UINT32 lawRegisterTable_Default [][2] = {
    { FLASH_BASE_PART0_DEFAULT >> 12,
      (QORIQ_TGTID_ELBC << 20) | LAWAR_SIZE_16MB | QORIQ_LAWAR_ENABLE },
    { CPLD_BASE >> 12,
      (QORIQ_TGTID_ELBC << 20) | LAWAR_SIZE_1MB | QORIQ_LAWAR_ENABLE },
    { VSC7385_BASE >> 12,
      (QORIQ_TGTID_ELBC << 20) | LAWAR_SIZE_128KB | QORIQ_LAWAR_ENABLE },
    { PCIEX1_LAW_BASE >> 12,
      (QORIQ_TGTID_PCIE1 << 20) | LAWAR_SIZE_256MB | QORIQ_LAWAR_ENABLE },
    { PCIEX2_LAW_BASE >> 12,
      (QORIQ_TGTID_PCIE2 << 20) | LAWAR_SIZE_256MB | QORIQ_LAWAR_ENABLE },
    { LOCAL_MEM_LOCAL_ADRS >> 12,
      (QORIQ_TGTID_DDR << 20) | LAWAR_SIZE_1GB | QORIQ_LAWAR_ENABLE },
};

int lawNum_Default = NELEMENTS(lawRegisterTable_Default);

/*
 * The DPAA's window must be configured as an 8 Mbyte window at address
 * 0xFF00_0000. It is required that this LAW is overlapped by the default
 * CCSR space in the most significant 2 Mbytes. The NOR flash is addressed
 * at 0xFE00_0000. The DPAA and CCSR's windows overlap the window of the
 * NOR flash. If two or more LAWs overlap, the lower numbered window takes
 * precedence. Therefore, we set access LAWs for DPAA and CCSR before NOR
 * flash.
 */

UINT32 lawRegisterTable_P1023 [][2] = {
    { QMAN_LAW_BASE >> 12,
      (QORIQ_TGTID_DPAA << 20) | LAWAR_SIZE_4MB | QORIQ_LAWAR_ENABLE },
    { FLASH_BASE_PART0_P1023 >> 12,
      (QORIQ_TGTID_ELBC << 20) | LAWAR_SIZE_64MB | QORIQ_LAWAR_ENABLE },
    { PCIEX1_LAW_BASE >> 12,
      (QORIQ_TGTID_PCIE1 << 20) | LAWAR_SIZE_256MB | QORIQ_LAWAR_ENABLE },
    { PCIEX2_LAW_BASE >> 12,
      (QORIQ_TGTID_PCIE2 << 20) | LAWAR_SIZE_256MB | QORIQ_LAWAR_ENABLE },
    { PCIEX3_LAW_BASE >> 12,
      (QORIQ_TGTID_PCIE3 << 20) | LAWAR_SIZE_256MB | QORIQ_LAWAR_ENABLE },
    { LOCAL_MEM_LOCAL_ADRS >> 12,
      (QORIQ_TGTID_DDR << 20) | LAWAR_SIZE_512MB | QORIQ_LAWAR_ENABLE },
};

int lawNum_P1023 = NELEMENTS(lawRegisterTable_P1023);

int     sysBus      = BUS_TYPE_NONE;    /* system bus type */
int     sysCpu      = CPU;              /* system CPU type */
char *  sysBootLine = BOOT_LINE_ADRS;   /* address of boot line */
char *  sysExcMsg   = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum  = 0;                /* processor number of this CPU */

UINT32  inFullVxWorksImage = FALSE;
UINT32  coreFreq;
UINT32  core1Freq;
UINT32  ddrFreq;
UINT32  systemFreq;

UINT32  sysClkFreqGet(void);
char *  sysModel (void);
UINT32  ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;

#ifdef _WRS_CONFIG_SMP
#   if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;                      /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;                      /* lower 32 bit value of timebase */
#   endif   /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
#endif /* _WRS_CONFIG_SMP */

/* Clock Ratio Tables */

#define MAX_VALUE_DDR_RATIO     16
UINT32 ddrRatioTable[MAX_VALUE_DDR_RATIO] = {
    0,  0,  0,  3,  4,  0, 6,  0,
    8,  0,  10, 0,  12, 0, 14, 0
};

#define MAX_VALUE_PLAT_RATIO    32
UINT32 platRatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 3, 0 },
    { 4,  0 }, { 5, 0 }, { 6,  0 }, { 0, 0 },
    { 8,  0 }, { 9, 0 }, { 10, 0 }, { 0, 0 },
    { 12, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 }
};

#define MAX_VALUE_E500_RATIO    10
UINT32 e500RatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0, 0 }, { 0, 0 }, { 1, 0 }, { 3, 1 }, { 2, 0 },
    { 5, 1 }, { 3, 0 }, { 7, 1 }, { 4, 0 }, { 9, 1 }
};

/* externs */

IMPORT char _wrs_kernel_data_start[];
IMPORT void mmuE500TlbDynamicInvalidate();
IMPORT void mmuE500TlbStaticInvalidate();
IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, BOOL cacheAllow);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;
IMPORT void sysIvprSet(UINT32);
IMPORT device_method_t * pSysPlbMethods;

IMPORT void (*_vxb_delayRtn)();
IMPORT void (*_vxb_msDelayRtn)();
IMPORT void (*_vxb_usDelayRtn)();

IMPORT BOOL e500_spe_exc_enable;
IMPORT BOOL e500_spe_exc_mask;

IMPORT UINT8 _binary_vsc2bin_start[];
IMPORT UINT8 _binary_vsc2bin_end[];

#ifdef INCLUDE_P10XX_FMAN_UCODE
IMPORT  UINT8 _binary_fsl_fman_ucode_P1023_160_13_bin_start[];
#endif /* INCLUDE_P10XX_FMAN_UCODE */

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

IMPORT void vxSdaInit (void);

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP();
IMPORT void vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheAimE500FlushAllD (void);
IMPORT STATUS cacheAimE500DisableDFromMmu (void);
IMPORT STATUS cacheAimE500DisableIFromMmu (void);
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheArchDisableFromMmu (CACHE_TYPE cache);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void   sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
IMPORT STATUS vxbIntToCpuRoute (unsigned int cpu);
#endif  /* _WRS_CONFIG_SMP */

IMPORT void hardWareInterFaceInit();

#ifdef  INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif   /* INCLUDE_SIO_UTILS */

IMPORT void sysCpu1Start (void);
IMPORT void sysCpu1Stop (void);

#ifdef INCLUDE_SPE
IMPORT int  (* _func_speProbeRtn) ();
#endif /* INCLUDE_SPE */

IMPORT void sysL1Csr1Set(UINT32);
IMPORT UINT sysTimeBaseLGet(void);
IMPORT void kwriteSioInit (int, int);
IMPORT void hardWareInterFaceInit (void);

/* forward declarations */

VOID    sysUsDelay(int);
VOID    sysMsDelay(UINT);
STATUS  sysIntEnablePIC(int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable (int intNum);
STATUS  sysCascadeIntDisable(int intNum);

#ifdef  _WRS_CONFIG_SMP
LOCAL   void sysCpu1ShutdownLoop (void);
LOCAL   UINT32 physCpuAvailableGet (void);
#   if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))
STATUS  sysSmpTimeBaseSync (void);
#   endif   /* DRV_TIMER_DEC_PPC && INCLUDE_SYS_TIMESTAMP */
#endif  /* _WRS_CONFIG_SMP */

LOCAL   BOOL sysCpu1Check (void);

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL   void sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_L2_SRAM
LOCAL   void sysL2SramEnable(BOOL both);
#endif  /* INCLUDE_L2_SRAM */

METHOD_DECL(sysBspDevFilter);
LOCAL STATUS  sysDeviceFilter(VXB_DEVICE_ID pDev);

#ifdef INCLUDE_AMP
void    sysAmpCpuStatusSet(int status,int cpu);
int     sysAmpCpuStatusGet(int cpu);
LOCAL UINT64 sysVirtToPhys (void * pVirt);
LOCAL void * sysPhysToVirt (UINT64 pPhys);
#endif  /* INCLUDE_AMP */
LOCAL UINT32 sysFmanClkFreqGet (void);

DEVMETHOD_DEF(sysVirtToPhys, "BSP virtual to physical address translation");
DEVMETHOD_DEF(sysPhysToVirt, "BSP physical to virtual address translation");
DEVMETHOD_DEF(sysFmanClkFreqGet, "Get Fman clock frequency");

LOCAL struct vxbDeviceMethod p10xxPlbMethods[] = {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysCpu1Check),
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
#ifdef INCLUDE_AMP
    /* Only used for AMP images running above 32-bit boundary. */

    DEVMETHOD(sysVirtToPhys, sysVirtToPhys),
    DEVMETHOD(sysPhysToVirt, sysPhysToVirt),
#endif  /* INCLUDE_AMP */
    DEVMETHOD(sysFmanClkFreqGet, (FUNCPTR)sysFmanClkFreqGet),
    { 0, 0 }
};

UINT32 sysTimerClkFreq;
UINT32 sysBoardModel;
LOCAL char * physTop = NULL;
LOCAL char * memTop  = NULL;

#ifdef IEEE754_FIX
LOCAL SPE_CONTEXT *speCtx;
#endif  /* IEEE754_FIX */

/* includes */

#if defined(INCLUDE_L2_CACHE) || defined(INCLUDE_L2_SRAM)
#   include "sysL2Cache.c"
#endif  /* INCLUDE_L2_CACHE */

#ifdef INCLUDE_FLASH
#   include "nvRamToFlash.c"
#else  /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif  /* INCLUDE_FLASH */

#ifdef INCLUDE_NETWORK
#   include "sysNet.c"
#endif  /* INCLUDE_NETWORK */

#include "hwconf.c"

#ifdef DRV_SPI_S25FLXX
#   include "sysFslSpi.c"
#   include "spS25flxxSpiFlash.c"
#endif  /* DRV_SPI_S25FLXX */

/* defines */

#define SYS_MODEL_MASK          0xfffffff0
#define SYS_MODEL_P1025E        "Freescale P1025E - Security Engine"
#define SYS_MODEL_P1025         "Freescale P1025"
#define SYS_MODEL_P1024E        "Freescale P1024E - Security Engine"
#define SYS_MODEL_P1024         "Freescale P1024"
#define SYS_MODEL_P1021E        "Freescale P1021E - Security Engine"
#define SYS_MODEL_P1021         "Freescale P1021"
#define SYS_MODEL_P1015E        "Freescale P1015E - Security Engine"
#define SYS_MODEL_P1015         "Freescale P1015"
#define SYS_MODEL_P1016E        "Freescale P1016E - Security Engine"
#define SYS_MODEL_P1016         "Freescale P1016"
#define SYS_MODEL_P1012E        "Freescale P1012E - Security Engine"
#define SYS_MODEL_P1012         "Freescale P1012"
#define SYS_MODEL_P1023E        "Freescale P1023E - Security Engine"
#define SYS_MODEL_P1023         "Freescale P1023"
#define SYS_MODEL_P1017E        "Freescale P1017E - Security Engine"
#define SYS_MODEL_P1017         "Freescale P1017"
#define SYS_MODEL_E500          "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN       "Freescale Unknown processor"
#define SYS_MODEL               sysModel()

#define P1025RDB                0
#define P1024RDB                1
#define P1021RDB                2
#define P1023RDB                3


#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000

#ifdef INCLUDE_AMP

/* definitions for core cluster ID setting in PIR */

#define PIR_CORE_CLUSTER_ID_SHIFT   5
#define PIR_CORE_CLUSTER_ID_MASK    0x1f
#define PIR_CORE_CLUSTER_ID_SET(id) \
    ((id & PIR_CORE_CLUSTER_ID_MASK) << PIR_CORE_CLUSTER_ID_SHIFT)

typedef struct
{
    char* deviceName;
    int   unit;
    int   cpu;
    } AMP_CPU_TABLE;

/*
 * Table includes only devices we want to filter
 * if no action required for a device per CPU then it's not in the table
 */

AMP_CPU_TABLE p1021rdbAmpCpuTable[] = {
#ifdef INCLUDE_ETSEC_VXB_END
    { ETSEC_NAME,           0,  ETSEC0_CORE_NUM },
    { ETSEC_NAME,           1,  ETSEC1_CORE_NUM },
    { ETSEC_NAME,           2,  ETSEC2_CORE_NUM },
#endif /* INCLUDE_ETSEC_VXB_END */
    { "ns16550",            0,  DEVICE_CORE0 },
    { "ns16550",            1,  DEVICE_CORE0 },
    { "norFlash",           0,  DEVICE_CORE0 },
    { "fslI2c",             0,  DEVICE_CORE0 },
    { "fslI2c",             1,  DEVICE_CORE0 },
    { "openPicTimer",       0,  DEVICE_CORE0 },
    { "openPicTimer",       1,  DEVICE_CORE0 },
    { "m85xxCCSR",          0,  DEVICE_CORE0 },
    { "m85xxPci",           0,  DEVICE_CORE0 },
    { "m85xxPci",           1,  DEVICE_CORE0 },
    { "vxbPlbUsbEhci",      0,  DEVICE_CORE0 },
    { "fslDma",             0,  DEVICE_CORE0 },
    { "sdhci",              0,  DEVICE_CORE0 },
};

AMP_CPU_TABLE p1023rdbAmpCpuTable[] = {
    { "ns16550",            0,  DEVICE_CORE0 },
    { "ns16550",            1,  DEVICE_CORE0 },
    { "norFlash",           0,  DEVICE_CORE0 },
    { "fslI2c",             0,  DEVICE_CORE0 },
    { "openPicTimer",       0,  DEVICE_CORE0 },
    { "openPicTimer",       1,  DEVICE_CORE0 },
    { "vxbPlbUsbEhci",      0,  DEVICE_CORE0 },
    { "dtsec",              0,  DEVICE_CORE0 },
    { "dtsec",              1,  DEVICE_CORE1 },
    { "QorIQPciEx",         0,  DEVICE_CORE0 },
    { "QorIQPciEx",         1,  DEVICE_CORE0 },
    { "QorIQPciEx",         2,  DEVICE_CORE0 },
};

AMP_CPU_TABLE p1024rdbAmpCpuTable[] = {
#ifdef INCLUDE_ETSEC_VXB_END
    { ETSEC_NAME,           0,  ETSEC0_CORE_NUM },
    { ETSEC_NAME,           1,  ETSEC1_CORE_NUM },
    { ETSEC_NAME,           2,  ETSEC2_CORE_NUM },
#endif /* INCLUDE_ETSEC_VXB_END */
    { "ns16550",            0,  DEVICE_CORE0 },
    { "ns16550",            1,  DEVICE_CORE0 },
    { "norFlash",           0,  DEVICE_CORE0 },
    { "fslI2c",             0,  DEVICE_CORE0 },
    { "fslI2c",             1,  DEVICE_CORE0 },
    { "openPicTimer",       0,  DEVICE_CORE0 },
    { "openPicTimer",       1,  DEVICE_CORE0 },
    { "m85xxCCSR",          0,  DEVICE_CORE0 },
    { "m85xxPci",           0,  DEVICE_CORE0 },
    { "m85xxPci",           1,  DEVICE_CORE0 },
    { "vxbPlbUsbEhci",      0,  DEVICE_CORE0 },
    { "fslDma",             0,  DEVICE_CORE0 },
    { "sdhci",              0,  DEVICE_CORE0 },
};

AMP_CPU_TABLE p1025rdbAmpCpuTable[] = {
#ifdef INCLUDE_ETSEC_VXB_END
    { ETSEC_NAME,           0,  ETSEC0_CORE_NUM },
    { ETSEC_NAME,           1,  ETSEC1_CORE_NUM },
    { ETSEC_NAME,           2,  ETSEC2_CORE_NUM },
#endif /* INCLUDE_ETSEC_VXB_END */
    { "ns16550",            0,  DEVICE_CORE0 },
    { "ns16550",            1,  DEVICE_CORE0 },
    { "quiccEngine",        0,  DEVICE_CORE0 },
    { "quiccEngineIntCtlr", 0,  DEVICE_CORE0 },
    { "qefcc",              0,  DEVICE_CORE0 },
    { "qefcc",              1,  DEVICE_CORE0 },
    { "qeUart",             2,  DEVICE_CORE0 },
    { "qeUart",             3,  DEVICE_CORE0 },
    { "norFlash",           0,  DEVICE_CORE0 },
    { "fslI2c",             0,  DEVICE_CORE0 },
    { "fslI2c",             1,  DEVICE_CORE0 },
    { "openPicTimer",       0,  DEVICE_CORE0 },
    { "openPicTimer",       1,  DEVICE_CORE0 },
    { "m85xxCCSR",          0,  DEVICE_CORE0 },
    { "m85xxPci",           0,  DEVICE_CORE0 },
    { "m85xxPci",           1,  DEVICE_CORE0 },
    { "vxbPlbUsbEhci",      0,  DEVICE_CORE0 },
    { "fslDma",             0,  DEVICE_CORE0 },
    { "sdhci",              0,  DEVICE_CORE0 },
};

#endif /* INCLUDE_AMP */

typedef struct {
    char* deviceName;
    int   unit;
    } BSP_DEV_INCLUDE_TABLE;

/*
 * The common BSP fsl_p10xx_rdb support a series of Freescale's P10XXRDB, 
 * such as P1021RDB, P1023RDB, P1024RDB and P1025RDB. In this BSP, device
 * drivers needed by these boards are all listed in table hcfDeviceList[], in order to
 * runs VxWorks image on a specified target, useful device drivers need to be
 * selected during the systerm boot process.
 *
 */

/*
 * A select group of device drivers should be be listd in table p10xxExcludeDevTable[].
 * Those unused device drivers should not be listed.
 *
 * Device name              Device unit
 * -----------              -----------
 * "quiccEngine",           0
 * "quiccEngineIntCtlr",    0
 * "qefcc",                 0
 * "qefcc",                 1
 * "qeUart",                2
 * "qeUart",                3
 * "uccMdio",               0
 *
 */

BSP_DEV_INCLUDE_TABLE p1021IncludeDevTable [] ={
    { "ppcIntCtlr",        0 },
    { "epic",              0 },
    { "norFlash",          0 },
    { "fslI2c",            0 },
    { "fslI2c",            1 },
    { "motetsec",          0 },
    { "motetsec",          1 },
    { "motetsec",          2 },
    { "dummyMdio",         0 },
    { "tsecMdio",          0 },
    { "ns16550",           0 },
    { "ns16550",           1 },
    { "m85xxTimerDev",     0 },
    { "openPicTimer",      0 },
    { "openPicTimer",      1 },
    { "vxbPlbUsbEhci",     0 },
    { "fslDma",            0 },
    { "sdhci",             0 },
    { "m85xxCCSR",         0 },
    { "m85xxPci",          0 },
    { "m85xxPci",          1 },
};

BSP_DEV_INCLUDE_TABLE p1024IncludeDevTable [] ={
    { "ppcIntCtlr",        0 },
    { "epic",              0 },
    { "norFlash",          0 },
    { "fslI2c",            0 },
    { "fslI2c",            1 },
    { "motetsec",          0 },
    { "motetsec",          1 },
    { "motetsec",          2 },
    { "tsecMdio",          0 },
    { "ns16550",           0 },
    { "ns16550",           1 },
    { "m85xxTimerDev",     0 },
    { "openPicTimer",      0 },
    { "openPicTimer",      1 },
    { "vxbPlbUsbEhci",     0 },
    { "fslDma",            0 },
    { "sdhci",             0 },
    { "m85xxCCSR",         0 },
    { "m85xxPci",          0 },
    { "m85xxPci",          1 },
};


BSP_DEV_INCLUDE_TABLE p1025IncludeDevTable [] ={
    { "ppcIntCtlr",        0 },
    { "epic",              0 },
    { "quiccEngineIntCtlr",0 },
    { "quiccEngine",       0 },
    { "norFlash",          0 },
    { "fslI2c",            0 },
    { "fslI2c",            1 },
    { "motetsec",          0 },
    { "motetsec",          1 },
    { "motetsec",          2 },
    { "tsecMdio",          0 },
    { "qefcc",             0 },
    { "qefcc",             1 },
    { "uccMdio",           0 },
    { "ns16550",           0 },
    { "ns16550",           1 },
    { "qeUart",            2 },
    { "qeUart",            3 },
    { "m85xxTimerDev",     0 },
    { "openPicTimer",      0 },
    { "openPicTimer",      1 },
    { "vxbPlbUsbEhci",     0 },
    { "fslDma",            0 },
    { "sdhci",             0 },
	{ "m85xxCCSR",         0 },
	{ "m85xxPci",          0 },
	{ "m85xxPci",          1 },
};

BSP_DEV_INCLUDE_TABLE p1023IncludeDevTable [] ={
    { "ppcIntCtlr",        0 },
    { "epic",              0 },
    { "norFlash",          0 },
    { "ns16550",           0 },
    { "ns16550",           1 },
    { "m85xxTimerDev",     0 },
    { "openPicTimer",      0 },
    { "openPicTimer",      1 },
    { "fslDma",            0 },
    { "QorIQQman",         0 },
    { "QorIQBman",         0 },
    { "QorIQFman",         0 },
    { "dtsec",             0 },
    { "dtsec",             1 },
    { "dtsecMdio",         0 },
    { "QorIQPciEx",        0 },
    { "QorIQPciEx",        1 },
    { "QorIQPciEx",        2 },
};

/*******************************************************************************
*
* sysDeviceFilter - prevent device from being announced
*
* Called from vxbus to prevent device from being announced and therefore used
* by vxWorks. The default is to return OK.
*
* RETURNS: OK or ERROR
*
* \NOMANUAL
*/

LOCAL STATUS sysDeviceFilter
    (
    VXB_DEVICE_ID pDev
    )
    {

    /*
     * Compare devices name and unit number with those you want to remove
     * at run-time. Used here to selectively use devices on one cpu vs another.
     */

    BSP_DEV_INCLUDE_TABLE * pIncludeDevTable = NULL;
    UINT32  deviceIndex;
    UINT32  deviceNumber;
    UINT32  i;
#ifdef INCLUDE_AMP
    BOOL    deviceMatch;
#endif
    if (pDev->pName == NULL)
        return OK;

    /*Do not check devices that are not in the hcfDeviceList[] */

    for (i = 0; i < hcfDeviceNum; i++)
        {
        if (strcmp (pDev->pName, hcfDeviceList[i].devName) == 0)
            break;
        }
    if (i == hcfDeviceNum)
        return OK;

    switch (sysBoardModel)
        {
        case P1021RDB:
            pIncludeDevTable = p1021IncludeDevTable;
            deviceNumber = NELEMENTS(p1021IncludeDevTable);
            break;

        case P1023RDB:
            pIncludeDevTable = p1023IncludeDevTable;
            deviceNumber = NELEMENTS(p1023IncludeDevTable);
            break;

        case P1024RDB:
            pIncludeDevTable = p1024IncludeDevTable;
            deviceNumber = NELEMENTS(p1024IncludeDevTable);
            break;

        case P1025RDB:
            pIncludeDevTable = p1025IncludeDevTable;
            deviceNumber = NELEMENTS(p1025IncludeDevTable);
            break;

         default:
            deviceNumber = 0;
            break;
        }

    for (deviceIndex = 0; deviceIndex < deviceNumber; deviceIndex ++)
        {
        if ( (strcmp(pDev->pName, pIncludeDevTable[deviceIndex].deviceName)==OK) &&
             (pDev->unitNumber == pIncludeDevTable[deviceIndex].unit) )
            break;
        }

    if (deviceIndex == deviceNumber)
        return ERROR;

#ifdef INCLUDE_AMP
    AMP_CPU_TABLE * pAmpCpuTable = NULL;
    deviceMatch = FALSE;

    switch (sysBoardModel)
        {
        case P1021RDB:
            pAmpCpuTable = p1021rdbAmpCpuTable;
            deviceNumber = NELEMENTS(p1021rdbAmpCpuTable);
            break;

        case P1023RDB:
            pAmpCpuTable = p1023rdbAmpCpuTable;
            deviceNumber = NELEMENTS(p1023rdbAmpCpuTable);
            break;

        case P1024RDB:
            pAmpCpuTable = p1024rdbAmpCpuTable;
            deviceNumber = NELEMENTS(p1024rdbAmpCpuTable);
            break;

        case P1025RDB:
            pAmpCpuTable = p1025rdbAmpCpuTable;
            deviceNumber = NELEMENTS(p1025rdbAmpCpuTable);
            break;

        default:
            deviceNumber = 0;
            break;
        }

    for (deviceIndex = 0; deviceIndex < deviceNumber; deviceIndex ++)
        {
        if ( (strcmp(pDev->pName,pAmpCpuTable[deviceIndex].deviceName)==OK) &&
             (pDev->unitNumber==pAmpCpuTable[deviceIndex].unit) )
            {
            deviceMatch = TRUE;
            break;
            }
        }

    /* return OK if no match device - assume pass through */

    if (deviceMatch == FALSE)
        return(OK);

    /* if device match then we need to decide whether to filter */

    if(pAmpCpuTable[deviceIndex].cpu!=(sysCpu1Check()))
        return(ERROR);
#endif /* INCLUDE_AMP */

    return(OK);
    }

#if defined (INCLUDE_SPE)

/*******************************************************************************
*
* sysSpeProbe - Check if the CPU has SPE unit
*
* This routine returns OK if the CPU has an SPE unit in it.
* Presently it assumes available.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

int sysSpeProbe (void)
    {
    return OK;
    }

#endif  /* INCLUDE_SPE */

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

    device = *M85XX_SVR(CCSBAR);
    switch (device & SYS_MODEL_MASK)
        {
        case 0x80EC0310:
            retChar = SYS_MODEL_P1025E;
            break;
        case 0x80E40310:
            retChar = SYS_MODEL_P1025;
            break;
        case 0x80EC0210:
            retChar = SYS_MODEL_P1024E;
            break;
        case 0x80E40210:
            retChar = SYS_MODEL_P1024;
            break;
        case 0x80EC0110:
            retChar = SYS_MODEL_P1021E;
            break;
        case 0x80E40110:
            retChar = SYS_MODEL_P1021;
            break;
        case 0x80ED0310:
            retChar = SYS_MODEL_P1016E;
            break;
        case 0x80E50310:
            retChar = SYS_MODEL_P1016;
            break;
        case 0x80ED0210:
            retChar = SYS_MODEL_P1015E;
            break;
        case 0x80E50210:
            retChar = SYS_MODEL_P1015;
            break;
        case 0x80ED0110:
            retChar = SYS_MODEL_P1012E;
            break;
        case 0x80E50110:
            retChar = SYS_MODEL_P1012;
            break;
        case 0x80FE0010:
            retChar = SYS_MODEL_P1023E;
            break;
        case 0x80F60010:
            retChar = SYS_MODEL_P1023;
            break;
        case 0x80FF0010:
            retChar = SYS_MODEL_P1017E;
            break;
        case 0x80F70010:
            retChar = SYS_MODEL_P1017;
            break;
        default:
            retChar = SYS_MODEL_E500;
            break;
        }

    device = *M85XX_PVR(CCSBAR);

    if ((device & 0xffff0000) != 0x80210000)
        retChar =SYS_MODEL_UNKNOWN;

    return(retChar);
    }

/******************************************************************************
*
* sysBoardModelCheck - check the P10XXRDB board model
*
* This routine to check what is the board model
*
* RETURNS: 0 - P1025RDB  1 - P1024RDB  2 - P1021RDB
*
* ERRNO: N/A
*/

LOCAL UINT32 sysBoardModelCheck (void)
    {
    UINT32 boardModel;

    boardModel = *M85XX_SVR(CCSBAR);

    switch (boardModel & SYS_MODEL_MASK)
        {
        case 0x80EC0310:        /* P1025E */
        case 0x80E40310:        /* P1025 */
        case 0x80ED0310:        /* P1016E */
        case 0x80E50310:        /* P1016 */
            boardModel = P1025RDB;    /* P1016 is the single core version of P1025 */
            break;

        case 0x80EC0210:        /* P1024E */
        case 0x80E40210:        /* P1024 */
        case 0x80ED0210:        /* P1015E */
        case 0x80E50210:        /* P1015 */
            boardModel = P1024RDB;    /* P1016 is the single core version of P1024 */
            break;

        case 0x80EC0110:        /* P1021E */
        case 0x80E40110:        /* P1021  */
        case 0x80ED0110:        /* P1012E */
        case 0x80E50110:        /* P1012 */
            boardModel = P1021RDB;    /* P1012 is the single core version of P1021 */
            break;

        case 0x80FE0010:         /* P1023E */
        case 0x80F60010:         /* P1023 */
        case 0x80FF0010:         /* P1017E */
        case 0x80F70010:         /* P1017 */
            boardModel = P1023RDB;
            break;

        default:
            boardModel = P1025RDB;
            break;
        }

    return  boardModel;
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

/******************************************************************************
*
* sysResourceFixup - fix resource file for fsl_p10xx_rdb board 
*
* This routine fixes resources defined in hwconf.c for fsl_p10xx_rdb board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysResourceFixup (void)
    {
    UINT32  i;

    /*
     * P1025RDB board is supported by default, nothing needs fixed
     * here for it
     */

    switch (sysBoardModel)
        {
        case P1021RDB:
            {
#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
            /*
             * VSC7385 used as ETSEC0's PHY device, so "dummyMdio"
             * should be configured as MII interface
             */

            for (i = 0; i < NELEMENTS(etsec0Resources); i++)
                if (strcmp ("miiIfName", etsec0Resources[i].name) == 0)
                    etsec0Resources[i].u.string = "dummyMdio";
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef DRV_TIMER_M85XX
            /*
             * The clock frquency of FIT timer is same as CCB's
             * frquency is 400MHz, but the default FIT clock frquency is
             * 266.667MHz, so need to be fixed here.
             */

            for (i = 0; i < NELEMENTS(m85xxTimerResources); i++)
                {
                if (strcmp ("fitMinClkRate", m85xxTimerResources[i].name) == 0)
                    m85xxTimerResources[i].u.integer = P1021RDB_FIT_CLK_RATE_MIN;
                if (strcmp ("fitMaxClkRate", m85xxTimerResources[i].name) == 0)
                    m85xxTimerResources[i].u.integer = P1021RDB_FIT_CLK_RATE_MAX;
                }
#endif /* DRV_TIMER_M85XX */
            }
            break;

        case P1024RDB:
            {
#ifdef INCLUDE_I2C_BUS
            /* Only one AT24C01 is connected on I2C bus 2.*/

            for (i = 0; i < NELEMENTS(i2c1Resources); i++)
                {
                if (strcmp ("i2cDev", i2c1Resources[i].name) == 0)
                    i2c1Resources[i].u.addr = (void *)&p1024rdbI2c1DevTbl;
                if (strcmp ("i2cDevNum", i2c1Resources[i].name) == 0)
                    i2c1Resources[i].u.integer = NELEMENTS(p1024rdbI2c1DevTbl);
                }
#endif /* INCLUDE_I2C_BUS */

#ifdef DRV_TIMER_M85XX
            for (i = 0; i < NELEMENTS(m85xxTimerResources); i++)
                {
                if (strcmp ("fitMinClkRate", m85xxTimerResources[i].name) == 0)
                    m85xxTimerResources[i].u.integer = P1024RDB_FIT_CLK_RATE_MIN;
                if (strcmp ("fitMaxClkRate", m85xxTimerResources[i].name) == 0)
                    m85xxTimerResources[i].u.integer = P1024RDB_FIT_CLK_RATE_MAX;
                }
#endif /* DRV_TIMER_M85XX */

            }
            break;

        case P1023RDB:
            {
#ifdef DRV_TIMER_M85XX
            for (i = 0; i < NELEMENTS(m85xxTimerResources); i++)
                {
                if (strcmp ("fitMinClkRate", m85xxTimerResources[i].name) == 0)
                    m85xxTimerResources[i].u.integer = P1023RDB_FIT_CLK_RATE_MIN;
                if (strcmp ("fitMaxClkRate", m85xxTimerResources[i].name) == 0)
                    m85xxTimerResources[i].u.integer = P1023RDB_FIT_CLK_RATE_MAX;
                }
#endif /* DRV_TIMER_M85XX */
            }
            break;

        case P1025RDB:
            break;

        default:
            {
#ifdef INCLUDE_I2C_BUS
            /* PCA9555 is connected on I2C bus 1.*/

            for (i = 0; i < NELEMENTS(i2c0Resources); i++)
                {
                if (strcmp ("i2cDev", i2c0Resources[i].name) == 0)
                    i2c0Resources[i].u.addr = (void *)&p1025rdbI2c0DevTbl;
                if (strcmp ("i2cDevNum", i2c0Resources[i].name) == 0)
                    i2c0Resources[i].u.integer = NELEMENTS(p1025rdbI2c0DevTbl);
                }
#endif /* INCLUDE_I2C_BUS */
            }
            break;
        }
    }

/*******************************************************************************
*
* sysClkFreqGet - returns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet (void)
    {
    UINT32  sysClkFreq;
    UINT32  e500Ratio;
    UINT32  e5001Ratio;
    UINT32  platRatio;
    UINT32  ddrRatio;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);
    ddrRatio = M85XX_PORPLLSR_DDR_RATIO(CCSBAR);

    if ((platRatio >= MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio][0] == 0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

    switch (sysBoardModel)
        {
        case P1021RDB:
        case P1024RDB:
        case P1025RDB:
            systemFreq = SYS_CLK_FREQ;
            break;
        case P1023RDB:
            systemFreq = SYS_CLK_FREQ_P1023;
            break;
        default:
            systemFreq = SYS_CLK_FREQ;
            break;
        }

    ddrFreq = DDR_CLK_FREQ * ddrRatioTable[ddrRatio];
    sysClkFreq = ((UINT32)(systemFreq * platRatioTable[platRatio][0])) >> \
                 ((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    e5001Ratio = M85XX_PORPLLSR_E500_1_RATIO(CCSBAR);

    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0])) >> \
               ((UINT32)e500RatioTable[e500Ratio][1]);
    core1Freq = ((UINT32)(sysClkFreq * e500RatioTable[e5001Ratio][0]))>> \
                ((UINT32)e500RatioTable[e5001Ratio][1]);

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
    UINT32  picClockDivisor /* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
        return sysClkFreqGet() / picClockDivisor;
    else
        return HPCN_RTC_FREQ;   /* 14.318 MHz */
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
* umMiiExpose - Init values to establish correct IO Port configuration.
*
* The routine sets up the pin connections for the functionality required by
* this BSP. The information is obtained from the chip users maunual and the
* board schematics.
*
* RETURNS: NONE
*
* ERROR
*/

void umMiiExpose (void)
    {

    /* QE9 and QE12 need to be set for enabling QE MII managment signals */

    M85XX_PMUXCR |= (MPC85xx_PMUXCR_QE9 | MPC85xx_PMUXCR_QE12);
    }

/*******************************************************************************
*
* umMiiRelease - Init values to establish correct IO Port configuration.
*
* The routine sets up the pin connections for the functionality required by
* this BSP. The information is obtained from the chip users maunual and the
* board schematics.
*
* RETURNS: NONE
*
* ERROR
*/
void umMiiRelease (void)
    {

    /*
     * For P1025, QE12 is muxed with LBCTL, it needs to be released for enabling
     * LBCTL signal for LBC usage.
     */

    M85XX_PMUXCR &= ~MPC85xx_PMUXCR_QE12;
    }

/*******************************************************************************
*
* qeMuxPinSwitchEnable - enable or disable QE/eLBC multiplexing switch
*
* The routine set the QE/eLBC multiplexing switch. For P1025RDB board,
* QE and eLBC pins are multiplexed, I2C to Parallel Port Expander Device
* PCA9555 is designed to control the multiplex pins switchs, so the PCA9555
* should be set before the QE port initialization.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void qeMuxPinSwitchEnable
    (
    BOOL    enable      /* TRUE- enable FALSE - disable*/
    )
    {
    VUINT8   a;

    if (enable == TRUE)
        {
        a = 0xf8;
        vxbI2cByNameWrite("gpio_pca9555", 0, 6, &a, 1);
        vxbI2cByNameWrite("gpio_pca9555", 0, 2, &a, 1);
        }
    else
        {
        a = 0xff;
        vxbI2cByNameWrite("gpio_pca9555", 0, 6, &a, 1);
        vxbI2cByNameWrite("gpio_pca9555", 0, 2, &a, 1);
        }
    sysMsDelay(1);
    }

/*******************************************************************************
*
* qeEndPortEnable - configurate the QEFCC END port's multiplexing IO ports
*
* The routine sets up the pin connections for the QEFCC END ports function
* required by this BSP. The information is obtained from the chip users maunual
* and the board schematics.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void qeEndPortEnable
    (
    int ucc_num
    )
    {
    UINT32    temp;

     /* 
      * Setting GBL enables snooping of data buffers, BD, interrupt queues
      * and free buffer pool.
      */

    *QE_SDMR(CCSBAR) |= QE_SDMR_GLB_1_MSK;

    *QE_CMXUCR1(CCSBAR) = QE_CMXUCR1_UCC1_RX_CLK12 | QE_CMXUCR1_UCC1_TX_CLK9;
    *QE_CMXUCR2(CCSBAR) = QE_CMXUCR2_UCC5_RX_CLK13 | QE_CMXUCR2_UCC5_TX_CLK13;
    *QE_CMXUCR3(CCSBAR) = 0;

    /* 
     * P1025 QE's external pins are muxed with other functions. To enable QE UEC
     * mode, bit QE0 should be set as UCC1 Ethernet mode, QE0 and QE3 should be
     * set as for UCC5 Ethernet mode in PMUXCR register.
     */

    M85XX_PMUXCR= (MPC85xx_PMUXCR_QE0 | MPC85xx_PMUXCR_QE3);

    /* Set ethernet MII clock master */
    /* Both of the QE_UCC1 and QE_UCC5 can be the master of the mii*/

    temp = *QE_CMXGCR(CCSBAR);
    temp &= ~QE_CMXGCR_MII_ENET_MNG_MASK;
    temp |= ((ucc_num - 1) << QE_CMXGCR_MII_ENET_MNG_SHIFT);
    *QE_CMXGCR(CCSBAR) = temp;
    }

/*******************************************************************************
*
* qeSioPortEnable - configurate the QEUCC UART port's multiplexing IO ports
*
* The routine sets up the pin connections for the QEUCC UART function
* required by this BSP. The information is obtained from the chip users maunual
* and the board schematics.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void qeSioPortEnable (void)
    {
     /* 
      * Setting GBL enables snooping of data buffers, BD, interrupt queues
      * and free buffer pool.
      */

    *QE_SDMR(CCSBAR) |= QE_SDMR_GLB_1_MSK;

    /* QE0 Exposes QUICC Engine UCC3 pins*/

    M85XX_PMUXCR |= MPC85xx_PMUXCR_QE0;

    /* connect BRG2 to UCC3 Rx/Tx */

    *QE_CMXUCR1(CCSBAR) &= QE_CMXUCR_HIGH_MASK;
    *QE_CMXUCR1(CCSBAR) |= 0x00000022;

    /* connect BRG6 to UCC7 Rx/Tx */

    *QE_CMXUCR2(CCSBAR) &= QE_CMXUCR_HIGH_MASK;
    *QE_CMXUCR2(CCSBAR) |= 0x00000022;
    }

/*******************************************************************************
*
* sysLocalBusAccessEnable - Enabble or disable access to SOC's local bus
*
* The routine to enabble or disable access SOC's local bus.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

void sysLocalBusAccessEnable
    (
    BOOL    enable      /* TRUE- enable FALSE - disable*/
    )
    {
    if (sysBoardModel == P1025RDB)
        {
        if (enable == TRUE)
            {
            M85XX_PMUXCR &= ~MPC85xx_PMUXCR_QE0;
            qeMuxPinSwitchEnable (FALSE);
            }
        else
            {
            M85XX_PMUXCR |= MPC85xx_PMUXCR_QE0;
            qeMuxPinSwitchEnable (TRUE);
            }
        }
    }

/*******************************************************************************
*
* vsc7385Init - initialize VSC7385 L2 switch
*
* This routine load firmware routine to VSC7385.
*
* RETURNS: OK, or ERROR.
*/

LOCAL STATUS vsc7385Init (void)
    {
    UINT32 size;
    UINT32 i;
    STATUS retVal = OK;
    VUINT8 * VSC7385_image;

    size = (UINT32)&_binary_vsc2bin_end - (UINT32)&_binary_vsc2bin_start;
    VSC7385_image = _binary_vsc2bin_start;

    VSC7385_RESET = 3;

    sysUsDelay(200);

    VSC7385_CTRL = ((1 << 7) | (1 << 3) | (1 << 2) | (1 << 1) | (0 << 0));
    _WRS_ASM(" sync");

    VSC7385_REG_MAP = 1;
    _WRS_ASM(" sync");

    VSC7385_ADDR = 0x0;
    _WRS_ASM(" sync");

    for (i = 0; i < size; i++)
        {
        VSC7385_DATA = VSC7385_image[i];
        _WRS_ASM(" sync");
        }

    VSC7385_ADDR = 0x0;

    for (i = 0; i < size; i++)
        {
        _WRS_ASM(" sync");

        if (VSC7385_DATA != VSC7385_image[i])
            {
            retVal = ERROR;
            break;
            }
        }

    VSC7385_CTRL = ((0 << 7) | (1 << 3) | (0 << 2) | (1 << 1) | (1 << 0));

    sysUsDelay(1000);

    return (retVal);

    }

/*******************************************************************************
*
* dtsecEndPortEnable - configurate the DTSEC END port's multiplexing IO ports
*
* The routine sets up the pin connections for the DTSEC END ports function
* required by this BSP. The information is obtained from the chip users maunual
* and the board schematics.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void dtsecEndPortEnable()
    {
    M85XX_PMUXCR |= MPC85xx_PMUXCR_DTSEC1_1;
    }

/*******************************************************************************
*
* coreLawBarSet - set LAWs for devices
*
* This routine sets local access window registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void coreLawBarSet (void)
    {
    int i;
    int lawIndex;
    UINT32 tgtID[M85XX_LAW_CNT][2];
    VUINT32 * lawBARn;
    VUINT32 * lawARn;

    lawIndex = 0;
    bzero(tgtID, sizeof(tgtID));

    for (i = 0; i < M85XX_LAW_CNT; i++ )
        {
        lawARn = (VUINT32 *)((UINT32)M85XX_LAWAR0(CCSBAR) + i * M85XX_LAW_SIZE);
        lawBARn = (UINT32 *)((UINT32)M85XX_LAWBAR0(CCSBAR) + i * M85XX_LAW_SIZE);
        if (*lawARn & QORIQ_LAWAR_ENABLE)
            tgtID[i][0] = *lawBARn;
            tgtID[i][1] = *lawARn & QORIQ_LAWAR_TGTID;
        }

entry:
    for (i = 0; i < M85XX_LAW_CNT; i++ )
        {

        if ( tgtID[i][0] == lawRegisterTable[lawIndex][0] && 
            (tgtID[i][1] == (lawRegisterTable[lawIndex][1] & QORIQ_LAWAR_TGTID)))
            {
            lawIndex++;
            }
        if (lawIndex == lawNum)
            return;
        }

    for (i = 0; i < M85XX_LAW_CNT; i++ )
        {
        lawARn = (VUINT32 *)((UINT32)M85XX_LAWAR0(CCSBAR) + i * M85XX_LAW_SIZE);
        lawBARn = (VUINT32 *)((UINT32)M85XX_LAWBAR0(CCSBAR) + i * M85XX_LAW_SIZE);

        if (*lawARn & QORIQ_LAWAR_ENABLE)
            continue;

        *lawBARn = lawRegisterTable[lawIndex][0];
        *lawARn = lawRegisterTable[lawIndex][1];
        lawIndex++;
        if (lawIndex < lawNum)
            goto entry;
        else
            return;
        }

    }


/*******************************************************************************
*
* coreLbcSet - set LBC for devices
*
* This routine sets local bus controller registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void coreLbcSet (void)
    {
        switch (sysBoardModel)
        {
        case P1021RDB:
        case P1024RDB:
        case P1025RDB:

            /* Nor Flash */

            * (VINT32 *) M85XX_BR0(CCSBAR) = 0xff001001;
            * (VINT32 *) M85XX_OR0(CCSBAR) = 0xff000030;

            /* CPLD */

            * (VINT32 *) M85XX_BR1(CCSBAR) = 0xf1000801;
            * (VINT32 *) M85XX_OR1(CCSBAR) = 0xf1000000;

           /* VSC7385 */

            * (VINT32 *) M85XX_BR2(CCSBAR) = 0xf4000801;
            * (VINT32 *) M85XX_OR2(CCSBAR) = 0xfffe09ff;

            break;

        case P1023RDB:

            /* Nor Flash */

            * (VINT32 *) M85XX_BR0(CCSBAR) = 0xfc001001;
            * (VINT32 *) M85XX_OR0(CCSBAR) = 0xfc000010;

            break;

         default:
            break;
        }
    return;
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the P10XXRDB board. It sets up
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
    sysBoardModel = sysBoardModelCheck ();

    l2CacheLineNum = ((2048 + 4096) * L2_CACHE_SIZE);

    if (sysBoardModel == P1023RDB)
        {
        memcpy (sysStaticTlbDesc, sysStaticTlbDesc_P1023, sizeof(sysStaticTlbDesc_P1023));
        sysStaticTlbDescNumEnt = sysStaticTlbDescNumEnt_P1023;
        memcpy (sysPhysMemDesc, sysPhysMemDesc_P1023, sizeof(sysPhysMemDesc_P1023));
        sysPhysMemDescNumEnt = sysPhysMemDescNumEnt_P1023;   
        memcpy (lawRegisterTable, lawRegisterTable_P1023, sizeof(lawRegisterTable_P1023));
        lawNum = lawNum_P1023;
        }
    else
        {
        memcpy (sysStaticTlbDesc, sysStaticTlbDesc_Default, sizeof(sysStaticTlbDesc_Default));
        sysStaticTlbDescNumEnt = sysStaticTlbDescNumEnt_Default;  
        memcpy (sysPhysMemDesc, sysPhysMemDesc_Default, sizeof(sysPhysMemDesc_Default));
        sysPhysMemDescNumEnt = sysPhysMemDescNumEnt_Default;
        memcpy (lawRegisterTable, lawRegisterTable_Default, sizeof(lawRegisterTable_Default));
        lawNum = lawNum_Default;
        }

    coreLawBarSet();
    coreLbcSet();
    
    /* fix up resources */

    sysResourceFixup ();

#ifdef INCLUDE_AMP
#   ifdef INCLUDE_AMP_CPU_00
    sysAmpCpuStatusSet(ampCpuStateReboot, 1);
    sysAmpCpuStatusSet(ampCpuStateRunning, 0);
#   else  /* INCLUDE_AMP_CPU_00 */
    sysAmpCpuStatusSet(ampCpuStateRunning, 1);
#   endif  /* INCLUDE_AMP_CPU_00 */
#endif /* INCLUDE_AMP */

#ifdef _WRS_CONFIG_SMP

    /*
     * Here we initialize cpu1 init address so we don't wait until the
     * correct value is initialized
     */

    *(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_RBV;
#endif  /* _WRS_CONFIG_SMP */

    /* Disable L1 Icache */

    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* Put cachePpcReadOrigin at the start of data section */

    cachePpcReadOrigin = (char *) &_wrs_kernel_data_start;

    /* Check for architecture support for 36 bit physical addressing */

#if defined(PPC_e500v2)
    vxHid0Set(_PPC_HID0_MAS7EN|vxHid0Get());
#endif  /* PPC_e500v2 */

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    /* Enable time base for delay use before DEC interrupt is setup */

    vxDecSet(0xffffffff);
    vxDecarSet(0xffffffff);
    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

    sysTimerClkFreq = sysClkFreqGet () >> 3;

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif  /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

    /* Initialize L2CTL register to reset value */

    vxL2CTLSet(0, M85XX_L2CTL(CCSBAR));

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (!defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL))
    mmuPpcIEnabled = TRUE;
    mmuPpcDEnabled = TRUE;
#else  /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

    if (inFullVxWorksImage==FALSE)
        {
        mmuPpcIEnabled = TRUE;
        mmuPpcDEnabled = TRUE;
        }

    /* Enable I Cache if instruction mmu disabled */

#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    mmuPpcIEnabled = TRUE;
#endif  /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif  /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

#if (SW_MMU_ENABLE == TRUE)
    mmuPpcIEnabled = TRUE;
    mmuPpcDEnabled = TRUE;
#endif  /* SW_MMU_ENABLE == TRUE */

    vxHid1Set(HID1_ABE); /* Address Broadcast enable */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2CacheInit();
#endif  /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    /* Machine check via RXFE for RIO */

    vxHid1Set (vxHid1Get()| HID1_ASTME | HID1_RXFE);

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

    WRS_ASM("isync");

    e500_spe_exc_enable = FALSE;
    e500_spe_exc_mask = 0;

    pSysPlbMethods = p10xxPlbMethods;

    if (sysBoardModel == P1021RDB)
        vsc7385Init();
    else if (sysBoardModel == P1023RDB)
        {
#ifdef INCLUDE_P10XX_FMAN_UCODE
        int x;
        int y;
        HCF_DEVICE * pDev;
        struct hcfResource * pRes;
        UINT32 chipRev;
        void * pUcode = NULL;

        pUcode = _binary_fsl_fman_ucode_P1023_160_13_bin_start;
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
#endif /* INCLUDE_P10XX_FMAN_UCODE */

        dtsecEndPortEnable();
        }
    /* config eSDHC pins */

#ifdef DRV_STORAGE_SDHC
    M85XX_PMUXCR &= ~SD_DATA;
    M85XX_PMUXCR |= SDHC_CD | SDHC_WP;
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_SPI_S25FLXX
    sysSpiDrvInit(0, FREQ_33_MHZ);
#endif /* DRV_SPI_S25FLXX */

    _vxb_usDelayRtn = &sysUsDelay;
    _vxb_msDelayRtn = &sysMsDelay;
    _vxb_delayRtn = &sysDelay;

    hardWareInterFaceInit();
#ifdef INCLUDE_L2_SRAM
#   if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
        sysL2SramEnable (TRUE);
#   elif (defined(INCLUDE_L2_SRAM))
        sysL2SramEnable (FALSE);
#   endif
#endif /* INCLUDE_L2_SRAM */

    CACHE_PIPE_FLUSH();
    }

#ifdef INCLUDE_L2_SRAM

/*******************************************************************************
*
* sysL2SramEnable - enables L2SRAM if L2SRAM only
*
* This routine enables L2SRAM if L2SRAM only or initializes blk
* size etc and leaves the rest to L2 cache code.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL2SramEnable
    (
    BOOL both
    )
    {
    volatile int l2CtlVal;

    /*
     * if INCLUDE_L2_CACHE and CACHE_SUPPORT
     * if ((L2_SRAM_SIZE + L2_CACHE_SIZE) > l2Siz)
     * Setup Windows for L2SRAM
     */

    *(M85XX_L2SRBAR0(CCSBAR)) = (UINT32)(L2SRAM_ADDR & M85XX_L2SRBAR_ADDR_MSK);

    /* Get present value */

    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));

    /* Disable L2CTL initially to allow changing of block size */

    l2CtlVal &=(~M85XX_L2CTL_L2E_MSK);
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    l2CtlVal &= ~M85XX_L2CTL_L2SRAM_MSK;

    if (both == TRUE)
        {

        /* Setup as half and half */

        l2CtlVal |= (0x2 << M85XX_L2CTL_L2SRAM_BIT);
        }
    else
        {

        /* All sram */

        l2CtlVal |= (0x1 << M85XX_L2CTL_L2SRAM_BIT);
        }

    /* Setup L2CTL for SRAM */

    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));

    if (both == FALSE)
        {

        /* This is done here so L2SRAM is set before enable */

        l2CtlVal |= M85XX_L2CTL_L2E_MSK; /* No cache so go ahead and enable */

        /* Enable L2CTL for SRAM */

        vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
        }

    }

#endif /* INCLUDE_L2_SRAM */

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
        if (sysBoardModel == P1023RDB)
            physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + PHYS_MEM_SIZE_P1023);
        else
            physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + PHYS_MEM_SIZE_DEFAULT);            
        }

    return(physTop) ;
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

        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif  /* INCLUDE_EDR_PM */
        }

    return memTop;
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
*
* ERRNO: N/A
*/

STATUS sysToMonitor
    (
    int startType   /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);   /* Warm reboot */
#ifdef INCLUDE_AMP
    int sysMonDelay;
    int basePhysCpu = CPU_LOGICAL_TO_PHYS(0);
#endif  /* INCLUDE_AMP */

#ifdef _WRS_CONFIG_SMP
    cpuset_t cpuset;
    int i;
#else /* _WRS_CONFIG_SMP */
    int token = intCpuLock();
#endif  /* _WRS_CONFIG_SMP */

#ifdef _WRS_HAS_DCC_ASM_SYNTAX 
     __asm__  (
    "addis   r4, 0, 0x100F;"
    "ori     r4, r4, 0x0000;"
    "mtspr   MAS0,   r4;"
    "addis   r5, 0,  0xC000;"
    "ori     r5, r5, 0x00000500;"
    "mtspr   MAS1,   r5;"
    "addis   r6, 0,  %hi(0xF3000000);"
    "ori     r6, r6, 0x000a;"
    "mtspr   MAS2,   r6;"
    "addis   r7, 0,  %hi(0xF3000000);"
    "ori     r7, r7, 0x003f;"
    "mtspr   MAS3,   r7;"
    "isync;"
    "msync;"
    "tlbwe;"
    "tlbsync;"
    );
#endif

#ifdef _WRS_HAS_GCC_ASM_SYNTAX 
    __asm__  (
    "addis   4, 0, 0x100F;"
    "ori     4, 4, 0x0000;"
    "mtspr   624,  4;"
    "addis   5, 0,  0xC000;"
    "ori     5, 5, 0x00000500;"
    "mtspr   625,   5;"
    "addis   6, 0,  (0xF3000000)@h;"
    "ori     6, 6, 0x000a;"
    "mtspr   626,   6;"
    "addis   7, 0,  (0xF3000000)@h;"
    "ori     7, 7, 0x003f;"
    "mtspr   627,   7;"
    "isync;"
    "msync;"
    "tlbwe;"
    "tlbsync;"
    );
#endif

#if !defined(INCLUDE_AMP) || \
    (defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00))

    /* Make sure the eLBC pins switch to NorFlash device */

    sysLocalBusAccessEnable (TRUE);
#endif /*  !AMP || (AMP && INCLUDE_AMP_CPU_00) */

#ifdef _WRS_CONFIG_SMP

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

    /* Only CPU 0 gets here */

    intCpuLock ();      /* CPC done, now safe to disable interrupts */

    i =  50;            /* 0.5sec (50 * 0.01) max wait for CPU1 */
    do  {
        sysMsDelay(10);

        /* NOTE: this test implicitly assumes a dual-core processor.
         * If porting this code to processors with more than 2 cores
         * it will need more complex logic to determine a) the number
         * of SMP cpus and b) the physical ID of those cpus so as to
         * check the appropriate reserved memory addresses.
         */

        if (*(UINT32*)CPU1_INIT_START_ADR == MP_MAGIC_CACHE_FLUSHED)
            break;
        } while (--i > 0);

    /* The cacheDisable() API is not supported in SMP, so call the AIM 
     * flush primitives directly 
     */

    cacheAimE500DisableDFromMmu();
    cacheAimE500DisableIFromMmu();

#else   /* _WRS_CONFIG_SMP */
#   ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#   endif  /* INCLUDE_CACHE_SUPPORT */
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

    vxMsrSet(0);

    /* Clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

#ifdef  _WRS_CONFIG_SMP

    /*
     * We need to forcibly reset the second core. This is done
     * using the processor reset register in the PIC.
     *
     * Note also that the bits in the EPIC Hard Reset Register are
     * not auto-clearing.
     */

    *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
    sysCpu1Stop ();
    *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
#endif  /* _WRS_CONFIG_SMP */

#ifdef  INCLUDE_AMP
    /* Clear boot page */

    *M85XX_BPTR(CCSBAR) = 0x0;

    /* SMP specific */

    *((volatile int *)CPU1_FUNC_START_ADR) = (int)0;

    /* SMP specific not used just loaded */

    *((volatile int *)CPU1_STACK_START_ADR) = (int)RAM_LOW_ADRS - FRAMEBASESZ;

    /* This is used if core waiting after core1 did asynchronous reboot */

    *((volatile int *)CPU1_INIT_START_ADR) = (int)MP_MAGIC_RBV;

    _WRS_ASM("sync");

#   ifdef INCLUDE_AMP_CPU_00

    sysCpu1Stop();

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

    *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
    _WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

    *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
    WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

    sysAmpCpuStatusSet(ampCpuStateReboot,0);

#   else  /* INCLUDE_AMP_CPU_00 */

    /* reset PIR prior to entering into bootrom */
       
    vxPirSet (PIR_CORE_CLUSTER_ID_SET(basePhysCpu));

    sysAmpCpuStatusSet(ampCpuStateReboot,1);

#   endif  /* INCLUDE_AMP_CPU_00 */

#endif /* INCLUDE_AMP */
    *CAST(VUINT32 *)(CCSBAR) = 0x000F3000;
    (*pRom) (startType);    /* jump off to romInit.s */

    return(OK);    /* in case we ever continue from ROM monitor */
    }

#if defined(_WRS_CONFIG_SMP)

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT void excIvorInit();
IMPORT void mmuPpcPidSet(UINT32);
IMPORT void sysCacheEnable();

/*******************************************************************************
*
* sysCpuInit - initialize additional core
*
* RETURNS: NONE
*
*/

void sysCpuInit (void)
    {

    vxIvprSet((UINT32)VEC_BASE_ADRS);
    excIvorInit();

#if defined(PPC_e500v2)
    vxHid0Set(_PPC_HID0_MAS7EN | vxHid0Get());
#endif  /* PPC_e500v2 */

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    vxHid1Set (vxHid1Get() | HID1_ASTME | HID1_ABE | HID1_RXFE);

    sysL2CacheInit();

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    if(_pSysL2CacheInvFunc!=NULL)
        _pSysL2CacheInvFunc();

    sysCacheEnable();

    if(_pSysL2CacheEnable!=NULL)
        _pSysL2CacheEnable();

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

    mmuPpcPidSet(MMU_ASID_GLOBAL);

#if (SW_MMU_ENABLE == FALSE)
    vxMsrSet(vxMsrGet() | _PPC_MSR_IS | _PPC_MSR_DS);
#endif  /* SW_MMU_ENABLE == FALSE */

    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

#if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))

    /*
     * Set the current timebase read from CPU0 onto CPU1.
     * The timebase is kept halt till it's re-started by the CPU0.
     */

    sysSmpTimeBaseSet (tbHI, tbLO);

#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    }

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

void sysCpu1Loop (void)
    {
    FUNCPTR  p;

    /*
     * The call to vxSdaInit() must be the first operation here.
     * This is because vxSdaInit() sets the r2 and r13 registers to the
     * SDA and SDA2 base registers. No C code must be placed before this
     * call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in sysCpu1Loop() */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

    p = (FUNCPTR)(*(UINT32*)CPU1_FUNC_START_ADR);

    sysCpuInit();

    if (p != NULL)
        {
        p ();
        }

    for (;;);

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

    if (cpuNum != 1)
        {
        return ERROR;
        }

    *((int *)CPU1_FUNC_START_ADR) = (int)pCpuState->regs.pc;
    *((int *)CPU1_STACK_START_ADR) = (int)pCpuState->regs.spReg;
    *((int *)CPU1_INIT_START_ADR) = (int)sysCpu1Loop;

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    {
#   ifndef VX_ENABLE_CPU_TIMEOUT
#       define VX_ENABLE_CPU_TIMEOUT   5   /* default timeout five seconds */
#   endif  /* VX_ENABLE_CPU_TIMEOUT */

    cpuset_t    cpuset;
    UINT32  i = 0;

    /*
     * PPC Dec timer will not be available while synchronizing timebase
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
     * For future reference:  DEVDISR is internal to the P1020 chip.
     * Platforms using multiple P1020's (i.e. more than two CPUs) cannot
     * achieve perfect synchronization; the best that can be done is to
     * deassert the multiple DEVDISR[TB] signals as near simultaneously as
     * possible by using CPC.  On such a platform, the |= operation below
     * should be replicated, specifying the appropriate base address to
     * access each P1020's CCSR space; and the &= operation below
     * should be replaced with a call to an assembly-language function
     * which writes the multiple DEVDISR registers using consecutive
     * machine instructions.
     */

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the synchronization between two cores gets
     * completed.
     */

    *(M85XX_DEVDISR(CCSBAR)) |= (M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1);

     /*
      * Save the current timebase on CPU0 into static valuable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * synchronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif  /* DRV_TIMER_M85XX && INCLUDE_TIMESTAMP */

    /* Flush Data cache so as to make sure other cores have updated cache */

    {
    volatile char* temp;

    temp = malloc(0x100000);

    memset((char*)temp, 0x0, 0x100000);

#if (defined(INCLUDE_UCC_VXB_END) || defined(DRV_SIO_UCC))
    sysLocalBusAccessEnable (TRUE);
#endif /* INCLUDE_UCC_VXB_END || DRV_SIO_UCC */

    sysCpu1Start ();

#if (defined(INCLUDE_UCC_VXB_END) || defined(DRV_SIO_UCC))
    sysMsDelay (2);
    sysLocalBusAccessEnable (FALSE);
#endif /* INCLUDE_UCC_VXB_END || DRV_SIO_UCC */

    memset((char*)temp, 0x0, 0x100000);

    free((char*)temp);
    }

    /* TBEN should be set by now */

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)

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

    *(M85XX_DEVDISR(CCSBAR)) &= ~(M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1);

    intCpuUnlock(key);

    }
#endif /* DRV_TIMER_M85XX && INCLUDE_SYS_TIMESTAMP */

    return status;
    }

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
/*******************************************************************************
*
* sysCpcTimeBaseSet - CPC receiver for sysSmpTimeBaseInit
*
* This function receives the CPC from sysSmpTimeBaseInit,
* and passes the received parameters to sysSmpTimeBaseSet.
*
*/

LOCAL STATUS sysCpcTimeBaseSet
    (
    void    *ptr,   /* ptr to CPC parameters */
    UINT    size    /* size of CPC parameters */
    )
    {
    UINT32  tbHI = *(UINT32 *)ptr;
    UINT32  tbLO = ((UINT32 *)ptr)[1];

    sysSmpTimeBaseSet(tbHI, tbLO);
    return OK;
    }

/*******************************************************************************
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
* RETURNS: OK or ERROR
*
*/

LOCAL STATUS sysSmpTimeBaseInit
    (
    UINT32  tbHI,
    UINT32  tbLO
    )
    {
    cpuset_t  cpcInvokeCpuSet;
    UINT32    cpcParam[2];
    int       key;

    CPUSET_ZERO (cpcInvokeCpuSet);  /* => all but self */
    cpcParam[0] = tbHI;
    cpcParam[1] = tbLO;

    /* lock interrupts for a critical section */

    key = intCpuLock ();

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the synchronization between two cores gets
     * completed.
     */

    *(M85XX_DEVDISR(CCSBAR)) |= M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1;

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
     * Keep stopping timebase till the synchronization between two cores gets
     * completed.
     */

    *(M85XX_DEVDISR(CCSBAR)) &= ~(M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1);

    intCpuUnlock (key);

    return OK;
    }

/*******************************************************************************
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

    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuAvail), (void *)&avail);

    return (avail);
    }
#endif /* _WRS_CONFIG_SMP */

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
    UINT8   pin;

    if (initialized)
        return;

    if (sysBoardModel != P1023RDB)
        {

        /* Enable QE Mux pins switch controller */

#if !defined(INCLUDE_AMP) || \
    (defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00))

        /* 
         * Set the I/O ports of PCA9555 direction, port 0,1,2
         * configured as output pins.
         */

        pin = 0xf8;
        vxbI2cByNameWrite("gpio_pca9555", 0, 6, &pin, 1);

        sysLocalBusAccessEnable (TRUE);
#endif /*  !AMP || (AMP && INCLUDE_AMP_CPU_00) */

#if (defined(INCLUDE_UCC_VXB_END) || defined(DRV_SIO_UCC))
        qeMuxPinSwitchEnable (TRUE);
#endif /* INCLUDE_UCC_VXB_END || DRV_SIO_UCC */
        }

    if (sysBoardModel == P1025RDB)
        {
#ifdef INCLUDE_UCC_VXB_END
        qeEndPortEnable(QE_UCC5);
#endif /* INCLUDE_UCC_VXB_END */

#ifdef DRV_SIO_UCC
        qeSioPortEnable();
#endif /* DRV_SIO_UCC */
        }

#ifdef  INCLUDE_VXBUS
    vxbDevInit();
#endif  /* INCLUDE_VXBUS */

    /* initialize serial interrupts */

#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
#endif  /* INCLUDE_SIO_UTILS */

#if  defined (INCLUDE_SPE)
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */

#ifdef  INCLUDE_VXBUS
    taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */

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
    return (sysProcNum);
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

/*******************************************************************************
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

/*******************************************************************************
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
     * e.g., 199999999 cycles     1 tick      1 second            25 ticks
     *       ----------------  *  ------   *  --------         ~  --------
     *       second               8 cycles    1000000 microsec    microsec
     */

    /* add to round up before div to provide "at least" */

    oneUsDelay = ((sysTimerClkFreq + 1000000) / 1000000);

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

/*******************************************************************************
*
* sysMsDelay - 1ms delay increments
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

/*******************************************************************************
*
* sysDelay - Fixed 1ms delay
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
/*******************************************************************************
*
* sysL1CacheQuery - configure L1 cache size and alignment
*
* Populates L1 cache size and alignment from configuration registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL1CacheQuery (void)
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
            ppcE500CACHE_ALIGN_SIZE = 32;
            break;
        case 1:
            ppcE500CACHE_ALIGN_SIZE = 64;
            break;
        default:
            ppcE500CACHE_ALIGN_SIZE = 32;
            break;
        }

    ppcE500DCACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);
    ppcE500ICACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);

    /*
     * The core manual suggests for a 32 byte cache line and 8 lines per set
     * we actually need 12 unique address loads to flush the set.
     * The number of lines to flush should be ( 3/2 * cache lines )
     */

    ppcE500DCACHE_LINE_NUM = (3 * ppcE500DCACHE_LINE_NUM) >> 1;
    ppcE500ICACHE_LINE_NUM = (3 * ppcE500ICACHE_LINE_NUM) >> 1;

    }

#endif /* INCLUDE_CACHE_SUPPORT */

/*******************************************************************************
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
    strcpy ((char *)EXC_MSG_OFFSET, errorMsg);
    }

/*******************************************************************************
*
* chipErrataCpu29Print - save Silicon fault message to save area
*
* The message will save Silicon fault message to save area for catastrophic
* error.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void chipErrataCpu29Print (void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    }

/*******************************************************************************
*
* sysGetPeripheralBase - Provides CCSRBAR address for security engine drivers
*
* RETURNS: CCSRBAR value
*
* ERRNO: N/A
*/
UINT32 sysGetPeripheralBase (void)
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

#ifdef INCLUDE_SHOW_ROUTINES

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
    VINT32 tmp, tmp2;

    tmp = * (VINT32 *) M85XX_BR0(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR0(CCSBAR);
    printf("Local bus BR0 = 0x%x\tOR0 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR1(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR1(CCSBAR);
    printf("Local bus BR1 = 0x%x\tOR1 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR2(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR2(CCSBAR);
    printf("Local bus BR2 = 0x%x\tOR2 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR3(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR3(CCSBAR);
    printf("Local bus BR3 = 0x%x\tOR3 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR4(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR4(CCSBAR);
    printf("Local bus BR4 = 0x%x\tOR4 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR5(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR5(CCSBAR);
    printf("Local bus BR5 = 0x%x\tOR5 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR6(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR6(CCSBAR);
    printf("Local bus BR6 = 0x%x\tOR6 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR7(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR7(CCSBAR);
    printf("Local bus BR7 = 0x%x\tOR7 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_LBCR(CCSBAR);
    printf("Local bus LBCR = 0x%x\n", tmp);

    tmp = * (VINT32 *) M85XX_LCRR(CCSBAR);
    printf("Local bus LCRR = 0x%x\n", tmp);
    }


#   define xbit0(x, n)    ((x & (1 << (31 - n))) >> (31 - n))  /* 0..31 */
#   define xbit32(x, n)   ((x & (1 << (63 - n))) >> (63 - n))  /* 32..63 */
#   define onoff0(x, n)   xbit0(x, n) ? "ON", "OFF"
#   define onoff32(x, n)  xbit32(x, n) ? "ON", "OFF"

/*******************************************************************************
*
* coreShow - Show routine for core registers
*
* This routine shows the core registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreShow (void)
    {
    VUINT32 tmp, tmp2;

    tmp = vxMsrGet();
    printf("MSR - 0x%x\n", tmp);
    printf("      UCLE-%x SPE-%x WE-%x CE-%x EE-%x PR-%x ME-%x\n",
           xbit32(tmp,37), xbit32(tmp,38), xbit32(tmp,45), xbit32(tmp,46),
           xbit32(tmp,48), xbit32(tmp,49), xbit32(tmp,51));
    printf("      UBLE-%x DE-%x IS-%x DS-%x PMM-%x\n",
           xbit32(tmp,53), xbit32(tmp,54), xbit32(tmp,58), xbit32(tmp,59),
           xbit32(tmp,61));
    tmp = vxHid0Get();
    tmp2 = vxHid1Get();
    printf("HID0 = 0x%x, HID1 = 0x%x\n", tmp, tmp2);
    tmp = vxL1CSR0Get();
    printf("L1CSR0: cache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = vxL1CSR1Get();
    printf("L1CSR1: Icache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = vxL1CFG0Get();
    tmp2 = vxL1CFG1Get();
    printf("L1CFG0 = 0x%x, L1CFG1 = 0x%x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x20000);
    printf("L2CTL - 0x%x\n", tmp);
    printf("        l2 is %s\n", tmp&0x80000000?"ON":"OFF");
    printf("        l2siz-%x l2blksz-%x l2do-%x l2io-%x\n",
           (xbit0(tmp,2)<<1)|xbit0(tmp,3), (xbit0(tmp,4)<<1)|xbit0(tmp,5),
           xbit0(tmp,9), xbit0(tmp,10));
    printf("        l2pmextdis-%x l2intdis-%x l2sram-%x\n",
           xbit0(tmp,11), xbit0(tmp,12),
           (xbit0(tmp,13)<<2)|(xbit0(tmp,14)<<1)|xbit0(tmp,15));
    tmp = *(VUINT32 *) (CCSBAR + 0x20100);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x20108);
    printf("L2SRBAR0 - 0x%x\n", tmp);
    printf("L2SRBAR1 - 0x%x\n", tmp2);

    printf("Core CPU0 Freq = %3d Hz\n", coreFreq);
    printf("Core CPU1 Freq = %3d Hz\n", core1Freq);
    printf("DDR Freq = %3d Hz\n", ddrFreq);
    printf("CCB Freq = %3d Hz\n", sysClkFreqGet());
    printf("System Freq = %3d Hz\n", systemFreq);
    printf("QE Freq = %3d Hz\n", sysClkFreqGet());

    }
#endif  /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciEx1IntRoute [NUM_PCIEX1_SLOTS][4] = {
    { PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL },
    { PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL },
    { PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL },
    { PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL },
};

LOCAL UCHAR sysPciEx2IntRoute [NUM_PCIEX2_SLOTS][4] = {
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL },
    { PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL },
    { PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL },
    { PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL },
};

/*******************************************************************************
*
* sysPciEx2AutoConfigInclude - PCI Express 2 autoconfig support routine
*
* This routine performs the PCI Express 2 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciExAutoConfigInclude
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UINT            devVend     /* deviceID/vendorID of device */
    )
    {
    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* sysPciEx1AutoConfigIntAssign - PCI Express 1 autoconfig support routine
*
* This routine peforms the PCI Express 1 auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO: N/A
*/

UCHAR sysPciEx1AutoConfigIntAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciEx1IntRoute[(pLoc->device) & 3][pin - 1];
    }

/*******************************************************************************
*
* sysPciEx2AutoConfigIntAssign - PCI Express 2 autoconfig support routine
*
* This routine peforms the PCI Express 2 auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO: N/A
*/

UCHAR sysPciEx2AutoConfigIntAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciEx2IntRoute[(pLoc->device) & 3][pin - 1];
    }

#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SIO_UTILS
#   define BSP_SERIAL_CHAN_GET  bspSerialChanGet
#else
#   define BSP_SERIAL_CHAN_GET  sysSerialChanGet
#endif /* INCLUDE_SIO_UTILS */

/*******************************************************************************
*
* BSP_SERIAL_CHAN_GET - get the serial channel
*
* This routine is called by the vxbus sio driver.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

SIO_CHAN *BSP_SERIAL_CHAN_GET
    (
    int channel
    )
    {
    return ((SIO_CHAN *)ERROR);
    }


/*******************************************************************************
*
* sysCpu1Check - check CPU number
*
* This routine is called to return CPU number.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL BOOL sysCpu1Check (void)
    {
    return (CPU1CHECK);
    }

#ifdef _WRS_CONFIG_SMP
/*******************************************************************************
*
* sysCpu1ShutdownLoop - parking loop for CPU1
*
* This routine is launched onto core 1 during system reboot. It locks out
* interrupts and puts the core into a tight loop so that it doesn't interfere
* with the sysToMonitor() routine.
*
* RETURNS: N/A
*/

LOCAL void sysCpu1ShutdownLoop (void)
    {
    int token;

    token = intCpuLock();

    /*   
     * Flush L1 data cache first.
     * This must be done before flushing L2 cache unless L2 cache is
     * flushed and disabled.
     *
     * The cacheFlush API is a no-op in an SMP system.
     * Instead, call the AIM primitive directly.
     */

    cacheAimE500FlushAllD ();

    _WRS_ASM("sync");

    /* Report to CPU0 this CPU's cache flush is complete */

    *(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_CACHE_FLUSHED;

    /*
     * Do not over write any persistent data, which will be accessed by
     * bootrom from now on.  Data cache is still enabled and the cached data
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

    FOREVER
        {
#ifdef INCLUDE_CPU_PWR_MGMT
        vxHid0Set(vxHid0Get() | _PPC_HID0_DOZE);
        WRS_ASM("sync");
        vxMsrSet(vxMsrGet()| _PPC_MSR_WE);
        WRS_ASM("isync");
#endif  /* INCLUDE_CPU_PWR_MGMT */

        /* expect to lock in DOZE mode until HRESET */

        sysDelay();
        }
    }
#endif /* _WRS_CONFIG_SMP */

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

    /*
     * Here if core already running then maybe be spinning
     * bootimage should then spin waiting for CPU1_INIT_START_ADR to be
     * non-zero
     */

    *((volatile int *)CPU1_FUNC_START_ADR) = (int)0;    /* SMP specific */

    /* SMP specific not used just loaded */

    *((volatile int *)CPU1_STACK_START_ADR) = ((int)entryPt - 0x20);

    /* This is used if core waiting after core1 did asynchronous reboot */

    *((volatile int *)CPU1_INIT_START_ADR) = (int)entryPt;

    /* If core1 is not already running then enable it */

    if((*EEB_PCR(CCSBAR) & EEB_PCR_CORE1_ENABLE) == 0)
        {
        *M85XX_BPTR(CCSBAR) = ((UINT32)entryPt >> BPTR_BOOT_PAGE_SHIFT ) | BPTR_EN;

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
    void * arg
    )
    {
    volatile int sysMonDelay;
    if (cpuId == 1)
        {

        /*
         * Check whether cpu1 has run, if it not be started, not to
         * initialize and reset it
         */

        if (*EEB_PCR(CCSBAR) & 0x02000000)
            {

            /* shutdown ints */

            vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable), (void *)cpuId);

            /* Clear boot page */

            *M85XX_BPTR(CCSBAR) = 0x0;

            *((volatile int *)CPU1_FUNC_START_ADR) = (int)0;

            /* SMP specific not used just loaded */

            *((volatile int *)CPU1_STACK_START_ADR) = (int)(RAM_LOW_ADRS - FRAMEBASESZ);

            /* This is used if core waiting after core1 did asynchronous reboot */

            *((volatile int *)CPU1_INIT_START_ADR) = (int)MP_MAGIC_RBV;

            WRS_ASM("sync");

            /* hold core in reset while clearing PCR! */

            *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
            WRS_ASM("sync");
            for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

            sysCpu1Stop();

            *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
            WRS_ASM("sync");

         }

        return OK;
        }

    /* Cannot prep cpu 0 */

    return ERROR;
    }

#endif /* INCLUDE_WRLOAD */

#ifdef INCLUDE_AMP
/*******************************************************************************
*
* sysAmpCpuStatusSet - Sets the status value of a specific CPU
*
* RETURNS: NONE
*
*/

void sysAmpCpuStatusSet
    (
    int status,         /* CPU status value */
    int cpu             /* CPU ID */
    )
    {
    /* only allow CPU1 to set it's own status */

    if( CPU1CHECK )
        cpu = 1;

    switch(cpu)
        {
        case 1:
            *((VINT32*)AMP_CPU1_STATUS) = status;
            break;
        case 0:
            *((VINT32*)AMP_CPU0_STATUS) = status;
            break;
        default:
            break;
        }
    }

/*******************************************************************************
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
            status = *((VINT32*)AMP_CPU1_STATUS);
            break;
        case 0:
            status = *((VINT32*)AMP_CPU0_STATUS);
            break;
        default:
            break;
        }
    return(status);
    }

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
* core cluster 3 PLL frequency divided by two. For the P5020DS board,
* the RCW supplied by Freescale uses the platform clock frequency.
*
* RETURNS: frame manager clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 sysFmanClkFreqGet (void)
    {
    return (sysPlbClkFreqGet());
    }

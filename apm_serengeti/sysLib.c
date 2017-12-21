/* sysLib.c - APM Serengeti board system-dependent library */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01o,01mar12,syt  fixed compile warnings and SMP reboot issue.
01n,29feb12,syt  updated for new SATA driver with INCLUDE_DRV_STORAGE_AHCI.
01m,06feb12,syt  added SDHC,SATA,I2C,SPI and TFFS support.
01l,11jan12,x_s  replaced cacheDisable API with arch dependent functions.
01k,26dec11,x_s  changed SDRAM TLB attribuite to VM_STATE_CACHEABLE for AMP&SMP;
                 corrected sysToMonitor process for SMP.
01j,21dec11,x_z  bound clock-related initialization to core 0 for all the
                 supported modules.
01i,20dec11,x_z  added support for QMAN and on-chip MAC.
01h,07dec11,x_s  add AMP support.
01g,07sep11,x_s  add SMP support.
01f,29aug11,x_s  add l2 cache support.
01e,06aug11,syt  replace function mmu440TlbStaticInit with mmu465TlbStaticInit.
01d,30jul11,x_z  fix PCI Express address space config;
                 add memory shutdown operation for PCI Express;
                 fix little issues.
01c,22jul11,b_m  add PCI Express support.
01b,18jul11,b_m  add usb host support.
01a,23jun11,syt  initial creation.
*/

/*
DESCRIPTION
This library provides board-specific routines for APM Serengeti board

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    vxbPpcIntCtlr.c     -   PowerPC Interrupt Controller driver
    vxbMpicCtlr.c       -   Multiprocessor Interrupt Controller driver
    vxbPpc440Timer.c    -   PPC465 Systerm Timer driver
    vxbNs16550Sio.c     -   APM86290 UART driver
    vxbApmPProMacEnd.c  -   APM PacketPro MAC VxBus END driver
    vxbApmPProMacCle.c  -   APM PacketPro MAC Classifier driver
    vxbApmPProQman.c    -   APM PacketPro Queue Manager/Traffic Manager driver
    vxbApmPciEx.c       -   APM86290 PCI Express driver
    vxbSdhcStorage.c    -   APM86290 SDHC driver
    vxbAhciStorage.c    -   APM86290 AHCI driver
    sysApmI2c.c         -   APM86290 I2C driver
    sysApmSpi.c         -   APM86290 SPI driver
    flashMem.c          -   S29WS512P flash driver for NVRam

INCLUDE FILES:
    sysLib.h
    vxWorks.h
    memLib.h
    cacheLib.h
    config.h
    string.h
    intLib.h
    logLib.h
    stdio.h
    taskLib.h
    vxLib.h
    tyLib.h
    mmu440Lib.h
    ppc440.h
    vxPpcLib.h
    vmLibP.h
    vxBus.h

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
#include <arch/ppc/mmu440Lib.h>
#include <arch/ppc/ppc440.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>
#include <hwif/vxbus/vxBus.h>

#ifdef _WRS_CONFIG_SMP
#   include <private/cpcLibP.h>
#   include <private/kernelLibP.h>		/* KERNEL_ENTERED_ME() */
#endif  /* _WRS_CONFIG_SMP */

/* defines */

/*
 * 440 cache library requires I and D cache on/off configurations to be
 * identical.  If one is enabled, the other has to be. If both are,
 * they must both be on or both be off.
 */

#ifdef INCLUDE_CACHE_SUPPORT

#   if (defined(USER_I_CACHE_ENABLE) && !defined(USER_D_CACHE_ENABLE))
#       error "USER_I_CACHE_ENABLE requires USER_D_CACHE_ENABLE"
#   elif (defined(USER_D_CACHE_ENABLE) && !defined(USER_I_CACHE_ENABLE))
#       error "USER_D_CACHE_ENABLE requires USER_I_CACHE_ENABLE"
#   elif (defined(USER_I_CACHE_ENABLE) && defined(USER_D_CACHE_ENABLE))
#       if (USER_I_CACHE_MODE == CACHE_DISABLED && USER_D_CACHE_MODE != CACHE_DISABLED)
#           error "USER_I_CACHE_MODE disabled requires USER_D_CACHE_MODE disabled"
#       elif (USER_D_CACHE_MODE == CACHE_DISABLED && USER_I_CACHE_MODE != CACHE_DISABLED)
#           error "USER_D_CACHE_MODE disabled requires USER_I_CACHE_MODE disabled"
#       endif /* I & D cache on/off modes identical */

#   endif /* I, D cache enablement identical */

#endif /* INCLUDE_CACHE_SUPPORT */

/*
 * The static TLB memory mappings contain caching attributes that track
 * the default cache type as defined by USER_D_CACHE_MODE.  The ideal
 * caching attribute for any given memory region should be set to no
 * more than the default cache mode (either CACHE_DISABLED,
 * CACHE_WRITETHROUGH, or CACHE_COPYBACK).  Even in cache mode COPYBACK,
 * it is necessary to support some memory regions as WRITETHROUGH and
 * some as cache DISABLED due to specific device or driver requirements.
 *
 * In addition, when cache is DISABLED, some normally WRITETHROUGH
 * regions are not just marked cache inhibited, but also as Guarded, to
 * remain consistent with the memory map provided by the Ebony OpenBIOS.
 *
 *               #   USER_D_CACHE_MODE (default cache mode)
 *  Static Memory Region #
 *  Ideal Caching Mode   #  DISABLED    WRITETHROUGH    COPYBACK
 *  =====================#==========================================
 *  _WRITETHROUGH        #  -I-          W--            W--
 *  _WRITETHROUGH_OR_G   #  -IG          W--            W--
 *  _COPYBACK            #  -I-          W--            ---
 */

#if (USER_D_CACHE_MODE == CACHE_DISABLED || !defined(USER_D_CACHE_ENABLE))
#   define _WRITETHROUGH        _MMU_TLB_ATTR_I
#   define _WRITETHROUGH_OR_G   _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
#   define _COPYBACK            _MMU_TLB_ATTR_I
#elif (USER_D_CACHE_MODE == CACHE_WRITETHROUGH)
#   define _WRITETHROUGH        _MMU_TLB_ATTR_W
#   define _WRITETHROUGH_OR_G   _MMU_TLB_ATTR_W
#   define _COPYBACK            _MMU_TLB_ATTR_W
#elif (USER_D_CACHE_MODE == CACHE_COPYBACK)
#   define _WRITETHROUGH        _MMU_TLB_ATTR_W
#   define _WRITETHROUGH_OR_G   _MMU_TLB_ATTR_W
#   define _COPYBACK            0
#endif /* USER_D_CACHE_MODE */

/* globals */

/*
 * Static TLB memory mappings
 *
 * The chip provides TLB entry registers which maintain a cache of
 * virtual to physical memory (PLB) address translations. The first few
 * of these entries are set up once at boot to provide a linear mapping
 * of large blocks that emulates a 'real' mode.
 *
 * The remaining entries are dynamically used and reused in a round
 * robin fashion to implement the VM library's address map. These
 * are controlled by the sysPhysMemDesc array, above.
 *
 */

TLB_ENTRY_DESC sysStaticTlbDesc [] = {

    /* Nor Flash space  */

    {
    0xf0000000,
    FLASH_BASE_HI,
    0xf0000000,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | \
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },

    /* System memory space */

    {
    PHY_MEM_BASE,
    PHY_MEM_BASE_HI,
    PHY_MEM_BASE,
#if defined( _WRS_CONFIG_SMP) || defined(INCLUDE_AMP_CPU)
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1G | _MMU_TLB_ATTR_WL1 | _MMU_TLB_ATTR_M | _MMU_TLB_PERM_X | \
    _MMU_TLB_PERM_W
#else /* !_WRS_CONFIG_SMP && !INCLUDE_AMP_CPU */
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1G | _COPYBACK  | _MMU_TLB_PERM_X | \
    _MMU_TLB_PERM_W
#endif /* _WRS_CONFIG_SMP || INCLUDE_AMP_CPU */
    },

    /* APM86x90 On-Chip Memory (OCM) map space */

    {
    APM_OCM_BASE,
    APM_OCM_BASE_PHY_BASE_HI,
    APM_OCM_BASE_PHY_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_16K | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    APM_OCM_BASE + 0x4000,
    APM_OCM_BASE_PHY_BASE_HI,
    APM_OCM_BASE_PHY_BASE + 0x4000,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_16K | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    /* APM86x90 Freepool Mailboxes and DDR controller map space */

    {
    APM_MEMC_BASE,
    APM_MEMC_PHY_BASE_HI,
    APM_MEMC_PHY_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_16K | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    /* APM86x90 CSR registers map space */

    {
    APM_CSR_BASE,
    APM_CSR_BASE_HI,
    APM_CSR_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    /* APM86x90 MPIC registers map space */

    {
    APM_MPIC_BASE,
    APM_MPIC_BASE_HI,
    APM_MPIC_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |_MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    /* APM86x90 peripherals Registers map space */

    {
    APM_PERIPHERAL_BASE,
    APM_PERIPHERAL1_PHY_BASE_HI,
    APM_PERIPHERAL1_PHY_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    (APM_PERIPHERAL_BASE + 0x100000),
    APM_PERIPHERAL1_PHY_BASE_HI,
    (APM_PERIPHERAL1_PHY_BASE + 0x100000),
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    (APM_PERIPHERAL_BASE + 0x200000),
    APM_PERIPHERAL2_PHY_BASE_HI,
    APM_PERIPHERAL2_PHY_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

#ifdef INCLUDE_DRV_STORAGE_AHCI
    {
    (APM_PERIPHERAL_BASE + 0x300000),
    APM_SATA_PHY_BASE_HI,
    APM_SATA_PHY_BASE_LOW,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },
#endif /* INCLUDE_DRV_STORAGE_AHCI */

#ifdef DRV_PCIBUS_APM_PCIEX
    {
    APM_PCIEX0_CFG_BASE,
    PCIEX0_MEM_PHY_ADRS_HI,
    APM_PCIEX0_CFG_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    PCIEX0_MEM_ADRS,
    PCIEX0_MEM_PHY_ADRS_HI,
    PCIEX0_MEM_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    APM_PCIEX1_CFG_BASE,
    PCIEX1_MEM_PHY_ADRS_HI,
    APM_PCIEX1_CFG_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    PCIEX1_MEM_ADRS,
    PCIEX1_MEM_PHY_ADRS_HI,
    PCIEX1_MEM_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    APM_PCIEX2_CFG_BASE,
    PCIEX2_MEM_PHY_ADRS_HI,
    APM_PCIEX2_CFG_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },

    {
    PCIEX2_MEM_ADRS,
    PCIEX2_MEM_PHY_ADRS_HI,
    PCIEX2_MEM_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    },
#endif /* DRV_PCIBUS_APM_PCIEX */

#ifdef DRV_APM_PPRO_QMAN

    /* APM86x90 QMan HBF map space */

    {
    APM_QMAN_HBF_BASE,
    APM_QMAN_HBF_PHY_BASE_HI,
    APM_QMAN_HBF_PHY_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_PERM_W
    }
#endif /* DRV_APM_PPRO_QMAN */
};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#define PHYS_64BIT_ADDR(h, l)       (((PHYS_ADDR)h << 32) + l)

PHYS_MEM_DESC sysPhysMemDesc [] = {

#if !defined(INCLUDE_AMP_CPU) || \
    (defined(INCLUDE_AMP_CPU) && defined(INCLUDE_AMP_CPU_00))

    /* RAM - must be first entry, size is updated in sysHwInit() */

    {
    (VIRT_ADDR)LOCAL_MEM_LOCAL_ADRS,
    PHYS_64BIT_ADDR(0x0, LOCAL_MEM_LOCAL_ADRS),
    0x40000000,
#if !defined( _WRS_CONFIG_SMP) && !defined(INCLUDE_AMP_CPU)
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE ,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE
#else /* _WRS_CONFIG_SMP || INCLUDE_AMP_CPU */
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY, 
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE  |
    VM_STATE_MEM_COHERENCY
#endif /* !_WRS_CONFIG_SMP && !INCLUDE_AMP_CPU */
    },

#else /* AMP && !INCLUDE_AMP_CPU_00 */

    /*
     * Shared CPU space to use by AMP, the space is below RAM_LOW address of core0
     * and the size is 0x10000 * 1 (max CPU index).
     */

    {
    (VIRT_ADDR) CPUn_SPACE(1),
    PHYS_64BIT_ADDR (0x0, CPUn_SPACE(1)),
    CPUn_SPACE(0) - CPUn_SPACE(1),
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    PHYS_64BIT_ADDR (0x0, LOCAL_MEM_LOCAL_ADRS),
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE |
    VM_STATE_MEM_COHERENCY
    },

#endif /*  !AMP || (AMP && INCLUDE_AMP_CPU_00) */

    {
    (VIRT_ADDR)FLASH_BASE_ADRS,
    PHYS_64BIT_ADDR(FLASH_BASE_HI, FLASH_BASE_ADRS),
    FLASH_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
    /*
     * Exceptions and interrupts which reenable MMU in the enter routine
     * need the vector page mapped when they return to the vector stub
     * to call the handler. Likewise for the handler when it returns to
     * call the exit routine.
     */
    {
    (VIRT_ADDR)VEC_BASE_ADRS,
    PHYS_64BIT_ADDR(LOCAL_MEM_ERPN, VEC_BASE_ALT_ADRS),
    VEC_BASE_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | MEM_STATE
    },
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */

    /* APM86x90 On-Chip Memory (OCM) map space */
    {
    (VIRT_ADDR)APM_OCM_BASE,
    PHYS_64BIT_ADDR(APM_OCM_BASE_PHY_BASE_HI, APM_OCM_BASE_PHY_BASE),
    APM_OCM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    /* APM86x90 Memory Controller (MEMC) map space */
    {
    (VIRT_ADDR)APM_MEMC_BASE,
    PHYS_64BIT_ADDR(APM_MEMC_PHY_BASE_HI, APM_MEMC_PHY_BASE),
    APM_MEMC_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    /* APM86x90 MPIC Register Map Space */

    {
    (VIRT_ADDR)APM_CSR_BASE,
    PHYS_64BIT_ADDR(APM_CSR_BASE_HI, APM_CSR_BASE),
    APM_CSR_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    /* APM86x90 MPIC registers map space */

    {
    (VIRT_ADDR)APM_MPIC_BASE,
    PHYS_64BIT_ADDR(APM_MPIC_BASE_HI, APM_MPIC_BASE),
    APM_MPIC_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    /* APM86x90 Peripherals Register Map Space */

    {
    (VIRT_ADDR)APM_PERIPHERAL_BASE,
    PHYS_64BIT_ADDR(APM_PERIPHERAL1_PHY_BASE_HI, APM_PERIPHERAL1_PHY_BASE),
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) (APM_PERIPHERAL_BASE + 0x100000),
    PHYS_64BIT_ADDR(APM_PERIPHERAL1_PHY_BASE_HI, (APM_PERIPHERAL1_PHY_BASE + 0x100000)),
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)(APM_PERIPHERAL_BASE +0x200000),
    PHYS_64BIT_ADDR(APM_PERIPHERAL2_PHY_BASE_HI, APM_PERIPHERAL2_PHY_BASE),
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

#ifdef INCLUDE_DRV_STORAGE_AHCI
    {
    (VIRT_ADDR)(APM_PERIPHERAL_BASE +0x300000),
    PHYS_64BIT_ADDR(APM_SATA_PHY_BASE_HI, APM_SATA_PHY_BASE_LOW),
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },
#endif /* INCLUDE_DRV_STORAGE_AHCI */

#ifdef DRV_PCIBUS_APM_PCIEX
    {
    (VIRT_ADDR)APM_PCIEX0_CFG_BASE,
    PHYS_64BIT_ADDR(PCIEX0_MEM_PHY_ADRS_HI, APM_PCIEX0_CFG_BASE),
    APM_PCIEX0_CFG_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)PCIEX0_MEM_ADRS,
    PHYS_64BIT_ADDR(PCIEX0_MEM_PHY_ADRS_HI, PCIEX0_MEM_ADRS),
    PCIEX0_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    PCIEX0_MAP_SIZE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)APM_PCIEX1_CFG_BASE,
    PHYS_64BIT_ADDR(PCIEX1_MEM_PHY_ADRS_HI, APM_PCIEX1_CFG_BASE),
    APM_PCIEX1_CFG_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)PCIEX1_MEM_ADRS,
    PHYS_64BIT_ADDR(PCIEX1_MEM_PHY_ADRS_HI, PCIEX1_MEM_ADRS),
    PCIEX1_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)APM_PCIEX2_CFG_BASE,
    PHYS_64BIT_ADDR(PCIEX2_MEM_PHY_ADRS_HI, APM_PCIEX2_CFG_BASE),
    APM_PCIEX2_CFG_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)PCIEX2_MEM_ADRS,
    PHYS_64BIT_ADDR(PCIEX2_MEM_PHY_ADRS_HI, PCIEX2_MEM_ADRS),
    PCIEX2_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    },
#endif /* DRV_PCIBUS_APM_PCIEX */

#ifdef DRV_APM_PPRO_QMAN

    /* APM86x90 QMan HBF map space */

    {
    (VIRT_ADDR) APM_QMAN_HBF_BASE,
    PHYS_64BIT_ADDR(APM_QMAN_HBF_PHY_BASE_HI, APM_QMAN_HBF_PHY_BASE),
    APM_QMAN_HBF_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED
    }
#endif /* DRV_APM_PPRO_QMAN */
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* forward declarations */

STATUS  sysInfoGet (SYS_INFO *);
void    sysUsDelay (int);
void    sysMsDelay (int);
void    sysDelay (void);

/* globals */

int         sysBus;                       /* system bus type                */
int         sysCpu      = CPU;            /* system CPU type (PPC465)       */
char *      sysBootLine = BOOT_LINE_ADRS; /* address of boot line           */
char *      sysExcMsg   = EXC_MSG_ADRS;   /* catastrophic message area      */
int         sysTimerClkFreq;              /* Timer clock frequency          */
int         sysProcNum;                   /* processor number of this CPU   */
SYS_INFO    systemInfo;                   /* PLL and system frequencies     */

/* locals */

#ifdef _WRS_CONFIG_SMP
#   if defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;               /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;               /* lower 32 bit value of timebase */
#   endif   /* INCLUDE_TIMESTAMP */
LOCAL void sysCpuShutdownLoop (void);
#endif /* _WRS_CONFIG_SMP */

/* externs */

IMPORT UINT32 vxCcr1Get (void);
IMPORT void vxCcr1Set (UINT32);
IMPORT void vxDbcr0Set (UINT32 val);
IMPORT UINT32 vxDbcr0Get (void);

IMPORT UINT32 sysRead32 (UINT32 address);
IMPORT void   sysWrite32 (UINT32 address, UINT32 value);
IMPORT UINT32 sysRead32Le (UINT32 address);
IMPORT void   sysWrite32Le (UINT32 address, UINT32 value);
IMPORT void (*_vxb_usDelayRtn) (int);
IMPORT void (*_vxb_msDelayRtn) (int);
IMPORT void (*_vxb_delayRtn) (void);

IMPORT void cacheAim465FlushL1D_Impl (void);
IMPORT void mmu465TlbStaticInit (UINT32, TLB_ENTRY_DESC *, BOOL, UINT32 *);
IMPORT void hardWareInterFaceInit (void);
IMPORT FUNCPTR _func_cache440EarlyCleanup;
IMPORT STATUS cacheAim465DisableI (void);
IMPORT STATUS cacheAim465DisableD (void);

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif

#ifdef INCLUDE_USB
IMPORT void sysUsbInit (void);
#endif /* INCLUDE_USB */

#ifdef INCLUDE_DRV_STORAGE_AHCI
IMPORT void sysSataSerdesInit (void);
#endif /* INCLUDE_DRV_STORAGE_AHCI */

#ifdef  INCLUDE_440X5_PARITY_RECOVERY
IMPORT BOOL     exc440x5_parity;
IMPORT BOOL     exc440x5_tlb;
IMPORT BOOL     exc440x5_dcache;
IMPORT char *   excVecBaseAltAdrs;
IMPORT FUNCPTR  exc440x5_logger;
#endif /* INCLUDE_440X5_PARITY_RECOVERY */

#ifdef  _WRS_CONFIG_SMP
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
IMPORT void excIvorInit();
IMPORT void mmuPpcPidSet(UINT32);
IMPORT UINT32 vxPirGet (void);
IMPORT void mmuPpcMmucrSet (UINT32 pid);
IMPORT UINT32 mmuPpcMmucrGet (void);
IMPORT void cache465DCReset (void);
IMPORT void cache465ICReset (void);
#endif  /* _WRS_CONFIG_SMP */

/* nvRam */

#include "flashMem.c"
#include "nvRamToFlash.c"

/* L2 cache support */

#ifdef INCLUDE_L2_CACHE
#   include "sysL2Cache.c"
#endif  /* INCLUDE_L2_CACHE */

/* bootrom 'M' command support */

#ifdef ETHERNET_MAC_HANDLER
#   include "sysNet.c"
#endif /* ETHERNET_MAC_HANDLER */

/* vxbus support */

#include <hwif/vxbus/vxBus.h>

#ifdef DRV_PCIBUS_APM_PCIEX
#   include "sysPciEx.c"
#endif /* DRV_PCIBUS_APM_PCIEX */

#ifdef INCLUDE_DRV_STORAGE_AHCI
#   include "sysSata.c"
#endif /* INCLUDE_DRV_STORAGE_AHCI */

#ifdef INCLUDE_APM_SPI
#   include "sysApmSpi.c"
#   include "sysSpiFlash.c"
#endif /* INCLUDE_APM_SPI */

#ifdef INCLUDE_APM_I2C
#   include "sysApmI2c.c"
#   include "sysI2cEeprom.c"
#endif /* INCLUDE_APM_I2C */

/* vxBus drivers */

#include "hwconf.c"

/*******************************************************************************
*
* sysSocBlockEnable - enable various SOC IP blocks
*
* The routine enables various SOC IP blocks, by first resetting the IP block
* and turning off its clock, then releasing the IP block out of reset and
* turning on its clock again.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysSocBlockEnable (void)
    {
    UINT32 csrSrst, csrSrst1;
    UINT32 srst, srst1;
    UINT32 clkEn, clkEn1;
    UINT32 loop;

    /* turn on MPIC by default */

    csrSrst = SCU_CSR_SRST_MPIC;
    csrSrst1 = 0;
    srst = SCU_SRST_MPIC;
    srst1 = 0;
    clkEn = 0;
    clkEn1 = 0;
    
    /* SATA */

    srst |= (SCU_SRST_SATA0 | SCU_SRST_SATA1);
    csrSrst |= (SCU_CSR_SRST_SATA0 | SCU_CSR_SRST_SATA1);
    clkEn |= (SCU_CLKEN_SATA0 | SCU_CLKEN_SATA1);

    /* PCIe */

    csrSrst |= (SCU_CSR_SRST_PCIEX0 | SCU_CSR_SRST_PCIEX1 | SCU_CSR_SRST_PCIEX2);
    srst |= (SCU_SRST_PCIEX0 | SCU_SRST_PCIEX1 | SCU_SRST_PCIEX2);
    clkEn |= (SCU_CLKEN_PCIEX0 | SCU_CLKEN_PCIEX1 | SCU_CLKEN_PCIEX2);

    /* USB */

    srst |= (SCU_SRST_USB0 | SCU_SRST_USB1 | SCU_SRST_USB2);
    clkEn |= (SCU_CLKEN_USB0 | SCU_CLKEN_USB1 | SCU_CLKEN_USB2);

    /* SDHC */

    srst |= SCU_SRST_SDIO;
    clkEn |= SCU_CLKEN_SDIO;

    /* SPI */

    srst |= SCU_SRST_SPI;
    csrSrst |= SCU_CSR_SRST_SPI;
    clkEn |= SCU_CLKEN_SPI;

    /* QMAN */

    srst |= SCU_SRST_QMTM;
    csrSrst |= SCU_CSR_SRST_QMTM;
    clkEn |= SCU_CLKEN_QMTM;

    /* Ethernet */

    srst |= (SCU_SRST_ENET0 | SCU_SRST_ENET1);
    srst1 |= (SCU_SRST1_CLE0 | SCU_SRST1_CLE1);
    csrSrst |= (SCU_CSR_SRST_ENET0 | SCU_CSR_SRST_ENET1);
    csrSrst1 |= SCU_CSR_SRST1_CLE;
    clkEn |= (SCU_CLKEN_ENET0 | SCU_CLKEN_ENET1);
    clkEn1 |= (SCU_CLKEN1_CLE0_IL | SCU_CLKEN1_CLE1_IL);

    /* assert reset to SOC blocks */

    sysWrite32(SCU_CSR_SRST, sysRead32(SCU_CSR_SRST) | csrSrst);
    sysWrite32(SCU_CSR_SRST1, sysRead32(SCU_CSR_SRST1) | csrSrst1);

    sysWrite32(SCU_SRST, sysRead32(SCU_SRST) | srst);
    sysWrite32(SCU_SRST1, sysRead32(SCU_SRST1) | srst1);

    /* disable clocks for SOC blocks */

    sysWrite32(SCU_CLKEN, sysRead32(SCU_CLKEN) & ~clkEn);
    sysWrite32(SCU_CLKEN1, sysRead32(SCU_CLKEN1) & ~clkEn1);

    /* enable clocks for SOC blocks */

    sysWrite32(SCU_CLKEN, sysRead32(SCU_CLKEN) | clkEn);
    sysWrite32(SCU_CLKEN1, sysRead32(SCU_CLKEN1) | clkEn1);

    /* release reset to SOC blocks */

    sysWrite32(SCU_SRST, sysRead32(SCU_SRST) & ~srst);
    sysWrite32(SCU_SRST1, sysRead32(SCU_SRST1) & ~srst1);

    sysWrite32(SCU_CSR_SRST, sysRead32(SCU_CSR_SRST) & ~csrSrst);
    sysWrite32(SCU_CSR_SRST1, sysRead32(SCU_CSR_SRST1) & ~csrSrst1);

    /* I2C */

    sysWrite32(SCU_SOCDIV3, sysRead32(SCU_SOCDIV3) | IICCLK_APB_CLK);

    /* clear shutdown status */

    /* PCIe */

    if (sysRead32 (APM_PCIEX0_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN) != 0)
        {
        sysWrite32 (APM_PCIEX0_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN, 0);
        sysRead32 (APM_PCIEX0_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN);

        for (loop = 0; loop < MEM_RDY_TIMEOUT_COUNT; loop++)
            {
            if ((sysRead32 (SCU_MRDY) & PCIEX0_RDY_MASK) != 0)
                break;
            }
        }

    if (sysRead32 (APM_PCIEX1_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN) != 0)
        {
        sysWrite32 (APM_PCIEX1_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN, 0);
        sysRead32 (APM_PCIEX1_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN);

        for (loop = 0; loop < MEM_RDY_TIMEOUT_COUNT; loop++)
            {
            if ((sysRead32 (SCU_MRDY) & PCIEX1_RDY_MASK) != 0)
                break;
            }
        }

    if (sysRead32 (APM_PCIEX2_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN) != 0)
        {
        sysWrite32 (APM_PCIEX2_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN, 0);
        sysRead32 (APM_PCIEX2_CSR_BASE + APM_PCIEX_MEM_SHUTDOWN);

        for (loop = 0; loop < MEM_RDY_TIMEOUT_COUNT; loop++)
            {
            if ((sysRead32 (SCU_MRDY) & PCIEX2_RDY_MASK) != 0)
                break;
            }
        }

    /* QMAN */

    if (sysRead32 (APM_QMAN_MEM_SHTDOWN) != 0)
        {
        sysWrite32 (APM_QMAN_MEM_SHTDOWN, 0);
        sysRead32 (APM_QMAN_MEM_SHTDOWN);

        for (loop = 0; loop < MEM_RDY_TIMEOUT_COUNT; loop++)
            {
            if ((sysRead32 (SCU_MRDY) & QMTM_RDY_MASK) != 0)
                break;
            }
        }

    /* Ethernet */

    if (sysRead32 (APM_ETH_MEM_SHTDOWN) != 0)
        {
        sysWrite32 (APM_ETH_MEM_SHTDOWN, 0);
        sysRead32 (APM_ETH_MEM_SHTDOWN);

        for (loop = 0; loop < MEM_RDY_TIMEOUT_COUNT; loop++)
            {
            if ((sysRead32 (SCU_MRDY) & ENET_RDY_MASK) != 0)
                break;
            }
        }

    /* Classifier */

    if (sysRead32 (APM_CLE_MEM_SHTDOWN) != 0)
        {
        sysWrite32 (APM_CLE_MEM_SHTDOWN, 0);
        sysRead32 (APM_CLE_MEM_SHTDOWN);

        for (loop = 0; loop < MEM_RDY_TIMEOUT_COUNT; loop++)
            {
            if ((sysRead32 (SCU_MRDY) & CLE_RDY_MASK) != 0)
                break;
            }
        }
    }

#ifdef DRV_STORAGE_SDHC

/*******************************************************************************
*
* sysSdhcCapInit - SDHC capability setting initialization
*
* This routine initializes SDHC capability setting register.
*
* RETURNS: N/A
*/

void sysSdhcCapInit (void)
    {
    sysWrite32 (AHB_BASE + SDHC_CAP_REG_LOW_S1, APM_SDHC_CAP_REG_LO_INIT_VAL);
    sysWrite32 (AHB_BASE + SDHC_CAP_REG_HIGH_S1, APM_SDHC_CAP_REG_HI_INIT_VAL);

    sysWrite32 (AHB_BASE + SDHC_CAP_REG_LOW_S2, APM_SDHC_CAP_REG_LO_INIT_VAL);
    sysWrite32 (AHB_BASE + SDHC_CAP_REG_HIGH_S2, APM_SDHC_CAP_REG_HI_INIT_VAL);

    sysMsDelay (100);
    }
#endif /* DRV_STORAGE_SDHC */

/*******************************************************************************
*
* sysSocEthPllInit - initialize SOC Ethernet PLL block
*
* The routine initializes the SOC Ethernet PLL block.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysSocEthPllInit (void)
    {
    UINT32  regVal;

    regVal = sysRead32 (SCU_SOCPLL4);

    /* just return if already initialized */

    if ((regVal & PLL_DOWN) == 0)
        return;

    /* clear Power down and bypass mode and reset PLL */

    regVal &= ~(PLL_BYPASS | PLL_DOWN);
    regVal |= PLL_RESET;
    sysWrite32 (SCU_SOCPLL4, regVal);

    /* setup Ethernet PLL */

    regVal &=
        ~((CLKR_MASK << CLKR_SHIFT) | (CLKOD_MASK << CLKOD_SHIFT) | CLKF_MASK);
    regVal |= (PLL_NR_SET (0) | PLL_OD_SET (0) |
        PLL_NF_SET (PLL_NF_ETH_DEFAULT));

    sysWrite32 (SCU_SOCPLL4, regVal);
    sysRead32 (SCU_SOCPLLADJ4);
    sysWrite32 (SCU_SOCPLLADJ4,
        (sysRead32 (SCU_SOCPLLADJ4) & ~BWADJ_MASK) | PLL4_BWADJ_DEFAULT);

    /* wait for ready */

    regVal = sysRead32 (SCU_PLLDLY);
    sysUsDelay (10);

    sysWrite32 (SCU_SOCPLL4, sysRead32 (SCU_SOCPLL4) & ~PLL_RESET);

    regVal = sysRead32 (SCU_PLLDLY);
    sysUsDelay (10);

    /* set up default RGMII Clock Divider Ratios */

    sysWrite32 (SCU_SOCDIV4, RGMII0_DIV_SET (RGMII_DIV_DEFAULT_1G) |
        RGMII1_DIV_SET (RGMII_DIV_DEFAULT_1G));
    sysRead32 (SCU_SOCDIV4);

    }

/*******************************************************************************
*
* sysSocEthClkdivSet - set up RGMII Clock Divider Ratios
*
* The routine sets up RGMII Clock Divider Ratios.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysSocEthClkdivSet
    (
    UINT32 port,
    UINT32 speed
    )
    {
    UINT32 div;

    div = sysRead32 (SCU_SOCDIV4);
    switch (speed)
        {
        case 10:
            if (port == 0)
                div = (div & (~RGMII0_DIV_MASK)) |
                    RGMII0_DIV_SET (RGMII_DIV_DEFAULT_10M);
            else
                div = (div & (~ RGMII1_DIV_SET (RGMII1_DIV_MASK))) |
                    RGMII1_DIV_SET (RGMII_DIV_DEFAULT_10M);
            break;

        case 100:
            if (port == 0)
                div = (div & (~RGMII0_DIV_MASK)) |
                    RGMII0_DIV_SET (RGMII_DIV_DEFAULT_100M);
            else
                div = (div & (~ RGMII1_DIV_SET (RGMII1_DIV_MASK))) |
                    RGMII1_DIV_SET (RGMII_DIV_DEFAULT_100M);
            break;

        case 1000:
            if (port == 0)
                div = (div & (~RGMII0_DIV_MASK)) |
                    RGMII0_DIV_SET (RGMII_DIV_DEFAULT_1G);
            else
                div = (div & (~ RGMII1_DIV_SET (RGMII1_DIV_MASK))) |
                    RGMII1_DIV_SET (RGMII_DIV_DEFAULT_1G);
            break;

        default:
            return;
        }

    sysWrite32 (SCU_SOCDIV4, div);
    sysRead32 (SCU_SOCDIV4);
    }

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU.
*
*/

char * sysModel (void)
    {
    return (SYS_MODEL_86290);
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number,
* for example, 1.1/0. BSP_REV is concatenated to BSP_VERSION and
* returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwInit0 - Pre-initialize the system hardware
*
* This routine initializes the features which need be installed early.
*
* RETURNS: N/A
*/

void sysHwInit0 (void)
    {
#ifdef  INCLUDE_440X5_PARITY_RECOVERY
    exc440x5_parity   = TRUE;
    exc440x5_tlb      = _WRS_PPC440X5_CFG_TLB;
    exc440x5_dcache   = _WRS_PPC440X5_CFG_DCR;
    excVecBaseAltAdrs = VEC_BASE_ALT_ADRS;
    exc440x5_logger   = _PPC_440X5_MCH_LOG_RTN;
#endif /* INCLUDE_440X5_PARITY_RECOVERY */
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes features of board. It sets up the control registers,
* initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {
    sysInfoGet (&(systemInfo));

#ifdef INCLUDE_L2_CACHE
    sysL2CacheInit();
#endif  /* INCLUDE_L2_CACHE */

    /*
     * before inhibit cache, flush cache first.
     * mmu440Lib.c would see if the hook _func_cache440EarlyCleanup is registered
     * and flush the cache before inhibiting cache based on TLB attributes.
     */

    _func_cache440EarlyCleanup = (void *)cacheAim465FlushL1D_Impl;

#ifdef INCLUDE_L2_CACHE
    if (_pSysL2CacheFlush != NULL)
        (*_pSysL2CacheFlush)();
    if (_pSysL2CacheInvFunc != NULL)
        (*_pSysL2CacheInvFunc)();
    if (_pSysL2CacheDisable != NULL)
        (*_pSysL2CacheDisable)();
#endif  /* INCLUDE_L2_CACHE */

    _func_cache440EarlyCleanup();

    /* write the sysStaticTlbDesc[] table to the TLB */

    mmu465TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], FALSE, NULL);

    sysTimerClkFreq = systemInfo.cpu0ClkFreq;

    _vxb_usDelayRtn = &sysUsDelay;
    _vxb_msDelayRtn = &sysMsDelay;
    _vxb_delayRtn = &sysDelay;

#if defined(INCLUDE_AMP_CPU_00) || !defined(INCLUDE_AMP_CPU)
    /* Initialize Ethernet PLL */

    sysSocEthPllInit ();

    /* Enable SOC blocks */

    sysSocBlockEnable();

#endif /* INCLUDE_AMP_CPU_00 || !INCLUDE_AMP_CPU */

#ifdef INCLUDE_USB
    sysUsbInit();
#endif

#ifdef INCLUDE_DRV_STORAGE_AHCI
    sysSataSerdesInit ();
#endif /* INCLUDE_DRV_STORAGE_AHCI */

#ifdef DRV_STORAGE_SDHC
    sysSdhcCapInit ();
#endif /* DRV_STORAGE_SDHC */

    hardWareInterFaceInit();

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy(sysBootLine, DEFAULT_BOOT_LINE, strlen(DEFAULT_BOOT_LINE) + 1);
#endif /* FORCE_DEFAULT_BOOT_LINE */

#if defined(INCLUDE_AMP_CPU_00) || defined(_WRS_CONFIG_SMP)
    /* reset core1 */

    sysWrite32(APM_MPIC_PRR, sysRead32(APM_MPIC_PRR) | APM_MPIC_PRR_RST1);
    sysUsDelay (10);

    /* enable core1 */

    sysWrite32(APM_MPIC_PRR, sysRead32(APM_MPIC_PRR) & ~APM_MPIC_PRR_RST1);
#endif /* INCLUDE_AMP_CPU || _WRS_CONFIG_SMP */
    }

/*******************************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupts and configures any required
* features not configured by sysHwInit().
*
* RETURNS: N/A
*/

void sysHwInit2 (void)
    {
    static BOOL configured = FALSE;

    if (configured)
        return;

    /* initialize VXBUS devices */

    vxbDevInit();

#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll ();
#endif  /* INCLUDE_SIO_UTILS */

    taskSpawn("tDevConn", 11, 0, 10000,
              vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

    configured = TRUE;
    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* The user specifies the amount of physical memory with the
* macro LOCAL_MEM_SIZE in config.h.
*
* NOTE: Do not adjust LOCAL_MEM_SIZE to reserve memory for application
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
        /* Don't do autosizing, size is given */

        physTop = (char *)(PHY_MEM_BASE + PHY_MEM_SIZE);
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
* The user can reserve memory space by defining the macro
* USER_RESERVED_MEM in config.h. This routine returns the address of the
* reserved memory area. The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*/

char * sysMemTop (void)
    {
    static char * memTop = NULL;

    if (memTop == NULL)
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM

        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif /* INCLUDE_EDR_PM */
        }

    return memTop;
    }

/*******************************************************************************
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor. Normally, it is
* called only by reboot()--which services ^X--and by bus errors at
* interrupt level. However, in special circumstances, the user may wish
* to introduce a <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*
* ERRNO
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
#ifdef _WRS_CONFIG_SMP
    cpuset_t cpuset;
    int i = 0, thisPhysCpu = 0, countSmpCpus = 0, countCacheFlushed = 0;
#else /* _WRS_CONFIG_SMP */
    int token = intCpuLock();
#endif  /* _WRS_CONFIG_SMP */

    FUNCPTR pRom = (FUNCPTR) (ROM_WARM_ADRS); /* Warm reboot */
    UINT32 cpuId = vxCpuPhysIndexGet ();

#ifdef _WRS_CONFIG_SMP
    /*
     * Make sure we cannot migrate, but without locking
     * interrupts -- see cpcInvoke() requirements.
     */

    if (!INT_CONTEXT() && !KERNEL_ENTERED_ME())
        taskCpuLock();

    /*
     * As an added precaution to save as much state as possible,
     * flush the d-cache before attempting to send any CPC's.
     *
     * Also, the cacheFlush API is a no-op in an SMP system.
     * Instead, call the AIM primitive directly.
     */

    _func_cache440EarlyCleanup();

#ifdef INCLUDE_L2_CACHE
    if (_pSysL2CacheFlush != NULL)
        (*_pSysL2CacheFlush)();
#endif /* INCLUDE_L2_CACHE */

    /* invalidate cache */

    cache465ICReset();
    cache465DCReset();

#ifdef INCLUDE_L2_CACHE
    if (_pSysL2CacheInvFunc != NULL)
        (*_pSysL2CacheInvFunc)();
#endif /* INCLUDE_L2_CACHE */

    /*
     * In SMP mode, the last part of sysToMonitor() must run on core 0
     * because it will jump to the bootrom.
     *
     * If we happen to already be on core 0, we first cause CPU 1 to n to clear
     * their caches and enter nap mode (or a parking loop) after which CPU 0
     * can proceed with shutdown.  If we are on a core n, we will proceed
     * to the spin loop mode after arranging for sysToMonitor() to be reinvoked
     * on core 0.
     */

    if (vxCpuIndexGet() == 0)
        {
        CPUSET_ZERO (cpuset);      /* implicitly sends CPC to all except self */

        cpcInvoke (0, (CPC_FUNC)sysCpuShutdownLoop, NULL, 0, VX_CPC_ASYNC);
        }
    else
        {
        CPUSET_ZERO (cpuset);
        CPUSET_SET  (cpuset, 0);     /* send CPC to CPU 0 only */

        cpcInvoke (cpuset, (CPC_FUNC)sysToMonitor, (void *)startType,
            sizeof(startType), VX_CPC_ASYNC);

        /* Wait here for sysToMonitor to shut us down */

        FOREVER;
        }

    /* CPC done, now safe to disable interrupts and swith to static TLB */
    
    vxMsrSet (0);    

    /* Only CPU 0 gets here */

    thisPhysCpu = vxCpuPhysIndexGet();

    i = 500;            /* 0.5sec (500 * 0.001) max wait for CPUn */
    do
        {
        sysMsDelay(1);

        countSmpCpus = VX_SMP_NUM_CPUS - 1;
        countCacheFlushed = 0;

        do
            {
            if (*(UINT32*)CPUn_INIT_STATUS_ADR(thisPhysCpu + countSmpCpus) ==
                mpCpuStateSleep)
                {
                countCacheFlushed++;
                }
            } while (--countSmpCpus > 0);

        if (countCacheFlushed == (VX_SMP_NUM_CPUS - 1))
            {
            break;
            }
        } while (--i > 0);

    /*
     * cacheDisable need to be done here. But the cacheDisable() API is not
     * supported in SMP, so call the AIM disable primitives directly to disable
     * cache
     */

#ifdef INCLUDE_L2_CACHE							 
    if (_pSysL2CacheDisable != NULL)
        (*_pSysL2CacheDisable)();
#endif /* INCLUDE_L2_CACHE */
								  
    mmu465TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], FALSE, NULL);

#else /* !_WRS_CONFIG_SMP */

    /* static TLB should be used to prevent TLB missing */
    
    vxMsrSet (0);
    
    /* disable cache */

#   ifdef  INCLUDE_CACHE_SUPPORT

#      ifdef  USER_D_CACHE_ENABLE
#         ifdef INCLUDE_L2_CACHE
       if (_pSysL2CacheFlush != NULL)
           (*_pSysL2CacheFlush) ();

       if (_pSysL2CacheInvFunc != NULL)
           (*_pSysL2CacheInvFunc) ();

       if (_pSysL2CacheDisable != NULL)
           (*_pSysL2CacheDisable) ();
#         endif /* INCLUDE_L2_CACHE */

       cacheAim465DisableD();
#      endif /* USER_D_CACHE_ENABLE */

#      ifdef  USER_I_CACHE_ENABLE
       cacheAim465DisableI();
#      endif /* USER_I_CACHE_ENABLE */

#endif /* INCLUDE_CACHE_SUPPORT */
#endif /* _WRS_CONFIG_SMP */

#if defined(INCLUDE_AMP_CPU) || defined(_WRS_CONFIG_SMP)

    /* 
     * For AMP & SMP, the cold reset would be executed,
     * Since immature SOC verison currentrly.
     */

    vxDbcr0Set(vxDbcr0Get () | _DBCR0_RST);

    /* should go here never */

    return (OK);

#else /* !(defined(INCLUDE_AMP_CPU) && defined(_WRS_CONFIG_SMP)) */

    /* jump to bootrom */

    (*pRom) (startType);

    /* in case we ever continue from ROM monitor */

    return (OK);
#endif /* defined(INCLUDE_AMP_CPU) || defined(_WRS_CONFIG_SMP) */

    }

/*******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board, always 0 (zero).
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
* This routine sets the processor number for the CPU board. Processor
* numbers should be unique on a single backplane.
*
* NOTE: This routine has no effect, since there is no VMEbus.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet(),
*/

void sysProcNumSet
    (
    int procNum     /* processor number */
    )
    {
    sysProcNum = procNum;
    }

/*******************************************************************************
*
* sysClkFreqGet - calculate clock
*
* This routine calculate and save clock configuration
*
* RETURNS: CSB clock frequenty
*
* ERRNO N/A
*/

UINT32 sysClkFreqGet (void)
    {
    return sysTimerClkFreq;
    }

/***************************************************************************
*
* sysCpuAvailableGet - return number of CPUs available
*
* This routine returns the number of CPUs available for the processor.
*
* RETURNS: the number of CPUs available for the processor
*
* ERRNO N/A
*/

UINT32 sysCpuAvailableGet (void)
    {

    UINT32 avail = SERENGETI_MAX_CPUS;

#ifdef INCLUDE_VXBUS
    vxbDevMethodRun (DEVMETHOD_CALL (vxbIntCtlrCpuAvail), &avail);
#endif

    return (avail);
    }

/*******************************************************************************
*
* sysApbClkFreqGet - get APB frequency
*
* RETURNS: Clock freq of SDHC controller
*
* ERRNO: N/A
*/

UINT32 sysApbClkFreqGet (void)
    {
    return (systemInfo.apbClkFreq);
    }

/*******************************************************************************
*
* sysSdhcClkFreqGet - get SDHC controller clock frequency
*
* RETURNS: Clock freq of SDHC controller
*
* ERRNO: N/A
*/

UINT32 sysSdhcClkFreqGet (void)
    {
    return (systemInfo.sdIoClkFreq);
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
*/

void sysUsDelay
    (
    int    delay        /* length of time in microsec to delay */
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
     * e.g., 400000000 cycles     1 tick      1 second            50 ticks
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
    int  delay                   /* length of time in MS to delay */
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


/* vxbus SIO driver support */

#ifdef INCLUDE_SIO_UTILS
#   define BSP_SERIAL_CHAN_GET  bspSerialChanGet
#else /* INCLUDE_SIO_UTILS */
#   define BSP_SERIAL_CHAN_GET  sysSerialChanGet
#endif /* INCLUDE_SIO_UTILS */

/*******************************************************************************
*
* BSP_SERIAL_CHAN_GET - get the serial channel
*
* This routine is called by the vxbus sio driver.
*
* RETURNS: always ERROR
*
*/

SIO_CHAN *BSP_SERIAL_CHAN_GET
    (
    int channel /* channel index */
    )
    {
    return ((SIO_CHAN *) ERROR);
    }

/*******************************************************************************
*
* sysInfoGet - Determines system information (clock frequencies, etc.)
*
* This routine fills out a SYS_INFO structure with the frequencies
* of each subsection of the APM86290.
*
* RETURNS: OK, or ERROR on unexpected results.
*/
STATUS sysInfoGet
    (
    SYS_INFO * sysInfo
    )
    {
    UINT32  nfRatio;
    UINT32  nrRatio;
    UINT32  odRatio;

    sysInfo->sysFreq = SYS_REFCLK;

    /* CPU PLL configurations */

    nfRatio = PLL_NF_RATIO(sysRead32 (SCU_SOCPLL0));
    nrRatio = PLL_NR_RATIO(sysRead32 (SCU_SOCPLL0));
    odRatio = PLL_OD_RATIO(sysRead32 (SCU_SOCPLL0));

    if (sysRead32 (SCU_SOCPLL0) & BYPASS_MASK)
        sysInfo->cpuPllFreq = sysInfo->sysFreq;
    else
        sysInfo->cpuPllFreq = (sysInfo->sysFreq * nfRatio) / (nrRatio * odRatio);

    sysInfo->cpu0ClkFreq = (sysInfo->cpuPllFreq / 2);
    sysInfo->cpu1ClkFreq = (sysInfo->cpuPllFreq / 2);

    sysInfo->plbClkFreq = (sysInfo->cpuPllFreq / 4);
    sysInfo->tracePpcClkFreq = 125000000;

    /* SOC PLL Configurations */

    nfRatio = PLL_NF_RATIO(sysRead32 (SCU_SOCPLL1));
    nrRatio = PLL_NR_RATIO(sysRead32 (SCU_SOCPLL1));
    odRatio = PLL_OD_RATIO(sysRead32 (SCU_SOCPLL1));

    if (sysRead32 (SCU_SOCPLL1) & BYPASS_MASK)
        sysInfo->socPllOutClkFreq = SYS_REFCLK;
    else
        sysInfo->socPllOutClkFreq = (SYS_REFCLK * nfRatio) / (nrRatio * odRatio);

    sysInfo->hbfClkFreq = sysInfo->socPllOutClkFreq / AXI_DIV_RATIO(sysRead32 (SCU_SOCDIV));
    sysInfo->ahbClkFreq = sysInfo->socPllOutClkFreq / AHB_DIV_RATIO(sysRead32 (SCU_SOCDIV));
    sysInfo->apbClkFreq = sysInfo->socPllOutClkFreq / APB_DIV_RATIO(sysRead32 (SCU_SOCDIV));
    sysInfo->pcieClkFreq = sysInfo->socPllOutClkFreq / PCIE_DIV_RATIO(sysRead32 (SCU_SOCDIV1));
    sysInfo->cryptoClkFreq = sysInfo->socPllOutClkFreq / CRYPTO_DIV_RATIO(sysRead32 (SCU_SOCDIV2));
    sysInfo->ebusClkFreq = sysInfo->socPllOutClkFreq / EBUS_DIV_RATIO(sysRead32 (SCU_SOCDIV5));
    sysInfo->sdIoClkFreq = sysInfo->socPllOutClkFreq / SDIO_DIV_RATIO(sysRead32 (SCU_SOCDIV5));

    /* DDR_PLL Configurations */

    nfRatio = PLL_NF_RATIO(sysRead32 (SCU_SOCPLL2));
    nrRatio = PLL_NR_RATIO(sysRead32 (SCU_SOCPLL2));
    odRatio = PLL_OD_RATIO(sysRead32 (SCU_SOCPLL2));

    if (sysRead32 (SCU_SOCPLL2) & BYPASS_MASK)
        sysInfo->ddrPllOutClkFreq = SYS_REFCLK;
    else
        sysInfo->ddrPllOutClkFreq = (SYS_REFCLK * nfRatio) / (nrRatio * odRatio);

    sysInfo->ddr2xClkFreq = sysInfo->ddrPllOutClkFreq / DDR2X_DIV_RATIO(sysRead32 (SCU_SOCDIV1));
    sysInfo->ddr1xClkFreq = sysInfo->ddr2xClkFreq / 2;

    /* ETH PLL Configurations */

    nfRatio = PLL_NF_RATIO(sysRead32 (SCU_SOCPLL4));
    nrRatio = PLL_NR_RATIO(sysRead32 (SCU_SOCPLL4));
    odRatio = PLL_OD_RATIO(sysRead32 (SCU_SOCPLL4));

    sysInfo->ethPllOutFreq = (SYS_REFCLK * nfRatio) / (nrRatio * odRatio);
    sysInfo->mac0TxClkFreq = sysInfo->ethPllOutFreq / RGMII0_DIV_RATIO(sysRead32 (SCU_SOCDIV4));
    sysInfo->mac1TxClkFreq = sysInfo->ethPllOutFreq / RGMII1_DIV_RATIO(sysRead32 (SCU_SOCDIV4));

    return (OK);
    }

#ifdef  INCLUDE_SHOW_ROUTINES
/*******************************************************************************
*
* sysInfoShow - displays APM86290 clocking information.
*
* This routine reads the current APM86290 clocking information and displays it.
*
* RETURNS: OK, or ERROR if sysInfoGet() does not return OK.
*/

STATUS sysInfoShow (void)
    {
    STATUS      status;
    SYS_INFO    sysInfo;
    UINT32      freq;
    UINT32      mhzInt;
    UINT32      mhzDec;
    UINT32      cpr0Pllc;

    printf ("%s\n", sysModel ());
    printf ("PVR:                       0x%x\n", vxPvrGet ());
    printf ("memory size:               %-6d MB\n",
           (UINT32) (sysPhysMemTop ()) / (1024 * 1024));

    status = sysInfoGet (&sysInfo);

    if (status != OK)
        return (ERROR);

    freq   = sysInfo.sysFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("SYS_CLK:                   %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.cpuPllFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("CPU_PLL_CLK:               %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.cpu0ClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("CPU0_CLK:                  %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.cpu1ClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("CPU1_CLK:                  %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.plbClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("PLB_CLK:                   %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.tracePpcClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("TRACE_PPC_CLK:             %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.socPllOutClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("SOC_PLL_OUT_CLK:           %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.hbfClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("HBF_CLK:                   %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.ahbClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("AHB_CLK:                   %4d.%2d MHz\n", mhzInt, mhzDec);
    freq   = sysInfo.apbClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("APB_CLK:                   %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");

#ifdef INCLUDE_PCI_BUS
    freq   = sysInfo.pcieClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("PCIE_REFCLK:               %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
#endif /* INCLUDE_PCI_BUS */

    freq   = sysInfo.cryptoClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("CRYPTO_CLK:                %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
    freq   = sysInfo.ebusClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("EBUS_CLK:                  %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");

#ifdef DRV_STORAGE_SDHC
    freq   = sysInfo.sdIoClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("SDIO_CLK:                  %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
#endif /* DRV_STORAGE_SDHC */

    freq   = sysInfo.ddrPllOutClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("DDR_PLL_OUT_CLK:           %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
    freq   = sysInfo.ddr2xClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("DDR2X_CLK:                 %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
    freq   = sysInfo.ddr1xClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("DDR1X_CLK:                 %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");

#ifdef INCLUDE_END
    freq   = sysInfo.ethPllOutFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("ETH_PLL_OUT_CLK:           %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
    freq   = sysInfo.mac0TxClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("MAC0_TXCLK:                %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
    freq   = sysInfo.mac1TxClkFreq;
    mhzInt = freq / 1000000;
    mhzDec = (freq  - (mhzInt * 1000000)) / 10000;
    printf ("MAC1_TXCLK:                %4d.%2d MHz\n", mhzInt, mhzDec);
    printf ("\n");
#endif /* INCLUDE_END */

    return (OK);
    }
#endif /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_WRLOAD

/******************************************************************************
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

    *((volatile int *) CPUn_FUNC_START_ADR (cpu)) = (int) 0;

    *((volatile int *) CPUn_STACK_START_ADR (cpu)) = ((int) entryPt - 0x20);

    /* This is used if core waiting after core1 did asynchronous reboot */

    *((volatile int *) CPUn_INIT_START_ADR (cpu)) = (int) entryPt;

    /* relase CPU from waiting loop */

    *((volatile int *) CPUn_INIT_STATUS_ADR (cpu)) = mpCpuStateReady;
    }

/******************************************************************************
*
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
    UINT32  physCpuIndex,
    void *  flags
    )
    {
    phys_cpuset_t physCpuset;

    PHYS_CPUSET_ZERO (physCpuset);

    /* can't load AMP image to core 0 or current core */

    if ((physCpuIndex == 0) || (physCpuIndex == vxCpuPhysIndexGet ()))
        return (ERROR);

    /* get phys_cpuset_t bit location of requested cpuId */

    PHYS_CPUSET_ZERO (physCpuset);
    PHYS_CPUSET_SET (physCpuset, physCpuIndex);

    /* shut down interrupts */

    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable),
                     (void *)physCpuIndex);

    /* configure AMP image parameters */

    *((volatile int *) CPUn_FUNC_START_ADR (physCpuIndex)) =
                     (int) 0; /* SMP specific */

    /* SMP specific not used just loaded */

    *((volatile int *) CPUn_STACK_START_ADR (physCpuIndex)) =
                     (int) RAM_LOW_ADRS - FRAMEBASESZ;

    /* used if waiting after CPU did asynchronous reboot */

    *((volatile int *)CPUn_INIT_START_ADR(physCpuIndex)) =
                     (int) MP_MAGIC_RBV;

    return (OK);
    }

#endif /* INCLUDE_WRLOAD */

#if defined(_WRS_CONFIG_SMP)

/*******************************************************************************
*
* sysCpuShutdownLoop - parking loop for others CPUs
*
* This routine is launched onto other core during system reboot. It locks out
* interrupts and puts the core into a tight loop or reboot themselves based on
* if CPU state has been set at REBOOT mode.
*
* RETURNS: N/A
*/

LOCAL void sysCpuShutdownLoop (void)
    {
    int token;
    FUNCPTR pRom = (FUNCPTR) (ROM_WARM_ADRS); /* Warm reboot */
    UINT32 physCpuIndex = vxCpuPhysIndexGet ();

    token = intCpuLock();

    /* shut down interrupts */

    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable), (void *)physCpuIndex);

    /* Report to CPU0 this CPU's cache flush is complete */

    *(UINT32*)CPUn_INIT_STATUS_ADR(physCpuIndex) = mpCpuStateSleep;

    FOREVER
        {
        if (*(UINT32*)CPUn_INIT_STATUS_ADR(physCpuIndex) == 
            mpCpuStateReboot)
            {
            /* jump to bootrom */

            (*pRom) (BOOT_NORMAL);
            }
        }
    }

/*******************************************************************************
*
* sysCpuInit - initialize additional core
*
* RETURNS: NONE
*
*/

void sysCpuInit
    (
    UINT32 physCpuIndex
    )
    {
    vxIvprSet((UINT32)VEC_BASE_ADRS);

    excIvorInit();

    /* flush cache */

    _func_cache440EarlyCleanup();

    if (_pSysL2CacheFlush != NULL)
        (*_pSysL2CacheFlush)();

    /* invalidate cache */

    cache465ICReset();
    cache465DCReset();

    if (_pSysL2CacheInvFunc != NULL)
        (*_pSysL2CacheInvFunc)();

    /* enable cache */

    if (_pSysL2CacheEnable != NULL)
        (*_pSysL2CacheEnable)();

    /* write the sysStaticTlbDesc[] table to the TLB */

    mmu465TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE, NULL);

    /*
     * static TLB entries are always written with TID 0 so they will
     * match for all settings of the PID register.
     */

    mmuPpcMmucrSet(mmuPpcMmucrGet() | (_MMUCR_STS_U << 16));

    mmuPpcPidSet(MMU_ASID_GLOBAL);

#if (SW_MMU_ENABLE == FALSE)
    vxMsrSet(vxMsrGet() | _PPC_MSR_IS | _PPC_MSR_DS);
#endif

#if defined(INCLUDE_TIMESTAMP)
    /*
     * Set the current timebase read from CPU0 onto CPU1-N.
     * The timebase is kept halt till it's re-started by the CPU0.
     */

    sysSmpTimeBaseSet (tbHI, tbLO);

#endif  /* INCLUDE_TIMESTAMP */

    return;
    }

/*******************************************************************************
*
* sysCpuLoop - CPU loop
*
* This routine is others CPUs idle loop function which will invoke the startup
* function specified in the sysCpuEnable function.
*
* RETURNS: NONE
*
*/

void sysCpuLoop(void)
    {
    unsigned int a = 0;
    FUNCPTR p;

    unsigned int physCpuIndex; /* physical cpu index */

    /*
     * The call to vxSdaInit() must be the first operation here.
     * This is because vxSdaInit() sets the r2 and r13 registers to the
     * SDA and SDA2 base registers. No C code must be placed before this
     * call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in sysCpu1Loop() */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

    physCpuIndex = vxPirGet();
    p = (FUNCPTR)(*(UINT32*)CPUn_FUNC_START_ADR(physCpuIndex));

    sysCpuInit(physCpuIndex);

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
    unsigned int      physCpuIndex,	/* physical cpu index */
    WIND_CPU_STATE  * pCpuState
    )
    {
    STATUS status = OK;

    /* make sure physCpuIndex is requesting an available CPU other than 0 */

    if ((physCpuIndex < 1) || (physCpuIndex > (sysCpuAvailableGet () - 1)))
        {
        return ERROR;
        }

    /*initialize status for sysCpusetReset () */

    *((int *)CPUn_FUNC_START_ADR(physCpuIndex)) = (int)pCpuState->regs.pc;
    *((int *)CPUn_STACK_START_ADR(physCpuIndex)) = (int)pCpuState->regs.spReg;
    *((int *)CPUn_INIT_START_ADR(physCpuIndex)) = (int)sysCpuLoop;
    *((int *)CPUn_INIT_STATUS_ADR(physCpuIndex)) = (int)mpCpuStateReady;

#if defined(INCLUDE_TIMESTAMP)
    {
#   ifndef VX_ENABLE_CPU_TIMEOUT
#       define VX_ENABLE_CPU_TIMEOUT   5   /* default timeout five seconds */
#   endif  /* VX_ENABLE_CPU_TIMEOUT */

    cpuset_t    cpuset;
    phys_cpuset_t physCpuset;
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
     * Save the current timebase on CPU0 into static valuable.
     * It will be set on CPU1-N later by sysCpuInit() and the timebase will be
     * synchronized between these cores.
     */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif  /* INCLUDE_TIMESTAMP */

#if defined(INCLUDE_TIMESTAMP)

    /*
     * usrCpuEnable() or kernelCpuEnable() relies on the timer to count timeout.
     * If the system clock is PPC Dec Timer, it can not be used while timebase
     * is disabled.
     * Use a magic number for busy loop to count timeout here.
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

        sysSmpTimeBaseSync();
        }

    intCpuUnlock(key);
    }

#endif /* INCLUDE_SYS_TIMESTAMP */
    return status;
    }

#if defined(INCLUDE_TIMESTAMP)

/*******************************************************************************
*
* sysCpcTimeBaseSet - CPC receiver for sysSmpTimeBaseInit
*
* This function receives the CPC from sysSmpTimeBaseInit,
* and passes the received parameters to sysSmpTimeBaseSet.
*
* RETURNS: OK
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
#endif  /* INCLUDE_TIMESTAMP */
#endif /* if defined(_WRS_CONFIG_SMP) */


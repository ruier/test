/* sysLib.c - LSI Acp34xx Board system-dependent routines library */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01x,12nov12,j_z  add SRIO support.
01w,25sep12,x_s  fixed the building warning issue. (WIND00378291)
01v,03aug12,x_s  added support for LSI ACP34XX JUNCTION target board.
01u,15may12,x_s  added support for silicon Rev 2.1.
01t,30mar12,x_z  added sysHwInit0() and moved vxPirSave() to it;
                 set up L2 Cache DCR base address in sysHwInit0(WIND00315263);
                 removed unused global function declaration. 
01s,05oct11,d_c  Defect WIND00309565 Fix WIND00309570
                 Enable cache snoop mode in cache mode masks.
01r,27apr11,x_s  replaced SMP cold reboot with warm reboot. (WIND00270698);
                 removed AMP state setting as running in sysHwInit.
01q,25apr11,x_s  made shared CPU space accessible by other Cores. (WIND00267959)
01p,13apr11,x_z  splited sysUartClkGet () to 2 routines and placed its timer
                 initialization to the 2 routines.
01o,15mar11,x_z  modified sysUartClkInit() to sysClkFreqInit () and added PPC
                 core clock and peripheral clock calculation into it;
                 replaced sysTimerClkFreq with sysCoreClkFreq.
01n,15mar11,x_s  added SMP M-N support.
01m,08mar03,x_z  replaced INCLUDE_PCI with DRV_PCIBUS_LSI_PCIEX.
01l,06mar11,x_s  added SMP support.
01k,28feb11,x_z  replaced CACHE_TEXT_UPDATE with sysDcbf to flush the stub to
                 memory for other cores.
01j,23feb11,x_z  fixed sysModel issue.
01i,10feb11,x_z  merged static TLB entries to one for External Bus Controller.
01h,22jan11,x_s  added PCIe support.
01g,12jan11,x_z  removed unnecessary _MMU_TLB_ATTR_G for _WRITETHROUGH and
                 _COPYBACK.
01f,28dec10,x_z  moved L2 Cache support to ARCH and fixed warm reset issue.
01f,22dec10,x_s  added I2C support.
01e,21dec10,x_s  added SPI support.
01d,02dec10,x_z  fixed sysPhysMemDesc[] to support RTP.
01c,26oct10,x_z  added support for uAMP and USB.
01b,19oct10,x_z  fixed warm reset issue and code clean.
01a,01jun10,x_z  written.
*/

/*
DESCRIPTION
This library provides board-specific routines for the LSI ACP34XX ELPASO Board
BSP.

It #includes the following chip drivers:
    sysNandFlash.c -    NAND Flash memory driver

if INCLUDE_LSI_SPI is defined, it #includes:
    sysLsiSpi.c -      LSI SPI controller driver
    sysSpiFlash.c -    SPI Flash memory driver

if INCLUDE_I2C is defined, it #includes:
    sysLsiI2c.c -      LSI I2C controller driver

if INCLUDE_I2C and INCLUDE_AT24C_MEM are defined, it #includes:
    at24cEeprom.c -    Atmel AT24C01B EEPROM driver

if INCLUDE_VXBUS is defined, it #includes:
    vxBus.h -           vxBus subsystem header file
    hwconf.c -          hardware configuration support module

if INCLUDE_MMU_BASIC is defined or INCLUDE_MMU_FULL is defined, it #includes:
    vmLibP.h -          architecture independent mmu interface private header

INCLUDE FILES: cacheLib.h asm.h vme.h iv.h esf.h ioLib.h sysLib.h
               memLib.h excLib.h logLib.h vxLib.h string.h intLib.h taskLib.h
               stdio.h vmLibP.h mmu476Lib.h

SEE ALSO:
\tb "VxWorks Programmer's Guide: Configuration",
\tb "PowerPC 476FP Embedded Processor Core User's Manual",
\tb "Axxia Communication Processor (ACP3400) Technical Guide",
\tb "Axxia Communication Processor ACP3448 Evaluation Kit User Guide",
\tb "Axxia Communication Processor Boot Process and Configuration Application Note"
*/

/* includes */

#include <vxWorks.h>
#include <cacheLib.h>
#include <asm.h>
#include <iv.h>
#include <esf.h>
#include <ioLib.h>
#include <sysLib.h>
#include <memLib.h>
#include <excLib.h>
#include <logLib.h>
#include <vxLib.h>
#include <string.h>
#include <intLib.h>
#include <taskLib.h>
#include <stdio.h>
#include <private/vmLibP.h>
#include <arch/ppc/mmu476Lib.h>
#include <hwif/intCtlr/vxbMpicIntCtlr.h>
#include "config.h"

#ifdef _WRS_CONFIG_SMP
#   include <private/cpcLibP.h>
#   include <private/kernelLibP.h>		/* KERNEL_ENTERED_ME() */
#endif  /* _WRS_CONFIG_SMP */

/* defines */

/*
 * PPC476FP cache library requires I and D cache on/off configurations to be
 * identical.  If one is enabled, the other has to be. If both are, they must
 * both be on or both be off.
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
 * remain consistent with the memory map.
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
#elif (USER_D_CACHE_MODE == (CACHE_COPYBACK | CACHE_SNOOP_ENABLE))
#   define _WRITETHROUGH        _MMU_TLB_ATTR_W
#   define _WRITETHROUGH_OR_G   _MMU_TLB_ATTR_W
#   define _COPYBACK            0
#endif /* USER_D_CACHE_MODE */

/* globals */

IMPORT UINT32   vxCpuPhysIndexGet (void);
IMPORT void     hardWareInterFaceInit (void);
IMPORT void     vxDbcr0Set (UINT32 val);
IMPORT void     sysDcbf (UINT32);
IMPORT void     vxPirSave (void);
IMPORT STATUS   ppc476L2CacheBaseSet (UINT32 cpuId, UINT32 baseAdrs);

#ifdef  _WRS_CONFIG_SMP
IMPORT void cacheAim476TlbSetup(UINT32 cacheEnable);
IMPORT STATUS cache476Clear (CACHE_TYPE);
IMPORT STATUS cache476Invalidate (CACHE_TYPE);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
IMPORT void excIvorInit();
IMPORT void mmuPpcPidSet(UINT32);
IMPORT void mmu476TlbStaticInit(void);
IMPORT void vxPirSet(UINT32);
IMPORT UINT32 vxPirGet (void);
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_SIO_UTILS
IMPORT void     sysSerialConnectAll(void);
#endif

int    sysBus        = BUS;            /* system bus type (VME_BUS, etc)  */
int    sysCpu        = CPU;            /* system CPU type (PPC440EPx)     */
char * sysBootLine   = BOOT_LINE_ADRS; /* address of boot line            */
char * sysExcMsg     = EXC_MSG_ADRS;   /* catastrophic message area       */
int    sysCoreClkFreq;                 /* PPC core clock frequency        */
int    sysUartTimerDiv;                /* UART timer divisor              */
int    sysPerClkFreq;                  /* Peripherals clock frequency     */

/* processor number of this CPU */

#ifdef INCLUDE_AMP_CPU
int     sysProcNum = AMP_CPU_NUMBER;
#else
int     sysProcNum = 0;           /* processor number of this CPU */
#endif /* INCLUDE_AMP_CPU */

BOOL  sysVmeEnable = FALSE;     /* by default no VME */

/* locals */

/*
 * Static TLB memory mappings
 *
 * The chip provides 6 Bolted TLB entries to provide a linear mapping of large
 * blocks that emulates a 'real' mode. Bolted entries are automatically skipped
 * by hardware-assisted way selection.
 *
 * The remaining entries are dynamically used and reused with hardware
 * assistance to implement the VM library's address map. These are controlled by
 * the sysPhysMemDesc array.
 */

#define TLB_WORD0(x)   ((x) << _MMU_TLB_WORD0_SHIFT)

TLB_ENTRY_DESC sysStaticTlbDesc [] = {

    /* DDR SDRAM 0 - Cached */

    {
    DRAM_BASE_ADRS, DRAM_BASE_ADRS_PHYS_HI, DRAM_BASE_ADRS_PHYS_LO,
    TLB_WORD0 (_MMU_TLB_TS_0 | _MMU_TLB_SZ_1G) | _COPYBACK | _MMU_TLB_ATTR_M |
    _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
#if (SW_MMU_ENABLE == 1)
    | _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX
#endif
    },

    /* External Bus Controller */


    {
    PER_BASE_ADRS, PER_BASE_ADRS_PHYS_HI, PER_BASE_ADRS_PHYS_LO,
    TLB_WORD0 (_MMU_TLB_TS_0 | _MMU_TLB_SZ_16M) | _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_M | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },

    /* PCIE */

#ifdef DRV_PCIBUS_LSI_PCIEX

    {
    PCIE0_CFG_ADRS,
    PCIE_BASE_ADRS_PHYS_HI, PCIE_BASE_ADRS_PHYS_LO,
    TLB_WORD0 (_MMU_TLB_TS_0 | _MMU_TLB_SZ_1G) | _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_M | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_PERM_R
    },
#endif /* DRV_PCIBUS_LSI_PCIEX */

#ifdef DRV_SRIO_LSI
    /* SRIO */
    {
    LSI_RIO_MEM_ADRS,
    SRIO_BASE_ADRS_PHYS_HI, SRIO_BASE_ADRS_PHYS_LO,
    TLB_WORD0 (_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M) | _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_M | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_PERM_R
    }
#endif/* DRV_SRIO_LSI */
    };

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE)
 * array used by the MMU to translate addresses with single page (4k)
 * granularity.
 *
 * In VxWorks 6.x, both MMU_BASIC and MMU_FULL is supported. The VxWorks
 * vmBaseLib and vmLib support the full 36-bit PLB address space because
 * PHYS_ADDR is defined with 64-bit storage. Therefore, the VM library
 * can map all devices.  MMU_FULL supports multiple VM contexts, which
 * are associated with RTPs to provide protected address spaces.
 *
 * PTEs are held in a 2-level page table. Each valid VM context has one
 * Level 1 page table which maps the entire 4GB virtual address space.
 * The Level 1 page table points to several Level 2 page tables which
 * each map portions of a contiguous 4MB address space.  Individual page
 * table entries map an address space of size VM_PAGE_SIZE (typically
 * 4KB).
 *
 * With MMU_FULL, level 2 page tables may be shared between contexts
 * where the mappings are identical -- typically the kernel's Level 2
 * table pointers are shared with the RTPs.
 *
 */

#define PHYS_XBIT_ADDR(h, l)   (((PHYS_ADDR)(h) << 32) + (l))

PHYS_MEM_DESC sysPhysMemDesc [] =
    {

    /* RAM */
    
#if !defined(INCLUDE_AMP_CPU) || \
    (defined(INCLUDE_AMP_CPU) && defined(INCLUDE_AMP_CPU_00))

    {
    (VIRT_ADDR) DRAM_BASE_ADRS,
    PHYS_XBIT_ADDR (DRAM_BASE_ADRS_PHYS_HI, DRAM_BASE_ADRS_PHYS_LO),
    DRAM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE |
    VM_STATE_MEM_COHERENCY
    },

#else /* AMP && !INCLUDE_AMP_CPU_00 */

    /* 
     * Shared CPU space to use by AMP, the space is below RAM_LOW address of core0
     * and the size is 0x10000 * 3 (max CPU index).
     */

    {
    (VIRT_ADDR) CPUn_SPACE(3),
    PHYS_XBIT_ADDR (DRAM_BASE_ADRS_PHYS_HI, DRAM_BASE_ADRS_PHYS_LO + CPUn_SPACE(3)),
    CPUn_SPACE(0) - CPUn_SPACE(3),
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE |
    VM_STATE_MEM_COHERENCY
    },
    
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    PHYS_XBIT_ADDR (DRAM_BASE_ADRS_PHYS_HI, LOCAL_MEM_LOCAL_ADRS),
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE |
    VM_STATE_MEM_COHERENCY
    },

#   if defined(INCLUDE_MIPC_SM) && !defined(INCLUDE_AMP_CPU_03)

    /*
     * If shared memory pool is not in the memory range of CPU 3 image,
     * the entry must be adjusted to fit it.
     */

    {
    (VIRT_ADDR) MIPC_SM_SYSTEM_POOL_BASE,
    PHYS_XBIT_ADDR (DRAM_BASE_ADRS_PHYS_HI, MIPC_SM_SYSTEM_POOL_BASE),
    MIPC_SM_SYSTEM_POOL_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE |
    VM_STATE_MEM_COHERENCY
    },

#   endif /* INCLUDE_MIPC_SM && !INCLUDE_AMP_CPU_03 */

#endif /* !AMP || (AMP && INCLUDE_AMP_CPU_00) */

    /*  External Bus Controller */

    {
    (VIRT_ADDR) PER_BASE_ADRS,
    PHYS_XBIT_ADDR (PER_BASE_ADRS_PHYS_HI, PER_BASE_ADRS_PHYS_LO),
    PER_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED | VM_STATE_MEM_COHERENCY
    },

#ifdef DRV_PCIBUS_LSI_PCIEX
    {
    (VIRT_ADDR) PCIE0_CFG_ADRS,
    PHYS_XBIT_ADDR (PCIE_BASE_ADRS_PHYS_HI, PCIE_BASE_ADRS_PHYS_LO),
    PCIE_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED | VM_STATE_MEM_COHERENCY
    },
#endif /* DRV_PCIBUS_LSI_PCIEX */

#ifdef DRV_SRIO_LSI
    {
    (VIRT_ADDR) LSI_RIO_MEM_ADRS,
    PHYS_XBIT_ADDR (SRIO_BASE_ADRS_PHYS_HI, SRIO_BASE_ADRS_PHYS_LO),
    LSI_RIO_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE_NOT |
    VM_STATE_GUARDED | VM_STATE_MEM_COHERENCY
    }
#endif/* DRV_SRIO_LSI */
    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* Locals */

#ifdef DRV_PCIBUS_LSI_PCIEX
LOCAL void sysPciePreCfg (void); /* PCIe preconfig routine */
#endif /* DRV_PCIBUS_LSI_PCIEX */

#ifdef _WRS_CONFIG_SMP
#   if defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;               /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;               /* lower 32 bit value of timebase */
#   endif   /* INCLUDE_TIMESTAMP */
LOCAL void sysCpuShutdownLoop (void);
#endif /* _WRS_CONFIG_SMP */

/* forward declarations */

IMPORT void (*_vxb_usDelayRtn) (int delayTime);
IMPORT void (*_vxb_msDelayRtn) (int delayTime);
IMPORT void (*_vxb_delayRtn) (void);
IMPORT void sysBootStub (void);

void   sysMsDelay (int delay);
void   sysUsDelay (int delay);
void   sysDelay (void);
void   sysClkFreqInit (void);
void   sysCpusetReset (phys_cpuset_t physCpuset);
UINT32 sysCpuAvailableGet (void);
BOOL   sysSocIsAcp3423 (void);

/* BSP Drivers */

#include "sysNandFlash.c"       /* flash driver */
#include "sysNet.c"             /* bootrom 'M' command support */

/* vxbus support */

#include <hwif/vxbus/vxBus.h>

/* vxBus drivers */

#include "hwconf.c"

/* SPI & SPI Flash */

#ifdef INCLUDE_LSI_SPI
#   include "sysLsiSpi.c"
#   include "sysSpiFlash.c"
#endif /* INCLUDE_LSI_SPI */

/* I2C & EEPROM */

#ifdef INCLUDE_I2C
#   include "sysLsiI2c.c"          /* I2C api libary */
#   ifdef INCLUDE_AT24C_MEM
#       include "at24cEeprom.c"    /* EEPROM dirver */
#   endif
#endif /* INCLUDE_I2C */

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU.
*
*/

char * sysModel (void)
    {
    switch (vxPvrGet ())
        {
        case ACP3448_V1_PVR:
            return (SYS_MODEL_ACP3448" V1");
        case ACP3448_V2_PVR:
            return (SYS_MODEL_ACP3448" V2");
        case ACP3448_V2_1_PVR:
            return (SYS_MODEL_ACP3448" V2.1");           
        case ACP3423_V2_PVR:
            return (SYS_MODEL_ACP3423" V2");
        default:
            return (SYS_MODEL"XX");
        }
    }

/******************************************************************************
*
* sysSocIsAcp3423 - check if SOC is ACP3423.
*
* This routine checks if SOC is ACP3423.
*
* RETURNS: TRUE if ACP3423 of FALSE if not
*
* ERRNO: N/A
*/

BOOL sysSocIsAcp3423(void)
    {

    if ((vxPvrGet () & ACP34XX_PVR_SOC_MASK) == 
        (ACP3423_V2_PVR & ACP34XX_PVR_SOC_MASK))
        return TRUE;
    else
        return FALSE;
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
    int i;

    /* Saved PIR register for CPU index calculation */

    vxPirSave ();

    /* Set up L2 cache DCR base address */

#ifdef INCLUDE_CACHE_SUPPORT
#   ifdef _WRS_CONFIG_SMP

    /* Set for all the CPUs */

    for (i = 0; i < vxCpuConfiguredGet (); i++)
        {
        ppc476L2CacheBaseSet (CPU_LOGICAL_TO_PHYS (i), 
                              L2CACHE_BAn (CPU_LOGICAL_TO_PHYS (i)));
        }
#   else /* !_WRS_CONFIG_SMP */
    ppc476L2CacheBaseSet (CPU_LOGICAL_TO_PHYS (0), 
                          L2CACHE_BAn (CPU_LOGICAL_TO_PHYS (0)));
#   endif /* _WRS_CONFIG_SMP */
#endif /* INCLUDE_CACHE_SUPPORT */
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the LSI ACP34xx ELPASO
* board. This routine clears and turns off the external interrupts. It
* is called from usrInit() in usrConfig.c.
*
* NOTE: This routine should not be called directly by the user
* application.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {
    int i;

    /* timer and clock initialize */

    sysClkFreqInit ();

    /* Set the 476 Interrupt Vector Prefix Register */

    vxIvprSet (VEC_BASE_ADRS);

    /*
     * For cold reset, LSI stage 1 loader will put the other cores than core 0
     * to sleep status. Core 0 must send IPI0 to other cores to wake them up,
     * and then other cores will jump to physical address 0x0.
     *
     * For more detail, please see "Axxia Communication Processor Boot Process
     * and Configuration Application Note".
     */

    if ((vxCpuPhysIndexGet () == 0) && ((vxDbsrGet () & _DBSR_MRR) != 0))
        {

        /* clear reset status */

        vxDbsrClear (_DBSR_MRR);

        /* must copy stub to physical address 0x0 first */

        bcopy ((const char *) sysBootStub, (char *) 0, ACP34XX_BOOT_STUB_SIZE);
        sysDcbf (0);

        /* wake all the other cores up */

        for (i = 1; i < sysCpuAvailableGet (); i++)
            {

            /* make the core enter one loop */

            *((volatile int *) CPUn_INIT_STATUS_ADR (i)) = mpCpuStateSleep;
            sysDcbf (CPUn_SPACE (i));

            /* send IPI0 to wake up the core */

            dcrOutLong (MPIC0_DCR_BA + MPIC_IPI_DPATCH_REG0,
                MPIC_CPUID_DST (i));
            while (*((volatile int *) CPUn_INIT_STATUS_ADR (i)) !=
                mpCpuStateBooting)
                sysUsDelay (10);
            }
        }

#ifdef _WRS_CONFIG_SMP
    /* For SMP, logical core 1 to n state would be set with REBOOT */

    for (i = 1; i < vxCpuConfiguredGet (); i++)
        {
        *((volatile int *) CPUn_INIT_STATUS_ADR (CPU_LOGICAL_TO_PHYS(i))) =
            mpCpuStateReboot;
        sysDcbf (CPUn_SPACE (CPU_LOGICAL_TO_PHYS(i)));
        }

#endif /* _WRS_CONFIG_SMP */

#ifdef DRV_PCIBUS_LSI_PCIEX
    sysPciePreCfg();
#endif /* DRV_PCIBUS_LSI_PCIEX */

    /* initialize HWIF Pre-Kernel */

    _vxb_usDelayRtn = &sysUsDelay;
    _vxb_msDelayRtn = &sysMsDelay;
    _vxb_delayRtn = &sysDelay;
    hardWareInterFaceInit ();
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

    vxbDevInit();

#ifdef INCLUDE_SIO_UTILS
    /* connect all SIO_CHAN devices */

    sysSerialConnectAll ();
#endif /* INCLUDE_SIO_UTILS */

    /* intialize NAND FLASH */

    if (sysSocIsAcp3423())
        sysNandInit (NAND_TYPE_EP501G3);
    else
        sysNandInit (NAND_TYPE_EP501G1);

    /* initialize SPI controller */

#ifdef INCLUDE_LSI_SPI
    sysSpiInit ();
#endif /* INCLUDE_LSI_SPI */

    /* initialize I2C controller */

#ifdef INCLUDE_I2C
    sysI2cInit();
#endif /* INCLUDE_I2C */

#ifdef INCLUDE_VXBUS
    taskSpawn("tDevConn", 11, 0, 10000,
              vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
#endif /* INCLUDE_VXBUS */

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

        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
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
#endif  /* INCLUDE_EDR_PM */
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

    cache476Clear ((CACHE_TYPE) 0);
    
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
    
    intCpuLock ();      /* CPC done, now safe to disable interrupts */
    
    /*
     * Only CPU 0 gets here, both physical core 0 and logical core 0 on an SMP 
     * M-N system would get here.
     */

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

    cacheAim476TlbSetup(FALSE);
#else /* !_WRS_CONFIG_SMP */
    /* disable cache */

#   ifdef  INCLUDE_CACHE_SUPPORT

#      ifdef  USER_D_CACHE_ENABLE
        cacheDisable ((CACHE_TYPE) _DATA_CACHE);
#      endif

#      ifdef  USER_I_CACHE_ENABLE
        cacheDisable ((CACHE_TYPE) _INSTRUCTION_CACHE);
#      endif
#   endif /* INCLUDE_CACHE_SUPPORT */

#endif  /* _WRS_CONFIG_SMP */

    vxMsrSet (0);

#if defined(INCLUDE_AMP_CPU) || defined(_WRS_CONFIG_SMP)
    /* Physical core 0 would reset the entire board */

#   ifdef INCLUDE_AMP_CPU
    if (cpuId == 0)
        {
        /* core 0 can reset other cores by reset the entire board */

        vxDbcr0Set ((vxDbcr0Get () & (~(_DBCR0_RST_MASK_U << 16))) |
            (_DBCR0_RST_SYS_U << 16));

        /* should go here never */

        return (OK);
        }
    else
#  endif /* INCLUDE_AMP_CPU */                
        {
        /* shut down interrupts */

        vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable), (void *)cpuId);

#   ifdef _WRS_CONFIG_SMP
        /*
         * For SMP M-N, logical core 0 should make others slave core reboot, but
         * for LSI ELPASO board can NOT implement this point. So logical core 0
         * set others slave core's state to REBOOT, so that oters slave core can
         * reboot themselves. And logical 0 would jump to bootrom also.
         */
		 
        if (vxCpuIndexGet() == 0)
            {               
            for (i = 0; i < vxCpuConfiguredGet (); i++)
                {
                cpuId = CPU_LOGICAL_TO_PHYS(i);
#   endif /* _WRS_CONFIG_SMP */
        *((volatile int *) CPUn_FUNC_START_ADR (cpuId)) =
            (int) 0; /* SMP specific */

        /* SMP specific not used just loaded */

        *((volatile int *) CPUn_STACK_START_ADR (cpuId)) =
        (int) RAM_LOW_ADRS - FRAMEBASESZ;

        /* This is used if core waiting after core1 did asynchronous reboot */

        *((volatile int *) CPUn_INIT_START_ADR (cpuId)) = (int) MP_MAGIC_RBV;

        /* set the core status */

        *((volatile int *) CPUn_INIT_STATUS_ADR (cpuId)) = mpCpuStateReboot;

        sysDcbf (CPUn_SPACE (cpuId));
        
#   ifdef _WRS_CONFIG_SMP
                }
            }
#   endif /* _WRS_CONFIG_SMP */ 
        /* jump to bootrom */

        (*pRom) (startType);

        /* in case we ever continue from ROM monitor */

        return (OK);
        }
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
* RETURNS: The processor number for the CPU board.
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
    int procNum /* processor number */
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
    return sysCoreClkFreq;
    }

/*******************************************************************************
*
* sysTimerClkFreqGet - calculate AXI Timer clock
*
* This routine return AXI Timer clock frequenty.
*
* RETURNS: AXI Timer clock frequenty
*
* ERRNO N/A
*/

UINT32 sysTimerClkFreqGet (void)
    {
    return sysPerClkFreq;
    }

/*******************************************************************************
*
* sysClkFreqInit - calculate clock frequence for system, peripheral and UART
*
* This routine calculates the system and peripheral clock and sets the divisor
* to the source timer for UART.
*
* RETURNS: N/A
*
* ERRNO N/A
*/

void sysClkFreqInit (void)
    {
    UINT32 divisor;
    UINT32 ibrd;
    UINT32 clkCtrl;
    UINT32 clkStatus;
    UINT8  clkPllUnlockMsg[] = "PPC PLL is unclocked, so restart the board!\n";
    UINT8  clkUndefMsg[] = "PPC clock is undefined, so restart the board!\n";

    clkCtrl = dcrInLong (SYSCLK_MCGC);
    clkStatus = dcrInLong (SYSCLK_MCGS);

    /* Peripheral clock */

    if (0 == (clkCtrl & SYSCLK_MCGC_PER_CLK_REF0))
        sysPerClkFreq = BSP_CLK_REF0_FREQ;
    else
        {
        if (0 == (clkStatus & SYSCLK_MCGS_PPC_PLL_LOCK))
            {
            strncat(sysExcMsg, clkPllUnlockMsg, strlen(clkPllUnlockMsg));
            sysExcMsg += strlen(sysExcMsg);
            sysToMonitor(BOOT_WARM_NO_AUTOBOOT);

            return; /* should go here never */
            }
        else
            sysPerClkFreq = BSP_PPC_PLL_OUTB_FREQ;
        }

    /* PPC core clock */

    if (SYSCLK_MCGC_PPC_CLK_REF0 == SYSCLK_MCGC_PPC_CLK (clkCtrl))
        sysCoreClkFreq = BSP_CLK_REF0_FREQ;
    else if (SYSCLK_MCGC_PPC_CLK_PLLA == SYSCLK_MCGC_PPC_CLK (clkCtrl))
        sysCoreClkFreq = BSP_PPC_PLL_OUTA_FREQ;
    else if (SYSCLK_MCGC_PPC_CLK_PLLA_D2 == SYSCLK_MCGC_PPC_CLK (clkCtrl))
        sysCoreClkFreq = BSP_PPC_PLL_OUTA_FREQ / 2;
    else
        {
        strncat(sysExcMsg, clkUndefMsg, strlen(clkUndefMsg));
        sysExcMsg += strlen(sysExcMsg);
        sysToMonitor(BOOT_WARM_NO_AUTOBOOT);

        return; /* should go here never */
        }

    /*
     * UART clock
     *
     * The UART clock source is AXI Timer 1 and 2, so the timer load value must
     * be set to the divisor. Since the IBDR (integer part of the baud rate
     * divisor) of UART is a 16 bit quatity, find the minimum load value that
     * will let the IBDR/FBDR result in the desired baud rate.
     */

    divisor = 1;

    do
        {
        divisor++;
        ibrd = (sysPerClkFreq / divisor) / (16 * CONSOLE_BAUD_RATE);
        } while(ibrd > 0xff);

    sysUartTimerDiv = divisor;
    }

/*******************************************************************************
*
* sysUart0ClkGet - get UART0 clock and initialize UART0 timer
*
* This routine gets the divisor to the source timer and initialize it for UART0.
*
* RETURNS: UART0 clock frequenty
*
* ERRNO N/A
*/

UINT32 sysUart0ClkGet (void)
    {
    sysOutLong (UART0_TIMER_BA + AXIS_TIMER_CTRL, 0);
    sysOutLong (UART0_TIMER_BA + AXIS_TIMER_LOAD_VAL, sysUartTimerDiv - 1);
    sysOutLong (UART0_TIMER_BA + AXIS_TIMER_CTRL,
        AXIS_TIMER_EN | AXIS_TIMER_MODE_PERIOD);

    return (sysPerClkFreq / sysUartTimerDiv);
    }

/*******************************************************************************
*
* sysUart1ClkGet - get UART1 clock and initialize UART1 timer
*
* This routine gets the divisor to the source timer and initialize it for UART1.
*
* RETURNS: UART1 clock frequenty
*
* ERRNO N/A
*/

UINT32 sysUart1ClkGet (void)
    {
    sysOutLong (UART1_TIMER_BA + AXIS_TIMER_CTRL, 0);
    sysOutLong (UART1_TIMER_BA + AXIS_TIMER_LOAD_VAL, sysUartTimerDiv - 1);
    sysOutLong (UART1_TIMER_BA + AXIS_TIMER_CTRL,
        AXIS_TIMER_EN | AXIS_TIMER_MODE_PERIOD);

    return (sysPerClkFreq / sysUartTimerDiv);
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

    UINT32 avail;

    if (sysSocIsAcp3423())
        {
        /* 
         * For ACP342X core, supppored maximum core numbers can not be correctly 
         * acquired from hardware register. So a solid value is given.
         */
         
        avail = ACP342X_MAX_CPUS; 
        }
    else
        {
        avail = ACP344X_MAX_CPUS;             
    vxbDevMethodRun (DEVMETHOD_CALL (vxbIntCtlrCpuAvail), &avail);
        }

    return (avail);
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
    int delay        /* length of time in microsec to delay */
    )
    {
    UINT baselineTickCount;
    UINT curTickCount;
    UINT terminalTickCount;
    int  actualRollover = 0;
    int  calcRollover = 0;
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

    oneUsDelay = ((sysCoreClkFreq + 1000000) / 1000000);

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
* sysMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by calling sysUsDelay
* with (delay*1000).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    int delay     /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (UINT32) delay * 1000 );
    }

/*******************************************************************************
*
* sysDelay - Fixed 1ms delay. Just calls sysMsDelay
*
* This routine consumes 1ms of time. It just calls sysMsDelay.
*
* RETURNS: N/A
*
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

#ifdef DRV_PCIBUS_LSI_PCIEX

/* only INTA pin is supported */

LOCAL UCHAR sysPciEx0IntRoute [4] = {
    INUM_PEI0,
    INUM_PEI0,
    INUM_PEI0,
    INUM_PEI0,
};

/*******************************************************************************
*
* sysPciEx0AutoCfgIntAssign - autoconfig PCI Express 0 interrupt line number
*
* This routine assigns the interrupt line number for the special PCI Express 0 
* int pin.
*
* RETURNS: PCI interrupt line number for the given pin
*/

UCHAR sysPciEx0AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function in question */
    UCHAR           pin             /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for the bus.
     * If OK, return INT level, else we return 0xff.
     */

    if ((pin > 0) && (pin < 5))
        tmpChar = sysPciEx0IntRoute [(pin-1)];

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPciEx0AutoCfgInclude - PCI Express 0 autoconfig support routine
*
* This routine performs the PCI Express 0 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx0AutoCfgInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC *    pLoc,          /* pointer to function in question */
    UINT         devVend        /* deviceID/vendorID of device */
    )
    {

    /*
     * Host controller itself (device number is 0) won't be configured;
     */

    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return (ERROR);

    return (OK); /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciePreCfg - preconfigure the specified PCI Express port
*
* This routine preconfigures the specified PCI Express port.
*
* RETURNS: NONE
*/

LOCAL void sysPciePreCfg (void)
    {
    UINT32 rstCon;
    UINT32 phyCtrl0;
    
    rstCon = dcrInLong(SYSRST_MAIN_RST_CON);

    /* see if system is in PCIe mode, if yes setting mode */

    if (rstCon & SYSRST_MAIN_RST_CON_PCI_SEL)
        {
        phyCtrl0 = sysInLong(GPREG_PHY_CTRL0);
        
        switch ((rstCon & SYSRST_MAIN_RST_CON_PCI_MOD) >> 
                SYSRST_MAIN_RST_CON_PCI_MOD_SHIFT)
            {
            case 0:
                phyCtrl0 |= GPREG_PHY_CTRL0_X1;
                break;
            case 1:
                phyCtrl0 |= GPREG_PHY_CTRL0_X2;
                break;
            case 2:
                phyCtrl0 |= GPREG_PHY_CTRL0_X3;
                break;
            case 3:
                phyCtrl0 |= GPREG_PHY_CTRL0_X4;
                break;
            default:
                break;
            }
        
        sysOutLong(GPREG_PHY_CTRL0, phyCtrl0);
        
        /* 
         * The 2nd of U-Boot has reset PCIe PHY already, here found re-reset PCIe 
         * phy is failed. Add code here in case U-Boot is discarded and whole 
         * VxWorks native bootrom is needed.
         *
         * rstMis = dcrInLong(SYSRST_MAIN_RST_MIS);
         * dcrOutLong(SYSRST_MAIN_RST_MIS, rstMis | SYSRST_MAIN_RST_MIS_PHY);
         * dcrOutLong(SYSRST_MAIN_RST_MIS, rstMis & ~SYSRST_MAIN_RST_MIS_PHY);
        */
        }
    }
#endif /* DRV_PCIBUS_LSI_PCIEX */

/* vxbus SIO driver support */

#ifdef INCLUDE_SIO_UTILS
#   define BSP_SERIAL_CHAN_GET  bspSerialChanGet
#else
#   define BSP_SERIAL_CHAN_GET  sysSerialChanGet
#endif

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

#if defined(INCLUDE_USB) && defined(INCLUDE_EHCI)
/******************************************************************************
*
* sysEhciInit - intializes the on-chip EHCI
*
* This routine initializes the USB DR clock, configures the DR pins and
* enables the ports.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

void sysEhciInit ()
    {

    /* Setup GPREG for USB to enable the 6-bit address line */

    sysOutLong (GPREG_USB_EXT_ADRS, 0);

    /* select USB host mode */

    sysOutLong (USBDR_USBMODE, USBDR_USBMODE_CM_HOST);

    /* Enable ports */

    sysOutLong (USBDR_PORTSC, USBDR_PORTSC_PTS_ULPI);
    }

/***************************************************************************
*
* usbOpAfterReset - set to host mode after usb ehcd controller reset
*
* This routine sets usb DR controller to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

void usbOpAfterReset ()
    {

    /* select USB host mode */

    sysOutLong (USBDR_USBMODE,
        sysInLong (USBDR_USBMODE) & (~USBDR_USBMODE_CM) | USBDR_USBMODE_CM_HOST);

    /* workaround for USB Host Mode Retry Failure */

    sysInLong (USBDR_HWTXBUF);
    sysInLong (USBDR_TXTTFILLTUNING);

    /* configure Tx burst size */

    sysOutLong (USBDR_BURSTSIZE,
        (sysInLong (USBDR_BURSTSIZE) & (~USBDR_BURSTSIZE_TX_MASK)) |
        USBDR_BURSTSIZE_TX_DEF);
    }
#endif /* defined(INCLUDE_USB) && defined(INCLUDE_EHCI) */

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

    *((volatile int *) CPUn_FUNC_START_ADR (cpu)) = (int) 0; /* SMP specific */

    /* SMP specific not used just loaded */

    *((volatile int *) CPUn_STACK_START_ADR (cpu)) = ((int) entryPt - 0x20);

    /* This is used if core waiting after core1 did asynchronous reboot */

    *((volatile int *) CPUn_INIT_START_ADR (cpu)) = (int) entryPt;

    /* relase CPU from waiting loop */

    *((volatile int *) CPUn_INIT_STATUS_ADR (cpu)) = mpCpuStateReady;
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

    /* relase CPU from bootrom */

    *((volatile int *) CPUn_INIT_STATUS_ADR (physCpuIndex)) =
        mpCpuStateReboot;

    sysDcbf (CPUn_SPACE (physCpuIndex));
    _WRS_ASM ("sync");

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
    
    cache476Clear ((CACHE_TYPE) 0);
        
    /* Report to CPU0 this CPU's cache flush is complete */

    *(UINT32*)CPUn_INIT_STATUS_ADR(physCpuIndex) = mpCpuStateSleep;

    FOREVER
        {
        /* 
         * For SMP M-N, if logical core 0 is not equial to the physical core 0,
         * others slave cores need to check state to jump to bootrom themselves.
         */
        
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
    /* write logical CPU */

    vxPirSet(CPU_PHYS_TO_LOGICAL(physCpuIndex));

    vxIvprSet((UINT32)VEC_BASE_ADRS);
    
    excIvorInit();

    /*
     * flush entire cache and then initialize all static TLB entries with
     * cache-disabled.
     */
     
    cache476Clear ((CACHE_TYPE) 0);
    
    /* static TLB init witha cache disabled */
    
    mmu476TlbStaticInit();
    
    /*
     * all cache must be reseted here, or after cache is enabled, the
     * content of cache will be different with the memory.
     */    

    cache476Invalidate ((CACHE_TYPE) 0);
    
    /* enable cache */
    
    cacheAim476TlbSetup(TRUE);

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

    vxPirSave();

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
    int delay;
    unsigned int cpu; /* physical cpu index */
    volatile char * temp;

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

    /* Flush Data cache so as to make sure other cores have updated cache */

    cache476Clear ((CACHE_TYPE) 0);

    /* TBEN should be set by now */

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


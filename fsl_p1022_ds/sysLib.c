/* sysLib.c - Freescale P1022DS board system-dependent library */

/*
 * Copyright (c) 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,13nov12,d_l  Fix AMP address overlap error. (WIND00387682)
01d,12nov12,x_s  Fix the AMP device filter. (WIND00387606)
01c,23may12,y_f  add PIXIS indirect accessing mode
01b,23mar12,d_l  Move L1 parity handling to arch lib. (WIND00309426)
                 Replace SPI driver with fsl_p5020_ds's.
                 Replace I2C and Flash driver with their VxBus version.
                 Modify SMP and AMP implementation.
01a,17aug10,e_d  derived from fsl_p2020_ds/sysLib.c
*/

/*
DESCRIPTION
This library provides board-specific routines for P1022DS

INCLUDE FILES:
    spS25flxxSpiFlash.c - S25FL128P SPI flash driver.
    sysFslSpi.c         - Freescale SPI controller support.
    usbPciStub.c        - USB Host/Device controller support.

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
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
#   include <private/kernelLibP.h>	/* KERNEL_ENTERED_ME() */
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#   include "mot85xxPci.h"
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SPE
#   include <speLib.h>
#endif  /* INCLUDE_SPE */

#include "p1022.h"

/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash */

    /* needed be first entry here */

    {
    0xf0000000, 0x0, 0xf0000000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M  | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },

    /*
     * LOCAL MEMORY needed be second entry here  -
     * one TLB would be 1GB so use to get required 2GB
     */

#ifdef INCLUDE_AMP_CPU_01
    {
    AMP_MEM_START, 0x0, AMP_MEM_START,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G    | _MMU_TLB_PERM_W  |
    _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT |
    _MMU_TLB_ATTR_M
    },
#else
    {
    LOCAL_MEM_LOCAL_ADRS, 0x0, LOCAL_MEM_LOCAL_ADRS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G    | _MMU_TLB_PERM_W  |
    _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT |
    _MMU_TLB_ATTR_M
    },
#endif /* INCLUDE_AMP_CPU_01 */

    {
    0x40000000, 0x0, 0x40000000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G    | _MMU_TLB_PERM_W  |
    _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT |
    _MMU_TLB_ATTR_M
    },

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W   | _MMU_TLB_IPROT
    },

    /* All these are not protected */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_1   | _MMU_TLB_SZ_64M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
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
    }
#endif  /* INCLUDE_PCI_BUS */

};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#ifdef MMU_ASID_MAX     /* Base 6 MMU library in effect */

/* macro to compose 64-bit PHYS_ADDRs */

# define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))
#endif  /* MMU_ASID_MAX */

/*
* sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
* used by the MMU to translate addresses with single page (4k) granularity.
* PTE memory space should not, in general, overlap BAT memory space but
* may be allowed if only Data or Instruction access is mapped via BAT.
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

PHYS_MEM_DESC sysPhysMemDesc [] =
{
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,

/*
 * Because the INCLUDE_SPI_BOOT is reserved hig 2M memory for spi boot,
 * so we must add the reserved memory in TLB for ctrl + x command can
 * be visited these memory.
 */

#ifdef INCLUDE_AMP_CPU_00
    LOCAL_MEM_SIZE,
#else
    LOCAL_MEM_SIZE + SPI_BOOT_RESMEM,
#endif /* INCLUDE_AMP_CPU_00 */
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

#ifdef INCLUDE_AMP
#ifdef INCLUDE_AMP_CPU_00

    /* CORE 0 maps all memory now */
    {
    (VIRT_ADDR) (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE),
    (PHYS_ADDR) (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE),
    (AMP_MEM_SIZE - 0x40000000),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

#else /* AMP CORE 1 */

    {
    (VIRT_ADDR) (CPU1_FUNC_START_ADR),
    (PHYS_ADDR) (CPU1_FUNC_START_ADR),
    MMU_PAGE_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

    /* Both ETSEC END driver and VxBus Nor Flash driver use this in AMP mode */

    {
    (VIRT_ADDR) TM_ANCHOR_ADRS & ~(MMU_PAGE_SIZE -1),
    (PHYS_ADDR) TM_ANCHOR_ADRS & ~(MMU_PAGE_SIZE -1),
    MMU_PAGE_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  | \
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE           | \
    VM_STATE_MEM_COHERENCY
    },

    /* MIPC SM memory doesn't need to be mapped explicitly for core 0 and 1. */

#endif /* INCLUDE_AMP_CPU_00 */

#endif /* INCLUDE_AMP */

    {
    (VIRT_ADDR) PIXIS_BASE,
    (PHYS_ADDR) PIXIS_BASE,
    PIXIS_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    },

    {
    (VIRT_ADDR) CCSR_BASE,
    (PHYS_ADDR) CCSR_BASE,
    CCSR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) LCS1_BASE,
    (PHYS_ADDR) LCS1_BASE,
    LCS1_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

#ifdef DRV_PCIBUS_M85XX
    {
    (VIRT_ADDR) PCIEX1_MEM_ADRS,
    (PHYS_ADDR) PCIEX1_MEM_ADRS,
    PCIEX1_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX1_MEMIO_ADRS,
    PCIEX1_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_IO_ADRS,
    (PHYS_ADDR) PCIEX1_IO_ADRS,
    PCIEX1_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEM_ADRS,
    (PHYS_ADDR) PCIEX2_MEM_ADRS,
    PCIEX2_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
    | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    | VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
    PCIEX2_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
    | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    | VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_IO_ADRS,
    (PHYS_ADDR) PCIEX2_IO_ADRS,
    PCIEX2_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
    | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    | VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX3_MEM_ADRS,
    (PHYS_ADDR) PCIEX3_MEM_ADRS,
    PCIEX3_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX3_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX3_MEMIO_ADRS,
    PCIEX3_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX3_IO_ADRS,
    (PHYS_ADDR) PCIEX3_IO_ADRS,
    PCIEX3_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
#endif /* DRV_PCIBUS_M85XX */

    {
    (VIRT_ADDR) FLASH_BASE_ADRS,
    (PHYS_ADDR) FLASH_BASE_ADRS,
    FLASH_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

};

int sysPhysMemDescNumEnt = NELEMENTS(sysPhysMemDesc);

int     sysBus      = BUS_TYPE_NONE;    /* system bus type */
int     sysCpu      = CPU;              /* system CPU type */
char *  sysBootLine = BOOT_LINE_ADRS;   /* address of boot line */
char *  sysExcMsg   = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum  = 0;                /* processor number of this CPU */

UINT32  inFullVxWorksImage = FALSE;

#ifdef INCLUDE_SHOW_ROUTINES
UINT32  systemFreq;
UINT32  coreFreq;
UINT32  core1Freq;
#endif /* INCLUDE_SHOW_ROUTINES */

UINT32  ddrFreq;
LOCAL   UINT32 sysClkFreq;
UINT32  sysClkFreqGet(void);
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
    0,  0,  0,  3,  4,  0xffffffff, 6,
    0,  8,  0,  10, 0,  12, 0,  14, 0
};

#define MAX_VALUE_PLAT_RATIO    32
UINT32 platRatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 3, 0 },
    { 4,  0 }, { 5, 0 }, { 6,  0 }, { 0, 0 },
    { 8,  0 }, { 9, 0 }, { 10, 0 }, { 0, 0 },
    { 12, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 20, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 }
};

#define MAX_VALUE_E500_RATIO    10
UINT32 e500RatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 3, 1 }, { 2, 0 },
    { 5, 1 }, { 3, 0 }, { 7, 1 }, { 4, 0 }, { 9, 1 }
};

/* externs */

IMPORT char     _wrs_kernel_data_start[];
IMPORT char *   cachePpcReadOrigin;
IMPORT void mmuE500TlbDynamicInvalidate();
IMPORT void mmuE500TlbStaticInvalidate();
IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, BOOL cacheAllow);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;
IMPORT void sysIvprSet(UINT32);
IMPORT device_method_t * pSysPlbMethods;

IMPORT BOOL e500_spe_exc_enable;
IMPORT BOOL e500_spe_exc_mask;

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

IMPORT void vxSdaInit (void);

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP();
IMPORT void vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheArchDisableFromMmu (CACHE_TYPE cache);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
IMPORT STATUS vxbIntToCpuRoute (unsigned int cpu);
#endif  /* _WRS_CONFIG_SMP */

IMPORT void sysBoardReset (void);

#ifdef  INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#ifdef  INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif   /* INCLUDE_SIO_UTILS */

#if (defined(_WRS_CONFIG_SMP) || defined(INCLUDE_WRLOAD))
IMPORT void sysCpu1Start (void);

#endif  /* _WRS_CONFIG_SMP || INCLUDE_WRLOAD */

#if (defined(_WRS_CONFIG_SMP) || defined(INCLUDE_WRLOAD))
IMPORT void sysCpu1Stop (void);
#endif /* _WRS_CONFIG_SMP || INCLUDE_WRLOAD */

#ifdef INCLUDE_SPE
IMPORT int  (* _func_speProbeRtn) ();
#endif /* INCLUDE_SPE */

IMPORT void sysL1Csr1Set(UINT32);
IMPORT UINT sysTimeBaseLGet(void);

/* forward declarations */

VOID    sysUsDelay                   (int);
VOID    sysMsDelay                   (UINT);
STATUS  sysIntEnablePIC           (int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable (int intNum);
STATUS  sysCascadeIntDisable(int intNum);
void    sysPixisWrite (VUINT8 * pAddr, UINT8 value);
UINT8   sysPixisRead (VUINT8 * pAddr);

#ifdef  _WRS_CONFIG_SMP
LOCAL   void sysCpu1ShutdownLoop (void);
LOCAL   UINT32 physCpuAvailableGet (void);
#   if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))
STATUS  sysSmpTimeBaseSync (void);
#   endif   /* DRV_TIMER_DEC_PPC && INCLUDE_SYS_TIMESTAMP */
#endif  /* _WRS_CONFIG_SMP */

LOCAL BOOL sysCpu1Check (void);

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL   void sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_AMP
METHOD_DECL(sysBspDevFilter);
STATUS  sysDeviceFilter(VXB_DEVICE_ID pDev);
void    sysAmpCpuStatusSet(int status,int cpu);
int     sysAmpCpuStatusGet(int cpu);
#endif  /* INCLUDE_AMP */

LOCAL struct vxbDeviceMethod p1022PlbMethods[] =
    {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysCpu1Check),
#ifdef INCLUDE_AMP

    /*
     * Only used for AMP filtering per core for now so not built for
     * anything else
     */

    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
#endif  /* INCLUDE_AMP */
    { 0, 0 }
    };

LOCAL char * physTop = NULL;
LOCAL char * memTop  = NULL;

#ifdef IEEE754_FIX
LOCAL SPE_CONTEXT *speCtx;
#endif  /* IEEE754_FIX */

/* includes */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
#   include "sysL2Cache.c"
#endif  /* INCLUDE_L2_CACHE && INCLUDE_CACHE_SUPPORT*/

#ifdef DRV_SPI_S25FLXX
#   include "sysFslSpi.c"
#   include "spS25flxxSpiFlash.c"
#endif  /* DRV_SPI_S25FLXX */

#ifdef INCLUDE_NETWORK
#   include "sysNet.c"
#endif  /* INCLUDE_NETWORK */

#include "nvRamToFlash.c"
#include "hwconf.c"
#include "sysGpio.c"

/* defines */

#define SYS_MODEL_P1022E        "Freescale P1022E - Security Engine"
#define SYS_MODEL_P1022         "Freescale P1022"
#define SYS_MODEL_P1013E        "Freescale P1013E - Security Engine"
#define SYS_MODEL_P1013         "Freescale P1013"
#define SYS_MODEL               "Freescale P1022"

#define SYS_MODEL_E500          "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN       "Freescale Unknown processor"

#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000

/* definitions for core cluster ID setting in PIR */

#define PIR_CORE_CLUSTER_ID_SHIFT   5
#define PIR_CORE_CLUSTER_ID_MASK    0x1f
#define PIR_CORE_CLUSTER_ID_SET(id) \
    ((id & PIR_CORE_CLUSTER_ID_MASK) << PIR_CORE_CLUSTER_ID_SHIFT)

#ifdef INCLUDE_AMP
typedef struct
{
    char* deviceName;
    int   unit;
    int   cpu;
    }AMP_CPU_TABLE;

/*
 * Table includes only devices we want to filter
 * if no action required for a device per CPU then it's not in the table
 */

AMP_CPU_TABLE ampCpuTable[] = {
    { "motetsec",       0,  DEVICE_CORE0 },
    { "motetsec",       1,  DEVICE_CORE1 },
    { "ns16550",        0,  DEVICE_CORE0 },
    { "ns16550",        1,  DEVICE_CORE0 },
    { "openPicTimer",   0,  DEVICE_CORE0 },
    { "openPicTimer",   1,  DEVICE_CORE0 },
    { "m85xxCCSR",      0,  DEVICE_CORE0 },
    { "m85xxPci",       0,  DEVICE_CORE0 },
    { "m85xxPci",       1,  DEVICE_CORE0 },
    { "m85xxPci",       2,  DEVICE_CORE0 },
    { "vxbPlbUsbEhci",  0,  DEVICE_CORE0 },
    { "fslDma",         0,  DEVICE_CORE0 },
    { "sdhci",          0,  DEVICE_CORE0 },
    { "fslSata",        0,  DEVICE_CORE0 },
    { "fslSata",        1,  DEVICE_CORE0 },
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

    if (pDev->pName == NULL)
        return OK;

    deviceMatch=FALSE;

    for (deviceIndex = 0; deviceIndex < NELEMENTS(ampCpuTable); deviceIndex ++)
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

    /* if device match then we need to decide whether to filter */

    if(ampCpuTable[deviceIndex].cpu!=(sysCpu1Check()))
        return(ERROR);

    return(OK);
    }

#endif /* INCLUDE_AMP */

#if defined (INCLUDE_SPE)

/*******************************************************************************
*
* sysSpeProbe - Check if the CPU has SPE unit
*
* This routine returns OK it the CPU has an SPE unit in it.
* Presently it assumes available.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

int  sysSpeProbe (void)
    {
    UINT32 regVal;
    int speUnitPresent = OK;

    /* The CPU type is indicated in the SVR register */

    regVal = *M85XX_SVR(CCSBAR);

    if ((regVal == P1022E_SVR) || (regVal == P1013E_SVR))
        speUnitPresent = OK;
    else
        speUnitPresent = FALSE;

    return(speUnitPresent);
    }
#endif  /* INCLUDE_SPE */

/*******************************************************************************
*
* sysPixisWrite - write data to pixis register
*
* This routine writes data to pixis register.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysPixisWrite
    (
    VUINT8 *    pAddr,
    UINT8       value
    )
    {
    VUINT32     reg32Data   = 0;
    VUINT8 *    pLcs0       = NULL;
    VUINT8 *    pLcs1       = NULL;

    reg32Data = M85XX_PMUXCR;
    if (reg32Data & (0x1 << 30))
        {
        pLcs0 = (UINT8 *)((*M85XX_BR0(CCSBAR)) & (*M85XX_OR0(CCSBAR)) &
                           0xFFFF8000);
        pLcs1 = (UINT8 *)((*M85XX_BR1(CCSBAR)) & (*M85XX_OR1(CCSBAR)) &
                           0xFFFF8000);

        *pLcs0 = (UINT8)((UINT32)pAddr & 0xFF);
        *pLcs1 = value;
        }
    else
        {
        *pAddr = value;
        }

    _WRS_ASM("eieio");
    }

/*******************************************************************************
*
* sysPixisRead - read data from pixis register
*
* This routine reads data from pixis register.
*
* RETURNS: the value of pixis register
*
* ERRNO: N/A
*/

UINT8 sysPixisRead
    (
    VUINT8 *    pAddr
    )
    {
    VUINT32     reg32Data   = 0;
    VUINT8 *    pLcs0       = NULL;
    VUINT8 *    pLcs1       = NULL;

    reg32Data = M85XX_PMUXCR;
    if (reg32Data & (0x1 << 30))
        {
        pLcs0 = (UINT8 *)((*M85XX_BR0(CCSBAR)) & (*M85XX_OR0(CCSBAR)) &
                           0xFFFF8000);
        pLcs1 = (UINT8 *)((*M85XX_BR1(CCSBAR)) & (*M85XX_OR1(CCSBAR)) &
                           0xFFFF8000);

        *pLcs0 = (UINT8)((UINT32)pAddr & 0xFF);
        _WRS_ASM("eieio");
        return *pLcs1;
        }
    else
        {
        _WRS_ASM("eieio");
        return *pAddr;
        }
    }

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

    switch(device & 0xffff0000)
        {
        case P1022E_SVR:
            retChar = SYS_MODEL_P1022E;
            break;
        case P1022_SVR:
            retChar = SYS_MODEL_P1022;
            break;
        case P1013E_SVR:
            retChar = SYS_MODEL_P1013E;
            break;
        case P1013_SVR:
            retChar = SYS_MODEL_P1013;
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
* sysClkFreqGet - returns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet (void)
    {
    UINT32  oscFreq;
    UINT32  platRatio;
    UINT32  ddrRatio;
    UINT8   osc;
    UINT8   ddr;

#ifdef INCLUDE_SHOW_ROUTINES
    UINT32  e500Ratio;
    UINT32  e5001Ratio;
#endif /* INCLUDE_SHOW_ROUTINES */

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);
    ddrRatio = M85XX_PORPLLSR_DDR_RATIO(CCSBAR);

#ifdef FORCE_DEFAULT_FREQ
    return(DEFAULT_SYSCLKFREQ);
#endif /* FORCE_DEFAULT_FREQ */

    if ((platRatio >= MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio][0] == 0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

#ifdef INCLUDE_AMP_CPU_01
    osc = *((VINT32*)AMP_CPU1_CCBSPD);
#else
    osc = sysPixisRead (PIXIS_SPD(PIXIS_BASE)) & 0x7;
#endif /* INCLUDE_AMP_CPU_01 */

#ifdef INCLUDE_AMP_CPU_00
    *((VINT32*)AMP_CPU1_CCBSPD) = osc;
#endif /* INCLUDE_AMP_CPU_00 */

    switch (osc)
        {
        case PIXIS_OSC_66_MHZ:
            oscFreq = FREQ_66_MHZ;
            break;
        case PIXIS_OSC_75_MHZ:
            oscFreq = FREQ_75_MHZ;
            break;
        case PIXIS_OSC_83_MHZ:
            oscFreq = FREQ_83_MHZ;
            break;
        case PIXIS_OSC_90_MHZ:
            oscFreq = FREQ_90_MHZ;
            break;
        case PIXIS_OSC_100_MHZ:
            oscFreq = FREQ_100_MHZ;
            break;
        case PIXIS_OSC_111_MHZ:
            oscFreq = FREQ_111_MHZ;
            break;
        case PIXIS_OSC_125_MHZ:
            oscFreq = FREQ_125_MHZ;
            break;
        case PIXIS_OSC_133_MHZ:
            oscFreq = FREQ_133_MHZ;
            break;

        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

#ifdef INCLUDE_AMP_CPU_01
    ddr = *((VINT32*)AMP_CPU1_DDRSPD);
#else
    ddr = (sysPixisRead (PIXIS_SPD(PIXIS_BASE)) >> 3) & 0x7;
#endif /* INCLUDE_AMP_CPU_01 */

#ifdef INCLUDE_AMP_CPU_00
    *((VINT32*)AMP_CPU1_DDRSPD) = ddr;
#endif /* INCLUDE_AMP_CPU_00 */

    switch (ddr)
        {
        case PIXIS_DDR_66_MHZ:
            ddrFreq = FREQ_66_MHZ;
            break;
        case PIXIS_DDR_75_MHZ:
            ddrFreq = FREQ_75_MHZ;
            break;
        case PIXIS_DDR_83_MHZ:
            ddrFreq = FREQ_83_MHZ;
            break;
        case PIXIS_DDR_90_MHZ:
            ddrFreq = FREQ_90_MHZ;
            break;
        case PIXIS_DDR_100_MHZ:
            ddrFreq = FREQ_100_MHZ;
            break;
        case PIXIS_DDR_111_MHZ:
            ddrFreq = FREQ_111_MHZ;
            break;
        case PIXIS_DDR_125_MHZ:
            ddrFreq = FREQ_125_MHZ;
            break;
        case PIXIS_DDR_133_MHZ:
            ddrFreq = FREQ_133_MHZ;
            break;
        default:
            ddrFreq = OSCILLATOR_FREQ;
            break;
        }

    sysClkFreq = ((UINT32)(oscFreq * platRatioTable[platRatio][0])) >> \
                            ((UINT32)platRatioTable[platRatio][1]);

#ifdef INCLUDE_SHOW_ROUTINES
    systemFreq = osc;
    ddrFreq = ddrFreq * ddrRatioTable[ddrRatio];
    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    e5001Ratio = M85XX_PORPLLSR_E500_1_RATIO(CCSBAR);

    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0])) >> \
                            ((UINT32)e500RatioTable[e500Ratio][1]);
    core1Freq = ((UINT32)(sysClkFreq * e500RatioTable[e5001Ratio][0]))>> \
                            ((UINT32)e500RatioTable[e5001Ratio][1]);
#endif /* INCLUDE_SHOW_ROUTINES */

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
* sysHwMemInit - initialize and configure system memory
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

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the MPC8260 ADS board. It sets up
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

    UINT32 regTemp = 0;
    volatile UINT32 reg32Temp = 0;

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

    /* enable time base for delay use before DEC interrupt is setup */

    vxDecSet(0xffffffff);
    vxDecarSet(0xffffffff);
    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

    /* Initialize L2CTL register */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxL2CTLSet(0x28000000, M85XX_L2CTL(CCSBAR));
#endif  /* INCLUDE_L2_CACHE && INCLUDE_CACHE_SUPPORT*/

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
#endif  /* INCLUDE_L2_CACHE && INCLUDE_CACHE_SUPPORT*/

    sysClkFreq = 0;
    sysClkFreqGet();

    /* Machine check via RXFE for RIO */

    vxHid1Set (vxHid1Get()| HID1_ASTME | HID1_RXFE);

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

    _WRS_ASM("isync");

    e500_spe_exc_enable = FALSE;
    e500_spe_exc_mask = 0;

    pSysPlbMethods = p1022PlbMethods;

#ifdef DRV_STORAGE_FSLSATA
    M85XX_SATASTS = 0x20000000;
    M85XX_SATACTL = 0x00000100;
    _WRS_ASM("isync");
    M85XX_SATA2STS = 0x20000000;
    M85XX_SATA2CTL = 0x00000100;
#endif /* DRV_STORAGE_FSLSATA */

#ifdef DRV_I2CBUS_FSL
    reg32Temp = M85XX_PMUXCR;
    M85XX_PMUXCR = (reg32Temp | 0x00001000);
    reg32Temp = M85XX_PMUXCR;
    _WRS_ASM("eieio");
#endif /* DRV_I2CBUS_FSL */

#ifdef INCLUDE_END
    regTemp = M85XX_PMUXCR2;
    M85XX_PMUXCR2 = regTemp & 0xFFE00000;
    _WRS_ASM("eieio");
#endif /* INCLUDE_END */

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

    CACHE_PIPE_FLUSH();
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
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
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
* |                            flush d-cache                              |
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
* |  wait for CPUn flush or timeout |   |    flush & disable d-cache      |
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
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);

#ifdef _WRS_CONFIG_SMP
    cpuset_t cpuset;
    int i;
#else /* _WRS_CONFIG_SMP */
    int token = intCpuLock();
#endif  /* _WRS_CONFIG_SMP */

    int basePhysCpu = CPU_LOGICAL_TO_PHYS(0);

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

    cacheAimE500FlushAllD();

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

    /* 
     * set MSR to its power-up default value
     * disables ALL interrupts and exceptions, including debugging breakpoints
     */

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

    sysAmpCpuStatusSet(ampCpuStateReboot,0);

    sysBoardReset();

#   else  /* INCLUDE_AMP_CPU_00 */
    
    /* reset PIR prior to entering into bootrom */

    vxPirSet (PIR_CORE_CLUSTER_ID_SET(basePhysCpu));

    sysAmpCpuStatusSet(ampCpuStateReboot,1);

#   endif  /* INCLUDE_AMP_CPU_00 */

#endif /* INCLUDE_AMP */

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
#endif
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

    *(M85XX_DEVDISR(CCSBAR)) |= (M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1);

     /*
      * Save the current timebase on CPU0 into static valuable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * synchronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif  /* DRV_TIMER_M85XX && INCLUDE_TIMESTAMP */

#ifdef INCLUDE_SPI_BOOT
    memcpy((void *)(SPI_BOOT_SMP_COPYDES), (void *)(SPI_BOOT_SMP_COPYSRC), SPI_BOOT_SMP_COPYSIZE);
    *(volatile UINT32 *)(SPI_BOOT_SMP_BPTR) = SPI_BOOT_SMP_CODE;
    *M85XX_BPTR(CCSBAR) = ((UINT32)(SPI_BOOT_SMP_COPYDES) >> BPTR_BOOT_PAGE_SHIFT ) | BPTR_EN;
#endif /* INCLUDE_SPI_BOOT */

    /* Flush Data cache so as to make sure other cores have updated cache */

    {
    volatile char* temp;

    temp = malloc(0x100000);

    memset((char*)temp, 0x0, 0x100000);


    sysCpu1Start ();

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
    int i = 0;
    volatile UINT8 regTemp = 0;
    volatile UINT32 reg32Temp = 0;

#ifdef DRV_STORAGE_SDHC
    regTemp = *PX_BRDCFG0(PIXIS_BASE);
    *PX_BRDCFG0(PIXIS_BASE) = regTemp | 0x04;

    _WRS_ASM("isync");
    for(i = 0; i<1000000; i++);

    reg32Temp = M85XX_PMUXCR;
    M85XX_PMUXCR = (reg32Temp & 0xFFE0FFFF);
    M85XX_PMUXCR = 0x80001000;

#endif  /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_FSL_SPI
    regTemp = *PX_BRDCFG0(PIXIS_BASE);
    *PX_BRDCFG0(PIXIS_BASE) = regTemp | 0x80;
    reg32Temp = M85XX_PMUXCR;
    M85XX_PMUXCR = (reg32Temp & 0xFFDFFFFF);
    sysSpiDrvInit(0, FREQ_33_MHZ);
#endif /* INCLUDE_FSL_SPI */

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

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif  /* INCLUDE_AUX_CLK */

    /* configure LCS1 for indirect accessing PIXIS registers */

    *M85XX_BR1(CCSBAR) &= (UINT32)(0x1FFFF << 15);
    *M85XX_BR1(CCSBAR) |= (*M85XX_BR0(CCSBAR) & 0x7FFF);

    *M85XX_OR1(CCSBAR) &= (UINT32)(0x1FFFF << 15);
    *M85XX_OR1(CCSBAR) |= (*M85XX_OR0(CCSBAR) & 0x7FFF);
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
    return(CPU1CHECK);
    }

/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* Not applicable for the bus-less 8260Ads.
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

        /* Get current Time Base Lower register count. */

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


    #define xbit0(x, n)    ((x & (1 << (31 - n))) >> (31 - n))  /* 0..31 */
    #define xbit32(x, n)   ((x & (1 << (63 - n))) >> (63 - n))  /* 32..63 */
    #define onoff0(x, n)   xbit0(x, n) ? "ON", "OFF"
    #define onoff32(x, n)  xbit32(x, n) ? "ON", "OFF"

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

    }

/*******************************************************************************
*
* pixisShow - Show routine for PIXIS registers
*
* This routine shows the ngPIXIS registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void pixisShow (void)
    {

    printf("    PIXIS_ID        = 0x%02x \r\n", *PIXIS_ID(PIXIS_BASE));
    printf("    PIXIS_VER       = 0x%02x \r\n", *PIXIS_VER(PIXIS_BASE));
    printf("    PIXIS_PVER      = 0x%02x \r\n", *PIXIS_PVER(PIXIS_BASE));
    printf("    PIXIS_CSR       = 0x%02x \r\n", *PIXIS_CSR(PIXIS_BASE));
    printf("    PIXIS_RST       = 0x%02x \r\n", *PIXIS_RST(PIXIS_BASE) );
    printf("    PIXIS_AUX       = 0x%02x \r\n", *PIXIS_AUX(PIXIS_BASE) );
    printf("    PIXIS_SPD       = 0x%02x \r\n", *PIXIS_SPD(PIXIS_BASE));
    printf("    PIXIS_BRDCFG0   = 0x%02x \r\n", *PX_BRDCFG0(PIXIS_BASE));
    printf("    PIXIS_DMA       = 0x%02x \r\n", *PX_DMA(PIXIS_BASE));
    printf("    PIXIS_ADDR      = 0x%02x \r\n", *PX_ADDR(PIXIS_BASE));
    printf("    PIXIS_DATA      = 0x%02x \r\n", *PX_DATA(PIXIS_BASE));
    printf("    PIXIS_LED       = 0x%02x \r\n", *PX_LED(PIXIS_BASE));
    printf("    PIXIS_VCTL      = 0x%02x \r\n", *PX_VCTL(PIXIS_BASE));
    printf("    PIXIS_VSTAT     = 0x%02x \r\n", *PX_VSTAT(PIXIS_BASE));
    printf("    PIXIS_VCFGEN0   = 0x%02x \r\n", *PX_VCFGEN0(PIXIS_BASE));
    printf("    PIXIS_SCLK0     = 0x%02x \r\n", *PX_SCLK0(PIXIS_BASE));
    printf("    PIXIS_SCLK1     = 0x%02x \r\n", *PX_SCLK1(PIXIS_BASE));
    printf("    PIXIS_SCLK2     = 0x%02x \r\n", *PX_SCLK2(PIXIS_BASE));
    printf("    PIXIS_DCLK0     = 0x%02x \r\n", *PX_DCLK0(PIXIS_BASE));
    printf("    PIXIS_DCLK1     = 0x%02x \r\n", *PX_DCLK1(PIXIS_BASE));
    printf("    PIXIS_DCLK2     = 0x%02x \r\n", *PX_DCLK2(PIXIS_BASE));
    printf("    PIXIS_WATCH     = 0x%02x \r\n", *PX_WATCH(PIXIS_BASE));
    printf("    PIXIS_SW1       = 0x%02x \r\n", *PX_SW1(PIXIS_BASE));
    printf("    PIXIS_SW2       = 0x%02x \r\n", *PX_SW2(PIXIS_BASE));
    printf("    PIXIS_SW3       = 0x%02x \r\n", *PX_SW3(PIXIS_BASE));
    printf("    PIXIS_SW4       = 0x%02x \r\n", *PX_SW4(PIXIS_BASE));
    printf("    PIXIS_SW5       = 0x%02x \r\n", *PX_SW5(PIXIS_BASE));
    printf("    PIXIS_SW6       = 0x%02x \r\n", *PX_SW6(PIXIS_BASE));
    printf("    PIXIS_SW7       = 0x%02x \r\n", *PX_SW7(PIXIS_BASE));
    printf("    PIXIS_SW8       = 0x%02x \r\n", *PX_SW8(PIXIS_BASE));
    printf("    PIXIS_EN1       = 0x%02x \r\n", *PX_EN1(PIXIS_BASE));
    printf("    PIXIS_EN2       = 0x%02x \r\n", *PX_EN2(PIXIS_BASE));
    printf("    PIXIS_EN3       = 0x%02x \r\n", *PX_EN3(PIXIS_BASE));
    printf("    PIXIS_EN4       = 0x%02x \r\n", *PX_EN4(PIXIS_BASE));
    printf("    PIXIS_EN5       = 0x%02x \r\n", *PX_EN5(PIXIS_BASE));
    printf("    PIXIS_EN6       = 0x%02x \r\n", *PX_EN6(PIXIS_BASE));
    printf("    PIXIS_EN7       = 0x%02x \r\n", *PX_EN7(PIXIS_BASE));
    printf("    PIXIS_EN8       = 0x%02x \r\n", *PX_EN8(PIXIS_BASE));

    }

#endif  /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciEx1IntRoute [NUM_PCIEX1_SLOTS][4] =
{
    { PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL }
};

LOCAL UCHAR sysPciEx2IntRoute [NUM_PCIEX2_SLOTS][4] =
{
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL }
};

LOCAL UCHAR sysPciEx3IntRoute [NUM_PCIEX3_SLOTS][4] =
{
    { PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL }
};

/*******************************************************************************
*
* sysPciEx1AutoConfigInclude - PCI Express 1 autoconfig support routine
*
* This routine performs the PCI Express 1 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx1AutoConfigInclude
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
    return sysPciEx1IntRoute[0][pin - 1];
    }

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

STATUS sysPciEx2AutoConfigInclude
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
    return sysPciEx2IntRoute[0][pin - 1];
    }

/*******************************************************************************
*
* sysPciEx3AutoConfigInclude - PCI Express 3 autoconfig support routine
*
* This routine performs the PCI Express 3 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx3AutoConfigInclude
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
* sysPciEx3AutoConfigIntAssign - PCI Express 3 autoconfig support routine
*
* This routine peforms the PCI Express 3 auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO: N/A
*/

UCHAR sysPciEx3AutoConfigIntAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciEx3IntRoute[0][pin - 1];
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
* RETURNS: TRUE if CPU is core1, FALSE if CPU is core0.
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

#ifdef INCLUDE_CPU_PWR_MGMT
    vxHid0Set(vxHid0Get() | _PPC_HID0_DOZE);
    _WRS_ASM("sync");
    vxMsrSet(vxMsrGet()| _PPC_MSR_WE);
    _WRS_ASM("isync");
#endif  /* INCLUDE_CPU_PWR_MGMT */

    /* expect to lock in DOZE mode until HRESET */

    FOREVER;

    }
#endif /* _WRS_CONFIG_SMP */

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
         * Check whether cpu1 has run, if cpu1 is running,
         * clean up and shut it down.
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

            _WRS_ASM("sync");

            /* hold core in reset while clearing PCR! */

            *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
            _WRS_ASM("sync");
            for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++);

            sysCpu1Stop();

            *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
            _WRS_ASM("sync");

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

#endif /* INCLUDE_AMP */

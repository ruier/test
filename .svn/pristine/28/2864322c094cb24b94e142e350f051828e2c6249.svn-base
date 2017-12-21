/* sysLib.c - Freescale P2020RDB board system-dependent library */

/*
 * Copyright (c) 2009-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,04jan12,l_z  Fix the AMP device filter. (WIND00387440)
01i,04jan12,agf  Correct value used to init L2CTL register (WIND00325436)
01h,16nov11,ers  Moved L1 parity handling to arch lib. (WIND00309426)
01g,01aug11,ers  CQ253685:Fix memory mapping bugs and isolate the cores'
                 memory from each other.  Support wrload memory mapping and 
                 loading.
01g,10mar11,my_  rename DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC.
01f,24jan11,agf  ensure all D-caches are flushed during SMP sysToMonitor()
                 (WIND00242091)
                 replace manual cache flushing in sysCpuEnable with a call 
                 to the arch routine for cache flushing -- the malloc and
                 free were being done improperly anyway, inside a region of
                 code with interrupts locked (WIND00239454)
01e,22dec10,syt  corrected a error in sysStaticTlbDesc entry.(WIND00247805)
01d,03dec10,swu  Modify CCSBAR mmu table size.(WIND00230875)
01c,23apr10,rec  remove INCLUDE_CPU_PWR_MGMT
01b,29jan10,syt  added AMP support.
01a,10oct09,syt  derived from version 01a of fsl_p2020_ds/sysLib.c
*/

/*
DESCRIPTION
This library provides board-specific routines for P2020RDB

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    flashMem.c          - S29GL128P flash driver for NVRam.
    sysMtd.c            - S29GL128P flash driver for tureFFS.
    sysFslSpi.c         - FreeScale SPI controller support.
    sysSpiFlash.c       - S25FL128P SPI flash driver.
    sysNandFlash.c      - FreeScale Nand Flash controller support.
    sysL2Cache.c        - L2 cache support.
    sysGpio.c           - GPIO controller driver.
    sysMpc85xxI2c.c     - FreeScale I2c controller support.
    ds1339Rtc.c         - I2c type real time clock device driver.
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

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SPE
#   include <speLib.h>
#endif  /* INCLUDE_SPE */

/* globals */

#ifdef WRLOAD_IMAGE_BUILD_PHYS_BIAS
#define PHYS_BIAS_LO  (UINT32)         (WRLOAD_IMAGE_BUILD_PHYS_BIAS)
#define PHYS_BIAS_HI  (UINT32) ((UINT64)WRLOAD_IMAGE_BUILD_PHYS_BIAS >> 32)
#else
#define PHYS_BIAS_LO 0
#define PHYS_BIAS_HI 0
#define WRLOAD_IMAGE_BUILD_PHYS_BIAS 0
#endif

TLB_ENTRY_DESC sysStaticTlbDesc [] = {

    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash */

    /* needed be first entry here */

    {
    FLASH_BASE_ADRS, 0x0, FLASH_BASE_ADRS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
    {
    LOCAL_MEM_LOCAL_ADRS,  PHYS_BIAS_HI, 
    LOCAL_MEM_LOCAL_ADRS + PHYS_BIAS_LO,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M  | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | CAM_DRAM_CACHE_MODE |
    _MMU_TLB_ATTR_M
    },
#else
    {
    LOCAL_MEM_LOCAL_ADRS, 0x0, LOCAL_MEM_LOCAL_ADRS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G    | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | CAM_DRAM_CACHE_MODE |
    _MMU_TLB_ATTR_M
    },
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

    /* CCSR space */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M    | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G 
    },

    /* All these are not protected */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_1   | _MMU_TLB_SZ_1M                       | 
    _MMU_TLB_PERM_W |                      _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G 
    },

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
/*
 * The PPC TLB1 only supports specific block sizes in 4x increments. The
 * primary maps 1GB and secondaries map 256M in entry [1] above.  To give
 * a bit more flexability, we allow secondaries to map an additional 256MB.
 * Remember that sysStaticTlbDesc[] maps are only used by the kernel & ISRs.
 * For a 512M RAM mapping place the other 256M map after sysStaticTlbDesc[3]
 * to avoid conflicts with romInit. Currently, a 512MB mapping doesn't work
 * if a load bias is used.
 */
    #if ((LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE + WRLOAD_IMAGE_BUILD_PHYS_BIAS) \
         > PHYS_MEM_SIZE)
        #error Your image footprint exceeds available memory.
    #endif

    #if (LOCAL_MEM_SIZE > 0x10000000)
    {
    LOCAL_MEM_LOCAL_ADRS + 0x10000000,  PHYS_BIAS_HI, 
    LOCAL_MEM_LOCAL_ADRS + 0x10000000 + PHYS_BIAS_LO,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M  | _MMU_TLB_IPROT      |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | CAM_DRAM_CACHE_MODE |
    _MMU_TLB_ATTR_M
    },
    #endif

    #if (LOCAL_MEM_SIZE > 0x20000000)
        #error You need to add another TLB.
    #endif
#endif

#ifdef INCLUDE_L2_SRAM
    {
    L2SRAM_ADDR, 0x0, L2SRAM_ADDR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256K                    | 
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I | 
    _MMU_TLB_ATTR_G
    },
#endif /* INCLUDE_L2_SRAM */

    {
    VSC7385_BASE, 0x0, VSC7385_BASE,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_64K   | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G 
    },

    {
    (VSC7385_BASE + 0x10000), 0x0, (VSC7385_BASE + 0x10000),
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_64K   | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G 
    },

#ifdef INCLUDE_NAND_FLASH
    {
    NAND_BUFFER_BASE, 0x0, NAND_BUFFER_BASE,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256K  | _MMU_TLB_IPROT  |
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G 
    },

    {
    (NAND_BUFFER_BASE + 0x40000), 0x0, (NAND_BUFFER_BASE + 0x40000),
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256K  | _MMU_TLB_IPROT
    _MMU_TLB_PERM_W |                     _MMU_TLB_ATTR_I |
    _MMU_TLB_ATTR_G | 
    },
#endif  /* INCLUDE_NAND_FLASH */

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

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

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

PHYS_MEM_DESC sysPhysMemDesc [] = {
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + (PHYS_ADDR) WRLOAD_IMAGE_BUILD_PHYS_BIAS,
    LOCAL_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
    {
    (VIRT_ADDR) 0,  /* For when we dereference NULL pointers */
    (PHYS_ADDR) 0,
                MMU_PAGE_SIZE,   
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RW   | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

    {
    (VIRT_ADDR) CPU1_FUNC_START_ADR,
    (PHYS_ADDR) CPU1_FUNC_START_ADR,
    MMU_PAGE_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RW   | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

#ifdef INCLUDE_MIPC_SM
    /* Call to usrMipcSmInitReal() sets the area used for MIPC */

    {
    (VIRT_ADDR) MIPC_SM_SYSTEM_POOL_BASE,
    (PHYS_ADDR) MIPC_SM_SYSTEM_POOL_BASE,
                MIPC_SM_SYSTEM_POOL_SIZE,   
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RW   | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },
#endif /* INCLUDE_MIPC_SM */

#ifdef INCLUDE_TSEC_MDIO
    {
    (VIRT_ADDR) TM_ANCHOR_ADRS & ~(MMU_PAGE_SIZE -1),
    (PHYS_ADDR) TM_ANCHOR_ADRS & ~(MMU_PAGE_SIZE -1),
    MMU_PAGE_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RW   | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },
#endif /* INCLUDE_TSEC_MDIO */

#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

#ifdef INCLUDE_L2_SRAM
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    L2SRAM_WINDOW_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    },
#endif /* INCLUDE_L2_SRAM */

    {
    (VIRT_ADDR) VSC7385_BASE,
    (PHYS_ADDR) VSC7385_BASE,
    VSC7385_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

#ifdef INCLUDE_NAND_FLASH
    {
    (VIRT_ADDR) NAND_BUFFER_BASE,
    (VIRT_ADDR) NAND_BUFFER_BASE,
    NAND_BUFFER_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },
#endif  /* INCLUDE_NAND_FLASH */

    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    CCSR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

#ifdef INCLUDE_PCI_BUS
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
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RW   | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
    PCIEX2_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RW   | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_IO_ADRS,
    (PHYS_ADDR) PCIEX2_IO_ADRS,
    PCIEX2_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RW   | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

#endif /* INCLUDE_PCI_BUS */

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

UINT32 inFullVxWorksImage = FALSE;

UINT32  coreFreq;
UINT32  core1Freq;
UINT32  ddrFreq;
UINT32  systemFreq;

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

IMPORT char     _wrs_kernel_text_start[];
IMPORT void mmuE500TlbDynamicInvalidate();
IMPORT void mmuE500TlbStaticInvalidate();
IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, BOOL cacheAllow);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;
IMPORT void sysIvprSet(UINT32);
IMPORT device_method_t * pSysPlbMethods;

IMPORT BOOL e500_spe_exc_enable;
IMPORT BOOL e500_spe_exc_mask;

IMPORT UINT8 _binary_vsc2bin_start[];
IMPORT UINT8 _binary_vsc2bin_end[];

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

IMPORT void vxSdaInit (void);

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP();
IMPORT void vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheAimE500DisableDFromMmu (void);
IMPORT STATUS cacheAimE500DisableIFromMmu (void);
IMPORT STATUS cacheAimE500FlushAllD (void);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
IMPORT STATUS vxbIntToCpuRoute (unsigned int cpu);
#endif  /* _WRS_CONFIG_SMP */

#ifdef  INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

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

#ifdef _WRS_VX_AMP
METHOD_DECL(sysBspDevFilter);
STATUS  sysDeviceFilter(VXB_DEVICE_ID pDev);
#endif  /* _WRS_VX_AMP */

#ifdef INCLUDE_AMP
LOCAL UINT64 sysVirtToPhys (void *);
LOCAL void * sysPhysToVirt (UINT64);

DEVMETHOD_DEF(sysVirtToPhys, "BSP virtual to physical address translation");
DEVMETHOD_DEF(sysPhysToVirt, "BSP physical to virtual address translation");
#endif

LOCAL struct vxbDeviceMethod p2020PlbMethods[] = {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysCpu1Check),
#ifdef _WRS_VX_AMP
    /*
     * Only used for AMP filtering per core for now so not built for
     * anything else
     */

    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
    DEVMETHOD(sysVirtToPhys, sysVirtToPhys),
    DEVMETHOD(sysPhysToVirt, sysPhysToVirt),
#endif  /* _WRS_VX_AMP */
    { 0, 0 }
};

UINT32 sysTimerClkFreq;
LOCAL char * physTop = NULL;
LOCAL char * memTop  = NULL;

#ifdef IEEE754_FIX
LOCAL SPE_CONTEXT *speCtx;
#endif  /* IEEE754_FIX */

/* includes */

#ifdef INCLUDE_L2_CACHE
#   include "sysL2Cache.c"
#endif  /* INCLUDE_L2_CACHE */

#ifdef INCLUDE_I2C
#   include "sysMotI2c.c"
#   include "sysMpc85xxI2c.c"
#   include "ds1339Rtc.c"
#endif  /* INCLUDE_I2C */

#ifdef INCLUDE_FLASH
#   include "flashMem.c"
#   include "nvRamToFlash.c"
#else  /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif  /* INCLUDE_FLASH */

#ifdef INCLUDE_NETWORK
#   include "sysNet.c"
#endif  /* INCLUDE_NETWORK */

#include "hwconf.c"

#ifdef INCLUDE_GPIO
#   include "sysGpio.c"
#endif  /* INCLUDE_GPIO */

#ifdef INCLUDE_FSL_SPI
#   include "sysFslSpi.c"
#   include "sysSpiFlash.c"
#endif  /* INCLUDE_FSL_SPI */

#ifdef INCLUDE_NAND_FLASH
#   include "sysNandFlash.c"
#endif  /* INCLUDE_NAND_FLASH */

/* defines */

#define SYS_MODEL_P2020E        "Freescale P2020E - Security Engine"
#define SYS_MODEL_P2020         "Freescale P2020"
#define SYS_MODEL               "Freescale P2020"

#define SYS_MODEL_E500          "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN       "Freescale Unknown processor"

#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000

#ifdef _WRS_VX_AMP
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
    { "motetsec",       2,  DEVICE_CORE1 },
    { "ns16550",        0,  DEVICE_CORE0 },
    { "ns16550",        1,  DEVICE_CORE0 },
    { "openPicTimer",   0,  DEVICE_CORE0 },
    { "openPicTimer",   1,  DEVICE_CORE0 },
    { "m85xxCCSR",      0,  DEVICE_CORE0 },
    { "m85xxPci",       1,  DEVICE_CORE0 },
    { "vxbPlbUsbEhci",  0,  DEVICE_CORE0 },
    { "fslDma",         0,  DEVICE_CORE0 },
    { "sdhci",          0,  DEVICE_CORE0 },
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

    if ( pDev->pName == NULL)
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

#endif /* _WRS_VX_AMP */

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
* resetPhyDev - reset Ehernet PHY devices
*
* This routine used to reset the three Ehernet PHY devices, GbE switch,
* SGMII PHY and RGMII PHY, GPIO-12,GPIO-14 and GPIO-6 connected to
* their reset pins.
*
* RETURNS: N/A
*/

void resetPhyDev (void)
    {
    sysGpioLineConfig (GPIO_6, GPIO_OUT);
    sysGpioLineConfig (GPIO_12, GPIO_OUT);
    sysGpioLineConfig (GPIO_14, GPIO_OUT);

    sysGpioLineSet (GPIO_6, GPIO_HIGH);
    sysGpioLineSet (GPIO_12, GPIO_HIGH);
    sysGpioLineSet (GPIO_14, GPIO_HIGH);

    sysGpioLineSet (GPIO_6, GPIO_LOW);
    sysGpioLineSet (GPIO_12, GPIO_LOW);
    sysGpioLineSet (GPIO_14, GPIO_LOW);

    sysMsDelay (100);

    sysGpioLineSet (GPIO_6, GPIO_HIGH);
    sysGpioLineSet (GPIO_12, GPIO_HIGH);
    sysGpioLineSet (GPIO_14, GPIO_HIGH);

    sysMsDelay (500);

    }

/*******************************************************************************
* resetUsbPhy - reset USB PHYs (USB3300)
*
* This routine used to reset USB devices (GPIO-5)
*
* RETURNS: N/A
*/

void resetUsbPhy (void)
    {
    sysGpioLineConfig (GPIO_5, GPIO_OUT);
    sysMsDelay (10);
    sysGpioLineSet (GPIO_5, GPIO_HIGH);
    sysMsDelay (50);
    sysGpioLineSet (GPIO_5, GPIO_LOW);
    sysMsDelay (50);
    }

/*******************************************************************************
* resetPciDev - reset Pci devices
*
* This routine used to reset devices pluged in PCI-EX slots (GPIO-15)
*
* RETURNS: N/A
*/

void resetPciDev (void)
    {
    sysGpioLineConfig (GPIO_15, GPIO_OUT);
    sysGpioLineSet (GPIO_15, GPIO_HIGH);
    sysGpioLineSet (GPIO_15, GPIO_LOW);
    sysMsDelay (10);
    sysGpioLineSet (GPIO_15, GPIO_HIGH);
    }

/*******************************************************************************
*
* vsc7385Init - initialize VSC7385 L2 switch
*
* This routine load firmware routine to VSC7385.
*
* RETURNS: OK, or ERROR.
*/

STATUS vsc7385Init (void)
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
        case 0x80E20000:
            retChar = SYS_MODEL_P2020;
            break;
        case 0x80EA0000:
            retChar = SYS_MODEL_P2020E;
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
    UINT32  sysClkFreq;
    UINT32  e500Ratio;
    UINT32  e5001Ratio;
    UINT32  platRatio;
    UINT32  ddrRatio;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);
    ddrRatio = M85XX_PORPLLSR_DDR_RATIO(CCSBAR);

    if (   (platRatio >= NELEMENTS(platRatioTable)) 
        || (platRatioTable[platRatio] == 0)
       )
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

    systemFreq = FREQ_100_MHZ;
    ddrFreq = DDR_CLK_FREQ;

    ddrFreq = ddrFreq * ddrRatioTable[ddrRatio];
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

#ifdef DRV_STORAGE_SDHC

/*******************************************************************************
*
* sysSdhcClkFreqGet - get eSDHC controller clock frequency
*
* RETURNS: Clock freq of eSDHC controller
*
* ERRNO: N/A
*/
UINT32 sysSdhcClkFreqGet (void)
    {
    return (sysClkFreqGet () / 2);
    }
#endif  /* DRV_STORAGE_SDHC */

/*******************************************************************************
*
* sysCtrlDevResst - reset external END PHY,USB PHY and PCI-EX device
*
* This routine used to reset external END PHY,USB PHY and PCI-EX external bus
* on P2020RDB board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysCtrlDevResst (void)
    {

    /* Reset PHY devices */

    resetPhyDev ();

    /* Reset PCI device */

    resetPciDev ();

    /* Reset USB interface device */

    resetUsbPhy ();

    }


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
    return ((void *)((pPhys - WRLOAD_IMAGE_BUILD_PHYS_BIAS) & 0xFFFFFFFF));
    }

#endif /* INCLUDE_AMP */

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the P2020RDB board. It sets up
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
    vxIvprSet((UINT32)VEC_BASE_ADRS);

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

    cachePpcReadOrigin = (char *) &_wrs_kernel_text_start;

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

    /* reset enternal PCI bus and PHY device */

#ifdef _WRS_VX_AMP
#   ifdef INCLUDE_AMP_CPU_00
    sysCtrlDevResst ();
#   endif /* INCLUDE_AMP_CPU_00 */
#else /* _WRS_VX_AMP */
    sysCtrlDevResst ();
#endif /* _WRS_VX_AMP */

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

    pSysPlbMethods = p2020PlbMethods;

    /* vsc7385 initialize */

    vsc7385Init();

    /* config eSDHC pins */

#ifdef DRV_STORAGE_SDHC
    M85XX_PMUXCR &= ~SD_DATA;
    M85XX_PMUXCR |= SDHC_CD | SDHC_WP;
#endif  /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#ifdef INCLUDE_I2C
    i2cDrvInit (0, 0);
    i2cDrvInit (1, 0);
#endif  /* INCLUDE_I2C */

#ifdef INCLUDE_NAND_FLASH
    fcmReset ();
    fcmInit ();
#endif  /* INCLUDE_NAND_FLASH */

#ifdef INCLUDE_L2_SRAM
#   if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
        sysL2SramEnable (TRUE);
#   elif (defined(INCLUDE_L2_SRAM))
        sysL2SramEnable (FALSE);
#   endif
#endif

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


#ifdef _WRS_VX_AMP
    volatile int sysMonDelay;
#endif  /* _WRS_VX_AMP */

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

    /* set MSR to its power-up default value
     * disables ALL interrupts and exceptions, including debugging breakpoints
     */

    vxMsrSet(0);

    /* Clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

#ifdef  _WRS_CONFIG_SMP

    /*
     * Finally forcibly reset the secondary cores. This is done
     * using the processor reset register in the PIC.
     *
     * Note also that the bits in the EPIC Hard Reset Register are
     * not auto-clearing.
     */

    *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
    sysCpu1Stop ();
    *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
#endif  /* _WRS_CONFIG_SMP */

#ifdef  _WRS_VX_AMP 

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

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++)
        ;

    *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
    _WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++)
        ;

    *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
    WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */

    for(sysMonDelay = 0; sysMonDelay < 0x10000; sysMonDelay++)
        ;
#   endif  /* INCLUDE_AMP_CPU_00 */

#endif /* _WRS_VX_AMP */

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

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2CacheInit();
#endif  /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    if(_pSysL2CacheInvFunc!=NULL)
        _pSysL2CacheInvFunc();
#endif  /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    sysCacheEnable();

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    if(_pSysL2CacheEnable!=NULL)
        _pSysL2CacheEnable();
#endif  /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

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
     * For future reference:  DEVDISR is internal to the P2020 chip.
     * Platforms using multiple P2020's (i.e. more than two CPUs) cannot
     * achieve perfect synchronization; the best that can be done is to
     * deassert the multiple DEVDISR[TB] signals as near simultaneously as
     * possible by using CPC.  On such a platform, the |= operation below
     * should be replicated, specifying the appropriate base address to
     * access each P2020's CCSR space; and the &= operation below
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

        /*
         * In an SMP system the cacheFlush API is no-op, so call the AIM
         * primitive directly. Note, this is a restricted API, it must not
         * be used by regular application code. 
         */

        cacheAimE500FlushAllD();

        sysCpu1Start ();

        cacheAimE500FlushAllD();
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
* INTERNAL
* For future reference:  DEVDISR is internal to the P2020 chip.  Platforms
* using multiple P2020's (i.e. more than two CPUs) cannot achieve perfect
* synchronization; the best that can be done is to deassert the multiple
* DEVDISR[TB] signals as near simultaneously as possible.  On such a platform,
* the |= operation below should be replicated, specifying the appropriate
* base address to access each P2020's CCSR space; and the &= operation
* below should be replaced with a call to an assembly-language function
* which writes the multiple DEVDISR registers using consecutive machine
* instructions.
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

    if (initialized)
        return;

#ifdef INCLUDE_FSL_SPI
    sysSpiInit ();
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
    return(CPU1CHECK);
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
    strncpy ((char *)EXC_MSG_OFFSET, errorMsg, 0x100);
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

    }
#endif  /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciEx1IntRoute [NUM_PCIEX1_SLOTS][4] = {
    { PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL }
};

LOCAL UCHAR sysPciEx2IntRoute [NUM_PCIEX2_SLOTS][4] = {
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL }
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
    int token;

    token = intCpuLock();       /* CPC done, now safe to disable interrupts */

    cacheAimE500FlushAllD ();

    /* Disable the d-cache so the magic num is sure to be written to memory */

    vxL1CSR0Set(0);
    WRS_ASM("sync");

    /* Report to CPU0 this CPU's cache flush is complete */

    *(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_CACHE_FLUSHED;

    /* Finally idle this CPU */

#ifdef INCLUDE_CPU_LIGHT_PWR_MGR
    vxHid0Set(vxHid0Get() | _PPC_HID0_DOZE);
    WRS_ASM("sync");
    vxMsrSet(vxMsrGet()| _PPC_MSR_WE);
    WRS_ASM("isync");
#endif  /* INCLUDE_CPU_LIGHT_PWR_MGR */

    /* expect to lock in DOZE mode until HRESET */

    FOREVER;
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

    *((volatile unsigned int *)CPU1_STACK_START_ADR) 
            = ((unsigned int)  entryPt - 0x20);

    /* This is used if core waiting after core1 did asynchronous reboot */

    *((volatile int *)CPU1_INIT_START_ADR) = (int)entryPt;
    *((volatile int *)CPU1_LOCAL_MEM_ERPN) = usrWrloadBiasHighGet(cpu);
    *((volatile int *)CPU1_LOCAL_MEM_RPN)  = usrWrloadBiasLowGet (cpu);

#ifdef INCLUDE_CACHE_SUPPORT
    cacheFlush(_DATA_CACHE, CPU1_FUNC_START_ADR,
                 (CPU1_LOCAL_MEM_RPN-CPU1_FUNC_START_ADR));
#endif

    /* If core1 is not already running then enable it */

    if((*EEB_PCR(CCSBAR) & EEB_PCR_CORE1_ENABLE) == 0)
        {
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

/* sysLib.c - Freescale P1010RDB board system-dependent library */

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
01c,05mar12,y_y  add vxBus I2C support.
01b,04jan12,agf  Correct value used to init L2CTL register (WIND00325436)
01a,10sep11,fao  derived from fsl_p1020_rdb/sysLib.c
*/

/*
DESCRIPTION
This library provides board-specific routines for P1010RDB

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
    spS25flxxSpiFlash.c - S25FL128P SPI flash driver.
    sysL2Cache.c        - L1 cache parity support.
    sysGpio.c           - GPIO controller driver.
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

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SPE
#   include <speLib.h>
#endif  /* INCLUDE_SPE */

/* globals */

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

    {
    (FLASH_BASE_ADRS + 0x1000000), 0x0, (FLASH_BASE_ADRS + 0x1000000),
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M   | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },

    {
    0x00000000, 0x0, 0x00000000,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G    | _MMU_TLB_PERM_W |
    _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT  |
    _MMU_TLB_ATTR_M
    },

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W  | _MMU_TLB_IPROT
    },

#ifdef INCLUDE_PCI_BUS
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
    (VIRT_ADDR) PHYS_MEM_START,
    (PHYS_ADDR) PHYS_MEM_START,
    PHYS_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

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
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX1_MEMIO_ADRS,
    PCIEX1_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_IO_ADRS,
    (PHYS_ADDR) PCIEX1_IO_ADRS,
    PCIEX1_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },


    {
    (VIRT_ADDR) PCIEX2_MEM_ADRS,
    (PHYS_ADDR) PCIEX2_MEM_ADRS,
    PCIEX2_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
    PCIEX2_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_IO_ADRS,
    (PHYS_ADDR) PCIEX2_IO_ADRS,
    PCIEX2_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
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
UINT32  ddrFreq;
UINT32  systemFreq;

UINT32  sysClkFreqGet(void);
UINT32  ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;

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

IMPORT void mmuE500TlbDynamicInvalidate();
IMPORT void mmuE500TlbStaticInvalidate();
IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, BOOL cacheAllow);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;
IMPORT void sysIvprSet(UINT32);

IMPORT BOOL e500_spe_exc_enable;
IMPORT BOOL e500_spe_exc_mask;

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

IMPORT void vxSdaInit (void);

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP();
IMPORT void vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef  INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif   /* INCLUDE_SIO_UTILS */

#ifdef INCLUDE_SPE
IMPORT int  (* _func_speProbeRtn) ();
#endif /* INCLUDE_SPE */

IMPORT void sysL1Csr1Set(UINT32);
IMPORT UINT sysTimeBaseLGet(void);
IMPORT void hardWareInterFaceInit (void);

/* forward declarations */

IMPORT void (*_vxb_usDelayRtn) (int delayTime);
IMPORT void (*_vxb_msDelayRtn) (int delayTime);
IMPORT void (*_vxb_delayRtn) (void);

VOID    sysUsDelay(int);
VOID    sysMsDelay(int);
STATUS  sysIntEnablePIC(int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable (int intNum);
STATUS  sysCascadeIntDisable(int intNum);

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL   void sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

#ifdef DRV_STORAGE_SDHC
VOID sysSdhcEnable();
VOID sysSdhcDisable();
#endif /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_USB1
VOID sysUsb1Enable();
VOID sysUsb1Disable();
#endif /* INCLUDE_USB1 */

UINT32 sysTimerClkFreq;
LOCAL char * physTop = NULL;
LOCAL char * memTop  = NULL;

#ifdef IEEE754_FIX
LOCAL SPE_CONTEXT *speCtx;
#endif  /* IEEE754_FIX */

#ifdef INIT_HWMEMPOOL_IN_BSP 
char  hwMemPool[HWMEM_POOL_SIZE];
IMPORT void hwMemLibInit (void);
IMPORT STATUS hwMemPoolCreate (char * pMem, size_t size);
#endif /* INIT_HWMEMPOOL_IN_BSP */

/* includes */

#ifdef INCLUDE_L2_CACHE
#   include "sysL2Cache.c"
#endif  /* INCLUDE_L2_CACHE */
 
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

#ifdef DRV_SPI_S25FLXX
#   include "sysFslSpi.c"
#   include "spS25flxxSpiFlash.c"
#endif  /* DRV_SPI_S25FLXX */

/* defines */

#define SYS_MODEL_P1010E        "Freescale P1010E - Security Engine"
#define SYS_MODEL_P1010         "Freescale P1010"
#define SYS_MODEL_P1014E        "Freescale P1014E - Security Engine"
#define SYS_MODEL_P1014         "Freescale P1014"
#define SYS_MODEL               "Freescale P1010"

#define SYS_MODEL_E500          "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN       "Freescale Unknown processor"

#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000

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
    sysGpioLineConfig (GPIO_12, GPIO_OUT);
    sysGpioLineConfig (GPIO_13, GPIO_OUT);
    sysGpioLineConfig (GPIO_14, GPIO_OUT);
    sysGpioLineConfig (GPIO_15, GPIO_OUT);

    sysGpioLineSet (GPIO_12, GPIO_HIGH);
    sysGpioLineSet (GPIO_13, GPIO_HIGH);
    sysGpioLineSet (GPIO_14, GPIO_HIGH);
    sysGpioLineSet (GPIO_15, GPIO_HIGH);

    sysGpioLineSet (GPIO_12, GPIO_LOW);
    sysGpioLineSet (GPIO_13, GPIO_LOW);
    sysGpioLineSet (GPIO_14, GPIO_LOW);
    sysGpioLineSet (GPIO_15, GPIO_LOW);

    sysMsDelay (100);

    sysGpioLineSet (GPIO_12, GPIO_HIGH);
    sysGpioLineSet (GPIO_13, GPIO_HIGH);
    sysGpioLineSet (GPIO_14, GPIO_HIGH);
    sysGpioLineSet (GPIO_15, GPIO_HIGH);

    sysMsDelay (500);

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
    sysGpioLineConfig (GPIO_4, GPIO_OUT);
    sysGpioLineSet (GPIO_4, GPIO_HIGH);
    sysGpioLineSet (GPIO_4, GPIO_LOW);
    sysMsDelay (10);
    sysGpioLineSet (GPIO_4, GPIO_HIGH);
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
    switch(device)
        {
        case 0x80F10010:
            retChar = SYS_MODEL_P1010;
            break;
        case 0x80F90010:
            retChar = SYS_MODEL_P1010E;
            break;
        case 0x80F90110:
            retChar = SYS_MODEL_P1014E;
            break;
        case 0x80F10110:
            retChar = SYS_MODEL_P1014;
            break;
        default:
            retChar = SYS_MODEL_E500;
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
    UINT32  platRatio;
    UINT32  ddrRatio;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);
    ddrRatio = M85XX_PORPLLSR_DDR_RATIO(CCSBAR);

    if ((platRatio >= MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio][0] == 0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

    systemFreq = FREQ_66_MHZ;

    ddrFreq = DDR_CLK_FREQ * ddrRatioTable[ddrRatio];
    sysClkFreq = ((UINT32)(systemFreq * platRatioTable[platRatio][0])) >> \
                 ((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);

    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0])) >> \
               ((UINT32)e500RatioTable[e500Ratio][1]);

    return(sysClkFreq);
    }

#ifdef DRV_I2CBUS_FSL

/*******************************************************************************
*
* sysI2cClkFreqGet - get I2C controller clock frequency
*
* RETURNS: Clock freq of I2C controller
*
* ERRNO: N/A
*/

UINT32 sysI2cClkFreqGet (void)
    {
    return (sysClkFreqGet () / 2);
    }
#endif  /* DRV_I2CBUS_FSL */

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
        return HPCN_RTC_FREQ;   /* 32.768 MHz */
    }

/*******************************************************************************
*
* sysCtrlDevReset - reset external END PHY,USB PHY and PCI-EX device
*
* This routine used to reset external END PHY,USB PHY and PCI-EX external bus
* on P1010RDB board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysCtrlDevReset (void)
    {

    /* Reset PHY devices */

    resetPhyDev ();

    /* Reset PCI device */

    resetPciDev ();

    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the P1010RDB board. It sets up
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
#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    l2CacheLineNum = ((2048 + 4096) * L2_CACHE_SIZE);
#endif  /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    /* Disable L1 Icache */

    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

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

    sysCtrlDevReset ();

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

    _vxb_usDelayRtn = &sysUsDelay;
    _vxb_msDelayRtn = &sysMsDelay;
    _vxb_delayRtn = &sysDelay;
    hardWareInterFaceInit();
 
    CACHE_PIPE_FLUSH();

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy(sysBootLine, DEFAULT_BOOT_LINE, strlen(DEFAULT_BOOT_LINE)+1);
#endif /* FORCE_DEFAULT_BOOT_LINE */

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
* only by reboot()--which services ^X--and bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*
* ERRNO: N/A
*/

STATUS sysToMonitor
    (
    int startType   /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);   /* Warm reboot */
    int token;

    token = intCpuLock();

#ifdef DRV_STORAGE_SDHC
    sysSdhcDisable ();
#endif /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_USB1
    sysUsb1Disable ();
#endif /* INCLUDE_USB1 */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#endif  /* INCLUDE_CACHE_SUPPORT */

    vxMsrSet(0);

    /* Clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    (*pRom) (startType);    /* jump off to romInit.s */

    return(OK);    /* in case we ever continue from ROM monitor */
    }

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
    sysSpiDrvInit(0, FREQ_33_MHZ);
#endif /* INCLUDE_FSL_SPI */ 

#ifdef DRV_STORAGE_SDHC
    sysSdhcEnable ();
#endif /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_FSL_TCD
    ehciInit();
#endif /* INCLUDE_FSL_TCD */

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
    int      delay                   /* length of time in MS to delay */
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

#ifdef DRV_STORAGE_SDHC
/*******************************************************************************
*
* sysSdhcEnable - enable SDHC
*
* This function call is used to enable SDHC.
*
* RETURNS: N/A
*/

VOID sysSdhcEnable (void)
    {
    UINT8 a = 0xfb;
    UINT8 b = 0xf0;
    UINT8 c = 0xf4;

    vxbI2cByNameWrite("gpio_pca9557", 0, 3, &a, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 2, &b, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 1, &c, 1);

    M85XX_PMUXCR |= SDHC_CLK | SDHC_DATA | SDHC_WP | SDHC_CD;
 
    }

/*******************************************************************************
*
* sysSdhcDisable - disable SDHC
*
* This function call is used to disable SDHC.
*
* RETURNS: N/A
*/

VOID sysSdhcDisable (void)
    {
    UINT8 a = 0xf0;
    UINT8 b = 0xf0;
    UINT8 c = 0xf1;

    vxbI2cByNameWrite("gpio_pca9557", 0, 3, &a, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 2, &b, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 1, &c, 1);

    M85XX_PMUXCR &= ~(SDHC_CLK | SDHC_DATA | SDHC_WP | SDHC_CD);

    }
#endif /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_USB1
/*******************************************************************************
*
* sysUsb1Enable - enable usb1
*
* This function call is used to enable usb1.
*
* RETURNS: N/A
*/

VOID sysUsb1Enable (void)
    {
    UINT8 a = 0xf8;
    UINT8 b = 0xf0;
    UINT8 c = 0xf2;

    vxbI2cByNameWrite("gpio_pca9557", 0, 3, &a, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 2, &b, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 1, &c, 1);

    M85XX_PMUXCR |= USB_DATA | USB_CLK | USB_STP | USB_NXT;

    }

/*******************************************************************************
*
* sysUsb1Disable - disable usb1
*
* This function call is used to disable usb1.
*
* RETURNS: N/A
*/

VOID sysUsb1Disable (void)
    {
    UINT8 a = 0xf0;
    UINT8 b = 0xf0;
    UINT8 c = 0xf1;

    vxbI2cByNameWrite("gpio_pca9557", 0, 3, &a, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 2, &b, 1);
    vxbI2cByNameWrite("gpio_pca9557", 0, 1, &c, 1);

    sysMsDelay(1000);

    M85XX_PMUXCR &= ~(USB_DATA | USB_CLK | USB_STP | USB_NXT);
 
    }
#endif /* INCLUDE_USB1 */

#ifdef INCLUDE_SHOW_ROUTINES

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

    printf("Core CPU Freq = %3d Hz\n", coreFreq);
    printf("DDR Freq = %3d Hz\n", ddrFreq);
    printf("CCB Freq = %3d Hz\n", sysClkFreqGet());
    printf("System Freq = %3d Hz\n", systemFreq);

    }
#endif  /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciEx2IntRoute [NUM_PCIEX2_SLOTS][4] = {
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL }
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

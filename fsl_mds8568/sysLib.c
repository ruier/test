/* sysLib.c - Freescale MDS8568 board system-dependent library */

/*
 * Copyright (c) 2007-2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01l,27aug09,x_z  remove qeCpcrReset().
01k,15jul09,b_m  remove QE IO port pin mux codes.
01j,06mar09,syt  correct a BCSR register typo in sysPhyReset().(WIND00156916)
01i,30oct08,x_z  add vxEnableBP to sysHwInit and replace disableBranchPrediction
                 with vxDisableBP.(WIND00127193)
01h,24jul08,b_m  switch to use new vxbus etsec and ucc driver.
01g,23aug07,mmi  remove vxPowerModeSet() since deprecated
01f,30jul07,b_m  remove cmdLine.c dependency.
01e,26jul07,b_m  modify to use m85xxTimer vxbus driver.
01d,18jul07,b_m  modify to use epic/qe interrupt controller vxbus driver.
01c,12jul07,b_m  modify to use m85xxPci vxbus driver.
01b,28may07,b_m  add QE interrupt, IO port mux and clock routing.
01a,10may07,b_m  derived from version 01c of wrSbc8548/sysLib.c.
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <sysLib.h>
#include <memLib.h>
#include <cacheLib.h>
#include <vmLib.h>
#include <intLib.h>
#include <logLib.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <arch/ppc/mmuE500Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include "config.h"
#ifdef INCLUDE_QUICC_ENGINE_UTILS
#include <../src/hwif/h/resource/qeCp.h>
#endif

/* globals */

/* effAddr, realAddrExt, realAddr, ts | size | permissions | attributes */

TLB_ENTRY_DESC sysStaticTlbDesc [] = {
    {
    FLASH_BASE_ADRS - FLASH_SIZE, 0, FLASH_BASE_ADRS - FLASH_SIZE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

    {
    LOCAL_MEM_LOCAL_ADRS, 0, LOCAL_MEM_LOCAL_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M | CAM_DRAM_CACHE_MODE
    },

    {
    LOCAL_MEM_LOCAL_ADRS + (LOCAL_MEM_SIZE / 2), 0, LOCAL_MEM_LOCAL_ADRS + (LOCAL_MEM_SIZE / 2),
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M | CAM_DRAM_CACHE_MODE
    },

    {
    CCSBAR, 0, CCSBAR,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

    {
    BCSR_BASE_ADRS, 0, BCSR_BASE_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_16K | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

#ifdef INCLUDE_LBC_SDRAM
    {
    LOCAL_MEM_LOCAL_ADRS2, 0, LOCAL_MEM_LOCAL_ADRS2,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M | CAM_DRAM_CACHE_MODE
    },
#endif

#ifdef DRV_PCIBUS_M85XX
    {
    PCIEX_MEM_ADRS, 0, PCIEX_MEM_ADRS,
    _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },
#endif

#ifdef INCLUDE_L2_SRAM
    {
    L2SRAM_ADDR, 0, L2SRAM_ADDR,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },
#endif
};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

/* Base 6 MMU library in effect */

#ifdef MMU_ASID_MAX
/* macro to compose 64-bit PHYS_ADDRs */
#   define PHYS_64BIT_ADDR(h, l)    (((PHYS_ADDR)(h) << 32) + (l))
#endif

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
 *
 */

PHYS_MEM_DESC sysPhysMemDesc [] = {
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE | MMU_ATTR_CACHE_COHERENCY
    },

    {
    (VIRT_ADDR) FLASH_BASE_ADRS,
    (PHYS_ADDR) FLASH_BASE_ADRS,
    FLASH_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    0x00100000,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) BCSR_BASE_ADRS,
    (PHYS_ADDR) BCSR_BASE_ADRS,
    BCSR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    },

#ifdef INCLUDE_LBC_SDRAM
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    LOCAL_MEM_SIZE2,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE | MMU_ATTR_CACHE_COHERENCY
    },
#endif

#ifdef DRV_PCIBUS_M85XX
    {
    (VIRT_ADDR) PCIEX_MEM_ADRS,
    (PHYS_ADDR) PCIEX_MEM_ADRS,
    PCI_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX_IO_ADRS,
    (PHYS_ADDR) PCIEX_IO_ADRS,
    PCI_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
#endif

#ifdef INCLUDE_L2_SRAM
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    L2SRAM_WINDOW_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    },
#endif
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);


/* Clock Ratio Tables */

#define MAX_VALUE_PLAT_RATIO    32
UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO][2] = {
    { 0,  0 }, { 0, 0 }, { 2,  0 }, { 3, 0 },
    { 4,  0 }, { 5, 0 }, { 6,  0 }, { 0, 0 },
    { 8,  0 }, { 9, 0 }, { 10, 0 }, { 0, 0 },
    { 12, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 16, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 20, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 }
};

#define MAX_VALUE_E500_RATIO    10
UINT32 e500RatioTable[MAX_VALUE_PLAT_RATIO][2] = {
    { 0, 0 }, { 0, 0 }, { 1, 0 }, { 3, 1 }, { 2, 0 },
    { 5, 1 }, { 3, 0 }, { 7, 1 }, { 4, 0 }, { 9, 1 }
};

int     sysBus          = BUS_TYPE_NONE;    /* system bus type (VME_BUS, etc) */
int     sysCpu          = CPU;              /* system CPU type */
char *  sysBootLine     = BOOT_LINE_ADRS;   /* address of boot line */
char *  sysExcMsg       = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum      = 0;                /* processor number of this CPU */

UINT32  coreFreq;
UINT32  sysTimerClkFreq = OSCILLATOR_FREQ;
UINT32  inFullVxWorksImage = FALSE;

UINT32  ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;

/* imports */

IMPORT void mmuE500TlbStaticInvalidate(void);
IMPORT void mmuE500TlbDynamicInvalidate(void);
IMPORT void mmuE500TlbStaticInit(int, TLB_ENTRY_DESC *, BOOL);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;

IMPORT void sysL1Csr1Set(UINT32);
IMPORT UINT sysTimeBaseLGet(void);

IMPORT void hardWareInterFaceInit(void);
#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void     vxEnableBP();
IMPORT void     vxDisableBP();
#endif

#ifdef INCLUDE_SPE
#include <speLib.h>
IMPORT int (* _func_speProbeRtn)();
#endif

/* forward declarations */

UINT32 sysClkFreqGet(void);
void   sysUsDelay(UINT32);
void   sysMsDelay(UINT);
void   sysDelay(void);

LOCAL void sysDuartEnable(void);
LOCAL void sysFlashEnable(void);
LOCAL void sysPhyReset(void);

#ifdef INCLUDE_SPE
int sysSpeProbe(void);
#endif

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL void sysL1CacheQuery(void);
#endif

#ifdef INCLUDE_L2_SRAM
LOCAL void sysL2SramEnable(BOOL both);
#endif

#ifdef INCLUDE_UCC_VXB_END
LOCAL void   qeIOPortInit(void);
LOCAL void   sysUccPhyEnable(void);
#else
LOCAL void   sysUccPhyDisable(void);
#endif

/* L2 Cache */

#include "sysL2Cache.c"

/* I2C */

#ifdef INCLUDE_I2C
#   include "sysMotI2c.c"
#   include "sysMpc85xxI2c.c"
#endif

/* RTC */

#ifdef INCLUDE_RTC
#   include "ds1374.c"
#endif

/* nvRam */

#include "flashMem.c"
#include "nvRamToFlash.c"

/* network */

#include "sysNet.c"

/* vxbus support */

#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"


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

char * sysModel(void)
    {
    UINT device;
    char * retChar = NULL;

    device = *M85XX_SVR(CCSBAR);

    switch (device & 0xffffff00)
        {
        case 0x807d0000:
            retChar = SYS_MODEL_8568E;
            break;
        case 0x80750000:
            retChar = SYS_MODEL_8568;
            break;
        case 0x807e0000:
            retChar = SYS_MODEL_8567E;
            break;
        case 0x80760000:
            retChar = SYS_MODEL_8567;
            break;
        default:
            retChar = SYS_MODEL_E500;
            break;
        }

    device = *M85XX_PVR(CCSBAR);

    if ((device & 0xfff00000) != 0x80200000)
        retChar = SYS_MODEL_UNKNOWN;

    return retChar;
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

char * sysBspRev(void)
    {
    return (BSP_VERSION BSP_REV);
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
* RETURNS: NA
*
* ERRNO: N/A
*/

void sysHwInit(void)
    {
#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    sysDuartEnable();
    sysFlashEnable();
    sysPhyReset();

#ifdef INCLUDE_UCC_VXB_END
    sysUccPhyEnable();
#else
    sysUccPhyDisable();
#endif

    /* disable L1 Icache */

    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    /* enable time base for delay use before DEC interrupt is setup */

    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

    /* clock div is 8 */

    sysTimerClkFreq = sysClkFreqGet() >> 3;

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif

    /* initialise L2CTL register */

    vxL2CTLSet(0x28000000, M85XX_L2CTL(CCSBAR));

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */

#ifndef BOOT_FROM_UBOOT
    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);
#endif

#if (!defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL))
    mmuPpcIEnabled = TRUE;
    mmuPpcDEnabled = TRUE;
#else
    if (inFullVxWorksImage == FALSE)
        {
        mmuPpcIEnabled = TRUE;
        mmuPpcDEnabled = TRUE;
        }

    /* Enable I Cache if instruction mmu disabled */

#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    mmuPpcIEnabled = TRUE;
#endif /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxHid1Set(HID1_ABE);
    sysL2CacheInit();
#endif

    /* machine check via RXFE for RIO */

    vxHid1Set(vxHid1Get()| HID1_ASTME | HID1_RXFE);

#ifdef INCLUDE_UCC_VXB_END
    qeIOPortInit();
#endif

    hardWareInterFaceInit();

#ifdef INCLUDE_I2C
    i2cDrvInit(0, 0);
#endif

#ifdef INCLUDE_L2_SRAM
#   if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
        sysL2SramEnable(TRUE);
#   elif (defined(INCLUDE_L2_SRAM))
        sysL2SramEnable(FALSE);
#   endif
#endif

    CACHE_PIPE_FLUSH();
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

void sysHwInit2(void)
    {
    vxbDevInit();

#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
#endif

#ifdef INCLUDE_SPE
    _func_speProbeRtn = sysSpeProbe;
#endif

    taskSpawn("tDevConn", 11, 0, 10000, vxbDevConnect, 0,1,2,3,4,5,6,7,8,9);
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

char * sysPhysMemTop(void)
    {
    static char * physTop = NULL;

    if (physTop == NULL)
        {
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
* The user can reserve memory space by defining the macro USER_RESERVED_MEM
* in config.h.  This routine returns the address of the reserved memory
* area.  The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*
* ERRNO: N/A
*/

char * sysMemTop(void)
    {
    static char * memTop = NULL;

    if (memTop == NULL)
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM

        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif
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
    FUNCPTR pRom = (FUNCPTR)(ROM_TEXT_ADRS + 4);    /* Warm reboot */

    intLock();

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#endif

    sysClkDisable();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif

    vxMsrSet(0);

    /* clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    /* jump to bootrom entry point */

    (*pRom)(startType);

    /* in case we ever continue from ROM monitor */

    return OK;
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

int sysProcNumGet(void)
    {
    return sysProcNum;
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
* sysClkFreqGet - return Core Complex Bus clock freq
*
* This function returns the CCB clock freq.
*
* RETURNS: CCB clock freq
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet(void)
    {
    UINT32  sysClkFreq;
    UINT32  e500Ratio, platRatio;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);

    if ((platRatio > MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio][0] == 0))
        return DEFAULT_SYSCLKFREQ;

    sysClkFreq = ((UINT32)(OSCILLATOR_FREQ * platRatioTable[platRatio][0]))
                 >> ((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0]))
               >> ((UINT32)e500RatioTable[e500Ratio][1]);

    return sysClkFreq;
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
    UINT32  delay               /* length of time in microsec to delay */
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
* sysMsDelay - delay at least the specified amount of time (in milliseconds)
*
* This routine just calls sysUsDelay to delay 1ms.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    UINT    delay               /* length of time in MS to delay */
    )
    {
    sysUsDelay((UINT32)delay * 1000);
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

void sysDelay(void)
    {
    sysMsDelay(1);
    }

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
    char * errorMsg
    )
    {
    strcpy((char *)EXC_MSG_OFFSET, errorMsg);
    }


/*******************************************************************************
*
* chipErrataCpu29Print - chip errata cpu 29 handler
*
* The message will be displayed upon rebooting with a bootrom.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void chipErrataCpu29Print(void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    }

/*******************************************************************************
*
* sysDuartEnable - enable DUART
*
* This routine is called to enable the DUART on the MDS8568 board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysDuartEnable(void)
    {
    *BCSR5 |= 0x01;
    *M85XX_PMUXCR(CCSBAR) |= 0x80000000;
    *M85XX_DEVDISR(CCSBAR) = 0;
    }

/*******************************************************************************
*
* sysFlashEnable - unprotect the flash part
*
* This routine is called to unprotect the flash part on MDS8568 board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysFlashEnable(void)
    {
    *BCSR9 &= ~0x01;
    }

/*******************************************************************************
*
* sysPhyReset - reset all the 4 PHYs
*
* This routine is called to reset the 4 PHYs on the MDS8568 board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysPhyReset(void)
    {
    unsigned int count;

    *BCSR4 |= 0x02;

    for (count = 0; count < 0x200; count++)
        {
        WRS_ASM("eieio");
        }

    *BCSR4 &= ~0x02;
    }

#ifdef INCLUDE_SPE

/*******************************************************************************
*
* sysSpeProbe - Check if the CPU has SPE unit.
*
* This routine returns OK it the CPU has an SPE unit in it.
* Presently it assumes available.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

int sysSpeProbe(void)
    {
    return OK;
    }

#endif

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
     */

    /* Setup Windows for L2SRAM */

    *(M85XX_L2SRBAR0(CCSBAR)) = (UINT32)(L2SRAM_ADDR & M85XX_L2SRBAR_ADDR_MSK);

    /* Get present value */

    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));

    /* Disable L2CTL initially to allow changing of block size */

    l2CtlVal &= ~M85XX_L2CTL_L2E_MSK;
    vxL2CTLSet(l2CtlVal, M85XX_L2CTL(CCSBAR));
    l2CtlVal &= ~M85XX_L2CTL_L2BLKSIZ_MSK;
    l2CtlVal &= ~M85XX_L2CTL_L2SRAM_MSK;

    if (both == TRUE)
        {
        /* Setup size of SRAM */

        l2CtlVal |= (L2SIZ_256KB << M85XX_L2CTL_L2BLKSIZ_BIT) |
                    (0x2 << M85XX_L2CTL_L2SRAM_BIT);
        }
    else
        {
        l2CtlVal |= (L2SIZ_512KB << M85XX_L2CTL_L2BLKSIZ_BIT) |
                    (0x1 << M85XX_L2CTL_L2SRAM_BIT);
        }

    /* Setup L2CTL for SRAM */

    vxL2CTLSet(l2CtlVal, M85XX_L2CTL(CCSBAR));

    if (both == FALSE)
        {
        /* This is done here so L2SRAM is set before enable */

        l2CtlVal |= M85XX_L2CTL_L2E_MSK; /* No cache so go ahead and enable */

        /* Enable L2CTL for SRAM */

        vxL2CTLSet(l2CtlVal, M85XX_L2CTL(CCSBAR));
        }
    }

#endif /* INCLUDE_L2_SRAM */

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

/* QE UCC Support */

#ifdef INCLUDE_UCC_VXB_END

/*******************************************************************************
*
* qeIOPortInit - Init values to establish correct IO Port configuration.
*
* The routine sets up the pin connections for the functionality required by
* this BSP. The information is obtained from the chip users maunual and the
* board schematics.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void qeIOPortInit(void)
    {
    /* enabling gbl bit for snooping */

    *QE_SDMR(CCSBAR)   |= 0x80000000;

    *QE_CMXUCR1(CCSBAR) = 0x00aa0000;
    *QE_CMXUCR3(CCSBAR) = 0x00bb0000;
    }

/*******************************************************************************
*
* sysUccPhyEnable - enable UCC PHYs
*
* This routine is called to enable the two UCC PHYs.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysUccPhyEnable(void)
    {
    *BCSR8 |= 0x80;
    *BCSR9 |= 0x80;
    }

#else /* !INCLUDE_UCC_VXB_END */

/*******************************************************************************
*
* sysUccPhyDisable - disable UCC PHYs
*
* This routine is called to disable the two UCC PHYs.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysUccPhyDisable(void)
    {
    *BCSR8 &= ~0x80;
    *BCSR9 &= ~0x80;
    }

#endif /* INCLUDE_UCC_VXB_END */

#ifdef DRV_PCIBUS_M85XX

LOCAL UCHAR sysPciExIntRoute [NUM_PCIEX_SLOTS][4] =
    {PCIEX_XINT1_LVL, PCIEX_XINT2_LVL, PCIEX_XINT3_LVL, PCIEX_XINT4_LVL};

/*******************************************************************************
*
* sysPciExAutoconfigInclude - PCI Express autoconfig support routine
*
* This routine performs the PCI Express auto configuration support function.
*
* RETURNS: OK or ERROR
*/

STATUS sysPciExAutoconfigInclude
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UINT            devVend     /* deviceID/vendorID of device */
    )
    {
    if ((pLoc->bus > 2) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0) ||
        (pLoc->device > 0))
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* sysPciExAutoconfigIntrAssign - PCI Express autoconfig support routine
*
* This routine peforms the PCI Express auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciExAutoconfigIntrAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciExIntRoute[0][pin - 1];
    }

#endif /* DRV_PCIBUS_M85XX */

#ifdef INCLUDE_LED_DEBUG

/*******************************************************************************
*
* sysLedOn - control the LEDs
*
* This routine controls the LEDs on the MDS8568 board to be ON or OFF.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysLedOn
    (
    int     color,
    BOOL    on
    )
    {
    if (on)
        *BCSR5 |= (color << 4);
    else
        *BCSR5 &= ~(color << 4);
    }

/*******************************************************************************
*
* sysLedClkRoutine - show clock activity on LED
*
* This routine blinks the BCSR LEDs alternately.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysLedClkRoutine
    (
    int arg
    )
    {
    static int  clkIntCount = 0;
    static INT8 clkCount = 0;

    if ((clkIntCount++ % 60) == 0)
        {
        clkCount++;
        switch (clkCount % 6)
            {
            case 0:
                sysLedOn(BCSR_LED_RED, 1);
                break;
            case 1:
                sysLedOn(BCSR_LED_RED, 0);
                break;
            case 2:
                sysLedOn(BCSR_LED_YELLOW, 1);
                break;
            case 3:
                sysLedOn(BCSR_LED_YELLOW, 0);
                break;
            case 4:
                sysLedOn(BCSR_LED_GREEN, 1);
                break;
            case 5:
                sysLedOn(BCSR_LED_GREEN, 0);
                break;
            }
        }
    }

#endif /* INCLUDE_LED_DEBUG */

#ifdef INCLUDE_SHOW_ROUTINES

#define xbit0(x, n)     ((x & (1 << (31 - n))) >> (31 - n)) /* 0..31 */
#define xbit32(x, n)    ((x & (1 << (63 - n))) >> (63 - n)) /* 32..63 */
#define onoff0(x, n)    xbit0(x, n) ? "ON", "OFF"
#define onoff32(x, n)   xbit32(x, n) ? "ON", "OFF"

/*******************************************************************************
*
* coreShow - show routine for core registers
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

    printf("Core Freq = %3d Hz\n",coreFreq);
    printf("CCB Freq = %3d Hz\n",sysClkFreqGet());
    printf("PCI Freq = %3d Hz\n",OSCILLATOR_FREQ);
    printf("DDR Freq = %3d Hz\n",sysClkFreqGet()>>1);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c08);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c10);
    printf("LAWBAR0 = 0x%08x\t LAWAR0 = 0x%08x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c28);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c30);
    printf("LAWBAR1 = 0x%08x\t LAWAR1 = 0x%08x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c48);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c50);
    printf("LAWBAR2 = 0x%08x\t LAWAR2 = 0x%08x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c68);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c70);
    printf("LAWBAR3 = 0x%08x\t LAWAR3 = 0x%08x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c88);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c90);
    printf("LAWBAR4 = 0x%08x\t LAWAR4 = 0x%08x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0ca8);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0cb0);
    printf("LAWBAR5 = 0x%08x\t LAWAR5 = 0x%08x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0cc8);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0cd0);
    printf("LAWBAR6 = 0x%08x\t LAWAR6 = 0x%08x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0ce8);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0cf0);
    printf("LAWBAR7 = 0x%08x\t LAWAR7 = 0x%08x\n", tmp, tmp2);
    }

/*******************************************************************************
*
* coreLbcShow - show routine for local bus controller
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

    tmp = * (VINT32 *) M85XX_BR0(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR0(CCSBAR);
    printf("Local Bus BR0 = 0x%08x\tOR0 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR1(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR1(CCSBAR);
    printf("Local Bus BR1 = 0x%08x\tOR1 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR2(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR2(CCSBAR);
    printf("Local Bus BR2 = 0x%08x\tOR2 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR3(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR3(CCSBAR);
    printf("Local Bus BR3 = 0x%08x\tOR3 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR4(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR4(CCSBAR);
    printf("Local Bus BR4 = 0x%08x\tOR4 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR5(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR5(CCSBAR);
    printf("Local Bus BR5 = 0x%08x\tOR5 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR6(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR6(CCSBAR);
    printf("Local Bus BR6 = 0x%08x\tOR6 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR7(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR7(CCSBAR);
    printf("Local Bus BR7 = 0x%08x\tOR7 = 0x%08x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_LBCR(CCSBAR);
    printf("Local Bus LBCR = 0x%08x\n", tmp);

    tmp = * (VINT32 *) M85XX_LCRR(CCSBAR);
    printf("Local Bus LCRR = 0x%08x\n", tmp);
    }

#endif /* INCLUDE_SHOW_ROUTINES */

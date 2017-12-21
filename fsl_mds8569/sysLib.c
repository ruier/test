/* sysLib.c - Freescale MDS8569 board system-dependent library */

/*
 * Copyright (c) 2009-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01g,30sep11,y_c  fix the reboot hang when build image with QE UART enabled.
                 (WIND00308478)
01f,29sep11,y_c  fix the build fail when enable I2C interface. (WIND00306080)
01e,07jul11,y_c  Support VxWorks native bootrom and SD/MMC card. (WIND00240156)
01d,15jan10,x_z  fix sysPciExAutoconfigIntrAssign() routine.
01c,15oct09,x_z  add support for UCC SIO and remove qeCpcrReset()
01b,10sep09,x_z  replace M85XX_PVR and M85XX_SVR with vxPvrGet() and vxSvrGet()
01a,10jul09,x_z  derived from version 01i of fsl_mmds8568/sysLib.c.
*/

/*
DESCRIPTION
This library provides board-specific routines for the Freescale MDS8569 BSP.

It includes the following chip drivers:
    flashMem.c -            flash memory driver
    nvRamToFlash.c -        routines for flash as NVRAM
    sysL2Cache.c -          L2 cache routines

if INCLUDE_I2C is defined, it includes:
    sysMpc85xxI2c.c -       I2C driver source (low level routines) module
    sysMotI2c.c -           I2C driver source module

if INCLUDE_I2C is defined and INCLUDE_RTC is defined, it #includes:
    ds1374.c -              Dallas DS1374 RTC driver

if INCLUDE_VXBUS is defined, it includes:
    vxBus.h -               vxBus subsystem header file
    hwconf.c -              hardware configuration support module

INCLUDE FILES: stdio.h string.h sysLib.h memLib.h cacheLib.h vmLib.h vmLibP.h
               intLib.h  taskLib.h vxLib.h tyLib.h mmuE500Lib.h vxPpcLib.h
               qeCp.h

SEE ALSO:
\tb "PowerPC e500 Core Family Reference Manual",
\tb "MPC8569E PowerQUICC III Integrated Processor Reference Manual",
\tb "MPC8569E-MDS-PB HW User Guide",
\tb "MPC8569E-MDS-PB Hardware Getting Started Guide",
\tb "MPC8569E-MDS-PB Schematics".
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <sysLib.h>
#include <memLib.h>
#include <cacheLib.h>
#include <vmLib.h>
#include <private/vmLibP.h>
#include <intLib.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <arch/ppc/mmuE500Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include "config.h"
#include <../src/hwif/h/resource/qeCp.h>

/* globals */

/* effAddr, realAddrExt, realAddr, ts | size | permissions | attributes */

TLB_ENTRY_DESC sysStaticTlbDesc [] = {
    {
    FLASH_BASE_ADRS_TLB, 0, FLASH_BASE_ADRS_TLB,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

    {
    LOCAL_MEM_LOCAL_ADRS, 0, LOCAL_MEM_LOCAL_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1G | _MMU_TLB_IPROT |
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
    }

#ifdef DRV_PCIBUS_M85XX
    ,
    {
    PCIEX_MEM_ADRS, 0, PCIEX_MEM_ADRS,
    _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    }
#endif /* DRV_PCIBUS_M85XX */

#ifdef INCLUDE_L2_SRAM
    ,
    {
    L2SRAM_ADDR, 0, L2SRAM_ADDR,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },
#endif /* INCLUDE_L2_SRAM */
};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

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
 *        .                   .
 *        .                   .
 *        .                   .
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

PHYS_MEM_DESC sysPhysMemDesc [] = {
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
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
    CCSR_SIZE,
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
    }

#ifdef DRV_PCIBUS_M85XX
    ,
    {
    (VIRT_ADDR) PCIEX_MEM_ADRS,
    (PHYS_ADDR) PCIEX_MEM_ADRS,
    PCIEX_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX_MEMIO_ADRS,
    PCIEX_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX_IO_ADRS,
    (PHYS_ADDR) PCIEX_IO_ADRS,
    PCIEX_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    }
#endif /* DRV_PCIBUS_M85XX */

#ifdef INCLUDE_L2_SRAM
    ,
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    L2SRAM_WINDOW_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    }
#endif /* INCLUDE_L2_SRAM */
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* Clock Ratio Tables */

#define MAX_VALUE_PLAT_RATIO    12
UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO][2] = {
    { 0,  0 }, { 0, 0 }, { 2,  0 }, { 3, 0 },
    { 4,  0 }, { 5, 0 }, { 6,  0 }, { 7, 0 },
    { 8,  0 }, { 9, 0 }, { 10, 0 }, { 0, 0 },
};

#define MAX_VALUE_E500_RATIO    10
UINT32 e500RatioTable[MAX_VALUE_PLAT_RATIO][2] = {
    { 0, 0 }, { 0, 0 }, { 1, 0 }, { 3, 1 }, { 2, 0 },
    { 5, 1 }, { 3, 0 }, { 7, 1 }, { 4, 0 }, { 9, 1 }
};

int     sysBus          = BUS_TYPE_NONE;    /* system bus type (VME_BUS, etc) */
int     sysCpu          = CPU;              /* system CPU type                */
char *  sysBootLine     = BOOT_LINE_ADRS;   /* address of boot line           */
char *  sysExcMsg       = EXC_MSG_ADRS;     /* catastrophic message area      */
int     sysProcNum      = 0;                /* processor number of this CPU   */

UINT32  coreFreq;
UINT32  sysTimerClkFreq;
UINT32  inFullVxWorksImage      = FALSE;

UINT32  ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;

/* imports */

IMPORT void mmuE500TlbStaticInvalidate (void);
IMPORT void mmuE500TlbDynamicInvalidate (void);
IMPORT void mmuE500TlbStaticInit (int, TLB_ENTRY_DESC *, BOOL);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;

IMPORT void sysL1Csr1Set (UINT32);
IMPORT void (*_vxb_usDelayRtn) (int);
IMPORT void (*_vxb_msDelayRtn) (int);

IMPORT void hardWareInterFaceInit (void);

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif /* INCLUDE_SIO_UTILS */

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP ();
IMPORT void vxDisableBP ();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_SPE
#   include <speLib.h>
IMPORT int (* _func_speProbeRtn) ();
#endif /* INCLUDE_SPE */

/* locals */

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL void sysL1CacheQuery (void);
#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_L2_SRAM
LOCAL void sysL2SramEnable (BOOL both);
#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_UCC_VXB_END
LOCAL void   qeIOPortInitEth(void);
LOCAL void   sysUccPhyEnable(void);
#else
LOCAL void   sysUccPhyDisable(void);
#endif

/* forward declarations */

UINT32 sysClkFreqGet (void);
UINT32 sysQeFreqGet (void);
void sysUsDelay (int);
void sysMsDelay (UINT);
void sysDelay (void);

#ifdef INCLUDE_LED_DEBUG
void sysLedOn (int, BOOL);
void sysLedSet (UINT8);
#endif /* INCLUDE_LED_DEBUG */

#ifdef INCLUDE_SPE
int sysSpeProbe (void);
#endif /* INCLUDE_SPE */

/* L2 Cache */

#include "sysL2Cache.c"

/* I2C */

#ifdef INCLUDE_I2C
#   include "sysMotI2c.c"
#   include "sysMpc85xxI2c.c"

/* RTC */

#   ifdef INCLUDE_RTC
#       include "ds1374.c"
#   endif /* INCLUDE_RTC */

#endif /* INCLUDE_I2C */

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

char * sysModel (void)
    {
    switch (vxSvrGet() & SVR_MPC8569_MASK)
        {
        case SVR_MPC8569:
            return (SYS_MODEL_8569);
        case SVR_MPC8569E:
            return (SYS_MODEL_8569E);
        default:
            return (SYS_MODEL_8569);
        }
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
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the MDS8569 board. It sets up
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
    vxEnableBP ();
#endif /* INCLUDE_BRANCH_PREDICTION */

    /* disable the FLASH write protection */

    *BCSR17 |= BCSR17_FLASH_WP;

#ifdef INCLUDE_UCC_VXB_END
    sysUccPhyEnable();
#else
    sysUccPhyDisable();
#endif

#ifdef DRV_SIO_UCC

    /* enable QE UART */

    *BCSR15 |= BCSR15_QE_UART_EN;

#else

    /* disable QE UART */

    *BCSR15 &= ~BCSR15_QE_UART_EN;

#endif /* DRV_SIO_UCC */

    /* disable L1 Icache */

    vxL1CSR1Set (vxL1CSR1Get () & ~0x1);

    /* enable machine check pin */

    vxHid0Set (HID0_MCP | vxHid0Get ());

    /*
     * enable time base for delay use before DEC interrupt is setup and select
     * the processor clock as the base of time base clock (TB is updated every
     * 8 core complex bus (CCB) clocks).
     */

    vxHid0Set ((vxHid0Get () | _PPC_HID0_TBEN) & (~HID0_SEL_TBCLK));

    /* clock div is 8 */

    sysTimerClkFreq = sysClkFreqGet () >> 3;

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery ();
#endif /* INCLUDE_CACHE_SUPPORT */

    /* initialise L2CTL register */

    vxL2CTLSet (L2CTL_INIT_VAL, M85XX_L2CTL(CCSBAR));

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */

    mmuE500TlbDynamicInvalidate ();
    mmuE500TlbStaticInvalidate ();
    mmuE500TlbStaticInit (sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

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

#   if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
        mmuPpcIEnabled = TRUE;
#   endif /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxHid1Set (HID1_ABE);
    sysL2CacheInit ();
#endif /* (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT)) */

    /* config eSDHC pins */

#ifdef DRV_STORAGE_SDHC
    *(volatile UINT32 *)GENCFGR |= SDHC_WP_INV;

    /* select the pin's eSDHC function */

    *(volatile UINT32 *)PLPPAR1 |= 0x00000a80;

    /* config the pin's direction */

    *(volatile UINT32 *)PLPDIR1 |= 0x00000fc0;

    *BCSR6 |= (BCSR6_SD_CARD_1BIT_EN | BCSR6_SD_CARD_4BIT_EN);
#endif /* DRV_STORAGE_SDHC */

    /* Enabel machine check via RXFE and address bus streaming mode */

    vxHid1Set(vxHid1Get()| HID1_ASTME | HID1_RXFE);
    WRS_ASM("isync");

    /* set UCC1 as the master of the serial management interface */

    *QE_CMXGCR &= (~QE_CMXGCR_MEM_MASK);

#ifdef INCLUDE_UCC_VXB_END
    qeIOPortInitEth();
#endif

#ifdef DRV_SIO_UCC

    /* connect BRG9 to UCC2 Rx/Tx */

     *QE_CMXUCR3 &= (~QE_CMXUCR_HIGH_MASK);
     *QE_CMXUCR3 |= (((1 << QE_CMXUCR_RUCS_SHIFT) | (1 << QE_CMXUCR_TUCS_SHIFT)) <<
                     QE_CMXUCR_SHIFT);
#endif  /* DRV_SIO_UCC */

    /* hook the vxbus timer delay routines */

    _vxb_usDelayRtn = &sysUsDelay;
    _vxb_msDelayRtn = &sysMsDelay;

    /* initialize HWIF Pre-Kernel */

    hardWareInterFaceInit ();

#ifdef INCLUDE_I2C
    i2cDrvInit (0, 0);
#endif /* INCLUDE_I2C */

#ifdef INCLUDE_L2_SRAM
#   if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
        sysL2SramEnable (TRUE);
#   elif (defined(INCLUDE_L2_SRAM))
        sysL2SramEnable (FALSE);
#   endif /* (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT)) */
#endif /* INCLUDE_L2_SRAM */

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

void sysHwInit2 (void)
    {

    vxbDevInit ();

#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll ();
#endif /* INCLUDE_SIO_UTILS */

#ifdef INCLUDE_SPE
    _func_speProbeRtn = sysSpeProbe;
#endif /* INCLUDE_SPE */

    taskSpawn ("tDevConn", 11, 0, 10000, vxbDevConnect, 0,1,2,3,4,5,6,7,8,9);
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
#endif /* INCLUDE_EDR_PM */
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
    FUNCPTR pRom = (FUNCPTR) ROM_WARM_ADRS;    /* Warm reboot */

    intLock ();

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    sysClkDisable();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif /* INCLUDE_AUX_CLK */

#if (defined(INCLUDE_MMU_BASIC) && \
     (defined(USER_D_MMU_ENABLE) || defined(USER_D_MMU_ENABLE)))
    VM_ENABLE (FALSE);
#endif

#ifdef  INCLUDE_CACHE_SUPPORT

#   ifdef  USER_D_CACHE_ENABLE
        cacheDisable(_DATA_CACHE);
#   endif /* USER_D_CACHE_ENABLE */

#   ifdef  USER_I_CACHE_ENABLE
        cacheDisable(_INSTRUCTION_CACHE);
#   endif /* USER_I_CACHE_ENABLE */

#endif /* INCLUDE_CACHE_SUPPORT */

    vxMsrSet(0);

    /* jump to bootrom entry point */

    (*pRom) (startType);

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
* sysQeFreqGet - return QUICC Engine clock freq
*
* This function returns the QUICC Engine clock freq.
*
* RETURNS: QUICC Engine clock freq
*
* ERRNO: N/A
*/

UINT32 sysQeFreqGet(void)
    {
    return (M85XX_PORPLLSR_QE_RATIO(CCSBAR) * OSCILLATOR_FREQ);
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
    int  actualRollover = 0;
    int  calcRollover   = 0;
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
    sysUsDelay ((UINT32) delay * 1000);
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
    sysMsDelay (1);
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

#endif /* INCLUDE_SPE */

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

    temp = vxL1CFG0Get ();

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

    *(M85XX_L2SRBAR0(CCSBAR)) = (UINT32) (L2SRAM_ADDR & M85XX_L2SRBAR_ADDR_MSK);

    /* Get present value */

    l2CtlVal = vxL2CTLGet (M85XX_L2CTL(CCSBAR));

    /* Disable L2CTL initially to allow changing of block size */

    l2CtlVal &= ~M85XX_L2CTL_L2E_MSK;
    vxL2CTLSet (l2CtlVal, M85XX_L2CTL(CCSBAR));
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

    vxL2CTLSet (l2CtlVal, M85XX_L2CTL(CCSBAR));

    if (both == FALSE)
        {
        /* This is done here so L2SRAM is set before enable */

        l2CtlVal |= M85XX_L2CTL_L2E_MSK; /* No cache so go ahead and enable */

        /* Enable L2CTL for SRAM */

        vxL2CTLSet (l2CtlVal, M85XX_L2CTL(CCSBAR));
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
* qeIOPortInitEth - Init values to establish correct IO Port configuration
*
* This routine sets up the pin connections for the functionality required by
* BSP. The setting value is obtained from the chip user maunual and the
* board schematics.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void qeIOPortInitEth(void)
    {
    /* configure the clock route of the UCC */

    *QE_CMXUCR1 = QE_CMXUCR1_UCC1_TX_CLK12 | QE_CMXUCR1_UCC1_RX_DIS |
                  QE_CMXUCR1_UCC3_TX_CLK12 | QE_CMXUCR1_UCC3_RX_DIS;
    *QE_CMXUCR3 = QE_CMXUCR1_UCC2_TX_CLK17 | QE_CMXUCR1_UCC2_RX_DIS |
		          QE_CMXUCR1_UCC4_TX_CLK17 | QE_CMXUCR1_UCC4_RX_DIS;
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
    *BCSR7 |= 0x80 | 0x40;
    *BCSR8 |= 0x80 | 0x40;
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
    *BCSR7 &= ~0x80;
    *BCSR8 &= ~0x80;
    }

#endif /* INCLUDE_UCC_VXB_END */

/*******************************************************************************
*
* sysGetPeripheralBase - provide CCSRBAR for security engine drivers
*
* This routine provides CCSRBAR address for security engine drivers.
*
* RETURNS: CCSBAR
*
* ERRNO
*/

UINT32 sysGetPeripheralBase(void)
    {
    return(CCSBAR);
    }

#ifdef DRV_PCIBUS_M85XX

LOCAL UCHAR sysPciExIntRoute [NUM_PCIEX_SLOTS][4] = {
    {PCIEX_XINT1_LVL, PCIEX_XINT2_LVL, PCIEX_XINT3_LVL, PCIEX_XINT4_LVL}
};

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
    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
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
    return sysPciExIntRoute [pLoc->device & (NUM_PCIEX_SLOTS - 1)][pin - 1];
    }

#endif /* DRV_PCIBUS_M85XX */

#ifdef INCLUDE_LED_DEBUG

/*******************************************************************************
*
* sysLedOn - control the LEDs
*
* This routine controls the LEDs on the MDS8569 board to be ON or OFF.
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
        *BCSR11 |= (color << 4);
    else
        *BCSR11 &= ~(color << 4);
    }

/*******************************************************************************
*
* sysLedSet - set LED by number
*
* This routine light the BCSR LEDs by the number.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysLedSet
    (
    UINT8 num
    )
    {
    switch(num)
        {
        case 1:
            sysLedOn (BCSR11_LED_GREEN, 0);
            sysLedOn (BCSR11_LED_YELLOW, 0);
            sysLedOn (BCSR11_LED_RED, 1);
            return;
        case 2:
            sysLedOn (BCSR11_LED_GREEN, 0);
            sysLedOn (BCSR11_LED_YELLOW, 1);
            sysLedOn (BCSR11_LED_RED, 0);
            return;
        case 3:
            sysLedOn (BCSR11_LED_GREEN, 0);
            sysLedOn (BCSR11_LED_YELLOW, 1);
            sysLedOn (BCSR11_LED_RED, 1);
            return;
        case 4:
            sysLedOn (BCSR11_LED_GREEN, 1);
            sysLedOn (BCSR11_LED_YELLOW, 0);
            sysLedOn (BCSR11_LED_RED, 0);
            return;
        case 5:
            sysLedOn (BCSR11_LED_GREEN, 1);
            sysLedOn (BCSR11_LED_YELLOW, 0);
            sysLedOn (BCSR11_LED_RED, 1);
            return;
        case 6:
            sysLedOn (BCSR11_LED_GREEN, 1);
            sysLedOn (BCSR11_LED_YELLOW, 1);
            sysLedOn (BCSR11_LED_RED, 0);
            return;
        case 7:
            sysLedOn (BCSR11_LED_GREEN, 1);
            sysLedOn (BCSR11_LED_YELLOW, 1);
            sysLedOn (BCSR11_LED_RED, 1);
            return;
        default:
            sysLedOn (BCSR11_LED_GREEN, 0);
            sysLedOn (BCSR11_LED_YELLOW, 0);
            sysLedOn (BCSR11_LED_RED, 0);
            return;
        }
    }

#endif /* INCLUDE_LED_DEBUG */

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
*/

UINT32 sysSdhcClkFreqGet (void)
    {
    return (sysClkFreqGet () / 2);
    }
    
#endif /* DRV_STORAGE_SDHC */

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
    VUINT32 val;
    UINT8   lawIndex;

    val = vxPvrGet ();
    printf ("Core:      E500V2, %d.%d, (%.8x)\n", ((val >> 4) & 0xf),
            (val & 0xf), val);

    val = vxSvrGet ();
    printf ("CPU:       %s, %d.%d, (%.8x))\n", sysModel (),
            ((val >> 4) & 0xf), (val & 0xf), val);

    val = vxMsrGet ();
    printf ("MSR:       0x%x\n", val);
    printf ("           UCLE-%x SPE-%x WE-%x CE-%x EE-%x PR-%x ME-%x\n",
            xbit32(val, 37), xbit32(val, 38), xbit32(val, 45), xbit32(val, 46),
            xbit32(val, 48), xbit32(val, 49), xbit32(val, 51));
    printf ("           UBLE-%x DE-%x IS-%x DS-%x PMM-%x\n",
            xbit32(val, 53), xbit32(val, 54), xbit32(val, 58), xbit32(val, 59),
            xbit32(val, 61));

    printf ("HID0:      0x%x\nHID1:      0x%x\n", vxHid0Get (), vxHid1Get ());

    val  = vxL1CSR0Get ();
    printf ("L1CSR0:    cache is %s - 0x%x\n", val & 1 ? "ON" : "OFF", val);
    val  = vxL1CSR1Get ();
    printf ("L1CSR1:    Icache is %s - 0x%x\n", val & 1 ? "ON" : "OFF", val);
    printf ("L1CFG0:    0x%x\nL1CFG1:    0x%x\n",
            vxL1CFG0Get (), vxL1CFG1Get ());

    val  = *M85XX_L2CTL(CCSBAR);
    printf ("L2CTL:     0x%x\n", val);
    printf ("           l2 is %s\n", val & 0x80000000 ? "ON" : "OFF");
    printf ("           l2siz-%x l2blksz-%x l2do-%x l2io-%x\n",
            (xbit0(val, 2) << 1) | xbit0(val, 3),
            (xbit0(val, 4) << 1) | xbit0(val, 5),
             xbit0(val, 9), xbit0(val, 10));
    printf ("           l2pmextdis-%x l2intdis-%x l2sram-%x\n",
           xbit0(val, 11), xbit0(val, 12),
           (xbit0(val, 13) << 2) | (xbit0(val, 14) << 1) | xbit0(val, 15));
    printf ("L2SRBAR0:  0x%x\n", *M85XX_L2SRBAR0(CCSBAR));
    printf ("L2SRBAR1:  0x%x\n", *M85XX_L2SRBAR1(CCSBAR));

    printf ("Core Freq: %3d Hz\n", coreFreq);
    printf ("CCB Freq:  %3d Hz\n", sysClkFreqGet ());
    printf ("QE Freq:   %3d Hz\n",
            M85XX_PORPLLSR_QE_RATIO(CCSBAR) * OSCILLATOR_FREQ);
    printf ("DDR Freq:  %3d Hz\n",
            M85XX_PORPLLSR_DDR_RATIO(CCSBAR) * OSCILLATOR_FREQ / 2);
    printf ("LBC Freq:  %3d Hz\n",
            sysClkFreqGet () /
            ((*M85XX_LCRR(CCSBAR)) & M85XX_LCRR_CLKDIV_MASK));
    printf ("PCI Freq:  %3d Hz\n",OSCILLATOR_FREQ);

    for (lawIndex = 0; lawIndex < M85XX_LAW_NUM; lawIndex++)
        {
    printf ("LAWBAR%d:   0x%08x\t LAWAR%d: 0x%08x\n",
               lawIndex, *M85XX_LAWBARn(CCSBAR, lawIndex),
               lawIndex, *M85XX_LAWARn(CCSBAR, lawIndex));
        }
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
    UINT8 csIndex;

    for (csIndex = 0; csIndex < M85XX_CS_NUM; csIndex++)
        {
        printf("Local Bus BR%d: 0x%08x\tOR%d: 0x%08x\n",
               csIndex, *M85XX_BRn(CCSBAR, csIndex),
               csIndex, *M85XX_ORn(CCSBAR, csIndex));
        }
    }

#endif /* INCLUDE_SHOW_ROUTINES */


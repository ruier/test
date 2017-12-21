/* sysLib.c - Freescale MPC8536DS board system-dependent library */

/*
 * Copyright (c) 2008, 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01d,10mar11,my_  rename DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC.
01c,02jul10,b_m  fix SDHC_WP inversion on silicon rev 1.1
01b,05jun10,sye  add support to eSDHC
01a,20oct08,b_m  derived from version 01h of fsl_mds8568/sysLib.c
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

/* globals */

/* effAddr, realAddrExt, realAddr, ts | size | permissions | attributes */

TLB_ENTRY_DESC sysStaticTlbDesc [] = {
    {
    FLASH_BASE_ADRS - FLASH_SIZE, 0, FLASH_BASE_ADRS - FLASH_SIZE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_IPROT |
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
    CCSR_BASE, 0, CCSR_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

    {
    PIXIS_BASE, 0, PIXIS_BASE,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64K | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

#ifdef DRV_PCIBUS_M85XX

    {
    PCI_MEM_ADRS, 0, PCI_MEM_ADRS,
    _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

    {
    PCIEX1_MEM_ADRS, 0, PCIEX1_MEM_ADRS,
    _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

    {
    PCIEX2_MEM_ADRS, 0, PCIEX2_MEM_ADRS,
    _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

    {
    PCIEX3_MEM_ADRS, 0, PCIEX3_MEM_ADRS,
    _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },

#endif /* DRV_PCIBUS_M85XX */

#ifdef INCLUDE_L2_SRAM
    {
    L2SRAM_ADDR, 0, L2SRAM_ADDR,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    },
#endif /* INCLUDE_L2_SRAM */
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
    LOCAL_MEM_SIZE,
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
    (VIRT_ADDR) CCSR_BASE,
    (PHYS_ADDR) CCSR_BASE,
    CCSR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PIXIS_BASE,
    (PHYS_ADDR) PIXIS_BASE,
    PIXIS_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    },

#ifdef DRV_PCIBUS_M85XX

    {
    (VIRT_ADDR) PCI_MEM_ADRS,
    (PHYS_ADDR) PCI_MEM_ADRS,
    PCI_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCI_MEMIO_ADRS,
    (PHYS_ADDR) PCI_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCI_IO_ADRS,
    (PHYS_ADDR) PCI_IO_ADRS,
    PCI_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX1_MEM_ADRS,
    (PHYS_ADDR) PCIEX1_MEM_ADRS,
    PCI_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX1_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX1_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX1_IO_ADRS,
    (PHYS_ADDR) PCIEX1_IO_ADRS,
    PCI_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX2_MEM_ADRS,
    (PHYS_ADDR) PCIEX2_MEM_ADRS,
    PCI_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX2_IO_ADRS,
    (PHYS_ADDR) PCIEX2_IO_ADRS,
    PCI_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) PCIEX3_MEM_ADRS,
    (PHYS_ADDR) PCIEX3_MEM_ADRS,
    PCI_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX3_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX3_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },
    {
    (VIRT_ADDR) PCIEX3_IO_ADRS,
    (PHYS_ADDR) PCIEX3_IO_ADRS,
    PCI_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

#endif /* DRV_PCIBUS_M85XX */

#ifdef INCLUDE_L2_SRAM
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    L2SRAM_WINDOW_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    },
#endif /* INCLUDE_L2_SRAM */
};

int sysPhysMemDescNumEnt = NELEMENTS(sysPhysMemDesc);

int     sysBus      = BUS_TYPE_NONE;    /* system bus type */
int     sysCpu      = CPU;              /* system CPU type */
char *  sysBootLine = BOOT_LINE_ADRS;   /* address of boot line */
char *  sysExcMsg   = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum  = 0;                /* processor number of this CPU */

UINT32  sysTimerClkFreq = OSCILLATOR_FREQ;
UINT32  inFullVxWorksImage = FALSE;

UINT32  ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;

/* externs */

IMPORT void mmuE500TlbStaticInvalidate (void);
IMPORT void mmuE500TlbDynamicInvalidate (void);
IMPORT void mmuE500TlbStaticInit (int, TLB_ENTRY_DESC *, BOOL);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;

IMPORT void sysL1Csr1Set (UINT32);
IMPORT UINT sysTimeBaseLGet (void);

IMPORT void hardWareInterFaceInit (void);
#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP (void);
IMPORT void vxDisableBP (void);
#endif

#ifdef INCLUDE_SPE
#include <speLib.h>
IMPORT int (* _func_speProbeRtn)();
#endif

/* forward declarations */

UINT32 sysCcbClkFreqGet (void);
void   sysUsDelay (UINT32);
void   sysMsDelay (UINT);
void   sysDelay (void);

#ifdef INCLUDE_SPE
int sysSpeProbe (void);
#endif

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL void sysL1CacheQuery (void);
#endif

#ifdef INCLUDE_L2_SRAM
LOCAL void sysL2SramEnable (BOOL both);
#endif

/* L2 Cache */

#include "sysL2Cache.c"

/* I2C */

#ifdef INCLUDE_I2C
#   include "sysMotI2c.c"
#   include "sysMpc85xxI2c.c"
#   include "ds1339Rtc.c"
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

char * sysModel (void)
    {
    UINT32 svr = *(volatile UINT32 *)M85XX_SVR;

    switch (svr)
        {
        case SVR_MPC8536E:
            return SYS_MODEL_SEC;
        case SVR_MPC8536:
        default:
            return SYS_MODEL;
        }
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version with the revision
*
* This routine returns a pointer to a BSP version with the revision.
* for eg. 2.0/<x>.  BSP_REV defined in config.h is concatenated to
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
* This routine initializes various features of the hardware.
* Normally, it is called from usrInit() in usrConfig.c.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit (void)
    {
#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP ();
#endif

    /* disable L1 Icache */

    sysL1Csr1Set (vxL1CSR1Get() & ~0x1);

    /* enable machine check pin */

    vxHid0Set (HID0_MCP | vxHid0Get());

    /* enable time base for delay use before DEC interrupt is setup */

    vxHid0Set (vxHid0Get() | _PPC_HID0_TBEN);

    /* clock div is 8 */

    sysTimerClkFreq = sysCcbClkFreqGet () >> 3;

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery ();
#endif

    /* initialise L2CTL register */

    vxL2CTLSet (0x28000000, M85XX_L2CTL(CCSR_BASE));

    /* setup static TLB entries for bootrom or any non-MMU enabled images */

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

#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    mmuPpcIEnabled = TRUE;
#endif /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxHid1Set (HID1_ABE);
    sysL2CacheInit ();
#endif

    /* config eSDHC pins */
    
#ifdef DRV_STORAGE_SDHC
    *(volatile UINT32 *)GENCFGR |= SDHC_WP_INV;

    *(volatile UINT32 *)PMUXCR &= ~SDHC_DATA;
    *(volatile UINT32 *)PMUXCR |= (SDHC_CD | SDHC_WP);
#endif /* DRV_STORAGE_SDHC */

    /* machine check via RXFE for RIO */

    vxHid1Set (vxHid1Get()| HID1_ASTME | HID1_RXFE);

    hardWareInterFaceInit ();

#ifdef INCLUDE_I2C
    i2cDrvInit (0, 0);
    i2cDrvInit (1, 0);
#endif

#ifdef INCLUDE_L2_SRAM
#   if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
        sysL2SramEnable (TRUE);
#   elif (defined(INCLUDE_L2_SRAM))
        sysL2SramEnable (FALSE);
#   endif
#endif

    CACHE_PIPE_FLUSH ();
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
#endif

#ifdef INCLUDE_SPE
    _func_speProbeRtn = sysSpeProbe;
#endif

    taskSpawn ("tDevConn", 11, 0, 10000, vxbDevConnect, 0,1,2,3,4,5,6,7,8,9);
    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Normally, the user specifies the amount of physical memory with the
* macro LOCAL_MEM_SIZE in config.h.  BSPs that support run-time
* memory sizing do so only if the macro LOCAL_MEM_AUTOSIZE is defined.
* If not defined, then LOCAL_MEM_SIZE is assumed to be, and must be, the
* true size of physical memory.
*
* NOTE: Do no adjust LOCAL_MEM_SIZE to reserve memory for application
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

char * sysMemTop (void)
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
* This routine transfers control to the ROM monitor.  It is usually called
* only by reboot() -- which services ^X -- and bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a new
* <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*
* ERRNO: N/A
*/

STATUS sysToMonitor
    (
    int startType
    )
    {
    FUNCPTR pRom = (FUNCPTR)(ROM_TEXT_ADRS + 4);

    intLock ();

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP ();
#endif

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable (INSTRUCTION_CACHE);
    cacheDisable (DATA_CACHE);
#endif

    sysClkDisable ();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable ();
#endif

    vxMsrSet (0);

    /* clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate ();
    mmuE500TlbStaticInvalidate ();

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
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return sysProcNum;
    }

/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* Set the processor number for the CPU board.  Processor numbers should be
* unique on a single backplane.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int procNum
    )
    {
    sysProcNum = procNum;
    }

/* Clock Ratio Tables */

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

/* avaliable oscillator frequeny */

LOCAL UINT32 sysOscFreqTable [] =
    {
    FREQ_33_MHZ,
    FREQ_40_MHZ,
    FREQ_50_MHZ,
    FREQ_66_MHZ,
    FREQ_83_MHZ,
    FREQ_100_MHZ,
    FREQ_133_MHZ,
    FREQ_166_MHZ,
    };

/*******************************************************************************
*
* sysOscClkFreqGet - get the oscillator clock.
*
* This routine gets the oscillator clock freq.
*
* RETURNS: oscillator clock
*
* ERRNO: N/A
*/

UINT32 sysOscClkFreqGet (void)
    {
    UINT8 osc = *(volatile UINT8 *)PIXIS_SPD;
    return sysOscFreqTable [osc & 7];
    }

/*******************************************************************************
*
* sysCcbClkFreqGet - get the CCB clock.
*
* This routine gets the CCB clock freq.
*
* RETURNS: CCB clock
*
* ERRNO: N/A
*/

UINT32 sysCcbClkFreqGet (void)
    {
    UINT32 pll, platRatio;
    UINT32 oscFreq, ccbFreq;

    pll = *(volatile UINT32 *)PORPLLSR;
    platRatio = ((pll & PORPLLSR_PLAT_RATIO_MASK) >> 1);

    oscFreq = sysOscClkFreqGet ();

    if ((platRatio > MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio][0] == 0))
        return CCB_FREQ_DEFAULT;

    ccbFreq = (oscFreq * platRatioTable[platRatio][0])
              >> platRatioTable[platRatio][1];

    return ccbFreq;
    }

/*******************************************************************************
*
* sysCoreClkFreqGet - get the core clock.
*
* This routine gets the core clock freq.
*
* RETURNS: core clock
*
* ERRNO: N/A
*/

UINT32 sysCoreClkFreqGet (void)
    {
    UINT32 pll, e500Ratio;
    UINT32 oscFreq, ccbFreq, coreFreq;

    pll = *(volatile UINT32 *)PORPLLSR;
    e500Ratio = ((pll & PORPLLSR_E500_RATIO_MASK) >> 16);

    oscFreq = sysOscClkFreqGet ();
    ccbFreq = sysCcbClkFreqGet ();

    coreFreq = (ccbFreq * e500RatioTable[e500Ratio][0])
               >> e500RatioTable[e500Ratio][1];

    return coreFreq;
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

    baselineTickCount = sysTimeBaseLGet ();

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

        curTickCount = sysTimeBaseLGet ();

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
    sysUsDelay ((UINT32)delay * 1000);
    }

/*******************************************************************************
*
* sysDelay - delay a fixed 1 millisecond.
*
* This routine consumes 1ms of time.
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
    strcpy ((char *)EXC_MSG_OFFSET, errorMsg);
    }

/*******************************************************************************
*
* chipErrataCpu29Print - print information of chip errata cpu 29
*
* The message will be displayed upon rebooting with a bootrom.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void chipErrataCpu29Print (void)
    {
    saveExcMsg ("Silicon fault detected, possible machine state corruption.\n"
                "System rebooted to limit damage.\n");
    }

#ifdef INCLUDE_SPE

/*******************************************************************************
*
* sysSpeProbe - check if the CPU has SPE unit.
*
* This routine returns OK it the CPU has an SPE unit in it.
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

#endif

#ifdef INCLUDE_CACHE_SUPPORT

/*******************************************************************************
*
* sysL1CacheQuery - configure L1 cache size and alignment
*
* This routine populates L1 cache size and alignment from
* configuration registers.
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

    /* setup Windows for L2SRAM */

    *(M85XX_L2SRBAR0(CCSR_BASE)) = (UINT32)(L2SRAM_ADDR & M85XX_L2SRBAR_ADDR_MSK);

    /* get present value */

    l2CtlVal = vxL2CTLGet (M85XX_L2CTL(CCSR_BASE));

    /* disable L2CTL initially to allow changing of block size */

    l2CtlVal &= ~M85XX_L2CTL_L2E_MSK;
    vxL2CTLSet (l2CtlVal, M85XX_L2CTL(CCSR_BASE));
    l2CtlVal &= ~M85XX_L2CTL_L2BLKSIZ_MSK;
    l2CtlVal &= ~M85XX_L2CTL_L2SRAM_MSK;

    if (both == TRUE)
        {
        /* setup size of SRAM */

        l2CtlVal |= (L2SIZ_256KB << M85XX_L2CTL_L2BLKSIZ_BIT) |
                    (0x2 << M85XX_L2CTL_L2SRAM_BIT);
        }
    else
        {
        l2CtlVal |= (L2SIZ_512KB << M85XX_L2CTL_L2BLKSIZ_BIT) |
                    (0x1 << M85XX_L2CTL_L2SRAM_BIT);
        }

    /* setup L2CTL for SRAM */

    vxL2CTLSet (l2CtlVal, M85XX_L2CTL(CCSR_BASE));

    if (both == FALSE)
        {
        /* This is done here so L2SRAM is set before enable */

        l2CtlVal |= M85XX_L2CTL_L2E_MSK; /* No cache so go ahead and enable */

        /* Enable L2CTL for SRAM */

        vxL2CTLSet (l2CtlVal, M85XX_L2CTL(CCSR_BASE));
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

#ifdef DRV_PCIBUS_M85XX

LOCAL UCHAR sysPciEx1IntRoute [NUM_PCIEX_SLOTS][4] =
    { PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL };

LOCAL UCHAR sysPciEx2IntRoute [NUM_PCIEX_SLOTS][4] =
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL };

LOCAL UCHAR sysPciEx3IntRoute [NUM_PCIEX_SLOTS][4] =
    { PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL };

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] =
    { PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL };

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

/*******************************************************************************
*
* sysPciAutoConfigInclude - PCI autoconfig support routine
*
* This routine performs the PCI auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciAutoConfigInclude
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
* sysPciAutoConfigIntAssign - PCI autoconfig support routine
*
* This routine peforms the PCI auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO: N/A
*/

UCHAR sysPciAutoConfigIntAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciIntRoute[0][pin - 1];
    }

#endif /* DRV_PCIBUS_M85XX */

#ifdef DRV_TIMER_OPENPIC

/*******************************************************************************
*
* sysOpenPicTimerClkFreqGet - get the openpic timer clock.
*
* This routine gets the openpic timer clock freq.
*
* RETURNS: openpic timer clock
*
* ERRNO: N/A
*/

UINT32 sysOpenPicTimerClkFreqGet
    (
    UINT32  picClockDivisor     /* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
        return (sysCcbClkFreqGet() / picClockDivisor);
    else
        return RTC_FREQ;
    }

#endif /* DRV_TIMER_OPENPIC */

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
* This routine returns the clock frequency of eSDHC controller. 
* 
* RETURNS: clock frequency of eSDHC controller
*
* ERRNO: N/A
*/

UINT32 sysSdhcClkFreqGet (void)
    {
    return (sysCcbClkFreqGet () / 2);
    }
    
#endif /* DRV_STORAGE_SDHC */


/*******************************************************************************
*
* sysGetPeripheralBase - get the CCSR base address for security engine driver
*
* This routine returns the CCSR base address for security engine driver.
*
* RETURNS: base address of CCSR
*
* ERRNO: N/A
*/

UINT32 sysGetPeripheralBase (void)
    {
    return (CCSR_BASE);
    }

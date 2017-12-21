/* sysLib.c - Freescale cds85xx board system-dependent library */

/*
 * Copyright (c) 2002-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02n,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
02m,21dec10,swu  Modify CCSBAR mmu table size.(WIND00247711)
02l,12jan10,pch  fix compiler warning re sysSerialConnectAll()
02k,09dec09,pch  CQ195058: adjust static TLB setup based on LOCAL_MEM_SIZE
02j,23feb09,x_z  add M command support, remove sysEnetAddrGet() and fix compile
                 warning.(WIND00046372)
02i,31oct08,h_k  fixed a compile error at sysHwInit() when
                 INCLUDE_BRANCH_PREDICTION and INCLUDE_CPM are defined.
02h,30oct08,x_z  add vxEnableBP to sysHwInit and replace disableBranchPrediction
                 with vxDisableBP.(WIND00127193)
02g,07aug08,fao  add END2 support
02f,04jun08,mdo  CQ:WIND00106094 - Provide sysGetPeripheralBase for security
                 engine support
02e,20sep07,pmr  fix apigen error
02d,22aug07,pdg  added vxBus m85xxPci support
02c,21aug07,mmi  remove vxPowrModeSet() call since deprecated
02b,16aug07,wap  Switch to new VxBus TSEC driver, remove FCC support
                 (cds8541/cds8555 boards have no FCC ports) (WIND00101678)
02a,08aug07,agf  remove duplicate variable definitions
01z,10jul07,p_g  removed cmdLine.c inclusion
01y,15jun07,pmr  fix 16550 SIO include
01x,25jan07,ami  Ported to vxBus
01w,13dec06,y_w  WIND00061260: modify sysModel() to use SYS_MODEL.
01v,24mar05,mdo  Documentation fixes for apigen
01u,22mar05,dtr  Include Tsec configlette.
01t,22nov04,jln  set ASTME bit for cache
01s,21nov04,jln  fixed compiler warning
01r,02sep04,jln  modified from ads85xx/01q
01q,30jul04,dtr  Fixing sysMsDelay, added saveExcMsg.
01p,22jul04,dtr  Disable ICACHE after boot up for initialisation.
01o,31mar04,dtr  Some minor mods. Install parity handler.
                 Branch prediction support.
01n,20nov03,dtr  Setting HID1 ABE bit by default .
01m,03nov03,dtr  Minor fixes..
01l,27oct03,dtr  Use GNU_TOOL instead of __GCC__.
01k,21oct03,dtr  Remove EUMBAR read/write.
01j,17oct03,dtr  Modifying Pvr lkup.
01i,29sep03,dtr  Set default aux clock setting if included.
01h,09sep03,dtr  Clear L2 control register prior to Enable.
01g,20aug03,dtr  Adding in TSEC configlette file.
                 Adding new FCC2 End driver support.
                 Test for bootrom then initialise static TLB entries.
                 Adding dynamic config of L1 cache.
01f,04aug03,dtr  Adding in support for LBC SDRAM.
01e,29jul03,dtr  Removing CCSBAR magic number,support for MMU 'off' and
                 support for SNOOP when MMU 'off'.
01d,24jul03,mil  Added storing of flash params.
01c,19jun03,mil  Changed ROM TLB entry to writable, added SCC and FCC.
01b,07jan03,dtr  Adding TLB static table entries.
01a,13oct02,dtr  More complete file for further development.
*/

/*
DESCRIPTION
This library provides board-specific routines for ADS85XX.

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/* includes */

#include <vxWorks.h>
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

#ifdef INCLUDE_CPM
    #include "m8560CpmIntrCtl.h"
#endif

#ifdef INCLUDE_VXBUS
   #include "hwif/vxbus/vxBus.h"
   #include "hwconf.c"
  #ifdef DRV_SIO_NS16550
    #include "../src/hwif/h/sio/vxbNs16550Sio.h"
    LOCAL void sysVxbSerialInit (void);
  #endif
IMPORT  void    hardWareInterFaceInit (void);
#endif /* INCLUDE_VXBUS */

#ifdef	INCLUDE_SIO_UTILS
IMPORT	void	sysSerialConnectAll (void);
#endif	/* INCLUDE_SIO_UTILS */

/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash */

    /* needed be first entry here */
    { FLASH1_BASE_ADRS, 0x0, FLASH1_BASE_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_16M |
        _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
        _MMU_TLB_ATTR_G
    },
    /*
     * LOCAL MEMORY needed be second entry here  -
     * E500 TLB entries are limited to 256MB each, so the number needed
     * to map the SDRAM depends on LOCAL_MEM_SIZE.  Only the first is
     * defined here; the rest are at the end of sysStaticTlbDesc[].
     */
    { 0x00000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
        _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
        CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },

#ifdef INCLUDE_LBC_SDRAM
    /* needed be second entry here */
    { LOCAL_MEM_LOCAL_ADRS2, 0x0, LOCAL_MEM_LOCAL_ADRS2, _MMU_TLB_TS_0 |
        _MMU_TLB_SZ_64M | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X |
        CAM_DRAM_CACHE_MODE | _MMU_TLB_ATTR_M | _MMU_TLB_IPROT
    },
#endif /* LBC_SDRAM */

    { CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
    { CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_1 | _MMU_TLB_SZ_1M |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

#ifdef INCLUDE_L2_SRAM
    ,
    { L2SRAM_ADDR, 0x0, L2SRAM_ADDR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
        _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
        _MMU_TLB_ATTR_G
    }

#endif /* INCLUDE_L2_SRAM */

    /* 16 MB of LBC CS 3 area */
    , {
        LBC_CS3_LOCAL_ADRS, 0x0, LBC_CS3_LOCAL_ADRS,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M | _MMU_TLB_IPROT |
        _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_ATTR_M
    }

    /* Assume PCI space contiguous and within 256MB */
#ifdef INCLUDE_PCI_BUS
    ,
    { PCI_MEM_ADRS, 0x0, PCI_MEM_ADRS, _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
#ifdef INCLUDE_CDS85XX_SECONDARY_PCI
    ,
    { PCI_MEM_ADRS2, 0x0, PCI_MEM_ADRS2, _MMU_TLB_TS_0 |  PCI_MMU_TLB_SZ |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
#endif /* INCLUDE_CDS85XX_SECONDARY_PCI */

#endif  /* INCLUDE_PCI */

    /*
     * LOCAL MEMORY above 256MB
     *
     * This is handled at the end of sysStaticTlbDesc so that, if we
     * run out of TLB1 entries, we lose only mapping of high-addressed
     * SDRAM rather than something critical.
     *
     * Since the cds85xx board has only one DIMM slot, we assume here
     * that the installed DDR SDRAM size will be a power of two.
     */

#if	(LOCAL_MEM_SIZE > MMUE500_MAX_STATIC_TLB_SIZE)
    ,
    { MMUE500_MAX_STATIC_TLB_SIZE, 0x0, MMUE500_MAX_STATIC_TLB_SIZE,
	_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
	_MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
	CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    }
#endif	/* LOCAL_MEM_SIZE > MMUE500_MAX_STATIC_TLB_SIZE */

#if	(LOCAL_MEM_SIZE > 2 * MMUE500_MAX_STATIC_TLB_SIZE)
    ,
    { 2 * MMUE500_MAX_STATIC_TLB_SIZE, 0x0, 2 * MMUE500_MAX_STATIC_TLB_SIZE,
	_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
	_MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
	CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },
    { 3 * MMUE500_MAX_STATIC_TLB_SIZE, 0x0, 3 * MMUE500_MAX_STATIC_TLB_SIZE,
	_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
	_MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
	CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    }
#endif	/* LOCAL_MEM_SIZE > 2 * MMUE500_MAX_STATIC_TLB_SIZE */

#if	(LOCAL_MEM_SIZE > 4 * MMUE500_MAX_STATIC_TLB_SIZE)
    ,
    { 4 * MMUE500_MAX_STATIC_TLB_SIZE, 0x0, 4 * MMUE500_MAX_STATIC_TLB_SIZE,
	_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
	_MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
	CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },
    { 5 * MMUE500_MAX_STATIC_TLB_SIZE, 0x0, 5 * MMUE500_MAX_STATIC_TLB_SIZE,
	_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
	_MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
	CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },
    { 6 * MMUE500_MAX_STATIC_TLB_SIZE, 0x0, 6 * MMUE500_MAX_STATIC_TLB_SIZE,
	_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
	_MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
	CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },
    { 7 * MMUE500_MAX_STATIC_TLB_SIZE, 0x0, 7 * MMUE500_MAX_STATIC_TLB_SIZE,
	_MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
	_MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
	CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    }
#endif	/* LOCAL_MEM_SIZE > 4 * MMUE500_MAX_STATIC_TLB_SIZE */

};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);


/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, the Board Control and
 * Status registers, the MPC8260 Internal Memory Map, and local FLASH RAM are set here.
 *
 * PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
 * integer powers of two based on amount of memory to be mapped and a
 * minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
 * for 32-bit PowerPCs are:
 *
 * Total mapped memory		Page Table size
 * -------------------		---------------
 *        8 Meg			     64 K
 *       16 Meg			    128 K
 *       32 Meg			    256 K
 *       64 Meg			    512 K
 *      128 Meg			      1 Meg
 * 	.				.
 * 	.				.
 * 	.				.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 *
 */
PHYS_MEM_DESC sysPhysMemDesc [] =
{

    {
        /* Vector Table and Interrupt Stack */

	(VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
	(PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
	LOCAL_MEM_SIZE,
	VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
	VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    }
#ifdef	INCLUDE_LBC_SDRAM
    ,
    {
        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS2,
        (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS2,
        LOCAL_MEM_SIZE2,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY ,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_LBC_SDRAM */

#ifdef INCLUDE_L2_SRAM
    ,
    {
        (VIRT_ADDR) L2SRAM_ADDR,
        (PHYS_ADDR) L2SRAM_ADDR,
        256 * 1024,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    }
#endif

#ifdef INCLUDE_LBC_CS3
    ,{
        (VIRT_ADDR) LBC_CS3_LOCAL_ADRS,
        (PHYS_ADDR) LBC_CS3_LOCAL_ADRS,
        16 * 0x100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_LBC_CS3 */

#ifdef INCLUDE_PCI_BUS
    ,
    {
        (VIRT_ADDR) PCI_MEM_ADRS,
        (PHYS_ADDR) PCI_MEM_ADRS,
        PCI_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCI_MEMIO_ADRS,
        (PHYS_ADDR) PCI_MEMIO_ADRS,
        PCI_MEMIO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCI_IO_ADRS,
        (PHYS_ADDR) PCI_IO_ADRS,
        PCI_IO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }

#ifdef INCLUDE_CDS85XX_SECONDARY_PCI
    ,
    {
        (VIRT_ADDR) PCI_MEM_ADRS2,
        (PHYS_ADDR) PCI_MEM_ADRS2,
        PCI_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCI_MEMIO_ADRS2,
        (PHYS_ADDR) PCI_MEMIO_ADRS2,
        PCI_MEMIO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCI_IO_ADRS2,
        (PHYS_ADDR) PCI_IO_ADRS2,
        PCI_IO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_CDS85XX_SECONDARY_PCI */
#endif /* INCLUDE_PCI_BUS */
    ,
    {
        (VIRT_ADDR) FLASH1_BASE_ADRS,
        (PHYS_ADDR) FLASH1_BASE_ADRS,
        16 * 1024 *1024,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* Clock Ratio Tables */
#define MAX_VALUE_PLAT_RATIO 32
UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
    { 0, 0},
    { 0, 0},
    { 2, 0},
    { 3, 0},
    { 4, 0},
    { 5, 0},
    { 6, 0},
    { 7, 0},
    { 8, 0},
    { 9, 0},
    { 10, 0},
    { 0, 0},
    { 12, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 16, 0},
    { 0, 0}
};
#define MAX_VALUE_E500_RATIO 8
UINT32 e500RatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 2, 0},
    { 5, 1},
    { 3, 0},
    { 7, 1}
};

int   sysBus      = BUS;                /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (PPC8260) */
char *sysBootLine = BOOT_LINE_ADRS; /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;   /* catastrophic message area */
int   sysProcNum;           /* processor number of this CPU */
BOOL  sysVmeEnable = FALSE;     /* by default no VME */
UINT32  coreFreq;

IMPORT void     mmuE500TlbDynamicInvalidate();
IMPORT void     mmuE500TlbStaticInvalidate();
IMPORT void mmuE500TlbStaticInit (int numDescs,
                                  TLB_ENTRY_DESC *pTlbDesc,
                                  BOOL cacheAllow);
IMPORT BOOL     mmuPpcIEnabled;
IMPORT BOOL     mmuPpcDEnabled;

/* forward declarations */

void   sysUsDelay (UINT32);

/* 8260 Reset Configuration Table (From page 9-2 in Rev0 of 8260 book) */
#define END_OF_TABLE 0

UINT32 sysClkFreqGet(void);
UINT32 ppcE500ICACHE_LINE_NUM = (128 * 12);
UINT32 ppcE500DCACHE_LINE_NUM = (128 * 12);

UINT32 ppcE500CACHE_ALIGN_SIZE = 32;

#include "m85xxTimer.c"

#ifdef INCLUDE_NV_RAM
    #include "mem/byteNvRam.c"      /* Generic NVRAM routines */
#endif /* INCLUDE_NV_RAM */

#ifdef INCLUDE_CPM
UINT32 vxImmrGet();
    #include "mem/m82xxDpramLib.c"
#endif

UINT32 inFullVxWorksImage=FALSE;

#define EXT_VEC_IRQ0            56
#define EXT_NUM_IRQ0            EXT_VEC_IRQ0
#define EXT_MAX_IRQS            200

STATUS  sysIntEnablePIC          (int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable      (int intNum);
STATUS  sysCascadeIntDisable     (int intNum);
void    flashTest(VUINT16 *address,VUINT16 *buffer,VINT32 length);

UINT32   baudRateGenClk;

#include "sysEpic.c"

#include "sysL2Cache.c"

/* bootrom 'M' command support */

#include "sysNet.c"

#ifdef INCLUDE_CPM
void m85xxCpmInt(void);
#endif

#ifdef INCLUDE_CPM
    #include "m8560CpmIntrCtl.c"
#endif

#ifdef INCLUDE_TFFS
    #include "am29lv64xdMtd.c"
#endif /* INCLUDE_TFFS */

#define WB_MAX_IRQS 256

/* defines */

#define ZERO    0
#define SYS_MODEL_8541    "Freescale CDS MPC8541"
#define SYS_MODEL_8555    "Freescale CDS MPC8555"
#define SYS_MODEL_E500    "Motorola E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN "Motorola Unknown processor"

#define DELTA(a,b)                 (abs((int)a - (int)b))
#define HID0_MCP 0x80000000
#define HID1_ABE 0x00001000
#define HID1_ASTME 0x00002000

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void     vxEnableBP();
IMPORT void     vxDisableBP();
#endif

#ifdef INCLUDE_L2_SRAM
LOCAL void sysL2SramEnable(BOOL both);
#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_SPE
    #include "speLib.h"
IMPORT int       (* _func_speProbeRtn) () ;
#endif /* INCLUDE_SPE */

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL void sysL1CacheQuery();
#endif

UINT32 sysTimerClkFreq = OSCILLATOR_FREQ;

IMPORT  void    sysL1Csr1Set(UINT32);
IMPORT  UINT    sysTimeBaseLGet(void);

#ifdef DRV_SIO_NS16550
LOCAL void sysVxbSerialInit (void);
#endif


#if     defined (INCLUDE_SPE)

/*******************************************************************************
*
* sysSpeProbe - Check if the CPU has SPE unit
*
* This routine returns OK if the CPU has an SPE unit in it.
* Presently it assumes available.
*
* RETURNS: OK
*
* ERRNO
*/

int  sysSpeProbe (void)
    {
    ULONG regVal;
    int speUnitPresent = OK;

    /* The CPU type is indicated in the Processor Version Register (PVR) */

    regVal = 0;

    switch (regVal)
        {
        case 0:
        default:
            speUnitPresent = OK;
            break;
        }      /* switch  */

    return(speUnitPresent);
    }
#endif  /* INCLUDE_SPE */


/******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*
* ERRNO
*/

char * sysModel (void)
    {
    UINT device;

    device = *M85XX_SVR(CCSBAR);

    if ((device & 0xffff0000) == 0x80790000)
        return(SYS_MODEL " CDS MPC8555");

    if ((device & 0xffff0000) == 0x807a0000)
        return(SYS_MODEL " CDS MPC8541");

    device = *M85XX_PVR(CCSBAR);
    if ((device & 0xffff0000) == 0x80200000)
        return(SYS_MODEL " E500 : Unknown system version");

    return("Unknown processor");

    }

/******************************************************************************
*
* sysBspRev - return the BSP version with the revision eg 1.0/<x>
*
* This function returns a pointer to a BSP version with the revision.
* for eg. 1.0/<x>. BSP_REV defined in config.h is concatenated to
* BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO
*/

char * sysBspRev (void)
    {
    return(BSP_VERSION BSP_REV);
    }

UINT32 sysClkFreqGet
(
void
)
    {
    UINT32  sysClkFreq;
    UINT32 e500Ratio,platRatio;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);

#ifdef FORCE_DEFAULT_FREQ
    return(DEFAULT_SYSCLKFREQ);
#endif

    if ((platRatio>MAX_VALUE_PLAT_RATIO)||(platRatioTable[platRatio][0]==0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

    sysClkFreq = ((UINT32)(OSCILLATOR_FREQ * platRatioTable[platRatio][0]))>>((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0]))>>((UINT32)e500RatioTable[e500Ratio][1]);


    return(sysClkFreq);
    }

/******************************************************************************
*
* sysCpmFreqGet - Determines the CPM Operating Frequency
*
* This routine determines the CPM Operating Frequency.
*
* From page 9-2 Rev. 0  MPC8260  PowerQUICC II User's Manual
*
* RETURNS: CPM clock frequency for the current MOD_CK and MOD_CK_H settings
*
* ERRNO
*/

UINT32 sysCpmFreqGet (void)
    {
    UINT32 sysClkFreq = sysClkFreqGet();
    return(sysClkFreq);

    }

/******************************************************************************
*
* sysBaudClkFreq - Obtains frequency of the BRG_CLK in Hz
*
* From page 9-5 in Rev. 0 MPC8260 PowerQUICC II User's Manual
*
*     baud clock = 2*cpm_freq/2^2*(DFBRG+1) where DFBRG = 01
*                = 2*cpm_freq/16
*
* RETURNS: frequency of the BRG_CLK in Hz
*
* ERRNO
*/

UINT32 sysBaudClkFreq (void)
    {
    UINT32 cpmFreq = sysCpmFreqGet();

    return cpmFreq*2/16;
    }

/******************************************************************************
*
* sysHwMemInit - initialize and configure system memory.
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
* ERRNO
*/

void sysHwMemInit (void)
    {
    /* Call sysPhysMemTop() to do memory autosizing if available */

    sysPhysMemTop ();

    }

/******************************************************************************
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
* ERRNO
*/

void sysHwInit (void)
    {
#ifdef INCLUDE_CPM
    UINT32 immrVal;
#endif /* INCLUDE_CPM */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CPM
    immrVal = vxImmrGet();
    memset((char*)(CCSBAR+0x80000),(int)0,0x1000);
    m82xxDpramLibInit();


    /* Zero IO Port registers */
    /* reset the parallel ports */

    * M8260_IOP_PADIR(immrVal) = 0x00000000;
    * M8260_IOP_PAPAR(immrVal) = 0x00000000;
    * M8260_IOP_PASO(immrVal) = 0x00000000;
    * M8260_IOP_PAODR(immrVal) = 0x00000000;
    * M8260_IOP_PADAT(immrVal) = 0x00000000;

    * M8260_IOP_PBDIR(immrVal) = 0x00000000;
    * M8260_IOP_PBPAR(immrVal) = 0x00000000;
    * M8260_IOP_PBSO(immrVal) = 0x00000000;
    * M8260_IOP_PBODR(immrVal) = 0x00000000;
    * M8260_IOP_PBDAT(immrVal) = 0x00000000;

    * M8260_IOP_PCDIR(immrVal) = 0x00000000;
    * M8260_IOP_PCPAR(immrVal) = 0x00000000;
    * M8260_IOP_PCSO(immrVal) = 0x00000000;
    * M8260_IOP_PCODR(immrVal) = 0x00000000;
    * M8260_IOP_PCDAT(immrVal) = 0x00000000;

    * M8260_IOP_PDDIR(immrVal) = 0x00000000;
    * M8260_IOP_PDPAR(immrVal) = 0x00000000;
    * M8260_IOP_PDSO(immrVal) = 0x00000000;
    * M8260_IOP_PDODR(immrVal) = 0x00000000;
    * M8260_IOP_PDDAT(immrVal) = 0x00000000;

#endif /* INCLUDE_CPM */

    /* Disable L1 Icache */
    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* Enable machine check pin */
    vxHid0Set(HID0_MCP|vxHid0Get());

    /* enable time base for delay use before DEC interrupt is setup */
    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

    sysTimerClkFreq = sysClkFreqGet()>>3 /* Clock div is 8 */;

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif /* INCLUDE_CACHE_SUPPORT */

    /* Initialize L2CTL register */
    vxL2CTLSet(0x28000000,M85XX_L2CTL(CCSBAR));

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */

    mmuE500TlbDynamicInvalidate();

    /* Truncate sysStaticTlbDesc[] if too large */

    if (sysStaticTlbDescNumEnt > MMUE500_NUM_STATIC_TLB_ENTIRES)
	sysStaticTlbDescNumEnt = MMUE500_NUM_STATIC_TLB_ENTIRES;

    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (!defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL))
    mmuPpcIEnabled=TRUE;
    mmuPpcDEnabled=TRUE;
#else /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */
    if (inFullVxWorksImage==FALSE)
        {
        mmuPpcIEnabled=TRUE;
        mmuPpcDEnabled=TRUE;
        }
    /* Enable I Cache if instruction mmu disabled */
#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    mmuPpcIEnabled=TRUE;
#endif /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

    /* set DPPC in SIUMCR to 10 so that timer is enabled (TBEN) */

    /* reset the Communications Processor */
#ifdef INCLUDE_CPM
    *M8260_CPCR(CCSBAR + 0x80000) = 0x80010000;
    baudRateGenClk = sysBaudClkFreq();
#endif

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxHid1Set(HID1_ABE); /* Address Broadcast enable */
    sysL2CacheInit();
#endif /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    vxHid1Set(vxHid1Get()| HID1_ASTME); /* Address Stream Enable */

    /* enable the flash window */

    *M85XX_LAWBAR3(CCSBAR) = FLASH1_BASE_ADRS >> LAWBAR_ADRS_SHIFT;
    *M85XX_LAWAR3(CCSBAR)  = LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_8MB;

    WRS_ASM("isync");

    /* Initialize the Embedded Programmable Interrupt Controller */

    sysEpicInit();

#ifdef  INCLUDE_VXBUS

    /* Call the routine to do the pre-kernel vxBus specific Initializaiton */

    hardWareInterFaceInit();

#endif /* INCLUDE_VXBUS */


#if defined (DRV_SIO_NS16550)

    /* local the routine to do the BSP specific serial bus intiialization */

    sysVxbSerialInit ();

#endif

/* FIXME: this should be cleaned up */
#if defined(INCLUDE_L2_SRAM)
#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2SramEnable(TRUE);
#elif (defined(INCLUDE_L2_SRAM))
    sysL2SramEnable(FALSE);
#endif
#endif

    CACHE_PIPE_FLUSH();

    }

#ifdef INCLUDE_L2_SRAM
/*************************************************************************
*
* sysL2SramEnable - enables L2SRAM if L2SRAM only
*
* This routine enables L2SRAM if L2SRAM only or initializes blk
* size etc and leaves the rest to L2 cache code.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void sysL2SramEnable
(
BOOL both
)
    {
    volatile int l2CtlVal;

    /* if INCLUDE_L2_CACHE and CACHE_SUPPORT */
    /* if ((L2_SRAM_SIZE + L2_CACHE_SIZE) > l2Siz) */
    /* Setup Windows for L2SRAM */
    *(M85XX_L2SRBAR0(CCSBAR)) = (UINT32)(L2SRAM_ADDR & M85XX_L2SRBAR_ADDR_MSK);
    /* Get present value */
    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));

    /* Disable L2CTL initially to allow changing of block size */
    l2CtlVal&=(~M85XX_L2CTL_L2E_MSK);
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    l2CtlVal &= ~M85XX_L2CTL_L2BLKSIZ_MSK;
    l2CtlVal &= ~M85XX_L2CTL_L2SRAM_MSK;

    if (both == TRUE)
        {
        /* Setup size of SRAM */
        l2CtlVal |= (L2SIZ_128KB << M85XX_L2CTL_L2BLKSIZ_BIT) |
                    (0x2 << M85XX_L2CTL_L2SRAM_BIT);
        }
    else
        {
        l2CtlVal |= (L2SIZ_256KB << M85XX_L2CTL_L2BLKSIZ_BIT) |
                    (0x1 << M85XX_L2CTL_L2SRAM_BIT);
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

/**************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* RETURNS: The address of the top of physical memory.
*
* ERRNO
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    LOCAL char * physTop = NULL;

    if (physTop == NULL)
        {
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
        }

    return(physTop) ;
    }

/***************************************************************************
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
* ERRNO
*/

char * sysMemTop (void)
    {
    LOCAL char * memTop = NULL;

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

/**************************************************************************
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
* ERRNO
*/

STATUS sysToMonitor
    (
    int startType   /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);   /* Warm reboot */

    intLock();

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#endif
    sysClkDisable();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif

    vxMsrSet (0);                     /* Let bootrom re-initialize */

    (*pRom) (startType);    /* jump to bootrom entry point */

    return(OK);    /* in case we ever continue from ROM monitor */
    }

/******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysHwInit2 (void)
    {

    excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_DECR,
                   (VOIDFUNCPTR) sysClkInt);

    sysClkEnable();

#ifdef INCLUDE_AUX_CLK
    excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_FIT, (VOIDFUNCPTR) sysAuxClkInt);
#endif

    /* initialize the EPIC interrupts */
    sysEpicIntrInit ();

#ifdef INCLUDE_VXBUS

    vxbDevInit();

#endif



#ifdef INCLUDE_CPM
    sysCpmHwInit2();
#endif

    /* initialize serial interrupts */
#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll ();
#endif

#if defined(INCLUDE_DUART)
    sysSerialHwInit2 ();
#endif /* INCLUDE_DUART */

#if     defined (INCLUDE_SPE)
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */

#ifdef INCLUDE_VXBUS

    /* call the vxbDevConnect to do the third stage device initialization */

    taskSpawn("tvxbDevConn", 11, 0, 10000,
              vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

#endif /* INCLUDE_VXBUS */

    }

/******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board.
*
* ERRNO
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return(sysProcNum);
    }

/******************************************************************************
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
* ERRNO
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
* sysLocalToBusAdrs - convert a local address to a bus address
*
* This routine gets the VMEbus address that accesses a specified local
* memory address.
*
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* ERRNO
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
    return(ERROR);
    }

/******************************************************************************
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
* ERRNO
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
    return(ERROR);
    }

/******************************************************************************
*
* sysBusTas - test and set a location across the bus
*
* This routine does an atomic test-and-set operation across the backplane.
*
* Not applicable for the 8260Ads.
*
* RETURNS: FALSE, always.
*
* ERRNO
*
* SEE ALSO: vxTas()
*/

BOOL sysBusTas
    (
    char *  adrs        /* address to be tested-and-set */
    )
    {
    return(FALSE);
    }

/******************************************************************************
*
* sysBusClearTas - test and clear
*
* This routine is a null function.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysBusClearTas
    (
    volatile char * address /* address to be tested-and-cleared */
    )
    {
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
* ERRNO
*/

void sysUsDelay
    (
    UINT32    delay        /* length of time in microsec to delay */
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


void sysMsDelay
    (
    UINT      delay                   /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (UINT32) delay * 1000 );
    }


/*********************************************************************
*
* sysDelay - Fixed 1ms delay.
*
* This routine consumes 1ms of time. It just calls sysMsDelay.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

/***************************************************************************
*
* sysIntConnect - connect the BSP interrupt to the proper epic/i8259 handler.
*
* This routine checks the INT level and connects the proper routine.
* pciIntConnect() or intConnect().
*
* RETURNS:
* OK, or ERROR if the interrupt handler cannot be built.
*
* ERRNO
*/

STATUS sysIntConnect
    (
    VOIDFUNCPTR *vector,        /* interrupt vector to attach to     */
    VOIDFUNCPTR routine,        /* routine to be called              */
    int parameter               /* parameter to be passed to routine */
    )
    {
    int tmpStat = ERROR;
    UINT32 read;

    if (((int) vector < 0) || ((int) vector >= EXT_VEC_IRQ0 + EXT_MAX_IRQS))
        {
        logMsg ("Error in sysIntConnect: out of range vector = %d.\n",
                (int)vector,2,3,4,5,6);

        return(ERROR);
        }

    if (vxMemProbe ((char *) routine, VX_READ, 4, (char *) &read) != OK)
        {
        logMsg ("Error in sysIntConnect: Cannot access routine.\n",
                1,2,3,4,5,6);
        return(ERROR);
        }

    if ((int) vector < EXT_VEC_IRQ0)
        {
        tmpStat = intConnect (vector, routine, parameter);
        }
    else
        {
        /* call external int controller connect */
        /* tmpStat = cascadeIntConnect (vector, routine, parameter); */
        }

    if (tmpStat == ERROR)
        {
        logMsg ("Error in sysIntConnect: Connecting vector = %d.\n",
                (int)vector,2,3,4,5,6);
        }

    return(tmpStat);
    }

#if 0
/***************************************************************************
*
* sysCascadeIntConnect - connect an external controller interrupt
*
* This function call is used to connect an interrupt outside of the MPC8540
* PIC.
*
* RETURNS: OK or ERROR if unable to connect interrupt.
*
* ERRNO
*/

STATUS sysCascadeIntConnect
    (
    VOIDFUNCPTR *vector,        /* interrupt vector to attach to     */
    VOIDFUNCPTR routine,        /* routine to be called              */
    int parameter               /* parameter to be passed to routine */
    )
    {
    return(ERROR);
    }
#endif /* 0 */


/*******************************************************************************
*
* sysIntEnable - enable an interrupt
*
* This function call is used to enable an interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*
* ERRNO
*/

STATUS sysIntEnable
    (
    int intNum
    )
    {
    int tmpStat = ERROR;
    if (((int) intNum < 0) || ((int) intNum >= EXT_NUM_IRQ0 + EXT_MAX_IRQS))
        {
        logMsg ("Error in sysIntEnable: Out of range intNum = %d.\n",
                (int)intNum,2,3,4,5,6);

        return(ERROR);
        }

    if ((int) intNum < EXT_NUM_IRQ0)
        {
        tmpStat = intEnable (intNum);
        }
    else
        {
        /* call external int controller connect */
        tmpStat = sysCascadeIntEnable (intNum - EXT_NUM_IRQ0);
        }

    if (tmpStat == ERROR)
        {
        logMsg ("Error in sysIntEnable: intNum = %d.\n",
                (int)intNum,2,3,4,5,6);
        }

    return(tmpStat);
    }

/****************************************************************************
*
* sysCascadeIntEnable - enable an external controller interrupt
*
* This function call is used to enable an interrupt outside of the MPC8540 PIC.
*
* RETURNS: ERROR, always
*
* ERRNO
*/

STATUS sysCascadeIntEnable
    (
    int intNum
    )
    {
    return(ERROR);
    }

/****************************************************************************
*
* sysIntDisable - disable an interrupt
*
* This function call is used to disable an interrupt.
*
* RETURNS: OK or ERROR if unable to disable interrupt.
*
* ERRNO
*/

STATUS sysIntDisable
    (
    int intNum
    )
    {
    int tmpStat = ERROR;
    if (((int) intNum < 0) || ((int) intNum >= EXT_NUM_IRQ0 + EXT_MAX_IRQS))
        {
        logMsg ("Error in sysIntDisable: Out of range intNum = %d.\n",
                (int)intNum,2,3,4,5,6);

        return(ERROR);
        }

    if ((int) intNum < EXT_NUM_IRQ0)
        {
        tmpStat = intDisable (intNum);
        }
    else
        {
        /* call external int controller connect */
        tmpStat = sysCascadeIntDisable (intNum - EXT_NUM_IRQ0);
        }

    if (tmpStat == ERROR)
        {
        logMsg ("Error in sysIntDisable: intNum = %d.\n",
                (int)intNum,2,3,4,5,6);
        }

    return(tmpStat);
    }

/**************************************************************************
*
* sysCascadeIntDisable - disable an external controller interrupt
*
* This function call is used to disable an interrupt outside of the MPC8540 PIC.
*
* RETURNS: OK or ERROR if unable to disable interrupt.
*
* ERRNO
*/

STATUS sysCascadeIntDisable
    (
    int intNum
    )
    {
    return(ERROR);
    }

#ifdef INCLUDE_CACHE_SUPPORT
/***********************************************************************
*
* sysL1CacheQuery - configure L1 cache size and alignment
*
* Populates L1 cache size and alignment from configuration registers.
*
* RETURNS: N/A
*
* ERRNO
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
            ppcE500CACHE_ALIGN_SIZE=32;
            break;
        case 1:
            ppcE500CACHE_ALIGN_SIZE=64;
            break;
        default:
            ppcE500CACHE_ALIGN_SIZE=32;
            break;
        }

    ppcE500DCACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);
    ppcE500ICACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);

	/*
	 * The core manual suggests for a 32 byte cache line and 8 lines per set
	 *  we actually need 12 unique address loads to flush the set.
	 *  The number of lines to flush should be ( 3/2 * cache lines )
	 */
    ppcE500DCACHE_LINE_NUM = (3*ppcE500DCACHE_LINE_NUM)>>1;
    ppcE500ICACHE_LINE_NUM = (3*ppcE500ICACHE_LINE_NUM)>>1;

    }

#endif /* INCLUDE_CACHE_SUPPORT */

/***************************************************************************
*
* saveExcMsg - write exception message to save area for catastrophic error
*
* The message will be displayed upon rebooting with a bootrom.
*
* RETURNS: N/A
*
* ERRNO
*/

void saveExcMsg
    (
    char *errorMsg
    )
    {
    strcpy ((char *)EXC_MSG_OFFSET, errorMsg);
    }


void chipErrataCpu29Print(void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    }


/***************************************************************************
*
* vxImmrGet - get the CPM DPRAM base address
*
* This routine returns the CPM DP Ram base address for CPM device drivers.
*
* RETURNS: CPM DP Ram base address for CPM
*
* ERRNO
*/

UINT32 vxImmrGet(void)
    {
    return(CCSBAR + 0x80000);
    }

/******************************************************************************
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

#if defined (_GNU_TOOL)
void sysIntHandler (void)
    {
    }

void vxDecInt (void)
    {
    }

int excRtnTbl = 0;
#endif  /* _GNU_TOOL */


#ifdef INCLUDE_SHOW_ROUTINES

/***************************************************************************
*
* coreLbcShow - Show routine for local bus controller
*
* This routine shows the local bus controller registers.
*
* RETURNS: N/A
*
* ERRNO
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

/***************************************************************************
*
* coreShow - Show routine for core registers
*
* This routine shows the core registers.
*
* RETURNS: N/A
*
* ERRNO
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

    printf("Core Freq = %3d Hz\n",coreFreq);
    printf("CCB Freq = %3d Hz\n",sysClkFreqGet());
    printf("PCI Freq = %3d Hz\n",OSCILLATOR_FREQ);
    printf("CPM Freq = %3d Hz\n",sysClkFreqGet());

    }

#endif

#ifdef DRV_SIO_NS16550
/***************************************************************************
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
    int channel     /* serial channel */
    )
    {
    return ((SIO_CHAN *) ERROR);
    }

char  *eumbbar_base;                    /* eumbbar base address */

/******************************************************************************
*
* sysVxbSerialInit - intializes the DUARTs
*
* After initialization, the sysDuartHwInit() of the non-vxBus version of BSP
* changes the value of value of serail base registers depending on the
* status of chan.channelMode. This routine implements the same.
*
* RETURNS: N/A
*
* ERRNO: none
*/

LOCAL void sysVxbSerialInit (void)
    {
    NS16550VXB_CHAN * pNS16550Chan = NULL;

    eumbbar_base = (char *)CCSBAR;

    /* find the 0th channel */

    pNS16550Chan = (NS16550VXB_CHAN *)sysSerialChanGet (0);

    if (pNS16550Chan->channelMode == SIO_MODE_INT)
	{
        eumbbar_base[UDCR2] = 0x01;  /* set duart mode */
        eumbbar_base[ULCR2] = 0x80;  /* open DLAB */
        eumbbar_base[UAFR2] = 0x00;
        eumbbar_base[UDMB2] = 0x06;  /* MSB 9600bps @266Mhz */
	eumbbar_base[UDLB2] = 0xc8;  /* LSB */
	eumbbar_base[ULCR2] = 0x03;  /* clear DLAB, no-parity, 1stop bit, 8bit data */
	eumbbar_base[UMCR2] = 0x02;  /* disable loopback mode */
	eumbbar_base[UIER2] = 0x03;  /* Tx empty, Rx interrupt enable */
	}
    } /* sysVxbSerialInit () */


#endif /* DRV_SIO_NS16550 */

#ifdef INCLUDE_PCI_BUS

/* INT LINE TO IRQ assignment for MPC85xxADS-PCI board. */

LOCAL UCHAR sysPci0IntRoute [NUM_PCI1_SLOTS][4] = {
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL}, /* slot 1 */
    {PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL}, /* slot 2 */
    {PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL}, /* slot 3 */
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL}, /* slot 4 */
};

#ifdef INCLUDE_CDS85XX_SECONDARY_PCI

LOCAL UCHAR sysPci1IntRoute [4] =
    {PCI2_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL};

#endif /* INCLUDE_CDS85XX_SECONDARY_PCI */

/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI 0 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci0AutoconfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {

    /*
     * Only support BUS 0;
     * Host controller itself (device number is 0) won't be configured;
     * Bridge on the Arcadia board (device number 17) won't be configured;
     */

    if ((pLoc->bus > 0)                                               ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)  ||
        (pLoc->bus == 0 && pLoc->device == 17 && pLoc->function == 0) ||
        (devVend == PCI_ARCADIA_BRIDGE_DEV_ID))

        return(ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci0AutoconfigIntrAssign - PCI 0 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci0AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin > 0) && (pin < 5))       				&&
	(((pLoc->device) - PCI_SLOT1_DEVNO) < NUM_PCI1_SLOTS) 	&&
	(((pLoc->device) - PCI_SLOT1_DEVNO) >= 0))
	{
	tmpChar =
	    sysPci0IntRoute [((pLoc->device) - PCI_SLOT1_DEVNO)][(pin-1)];
	}

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

#ifdef INCLUDE_CDS85XX_SECONDARY_PCI
/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI 1 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {

    /*
     * Only support BUS 0;
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
        return(ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci1AutoconfigIntrAssign - PCI 1 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci1AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if ((pin > 0) && (pin < 5))	
	tmpChar = sysPci1IntRoute [(pin-1)];

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

#endif /* INCLUDE_CDS85XX_SECONDARY_PCI */

#endif /* INCLUDE_PCI_BUS */




/* sysLib.c - Wind River SBC548 board system-dependent library */

/*
 * Copyright (c) 2006-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,16apr09,x_z  add static TLB item for TFFS FLASH.(WIND00161740)
01j,30oct08,x_z  add vxEnableBP to sysHwInit and replace disableBranchPrediction
                 with vxDisableBP.(WIND00127193)
01i,24jun08,wap  Remove the limit on the number of allowed PCI buses
01h,06may08,x_f  remove static tlb entry when LOCAL_MEM_SIZE is more than 256M.
01g,20sep07,b_m  add ns16550, ppcIntCtlr, epic, m85xxTimer vxbus driver.
01f,10sep07,wap  Switch to VxBus PCI driver (WIND00104076)
01e,21aug07,mmi  remove vxPowerModeSet() since deprecated
01d,31jul07,agf  update vxBus cmdLineBuild
01c,25may07,b_m  modify static tlb table to enable boot device selection.
01b,03apr07,b_m  add TFFS map to static tlb table and phyMemDesc table.
01a,31jan06,kds  Modified from cds8458/sysLib.c/01c
*/

/*
 * DESCRIPTION
 * This library provides board-specific routines for SBC8548.
 *
 * INCLUDE FILES:
 *
 * SEE ALSO:
 * .pG "Configuration"
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
#include "bootAppShell.c"

#include "sysUsr.c"
#include "src/vxbEtsecEnd.c"
/*jingzhong*/
#include "rioLib/rioFsl.h"
#include "sysUsr.h"

#ifndef COMPILE_BOOTROM
#include "rioLib/rioApi.c"
#endif
/*end*/
#ifdef INCLUDE_TFFS
#include "wamdmtd.c"
#endif

/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /*
     * effAddr,  Unused,  realAddr, ts | size | attributes | permissions
     *
     * TLB #0.  Flash
     *
     * needed be first entry here
     */

    { 0xf0000000, 0x0, 0xf0000000, _MMU_TLB_TS_0 | BOOT_FLASH_TLB_SIZE |
        _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
        _MMU_TLB_ATTR_G
    },

    /* LOCAL MEMORY needed be second entry here and one TLB would be 256M */
    { 0x00000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
        _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
        CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },
    
    { 0x10000000, 0x0, 0x10000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
        _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M |
        CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    },

    { CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    }

#ifdef INCLUDE_LBC_SDRAM
    ,
    { LOCAL_MEM_LOCAL_ADRS2, 0x0, LOCAL_MEM_LOCAL_ADRS2, _MMU_TLB_TS_0 |
        _MMU_TLB_SZ_64M | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X |
        CAM_DRAM_CACHE_MODE | _MMU_TLB_ATTR_M | _MMU_TLB_IPROT
    }
#endif /* LBC_SDRAM */

#ifdef INCLUDE_L2_SRAM
    ,
    { L2SRAM_ADDR, 0x0, L2SRAM_ADDR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
        _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
        _MMU_TLB_ATTR_G
    }
#endif /* INCLUDE_L2_SRAM */

    /* 16 MB of LBC CS2 area */
    , 
    {
        0x90000000, 0x0, 0x90000000,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_16M | _MMU_TLB_IPROT |
        _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_ATTR_M
    }
#ifdef INCLUDE_RAPIDIO_BUS
    ,
    {
    RAPIDIO_ADRS, 0x0, RAPIDIO_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
    _MMU_TLB_ATTR_I | _MMU_TLB_PERM_W | _MMU_TLB_ATTR_G
    }
#endif

    /* Assume PCI space contiguous and within 256MB */
#ifdef INCLUDE_PCI_BUS
    /*,
    { PCI_MEM_ADRS, 0x0, PCI_MEM_ADRS, _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }*/

    ,
    { PCI_MEM_ADRS3, 0x0, PCI_MEM_ADRS3, _MMU_TLB_TS_0 |  PCI_MMU_TLB_SZ |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

#endif  /* INCLUDE_PCI_BUS */

#if 0
#ifdef INCLUDE_TFFS   
    ,
    { FLASH1_BASE_ADRS, 0x0, FLASH1_BASE_ADRS, _MMU_TLB_TS_0 | TFFS_FLASH_TLB_SIZE |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
#endif
#endif
};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#ifdef MMU_ASID_MAX     /* Base 6 MMU library in effect */

  /* macro to compose 64-bit PHYS_ADDRs */

# define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))
#endif

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
      /*
	 * Vector Table and Interrupt Stack
     * Must be sysPhysMemDesc [0] to allow adjustment in sysHwInit()
	 */

        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
        (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
        LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    }
    ,      
    {
      /*
    * CCSBAR
    */
        (VIRT_ADDR) CCSBAR,
        (PHYS_ADDR) CCSBAR,
        0x00100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
        VM_STATE_MASK_MEM_COHERENCY | VM_STATE_MASK_GUARDED,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT |
        VM_STATE_MEM_COHERENCY | VM_STATE_GUARDED
    }

    ,{
        (VIRT_ADDR) 0x90000000,
        (PHYS_ADDR) 0x90000000,
        16 * 0x100000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }

#ifdef INCLUDE_PCI_BUS
#if 0
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
#endif    
    /*,  
    {
        (VIRT_ADDR) PCI_MEM_ADRS3,
        (PHYS_ADDR) PCI_MEM_ADRS3,
        PCI_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }*/
    ,
    {
        (VIRT_ADDR) PCI_MEMIO_ADRS3,
        (PHYS_ADDR) PCI_MEMIO_ADRS3,
        0x40000000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    },
    {
        (VIRT_ADDR) 0x80000000,
        (PHYS_ADDR) 0x80000000,
        0x10000000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    /*,
    {
        (VIRT_ADDR) PCI_IO_ADRS3,
        (PHYS_ADDR) PCI_IO_ADRS3,
        PCI_IO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    } */  
#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_RAPIDIO_BUS
    ,
    {
    (VIRT_ADDR) RAPIDIO_ADRS,
    (PHYS_ADDR) RAPIDIO_ADRS,
    RAPIDIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE ,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    }
#endif /* INCLUDE_RAPIDIO_BUS */

	,
	{
		(VIRT_ADDR) (0xa0000000),
		(PHYS_ADDR) (0xa0000000),
		0x20000000,
		MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
		MMU_ATTR_VALID	   | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
		MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
	}
	,
	{
		(VIRT_ADDR) (0xc0000000),
		(PHYS_ADDR) (0xc0000000),
		0x10000000,
		MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
		MMU_ATTR_VALID	   | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
		MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
	}  
	,
    {
        (VIRT_ADDR) 0xF0000000,
        (PHYS_ADDR) 0xF0000000,
        0x10000000,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#if 0
    ,
    {
        (VIRT_ADDR) FLASH1_BASE_ADRS,
        (PHYS_ADDR) FLASH1_BASE_ADRS,
        FLASH1_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#endif
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
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 20, 0},
    { 0, 0}
};

#define MAX_VALUE_E500_RATIO 10
UINT32 e500RatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
    { 0, 0},
    { 0, 0},
    { 1, 0},
    { 3, 1},
    { 2, 0},
    { 5, 1},
    { 3, 0},
    { 7, 1},
    { 4, 0},
    { 9, 1}
};

UINT32   sysPciMode;
UINT32   sysPciSlotDeviceNumber;
int   sysBus      = BUS_TYPE_NONE;      /* system bus type (VME_BUS, etc) */
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
IMPORT void     sysIvprSet(UINT32);

/* forward declarations */

void   sysUsDelay (UINT32);
void   sysLedClkRoutine (int arg);

#ifdef INCLUDE_L1_IPARITY_HDLR_INBSP
    #define _EXC_OFF_L1_PARITY 0x1500
IMPORT void jumpIParity();
IMPORT void sysIvor1Set(UINT32);
UINT32 instrParityCount = 0;
#endif  /* INCLUDE_L1_IPARITY_HDLR_INBSP */


/* 8260 Reset Configuration Table (From page 9-2 in Rev0 of 8260 book) */

#define END_OF_TABLE 0

UINT32 sysClkFreqGet(void);
UINT32 ppcE500ICACHE_LINE_NUM = (128 * 12);
UINT32 ppcE500DCACHE_LINE_NUM = (128 * 12);

UINT32 ppcE500CACHE_ALIGN_SIZE = 32;

/*#include "sysMotI2c.c"
#include "sysMpc85xxI2c.c"*/

#ifdef INCLUDE_NV_RAM
    #include "eeprom.c"
    #include <mem/byteNvRam.c>      /* Generic NVRAM routines */
#else
/*#    include <mem/nullNvRam.c>*/
#endif /* INCLUDE_NV_RAM */


#ifdef INCLUDE_L1_IPARITY_HDLR
    #include "sysL1ICacheParity.c"
#endif

UINT32 inFullVxWorksImage=FALSE;

#define EXT_VEC_IRQ0            56
#define EXT_NUM_IRQ0            EXT_VEC_IRQ0
#define EXT_MAX_IRQS            200

STATUS  sysIntEnablePIC     (int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable      (int intNum);
STATUS  sysCascadeIntDisable     (int intNum);
void    flashTest(VUINT16 *address,VUINT16 *buffer,VINT32 length);

UINT32   baudRateGenClk;

#include "sysL2Cache.c"

#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif
#endif /* INCLUDE_VXBUS */

#define WB_MAX_IRQS 256

#ifdef INCLUDE_SYSLED
#  include "sysLed.c"
#endif /* INCLUDE_SYSLED */

/* defines */

#define ZERO    0

/* needed to enable timer base */

#ifdef INCLUDE_PCI_BUS
    #define      M8260_DPPC_MASK	0x0C000000 /* bits 4 and 5 */
    #define      M8260_DPPC_VALUE	0x0C000000 /* bits (4,5) should be (1,0) */
#else
    #define      M8260_DPPC_MASK	0x0C000000 /* bits 4 and 5 */
    #define      M8260_DPPC_VALUE	0x08000000 /* bits (4,5) should be (1,0) */
#endif /*INCLUDE_PCI_BUS */

#define DELTA(a,b)                 (abs((int)a - (int)b))
#define HID0_MCP 0x80000000
#define HID1_ABE 0x00001000
#define HID1_ASTME 0x00002000
#define HID1_RXFE  0x00020000


#ifdef INCLUDE_VXBUS
#include <hwif/vxbus/vxBus.h>
#include <../src/hwif/h/busCtlr/m85xxRio.h>
#endif

#ifdef INCLUDE_MOT_TSEC_END
#include "sysNet.c"
#endif /* INCLUDE_MOT_TSEC_END */

#ifdef INCLUDE_VXBUS
#include "hwconf.c"
#endif

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void     vxEnableBP();
IMPORT void     vxDisableBP();
#endif

#ifdef INCLUDE_L2_SRAM
LOCAL void sysL2SramEnable(BOOL both);
#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_SPE
    #include <speLib.h>
IMPORT int       (* _func_speProbeRtn) () ;
#endif /* INCLUDE_SPE */

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL void sysL1CacheQuery();
#endif

UINT32 sysTimerClkFreq = OSCILLATOR_FREQ;

IMPORT  void    sysL1Csr1Set(UINT32);
IMPORT  UINT    sysTimeBaseLGet(void);

LOCAL char * physTop = NULL;
LOCAL char * memTop = NULL;

#define INCLUDE_IFCONFIG
#define INCLUDE_NET_IF_SHOW
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_IPPING_CMD
#define INCLUDE_PING

#if     defined (INCLUDE_SPE)

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


/****************************************************************************
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
    UINT device;
    char* retChar = NULL;
    device = *M85XX_SVR(CCSBAR);

    switch(device & 0xffffff00)
	{
	case 0x80390000:
	    retChar = SYS_MODEL_8548E;
	    break;
	case 0x80310000:
	    retChar = SYS_MODEL_8548;
	    break;
	case 0x80390100:
	    retChar = SYS_MODEL_8547E;
	    break;
	case 0x80390200:
	    retChar = SYS_MODEL_8545E;
	    break;
	case 0x80310200:
	    retChar = SYS_MODEL_8545;
	    break;
	case 0x803A0000:
	    retChar = SYS_MODEL_8543E;
	    break;
	case 0x80320000:
	    retChar = SYS_MODEL_8543;
	    break;
	default:
	    retChar = SYS_MODEL_E500;
	    break;
	}


    device = *M85XX_PVR(CCSBAR);

    if ((device & 0xfff00000) != 0x80200000)
        retChar =SYS_MODEL_UNKNOWN;

    return(retChar);

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
* ERRNO: N/A
*/

char * sysBspRev (void)
    {
    return(BSP_VERSION BSP_REV);
    }

/******************************************************************************
*
* sysClkFreqGet - return Core Complex Bus clock freq
*
* This function returns the CCB clock freq.
*
* RETURNS: CCB clock freq
*
* ERRNO: N/A
*/

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
* ERRNO: N/A
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
* ERRNO: N/A
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
* ERRNO: N/A
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
* RETURNS: NA
*
* ERRNO: N/A
*/

void sysHwInit (void)
{
	UINT32 uiHost = 0;
     /*ns16550Pollprintf("enter \n");*/

    sysCpuLawInit();
#if 1
	uiHost = FSL_READ_REG(CCSBAR + SRIO_GCCSR);
    if(0x0 != (uiHost & (1 << 31)))
    {
        FSL_WRITE_REG((CCSBAR + 0xc0060),0xffffff);
    }
#endif
#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_RAPIDIO_BUS

    /* Errata not yet described - required for rapidIO TAS */
    *(UINT32*)(CCSBAR + 0x1010) = 0x01040004;
#endif

    sysIvprSet(0x0);

    /* Disable L1 Icache */

    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* Check for architecture support for 36 bit physical addressing */

#if defined(PPC_e500v2)
    vxHid0Set(_PPC_HID0_MAS7EN|vxHid0Get());
#endif

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP|vxHid0Get());

#ifdef E500_L1_PARITY_RECOVERY

    /* Enable Parity in L1 caches */

    vxL1CSR0Set(vxL1CSR0Get() | _PPC_L1CSR_CPE);
    vxL1CSR1Set(vxL1CSR1Get() | _PPC_L1CSR_CPE);
#endif  /* E500_L1_PARITY_RECOVERY */

    /* enable time base for delay use before DEC interrupt is setup */

    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

    sysTimerClkFreq = sysClkFreqGet()>>3 /* Clock div is 8 */;

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif /* INCLUDE_CACHE_SUPPORT */

    /* Initialise L2CTL register */

    vxL2CTLSet(0x28000000,M85XX_L2CTL(CCSBAR));

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */
    mmuE500TlbDynamicInvalidate();
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


#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxHid1Set(HID1_ABE); /* Address Broadcast enable */
    sysL2CacheInit();
#endif /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    /* Machine check via RXFE for RIO */

    vxHid1Set(vxHid1Get()| HID1_ASTME | HID1_RXFE); /* Address Stream Enable */

    /* enable the flash window */

    /**M85XX_LAWBAR3(CCSBAR) = FLASH1_BASE_ADRS >> LAWBAR_ADRS_SHIFT;
    *M85XX_LAWAR3(CCSBAR)  = LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_64MB;*/
    WRS_ASM("isync");
#if 0
#if (BOOT_FLASH == ON_BOARD_FLASH)
    *M85XX_BR6(CCSBAR) = 0xD0001801;
    *M85XX_OR6(CCSBAR) = 0xFC006E65;
#else
    *M85XX_BR6(CCSBAR) = 0xFB800801;
    *M85XX_OR6(CCSBAR) = 0xFF806E65;
#endif
#endif
    WRS_ASM("isync");

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */

#ifdef E500_L1_PARITY_RECOVERY
    vxIvor1Set(_EXC_OFF_L1_PARITY);
#endif  /* E500_L1_PARITY_RECOVERY */
#ifdef INCLUDE_L1_IPARITY_HDLR
    installL1ICacheParityErrorRecovery();
#endif /* INCLUDE_L1_IPARITY_HDLR */

#if defined(INCLUDE_L2_SRAM)
#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2SramEnable(TRUE);
#elif (defined(INCLUDE_L2_SRAM))
    sysL2SramEnable(FALSE);
#endif
#endif

    CACHE_PIPE_FLUSH();

    /*#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine,DEFAULT_BOOT_LINE,strlen(DEFAULT_BOOT_LINE)+1);
    #endif*/

    /*ns16550Pollprintf("exit\n");*/
    
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

    l2CtlVal &=(~M85XX_L2CTL_L2E_MSK);
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
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
* ERRNO: N/A
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

    vxMsrSet(0);

    /* Clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    (*pRom) (startType);    /* jump to bootrom entry point */

    return(OK);    /* in case we ever continue from ROM monitor */
}

SEM_ID semPrintSem;


VOID sysPrintInit(VOID)
{
    
    semPrintSem = semMCreate(SEM_INVERSION_SAFE | SEM_Q_PRIORITY);
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
* ERRNO: N/A
*/

void sysHwInit2 (void)
{

#ifdef  INCLUDE_VXBUS
    vxbDevInit();
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_SYSLED

     /* clear the LED display */

     sysLedInit();

     /* cascade the demo LEDs */

     sysDemoLedsCascade();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(AUX_CLK_RATE);
    sysAuxClkConnect((FUNCPTR)sysLedClkRoutine, 0);
    sysAuxClkEnable();
#endif

#endif /* INCLUDE_SYSLED */

/*
 * This was previously reqd for errata workaround #29, the workaround
 * has been replaced with patch for spr99776, so it now serves as an
 * example of implementing an l1 instruction parity handler
 */

#ifdef INCLUDE_L1_IPARITY_HDLR_INBSP
    memcpy((void*)_EXC_OFF_L1_PARITY, (void *)jumpIParity, sizeof(INSTR));
    cacheTextUpdate((void *)_EXC_OFF_L1_PARITY, sizeof(INSTR));
    sysIvor1Set(_EXC_OFF_L1_PARITY);
    cacheDisable(INSTRUCTION_CACHE);
    vxL1CSR1Set(vxL1CSR1Get() | _PPC_L1CSR_CPE);
    cacheEnable(INSTRUCTION_CACHE);
#endif  /* INCLUDE_L1_IPARITY_HDLR_INBSP */

#if defined(INCLUDE_VXBUS) && defined(INCLUDE_SIO_UTILS)
    sysSerialConnectAll();
#endif

#if     defined (INCLUDE_SPE)
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */


#ifdef  INCLUDE_VXBUS
    taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */
    sysPrintInit();
    sysUsrDummp();
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
* ERRNO: N/A
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
* ERRNO: N/A
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
* ERRNO: N/A
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
* ERRNO: N/A
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

        /*
	 * call external int controller connect
         * tmpStat = cascadeIntConnect (vector, routine, parameter);
	 */
        }

    if (tmpStat == ERROR)
        {
        logMsg ("Error in sysIntConnect: Connecting vector = %d.\n",
                (int)vector,2,3,4,5,6);
        }

    return(tmpStat);
    }

/*******************************************************************************
*
* sysIntEnable - enable an interrupt
*
* This function call is used to enable an interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*
* ERRNO: N/A
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
* RETURNS: OK or ERROR if unable to enable interrupt.
*
* ERRNO: N/A
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
* ERRNO: N/A
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
* ERRNO: N/A
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
     * we actually need 12 unique address loads to flush the set.
     * The number of lines to flush should be ( 3/2 * cache lines )
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
* ERRNO: N/A
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
* RETURNS:
*
* ERRNO: N/A
*/

UINT32 vxImmrGet(void)
    {
    return(CCSBAR + 0x80000);
    }

/***************************************************************************
*
* sysGetPeripheralBase   - Provides CCSRBAR address fro security engine
*                          drivers.
*
* RETURNS:
*
* ERRNO: N/A
*/
UINT32 sysGetPeripheralBase()
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

#ifdef INCLUDE_SYSLED

/***********************************************************************
*
* sysLedClkRoutine - shows clock activity on LED
*
* This routine blinks the dot on the Hex LED
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
    static int clkIntCount = 0;
    static INT8 clkCount = 0;

    if((clkIntCount++ % 60) == 0)
        {
        (clkCount++ & 0x1) ?
            sysLedSet(LED_POINT, LED_POINT) : sysLedSet(LED_POINT, 0);
        }
    }

#endif /* INCLUDE_SYSLED */


#ifdef INCLUDE_SHOW_ROUTINES

/***************************************************************************
*
* coreLbcShow - Show routine for local bus controller
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
    printf("CPM Freq = %3d Hz\n",sysClkFreqGet());

    }

#endif

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPci1IntRoute [NUM_PCI1_SLOTS][4] =
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL};

LOCAL UCHAR sysPci3IntRoute [NUM_PCIEX_SLOTS][4] =
    {PCIEX_XINT1_LVL, PCIEX_XINT2_LVL, PCIEX_XINT3_LVL, PCIEX_XINT4_LVL};

/*******************************************************************************
*
* sysPci1AutoconfigIntrAssign - PCI 1 autoconfig support routine
*
* This routine peforms the PCI 1 auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci1AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UCHAR pin                           /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if ((pin > 0) && (pin < 5))
        tmpChar = sysPci1IntRoute [0][(pin-1)];

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci3AutoconfigIntrAssign - PCI Express autoconfig support routine
*
* This routine peforms the PCI Express auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci3AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UCHAR pin                           /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin > 0) && (pin < 5))                                &&
        (((pLoc->device) - sysPciSlotDeviceNumber) < NUM_PCIEX_SLOTS)   &&
        (((pLoc->device) - sysPciSlotDeviceNumber) >= 0))
        {
        tmpChar =
            sysPci3IntRoute [0][(pin-1)];
        }
    
    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI 1 autoconfig support routine
*
* This routine performs the PCI 1 auto configuration support function.
*
* RETURNS: OK or ERROR
*/

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UINT devVend                        /* deviceID/vendorID of device */
    )
    {

    /*
     * Host controller itself (device number is 0) won't be configured;
     * Bridge on the Arcadia board (device number 17) won't be configured;
     */

    if ((pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)  ||
        (pLoc->bus == 0 && pLoc->device == 18 && pLoc->function == 0) ||
        (devVend == PCI_ARCADIA_BRIDGE_DEV_ID))

        return(ERROR);


    return (OK); /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci3AutoconfigInclude - PCI Express autoconfig support routine
*
* This routine performs the PCI Express auto configuration support function.
*
* RETURNS: OK or ERROR
*/

STATUS sysPci3AutoconfigInclude
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UINT devVend                        /* deviceID/vendorID of device */
    )
    {

    /*
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0) ||
        (pLoc->device > 0))
        return(ERROR);

    return OK; /* Autoconfigure all devices */
    }
#endif /* INCLUDE_PCI_BUS */

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

UINT32 divisorGet
    (
    UINT32 xtal,
    UINT32 baud
    )
    {
    return (xtal / (16 * baud));
    }

#define SYS_MAX_BOARD_LEN  0x32

char sysBoradSerial[SYS_MAX_BOARD_LEN]={0};

char * sysBoard (UINT8 flag)
{
    UINT32 uiLen = 0;
    UINT32 uiAddr = 0;

    uiLen = SYS_MAX_BOARD_LEN;
    uiAddr = FLASH_BOOT_OFFSET + NV_BOARD_ADRS_OFFSET + flag*0x50;
    
    if(SYS_MAX_BOARD_LEN >= (strlen(uiAddr) + 1))
    {
        uiLen = strlen(uiAddr) + 1;
        snprintf (sysBoradSerial,uiLen,"%s\n",uiAddr);
    }
    else
    {
        snprintf (sysBoradSerial,1,"%s\n","\0");
    }

    return sysBoradSerial;          
}




STATUS sysSetBoardID(char *string)
{
    return sysNvRamSet(string, (strlen(string)+1), NV_BOARD_ADRS_OFFSET);          
}

STATUS sysSetBoardStatus(char *string)
{
    return sysNvRamSet(string,(strlen(string)+1),NV_BOARD_ADRS_OFFSET + 0x50);          
}

#define ETH_MII_MCFG           0x24520
#define ETH_MII_MCOM           0x24524
#define ETH_MII_MADD           0x24528
#define ETH_MII_MCON           0x2452C
#define ETH_MII_MSTAT          0x24530
#define ETH_MII_MIND           0x24534

int sysPhyRead(int phyaddr,int regaddr)
{
	UINT32 uiRegVal = 0;
	UINT32 uiVal = 0;

    uiRegVal = ((phyaddr & 0x1f )<< 8) | (regaddr);
    
    FSL_WRITE_REG((ETH_MII_MADD + CCSBAR),uiRegVal);
    FSL_WRITE_REG((ETH_MII_MCOM + CCSBAR),0);
    FSL_WRITE_REG((ETH_MII_MCOM + CCSBAR),1);
    taskDelay(2);
    uiRegVal = FSL_READ_REG((UINT32*)(CCSBAR + ETH_MII_MIND));
    if(0x00 != (uiRegVal & 1))
    {
        printf("MII MGMT write fail\n");
        return -1;
    }
    uiVal = FSL_READ_REG((UINT32*)(CCSBAR + ETH_MII_MSTAT));

    printf("read reg %d val 0x%x\n",regaddr,uiVal);
}

#define PMS_MAX_LEN   0x800000
#define PMS_BLK_SIZE  0x2000
#define PMS_BASE_ADRS 0xf0000000
#define PMS_OS_ADRS   0xf0000000

#define PMS_CPU_LE16(x)	x

INT32 pmsProgram(UINT32 uiBaseAddr,UINT32 uiAddr,UINT8 *pucBuf,UINT32 uiLen,
                   UINT32 uiFlag)
{
    VUINT16 *puiBaseAddr = (VUINT16*)uiBaseAddr;
    VUINT16 *puiSectorAddr = (VUINT16*)uiAddr;
    UINT32  *puiData = NULL;
    UINT16   usVal = 0;
	UINT32   uiLoop = 0;
    UINT32   uiOnePercent = 0;
    UINT32   uiPercent = 0;

    uiLen = uiLen / sizeof(UINT32);
    puiData = (UINT32*)pucBuf;
    uiOnePercent = (uiLen / 100) + 1;

	for(uiLoop = 0; uiLoop < uiLen; uiLoop++)
	{
        if((0x0 != uiFlag) && (0x00 == (uiLoop % uiOnePercent)))
        {
            if(uiPercent < 100)
            {
                printf("program flash %d%%\r",uiPercent);
            }
            uiPercent++;
        }
        
		*(puiBaseAddr + 0x555) = 0x00aa;
    	*(puiBaseAddr + 0x2aa) = 0x0055;
    	*(puiBaseAddr + 0x555) = 0x00a0;
        
        usVal = PMS_CPU_LE16(((*puiData) >> 16) & 0xffff);
    	*puiSectorAddr = usVal;
        
        if(OK != flashProgramWait(puiSectorAddr,usVal))
        {
            return ERROR;
        }
        /*puiBaseAddr = 0x00f0;*/
		puiSectorAddr++;

        *(puiBaseAddr + 0x555) = 0x00aa;
    	*(puiBaseAddr + 0x2aa) = 0x0055;
    	*(puiBaseAddr + 0x555) = 0x00a0;
        
    	usVal = PMS_CPU_LE16((*puiData) & 0xffff);
    	*puiSectorAddr = usVal;
        
        if(OK != flashProgramWait(puiSectorAddr,usVal))
        {
            return ERROR;
        }
        
		puiSectorAddr++;
		puiData++;
	}
    
	return OK;
}
INT32 pmsErasePart(UINT32 uiBaseAddr,UINT32 uiAddr,UINT32 uiTimes,UINT32 uiFlag)
{
    UINT32   uiLoop = 0;
    UINT32   uiOffset = 0;
    UINT32   uiOnePercent = 0;
    UINT32   uiPercent = 0;

    
    uiOffset = uiAddr;
    uiOnePercent = (uiTimes / 100) + 1;
    if(0x0 == uiTimes)
    {
        return OK;
    }
    
    for(uiLoop = 0; uiLoop < uiTimes;uiLoop++)
	{
		if(OK != flashErase(uiBaseAddr,uiOffset))
		{
            return ERROR;
        }
		uiOffset += 0x2000;
        if((0x0 != uiFlag) && (0x00 == (uiLoop % uiOnePercent)))
        {
            if(0x1 == uiOnePercent)
            {
                uiPercent = 100 * uiLoop / uiTimes;
            }
            
            if(uiPercent < 100)
            {
                printf("erase flash %d%%\r",uiPercent);
            }
            uiPercent++;
        }
	}
    
    return OK;
}

void sysPMSProgram(UINT32 len,UINT8 *data)
{
    UINT32     uiOffset = 0;
	UINT32     uiTimes = 0;

	uiTimes = (PMS_MAX_LEN/PMS_BLK_SIZE);
    if(OK != pmsErasePart(PMS_BASE_ADRS,PMS_OS_ADRS,uiTimes,FLASH_PROG_PRINT))
    {
        return ;
    }
    
    uiOffset = PMS_OS_ADRS;
    if(OK != pmsProgram(PMS_BASE_ADRS,uiOffset,data,len,FLASH_PROG_PRINT))
    {
        return ;
    }
}

STATUS pmFileLoad(char *pucFileName)
{
    FILE            *pstFp = NULL;
    UINT8           *pucBuf = NULL;
    UINT32           uiReadLen = 0;
    ULONG            ulFileLen = 0;

    ulFileLen = flashGetFileLen(pucFileName);
    if(0x00 == ulFileLen)
    {
        logMsg("file not exist\n",0,0,0,0,0,0);
        return ERROR;
    }

    pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    
    if(ulFileLen > PMS_MAX_LEN)
    {
        logMsg("file is too large max 8M\n",0,0,0,0,0,0);
        fclose(pstFp);
        return ERROR;
    }
    
    pucBuf = malloc(PMS_MAX_LEN);
    if(NULL == pucBuf)
    {
        logMsg("malloc memory failed\n",0,0,0,0,0,0);
        fclose(pstFp);
        return ERROR;
    }
    
    uiReadLen = fread(pucBuf,sizeof(UINT8),PMS_MAX_LEN,pstFp);
    if(ulFileLen != uiReadLen)
    {
        logMsg("read file error\n",0,0,0,0,0,0);
        fclose(pstFp);
        free(pucBuf);
        return ERROR;
    }
    
    sysPMSProgram(uiReadLen,pucBuf);
    
    free(pucBuf);
    fclose(pstFp);

    return OK;
}


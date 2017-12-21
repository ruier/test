/* sysLib.c - Freescale ads8544 (Intrepid) board system-dependent library */

/*
 * Copyright (c) 2007-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01m,21dec10,swu  Modify CCSBAR mmu table size.(WIND00247711)
01l,20aug09,l_z  Fix BSPVTS sysModel test failed. (WIND00174714)
01k,30oct08,x_z  add vxEnableBP to sysHwInit and replace disableBranchPrediction
                 with vxDisableBP.(WIND00127193)
01j,29apr08,x_f  Fixed the delay period when boot from bootrom resident.
01i,09apr08,x_f  Fixed sysTimerClkFreq and sysStaticTlbDesc error,
                 modify sysModel().
01h,26sep07,pdg  Fixed doc errors
01g,23aug07,dtr  Remove Epic/Timer for vxBus version.
01f,21aug07,mmi  remove vxPowerModeSet() since deprecated
01e,12jul07,vik  removed cmdLine.c inclusion
01d,22feb07,dtr  SYS_MODEL is undefined.
01c,22feb07,dtr  Add support for vxBus sio.
01b,20feb07,dtr  Modify sysNet.c include conditional.
01a,07feb07,dtr  Created from cds8548/sysLib.c/01e
*/

/*
DESCRIPTION
This library provides board-specific routines for ads8544 (Intrepid)

INCLUDE FILES:

SEE ALSO:
.pG "Configuration"
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

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#   include "mot85xxPci.h"
#endif /* INCLUDE_PCI_BUS */

/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [] = {

    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash + PIXIS */

    /* needed be first entry here */
    {
    0xf0000000, 0x0, 0xf0000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
    _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | 
    _MMU_TLB_ATTR_G
    }

    /* LOCAL MEMORY needed be second entry here  - 
     * one TLB would be 256MB so use to get required 512MB 
     */
    ,
    {
    0x00000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | 
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X |  
    CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT| _MMU_TLB_ATTR_M
    }

#if (LOCAL_MEM_SIZE > 0x10000000)
    ,
    { 
    0x10000000, 0x0, 0x10000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | 
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M | 
    CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT
    } 
#endif

    ,
    { 
    CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | 
    _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    }

    /* All these are not protected */
    , 
    { 
    CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_1 | _MMU_TLB_SZ_1M | 
    _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

#ifdef INCLUDE_L2_SRAM
    ,
    { 
    L2SRAM_ADDR, 0x0, L2SRAM_ADDR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K | 
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
    }
#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_PCI_BUS
    ,
    { 
    PCIEX1_MEM_ADRS, 0x0, PCIEX1_MEM_ADRS, _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ | 
    _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
    ,
    { 
    PCIEX2_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS, _MMU_TLB_TS_0 |  
    PCI_MMU_TLB_SZ | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
    ,
    { 
    PCIEX3_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS, _MMU_TLB_TS_0 |  
    PCI_MMU_TLB_SZ | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
    ,
    { 
    PCI_MEM_ADRS, 0x0, PCI_MEM_ADRS, _MMU_TLB_TS_0 |  PCI_MMU_TLB_SZ | 
    _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
#endif /* INCLUDE_PCI_BUS */
    };

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#ifdef MMU_ASID_MAX     /* Base 6 MMU library in effect */

/* macro to compose 64-bit PHYS_ADDRs */

#   define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))
#endif /* MMU_ASID_MAX */

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, the Board Control 
 * and Status registers, the MPC8260 Internal Memory Map, and local FLASH RAM 
 * are set here.
 *
 * PTEs are held in a Page Table.  Page Table sizes are
 * integer powers of two based on amount of memory to be mapped and a
 * minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
 * for 32-bit PowerPCs are:
 *
 * Total mapped memory      Page Table size
 * -------------------      ---------------
 *        8 Meg             64 K
 *       16 Meg             128 K
 *       32 Meg             256 K
 *       64 Meg             512 K
 *      128 Meg             1 Meg
 *         .                .
 *         .                .
 *         .                .
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

PHYS_MEM_DESC sysPhysMemDesc [] = {
    {

    /* 
     * Vector Table and Interrupt Stack 
     * Must be sysPhysMemDesc [0] to allow adjustment in sysHwInit() 
     */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID     | VM_STATE_MASK_WRITABLE | 
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID          | VM_STATE_WRITABLE      | 
    TLB_CACHE_MODE          | VM_STATE_MEM_COHERENCY
    }

#ifdef INCLUDE_L2_SRAM
    ,
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    L2SRAM_WINDOW_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    }
#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_LBC_CS3
    ,
    {
    (VIRT_ADDR) LBC_CS3_LOCAL_ADRS,
    (PHYS_ADDR) LBC_CS3_LOCAL_ADRS,
    LBC_CS3_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_LBC_CS3 */

#ifdef INCLUDE_PCI_BUS
    ,
    {
    (VIRT_ADDR) PCIEX1_MEM_ADRS,
    (PHYS_ADDR) PCIEX1_MEM_ADRS,
    PCIEX1_MEM_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX1_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX1_MEMIO_ADRS,
    PCIEX1_MEMIO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX1_IO_ADRS,
    (PHYS_ADDR) PCIEX1_IO_ADRS,
    PCIEX1_IO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX2_MEM_ADRS,
    (PHYS_ADDR) PCIEX2_MEM_ADRS,
    PCIEX2_MEM_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
    PCIEX2_MEMIO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX2_IO_ADRS,
    (PHYS_ADDR) PCIEX2_IO_ADRS,
    PCIEX2_IO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX3_MEM_ADRS,
    (PHYS_ADDR) PCIEX3_MEM_ADRS,
    PCIEX3_MEM_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX3_MEMIO_ADRS,
    (PHYS_ADDR) PCIEX3_MEMIO_ADRS,
    PCIEX3_MEMIO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCIEX3_IO_ADRS,
    (PHYS_ADDR) PCIEX3_IO_ADRS,
    PCIEX3_IO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCI_MEM_ADRS,
    (PHYS_ADDR) PCI_MEM_ADRS,
    PCI_MEM_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCI_MEMIO_ADRS,
    (PHYS_ADDR) PCI_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCI_IO_ADRS,
    (PHYS_ADDR) PCI_IO_ADRS,
    PCI_IO_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_PCI_BUS */

    ,
    {
    (VIRT_ADDR) FLASH_BASE_ADRS,
    (PHYS_ADDR) FLASH_BASE_ADRS,
    MAIN_FLASH_SIZE,
    VM_STATE_MASK_VALID   | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID        | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* Clock Ratio Tables */

#define MAX_VALUE_PLAT_RATIO 32
UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO][2] = {
    { 0,  0},
    { 0,  0},
    { 2,  0},
    { 3,  0},
    { 4,  0},
    { 5,  0},
    { 6,  0},
    { 7,  0}, 
    { 8,  0},
    { 9,  0},
    { 10, 0},
    { 0,  0},
    { 12, 0},
    { 0,  0},
    { 0,  0},
    { 0,  0},
    { 16, 0},
    { 0,  0},
    { 0,  0},
    { 0,  0},
    { 20, 0},
    { 0,  0}
};

#define MAX_VALUE_E500_RATIO 10
UINT32 e500RatioTable[MAX_VALUE_PLAT_RATIO][2] = {
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

int     sysBus        = BUS_TYPE_NONE;      /* system bus type (VME_BUS, etc) */
int     sysCpu        = CPU;                /* system CPU type (PPC8260) */
char *  sysBootLine   = BOOT_LINE_ADRS;     /* address of boot line */
char *  sysExcMsg     = EXC_MSG_ADRS;       /* catastrophic message area */
int     sysProcNum;                         /* processor number of this CPU */
BOOL    sysVmeEnable  = FALSE;              /* by default no VME */
UINT32  coreFreq;

IMPORT  void mmuE500TlbDynamicInvalidate();
IMPORT  void mmuE500TlbStaticInvalidate();
IMPORT  void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, 
                                   BOOL cacheAllow);
IMPORT  BOOL mmuPpcIEnabled;
IMPORT  BOOL mmuPpcDEnabled;
IMPORT  void sysIvprSet(UINT32);

/* forward declarations */

void    sysUsDelay (int);

/* 8260 Reset Configuration Table (From page 9-2 in Rev0 of 8260 book) */

#define END_OF_TABLE 0

UINT32  sysClkFreqGet(void);
UINT32  ppcE500ICACHE_LINE_NUM = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM = (128 * 12);
        
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;

#include "sysMotI2c.c"
#include "sysMpc85xxI2c.c"

#ifdef  INCLUDE_FLASH
#   include <mem/flashMem.c>
#   include <mem/nvRamToFlash.c>
#else  /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif  /* INCLUDE_FLASH */

UINT32  inFullVxWorksImage = FALSE;

#define EXT_VEC_IRQ0            56
#define EXT_NUM_IRQ0            EXT_VEC_IRQ0
#define EXT_MAX_IRQS            200

STATUS  sysIntEnablePIC         (int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable     (int intNum);
STATUS  sysCascadeIntDisable    (int intNum);
void    flashTest(VUINT16 * address, VUINT16 * buffer, VINT32 length);

UINT32  baudRateGenClk;

IMPORT  void sysSerialConnectAll (void);

#include "sysL2Cache.c"

#ifdef  INCLUDE_TFFS
#   include "am29lv64xdMtd.c"
#endif  /* INCLUDE_TFFS */

#ifdef  INCLUDE_VXBUS
IMPORT void (*_vxb_usDelayRtn)(int  delayTime); 
IMPORT void hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#define WB_MAX_IRQS 256

/* defines */

#define ZERO    0

#define SYS_MODEL_8533E    "Freescale MPC8533E - Security Engine"
#define SYS_MODEL_8533     "Freescale MPC8533"
#define SYS_MODEL_8544E    "Freescale MPC8544E - Security Engine"
#define SYS_MODEL_8544     "Freescale MPC8544"
                           
#define SYS_MODEL          "Freescale MPC8544E - Security Engine"
                           
#define SYS_MODEL_UNKNOWN  "Freescale Unknown processor"

#define MPC8544E           0x803c0000
#define MPC8544            0x80340000

#define DELTA(a,b)         (abs((int)a - (int)b))
#define HID0_MCP           0x80000000
#define HID1_ABE           0x00001000
#define HID1_ASTME         0x00002000
#define HID1_RXFE          0x00020000

#ifdef  INCLUDE_VXBUS
#   include "hwconf.c"
#endif  /* INCLUDE_VXBUS */

#ifdef  INCLUDE_NETWORK
#   include "sysNet.c"
#endif  /* INCLUDE_NETWORK */

#ifdef  INCLUDE_BRANCH_PREDICTION
IMPORT void     vxEnableBP();
IMPORT void     vxDisableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

#ifdef  INCLUDE_L2_SRAM
LOCAL   void sysL2SramEnable(BOOL both);
#endif  /* INCLUDE_L2_SRAM */

#ifdef  INCLUDE_SPE
#   include <speLib.h>
IMPORT  int (* _func_speProbeRtn) () ;
#endif  /* INCLUDE_SPE */

#ifdef  INCLUDE_CACHE_SUPPORT
LOCAL   void sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

UINT32  sysTimerClkFreq;

IMPORT  void    sysL1Csr1Set(UINT32);
IMPORT  UINT    sysTimeBaseLGet(void);

LOCAL   char *  physTop = NULL;
LOCAL   char *  memTop  = NULL;

#if defined (INCLUDE_SPE)
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
    ULONG   regVal;
    int     speUnitPresent = OK;

    /* The CPU type is indicated in the Processor Version Register (PVR) */

    regVal = 0;

    switch (regVal)
        {
        case 0:
        default:
            speUnitPresent = OK;
            break;
        }

    return(speUnitPresent);
    }
#endif  /* INCLUDE_SPE */


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
    UINT32  device;
    char *  retChar = NULL;

    device = *M85XX_SVR(CCSBAR);
    switch (device & 0xffff0000)
        {
        case MPC8544E:
            retChar = SYS_MODEL_8544E;
            break;
        case MPC8544:
            retChar = SYS_MODEL_8544;
            break;
        default:
            retChar = SYS_MODEL_UNKNOWN;
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
* sysClkFreqGet - reutrns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet(void)
    {
#ifdef FORCE_DEFAULT_FREQ
    return(DEFAULT_SYSCLKFREQ);
#else  /* FORCE_DEFAULT_FREQ */
    UINT32  sysClkFreq, oscFreq;
    UINT32  e500Ratio, platRatio;
    UINT8   osc;
    
    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR); 

    if ((platRatio > MAX_VALUE_PLAT_RATIO) || 
        (platRatioTable[platRatio][0] == 0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */
        
    osc = *PIXIS_OSC(PIXIS_BASE);
    
    switch (osc)
        {
        case PIXIS_OSC_33_MHZ:
            oscFreq = FREQ_33_MHZ;
            break;
        case PIXIS_OSC_66_MHZ:
            oscFreq = FREQ_66_MHZ;
            break;
        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    sysClkFreq  = ((UINT32)(oscFreq * platRatioTable[platRatio][0]))
                   >> ((UINT32)platRatioTable[platRatio][1]);

    e500Ratio   = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    coreFreq    = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0]))
                   >> ((UINT32)e500RatioTable[e500Ratio][1]);

    return(sysClkFreq);
#endif  /* FORCE_DEFAULT_FREQ */
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
    UINT32  picClockDivisor    /* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
        return sysClkFreqGet() / picClockDivisor;
    else
        return HPCN_RTC_FREQ;    /* 14.318 MHz */
    }

/*******************************************************************************
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

void sysHwInit (void)
    {
#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    sysIvprSet(0x0);

    /* Disable L1 Icache */

    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* Check for architecture support for 36 bit physical addressing */

#if defined(PPC_e500v2)
    vxHid0Set(_PPC_HID0_MAS7EN | vxHid0Get());
#endif

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    /* enable time base for delay use before DEC interrupt is setup */

    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

    sysTimerClkFreq = sysClkFreqGet() >> 3;   /* Clock div is 8 */

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery(); 
#endif /* INCLUDE_CACHE_SUPPORT */

    i2cDrvInit(0, 0);
    i2cDrvInit(1, 0);

    /* Initialise L2CTL register */

    vxL2CTLSet(0x28000000, M85XX_L2CTL(CCSBAR));

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
#else /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */
    if (inFullVxWorksImage == FALSE)
        {
        mmuPpcIEnabled = TRUE;
        mmuPpcDEnabled = TRUE;
        }

    /* Enable I Cache if instruction mmu disabled */

#   if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
        mmuPpcIEnabled = TRUE;
#   endif   /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */
#endif  /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxHid1Set(HID1_ABE); /* Address Broadcast enable */ 
    sysL2CacheInit();
#endif  /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    /* Machine check via RXFE for RIO */

    vxHid1Set(vxHid1Get()| HID1_ASTME | HID1_RXFE); /* Address Stream Enable */

    WRS_ASM("isync");

#ifdef  INCLUDE_VXBUS
     _vxb_usDelayRtn = &sysUsDelay;
    hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#if defined(INCLUDE_L2_SRAM)
#   if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2SramEnable(TRUE);
#   elif (defined(INCLUDE_L2_SRAM))
    sysL2SramEnable(FALSE);
#   endif
#endif  /* INCLUDE_L2_SRAM */

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

    l2CtlVal &= (~M85XX_L2CTL_L2E_MSK);
    vxL2CTLSet(l2CtlVal, M85XX_L2CTL(CCSBAR));
    l2CtlVal &= (~M85XX_L2CTL_L2SRAM_MSK);

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
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);   /* Warm reboot */

    intLock();    

#ifdef  INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

#ifdef  INCLUDE_CACHE_SUPPORT
    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#endif  /* INCLUDE_CACHE_SUPPORT */

    vxMsrSet(0);
    
    /* Clear unnecessary TLBs */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    (*pRom) (startType);    /* jump to bootrom entry point */

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

#ifdef  INCLUDE_VXBUS
    vxbDevInit();
#endif  /* INCLUDE_VXBUS */

    /* initialize serial interrupts */

    sysSerialConnectAll();

#if defined (INCLUDE_SPE)
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */

#ifdef  INCLUDE_VXBUS
    taskSpawn("devConnect", 0, 0, 10000, vxbDevConnect, 0,0,0,0,0,0,0,0,0,0);
#endif  /* INCLUDE_VXBUS */
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
    int procNum             /* processor number */
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
    int     adrsSpace,      /* bus address space where busAdrs resides */
    char *  localAdrs,      /* local address to convert */ 
    char ** pBusAdrs        /* where to return bus address */ 
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
    int     adrsSpace,      /* bus address space where busAdrs resides */
    char *  busAdrs,        /* bus address to convert */
    char ** pLocalAdrs      /* where to return local address */
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
    int    delay            /* length of time in microsec to delay */
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
         * The Time Base counts UP from 0 to all F's.
         */

        curTickCount = sysTimeBaseLGet();

        /* Check for actual rollover */

        if (curTickCount < baselineTickCount)
            {
            actualRollover = 1;
            }

        if (((curTickCount >= terminalTickCount) &&
            (actualRollover == calcRollover)) ||
            ((curTickCount < terminalTickCount) &&
            (actualRollover > calcRollover)))
            {

            /* Delay time met */

            break;
            }
        }
    while (TRUE); /* breaks above when delay time is met */
    }

/*******************************************************************************
*
* sysMsDelay - 1ms delay increments. 
*
* This routine consumes 1ms of time * delay.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    UINT    delay                   /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (int) delay * 1000 );
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

LOCAL void sysL1CacheQuery(void)
    {
    UINT32 temp;
    UINT32 align;
    UINT32 cachesize;

    temp      = vxL1CFG0Get();
    cachesize = (temp & 0xFF) << 10;
    align     = (temp >> 23) & 0x3;


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
#endif  /* INCLUDE_CACHE_SUPPORT */

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
    char *  errorMsg
    )
    {
    strcpy ((char *)EXC_MSG_OFFSET, errorMsg);
    }


void chipErrataCpu29Print(void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\n"
               "System rebooted to limit damage.");
    }

/*******************************************************************************
*
* sysGetPeripheralBase   - Provides CCSBAR for security engine drivers
*
* RETURNS: CCSRBAR value
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


#ifdef  INCLUDE_SHOW_ROUTINES

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

void coreLbcShow(void)
    {
    VINT32 tmp, tmp2;

    tmp  = * (VINT32 *) M85XX_BR0(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR0(CCSBAR);
    printf("Local bus BR0 = 0x%x\tOR0 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_BR1(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR1(CCSBAR);
    printf("Local bus BR1 = 0x%x\tOR1 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_BR2(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR2(CCSBAR);
    printf("Local bus BR2 = 0x%x\tOR2 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_BR3(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR3(CCSBAR);
    printf("Local bus BR3 = 0x%x\tOR3 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_BR4(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR4(CCSBAR);
    printf("Local bus BR4 = 0x%x\tOR4 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_BR5(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR5(CCSBAR);
    printf("Local bus BR5 = 0x%x\tOR5 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_BR6(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR6(CCSBAR);
    printf("Local bus BR6 = 0x%x\tOR6 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_BR7(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR7(CCSBAR);
    printf("Local bus BR7 = 0x%x\tOR7 = 0x%x\n", tmp, tmp2);

    tmp  = * (VINT32 *) M85XX_LBCR(CCSBAR);
    printf("Local bus LBCR = 0x%x\n", tmp);

    tmp  = * (VINT32 *) M85XX_LCRR(CCSBAR);
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

void coreShow(void)
    {
    VUINT32 tmp, tmp2;

    tmp = vxMsrGet();
    printf("MSR - 0x%x\n", tmp);
    printf("      UCLE-%x SPE-%x WE-%x CE-%x EE-%x PR-%x ME-%x\n",
           xbit32(tmp, 37), xbit32(tmp, 38), xbit32(tmp, 45), xbit32(tmp, 46),
           xbit32(tmp, 48), xbit32(tmp, 49), xbit32(tmp, 51));
    printf("      UBLE-%x DE-%x IS-%x DS-%x PMM-%x\n",
           xbit32(tmp, 53), xbit32(tmp, 54), xbit32(tmp, 58), xbit32(tmp, 59),
           xbit32(tmp, 61));
    tmp  = vxHid0Get();
    tmp2 = vxHid1Get();
    printf("HID0 = 0x%x, HID1 = 0x%x\n", tmp, tmp2);
    tmp  = vxL1CSR0Get();
    printf("L1CSR0: cache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp  = vxL1CSR1Get();
    printf("L1CSR1: Icache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp  = vxL1CFG0Get();
    tmp2 = vxL1CFG1Get();
    printf("L1CFG0 = 0x%x, L1CFG1 = 0x%x\n", tmp, tmp2);
    tmp  = *(VUINT32 *) (CCSBAR + 0x20000);
    printf("L2CTL - 0x%x\n", tmp);
    printf("        l2 is %s\n", tmp & 0x80000000?"ON":"OFF");
    printf("        l2siz-%x l2blksz-%x l2do-%x l2io-%x\n",
           (xbit0(tmp, 2) << 1) | xbit0(tmp, 3), 
           (xbit0(tmp, 4) << 1) | xbit0(tmp, 5),
           xbit0(tmp, 9), 
           xbit0(tmp, 10));
    printf("        l2pmextdis-%x l2intdis-%x l2sram-%x\n",
           xbit0(tmp, 11), xbit0(tmp, 12),
           (xbit0(tmp, 13) << 2) | (xbit0(tmp, 14) << 1) | xbit0(tmp, 15));
    tmp  = *(VUINT32 *) (CCSBAR + 0x20100);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x20108);
    printf("L2SRBAR0 - 0x%x\n", tmp);
    printf("L2SRBAR1 - 0x%x\n", tmp2);

    printf("Core Freq = %3d Hz\n", coreFreq);
    printf("CCB Freq = %3d Hz\n",  sysClkFreqGet());
    printf("PCI Freq = %3d Hz\n",  OSCILLATOR_FREQ);
    printf("CPM Freq = %3d Hz\n",  sysClkFreqGet());
    }
#endif  /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_PCI_BUS

/* INT LINE TO IRQ assignment for wrSbc85xx-PCI board. */

LOCAL UCHAR sysPci1IntRoute [NUM_PCIEX1_SLOTS][4] = {
    {PCIEX_XINT1_LVL, PCIEX_XINT2_LVL, PCIEX_XINT3_LVL, PCIEX_XINT4_LVL}
};

LOCAL UCHAR sysPci2IntRoute [NUM_PCIEX2_SLOTS][4] = {
    {PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL}
};

LOCAL UCHAR sysPci3IntRoute [NUM_PCIEX3_SLOTS][4] = {
    {PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL}
};

LOCAL UCHAR sysPci4IntRoute [NUM_PCI_SLOTS][4] = {
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL},
    {PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL}
};

/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI 1 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM *    pSys,            /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,            /* pointer to function in question */
    UINT            devVend          /* deviceID/vendorID of device */
    )
    {

    /* 
     * Only support BUS 0;
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus > 1) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0) ||
        (pLoc->device > 0))
        return(ERROR);

    return (OK);     /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci2AutoconfigInclude - PCI 2 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci2AutoconfigInclude
    (
    PCI_SYSTEM *    pSys,            /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,            /* pointer to function in question */
    UINT            devVend          /* deviceID/vendorID of device */
    )
    {

    /* 
     * Only support BUS 0;
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus > 1) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0) ||
        (pLoc->device > 0))
        return(ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci3AutoconfigInclude - PCI 2 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci3AutoconfigInclude
    (
    PCI_SYSTEM *    pSys,            /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,            /* pointer to function in question */
    UINT            devVend          /* deviceID/vendorID of device */
    )
    {

    /* Unsupported */

    return(ERROR);
    }

/*******************************************************************************
*
* sysPci4AutoconfigInclude - PCI 4 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci4AutoconfigInclude
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function in question */
    UINT            devVend         /* deviceID/vendorID of device */
    )
    {

    /* 
     * Only support BUS 0;
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0) )
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
    PCI_SYSTEM *    pSys,            /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,            /* pointer to function in question */
    UCHAR           pin              /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* 
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if (((pin >= 1) && (pin < 5)) && ((pLoc->device) < NUM_PCIEX1_SLOTS))
        {
        tmpChar = sysPci1IntRoute [(pLoc->device)][pin - 1];
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci2AutoconfigIntrAssign - PCI 2 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci2AutoconfigIntrAssign
    (
    PCI_SYSTEM *    pSys,            /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,            /* pointer to function in question */
    UCHAR           pin              /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* 
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if (((pin >= 1) && (pin < 4)) && ((pLoc->device) < NUM_PCIEX2_SLOTS))
        {
        tmpChar = sysPci2IntRoute [(pLoc->device)][pin - 1];
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci3AutoconfigIntrAssign - PCI 3 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci3AutoconfigIntrAssign
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function in question */
    UCHAR           pin             /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* 
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if (((pin >=1) && (pin < 4)) && ((pLoc->device) < NUM_PCIEX3_SLOTS))
        {
        tmpChar = sysPci3IntRoute [(pLoc->device)][pin - 1];
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci4AutoconfigIntrAssign - PCI 4 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci4AutoconfigIntrAssign
    (
    PCI_SYSTEM *    pSys,            /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,            /* pointer to function in question */
    UCHAR           pin              /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* 
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if (((pin >= 1) && (pin < 4)) && 
        ((pLoc->device - PCI_SLOT1_DEVNO) < NUM_PCI_SLOTS) &&
        ((pLoc->device - PCI_SLOT1_DEVNO) >= 0 ))
        {
        tmpChar = sysPci4IntRoute [(pLoc->device - PCI_SLOT1_DEVNO)][pin - 1];
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }
#endif /* INCLUDE_PCI_BUS */

/*******************************************************************************
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
    int channel                     /* serial channel */
    )
    {
    return ( (SIO_CHAN *)ERROR );
    }


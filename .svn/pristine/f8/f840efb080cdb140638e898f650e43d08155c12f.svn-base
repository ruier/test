/* sysLib.c - Freescale ads834x board system dependent library. */

/*
 * Copyright (c) 2005-2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01v,30apr10,dhy  delete _MMU_UBAT_VP attribute(WIND00162540)
01u,24jul08,z_l  Support MDS8349EA.
01t,14may08,wap  Switch to VxBus TSEC driver
01s,19oct07,ami  USB Support added (Defect WIND00108412 Fix)
01r,18sep07,dtr  Fix doc errors.
01q,17sep07,h_k  fixed #else if to #elif.
01p,21aug07,mmi  remove vxPowerModeSet() since deprecated
01o,08aug07,agf  remove duplicate variable definitions
01n,20jul07,pdg  added m83xx pci support
01m,13jul07,dtr  Remove old interrupt lib.
01m,09jul07,p_g  removed cmdLine.c inclusion
01l,20jun07,sup  serial driver vxBus compliant
01k,19oct06,pdg  fixed build error due to merge
01j,12oct06,dtr  Support vxBus TSEC HEnd driver.
01i,10oct06,sup  Changes for supporting vxBus complaint quicc timer driver

01i,25aug06,dtr  WIND00061424 - CPU 1 errata causes intermittent crash.
01h,27oct05,mdo  SPR#114197 - protect against multiple defines for
                 INCLUDE_PCICFG_SHOW
01g,06oct05,dtr  Fix adrsSpaceLib error for SEC ENGINE.
01f,16aug05,dtr  Remove debug branch from 0x900.SEt optimal SCCR settings 
                 for TSEC/USB and SEC engine. Fix PVR test.
01e,02aug05,dtr  Add PCI2 support.
01d,26apr05,kab  Doc fixes (SPR 108362)
01c,25mar05,dtr  Enable machine check. Add extra BAT support.
01b,22mar05,dtr  Add IBAT for local memory.
01a,10jan05,dtr  adapted from ads827x/sysLib.c.
*/

/*
DESCRIPTION

This library provides board-specific routines.

INCLUDE FILES:
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
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>
#include <miiLib.h>

/* globals */

/*
 * sysBatDesc[] is used to initialize the block address translation (BAT)
 * registers within the PowerPC 603/604 MMU.  BAT hits take precedence
 * over Page Table Entry (PTE) hits and are faster.  Overlap of memory
 * coverage by BATs and PTEs is permitted in cases where either the IBATs
 * or the DBATs do not provide the necessary mapping (PTEs apply to both
 * instruction AND data space, without distinction).
 *
 * The primary means of memory control for VxWorks is the MMU PTE support
 * provided by vmLib and cacheLib.  Use of BAT registers will conflict
 * with vmLib support.  User's may use BAT registers for i/o mapping and
 * other purposes but are cautioned that conflicts with cacheing and mapping
 * through vmLib may arise.  Be aware that memory spaces mapped through a BAT
 * are not mapped by a PTE and any vmLib() or cacheLib() operations on such
 * areas will not be effective, nor will they report any error conditions.
 *
 * Note: BAT registers CANNOT be disabled - they are always active.
 * For example, setting them all to zero will yield four identical data
 * and instruction memory spaces starting at local address zero, each 128KB
 * in size, and each set as write-back and cache-enabled.  Hence, the BAT regs
 * MUST be configured carefully.
 *
 * With this in mind, it is recommended that the BAT registers be used
 * to map LARGE memory areas external to the processor if possible.
 * If not possible, map sections of high RAM and/or PROM space where
 * fine grained control of memory access is not needed.  This has the
 * beneficial effects of reducing PTE table size (8 bytes per 4k page)
 * and increasing the speed of access to the largest possible memory space.
 * Use the PTE table only for memory which needs fine grained (4KB pages)
 * control or which is too small to be mapped by the BAT regs.
 *
 * All BATs point to PROM/FLASH memory so that end customer may configure
 * them as required.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT + _MMU_NUM_EXTRA_IBAT + _MMU_NUM_EXTRA_DBAT)] =
    {
    /* I BAT 0 */
    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* I BAT 1 */

    0, 0,

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,

    /* D BAT 0 */

    0, 0,

    /* D BAT 1 */

    /* use for internal registers into data space */
    /* NOTE! the internal space cannot be cached and should be guarded */
    ((CCSBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128K |
    _MMU_UBAT_VS),
    ((CCSBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

#ifdef INCLUDE_PCI
    /* D BAT 2 - PCI 1 */

    ((PCI_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((PCI_MEM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCI 2 */

#   ifdef INCLUDE_SECONDARY_PCI
    ((CPU_PCI2_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((CPU_PCI2_MEM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#   else /* INCLUDE_SECONDARY_PCI */

    /* D BAT 3 */

    0, 0,
#   endif /* INCLUDE_SECONDARY_PCI */
#else /* INCLUDE_PCI */
    /* D BAT 2 */

    0, 0,

    /* D BAT 3 */

    0, 0,
#endif /* INCLUDE_PCI */
    /*
     * These entries are for the I/D BATs (4-7) on the MPC7455/755/834x/8272.
     * They should be defined in the following order.
     * IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
     * DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
     */

    /* I BAT 4 */

    ((LOCAL_MEM_LOCAL_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((LOCAL_MEM_LOCAL_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_MEM_COHERENT),


    /* I BAT 5 */

    0, 0,

    /* I BAT 6 */

    0, 0,

    /* I BAT 7 */

    0, 0,

    /* D BAT 4 */
#ifndef MPC8349EA_MDS
    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
    _MMU_UBAT_VS),
#else  /* MPC8349EA_MDS */
    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_32M |
    _MMU_UBAT_VS),
#endif /* MPC8349EA_MDS */
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 5 */

    0, 0,

    /* D BAT 6 */

    0, 0,

    /* D BAT 7 */

    0, 0

 
    };

/*
 * mmuPcpBatInitMPC7x5 initializes the standard 4 (0-3) I/D BATs &
 * the additional 4 (4-7) I/D BATs present on the MPC74[45]5 & MPC83xx.
 */

IMPORT void mmuPpcBatInitMPC7x5(UINT32 *pSysBatDesc); /* MPC7x5 and 83xx BAT Support */
IMPORT FUNCPTR _pSysBatInitFunc;

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, the Board Control and
 * Status registers, the MPC834X Internal Memory Map, and local FLASH RAM are set here.
 *
 * PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
 * integer powers of two based on amount of memory to be mapped and a
 * minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
 * for 32-bit PowerPCs are:
 *
 * Total mapped memory        Page Table size
 * -------------------        ---------------
 *        8 Meg                 64 K
 *       16 Meg                128 K
 *       32 Meg                256 K
 *       64 Meg                512 K
 *      128 Meg                  1 Meg
 *     .                .
 *     .                .
 *     .                .
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
    RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_GUARDED |
    VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_GUARDED |
    VM_STATE_CACHEABLE_NOT
    },

    {

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    LOCAL_MEM_SIZE - RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    },

#ifdef INCLUDE_SECONDARY_DRAM
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    LOCAL_MEM_SIZE2,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    },
#endif /* INCLUDE_SECONDARY_DRAM */

    {
        
    /* Board Control and Status Registers */

    (VIRT_ADDR) BCSR_BASE_ADRS,
    (PHYS_ADDR) BCSR_BASE_ADRS,
    BCSRS_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    }

    ,
    {
        
    /* MPC834x Device Memory Map */

    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    CCSBAR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    }

    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int   sysBus      = BUS;              /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;              /* system CPU type (PPC603) */
char *sysBootLine = BOOT_LINE_ADRS;   /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;     /* catastrophic message area */
int   sysProcNum;                     /* processor number of this CPU */
UINT32   baudRateGenClk;

LOCAL char * memTop = NULL;
LOCAL BOOL configured = FALSE;
LOCAL char * physTop = NULL;

/* forward declarations */

UINT32 sysClkFreqGet();
void sysUsDelay(UINT32);
IMPORT void hardWareInterFaceInit();
 
#ifdef INCLUDE_FLASH
#   ifdef MPC8349EA_MDS
#       include "flashMem.c"
#       include "nvRamToFlash.c"
#   else  /* MPC8349EA_MDS */
#       include <mem/flashMem.c>
#       include <mem/nvRamToFlash.c>
#   endif /* MPC8360EA_MDS */
#else  /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */

#ifdef INCLUDE_PIB_SUPPORT
#   include "sysMotI2c.c"
#   include "sysMpc834xI2c.c"
#endif /* INCLUDE_PIB_SUPPORT */

#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif /* INCLUDE_SIO_UTILS */

#if (defined(INCLUDE_TSEC_VXB_END2) || defined(INCLUDE_TSEC_VXB_END) )
#   include "sysNet.c"
#endif /* INCLUDE_TSEC_VXB_END2 & ... */

void sysControlLED  ( int LED, int on);
void sysLedInit     (void);

/* defines */

#define ZERO                0
#define SYS_MODEL           "Motorola MPC834"
#define SYS_MODEL_8349      "Motorola MPC8349 ADS"
#define SYS_MODEL_8349E     "Motorola MPC8349E ADS"
#define SYS_MODEL_8347T     "Motorola MPC8347 TBGA ADS"
#define SYS_MODEL_8347ET    "Motorola MPC8347E TBGA ADS"
#define SYS_MODEL_8347P     "Motorola MPC8347 PBGA ADS"
#define SYS_MODEL_8347EP    "Motorola MPC8347E PBGA ADS"
#define SYS_MODEL_8343E     "Motorola MPC8343E ADS"
#define SYS_MODEL_8343      "Motorola MPC8343 ADS"

#define SYS_MODEL_UNKNOWN   "Not an E300 core - device unknown"

/* Set ACR to park on PCI bus */

#define M83XX_ACR_PARKM_PCI 0x1

/* Clock configuration */

typedef struct 
    {
    UINT32 spmf;
    UINT32 corepll;
    UINT32 clkin;
    UINT32 csbfreq;
    UINT32 corefreq;
    } CLK_TABLE;


LOCAL CLK_TABLE clkConfTable[] = 
    {
#ifdef GNU_SUPPORTS_BINARY_FORMAT
    {0b101 ,0b0000010,66 ,333 ,333},
    {0b101 ,0b0100010,66 ,333 ,333},
    {0b011 ,0b0000100,66 ,200 ,400},
    {0b011 ,0b0100100,66 ,200 ,400},
    {0b100 ,0b0000011,66 ,266 ,400},
    {0b100 ,0b0100011,66 ,266 ,400},
    {0b101 ,0b0000011,66 ,333 ,500},
    {0b101 ,0b0100011,66 ,333 ,500},
    {0b011 ,0b0000101,66 ,200 ,500},
    {0b100 ,0b0000100,66 ,266 ,533},
    {0b011 ,0b0000100,66 ,200 ,600},
    {0b100 ,0b0000101,66 ,266 ,667},
    {0b101 ,0b0000100,66 ,333 ,667},
#else  /* GNU_SUPPORTS_BINARY_FORMAT */
    {0x5 ,0x02,66 ,333 ,333},
    {0x5 ,0x22,66 ,333 ,333},
    {0x3 ,0x04,66 ,200 ,400},
    {0x3 ,0x24,66 ,200 ,400},
    {0x4 ,0x03,66 ,266 ,400},
    {0x4 ,0x23,66 ,266 ,400},
    {0x5 ,0x03,66 ,333 ,500},
    {0x5 ,0x23,66 ,333 ,500},
    {0x3 ,0x05,66 ,200 ,500},
    {0x4 ,0x04,66 ,266 ,533},
    {0x3 ,0x04,66 ,200 ,600},
    {0x4 ,0x05,66 ,266 ,667},
    {0x5 ,0x04,66 ,333 ,667},    
#endif /* GNU_SUPPORTS_BINARY_FORMAT */
    {0}
    };

/* Temp base freq until better solution found */

UINT32 csb = 0,lbiu,ddr,spmf,clkDiv,corePll,coreFreq = 0,tempVal;

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*/

char * sysModel (void)
    {
    char * model;
    if ((vxPvrGet() & 0xffff0000) == 0x80830000)
        {
        UINT32 temp;

        temp=vxSvrGet();

        switch(temp & 0xffff0000)
            {
            case 0x80500000:
                 model = SYS_MODEL_8349E;
                 break;
            case 0x80510000:
                 model = SYS_MODEL_8349;
                 break;
            case 0x80520000:
                 model = SYS_MODEL_8347ET;
                 break;
            case 0x80530000:
                 model = SYS_MODEL_8347T;
                 break;
            case 0x80540000:
                 model = SYS_MODEL_8347EP;
                 break;
            case 0x80550000:
                 model = SYS_MODEL_8347P;
                 break;
            case 0x80560000:
                 model = SYS_MODEL_8343E;
                 break;
            case 0x80570000:
                 model = SYS_MODEL_8343;
                 break;
            default:
                 model = "Motorola E300 - Unknown version";
            break;
            }
        }
    else
        {
        model = SYS_MODEL_UNKNOWN;
        }
    return(model);

    }

/*******************************************************************************
*
* sysBspRev - return the bsp version with the revision eg 1.0/<x>
*
* This function returns a pointer to a bsp version with the revision.
* for eg. 1.0/<x>. BSP_REV defined in config.h is concatenated to
* BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwMemInit - initialize and configure system memory.
*
* This routine is called before sysHwInit(). It performs memory auto-sizing
* and updates the system's physical regions table, `sysPhysRgnTbl'. It may
* include the code to do runtime configuration of extra memory controllers.
*
* NOTE: This routine should not be called directly by the user application. It
* cannot be used to initialize interrupt vectors.
*
* RETURNS: N/A
*/

void sysHwMemInit (void)
    {
    /* Call sysPhysMemTop() to do memory autosizing if available */

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () -  RAM_LOW_ADRS);   

    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes features of the MPC834X-ADS board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {

    /* This is a workaround for errata PCI9 */

#ifdef INCLUDE_PCI
#   ifndef REV3_SILICON 

    /* errata not present on rev3 silicon */

    *M83XX_ACR(CCSBAR) |= M83XX_ACR_PARKM_PCI;  /* Park on PCI */
#   endif /* REV3_SILICON */
#else  /* INCLUDE_PCI */
    *M83XX_ACR(CCSBAR) &= ~M83XX_ACR_PARKM_PCI; /* Park PPC core */
#endif /* INCLUDE_PCI */

    /* Initialize Machine check pin */

    vxHid0Set(0x80000000);
    vxMsrSet(vxMsrGet() | 0x00000002);

    /* HID2 [IFEB / IFEC / EBQS / EBPX] See MPC8349 for details */
    /* This will make use of under the hood architecture features */
    
    vxHid2Set(vxHid2Get()|0x04e00000);

    /*Set TSEC/CSB 1:1 clk ratio for performance */
    
    *M83XX_SCCR(CCSBAR)  &= 0x5fffffff ;

    /* set pointer to BAT initialization functions */
    
    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Get the Baud Rate Generator Clock  frequency */

    baudRateGenClk = sysClkFreqGet();

    /* Enable flash writes */
    
    *BCSR1 &= 0xfe;

    /* enable both ethernet ports on the board */
    
    *BCSR0 &= ~0xc0;

    sysLedInit();

    /* 
     * Reset serial channels, the UARTs are setup at this point,
     * polled output can be used.  
     */
       
    *BCSR0 &= ~0x10 ;

    /* Enable the e300 core timebase */

    *M83XX_SPCR(CCSBAR) |= M83XX_SPCR_TBEN;

    /* 
     * put correct memory size in sysPhysMemDesc[1].len for
     * size of local memory
     */
     
    sysHwMemInit();

    hardWareInterFaceInit();

    /* set the TSEC/SEC/USB/PCI 1-1 enabled clocking with CSB */
    /* Do we need to set these and if so in romInit.s */

    CACHE_PIPE_FLUSH();

    /* ifdef PIB, setup PIB */

#ifdef INCLUDE_PIB_SUPPORT
    i2cDrvInit(0,0);
    initPibBoard();
#endif /* INCLUDE_PIB_SUPPORT */

    /* Initialize interrupts */

#ifdef INCLUDE_SECURITY_ENGINE
    /* Initialize the Security Block */
    *QUICC_SECMR (CCSBAR) = 0xfffe0000;    
    *QUICC_SECBR (CCSBAR) = SEC_ENG_BASE_ADRS | 0x1;

    /* SEC/CSB Engine 1:1 clk ratio for performance and disable DR clk */
    
#   ifndef MPC8349EA_MDS
    *M83XX_SCCR(CCSBAR)  &= 0xfdffffff;
#   endif /* MPC8349EA_MDS */

#endif /* INCLUDE_SECURITY_ENGINE */

    }


/*******************************************************************************
*
* sysGetPeripheralBase - provide CCSRBAR from security engine drivers
*
* This routine provides CCSRBAR address from security engine drivers.
*
* RETURNS: CCSBAR
*
* ERRNO
*/

UINT32 sysGetPeripheralBase(void)
    {
    return(CCSBAR);
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

UINT32 sysClkFreqGet()
    {
    int loop;

    /* Read dip switches*/

    tempVal = *M83XX_SPMR(CCSBAR);
    lbiu    = M83XX_SPMR_LBIU_VAL(tempVal);
    ddr     = M83XX_SPMR_DDR_VAL(tempVal);
    spmf    = M83XX_SPMR_SPMF_VAL(tempVal);
    clkDiv  = M83XX_SPMR_CLK_DIV(tempVal);
    corePll = M83XX_SPMR_CLK_PLL(tempVal);

    /* Assume 66MHz oscillator clk */

    for (loop=0;  clkConfTable[loop].spmf != 0; loop++)
        {
        if(spmf == clkConfTable[loop].spmf)
            {
            if (corePll == clkConfTable[loop].corepll)
                {
                coreFreq = clkConfTable[loop].corefreq;
                csb = clkConfTable[loop].csbfreq;
                break;
                }
            }
        }

    /* If loop drops out the bottom then use default BSP value for CSB */
    
    if(clkConfTable[loop].spmf == 0)
        {
        csb = SYS_CLK_FREQ;
        coreFreq = csb * (corePll & 3);
        }
    else
        {
        csb = csb * 1000000;
        coreFreq = coreFreq * 1000000;
        }

    return(csb);

    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Determine installed memory by reading memory control registers
* and calculating if one or 2 chip selects are used for SDRAM.
* Use the address mask and valid bit to determine each bank size.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

    return (physTop) ;
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
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 8);    /* Warm reboot */

    intLock();

    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_TIMESTAMP
    sysTimestampDisable();
#endif /* INCLUDE_TIMESTAMP */

#if (defined(INCLUDE_MPC834X_USB) && defined(INCLUDE_USB))

    /* Make sure the USB controller is stopped. */

    *M83XX_USBCMD(CCSBAR) &= LONGSWAP(~0x1) ;
#endif /* defined(INCLUDE_MPC834X_USB) && defined(INCLUDE_USB) */

    vxMsrSet (0);

    (*pRom) (startType);    /* jump to bootrom entry point */

    return (OK);    /* in case we ever continue from ROM monitor */
    }

/*******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.
*
* RETURNS: N/A
*/
void sysHwInit2 (void)
    {
    
    if (!configured)
    {

    vxbDevInit();

    /* initialize serial interrupts */

    *BCSR1 &= ~0x01;

#ifdef INCLUDE_SIO_UTILS
     sysSerialConnectAll();
#endif /* INCLUDE_SIO_UTILS */

    configured = TRUE;

    taskSpawn("tDevConn", 11, 0, 10000,
               vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

    }
    }

/*******************************************************************************
*
* sysUsDelay - delay for at least x microseconds.
*
* Use the timebase register, running at csb, to delay at least
* the requested number of microseconds.
*
* RETURNS: N/A.
* 
* ERRNO
*
* \NOMANUAL
*
* \INTERNAL
*
* Do not call before sysHwInit determines csb.
*/

void sysUsDelay
    (
    UINT32 delay
    )
    {
    UINT32      hi;         /* current timebase upper 32 bits   */
    UINT32      lo;         /* current timebase lower 32 bits   */
    UINT32      ticks;      /* number of ticks needed for delay */
    FAST UINT32 target_hi;  /* target timebase upper 32 bits    */
    FAST UINT32 target_lo;  /* target timebase lower 32 bits    */

    /* get start time */
    
    vxTimeBaseGet(&hi, &lo);

    /* 
     * Calculate number of ticks for desired elapsed time. Note that the time
     * base register is incremented once every four bus clock cycles
     */
     
    ticks = delay * ((csb + 1000000) / 1000000 / 4);

    /* set target hi and lo, and account for rollover */
    
    target_lo = lo + ticks;                     /* may cause rollover */
    target_hi = hi + (target_lo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
        {
        vxTimeBaseGet(&hi, &lo);
        }
    while (hi < target_hi || (hi == target_hi && lo < target_lo));

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
    uint32_t        delay     /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (UINT32) delay * 1000 );
    }


/*******************************************************************************
*
* sysDelay - Fixed 1ms delay. Just calls sysMsDelay
*
* sysDelay(void)
*
* RETURNS: N/A
*
*/

void sysDelay (void)
    {
    sysMsDelay (1);
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
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* Not applicable for the busless 8260Ads.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*
*/

void sysProcNumSet
    (
    int 	procNum    /* processor number */
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
* SEE ALSO: sysBusToLocalAdrs()
*/

STATUS sysLocalToBusAdrs
    (
    int 	adrsSpace,	/* bus address space where busAdrs resides */
    char *	localAdrs,	/* local address to convert */
    char **	pBusAdrs	/* where to return bus address */
    )
    {
    return (ERROR);
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
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
    (
    int      adrsSpace,     /* bus address space where busAdrs resides */
    char *   busAdrs,       /* bus address to convert */
    char **  pLocalAdrs     /* where to return local address */
    )
    {
    return (ERROR);
    }

/*******************************************************************************
*
* sysBusTas - test and set a location across the bus
*
* This routine does an atomic test-and-set operation across the backplane.
*
* Not applicable for the 8260Ads.
*
* RETURNS: FALSE, always.
*
* SEE ALSO: vxTas()
*/

BOOL sysBusTas
    (
     char * adrs    /* address to be tested-and-set */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* sysBusClearTas - test and clear
*
* This routine is a null function.
*
* RETURNS: N/A
*/

void sysBusClearTas
    (
     volatile char * address    /* address to be tested-and-cleared */
    )
    {
    }

/*******************************************************************************
*
* sysFlashLED - flash one of the signal lamps
*
* This routine will flash the indicated signal lamp for about 1/4 second
*
* RETURNS: N/A
*/

void sysFlashLED
    (
    int LED    /* bitmap of LED to flash */
    )
    {
    sysControlLED(LED, BCSR0_LED_ON);

    taskDelay(15);

    sysControlLED(LED, BCSR0_LED_OFF);

    }

/*******************************************************************************
*
* sysControlLED - control one of the signal lamps
*
* This routine will turn the indicated signal lamp on or off as
* requested.
*
* RETURNS: N/A
*/

void sysControlLED
    (
    int LED,  /* bitmap of LED to control */
    BOOL on   /* if TRUE turn LED OFF; otherwise, turn LED ON */
    )
    {
    BCSR_REG *bcsr = (BCSR_REG *)BCSR_BASE_ADRS;

    CACHE_PIPE_FLUSH();
    
    bcsr->field.led0 = on; /* clearing bit turns LED on */

    }

#ifdef INCLUDE_PCI

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] = {
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL}, /* slot 1 */
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL}, /* slot 2 */
    {PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL}, /* slot 3 */
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL}  /* slot 4 */
};

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*/

STATUS sysPciAutoConfigInclude
    (
    PCI_SYSTEM * pSys,  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,     /* pointer to function in question */
    UINT devVend        /* deviceID/vendorID of device */
    )
    {
    /* Don't want to auto configure the bridge */
    
    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))  
        return(ERROR);
  
    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - PCI autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoConfigIntrAssign
    (
    PCI_SYSTEM * pSys,   /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,      /* pointer to function in question */
    UCHAR pin            /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* 
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin > 0) && (pin < 5)) && 
    (((pLoc->device) - PCI_SLOT1_DEVNO) < NUM_PCI_SLOTS) &&
    (((pLoc->device) - PCI_SLOT1_DEVNO) >= 0))
    {
    tmpChar = 
      sysPciIntRoute [((pLoc->device) - PCI_SLOT1_DEVNO)][(pin-1)] + INUM_IRQ4;
    }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }
#endif /* INCLUDE_PCI */

/*******************************************************************************
*
* sysLedInit - Init LEDs
*
* Intializes LEDs
*
* RETURNS: NONE
*
* ERROR
*/
void sysLedInit(void)
{
    volatile UINT8 *    ptr = BCSR0 ;
    int                 key = intLock();

    /* turn off both LEDs */

    *BCSR7  = 0x33 ;
    *ptr   |= 0x06 ;

    intUnlock(key) ;
}

#ifdef DRV_SIO_NS16550

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

#endif /* DRV_SIO_NS16550 */

#if defined(INCLUDE_USB) && defined(INCLUDE_EHCI)
/******************************************************************************
*
* sysM834xEhciInit - intializes the on-chip EHCI
*
* This routine initializes the USB MPH clock, configures the MPH pins and 
* enables the ports.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

void sysM834xEhciInit(void)
    {

    /* USB/CSB 1:2 clk ratio for performance and disable DR clk */
    
    *M83XX_SCCR(CCSBAR)  &= 0xff8fffff;

    /* Make sure USBMPH pins are available */
    
    *M83XX_SICRL(CCSBAR) = 0x00000000 ;

    /* 256 MB */
    
    *M83XX_SNOOP1(CCSBAR) = 0x0000001b;

    /* Enable ports */
   
#ifdef MPC8349EA_MDS  
    *M83XX_PORTSC1(CCSBAR) = LONGSWAP(0x80800000) ;
#else
    *M83XX_PORTSC1(CCSBAR) = LONGSWAP(0x80000000) ;
#endif /* MPC8349EA_MDS */

    *M83XX_PORTSC2(CCSBAR) = LONGSWAP(0x80000000) ;

    /* Recommended values if not setting repeat mode */ 

    *M83XX_BURSTSIZE(CCSBAR) = LONGSWAP(0x00000808) ;

    /* rd prefetch 32byte base on burst size 8 x 4 */ 

    *M83XX_SI_CTRL(CCSBAR) = 0x00000000;

    /* Recommended prio and age cnt threshold */

    *M83XX_PRI_CTRL(CCSBAR) = 0x0000000c;
    *M83XX_AGE_CNT_THRESH(CCSBAR) = 0x00000040 ;

    /* Enable the phy */
    
    *M83XX_CONTROL(CCSBAR) = 0x00000004 ;
    }

/*******************************************************************************
*
* usbOpAfterReset - operation after usb ehcd controller reset
*
* This routine set MPC8349 USB DR controller to host mode after reset.
*
* RETURNS: N/A
*/

void usbOpAfterReset()
    {
    *M83XX_USBMODE(CCSBAR) |= LONGSWAP(USB_MODE_CM);
    }

#endif /* defined(INCLUDE_USB) & ... */

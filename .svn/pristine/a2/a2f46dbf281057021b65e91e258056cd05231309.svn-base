/* sysLib.c - Freescale rdb8323 board system-dependent library */

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
01n,20jan11,y_y  correct wrong flash size in sysBatDesc[]. (WIND00241512) 
01m,27aug09,x_z  remove qeCpcrReset().
01l,15jul09,b_m  remove QE IO port pin mux codes.
01k,26jun09,e_d  change timer and interrupt legacy driver to vxBus driver
01j,14apr09,x_z  remove _MMU_UBAT_VP from all static BAT items.(WIND00160963)
01i,15dec08,d_l  correct sysUsDelay and sysMsDelay.
01h,11nov08,d_l  enable PCI clk out and code cleanup. (WIND00142426)
01g,18apr08,wap  Clean up initialization of UCC I/O port pins
01f,17mar08,wap  Switch from hEnd qeFcc driver to VxBus UCC driver, remove
                 unneeded BSP method code not needed by new driver, document
                 the IO pin setup for UCC ports and correct the IO
                 configuration and clock routing for UCC2
01e,20sep07,pmr  fix apigen errors
01d,21aug07,mmi  remove vxPowerModeSet() since deprecated
01c,08aug07,agf  remove duplicate variable definitions
01b,12jul07,vik  removed cmdLine.c inclusion
01a,17jan07,pmr  created from mds8360/sysLib.c
*/

/*
DESCRIPTION
This library provides board-specific routines.
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
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>

#include <miiLib.h>

#ifdef INCLUDE_PCI_BUS
#    include <drv/pci/pciConfigLib.h>
#    include <drv/pci/pciIntLib.h>
#    include <drv/pci/pciAutoConfigLib.h>
#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_UCC_VXB_END
#   include <hwif/util/hwMemLib.h>
#   include <../src/hwif/h/hEnd/qeFccHEnd.h>
#endif

/* globals */

#ifdef INCLUDE_PCI_BUS
/* INT LINE TO IRQ assignment for MPC8323RDB board. */

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] = {
    {INUM_IRQ1, 0xff     , 0xff     , 0xff     }, /* USB */
    {INUM_IRQ2, INUM_IRQ3, 0xff     , 0xff     }, /* miniPCI1 */
    {INUM_IRQ3, INUM_IRQ4, INUM_IRQ0, INUM_IRQ2}, /* PCI/miniPCI2 */
};
#endif

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

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M |
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

    /*
     * use for internal registers into data space
     * NOTE! the internal space cannot be cached and should be guarded
     */

    ((CCSBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128K |
    _MMU_UBAT_VS),
    ((CCSBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 - PCI 1 */

    ((0x80000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((0x80000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCI 2 */

    ((0x90000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((0x90000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /*
     * These entries are for the I/D BATs (4-7) on the MPC7455/755/834x/8272/8323.
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

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M |
    _MMU_UBAT_VS),
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

IMPORT void mmuPpcBatInitMPC7x5(UINT32 *pSysBatDesc);
IMPORT FUNCPTR _pSysBatInitFunc;

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, the Board Control
 * and Status registers, the MPC8323 Internal Memory Map, and local FLASH RAM
 * are set here.
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

    /* MPC8323 Device Memory Map */

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

int   sysBus      = BUS;                /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (PPC603) */
char *sysBootLine = BOOT_LINE_ADRS;     /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;       /* catastrophic message area */
int   sysProcNum;                       /* processor number of this CPU */
BOOL  sysVmeEnable = FALSE;             /* by default no VME */
UINT32   baudRateGenClk;

/* forward declarations */

UINT32 sysClkFreqGet(void);
void   sysControlLED(int LED,BOOL on);

#ifdef INCLUDE_FLASH
#   include <mem/flashMem.c>
#   include <mem/nvRamToFlash.c>
#else
#   include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */

#ifdef INCLUDE_I2C
#   include "sysMotI2c.c"
#   include "sysMpc8323I2c.c"
#endif /* INCLUDE_I2C */

#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#   include <vxBusLib.h>
#   include <hwif/vxbus/vxBus.h>
#   include <../src/hwif/h/resource/qeCp.h>
#   include "hwconf.c"
#   ifdef INCLUDE_SIO_UTILS
    IMPORT void sysSerialConnectAll(void);
#   endif
#endif /* INCLUDE_VXBUS */

void sysLedInit(void);

/* defines */

#define ZERO    0

#define SYS_MODEL                   "Freescale MPC8323 RDB"

#define SYS_MODEL_UNKNOWN_PROCESSOR "Not an E300 core - device unknown"
#define SYS_MODEL_UNKNOWN_SYSTEM    "Unknown E300 system"

/* Set ACR to park on PCI bus */

#define M83XX_ACR_PARKM_PCI 0x1

#include "sysNet.c"

/* Clock configuration */

typedef struct
    {
    UINT32 spmf;
    UINT32 corepll;
    UINT32 clkin;
    UINT32 csbfreq;
    UINT32 corefreq;
    } CLK_TABLE;

LOCAL CLK_TABLE clkConfTable[] = {
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
#else
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

LOCAL char * memTop   = NULL;
LOCAL BOOL configured = FALSE;

/* variant to calculate and save system clock configuration */

LOCAL UINT32 csb        = 0;
LOCAL UINT32 coreFreq   = 0;
LOCAL UINT32 lbiu       = 0;
LOCAL UINT32 ddr        = 0;
LOCAL UINT32 spmf       = 0;
LOCAL UINT32 clkDiv     = 0;
LOCAL UINT32 corePll    = 0;

#ifdef INCLUDE_PCI_BUS

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* This routine returns PCI autoconfig status.
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*/

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function */
    UINT devVend                        /* deviceID/vendorID of device */
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
* This routine returns PCI Int assing status.
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function */
    UCHAR pin                           /* contents of int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if (((pin > 0) && (pin < 5))                                &&
        (((pLoc->device) - PCI_SLOT1_DEVNO) < NUM_PCI_SLOTS)    &&
        (((pLoc->device) - PCI_SLOT1_DEVNO) >= 0))
        {
        tmpChar = sysPciIntRoute [((pLoc->device) - PCI_SLOT1_DEVNO)][(pin-1)];
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }
#endif /* INCLUDE_PCI_BUS */

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
    if ((vxPvrGet() & 0xffff0000) == 0x80840000)
        {
        if ((vxSvrGet() & 0xffff0000) == 0x80620000)
            model = SYS_MODEL;
        else
            model = SYS_MODEL_UNKNOWN_SYSTEM;
        }
    else
        model = SYS_MODEL_UNKNOWN_PROCESSOR;

    return(model);

    }

/*******************************************************************************
* sysBspRev - return the bsp version with the revision eg 1.0/<x>
*
* This function returns a pointer to a bsp version with the revision.
* for eg. 1.0/<x>. BSP_REV defined in config.h is concatanated to
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
* NOTE: This routine should not be called directly by the user application.  It
* cannot be used to initialize interrupt vectors.
*
* RETURNS: N/A
*/

void sysHwMemInit (void)
    {

    /* Call sysPhysMemTop() to do memory autosizing if available */

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () - RAM_LOW_ADRS);

    }
/*******************************************************************************
*
* qeIOPortInit - Init values to establish correct IO Port configuration.
*
* The routine sets up the pin connections for the functionality required by
* this BSP. The information is obtained from the chip users maunual and the
* board schematics.
*
* RETURNS: NONE
*
* ERROR
*/

void qeIOPortInit()
    {
    *QE_SDMR(CCSBAR) |=   0x80000000; /* enabling gbl bit for snooping */

    *QE_CMXGCR(CCSBAR) =  QE_CMXGCR_MEM_UCC3;
    *QE_CMXUCR1(CCSBAR) = QE_CMXUCR1_UCC3_RX_CLK9|QE_CMXUCR1_UCC3_TX_CLK10;
    *QE_CMXUCR2(CCSBAR) = 0x00000000;
    *QE_CMXUCR3(CCSBAR) = QE_CMXUCR3_UCC2_RX_CLK16|QE_CMXUCR3_UCC2_TX_CLK3|
                          QE_CMXUCR3_UCC4_RX_CLK7|QE_CMXUCR3_UCC4_TX_CLK8;
    *QE_CMXUPCR(CCSBAR) = 0x00000000;
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes features of the MPC8360-MDS board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: NA
*/

void sysHwInit (void)
    {

#ifdef INCLUDE_PCI_BUS
    *M83XX_OCCR(CCSBAR) = 0xe0000000; /* PCI_OUTPUT_CLK 0-2 */
#endif /* INCLUDE_PCI_BUS */

    /* Initialize Machine check pin and disable icache*/

    vxHid0Set(0x80000000);
    vxMsrSet(vxMsrGet()|0x00000002);

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Enable the e300 core timebase */

    *M83XX_SPCR(CCSBAR) |= M83XX_SPCR_TBEN;

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */

    /*
     * put correct memory size in sysPhysMemDesc[1].len for
     * size of local memory
     */

    sysHwMemInit();

    /*
     * set the TSEC/SEC/USB/PCI 1-1 enabled clocking with CSB
     * Do we need to set these and if so in romInit.s
     */

    CACHE_PIPE_FLUSH();

    /* Get the Baud Rate Generator Clock  frequency */

    baudRateGenClk = sysBaudClkFreq();
    

    /* initialize i2c eeprom interface */

#ifdef INCLUDE_I2C
    i2cDrvInit();
#endif /* INCLUDE_I2C */

    qeIOPortInit();

#ifdef INCLUDE_SECURITY_ENGINE

    /* Initialize the Security Block */

    *QUICC_SECMR (CCSBAR) = 0xfffe0000;
    *QUICC_SECBR (CCSBAR) = SEC_ENG_BASE_ADRS | 0x1;

#endif /* INCLUDE_SECURITY_ENGINE */

    }
/*******************************************************************************
*
* sysGetPeripheralBase   - Provides CCSRBAR address fro security engine
*                          drivers.
*
* RETURNS:
*
* ERRNO
*/

UINT32 sysGetPeripheralBase(void)
    {
    return(CCSBAR);
    }

/*******************************************************************************
*
* sysBaudClkFreq   - get CSB clk.
*
* RETURNS: Get CSB clk
*
* ERRNO
*/

UINT32 sysBaudClkFreq(void)
    {
    return(sysClkFreqGet());
    }

/*******************************************************************************
*
* divisorGet   - get uart's divisor.
*
* RETURNS: calculate the divisor for UART.
*
* ERRNO
*/

UINT32 sysUartDivisorGet
    (
    UINT32 xtal,
    UINT32 baud
    )
    {
    return (xtal / (16 * baud));
    }

/*******************************************************************************
*
* sysClkFreqGet   - get CSB clk.
*
* RETURNS: initialize some global varible to store CSB clk.
*
* ERRNO
*/

UINT32 sysClkFreqGet(void)
    {
    int loop;
    UINT32  tempVal;

    /* Read dip switches*/

    tempVal = *M83XX_SPMR(CCSBAR);
    lbiu    = M83XX_SPMR_LBIU_VAL(tempVal);
    ddr     = M83XX_SPMR_DDR_VAL (tempVal);
    spmf    = M83XX_SPMR_SPMF_VAL(tempVal);
    clkDiv  = M83XX_SPMR_CLK_DIV (tempVal);
    corePll = M83XX_SPMR_CLK_PLL (tempVal);

#ifdef FORCE_DEFAULT_CLK

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
#endif /* FORCE_DEFAULT_CLK */
        {
        csb = SYS_CLK_FREQ;
        coreFreq = csb * (corePll & 3);
        }
#ifdef FORCE_DEFAULT_CLK
    else
        {
        csb = csb * 1000000;
        coreFreq = coreFreq * 1000000;
        }
#endif /* FORCE_DEFAULT_CLK */
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
    LOCAL char * physTop = NULL;

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
     int startType  /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 8);   /* Warm reboot */

    intLock();

    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif /* INCLUDE_AUX_CLK */

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
* RETURNS: NA
*/

void sysHwInit2 (void)
    {

    if (!configured)
        {
#ifdef  INCLUDE_VXBUS
        vxbDevInit();
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif /* INCLUDE_SIO_UTILS */
        configured = TRUE;

#ifdef  INCLUDE_VXBUS
        taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */
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
    UINT32  hi;       /* current timebase upper 32 bits */
    UINT32  lo;       /* current timebase lower 32 bits */
    UINT32  ticks;    /* number of ticks needed for delay */
    FAST UINT32 target_hi;      /* target timebase upper 32 bits */
    FAST UINT32 target_lo;      /* target timebase lower 32 bits */

    /* get start time */

    vxTimeBaseGet(&hi, &lo);

    /*
     * Calculate number of ticks for desired elapsed time. Note that the time
     * base register is incremented once every four bus clock cycles
     */

    ticks = delay * ((csb + 1000000) / 1000000 / 4);

    /* set target hi and lo, and account for rollover */

    target_lo = lo + ticks;     /* may cause rollover */
    target_hi = hi + (target_lo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
        {
        vxTimeBaseGet(&hi, &lo);
        }
    while (hi < target_hi || (hi == target_hi && lo < target_lo));

    return;
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
* RETURNS : NONE
*
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

/* vxbus SIO driver support */

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
* RETURNS: NA
*
* SEE ALSO: sysProcNumGet()
*
*/

void sysProcNumSet
    (
    int     procNum         /* processor number */
    )
    {
    sysProcNum = procNum;
    }

/*******************************************************************************
* sysFlashWriteEnable - enable  writes to flash
*
* This routine enables the writes to cs0.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysFlashWriteDisable()
*/

void sysFlashWriteEnable (void)
    {
    volatile UINT16 * flashBase;
    int i;

    /* Unlock the flash */

    flashBase = (UINT16 *)FLASH_ADRS;
    *flashBase = FLASH28_CMD_LOCK_SETUP & 0xFF;
    *flashBase = FLASH28_CMD_UNLOCK & 0xFF;

    /* Wait for status indication. */

    for (i = 0; i < 0x500000; i++)
        {
        if (*flashBase == (FLASH28F008_STAT_WSMS & 0xFF))
            break;
        }
    if (i == 0x500000)
        printf ("Warning: flash unlock command timed out\n");

    /* Return to read array mode. */

    *flashBase = FLASH28_CMD_RESET & 0xFF;
    }

/*******************************************************************************
* sysFlashWriteDisable - Disable  writes to flash
*
* This routine Disable the writes to cs0.
*
* RETURNS: void
*
* ERRNO
*
* SEE ALSO: sysFlashWriteEnable()
*/

void sysFlashWriteDisable (void)
    {
    volatile UINT16 *flashBase;
    int i;

    /* Lock the flash */

    flashBase = (UINT16 *)FLASH_ADRS;
    *flashBase = FLASH28_CMD_LOCK_SETUP & 0xFF;
    *flashBase = FLASH28_CMD_LOCK & 0xFF;

    /* Wait for status indication. */

    for (i = 0; i < 0x500000; i++)
        {
        if (*flashBase == (FLASH28F008_STAT_WSMS & 0xFF))
            break;
        }
    if (i == 0x500000)
        printf ("Warning: flash lock command timed out\n");

    /* Return to read array mode. */

    *flashBase = FLASH28_CMD_RESET & 0xFF;
    }


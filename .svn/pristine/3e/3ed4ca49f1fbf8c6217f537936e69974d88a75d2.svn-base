/* sysLib.c - Freescale MPC8360 RDK board system-dependent library */

/*
 * Copyright (c) 2007-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,30apr10,dhy  delete _MMU_UBAT_VP attribute(WIND00162540)
01e,27aug09,x_z  remove qeCpcrReset().
01d,15jul09,b_m  remove QE IO port pin mux codes.
01c,18jun08,y_w  Added boot flash programming tool
01b,26dec07,z_l  Updated to vxWorks6.6
01a,15aug07,z_l  Created from mds8360/sysLib.c
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
#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_FLASH
#   include <mem/flashMem.c>
#   include <mem/nvRamToFlash.c>
#else  /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */

#include "sysNet.c"

#ifdef INCLUDE_VXBUS
    IMPORT void hardWareInterFaceInit();
    UINT32 sysClkFreqGet();
#   include <vxBusLib.h>
#   include <hwif/vxbus/vxBus.h>
#   include <../src/hwif/h/resource/qeCp.h>
#   include "hwconf.c"
#   ifdef INCLUDE_SIO_UTILS
        IMPORT void sysSerialConnectAll(void);
#   endif /* INCLUDE_SIO_UTILS */
#endif /* INCLUDE_VXBUS */

#define SYS_MODEL         "Freescale MPC83"
#define SYS_MODEL_8360    "Freescale MPC8360 RDK"
#define SYS_MODEL_UNKNOWN "Not an E300 core - device unknown"

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

typedef struct 
    {
    UINT32 csb;
    UINT32 lbiu;
    UINT32 ddr;
    UINT32 spmf;
    UINT32 clkDiv;
    UINT32 corePll;
    UINT32 coreFreq;
    } CLK_CONF;

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
    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_32M |
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

    /* D BAT 2 - PCI 1 */

    ((0x80000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((0x80000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 4 - PCI 2 */

    ((0x90000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((0x90000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

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

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_32M |
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
 * Total mapped memory      Page Table size
 * -------------------      ---------------
 *        8 Meg              64 K
 *       16 Meg             128 K
 *       32 Meg             256 K
 *       64 Meg             512 K
 *      128 Meg               1 Meg
 *      .                   .
 *      .                   .
 *      .                   .
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
    VM_STATE_MASK_VALID     | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID          | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE      | VM_STATE_MEM_COHERENCY
    },

#ifdef INCLUDE_SECONDARY_DRAM
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    LOCAL_MEM_SIZE2,
    VM_STATE_MASK_VALID     | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID          | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE      | VM_STATE_MEM_COHERENCY
    },
#endif /* INCLUDE_SECONDARY_DRAM */

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

int     sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int     sysBus          = BUS;              /* system bus type (VME_BUS, etc) */
int     sysCpu          = CPU;              /* system CPU type (PPC603) */
char *  sysBootLine     = BOOT_LINE_ADRS;   /* address of boot line */
char *  sysExcMsg       = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum;                         /* processor number of this CPU */
BOOL    sysVmeEnable    = FALSE;            /* by default no VME */
UINT32  baudRateGenClk;
CLK_CONF clkConfValue;

LOCAL CLK_TABLE clkConfTable[] = 
    {
#ifdef GNU_SUPPORTS_BINARY_FORMAT
    {0b101 ,0b0000010,66 ,333333333 ,333333333},
    {0b101 ,0b0100010,66 ,333333333 ,333333333},
    {0b011 ,0b0000100,66 ,200000000 ,400000000},
    {0b011 ,0b0100100,66 ,200000000 ,400000000},
    {0b100 ,0b0000011,66 ,266666666 ,400000000},
    {0b100 ,0b0100011,66 ,266666666 ,400000000},
    {0b101 ,0b0000011,66 ,333333333 ,500000000},
    {0b101 ,0b0100011,66 ,333333333 ,500000000},
    {0b011 ,0b0000101,66 ,200000000 ,500000000},
    {0b100 ,0b0000100,66 ,266666666 ,533333333},
    {0b011 ,0b0000100,66 ,200000000 ,600000000},
    {0b100 ,0b0000101,66 ,266666666 ,666666666},
    {0b101 ,0b0000100,66 ,333333333 ,666666666},
#else
    {0x5 ,0x02,66 ,333 ,333333333},
    {0x5 ,0x22,66 ,333 ,333333333},
    {0x3 ,0x04,66 ,200 ,400000000},
    {0x3 ,0x24,66 ,200 ,400000000},
    {0x4 ,0x03,66 ,266 ,400000000},
    {0x4 ,0x23,66 ,266 ,400000000},
    {0x5 ,0x03,66 ,333 ,500000000},
    {0x5 ,0x23,66 ,333 ,500000000},
    {0x3 ,0x05,66 ,200 ,500000000},
    {0x4 ,0x04,66 ,266 ,533333333},
    {0x3 ,0x04,66 ,200 ,600000000},
    {0x4 ,0x05,66 ,266 ,666666666},
    {0x5 ,0x04,66 ,333 ,666666666},    
#endif
    {0}
    };

LOCAL char * memTop = NULL;
LOCAL BOOL configured = FALSE;

/*
 * mmuPcpBatInitMPC7x5 initializes the standard 4 (0-3) I/D BATs &
 * the additional 4 (4-7) I/D BATs present on the MPC74[45]5 & MPC83xx.
 */

IMPORT void mmuPpcBatInitMPC7x5(UINT32 *pSysBatDesc);
IMPORT FUNCPTR _pSysBatInitFunc;

/* forward declarations */

uint32_t sysDecGet(void);
uint32_t sysBaudClkFreq(void);
UINT32   sysClkFreqGet();

#ifdef INCLUDE_VXBUS
#define MAX_NUM_METHODS 2
char pSysPlbMethodsMem[(sizeof(struct vxbDeviceMethod *))*MAX_NUM_METHODS];
IMPORT struct vxbDeviceMethod * pSysPlbMethods;

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif /* INCLUDE_SIO_UTILS */

#ifdef INCLUDE_UCC_VXB_END
#define IFM_ACTIVE  0x00000002  /* Interface attached to working net */
#define IFM_FDX     0x00100000  /* Force full duplex */

/*******************************************************************************
*
* qeIOPortInitEth - Init values to establish correct IO Port configuration.
*
* The routine sets up the pin connections for the functionality required by 
* this BSP. The information is obtained from the chip users maunual and the 
* board schematics.
*
* RETURNS: NONE
*
* ERROR
*/

void qeIOPortInitEth(void)
    {
    *QE_SDMR(CCSBAR) |=   0x80000000;       /* enabling gbl bit for snooping */

    *QE_CMXUCR1(CCSBAR) = 0x00050000;
    *QE_CMXUCR2(CCSBAR) = 0x00000087;
    *QE_CMXUCR3(CCSBAR) = 0x000600a9;
    }

#endif /* INCLUDE_UCC_VXB_END */

#endif /* INCLUDE_VXBUS */

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

        temp = vxSvrGet();

        switch(temp & 0xffff0000)
            {
            case 0x80480000:
                model = SYS_MODEL_8360;
                break;
            default:
                model = "Freescale E300 - Unknown version";
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

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () -  RAM_LOW_ADRS);
    
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
    /* This is a workaround for errata PCI9 */

    *M83XX_ACR(CCSBAR) |= M83XX_ACR_PARKM_PCI;  /* Park on PCI */

    /* This is primarily for PIB PCI support */

    *M83XX_OCCR(CCSBAR) = 0xe0000000; /* PCI_OUTPUT_CLK 0-2 */
#else  /* INCLUDE_PCI_BUS */
    *M83XX_ACR(CCSBAR) &= ~M83XX_ACR_PARKM_PCI; /* Park PPC core */
#endif /* INCLUDE_PCI_BUS */

    /* Initialize Machine check pin */

    vxHid0Set(0x80000000);
    vxMsrSet(vxMsrGet()|0x00000002);

    /* This will make use of under the hood architecture features */

    vxHid2Set(vxHid2Get()|0x04e00000);

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

    /* set the TSEC/SEC/USB/PCI 1-1 enabled clocking with CSB */

    CACHE_PIPE_FLUSH();

    /* Get the Baud Rate Generator Clock  frequency */

    baudRateGenClk = sysBaudClkFreq();

#ifdef REV_2_SILICON
    /* Check revision anyway */

    if((vxSvrGet() & 0xffff0000) == 0x80480000)
        {
        *(UINT32 *)0xe00014a8 = 0x5d556555;
        }
#endif /* REV_2_SILICON */

#ifdef INCLUDE_UCC_VXB_END
    qeIOPortInitEth();
#endif /* INCLUDE_UCC_VXB_END */

#ifdef INCLUDE_UCC_VXB_END
#   ifdef ETH_OPTION_SUPPORT
    *(UINT32 *)(CCSBAR+0x100400) &= 0xffff8fff;
    *(UINT32 *)(CCSBAR+0x100400) |= 0x00006000;
#   endif /* ETH_OPTION_SUPPORT */
#endif /* INCLUDE_UCC_VXB_END */

#ifdef INCLUDE_SECURITY_ENGINE
    /* Initialize the Security Block */

    *QUICC_SECMR (CCSBAR) = 0xfffe0000;
    *QUICC_SECBR (CCSBAR) = SEC_ENG_BASE_ADRS | 0x1;
#endif /* INCLUDE_SECURITY_ENGINE */
    }

/*******************************************************************************
*
* sysGetPeripheralBase - Provides CCSRBAR address for security engine drivers.
*
* RETURNS: Base address of MPC8360 internal resource
*
* ERRNO
*/

UINT32 sysGetPeripheralBase(void)
    {
    return(CCSBAR);
    }

/*******************************************************************************
*
* sysBaudClkFreq - get the value of clocks
*
* This routine get the clock frequency of csb bus
*
* RETURNS: The clock frequency of csb bus.
*
*/

uint32_t sysBaudClkFreq(void)
    {
    return(sysClkFreqGet());
    }

/*******************************************************************************
*
* sysClkFreqGet - calculate and get the value of clocks
*
* This routine calculate and get the clock frequency of csb bus
*
* RETURNS: The clock frequency of csb bus.
*
*/

UINT32 sysClkFreqGet (void)
    {
    int     loop;
    UINT32  tempVal;

    /* Read dip switches*/

    tempVal = *M83XX_SPMR(CCSBAR);

    clkConfValue.lbiu    = M83XX_SPMR_LBIU_VAL(tempVal);
    clkConfValue.ddr     = M83XX_SPMR_DDR_VAL (tempVal);
    clkConfValue.spmf    = M83XX_SPMR_SPMF_VAL(tempVal);
    clkConfValue.clkDiv  = M83XX_SPMR_CLK_DIV (tempVal);
    clkConfValue.corePll = M83XX_SPMR_CLK_PLL (tempVal);

    /* Assume 66MHz oscillator clk */

    for (loop=0;  clkConfTable[loop].spmf != 0; loop++)
        {
        if(clkConfValue.spmf == clkConfTable[loop].spmf)
            {
            if (clkConfValue.corePll == clkConfTable[loop].corepll)
                {
                clkConfValue.coreFreq = clkConfTable[loop].corefreq;
                clkConfValue.csb = clkConfTable[loop].csbfreq;
                break;
                }
            }
        }

    /* If loop drops out the bottom then use default BSP value for CSB */

    if(clkConfTable[loop].spmf == 0)
        {
        clkConfValue.csb = SYS_CLK_FREQ;
        clkConfValue.coreFreq = clkConfValue.csb * (clkConfValue.corePll & 3);
        }

    return(clkConfValue.csb);
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
#ifdef INCLUDE_VXBUS
        vxbDevInit();
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif /* INCLUDE_SIO_UTILS */
        configured = TRUE;

#ifdef INCLUDE_VXBUS
        taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */
        }
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
    int     adrsSpace,  /* bus address space where busAdrs resides */
    char *  localAdrs,  /* local address to convert */
    char ** pBusAdrs    /* where to return bus address */
    )
    {
    return (ERROR);
    }

#ifdef INCLUDE_PCI_BUS

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*/

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function */
    UINT            devVend         /* deviceID/vendorID of device */
    )
    {
    /* Don't want to auto configure the bridge */

    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
        {
        return(ERROR);
        }

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - PCI autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function */
    UCHAR           pin             /* contents of int pin register */
    )
    {
    UCHAR tmpChar;

    tmpChar = INUM_IRQ3;

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

#endif /* INCLUDE_PCI_BUS */

#if (defined (INCLUDE_VXBUS) && defined (DRV_SIO_NS16550))

/******************************************************************************
*
* bspSerialChanGet - get the SIO_CHAN device associated with duart channel
*
* This routine returns a pointer to the SIO_CHAN device associated
* with a specified serial channel.
*
* RETURNS: always ERROR
*/

SIO_CHAN * bspSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    return((SIO_CHAN *)ERROR);
    }

#endif /* INCLUDE_VXBUS && DRV_SIO_NS16550 */

#ifdef INCLUDE_BOOT_IMAGE_PROGRAM
/*******************************************************************************
*
* flashProgram - program a string into flash
*
* This routine performs a string programming to the given address.
*
* RETURNS: OK or ERROR if program failed.
*/

STATUS flashProgram
    (
    char *  string,     /* string to be copied into flash memory */
    int     strLen,     /* maximum number of bytes to copy       */
    int     offset      /* byte offset into flash memory         */
    )
    {
    /* 
     * sysFlashWrite() using FLASH_ADRS as base, but here, we need set the 
     * base address as FLASH_BASE_ADRS, so we use 
     * FLASH_BASE_ADRS - FLASH_ADRS, the value is "- 0x100000"
     */

    if (sysFlashWrite (FLASH_CAST (string), strLen, offset + \
        (ROM_BASE_ADRS - FLASH_ADRS), FLASH_28F640J3A, 0) == ERROR)
        {
        return (ERROR);
        }

    return OK;
    }

/*******************************************************************************
*
* programFiletoFlash - boot flash programmer
*
* This routine performs a boot flash programming to the given address.
*
* RETURNS: OK or ERROR if program failed.
*/

STATUS programFiletoFlash
    (
    const char *    fileName,   /* File name that want to progam to flash */
    UINT32          nSectors,   /* number of sectors (0x20000) will be erased */
    UINT32          offset      /* offset based on FLASH_BASE_ADRS */
    )
    {
    int     ret, cnt, i, j;
    int     buffSize = 1024;
    char    buf[1024];
    FILE *  fd = fopen (fileName, "r");

    /* Check the status of loading file from ethernet */

    if (fd == NULL)
        {
        printf ("File %s open error!\n", fileName);
        return (ERROR);
        }

    /* Erase boot sectors within 512KB */

    if (nSectors ==0)
        {
        nSectors = 4;       /* erase/program 4 sectors by default */
        }

    printf ("Erase flash from 0x%x to 0x%x, press any key to continue...",\
            ROM_BASE_ADRS + offset, ROM_BASE_ADRS + offset + \
            nSectors * FLASH_SEGMENT_SIZE);
    getchar ();
    printf ("Erasing ...");

    /* calling sysSectorErase() to erase 4 sectors (512KB)*/

    for (i=0; i<nSectors; i++)
        {
        if (sysSectorErase (FLASH_CAST (ROM_BASE_ADRS + offset + \
                            i * FLASH_SEGMENT_SIZE), FLASH_28F640J3A) == ERROR)
            {
            return (ERROR);
            }
        printf (".");
        }

    /* Program boot flash */

    printf ("OK!\nWritting file %s to flash ... \n", fileName);

    cnt = 0;
    j   = 0;

    do
        {
        ret = fread (buf, 1, buffSize, fd);

        flashProgram (buf, buffSize, offset + j * buffSize);

        cnt += ret;
        j++;

        for(i = 0; i < buffSize; i++)
            {
            buf[i]=0;
            }

        printf ("0x%06x\r",cnt);
        } while (ret >= buffSize);

    printf ("Size = 0x%06x (%d), OK!\n", cnt, cnt);
    fclose (fd);

    return OK;
    }
#endif /* INCLUDE_BOOT_IMAGE_PROGRAM */


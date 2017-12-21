/* sysLib.c - Freescale MPC837x MDS board system-dependent library */

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
01g,03mar10,x_z  adjust sysBatDesc[] to fit new PCIe memory map.
01f,20dec09,x_z  add support for booting from vxWorks bootrom and code clean.
01e,10nov09,x_z  fix PCIe and romResident issue.
01d,21feb08,x_s  added SATA support.
01c,15feb08,x_s  added PIB support. (WIND00113447)
01b,20oct07,x_s  modified for mds837x.
01a,30jul07,dtr  adapted from ads834x/sysLib.c.
*/

/*
DESCRIPTION

This library provides board-specific routines.

INCLUDE FILES:
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
#include <math.h>
#include <miiLib.h>
#ifdef INCLUDE_PCI_BUS
#    include <drv/pci/pciConfigLib.h>
#    include <drv/pci/pciIntLib.h>
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_VXBUS
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#endif

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

#define M83XX_BAT_NUM (_MMU_NUM_IBAT + _MMU_NUM_DBAT + _MMU_NUM_EXTRA_IBAT + \
                       _MMU_NUM_EXTRA_DBAT)

UINT32 sysBatDesc [2 * M83XX_BAT_NUM] = {

    /* I BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_32M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW),

    /* I BAT 1 */

    0, 0,

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,

    /* D BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_32M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 1 */

    /* use for internal registers into data space */
    /* NOTE! the internal space cannot be cached and should be guarded */

    ((CCSBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M |
    _MMU_UBAT_VS),
    ((CCSBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

#ifdef DRV_PCIBUS_M83XX

    /* D BAT 2 - PCI Memeory Space */

    ((PCI_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((PCI_MEM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCI IO Space*/

    ((PCI_IO_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((PCI_IO_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#else /* DRV_PCIBUS_M83XX */

    /* D BAT 2 */

    0, 0,

    /* D BAT 3 */

    0, 0,
#endif /* DRV_PCIBUS_M83XX */

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

#ifdef DRV_PCIBUS_M83XX_EX

    /* D BAT 4 - PCIEX1 Config & Memeory Space */

    ((CPU_PCIEX1_CFG_BASE& _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((CPU_PCIEX1_CFG_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 5 - PCIEX1 MemoryIO & IO Space*/

    ((CPU_PCIEX1_MEMIO_ADRS& _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((CPU_PCIEX1_MEMIO_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 6 - PCIEX2 Config & Memeory Space */

    ((CPU_PCIEX2_CFG_BASE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((CPU_PCIEX2_CFG_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 7 - PCIEX2 MemoryIO & IO Space*/

    ((CPU_PCIEX2_MEMIO_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((CPU_PCIEX2_MEMIO_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED)
#else /* DRV_PCIBUS_M83XX_EX */

    /* D BAT 4 */

    0, 0,

    /* D BAT 5 */

    0, 0,

    /* D BAT 6 */

    0, 0,

    /* D BAT 7 */

    0, 0
#endif /* DRV_PCIBUS_M83XX_EX */
    };

/*
 * mmuPcpBatInitMPC7x5 initializes the standard 4 (0-3) I/D BATs &
 * the additional 4 (4-7) I/D BATs present on the MPC74[45]5 & MPC83xx.
 */

IMPORT void    mmuPpcBatInitMPC7x5(UINT32 *);
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
    /* DDR SDRAM */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    },

    {
    /* Board Control and Status Registers */

    (VIRT_ADDR) BCSR_BASE_ADRS,
    (PHYS_ADDR) BCSR_BASE_ADRS,
    BCSRS_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

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

int          sysBus       = BUS;            /* system bus type (VME_BUS, etc) */
int          sysCpu       = CPU;            /* system CPU type (PPC603) */
char *       sysBootLine  = BOOT_LINE_ADRS; /* address of boot line */
char *       sysExcMsg    = EXC_MSG_ADRS;   /* catastrophic message area */
int          sysProcNum;                    /* processor number of this CPU */
BOOL         sysVmeEnable = FALSE;          /* by default no VME */
LOCAL BOOL   configured   = FALSE;
LOCAL char * memTop       = NULL;
UINT32       csb          = 0;
UINT32       coreFreq     = 0;

/* forward declarations */

UINT32   sysClkFreqGet();
void     sysMsDelay(uint32_t);
void     sysUsDelay(UINT32);
void     sysDelay (void);
VOID     serDesPhyInit (UINT8, UINT8);

/* imports */

IMPORT void (*_vxb_delayRtn) ();
IMPORT void (*_vxb_msDelayRtn) (int);
IMPORT void (*_vxb_usDelayRtn) (int);

/* FLASH and NVRAM */

#include "flashMem.c"
#include "nvRamToFlash.c"

/* I2C */

#ifdef INCLUDE_I2C
#   include "sysMotI2c.c"
#   include "sysMpc837xI2c.c"
#endif /* INCLUDE_I2C */

/* RTC */

#ifdef INCLUDE_RTC
#   include "ds1374.c"
#endif /* INCLUDE_RTC */

/* "M" command support */

#include "sysNet.c"

/* vxBus support */

IMPORT void hardWareInterFaceInit();
IMPORT void sysSerialConnectAll();
#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"

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
    {0x6 ,0x02, 66, 396 ,792},
    {0,0,0,0.0}
};

/******************************************************************************
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

    if ((vxPvrGet () & PVR_E300C4_MASK) == PVR_E300C4)
        {
        switch (vxSvrGet () & 0xffff0000)
            {
            case SVR_MPC8379E:
                model = SYS_MODEL_8379E;
                break;
            case SVR_MPC8379:
                model = SYS_MODEL_8379;
                break;
            case SVR_MPC8378E:
                model = SYS_MODEL_8378E;
                break;
            case SVR_MPC8378:
                model = SYS_MODEL_8378;
                break;
            case SVR_MPC8377E:
                model = SYS_MODEL_8377E;
                break;
            case SVR_MPC8377:
                model = SYS_MODEL_8377;
                break;
            default:
                model = "Freescale E300C4 - unknown";
                break;
            }
        }
    else
        {
        model = "Not an E300C4 core - device unknown";
        }

    return (model);
    }

/******************************************************************************
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

/******************************************************************************
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
    BCSR_REG * pBcsr = (BCSR_REG *)BCSR_BASE_ADRS;

    /* initialize BCSR */

    pBcsr->field.flashPrt   = 0;

    pBcsr->field.gethrst    = 1; /* Bring out of reset */
    pBcsr->field.gethrst    = 0;
    pBcsr->field.gethrst    = 1; /* Bring out of reset */
    pBcsr->field.rsvd       = 1;

    pBcsr->field.geth1_en   = 0; /* Enable TSEC */

    pBcsr->field.geth2_en   = 0; /* Enable TSEC */

    pBcsr->field.rs232en    = 0; /* Enable serial */
    pBcsr->field.pciHost    = 1; /* Enable PCI host */
    pBcsr->field.pciArb     = 0;  /* Enable PCI arbiter */

    pBcsr->field.usbUlpi    = 0; /* ULPI */
    pBcsr->field.usbMode    = 1; /* host */
    pBcsr->field.irq0Tri    = 1;
    pBcsr->field.irq1Tri    = 1;
    pBcsr->field.irq2Tri    = 1;
    pBcsr->field.irq3Tri    = 1;
    pBcsr->field.irq4Tri    = 1; /* Required for PIB PCI */
    pBcsr->field.irq5Tri    = 1; /* Required for PIB PCI */
    pBcsr->field.irq6Tri    = 1; /* Required for PIB PCI */
    pBcsr->field.irq7Tri    = 1; /* Required for PIB PCI */
    pBcsr->field.pcieEnable = 1;

    /* configure System I/O Configuration */

    *M83XX_SICRH = 0;

    /* configure Output Buffer Impedance */

    *M83XX_OBIR =
        M83XX_OBIR_RESERVED | M83XX_OBIR_TSEC1OBI_1 | M83XX_OBIR_TSEC2OBI_1;

    /* TSEC1 and TSEC2 clock/csb_clk = 1:1 */

    *M83XX_SCCR =
        ((*M83XX_SCCR) & (~(M83XX_SCCR_TSEC1CM | M83XX_SCCR_TSEC2CM))) |
        (M83XX_SCCR_TSEC1CM_1 | M83XX_SCCR_TSEC2CM_1);

    WRS_ASM("sync;isync");

#if defined(DRV_STORAGE_FSLSATA) || defined(DRV_PCIBUS_M83XX_EX)

    /* initialize SerDes interface */

    if (((vxPvrGet () & PVR_E300C4_MASK) == PVR_E300C4))
        {
        switch (vxSvrGet () & 0xffff0000)
            {
            case SVR_MPC8379E:
            case SVR_MPC8379:

                /* 4 SATA for 2 SreDes */

#ifdef DRV_STORAGE_FSLSATA
                serDesPhyInit (0, SRDSn_PROT_SATA);
                serDesPhyInit (1, SRDSn_PROT_SATA);
#endif /* DRV_STORAGE_FSLSATA */

                break;

            case SVR_MPC8378E:
            case SVR_MPC8378:

                /* 2 PCIe X1 for SreDes2 */

#ifdef DRV_PCIBUS_M83XX_EX
                serDesPhyInit (1, SRDSn_PROT_PEX_X1);
                break;
#endif /* DRV_PCIBUS_M83XX_EX */

            case SVR_MPC8377E:
            case SVR_MPC8377:

                /* 2 SATA for SreDes1 */

#ifdef DRV_STORAGE_FSLSATA
                serDesPhyInit (0, SRDSn_PROT_SATA);
#endif /* DRV_STORAGE_FSLSATA */

               /* 2 PCIe X1 for SreDes2 */

#ifdef DRV_PCIBUS_M83XX_EX
                serDesPhyInit (1, SRDSn_PROT_PEX_X1);
#endif /* DRV_PCIBUS_M83XX_EX */
                break;

            default:
                break;
            }
        }
#endif /* defined(DRV_STORAGE_FSLSATA) || defined(DRV_PCIBUS_M83XX_EX) */

#ifdef DRV_PCIBUS_M83XX

    /* Enable PCI clock outputs */

    *M83XX_OCCR |= M83XX_OCCR_PCICOE_ALL;

#endif /* DRV_PCIBUS_M83XX */

    /* Initialize Machine check pin */

    vxHid0Set (_PPC_HID0_EMCP);
    vxMsrSet (vxMsrGet () | _PPC_MSR_RI);

    /* HID2 [IFEB / IFEC / EBQS / EBPX] See MPC8349 for details */
    /* This will make use of under the hood architecture features */

    vxHid2Set (vxHid2Get () | 0x04e00000);

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Enable the e300 core timebase */

    *M83XX_SPCR |= M83XX_SPCR_TBEN;

    WRS_ASM("sync;isync");

#ifdef DRV_PCIBUS_M83XX_EX

    /* Reset CSB bridge and PCIe controller */

    *M83XX_PECR1 = M83XX_PECR_RC;
    *M83XX_PECR2 = M83XX_PECR_RC;

    sysMsDelay(1);

    WRS_ASM("sync;isync");

     /* Enable PCIe RC interfaces and negate reset  */

    *M83XX_PECR1 |=
        M83XX_PECR_LINK_RST | M83XX_PECR_CBRST| M83XX_PECR_CSR_RST | \
        M83XX_PECR_RC;
    *M83XX_PECR2 |=
        M83XX_PECR_LINK_RST | M83XX_PECR_CBRST | M83XX_PECR_CSR_RST | \
        M83XX_PECR_RC;;
#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef INCLUDE_I2C
    i2cDrvInit (0, 0);

#   ifdef INCLUDE_PIB_SUPPORT
    i2cDrvInit (1, 0);
    initPibBoard (1);
#   endif /* INCLUDE_PIB_SUPPORT */
#endif /* INCLUDE_I2C */

    /* initialize vxBus delay function pointes*/

    _vxb_delayRtn   = (void (*) ()) sysDelay;
    _vxb_msDelayRtn = (void (*) (int)) sysMsDelay;
    _vxb_usDelayRtn = (void (*) (int)) sysUsDelay;

    /* initialize HWIF Pre-Kernel */

    hardWareInterFaceInit ();

    /* set the TSEC/SEC/USB/PCI 1-1 enabled clocking with CSB */
    /* Do we need to set these and if so in romInit.s */

    CACHE_PIPE_FLUSH ();
    }

/***************************************************************************
*
* serDesPhyInit - initialize SerDes Phy interface
*
* This routine initializes the SerDes Phy interface.
*
* RETURNS: N/A
*/

VOID serDesPhyInit
    (
    UINT8 index,/* SerDes PHY index, 0 or 1 */
    UINT8 proto /* protocol for SerDes PHY  */
    )
    {
    UINT32 cnt = 0;

    /* clear Diff pk-pk swing */

    *SRDSn_CR0 (0) &= (~SRDSn_CR0_DPP);

    switch (proto)
        {
        case SRDSn_PROT_SATA:

            /* reset Rx and Tx for lane A and E*/

            *SRDSn_RSTCTL (index) |= SRDSn_RSTCTL_SATA_RESET;
            *SRDSn_RSTCTL (index) &= (~SRDSn_RSTCTL_SATA_RESET);

            *SRDSn_CR1 (index) &= (~SRDSn_CR1_PLLBW);
            *SRDSn_CR2 (index) = 
                      ((*SRDSn_CR2 (index)) & (~SRDSn_CR2_SEIC_MASK)) |
                      SRDSn_CR2_SEIC_SATA;
            *SRDSn_CR3 (index) = SRDSn_CR3_KFR_SATA      | SRDSn_CR3_KPH_SATA |
                                 SRDSn_CR3_SDFM_SATA_PEX | SRDSn_CR3_SDTXL_SATA;
            *SRDSn_CR4 (index) = SRDSn_CR4_RFCKS_100MHz | SRDSn_CR4_PROT_SATA;

            break;

        case SRDSn_PROT_PEX_X1:
        case SRDSn_PROT_PEX_X2:

            *SRDSn_CR1 (index) |= SRDSn_CR1_PLLBW;
            *SRDSn_CR2 (index) =
                      ((*SRDSn_CR2 (index)) & (~SRDSn_CR2_SEIC_MASK)) |
                      SRDSn_CR2_SEIC_PEX;
            *SRDSn_CR3 (index) = SRDSn_CR3_SDFM_SATA_PEX;

            if (proto == SRDSn_PROT_PEX_X2)
                *SRDSn_CR4 (index) =
                        SRDSn_CR4_RFCKS_100MHz | SRDSn_CR4_PROT_PEX |
                        SRDSn_CR4_PLANE_X2;
            else
                *SRDSn_CR4 (index) =
                        SRDSn_CR4_RFCKS_100MHz | SRDSn_CR4_PROT_PEX;

            break;
        default:
            return;
        }

        /* reset SerDes PHY */

        *SRDSn_RSTCTL (index) |= SRDSn_RSTCTL_RST;
        while(((*SRDSn_RSTCTL (index) & SRDSn_RSTCTL_RDONE) == 0) &&
               (cnt < SRDSn_RST_TIMEOUT))
            cnt++;
    }

/***************************************************************************
*
* sysGetPeripheralBase - provide CCSRBAR from security engine drivers
*
* This routine provides CCSRBAR address from security engine drivers.
*
* RETURNS: CCSBAR
*
* ERRNO
*/

UINT32 sysGetPeripheralBase (void)
    {
    return (CCSBAR);
    }

/*******************************************************************************
*
* sysClkFreqGet - return Coherent system Bus clock freq
*
* This function returns the CSB clock freq.
*
* RETURNS: CSB clock freq
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet ()
    {
    UINT32 loop;
    UINT32 tempVal;
    UINT32 lbiu;
    UINT32 ddr;
    UINT32 spmf;
    UINT32 clkDiv;
    UINT32 corePll;

    /* Read dip switches*/

    tempVal = *M83XX_SPMR;
    lbiu    = M83XX_SPMR_LBIU_VAL (tempVal);
    ddr     = M83XX_SPMR_DDR_VAL (tempVal);
    spmf    = M83XX_SPMR_SPMF_VAL (tempVal);
    clkDiv  = M83XX_SPMR_CLK_DIV (tempVal);
    corePll = M83XX_SPMR_CLK_PLL (tempVal);

    /* Assume 66MHz oscillator clk */

    for (loop = 0;  clkConfTable[loop].spmf != 0; loop++)
        {
        if(spmf == clkConfTable[loop].spmf)
            {
            if (corePll == clkConfTable[loop].corepll)
                {
                coreFreq = clkConfTable[loop].corefreq;
                csb      = clkConfTable[loop].csbfreq;
                break;
                }
            }
        }

    /* If loop drops out the bottom then use default BSP value for CSB */

    if(clkConfTable[loop].spmf == 0)
        {
        if(spmf == 0)
		    spmf = 16;

        csb = spmf * OSCILLATOR_FREQ;
        coreFreq = csb * (corePll & 3) >> 1;
        }
    else
        {
        csb = csb * 1000000;
        coreFreq = coreFreq * 1000000;
        }

    return(csb);
    }

#ifdef INCLUDE_SHOW_ROUTINES
/*******************************************************************************
*
* coreShow - show routine for core clocks
*
* This routine shows the core clocks.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreShow ()
    {
    printf("Bus: %dMhz\n",csb / 1000000);
    printf("Core: %dMhz\n",coreFreq / 1000000);
    }
#endif /* INCLUDE_SHOW_ROUTINES */

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

    physTop = (char *) (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

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

/******************************************************************************
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

#ifdef  INCLUDE_CACHE_SUPPORT

#   ifdef  USER_D_CACHE_ENABLE
        cacheDisable(DATA_CACHE);
#   endif /* USER_D_CACHE_ENABLE */

#   ifdef  USER_I_CACHE_ENABLE
        cacheDisable(INSTRUCTION_CACHE);
#   endif /* USER_I_CACHE_ENABLE */

#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif /* INCLUDE_AUX_CLK */

    vxMsrSet (0);

    (*pRom) (startType);    /* jump to bootrom entry point */

    return (OK);    /* in case we ever continue from ROM monitor */
    }

/******************************************************************************
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
#ifdef  INCLUDE_VXBUS
        vxbDevInit();
#endif /* INCLUDE_VXBUS */

        /* initialize serial interrupts */

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif /* INCLUDE_SIO_UTILS */

        configured = TRUE;

#ifdef  INCLUDE_VXBUS
        taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */
        }
    }

/******************************************************************************
*
* sysUsDelay - delay at least the specified amount of time (in microseconds)
*
* This routine will delay for at least the specified amount of time using the
* lower 32 bit "word" of the Time Base register as the timer.  The accuracy of
* the delay increases as the requested delay increases due to a certain amount
* of overhead.  As an example, a requested delay of 10 microseconds is
* accurate within approximately twenty percent, and a requested delay of 100
* microseconds is accurate within approximately two percent.
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
    UINT32    delay        /* length of time in microsec to delay */
    )
    {
    UINT32  hi;                 /* current timebase upper 32 bits */
    UINT32  lo;                 /* current timebase lower 32 bits */
    UINT32  ticks;              /* number of ticks needed for delay */
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
    uint32_t        delay                   /* length of time in MS to delay */
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

void sysDelay (void)
    {
    sysMsDelay (1);
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
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return (sysProcNum);
    }

/******************************************************************************
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
    int     procNum         /* processor number */
    )
    {
    sysProcNum = procNum;
    }


#ifdef DRV_PCIBUS_M83XX_EX

/* INT LINE TO IRQ assignment */

LOCAL UCHAR sysPciEx1IntRoute [NUM_PCIEX1_SLOTS][4] = {
    {PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL}
};

LOCAL UCHAR sysPciEx2IntRoute [NUM_PCIEX2_SLOTS][4] = {
    {PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL}
};

/*******************************************************************************
*
* sysPciEx1AutoconfigInclude - PCIEx1 autoconfig support routine
*
* This routine performs the PCIEx1 auto configuration support function.
*
* RETURNS: OK or ERROR
*/

STATUS sysPciEx1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UINT devVend            /* deviceID/vendorID of device */
    )
    {

    /*  Host controller itself (device number is 0) won't be configured */

    if  (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return (OK);
    }

/*******************************************************************************
*
* sysPciEx2AutoconfigInclude - PCIEx2 autoconfig support routine
*
* This routine performs the PCIEx2 auto configuration support function.
*
* RETURNS: OK or ERROR
*/

STATUS sysPciEx2AutoconfigInclude
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UINT devVend            /* deviceID/vendorID of device */
    )
    {

    /* Host controller itself (device number is 0) won't be configured */

    if  (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return(OK);
    }

/*******************************************************************************
*
* sysPciEx1AutoconfigIntrAssign - autoconfig PCIEx1 interrupt line number
*
* This routine assigns the interrupt line number for the special PCIEx1 int pin.
*
* RETURNS: PCIEx1 interrupt line number for the given pin
*/

UCHAR sysPciEx1AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

#ifdef INCLUDE_PCIEX1_MSI
    tmpChar = INUM_MSIR0;
#else /* INCLUDE_PCIEX1_MSI */
    if ((pin >=1) && (pin < 5))
    {
    tmpChar = sysPciEx1IntRoute [0][pin - 1];
    }
#endif /* INCLUDE_PCIEX1_MSI */ 

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPciEx2AutoconfigIntrAssign - autoconfig PCIEx2 interrupt line number
*
* This routine assigns the interrupt line number for the special PCIEx2 int pin.
*
* RETURNS: PCIEx2 interrupt line number for the given pin
*/

UCHAR sysPciEx2AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

#ifdef INCLUDE_PCIEX2_MSI
    tmpChar = INUM_MSIR0;
#else /* INCLUDE_PCIEX2_MSI */
    if ((pin >=1) && (pin < 5))
    {
    tmpChar = sysPciEx2IntRoute [0][pin - 1];
    }
#endif /* INCLUDE_PCIEX2_MSI */

    return (tmpChar);
    }
#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef DRV_PCIBUS_M83XX

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] = {
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL}, /* slot 1 */
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL}, /* slot 2 */
    {PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL}, /* slot 3 */
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL}, /* slot 4 */
};

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* This routine performs the PCI auto configuration support function.
*
* RETURNS: OK or ERROR
*/

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UINT devVend            /* deviceID/vendorID of device */
    )
    {

    /*
     * Host controller itself (device number is 0) won't be configured;
     */

    if  (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return(OK);
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - autoconfig PCI interrupt line number
*
* This routine assigns the interrupt line number for the special PCI int pin.
*
* RETURNS: PCI interrupt line number for the given pin
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if (((pin >=1) && (pin < 5)) &&
        ((pLoc->device - PCI_SLOT1_DEVNO) < NUM_PCI_SLOTS))
        {
        tmpChar = sysPciIntRoute [pLoc->device - PCI_SLOT1_DEVNO][pin - 1] +
                  INUM_IRQ4;
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }
#endif /* DRV_PCIBUS_M83XX */

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

#if defined(INCLUDE_USB) && defined(INCLUDE_EHCI)
/******************************************************************************
*
* sysM837xEhciInit - intializes the on-chip EHCI
*
* This routine initializes the USB DR clock, configures the DR pins and
* enables the ports.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

void sysM837xEhciInit ()
    {

    /* select USB host mode */

    *M83XX_USBMODE = LONGSWAP (M83XX_USBMODE_CM_HOST);

    /* Make sure USBDR pins are available */

    *M83XX_SICRL =
        ((*M83XX_SICRL) & (~(M83XX_SICRL_USB_A | M83XX_SICRL_USB_B))) |
        (M83XX_SICRL_USB_A_USB | M83XX_SICRL_USB_B_USB);

    /* enable SNOOP for all the 4GB space */

    *M83XX_SNOOP1 = M83XX_SNOOP_SIZE_2GB;
    *M83XX_SNOOP2 = 0x80000000 | M83XX_SNOOP_SIZE_2GB;

    /* rd prefetch 64byte and ignore system bus errors */

    *M83XX_SI_CTRL = M83XX_SI_CTRL_ERR_OFF | M83XX_SI_CTRL_RD_FETCH_64;

    /* Enable ports */

    *M83XX_PORTSC = LONGSWAP (M83XX_PORTSC_PTS_ULPI) ;

    /* Enable the phy */

    *M83XX_CONTROL = M83XX_CONTROL_USB_EN;
    }

/***************************************************************************
*
* usbOpAfterReset - operation after usb ehcd controller reset
*
* set MPC837x usb DR controller to host mode after reset.
*
*/
void usbOpAfterReset ()
    {
    *M83XX_USBMODE |= LONGSWAP (M83XX_USBMODE_CM_HOST) ;
    }

#endif /* defined(INCLUDE_USB) && defined(INCLUDE_EHCI) */


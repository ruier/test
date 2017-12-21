/* sysLib.c - Freescale MPC8315ERDB board system-dependent library */

/*
 * Copyright (c) 2008-2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,29aug11,wyt  Add support for PCI devices with bus number > 0.
01c,11dec09,x_z  Code clean.
01b,15dec08,z_l  Initialize PCI Express local access window
                 registers. (WIND00146982)
01a,05may08,z_l  Created, routines are derived from version 01c
                 of fsl_rdb8313/sysLib.c
*/

/*
DESCRIPTION
This library provides board-specific routines.
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

#define MPC83XX_BAT_NUM     (_MMU_NUM_IBAT + _MMU_NUM_DBAT + \
                             _MMU_NUM_EXTRA_IBAT + _MMU_NUM_EXTRA_DBAT)

UINT32 sysBatDesc [2 * MPC83XX_BAT_NUM] =
    {
    /* I BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
     _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW),

    /* I BAT 1 */

    ((LOCAL_MEM_LOCAL_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128M |
     _MMU_UBAT_VS),
    ((LOCAL_MEM_LOCAL_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_MEM_COHERENT),

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,

    /* D BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
     _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

#ifndef DRV_PCIBUS_M83XX

    /* D BAT 1 */

    0, 0,

#else

    /* D BAT 1 - PCI */

    ((PCI_LAW_BASE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    ((PCI_LAW_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

#endif /* DRV_PCIBUS_M83XX */

#ifndef DRV_PCIBUS_M83XX_EX

    /* D BAT 2 */

    0, 0,

    /* D BAT 3 */

    0, 0,

#else

    /* D BAT 2 - PCIEX memory space */

    ((CPU_PCIEX_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    ((PCIEX_LAW_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCIEX IO space */

    ((CPU_PCIEX_IO_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    (((PCIEX_LAW_BASE + 0x10000000) & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#endif /* DRV_PCIBUS_M83XX_EX */

    /*
     * These entries are for the I/D BATs (4-7)
     * They should be defined in the following order.
     * IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
     * DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
     */

    /* I BAT 4 */

    0, 0,

    /* I BAT 5 */

    0, 0,

    /* I BAT 6 */

    0, 0,

    /* I BAT 7 */

    0, 0,

    /* D BAT 4 */

    0, 0,

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

/* MPC7x5 and 83xx BAT Support */

IMPORT void mmuPpcBatInitMPC7x5 (UINT32 *pSysBatDesc);

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
 *          .                    .
 *          .                    .
 *          .                    .
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 *
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {

    /* memory space */

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT |
    MMU_ATTR_CACHE_COHERENCY
    },

    /* MPC8315 Register Memory Map */

    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    CCSBAR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    }

    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int      sysBus        = BUS;            /* system bus type (VME_BUS, etc) */
int      sysCpu        = CPU;            /* system CPU type (PPC603) */
char *   sysBootLine   = BOOT_LINE_ADRS; /* address of boot line */
char *   sysExcMsg     = EXC_MSG_ADRS;   /* catastrophic message area */
int      sysProcNum;                     /* processor number of this CPU */
CLK_CONF clkConfValue;

/* forward declarations */

/* timer */

UINT32 sysClkFreqGet ();
void   sysMsDelay (uint32_t);
void   sysUsDelay (UINT32);
void   sysDelay (void);

/* vxBus */

#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"

IMPORT void hardWareInterFaceInit ();

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif

/* FLASH */

#include "flashMem.c"
#include "nvRamToFlash.c"

/* M command */

#include "sysNet.c"

/* I2C & RTC */

#ifdef INCLUDE_RTC
#   include "sysMotI2c.c"
#   include "sysMpc8313I2c.c"
#   include "sysRtc.c"
#endif /* INCLUDE_RTC */

/* Clock configuration */

LOCAL CLK_TABLE clkConfTable[] =
    {
    {0x2 ,0x02,66 ,133 ,133},
    {0x3 ,0x22,66 ,200 ,200},
    {0x4 ,0x42,66 ,266 ,266},
    {0x2 ,0x04,66 ,133 ,266},
    {0x3 ,0x24,66 ,200 ,400},
    {0x5, 0x04,33 ,166 ,333},
    {0}
    };

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

    if ((vxPvrGet () & PVR_E300C3_MASK) == PVR_E300C3)
        {
        UINT32 temp;

        temp = vxSvrGet ();

        switch (temp & 0xffff0000)
            {
            case SVR_MPC8315:
                model = SYS_MODEL_8315E;
                break;
            case SVR_MPC8315E:
                model = SYS_MODEL_8315E;
                break;
            default:
                model = "Freescale E300C3 - Unknown version";
                break;
            }
        }
    else
        {
        model = "Not E300 C3 CORE - device unknown";
        }

    return (model);

    }

/*******************************************************************************
*
* sysBspRev - return the bsp version with the revision eg 2.0/<x>
*
* This function returns a pointer to a bsp version with the revision.
* for eg. 2.0/<x>. BSP_REV defined in config.h is concatenated to
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
* sysHwInit - initialize the system hardware
*
* This routine initializes features of board. It sets up the control registers,
* initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {
#ifdef DRV_PCIBUS_M83XX_EX
    volatile int tmp = 0;
#endif

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* configure all clocks */

    *M83XX_SCCR = (SCCR_TSEC1CM | SCCR_TSEC2CM | SCCR_ENCCM |
                   SCCR_USBDRCM | SCCR_PCIEXP1CM | SCCR_PCIEXP2CM |
                   SCCR_PCICM_EN | SCCR_SATA1CM | SCCR_SATA2CM);

    /* Initialize Machine check pin */

    vxHid0Set (0x80000000);
    vxMsrSet (vxMsrGet () | 0x00000002);

    /* HID2 [IFEB / IFEC / EBQS / EBPX] See MPC8315E for details */
    /* This will make use of under the hood architecture features */

    vxHid2Set (vxHid2Get () | 0x04e00000);

    /* Enable the e300 core timebase */

    *M83XX_SPCR |= M83XX_SPCR_TBEN;

    /* calculate clock */

    sysClkFreqGet ();

#ifdef DRV_PCIBUS_M83XX_EX

    /* Close PCI Express 1&2 local access window */

    *M83XX_PCIEXLAWBARn (0) = 0;
    *M83XX_PCIEXLAWARn (0) = 0;
    *M83XX_PCIEXLAWBARn (1) = 0;
    *M83XX_PCIEXLAWARn (1) = 0;

    /* Reset CSB bridge and PCI Express controller */

    *M83XX_PECR2 = M83XX_PECR2_RC;

    sysMsDelay (1);

    /* configure SRDS for PCI Express Controler 2 */

    *SRDSnRSTCTLn (1)  = SRDS_RSTCTL_CNT1 | SRDS_RSTCTL_CNT2 |
                                SRDS_RSTCTL_CNT3 | SRDS_RSTCTL_CNT4;
    *SRDSnCR0 (1)      = SRDS_CR0_RXEQA | SRDS_CR0_RXEQE |
                                SRDS_CR0_TXEQA | SRDS_CR0_TXEQE |
                                SRDS_CR0_IACCA | SRDS_CR0_IACCE;
    *SRDSnCR1 (1)      = 0;
    *SRDSnCR2 (1)      = SRDS_CR2_PCICA;
    *SRDSnCR3 (1)      = SRDS_CR3_SDFMA | SRDS_CR3_SDFME;
    *SRDSnCR4 (1)      = SRDS_CR4_PROTA | SRDS_CR4_PROTE;

    *SRDSnRSTCTLn (1) |= SRDS_RSTCTL_RST;

    /* wait till SerDes reset done */

    while (((*SRDSnRSTCTLn (1) & 0x40000000) == 0) && (tmp < 0x100000))
        tmp++;

    /* Enable PCI EX2 RC interfaces */

    *M83XX_PECR2 = M83XX_PECR2_LINK_RST | M83XX_PECR2_CBRST |
                   M83XX_PECR2_CSR_RST | M83XX_PECR2_RC;

#endif /* DRV_PCIBUS_M83XX_EX */

    /* initialize HWIF Pre-Kernel */

    hardWareInterFaceInit ();

    CACHE_PIPE_FLUSH ();

    /* Initialize I2C (only 1 I2C bus ) */

#ifdef INCLUDE_RTC
    i2cDrvInit (0, 0);
#endif

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine, DEFAULT_BOOT_LINE, strlen (DEFAULT_BOOT_LINE) + 1);
#endif /* FORCE_DEFAULT_BOOT_LINE */
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
    return (CCSBAR);
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

UINT32 sysClkFreqGet (void)
    {
    int     loop;
    UINT32  tempVal;

    /* Read dip switches */

    tempVal = *M83XX_SPMR;

    clkConfValue.lbiu    = M83XX_SPMR_LBIU_VAL (tempVal);
    clkConfValue.ddr     = M83XX_SPMR_DDR_VAL (tempVal);
    clkConfValue.spmf    = M83XX_SPMR_SPMF_VAL (tempVal);
    clkConfValue.clkDiv  = M83XX_SPMR_CLK_DIV (tempVal);
    clkConfValue.corePll = M83XX_SPMR_CLK_PLL (tempVal);

    /* Assume 66MHz oscillator clk */

    for (loop = 0; clkConfTable[loop].spmf != 0; loop++)
        {
        if (clkConfValue.spmf == clkConfTable[loop].spmf)
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

    if (clkConfTable[loop].spmf == 0)
        {
        clkConfValue.csb = SYS_CLK_FREQ;
        clkConfValue.coreFreq = (clkConfValue.csb *
                                 (clkConfValue.corePll & 3)) / 2;
        }
    else
        {
        clkConfValue.csb = clkConfValue.csb * 1000000;
        clkConfValue.coreFreq = clkConfValue.coreFreq * 1000000;
        }

    return (clkConfValue.csb);
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
#if defined(INCLUDE_USB)
    volatile UINT32 tmp;
#endif

    intLock ();

#ifdef  INCLUDE_CACHE_SUPPORT

#   ifdef  USER_I_CACHE_ENABLE
    cacheDisable (INSTRUCTION_CACHE);
#   endif

#   ifdef  USER_D_CACHE_ENABLE
    cacheDisable (DATA_CACHE);
#   endif

#endif

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable ();
#endif

#if defined(INCLUDE_USB)
    /* Make sure the USB controller is stopped. */

    if(!(*M83XX_USBSTS & LONGSWAP (USB_STS_HCH)))
        {
        *M83XX_USBCMD &= LONGSWAP (~USB_CMD_RC_RUN) ;

        sysMsDelay (1);

        do
            {
            tmp = LONGSWAP (*M83XX_USBSTS);
            } while (!(tmp & USB_STS_HCH));
        }
#endif

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
    LOCAL BOOL configured = FALSE;

    if (!configured)
        {
        vxbDevInit ();

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll ();
#endif /* INCLUDE_SIO_UTILS */

        configured = TRUE;

        taskSpawn ("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
        }
    }

/*******************************************************************************
*
* sysUsDelay - delay at least the specified amount of time (in microseconds)
*
* This routine will delay for at least the specified amount of time using the
* timebase register, running at csb.
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

    vxTimeBaseGet (&hi, &lo);

    /*
     * Calculate number of ticks for desired elapsed time. Note that the time
     * base register is incremented once every four bus clock cycles
     */

    ticks = delay * ((clkConfValue.csb + 1000000) / 1000000 / 4);

    /* set target hi and lo, and account for rollover */

    target_lo = lo + ticks;                     /* may cause rollover */
    target_hi = hi + (target_lo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
        {
        vxTimeBaseGet (&hi, &lo);
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
* This routine consumes 1ms of time. It just calls sysMsDelay.
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
* This routine sets the processor number for the CPU board. Processor numbers
* should be unique on a single backplane.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int     procNum            /* processor number */
    )
    {
    sysProcNum = procNum;
    }

#ifdef DRV_PCIBUS_M83XX

/*******************************************************************************
*
* sysPciAutoconfigInclude - define autoconfig devices
*
* This routine defines the devices which support autoconfig
*
* RETURNS: OK or ERROR for bridge.
*/

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC    * pLoc,          /* pointer to function */
    UINT         devVend        /* deviceID/vendorID of device */
    )
    {

    /* Don't want to auto configure the bridge */

    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return OK; /* autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - assign interrupt line number
*
* This routine assigns interrupt line number for pin.
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC    * pLoc,                  /* pointer to function */
    UCHAR        pin                    /* contents of int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if((pin >= 1) && (pin < 5))
        {
        tmpChar  = PCI_IRQ;
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);

    }
#endif /* DRV_PCIBUS_M83XX */

#ifdef DRV_PCIBUS_M83XX_EX

/*******************************************************************************
*
* sysPciExAutoconfigInclude - define autoconfig devices
*
* This routine defines the devices which support autoconfig
*
* RETURNS: OK or ERROR for bridge.
*/

STATUS sysPciExAutoconfigInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC    * pLoc,          /* pointer to function */
    UINT         devVend        /* deviceID/vendorID of device */
    )
    {

    /* Don't want to auto configure the bridge */

    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return OK; /* autoconfigure all devices */
    }


/*******************************************************************************
*
* sysPciExAutoconfigIntrAssign - assign interrupt line number
*
* This routine assigns interrupt line number for pin.
*
* RETURNS: PCI-Express interrupt line number given pin mask
*/

UCHAR sysPciExAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC    * pLoc,                  /* pointer to function */
    UCHAR        pin                    /* contents of int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */

    if ((pin >= 1) && (pin < 5))
        {
        tmpChar = INUM_PEX2_CNT;
        }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }


#endif /* DRV_PCIBUS_M83XX_EX */

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
* sysM8315EhciInit - intialize the on-chip EHCI
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

void sysM8315EhciInit ()
    {
    /* Enable PHY and clock */

    *M83XX_CONTROL = (USB_CRTL_UTMI_PHY_EN | USB_CRTL_REFSEL);

    while ((*M83XX_CONTROL & USB_CRTL_PHY_CLK_VALID) == 0 )
        ;

    /* Host mode */

    *M83XX_USBMODE |= LONGSWAP (USB_MODE_CM) ;

    /* Make sure USBDR pins are available */

    *M83XX_SICRL = 0;

    /* 256 MB snoop range */

    *M83XX_SNOOP1 = 0x0000001b;

    /* Over current detection */

    *M83XX_PORTSC1 = 0x00000010;

    /* Recommended values if not setting repeat mode */

    *M83XX_BURSTSIZE = LONGSWAP (USB_BURSTSIZE_TX | USB_BURSTSIZE_RX);

    /* RD prefetch 32byte base on burst size 8 x 4 */

    *M83XX_SI_CTRL = (USB_SICTRL_ERR_DISABLE);

    /* Recommended prio and age cnt threshold */

    *M83XX_PRI_CTRL = USB_PRICTRL_PRILVL0;
    *M83XX_AGE_CNT_THRESH = USB_AGE_CNT_THRESH;

    /* Enable PHY */

    *M83XX_CONTROL |= 0x00000004;
    }

/*******************************************************************************
*
* usbOpAfterReset - operation after usb ehcd controller reset
*
* This routine set MPC8315 USB DR controller to host mode after reset.
*
* RETURNS: N/A
*/

void usbOpAfterReset ()
    {
    *M83XX_USBMODE |= LONGSWAP (USB_MODE_CM);
    }

#endif /* INCLUDE_USB && ... */


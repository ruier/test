/* sysLib.c - Freescale MPC8308_RDB board system-dependent library */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,20may11,sye  rename DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC. (WIND00240151)
01a,26jun10,syt  derived from version 01f of fsl_rdb8313/sysLib.c
*/

/*
DESCRIPTION

This library provides board-specific routines. The chip drivers included are:

    sysFlash.c          - MX29GL640 flash driver for NVRam.
    nvRamToFlash.c      - MX29GL640 flash driver for NVRam.
    f2xFlashMem.c       - MX29GL649 flash driver for tureFFS.
    sysNandFlash.c      - FreeScale Nand Flash controller support.
    sysMotI2c.c         - FreeScale M83xx I2c controller support.
    sysMpc8313I2c.c     - FreeScale M83xx I2c controller support
    sysRtc.c            - I2c type real time clock device driver.
    usbPciStub.c        - USB Host/Device controller support.
																									
INCLUDE FILES: sysLib.h

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
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>
#include <miiLib.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>

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
     _MMU_UBAT_VS | _MMU_UBAT_VP),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* I BAT 1 */

    ((LOCAL_MEM_LOCAL_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128M |
     _MMU_UBAT_VS | _MMU_UBAT_VP),
    ((LOCAL_MEM_LOCAL_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_MEM_COHERENT),

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,


    /* D BAT 0 */

    0, 0,

    /* D BAT 1 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
     _MMU_UBAT_VS | _MMU_UBAT_VP),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

#ifndef DRV_PCIBUS_M83XX_EX

    /* D BAT 2 */

    0, 0,

    /* D BAT 3 */

    0, 0,

#else /* DRV_PCIBUS_M83XX_EX */

    /* D BAT 2 - PCIEX memory space */

    ((PCIEX_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    ((PCIEX_LAW_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCIEX IO space */

    ((PCIEX_IO_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
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

IMPORT void mmuPpcBatInitMPC7x5(UINT32 *pSysBatDesc);

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

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    RAM_LOW_ADRS,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    LOCAL_MEM_SIZE - RAM_LOW_ADRS,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT |
    MMU_ATTR_CACHE_COHERENCY
    },

    /* MPC8308 Register Memory Map */

    {
    (VIRT_ADDR) IMMR_BASE,
    (PHYS_ADDR) IMMR_BASE,
    IMMR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    /* VSC7385 */

    {
    (VIRT_ADDR) VSC7385_BASE,
    (PHYS_ADDR) VSC7385_BASE,
    VSC7385_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    },

    /* NAND buffer */

#ifdef INCLUDE_NAND_FLASH
    {
    (VIRT_ADDR) NAND_BUFFER_BASE,
    (PHYS_ADDR) NAND_BUFFER_BASE,
    NAND_BUFFER_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_GUARDED
    }
#endif /* INCLUDE_NAND_FLASH */

    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int     sysBus      = BUS_TYPE_NONE;    /* system bus type */
int     sysCpu      = CPU;              /* system CPU type */
char *  sysBootLine = BOOT_LINE_ADRS;   /* address of boot line */
char *  sysExcMsg   = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum  = 0;                /* processor number of this CPU */

/* forward declarations */

uint32_t    sysDecGet (void);
void        sysUsDelay (UINT32 delay);
UINT32      sysClkFreqGet (void);

#ifdef INCLUDE_FLASH
#   include "sysFlash.c"
#   include "nvRamToFlash.c"
#else /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */

#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif

IMPORT void (*_vxb_delayRtn)(void);

#ifdef INCLUDE_NETWORK
#   include "sysNet.c"
#endif /* INCLUDE_NETWORK */

#ifdef INCLUDE_RTC
#   include "sysMotI2c.c"
#   include "sysMpc8313I2c.c"
#   include "sysRtc.c"
#endif /* INCLUDE_RTC */

#ifdef INCLUDE_NAND_FLASH
#   include "sysNandFlash.c"
#endif /* INCLUDE_NAND_FLASH */

#include "hwconf.c"


#define ZERO                    0
#define SYS_MODEL               "Freescale MPC8308"
#define SYS_MODEL_UNKNOWN       "Not E300 C3 CORE - device unknown"

#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000

/* System Clock Domains */

UINT32  sysFreq = SYS_CLK_FREQ; /* system bus clock frequency */
UINT32  localBusFreq;           /* local bus interface unit clock frequency */
UINT32  ddrFreq;                /* DDR controller clock frequency */
UINT32  coreFreq;               /* CPU core clock frequency */
UINT32  corePllRatio;           /* CPU core PLL ratio*/

IMPORT UINT8 _binary_vsc2bin_start[];
IMPORT UINT8 _binary_vsc2bin_end[];

/*******************************************************************************
*
* vsc7385Init - initialize VSC7385 L2 switch
*
* This routine load firmware routine to VSC7385.
*
* RETURNS: OK, or ERROR.
*/

STATUS vsc7385Init (void)
    {
    UINT32 size;
    UINT32 i;
    INT32  retVal = OK;
    VUINT8 * VSC7385_image;

    size = (UINT32)&_binary_vsc2bin_end - (UINT32)&_binary_vsc2bin_start;
    VSC7385_image = _binary_vsc2bin_start;

    *VSC7385_RESET = 3;

    sysUsDelay(200);

    *VSC7385_CTRL = ((1 << 7) | (1 << 3) | (1 << 2) | (1 << 1) | (0 << 0));
    _WRS_ASM(" sync");

    *VSC7385_REG_MAP = 1;
    _WRS_ASM(" sync");

    *VSC7385_ADDR = 0x0;
    _WRS_ASM(" sync");

    for (i = 0; i < size; i++)
        {
        *VSC7385_DATA = VSC7385_image[i];
        _WRS_ASM(" sync");
        }

    *VSC7385_ADDR = 0x0;

    for (i = 0; i < size; i++)
        {
        _WRS_ASM(" sync");

        if (*VSC7385_DATA != VSC7385_image[i])
            {
            retVal = ERROR;
            break;
            }
        }

    *VSC7385_CTRL = ((0 << 7) | (1 << 3) | (0 << 2) | (1 << 1) | (1 << 0));

    sysUsDelay(1000);

    return (retVal);
    }

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

    if ((vxPvrGet() & 0xffff0000) == PVR_E300C3)
        {
        switch(vxSvrGet() & 0xffff0000)
            {
            case SVR_MPC8308:
                model = SYS_MODEL;
                break;
            default:
                model = "Freescale E300C3 - Unknown version";
                break;
            }
        }
    else
        model = SYS_MODEL_UNKNOWN;

    return(model);
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
* sysClkFreqGet - returns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet(void)
    {
    UINT32 spmr;

    spmr = *M83XX_SPMR;

    sysFreq = SYS_CLK_IN_FREQ * M83XX_SPMR_SPMF_VAL(spmr);
    localBusFreq = (sysFreq / (*(CAST(VUINT32 *)ELBC_LCRR) & CLKDIV_MASK));
    ddrFreq = 2 * sysFreq;

    corePllRatio = M83XX_SPMR_CLK_PLL(spmr);

    if (((corePllRatio & 0x60) >> 5) != 0x03)
        {
        switch(corePllRatio & 0x0f)
            {
            case 2:
                corePllRatio = 1;
                break;
            case 3:
                corePllRatio = 1.5;
                break;
            case 4:
                corePllRatio = 2;
                break;
            case 5:
                corePllRatio = 2.5;
                break;
            case 6:
                corePllRatio = 3;
                break;
            }

        coreFreq = corePllRatio * sysFreq;
        }

    return (sysFreq);
    }

#ifdef DRV_STORAGE_SDHC

/*******************************************************************************
*
* sysSdhcClkFreqGet - get eSDHC controller clock frequency
*
* RETURNS: Clock freq of eSDHC controller
*
* ERRNO: N/A
*/
UINT32 sysSdhcClkFreqGet (void)
    {
    return (sysClkFreqGet () * SDHC_CLK_RATIO);
    }

/*******************************************************************************
*
* sysSdhcInit - eSDHC interface initializtion
*
* RETURNS:
*
* ERRNO: N/A
*/

void sysSdhcInit (void)
    {
    /* Configure the clock for SDHC controller */

    *M83XX_SCCR &= ~SDHC_CLK_RATIO_MASK;
    *M83XX_SCCR |= (0x01 << SDHC_CLK_RATIO_SHIFT);

    /* Enable cache snooping in eSDHC controller */

    *M83XX_SDHCCR = SNOOP_ENABLE;

    /* enable eSDHC Pin Function */

    *M83XX_SICRH &= ~SDHC_PIN_MASK;

    }
#endif /* DRV_STORAGE_SDHC */

/*******************************************************************************
*
* sysUsbInterfaceInit - USB ULPI interface initializtion
*
* RETURNS:
*
* ERRNO: N/A
*/
void sysUsbInterfaceInit (void)
    {

    /* Configure the clock for USB controller */

    *M83XX_SCCR &= ~USB_CLK_RATIO_MASK;
    *M83XX_SCCR |= (0x01 << USB_CLK_RATIO_SHIFT);

    /* Configure pin mux for ULPI */

    *M83XX_SICRH &= ~USB_PIN_MASK;
    *M83XX_SICRH |= (0x01 << USB_PIN_MASK_SHIFT);

    *M83XX_CONTROL|= PHY_CLK_SEL_ULPI;

    }

/*******************************************************************************
*
* sysPciExInit - PCI Express Control initializtion
*
* This routine initializes PCIEX and SerDes PHY of board.
*
* RETURNS:
*
* ERRNO: N/A
*/

void sysPciExInit (void)
    {

    /* Close PCI-Ex local access window */

    *PCIEXP1LAWBAR = 0;
    *PCIEXP1LAWAR = 0;

    if (!(*M83XX_PECR1 & PCIEX_RC))
        {

        /* Set PCIEX as root complex mode */

        *M83XX_PECR1 = PCIEX_RC;

        /* Setup SerDes PHY interface */

        *SRDSCR0 &= ~SRDSCR0_DPP_1V2;

        *SRDSCR2 &= ~SRDSCR2_SEIC_MASK;
        *SRDSCR2|= SRDSCR2_SEIC_PEX;

        *SRDSCR3 = SRDSCR3_SDFM_SATA_PEX;

        *SRDSCR4 = (SERDES_CLK_100 | SRDSCR4_PROT_PEX);

        /* Initiate SerDes soft reset */

        *SRDSRSTCTL |= SRDSRSTCTL_RST;
        while (!(*SRDSRSTCTL & SRDSRSTCTL_RDONE));

        /* Enable PCIEX Controller clock */

        *M83XX_SCCR |= PCIEX_CLK_ENABLE;

        /* Deassert the resets in the control register */

        *M83XX_PECR1 |= (PCIEX_LINK_RST | PCIEX_CBRST | PCIEX_CSR_RST);
        }
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

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    vxMsrSet (vxMsrGet() | 0x00000002);

    /*
     * HID2 [IFEB / IFEC / EBQS / EBPX] See MPC8308 Reference Manual for details
     * This will make use of under the hood architecture features
     */

    vxHid2Set(vxHid2Get() | 0x04e00000);

    /* Enable the e300 core timebase */

    *M83XX_SPCR |= M83XX_SPCR_TBEN;

    (void)sysClkFreqGet();

    (void)vsc7385Init();

#if (defined(INCLUDE_EHCI) || defined(INCLUDE_FSL_TCD))

    /* initialize EHCI interface */

    sysUsbInterfaceInit ();

#endif /* (INCLUDE_EHCI || INCLUDE_FSL_TCD) */

#ifdef DRV_STORAGE_SDHC

    /* initialize eSDHC interface */

    sysSdhcInit ();

#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_PCIBUS_M83XX_EX

    sysPciExInit();

#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */

    /*
     * put correct memory size in sysPhysMemDesc[1].len for size of
     * local memory
     */

    sysHwMemInit();

    CACHE_PIPE_FLUSH();

#ifdef INCLUDE_NAND_FLASH

    fcmReset ();

    fcmInit ();

#endif  /* INCLUDE_NAND_FLASH */

#ifdef INCLUDE_RTC
    i2cDrvInit(0, 0);
#endif  /* INCLUDE_RTC */

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine, DEFAULT_BOOT_LINE, strlen(DEFAULT_BOOT_LINE) + 1);
#endif /* FORCE_DEFAULT_BOOT_LINE */

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
    LOCAL char * memTop = NULL;

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
#endif  /* INCLUDE_AUX_CLK */

#if (defined(INCLUDE_EHCI) || defined(INCLUDE_FSL_TCD))

    /* Make sure the USB controller is stopped. */

    *M83XX_USBCMD &= LONGSWAP(~USB_RUN);

#endif  /* (INCLUDE_EHCI || INCLUDE_FSL_TCD) */

#ifdef DRV_PCIBUS_M83XX_EX

    while(*PEX_CSB_STATM & LONGSWAP(IBPIORP | WDMARP));

#endif /* DRV_PCIBUS_M83XX_EX */

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
    UINT32      hi;         /* current timebase upper 32 bits */
    UINT32      lo;         /* current timebase lower 32 bits */
    UINT32      ticks;      /* number of ticks needed for delay */
    FAST UINT32 target_hi;  /* target timebase upper 32 bits */
    FAST UINT32 target_lo;  /* target timebase lower 32 bits */

    /* get start time */

    vxTimeBaseGet(&hi, &lo);

    /*
     * Calculate number of ticks for desired elapsed time. Note that the time
     * base register is incremented once every four bus clock cycles
     */

    ticks = delay * ((sysFreq + 1000000) / 1000000 / 4);

    /* set target hi and lo, and account for rollover */

    target_lo = lo + ticks;                     /* may cause rollover */
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
    sysUsDelay ((UINT32)delay * 1000);
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
    int     procNum            /* processor number */
    )
    {
    sysProcNum = procNum;
    }

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
        tmpChar = INUM_PCIE;
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

#ifdef INCLUDE_SHOW_ROUTINES

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

void coreLbcShow (void)
    {
    VINT32 tmp, tmp2;

    tmp = *(CAST(VUINT32 *)ELBC_BR0);
    tmp2 = *(CAST(VUINT32 *)ELBC_OR0);
    printf("Local bus BR0 = 0x%08x\tOR0 = 0x%08x\n", tmp, tmp2);

    tmp = *(CAST(VUINT32 *)ELBC_BR1);
    tmp2 = *(CAST(VUINT32 *)ELBC_OR1);
    printf("Local bus BR1 = 0x%08x\tOR1 = 0x%08x\n", tmp, tmp2);

    tmp = *(CAST(VUINT32 *)ELBC_BR2);
    tmp2 = *(CAST(VUINT32 *)ELBC_OR2);
    printf("Local bus BR2 = 0x%08x\tOR2 = 0x%08x\n", tmp, tmp2);

    tmp = *(CAST(VUINT32 *)ELBC_BR3);
    tmp2 = *(CAST(VUINT32 *)ELBC_OR3);
    printf("Local bus BR3 = 0x%08x\tOR3 = 0x%08x\n", tmp, tmp2);

    tmp = *(CAST(VUINT32 *)ELBC_LBCR);
    printf("Local bus LBCR = 0x%08x\n", tmp);

    tmp = *(CAST(VUINT32 *)ELBC_LCRR);
    printf("Local bus LCRR = 0x%08x\n", tmp);

    }

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

void cpuSpeedShow (void)
    {

    printf("Oscillator Freq \t = %3d Hz\n", SYS_CLK_IN_FREQ);
    printf("System Bus Clock Freq \t = %3d Hz\n", sysClkFreqGet());
    printf("DDR controller Freq \t = %3d Hz\n", ddrFreq);
    printf("CPU Core Freq \t\t = %3d Hz\n", coreFreq);
    printf("Local Bus Interface Freq = %3d Hz\n", localBusFreq);

    }
#endif  /* INCLUDE_SHOW_ROUTINES */

/* sysLib.c - Freescale MPC830x mitx board system-dependent library */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,13oct11,e_d  modified wrong parameter type for I2C function. (WIND00311227)
01b,12oct11,e_d  set HID2[14] bit for 8306/9 errata CPU-A002. (WIND00307278)
01a,30jun11,e_d  derived from version 01a of fsl_mpc8308_rdb/sysLib.c
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

#ifndef DRV_PCIBUS_M83XX

    /* D BAT 2 */

    0, 0,

    /* D BAT 3 */

    0, 0,

#else /* DRV_PCIBUS_M83XX */

    /* D BAT 2 - PCI memory space */

    ((PCI_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    ((PCI_LAW_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCI IO space */

    ((PCI_IO_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
     _MMU_UBAT_VS),
    (((PCI_LAW_BASE + 0x08000000) & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#endif /* DRV_PCIBUS_M83XX */

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
 * Status registers, the MPC830X Internal Memory Map, and local FLASH RAM are set here.
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

#ifdef BOOT_FROM_NAND

    {
    (VIRT_ADDR) NAND_BOOT_RESADR,
    (PHYS_ADDR) NAND_BOOT_RESADR,
    BOOT_FROM_NAND_MEMRES,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT |
    MMU_ATTR_CACHE_COHERENCY
    },
#endif /* BOOT_FROM_NAND */

    /* MPC8308 Register Memory Map */

    {
    (VIRT_ADDR) IMMR_BASE,
    (PHYS_ADDR) IMMR_BASE,
    IMMR_SIZE,
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
#endif  /* INCLUDE_NAND_FLASH */

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
#else  /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif  /* INCLUDE_FLASH */

#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif /* INCLUDE_SIO_UTILS */

IMPORT void (*_vxb_delayRtn)(void);

#ifdef INCLUDE_NETWORK
#   include "sysNet.c"
#endif  /* INCLUDE_NETWORK */

#ifdef INCLUDE_I2C
#   include "sysMotI2c.c"
#   include "sysMpc8313I2c.c"
int i2cGpioWrite (unsigned char reg, unsigned char val);
#endif  /* INCLUDE_I2C */

#ifdef INCLUDE_NAND_FLASH
#   include "sysNandFlash.c"
#endif  /* INCLUDE_NAND_FLASH */

#include "hwconf.c"

#ifdef INCLUDE_SPI
#   include "sysFslSpi.c"
#endif /* INCLUDE_SPI */

#define ZERO                     0
#define SYS_MODEL                "Freescale"
#define SYS_MODEL_8306           "Freescale MPC8306"
#define SYS_MODEL_8306S          "Freescale MPC8306S"
#define SYS_MODEL_8309           "Freescale MPC8309"
#define SYS_MODEL_UNKNOWN        "Not E300 C3 CORE - device unknown"

#define HID0_MCP                 0x80000000
#define HID1_ABE                 0x00001000
#define HID1_ASTME               0x00002000
#define HID1_RXFE                0x00020000

/* System Clock Domains */

UINT32  sysFreq = SYS_CLK_FREQ; /* system bus clock frequency */
UINT32  localBusFreq;           /* local bus interface unit clock frequency */
UINT32  ddrFreq;                /* DDR controller clock frequency */
UINT32  coreFreq;               /* CPU core clock frequency */
UINT32  corePllRatio;           /* CPU core PLL ratio*/

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
        switch(vxSvrGet())
            {
            case SVR_MPC8306:
                model = SYS_MODEL_8306;
                break;
            case SVR_MPC8306S:
                model = SYS_MODEL_8306S;
                break;
            case SVR_MPC8309:
                model = SYS_MODEL_8309;
                break;
            default:
                model = SYS_MODEL_UNKNOWN;
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
            default:
                corePllRatio = 1;
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

    /*
     * For 8306 som, CAN and SD pin is mixed. We
     * must set pca9536 IO0 pin to low to select
     * SD pin.
     */

#ifndef MPC8309_BOARD
    i2cGpioWrite(I2C_PCA9536_DREG, I2C_PCA9536_DREG_INIT);
#endif /* MPC8309_BOARD */

    }
#endif  /* DRV_STORAGE_SDHC */

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
* sysPciInit - PCI Control initializtion
*
* This routine initializes PCI control with set reset bit on PCI GCR register.
*
* RETURNS:
*
* ERRNO: N/A
*/

void sysPciInit (void)
    {
    *(volatile UINT32 *)(IMMR_BASE + PCI_GCR_OFFSET) = PCI_GCR_RESET_BIT;
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

void qeIOPortInit
    (
    int ucc_num
    )
    {
    UINT32    temp;

     /* enabling gbl bit for snooping */

    *QE_SDMR(IMMR_BASE) |=   QE_SDMR_GLB_1_MSK;

    *QE_CMXUCR1(IMMR_BASE) = 0x00560078;
    *QE_CMXUCR2(IMMR_BASE) = 0x0;
    *QE_CMXUCR3(IMMR_BASE) = 0x009a0000;

    /* Set ethernet MII clock master */

    temp =  *QE_CMXGCR(IMMR_BASE);
    temp &= ~QE_CMXGCR_MII_ENET_MNG_MASK;
    temp |= ((ucc_num - 1) << QE_CMXGCR_MII_ENET_MNG_SHIFT);
    *QE_CMXGCR(IMMR_BASE) = temp;

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

    volatile UINT8 temp= 0x00;

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    vxMsrSet (vxMsrGet() | 0x00000002);

    /*
     * HID2 [IFEB / IFEC / EBQS / EBPX] See MPC8308 Reference Manual for details
     * This will make use of under the hood architecture features.
     * Also set HID2[14] bit for 8306/9 errata CPU-A002
     */

    vxHid2Set(vxHid2Get() | 0x04e60000);

    /* Enable the e300 core timebase */

    *M83XX_SPCR |= M83XX_SPCR_TBEN;

#ifdef INCLUDE_I2C
    i2cDrvInit(0, 0);
    i2cDrvInit(1, 0);
#endif /* INCLUDE_I2C */

#ifdef INCLUDE_UCC_VXB_END
    qeIOPortInit(QE_UCC1);
#endif /* INCLUDE_UCC_VXB_END */

#if (defined(INCLUDE_EHCI) || defined(INCLUDE_FSL_TCD))

    /* initialize EHCI interface */

    sysUsbInterfaceInit ();

#endif /* (INCLUDE_EHCI || INCLUDE_FSL_TCD) */

#ifdef DRV_STORAGE_SDHC

    /* initialize eSDHC interface */

    sysSdhcInit ();
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_PCIBUS_M83XX
    sysPciInit();
    sysMsDelay(1000);
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

    /* Init System I/O Configuration Register 2 for UCC */

    *M83XX_SICRL = M83XX_SICHL_INIDATA;
    *M83XX_SICRH = M83XX_SICHR_INIDATA;

    *M83XX_SDHCCR = M83XX_SDHCCR_INIT;

#ifdef MPC8309_BOARD
    *M83XX_ACR = M83XX_ACR_INIT;
    *M83XX_SPCR = M83XX_SPCR_INIT;
#endif /* MPC8309_BOARD */

#ifdef INCLUDE_NAND_FLASH

    fcmReset ();

    fcmInit ();

#endif  /* INCLUDE_NAND_FLASH */

#ifdef INCLUDE_SPI
    sysSpiInit();
#endif /* INCLUDE_SPI */

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
    VXB_DEVICE_ID pDevTemp;
    FUNCPTR    uccMiiPhyWrite;

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

        /*
         * For MPC8306/9 board, UCC1 is used the PSB6972 switch
         * for phy interface. PHY address range from 0x00 to 0x0F
         * of MDC/MDIO interface is blocked. We should init switch
         * with following code.
         */

        pDevTemp=vxbInstByNameFind("uccMdio",0);

        uccMiiPhyWrite = vxbDevMethodGet (pDevTemp, (UINT32)&miiWrite_desc);
        uccMiiPhyWrite (pDevTemp, (UINT8)0x5, (UINT8)0x1, (UINT16)0x0004);
        uccMiiPhyWrite (pDevTemp, (UINT8)0x7, (UINT8)0x15, (UINT16)0x0373);

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

    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
        return(ERROR);

    return OK; /* autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - assign interrupt line number
*
* This routine assigns interrupt line number for pin.
*
* RETURNS: PCI-Express interrupt line number given pin mask
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

    if ((pin >= 1) && (pin < 5))
        {
        tmpChar = INUM_IRQ2;
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

#ifdef INCLUDE_I2C
/*******************************************************************************
*
* i2cEepromRead - perform interface's I2C read for 24LC256 EEPROM
*
* This function's purpose is to perform I2C read for 24LC256 EEPROM.
*
* RETURNS:  data read from I2C
*
* ERRNO
*/

unsigned char i2cEepromRead
    (
    unsigned int reg
    )
    {
    char eepromDatause;

    i2cRead (0, I2C_EEPROM_ADDR, I2C_DEVICE_TYPE_EEPROM_AT24C256,
             (unsigned int)reg, 1, &eepromDatause);

    return ((unsigned char) eepromDatause);
    }

/*******************************************************************************
*
* i2cEepromWrite - perform interface's I2C write for 24LC256 EEPROM
*
* This function's purpose is to perform I2C write for 24LC256 EEPROM.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*
* ERRNO
*/

int i2cEepromWrite
    (
    unsigned int reg,
    unsigned char val
    )
    {
    return (i2cWrite (0, I2C_EEPROM_ADDR, I2C_DEVICE_TYPE_EEPROM_AT24C256,
              (unsigned int)reg, 1, (char *)&val));
    }

/*******************************************************************************
*
* i2cGpioRead - perform interface's I2C read for PCA9536
*
* This function's purpose is to perform I2C read for PCA9536.
*
* RETURNS:  data read from I2C
*
* ERRNO
*/

unsigned char i2cGpioRead
    (
    unsigned char reg
    )
    {
    char gpioDatause;

    i2cRead (0, I2C_PCA9536_ADDR, I2C_DEVICE_TYPE_GPIO_PCA9536,
             (unsigned int)reg, 1, &gpioDatause);

    return ((unsigned char) gpioDatause);
    }

/*******************************************************************************
*
* i2cGpioWrite - perform interface's I2C write for PCA9536
*
* This function's purpose is to perform I2C write for PCA9536.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*
* ERRNO
*/

int i2cGpioWrite
    (
    unsigned char reg,
    unsigned char val
    )
    {
    return (i2cWrite (0, I2C_PCA9536_ADDR, I2C_DEVICE_TYPE_GPIO_PCA9536,
              (unsigned int)reg, 1, (char *)&val));
    }
#endif

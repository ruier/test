/* sysLib.c - Freescale ADS5121e board system-dependent library */

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
01p,18may10,b_m  add DIU DVI-I support.
01o,30apr10,dhy  delete _MMU_UBAT_VP attribute(WIND00162540)
01n,09oct09,x_f  add NAND via datalight support. (WIND00184589)
01m,04aug09,e_d  add register interval to ATA_RESOURCE. (WIND00171937) 
01l,22jun09,b_m  add more AXE encoder decoder support.
01k,11mar09,w_x  enable USB clock when INCLUDE_FSL_TCD.
01j,26feb09,b_m  add AC'97 and AXE support.
01i,04feb09,w_x  modify enable USB CLK macro condition.
01h,25dec08,x_f  add CAN, PCI and CPLD intCtlr support.
01g,23oct08,x_f  modify sysUsDelay() and remove sysDecDelay()
01f,25aug08,b_m  retrieve board rev in sysModel()
01e,07may08,b_m  correct sysFlashWriteProtect()
01d,12nov07,b_m  add I2C & PATA support.
01c,24sep07,b_m  add fec io port slew rate config.
01b,10sep07,b_m  add clock frequency caculation.
01a,06sep07,b_m  created.
*/

/* includes */

#include <vxWorks.h>
#include <sysLib.h>
#include <stdio.h>
#include <string.h>
#include <memLib.h>
#include <cacheLib.h>
#include <intLib.h>
#include <esf.h>
#include <excLib.h>
#include <logLib.h>
#include <taskLib.h>
#include <usrLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <vmLib.h>

#include <arch/ppc/archPpc.h>
#include <arch/ppc/excPpcLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>

#include "config.h"

#ifdef INCLUDE_ATA
#   include <drv/hdisk/ataDrv.h>
#endif

#ifdef INCLUDE_RTC
#include "m41t00Rtc.c"
#include "sysRtc.c"
#endif

/* defines */

#define ZERO    0

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
 * The BAT configuration for 4xx/6xx-based PPC boards is as follows:
 * All BATs point to PROM/FLASH memory so that end customer may configure
 * them as required.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

UINT32 sysBatDesc[2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT)] =
{
    /* I BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_64M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT),

    /* I BAT 1 */

    0, 0,

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,

    /* D BAT 0 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_64M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 1 */

    ((IMMRBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_4M |
    _MMU_UBAT_VS),
    ((IMMRBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 */

#ifdef DRV_PCIBUS_M83XX
    ((PCI_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((PCI_MEM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#else
    0, 0,
#endif

    /* D BAT 3 */

    0, 0
};

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, memory mapped
 * VME A16 space and local PROM/FLASH are set here.
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
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming  Environments]
 */

PHYS_MEM_DESC sysPhysMemDesc[] =
{
#ifndef DRV_FW_FSL_AXE
    {
        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
        (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
        LOCAL_MEM_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  |
        (MMU_ATTR_CACHE_DEFAULT | MMU_ATTR_CACHE_COHERENCY)
    },
#else
    {
        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
        (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
        (AXE_RSVD_DRAM_BASE - LOCAL_MEM_LOCAL_ADRS),
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  |
        (MMU_ATTR_CACHE_DEFAULT | MMU_ATTR_CACHE_COHERENCY)
    },

    {
        (VIRT_ADDR) AXE_RSVD_DRAM_BASE,
        (PHYS_ADDR) AXE_RSVD_DRAM_BASE,
        AXE_RSVD_DRAM_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  |
        (MMU_ATTR_CACHE_OFF | MMU_ATTR_CACHE_GUARDED)
    },

    {
        (VIRT_ADDR) (AXE_RSVD_DRAM_BASE + AXE_RSVD_DRAM_SIZE),
        (PHYS_ADDR) (AXE_RSVD_DRAM_BASE + AXE_RSVD_DRAM_SIZE),
        (LOCAL_MEM_SIZE - AXE_RSVD_DRAM_SIZE),
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  |
        (MMU_ATTR_CACHE_DEFAULT | MMU_ATTR_CACHE_COHERENCY)
    },
#endif /* DRV_FW_FSL_AXE */

    {
        (VIRT_ADDR) CPLD_BASE_ADRS,
        (PHYS_ADDR) CPLD_BASE_ADRS,
        CPLD_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  |
        (MMU_ATTR_CACHE_OFF | MMU_ATTR_CACHE_GUARDED)
    },

    {
        (VIRT_ADDR) SRAM_BASE_ADRS,
        (PHYS_ADDR) SRAM_BASE_ADRS,
        SRAM_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  |
        (MMU_ATTR_CACHE_OFF | MMU_ATTR_CACHE_GUARDED)
    },

#ifdef INCLUDE_NAND_FLASH
    {
        (VIRT_ADDR) NAND_BASE_ADRS,
        (PHYS_ADDR) NAND_BASE_ADRS,
        NAND_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  |
        (MMU_ATTR_CACHE_OFF | MMU_ATTR_CACHE_GUARDED)
    },
#endif /* INCLUDE_NAND_FLASH */
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* PATA Hard Disk Driver */

#ifdef  INCLUDE_ATA

ATA_RESOURCE ataResources[ATA_MAX_CTRLS] =
{
    {
        {
        0, 0,
        {(int)PATA_IOBA0, (int)PATA_IOBA1}, {0, 0}, 0,
        0, 0, 0, 0, 0
        },
        IDE_LOCAL, ATA_CTRL0_DRIVES, INUM_PATA, INUM_PATA,
        (ATA_PIO_AUTO | ATA_PIO_MULTI | ATA_BITS_16 | ATA_GEO_PHYSICAL),
        ATA_SEM_TIMEOUT_DEF, ATA_WDG_TIMEOUT_DEF, 0, 0, 4
    }
};

/*
 * The first member in the ATA_TYPE struct has a dual purpose.
 *   1) If cylinders == 0, the device location is not probed at startup.
 *   2) If cylinders |= 0, the device location is probed, and if a device
 *      is found, the driver will fill in the first 3 member of the struct
 *      with number of cylinders, number of heads, and sectors per track.
 *
 * The last 2 members of the struct are static and should not be changed.
 *
 * The ATA_TYPE struct has the following layout:
 *   int cylinders;
 *   int heads;
 *   int sectorsTrack;
 *   int bytesSector;
 *   int precomp;
 *
 * NOTE: If configType == ATA_GEO_FORCE, the user must fill in
 * values for cylinders, heads, and sectorsTrack.
 */

ATA_TYPE ataTypes [ATA_MAX_CTRLS][ATA_MAX_DRIVES] =
{
    {
        {ATA_DEV0_STATE, 0, 0, 512, 0xff},  /* ctrl 0, drive 0 */
        {ATA_DEV1_STATE, 0, 0, 512, 0xff},  /* ctrl 0, drive 1 */
    }
};

#endif

#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"

/*
 * This macro returns the positive difference between two unsigned ints.
 * Useful for determining delta between two successive decrementer reads.
 */

#define DELTA(a,b)      (abs((int)a - (int)b))

#define DEC_CLOCK_FREQ  sysGetCsbClock()    /* ppc dec timer base */

/* locals */

int     sysBus          = BUS_TYPE_NONE;    /* system bus type (VME_BUS, etc) */
int     sysCpu          = CPU;              /* system CPU type */
char    *sysBootLine    = BOOT_LINE_ADRS;   /* address of boot line */
char    *sysExcMsg      = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum      = 0;                /* processor number of this CPU */

LOCAL char sysModelStr[64];

/* externals */

IMPORT UINT32 vxHid2Get(void);
IMPORT void vxHid2Set(UINT32);

IMPORT void hardWareInterFaceInit(void);
#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif

#if defined INCLUDE_ATA
IMPORT ATA_CTRL ataCtrl[];
IMPORT VOIDFUNCPTR _func_sysAtaInit;
#endif

/* forward declarations */

LOCAL void sysIOPortInit(void);
LOCAL void sysSocClkEnable(void);
LOCAL void sysFlashWriteProtect(void);
#if (defined INCLUDE_ATA && defined ADS5121E_REV2)
LOCAL void sysPataPowerOn(void);
#endif
#ifdef INCLUDE_WINDML
LOCAL void sysDiuEnable(void);
#endif
void sysUsDelay(UINT delay);
void sysMsDelay(UINT delay);
void sysDelay(void);

/* nvram */

#include "nvRamToFlash.c"
#include "flashMem.c"

/* network */

#include "sysNet.c"

/* i2c */

#ifdef INCLUDE_I2C
#include "sysMpc512xI2c.c"
#include "sysMotI2c.c"
#endif

#ifdef INCLUDE_ATA
#include "sysAta.c"
#endif

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

char * sysModel(void)
    {
    UINT8 revU, revL;

    revU = *(volatile UINT8 *)CPLD_BOARD_ID_UPPER;
    revL = *(volatile UINT8 *)CPLD_BOARD_ID_LOWER;

    sprintf(sysModelStr, "%s (Rev %d.%d)", SYS_MODEL, revU, revL);

    return (sysModelStr);
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

char * sysBspRev(void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the ADS5121e board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: NA
*
* ERRNO: N/A
*/

void sysHwInit(void)
    {

    /* initialize Machine Check Pin */

    vxHid0Set(0x80000000);
    vxMsrSet(vxMsrGet() | 0x00000002);

    /* HID2 [IFEB / IFEC / EBQS / EBPX] */
    /* This will make use of under the hood architecture features */

    vxHid2Set(vxHid2Get() | 0x04e00000);

    /* initialize CSB arbiter */

    *(volatile UINT32 *)ARBITER_ACR |= 0x01;

#ifdef INCLUDE_CAN
    *(volatile UINT8 *)CPLD_NAND_FLASH_CTL |= 0x10; /* enable the CAN */
#endif /* INCLUDE_CAN */

#ifdef INCLUDE_NAND_FLASH
    *(volatile UINT32 *)XLBMEN_NFCBAR = NAND_BASE_ADRS;
#endif

    /* config flash write protect */

    sysFlashWriteProtect();

    /* enable soc clocks */

    sysSocClkEnable();

    /* IO port mux */

    sysIOPortInit();

    /*
     * If mmu tables are used, this is where we would dynamically
     * update the entry describing main memory, using sysPhysMemTop().
     * We must call sysPhysMemTop () at sysHwInit() time to do
     * the memory autosizing if available.
     */

    sysPhysMemTop();

    hardWareInterFaceInit();

#ifdef INCLUDE_I2C
    /* initialize i2c buses */

    i2cDrvInit(0);
    i2cDrvInit(1);
    i2cDrvInit(2);
#endif /* INCLUDE_I2C */

#if (defined INCLUDE_ATA && defined ADS5121E_REV2)
    /* turn on PATA power */

    sysPataPowerOn();

#endif /* #if (defined INCLUDE_ATA && defined ADS5121E_REV2) */

#ifdef INCLUDE_WINDML
    sysDiuEnable();
#endif /* INCLUDE_WINDML */
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

void sysHwInit2(void)
    {
    static int  initialized;

    if (!initialized)
        {
        initialized = TRUE;

        vxbDevInit();

#ifdef INCLUDE_ATA
        sysAtaInit(0);
#endif

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif

#ifdef INCLUDE_RTC
        m41t00RtcInit();
#endif

        taskSpawn("tDevConn", 11, 0, 10000, vxbDevConnect, 0,1,2,3,4,5,6,7,8,9);
        }
    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Normally, the user specifies the amount of physical memory with the
* macro LOCAL_MEM_SIZE in config.h.  BSPs that support run-time
* memory sizing do so only if the macro LOCAL_MEM_AUTOSIZE is defined.
* If not defined, then LOCAL_MEM_SIZE is assumed to be, and must be, the
* true size of physical memory.
*
* NOTE: Do no adjust LOCAL_MEM_SIZE to reserve memory for application
* use.  See sysMemTop() for more information on reserving memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop(void)
    {
    static UINT32 sysPhysMemSize = 0;

    if (sysPhysMemSize == 0)
        {
        /* Don't do auto-sizing, use defined constants */

        sysPhysMemSize = (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
        }

    return (char *)sysPhysMemSize;
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

char * sysMemTop(void)
    {
    static char * memTop = NULL;

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
* only by reboot()--which services ^X--and by bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*/

STATUS sysToMonitor
    (
    int startType   /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR)ROM_WARM_ADRS;

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#endif

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif

    /* Clear the MSR */

    vxMsrSet(0);

    (*pRom)(startType);

    return (OK);    /* in case we ever continue from ROM monitor */
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

int sysProcNumGet(void)
    {
    return sysProcNum;
    }

/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.  It also maps local resources onto
* the VMEbus.
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
    /*
     *  Init global variable - this needs to be done before
     *  calling sysUniverseInit2() because it calls sysProcNumGet()
     *  via the MACRO definition.
     */

    sysProcNum = procNum;
    }

/* clock module */

LOCAL UINT8 sysPllSpmf[] =
    {
    68, 0, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64
    };

/* doubled sysdiv table */

LOCAL UINT8 sysPllDiv[] =
    {
    4,  5,  6,  7,  8,  9,  10, 14, 12, 16, 18, 22,
    20, 24, 26, 30, 28, 32, 34, 38, 36, 40, 42, 46,
    44, 48, 50, 54, 52, 56, 58, 62, 60, 64, 66
    };

LOCAL UINT8 sysDiuDiv[] =
    {
    0, 0, 0, 0,  0,  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0,  0,  0, 0, 0, 0, 3, 0, 9, 0, 0, 0, 0,
    0, 4, 8, 12, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 6, 0, 0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 18, 24, 0, 36
    };

/*******************************************************************************
*
* sysGetCsbClock - get the CSB clock.
*
* This function get the CSB clock.
*
* RETURNS: N/A
*/

UINT32 sysGetCsbClock(void)
    {
    UINT32 reg;
    UINT8  spmf, sysdiv;

    reg = *(volatile UINT32 *)RESET_RCWLR;

    spmf = (reg >> 24) & 0x0F;
    sysdiv = (reg >> 8) & 0x3F;

    return (((SYS_OSC_FREQ * sysPllSpmf[spmf] * 2) / sysPllDiv[sysdiv]) / 2);
    }

/*******************************************************************************
*
* sysGetCoreClock - get the e300 core clock.
*
* This function get the e300 core clock.
*
* RETURNS: N/A
*/

UINT32 sysGetCoreClock(void)
    {
    UINT32 reg;
    UINT8  cpmf;

    reg = *(volatile UINT32 *)RESET_RCWLR;

    cpmf = (reg >> 16) & 0x0F;
    return ((sysGetCsbClock() * cpmf) / 2);
    }

/*******************************************************************************
*
* sysGetIpsClock - get the IPS clock.
*
* This function get the IPS clock.
*
* RETURNS: N/A
*/

UINT32 sysGetIpsClock(void)
    {
    UINT32 reg;
    UINT8  ipsdiv;

    reg = *(volatile UINT32 *)CLOCK_SCFR1;
    ipsdiv = (reg >> 23) & 0x07;
    return (sysGetCsbClock() / ipsdiv);
    }

/*******************************************************************************
*
* sysGetPciClock - get the PCI clock.
*
* This function get the PCI clock.
*
* RETURNS: N/A
*/

UINT32 sysGetPciClock(void)
    {
    UINT32 reg;
    UINT8  pcidiv;

    reg = *(volatile UINT32 *)CLOCK_SCFR1;
    pcidiv = (reg >> 20) & 0x07;
    return (sysGetCsbClock() / pcidiv);
    }

/*******************************************************************************
*
* sysGetMbxGpxClock - get the MBX bus clock.
*
* This function get the MBX bus clock.
*
* RETURNS: N/A
*/

UINT32 sysGetMbxBusClock(void)
    {
    return (sysGetCsbClock() / 2);
    }

/*******************************************************************************
*
* sysGetMbxGpxClock - get the MBX gpx clock.
*
* This function get the MBX gpx clock.
*
* RETURNS: N/A
*/

UINT32 sysGetMbxGpxClock(void)
    {
    UINT32 reg;
    UINT8  mbxdiv;

    reg = *(volatile UINT32 *)CLOCK_SCFR1;
    mbxdiv = (reg >> 14) & 0x07;
    return (sysGetMbxBusClock() / mbxdiv);
    }

/*******************************************************************************
*
* sysGetLpcClock - get the LPC clock.
*
* This function get the LPC clock.
*
* RETURNS: N/A
*/

UINT32 sysGetLpcClock(void)
    {
    UINT32 reg;
    UINT8  lpcdiv;

    reg = *(volatile UINT32 *)CLOCK_SCFR1;
    lpcdiv = (reg >> 11) & 0x07;
    return (sysGetIpsClock() / lpcdiv);
    }

/*******************************************************************************
*
* sysGetNfcClock - get the NFC clock.
*
* This function get the NFC clock.
*
* RETURNS: N/A
*/

UINT32 sysGetNfcClock(void)
    {
    UINT32 reg;
    UINT8  nfcdiv;

    reg = *(volatile UINT32 *)CLOCK_SCFR1;
    nfcdiv = (reg >> 8) & 0x07;
    return (sysGetIpsClock() / nfcdiv);
    }

/*******************************************************************************
*
* sysGetDiuClock - get the DIU clock.
*
* This function get the DIU clock.
*
* RETURNS: N/A
*/

UINT32 sysGetDiuClock(void)
    {
    UINT32 reg;
    UINT8  diudiv;

    reg = *(volatile UINT32 *)CLOCK_SCFR1;
    diudiv = (reg >> 2) & 0x3F;
    return (sysGetCsbClock() / sysDiuDiv[diudiv]);
    }

/*******************************************************************************
*
* sysIOPortInit - init values to establish correct IO Port configuration.
*
* The routine sets up the pin connections for the functionality required by
* this BSP. The information is obtained from the chip users maunual and the
* board schematics.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysIOPortInit(void)
    {

    int no, pin;
#ifdef DRV_PCIBUS_M83XX
    UINT32 addr;
#endif

    /* FEC */

    *(volatile UINT32 *)IOCTL_SPDIF_TXCLK &= ~(IOCTL_FUNMUX_MASK | IOCTL_DS_MASK);
    *(volatile UINT32 *)IOCTL_SPDIF_TXCLK |= (IOCTL_FUNCMUX_FEC | IOCTL_DS_CLASS2);
    *(volatile UINT32 *)IOCTL_SPDIF_TX &= ~(IOCTL_FUNMUX_MASK | IOCTL_DS_MASK);
    *(volatile UINT32 *)IOCTL_SPDIF_TX |= (IOCTL_FUNCMUX_FEC | IOCTL_DS_CLASS2);
    *(volatile UINT32 *)IOCTL_SPDIF_RX &= ~(IOCTL_FUNMUX_MASK | IOCTL_DS_MASK);
    *(volatile UINT32 *)IOCTL_SPDIF_RX |= (IOCTL_FUNCMUX_FEC | IOCTL_DS_CLASS2);

    for (no = 0; no < 3; no++)
        for (pin = 0; pin < 5; pin++)
            {
            *(volatile UINT32 *)IOCTL_PSC(no, pin) &= ~(IOCTL_FUNMUX_MASK | IOCTL_DS_MASK);
            *(volatile UINT32 *)IOCTL_PSC(no, pin) |= (IOCTL_FUNCMUX_FEC | IOCTL_DS_CLASS2);
            }

#ifdef INCLUDE_ATA
    *(volatile UINT32 *)IOCTL_PATA_CE1 |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_CE2 |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_ISOLATE |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_IOR |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_IOW |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_IOCHRDY |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_INTRQ |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_DRQ |= IOCTL_DS_CLASS4;
    *(volatile UINT32 *)IOCTL_PATA_DACK |= IOCTL_DS_CLASS4;
#endif /* INCLUDE_ATA */

#ifdef DRV_PCIBUS_M83XX
    for (no = 0; no < 54; no++)
        {
        addr = IOCTL_BA + 0x0F4 + 4 * no;
        *(volatile UINT32 *) addr = 0x00000000;
        }
#endif /* DRV_PCIBUS_M83XX */

#ifdef INCLUDE_CAN
    *(volatile UINT32 *)IOCTL_CAN1_TX = 0x00000003;
    *(volatile UINT32 *)IOCTL_CAN2_TX = 0x00000003;
#endif /* INCLUDE_CAN */

#ifdef INCLUDE_WINDML
    *(volatile UINT32 *)IOCTL_PSC(5, 0)= 0x00000007;
    *(volatile UINT32 *)IOCTL_PSC(5, 1)= 0x00000003;
    *(volatile UINT32 *)IOCTL_PSC(5, 2)= 0x00000003;
    *(volatile UINT32 *)IOCTL_PSC(5, 3)= 0x00000003;
    *(volatile UINT32 *)IOCTL_PSC(5, 4)= 0x00000003;
#endif /* INCLUDE_WINDML */
    }

/*******************************************************************************
*
* sysSocClkEnable - enable soc module clocks.
*
* The routine sets up the pin connections for the functionality required by
* this BSP. The information is obtained from the chip users maunual and the
* board schematics.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysSocClkEnable(void)
    {
    UINT32 reg32;
    UINT32 clk1 = *(volatile UINT32 *)CLOCK_SCCR1;
    UINT32 clk2 = *(volatile UINT32 *)CLOCK_SCCR2;

    /* default configuration */

    clk1 |= (CLOCK_CFG_EN | CLOCK_LPC_EN | CLOCK_NFC_EN |       \
            CLOCK_PSC3_EN | CLOCK_PSC4_EN | CLOCK_PSC_FIFO_EN | \
            CLOCK_FEC_EN | CLOCK_TPR_EN | CLOCK_DDR_EN);
    clk2 |= (CLOCK_MEM_EN | CLOCK_I2C_EN | CLOCK_MBX_BUS_EN);

    clk2 |= CLOCK_BDLC_EN;  /* if BDLC or MSCAN enabled */

#ifdef INCLUDE_ATA
    clk1 |= (CLOCK_PATA_EN | CLOCK_SATA_EN);
#endif

#ifdef INCLUDE_PCI_BUS
    clk1 |= CLOCK_PCI_EN;
#endif

#if defined (INCLUDE_EHCI) || defined (INCLUDE_FSL_TCD)
    clk2 |= (CLOCK_USB1_EN | CLOCK_USB2_EN);
#endif

#ifdef INCLUDE_WINDML
    clk1 |= CLOCK_PSC5_EN;
    clk2 |= CLOCK_DIU_EN;
#endif

#ifdef DRV_FW_FSL_AXE
    clk2 |= CLOCK_AXE_EN;
#endif

    *(volatile UINT32 *)CLOCK_SCCR1 = clk1;
    *(volatile UINT32 *)CLOCK_SCCR2 = clk2;

    if ((vxPvrGet() & 0x0000FFFF) == MPC5121E_SILICON_REV2)
        {
        /* mscan 1 clock control */

        *(volatile UINT32 *)CLOCK_M1CCR = 0x0;
        *(volatile UINT32 *)CLOCK_M1CCR = 0x3 << 17;
        *(volatile UINT32 *)CLOCK_M1CCR |= 0x10000;

        /* mscan 2 clock control */

        *(volatile UINT32 *)CLOCK_M2CCR = 0x0;
        *(volatile UINT32 *)CLOCK_M2CCR = 0x3 << 17;
        *(volatile UINT32 *)CLOCK_M2CCR |= 0x10000;
        }

    /* Set PCI divider for 33MHz */

    reg32 = *(volatile UINT32 *)CLOCK_SCFR1;
    reg32 &= ~(SCFR1_PCI_DIV_MASK);
    reg32 |= SCFR1_PCI_DIV << SCFR1_PCI_DIV_SHIFT;
    *(volatile UINT32 *)CLOCK_SCFR1 = reg32;
    }

/*******************************************************************************
*
* sysFlashWriteProtect - protect/unprotect the flash part
*
* This routine is called to protect/unprotect the flash part on ADS5121e board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysFlashWriteProtect(void)
    {

    /*
     * write protect the backup flash for security
     * unprotect the boot flash as TFFS may cry (first sector)
     * note: it seems that the CPLD logic for the #WP pin is reversed
     */

    *(volatile UINT8 *)CPLD_NOR_FLASH_CTL =     \
           (CPLD_FLASH_BOOT_WP_FULL_DIS |       \
            CPLD_FLASH_BOOT_WP_SECTOR_DIS |     \
            CPLD_FLASH_BACKUP_WP_FULL_EN |      \
            CPLD_FLASH_BACKUP_WP_SECTOR_EN |    \
            CPLD_FLASH_BOOT_RESET);
    }

#if (defined INCLUDE_ATA && defined ADS5121E_REV2)
/*******************************************************************************
*
* sysPataPowerOn - turn on PATA power
*
* This routine is called to turn on PATA power on ADS5121e board.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysPataPowerOn(void)
    {
    UINT8 data = 0xfb;

    i2cWrite(0, I2C_ADDR_PCF8574A, I2C_DEVICE_TYPE_IOPORT_PCF8574, 0, 1, &data);
    }
#endif /* (defined INCLUDE_ATA && defined ADS5121E_REV2) */

#ifdef INCLUDE_WINDML
/*******************************************************************************
*
* sysDiuEnable - turn on TFP410 for DVI-I digital output
*
* This routine is called to turn on TFP410 for DVI-I digital output.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysDiuEnable(void)
    {
    UINT8 data;

    data = 0xbf;
    i2cWrite(2, I2C_ADDR_TFP410, I2C_DEVICE_TYPE_TFP410, 8, 1, &data);

    data = 0x10;
    i2cWrite(2, I2C_ADDR_TFP410, I2C_DEVICE_TYPE_TFP410, 10, 1, &data);
    }
#endif /* INCLUDE_WINDML */

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
    UINT delay               /* length of time in MS to delay */
    )
    {
    UINT32  hi;              /* current timebase upper 32 bits */
    UINT32  lo;              /* current timebase lower 32 bits */
    UINT32  ticks;           /* number of ticks needed for delay */
    FAST UINT32 targetHi;    /* target timebase upper 32 bits */
    FAST UINT32 targetLo;    /* target timebase lower 32 bits */

    /* get start time */

    vxTimeBaseGet(&hi, &lo);

    /*
     * Calculate number of ticks for desired elapsed time. Note that the time
     * base register is incremented once every four bus clock cycles
     */

    ticks = delay * ((DEC_CLOCK_FREQ + 1000000) / 1000000 / 4);

    /* set target hi and lo, and account for rollover */

    targetLo = lo + ticks;     /* may cause rollover */
    targetHi = hi + (targetLo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
        {
        vxTimeBaseGet(&hi, &lo);
        }
    while (hi < targetHi || (hi == targetHi && lo < targetLo));

    return;
    }

/*******************************************************************************
*
* sysMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by calling sysUsDelay
* with (delay * 1000).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    UINT delay      /* length of time in MS to delay */
    )
    {
    sysUsDelay (delay * 1000);
    }

/*******************************************************************************
*
* sysDelay - delay for approximately one millisecond
*
* Delay for approximately one milli-second.
*
* RETURNS: N/A
*/

void sysDelay(void)
    {
    sysMsDelay(1);
    }

/* vxbus SIO driver support */

#ifdef INCLUDE_SIO_UTILS
#   define BSP_SERIAL_CHAN_GET  bspSerialChanGet
#else
#   define BSP_SERIAL_CHAN_GET  sysSerialChanGet
#endif

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

#ifdef INCLUDE_ATA

/*******************************************************************************
*
* sysIntEnablePIC - enable an ISA/PCI interrupt
*
* This function call is used to enable an ISA/PCI interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*/

STATUS sysIntEnablePIC
    (
    int intNum
    )
    {
    return (intEnable(intNum));
    }

/*******************************************************************************
*
* sysInByteString - reads a string of bytes from an io address.
*
* This function reads a byte string from a specified o address.
*
* RETURNS: N/A
*/

void sysInByteString
    (
    ULONG   ioAddr,
    char *  bufPtr,
    int     nBytes
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nBytes; loopCtr++)
        *bufPtr++ = *(char *)ioAddr;

    WRS_ASM("eieio");
    WRS_ASM("sync");
    }

/*******************************************************************************
*
* sysOutByteString - writes a string of bytes to an io address.
*
* This function writes a byte string to a specified io address.
*
* RETURNS: N/A
*/

void sysOutByteString
    (
    ULONG   ioAddr,
    char *  bufPtr,
    int     nBytes
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nBytes; loopCtr++)
        *(char *)ioAddr = *bufPtr++;

    WRS_ASM("eieio");
    WRS_ASM("sync");
    }

/*******************************************************************************
*
* sysInWordString - reads a string of words from an io address.
*
* This function reads a word string from a specified io address.
*
* RETURNS: N/A
*/

void sysInWordString
    (
    ULONG       ioAddr,
    UINT16 *    bufPtr,
    int         nWords
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *bufPtr++ = *(short *)ioAddr;

    WRS_ASM("eieio");
    WRS_ASM("sync");
    }

/*******************************************************************************
*
* sysInWordStringRev - reads a string of words that are byte reversed
*
* This function reads a string of words that are byte reversed from a
* specified io address.
*
* RETURNS: N/A
*/

void sysInWordStringRev
    (
    ULONG      ioAddr,
    UINT16 *   bufPtr,
    int        nWords
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *bufPtr++ = sysInWord (ioAddr);
    }

/*******************************************************************************
*
* sysOutWordString - writes a string of words to an io address.
*
* This function writes a word string from a specified io address.
*
* RETURNS: N/A
*/

void sysOutWordString
    (
    ULONG       ioAddr,
    UINT16 *    bufPtr,
    int         nWords
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *(short *)ioAddr = *bufPtr++;

    WRS_ASM("eieio");
    WRS_ASM("sync");
    }

/*******************************************************************************
*
* sysInLongString - reads a string of longwords from an io address.
*
* This function reads a longword string from a specified io address.
*
* RETURNS: N/A
*/

void sysInLongString
    (
    ULONG    ioAddr,
    ULONG *  bufPtr,
    int      nLongs
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nLongs; loopCtr++)
        *bufPtr++ = *(int *)ioAddr;

    WRS_ASM("eieio");
    WRS_ASM("sync");
    }

/*******************************************************************************
*
* sysOutLongString - writes a string of longwords to an io address.
*
* This function writes a longword string from a specified io address.
*
* RETURNS: N/A
*/

void sysOutLongString
    (
    ULONG   ioAddr,
    ULONG * bufPtr,
    int     nLongs
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nLongs; loopCtr++)
        *(int *)ioAddr = *bufPtr++;

    WRS_ASM("eieio");
    WRS_ASM("sync");
    }
#endif /* INCLUDE_ATA */

#ifdef INCLUDE_SHOW_ROUTINES

/* debug routines for MPC5121e */

#define XBIT(x, n)      ((x & (1 << (31 - n))) >> (31 - n))

/*******************************************************************************
*
* sysShowReset - show the reset configuration word.
*
* This function show the reset configuration word.
*
* RETURNS: N/A
*/

void sysShowReset(void)
    {
    UINT32 rcwLow, rcwHigh;

    printf("******* RESET Configuration Word Settings *******\n");

    rcwLow  = *(volatile UINT32 *)RESET_RCWLR;
    rcwHigh = *(volatile UINT32 *)RESET_RCWHR;

    printf("PLL Settings: SPMF = %d, CPMF = %d, SYSDIV = %d\n",
        (rcwLow >> 24) & 0x0F, (rcwLow >> 16) & 0x0F, (rcwLow >> 8) & 0x3F);

    printf("PCI %s, %dMHz, Arbiter %s\n", XBIT(rcwHigh, 0) ? "Host" : "Agent",
        XBIT(rcwHigh, 1) ? 66 : 33, XBIT(rcwHigh, 2) ? "Enabled" : "Disabled");
    printf("System Oscillator Bypass? %s\n", XBIT(rcwHigh, 3) ? "No" : "Yes");
    printf("e300 Core %s, Boot Vector @ 0x%08X, %s Endian Mode\n",
        XBIT(rcwHigh, 4) ? "Disabled" : "Enabled",
        XBIT(rcwHigh, 5) ? 0xFFF00100 : 0x00000100,
        XBIT(rcwHigh, 28) ? "Little" : "Big");
    printf("Software Watch Dog %s\n", XBIT(rcwHigh, 8) ? "Enabled" : "Disabled");
    printf("ROM Location @ %s\n", ((rcwHigh >> 20) & 0x03) ? "NAND" : "LPC");
    printf("Factory Test Mode %s\n", XBIT(rcwHigh, 11) ? "Enabled" : "Disabled");
    printf("LPC %s Mode\n", XBIT(rcwHigh, 20) ? "Multiplexed" : "Non-multiplexed");
    printf("LPC Data Port Size = %dbit\n", (((rcwHigh >> 8) & 0x03) + 1) * 8);
    printf("NAND Flash Page Size = %dbytes, Data Port Size = %dbit\n",
        XBIT(rcwHigh, 24) ? 2048 : 512, XBIT(rcwHigh, 25) ? 16 : 8);
    printf("Checkstop Input %s\n", XBIT(rcwHigh, 29) ? "Enabled" : "Disabled");

    printf("NAND info is: %x\n", (((rcwHigh >> 7)& 0x01) << 2) | ((rcwHigh >> 21) & 0x03));
    }

/*******************************************************************************
*
* sysShowClock - show all clocks in the system.
*
* This function show all clocks in the system.
*
* RETURNS: N/A
*/

void sysShowClock(void)
    {
    printf("******* CLOCK Settings *******\n");

    printf("CSB     : %3d MHz\n", sysGetCsbClock() / 1000000);
    printf("CORE    : %3d MHz\n", sysGetCoreClock() / 1000000);
    printf("IPS     : %3d MHz\n", sysGetIpsClock() / 1000000);
    printf("PCI     : %3d MHz\n", sysGetPciClock() / 1000000);
    printf("MBX Bus : %3d MHz\n", sysGetMbxBusClock() / 1000000);
    printf("MBX 3D  : %3d MHz\n", sysGetMbxGpxClock() / 1000000);
    printf("LPC     : %3d MHz\n", sysGetLpcClock() / 1000000);
    printf("NFC     : %3d MHz\n", sysGetNfcClock() / 1000000);
    printf("DIU     : %3d MHz\n", sysGetDiuClock() / 1000000);
    }

#endif /* INCLUDE_SHOW_ROUTINES */

#ifdef DRV_PCIBUS_M83XX

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* This routine performs the PCI auto configuration support function.
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
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
        return (ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - PCI autoconfig support routine
*
* This routine peforms the PCI auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC *    pLoc,          /* pointer to function */
    UCHAR        pin            /* contents of int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* return the value to be assigned to the pin */

    if (pLoc->bus == 0)
        {
        if (pLoc->device == PCI_SLOT1_DEVNO)
            return (PCI_INTA_SLOT1);

        if (pLoc->device == PCI_SLOT2_DEVNO)
            return (PCI_INTA_SLOT2);

        if (pLoc->device == PCI_SLOT3_DEVNO)
            return (PCI_INTA_SLOT3);
        }

    return (tmpChar);
    }

#endif /* DRV_PCIBUS_M83XX */

#ifdef DRV_FW_FSL_AXE

IMPORT STATUS fslAxeServerInit (VXB_DEVICE_ID);
IMPORT void   fslAxeSampleRun (int argc,  char * argv[]);

LOCAL char * axeEncArgv[6];
LOCAL char * axeEncTaskId[AXE_TASK_ENC_NUM] =
    {
    "TASK_MP3ENC", "TASK_MP2ENC", "TASK_OVENC"
    };

LOCAL char * axeDecArgv[7];
LOCAL char * axeDecTaskId[AXE_TASK_DEC_NUM] =
    {
    "TASK_MP3DEC", "TASK_MP2DEC", "TASK_AACDEC", "TASK_WMADEC", "TASK_OVDEC"
    };

/*******************************************************************************
*
* sysAxeInit - initialize the AXE device
*
* This routine initializes the AXE device.
*
* Note: This routine must be called after file system is ready as it requires
* reading AXE scheduler binary given by a file system path.
*
* RETURNS: OK or ERROR if initialization fails.
*/

STATUS sysAxeInit (void)
    {
    return fslAxeServerInit (vxbInstByNameFind ("fslAxe", 0));
    }

/*******************************************************************************
*
* sysAxeEncode - encode an audio file using AXE device
*
* This routine encodes an audio file using AXE device.
*
* RETURNS: OK or ERROR if taskId is wrong.
*/

STATUS sysAxeEncode
    (
    int     taskId,
    char *  codecPath,
    char *  inputFile,
    char *  outputFile
    )
    {
    if (taskId < 0 || taskId >= AXE_TASK_ENC_NUM)
        return ERROR;
    
    axeEncArgv[0] = "axec_codec";
    axeEncArgv[1] = codecPath;
    axeEncArgv[2] = "-t";
    axeEncArgv[3] = axeEncTaskId[taskId];
    axeEncArgv[4] = inputFile;
    axeEncArgv[5] = outputFile;

    fslAxeSampleRun (NELEMENTS(axeEncArgv), axeEncArgv);

    return OK;
    }

/*******************************************************************************
*
* sysAxeDecode - decode an audio file using AXE device
*
* This routine decodes an audio file using AXE device.
*
* RETURNS: OK or ERROR if taskId is wrong.
*/

STATUS sysAxeDecode
    (
    int     taskId,
    char *  codecPath,
    char *  inputFile,
    char *  outputFile
    )
    {
    if (taskId < 0 || taskId >= AXE_TASK_DEC_NUM)
        return ERROR;
    
    axeDecArgv[0] = "axec_codec";
    axeDecArgv[1] = codecPath;
    axeDecArgv[2] = "-t";
    axeDecArgv[3] = axeDecTaskId[taskId];
    axeDecArgv[4] = "-w";
    axeDecArgv[5] = inputFile;
    axeDecArgv[6] = outputFile;

    fslAxeSampleRun (NELEMENTS(axeDecArgv), axeDecArgv);

    return OK;
    }

#endif /* DRV_FW_FSL_AXE */

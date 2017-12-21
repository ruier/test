/* sysLib.c - Freescale Lite5200B board series system-dependent library */

/*
 * Copyright (c) 2004-2009, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02d,07apr13,ylu  Modify the value of XLB_CR register 
                 if USB and PCI are included.
02c,10sep09,x_z  replace vxSVRGet() with vxSvrGet() of vxALib.s 
02b,04aug09,e_d  Add register interval to ATA_RESOURCE. (WIND00171937) 
02a,29jun09,z_l  Add INCLUDE_ATA_SHOW. (WIND00157070)
01z,06oct08,b_m  make DRV_PCI_VIRTUAL work in bootrom.
01y,16apr08,b_m  add vxbus support; code cleanup.
01x,12Oct07,d_l  Solve problem of  bootapp test compilation failure.
                 (WIND00107656)
01w,07aug07,agf  remove sysFlags
01v,18jan07,b_m  Modify sysModel() to use SYS_MODEL; code cleanup.
01u,10jan07,jw   modified from Lite5200 to work on Lite5200B
01t,22feb06,pkr  go back to standard ataDrv
01s,03feb06,pkr  adapt to different frequency jumpers
01r,03feb06,pkr  add support for Lite5200b
01q,03feb06,pkr  add U_BOOT support
01p,02feb06,pkr  add support for MKL build
01o,26jan06,pkr  change USB -> USB11
01n,26jan06,pkr  remove VWARE
01m,26jan06,pkr  merge two RAM blocks into one
01l,26jan06,pkr  remove user access from BAT registers
01k,11aug05,j_b  add wireless LAN support
01j,02jun05,j_b  fix PM_RESERVED_MEM handling in sysMemTop (SPR 109828)
01i,24feb05,j_b  driver cleanup
01h,30dec04,k_p  SPR #99747,removed INCLUDE_NETWORK dependencies
01g,30dec04,k_p  added the error message "Dynamic memory sizing not supported"
                 to display when LOCAL_MEM_AUTOSIZE is defined
01f,29dec04,k_p  remove INCLUDE_NETWORK dependencies (SPR #99747)
01e,27dec04,k_p  teamF1,fix compiler warning, removed sysMemProbeSup()
01d,23dec04,k_p  teamF1,Update for vxWorks 6.0
01c,28may04,bjn  Configuration of XLB Pipeline
01b,05mar04,bjn  Remove m5200Sdma.c from BSP build
01a              Created by Wind River Services.
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    m5200Intr.c        - MPC5200 Interrupt Controller driver
    pciConfigLib.c     - PCI configuration library
    pciAutoConfig.c    - PCI device auto-configuration library
    ataDrv.c           - MPC5200 ATA driver

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
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
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <arch/ppc/excPpcLib.h>

#include "config.h"

#ifdef INCLUDE_PCI
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciIntLib.h>
#endif

#include <drv/dma/m5200Sdma.h>

#ifdef INCLUDE_ATA
#   include <drv/hdisk/ataDrv.h>
#endif /* INCLUDE_ATA */

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
#include "hostLib.h"
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

/* defines */

#define ZERO    0

/* globals */

/*
 * sysSDMA is required by some of the MPC5200 drivers. It's external
 * definition can be found in m5200Sdma.h.
 */

sdma_regs *sysSDMA = (sdma_regs *)(SDMA_BASE_ADRS);

#if defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_GLOBAL_MAP)
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

UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT)] =
    {
        /* I BAT 0 */
#if (FLASH_SIZE == 0x4000000)

        ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_64M |
         _MMU_UBAT_VS),

#elif (FLASH_SIZE == 0x2000000)

        ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_32M |
         _MMU_UBAT_VS ),

#elif (FLASH_SIZE == 0x1000000)

        ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M |
         _MMU_UBAT_VS ),

#elif (FLASH_SIZE == 0x800000)

        ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M  |
         _MMU_UBAT_VS ),

#elif (FLASH_SIZE == 0x400000)

        ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_4M  |
         _MMU_UBAT_VS ),

#elif (FLASH_SIZE == 0x200000)

        ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_2M  |
         _MMU_UBAT_VS ),

#elif (FLASH_SIZE == 0x100000)

        ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M |
         _MMU_UBAT_VS ),
#else

#error "sysBatDesc table: Incorrect FLASH_SIZE "

#endif
        ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW),

        /* I BAT 1 -- disabled */

        ((0 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128K),
        ((0 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_CACHE_INHIBIT),

        /* I BAT 2 -- disabled */

        ((0 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128K),
        ((0 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_CACHE_INHIBIT),

        /* I BAT 3 -- disabled */

        ((0 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128K),
        ((0 & _MMU_LBAT_BRPN_MASK) |  _MMU_LBAT_CACHE_INHIBIT),

        /* D BAT 0 - PCI prefetchable memory = 512MB */

        ((CPU_PCI_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_512M |
         _MMU_UBAT_VS ),

        ((CPU_PCI_MEM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
         _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

        /* D BAT 1 - PCI non-prefetchable memory  = 1GB */

        ((CPU_PCI_NO_PRE_MEM_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1G |
         _MMU_UBAT_VS ),

        ((CPU_PCI_NO_PRE_MEM_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
         _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

        /* D BAT 2 - PCI IO space = 256MB */

        ((CPU_PCI_IO_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
         _MMU_UBAT_VS ),

        ((CPU_PCI_IO_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW  |
         _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

        /* D BAT 3 - MBAR space 0xxxxx0000 to 0xxxx40000 */

        ((MBAR_VALUE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256K | _MMU_UBAT_VS),

        ((MBAR_VALUE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW  |
         _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED)

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

PHYS_MEM_DESC sysPhysMemDesc [] =
{
    /* "Vector Table" and Interrupt Stack */

    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (RAM_LOW_ADRS - LOCAL_MEM_LOCAL_ADRS),

    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
        | VM_STATE_MASK_MEM_COHERENCY,

    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE
        | VM_STATE_MEM_COHERENCY
    },

    /* Local SDRAM  */

    {
    (VIRT_ADDR) RAM_LOW_ADRS,
    (PHYS_ADDR) RAM_LOW_ADRS,
    (LOCAL_MEM_SIZE - (RAM_LOW_ADRS - LOCAL_MEM_LOCAL_ADRS)),

    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
        | VM_STATE_MASK_MEM_COHERENCY,

    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE
        | VM_STATE_MEM_COHERENCY
    },

    /* 8-bit flash */

    {
    (VIRT_ADDR) FLASH_BASE_ADRS,
    (PHYS_ADDR) FLASH_BASE_ADRS,
    FLASH_SIZE,

    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
        | VM_STATE_MASK_GUARDED,

    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
        | VM_STATE_GUARDED
    }
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

#endif /* defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_GLOBAL_MAP) */


#ifdef  INCLUDE_ATA         /* ATA Hard Disk Driver */

ATA_RESOURCE    ataResources[ATA_MAX_CTRLS] =   /* ataDrv.h */
{
    {
     {
     0, 0,
     {(int)ATA_BASE_ADRS0, (int)ATA_BASE_ADRS1}, {0, 0}, 0,
     0, 0, 0, 0, 0
     },
     IDE_LOCAL, ATA_CTRL0_DRIVES, INUM_ATA, INUM_ATA,
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

#endif  /* INCLUDE_ATA */

int    sysBus      = BUS_TYPE_NONE;     /* system bus type (VME_BUS, etc) */
int    sysCpu      = CPU;               /* system CPU type */
char * sysBootLine = BOOT_LINE_ADRS;    /* address of boot line */
char * sysExcMsg   = EXC_MSG_ADRS;      /* catastrophic message area */
int    sysProcNum  = 0;                 /* processor number of this CPU */

/* locals */

LOCAL char wrongCpuMsg[] = WRONG_CPU_MSG;

/* forward declarations */

void    sysCpuCheck (void);
char *  sysPhysMemTop (void);
void    sysDelay (void);            /* delay 1 millisecond */
LOCAL void    sysDecDelay (UINT decTicks);  /* delay N decrementer ticks */
void    sysMsDelay (UINT delay);    /* delay N millisecond(s) */
void    sysUsDelay (UINT delay);    /* delay N microsecond(s) */

#if defined(INCLUDE_END) && defined(BOOT_FROM_UBOOT)
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
void sysBootlineFromUBoot (void);
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

/* externals */

IMPORT UCHAR  sysInByte (ULONG);
IMPORT void   sysOutByte (ULONG, UCHAR);
IMPORT STATUS sysSdmaInit (void);
IMPORT void hardWareInterFaceInit(void);
#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#endif

#if defined INCLUDE_ATA                  /* ATA / EIDE driver */
IMPORT ATA_CTRL   ataCtrl [];            /* for sysAta.c */
IMPORT VOIDFUNCPTR _func_sysAtaInit;
#endif /* INCLUDE_ATA */

/* vxbus support */

#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"

/* BSP DRIVERS */

#include <intrCtl/m5200Intr.c>
#include "sysCacheLockLib.c"

#ifdef INCLUDE_MPC5200_RTC
#include "m5200Rtc.c"
#endif /* INCLUDE_MPC5200_RTC */

#ifdef INCLUDE_PCI
#include "sysPci.c"
#include <pci/pciIntLib.c>
#ifdef INCLUDE_PCI_AUTOCONF
#include "sysPciAutoConfig.c"
#endif /* INCLUDE_PCI_AUTOCONF */
#endif /* INCLUDE_PCI */

/* network driver */

#ifdef INCLUDE_END
#include "sysNet.c"
#endif /* INCLUDE_END */

#ifndef INCLUDE_FLASH
#include <mem/nullNvRam.c>
#else
#include "nvRamToFlash.c"
#include "flashMem.c"
#endif /* INCLUDE_FLASH */

#ifdef INCLUDE_ATA
#include "sysAta.c"           /* sysAtaInit() routine */
#endif  /* INCLUDE_ATA */

#ifdef INCLUDE_GPIO
#include "sysGpioDemux.c"
#endif /* INCLUDE_GPIO */


/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*/

char * sysModel(void)
    {
    UINT cpu;

    /* Determine CPU type and build display string */

    cpu = CPU_TYPE;

    switch (cpu)
        {
        case CPU_TYPE_5200:
            switch (vxSvrGet() & 0xffff)
                {
                case 0x0010:
                    return (SYS_MODEL "A");

                case 0x0012:
                    return (SYS_MODEL "A3");

                case 0x0020:
                    return (SYS_MODEL "B1");

                case 0x0021:
                    return (SYS_MODEL "B2");

                case 0x0022:
                    return (SYS_MODEL "B3");

                default:
                    return (SYS_MODEL " - Unknown version.");
                }
        default:
            return ("Not a MPC5200 processor. Unsupported!");
        }
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number, for
* example, 1.2/0. BSP_REV is concatenated to BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev(void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the CPU board.  It is called
* by usrInit() in usrConfig.c.  This routine sets up the control registers
* and initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user application.
* It cannot be used to initialize interrupt vectors.
*
* RETURNS: N/A
*/

void sysHwInit(void)
    {
    /* initialize _func_vxSvrGet for drivers and other modules */

    _func_vxSvrGet = vxSvrGet;
    
    sysCpuCheck (); /* Validate CPU type */

    /* set GPS port configuration */

    *GPIO_STD_PORTCFG = GPS_INIT_VALUE;

    /* Must be set to 4 - See Chapter 8.7.5 MPC5200BUM.pdf */

    *SDRAM_SDELAY = 4;

    /* set clocks */

    *CDM_CFG            = CDM_CFG_VALUE;

    if (SYS_PLL_FREQ == 16 * SYS_FREQ || SYS_PLL_FREQ == 32 * SYS_FREQ)
        *CDM_DIVIDER    = CDM_DIV_VALUE_16;
    else
        *CDM_DIVIDER    = CDM_DIV_VALUE_12;

    *CDM_CLK_ENABLE     = CDM_ENA_VALUE;

    /* setup XLB */

    /* XLB_SWR register: WINSIZE = 64Mb, set WINBASE, DS=0 */

#ifdef SNOOP_ENABLE
    *XLB_SWR = (LOCAL_MEM_LOCAL_ADRS & 0xfffff000) | DRAM_BITS;
#endif /* SNOOP_ENABLE */

    /*
     * XLB_CR register:
     * CPU Pipeline depends on USB & PCI
     * Snooping depends on SNOOP_ENABLE, defined in config.h
     */

#ifdef SNOOP_ENABLE

#if (defined (INCLUDE_USB) || defined (INCLUDE_PCI))
    *XLB_CR = XLB_CR_PLDIS_DISABLED | XLB_CR_SE_ENABLED | XLB_CR_BLDIS_DISABLED;
#else
    *XLB_CR = XLB_CR_PLDIS_ENABLED | XLB_CR_SE_ENABLED;
#endif

#else /* not SNOOP_ENABLE */

#if (defined (INCLUDE_USB) || defined (INCLUDE_PCI))
    *XLB_CR = XLB_CR_PLDIS_DISABLED | XLB_CR_SE_DISABLED | XLB_CR_BLDIS_DISABLED;
#else
    *XLB_CR = XLB_CR_PLDIS_ENABLED | XLB_CR_SE_ENABLED;
#endif

#endif /* SNOOP_ENABLE */

    /* set XLB priorities (0 highest) : PCI 0, Bestcomm 1, USB 2, Core 7 */

    *XLB_MPR = 0 << XLB_MPR_PCI_SHIFT
             | 1 << XLB_MPR_BESTCOMM_SHIFT
             | 2 << XLB_MPR_USB_SHIFT
             | 7 << XLB_MPR_CORE_SHIFT;
    *XLB_MPER = XLB_MPER_SOFTWARE;

    /* Initialize interrupt controller */

    m5200IntrInit ();

    *XLB_CR |= XLB_CR_TBEN_ENABLED; /* start decrementer operation */

    /* Initialize USB */

#ifdef INCLUDE_USB
    *GPIO_PCR &= ~GPIO_STD_PC_USB_MASK;
    *GPIO_PCR |= (GPIO_STD_PC_USB_1_ONLY | GPIO_STD_PC_USB_DM_MODE);
#endif

    /*  Initialize PCI driver library */

#ifdef INCLUDE_PCI
    if ((sysPciInit()) != OK)
        {

        /*
         * Note, bailing to sysToMonitor() in here will probally cause
         * an endless loop but there is little alternative at this stage.
         * Watch the bus access lights to help detect this condition.
         */

        sysToMonitor (BOOT_NO_AUTOBOOT);    /* BAIL */
        }
#endif

    /* Initialize BestComm SmartDMA support */

#ifdef INCLUDE_BESTCOMM
    sysSdmaInit();
#endif /* INCLUDE_BESTCOMM */

#ifdef INCLUDE_PCI
#ifdef INCLUDE_PCI_AUTOCONF
    sysPciAutoConfig ();
#endif /* INCLUDE_PCI_AUTOCONF */
#endif /* INCLUDE_PCI */

    /*
     * If mmu tables are used, this is where we would dynamically
     * update the entry describing main memory, using sysPhysMemTop().
     * We must call sysPhysMemTop () at sysHwInit() time to do
     * the memory autosizing if available.
     */

    sysPhysMemTop ();

    /* vxbus initialization stage 1 */

    hardWareInterFaceInit();

    /* Initialize network channels */

#ifdef INCLUDE_END
    sysNetHwInit ();
#endif

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine, DEFAULT_BOOT_LINE, strlen(DEFAULT_BOOT_LINE) + 1);
#endif /* FORCE_DEFAULT_BOOT_LINE */

#if defined(INCLUDE_END) && defined(BOOT_FROM_UBOOT)
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
    sysBootlineFromUBoot();
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */
    }

/*******************************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupt vectors and configures any
* required features not configured by sysHwInit. It is called from usrRoot()
* in usrConfig.c after the multitasking kernel has started.
*
* RETURNS: N/A
*/

void sysHwInit2(void)
    {
    static int  initialized;    /* must protect against double call! */

    if (!initialized)
        {
        initialized = TRUE;

        /* vxbus initialization stage 2 */

        vxbDevInit();

        /* specific setup for the ATA controller */

#ifdef INCLUDE_ATA
        sysAtaInit(0);      /* device zero used first time through */
#endif  /* INCLUDE_ATA */

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif

        /* vxbus initialization stage 3 */

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
    static UINT32 sysPhysMemSize = 0; /* ptr to top of mem + 1 */

    if (sysPhysMemSize == 0)
        {
        /* Don't do auto-sizing, use defined constants. */

        sysPhysMemSize = (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
        }

    return (char *) sysPhysMemSize;
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

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)

       /*
        * U-Boot clears 0xb0000 bytes at top of RAM which makes this
        * useless for EDR. We reserve this space here as PM goes above
        * sysMemTop()+USER_RESERVED_MEM, so adding this amount to
        * USER_RESERVED_MEM does not work out
        */

       memTop -= 0xb0000;
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

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
#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
    FUNCPTR pRom = (FUNCPTR) (0xFFF00100L);         /* Cold reboot   */
    int *startTypeStore = (int *)0x000FFF00;        /* see romInit.s */
#   else
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);   /* Warm reboot */
#   endif /* UBOOT_BOOT_OPTION */
#else
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);   /* Warm reboot */
#endif /* BOOT_FROM_UBOOT */

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable (INSTRUCTION_CACHE); /* Disable the Instruction Cache */
    cacheDisable (DATA_CACHE);        /* Disable the Data Cache */
#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable ();
#endif /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_END
    sysNetHwInit ();        /* reset network devices */
#endif /* INCLUDE_END */

    /* Clear the MSR */

    vxMsrSet (0);

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
        *startTypeStore++ = startType;
        *startTypeStore++ = startType;
        *startTypeStore++ = startType;
        *startTypeStore++ = startType;
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

    (*pRom) (startType);

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
    return (sysProcNum);
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

/*******************************************************************************
*
* sysCpuCheck - confirm the CPU type
*
* This routine validates the cpu type.  If the wrong cpu type is discovered
* a message is printed using the serial channel in polled mode.
*
* RETURNS: N/A.
*/

void sysCpuCheck (void)
    {
    int        msgSize;
    int        cpuType;
#ifndef INCLUDE_SIO_POLL
    int        msgIx;
    SIO_CHAN * pSioChan;        /* serial I/O channel */
#endif

    cpuType = CPU_TYPE;

    /* Check for a valid CPU type;  If one is found, just return */

    if  ((cpuType == CPU_TYPE_5200))
        return;

    /* Invalid CPU type; print error message and terminate */

    msgSize = strlen (wrongCpuMsg);

#ifdef INCLUDE_SIO_POLL
    if (_func_consoleOut) (*_func_consoleOut)(1,wrongCpuMsg,msgSize);
#else
    pSioChan = sysSerialChanGet (0);

    sioIoctl (pSioChan, SIO_MODE_SET, (void *) SIO_MODE_POLL);

    for (msgIx = 0; msgIx < msgSize; msgIx++)
        {
        while (sioPollOutput (pSioChan, wrongCpuMsg[msgIx]) == EAGAIN);
        }
#endif /* INCLUDE_SIO_POLL */

    sysToMonitor (BOOT_NO_AUTOBOOT);
    }

/*******************************************************************************
*
* sysDelay - delay for approximately one millisecond
*
* Delay for approximately one milli-second.
*
* RETURNS: N/A
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

/*******************************************************************************
*
* sysDecDelay - delay for the specified amount of time (decrementer ticks)
*
* This routine will delay for the specified amount of decrementer ticks by
* counting decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* a slightly longer delay than requested.
*
* RETURNS: N/A
*/

LOCAL void sysDecDelay
    (
    UINT decTicks
    )
    {
    register UINT oldval;              /* decrementer value */
    register UINT newval;              /* decrementer value */
    register UINT totalDelta;          /* Dec. delta for entire delay period */
    register UINT decElapsed;          /* cumulative decrementer ticks */

    totalDelta = decTicks;

    /*
     * Now keep grabbing decrementer value and incrementing "decElapsed" until
     * we hit the desired delay value.  Compensate for the fact that we may
     * read the decrementer at 0xffffffff before the interrupt service
     * routine has a chance to set in the rollover value.
     */

    decElapsed = 0;

    oldval = vxDecGet ();

    while (decElapsed < totalDelta)
        {
        newval = vxDecGet ();

        if ( DELTA(oldval,newval) < 1000 )
            decElapsed += DELTA(oldval,newval);  /* no rollover */
        else
            if (newval > oldval)
                decElapsed += abs((int)oldval);  /* rollover */

        oldval = newval;
        }
    }

/*******************************************************************************
*
* sysMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by counting
* decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* the sysMsDelay() causing a slightly longer delay than requested.
*
* This routine will produce incorrect results if the delay time requested
* requires a count larger than 0xffffffff to hold the decrementer
* elapsed tick count.  For a System Bus Speed of 67 MHZ this amounts to
* about 258 seconds.
*
* RETURNS: N/A
*/

void sysMsDelay
    (
    UINT        delay                  /* length of time in MS to delay */
    )
    {

    /*
     * Calculate delta of decrementer ticks for desired elapsed time.
     * The macro DEC_CLOCK_FREQ MUST REFLECT THE PROPER 6xx BUS SPEED.
     */

    sysDecDelay(((DEC_CLOCK_FREQ / 4) / 1000) * delay);
    }

/*******************************************************************************
*
* sysUsDelay - delay for the specified amount of time (US)
*
* This routine will delay for the specified amount of time by counting
* decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* the sysMsDelay() causing a slightly longer delay than requested.
*
* This routine will produce incorrect results if the delay time requested
* requires a count larger than 0xffffffff to hold the decrementer
* elapsed tick count.  For a System Bus Speed of 67 MHZ this amounts to
* about 258 milliseconds.
*
* RETURNS: N/A
*/

void sysUsDelay
    (
    UINT        delay                  /* length of time in MS to delay */
    )
    {

    /*
     * Calculate delta of decrementer ticks for desired elapsed time.
     * The macro DEC_CLOCK_FREQ MUST REFLECT THE PROPER 6xx BUS SPEED.
     */

    sysDecDelay(((DEC_CLOCK_FREQ / 4) / 1000000) * delay);
    }

/*******************************************************************************
*
* sysEieio - make eieio instruction available for driver code.
*
* RETURNS: N/A
*/

void sysEieio (void)
    {
    EIEIO;
    }


#ifdef  INCLUDE_ATA

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
    return (intEnable (intNum));
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
    EIEIO_SYNC;
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
    EIEIO_SYNC;
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
    EIEIO_SYNC;
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
    EIEIO_SYNC;
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
    EIEIO_SYNC;
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
    EIEIO_SYNC;
    }
#endif  /* INCLUDE_ATA */

#if defined(INCLUDE_END) && defined(BOOT_FROM_UBOOT)
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)

/*******************************************************************************
*
* sysBootlineFromUBoot - read boot line from U-Boot.
*
* RETURNS: N/A
*/

void sysBootlineFromUBoot (void)
    {
    BOOT_PARAMS params;
    FAST char * pS;
    extern int getenv_r (UCHAR *name, UCHAR *buf, unsigned len);
    UCHAR netmask[20];
    UCHAR ipAddr[BOOT_ADDR_LEN];

    pS = bootStringToStruct (sysBootLine, &params);
    if (*pS != EOS)
        {
        return;
        }

    getenv_r ("serverip", params.had, sizeof (params.had));
    getenv_r ("gatewayip", params.gad, sizeof (params.had));
    if (getenv_r ("ipaddr", ipAddr, sizeof (ipAddr)) > 0)
        {
        strcpy(params.ead, ipAddr);

        if (getenv_r ("netmask", netmask, sizeof (netmask)) > 0)
            {
            int b0,b1,b2,b3;

            if (sscanf(netmask, "%d.%d.%d.%d", &b0, &b1, &b2, &b3) == 4)
                sprintf(params.ead, "%s:%02x%02x%02x%02x", ipAddr, b0, b1, b2, b3);
            }
        }

    bootStructToString(sysBootLine, &params);
    }
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

#ifdef INCLUDE_CACHE_SUPPORT

/*******************************************************************************
*
* cacheArchFlush - flush cache line.
*
* This function flushes cache line.
*
* RETURNS: OK or ERROR
*/

STATUS cacheArchFlush
    (
    CACHE_TYPE  cache,
    void *      address,
    size_t      bytes
    )
    {
    if (cacheLib.flushRtn)
        return (cacheLib.flushRtn)(cache, address, bytes);
    else
        return ERROR;
    }

/*******************************************************************************
*
* cacheArchInvalidate - invalidate cache line.
*
* This function invalidates cache line.
*
* RETURNS: OK or ERROR
*/

STATUS cacheArchInvalidate
    (
    CACHE_TYPE  cache,
    void *      address,
    size_t      bytes
    )
    {
    if (cacheLib.invalidateRtn)
        return (cacheLib.invalidateRtn)(cache, address, bytes);
    else
        return ERROR;
    }

#else /* INCLUDE_CACHE_SUPPORT */

/*******************************************************************************
*
* cacheArchFlush - flush cache line.
*
* This function flushes cache line.
*
* RETURNS: OK, always
*/

STATUS cacheArchFlush
    (
    CACHE_TYPE  cache,
    void *      address,
    size_t      bytes
    )
    {
    return OK;
    }

/*******************************************************************************
*
* cacheArchInvalidate - invalidate cache line.
*
* This function invalidates cache line.
*
* RETURNS: OK, always
*/

STATUS cacheArchInvalidate
    (
    CACHE_TYPE  cache,
    void *      address,
    size_t      bytes
    )
    {
    return OK;
    }

#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef DYN_FREQ

const int cdmCorePllFactor[] =
        {
        -1, -1, -1,  1,  4,  4,  5,  9,         /* 00000 */
         6, 11,  8, 10, -1, 12,  7, -1,         /* 01000 */
         6,  5, 13,  1, 14,  4, 15,  9,         /* 10000 */
        -1, 11,  8, 10, 16, 12,  7, -1          /* 11000 */
        };

const int cdmSysPllFactor[] = { 16, 12, 32, 24 };

#define CDM_PORCR_PPC_PLL_CFG_MASK      0x0000001F
#define CDM_PORCR_PPC_PLL_CFG_SHIFT     0
#define CDM_PORCR_SYS_PLL_BYPASS_MASK   0x00010000
#define CDM_PORCR_XLB_CLK_SEL_MASK      0x00000020
#define CDM_PORCR_SYS_PLL_CFG_MASK      0x000000c0
#define CDM_PORCR_SYS_PLL_CFG_SHIFT     6
#define CDM_CFG_IPB_CLK_SEL_MASK        0x00000100
#define CDM_CFG_PCI_CLK_SEL_MASK        0x00000003
#define CDM_CFG_PCI_CLK_SEL_SHIFT       0

/*******************************************************************************
*
* sysGetFvcoFreq - get the VCO clock frequency.
*
* This routine gets the VCO clock frequency.
*
* RETURNS: VCO clock frequency
*/

int sysGetFvcoFreq(void)
    {
    static int sys_pll_cfg = -1;
    static int sys_pll_bypass = -1;

    if (sys_pll_cfg < 0)
        {
        sys_pll_cfg = ((*CDM_PORSTCFG) & CDM_PORCR_SYS_PLL_CFG_MASK)
                    >> CDM_PORCR_SYS_PLL_CFG_SHIFT;
        sys_pll_bypass = (*CDM_PORSTCFG) & CDM_PORCR_SYS_PLL_BYPASS_MASK;
        }

    if (sys_pll_bypass)
        return SYS_FREQ;
    else
        return SYS_FREQ * cdmSysPllFactor[sys_pll_cfg];
    }

/*******************************************************************************
*
* sysGetXlbClock - get the XLB clock frequency.
*
* This routine gets the XLB clock frequency.
*
* RETURNS: XLB clock frequency
*/

int sysGetXlbClock(void)
    {
    static int xlb_clk_sel = -1;
    static int sys_pll_cfg = -1;
    int result = -1;

    if (sys_pll_cfg < 0)
        {
        sys_pll_cfg = ((*CDM_PORSTCFG) & CDM_PORCR_SYS_PLL_CFG_MASK)
                    >> CDM_PORCR_SYS_PLL_CFG_SHIFT;
        xlb_clk_sel = ((*CDM_PORSTCFG) & CDM_PORCR_XLB_CLK_SEL_MASK);
        }

    result = sysGetFvcoFreq();

    if (xlb_clk_sel == 0)
        result /= 4;
    else
        result /= 8;

    if (sys_pll_cfg >= 2)
        result /= 2;

    return result;
    }

/*******************************************************************************
*
* sysGetIpbClock - get the IPB clock frequency.
*
* This routine gets the IPB clock frequency.
*
* RETURNS: IPB clock frequency
*/

int sysGetIpbClock(void)
    {
    int ipb_clk_sel = ((*CDM_CFG) & CDM_CFG_IPB_CLK_SEL_MASK);

    if (ipb_clk_sel == 0)
        return sysGetXlbClock();
    else
        return sysGetXlbClock() / 2;
    }

/*******************************************************************************
*
* sysGetPciClock - get the PCI clock frequency.
*
* This routine gets the PCI clock frequency.
*
* RETURNS: PCI clock frequency
*/

int sysGetPciClock(void)
    {
    int pci_clk_sel = ((*CDM_CFG) & CDM_CFG_PCI_CLK_SEL_MASK)
                >> CDM_CFG_PCI_CLK_SEL_SHIFT;

    switch (pci_clk_sel)
        {
        case 0:
            return sysGetIpbClock();
        case 1:
            return sysGetIpbClock() / 2;
        case 2:
            return sysGetIpbClock() / 4;
        default:
            return -1;
        }
    }

/*******************************************************************************
*
* sysGetCoreSpeed - get the powerpc core clock frequency.
*
* This routine gets the powerpc core clock frequency.
*
* RETURNS: core clock frequency
*/

int sysGetCoreSpeed(void)
    {
    static int ppc_pll_cfg = -1;

    if (ppc_pll_cfg < 0)
        ppc_pll_cfg = (*CDM_PORSTCFG) & CDM_PORCR_PPC_PLL_CFG_MASK;

    return cdmCorePllFactor[ppc_pll_cfg] * sysGetXlbClock() / 2;
    }

/*******************************************************************************
*
* sysInfo - print system specific information.
*
* This routine prints system specific information.
*
* RETURNS: N/A
*/

void sysInfo(void)
    {
    printf("CPU %d MHz XLB %d MHz IPB %d MHz PCI %d MHz\n",
           CORE_CLOCK_FREQ/1000000, XLB_CLOCK/1000000,
           IPB_CLOCK/1000000, PCI_CLOCK/1000000);
    printf("SVR 0x%x\n", vxSvrGet());
    }

#endif /* DYN_FREQ */

/*******************************************************************************
*
* sysGetUartClockFreq - get the UART clock frequency.
*
* This function get the UART clock frequency.
*
* RETURNS: N/A
*/

UINT32 sysGetUartClockFreq(void)
    {
    return UART_CLOCK;
    }

/*******************************************************************************
*
* sysGetDecClockFreq - get the powerpc decrementer clock frequency.
*
* This function get the powerpc decrementer clock frequency.
*
* RETURNS: N/A
*/

UINT32 sysGetDecClockFreq(void)
    {
    return DEC_CLOCK_FREQ;
    }

#ifdef DRV_TIMER_M5XXX_GPT

/*******************************************************************************
*
* sysGetGptClockFreq - get the general purpose timer clock frequency.
*
* This function get the general purpose timer clock frequency.
*
* RETURNS: N/A
*/

UINT32 sysGetGptClockFreq(void)
    {
    return IPB_CLOCK;
    }

#endif /* DRV_TIMER_M5XXX_GPT */

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

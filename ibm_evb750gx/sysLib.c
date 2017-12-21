/* sysLib.c - IBM750GX Evaluation Board system-dependent library */

/*
 * Copyright (c) 2005-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005-2006 Tundra Semiconductor Corp. */

/* Copyright 1996,1999 Motorola, Inc. */

/*
modification history
--------------------
01l,29may13,d_l  fix prevent errors. (WIND00419006)
01k,30apr10,dhy  delete _MMU_UBAT_VP attribute(WIND00162540)
01j,28jul09,fao  fixed VIP with INCLUDE_SIO_UTILS build fail.(WIND00172550)
01i,15jul09,my_  fixed diab compiler warnings (WIND00173188)
01h,03jul08,y_w  added VxBus support and code cleanup
01g,23oct07,y_w  remove sysFlags and add including of tsi108End.c.
01f,26jul07,y_w  solve the problem: Ctrl-X always results in cold reset.
01d,10jan07,y_w  modified to support Holly/Hickory evaluation board
01c,13jul06,avb  updated to match latest revision of HPC-II BSP
01b,06dec05,avb  clean up to meet vxWorks coding style requirements
01a,26jul05,avb  written from Motorola's sp7447 and templatePpc.c
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    sysTsi108Mpic.c     - Interrupt controller (in Tsi108) driver 
    ns16550Sio.c        - National Semiconductor 16550 UART driver
    flashMem.c          - 29F040 flash memory device driver.
    pciConfigLib.c      - PCI Configuration Space Access Library
    pciConfigShow.c     - PCI Configuration Space Display Library
    pciAutoconfigLib.c  - PCI Bus Auto-configuration Library
    sysGei8254xEnd.c    - Gigabit adapter device driver
    i2c.c               - Tsi108 I2C driver and library
    d1337RTC.c          - Support for Dallas Semiconductor DS1337 real-time clk
    sysTsi108AuxClk.c   - Tsi108 Aux clock driver
    
INCLUDE FILES: sysLib.h
*/

/* includes */

#include <stdio.h>
#include <vxWorks.h>
#include <vme.h>
#include <memLib.h>
#include <cacheLib.h>
#include <sysLib.h>
#include "config.h"
#include <string.h>
#include <intLib.h>
#include <logLib.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <vmLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include "tsi108.h"
#include "sysTsi108Mpic.h"

#ifdef INCLUDE_CACHE_L2
#   include "sysL2Cache.h"
#endif /* INCLUDE_CACHE_L2 */

#ifdef INCLUDE_PCI
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#   include <esf.h>
#endif /* INCLUDE_PCI */

#if defined (INCLUDE_NETWORK) && defined (INCLUDE_DC_NETIF)
#   include <drv/netif/if_dc.h>     /* netif dc driver */
#endif /* INCLUDE_NETWORK && INCLUDE_DC_NETIF*/

#ifdef INCLUDE_SYS_COPY_TO_FLASH
#   include <stat.h>
#endif /* INCLUDE_SYS_COPY_TO_FLASH */

IMPORT  void    hardWareInterFaceInit (void);

#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"

/* defines */

#define ZERO        0

#ifdef SYS_FLASH_TYPE
#   define FLASH_MEM_TYPE       SYS_FLASH_TYPE
#else
#   define FLASH_MEM_TYPE       0
#endif  /* SYS_FLASH_TYPE */

#define DELTA(a,b)  (abs((int)a - (int)b))

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
UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT +_MMU_NUM_EXTRA_IBAT+_MMU_NUM_EXTRA_DBAT)] =
    {
    /* I BAT 0 - Boot Flash */
    ((ROM_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M | _MMU_UBAT_VS ),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW | _MMU_LBAT_CACHE_INHIBIT),

    /* I BAT 1 */
    
    0,0,
    
    /* I BAT 2 */

    0,0,

    /* I BAT 3 */

    0,0,

    /* D BAT 0 - Boot Flash */
    ((ROM_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M | _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW | _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

#ifdef INCLUDE_PCI
    /* D BAT 1 - PCI prefetchable memory + non-prefetchable memory  = 256MB */
    ((PCI_MSTR_MEMIO_LOCAL & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((PCI_MSTR_MEMIO_LOCAL & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW | _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 - PCI IO space */
    ((PCI_MSTR_IO_LOCAL & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M | _MMU_UBAT_VS),
    ((PCI_MSTR_IO_LOCAL & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW  | _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 3 - PCI Configuration space (memory mapped) */
    ((PCI_MSTR_CFG_LOCAL & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M | _MMU_UBAT_VS),
    ((PCI_MSTR_CFG_LOCAL & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW  | _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),
#else
    /* D BAT 1 */
    
    0,0,
    
    /* D BAT 2 */

    0,0,

    /* D BAT 3 */

    0,0,
#endif /* INCLUDE_PCI */

    /*
     * These entries are for the the I/D BAT's (4-7) on the MPC7447/47A/48.
     * They should be defined in the following order.
     * IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
     * DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
     */

    /* I BAT 4 */
   
    0,0,

    /* I BAT 5 */
   
    0,0,
   
    /* I BAT 6 */

    0,0,

    /* I BAT 7 */

    0,0,
    /* D BAT 4 */
   
    0,0,

    /* D BAT 5 */
   
    0,0,
   
    /* D BAT 6 */

    0,0,

    /* D BAT 7 */

    0,0

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
 * All entries in this table both addresses and lengths must be page aligned.
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
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {
    {
    /* Vector Table and Interrupt Stack */
    /* Must be sysPhysMemDesc [0] to allow adjustment in sysHwInit() */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    },

    {

    /* Must be sysPhysMemDesc [1] to allow adjustment in sysHwInit() */
    (VIRT_ADDR) RAM_LOW_ADRS,
    (PHYS_ADDR) RAM_LOW_ADRS,
    LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE -  RAM_LOW_ADRS,  
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE       |
    VM_STATE_MEM_COHERENCY
    },

    {
    /* Tsi108 Internal Registers */
    (VIRT_ADDR) TSI108_CSR_BASE,
    (PHYS_ADDR) TSI108_CSR_BASE,
    TSI108_CSR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_MEM_COHERENCY | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_MEM_COHERENCY      | VM_STATE_GUARDED
    },

    { /* SRAM */
    (VIRT_ADDR) SRAM_BASE_ADRS,
    (PHYS_ADDR) SRAM_BASE_ADRS,
    SRAM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    }

#ifdef INCLUDE_NVRAM
    ,
    { /* Map entire NVRAM device (including reserved space) */
    (VIRT_ADDR) NV_RAM_BASE_PHYS,
    (PHYS_ADDR) NV_RAM_BASE_PHYS,
    NV_RAM_SIZE_PHYS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    }
#endif /* INCLUDE_NVRAM */

#ifdef  INCLUDE_FLASH
    ,
    {
    (VIRT_ADDR) FLASH_ADRS,
    (PHYS_ADDR) FLASH_ADRS,
    FLASH_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE  |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT   |
    VM_STATE_GUARDED
    },
#endif /* INCLUDE_FLASH */
    };

int   sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int   sysBus      = BUS_TYPE_PCI;
int   sysCpu      = CPU;                /* system CPU type (MC680x0) */
char *sysBootLine = BOOT_LINE_ADRS;     /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;       /* catastrophic message area */
int   sysProcNum  = 0;                  /* processor number of this CPU */
char  sysBootHost [BOOT_FIELD_LEN];     /* name of host from which we booted */
char  sysBootFile [BOOT_FIELD_LEN];     /* name of file from which we booted */

ULONG sysPbClock = 0;                   /* Processor Bus clock speed in Hz */

/* static locals */

LOCAL char sysModelStr [80] = SYS_MODEL;    /* Platform Model string */
LOCAL char sysInfoStr [200];                /* Platform INFO string */
LOCAL char wrongCpuMsg []   = WRONG_CPU_MSG; 

LOCAL UINT32 extIntSenses[] =
    {
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* INT[0] */ 
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* INT[1] */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH),  /* INT[2] */
    (TSI108_IRQ_SENSE_LEVEL | TSI108_IRQ_ACTIVE_HIGH)   /* INT[3] */
    };

/* static locals which define the PREP or CHRP mapping */

/* forward declarations */

UINT32  sysCpuBusSpd    (void);
char *  sysPhysMemTop   (void);         /* Top of physical memory */
char *  sysInfo         (void);
void    sysCpuCheck     (void);         /* detect CPU type */
void    sysDelay        (void);         /* delay 1 millisecond */
void    sysMsDelay      (UINT delay);   /* delay N millisecond(s) */

STATUS  sysIntEnablePIC (int intNum);   /* Enable i8259 or EPIC */

#if defined INCLUDE_PCI
LOCAL STATUS sysPciConfigRead  (int busNo,  int deviceNo, int funcNo,
                                int offset, int width, void * pData);
LOCAL STATUS sysPciConfigWrite (int busNo,  int deviceNo, int funcNo,
                                int offset, int width,  ULONG data);
void sysPciTrap (ESFPPC *pEsf);
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_NVRAM        /* NVRAM init and access routines */
void    sysNvramHwInit      (void);
UCHAR   sysNvRead       (ULONG);
void    sysNvWrite      (ULONG, UCHAR);
#endif /* INCLUDE_NVRAM */

void sysUsDelay(UINT32 delay);
#if defined(INCLUDE_NETWORK) && defined(INCLUDE_END)    /* network */
#   if defined(INCLUDE_DC_END) 
       STATUS  sysDec21x40PciInit (void);
#   endif /* INCLUDE_DC_END && INCLUDE_END */
#endif /* INCLUDE_NETWORK && INCLUDE_END */

/* external imports */

IMPORT VOID   sysOutWord    (ULONG address, UINT16 data);   /* sysALib.s */
IMPORT VOID   sysOutLong    (ULONG address, ULONG data);    /* sysALib.s */
IMPORT USHORT sysInWord     (ULONG address);                /* sysALib.s */
IMPORT ULONG  sysInLong     (ULONG address);                /* sysALib.s */
IMPORT VOID   sysOutByte    (ULONG, UCHAR);                 /* sysALib.s */
IMPORT UCHAR  sysInByte     (ULONG);                        /* sysALib.s */
IMPORT STATUS sysMemProbeSup    (int length, char * src, char * dest);
IMPORT void   sysGetPbClock (void);
IMPORT void   sysTsi108Init (void);
IMPORT int    sysPciErr();
IMPORT int    sysL2crGet();
IMPORT void   sysClearPciError (void);
IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;

IMPORT VOIDFUNCPTR _pSysL2TextInvalidate;
IMPORT VOIDFUNCPTR _pSysL2TextInvalidateAll;
IMPORT VOIDFUNCPTR _pSysL2TextUpdate;
IMPORT BOOL snoopEnable;

/* 
 * mmuPpcBatInit750fx() initializes the standard 4 (0-3) I/D BAT's &
 * the additional 4 (4-7) I/D BAT's present on the PPC750FX/GX.
 */

IMPORT void mmuPpcBatInit750fx (UINT32 *pSysBatDesc);

/* 
 * _pSysBatInitFunc needs to be set to the function above 
 * to make use of the additional BAT's in the PPC750FX/GX.
 * If this is not set or is set to NULL then the standard mmuPpcBatInit
 * fn. would be called which would initialize the (0-3) I/D BAT's
 */

IMPORT FUNCPTR _pSysBatInitFunc;

/* 
 * By default this is set to NULL in the mmu Library and  in that case 
 * the standard BAT init func. to initialize the 4 I/D BAT's is called.
 * If the standard & Extra BAT's need to be initialized then set this
 * to the ones supplied by the mmuLib or else implement your own fn. and
 * supply it here.
 */

/* BSP DRIVERS */

#include <vme/nullVme.c>        /* No VME bus */
#include "sysTsi108Init.c"
#include "sysTsi108Mpic.c"      /* Interrupt Controller driver */

#ifdef INCLUDE_SERIAL
#   include "ns16550Sio.c"          /* NS Uart driver */
#endif /* INCLUDE_SERIAL */

#ifdef INCLUDE_TSI108_DMA
#   include "sysTsi108Dma.c"
#endif

/*
 * Note: local copies of pciConfigLig.c and pciAutoConfigLib.c are
 * provided to work around a board problem with device IDSEL=12 causing
 * a freeze-up when probed
 */

#ifdef INCLUDE_PCI
#   include <pci/pciIntLib.c>           /* PCI int support */
#   include <pci/pciConfigLib.c>
#   ifdef INCLUDE_PCI_CFGSHOW
#       include <pci/pciConfigShow.c>   /* PCI config space display */
#   endif /* INCLUDE_PCI_CFGSHOW */
#   ifdef INCLUDE_PCI_AUTOCONF          /* Sandpoint uses autoconfig */
#       include <pci/pciAutoConfigLib.c>/* automatic PCI configuration */
#       include "sysBusPci.c"           /* pciAutoConfig BSP support file */
#   endif /*  INCLUDE_PCI_AUTOCONF */
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_I2C
#    include "i2c.c"
#endif /* INCLUDE_I2C */

#ifdef INCLUDE_NVRAM
#   include <mem/byteNvRam.c>       /* byte nvram routines */
#else
#   include <mem/nullNvRam.c>
#endif /* INCLUDE_NVRAM */

#ifdef INCLUDE_RTC
#   include "d1337RTC.c"            /* real time clock */
#endif /* INCLUDE_RTC */

#ifdef INCLUDE_SERIAL
#    include "sysSerial.c"          /* BSP serial setup */
#endif /* INCLUDE_SERIAL */

#ifdef INCLUDE_NETWORK
#   ifdef INCLUDE_END
#      include "sysNet.c"
#      ifdef INCLUDE_DC_END
#         include "sysDec21x40End.c"
#      endif /* INCLUDE_DC_END */
#      ifdef INCLUDE_TSI108_END
#         include "tsi108End.c"     /* include tsi108End drive */
#      endif
#   else
#      warning  "INCLUDE_END not defined!"
#   endif /* INCLUDE_END */
#endif  /* INCLUDE_NETWORK */

#ifdef  INCLUDE_FLASH
#   include "flashMem.c"            /* 29F040 is supported */
#endif  /* INCLUDE_FLASH */

#ifdef INCLUDE_GEI8254X_END
#   include "sysGei8254xEnd.c"      /* Gigabit adapter driver */
#endif /* INCLUDE_GEI8254X_END */

#ifdef INCLUDE_AUX_CLK
#   include "sysTsi108AuxClk.c"     /* Tsi108 MPIC AuxClk support */
#   include <vxbTimerLib.h>
#endif /* INCLUDE_AUX_CLK */

/*******************************************************************************
*
* sysCpuBusSpd - routine to get the speed of the CPU bus
*
* This routine returns processor bus clock speed
*
* RETURNS: Processor bus clock in Hz.
*/

UINT32 sysCpuBusSpd (void)
    {
    return (sysPbClock);
    }

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.  The returned string
* depends on the board model and CPU version being used, for example,
* "Tundra Tsi108/PPC750CL Eval. Board - PPC750CL"
*
* RETURNS: A pointer to the string.
*
* ERRNO: N/A
*/

char * sysModel (void)
    {
    ULONG regVal;

    sprintf (sysModelStr, "%s", SYS_MODEL);

    /* The CPU type is indicated in the Processor Version Register (PVR) */

    regVal = CPU_TYPE;

    switch (regVal)
        {
        case CPU_TYPE_750CL:
            strcat(sysModelStr, "- PPC750CL");
            break;
        case CPU_TYPE_750GX:
            strcat(sysModelStr, "- PPC750GX");
            break;
        default:
            break;
        }   /* switch  */

    return (sysModelStr);
    }


/*******************************************************************************
*
* sysInfo - return some information on the CPU board configuration
*
* This routine returns a string containing the L2 backside cache size,
* if enabled, the PCI map selected, and whether NVRAM is selected.
* The string may contain line feeds. For example,
*
*      512KB L2 backside cache enabled 
*      PCI Memory Map B
*      NVRAM enabled
*
* RETURNS: A pointer to the string.
*
* ERRNO: N/A
*/

char * sysInfo (void)
    {
    /* Show memory map being used */

    strcpy(sysInfoStr, "PCI Memory Map @0xE0000000\n");

#ifdef  INCLUDE_NVRAM
    strcat(sysInfoStr, "NVRAM enabled\n");
#else
    strcat(sysInfoStr, "No NVRAM enabled\n");
#endif  /* INCLUDE_NVRAM */
      
    return (sysInfoStr);
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number, for
* example, 1.1/0. BSP_REV is concatenated to BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO: N/A
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various hardware features of the board.
* It is the first board-specific C code executed, and runs with
* interrupts masked in the processor.
* This routine resets all devices to a quiescent state, typically
* by calling driver initialization routines.
*
* NOTE: Because this routine may be called from sysToMonitor, it must
* shutdown all potential DMA master devices.  If a device is doing DMA
* at reboot time, the DMA could interfere with the reboot. For devices
* on non-local busses, this is easy if the bus reset or sysFail line can
* be asserted.
*
* NOTE: This routine should not be called directly by any user application.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit (void)
    {
    /*
     * First, automatically detect processor bus clock speed
     * and setup proper global variable value.
     * Do this before _anything_ else in sysHwInit or else some Tsi108
     * registers will not be setup correctly by other routines.
     */

    sysGetPbClock();

    /* Initialize Tsi108 switching fabric and BARs (other than SDRAM). */

    sysTsi108Init();

    /* Validate the CPU type used */

    sysCpuCheck();

    /* Setup MMU if needed */

#if defined(INCLUDE_MMU_BASIC)
    /* 
     *  Setup the function pointers for the BAT initialization functions. 
     *  (Standard 4 I/D BAT's and extra 4 I/D BAT's
     */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInit750fx;

    /* 
     * run-time update of the sysPhysMemDesc MMU entry for main RAM 
     * This means SDRAM _must_ be represented by sysPhysMemDesc[1]
     */

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () - RAM_LOW_ADRS);
#endif /* INCLUDE_MMU */

    /* vxbus initialization stage 1 */

    hardWareInterFaceInit();

#ifdef INCLUDE_AUX_CLK
    /* populate the function pointer for the auxilary clock */

    _vxb_auxClkConnectRtn = &tsiAuxClkConnect;
    _vxb_auxClkDisableRtn = &tsiAuxClkDisable;
    _vxb_auxClkEnableRtn  = &tsiAuxClkEnable;
    _vxb_auxClkRateGetRtn = &tsiAuxClkRateGet;
    _vxb_auxClkRateSetRtn = &tsiAuxClkRateSet;
#endif /* INCLUDE_AUX_CLK */

#if defined (INCLUDE_PCI)

    /*  Initialize PCI configuration space support library. */

    if (pciConfigLibInit (PCI_MECHANISM_0, (ULONG) sysPciConfigRead,
                          (ULONG) sysPciConfigWrite, (ULONG)NULL) != OK)
        {
        /*
         * Note, bailing to sysToMonitor() in here will probally cause 
         * an endless loop but there is little alternative at this stage.
         * Watch the bus access lights to help detect this condition.
         */

        sysToMonitor (BOOT_NO_AUTOBOOT);  /* BAIL */
        }

    /*  Initialize PCI interrupt library. */

    if ((pciIntLibInit ()) != OK)
        {
        sysToMonitor (BOOT_NO_AUTOBOOT); 
        }

#   ifdef INCLUDE_PCI_AUTOCONF
#       if (PCI_CFG_TYPE == PCI_CFG_AUTO)
            sysPciAutoConfig();
#       endif /* (PCI_CFG_TYPE == PCI_CFG_AUTO) */
#   endif /* INCLUDE_PCI_AUTOCONF */

#endif  /* INCLUDE_PCI */

    /* Initialize the Embedded Programmable Interrupt Controller */

    sysTsi108MpicInit(0, extIntSenses);

#ifdef INCLUDE_SERIAL
    sysSerialHwInit();      /* init COM1 & COM2 serial devices */
#endif /* INCLUDE_SERIAL */

#ifdef FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine,DEFAULT_BOOT_LINE,strlen(DEFAULT_BOOT_LINE)+1);
#endif /* FORCE_DEFAULT_BOOT_LINE */

#ifdef INCLUDE_CACHE_SUPPORT

#ifdef INCLUDE_CACHE_L2

/*
 * Callbacks added to support certain text-only L2 caches.
 *
 * The initial use case is for the PPC750GX, whose L2 can be set to operate
 * in text-only mode, however it does not respond to the icbi instruction.
 * When operating an L2 cache in a text-only mode that does not respond to
 * icbi, the BSP must provide these callbacks to invalidate (at least) the
 * required part of the L2.  Because a text-only cache will not contain
 * modified lines, it is acceptable to invalidate the entire L2 even when
 * only a partial invalidation is required; however performance will be
 * improved by invalidating only the required part.
 *
 * Note that such invalidations are not needed when the L2 is operating in
 * unified or data-only mode, and *must not* be performed when operating
 * in unified or data-only copy-back mode because it would cause loss of
 * modified lines.  This can be accomplished either by setting the pointers
 * to NULL when the L2 is not in text-only mode, or by having the callback
 * functions check the L2 mode before performing the invalidate operation.
 *
 * All three functions have the same prototype and it is acceptable (but not
 * required) for all three pointers to specify the same callback function.
 * The parameters passed to (*_pSysL2TextInvalidateAll) will be NONE (which
 * is guaranteed to be different from NULL), and zero.
 */

#ifdef L2_MODE_I_ONLY    /* L2 text-only mode */
    _pSysL2TextInvalidate    = sysL2CacheGlobalInv;
    _pSysL2TextInvalidateAll = sysL2CacheGlobalInv;
    _pSysL2TextUpdate        = sysL2CacheGlobalInv;
#endif

    sysL2CacheInit();

#if (L2CACHE_MODE == L2CACHE_MODE_WRITETHROUGH)    
    sysL2CacheSetWT();
#endif /* L2CACHE_MODE_WRITETHROUGH */

    _pSysL2CacheInvFunc = sysL2CacheGlobalInv;
    _pSysL2CacheEnable  = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;
    _pSysL2CacheFlush   = sysL2CacheSWFlush;

#endif /* INCLUDE_CACHE_L2 */

#if (USER_D_CACHE_MODE & CACHE_SNOOP_ENABLE)
    snoopEnable = TRUE;
#else
    snoopEnable = FALSE;
#endif /* USER_D_CACHE_MODE & CACHE_SNOOP_ENABLE */

#endif /* INCLUDE_CACHE_SUPPORT */
    }

/*******************************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupt vectors and configures any
* required features not configured by sysHwInit. It is called from usrRoot()
* in usrConfig.c (via tsiClkConnect) after the multitasking kernel has started.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit2 (void)
    {
    static int  initialized;        /* Do this ONCE */
  
    if (!initialized)
        {
        initialized = TRUE;

        /* vxbus initialization stage 2 */

        vxbDevInit();

        /* initialize Aux clock */

#ifdef INCLUDE_AUX_CLK
        tsiAuxClkInit2();
#endif  /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_SERIAL
        sysSerialHwInit2 ();    /* connect serial interrupts */
#endif  /* INCLUDE_SERIAL */

#ifdef INCLUDE_NVRAM
        /* initialize the NV_RAM */

        sysNvramHwInit();
#endif /* INCLUDE_NVRAM  */

#ifdef INCLUDE_PCI
#   ifdef INCLUDE_GEI8254X_END
        int sysPci1SysNum = 1;
        pciConfigForeachFunc (0, FALSE, (PCI_FOREACH_FUNC) sys8254xDeviceCheck,\
                              (void *)&sysPci1SysNum);
#   endif /* INCLUDE_GEI8254X_END */
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_I2C
        i2cLibInit();
#endif /* INCLUDE_I2C */

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
*
* ERRNO: N/A
*/

char * sysPhysMemTop (void)
    {
    static char * physTop = NULL;

    if (physTop == NULL)
        {
#ifdef LOCAL_MEM_AUTOSIZE
#       error   "Dynamic memory sizing not supported"
#else
        /* Don't do autosizing, size is given */

        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
#endif /* LOCAL_MEM_AUTOSIZE */
    }

    return physTop;
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
    static char * memTop = NULL;

    if (memTop == NULL)
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;
#ifdef  INCLUDE_EDR_PM
        /* account for ED&R persistent memory */

        memTop -= PM_RESERVED_MEM;
#endif  /* INCLUDE_EDR_PM */
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
* The entry point for a warm boot is defined by the macro ROM_WARM_ADRS
* in config.h.  We do an absolute jump to this address to enter the
* ROM code.
*
* RETURNS: Does not return.
*
* ERRNO: N/A
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR     pRom = (FUNCPTR) (ROM_WARM_ADRS);
    int         ix;

    intLock ();                         /* disable interrupts */

    cacheDisable (INSTRUCTION_CACHE);   /* Disable the Instruction Cache */
    cacheDisable (DATA_CACHE);          /* Disable the Data Cache */

#if (CPU == PPC604)
    vxHid0Set (vxHid0Get () & ~_PPC_HID0_SIED); /* Enable Serial Instr Exec */
#endif  /* (CPU == PPC604) */
  
    /* turn off all MPIC interrupts */

    /* Reset MPIC */

    TSI108_REG_WR(BL_MPIC, MPIC_INT_GCR, MPIC_INT_GCR_R);

    /* Wait for reset completion */

    while(TSI108_REG_RD(BL_MPIC, MPIC_INT_GCR) & MPIC_INT_GCR_R)
        EIEIO_SYNC;

    /* Set default value for PB_OCN_BAR1 to enable BOOT mode */

    TSI108_REG_WR(BL_PB, PB_OCN_BAR1, 0xF0000003);

    /* read bacl to make sure that OCN_BAR1 decoder is set */

    ix = TSI108_REG_RD(BL_PB, PB_OCN_BAR1);
    SYNC;

    vxMsrSet (0);               /* Clear the MSR */

    (*pRom) (startType);        /* jump off to romInit.s */

    return (OK);                /* in case we continue from ROM monitor */
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
*
* ERRNO: N/A
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
* For bus systems, it is assumes that processor 0 is the bus master and
* exports its memory to the bus.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*
* ERRNO: N/A
*/

void sysProcNumSet
    (
    int procNum         /* processor number */
    )
    {
    sysProcNum = procNum;

    if (procNum == 0)
        {
        }
    }

/*******************************************************************************
*
* sysCpuCheck - confirm the CPU type
*
* This routine validates the cpu type.  If the wrong cpu type is discovered
* a message is printed using the serial channel in polled mode.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*/

void sysCpuCheck (void)
    {
    int         msgSize;
    int         msgIx;
    SIO_CHAN *  pSioChan;        /* serial I/O channel */

    /* Check for a valid CPU type;  If one is found, just return */

#if (CPU == PPC604) /* known kinds of 604 */
    if ((CPU_TYPE == CPU_TYPE_750FX) || (CPU_TYPE == CPU_TYPE_750GX) ||
        (CPU_TYPE == CPU_TYPE_750CL))
        {
        return; 
        }
#endif  /* (CPU == PPC604) */

    /* Invalid CPU type; polling print of error message and bail */

    msgSize = strlen (wrongCpuMsg);

    sysSerialHwInit ();

    pSioChan = sysSerialChanGet (0);

    sioIoctl (pSioChan, SIO_MODE_SET, (void *) SIO_MODE_POLL);

    for (msgIx = 0; msgIx < msgSize; msgIx++)
        {
        while (sioPollOutput (pSioChan, wrongCpuMsg[msgIx]) == EAGAIN);
        }

    sysToMonitor (BOOT_NO_AUTOBOOT);
    }


/*******************************************************************************
*
* sysDelay - delay for approximately one millisecond
*
* Delay for approximately one milli-second.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

/* PPC Decrementer - used as vxWorks system clock */

/*************************************************************************
*
* sysUsDelay - delay for at least x microseconds.
*
* Use the timebase register, running at sysTimerClkFreq, to delay at least
* the requested number of microseconds.
*
* RETURNS: N/A.
* 
* ERRNO: N/A
*
* Do not call before sysHwInit determines sysPbClock.
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

    /* Calculate number of ticks for desired elapsed time. */

    ticks = delay * (ROUND_UP((sysPbClock/4), 1000000) / 1000000);

    /* set target hi and lo, and account for rollover */

    target_lo = lo + ticks;     /* may cause rollover */
    target_hi = hi + (target_lo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
        {
        vxTimeBaseGet(&hi, &lo);
        } while (hi < target_hi || (hi == target_hi && lo < target_lo));

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
    UINT      delay                   /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (UINT32) delay * 1000 );
    }


/*******************************************************************************
*
* sysIntEnablePIC - enable an MPIC interrupt (internal or PCI)
*
* This function call is used to enable an individual MPIC interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*
* ERRNO: N/A
*/

STATUS sysIntEnablePIC
    (
    int intNum
    )
    {

    if ((intNum < TSI108_IRQ_BASE) ||
        (intNum > (TSI108_IRQ_BASE + TSI108_MAX_VECTORS)))
        {
        logMsg ("sysIntEnablePIC: Invalid interrupt number %d.\n", 
                intNum,2,3,4,5,6);
        return (ERROR);
        }

    return (intEnable (intNum));
    }


/*******************************************************************************
*
* sysIntEnable - enable an interrupt
*
* This function call is used to enable an ISA/PCI interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*
* ERRNO: N/A
*/

STATUS sysIntEnable
    (
    int intNum
    )
    {
    return (sysIntEnablePIC (intNum));
    }

/*******************************************************************************
*
* sysIntDisable - disable an interrupt
*
* This function call is used to disable an interrupt.
*
* RETURNS: OK or ERROR if unable to disable interrupt.
*
* ERRNO: N/A
*/

STATUS sysIntDisable
    (
    int intNum
    )
    {
    return (intDisable (intNum));
    }

#if  defined(INCLUDE_NVRAM)

/*******************************************************************************
*
* sysNvramHwInit - initialize the NVRAM to use for boot string
*
* This routine initializes the NVRAM on the Sandpoint BBSRAM.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysNvramHwInit 
    (
    void
    )
    {
    /* TO_DO */
    }

/*******************************************************************************
*
* sysNvRead - read one byte from NVRAM
*
* This routine reads a single byte from a specified offset in NVRAM.
*
* RETURNS: The byte from the specified NVRAM offset.
*
* ERRNO: N/A
*/

UCHAR sysNvRead
    (
    ULONG  offset   /* NVRAM offset to read the byte from */
    )
    {
    UCHAR  byte;

    byte = *(UCHAR *)(NV_RAM_ADRS + (NV_RAM_INTRVL * offset));
    return byte;
    }

/*******************************************************************************
*
* sysNvWrite - write one byte to NVRAM
*
* This routine writes a single byte to a specified offset in NVRAM. 
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysNvWrite
    (
    ULONG   offset, /* NVRAM offset to write the byte to */
    UCHAR   data    /* datum byte */
    )
    {
    *(UCHAR *)(NV_RAM_ADRS + (NV_RAM_INTRVL * offset)) = data;
    }

#endif /* INCLUDE_NVRAM */

#ifdef  INCLUDE_FLASH
/*******************************************************************************
*
* sysFlashWriteEnable - enable writing to flash memory
*
* This routine enables writing to flash on the Tsi108. 
*
* RETURNS: N/A 
*
* SEE ALSO: sysFlashWriteDisable()
*
* ERRNO: N/A
*/

void sysFlashWriteEnable (void)
    {
    ULONG tempLong;
    UINT  i;

    /* Enable flash Writes on Tsi108 */

    for (i = 0; i < 4; i++)
        {
        tempLong  = TSI108_REG_RD(BL_HLP, HLP_BX_CTRL0(i));
        tempLong |= HLP_BX_CTRL0_FWE;

        TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(i), tempLong);
        }
    }


/*******************************************************************************
*
* sysFlashWriteDisable - disable writing to flash memory
*
* This routine disables writing to flash on the Tsi108
*
* RETURNS: N/A 
*
* SEE ALSO: sysFlashWriteEnable()
*
* ERRNO: N/A
*/

void sysFlashWriteDisable (void)
    {
    ULONG tempLong;
    UINT  i;

    /* Delay to ensure any HLP activity is done before disabling writes */

    sysFlashDelay(1000000);
    WRS_ASM(" eieio; sync");

    /* Disable flash Writes on Tsi108 */

    for (i = 0; i < 4; i++)
        {
        tempLong  = TSI108_REG_RD(BL_HLP, HLP_BX_CTRL0(i));
        tempLong &= ~HLP_BX_CTRL0_FWE;

        TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(i), tempLong);
        }
    }


/*******************************************************************************
*
* sysFlashBoardDelay - delay for 1 uSecs
*
* This routine will delay for 1 uSec  by counting
* PPC decrementer ticks.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysFlashBoardDelay
    (
    void
    )
    {
    FAST UINT oldval;               /* decrementer value */
    FAST UINT newval;               /* decrementer value */
    FAST UINT totalDelta;           /* Dec. delta for entire delay period */
    FAST UINT decElapsed;           /* cumulative decrementer ticks */

    /*
     * Calculate delta of decrementer ticks for desired elapsed time.
     * The global variable sysPbClock MUST REFLECT THE PROPER 6xx BUS SPEED.
     */

    /* 1uSec */

    totalDelta = ((sysPbClock / 4) /1000000);

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

#ifdef INCLUDE_SYS_COPY_TO_FLASH
/******************************************************************************
*
* sysFlashGetSectorSize - determine the size of a sector of the flash device.
*
* RETURNS: An integer indicating the size of sector for the type of 
*          on-board flash memory.
*
* ERRNO: N/A
*/

UINT32 sysFlashGetSectorSize (void)
    {
    UINT32          retVal;    /* offset from base flash */
    static UINT8    flashType = FLASH_MEM_TYPE;

    if (flashType == 0)
        {
        flashType = sysFlashTypeGet ();
        }

    switch (flashType)
        {
        case (FLASH_28F128J3A):
#if ((2) == (FLASH_WIDTH))
            retVal = 0x10000;
#else 
            retVal = 0x20000;
#endif /* FLASH_WIDTH == 2 */
            break;

        case (FLASH_29LV040B):
            retVal = 0x10000;
            break;

        default:
            retVal = 0x10000;
            break;
        }

    return retVal;
    }

/*******************************************************************************
*
* sysFileCopyToFlash - overwrite the flash with data from a binary file.
*
* This code allows the user to overwrite the flash device with data from 
* a file.  It assumes flash support sysFlashWrite and other supporting 
* routines (from flashMem.c).  It will copy data from "filename" to the flash
* device.  It writes data to the flash device via sysFlashWrite(), and it will
* verify that the data was written correctly.  Writes to flash are wrapped
* within taskLock() and intLock().
*
* If Target Server File System is enabled and make sure the bootrom.bin could
* be accessed then:
*
*    sysCopyFileToFlash("bootrom.bin", 0x0)
*
* will copy the bootrom.bin image to FLASH_ADRS + 0.
*
* On success, this message will be printed.:
*   The flash has been updated without error.
* ->
*
* RETURNS: OK, or ERROR: if the file was not accessable, the
* file was too large to fit into the flash, the buffers cannot 
* be allocated, sysFlashWrite() or sysFlashGet() returned error, 
* the compare stage fails, or the flash device is inaccessable.
*
* ERRNO: N/A
*/

STATUS sysCopyFileToFlash
    (
    char *  filename,           /* file to read data from */
    UINT32  offset              /* offset from base flash */
    )

    {
    void *      pMemBuf;        /* holds the flash data */
    void *      pTestMemBuf;    /* used to compare data */
    int         fileFd;         /* the bootrom image fd */
    struct stat fioStatData;    /* used to get size */
    int         statReturn;     /* return values    */
    FAST int    lockKey;        /* for INT locking  */
    char        testR;
    int         sectorSize;
    int         flashSizeAvail;
    static UINT8 flashType = FLASH_MEM_TYPE;
    int         tmpSize;
    UINT32      addr;

    if (flashType == 0)
        {
        flashType = sysFlashTypeGet ();
        }

    sectorSize = sysFlashGetSectorSize();

    /* Ensure the flash is accessable */

    statReturn = vxMemProbe ((char *) FLASH_ADRS, VX_READ, 1, &testR);

    if (statReturn == ERROR)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("Could not access address 0x%x.\n", FLASH_ADRS);
        printErr ("The user should check the MMU mapping or "
                  "memory controller/bridge configuration.\n");
        return (ERROR);
        }

    /* check if the file can be opened. */
 
    fileFd = open (filename, O_RDONLY, 0666);

    if (fileFd == ERROR)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("The filename \"%s\" could not be opened.\n", filename);
        return (ERROR);
        }

    /* ensure the file is smaller than BSP's FLASH_SIZE */

    flashSizeAvail = FLASH_SIZE - offset;

    statReturn = ioctl (fileFd, FIOFSTATGET, (int) &fioStatData);

    if (statReturn == ERROR)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("FIOFSTATGET ioctl returned error.\n");
        close (fileFd);
        return (ERROR);
        }

    if (fioStatData.st_size > (ULONG) flashSizeAvail)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("The file \"%s\" is too large.\n", filename);
        printErr ("The file size is 0x%x.\n", fioStatData.st_size);
        printErr ("The size need to be below 0x%x.\n", flashSizeAvail);
        close (fileFd);
        return (ERROR);
        }

    /* now, copy the file to the target memory */

    pMemBuf = calloc (fioStatData.st_size+1, 1);

    if (pMemBuf == NULL)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("Could not allocate 0x%x bytes.\n", (long) flashSizeAvail);
        close (fileFd);
        return (ERROR);
        }

    printf("Reading file to memory pMemBuf = 0x%08x\n", (UINT32)pMemBuf);
    statReturn = read (fileFd, pMemBuf, flashSizeAvail);

    if (statReturn > fioStatData.st_size)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("The file size is 0x%x.\n", fioStatData.st_size);
        printErr ("However the \"read\" returned 0x%x.\n", statReturn);
        close (fileFd);
        cfree (pMemBuf);
        return (ERROR);
        }

    /* Erase sectors */

    for (tmpSize = fioStatData.st_size, addr = FLASH_ADRS + offset; 
         tmpSize > 0; 
         tmpSize -= sectorSize, addr += sectorSize)
        { 
        printf("Erasing 0x%08x's sector\n", addr); 
        if (sysSectorErase (FLASH_CAST (addr), flashType) == ERROR)
            {
            printErr ("sysSectorErase returns ERROR\n");
            close (fileFd);
            cfree (pMemBuf);
            return (ERROR);
            }
        }

    /* write the data to the flash device */
    
    printf ("Writing to flash address 0x%08x, size 0x%08lx\n",
            FLASH_ADRS+offset, fioStatData.st_size);
    sysFlashDelay(10000);
    EIEIO_SYNC;

    taskLock();         /* serialize this operation */
    lockKey = intLock();

    statReturn = sysFlashWrite ((FLASH_DEF *) (pMemBuf), fioStatData.st_size, 
                                offset, flashType, 0);
    
    if (statReturn == ERROR) 
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("sysFlashWrite (flashMem.c) - returned error.\n");
        intUnlock (lockKey);
        taskUnlock ();
        close (fileFd);
        cfree (pMemBuf);
        return (ERROR);
        }

    intUnlock (lockKey);
    taskUnlock ();      /* safe now */

    /* verify the write happened by comparing the data */

    pTestMemBuf = calloc (fioStatData.st_size+1,1);
 
    if (pTestMemBuf == NULL)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("Unable allocate 0x%x bytes.\n", (long)fioStatData.st_size);
        printErr ("Verify failed, but write suceeded.\n");
        close (fileFd);
        cfree (pMemBuf);
        return (ERROR);
        }

    statReturn = sysFlashGet (pTestMemBuf, fioStatData.st_size, offset);
 
    if (statReturn == ERROR)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("sysFlashGet (flashMem.c) - returned error.\n");
        close (fileFd);
        cfree (pMemBuf);
        cfree (pTestMemBuf);
        return (ERROR);
        }

    statReturn = bcmp (pMemBuf, pTestMemBuf, fioStatData.st_size);
 
    if (statReturn != 0)
        {
        printErr ("sysCopyFileToFlash: ");
        printErr ("Compare failed, flash does not match buffer.\n");
        close (fileFd);
        cfree (pMemBuf);
        cfree (pTestMemBuf);
        return (ERROR);
        }

    printf ("The flash has been updated without error.\n");

    close (fileFd);     /* Clean up */
    cfree (pMemBuf);
    cfree (pTestMemBuf);

    return (OK);
    }

/*******************************************************************************
*
* sysCopyMemToFlash - overwrite the flash with data from a memory location.
*
* This code allows the user to overwrite the flash device with data from 
* memory.  It assumes flash support sysFlashWrite and other supporting 
* routines (from flashMem.c).  It will copy data from <pMemSrc> of <srcSize>
* to the flash device.  It writes data to the flash device via sysFlashWrite(),
* and it will verify that the data was written correctly.  Writes to flash are
* wrapped within taskLock() and intLock().
*
* RETURNS: OK, or ERROR: the srcSize was too large to fit into the flash, 
* the buffers cannot be allocated, sysFlashWrite() or sysFlashGet() 
* returned error, the compare stage fails, or the flash device is 
* inaccessable.
*
* ERRNO: N/A
*/

STATUS sysCopyMemToFlash
    (
    UINT32 * pMemSrc,   /* start of location to be copied from */
    UINT32   srcSize,   /* size of memory to be copied */
    UINT32   offset     /* offset from base flash address */
    )

    {
    void *      pTestMemBuf;    /* used to compare data */
    int         statReturn;     /* return values    */
    FAST int    lockKey;        /* for INT locking  */
    char        testR;
    int         sectorSize;
    int         flashSizeAvail;
    static UINT8 flashType = FLASH_MEM_TYPE;
    int         tmpSize;
    UINT32      addr;

    if (flashType == 0)
        {
        flashType = sysFlashTypeGet ();
        }
    sectorSize = sysFlashGetSectorSize();

    /* Ensure the flash is accessable */

    statReturn = vxMemProbe ((char *) FLASH_ADRS, VX_READ, 1, &testR);

    if (statReturn == ERROR)
        {
        printErr ("sysCopyMemToFlash: ");
        printErr ("Could not access address 0x%x.\n", FLASH_ADRS);
        printErr ("The user should check the MMU mapping or "
                  "memory controller/bridge configuration.\n");
        return (ERROR);
        }

    /* ensure the src size is smaller than BSP's FLASH_SIZE */

    flashSizeAvail = FLASH_SIZE - offset;

    if (srcSize > (ULONG) flashSizeAvail)
        {
        printErr ("sysCopyMemToFlash: ");
        printErr ("The src at 0x%08x is too large.\n", pMemSrc);
        printErr ("The src size is 0x%x.\n", srcSize);
        printErr ("The size need to be below 0x%x.\n", flashSizeAvail);
        return (ERROR);
        }

    /* Erase sectors */

    for (tmpSize = srcSize, addr = FLASH_ADRS + offset; 
         tmpSize > 0; 
         tmpSize -= sectorSize, addr += sectorSize)
        { 
        if (sysSectorErase (FLASH_CAST (addr), flashType) == ERROR)
            return (ERROR);
        }

    printf ("Writing to flash address 0x%08x, size 0x%08x\n",
            FLASH_ADRS+offset, srcSize);
    sysFlashDelay(10000);
    EIEIO_SYNC;

    /* write the data to the flash device */

    taskLock();                     /* serialize this operation */
    lockKey = intLock(); 

    statReturn = sysFlashWrite ((FLASH_DEF *) (pMemSrc), srcSize, 
                                offset, flashType, 0);
    
    if (statReturn == ERROR) 
        {
        printErr ("sysCopyMemToFlash: ");
        printErr ("sysFlashWrite (flashMem.c) - returned error.\n");
        intUnlock (lockKey);
        taskUnlock ();      
        return (ERROR);
        }

    intUnlock (lockKey);
    taskUnlock ();      /* safe now */

    /* verify the write happened by comparing the data */

    pTestMemBuf = calloc (srcSize+1,1);
    if (pTestMemBuf == NULL)
        {
        printErr ("sysCopyMemToFlash: ");
        printErr ("Could not allocate 0x%x bytes.\n", (long) srcSize);
        printErr ("Verify failed, but write suceeded.\n");
        return (ERROR);
        }

    statReturn = sysFlashGet (pTestMemBuf, srcSize, offset);

    if (statReturn == ERROR)
        {
        printErr ("sysCopyMemToFlash: ");
        printErr ("sysFlashGet (flashMem.c) - returned error.\n");
        cfree (pTestMemBuf);
        return (ERROR);
        }

    statReturn = bcmp ((void *)pMemSrc, pTestMemBuf, srcSize);

    if (statReturn != 0)
        {
        printErr ("sysCopyMemToFlash: ");
        printErr ("Compare failed, flash does not match buffer.\n");
        cfree (pTestMemBuf);
        return (ERROR);
        }

    printf ("The flash has been updated without error.\n");

    cfree (pTestMemBuf);        /* Clean up */

    return (OK);
    }

#endif  /* INCLUDE_SYS_COPY_TO_FLASH */

#endif  /* INCLUDE_FLASH */

#ifdef INCLUDE_PCI     /* board level PCI routines */

/*******************************************************************************
*
* sysPciConfigRead - read from the PCI configuration space
*
* This routine reads either a byte, word or a long word specified by
* the argument <width>, from the PCI configuration space
*
* NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* ERRNO: N/A
*/

LOCAL STATUS sysPciConfigRead
    (
    int     busNo,      /* bus number */
    int     deviceNo,   /* device number */
    int     funcNo,     /* function number */
    int     offset,     /* offset into the configuration space */
    int     width,      /* width to be read */
    void *  pData       /* data read from the offset */
    )
    {
    UINT32  pciConfigAddr;
    FUNCPTR machCheckHandler;
    STATUS  result = OK;
    int     key;

    pciConfigAddr = (PCI_MSTR_CFG_LOCAL |
        pciConfigBdfPack (busNo, deviceNo, funcNo) | (offset & 0xfc));

    /*
     * Connect the PowerPC machine check exception to catch exceptions,
     * Save off the existing handler first.
     * NOTE:  The Tsi108 causes machine check exceptions to
     * occur when reads a non-present device address in the
     * PCI configuration space.
     */

    machCheckHandler = excVecGet ((FUNCPTR *) _EXC_OFF_MACH);
    key = intLock ();
    excVecSet ((FUNCPTR *) _EXC_OFF_MACH, (FUNCPTR)sysPciTrap);

    switch (width)
        {
        /* Note: *pData comes back as -1 if machine check happened */

        case 1: /* byte */
            if ((*(UINT8 *)pData = 
                PCI_IN_BYTE (pciConfigAddr | (offset & 0x3))) == 0xff)
            result = ERROR;
            break;

        case 2: /* word */
            if ((*(UINT16 *)pData = 
                PCI_IN_WORD (pciConfigAddr | (offset & 0x2))) == 0xffff)
            result = ERROR;
            break;

        case 4: /* long */
            if ((*(UINT32 *)pData = PCI_IN_LONG (pciConfigAddr)) == 0xffffffff)
                result = ERROR;
            break;

        default:
            result = ERROR;
            break;
        }

    /* Restore the PowerPC machine check exception */

    excVecSet ((FUNCPTR *) _EXC_OFF_MACH, (FUNCPTR)machCheckHandler);

    if (result == ERROR)
        {
        /* Clear error flags set in the PB and PCI block registers */

        sysClearPciError();
        }

    intUnlock (key);

    return (result);
    }


/*******************************************************************************
*
* sysPciConfigWrite - write to the PCI configuration space
*
* This routine writes either a byte, word or a long word specified by
* the argument <width>, to the PCI configuration space
* This routine works around a problem in the hardware which hangs
* PCI bus if device no 12 is accessed from the PCI configuration space.
*
* NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* ERRNO: N/A
*/

LOCAL STATUS sysPciConfigWrite
    (
    int     busNo,      /* bus number */
    int     deviceNo,   /* device number */
    int     funcNo,     /* function number */
    int     offset,     /* offset into the configuration space */
    int     width,      /* width to write */
    ULONG   data        /* data to write */
    )
    {
    UINT32 pciConfigAddr;

    pciConfigAddr = (PCI_MSTR_CFG_LOCAL |
                     pciConfigBdfPack (busNo, deviceNo, funcNo) |
                     (offset & 0xfc));

    switch (width)
        {
        case 1: /* byte */
            PCI_OUT_BYTE ((pciConfigAddr | (offset & 0x3)), (UINT8)data);
            break;

        case 2: /* word */
            PCI_OUT_WORD ((pciConfigAddr | (offset & 0x2)), (UINT16)data);
            break;

        case 4: /* long */
            PCI_OUT_LONG (pciConfigAddr, (UINT32)data);
            break;

        default:
            return (ERROR);
        }

    return (OK);
    }

/******************************************************************************
*
* sysPciTrap - trap handler for PCI exception
*
* This routine is called from the excConnectCode stub if the PCI configuration
* access generates an exception. By default, sysIn... returns the value read.
* This code changes the PC to sysPciErr which sets the return value to -1.
*  NOTE:  The Tsi108 PCI Configuration reads cause machine check exceptions to
*  occur when read from non-present device (Master Abort).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysPciTrap
    (
    ESFPPC *    pEsf            /* pointer to exception stack frame */
    )
    {
    REG_SET *pRegSet = &pEsf->regSet;
    pRegSet->pc = (_RType)sysPciErr;    /* sysPciErr forces a -1 return */
    }

#endif /* INCLUDE_PCI */

/***************************************************************************
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
    int channel         /* serial channel */
    )
    {
    return((SIO_CHAN *)ERROR);
    }

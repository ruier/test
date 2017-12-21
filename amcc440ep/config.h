/* config.h - AMCC 440EP configuration header */

/*
****************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

****************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2006-2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
*****************************************************************************
   COPYRIGHT   Applied Micro Circuits Corporation (AMCC) 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF AMCC
   All rights reserved
   AMCC Proprietary

****************************************************************************
\NOMANUAL
*/



/*
modification history
--------------------
01q,24may13,d_l  Update BSP_REV to "6.9/2"
01p,19jul12,clx  Update BSP_REV
01o,15apr10,clx  Update BSP version for vxWorks 6.9
01n,14dec09,x_s  Undefine INCLUDE_PCI & INCLUDE_FEI82557END.(WIND00194066)
01m,18feb09,x_z  Remove INCLUDE_PING.(WIND00137237) 
01l,10feb09,x_z  Modify the value of PM_RESERVED_MEM to the default value if 
                 INCLUDE_EDR_PM is not defined.(WIND00135575)
01k,04sep08,l_z  Remove RAM_LOW_ADRS and RAM_HIGH_ADRS
01j,21mar08,wap  Add VxBus support, switch to VxBus EMAC driver
01i,05jul07,l_z  update revision. 
01h,06dec06,y_w  Add an AUX_CLK_RATE macro, CQ:#61262,#46349
01g,31oct06,rec  vxWorks 6.5 update
01f,24apr06,mmi  Bump revision number
01e,15mar06,mmi  Mod MAX_MAC_ADRS to 2 
01e,28may06,jmt  Add SYS_MODEL definition
01d,15mar06,mmi  Mods to fix revision history from 2.2 to 2.1.1 
01c,15mar06,mmi  Mods to add M command 
01b,09mar06,mmi  Clean up of the file to adapt it to new board
01a,09mar06,mmi  Created based on bamboo config.h rev 01g
*/

/*
This file contains the configuration parameters for the AMCC 440EP
CPU evaluation board.
*/

#ifndef INCconfigh
#define INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/2"        /* 0 for first revision */

/* Base model string. sysModelGet adds processor details */

#define SYS_MODEL       "IBM PowerPC 440EP"

#include "configAll.h"

#define INCLUDE_VXBUS

#ifdef INCLUDE_VXBUS

#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE 50000
#define INCLUDE_PLB_BUS
#define INCLUDE_VXB_CMDLINE

#define INCLUDE_EMAC_VXB_END
#define INCLUDE_VXB_IBM_ZMII
#define INCLUDE_VXB_IBM_MAL
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

#endif /* INCLUDE_VXBUS */

/*
 * Default boot line
 */

#define DEFAULT_BOOT_LINE \
    "emac(0,0)host:vxWorks h=10.1.1.2 e=10.1.1.10 u=target"

/*
 * HW Debugger Support
 *
 * When CLEAR_DEBUG_REGISTERS is defined, the bootrom and kernel clear
 * the onchip debugging registers.  This is ideal for a delivered
 * application as it ensures that those chip resources are inert.
 *
 * When it is not defined, the bootrom and kernel do not initialize the
 * onchip debug control registers. This is the most appropriate setting
 * when using hardware assisted debugging.
 */

#undef CLEAR_DEBUG_REGISTERS

/*
 * Memory configuration.
 *
 * If LOCAL_MEM_AUTOSIZE is defined, SDRAM size is determined at boot
 * time, and overrides the default set in LOCAL_MEM_SIZE.
 */

#define LOCAL_MEM_AUTOSIZE                /* run-time memory sizing */
#define LOCAL_MEM_SIZE        0x02000000  /* 32MB memory default    */
#define LOCAL_MEM_LOCAL_ADRS  0x00000000  /* fixed at zero          */
#define USER_RESERVED_MEM     0           /* see sysMemTop()        */

/*
 * Define these for extra ED&R support
 *
#define INCLUDE_EDR_PM
#define INCLUDE_EDR_ERRLOG
#define INCLUDE_EDR_SHELL_CMD
#define INCLUDE_EDR_SHOW
*/

#ifdef INCLUDE_EDR_PM
#   undef  PM_RESERVED_MEM
#   define PM_RESERVED_MEM    0x10000  /* pmLib reserved area size */
#endif  /*INCLUDE_EDR_PM */
            

/*
 * 440EP Machine Check Handler configuration options.
 * See the 440EP User's Manual for more information.
 *
 * INCLUDE_440X5_DCACHE_RECOVERY
 *
 *  Makes data cache parity errors recoverable.  Selecting this
 *  option will also select INCLUDE_440X5_PARITY_RECOVERY, and
 *  set USER_D_CACHE_MODE to CACHE_WRITETHROUGH.
 *
 * INCLUDE_440X5_TLB_RECOVERY
 *
 *  Makes TLB parity errors recoverable.  Selecting this option will
 *  also select INCLUDE_440X5_PARITY_RECOVERY and INCLUDE_MMU_BASIC.
 *  (If the functionality delivered by the MMU library is not desired, it
 *  is permissible to #undef both USER_D_MMU_ENABLE and USER_I_MMU_ENABLE.)
 *
 * INCLUDE_440X5_PARITY_RECOVERY
 *
 *  Sets the PRE bit in CCR0.  This is required by the 440GX hardware
 *  if either TLB or DCACHE recovery is to be enabled.  Selecting this
 *  option will also select INCLUDE_EXC_HANDLING.
 *
 * INCLUDE_440X5_TLB_RECOVERY_MAX
 *
 *  Dedicates a TLB entry to the Machine Check handler, and a separate
 *  TLB entry to the remaining interrupt/exception vectors, to maximize
 *  recoverability from TLB parity errors.  Selecting this option will
 *  also select INCLUDE_440X5_TLB_RECOVERY.  This option does not change
 *  the physical location of the vectors, however:
 *
 *  * VEC_BASE_ADRS, which defines the (virtual) address at which the
 *    vector code will execute, must be set to a value higher than the
 *    physical memory size and different than VEC_BASE_TASK_ADRS.
 *
 *  * This option increases the number of static TLB entries by two,
 *    correspondingly reducing the number of dynamic entries available.
 *    The required TLB entries must be defined in sysStaticTlbDesc[]
 *    and coordinated with the value of VEC_BASE_ADRS.
 *
 * INCLUDE_440X5_MCH_LOGGER
 *
 *  Causes the Machine Check handler to log recovered events.
 *
 *   _PPC_440X5_MCH_LOG_RTN
 *
 *  Log function to call, or (FUNCPTR)1 to use logMsg().
 *
 *  The log function executes in an environment similar to that of a
 *  watchdog or an interrupt service routine.  It must not attempt to
 *  delay or suspend itself, and it should perform only the minimum
 *  necessary operations before returning.  It may write to a message
 *  queue provided the timeout parameter is specified as NO_WAIT, or
 *  it may give (but not take) a semaphore.
 *
 *  NOTE: The (FUNCPTR)1 convention is needed because of initialization
 *  sequencing.  Setting this parameter directly to logMsg() may result
 *  in logMsg() being called before it has been properly initialized.
 *
 * While there are performance costs associated with setting CCR0[PRE]
 * and with operating the data cache in writethrough mode, the default and
 * WRS-recommended configuration is to enable all 4 recovery options.
 */

#define INCLUDE_440X5_DCACHE_RECOVERY
#define INCLUDE_440X5_TLB_RECOVERY
#define INCLUDE_440X5_PARITY_RECOVERY
#define INCLUDE_440X5_TLB_RECOVERY_MAX
#define INCLUDE_440X5_MCH_LOGGER
#define _PPC_440X5_MCH_LOG_RTN  (FUNCPTR)1  /* use logMsg when available */

/*
 * Base address for accessing the exception/interrupt vector space from
 * code executing outside of it, typically zero.  VEC_BASE_ADRS will
 * normally be the same except when using INCLUDE_440X5_TLB_RECOVERY_MAX.
 */
#define VEC_BASE_ALT_ADRS 0x00000000

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS 0xcfff0000
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE are defined in config.h and Makefile.
 *
 * All definitions for these constants must be identical.
 *
 * Boot ROM address space.
 */

#define ROM_BASE_ADRS       0xfff00000             /* base address of ROM  */
#define ROM_TEXT_ADRS       ROM_BASE_ADRS          /* with PC & SP         */
#define ROM_WARM_ADRS       (ROM_TEXT_ADRS+0x0004) /* warm reboot entry    */
#define ROM_SIZE            0x00100000             /* 1MB                */
#define USER_RESERVED_MEM   0                      /* see sysMemTop()      */

#ifndef RAM_DST_ADRS         /* default uncompress dest. */
  #define RAM_DST_ADRS         RAM_HIGH_ADRS
#endif  /* RAM_DST_ADRS */

/*
 * Cache options
 *
 * Note that if both I and D MMUs are 'enabled', 440EP applies the
 * same cache mode to all accesses at a given virtual address -- there
 * is no way to selectively enable or disable the instruction cache
 * into a state different from the data cache.
 *
 * You can, however, leave the I MMU 'disabled' and D MMU 'enabled'.
 * In this case, you can manage the D MMU cache mode via the
 * virtual memory library, and manage the I MMU cache mode via
 * the sysStaticTlbDesc[] structure (see sysLib.c).
 */

#define INCLUDE_CACHE_SUPPORT

#define USER_D_CACHE_ENABLE
#define USER_I_CACHE_ENABLE
#undef USER_I_CACHE_MODE
#undef USER_D_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_WRITETHROUGH)
#define USER_D_CACHE_MODE (CACHE_COPYBACK)


/*
 * define this if you want the TLB set up so that boot space from
 * 0xf0000000 to 0xffffffff is writeable. This can be useful in
 * writing to the boot SRAM device with a JTAG debugger, or if
 * using the 256MB flash for TFFS.
 */
#define BOOT_SPACE_WRITEABLE


/*
 * timers (DEC, FIT, WDT) can be driven from an external clock
 * source, or at the same frequency as the CPU.
 */

#undef  TIMER_CLOCK_EXTERNAL           /* define to use external 25 MHz clock */


/*
 * The clock used to drive the UARTs on the chip can be derived from
 * one of two different sources. The 11.0592 MHz external oscillator on
 * the board can be used, or the internal PLB clock can be used with an additional
 * internal UART divisor (see the CPC0_CR0 register). If you want to use
 * the external UART clock oscillator, define UART_CLOCK_EXTERNAL.
 */

#define UART_CLOCK_EXTERNAL   /* define to use external 11.0592 MHz clock */


/*
 * Optional timestamp support
 */

#undef  INCLUDE_TIMESTAMP

/*
 * Auxiliary Timer rates
 */
#define AUX_CLK_RATE         (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_2)) 
                                                     /* apprx   253 Hz */
#define AUX_CLK_RATE_MIN     (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_3))
                                                     /* apprx    15 Hz */
#define AUX_CLK_RATE_MAX     (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_0))
                                                     /* apprx 64941 Hz */
/*
 * Watchdog Timer rates
 * NOTE: MIN of sysTimerClockFreq / (1<<33) not supported.
 */

#define WDT_RATE_MIN         (sysTimerClkFreq / (1 << WDT_DIVIDER_TAP_2))
#define WDT_RATE_MAX         (sysTimerClkFreq / (1 << WDT_DIVIDER_TAP_0))


/* Real Time Clock configuration (Dallas Semiconductor DS1743P) */

#define INCLUDE_RTC


/* Remove unused network drivers */

#undef  INCLUDE_EI
#undef  INCLUDE_EX
#undef  INCLUDE_ENP
#undef  INCLUDE_LN
#undef  INCLUDE_SM_NET
#undef  INCLUDE_SM_SEQ_ADDR


/* #define for floating point support */

#undef INCLUDE_SW_FP
#define INCLUDE_HW_FP


/* Serial port configuration */

#define  INCLUDE_SIO

/* PCI configuration */

#undef INCLUDE_PCI
#undef PCI_AUTO_DEBUG

#ifdef INCLUDE_PCI

#    define INCLUDE_PCI_AUTOCONF


/*   32 bit PCI bus memory map. 440EP
 *
 *    size      CPU (virt)   PLB (real)       PCI           usage            map by
 *
 * 0x40000000   0x00000000  0x0.00000000  0x00000000   440EP slave window     PTM1
 * 0x02000000   0x80000000  0x0.A0000000  0x80000000   PCI Mem w/o prefetch   PMM1
 * 0x02000000   0x90000000  0x0.B0000000  0x90000000   PCI Mem with prefetch  PMM0
 * 0x00010000   0xC1000000  0x0.E8000000  0x00000000   I/O                    fixed
 * 0x03800000   0xC2800000  0x0.E8800000  0x00800000   "Extra" I/O            fixed
 */

/*
 * Define a 1GB slave window that allows PCI masters to access local memory
 * of the 440EP.  This window will be setup with the PTM1 registers in the
 * bridge.
 */

#    define PCI_SLV_MEM_LOCAL      LOCAL_MEM_LOCAL_ADRS
#    define PCI_SLV_MEM_BUS        LOCAL_MEM_LOCAL_ADRS
#    define PCI_SLV_MEM_SIZE       0x40000000           /* 1GB */

#    define PTM1_LOCAL_ADRS        PCI_SLV_MEM_LOCAL
#    define PTM1_SIZE_ATTRIB       PTM_SIZE_1GB | PTM_ENABLE

/*
 * Define a 32MB master window that allows the CPU to access PCI Memory
 * (with prefetch).  This window will be setup with the PMM0 registers in the
 * bridge.The guarded bit should not be set in the TLB entries
 * covering this area.  The processor address of this region should reside
 * between PCI_MEMORY_START and PCI_MEMORY_END.
 */

#    define PCI_MSTR_MEM_LOCAL     PCIM0B_VIRT_START   /* Processor logical address */
#    define PCI_MSTR_MEM_BUS       PCIM0B_VIRT_START    /* PCI address       */
#    define PCI_MSTR_MEM_SIZE      0x02000000         /* 32MB */

#    define PMM0_LOCAL_ADRS        PCI_MEMORY2_PHY_START_LOW /* PLB address */
#    define PMM0_PCI_LOW_ADRS      PCI_MSTR_MEM_BUS
#    define PMM0_PCI_HIGH_ADRS     0x00000000
#    define PMM0_PCI_MASK_ATTRIB   (PMM_MASK_32MB | PMM_PREFETCH | PMM_ENABLE)

/*
 * Define a 32MB master window that allows the CPU to access PCI Memory
 * (non-prefetch). This window will be setup with the PMM1 registers in
 * the bridge.The guarded bit should not be set in the TLB entries
 * covering this area.  The processor address of this region should reside
 * between PCI_MEMORY_START and PCI_MEMORY_END.
 */

#    define PCI_MSTR_MEMIO_LOCAL   PCIM0A_VIRT_START        /* Processor logical address */
#    define PCI_MSTR_MEMIO_BUS     PCIM0A_VIRT_START        /* PCI address       */
#    define PCI_MSTR_MEMIO_SIZE    0x02000000         /* 32MB */

#    define PMM1_LOCAL_ADRS        PCI_MEMORY_PHY_START_LOW
#    define PMM1_PCI_LOW_ADRS      PCI_MSTR_MEMIO_BUS
#    define PMM1_PCI_HIGH_ADRS     0x00000000
#    define PMM1_PCI_MASK_ATTRIB   (PMM_MASK_32MB | PMM_ENABLE)

/*
 * Master window that allows CPU to access PCI I/O space.
 */

#    define PCI_MSTR_IO_LOCAL      PLB_PCI_IO_REGION_1_START_LOW
#    define PCI_MSTR_IO_BUS        0x00000000
#    define PCI_MSTR_IO_SIZE       PLB_PCI_IO_REGION_1_SIZE

/*
 * The third PCI master memory window is not being used
 */

#    define PMM2_LOCAL_ADRS       0x00000000
#    define PMM2_PCI_LOW_ADRS     0x00000000
#    define PMM2_PCI_HIGH_ADRS    0x00000000
#    define PMM2_PCI_MASK_ATTRIB  PMM_UNUSED

/*
 * The second PCI slave window in the bridge are not being used.
 */

#    define PTM2_LOCAL_ADRS       PTM_UNUSED
#    define PTM2_SIZE_ATTRIB      PTM_UNUSED

/* PCI Latency Timer value */
#    define PCI_LAT_TIMER          0


#endif /* INCLUDE_PCI */

/* Define the constant hereunder only if PPC440EP CHIP_3 ERRATA workaround
 * (hardware patch) is used. Warning : do not use this flag if the
 *  corresponding patch is not installed: risk of processor hang
 */

#undef PPC440EP_CHIP_3_ERRATA_HARD_PATCH

/* enable 'M' command */
#define ETHERNET_MAC_HANDLER
#define MAX_MAC_DEVS 1 /* one network device (emac) */
#ifdef BOOTAPP
#define INCLUDE_BOOT_ETH_MAC_HANDLER
#endif



/*
 * If network support is included, either the native EMAC or the PCI
 * Ethernet should be chosen. Both are END style drivers.
 */

#define INCLUDE_NETWORK
#undef  INCLUDE_LN97XEND          /* Network support using AMD PCI card */
#undef  INCLUDE_FEI82557END       /* include fei driver */
#define MAX_MAC_ADRS        2
#define ENET_MAC_DEBUG        /* use this to debug sysNetMacxx */ 

/*
 * Define to support both EMAC units
 */

#define USE_BOTH_EMACS


#ifdef  INCLUDE_NETWORK
#  define INCLUDE_END
#endif /* INCLUDE_NETWORK */


/*
 * WDB communication
 */

#define INCLUDE_WDB

#ifdef  INCLUDE_NETWORK
#  undef  WDB_COMM_TYPE
#  define WDB_COMM_TYPE   WDB_COMM_END
#  ifdef  INCLUDE_EMAC_NETWORK
#    define  INCLUDE_ZMII       /* 440 EMAC requires ZMII support */
#  endif  /* INCLUDE_EMAC_NETWORK */
#elif  defined(INCLUDE_SIO)
#  undef  WDB_COMM_TYPE
#  undef  WDB_TTY_BAUD
#  undef  WDB_TTY_ECHO
#  define WDB_COMM_TYPE   WDB_COMM_SERIAL
#  define WDB_TTY_BAUD    38400
#  define WDB_TTY_ECHO    TRUE
#endif  /* INCLUDE_NETWORK || INCLUDE_SIO */

/*
 * Base address for exception vector handlers
 */

#define VEC_BASE  (char *)0


/*
 * MMU support.
 *
 * On PowerPC Book E processors, MMU translation cannot be deactivated.
 * The VxWorks bootrom establishes 24 or 26 TLB entries mapping chunks
 * (see bootEntry in romInit.s). These entries statically map the entire
 * effective address space to physical addresses, allowing the boot
 * process to load a kernel.
 *
 * This process is repeated for the vxWorks kernel through the
 * sysStaticTlbDesc[] array (see sysLib.c), and for mappings beyond the
 * reach of vmLib (outside the lowest 4GB of physical address space).
 *
 * Finer-grained storage attributes (caching, guarded,
 * execute/read/write) and mapping (4KB chunks) are managed by the vmLib
 * and vmBaseLib API.  These MMU libraries accomplish this through use
 * and reuse of the remaining TLB entries.  In VxWorks 5.5, only devices
 * with 32-bit physical addresses can be mapped.
 *
 * Typically, setting the D-MMU as enabled and I-MMU as disabled gives
 * the best performance, as this reduces miss-replacement competition for
 * the 440's 64 TLB entries.
 *
 * For more details, see sysPhysMemDesc[] in sysLib.c.
 */

#define INCLUDE_MMU_BASIC
#define USER_D_MMU_ENABLE
#undef  USER_I_MMU_ENABLE


/* TFFS */
#undef  INCLUDE_TFFS

/*
 * Define this only if the board was built with Intel flash devices.
 * (Default is AMD.)
 */
#undef  INCLUDE_TFFS_INTEL2MB

#ifdef  INCLUDE_TFFS
#define INCLUDE_USR_FS_UTILS
#define INCLUDE_TFFS_SHOW
#endif  /* INCLUDE_TFFS */


/* Include the other configuration values */

/* 440EP and its associated evaluation board could achieve different configurations
   depending on chip pin sharing and board wiring.
   The table below describes all wiring configuration supported by the current BSP :

   CONFIG1 : No USB2 HOST,
             No USB2 Device,
             Ethernet packet reject,
             2 Ethernet ports,
             USB 1.1 Device,
             USB 1.1 Host,
             No Nand Flash,
             No SPI interface,
             No IIC1 interface,
             1 UART 8 pins (UART0)

   CONFIG2 : Same as config1 except :
             2 UARTs 4 pins (UART0,UART1)
*/


/* Select desired configuration */
/* NOTE: Project overwrite the NUM_TTY define so take care to set it in accordance */
#define SEL_CONFIG1

#undef NUM_TTY

#ifdef SEL_CONFIG1
  #define USE_BOTH_EMACS
  #define NUM_TTY 1
#elif defined SEL_CONFIG2
  #define USE_BOTH_EMACS
  #define NUM_TTY 2
#endif

#include "ppc440EP.h"        /* chip header file       */
#include "amcc440ep.h"         /* board header file      */

#ifdef __cplusplus
    }
#endif

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

